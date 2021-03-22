#include "main.h"

Controller master(ControllerId::master);

Motor   Da(5, true, Motor::gearset::green, Motor::encoderUnits::degrees, Logger::getDefaultLogger()), 
        Db(2, true, Motor::gearset::green, Motor::encoderUnits::degrees, Logger::getDefaultLogger()), 
        Dc(3, false, Motor::gearset::green, Motor::encoderUnits::degrees, Logger::getDefaultLogger()), 
        Dd(4, false, Motor::gearset::green, Motor::encoderUnits::degrees, Logger::getDefaultLogger()),
        intake1(13, false, Motor::gearset::green, Motor::encoderUnits::degrees, Logger::getDefaultLogger()),
        intake2(14, false, Motor::gearset::green, Motor::encoderUnits::degrees, Logger::getDefaultLogger()),
        intake3(15, false, Motor::gearset::blue, Motor::encoderUnits::degrees, Logger::getDefaultLogger());

int autonomousDefault = 1;
