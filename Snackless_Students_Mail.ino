#include<Arduino.h>
#include<WiFi.h>
#include<ESP_Mail_Client.h>
//#include<ESPAsyncWebServer.h>

//  Creating Web Server
int mailCount = 1;

#define RXp2 3
#define TXp2 1

#define WIFI_SSID "manas"  // Wifi/hotspot username
#define WIFI_PASSWORD "yehdilmangemore" //  wifi/hotspot - password

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define SUDO_EMAIL "SnacklessStudents@gmail.com"   // Sender's Email address
#define SUDO_PASSWORD "ccbnlfrftctlthds"          // (This is not the gmail password) Sender's Email App Password; How to setup ----> https://www.getmailbird.com/gmail-app-password/

/* Recipient's email*/
#define RECIPIENT_EMAIL "lfry1@vols.utk.edu" 

WiFiServer server(80);

SMTPSession smtp;
void smtpCallback(SMTP_Status status);

String addressing[] = {"hci", "research", "design", "testing", "feedback", "participation", "usability", "accessibility", "ux", "analysis", 
                  "survey", "prototype", "evaluation", "weRequestYou", "observations", "persona", "taskanalysis", "fieldstudy", "userneeds", "emptystomach", "datacollection",
                  "believeUsWeAreHungry", "please", "weBegYou", "fightForRight", "howCanYouDoThisToUs", "hungry", "food", "Snacks", "needfood"};

String message1 = "Dear VolCard Office,\n\n"
                 "The only place that sells food within a 5-minute walk of the Min Kao building is the vending machine on in MK338A. The card reader on it has been broken for over a year, and younger generations do not carry cash. We have spoken to both faculty and students about this topic and many have said that they have been hungry and distracted during lectures due to the unavailability of food. I don't think it's unfair to say that as Electrical Engineering and Computer Science students, we are some of the busiest and most stressed students on campus. I have seen multiple students there at 2 AM. We would very much like to be able to buy a snack in between classes!\n\n"
                 "We have attempted for months to use the proper channels to get the card reader repaired. Without our sustained effort, the university stops even trying to get it fixed.\n\n"
                 "Please take the basic needs of the students and teachers seriously and fix our vending machine card reader!";

String message2 = "Hey there Volcard Office!\n\n"
                 "We've had a bit of a problem with the vending machine lately. Unfortunately, the card reader in MK338A has been broken for over a year, and many students  ("+String(mailCount)+" to be precise)  have tried and failed to buy snacks from it. The thing is, it's the only place to buy food within a 5-minute walk of the Min Kao building, and younger generations don't carry cash.\n\n"
                 "As Electrical Engineering and Computer Science students, we're some of the busiest and most stressed out on campus. We've spoken to both faculty and students about this issue, and it seems like a lot of people have been distracted and hungry during lectures due to the unavailability of food. I've even seen students there at 2 AM!\n\n"
                 "We've tried using the proper channels to get the card reader fixed, but it's been months and we're not making much progress. We really need the university to take our basic needs seriously and fix the vending machine card reader. Can you help us out?";

String message3 = "Hewwo Vol Cawd Officeeee!~~~\n\n"
                 "We've got a widdle problemo with the vending machine, owo. It's been over a year since the card reader in MK338A stopped working, and at least "+String(mailCount)+" students have twied and failed to buy snacks from it.\n\n"
                 "But the thing is, that vending machine is the only place within a 5-minute walk of the Min Kao building where we can get some yummy food. And the young generations don't carry cash, so with the card reader broken, it's making us sad and hangwy.\n\n"
                 "As Electrical Engineering and Computer Science students, we're super duper busy and stressed out, uwu. We've talked to our teachers and other students, and it seems like many of us have been feeling hungry and distracted during lectures because we can't get any snacks. I've even seen some students there at 2 AM, trying to get their munchies fix!\n\n"
                 "We've been trying for months to get the card reader fixed through the proper channels, but it's been like chasing our tails. The university doesn't seem to be taking our needs seriously, and that's not very nice, uwu. So, we're hoping you can lend a paw and help us get that vending machine fixed, pwease?\n\n"
                 "Thank you so much! UwU";

String message4 = "Howdy there, partner!\n\n"
                 "We got ourselves a bit of a predicament with that dang vending machine. It's been a year now since that card reader in MK338A busted, and we got a whole parcel of students tryin' ("+String(mailCount)+" to be precise) to get some grub outta that thing and failing miserably.\n\n"
                 "Now, let me tell you, that vending machine is the only place where we can get some decent food within a 5-minute walk of the Min Kao building. But with that card reader broken, and these newfangled younger generations not carryin' cash, we got ourselves a real pickle.\n\n"
                 "As Electrical Engineering and Computer Science students, we're some of the busiest and most stressed-out folks on campus. We've spoken to our fellow students and even the faculty, and they all agree that this is a mighty big problem. It's been causing a lot of us to get distracted and downright hangry during lectures. Shoot, I've even seen some students campin' out at the machine at 2 AM!\n\n"
                 "We've been trying for months to get that card reader fixed through the proper channels, but it's been like tryin' to catch a greased pig at a rodeo. The university ain't puttin' in much effort, and we're gettin' mighty tired of it. We need them to start takin' our basic needs seriously and get that vending machine fixed up proper. Y'all reckon you can lend us a hand here?\n\n"
                 "Thanks, partner!";

String message_body[] = {message1, message2, message3, message4};

String subject_msgs[] = {
  "Vending Machine Ate My Card: Help!",
  "Card Reader Malfunction: Snacks on Hold",
  "Vending Machine Mutiny: Card Reader Down",
  "Emergency Snack Situation: Card Reader Broken",
  "Vending Machine Troubles: Card Reader Woes",
  "Card Reader Crisis: Vending Machine Rebellion",
  "Snackless in Tennessee: Broken Card Reader",
  "Vending Machine Rebellion: Card Reader Sabotage",
  "Hunger Strike: Broken Card Reader to Blame",
  "Snack Attack: Card Reader Disaster",
  "Broken Card Reader Blues: Vending Machine Woes",
  "Vending Machine Vexation: Card Reader Meltdown",
  "Snack Emergency: Card Reader Down for the Count",
  "Vending Machine Madness: Card Reader Chaos",
  "SOS: Card Reader on the Fritz",
  "Vending Machine Insanity: Card Reader Out of Order",
  "Broken Card Reader, No Snacks: The Struggle is Real",
  "Vending Machine Meltdown: Card Reader in Disarray",
  "Card Reader Catastrophe: Snackless in Knoxville",
  "Vending Machine Mayhem: Card Reader Rebellion in Progress"
};


void setup() {
   
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to WIFI...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // WiFi.begin(WIFI_SSID);    // open wifi connection
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }

  Serial.println("");
  Serial.println("WiFi Connected!😁");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println();

   if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
    Serial.println("SPIFFS mounted successfully");
  }

  smtp.debug(1); // 0 for no debug and 1 for basic debug
  server.begin();

}

void loop() {
  
  // Web Server
      WiFiClient client = server.available();   
      if (client) {                         
        Serial.println("New Client.");           
        String currentLine = "";                
        while (client.connected()) {           
          if (client.available()) {             
            char c = client.read();             
            Serial.write(c);                    
            if (c == '\n') {                  

              // if the current line is blank, you got two newline characters in a row.
              // that's the end of the client HTTP request, so send a response:
              if (currentLine.length() == 0) {
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();

                // the content of the HTTP response follows the header:
                client.print("<html>\
                        <head>\
                        <title>Snack Less Students</title>\
                        <style>\
                        * {\
                        font-family: \"Gill Sans\", sans-serif;\
                        }\
                        h1 {\
                        color: white;\
                        }\
                        h2 {\
                        height: 25px !important;\
                        }\
                        .body_style {\
                        background-color: #ff8200 !important;\
                        }\
                        .center {\
                        display: flex;\
                        justify-content: center;\
                        align-items: center;\
                        }\
                        .plate {\
                        width: 200px;\
                        height: 200px;\
                        background-color: white;\
                        border-radius: 50%;\
                        border: 10px solid #d3d3d3;\
                        position: relative;\
                        }\
                        .plate::before {\
                        content: \"\";\
                        width: 120px;\
                        height: 5px;\
                        background-color: #f5deb3;\
                        position: absolute;\
                        top: 80px;\
                        left: 40px;\
                        transform: rotate(-10deg);\
                        }\
                        .plate::after {\
                        content: \"\";\
                        width: 120px;\
                        height: 5px;\
                        background-color: #f5deb3;\
                        position: absolute;\
                        top: 80px;\
                        left: 40px;\
                        transform: rotate(10deg);\
                        }\
                        .container {\
                        display: flex;\
                        flex-direction: column;\
                        align-items: center;\
                        }\
                        .heading_align {\
                        text-align: center;\
                        }\
                        /* Center-aligned div */\
                        .center-div {\
                        display: flex;\
                        justify-content: center;\
                        align-items: center;\
                        }\
                        </style>\
                        </head>\
                        <header style=\"background-color: #ff8200;\">\
                            <div class=\"center\">\
                                <h1 style=\"font-size: 48px !important;\">Snackless Student's</h1>\
                            </div>\
                          </header>\
                        <body style=\"background-color: #ff8200;\">\
                        <div style=\"margin:75px auto;\">\
                        <div class=\"center\">\
                        <h1>Out of 4,899 students in Min Kao</h1>\
                        </div>\
                        <div class=\"center center-div\">\
                        <div class=\"container\">\
                        <div class=\"plate\">\
                        <div style=\"display: flex; justify-content: center; align-items: center;margin: 40%;\">\
                        <h2 style=\"font-size: 48px;\">"+String(mailCount)+"</h2>\
                        </div>\
                        </div>\
                        <h1>Student's are hungry....!</h1>\
                        </div>\
                        </div>\
                        </div>\
                        </body>\
                        </html>");

                // The HTTP response ends with another blank line:
                client.println();
                // break out of the while loop:
                break;
              } else {    // if you got a newline, then clear currentLine:
                currentLine = "";
              }
            } else if (c != '\r') {  // if you got anything else but a carriage return character,
              currentLine += c;      // add it to the end of the currentLine
            }

          }
        }

      }


  String msg_recieved = Serial2.readString();
  Serial.println("Message Received: ");
  Serial.println(msg_recieved);

  if(msg_recieved=="SENDEMAIL"){
    sendEmail(msg_recieved);
  }
  //sendEmail(msg_recieved);
  delay(1000);
}

void sendEmail(const String& messageContent){
    // Selecting Randomly one of the plus addressing
    randomSeed(analogRead(A0)); // taking random noise from the input circuitry
    String rand_addr = addressing[random(0,sizeof(addressing)/sizeof(addressing[0]))];

    // Selecting random Message Body
    randomSeed(analogRead(A0));
    String rand_body = message_body[random(0, sizeof(message_body)/sizeof(message_body[0]))];

    // Selecting random Subjects
    randomSeed(analogRead(A0));
    String rand_subject = subject_msgs[random(0, sizeof(subject_msgs)/sizeof(subject_msgs[0]))];

    
    String SENDER_EMAIL = "SnacklessStudents+"+rand_addr+"@gmail.com";

    ESP_Mail_Session session;
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = SUDO_EMAIL;
    session.login.password = SUDO_PASSWORD;
    session.login.user_domain = "gmail.com";

    SMTP_Message message;
    message.sender.name = "Snackless Student"; // message headers
    message.sender.email = SENDER_EMAIL;
    message.subject = rand_body;
    message.addRecipient("Snackless Students", RECIPIENT_EMAIL);

    // Sending text messages
    String msg = "Hundgry Student count: "+String(mailCount)+" it's increasing FAST";
    message.text.content = msg.c_str();
    //message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

    if(!smtp.connect(&session)) return;

    if(!MailClient.sendMail(&smtp, &message)){
      Serial.println("Error sending email, "+smtp.errorReason());
    }
    smtp.callback(smtpCallback);
}

void smtpCallback(SMTP_Status status){

  if(status.success()){
    mailCount++;
  }
  Serial.println(status.info());

  if(status.success()){
    Serial.println("------------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("--------------------\n");
    struct tm dt;

    for(size_t i=0;i<smtp.sendingResult.size();i++){
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n",i+1);
      ESP_MAIL_PRINTF("Status: %s\n",result.completed?"success":"failed");
     // ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n",dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n",result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n",result.subject.c_str());
    }
    Serial.println("---------------------------\n");
  }
}


