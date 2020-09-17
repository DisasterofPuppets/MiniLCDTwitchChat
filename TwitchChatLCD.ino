/*******************************************************************
Disaster of Puppets
YouTube: Http://disasterofpuppets.com
Twitch: Https://twitch.tv/disasterofpuppets

4 x 20 LCD Screen that displays Twitch Chat
//make sure you choose NodeMCU 1.0 (ESP - 12E Module) as programmer..?

Using https://github.com/fredimachado/ArduinoIRC

// Don't forget to edit and save Creds.h
 *******************************************************************/

#include <IRCClient.h>
#include "Creds.h"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

//-------------EDIT in Creds.h----------
char ssid[] = WIFI_SSID;       // your network SSID (name)
char password[] = WIFI_PASS;  // your network key

//------------------------------

int led = LED_BUILTIN;
String ircChannel = "";
int runonce = 0;
WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);

LiquidCrystal_I2C lcd(0x27, 16, 2); 


// put your setup code here, to run once:
void setup() {
  
  Serial.begin(115200);
  Wire.begin(D1,D2);
  lcd.begin(20,4);
  lcd.backlight();

  
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  ircChannel = "#" + twitchChannelName;

  client.setCallback(callback);

}

void loop() {

  // Try to connect to chat. If it loses connection try again
  if (!client.connected()) {
    Serial.println("Attempting to connect to " + ircChannel );
    // Attempt to connect
    // Second param is not needed by Twtich
    if (client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN)) {
      client.sendRaw("JOIN " + ircChannel);
      Serial.println("connected and ready to rock");
  
      lcd.setCursor(0, 0);  // Move the cursor at origin
      lcd.print("Welcome");
      lcd.setCursor(0, 1);
      lcd.print("to");
      lcd.setCursor(0, 2);
      lcd.print("Twitch");
      lcd.setCursor(0, 3);
      lcd.print("Chat");

    } else {
      Serial.println("failed... try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }
  client.loop();

}

void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}

void callback(IRCMessage ircMessage) {
  //Serial.println("In CallBack");

  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') {
    //Serial.println("Passed private message.");
    ircMessage.nick.toUpperCase();

    String message("<" + ircMessage.nick + "> " + ircMessage.text);
    String Nick(ircMessage.nick);
    //prints chat to serial
    Serial.println(message);
    Serial.println(Nick);

//send the same message to the LCD screen here: make sure to wrap it
    LCDoutput(message,Nick);

// end of function

    
    for (int i = 0; i < 6; i++) {
      digitalWrite(led, HIGH);
      delay(50);
      digitalWrite(led, LOW);
      delay(25);
    }

    return;
  }
}

void LCDoutput(String message, String Nick){
  
  if (Nick != "DISASTEROFPUPPETS" && Nick != "DRPUPPETMASTER"){
    
    if (message.length() <= 19) {
      String message1 = message.substring(0,19);
      clearit();
      lcd.setCursor(0,0);
      lcd.print(message1);
    }
    else if (message.length() > 19 && message.length() <= 39) {
      String message1 = message.substring(0,19);
      String message2 = message.substring(20,39);
      clearit();
      lcd.setCursor(0,0);
      lcd.print(message1);
      lcd.setCursor(0,1);
      lcd.print(message2);
    }
    else if (message.length() > 39 && message.length() <= 59) {
      String message1 = message.substring(0,19);
      String message2 = message.substring(20,39);
      String message3 = message.substring(40,59);
      clearit();
      lcd.setCursor(0,0);
      lcd.print(message1);
      lcd.setCursor(0,1);
      lcd.print(message2);
      lcd.setCursor(0,2);
      lcd.print(message3);
    }    
    else if (message.length() > 59 && message.length() <= 79) {
      String message1 = message.substring(0,19);
      String message2 = message.substring(20,39);
      String message3 = message.substring(40,59);
      String message4 = message.substring(60,79);
      clearit();
      lcd.setCursor(0,0);
      lcd.print(message1);
      lcd.setCursor(0,1);
      lcd.print(message2);
      lcd.setCursor(0,2);
      lcd.print(message3);
      lcd.setCursor(0,3);
      lcd.print(message4);
    }
    else if (message.length() > 80) {
      clearit();
      lcd.setCursor(0,0);
      lcd.print("Message was too long");
    }

  }
    
}

void clearit(){
  
      lcd.setCursor(0,0);
      lcd.print("                    ");
      lcd.setCursor(0,1);
      lcd.print("                    ");
      lcd.setCursor(0,2);
      lcd.print("                    ");
      lcd.setCursor(0,3);
      lcd.print("                    ");
}
