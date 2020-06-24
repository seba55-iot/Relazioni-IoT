import paho.mqtt.client as PahoMQTT
import time
import json
import requests



class MyPublisher:
	def __init__(self, clientID,topic, broker):
		self.clientID = clientID

		# create an instance of paho.mqtt.client
		self._paho_mqtt = PahoMQTT.Client(self.clientID, False) 
		# register the callback
		self._paho_mqtt.on_connect = self.myOnConnect

		#self.messageBroker = 'iot.eclipse.org'
		self.messageBroker = broker
		self.topic = topic

	def start (self):
		#manage connection to broker
		self._paho_mqtt.connect(self.messageBroker, 1883)
		self._paho_mqtt.loop_start()

	def stop (self):
		self._paho_mqtt.loop_stop()
		self._paho_mqtt.disconnect()

	def myPublish(self, message):
		# publish a message with a certain topic
		self._paho_mqtt.publish(self.topic, message, 2)

	def myOnConnect (self, paho_mqtt, userdata, flags, rc):
		print ("Connected to %s with result code and topic: %d %s" % (self.messageBroker, rc,self.topic))



if __name__ == "__main__":

        payload = {
        "serviceID": 789,
        "description": "LED_STATE"
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
        
        test = MyPublisher("MyPublisher", endpoint, our_broker2)
        test.start()
        ledState = 0

        while True:
                ledState = not(ledState)
                senML = {
                    "bn": "Yun",
                    "e": [{
                        "n": "led",
                        "u": "",
                        "v": int(ledState)
                    }]
                }
        
           
                test.myPublish(json.dumps(senML))
                time.sleep(60)
       
