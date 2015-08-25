#include "ofApp.h"
#include "ofxJSON.h"
#include "ofxOsc.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    font.loadFont("Akkurat-Mono.ttf", 20);
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    serial.setup("tty.usbmodem1451", 115200);
    
    sender.setup(HOST_0, PORT_0);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    bool bTemperature = false;
    bool bHeart = false;
    bool bGalvanicVoltage = false;
    bool bConductance = false;
    int value = 0;
        int byte;
    
        byte = serial.readByte();
    
        while( byte != int('[')){
            byte = serial.readByte();
            if ( byte == OF_SERIAL_NO_DATA || byte == OF_SERIAL_ERROR )
                return;
        };
    
        byte = serial.readByte();
        while( byte != int(']')){
            if ( byte != OF_SERIAL_NO_DATA && byte != OF_SERIAL_ERROR ){
                if (byte == int('T')){
                    bTemperature = true;
                    value = 0;
                }
                else if( byte == int('V')){
                    bGalvanicVoltage = true;
                    value = 0;
                }
                else if( byte == int('C')){
                    bConductance = true;
                    value = 0;
                }
                else if( byte == int('S')){
                    bHeart = true;
                    value = 0;
                }
                else{
                    value *= 10;
                    value += int ( char(byte) - '0');
                }
            }
            else{
                ofSleepMillis(20);
            
            }
            byte = serial.readByte();
        };
    
    
    ofxJSONElement response;
    
    if(bTemperature){
        ofLogNotice() << "Temperature: " << value;
        response.open("http://localhost:3000/temperature.json?v=" + ofToString(value));
    }
    if(bGalvanicVoltage){
        ofLogNotice() << "Galvanic Voltage: " << value;
        response.open("http://localhost:3000/galvanicVoltage?v=" + ofToString(value));
    }

    if(bConductance){
        ofLogNotice() << "Conductance: " << value;
        response.open("http://localhost:3000/conductance.json?v=" + ofToString(value));
    }
    
    if(bHeart){
        ofxOscMessage m;
        m.setAddress("/heart");
        m.addIntArg(value);
        sender.sendMessage(m);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
