#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

//Credenciales para la red wifi
const char* ssid = "Red-Paolo";
const char* password = "Altp9194*";

// Dirección del posteo por HTTP a la base de datos del servidor de la universidad o a ThingSpeak
const char* serverName = "http://api.thingspeak.com/update?api_key=ZSZHVORGHHCM6R06";

// Clave del servicio API
//String apiKey = "claveDeThingSepak";

float valor1 = 0.0, valor2=0.0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  dht.begin();
  delay(5000);
}

void loop() {
  //Se valida conexión a WiFi    
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http; 
      // Se establece el inicio del cliente HTTP para posteo de información.
      http.begin(client, serverName);    
      // Se especifica el encabezao content-type (en este caso por XML)
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");    
      //Información a enviar al servidor
      String httpRequestData = "&field1=" + String(t)+"&field2="+String(h);          
      // Se envía solicitud HTTP POST
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("Código de respuesta HTTP: ");
      Serial.println(httpResponseCode);  

    //Esta validación es para verificar que hubo conexión correcta con el servidor y nos arroja lo que envíe como respuesta.
      if (httpResponseCode==200){
        String cuerpo_respuesta = http.getString();
        Serial.println("El servidor respondió: ");
        Serial.println(cuerpo_respuesta);
      }   
      // Se liberan recursos del cliente HTTP
      http.end();
    }
    else {
      Serial.println("WiFi Desconectado..."); //Si Wifi se desconecta nos arroja este mensaje
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
        Serial.print(".");
        delay(5000);}
    }

    delay(10000); //Intervalo de envío entre mediciones de los sensores.
}
