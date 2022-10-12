#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads
#include "imgui_internal.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <ctime>

#include <iostream>
#include "../lib/Game.hpp"



int main() {
    srand ( time(NULL) );
    //create window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Virtual Rover - DIANA Recruitment");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    //setup stuff
    Game myMap(&window);

    //graphical loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();

        //------------------//
        myMap.showMenu();
        myMap.update();
        myMap.checkValues();
        myMap.feedback();
        myMap.show();
        //------------------//

        ImGui::SFML::Render(window);
        window.display();

    }


}
