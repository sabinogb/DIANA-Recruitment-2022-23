#ifndef RECRUITMENT_VIRTUALROVER_MAP_H
#define RECRUITMENT_VIRTUALROVER_MAP_H

#include "../lib/Obstacle.h"
#include "../lib/Rover.hpp"
#include "../lib/Target.h"
#include <vector>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio.hpp>
#include "../lib/MqttClient.h"

#define NOBSTACLES 11
#define NPROBES 3
#define FONT_DIR "./assets/Adam-Bold.ttf"
#define BACKGROUND_PATH "./assets/sandstone.png"
#define OBSTACLE_PATH "./assets/obstacle.png"
#define AUDIO_PATH "./assets/INTRNLCMD_Sunset_City_from_Royalty_Free_Planet.wav"
#define PROBE_AUDIO_PATH "./assets/probe.wav"

#define PIXELPERMETER 53.33



class Game{
public:
    Game(sf::RenderWindow *win):
            window(win),
            gameOver(false),
            win(false),
            rectObstacles(NOBSTACLES),
            startingArea({80,160}),
            rover({0,0}, {20,50}),
            backgroundTexture(),
            probes(NPROBES),
            probesTaken(0),
            background(),
            musicBuffer(),
            winBackground(),
            loseBackground(),
            music(),
            probeSound(),
            audioVolume(50),
            restart(false),
            clock(),
            gameStatus(OK),
            brokerIp("localhost:1883"),
            obstacleBounds(NOBSTACLES),
            client("GAME",brokerIp){
        //font setup
        myFont.loadFromFile(FONT_DIR);
        //background setup
        backgroundTexture.loadFromFile(BACKGROUND_PATH);
        obstacleTexture.loadFromFile(OBSTACLE_PATH);

        background.setTexture(backgroundTexture);
        background.setOrigin(background.getLocalBounds().width/2, background.getLocalBounds().height/2);
        background.setPosition(window->getSize().x/2, window->getSize().y/2);
        //audio setup
        musicBuffer.loadFromFile(AUDIO_PATH);
        music.setBuffer(musicBuffer);
        probeBuffer.loadFromFile(PROBE_AUDIO_PATH);
        probeSound.setBuffer(probeBuffer);

        //win and lose setup
        winBackground.setSize(sf::Vector2f (window->getSize().x, window->getSize().y) );
        loseBackground.setSize(sf::Vector2f (window->getSize().x, window->getSize().y) );
        winBackground.setFillColor(sf::Color(240,255,240,30));
        loseBackground.setFillColor(sf::Color(10,0,0,100));
        winBackground.setOrigin(winBackground.getSize().x/2,winBackground.getSize().y/2);
        loseBackground.setOrigin(loseBackground.getSize().x/2,loseBackground.getSize().y/2);
        winBackground.setPosition(window->getSize().x/2, window->getSize().y/2);
        loseBackground.setPosition(window->getSize().x/2, window->getSize().y/2);
        //
        startingArea.setOrigin(startingArea.getSize().x/2,startingArea.getSize().y/2);
        startingArea.setPosition(window->getSize().x*5/100,window->getSize().y/2);
        startingArea.setFillColor(sf::Color(0,255,0,50));

        for (auto &probe : probes ){
            probe.setSize(sf::Vector2f(15,15));
            probe.setFillColor(sf::Color(0,250,20));
            probe.setOutlineThickness(5);
            probe.setOutlineColor(sf::Color::Green);
            probe.setOrigin(5, 5);
        }

        for(int i=0;i<NOBSTACLES;i++){
            obstacleBounds[i].resize(4);
            for(int j=0; j<4;j++){
                obstacleBounds[i][j].setFillColor(sf::Color::Black);
            }
        }
        setCallbacks(client.is_connected());
        init();

    }

    void showMenu(){
        ImGui::Begin("Simulation");
        ImDrawList *draw_list(ImGui::GetWindowDrawList());

        ImGui::Text("MQTT settings");


        if(ImGui::Button(client.is_connected() ? "Disconnect from broker" : "Connect to Broker")){
                client.is_connected() ? client.disconnect():  client.connect("VIRTUALROVER", brokerIp);
                setCallbacks(client.is_connected());
            };
            ImGui::InputText("MQTT Broker Ip: <ip:port>", brokerIp, 100);
            ImGui::TextColored(client.is_connected() ? ImColor(0,255,0) : ImColor(255,0,0),client.is_connected() ? "Connected to broker" : "Disconnected");
            ImGui::Separator();
            ImGui::Text("Game");
                if(ImGui::Button("Restart!")) restart = true;
                ImGui::SliderFloat("Velocity", &rover.velocity,-2,2);
                ImGui::SliderFloat("Angle", &rover.wheel.steeringAngle.value,-44,44);
            ImGui::Text("Rover feedback");
                ImGui::Text("Velocity: %f", rover.velocity);
                ImGui::Text("Steering Angle : %f",rover.wheel.steeringAngle.value);
                ImGui::Text("Rover orientation : %f",rover.angle.value);
                /*
                if (distU >0)ImGui::Text("Front sensor : %f",distU);
                if (distD >0)ImGui::Text("Rear sensor : %f",distD);
                if (distR >0)ImGui::Text("Right sensor : %f",distR);
                if (distL >0)ImGui::Text("Left sensor : %f",distL);
                 */
            ImGui::Separator();
            ImGui::Text("Audio");
                ImGui::SliderFloat("Music Volume", &audioVolume,0,100);
        ImGui::End();
    };

    void update(){
        if(!win && !gameOver){
        music.setVolume(audioVolume);
        rover.move();
        }
    };

    /**
     * Checks map, rover and rover components, to see if game over, win or not.
     */
    void checkValues(){
        if (restart){
            init();
            restart = false;
            if (gameOver) gameOver = false;
        }
        //check rover collisions and angles
        if (!gameOver && !win){
            for (auto & obstacle : rectObstacles){
                //check if rover body hits an obstacle
                if (rover.getBody().getGlobalBounds().intersects(obstacle.getGlobalBounds())) gameOver = true;
                //check if rover wheels hit an obstacle
                for(auto & wheel : rover.getWheel().getWheels()){
                    if(wheel.getGlobalBounds().intersects(obstacle.getGlobalBounds())) gameOver = true;
                }
                gameStatus=COLLISION;
            }

            //check probe
            for (auto & probe : probes){
                //check if rover body hits an probe
                if (rover.getBody().getGlobalBounds().intersects(probe.getGlobalBounds())){
                    //yeet the probe
                    probe.setPosition(10000,10000);
                    probesTaken++;
                    probeSound.play();
                };
                //check if rover wheels hit a probe
                for(auto & wheel : rover.getWheel().getWheels()){
                    if(wheel.getGlobalBounds().intersects(probe.getGlobalBounds())){
                        probe.setPosition(10000,10000);
                        //yeet the probe
                        probesTaken++;
                        probeSound.play();
                    };
                }
            }

            if(rover.velocity > 1.5f ||rover.velocity < - 1.5f ){
                gameOver=true;
                gameStatus=BAD_VELOCITY;
            }

            if(rover.wheel.steeringAngle.value > 40.0f || rover.wheel.steeringAngle.value < - 40.0f ){
                gameOver=true;
                gameStatus=BAD_ANGLE;
            }

            //winning condition
            if (probesTaken==NPROBES) {
                win = true;
                gameStatus = WIN;
            }

            //sensors();
        }
    };

    //---mqtt---//
        /*
         * FEEDBACK (publish)
         * VR/game/status (stringa)
         * VR/game/code (int)
         * VR/game/probes (int)
         * VR/rover/feedback/velocity (float)
         * VR/rover/feedback/steeringAngle (float)
         * VR/rover/feedback/orientation (float)
         *
         * CONTROL (read)
         * VR/rover/control/velamout (float)
         * VR/rover/control/velangle (float)
         */

    void feedback(){
        //rover stuff
        if(client.is_connected() &&clock.getElapsedTime().asMilliseconds() >= 500){
            //game
            client.publish("VR/game/probes",std::to_string(NPROBES-probesTaken));
            //game
            client.publish("VR/game/code",std::to_string(gameStatus));
            //veclocity
            client.publish("VR/rover/feedback/velocity",std::to_string(rover.velocity));
            //orientation
            client.publish("VR/rover/feedback/orientation",std::to_string(rover.angle.value));
            //steeringAngle
            client.publish("VR/rover/feedback/steeringAngle",std::to_string(rover.wheel.steeringAngle.value));
            //steeringAngle
            client.publish("VR/game/probes",std::to_string(probesTaken));

            clock.restart();
        }
        //todo: insert MQTT here
    }

    void init(){
        win = false;
        probesTaken=0;
        gameStatus = OK;
        rover.setPosition({startingArea.getPosition().x,startingArea.getPosition().y});
        rover.angle.value=0;

        if(music.getPitch()!=1.0){
            music.setPitch(1.0);
        }
        music.play();

        bool inter = true;
        for (int i=0 ; i<NOBSTACLES; i++){
            int counter = 0;
            rectObstacles[i].setOrigin(rectObstacles[i].getSize().x/2, rectObstacles[i].getSize().y/2);
            rectObstacles[i].setSize(sf::Vector2f ((20+rand()%80)*i, (20+rand()%80)*i));
            //rectObstacles[i].setPosition(rand() % window->getSize().x, rand() % window->getSize().y);
            //rectObstacles[i].setFillColor(sf::Color::Red);
            rectObstacles[i].setTexture(&obstacleTexture, true);
            rectObstacles[i].setOutlineThickness(4);
            rectObstacles[i].setOutlineColor(sf::Color::Red);

            while (inter && counter < 20) {
                inter = false;
                rectObstacles[i].setPosition(rand() % window->getSize().x, rand() % window->getSize().y);
                for (int j = 0; j < i && !inter; j++) {
                    if (i != j && (rectObstacles[i].getGlobalBounds().intersects(rectObstacles[j].getGlobalBounds()) ||
                                   rectObstacles[i].getGlobalBounds().intersects(startingArea.getGlobalBounds()))) {
                        inter = true;
                        counter++;
                    }
                }
            }if(counter >= 20){
                //yeet the obstacle!
                rectObstacles[i].setPosition(-10000,-10000);
            }

            //place obstacle bounds
            //LEFT BOUND
            obstacleBounds[i][0].setSize({1,rectObstacles[i].getSize().y  });
            obstacleBounds[i][0].setPosition(sf::Vector2f (rectObstacles[i].getOrigin().x-rectObstacles[i].getSize().x/2,rectObstacles[i].getOrigin().y ));
            obstacleBounds[i][0].setPosition(rectObstacles[i].getPosition());
            //UP BOUND
            obstacleBounds[i][1].setSize({rectObstacles[i].getSize().x,1 });
            obstacleBounds[i][1].setPosition(sf::Vector2f (rectObstacles[i].getOrigin().x,rectObstacles[i].getOrigin().y- rectObstacles[i].getSize().y/2));
            obstacleBounds[i][1].setPosition(rectObstacles[i].getPosition());
            //RIGHT BOUND
            obstacleBounds[i][2].setSize({1,rectObstacles[i].getSize().y });
            obstacleBounds[i][2].setPosition(sf::Vector2f (rectObstacles[i].getOrigin().x+rectObstacles[i].getSize().x/2,rectObstacles[i].getOrigin().y ));
            obstacleBounds[i][2].setPosition(rectObstacles[i].getPosition());
            //DOWN BOUND
            obstacleBounds[i][3].setSize({rectObstacles[i].getSize().x,1});
            obstacleBounds[i][3].setPosition(sf::Vector2f (rectObstacles[i].getOrigin().x,rectObstacles[i].getOrigin().y+ rectObstacles[i].getSize().y/2));
            obstacleBounds[i][3].setPosition(rectObstacles[i].getPosition());

            inter = true;
        }

        inter = true;
        for (int i = 0; i<NPROBES; i++){
            while (inter){
                inter = false;
                probes[i].setPosition(window->getSize().x*2/3+rand() % window->getSize().x/3, rand() % window->getSize().y);
                //iterate over obstacles
                for (int j = 0; j <NOBSTACLES && !inter; j++) {
                    for (auto & obstacle : rectObstacles) {
                        if ((probes[i].getGlobalBounds().intersects(obstacle.getGlobalBounds()) ||
                                       probes[i].getGlobalBounds().intersects(startingArea.getGlobalBounds()) )) {
                            inter = true;
                        }
                    }
                    for (int j = 0; j<i; j++){
                        if(i != j && probes[i].getGlobalBounds().intersects(probes[j].getGlobalBounds()))
                        inter = true;
                    }
                }
            }
            inter = true;
        }

        //std::cout << "probe pos: "<<std::endl;
        for (auto &probe : probes){
            std::cout << probe.getPosition().x <<" "<< probe.getPosition().y<<std::endl;
            }
        rover.velocity= 0;
        rover.wheel.steeringAngle.value = 0;
    };

    void show(){

            window->draw(background);
            window->draw(startingArea);
            rover.show(window);


            for(int i=0 ;i<NOBSTACLES;i++){
                window->draw(rectObstacles[i]);
                /*
                for(int j=0;j<4;j++){
                    window->draw(obstacleBounds[i][j]);
                }
                 */
                if(i<NPROBES) window->draw(probes[i]);
            }

            if(gameOver) {
                music.setPitch(0.7);
                text.setString("GAME OVER \n" + codeMessage[gameStatus]+ "\n"+"(press restart to play again)");
                text.setFont(myFont);
                text.setCharacterSize(70);
                //text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
                text.setFillColor(sf::Color::White);
                //text.setPosition(window->getSize().x / 2, window->getSize().y / 2);
                text.setStyle(sf::Text::Bold);
                window->draw(loseBackground);
                window->draw(text);
            }

            if(win){
                text.setString("VICTORY!");
                text.setFont(myFont);
                text.setCharacterSize(70);
                //text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
                text.setFillColor(sf::Color::White);
                //text.setPosition(window->getSize().x / 2, window->getSize().y / 2);
                text.setStyle(sf::Text::Bold);
                window->draw(winBackground);
                window->draw(text);
            }
    };

    void sensors(){
        distR=-1;
        distL=-1;
        distU=-1;
        distD=-1;
        for(int i=0;i<NOBSTACLES;i++){
            if(rover.sensorR.getGlobalBounds().intersects(rectObstacles[i].getGlobalBounds())){
                std::cout << "sensorR has distance "<<findDistance(rover.sensorR,findBound(rover.sensorR, i),i)<<std::endl;
                distR = findDistance(rover.sensorR,findBound(rover.sensorR, i),i);

            }
            if(rover.sensorL.getGlobalBounds().intersects(rectObstacles[i].getGlobalBounds())){
                std::cout << "sensorL has distance "<<findDistance(rover.sensorL,findBound(rover.sensorL, i),i)<<std::endl;
                distL = findDistance(rover.sensorL,findBound(rover.sensorL, i),i);
            }
            if(rover.sensorU.getGlobalBounds().intersects(rectObstacles[i].getGlobalBounds())){
                std::cout << "sensorD has distance "<<findDistance(rover.sensorU,findBound(rover.sensorU, i),i)<<std::endl;
                distU = findDistance(rover.sensorU,findBound(rover.sensorU, i),i);
            }
            if(rover.sensorD.getGlobalBounds().intersects(rectObstacles[i].getGlobalBounds())){
                std::cout << "sensorD has distance "<<findDistance(rover.sensorD,findBound(rover.sensorD, i),i)<<std::endl;
                distD = findDistance(rover.sensorD,findBound(rover.sensorD, i),i);

            }
        }
    }

private:

    int findBound (sf::RectangleShape &sensor, int i ){
        int j=-1;
        bool exit = false;
        while(!exit && j<4) {
            j++;
            if(sensor.getGlobalBounds().intersects(obstacleBounds[i][j].getGlobalBounds())) exit = true;
            //std::cout << "i j "<<i<<" " <<j<<std::endl;
        }
        return j;
    }

    float findDistance(sf::RectangleShape & sensor, int bound,int i){
        /*
         * sensor
         * rover:
         * angle
         */
        obstacleBounds[i][bound];
        float a,d;
        Angle angle;


        if(& sensor == &rover.sensorD){
            angle = -rover.angle.value+ 180;
        } else if(& sensor == &rover.sensorL){
            angle = -rover.angle.value- 90;
        }else if (& sensor == &rover.sensorR){
            angle = -rover.angle.value+90;
        } else if (& sensor == &rover.sensorU){
            angle = -rover.angle.value;
        }

        angle.value = -rover.angle.value;
        if(bound == LEFT){
            a= obstacleBounds[i][bound].getGlobalBounds().left - rover.getBody().getPosition().x;
            d = (sin(angle.toRad()) != 0 ? a/abs(sin(angle.toRad())) : a);
        } else if (bound ==RIGHT){
            a= obstacleBounds[i][bound].getGlobalBounds().left - rover.getBody().getPosition().x;
            d= (sin(angle.toRad()) != 0 ? a/abs(sin(angle.toRad())) : a);
        }else if (bound == UP){
            a= obstacleBounds[i][bound].getGlobalBounds().top - rover.getBody().getPosition().y;
            d= (cos(angle.toRad()) != 0 ? a/abs(cos(angle.toRad())) : a);
        } else if (bound ==DOWN) {
            a = obstacleBounds[i][bound].getGlobalBounds().top - rover.getBody().getPosition().y;
            d= (cos(angle.toRad()) != 0 ? a/abs(cos(angle.toRad())) : a);
        }

        if(& sensor == &rover.sensorD || & sensor == &rover.sensorU ){
            d -= rover.getBody().getGlobalBounds().height/2;
        } else if(& sensor == &rover.sensorL || & sensor == &rover.sensorR){
            d -= rover.getBody().getGlobalBounds().width/2;
        } else{
            throw std::runtime_error("NON FUNZIONA");
        }

        return abs(d/PIXELPERMETER);
    }

    enum BOUNDS{LEFT, UP, RIGHT, DOWN};
    enum CODE{OK, COLLISION, BAD_ANGLE, BAD_VELOCITY, WIN, CODE_MAX};

    std::vector<std::string> codeMessage = {
        "ok",
        "The rover fatally collided with an obstacle.\n\n",
        "The rover received a bad steering angle: \nits mobility system is now compromised\n\n",
        "The rover received a high velocity: \nthe motors are compromised\n\n",
        "Win"
    };

    void setCallbacks(bool con){
        if(con){
            /*
             *VR/rover/control/velamount (float)
            * VR/rover/control/velangle (float)
             */
            client.subscribe("VR/rover/control/velamount", [this](mqtt::const_message_ptr msg){
                std::cout << "received vel: " << msg->get_payload_str()<<std::endl;
                rover.velocity= std::stof(msg->get_payload_str());

            });
            client.subscribe("VR/rover/control/velangle", [this](mqtt::const_message_ptr msg){
                std::cout << "received angle: " << msg->get_payload_str()<<std::endl;
                rover.wheel.steeringAngle = std::stof(msg->get_payload_str());
            });
        }
    };

    Rover rover;
    sf::RenderWindow *window;
    bool gameOver;
    std::vector<sf::RectangleShape > rectObstacles;
    std::vector<std::vector<sf::RectangleShape>> obstacleBounds;
    sf::RectangleShape startingArea;
    bool restart;
    sf::Font myFont;
    sf::Text text;
    sf::Texture backgroundTexture;
    sf::Texture obstacleTexture;

    sf::Sprite background;
    sf::RectangleShape winBackground;
    sf::RectangleShape loseBackground;
    sf::SoundBuffer musicBuffer;
    sf::SoundBuffer probeBuffer;
    sf::Clock clock;
    sf::Sound music;
    sf::Sound probeSound;
    std::vector<sf::RectangleShape> probes;
    int probesTaken;
    bool win;
    float audioVolume;
    MqttClient client;
    float distU, distD, distR, distL;
    char brokerIp[100];
    int gameStatus;

};
#endif
