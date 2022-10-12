#ifndef RECRUITMENT_VIRTUALROVER_ROVER_H
#define RECRUITMENT_VIRTUALROVER_ROVER_H
#include "../lib/Wheel.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <utility>
#include <vector>
#include <iostream>
#include <cstdlib>

#define TEXTURE_PATH "./assets/roverTexture.png"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class Rover{
public:
    float velocity;
    Angle angle;
    Wheel wheel;
    sf::RectangleShape sensorR, sensorL, sensorD, sensorU;

    /**
     * Constructor class
     * @param initialPos x and y coordinates of starting position. Rover will be initied at this position
     * @param bodySize size in pixels of the rover (x, y)
     */
    Rover(std::vector<float> initialPos, sf::Vector2f bodySize):
    body(bodySize),
    head(sf::Vector2f(bodySize.x/5,bodySize.y/5)),
    position(initialPos),
    angle(90),
    velocity(0),
    center({body.getSize().x/2,body.getSize().y/2}),
    wheel(body, angle, initialPos),
    steering_temp(angle),
    theta(0),roverTexture(),
    sensorR(), sensorL(), sensorU(), sensorD(){

        roverTexture.loadFromFile(TEXTURE_PATH);
        roverTexture.setSmooth(true);
        body.setTexture(&roverTexture);

        //set body origin to its center
        body.setOrigin(center[0], center[1]);
        //set body position to its starting position.
        body.setPosition(position[0], position[1]);
        //rotate body with starting rotation.
        body.setRotation(angle.value);

        //set origin to center of rover (relative to head!)
        head.setOrigin(head.getSize().x/2, -body.getSize().y/2-head.getSize().y/2);
        //set position -> translate the center to center of rover.
        head.setPosition(position[0],position[1]);

        head.setFillColor(sf::Color::Red);
        //initialize wheel positions
        wheel.init();

        sensorR.setSize(sf::Vector2f (5*body.getSize().x,2));
        sensorD.setSize(sf::Vector2f (2,body.getSize().y*3));
        sensorU.setSize(sensorD.getSize());
        sensorL.setSize(sensorR.getSize());

        sensorR.setOrigin(sf::Vector2f (sensorR.getSize().x + body.getSize().x/2,sensorR.getSize().y/2));
        sensorD.setOrigin(sf::Vector2f (sensorD.getSize().x/2 ,+sensorD.getSize().y + body.getSize().y/2));
        sensorU.setOrigin(sensorU.getSize().x/2,- body.getSize().y/2);
        sensorL.setOrigin(- body.getSize().x/2,sensorL.getSize().y/2);

        sensorL.setFillColor(sf::Color(0, 100,250,100));
        sensorU.setFillColor(sf::Color(0, 100,250,100));
        sensorD.setFillColor(sf::Color(0, 100,250,100));
        sensorR.setFillColor(sf::Color(0, 100,250,100));

    };

    void move(){
        onAngleChanged();
        float r = -((body.getSize().y/2)*(1.0/tan(wheel.steeringAngle.toRad()))+(body.getSize().x/2));

        //if(r < 0.001f) r= 0.001f;
        head.setOrigin(head.getSize().x/2, -body.getSize().y/2-head.getSize().y/2);

        theta.value += velocity * (180/M_PI) /r ;
        angle.value = theta.value;

        position[0] +=  velocity*(sin(-theta.toRad()));
        position[1] +=  velocity*(cos(theta.toRad()));

        body.setPosition(position[0], position[1]);
        body.setRotation(angle.value);
        //head.move(velocity*sin(-angle.toRad()), velocity*cos(angle.toRad()));

        sensorL.setPosition(position[0], position[1]);
        sensorU.setPosition(position[0], position[1]);
        sensorD.setPosition(position[0], position[1]);
        sensorR.setPosition(position[0], position[1]);

        sensorL.setRotation(angle.value);
        sensorU.setRotation(angle.value);
        sensorD.setRotation(angle.value);
        sensorR.setRotation(angle.value);


        //head.setRotation(angle.value);
        wheel.setAngle(angle.value);
        wheel.move();

        steering_temp.value = wheel.steeringAngle.value;

    };

    void show(sf::RenderWindow *window){
        //window->draw(head);
        //window->draw(sensorD);
        //window->draw(sensorU);
        //window->draw(sensorR);
        //window->draw(sensorL);
        window->draw(body);
        wheel.show(window);
    }


    void onAngleChanged(){
        /*
        if(steering_temp.value =! wheel.steeringAngle.value){
        theta = 0;
        }
         */
    };

    void setPosition(const std::vector<float> &position) {
        Rover::position = position;
    }

    const sf::RectangleShape &getBody() const {
        return body;
    }

    const Wheel &getWheel() const {
        return wheel;
    }

private:
    sf::RectangleShape body;
    sf::RectangleShape head;
    std::vector<float> center;
    std::vector<float> position;
    std::vector<float> speed;
    Angle steering_temp;
    Angle theta;
    sf::Texture roverTexture;

};
#endif
