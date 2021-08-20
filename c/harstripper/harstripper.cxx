#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

// using json = nlohmann::json;

unsigned long indentacao_nlohmann = 0;

size_t analisar_nlohmann(nlohmann::json &conteudo, std::string chave = std::string(),
                         std::function<size_t(nlohmann::json &, std::string)> predicado =
                             std::function<size_t(nlohmann::json &, std::string)>())
{
  size_t acumulador = 0;

  if (predicado)
    acumulador += predicado(conteudo, chave);

  bool e_objeto = false;
  switch (conteudo.type())
  {
  case nlohmann::json::value_t::object:
    e_objeto = true;
  case nlohmann::json::value_t::array:
  {
    unsigned long contador_de_array = 0;
    for (auto iterador = conteudo.begin(); iterador != conteudo.end(); ++iterador)
    {
      std::string chave_do_iterador;
      ++indentacao_nlohmann;
      if (e_objeto)
        chave_do_iterador = iterador.key();
      else
        chave_do_iterador = std::to_string(contador_de_array++);
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
  // std::ifstream arquivo_har("C:\\mimi\\testes\\app.deriv.com.har");
  // create a JSON object
  nlohmann::json j =
      {
          {"pi", 3.141},
          {"happy", true},
          {"name", "Niels"},
          {"nothing", nullptr},
          {"answer", {{"everything", 42}}},
          {"list", {1, 0, 2}},
          {"object", {{"currency", "USD"}, {"value", 42.99}}}};
  j["new"]["key"]["value"] = {"another", "list"};

  // arquivo_har >> j;

  // // count elements
  // auto s = j.size();
  // j["size"] = s;

  // pretty print with indent of 4 spaces
  // std::cout << std::setw(4) << j << '\n';

  // dumper_nlohmann(j, [](nlohmann::json &j) -> size_t
  //                      {
  //                        std::cout << j.size() << '\n';
  //                        for (auto &n : j.items())
  //                        {
  //                          std::cout << n.key() << '\n';
  //                        }
  //                        return j.size();
  //                      });
  analisar_nlohmann(j, "HAR", [](nlohmann::json &j, std::string s) -> size_t
                    {
                      // if (!s.compare("\"_webSocketMessages\""))
                      // {
                      std::cout << std::string(indentacao_nlohmann, '\t') << s << ": " << j.type_name() << '\n';
                      // }
                      return 1;
                    });
  // nlohmann::detail::iteration_proxy<nlohmann::detail::iter_impl<nlohmann::json>>
  // nlohmann::detail::iteration_proxy_value<nlohmann::detail::iter_impl<nlohmann::json>>
}
