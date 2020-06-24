import requests
import json
import time

if __name__=="__main__":

    while True:
        print("Add Device")
        deviceID = input("Device ID: ")
        resources = input("Resources: ")
        payload = {
            "deviceID": deviceID,
            "resources": resources
            }
        requests.put("http:localhost:8080/addDevice", payload)
        time.sleep(60)
 
