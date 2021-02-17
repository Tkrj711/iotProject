#include <WiFi.h>
#include <string> 
#include <ESP32Servo.h>
#include <HTTPClient.h>

const int servoPin1 = 25; // サーボ1のピン番号
const int servoPin2 = 26; // サーボ2のピン番号
Servo myservo1, myservo2;

// Wifi設定
const char* ssid = "*****";
const char* pass = "*****";
const char* server = "*****.herokuapp.com";
// お風呂のURL
String url = "/bath";
String host = String("http://") + server + url;
const int httpPort = 80;
WiFiClient client;
char number[1000];

void setup() 
{
  Serial.begin(115200);
  // standard 50 hz servo
  myservo1.setPeriodHertz(50);
  myservo2.setPeriodHertz(50);
  
  // *****************************************************
  // *          サーボに割り当てるピンを登録             *
  // * attach(pin,min,max)                               *
  // * デフォルト(min:544, max:2400)                     *
  // * min:サーボ角度が0度のときのパルス幅（マイクロ秒)  *
  // * max:サーボ角度が180度のときのパルス幅（マイクロ秒)*
  // *****************************************************
  myservo1.attach(servoPin1);   
  myservo2.attach(servoPin2);
  
  // ESP32にコンパイル時、WiFi接続無視して動作がおかしくなるため待機
  delay(1000);
  // Wi-fiに接続
  WiFi.begin(ssid, pass);
  
  // 繋がるまでループ
  while (WiFi.status() != WL_CONNECTED)
  {
    // 繋がらなかったら
    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      Serial.println("aborting wifi connection to SSID. plz check at config.");
    }
    Serial.println("connecting...");
    delay(6000);
  }

  Serial.println("success!");
  //サーバー接続
  if (client.connect(server,httpPort)) {
    Serial.println("Client Connected");
  } else {
    Serial.println("No Connection　desu");
  }
}

// 1分ごとに確認しにいく
void loop()
{
  if (client.connect(server,httpPort)) {
    Serial.println("監視中のサーバ接続中");
  } else {
    Serial.println("監視中のサーバ接続失敗");
  }
  
  // ※下記、ClientでもGET可能だそうだが、今回、パラメータが上手く取得できなかった。
  //  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  //               "Host: " + server + "\r\n" + 
  //               "Connection: close\r\n\r\n");
  //               
  //  Serial.print(String("GET ") + url + " HTTP/1.1\r\n" +
  //               "Host: " + server + "\r\n" + 
  //               "Connection: close\r\n\r\n");  
  //  String result;
  //  while(client.available()){
  //    result = client.readStringUntil('\r');
  //  }
  //  Serial.print(result);
  //  受け取る値を要確認
  //  result.toCharArray(number,result.length()+1);

  HTTPClient http;
	
  // 接続(HTTP:GET)
  http.begin(host.c_str());
  int httpCode = http.GET();

  String result = "";

  if (httpCode < 0) {
    result = http.errorToString(httpCode);
  } else if (http.getSize() < 0) {
    result =  "size is invalid";
  } else {
    result = http.getString();
  }
  // パラメータを配列に置き換え
  result.toCharArray(number,result.length()+1);
  http.end();
  
  Serial.println(number);
  Serial.println(number[3]);
  Serial.println(number[4]);
  // 受け取った値で処理を行う
  if(number[3] == '1') {
    // お風呂を入れる
    myservo2.write(140);
    delay(200);
    myservo2.write(180);
  } else if(number[3] == '2') {
    // お風呂を沸かす
    myservo1.write(140);
    delay(200);
    myservo1.write(180);
  } else {
  	// 今後、洗濯機や冷房など追加していく.....
  	Serial.println("coming soon....");
  }
  delay(25000);
}
