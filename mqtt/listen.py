import paho.mqtt.client as mqtt
import mysql.connector as mysql

Server = "10.0.0.33"
MQTT_Port = 1883
MQTT_Topic = "fingerprint"

def insert_finger(name, surname, finger):
	conn = mysql.connect(host = Server, database = 'attendance', user = 'User', password = 'Pass')
	query = "INSERT INTO fingerprints(jmeno, prijmeni, otisk) VALUES(%s, %s, %s)"
	args = (name, surname, finger)
	cursor = conn.cursor()
	cursor.execute(query, args)
	conn.commit()
	cursor.close()
	conn.close()
	
def on_connect(mosq, obj, flags, rc):
	client.subscribe(MQTT_Topic, 0)

def on_message(mosq, obj, msg):
	print("MQTT Topic: " + msg.topic)
	print("MQTT Message: " + msg.payload.decode("utf-8") + "\n")
	insert_finger("test", "test", msg.payload.decode("utf-8"))

client = mqtt.Client()
client.on_message = on_message
client.on_connect = on_connect

client.connect(Server, int(MQTT_Port), 50)
client.loop_forever()