

//We use this to initialize WiFi and the webconfig here, but you can manually initialize WLAN yourself too.
#include <shz_config.h>

//From arduino library
#include <shizonet.h>

//Initialize a receiving shizonet end node
shznet_client client;

//Used for webconfig
shz_config conf;

//debug output
bool          debug_output = false;

template<class T> void sprint(T prnt)
{
  if(!debug_output) return;
  Serial.print(prnt);
}
template<class T> void sprintln(T prnt)
{
  if(!debug_output) return;
  Serial.println(prnt);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //Look at config assist for how to use this...
  static const char* VARIABLES_DEF_YAML PROGMEM = R"~(
  )~";

  //Initializes WLAN eth etc...
  conf.init("ESP32LED", VARIABLES_DEF_YAML);


  //SHIZONET SETUP

  //Initialize network node name
  client.init("ESP32LED");
  
  //Do not show LEDS on this callback, use it to set data
  client.setArtDmxCallback([](uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, shznet_ip remoteIP)
  {
    //Receive a DMX universe frame and do something with it
  });

  //Show LEDS, update frame whatever
  client.setArtFrameCallback([]()
  {
    //This function gets called after 
    //ALL currently streamed universes have been received
  });


  //Initializ rest of webconfig stuff...
  conf.on_change([](String& key)
  {
    if(key == "debug")
    {
      debug_output = conf.get_int("debug");
      sprintln("debug output changed.");
    }
  });

  debug_output = conf.get_int("debug");

  Serial.print("Mem: ");
  Serial.println(ESP.getFreeHeap());
}

void loop() {
  client.update();
  conf.update();
}
