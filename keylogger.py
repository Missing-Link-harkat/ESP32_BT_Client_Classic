from pynput import keyboard
import socket

sock = None

def connect_to_esp32(addr):
    port = 1
    sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
    sock.connect((addr, port))
    print(f"Connected to ESP32 with address {addr}")
    return sock

def on_press(key):
    global sock
    try:
        key_str = key.char if hasattr(key, "char") else key.name
        
        if key_str == "esc":
            print("Exiting...")
            sock.close()
            exit(0)

        key_type = "char" if hasattr(key, "char") else "name"
        data = f"{{\"key\": \"{key_str}\", \"type\": \"{key_type}\"}}"
        sock.send(data.encode())
    except Exception as e:
        print(f"Error: {e}")

def main():
    global sock
    addr = "AC:15:18:D4:C3:36"

    start = input("Press enter to start the keylogger: ")
    while (start != ""):
        start = input("Press enter to start the keylogger: ")

    sock = connect_to_esp32(addr)

    with keyboard.Listener(on_press=on_press) as listener:
        listener.join()

if __name__ == "__main__":
    main()