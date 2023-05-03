#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL);

#define mermi1ledpin 22
#define mermi2ledpin 23
#define mermi3ledpin 24
#define can1ledpin 25
#define can2ledpin 26
#define can3ledpin 27
#define atesbuttonpin 28
#define yukaributtonpin 29
#define asagibuttonpin 30
#define secimbuttonpin 31
#define yuzlerbasamak7segmentbaslangicpin 32
#define onlarbasamak7segmentbaslangicpin 39
#define birlerbasamak7segmentbaslangicpin 46
#define buzzerpin 53
extern uint8_t SmallFont[];
int oncekimatris[8][16]= {{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                         {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
int sonrakimatris[8][16];
int potvalue=0;
int ldrvalue=0;
int gemininyeri=0;
unsigned long simdikizaman=0;
unsigned long oncekizaman=0;
unsigned long dokunulmazliksuresi=0;
unsigned long zormodsuresi=0;
int beklemesuresi=400;
boolean yenisutuneklenecekmi=false;
boolean atesemri=false;
int mermihakki=3;
int canhakki=2;
int skor=0;
int bonusitemsayaci=0;
int caneklenecekmi=0;
boolean kaybedildimi=false;
boolean dokunulmazlikvarmi=false;
boolean oyunzormoddami=true;
boolean menuekraninagidilecekmi=true;


void setup() {
  // put your setup code here, to run once:
   randomSeed(analogRead(0));
   
  if(!myOLED.begin(SSD1306_128X64))
    while(1);   // In case the library failed to allocate enough RAM for the display buffer...
    
  //myOLED.setFont(SmallFont);
  myOLED.setFont(SmallFont);
  pinMode(mermi1ledpin,OUTPUT);
  pinMode(mermi2ledpin,OUTPUT);
  pinMode(mermi3ledpin,OUTPUT);
  pinMode(can1ledpin,OUTPUT);
  pinMode(can2ledpin,OUTPUT);
  pinMode(can3ledpin,OUTPUT);
  pinMode(buzzerpin, OUTPUT);
  pinMode(atesbuttonpin,INPUT);
  pinMode(yukaributtonpin,INPUT);
  pinMode(asagibuttonpin,INPUT);
  pinMode(secimbuttonpin,INPUT);
  for(int i=0;i<7;i++){
    pinMode(yuzlerbasamak7segmentbaslangicpin+i,OUTPUT);
     pinMode(onlarbasamak7segmentbaslangicpin+i,OUTPUT);
      pinMode(birlerbasamak7segmentbaslangicpin+i,OUTPUT);
  }
  Serial.println("Program basladi");
  Serial.begin(9600);
  //canhakki++;
}

void loop() {
  simdikizaman=millis();
  myOLED.clrScr();
  if(menuekraninagidilecekmi){
     menuekrani();
     menuekraninagidilecekmi=false;
      //yenibaslangic
      for(int i =0; i<8;i++){
        for (int j=0;j<16;j++){
          oncekimatris[i][j]=0;
        }
      }
      beklemesuresi=400;
      yenisutuneklenecekmi=false;
      atesemri=false;
       mermihakki=3;
       canhakki=3;
       skor=0;
      bonusitemsayaci=0;
      kaybedildimi=false;
      dokunulmazlikvarmi=false;
      caneklenecekmi=0;


     if(oyunzormoddami){
       Serial.println("Zor mod secildi");
     }
     else
     {
       Serial.println("Kolay mod secildi");
     }
  }
  /*for (int i=0; i<7; i++) {
  digitalWrite(yuzlerbasamak7segmentbaslangicpin+i, HIGH);
  delay(100);
  digitalWrite(onlarbasamak7segmentbaslangicpin+i, HIGH);
  delay(100);
  digitalWrite(birlerbasamak7segmentbaslangicpin+i, HIGH);
  delay(100);
  }*/
  
   potvalue=analogRead(A1);
  //Serial.println(potvalue);
  gemininyeri=7-(map(potvalue,0,1023,0,7));
  //Serial.println(gemininyeri);
  for(int i=0;i<8;i++){
    if(oncekimatris[i][15]==2){
      oncekimatris[i][15]=0; // geminin oldugu yeri sil
    }
  }

  ldrvalue=analogRead(A2);
  //Serial.print("LDRDEGER--->");
  //Serial.println(ldrvalue);

  
  if(oncekimatris[gemininyeri][15]==0){
    oncekimatris[gemininyeri][15]=2;//uzay gemisi yerleştirme komutu
    
     if(oncekimatris[gemininyeri][14]==3|| oncekimatris[gemininyeri][14]==4 || oncekimatris[gemininyeri][14]==5){ // engele önden çarparsa
          if(dokunulmazlikvarmi){ // dokunulmazlık yoksa
          Serial.println("DOkunulmazligin var");
          kaybedildimi=false;  //   dokunulmazlık varsa kaybedilemez        
          }
          else{
            Serial.println("Kaybettin");
           kaybedildimi=true; 
          }
           
       }
       
      if(oncekimatris[gemininyeri][14]==6){ // önden can hakkı alırsa
      Serial.println("Can hakki aldin");
      oncekimatris[gemininyeri][14]=0;
        if(canhakki<3 && canhakki>0){// büyüktür 0 biraz düşündürücü
         canhakki++;
         Serial.println("Canhakkin:");
         Serial.print(canhakki);
         }
     }
      
      if(oncekimatris[gemininyeri][14]==7){ // önden dokunulmazlık alırsa
      Serial.println("Dokunulmazlik aldin");
      oncekimatris[gemininyeri][14]=0;
        dokunulmazliksuresi=millis(); // dokunulmazlığın alındığı zamanı tutar
        /*if(millis()-dokunulmazliksuresi <= 1800){ // eğer 3 saniye geçmemişse
          dokunulmazlikvarmi=true; //dokunulmazlık vardır
          Serial.println("dokunulmazsin");
        }
        else{
          dokunulmazlikvarmi=false; // 3 saniyeyi geçerse dokunulmazlık yok
        }*/
      }
  }
  else{
    
    if(oncekimatris[gemininyeri][15]==3|| oncekimatris[gemininyeri][15]==4 || oncekimatris[gemininyeri][15]==5){ //engelin üstüne gelirse
         
         if(dokunulmazlikvarmi){ // dokunulmazlık yoksa
          Serial.println("DOkunulmazligin var");
          kaybedildimi=false;  
          }
          else{
            Serial.println("Kaybettin");
           kaybedildimi=true; // dokunulmazlık varsa kaybedilemez
          }
      }

    if(oncekimatris[gemininyeri][15]==6){ //can hakkının üstüne gelirse
      Serial.println("Can hakki aldin");
      oncekimatris[gemininyeri][15]=2;
      if(canhakki<3 && canhakki>0 ){// büyüktür 0 biraz düşündürücü
         canhakki++;
         Serial.print("canhakkin :");
         Serial.println(canhakki);
      }
    }

    if(oncekimatris[gemininyeri][15]==7){ // dokunulmazlık hakkının üstüne gelirse
      Serial.println("Dokunulmazlik aldin");
      oncekimatris[gemininyeri][15]=2;
      dokunulmazliksuresi=millis(); // dokunulmazlığın alındığı zamanı tutar
    }

     oncekimatris[gemininyeri][15]=2; // bütün kontroller bitince uzay gemisini geri getirmeye çalışcaz
  }

        if(millis()-dokunulmazliksuresi <= 1800){ // eğer 3 saniye geçmemişse
          dokunulmazlikvarmi=true; //dokunulmazlık vardır
          Serial.println("dokunulmazsin");
        }
        else{
          dokunulmazlikvarmi=false; // 3 saniyeyi geçerse dokunulmazlık yok
        }

  if(digitalRead(asagibuttonpin)==1){
    Serial.println("asagi");
    while(digitalRead(asagibuttonpin)==1){ }
  }
  if(digitalRead(yukaributtonpin)==1){
    Serial.println("yukari");
  }
  if(digitalRead(secimbuttonpin)==1){
    Serial.println("secim");
  }
  if(digitalRead(atesbuttonpin)==1){
    Serial.println("ates");
    if(mermihakki>0){
      atesemri=true;
    }
    while(digitalRead(atesbuttonpin)==1){ }
  }

 /* uzaygemisiciz(2,2); // x ve y
  uzaybosluguciz(1,2);
  mermiciz(2,0);
  hasarsizmeteorciz(3,3);
  hasarlimeteorciz(3,4);
  uzaycopluguciz(4,4);
  canhakkiciz(5,5);
  dokunulmazlikhakkiciz(6,6);*/
  silahgoster(mermihakki);
  cangoster(canhakki);
  //Serial.print("Canhakkigoster--->");
  //Serial.println(canhakki);

  if(canhakki<=0){
    menuekraninagidilecekmi=true;
  }

  if(bonusitemsayaci>13){
    Serial.println("Bonuslar eklendi");
      caneklenecekmi++;
    if(mermihakki<3){
     mermihakki++;
    }
    bonusitemsayaci=0;
  }

  

  yedisegmentdisplayeyaz(skor+1);

  for(int i=0;i<8;i++){
     for(int j=0;j<16;j++){
       if(oncekimatris[i][j]==0)
          uzaybosluguciz(j,i);
        else if(oncekimatris[i][j]==1)
          mermiciz(j,i);
        else if(oncekimatris[i][j]==2)
          uzaygemisiciz(j,i);
        else if(oncekimatris[i][j]==3)
          uzaycopluguciz(j,i);
        else if(oncekimatris[i][j]==4)
          hasarlimeteorciz(j,i);
        else if(oncekimatris[i][j]==5)
          hasarsizmeteorciz(j,i);
        else if(oncekimatris[i][j]==6)
          canhakkiciz(j,i);
        else if(oncekimatris[i][j]==7)
          dokunulmazlikhakkiciz(j,i);
     }
   }
  
  if(oyunzormoddami){
      if(simdikizaman-zormodsuresi >= 4000){// 10 saniyede bir tekrarlancak işlemler
     Serial.println("Oyun hizlandirildi");
     zormodsuresi=simdikizaman;
     beklemesuresi= (80*beklemesuresi)/100;
      }
  }
  else{
    beklemesuresi=400;
  }
  


 if(simdikizaman-oncekizaman >= beklemesuresi){// 1 bekleme süresinde 1 kez yapilacak işlemler (kolay modsa saniyede 1)
 Serial.println("Oyun Alani Hareketlendi");
    oncekizaman=simdikizaman;
    /*for(int i=0;i<8;i++){
     for(int j=0;j<16;j++){
       if(oncekimatris[i][j]==0)
          uzaybosluguciz(j,i);
        else if(oncekimatris[i][j]==1)
          mermiciz(j,i);
        else if(oncekimatris[i][j]==2)
          uzaygemisiciz(j,i);
        else if(oncekimatris[i][j]==3)
          uzaycopluguciz(j,i);
        else if(oncekimatris[i][j]==4)
          hasarlimeteorciz(j,i);
        else if(oncekimatris[i][j]==5)
          hasarsizmeteorciz(j,i);
        else if(oncekimatris[i][j]==6)
          canhakkiciz(j,i);
        else if(oncekimatris[i][j]==7)
          dokunulmazlikhakkiciz(j,i);
     }
   }*/
   if(atesemri==true){
     oncekimatris[gemininyeri][15]=1; // bu normalde 14 idi denemek için 15 yaptım çalıştı gibi
     atesemri=false;
     mermihakki--;
   }

     for(int i=0;i<8;i++){
       for(int j=0;j<16;j++){
         if(oncekimatris[i][j]==1){//mermileri bul

           if(oncekimatris[i][j-1]==0) // merminin ilerisi boşluksa
           {
             oncekimatris[i][j-1]=1;//mermiyi ileri taşı
             oncekimatris[i][j]=0; // arkadaki mermiyi sil uzay boşluğu yap
            
             if(oncekimatris[i][j-2]==0){//ileriye alınan merminin ilerisi boşluksa
                 oncekimatris[i][j-2]=1;//mermiyi ileri taşı
                  oncekimatris[i][j-1]=0; // arkadaki mermiyi sil uzay boşluğu yap
             }

             if(oncekimatris[i][j-2]==3||oncekimatris[i][j-2]==4 || oncekimatris[i][j-2]==6 || oncekimatris[i][j-2]==7)  // tek canlı engel varsa
             {
             oncekimatris[i][j-2]=0;//engeli yok et
             oncekimatris[i][j-1]=0; // mermiyi yok et
             }

             if(oncekimatris[i][j-2]==5) //2 canlı engel varsa
             {
             oncekimatris[i][j-2]=4;//engeli degistir
             oncekimatris[i][j-1]=0; // mermiyi yok et
             }

           }
           
           if(oncekimatris[i][j-1]==3||oncekimatris[i][j-1]==4 || oncekimatris[i][j-1]==6 || oncekimatris[i][j-1]==7) // tek canlı engel varsa
           {
             oncekimatris[i][j-1]=0;//engeli yok et
             oncekimatris[i][j]=0; // mermiyi yok et
           }

           if(oncekimatris[i][j-1]==5) //2 canlı engel varsa
           {
             oncekimatris[i][j-1]=4;//engeli degistir
             oncekimatris[i][j]=0; // mermiyi yok et
           }

         }
       }
     }
     if(kaybedildimi==true){    
       Serial.println("kaybedilme fonksiyonu");
       digitalWrite(buzzerpin, HIGH); //buzzer çalıştırma
       Serial.println("Buzzer calisti");
       delay(100);
       digitalWrite(buzzerpin, LOW);
       canhakki--;
       dokunulmazliksuresi=millis();
       Serial.println("Dokunulmazlik verildi 2");
       oncekimatris[gemininyeri][14]=0;
       oncekimatris[gemininyeri][15]=2;
       kaybedildimi=false;
     }


    int yenisutun[8]={0,0,0,0,0,0,0,0};

    if(yenisutuneklenecekmi){
   for(int i=0;i<8;i++){
     int randomengelkoy=random(7);
     if(randomengelkoy==0||randomengelkoy==4||randomengelkoy==5||randomengelkoy==6){
       yenisutun[i]=0;
       yenisutun[i+1]=0;
       // i++;
     }
      if(randomengelkoy==1){
       yenisutun[i]=5; // hasarsız meteor
       yenisutun[i+1]=0;
       i++;
       bonusitemsayaci++;
     }
     if(randomengelkoy==2){
       yenisutun[i]=3; // uzay çöplüğü 
       yenisutun[i+1]=0;
       i++;
       bonusitemsayaci++;
     }
     if(randomengelkoy==3){
       //Serial.println("CANKOYAGİRDİ");
       if(caneklenecekmi>0){
         Serial.print("Caneklenecekmi degiskeni before:");
         Serial.println(caneklenecekmi);
        yenisutun[i]=6; // can hakkı
        yenisutun[i+1]=0;
        i++;
        caneklenecekmi--;
        Serial.print("Caneklenecekmi degiskeni after:");
         Serial.println(caneklenecekmi);
       }
       else{
           yenisutun[i]=7; // dokunulmazlık
           yenisutun[i+1]=0;
           i++;
       }
       
     }
     
   }
   yenisutuneklenecekmi=false;
   }
   else{
     yenisutuneklenecekmi=true;
   }
   for(int i= 0 ; i<8;i++){
     for(int j=0; j<16;j++){
       if(j==0){
         sonrakimatris[i][j]=yenisutun[i];
       }
       else{
         sonrakimatris[i][j]=oncekimatris[i][j-1];
       }
     }
   }
   for(int i= 0 ; i<8;i++){
     for(int j=0; j<16;j++){
       oncekimatris[i][j]=sonrakimatris[i][j];
     }
   }
   skor++;
     
 }
myOLED.update();//simdilik altta
}

void menuekrani(){
   Serial.println("20 nisan 23:59 son hali");
  while(1){ // menü loop döngüsü
    myOLED.clrScr();
      if(oyunzormoddami){ // ekran basımı
        myOLED.print("-> Zor Mod <-", CENTER, 20);
        myOLED.print("Kolay Mod", CENTER, 4);
        myOLED.print("Puan:" + String(skor), CENTER, 36);
      }
      else{
        myOLED.print("Zor Mod", CENTER, 20);
        myOLED.print("-> Kolay Mod <-", CENTER, 4);
        myOLED.print("Puan:" + String(skor), CENTER, 36);
      }

      if(digitalRead(asagibuttonpin)==1){
        Serial.println("asagi");
        oyunzormoddami=true;
        //while(digitalRead(asagibuttonpin)==1){ }
       }
      if(digitalRead(yukaributtonpin)==1){
       Serial.println("yukari");
       oyunzormoddami=false;
       //while(digitalRead(asagibuttonpin)==1){ }
       }
       if(digitalRead(secimbuttonpin)==1){
       Serial.println("secim");
       break;
       //while(digitalRead(asagibuttonpin)==1){ }
       }
    
    myOLED.update();
  }
}

void yedisegmentdisplayeyaz(int sayi){
  if(sayi>-1 && sayi<1000){
     int birler =  sayi % 10; // birler basamağındaki rakamı al
    int onlar = (sayi / 10) % 10; // onlar basamağındaki rakamı al
    int yuzler = (sayi/ 100) % 10; // yüzler basamağındaki rakamı al
    yedisegmentdisplaycalistir(birler,birlerbasamak7segmentbaslangicpin);
    yedisegmentdisplaycalistir(onlar,onlarbasamak7segmentbaslangicpin);
    yedisegmentdisplaycalistir(yuzler,yuzlerbasamak7segmentbaslangicpin);
  }
  else{
    Serial.println("7 Segment display icin hatali sayi girildi");
  }
}

void yedisegmentdisplaycalistir(int sayi,int displaybaslangici){
     if(sayi==0){
       digitalWrite(displaybaslangici  ,HIGH);
       digitalWrite(displaybaslangici+1,HIGH);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,HIGH);
       digitalWrite(displaybaslangici+4,HIGH);
       digitalWrite(displaybaslangici+5,HIGH);
       digitalWrite(displaybaslangici+6,LOW);
     }

     if(sayi==1){
       digitalWrite(displaybaslangici  ,LOW);
       digitalWrite(displaybaslangici+1,HIGH);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,LOW);
       digitalWrite(displaybaslangici+4,LOW);
       digitalWrite(displaybaslangici+5,LOW);
       digitalWrite(displaybaslangici+6,LOW);
     }

     if(sayi==2){
       digitalWrite(displaybaslangici  ,HIGH);
       digitalWrite(displaybaslangici+1,HIGH);
       digitalWrite(displaybaslangici+2,LOW);
       digitalWrite(displaybaslangici+3,HIGH);
       digitalWrite(displaybaslangici+4,HIGH);
       digitalWrite(displaybaslangici+5,LOW);
       digitalWrite(displaybaslangici+6,HIGH);
     }

     if(sayi==3){
       digitalWrite(displaybaslangici  ,HIGH);
       digitalWrite(displaybaslangici+1,HIGH);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,HIGH);
       digitalWrite(displaybaslangici+4,LOW);
       digitalWrite(displaybaslangici+5,LOW);
       digitalWrite(displaybaslangici+6,HIGH);
     }

     if(sayi==4){
       digitalWrite(displaybaslangici  ,LOW);
       digitalWrite(displaybaslangici+1,HIGH);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,LOW);
       digitalWrite(displaybaslangici+4,LOW);
       digitalWrite(displaybaslangici+5,HIGH);
       digitalWrite(displaybaslangici+6,HIGH);
     }

     if(sayi==5){
       digitalWrite(displaybaslangici  ,HIGH);
       digitalWrite(displaybaslangici+1,LOW);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,HIGH);
       digitalWrite(displaybaslangici+4,LOW);
       digitalWrite(displaybaslangici+5,HIGH);
       digitalWrite(displaybaslangici+6,HIGH);
     }

     if(sayi==6){
       digitalWrite(displaybaslangici  ,HIGH);
       digitalWrite(displaybaslangici+1,LOW);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,HIGH);
       digitalWrite(displaybaslangici+4,HIGH);
       digitalWrite(displaybaslangici+5,HIGH);
       digitalWrite(displaybaslangici+6,HIGH);
     }

     if(sayi==7){
       digitalWrite(displaybaslangici  ,HIGH);
       digitalWrite(displaybaslangici+1,HIGH);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,LOW);
       digitalWrite(displaybaslangici+4,LOW);
       digitalWrite(displaybaslangici+5,LOW);
       digitalWrite(displaybaslangici+6,LOW);
     }

     if(sayi==8){
       digitalWrite(displaybaslangici  ,HIGH);
       digitalWrite(displaybaslangici+1,HIGH);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,HIGH);
       digitalWrite(displaybaslangici+4,HIGH);
       digitalWrite(displaybaslangici+5,HIGH);
       digitalWrite(displaybaslangici+6,HIGH);
     }

     if(sayi==9){
       digitalWrite(displaybaslangici  ,HIGH);
       digitalWrite(displaybaslangici+1,HIGH);
       digitalWrite(displaybaslangici+2,HIGH);
       digitalWrite(displaybaslangici+3,HIGH);
       digitalWrite(displaybaslangici+4,LOW);
       digitalWrite(displaybaslangici+5,HIGH);
       digitalWrite(displaybaslangici+6,HIGH);
     }
}
void uzaygemisiciz(int matrisI,int matrisJ)
{
   int matris[8][8] = {
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {1, 0, 1, 0, 0, 1, 0, 1},
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 1, 0, 1}
  };
  
  if(dokunulmazlikvarmi){ // dokunulmazlık varsa kalkan ekle
    for(int i=1;i<7;i++){
      matris[0][i]=1;
    }
    matris[1][0]=1;
    matris[1][7]=1;
  } 

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (matris[col][row] == 1) {
        if(ldrvalue>500){
           myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row); // karanlık
        }
        else {
             myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row); //aydınlık
        }
        
      }
      if (matris[col][row] == 0) {
        if(ldrvalue>500){
          myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        else{
          myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        
      }
    }
  }
}

void uzaybosluguciz(int matrisI,int matrisJ)
{
   int matris[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  };
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
     if (matris[col][row] == 1) {
        if(ldrvalue>500){
           myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row); // karanlık
        }
        else {
             myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row); //aydınlık
        }
        
      }
      if (matris[col][row] == 0) {
        if(ldrvalue>500){
          myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        else{
          myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        
      }
    }
  }
}

void mermiciz(int matrisI,int matrisJ)
{
   int matris[8][8] = {
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
  };
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
     if (matris[col][row] == 1) {
        if(ldrvalue>500){
           myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row); // karanlık
        }
        else {
             myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row); //aydınlık
        }
        
      }
      if (matris[col][row] == 0) {
        if(ldrvalue>500){
          myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        else{
          myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        
      }
    }
  }
}

void hasarsizmeteorciz(int matrisI,int matrisJ)
{
   int matris[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 0, 1, 1},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  };
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (matris[col][row] == 1) {
        if(ldrvalue>500){
           myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row); // karanlık
        }
        else {
             myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row); //aydınlık
        }
        
      }
      if (matris[col][row] == 0) {
        if(ldrvalue>500){
          myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        else{
          myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        
      }
    }
  }
}

void hasarlimeteorciz(int matrisI,int matrisJ)
{
   int matris[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 0},
    {0, 1, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 0, 1, 1, 0},
    {0, 0, 1, 1, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  };
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
     if (matris[col][row] == 1) {
        if(ldrvalue>500){
           myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row); // karanlık
        }
        else {
             myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row); //aydınlık
        }
        
      }
      if (matris[col][row] == 0) {
        if(ldrvalue>500){
          myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        else{
          myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        
      }
    }
  }
}

void uzaycopluguciz(int matrisI,int matrisJ)
{
   int matris[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  };
    for(int i=0; i<32;i++){
      int randomi=random(8);
      int randomj=random(8);
      if(matris[randomi][randomj]==0){
        matris[randomi][randomj]=1;
      }
    }  
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (matris[col][row] == 1) {
        if(ldrvalue>500){
           myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row); // karanlık
        }
        else {
             myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row); //aydınlık
        }
        
      }
      if (matris[col][row] == 0) {
        if(ldrvalue>500){
          myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        else{
          myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        
      }
    }
  }
}

void canhakkiciz(int matrisI,int matrisJ)
{
   int matris[8][8] = {
    {0, 1, 1, 0, 0, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
  };
      
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (matris[col][row] == 1) {
        if(ldrvalue>500){
           myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row); // karanlık
        }
        else {
             myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row); //aydınlık
        }
        
      }
      if (matris[col][row] == 0) {
        if(ldrvalue>500){
          myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        else{
          myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        
      }
    }
  }
}

void dokunulmazlikhakkiciz(int matrisI,int matrisJ)
{
   int matris[8][8] = {
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  };
      
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
     if (matris[col][row] == 1) {
        if(ldrvalue>500){
           myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row); // karanlık
        }
        else {
             myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row); //aydınlık
        }
        
      }
      if (matris[col][row] == 0) {
        if(ldrvalue>500){
          myOLED.clrPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        else{
          myOLED.setPixel((matrisI*8)+col,(matrisJ*8)+row);
        }
        
      }
    }
  }
}

void cangoster(int sayi){
  if(sayi==0){
    digitalWrite(can1ledpin,LOW);
    digitalWrite(can2ledpin,LOW);
    digitalWrite(can3ledpin,LOW);
  }
  if(sayi==1){
    digitalWrite(can1ledpin,HIGH);
    digitalWrite(can2ledpin,LOW);
    digitalWrite(can3ledpin,LOW);
  }
  if(sayi==2){
    digitalWrite(can1ledpin,HIGH);
    digitalWrite(can2ledpin,HIGH);
    digitalWrite(can3ledpin,LOW);
  }
  if(sayi==3){
    digitalWrite(can1ledpin,HIGH);
    digitalWrite(can2ledpin,HIGH);
    digitalWrite(can3ledpin,HIGH);
  }
}

void silahgoster(int sayi){
  if(sayi==0){
    digitalWrite(mermi1ledpin,LOW);
    digitalWrite(mermi2ledpin,LOW);
    digitalWrite(mermi3ledpin,LOW);
  }
  if(sayi==1){
    digitalWrite(mermi1ledpin,HIGH);
    digitalWrite(mermi2ledpin,LOW);
    digitalWrite(mermi3ledpin,LOW);
  }
  if(sayi==2){
    digitalWrite(mermi1ledpin,HIGH);
    digitalWrite(mermi2ledpin,HIGH);
    digitalWrite(mermi3ledpin,LOW);
  }
  if(sayi==3){
    digitalWrite(mermi1ledpin,HIGH);
    digitalWrite(mermi2ledpin,HIGH);
    digitalWrite(mermi3ledpin,HIGH);
  }
}
//20 nisan 23:59 son hali
