#include "http_module.h"

/*====== Process configuration ==============*/
String processor(const String& var){
  /* Replaces "placeholder" in  html file with sensors values */
  /* accessors functions get_... are in sensors.ino file   */ 
  if(var == "TEMPERATURE"){
    return get_t();
    /* On aimerait écrire : return get_temperature(TempSensor);
     * mais c'est un exemple de ce qu'il ne faut surtout pas écrire ! 
     * yield + async => core dump ! 
     */
    //return get_temperature(TempSensor);
  }
  else if(var == "LIGHT"){
    return get_light(LightPin);
  }

  else if(var == "UPTIME") {
    return getUptime();
  }
  else if(var == "WHERE") {
    return LOCATION;
  }
  else if(var == "SSID") {
    return getSSID();
  }
  else if(var == "MAC") {
    return getMAC();
  }
  else if(var == "IP") {
    return getIP();
  }
  else if(var == "COOLER") {
    return get_cooler_str(COOL);
  }
  else if(var == "HEATER") {
    return get_heater_str(HEAT);
  }
  return String();
}


/**
 * sets up the http server and routes for use
 */
void setup_http_server() {
  /* 
   * Sets up AsyncWebServer and routes 
   */
  // doc => Serve files in directory "/" when request url starts with "/"
  // Request to the root or none existing files will try to server the default
  // file name "index.htm" if exists 
  // => premier param la route et second param le repertoire servi (dans le SPIFFS) 
  // Sert donc les fichiers css !  
  server.serveStatic("/", SPIFFS, "/").setTemplateProcessor(processor);  
  
  // Declaring root handler, and action to be taken when root is requested
  auto root_handler = server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        /* This handler will download index.html (stored as SPIFFS file) and will send it back */
        request->send(SPIFFS, "/index.html", String(), false, processor); 
        // cf "Respond with content coming from a File containing templates" section in manual !
        // https://github.com/me-no-dev/ESPAsyncWebServer
        // request->send_P(200, "text/html", page_html, processor); // if page_html was a string .   
  });
  
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    /* The most simple route => hope a response with temperature value */ 
    /* Exemple de ce qu'il ne faut surtout pas écrire car yield + async => core dump !*/
    //request->send_P(200, "text/plain", get_temperature(TempSensor).c_str());
    
    request->send_P(200, "text/plain", get_t().c_str());
  });

  server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request){
    /* The most simple route => hope a response with light value */ 
    request->send_P(200, "text/plain", get_light(LightPin).c_str());
  });

  // This route allows users to change thresholds values through GET params
  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request){
    /* A route with a side effect : this get request has a param and should     
     *  set a new light_threshold ... used for regulation !
     */
      if (request->hasArg("light_threshold")) { // request may have arguments
  Light_threshold = atoi(request->arg("light_threshold").c_str());
  request->send_P(200, "text/plain", "Threshold Set !");
      }
    });

  /**
   * FOR THE updating NODERED DASHBOARD manually :D
   */
  server.on("/esp", HTTP_GET, [](AsyncWebServerRequest *request){      
    request->send(200, "application/json", getJSONString_fromstatus(float(t),int(l)).c_str() );
  });
  
  server.on("/target", HTTP_POST, [](AsyncWebServerRequest *request){
    /* A route receiving a POST request with Internet coordinates 
     *  of the reporting target host.
     */
     Serial.println("Receive Request for a ""target"" route !"); 
        if (request->hasArg("ip") &&
        request->hasArg("port") &&
        request->hasArg("sp")) {
          target_ip = request->arg("ip");
          target_port = atoi(request->arg("port").c_str());
          target_sp = atoi(request->arg("sp").c_str());
        }
//        if(target_ip.trim().equals("")) {
//          target_ip == TARGET_IP;
//        }
//        if(target_port.trim().equals("")){
//          target_port = TARGET_PORT;
//        }
//        if(target_sp.trim().equals("")) {
//          target_sp = TARGET_REFRESH;
//        }
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    /**
     * build a json response depending on the passed in arguments 
     */
    server.on("/value", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Requesting build of status JSON..."); 
      AsyncJsonResponse * response = new AsyncJsonResponse();
      response->addHeader("Server","ESP Async Web Server");
      JsonObject& jsondoc = (JsonObject&) response->getRoot();
      
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(request->hasParam("temperature")){
          jsondoc["status"]["temperature"] = t; // Temp value
        } 
        if(request->hasParam("light")){
          jsondoc["status"]["light"] = l;
        } 
        if(request->hasParam("target_ip")){
          jsondoc["reporthost"]["target_ip"] = target_ip;
        } 
        if(request->hasParam("target_port")){
          jsondoc["reporthost"]["target_port"] = target_port;
        } 
        if(request->hasParam("target_sp")){
          jsondoc["reporthost"]["target_sp"] = target_sp;
        } 
        if(request->hasParam("uptime")){
          jsondoc["info"]["uptime"] = getUptime();
        } 
        if(request->hasParam("ssid")){
          jsondoc["info"]["ssid"] = getSSID();
        } 
        if(request->hasParam("mac")){
          jsondoc["info"]["mac"] = getMAC();
        } 
        if(request->hasParam("ip")){
          jsondoc["info"]["ip"] = getIP();
        } 
        if(request->hasParam("loc")){
          jsondoc["info"]["loc"] = LOCATION;
        } 
      }
  
        response->setLength();
        request->send(response);
    });

//    server.on("/set", HTTP_POST, [](AsyncWebServerRequest *request){
//    /* A route receiving a POST request with Internet coordinates 
//     *  of the reporting target host.
//     */
//     Serial.println("Receive Request for a ""target"" route !"); 
//        if (request->hasArg("led1")) {
//          Serial.println(request->arg("led1").c_str());
////           if(tolower(request->arg("led1").c_str()).equals("on")){
////              led1 = ON;
////           }
////           if(tolower(request->arg("led1").c_str()).equals("off")){
////              led1 = OFF;
////           }
//        }     
//    });
    
  // If request doesn't match any route, returns 404.
  server.onNotFound([](AsyncWebServerRequest *request){
      request->send(404);
  });

  // Start server
  server.begin();
}
