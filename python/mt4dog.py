import sys
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

class CACHORRO_VIGIA(FileSystemEventHandler):
    def on_any_event(self, evento):

        # Aqui se insere o código a ser executado quando houver um evento de arquivo da pasta
        # (código da DERIV)
        print("TIPO:")
        print(type(evento))
        print("DIR:")
        print(dir(evento))
        print("VARS:")
        print(vars(evento))

if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else 'C:\\mimi\\testes\\csv'
    CACHORRAO = CACHORRO_VIGIA()
    observer = Observer()
    observer.schedule(CACHORRAO, path, recursive=True)
    observer.start()
    try:
        while True:
            time.sleep(1)
    finally:
        observer.stop()
        observer.join()
