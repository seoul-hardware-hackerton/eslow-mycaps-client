import paho.mqtt.client as mqtt

class mqttSender:
    def __init__(self, id):
        self.client2 = mqtt.Client(id)
        
    def connect(self, address):
        self.client2.connect(address)

    def publish(self, channel, data):
        pub = self.client2.publish(channel, data)
        pub.wait_for_publish()

    def close(self):
        # close 방법 생가해볼 필요가 있음..
        self.client2.disconnect()
