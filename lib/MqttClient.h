#ifndef RECRUITMENT_VIRTUALROVER_MQTTCLIENT_H
#define RECRUITMENT_VIRTUALROVER_MQTTCLIENT_H

#include <iostream>
#include "mqtt/async_client.h"

class callback : public virtual mqtt::callback , mqtt::will_options{
    bool& connected_flag;
    std::shared_ptr<mqtt::async_client> client;

    std::function<void()> callbackFunc = nullptr;

    void connected(const std::string& cause) override {
        std::cout << "MQTT Connection success" << std::endl;
        connected_flag = true;
        client->publish("VR/game/status ", "active");
    }

    void connection_lost(const std::string& cause) override {
        std::cout << "Connection lost" << std::endl;
        connected_flag = false;
    }

    void message_arrived(mqtt::const_message_ptr msg) override {
            callbacks[msg->get_topic()](msg);
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
    callback(bool& connected, std::shared_ptr<mqtt::async_client> cli) : connected_flag(connected), client(cli){
    }

    void set_callback(std::string topic, std::function<void(mqtt::const_message_ptr msg)> callbackFunction){
        callbacks[topic] = callbackFunction;
        std::cout << "MqttHandler : added " << topic << " topic to list."<<std::endl;
    }

    std::map<std::string,std::function<void(mqtt::const_message_ptr msg)>> callbacks;

};

class MqttClient {
    inline static std::string brokerip = "127.0.0.1:1883";
    bool connected = false;
    std::shared_ptr<mqtt::async_client> client;
    std::shared_ptr<callback> cb;
    mqtt::connect_options connOpts;

public:

    MqttClient(std::string ID, std::string ip){
        connect(ID,ip);
    }

    ~MqttClient(){
        this->disconnect();
    }


    bool connect(std::string ID, std::string ip){

        client = std::make_shared<mqtt::async_client>(ip, ID);
        cb = std::make_shared<callback>(connected, client);
        client->set_callback(*cb);
        brokerip = ip;

        try {
            connOpts.set_automatic_reconnect(true);
            //set last will for the subsystem
            connOpts.set_will_message(mqtt::message("VR/game/status", "lost"));
            //set clean session
            connOpts.set_clean_session(true);

            client->connect(connOpts)->wait();
        }
        catch (const mqtt::exception& e) {
            std::cerr<<"Can't connect to MQTT server"<<std::endl;
            return false;
        }
        connected = true;
        return true;
    }

    bool disconnect(){
        try {
            this->publish("VR/game/status ", "lost");
            client->disconnect()->wait();
        }
        catch (const mqtt::exception& e) {
            std::cerr<<"Error while disconnecting"<<std::endl;
            return false;
        }
        connected = false;
        return true;
    }

    bool is_connected(){
        return connected;
    }

    void subscribe(std::string topic, std::function<void( mqtt::const_message_ptr)> callback, int qos = 0){

        if (cb->callbacks.find(topic) != cb->callbacks.end()){
            std::cerr <<"MqttHandler: error : "<< topic <<" already in map";
        } else {
            //add subscription and topic
            std::this_thread::sleep_for(std::chrono::seconds(1));
            client->subscribe(topic, qos)->wait();
            cb->set_callback(topic, callback);
        }
    }

    void publish(std::string topic, std::string payload, int qos = 2, bool retain = true){
        if(is_connected()){
            client->publish(topic, payload, qos, retain);
        }
    }

    const std::shared_ptr<mqtt::async_client> &getClient() const {
        return client;
    }

};


#endif //RECRUITMENT_VIRTUALROVER_MQTTCLIENT_H
