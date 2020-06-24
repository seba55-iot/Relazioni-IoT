import paho.mqtt.client as PahoMQTT
import time
import json
import requests



class MyPublisher:
	def __init__(self, clientID,topic):
		self.clientID = clientID

		# create an instance of paho.mqtt.client
		self._paho_mqtt = PahoMQTT.Client(self.clientID, False) 
		# register the callback
		self._paho_mqtt.on_connect = self.myOnConnect

		#self.messageBroker = 'iot.eclipse.org'
		self.messageBroker = 'test.mosquitto.org'
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
    test = MyPublisher("MyPublisher", "tiot/6/device")
    test.start()

    while True:
            deviceID = input("Device ID: ")
            resources = input("Resources: ")
            subscription = {
                    "deviceID": deviceID,
                    "resources": resources
            }
            print(subscription)
            test.myPublish(json.dumps(subscription))
            time.sleep(60)
       
