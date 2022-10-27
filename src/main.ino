/*
MIT License

Copyright (c) 2022 Software-Ingenier, Popov Kirill Michailovich

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "SimpleModbusMaster.h"
#include "ESP8266WebServer.h"
#include "./client/HTML/site.html"
#include "./client/HTML/ToChangePass.html"
#include "./client/HTML/DELuser.html"
#include "./client/HTML/REGuser.html"
#include "./client/HTML/error403_2.html"
#include "./client/HTML/error_403.html"
#include "./client/HTML/Sign_in.html"
#include "./client/JS/jquery-3.6.0.min.js"
#include "./client/JS/functionsPROJECT.js"
#include "./client/CSS/bootstrap.min.css" 

#define baud 9600     // Скорость                                        
#define timeout 2000  // Задержка между пакетами;
#define polling 1000  // Интервал опросов пакетов Modbus;
#define retry_count 5 // Колличество повторов;
#define TxEnablePin 5 // Пин управления приём/передача по RS485;

void  HTTP_connection(),
      Jquer(),
      handle_OnConnect(),
      bootstrapCSS(),
      functions(),                           
      btnSITE(),
      SubmitAuntefication(),
      ConfigJSON(),
      typeSKZ(),
      Auntefication(),
      changePass(),
      VerificedPass(),
      regUser(),
      regUserBTN(),
      delUser(),
      delUserBTN(),
      locarionSITE(),
      Button_Uout(),
      Button_Iout(),
      Button_Upp(),
      Button_Usp();


bool    FLAG_SubmitAuntefication,
        slct1, slct2, slct3, slct4 = false;     // slct* - Селекты формы Web-сайта (флаги = false) 

unsigned int  regs1[30],                        // регистры чтения
              regs2[2] = { 1, 0 };              // регистры записи
                        
enum                                                      
{                                                              
  PACKET1,              // пакет на чтение
  PACKET2,              // пакет на запись
  TOTAL_NO_OF_PACKETS   // Итоговое колличество пакетов          
};                                                             
  Packet packets[TOTAL_NO_OF_PACKETS];                         
  packetPointer packet1 = &packets[PACKET1],
                packet2 = &packets[PACKET2];

String  Login[] = {"admin"},
        Password[] = {"admin"},
        space = "",
        Uout,
        Iout,
        Upp,
        Usp,
        label2,
        label3, 
        label4, 
        label5, 
        label6 = "",
        Num = "Значение не введено",                                  
        ssidAP = "ESP",              // ESP - название точки доступа;
        passwordAP = "",             // Пароль точки доступа ESP "" (пароля нет);
        data1(),
        data2(),
        data3(),
        data4(),
        data5(),
        data6(),
        data7(),
        data8(),
        data9(),
        predata(uint16 count);
                                    
ESP8266WebServer HTTP(80);           // Старт сетвера на порту 80;
IPAddress apIP(192, 168, 10, 1);     // Задаём статический IP-адрес для web-странички;
                                                          
void setup()                                                 
{                                                              
  modbus_configure( baud,
                    timeout,
                    polling,
                    retry_count,
                    TxEnablePin,
                    packets,
                    TOTAL_NO_OF_PACKETS);
                    HTTP_connection();                         // Старнуем HTTP-соеденение

// Настройка чтения регистров
  packet1->id = 111;                                           //  Идентификатор станции;
  packet1->function = READ_HOLDING_REGISTERS;                  //  Фукция 0x03 - чтение регистров ModbusRTU;
  packet1->address = 0;                                        //  Стартовый адрес ModbusRTU;
  packet1->no_of_registers = 30;                               //  Колличество регистров Modbus;
  packet1->register_array = regs1;                             //  Выводим данные в массив regs1

// Настройка записи в регистры
  packet2->id = 111;
  packet2->function = PRESET_MULTIPLE_REGISTERS;               //  Фукция 0x10 - чтение регистров ModbusRTU;
  packet2->address = 5;
  packet2->no_of_registers = 2;
  packet2->register_array = regs2;                             //  Берём данные из regs2 и передаём данные по Модбас для записи
} 
                                                               
void loop()
{
  unsigned int connection_status = modbus_update(packets);     //  Циклично обновляем пакеты
  HTTP.handleClient();                                         //  Инициализация клиента (выводим web-страничку)                                                        //
}

void HTTP_connection()
{           
  WiFi.mode(WIFI_AP);                                          //  Включаем режим точки доступа
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));  //  Маска сети
  WiFi.softAP(ssidAP.c_str(), passwordAP.c_str());             //  Подключение к точке доступа ssidAP с паролем paswordAP
  HTTP.on("/", Auntefication); 
  HTTP.on("/SubmitAuntefication", SubmitAuntefication);
  HTTP.onNotFound([]() {(404, "text/plain", "FileNotFound");});
  HTTP.on("/ConfigJSON", ConfigJSON);      
  HTTP.on("/typeSKZ", typeSKZ);                         
  HTTP.on("/bootstrapCSS", bootstrapCSS);
  HTTP.on("/Jquer", Jquer);
  HTTP.on("/functions", functions);
  HTTP.on("/Button_Uout", Button_Uout);                  
  HTTP.on("/Button_Iout", Button_Iout);                   
  HTTP.on("/Button_Upp", Button_Upp);               
  HTTP.on("/Button_Usp", Button_Usp);                  
  HTTP.on("/btnSITE", btnSITE);                     
  HTTP.on("/changePass", changePass); 
  HTTP.on("/VerificedPass", VerificedPass); 
  HTTP.on("/regUser", regUser); 
  HTTP.on("/delUser", delUser); 
  HTTP.on("/locarionSITE", locarionSITE); 
  HTTP.begin();                                   
}    

void Jquer(){
    HTTP.send(200, "text/json", Jquery);
}
     
void ConfigJSON() 
{    
  data1();
  data2();
  data3();
  data4();
  data5();
  data6();
  data7();
  data8();
  data9();
  String  json = "{";     
          json += "\"Uout\":\"";
          json += Uout;
          json += "\",\"Iout\":\"";
          json += Iout;
          json += "\",\"Upp\":\"";
          json += Upp;
          json += "\",\"Usp\":\"";
          json += Usp;
          json += "\",\"label2\":\"";
          json += label2;
          json += "\",\"label3\":\"";
          json += label3;
          json += "\",\"label4\":\"";
          json += label4;
          json += "\",\"label5\":\"";
          json += label5;
          json += "\",\"label6\":\"";
          json += label6;
          json += "\"}";
  HTTP.send(200, "text/json", json);
}             

void typeSKZ()
{
  uint16_t SKZType = regs1[16] >> 8;
  String Str = "";
  String In = "Блок ";
  String NumberStation = " · " + String(regs1[25]);
  String user = "";
  if(SKZType == 0) 
          Str = "ИПКЗ-РА";
  else if(SKZType == 1) Str = "ИПКЗ-М-РА";
  else if(SKZType == 2) Str = "В-ОПЕ-Ц";
  else if(SKZType == 3) Str = "ИПКЗ-РД";
  else if(SKZType == 4) Str = "ИПКЗ-М-РД";
  else if(SKZType == 5) Str = "В-ОПЕ-Ц-РД";
  else if(SKZType == 6) Str = "ИПКЗ-МС-РА";
  else if(SKZType == 7) Str = "ИПКЗ-Тверца";
  else if(SKZType == 8) Str = "ИПКЗ-ЕМ-РА";
  else if(SKZType == 9) Str = "ИПКЗ-Е-РА";
  else if(SKZType == 10) Str = "САУД";
  else if(SKZType == 11) Str = "КСЭР";
  else if(SKZType == 12) Str = "УДП";
  String  json = "{";     
          json += "\"SKZType\":\"";
          json += In + Str + NumberStation;
          json += "\",\"user\":\"";
          json += Login[0];
          json += "\",\"flagSubmit\":\"";
          json += FLAG_SubmitAuntefication;
          json += "\"}";
  HTTP.send(200, "text/json", json);
}

void Auntefication()
{
  HTTP.send(200, "text/html", sign);  
}

void SubmitAuntefication()
{           
  String inLogin = HTTP.arg("inputLogin");
  String inPassword = HTTP.arg("inputPassword");
  if ((inLogin == Login[0]) && (inPassword == Password[0])) 
          HTTP.send(200, "text/html", "true");  
  if ((inLogin != Login[0])||(inPassword != Password[0])) 
          HTTP.send(403, "text/html", "false");  
}     

void locarionSITE(){
  HTTP.send(403, "text/html", site);  
}

void bootstrapCSS()
{
  HTTP.send(200, "text/css", BOOTSTRAPcss );
}

void functions()
{
  HTTP.send(200, "text/javascript", FUNCTIONS );
}

String data1()                                                 //  Функция вывода Напряжения
{                                                              //  Разбиваем число из массива для чтения на элементы (Paramrter*)
  uint16 Parameter1 = regs1[0] / 10;                           //  Берём делитель (Paramrter1) - первое число
  uint16 Parameter2 = regs1[0] % 10;                           //  Берём остаток от деления (Paramrter2) - последнее число
  
  return Uout = String(Parameter1) + "." + String(Parameter2) + " В "; //  Добовляем между ними запятую                     
} 
                                                               
String data2()
{
  uint16 Parameter1 = regs1[1] / 10;
  uint16 Parameter2 = regs1[1] % 10;
  
  return Iout = String(Parameter1) + "." + String(Parameter2) + " А ";
}
                                                              
String data3()
{
  return Upp = (predata(regs1[28]) + " В ");                  // Переходим в функцию predata и выполняем вычисления, тоже для data4
}                                                             // regs1[28] - здесь нахядятся данные о Upp, но данные нужно преобразовать

String data4()
{
  return Usp = (predata(regs1[2]) + " В ");                   // regs1[2] = Usp
}
                                                               
String data5()
{
  uint32 count = regs1[7] << 16;
  uint32 res = count + regs1[8];
  return label2 = (String(res / 10) + " кВт ч ");             // Выводим Str с кодом 200
}

String data6()
{        
  String Str = "";                                         
  if(regs1[5] == 1)                                           // Если select* = true записываем в массив для записи по Modbus данные
  { 
    Str = "Напряжение (U)";
  }
  else if(regs1[5] == 2)                                      // Если select* = true записываем в массив для записи по Modbus данные
  {       
    Str = "Ток (I)";
  }
  else if(regs1[5] == 4)                                      // Если select* = true записываем в массив для записи по Modbus данные
  {     
    Str = "Поляризационный потенциал (ПП)";
  }  
  else if(regs1[5] == 3)                                      // Если select* = true записываем в массив для записи по Modbus данные
  {         
    Str = "Суммарный потенциал (СП)";
  } 
  return label3 = Str;
}

String data7()
{ 
  String Atr = "";
  byte count1 = 0;
  byte count2 = 0;

  if (regs1[5] == 1) // если выбран U
  {     
    if (regs1[6] < 10) 
    {   
      count1 = 0; 
      count2 = regs1[6];
      Atr = " А";
    }
    else if (regs1[6] >= 10)
    {
      count1 = regs1[6] / 10;
      count2 = regs1[6] % 10;
      Atr = " В";
    }

    label4 = String(count1) + "." + String(count2) + " В";
  }
  else if (regs1[5] == 2)  // если выбран I
  {
    if (regs1[6] < 10) 
    {   
      count1 = 0; 
      count2 = regs1[6];
      Atr = " А";
    }
    else if (regs1[6] >= 10)
    {
      count1 = regs1[6] / 10;
      count2 = regs1[6] % 10;
      Atr = " В";
    }

    label4 = String(count1) + "." + String(count2) + " А";
  }
  else if ((regs1[5] == 3))    // если выбран Usp
  { 
    byte param = regs1[6] / 10;
    String p = "";
    if (regs1[6] < 100) 
    {   
      count1 = 0; 
      count2 = regs1[6];
      Atr = " В";
    }
    else if (regs1[6] >= 100)
    {
      count1 = regs1[6] / 100;
      count2 = regs1[6] % 100;
      Atr = " В";
    }
    if (param % 10 == 0) p = "0";

    label4 = "-" + String(count1) + "." + p + String(count2) + " В";
  }
  else if (regs1[5] == 4)    // если выбран Upp 
  { 
    byte param = regs1[6] / 10;
    String p = "";
    if (regs1[6] < 100) 
    {   
      count1 = 0; 
      count2 = regs1[6];
      Atr = " В";
    }
    else if (regs1[6] >= 100)
    {
      count1 = regs1[6] / 100;
      count2 = regs1[6] % 100;
      Atr = " В";
    }
    if (param % 10 == 0) p = "0";

    label4 = "-" + String(count1) + "." + p + String(count2) + " В";
  }
  
  return label4;
}

String data8()
{                                       
  uint32 count = regs1[10] << 16;
  uint32 res = count + regs1[11];
                                                  
  return label5 = (String(res / 10) + " ч "); 
}

String data9()
{
  uint32 count = regs1[12] << 16;
  uint32 res = count + regs1[13];
                                                            
  return label6 = (String(res / 10) + " ч ");  
}

void Button_Uout()  
{                                                              //                                        
  slct1 = true;                                                //  Ставим флаги в true в зависимости от того какой режим выбрали на форме web-сайта 
  slct2 = false;                                               //
  slct3 = false;                                               //
  slct4 = false;                                               //
                                                               //
  HTTP.send(200, "text/html", "OK");                           //  Выводим на страничку RadioButton* ОК с колом 200
}                                                              //
                                                              
void Button_Iout()
{                                                              //
  slct1 = false;                                               //  
  slct2 = true;                                                // 
  slct3 = false;                                               // 
  slct4 = false;                                               // 
                                                               // 
  HTTP.send(200, "text/html", "OK");                           //
}                                                              //
                                                               
void Button_Upp()
{                                                              // 
  slct1 = false;                                               // 
  slct2 = false;                                               // 
  slct3 = true;                                                // 
  slct4 = false;                                               // 
                                                               // 
  HTTP.send(200, "text/html", "OK");                           // 
}                                                              // 
                                                                
void Button_Usp()
{                                                              // 
  slct1 = false;                                               // 
  slct2 = false;                                               // 
  slct3 = false;                                               // 
  slct4 = true;                                                // 
                                                               // 
  HTTP.send(200, "text/html", "OK");                           // 
}        
                      
void btnSITE()
{          
  Num = HTTP.arg("num");
  String s1, s2 = "";       
  bool f = false;         
  for(int i=0; i<5; i++)                                       // Если при нажатии на кнопку Submit введёно значение с запятой, то запятую не записываем в переменную s1
      if ( (Num[i] != '.') && (Num[i] != '-') ) {
        s1 += String( Num[i] );    
        s2 += String( Num[i] ); 
      }  
      else if(Num[i] == '.') f = true;
  if(f == false) {
    s1 += "0"; 
    s2 += "00";
  }
  else {
      if(String(Num[4]) == "") s2 += "0"; 
  }

  if(slct1)                                                    // Если select* = true записываем в массив для записи по Modbus данные
  {                                                            //
    regs2[0] = 1;                                              //
    regs2[1] = atoi(s1.c_str());                               //
  }                                                            // 
  else if(slct2)
  {                                                            //
    regs2[0] = 2;                                              //
    regs2[1] = atoi(s1.c_str());                               //
  }                                                            //
  else if(slct3)
  {                                                            //
    regs2[0] = 4;                                              //
    regs2[1] = atoi(s2.c_str());                               //
  }                                                            //
  else if(slct4)
  {                                                            //
    regs2[0] = 3;                                              //
    regs2[1] = atoi(s2.c_str());                               //
  }                                                            //

  HTTP.send(200, "text/html", "OK");                           // Выводим сайт с кодом 200
}

void changePass(){
  HTTP.send(200, "text/html", ChPass);  
}

void VerificedPass(){
    String Log = HTTP.arg("Log");  
    String LastPass = HTTP.arg("LastPass"); 
    String FutPass = HTTP.arg("FuturePass");

    for(uint8_t i=0; i<10 ; i++){
      if((Login[i] == Log)&&(Password[i] == LastPass)) {
        Password[i] = FutPass;   
        HTTP.send(200, "text/html", site);        
      }
    else 
    HTTP.send(200, "text/html", ChPassERROR); 
    }
}

void regUser(){
  HTTP.send(200, "text/html", rUser); 
}
/*
void regUserBTN(){
    String Log = HTTP.arg("Log");
    String Pass = HTTP.arg("LastPass"); 

    for(uint8_t i=0; i<10 ; i++){
      if((Login[i] == false)&&(Password[i] == false)){
        Password[i] = Pass;
        Login[i] = Log;          
      }
    }
    if((Login[9])&&(Password[9])) 
      HTTP.send(403, "text/html", rUser);
      else HTTP.send(200, "text/html", rUser);
}

*/
void delUser(){
  HTTP.send(200, "text/html", dUser); 
}

void delUserBTN(){
    String Log = HTTP.arg("Log");  
    String Pass = HTTP.arg("LastPass"); 

    for(uint8_t i=0; i<10 ; i++){
      if((Login[i] == Log)&&(Password[i] == Pass)) {
        Login[i] = "";
        Password[i] = "";   
        HTTP.send(200, "text/html", dUser);        
      }
    else 
    HTTP.send(403, "text/html", dUser); 
    }
}

String predata(uint16 count)                                   //  функция вывода символов на web-страничку
{                                                              //                             
  uint16 Number1;                                              //
  uint16 Number2;                                              //
  uint16 Number3;                                              //
  String Result;                                               //
                                                               //
  uint16 par1 = 501 - count;                                   //  0...499 = "-"
  uint16 par2 = count - 499;                                   //  501...1000 = "+"
                                                               //                                                        
  uint16 par00 = par1 / 100;                                   //  Первое отцательное чило (делим отрицательное чило на элементы, определяем сколько цифр в числе)
  uint16 par01 = par1 / 10;                                    //  Второе отцательное число
  uint16 count0 = par1 % 10;                                   //  Третье отцательное число
                                                               //
  uint16 par10 = par2 / 100;                                   //  Первое положительное число (тоже, что и для отцательных)
  uint16 par11 = par2 / 10;                                    //  Второе положительное число
  uint16 count1 = par2 % 10;                                   //  Третье положительное число
                                                               //
  if (count < 500)                                             //  если число отрицательное
  {                                                            //
    if (par00 != 0)                                            //  если число 3-значное (*00)
    {                                                          //
      Number1 = par00;                                         //  число 1
      Number2 = par01 % 10;                                    //  число 2
      Number3 = par1 % 10;                                     //  число 3
      Result = "-" +                                           //  Всё склеиваем и добавляем минус и запятую                          
               String(Number1) +                               //          
               "." +                                           //
               String(Number2) +                               //
               String(Number3);                                //
    }                                                          //
    else if (par01 != 0)                                       //  если число 2-значное (*0)
    {                                                          //
      Number1 = par01;                                         //  число 1
      Number2 = par1 % 10;                                     //  число 2
      Result = "-0." + String(Number1) + String(Number2);      //  Всё склеиваем и добавляем минус и запятую 
    }                                                          //
    else if (count0 != 0)                                      //  если число 1-значное (*)
    {                                                          //
      Number1 = par1;                                          //  число 1
      Result = "-0.0" + String(Number1);                       //  Всё склеиваем и добавляем минус и запятую 
    }                                                          //
  }                                                            //
  else if (count == 500)                                       //  если число равно 0 выводим 0,00
  {                                                            //
    Result = "0.00";                                           //
  }                                                            //
  else if (count > 500)                                        //  если число положительное
  {                                                            //
    if (par10 != 0)                                            //  если число 3-значное
    {                                                          //
      Number1 = par10;                                         //  Всё тоже, что и для положительных
      Number2 = par11 % 10;                                    //
      Number3 = par2 % 10;                                     //
      Result = String(Number1) +                               //
               "." +                                           //
               String(Number2) +                               //
               String(Number3);                                //
    }                                                          //
    else if (par11 != 0)                                       // если число 2-значное
    {                                                          //
      Number1 = par11;                                         //
      Number2 = par2 % 10;                                     //
      Result = "0." + String(Number1) + String(Number2);       //
    }                                                          //
    else if (count1 != 0)                                      // если число 1-значное
    {                                                          //
      Number1 = par2;                                          //
      Result = "0.0" + String(Number1);                        //
    }                                                          //
  }                                                            //
                                                               //
  return Result;                                               // возврящаем результат
}                                                              //                                  
