import paho.mqtt.client as PahoMQTT
import cherrypy
import json
import time
import threading

DEVICES = []
USERS = []
SERVICES = []

class Thread(threading.Thread):
    def __init__(self, threadID):
        threading.Thread.__init__(self)
        self.threadID = threadID
        
    def run(self):
        while True:
            for item in DEVICES:
                if time.time() - item["timestamp"] >= 20000:
                    DEVICES.remove(item)

            time.sleep(60)

class MySubscriber:
    def __init__(self, clientID,topic,broker):
        self.clientID = clientID
        self._paho_mqtt = PahoMQTT.Client(clientID, False)
        self._paho_mqtt.on_connect = self.myOnConnect
        self._paho_mqtt.on_message = self.myOnMessageReceived
        self.topic = topic
        self.messageBroker =broker 

    def start (self):
        self._paho_mqtt.connect(self.messageBroker, 1883)
        self._paho_mqtt.loop_start()
        self._paho_mqtt.subscribe(self.topic, 2)

    def stop (self):
        self._paho_mqtt.unsubscribe(self.topic)
        self._paho_mqtt.loop_stop()
        self._paho_mqtt.disconnect()

    def myOnConnect (self, paho_mqtt, userdata, flags, rc):
        print ("Connected to %s with result code: %d" % (self.messageBroker, rc))

    def myOnMessageReceived (self, paho_mqtt , userdata, msg):
        print ("Topic:'" + msg.topic+"', QoS: '"+str(msg.qos)+"' Message: '"+str(msg.payload) + "'")
        payload = json.loads(msg.payload)
        new_device = {
                "deviceID": payload["deviceID"],
                "end-points": payload["end-points"],
                "resources": payload["resources"],
                "timestamp": time.time()
            }
        DEVICES.append(new_device)
       


class Lab2SW(object):
    exposed = True

    def __init__(self):

        data = {}
        self.json = json.dumps(data)
        
        self.broker = "test.mosquitto.org"
        self.port = "1883"
        self.thread = Thread("Thread 1")
        self.thread.start()

    
    def GET(self, *uri, **params):
        
        if uri[0] == "broker":
            payload = {
                "broker": self.broker,
                "port": self.port
                }
            return json.dumps(payload)
        
        elif uri[0] == "devices": #punto3
            return json.dumps(DEVICES)
        
        elif uri[0] == "deviceID": #punto4
            item_return =""
            for item in DEVICES:
                if item["deviceID"] == int(uri[1]):
                    item_return = item
            if item_return == "":
                raise cherrypy.HTTPError(400, "deviceID not found")
            return json.dumps(item_return)
        
        elif uri[0] == "users": #punto6
            return json.dumps(USERS)

        elif uri[0] == "userID": #punto7
            item_return = ""
            for item in USERS:
                if item["userID"] == int(uri[1]):
                    item_return = item
            if item_return == "":
                raise cherrypy.HTTPError(400, "userID not found")
            return json.dumps(item_return)

        elif uri[0] == "services": #punto9
            return json.dumps(SERVICES)

        elif uri[0] == "serviceID": #punto10
            item_return = ""
            for item in SERVICES:
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
            
            for item in DEVICES: #per esercizio 3
                if item["deviceID"] == int(new_device["deviceID"]):
                    DEVICES.remove(item)
                    
            DEVICES.append(new_device)

        elif uri[0] == "addUser": #punto5
            self.json = json.loads(cherrypy.request.body.read())
        
            new_user = {
                "userID": int(self.json["userID"]),
                "name": self.json["name"],
                "surname": self.json["surname"],
                "email": self.json["email"]
            }
            USERS.append(new_user)

        elif uri[0] == "addService": #punto7
            self.json = json.loads(cherrypy.request.body.read())

            
            new_service = {
                "serviceID": int(self.json["serviceID"]),
                "description": self.json["description"],
                "end-points": "http://localhost:8080/addService",
                "timestamp": time.time()
            }
            SERVICES.append(new_service)

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
    test=MySubscriber("MySubscriber", "tiot/6/device", "test.mosquitto.org")
    test.start()
    cherrypy.engine.start()
    cherrypy.engine.block()
