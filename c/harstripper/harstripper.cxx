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

int main()
{
  // json j =
  //     {
  //         {"pi", 3.141},
  //         {"happy", true},
  //         {"name", "Niels"},
  //         {"nothing", nullptr},
  //         {"answer", {{"everything", 42}}},
  //         {"list", {1, 0, 2}},
  //         {"object", {{"currency", "USD"}, {"value", 42.99}}}};
  // j["new"]["key"]["value"] = {"another", "list"};

  ifstream arquivo_har("C:\\mimi\\testes\\app.deriv.com.har");
  json json_har;
  arquivo_har >> json_har;
  arquivo_har.close();

  json json_wss;

  analisar_nlohmann(json_har, "HAR", [&json_wss](json &conteudo, string chave) -> size_t
                    {
                      if (!chave.compare("_webSocketMessages"))
                      {
                        // cout << string(indentacao_nlohmann, '\t') << chave << ": " << conteudo.type_name() << '\n';
                        cout << "Chave " << chave << " encontrada, tipo: " << conteudo.type_name() << endl;
                        json_wss = conteudo;
                      }
                      return 1;
                    });

  ofstream arquivo_json_wss("C:\\mimi\\testes\\trafego_websockets_deriv.json");
  arquivo_json_wss << json_wss;
  arquivo_json_wss.close();

  // analisar_nlohmann(json_wss, "HAR", [](json &conteudo, string chave) -> size_t { cout << string(indentacao_nlohmann, '\t') << chave << ": " << conteudo.type_name() << '\n'; return 1; });
  // for(auto iterador = json_wss.begin(); iterador != json_wss.end(); ++iterador) {
  unsigned long contador_de_mensagens = 0;
  ofstream arquivo_analise_json_har("C:\\mimi\\testes\\analise_deriv.txt");
  for (auto mensagem : json_wss)
  {
    string dados = mensagem["data"];
    stringstream analise_da_mensagem;
    analise_da_mensagem
        << setw(12) << left << "MSG N:" << contador_de_mensagens++ << endl
        // << setw(12) << left << "Opcode:" << mensagem["opcode"] << endl
        << setw(12) << left << "Tipo:" << mensagem["type"] << endl
        // << setw(12) << left << "Hora:" << mensagem["time"] << endl
        << setw(12) << left << "Dados:" << mensagem["data"].type_name() << ", tamanho " << dados.length() << endl
        << dados.substr(0, 119) << endl
        << endl;
    arquivo_analise_json_har << analise_da_mensagem.str();
    cout << analise_da_mensagem.str();
  }
  arquivo_analise_json_har.close();
}

// detail::iteration_proxy<detail::iter_impl<json>>
// detail::iteration_proxy_value<detail::iter_impl<json>>
