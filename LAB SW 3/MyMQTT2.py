import paho.mqtt.client as PahoMQTT
import requests
import json

class MyMQTT:
    def __init__(self, clientID, broker, topic, port):
        self.broker = broker
        self.port = port
        #self.notifier = self
        self.clientID = clientID
 
        self._topic = topic
        self._isSubscriber = False
 
        # create an instance of paho.mqtt.client
        self._paho_mqtt = PahoMQTT.Client(clientID, False) 
 
        # register the callback
        self._paho_mqtt.on_connect = self.myOnConnect
        self._paho_mqtt.on_message = self.myOnMessageReceived
 
 
    def myOnConnect (self, paho_mqtt, userdata, flags, rc):
        print ("Connected to %s with result code: %d" % (self.broker, rc))
 
    def myOnMessageReceived (self, paho_mqtt , userdata, msg):
        # A new message is received
        print(msg.topic + msg.payload)
 
 
    def myPublish (self, topic, msg):
        # if needed, you can do some computation or error-check before publishing
        #uncomment the line below for debugging
        #print ("publishing '%s' with topic '%s'" % (msg, topic))
        # publish a message with a certain topic
        self._paho_mqtt.publish(topic, msg, 2)
 
    def mySubscribe (self, topic):
        # if needed, you can do some computation or error-check before subscribing
        #assert hasattr(self.notifier,'notify')
        print ("subscribing to %s" % (topic))
        # subscribe for a topic
        self._paho_mqtt.subscribe(topic, 2)
 
        # just to remember that it works also as a subscriber
        self._isSubscriber = True
        self._topic = topic
 
    def start(self):
        #manage connection to broker
        self._paho_mqtt.connect(self.broker , self.port)
        self._paho_mqtt.loop_start()
    def unsubscribe(self):
        if (self._isSubscriber):
            # remember to unsuscribe if it is working also as subscriber 
            self._paho_mqtt.unsubscribe(self._topic)
    def stop (self):
        if (self._isSubscriber):
            # remember to unsuscribe if it is working also as subscriber 
            self._paho_mqtt.unsubscribe(self._topic)
 
        self._paho_mqtt.loop_stop()
        self._paho_mqtt.disconnect()


if __name__=="__main__":

    payload = {
        "serviceID": 789,
        "description": "MyMQTT"
        }
    
    requests.put("http://localhost:8080/addService", json.dumps(payload))
    response = requests.get("http://localhost:8080/deviceID/123")
    our_device = response.json()
    endpoint = our_device["end-points"]
    #endpoint = "tiot/6/device/sensors"
    print(endpoint)
        
    #dato che è un subscriber, non può pubblicare -> per ricavare il broker facciamo una
    #richiesta GET
    broker = requests.get("http://localhost:8080/broker")
    our_broker = broker.json()
    our_broker2 = our_broker["broker"]
    our_broker3 = our_broker["port"]
    print(our_broker2)
    
    test=MyMQTT("MySubscriber2", our_broker2, endpoint,1883)
    #test.mySubscribe(endpoint)
    test.start()
    test.mySubscribe(endpoint)


    while True:
        user_input = input("""Cosa vuoi fare?
lcd: Stampa qualcosa nell'lcd
temp: Per azionare il motore
led: Settare il led in base alla temperatura\n""")
        
        senML = {
            "bn": "Yun",
            "e": [{
                "n": str(user_input),
                "u": "",
                "v": ""
                    }]
                }
        
        if (user_input == "lcd"):
            senML["e"][0]["v"] = input("Cosa vuoi stampare?")
        if (user_input == "temp"):
            senML["e"][0]["v"] = input("Valore di temperatura: ")
        if (user_input == "led"):
            senML["e"][0]["v"] = input("Valore di temperatura: ")

        test.myPublish(endpoint, json.dumps(senML))



            
            
            
        
    
    
