#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

// using json = nlohmann::json;

unsigned long nlohmann_json_dumper_indentation = 0;

size_t nlohmann_json_dumper(nlohmann::json &j, std::string k = std::string(),
                            std::function<size_t(nlohmann::json &, std::string)> f =
                                std::function<size_t(nlohmann::json &, std::string)>())
{
  size_t c = 0;

  //
  // std::cout << std::string(nlohmann_json_dumper_indentation, '\t');
  // if (!k.empty())
  //   std::cout << k << ": ";
  // std::cout << "(" << j.type_name() << ")";
  //

  bool is_object = false;
  switch (j.type())
  {
  case nlohmann::json::value_t::object:
    is_object = true;
  case nlohmann::json::value_t::array:
  {
    unsigned long counter = 0;
    // std::cout << std::endl;
    for (auto it = j.begin(); it != j.end(); ++it)
    {
      std::string key;
      ++nlohmann_json_dumper_indentation;
      if (is_object)
        key = "\"" + it.key() + "\"";
      else
        key = std::to_string(counter++);
      if (f)
        c += f(it.value(), key);
      nlohmann_json_dumper(it.value(), key, f);
      --nlohmann_json_dumper_indentation;
    }
  }
  break;
  default:
    // std::cout << ' ' << j;
    // std::cout << std::endl;
    break;
  }
  return c;
}

int main()
{
  std::ifstream arquivo_har("C:\\mimi\\testes\\app.deriv.com.har");
  // create a JSON object
  nlohmann::json j; // =
                    // {
                    //     {"pi", 3.141},
                    //     {"happy", true},
                    //     {"name", "Niels"},
                    //     {"nothing", nullptr},
                    //     {"answer", {{"everything", 42}}},
                    //     {"list", {1, 0, 2}},
                    //     {"object", {{"currency", "USD"}, {"value", 42.99}}}};

  // // add new values
  // j["new"]["key"]["value"] = {"another", "list"};

  arquivo_har >> j;

  // // count elements
  // auto s = j.size();
  // j["size"] = s;

  // pretty print with indent of 4 spaces
  // std::cout << std::setw(4) << j << '\n';

  // nlohmann_json_dumper(j, [](nlohmann::json &j) -> size_t
  //                      {
  //                        std::cout << j.size() << '\n';
  //                        for (auto &n : j.items())
  //                        {
  //                          std::cout << n.key() << '\n';
  //                        }
  //                        return j.size();
  //                      });
  nlohmann_json_dumper(j, "HAR", [](nlohmann::json &j, std::string s) -> size_t
                       {
                         if (!s.compare("\"_webSocketMessages\"")) {
                           std::cout << std::string(nlohmann_json_dumper_indentation, ' ') << s << ": " << j.type_name() << '\n';
                         }
                         return 1;
                       });
  // nlohmann::detail::iteration_proxy<nlohmann::detail::iter_impl<nlohmann::json>>
  // nlohmann::detail::iteration_proxy_value<nlohmann::detail::iter_impl<nlohmann::json>>
}
