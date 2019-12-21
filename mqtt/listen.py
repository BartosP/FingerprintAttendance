import paho.mqtt.client as mqtt
from sqlalchemy import create_engine, Column, Integer, String, Text
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship
from PIL import Image

Server = "10.0.0.41"
MQTT_Port = 1883
MQTT_Topic = "fingerprint"

Base = declarative_base()

class Fingerprint(Base):
	__tablename__ = 'fingerprints'
	
	id = Column('id', Integer, primary_key=True)
	name = Column('jmeno', String(50), index=True)
	surname = Column('prijmeni', String(50), index=True)
	finger = Column('otisk', Text)

def insert_finger(name, surname, finger):
	fingerprint = Fingerprint(name = name, surname = surname, finger = finger)
	session.add(fingerprint)
	session.commit()
	
def on_connect(mosq, obj, flags, rc):
	client.subscribe(MQTT_Topic, 0)

def on_message(mosq, obj, msg):
	message = msg.payload.decode("UTF-8").split("//")
	if len(message) != 3:
		print("Input error")
	else:
		print("MQTT Topic: " + msg.topic)
		print("MQTT Message: " + message[0] + ", " + message[1] + ", " + message[2] + "\n")
		insert_finger(message[0], message[1], message[2])


engine = create_engine('mysql+pymysql://User:Password@' + Server + '/attendance')
Base.metadata.create_all(bind = engine)
Session = sessionmaker(bind = engine)
session = Session()

client = mqtt.Client()
client.on_message = on_message
client.on_connect = on_connect

client.connect(Server, int(MQTT_Port), 50)
client.loop_forever()