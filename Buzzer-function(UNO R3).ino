
//中音NTF 0为空拍
#define NTF0 -1
#define NTF1 350
#define NTF2 393	
#define NTF3 441
#define NTF4 495
#define NTF5 556
#define NTF6 624
#define NTF7 661

//高音NTFH
#define NTFH1 700
#define NTFH2 786
#define NTFH3 882
#define NTFH4 935
#define NTFH5 965
#define NTFH6 996
#define NTFH7 1023

//低音NTFL
#define NTFL1 175
#define NTFL2 196
#define NTFL3 221
#define NTFL4 234
#define NTFL5 262
#define NTFL6 294
#define NTFL7 330

//音符频率数组 
int tune[]=
{
	NTF3,NTF3,NTF3,NTF3,NTF3,NTF3,
	NTF3,NTF5,NTF1,NTF2,NTF3,NTF0,
	NTF4,NTF4,NTF4,NTF4,NTF4,NTF3,NTF3,NTF3,NTF3,
	NTF5,NTF5,NTF4,NTF2,NTF1,NTF0,

	NTFL5,NTF3,NTF2,NTF1,NTFL5,NTF0,NTFL5,NTFL5,
	NTFL5,NTF3,NTF2,NTF1,NTFL6,NTF0,
	NTFL6,NTF4,NTF3,NTF2,NTFL7,NTF0,
	NTF5,NTF5,NTF4,NTF2,NTF3,NTF1,NTF0,

	NTFL5,NTF3,NTF2,NTF1,NTFL5,NTF0,
	NTFL5,NTF3,NTF2,NTF1,NTFL6,NTF0,NTFL6,
	NTFL6,NTF4,NTF3,NTF2,NTF5,NTF5,NTF5,NTF5,
	NTF6,NTF5,NTF4,NTF2,NTF1,NTF0
};
//音符节拍数组
float durt[]=
{
	0.5,0.5,1,0.5,0.5,1,
	0.5,0.5,0.75,0.25,1.5,0.5,
	0.5,0.5,1,0.5,0.5,0.5,0.5,0.25,0.25,
	0.5,0.5,0.5,0.5,1.5,0.5,

	0.5,0.5,0.5,0.5,1,0.5,0.25,0.25,
	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,0.75,0.25,

	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,0.5,0.5,
	0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
	0.5,0.5,0.5,0.5,0.75,0.25
};
const int TrigPin = 15;
const int EchoPin = 16;
float cm;
//定义蜂鸣器引脚，音符长度变量
int buzzer_pin = 14;
int length;
int ledp=2;




//setup函数，初始化引脚，计算长度
void setup()
{
	pinMode(buzzer_pin, OUTPUT);
	length = sizeof(tune)/sizeof(tune[0]);
 
pinMode(TrigPin, OUTPUT);
pinMode(EchoPin, INPUT);
pinMode(ledp, OUTPUT);
}

//loop函数
void loop()
{
  digitalWrite(14, LOW);

digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
delayMicroseconds(2);
digitalWrite(TrigPin, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin, LOW);

cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm
cm = (int(cm * 100.0)) / 100.0; //保留两位小数
if (cm>=2 && cm<=10)
	//for循环演奏曲子
	for(int x=0;x<length;x++)
	{
		tone(buzzer_pin, tune[x]);
    digitalWrite(ledp,HIGH);
		delay(500*durt[x]);	
    digitalWrite(ledp,LOW);	
    delay(100*durt[x]);				//这里的500为控制每个音符的时长来定曲子的节奏
		noTone(buzzer_pin);
	}
	delay(500);								//开始下一轮循环的时间间隔
}

