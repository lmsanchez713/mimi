import sys
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import pathlib
from os import listdir
from os.path import isfile, join
import os
import shutil
import asyncio
import websockets
import json
# import io

token_deriv = "HVumMdEbE5TdVUP"#certo
# token_deriv = "HVumMdEbE5TdVUPU"#errado
# token_deriv_mimes = "aFGutHpwxtAT3If"
uri_deriv = "wss://ws.binaryws.com/websockets/v3?app_id=1089"
auth_deriv = False

async def enviar_deriv(soquete, requisicao):
    await soquete.send(requisicao)
    resposta = await soquete.recv()
    return resposta

async def autenticar_deriv(soquete, token):
    requisicao = "{\"authorize\":\"" + token + "\"}"
    resposta = await enviar_deriv(soquete, requisicao)
    return resposta

def processar_sinal_retorno_v2(sinal):
    colunas = sinal.split(",")
    if len(colunas) == 6:
        print("Sinal salvo")
        print("timestamp:\t" + colunas[0] + "\tativo:\t" + colunas[1] + "\tacao:\t" + colunas[2])
        print("expiracao:\t" + colunas[3] + "\tgale:\t" + colunas[4] + "\tvalor:\t" + colunas[5])
        #

class sentinela_de_arquivos(FileSystemEventHandler):
    def on_created(self, evento):
        # print("Arquivo criado: " + evento.src_path)
        split_path = evento.src_path.split('\\')
        target_path = os.path.join(pasta_copias, split_path[-1])
        try:
            shutil.copyfile(evento.src_path, target_path)
            # print("Copiando para " + target_path)
        except Exception as e:
            print('Falha ao copiar %s. Motivo: %s' % (target_path, e))
    def on_modified(self, evento):
        # print("Arquivo modificado: " + evento.src_path)
        # print(vars(evento))
        split_path = evento.src_path.split('\\')
        target_path = os.path.join(pasta_copias, split_path[-1])
        # print("Comparando com o arquivo " + target_path + "...")
        with open(evento.src_path, 'r') as file1:
            with open(target_path, 'a') as file2:
                # print("Posição reportada pelo tell(): " + str(file2.tell()))
                file1.seek(file2.tell())
                linha = file1.readline()
                # print("Linhas novas:")
                while len(linha):
                    processar_sinal_retorno_v2(linha)
                    linha = file1.readline()
                # print(os.path.getsize(evento.src_path))
                # print(file2.read())
                # print(file1.seek(io.SEEK_END))
                # same = set(file1).difference(file2)
                # print("Linhas diferentes:")
                # print(same)
        file1.close()
        file2.close()
        try:
            shutil.copyfile(evento.src_path, target_path)
            # print("Copiando para " + target_path)
        except Exception as e:
            print('Falha ao copiar %s. Motivo: %s' % (target_path, e))

async def funcao_principal():
    async with websockets.connect(uri_deriv) as soquete:
        print("Mimi Watchdog v0.1")

        rodando = False

        print("Conectando aos servidores da Deriv...")
        resposta_auth_deriv = await autenticar_deriv(soquete, token_deriv)
        auth_deriv = json.loads(resposta_auth_deriv)
        if "authorize" in auth_deriv:
            print("Autenticação bem-sucedida")
            rodando = True
        else:
            print("Falha na autenticação. Saindo do programa...")

        print("Subscrevendo ao saldo")
        print(await enviar_deriv(soquete, """{"balance": 1,"subscribe":1}"""))

        print("Subscrevendo a transações")
        print(await enviar_deriv(soquete, """{"transaction": 1,"subscribe":1}"""))

        print("Fazendo CALL de teste")
        print(await enviar_deriv(soquete, """{
  "proposal": 1,
  "amount": 10,
  "barrier": "+0.1",
  "basis": "payout",
  "contract_type": "CALL",
  "currency": "USD",
  "duration": 60,
  "duration_unit": "s",
  "symbol": "R_100"
}"""))

        while rodando:
            # name = input("Digite 'exit' para sair\n")
            # if name == 'exit':
            #     rodando = False
            # else:
            #     processar_sinal_retorno_v2(name)
            print(await soquete.recv())

if __name__ == "__main__":
    
    pasta_csv = sys.argv[1] if len(sys.argv) > 1 else 'C:\\mimi\\testes\\csv'
    # pasta_csv = sys.argv[1] if len(sys.argv) > 1 else 'C:\\Users\\marro\\AppData\\Roaming\\MetaQuotes\\Terminal\\9D15457EC01AD10E06A932AAC616DC32\\MQL4\\Files'
    pasta_copias = str(pathlib.Path(__file__).parent.resolve()) + "\copias"
    if not os.path.exists(pasta_copias):
        os.mkdir(pasta_copias)
    
    print("Pasta CSV: " + pasta_csv)
    print("Pasta copias: " + pasta_copias)

    print("Esvaziando pasta copias...")
    for filename in os.listdir(pasta_copias):
        file_path = os.path.join(pasta_copias, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
            print("Apagando " + file_path)
        except Exception as e:
            print('Falha ao apagar %s. Motivo: %s' % (file_path, e))

    print("Copiando pasta csv...")
    for filename in os.listdir(pasta_csv):
        file_path = os.path.join(pasta_csv, filename)
        copy_path = os.path.join(pasta_copias, filename)
        try:
            shutil.copyfile(file_path, copy_path)
            print("Copiando " + file_path)
        except Exception as e:
            print('Falha ao copiar %s. Motivo: %s' % (file_path, e))

    sentinela_mt4 = sentinela_de_arquivos()
    observer = Observer()
    observer.schedule(sentinela_mt4, pasta_csv, recursive=True)
    observer.start()
    try:
        asyncio.run(funcao_principal())
    finally:
        observer.stop()
        observer.join()
