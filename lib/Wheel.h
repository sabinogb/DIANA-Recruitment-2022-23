#ifndef RECRUITMENT_VIRTUALROVER_WHEEL_H
#define RECRUITMENT_VIRTUALROVER_WHEEL_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <utility>
#include <vector>
#include <iostream>
#include <cstdlib>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * utility struct to store and manipulate angle values.
 */
struct Angle{
    float value;

    /**
    * @brief constructor, inits the value to val
     * @param val the value to set the angle. If greather than -180/180, will be normalized
    */
    Angle(long val):value(val){
        value = val;
        //normalize();
    };

    /**
     * @brief default constructor
     */

    Angle():value(0){};
    /**
     * @brief operator = overload,
     */
    Angle & operator=( const double &other) {
        if (this->value != other){
            this->value = other;
            //normalize();
        }
        return *this;
    };

    /**
     * Normalize the angle between -180 and 180, and set it not to be exactly a multiple of 90
     */
    void normalize(){
        while (value > 180)value -= 360;
        while (value < -180) value += 360;

        if(value == 0 || value == -90 || value == 90 ){
            value += 0.000001;
        }
    };
    /**
     * returns the value converted to radians.
     * @return value in radians
     */
    double toRad(){
        return value*M_PI/180;
    }
};
/**
 * Graphical class for wheel rendering and rotation computation
 */
class Wheel {
public:
    Angle steeringAngle;

    Wheel(sf::RectangleShape &bod, Angle &an, std::vector<float> & p):
    body(bod),
    angle(an),
    steeringAngle(0.00001),
    position(p){
        wheels.resize(6);
        for (auto & wheel : wheels){
            wheel.setSize(sf::Vector2f (20,20));
            wheel.setFillColor(sf::Color::Yellow);
        }
    }

    double getAngle() const {
        return angle.value;
    }
    /**
     * Setter method for wheel angle.
     * @param an angle, in radians, -pi/4 < an < pi/4
     */
    void setAngle(double an){
        angle = an;
    }

    void move(){
        int d =sqrt( (body.getSize().x/2+wheels[0].getSize().x/2)*(body.getSize().x/2+wheels[0].getSize().x/2)+(body.getSize().y/2+wheels[0].getSize().y/2)*(body.getSize().y/2+wheels[0].getSize().y/2));
        wheels[0].setPosition(body.getPosition().x+d*sin(-(angle.toRad()-M_PI/6)),body.getPosition().y+d*cos(angle.toRad()-M_PI/6));
        wheels[1].setPosition(body.getPosition().x+d*sin(-(angle.toRad()+M_PI/6)),body.getPosition().y+d*cos((angle.toRad()+M_PI/6)));
        wheels[2].setPosition(body.getPosition().x+d*sin(-(angle.toRad()-M_PI*5/6)),body.getPosition().y+d*cos((angle.toRad()-M_PI*5/6)));
        wheels[3].setPosition(body.getPosition().x+d*sin(-(angle.toRad()+M_PI*5/6)),body.getPosition().y+d*cos((angle.toRad()+M_PI*5/6)));
        d = sqrt((body.getSize().x/2+wheels[0].getSize().x/2)*(body.getSize().x/2+wheels[0].getSize().x/2));
        wheels[4].setPosition(body.getPosition().x+d*sin(-(angle.toRad()+M_PI/2)),body.getPosition().y+d*cos((angle.toRad()+M_PI/2)));
        wheels[5].setPosition(body.getPosition().x+d*sin(-(angle.toRad()-M_PI/2)),body.getPosition().y+d*cos((angle.toRad()-M_PI/2)));

        for(int i=0 ; i<6; i++){
            wheels[i].setRotation(i < 4 ?  (i==2 || i ==3 ? angle.value+steeringAngle.value : angle.value-steeringAngle.value)  : angle.value);
        }

    }

    void show(sf::RenderWindow *window){
        for (auto &wheel : wheels){
            window->draw(wheel);
        }
    }

    void init(){
        for (auto & wheel : wheels){
            wheel.setSize(sf::Vector2f(body.getSize().x/4, body.getSize().y/4));
            wheel.setOrigin(wheel.getSize().x/2,wheel.getSize().y/2);
        }

        wheels[0].setPosition(body.getPosition().x+body.getSize().x/2+wheels[0].getSize().x/2,body.getPosition().y+body.getSize().y/2+wheels[0].getSize().y/2 );
        wheels[1].setPosition(body.getPosition().x-body.getSize().x/2-wheels[0].getSize().x/2,body.getPosition().y+body.getSize().y/2+wheels[0].getSize().y/2 );
        wheels[2].setPosition(body.getPosition().x-body.getSize().x/2-wheels[0].getSize().x/2,body.getPosition().y-body.getSize().y/2-wheels[0].getSize().y/2 );
        wheels[3].setPosition(body.getPosition().x+body.getSize().x/2+wheels[0].getSize().x/2,body.getPosition().y-body.getSize().y/2-wheels[0].getSize().y/2  );
        wheels[4].setPosition(body.getPosition().x-body.getSize().x/2-wheels[0].getSize().x/2,body.getPosition().y );
        wheels[5].setPosition(body.getPosition().x+body.getSize().x/2+wheels[0].getSize().x/2,body.getPosition().y );

    };

    const std::vector<sf::RectangleShape> &getWheels() const {
        return wheels;
    }

private:
    std::vector<sf::RectangleShape> wheels;
    Angle angle;
    sf::RectangleShape &body;
    std::vector<float> position;

};


#endif //RECRUITMENT_VIRTUALROVER_WHEEL_H
