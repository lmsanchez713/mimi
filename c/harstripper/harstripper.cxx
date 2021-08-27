#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

unsigned long indentacao_nlohmann = 0;

size_t analisar_nlohmann(json &conteudo, string chave = string(),
                         function<size_t(json &, string)> predicado =
                             function<size_t(json &, string)>())
{
  size_t acumulador = 0;

  if (predicado)
    acumulador += predicado(conteudo, chave);

  bool e_objeto = false;
  switch (conteudo.type())
  {
  case json::value_t::object:
    e_objeto = true;
  case json::value_t::array:
  {
    unsigned long contador_de_array = 0;
    for (auto iterador = conteudo.begin(); iterador != conteudo.end(); ++iterador)
    {
      string chave_do_iterador;
      ++indentacao_nlohmann;
      if (e_objeto)
        chave_do_iterador = iterador.key();
      else
        chave_do_iterador = to_string(contador_de_array++);
      // if (predicado) acumulador += predicado(iterador.value(), chave_do_iterador);
      analisar_nlohmann(iterador.value(), chave_do_iterador, predicado);
      --indentacao_nlohmann;
    }
  }
  break;
  default:
    // if (predicado) acumulador += predicado(conteudo, chave);
    break;
  }
  return acumulador;
}

string remover_barras_invertidas_de_escape(string &texto_a_processar)
{
  string texto_a_retornar;
  for (unsigned long posicao_no_texto = 0; posicao_no_texto < texto_a_processar.length();)
  {
    if (texto_a_processar[posicao_no_texto] == '\\')
    {
      ++posicao_no_texto;
    }
    texto_a_retornar.push_back(texto_a_processar[posicao_no_texto++]);
  }
  return texto_a_retornar;
}

int main()
{
  cout << endl
       << "Mimi HarStripper v0.1b" << endl
       << endl;
  ifstream arquivo_har("C:\\mimi\\testes\\app.deriv.com.har");
  json json_har;
  arquivo_har >> json_har;
  arquivo_har.close();

  json json_wss;

  analisar_nlohmann(json_har, "HAR", [&json_wss](json &conteudo, string chave) -> size_t
                    {
                      if (!chave.compare("_webSocketMessages"))
                      {
                        cout << "Chave " << chave << " encontrada, tipo: " << conteudo.type_name() << endl
                             << endl;
                        json_wss = conteudo;
                      }
                      return 1;
                    });

  ofstream arquivo_json_wss("C:\\mimi\\testes\\trafego_websockets_deriv.json");
  arquivo_json_wss << json_wss;
  arquivo_json_wss.close();

  // analisar_nlohmann(json_wss, "HAR", [](json &conteudo, string chave) -> size_t { cout << string(indentacao_nlohmann, '\t') << chave << ": " << conteudo.type_name() << '\n'; return 1; });
  unsigned long contador_de_mensagens = 0;
  ofstream arquivo_analise_json_har("C:\\mimi\\testes\\analise_deriv.txt");
  size_t qntd_digitos_n_msg = to_string(json_wss.size()).length();

  for (auto mensagem : json_wss)
  {
    string dados = mensagem["data"];
    stringstream analise_da_mensagem;

    analise_da_mensagem
        << "MSG " << setw(qntd_digitos_n_msg) << right << contador_de_mensagens++
        << " - " << (!string(mensagem["type"]).compare("send") ? "ENVIADA" : "RECEBIDA") << endl;

    string texto_mensagem = mensagem["data"];

    json json_mensagem = json::parse(texto_mensagem);

    analisar_nlohmann(json_mensagem, "Chaves", [&analise_da_mensagem](json &conteudo, string chave) -> size_t
                      {
                        unsigned long threshold_nlohmann = 4;
                        if (indentacao_nlohmann <= threshold_nlohmann)
                        {
                          // analise_da_mensagem
                          //     << ' ' << chave;
                          analise_da_mensagem
                              << string(indentacao_nlohmann, '\t') << chave << ": ";
                          if ((conteudo.type() == json::value_t::object) || (conteudo.type() == json::value_t::array))
                          {
                            if (indentacao_nlohmann >= threshold_nlohmann)
                            {
                              analise_da_mensagem
                                  << conteudo.type_name();
                            }
                          }
                          else
                          {
                            analise_da_mensagem
                                << conteudo;
                          }
                          analise_da_mensagem
                              << endl;
                        }
                        // else if (indentacao_nlohmann == 0)
                        // {
                        //   analise_da_mensagem
                        //       << endl << string(indentacao_nlohmann, '\t') << chave << ':';
                        // }
                        return 1;
                      });

    analise_da_mensagem << endl;

    arquivo_analise_json_har << analise_da_mensagem.str();
    cout << analise_da_mensagem.str();
  }
  arquivo_analise_json_har.close();
}
