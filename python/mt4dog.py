import sys
import time
import logging
from watchdog.observers import Observer
from watchdog.events import LoggingEventHandler
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
    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s - %(message)s',
                        datefmt='%Y-%m-%d %H:%M:%S')
    path = sys.argv[1] if len(sys.argv) > 1 else '.'
    event_handler = LoggingEventHandler()
    CACHORRAO = CACHORRO_VIGIA()
    observer = Observer()
    # observer.schedule(event_handler, path, recursive=True)
    observer.schedule(CACHORRAO, path, recursive=True)
    observer.start()
    try:
        while True:
            time.sleep(1)
    finally:
        observer.stop()
        observer.join()
