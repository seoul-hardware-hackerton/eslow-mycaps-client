#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include "mbed.h"
#include "MAX4466.h"
#include "mbed_events.h"

#define MAX_BUF_SIZE 128
#define SERVER_HOST "221.147.12.67"
#define SERVER_PORT 12345
#define HTTP_URI "/"

Serial pc (USBTX, USBRX, 115200);
DigitalOut red(D0);
DigitalOut green(D1);
DigitalOut blue(D2);
MAX4466 mic(A1);

void messageArrived(MQTT::MessageData& md)
{
        MQTT::Message &message = md.message;
     // sprintf(x, "%d", message.payload);
        int data = *((int *)message.payload) - 48;
        
        switch ( data ) {
            case 0:
                break;
            case 1:
                green = 0;
                blue = 0;
                red = 1;
                break;        
            case 2:
                green = 1;
                blue = 0;
                red = 0;
                break;   
            case 3:
                green = 0;
                blue = 1;
                red = 0;
                break;   
            case 4:
                break;   
        }
        
        pc.printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);;
        
    
   
}



int main(void) {
    pc.printf("Connecting to the network using Wifi...\n");
    NetworkInterface* net = NetworkInterface::get_default_instance();
    
    nsapi_error_t net_status = -1;
    for (int tries = 0; tries < 3; tries++) {
        net_status = net->connect();
        if (net_status == NSAPI_ERROR_OK) {
            break;
        } else {
            pc.printf("Unable to connect to network. Retrying...\n");
        }
    }

    if (net_status != NSAPI_ERROR_OK) {
        printf("ERROR: Connecting to the network failed (%d)!\n", net_status);
        return -1;
    }

    pc.printf("Connected to the network successfully. IP address: %s\n", net->get_ip_address());
    
    MQTTNetwork mqttNetwork(net);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);
 
    float version = 0.6;
    const char* pub_topic = "eslow/voice";
    const char* sub_topic = "eslow/alarm";
    
    const char* hostname = "52.141.36.28";
    int port = 1883;
    pc.printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNetwork.connect(hostname, port);
    if (rc != 0)
        pc.printf("rc from TCP connect is %d\r\n", rc);
 
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "ClientPublisher";
    
    if ((rc = client.connect(data)) != 0)
        pc.printf("rc from MQTT connect is %d\r\n", rc);
 
    if ((rc = client.subscribe(sub_topic, MQTT::QOS0, messageArrived)) != 0)
        pc.printf("rc from MQTT subscribe is %d\r\n", rc);

    char buf[10];
    MQTT::Message message;
    float x = 0;  
    while(1) {
        mic.volume_indicator();
        x = mic.sound_level();
        pc.printf("%f", x);
        sprintf(buf, "%f", x);
        message.qos = MQTT::QOS0;
        message.retained = false; 
        message.dup = false;
        message.payload = (void*)buf;
        message.payloadlen = strlen(buf)+1;
        client.publish(pub_topic, message);
        client.yield(1000); 
    }
  
    if ((rc = client.unsubscribe(sub_topic)) != 0)
        pc.printf("rc from unsubscribe was %d\r\n", rc);
 
    if ((rc = client.disconnect()) != 0)
        pc.printf("rc from disconnect was %d\r\n", rc);
 
    mqttNetwork.disconnect();
 
    pc.printf("Version %.2f: finish %d msgs\r\n", version);
 
}
