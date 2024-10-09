import json
import serial

path = 'Dosenkueler_Testreihe/dosenkueler_v1.json'
content = {}

def load_file():
    global content
    try:
        with open(path, 'r', encoding='utf-8') as f:
            content = json.load(f)
    except FileNotFoundError:
        pass

def add_content(id, new_content):
    content[id] = new_content
    save_file()

def remove_content(id):
    if id in content:
        content.pop(id)
        save_file()

def save_file():
    with open(path, 'w') as f:
        json.dump(content, f, indent=4)


ser = serial.Serial()
ser.port = 'COM4'
ser.baudrate = 115200
ser.dtr = False
ser.rts = False

ser.open()

load_file()

ser.write(bytes('start', 'utf-8'))
print('Datenreihe gestartet')

while (True):
    input = ser.readline()
    print(input)

    input_list = input.decode('utf-8').split(sep=';')
    add_content(id=input_list[0], new_content={"airTemp":input_list[1], "liquidTemp":input_list[2]})



