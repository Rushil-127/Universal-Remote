#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);
#include <EEPROM.h>


String essid;
String  epass;

int Tnetwork=0;
//String s="",ssid_names[20];

int index_value; 
boolean wifiConnect = false;
boolean eFlag = false;
boolean state = true;

const char* ssid = "Redmi Note 10";
const char* password = "Saurin@1968";

const char* INPUT_1 = "names";
const char* INPUT_2 = "pass";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
    <style>
        body {
            background: rgb(50,100,150);
            
            }
        .box {
            align-items: center;
            margin-left: 25%;
            margin-top: 5%;
            background:rgb(170, 195, 215);
            color: antiquewhite;
            width: 30%;
            height: 30%;
            float: none;
            border: 5px;
            border-radius: 10px;
            border-style: solid;
            border-color: rgb(240, 225, 235);
            padding: 10px;
            justify-content: center;
            }
            .fa{
                cursor: pointer;
            }
    </style>
</head>
<body>
    <div class="box">
        <h2>Change your ssid and password</h2>
        <form action="/get"> 
           <div>
            <label for="Select SSID">Select SSID</label>
            
                <select name="names">%xyz%</select>
           
            </div>
            <div>
                <label for="pass">PASSWORD</label>
                <input type="password" name="pass" id="pass" placeholder="enter password">
                <i class="fa fa-eye-slash" id="eye" aria-hidden="true"></i>
            </div>
            <div>
                <button type="submit" value="submit">submit</button>
                <button type="reset" value="reset">reset</button>
            </div>
                            
        </form>
    </div>
    <script>
        document.getElementById("eye").addEventListener("click",function()
        {
            if(document.getElementById("pass").type=="password")
            {
                document.getElementById("pass").type="text";
                this.classList.add("fa-eye");
                this.classList.remove("fa-eye-slash");
            }
            else
            {
                document.getElementById("pass").type="password";
                this.classList.remove("fa-eye");
                this.classList.add("fa-eye-slash");
            }
        });
    </script>
</body>
</html>
)rawliteral";


String option(const String& var){

  String s="",ssid_names[20];  
  Serial.println(Tnetwork);
  for (int i = 0; i < Tnetwork; i++){
      ssid_names[i] = WiFi.SSID(i);
  }
if(var == "xyz"){
  for (int op=0;op<Tnetwork;op++){
    s += "<td>";
    s += "<option value='";
    s += ssid_names[op];
    s += "'>";
    s += ssid_names[op];
    s += "</option>";
    s += "</td>";
  }
  return s;
}
return String();
}
//*********************8
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void eepromWrite(int address, String ssid, String pass){
  byte len = ssid.length();
  EEPROM.write(address, len);

  for(int i=0; i<len; i++){
    EEPROM.write(address + 1 + i, ssid[i]);
  }
  EEPROM.commit();

  EEPROM.write(len+1, pass.length());
  len += 1;
  for(int i=0; i<pass.length(); i++){
    EEPROM.write(len + 1 + i, pass[i]);
  }
  EEPROM.commit();
}
void eepromRead(int address){
  int lens = EEPROM.read(address);
  char datassid[lens + 1];

  for(int i=0; i<lens; i++){
    datassid[i] = EEPROM.read(address + 1 + i);
  }

  datassid[lens] = '\0';
  
  lens = lens + 1;
  int lenp = EEPROM.read(lens);
  char datapass[lenp +1];
   
  for(int i=0; i<lenp; i++){
    datapass[i] = EEPROM.read(lens + 1 + i);
  }
  
  datapass[lenp] = '\0';

  essid = String(datassid);
   epass = String(datapass);
}


boolean setup_wifi() {
  boolean state = true; 
  int i = 0;
  Serial.print("setup_wifi() >> ");
  Serial.println("Connecting to network..."); 
  WiFi.mode(WIFI_STA); 
  
  if(eFlag){
    eepromRead(0);
    WiFi.begin(essid,  epass); 
  }

  else{
    WiFi.begin(ssid, password); 
  }

 Serial.print("                ");
  while(WiFi.status() != WL_CONNECTED){ 
    
    Serial.print("."); 
    delay(500); 
    if (i > 20) { 
      state = false; 
      break; 
    }
    i++;
  }
  Serial.println("");

//  Serial.print("                ");
  if(state){
    if(eFlag)
    {Serial.print("                ");
      Serial.print(essid); 
    }
    else
    { Serial.print("                ");
      Serial.print(ssid);
  }
//    Serial.print("                ");
    Serial.println("'s WiFi Connected Successfully"); 
    Serial.print("                ");
    Serial.print("IP address :"); 
    IPAddress myIP = WiFi.localIP(); 
    Serial.println(myIP); 
  }

  return state; 
}



void setup() {
  Serial.begin(115200);  

  Tnetwork = WiFi.scanNetworks();

  EEPROM.begin(512);

  eepromRead(0);

  Serial.print("                ");
  wifiConnect = setup_wifi(); 
  eFlag = true;

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){

  request->send_P(200, "text/html", index_html, option);
  });
  
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputMessage1;
    String inputMessage2;
    
    String inputParam;
    String inputParam1;
    String inputParam2;
    
    
    if (request->hasParam(INPUT_1)) {
      inputMessage1 = request->getParam(INPUT_1)->value();
      inputParam1 = INPUT_1;
    }
    
    if (request->hasParam(INPUT_2)) {
      inputMessage2 = request->getParam(INPUT_2)->value();
      inputParam2 = INPUT_2;
    }
    
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

    for (int i = 0 ; i < 512 ; i++) {
      EEPROM.write(i, 0);
    }
  
    eepromWrite(0, inputMessage1, inputMessage2);


    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  eFlag = true;
    if(WiFi.status() != WL_CONNECTED){
    eFlag = true;
    setup_wifi(); 
  }
    if(WiFi.status() != WL_CONNECTED)
    {
      eFlag = false;
      setup_wifi();
    }
}    
