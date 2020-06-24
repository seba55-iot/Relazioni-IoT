import tkinter as tk
import requests
import json

window = tk.Tk()
window.geometry("480x500")
window.title("MANAGE ARDUINO")
window.configure(background = "gold")
window.grid_columnconfigure(0, weight=1)

def temperature():
    output = requests.get("http://seba.local/arduino/temperature").json()
    text_response = f"Il valore di temperatura è: {output['e'][0]['v']}"
    textwidget = tk.Text(height=2)
    textwidget.configure(background = "deep sky blue")
    textwidget.insert(tk.END, text_response)
    textwidget.grid(row=1, column =0, sticky="WE", padx = 10)


def led():
    output = requests.get("http://seba.local/arduino/led").json()
    text_response = f"Il led è: {output['e'][0]['v']}"
    textwidget = tk.Text(height=2)
    textwidget.configure(background = "deep sky blue")
    textwidget.insert(tk.END, text_response)
    textwidget.grid(row=1, column =0, sticky="WE", padx = 10)

def move():
    output = requests.get("http://seba.local/arduino/move").json()
    text_response = f"Il numero di movimenti rilevati negli ultimi 60 sec è: {output['e'][0]['v']}"
    textwidget = tk.Text(height=2)
    textwidget.configure(background = "deep sky blue")
    textwidget.insert(tk.END, text_response)
    textwidget.grid(row=1, column =0, sticky="WE", padx = 10)

def dcMotorOn():
    output = requests.get("http://seba.local/arduino/dcmotor1").json()
    text_response = f"La velocità della ventola è: {output['e'][0]['v']}"
    textwidget = tk.Text(height=2)
    textwidget.configure(background = "deep sky blue")
    textwidget.insert(tk.END, text_response)
    textwidget.grid(row=1, column =0, sticky="WE", padx = 10)

def dcMotorOff():
    output = requests.get("http://seba.local/arduino/dcmotor2").json()
    text_response = f"La velocità della ventola è: {output['e'][0]['v']}"
    textwidget = tk.Text(height=2)
    textwidget.configure(background = "deep sky blue")
    textwidget.insert(tk.END, text_response)
    textwidget.grid(row=1, column =0, sticky="WE", padx = 10)
    



welcome_label = tk.Label(window,
                         text = "Welcome!",
                         background = "DeepPink2",
                         font=("Helvetica",20))
welcome_label.grid(row=0, column=0, sticky="N", padx=20, pady=10)
string = """Premi:
1. Temperature = per sapere il valore di temperatura;
2. Led = per cambiare lo stato attuale del led
3. Movement = per sapere il numero di movimenti rilevati negli ultimi 60 sec
4. DcMotorON = per avviare la ventola
5. DcMotorOFF = per stoppare la ventola\n"""
textwidget = tk.Text(height=8)
textwidget.configure(background = "pale green", fg="orangered")
textwidget.insert(tk.END, string)
textwidget.grid(row=8, column =0, sticky="WE", padx = 10)

download_button = tk.Button(text="Temperature", command=temperature, background = "white smoke", fg="firebrick1", font="Aharoni")
download_button.grid(row=3, column=0, sticky="WE", pady=5, padx=50)

download_button = tk.Button(text="Led", command=led, background = "grey3",fg="violetred",font="Aharoni")
download_button.grid(row=4, column=0, sticky="WE", pady=5, padx=50)

download_button = tk.Button(text="Movement", command=move, background = "white smoke",fg="pale violet red",font="Aharoni")
download_button.grid(row=5, column=0, sticky="WE", pady=5, padx=50)

download_button = tk.Button(text="DCMotorON", command=dcMotorOn, background = "grey3",fg="Slateblue1",font="Aharoni")
download_button.grid(row=6, column=0, sticky="WE", pady=5, padx=50)

download_button = tk.Button(text="DCMotorOFF", command=dcMotorOff, background = "white smoke",fg="maroon2",font="Aharoni")
download_button.grid(row=7, column=0, sticky="WE", pady=5, padx=50)


if __name__=="__main__":
    window.mainloop()
    
