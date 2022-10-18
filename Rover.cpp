#include "Rover.hpp"
#include "MQTT.hpp"

void Rover::setVelocity(const char* velocity)
{
	MQTT::publish("VR/rover/control/velamount", (void*) velocity);
}

void Rover::setSteeringAngle(const char* steeringAngle)
{
	MQTT::publish("VR/rover/control/velangle", (void*) steeringAngle);
}