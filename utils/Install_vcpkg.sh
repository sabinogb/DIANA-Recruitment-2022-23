#!/bin/bash

#Install some third party library
sudo apt install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev autoconf libtool
sudo apt-get install libx11-dev libxrandr-dev libxi-dev libudev-dev libgl1-mesa-dev
cd ..
#Look for vcpkg if it exists
if [ -d "vcpkg/*" ]
then
  echo "Install Vcpkg : vcpkg folder already existing. To install vcpkg using this script, delete ./vcpkg folder."
  exit 1
else
  #Install dependecies
  sudo apt-get install  -y curl zip unzip tar
  #Clone VCPKG
   echo "Installing VCPKG..."
  git clone https://github.com/Microsoft/vcpkg.git
  # build vcpkg
  ./vcpkg/bootstrap-vcpkg.sh
  # integrate in local builds
  cd vcpkg
  ./vcpkg integrate install
fi
exit 0
