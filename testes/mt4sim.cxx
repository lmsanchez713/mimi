// tempo,ativo,acao,expiracao
// 1626890791,CADCHF,call,5
// 1626891003,CADCHF,call,5
// 1626891345,CADCHF,call,5
// 1626891604,CADCHF,put,5
// 1626891920,CADCHF,put,5

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <thread>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{

  if (!fs::exists("C:\\mimi\\testes\\csv\\sinais.csv"))
  {

    fstream arquivo("C:\\mimi\\testes\\csv\\sinais.csv", ios_base::out);
    if (arquivo.good())
    {

      arquivo << "tempo,ativo,acao,expiracao";
      arquivo.close();

    }

  }

  #define n_par 3
  #define n_acao 2
  #define n_n 9
  
  string
    par[n_par] = { "CADCHF", "USDBRL", "USDETH" },
    acao[n_acao] = { "call", "put" },
    algarismos[n_n] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

  while (1)
  {

    unsigned int
      i_par = rand() % n_par,
      i_acao = rand() % n_acao,
      i_n = rand() % n_n;

    stringstream valor;
    valor << "162689";
    valor << (rand() > (RAND_MAX / 2) ? '1' : '0');
    valor << (rand() % 10) << (rand() % 10) << (rand() % 10);

    //cout << valor.str() << ',' << par[i_par] << ',' << acao[i_acao] << ',' << algarismos[i_n] << '\n';

    fstream arquivo("C:\\mimi\\testes\\csv\\sinais.csv", ios_base::app);
    if (arquivo.good())
    {

      arquivo << '\n' << valor.str() << ',' << par[i_par] << ',' << acao[i_acao] << ',' << algarismos[i_n];
      arquivo.close();

    }

    this_thread::sleep_for(1000ms);

  }

  return 0;
}