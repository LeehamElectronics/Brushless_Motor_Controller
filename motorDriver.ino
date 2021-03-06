/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                     PLEASE READ THE FOLLOWING!                                    *
 * This open source software was written by Liam Price 2020 and is FREE to use...                    *
 * The purpose of this software is to compile and run on an Arduino based IoT device, like an ESP32  *
 *                                                                                                   *
 * This is used to control an 'Exploration Robot' that I designed to be controlled from a computer   *
 * steering wheel joystick, however you can use the code for any robot project, if you want to see   *
 * the Exploration Robot I designed this for software, visit my GitHub Repo for it:                  *
 * https://github.com/LeehamElectronics/Exploration-Robot-Arduino                                    *
 *                                                                                                   *
 * And here is the open source Python Control Panel I made for it:                                   *
 * https://github.com/LeehamElectronics/Exploration-Robot-Control-Panel                              *
 *                                                                                                   *
 * If you'd like to support my open source work, buy me a coffee:                                    *
 * https://www.paypal.com/paypalme/liamproice/                                                       *
 *                                                                                                   *
 * And if you need help, feel free to email me at liamisprice@gmail.com                              *
 * Thanks!                                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

 /*
 ██╗░░░░░███████╗███████╗██╗░░██╗░█████╗░███╗░░░███╗██╗░██████╗      ░██████╗░█████╗░███████╗████████╗░██╗░░░░░░░██╗░█████╗░██████╗░███████╗
 ██║░░░░░██╔════╝██╔════╝██║░░██║██╔══██╗████╗░████║╚█║██╔════╝      ██╔════╝██╔══██╗██╔════╝╚══██╔══╝░██║░░██╗░░██║██╔══██╗██╔══██╗██╔════╝
 ██║░░░░░█████╗░░█████╗░░███████║███████║██╔████╔██║░╚╝╚█████╗░      ╚█████╗░██║░░██║█████╗░░░░░██║░░░░╚██╗████╗██╔╝███████║██████╔╝█████╗░░
 ██║░░░░░██╔══╝░░██╔══╝░░██╔══██║██╔══██║██║╚██╔╝██║░░░░╚═══██╗      ░╚═══██╗██║░░██║██╔══╝░░░░░██║░░░░░████╔═████║░██╔══██║██╔══██╗██╔══╝░░
 ███████╗███████╗███████╗██║░░██║██║░░██║██║░╚═╝░██║░░░██████╔╝      ██████╔╝╚█████╔╝██║░░░░░░░░██║░░░░░╚██╔╝░╚██╔╝░██║░░██║██║░░██║███████╗
 ╚══════╝╚══════╝╚══════╝╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░░░░╚═╝░░░╚═════╝░     ╚═════╝░░╚════╝░╚═╝░░░░░░░░╚═╝░░░░░░╚═╝░░░╚═╝░░╚═╝░░╚═╝╚═╝░░╚═╝╚══════╝
 */

/*
   Include Libaries:
*/
#include <SoftwareSerial.h>
SoftwareSerial s(6, 5);  //serial pins for the serial libary are specified here, rx/tx
/*
   Setup Variables
*/
const int reedInput = 9;
//const int relayOut = 8;
int reedSwitchState = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)
int previous = 0;
float speedCounter;
boolean runMotor = false;
int data; //variable to store incomming data from wifi board

void setup() {
    // pinMode(relayOut, OUTPUT);
    pinMode(reedInput, INPUT_PULLUP);
    s.begin(22800); //initialize serial between the 2 boards
    Serial.begin(19200);   // Initiate a serial communication
    Serial.println("Setup completed");
    // digitalWrite(relayOut, LOW);
    // delay(500);
    // digitalWrite(relayOut, HIGH);
    // delay(500);
    // digitalWrite(relayOut, LOW);
    //delay(500);
    // digitalWrite(relayOut, HIGH);
    //  Serial.println("Test000 completed");
    /*
      while (runMotor == false) {
        delay(300);
        if (s.available() > 0) //if data in serial is greater than 0...
        {
          data = s.read(); //then write that into the "data" variable for furthur use
          if (data == '3') {
            runMotor = true;
          }
        }
      }
    */

}

void loop() {
    unsigned long currentMillis = millis();
    reedSwitchState = digitalRead(reedInput);

    if (currentMillis - previousMillis >= interval) {
        //send value to another arduino board since multithreading doesnt work
        s.write(speedCounter);
        Serial.println(speedCounter);
        previousMillis = currentMillis;
        speedCounter = 0;
    }


    if (reedSwitchState == LOW) {
        if (previous == 1) {
            delay(10);
            previous = 0;
            //  Serial.println("low");
            speedCounter = speedCounter + 1;

        }
    }

    if (reedSwitchState != LOW) {
        if (previous == 0) {
            previous = 1;
            //Serial.println("high");
        }
    }

    // else (ree) {
    // previous = 1;
    // }




}