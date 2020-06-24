import requests
import json
import datetime

if __name__=="__main__":

    while True:
        user_input = input("""Retrieve information about:
m: message broker
rd: all the registered devices
d: device with a specific deviceID
ru: all users
u: user with a specific userID
rs: all the register services
s: service with a specific serviceID\n""")
        
        if user_input =="m":
            url = "http://localhost:8080/broker"
            response = requests.get(url)
            print(response.json())
            
        elif user_input == "rd":
            url = "http://localhost:8080/devices"
            response = requests.get(url)
            print(response.json())
            

        elif user_input == "d":
           deviceID = input("Inserisci deviceID: ")
           response = requests.get(f"http://localhost:8080/deviceID/{deviceID}")
           print(response.json())
           
            
        elif user_input=="ru":
            response = requests.get("http://localhost:8080/users")
            print(response.json())

        elif user_input=="u":
            userID = input("Inserisci userID: ")
            response = requests.get(f"http://localhost:8080/userID/{userID}")
            print(response.json())

        elif user_input=="rs":
            response = requests.get("http://localhost:8080/services")
            print(response.json())

        elif user_input=="s":
            userID = input("Inserisci userID: ")
            response = requests.get(f"http://localhost:8080/serviceID/{serviceID}")
            print(response.json())
