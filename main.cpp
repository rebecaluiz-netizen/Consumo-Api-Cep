#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void conexaoWifi();
void reconexaoWifi();
String consultaApiCep(String cep);

const char *SSID = "SENAI EDUCACIONAL";
const char *SENHA = "info@134";

const unsigned long tempoConexao = 10000;
const unsigned long tempoReconexao = 5000;

const String enderecoApi = "https://brasilapi.com.br/api/cep/v2/";


void setup() {
  Serial.begin(115200);

  conexaoWifi();

  String cep = "09510200";

  String respostaApiCep = consultaApiCep(cep);

  if (respostaApiCep != "")
  {
    JsonDocument doc;

    DeserializationError erro = deserializeJson(doc, respostaApiCep);

    if (!erro)
    {
      String rua = doc["street"];
      String bairro = doc["neighborhood"];
      String cidade = doc["city"];
      String estado = doc["state"];

      Serial.println();
      Serial.println("DADOS DO ENDERECO");
      Serial.println("-----------------");
      Serial.println("CEP:  " + cep);
      Serial.println("Rua:  " + rua);      
      Serial.println("Bairro:  " + bairro);      
      Serial.println("Cidade:  " + cidade);      
      Serial.println("Estado:  " + estado);      
    }

    else
    {
      Serial.println("Erro ao interpretar o JSON");
    }
    
  }

}


void loop() {
  reconexaoWifi();

  String respostaApiCep = "";
  String cep = "";

   if (Serial.available() > 0) {
  
    cep = Serial.readStringUntil('\n');
    
    cep.trim();
    
    if (cep.length() == 8) 
    {
      respostaApiCep = consultaApiCep(cep);
    } 

    else 
    {
      Serial.println("CEP invalido");
    }
  }

  if (respostaApiCep != "")
  {
    JsonDocument doc;

    DeserializationError erro = deserializeJson(doc, respostaApiCep);
  }
  
  
}

void conexaoWifi()
{
  Serial.printf("Conectando ao wifi: %s", SSID);

  WiFi.begin(SSID, SENHA);

  unsigned long tempoWifi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - tempoWifi < tempoConexao)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Wifi conectado com sucesso!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  }

  else
  {
    Serial.println("Falha ao se conectar");
  }
}

void reconexaoWifi()
{
  unsigned long tempoAtual = millis();
  unsigned long tempoUltimaConexao = 0;

  if (tempoAtual - tempoUltimaConexao > tempoReconexao)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Conexao Wifi perdida");
      conexaoWifi();
    }
    tempoUltimaConexao = tempoAtual;
  }
}

String consultaApiCep(String cep)
{
  HTTPClient http;
  http.begin(enderecoApi + cep);

  int codigoHttp = http.GET();
  String resposta = "";

  delay(3000);

  if (codigoHttp == 200)
  {
    resposta = http.getString();
  }

  else
  {
    Serial.println("Erro. Codigo HTTP: " + String(codigoHttp));
  }

  http.end();

  return resposta;
  
}