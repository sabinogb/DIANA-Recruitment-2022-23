#include <iostream>
#include <cstring>
#include "MQTT.hpp"

static MQTTClient LMAO;

/* This function initializes the MQTT client that will be used in the program. */
void MQTT::initialize()
{
	int resultCode;
	
	resultCode = MQTTClient_create(&LMAO, "tcp://localhost:1883", "LMAO", MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if(resultCode != MQTTCLIENT_SUCCESS)
	{
		std::cout << "Could not create an MQTT client.\n";
		exit(0);
	}	
	
	MQTTClient_connectOptions connectOptions = MQTTClient_connectOptions_initializer;
	resultCode = MQTTClient_connect(LMAO, &connectOptions);
	
	if(resultCode != MQTTCLIENT_SUCCESS)
	{
		std::cout << "Could not connect.\n";
		exit(0);
	}
}

/* This function will be called for any PUBLISH operation. */
void MQTT::publish(const char* topic, void *payload)
{
	MQTTClient_message message = MQTTClient_message_initializer;
	message.payload = payload;
    message.payloadlen = strlen((char*) payload);
	message.qos = MQTT::QOS;
	MQTTClient_deliveryToken token;
	
	MQTTClient_publishMessage(LMAO, topic, &message, &token);
	MQTTClient_waitForCompletion(LMAO, token, 5000UL);
}

/* This function will be called just before the program exits. */
void MQTT::finalize()
{
	int resultCode = MQTTClient_disconnect(LMAO, 5000UL);
	
	if(resultCode != MQTTCLIENT_SUCCESS)
	{
		std::cout << "Could not disconnect.\n";
		exit(0);
	}
	
    MQTTClient_destroy(&LMAO);
}