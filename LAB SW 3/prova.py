import paho.mqtt.client as PahoMQTT
import requests
import json
import time


class MySubscriber:
    def __init__(self, clientID,topic,broker):
        self.clientID = clientID
        self._paho_mqtt = PahoMQTT.Client(clientID, False)
        self._paho_mqtt.on_connect = self.myOnConnect
        self._paho_mqtt.on_message = self.myOnMessageReceived
        self.topic = topic
        self.messageBroker = broker 

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
       
        


if __name__=="__main__":

    payload = {
        "serviceID": 789,
        "description": "temperature information"
        }
    
    requests.put("http://localhost:8080/addService", json.dumps(payload))
    response = requests.get("http://localhost:8080/deviceID/123")
    our_device = response.json()
    endpoint = our_device["end-points"]
    print(endpoint)
        
    #dato che è un subscriber, non può pubblicare -> per ricavare il broker facciamo una
    #richiesta GET
    broker = requests.get("http://localhost:8080/broker")
    our_broker = broker.json()
    our_broker2 = our_broker["broker"]
    print(our_broker2)
    
    test2 = MySubscriber("MySubscriber", endpoint, our_broker2)
    
    test2.start()
    time.sleep(60)
    
