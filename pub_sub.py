import random
import time

from paho.mqtt import client as mqtt_client

broker = 'broker.mqtt-dashboard.com'
port = 1883
topic = "outTopic"

client_id = f'python-mqtt-5'

client_id2 = f'python-mqtt-3'
username = 'subscriber'
password = 'public'


def connect_mqtt(client_id,pub_sub) -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0 and pub_sub==0:
            print("Publisher Connected to MQTT Broker!") 
        elif rc == 0 and pub_sub==1:
            print("Subscriber Connected to MQTT Broker!") 
        else:
            print("Failed to connect, return code %d\n", rc)
        # Add subsription for topics and assign "on message" handler 
        client.subscribe(topic)
    
    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.on_message = on_message
    
    client.connect(broker, port)
    return client

# def subscribe(client: mqtt_client):
def on_message(client, userdata, msg):
    print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

    # client.on_message = on_message
    
def pub_sub(client1):
    time.sleep(2.05)
    
    for i in range(0,10):
        msg = f"messages: {i}"
        result = client1.publish(topic, msg)
        print('-------------------------------------------------------------')
        status = result[0]
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
            # subscribe(client2)
            # client1.loop_start()
            time.sleep(2.05)
        
        else:
            print(f"Failed to send message to topic {topic}")
        

def run():
    client = connect_mqtt(client_id,pub_sub=0)
    # client2 = connect_mqtt(client_id2,pub_sub=1) 
    client.loop_start()
    pub_sub(client)

run()