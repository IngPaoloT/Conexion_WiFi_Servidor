#include <WiFi.h>
#include <HTTPClient.h>

//Credenciales para la red wifi
const char* ssid = "Nombre_de_la_Red";
const char* password = "Contraseña_de_la_red";

// Dirección del posteo por HTTP a la base de datos del servidor de ThingSpeak
const char* serverName = "http://api.thingspeak.com/update?api_key=ZSZHVORGHHCM6R06";

void setup() {
  Serial.begin(9600);
  conexionWiFi();
  delay(5000);
}

void loop() {  
  posteoDatos();
  delay(10000); //Intervalo de envío entre mediciones de los sensores.
}

/*---------Se valida conexión a WiFi----------*/

void conexionWiFi(){
  Serial.println("WiFi Desconectado..."); //Si Wifi se desconecta nos arroja este mensaje
  while(WiFi.status() != WL_CONNECTED){
    /*Se conecta a una configuración de red WPA/WPA2. 
      Si la conexión es red WEP cambiar el código (Ver documentación).*/
    WiFi.begin(ssid, password);
    Serial.print(".");
    delay(2000);
  }
}

/*---------Envío de datos por posteo HTTP----------*/

void posteoDatos(){
  if(WiFi.status()== WL_CONNECTED){
      respuestaHTTP();
    }
    else {
      conexionWiFi();
    }
}

void respuestaHTTP(){
  WiFiClient client;
  HTTPClient http;
  // Se establece el inicio del cliente HTTP para posteo de información.
  http.begin(client, serverName);
  // Se especifica el encabezao content-type (en este caso por XML)
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    
  //Información a enviar al servidor
  String httpRequestData = "&field1=" + (String)random(100)+"&field2="+(String)random(100);          
  // Se envía solicitud HTTP POST
  int httpResponseCode = http.POST(httpRequestData);
    
  //Esta validación es para verificar que hubo conexión correcta con el servidor y nos arroja lo que envíe como respuesta.
  if (httpResponseCode==200){
      String cuerpo_respuesta = http.getString();
      Serial.println("El servidor se ha conectado y respondió: ");
      Serial.println(cuerpo_respuesta);
    }
  else{
    Serial.print("Código de respuesta HTTP: ");
    Serial.println(httpResponseCode); 
  }
  // Se liberan recursos del cliente HTTP
  http.end();
}
