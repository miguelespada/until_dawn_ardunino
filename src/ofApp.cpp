#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofBackground(0);
    ofSetBackgroundAuto(false);
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    //connectArduino();
    isArduinoConnected = false;
    
    sender.setup(HOST_0, PORT_0);
    receiver.setup(PORT_1);
    serial.listDevices();
}

//--------------------------------------------------------------
void ofApp::update(){
    // arduino hotplug
    // check every ~2s
    if(ofGetFrameNum() % 120 == 0){
        int port = isArduinoPortFound();
        if(port>=0){
            if(isArduinoConnected){
                //cout << "Arduino is connected" << endl;
            }else{
                cout << "Trying to connect..." << endl;
                serial.close();
                isArduinoConnected = serial.setup(serial.getDeviceList()[port].getDeviceName(), ARDUINO_BAUD_RATE);
            }
        }else{
            cout << "Arduino port not present..." << endl;
            isArduinoConnected = false;
        }
    }
    
    
    // abort if arduino is not connected
    if(!isArduinoConnected)
        return;
    
    
    bool bTemperature = false;
    bool bHeart = false;
    bool bGalvanicVoltage = false;
    bool bConductance = false;
    
    if(getSerialString()){
        // String available for parsing
        //cout << buffer << endl;
        
        // parse string
        string str(buffer);
        vector<string> splitString = ofSplitString(str, ",");
        
        // iterate the receibed values
        if (splitString.size() == 4) {
            
            for (int i=0; i<splitString.size(); i++) {
                sensorValues[i] = ofToInt(splitString[i]);            }
            
            ofxOscMessage m;
            m.setAddress("/data");
            m.addIntArg(sensorValues[SENSOR_TEMPERATURE]);
            m.addIntArg(sensorValues[SENSOR_CONDUCTANCE]);
            m.addIntArg(sensorValues[SENSOR_VOLTAGE]);
            m.addIntArg(sensorValues[SENSOR_HEARTBEAT]);
            sender.sendMessage(m);
        }
        
        // request new data from arduino
        sendArduinoDataRequest();
    }
    
    // request new data from arduino again
    // if we have been stuck without data for too long
    if (ofGetElapsedTimef() - timer > IN_DATA_TIMEOUT) {
        sendArduinoDataRequest();
    }
    
    // ---
    // arduino pump control
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        // check for mouse moved message
        if(m.getAddress() == "/pump"){
            // both the arguments are int32's
            int val = m.getArgAsInt32(0);
            
            cout << val << endl;
            
            if(val == 0){
                serial.writeByte('b');
            } else if(val == 1){
                serial.writeByte('c');
            } else if(val == 2){
                serial.writeByte('d');
            }
        }
    }
}

//--------------------------------------------------------------
bool ofApp::getSerialString(){
    startChar = '[';
    endChar = ']';
    bufferIndex = 0;
    storeString = false;                   // flag to put the data in our buffer
    
    while(serial.available() > 0){        
        char incomingbyte = serial.readByte();
        
        if(storeString){
            if(bufferIndex == DATABUFFERSIZE){
                //cout << "Oops, index is pointing to an array element outside our buffer" << endl;
                bufferIndex = 0;
                break;
            }
            
            if(incomingbyte == endChar){
                //cout << "data string is complete" << endl;
                buffer[bufferIndex] = 0; //null terminate the C string
                return true;
            }else{
                buffer[bufferIndex++] = incomingbyte;
                buffer[bufferIndex] = 0; //null terminate the C string
            }
        }
        
        // doing the check here will drop the startChar from the buffer
        if(incomingbyte == startChar){
            bufferIndex = 0;
            storeString = true;
        }
    }
    
    //We've read in all the available Serial data, and don't have a valid string yet, so return false
    return false;
}

//--------------------------------------------------------------
int ofApp::isArduinoPortFound(){
    ofSerial serial;
    vector <ofSerialDeviceInfo> deviceList =  serial.getDeviceList();
    for(int i=0; i<deviceList.size(); i++){
        if(deviceList[i].getDeviceName().find(ARDUINO_PORT_NAME) != std::string::npos){
            return i;
        }
    }
    cout << "arduino port not found" << endl;
    return -1;
}

void ofApp::sendArduinoDataRequest(){
    serial.writeByte('a');
    timer = ofGetElapsedTimef();
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(50);
    if(isArduinoConnected)
        ofSetColor(0, 255, 0);
    else
        ofSetColor(255, 0, 0);
    ofEllipse(50, 50, 20, 20);
    
    int x = ofGetFrameNum() % ofGetWidth();
    int y = ofMap(sensorValues[SENSOR_HEARTBEAT], 100, 600, 0, 200);
    ofLine(x, 200, x, 200+y);
    
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
    if(isArduinoConnected){
        serial.flush();
        serial.writeByte('0');
    }
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
