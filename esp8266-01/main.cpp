#include "mbed.h"
#include <stdio.h>
#include <string>
#include <SerialBase.h>


string mSSID="your-SSID";
string mPassword="your-pass";

string MC_ID="0";
string mRestKey="";

Serial mPc(USBTX, USBRX);
Timer mTimer;

AnalogIn inSen1(dp13);
AnalogIn inSen2(dp11);
AnalogIn inSen3(dp10);
AnalogIn inSen4(dp9 );

DigitalOut outRelary_1(dp14);
DigitalOut outRelary_2(dp28);
DigitalOut outRelary_3(dp26);
DigitalOut outRelary_4(dp25);

int mSTAT =0;
int mSTAT_CONECT=1;
int mSTAT_VALVE=2;
int mTmMax=30;

int mValveGet = 20;
int mCtOpen   = 0;

int mOK_CODE=1;
int mNG_CODE=0;
int mWait_Debug=5;

string mResponse3="";
string mOK_STR="OK";
string mKey_HEAD ="web-response1=";

//Struct
struct stParam{
 string stat;
 string statCode;
 string moi_num;
 string vnum_1;
 string vnum_2;
 string vnum_3;
 string vnum_4;
 string kai_num_1;
 string kai_num_2;
 string kai_num_3;
 string kai_num_4;
};

string conv_zeroStr(string src ,int width){
    string ret="00000000";
    ret +=src;
    int iNum = ret.length();
    ret = ret.substr(iNum-width);
    return ret;    
}
/*
wait_forKey
wCount : count (10 msec * wCount= Limit wait-time)
*/
bool wait_forKey(  string sKey, int wCount){
    bool ret=false;   
    
    //int iCt=0;
    string sRes="";
    
    if(mPc.readable()){
        while(1) {
            char c= mPc.getc();
            sRes +=c;
            int loc = sRes.find( sKey, 0 );
            if(loc != string::npos ){
                if(loc >= 0){
                    //mPc.printf("#find-loc= %d\n" , loc);
                    return true;
                }
            }
        }
    }
    return ret;
}

// init_proc
bool init_proc(){
    bool ret=false;
    mPc.printf("AT+CWMODE=1\r\n");
    wait(mWait_Debug);
    if(wait_forKey(  mOK_STR, 100)==false){
        return ret;
    }
    mPc.printf("AT+CWJAP=\"%s\",\"%s\"\r\n" ,mSSID ,mPassword);
    wait(6);
    if(wait_forKey(  mOK_STR, 300)==false){
        return ret;
    }

    ret=true;    
    return ret;
}

//
string get_message(){
    string ret="";
    
    ret  ="GET /api1234.php?dat_1=1234 HTTP/1.0\r\n";
    ret  +="Host: dns1234.com\r\n\r\n";
    return ret;
}
    
//main
int main() {
    mPc.baud( 115200 );
    mSTAT = mSTAT_CONECT;
    mTimer.start();
    
    while(1) {
        float sec= mTimer.read();
        if(mSTAT == mSTAT_CONECT){
            if(sec >= mTmMax){
                mTimer.reset();
                string msg=get_message();
                int len=msg.length();
                mPc.printf("AT+CIPSTART=\"TCP\",\"dns1234.com\",80\r\n");
                wait(mWait_Debug);
                mPc.printf("AT+CIPSEND=%d\r\n" , len);
                wait(mWait_Debug);
                mPc.printf("%s",msg);
                wait(mWait_Debug);
            }
        }
    }
}
