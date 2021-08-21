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
      // if (predicado)
      //   acumulador += predicado(iterador.value(), chave_do_iterador);
      analisar_nlohmann(iterador.value(), chave_do_iterador, predicado);
      --indentacao_nlohmann;
    }
  }
  break;
  default:
    // if (predicado)
    //       acumulador += predicado(conteudo, chave);
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
        << ", " << mensagem["type"] << endl;
    // << setw(12) << left << "Opcode:" << mensagem["opcode"] << endl
    // << setw(12) << left << "Tipo:" << mensagem["type"] << endl;
    // << setw(12) << left << "Hora:" << mensagem["time"] << endl
    // << setw(12) << left << "Dados:" << mensagem["data"].type_name() << ", tamanho " << dados.length() << endl
    // << dados.substr(0, 119) << endl

    // analise_da_mensagem
    //     << setw(12) << left << "msgdata_t:" << mensagem["data"].type_name() << endl;

    string texto_mensagem = mensagem["data"];
    // for(unsigned long cc = 0; cc < 16; ++cc) {
    //   analise_da_mensagem << setw(2) << right << cc << ": " << texto_mensagem[cc] << endl;
    // }
    // texto_mensagem = remover_barras_invertidas_de_escape(texto_mensagem);
    json json_mensagem = json::parse(texto_mensagem);
    // analise_da_mensagem
        // << setw(12) << left << "Tipo dados:" << json_mensagem.type_name() << endl
        // << json_mensagem << endl;
        // << setw(12) << left << "Campos:" << json_mensagem.size() << endl;

    analisar_nlohmann(json_mensagem, "MSG", [&analise_da_mensagem](json &conteudo, string chave) -> size_t
                      {
                        if (indentacao_nlohmann <= 2)
                        {
                          analise_da_mensagem
                              << string(indentacao_nlohmann, '\t') << chave << ": ";
                          if ((conteudo.type() == json::value_t::object) || (conteudo.type() == json::value_t::array))
                            analise_da_mensagem
                                << conteudo.type_name();
                          else
                            analise_da_mensagem
                                << conteudo;
                          analise_da_mensagem
                              << endl;
                        }
                        return 1;
                      });

    analise_da_mensagem << endl;

    arquivo_analise_json_har << analise_da_mensagem.str();
    cout << analise_da_mensagem.str();
  }
  arquivo_analise_json_har.close();
}

// detail::iteration_proxy<detail::iter_impl<json>>
// detail::iteration_proxy_value<detail::iter_impl<json>>
