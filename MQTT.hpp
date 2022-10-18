#include "MQTTClient.h"

namespace MQTT
{
	const int QOS = 2;
	void initialize();
	void publish(const char* topic, void *payload);
	void finalize();
}