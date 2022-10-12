# Mars Mission
<p align="center">
    <img src="img/game.png" width="400">
</p>

> After God knows how much time, ARDITO has finally reached Mars!

*This was what everyone was thinking at the Mission Control Center. Little did they know how difficult was the path ahead. From geologists thrilled to find water in craters to mechatronic engineers anxious to move their beloved rover, nobody noticed that things were not going as they wished.*

*Soon they realized that ARDITO did not land where expected; instead of the Gale Crater, the rover was in a region full of rocks!*

*Luckily for us, our engineers have thought of everything! In fact, ARDITO does not fear boulders and can face such a difficult challenge, thanks to our LMAO (**L**eading to **M**ars **A**nd **O**rbiting) remote controller.*

*This smart component of our mission is a Mars orbiter that helps us drive the rover manually when we encounter very challenging terrains. In particular, we can send MQTT messages from Earth to LMAO to set the velocity and steering angle of the rover. Then, LMAO automatically forwards them to ARDITO, which will eventually move as we desire!*

*However, sending MQTT messages is veeeeery dull, and we want you to help us automate this step. After all, we are cool guys who program rovers: we cannot be boring! :wink:*

*Do you want to make a difference?*

## Software Requirements

Your main purpose in this mission is to write a program that can send MQTT messages to LMAO. In particular, you can set the velocity and the steering angle of the rover by publishing messages on the topics:

- *VR/rover/control/velamout*: for the velocity;
- *VR/rover/control/velangle*: for the steering angle.

Both topics accept `float` values, with the following limits:
- -42° < steering angle < +42° (note: the steering angle must be defined in euler angles);
- -1.5 < velocity < +1.5 (velocity is expressed in m/s).

To make things funnier, we provide you with a `Recruitment_VirtualRover.exe` file that you can use to test your program; actually, it is a game! 

In this game, you can use your MQTT messages to drive the rover through the terrain, trying to reach the probes represented in green on the map. If you reach all of them without crashing into rocks, you've won :)

To provide feedback, LMAO uses also other topics to inform you about the mission status: 

- *VR/game/status*: an `active` or `lost` message is published when LMAO connects or disconnects, respectively;
- *VR/game/probes*: it tells you how many probes you have still to collect, when this number reaches 0, you've won;
- *VR/rover/feedback/velocity*: current velocity of the rover;
- *VR/rover/feedback/orientation*: current orientation of the rover (0°: the rover points downwards, the angle grows in clockwise order);
- *VR/rover/feedback/steeringAngle*: current steering angle of the rover;

You do not need to use them, but you can subscribe to them to understand what is happening.

If you win the game, feel free to insert a few screenshots proving your results in your repository in an `images/` folder. Winning the game is a **HUGE** improvement to your evaluation.

You can use whichever language you want!

*Hint 1*: you can find open-source libraries to use MQTT and use them (we suggest using [PAHO](https://www.google.com/search?q=PAHO+library&oq=PAHO+library&aqs=chrome..69i57j69i60.2392j0j1&sourceid=chrome&ie=UTF-8), available for every programming language)- you do not need to implement either the whole protocol or the MQTT broker from scratch! 

*Hint 2*: commenting on the code helps us understand what you do and makes us happier (which is very good for you!)

## Requested Output

Your program should be able to:
- send MQTT messages to the velocity and steering angles topics;
- receive user commands in real-time (i.e. the user defines the rover velocity at any time -> your program publishes the corresponding MQTT messages at the right topic);
- check that the user inputs respect the imposed limits and ignore them in case they don't;
- be as reactive as possible (the less a user must wait to provide input, the better)

*Final advice*: even if you do not manage to complete the program in time, send us what you have done so far - doing something is better than doing nothing!

## More Hints
- [here](docs/Build%20Instructions.md) you can find instructions if you need to build the executable (for example if you need to run it on Linux)
- [here](docs/Mqtt.md) you can find some more hints on how to connect to mqtt
- 
Good luck, and have fun! 
This is a unique opportunity to challenge yourself!
