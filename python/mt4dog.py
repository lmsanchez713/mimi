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

class sentinela_de_arquivos(FileSystemEventHandler):
    def on_created(self, evento):
        print("Arquivo criado: " + evento.src_path)
        split_path = evento.src_path.split('\\')
        target_path = os.path.join(pasta_copias, split_path[-1])
        try:
            shutil.copyfile(evento.src_path, target_path)
            print("Copiando para " + target_path)
        except Exception as e:
            print('Falha ao copiar %s. Motivo: %s' % (target_path, e))
    def on_modified(self, evento):
        print("Arquivo modificado: " + evento.src_path)
        print(vars(evento))
        split_path = evento.src_path.split('\\')
        target_path = os.path.join(pasta_copias, split_path[-1])
        print("Comparando com o arquivo " + target_path + "...")
        with open(evento.src_path, 'r') as file1:
            with open(target_path, 'r') as file2:
                same = set(file1).difference(file2)
                print("Linhas diferentes:")
                print(same)

if __name__ == "__main__":
    print("Mimi Watchdog v0.1")
    
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

    async def hello():
        uri = "wss://ws.binaryws.com/websockets/v3?app_id=1089"
        async with websockets.connect(
            uri, ssl=True
        ) as websocket:
            req = """{
  "authorize": "HVumMdEbE5TdVUP"
}"""

            await websocket.send(req)
            print(f"> {req}")

            greeting = await websocket.recv()
            print(f"< {greeting}")

    asyncio.get_event_loop().run_until_complete(hello())

    sentinela_mt4 = sentinela_de_arquivos()
    observer = Observer()
    observer.schedule(sentinela_mt4, pasta_csv, recursive=True)
    observer.start()
    try:
        while True:
            time.sleep(1)
    finally:
        observer.stop()
        observer.join()
