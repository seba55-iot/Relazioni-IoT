import cherrypy
import json
import time
import threading

class Thread(threading.Thread):
    def __init__(self, threadID, devices, services):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.devices = devices
        self.services = services
    def run(self):
        while True:
            for item in self.devices:
                if time.time() - item["timestamp"] >= 120:
                    self.devices.remove(item)
                    
            for item in self.services:
                if time.time() - item["timestamp"] >= 120:
                    self.services.remove(item)

            time.sleep(60)
        

class Lab2SW(object):
    exposed = True

    def __init__(self):
        self.devices = []
        self.users = []
        self.services = []
        data = {}
        self.json = json.dumps(data)
        self.broker = "mqtt.eclipse.org"
        self.port = "8080"
        self.thread = Thread("Thread 1", self.devices, self.services)
        self.thread.start()
        

    
    def GET(self, *uri, **params):
        if uri[0] == "broker":
            return self.broker + self.port
        
        if uri[0] == "devices": #punto3
            return json.dumps(self.devices)
        
        elif uri[0] == "deviceID": #punto4
            item_return =""
            for item in self.devices:
                if item["deviceID"] == int(uri[1]):
                    item_return = item
            if item_return == "":
                raise cherrypy.HTTPError(400, "deviceID not found")
            return json.dumps(item_return)
        
        elif uri[0] == "users": #punto6
            return json.dumps(self.users)

        elif uri[0] == "userID": #punto7
            item_return = ""
            for item in self.users:
                if item["userID"] == int(uri[1]):
                    item_return = item
            if item_return == "":
                raise cherrypy.HTTPError(400, "userID not found")
            return json.dumps(item_return)

        elif uri[0] == "services": #punto9
            return json.dumps(self.services)

        elif uri[0] == "serviceID": #punto10
            item_return = ""
            for item in self.services:
                if item["serviceID"] == int(uri[1]):
                    item_return = item
            if item_return == "":
                raise cherrypy.HTTPError(400, "serviceID not found")
            return json.dumps(item_return)   
        else:
            raise cherrypy.HTTPError(400, "Dati non disponibili")

    def PUT(self,*uri,**params):
        if uri[0] == "addDevice": #punto2
            self.json = json.loads(cherrypy.request.body.read())
            
            new_device = {
                "deviceID": int(self.json["deviceID"]),
                "end-points": "http://localhost:8080/addDevice",
                "resources": self.json["resources"],
                "timestamp": time.time()
            }
            
            for item in self.devices: #per esercizio 3
                if item["deviceID"] == int(new_device["deviceID"]):
                    self.devices.remove(item)
                    
            self.devices.append(new_device)

        elif uri[0] == "addUser": #punto5
            self.json = json.loads(cherrypy.request.body.read())
        
            new_user = {
                "userID": int(self.json["userID"]),
                "name": self.json["name"],
                "surname": self.json["surname"],
                "email": self.json["email"]
            }
            self.users.append(new_user)

        elif uri[0] == "addService": #punto7
            self.json = json.loads(cherrypy.request.body.read())
            
            new_service = {
                "serviceID": int(self.json["serviceID"]),
                "description": self.json["description"],
                "end-points": "http://localhost:8080/addService",
                "timestamp": time.time()
            }
            self.services.append(new_service)

        else:
            raise cherrypy.HTTPError(404, "Servizio non sviluppato")



if __name__=="__main__":
#Standard configuration to serve the url "localhost:8080"
    conf={
        '/':{
               'request.dispatch':cherrypy.dispatch.MethodDispatcher(),
                'tool.session.on':True
               }
        }
    cherrypy.tree.mount(Lab2SW(),'/',conf)
    cherrypy.config.update({"server.socket_host":'0.0.0.0'})
    cherrypy.config.update({'server.socket_port': 8080})
    cherrypy.engine.start()
    cherrypy.engine.block()
