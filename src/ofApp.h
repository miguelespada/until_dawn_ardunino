#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxJSON.h"

#define ARDUINO_BAUD_RATE 57600
#define ARDUINO_PORT_NAME "tty.usbmodem"

#define HOST_0 "192.168.1.42"
#define PORT_0 12350
#define PORT_1 12351


#define DATABUFFERSIZE 25   // needs to be big enough to hold the incomming messages

#define IN_DATA_TIMEOUT 2   // how long to wait until requesting more data from arduino

enum SensorType {
    SENSOR_TEMPERATURE = 0,
    SENSOR_CONDUCTANCE,
    SENSOR_VOLTAGE,
    SENSOR_HEARTBEAT
};


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofSerial serial;
    
    char buffer[DATABUFFERSIZE];
    int bufferIndex;
    char startChar;
    char endChar;
    bool storeString;
    
    bool getSerialString();
    
    // arduino serial hot plugging
    int isArduinoPortFound();
    bool isArduinoConnected;
    
    // request more data from arduino
    void sendArduinoDataRequest();
    float timer;
    
    
    int sensorValues[4];
    
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
};
