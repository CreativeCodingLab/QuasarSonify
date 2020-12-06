/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"


class OSCHandler : private OSCReceiver, private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>
{
public:

    OSCHandler()
    {
        // connect for listening
        if(connect(12234))
        {
            std::cout << "---- successful connection listening on port 6789" << std::endl;
            
            // set the receiver to listen to the following address:
            String listeningAddress("/juce");
            addListener(this, listeningAddress);
        }
        
        // connect for sending.
        oscSend.connect("127.0.0.1", 9876);
    };
    ~OSCHandler(){}
    
    
    void sendOSCMessageFloat(String address, float content)
    {
        oscSend.send(address, content);
    }
    
    void sendOSCMessageString(String address, String content)
    {
        oscSend.send(address, content);
    }

    
    
private:
    
    
    void oscMessageReceived (const OSCMessage& message) override
    {
        for(int i = 0; i <  message.size(); i++)
        {
            if(message[i].isString())
            {
                std::cout << "message received..." << message[i].getString() << std::endl;
            }
        }
    }
    
    OSCSender oscSend;
    
};
