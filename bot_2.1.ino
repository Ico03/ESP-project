//#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define WIFI_SSID "FRITZ!Box 7590 ET"  //ssid wifi
#define PASSWORD_WIFI "mammamia"   //password wifi
 #define BOT_API_TOKEN "5442330903:AAHGdmi4X4Ia4fi_kNa-HY9_Bry1Tof_sFI"  //token bot

//intervallo di comunicazione per controllo nuovi messaggi
const unsigned long intervallo = 1000;  //la scheda controllerà mesaggi ogni secondo, al variare del parametro varia il tempo
unsigned long ultima_chiamata = 0;

//Configurazione client e bot per la comunicazione esp->telegram
WiFiClientSecure s_client;
UniversalTelegramBot bot(BOT_API_TOKEN, s_client);

//funzione per la gestione dei messaggi
void gestisci_messaggi(int num_nuovi_messaggi)
{
  for (int i=0; i<num_nuovi_messaggi; i++)
  {
    String chat_id = bot.messages[i].chat_id;     //id chat
    String text = bot.messages[i].text;           //testo del messaggio
    String from_name = bot.messages[i].from_name; //nome di chi ha scritto il messaggio

    if (from_name == "")
    {
      from_name = "Utente sconosciuto";
      Serial.println(" Messaggio mandato da utente sconosciuto");  
    }

    if (text == "/start")
    {
      bot.sendMessage(chat_id, "Benvenuto");
      Serial.println("Benvenuto");
    }

    Serial.println(from_name);
    Serial.println(chat_id);
    Serial.println(text);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println();
  Serial.print("Connesso alla rete ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, PASSWORD_WIFI);

  s_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Wait...");
    delay(500);
  }

  Serial.print("WIFI connesso. IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - ultima_chiamata > intervallo) //se l'intervallo di tempo scade, eseguo la funzione loop 
  {
    int num_nuovi_messaggi = bot.getUpdates(bot.last_message_received + 1);
    while(num_nuovi_messaggi)
    {
      Serial.println("messaggio ricevuto");
      gestisci_messaggi(num_nuovi_messaggi);
      num_nuovi_messaggi = bot.getUpdates(bot.last_message_received + 1);                               
    }
    ultima_chiamata = millis();
  }

}
