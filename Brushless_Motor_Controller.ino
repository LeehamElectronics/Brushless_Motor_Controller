/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                     PLEASE READ THE FOLLOWING!                                    *
 * This open source software was written by Liam Price 2019 and is FREE to use...                    *
 * The purpose of this software is to compile and run on an Arduino based IoT device, like an ESP32  *
 *                                                                                                   *
 * This is used to control a brushless motor that I modeled with a fidget spinner. Sounds pretty     *
 * stupid, and it is. It was pretty good for my Year 11 VCE Physics practial investigation though.   *
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

#include <SoftwareSerial.h>
SoftwareSerial s(3, D5);  //serial pins for the serial libary are specified here, rx/tx
#include <ESP8266WiFi.h> //just the basic libary for coding esp8266 Wi-Fi boards like the nodemcu
#include <FirebaseArduino.h> //live database libary so we can read / write to the Internet of Thing!

//Firebase Definitions:
//setup firebase variables to connect to my firebase project online
#define FIREBASE_HOST "REDACTED"
#define FIREBASE_AUTH "REDACTED"
#define WIFI_SSID "REDACTED"
#define WIFI_PASSWORD "REDACTED"

float firebaseActivationVar = 0; //for firebase
float previous = 0; //for firebase
boolean runMotor = false;
float data; //variable to store incomming data from arduino nano
float previous2 = 0; //for firebase
float maxSpeed = 0;
float acceleration = 0;
float maxAcceleration = 0;
float previousSpeed = 0;

void setup()
{
    Serial.begin(9600);   // Initiate a serial communication
    s.begin(22800); //initialize serial between the 2 boards
    Serial.println("turned on");
    // connect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());

    //connect to firebase server and specift the variables we want to read and write to
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Serial.println(Firebase.getFloat("FS/firebaseActivationVar")); //this variable in the database stores the movement variable (left right forward back)
    Firebase.setFloat("/object", 0);
    Firebase.setFloat("/maxSpeed", 0);
    Firebase.setFloat("/maxAcceleration", 0);

    while (runMotor == false) {
        firebaseActivationVar = Firebase.getFloat("FS/firebaseActivationVar"); //firebase variable
        if (firebaseActivationVar == 3) {
            runMotor = true;
            Serial.print("Motor On");
            //tell other arduino to start motor and sending data  
            s.write('3');
        }
    }
}

void loop()
{
    previousSpeed = data;
    //read from other arduino board, if input has changed send that to firebas
    if (s.available() > 0) //if data in serial is greater than 0...
    {
        data = s.read(); //then write that into the "data" variable for furthur use
        Firebase.setFloat("/object", data);
    }
    if (data > maxSpeed) {
        maxSpeed = data;
        Firebase.setFloat("/maxSpeed", maxSpeed);
    }
    acceleration = data - previousSpeed;

    if (acceleration > maxAcceleration) {
        maxAcceleration = acceleration;
        Firebase.setFloat("/maxAcceleration", maxAcceleration);
    }
}
