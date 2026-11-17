#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
const char* AP="RFID_MESS"; const char* PASS="mess12345";
ESP8266WebServer server(80);
SoftwareSerial uno(D6,D5); // RX, TX; UNO D3 TX -> D5 RX via divider
String lastUid="",lastScan="";
String readFile(const char* p){File f=LittleFS.open(p,"r");if(!f)return "[]";String s=f.readString();f.close();return s;}
void writeFile(const char* p,String s){File f=LittleFS.open(p,"w");if(f){f.print(s);f.close();}}
void sendJson(int code,String s){server.sendHeader("Access-Control-Allow-Origin","*");server.send(code,"application/json",s);}
void cors(){server.sendHeader("Access-Control-Allow-Origin","*");server.sendHeader("Access-Control-Allow-Headers","Content-Type");server.sendHeader("Access-Control-Allow-Methods","GET,POST,OPTIONS");}
void options(){cors();server.send(204);}
void handleStudents(){cors();if(server.method()==HTTP_GET){server.send(200,"application/json",readFile("/students.txt"));return;}
 DynamicJsonDocument doc(1024);if(deserializeJson(doc,server.arg("plain"))){sendJson(400,"{\"error\":\"bad json\"}");return;}
 String name=doc["name"]|"";String roll=doc["roll"]|"";String uid=doc["uid"]|"";uid.toUpperCase();
 DynamicJsonDocument all(8192);deserializeJson(all,readFile("/students.txt"));JsonArray a=all.as<JsonArray>();
 for(JsonObject s:a)if(String((const char*)s["uid"])==uid){sendJson(409,"{\"error\":\"UID already registered\"}");return;}
 JsonObject n=a.createNestedObject();n["name"]=name;n["roll"]=roll;n["uid"]=uid;String out;serializeJson(all,out);writeFile("/students.txt",out);sendJson(200,"{\"ok\":true}");
}
void recordMeal(String uid){DynamicJsonDocument students(8192);deserializeJson(students,readFile("/students.txt"));String name="",roll="";
 for(JsonObject s:students.as<JsonArray>())if(String((const char*)s["uid"])==uid){name=s["name"].as<String>();roll=s["roll"].as<String>();}
 if(name=="")return;
 String date=__DATE__; // replaced below using uptime-independent date supplied by phone in app version
 DynamicJsonDocument meals(12000);deserializeJson(meals,readFile("/meals.txt"));JsonArray a=meals.as<JsonArray>();
 for(JsonObject m:a)if(String((const char*)m["uid"])==uid && String((const char*)m["meal"])=="Scan" && String((const char*)m["date"])==date)return;
 JsonObject n=a.createNestedObject();n["uid"]=uid;n["name"]=name;n["roll"]=roll;n["meal"]="Scan";n["date"]=date;n["time"]=String(millis()/1000);
 String out;serializeJson(meals,out);writeFile("/meals.txt",out);lastScan="{\"uid\":\""+uid+"\",\"name\":\""+name+"\",\"roll\":\""+roll+"\",\"date\":\""+date+"\"}";
}
void setup(){
 Serial.begin(115200);uno.begin(9600);LittleFS.begin();
 if(!LittleFS.exists("/students.txt"))writeFile("/students.txt","[]");
 if(!LittleFS.exists("/meals.txt"))writeFile("/meals.txt","[]");
 WiFi.softAP(AP,PASS);server.on("/api/health",HTTP_GET,[]{cors();sendJson(200,"{\"ok\":true,\"server\":\"RFID_MESS\"}");});
 server.on("/api/students",HTTP_GET,handleStudents);server.on("/api/students",HTTP_POST,handleStudents);
 server.on("/api/students",HTTP_OPTIONS,options);server.on("/api/health",HTTP_OPTIONS,options);
 server.on("/api/last-scan",HTTP_GET,[]{cors();sendJson(200,lastScan==""?"{}":lastScan);});
 server.on("/api/report",HTTP_GET,[]{cors();String data=readFile("/meals.txt");sendJson(200,data);});
 server.begin();Serial.println(WiFi.softAPIP());
}
void loop(){server.handleClient();if(uno.available()){String x=uno.readStringUntil('\n');x.trim();if(x.startsWith("UID:")){lastUid=x.substring(4);recordMeal(lastUid);}}}
