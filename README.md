# Mars Mission - Solution
The program I coded for the assignment is written in C++ and it uses the Eclipse Paho C Client library for MQTT. The code is commented, but this README explains the key concepts with more detail.

## User Commands
The assignment requires the program to receive user commands in real time.

I chose the following syntax for such commands:
- `V <value>` for the velocity,
- `A <value>` for the steering angle.

Commands are case-insensitive ('a' and 'A' are equivalent, 'v' and 'V' are equivalent). Whitespaces between members are ignored. The numbers must be written in a correct format.

The following example commands have a valid syntax:

- `a 10`				sets the steering angle to 10°
- `V 1.1`				sets the velocity to 1.1 m/s
- `A -20.9`				sets the steering angle to -20.9°
- `v -1`				sets the velocity to -1 m/s

I took advantages of the power of **regular expressions** to handle such syntax in a more elegant way.

(https://github.com/sabinogb/DIANA-Recruitment-2022-23/blob/Mars_Mission/Command.cpp#L20)

`\\s*[avAV]\\s*[+-]?[0-9]+([.][0-9]+)?\\s*`

![Regexp](https://github.com/sabinogb/DIANA-Recruitment-2022-23/blob/Mars_Mission/images/regexp.jpg)



### Exceptions handling
If the pattern described above is matched, then the number has been written in a correct form.

However, it is not guaranteed that such number could be represented by a float variable.

For example, attempting to convert the strings `"0.000000000000000000000000000000000000002"`  or `"1000000000000000000000000000000000000000"` to float values (using **std::stof()**), results in a **std::out_of_range** exception (as *underflow* would occur for the first number and *overflow* would occur for the second number).

Therefore, I enclose the call to **std::stof()** in a **try** block:

(https://github.com/sabinogb/DIANA-Recruitment-2022-23/blob/Mars_Mission/Command.cpp#L66)



After this check, the program can proceed and check if such number is included between the given intervals.

## Building the program
I included a simple *build.sh* file in the project, in case you'd want to reproduce the executable in a Windows environment using MSYS2 and MinGW64. Make sure to use a C++11 compliant version of the compiler, as I use the `<regex>` header from the standard library.


# Notes for developers at DIANA
The assignment requires the valid values for the steering angle being included between the interval [-42°, +42°]. (https://github.com/team-diana/DIANA-Recruitment-2022-23/tree/Mars_Mission#software-requirements):
> -42° < steering angle < +42° (note: the steering angle must be defined in euler angles);



However, reading the source code of *Recruitment_VirtualRover.exe* I noticed that the game is over if the values provided for the steering angle aren't included between the inverval [-40°, +40°]:
(https://github.com/team-diana/DIANA-Recruitment-2022-23/blob/Mars_Mission/lib/Game.hpp#L187):


Consequently, I decided to code my program in such a way that the only acceptables values for the steering angle would be the ones included between the interval [-40°, +40°], which constitutes a stricter requirement, but at the same time such stricter condition prevents the simulation from failing in case the user inserts values included between [-42°, -40°) or (+40°, +42°]:

(https://github.com/sabinogb/DIANA-Recruitment-2022-23/blob/Mars_Mission/Command.cpp#L85)



# Screenshots

See the directory `/images`.

![Screenshot 1](https://github.com/sabinogb/DIANA-Recruitment-2022-23/blob/Mars_Mission/images/Screenshot1.JPG)


![Screenshot 2](https://github.com/sabinogb/DIANA-Recruitment-2022-23/blob/Mars_Mission/images/Screenshot2.JPG)

![Screenshot 3](https://github.com/sabinogb/DIANA-Recruitment-2022-23/blob/Mars_Mission/images/Screenshot3.JPG)
