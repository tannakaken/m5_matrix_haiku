#include <M5Atom.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <driver/adc.h>
#define WIDTH 5


// secret.hにgithubに入れない情報を収納している。
// const char *ssid = "wifi-ssid";
// const char *password = "wifi-password";
// 
// const String apiURL = "https://script.google.com/macros/s/MY_GOOGLE_APP_SCRIPT_WEB_API_DEPLOY_ID/exec";
// 動かしたければ、上のコード例を参考に自分の情報を入れたsecret.hを作る。
// サーバーはgoogle spreadsheetに紐づいた。google app scriptをWeb Appとしてデプロイしている。
// 詳しくは、https://github.com/tannakaken/m5_matrix_haiku_server にgoogle app scriptのコードがある。
// これを自分の環境に合わせて編集して、Web Appとしてデプロイすれば、URLが生成されるので、それをapiURLにすればよい。
#include "secret.h"

uint8_t disp[WIDTH][WIDTH];

void drawDisp() {
  for (int i = 0; i < WIDTH; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if (disp[i][j]) {
        M5.dis.drawpix(i * WIDTH + j, 0xffffff);
      } else {
        M5.dis.drawpix(i * WIDTH + j, 0x000000);
      }
    }
  }
}

/**
 * WiFiへの接続設定に使う。
 * 今はまだ使ってない。
 */ 
//void drawAlphanumeric(char c) {
//  switch (c) {
//    case 'a':
//      disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
//      break;
//    case 'b':
//      disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'c':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'd':
//      disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'e':
//      disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 'f':
//      disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'g':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'h':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 'i':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 'j':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 'k':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 'l':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 'm':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
//      disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 1;
//      disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
//      break;
//    case 'n':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
//      disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
//      disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 1;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
//      break;
//    case 'o':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'p':
//      disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'q':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 'r':
//      disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 's':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 't':
//      disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
//      disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'u':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'v':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'w':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 1;
//      disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 1;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case 'x':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
//      break;
//    case 'y':
//      disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case 'z':
//      disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
//      disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
//      break;
//    case '0':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case '1':
//      disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case '2':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case '3':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case '4':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 1; disp[3][3] = 1; disp[3][4] = 1;
//      disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case '5':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    case '6':
//      disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case '7':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case '8':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
//      break;
//    case '9':
//      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
//      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
//      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
//      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
//      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
//      break;
//    default:
//      Serial.println(String("unknown character:") + c);
//  }
//  drawDisp();
//}

void drawNumber(int i) {
  switch (i) {
    case 0:
      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
      break;
    case 1:
      disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
      break;
    case 2:
      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
      break;
    case 3:
      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
      break;
    case 4:
      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 1; disp[3][3] = 1; disp[3][4] = 1;
      disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
      break;
    case 5:
      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
      break;
    case 6:
      disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
      break;
    case 7:
      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
      break;
    case 8:
      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
      break;
    case 9:
      disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
      disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
      disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
      disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
      disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
      return;
    default:
      Serial.println(String("unknown number:") + i);
  }
  drawDisp();
}
uint8_t draw_kana(String rest) {
  Serial.println(rest);
  uint8_t progress = 2;
  if (rest.startsWith(" ")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
    progress = 1;
  } else if (rest.startsWith("a")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
    progress = 1;
  } else if (rest.startsWith("i")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
    progress = 1;
  } else if (rest.startsWith("u")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
    progress = 1;
  } else if (rest.startsWith("e")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
    progress = 1;
  } else if (rest.startsWith("o")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
    progress = 1;
  } else if (rest.startsWith("ka")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("ki")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 1; disp[3][2] = 1; disp[3][3] = 1; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ku")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ke")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ko")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("sa")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("si")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("su")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("se")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("so")) {
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
    progress = 2;
  } else if (rest.startsWith("ta")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ti")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("tu")) {
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("te")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("to")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("na")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ni")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("nu")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("ne")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 1; disp[3][3] = 1; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("no")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ha")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("hi")) {
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("hu")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("he")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("ho")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("ma")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("mi")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("mu")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("me")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("mo")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("ya")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("yu")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("yo")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("ra")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("ri")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ru")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("re")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ro")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("yu")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("wa")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("wo")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("nn")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("xa")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("xi")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("xu")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("xe")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("xo")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("xtu")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
    progress = 3;
  } else if (rest.startsWith("xya")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
    progress = 3;
  } else if (rest.startsWith("xyu")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
    progress = 3;
  } else if (rest.startsWith("xyo")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
    progress = 3;
  } else if (rest.startsWith("ga")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("gi")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 1; disp[3][1] = 1; disp[3][2] = 1; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("gu")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ge")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("go")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("za")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("zi")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("zu")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("ze")) {
    disp[0][0] = 0; disp[0][1] = 1; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("zo")) {
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 1; disp[0][4] = 0;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("da")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("di")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("du")) {
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("de")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("do")) {
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("ba")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("bi")) {
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("bu")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("be")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("bo")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 1;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("pa")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 1; disp[4][4] = 0;
  } else if (rest.startsWith("pi")) {
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 0; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 1; disp[4][3] = 1; disp[4][4] = 1;
  } else if (rest.startsWith("pu")) {
    disp[0][0] = 1; disp[0][1] = 1; disp[0][2] = 1; disp[0][3] = 1; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("pe")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("po")) {
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 1; disp[1][1] = 1; disp[1][2] = 1; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 1;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("ch")) { // 完了
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 1;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 0; disp[2][3] = 1; disp[2][4] = 0;
    disp[3][0] = 1; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 1; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("+")) { // 追加
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 1; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 1; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
    progress = 1;
  } else if (rest.startsWith("xx")) { // キャンセル
    disp[0][0] = 1; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 1;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 1; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 1; disp[3][4] = 0;
    disp[4][0] = 1; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 1;
  } else if (rest.startsWith("<-")) { // 戻る
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 1; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 1; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 1; disp[2][1] = 1; disp[2][2] = 1; disp[2][3] = 1; disp[2][4] = 1;
    disp[3][0] = 0; disp[3][1] = 1; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 1; disp[4][3] = 0; disp[4][4] = 0;
  } else if (rest.startsWith("tt")) { // 点 待ち受けに使う
    disp[0][0] = 0; disp[0][1] = 0; disp[0][2] = 0; disp[0][3] = 0; disp[0][4] = 0;
    disp[1][0] = 0; disp[1][1] = 0; disp[1][2] = 0; disp[1][3] = 0; disp[1][4] = 0;
    disp[2][0] = 0; disp[2][1] = 0; disp[2][2] = 1; disp[2][3] = 0; disp[2][4] = 0;
    disp[3][0] = 0; disp[3][1] = 0; disp[3][2] = 0; disp[3][3] = 0; disp[3][4] = 0;
    disp[4][0] = 0; disp[4][1] = 0; disp[4][2] = 0; disp[4][3] = 0; disp[4][4] = 0;
  }
  drawDisp();
  return progress;
}

void drawHaiku(String haiku) {
  for (uint8_t i = 0; i < haiku.length();) {
    i += draw_kana(haiku.substring(i));
    delay(500);
  }
  draw_kana(" ");
}

enum AppState {
  HaikuSelect,
  GyoSelect,
  DanSelect,
};

enum AppState current_state = HaikuSelect;
int num_of_haiku = 0;

uint8_t current_i = 0;
const uint8_t gyo_length = 22;
String gyo[gyo_length] = {"a", "ka", "sa", "ta", "na", "ha", "ma", "ya", "ra", "wa", "nn", "ga", "za", "da", "ba", "pa", "xa", "xya", "xtu", "<-", "ch", "xx"};
String dans[gyo_length][6] = {
  {"a", "i", "u", "e", "o", "<-"},
  {"ka", "ki", "ku", "ke", "ko", "<-"},
  {"sa", "si", "su", "se", "so", "<-"},
  {"ta", "ti", "tu", "te", "to", "<-"},
  {"na", "ni", "nu", "ne", "no", "<-"},
  {"ha", "hi", "hu", "he", "ho", "<-"},
  {"ma", "mi", "mu", "me", "mo", "<-"},
  {"ya", "yu", "yo", "<-"},
  {"ra", "ri", "ru", "re", "ro", "<-"},
  {"wa", "wo", "<-"},
  {"nn"},
  {"ga", "gi", "gu", "ge", "go", "<-"},
  {"za", "zi", "zu", "ze", "zo", "<-"},
  {"da", "di", "du", "de", "do", "<-"},
  {"ba", "bi", "bu", "be", "bo", "<-"},
  {"pa", "pi", "pu", "pe", "po", "<-"},
  {"xa", "xi", "xu", "xe", "xo", "<-"},
  {"xya", "xyu", "xyo", "<-"},
  {"xtu"},
  {"<-"}, // 戻る
  {"ch"}, // 完了
  {"xx"}, // キャンセル
};
uint8_t dan_lengths[gyo_length] = {
  6, // あ
  6, // か
  6, // さ
  6, // た
  6, // な
  6, // は
  6, // ま
  4, // や
  6, // ら
  3, // わ
  1, // ん
  6, // が
  6, // ざ
  6, // だ
  6, // ば
  6, // ぱ
  6, // ぁ
  4, // ゃ
  1, // っ
  1, // <-
  1, // ch
  1, // xx
};
String *current_dan = NULL;
uint8_t current_dan_length = 0;
uint8_t current_j = 0;

String current_kana = "a";

String current_haiku = "";

void postHaiku(String haiku) {
  HTTPClient http;
  http.begin(apiURL);
  http.addHeader("Content-Type", "text/plain");
  http.POST(haiku);
  http.end();
}

void deleteHaiku(uint8_t haikuNum) {
  HTTPClient http;
  http.begin(apiURL);
  http.addHeader("Content-Type", "text/plain");
  String message = String("delete:") + haikuNum;
  http.POST(message);
  http.end();
}

/**
 * gasをWebAPIとして使おうとするとき、
 * text/plainでresponseを返すとなぜか302でリダイレクトになる。
 * リダイレクトの対応をしてもよかったのだが、面倒なので、***abc***のように、結果の文字列を***で挟んで、HTMLとして返す（これならリダイレクトにならない）。
 * 
 * 問題としては、responseが大きくなることと、結果を取り出すのが大変。
 * そこで***を検索して結果を取り出す。
 * これをHTMLの頭からやるととても遅いので、実験して大体これくらい、という位置から始める。
 */  
String getResultString(String totalResponse) {
  // uint8_tでは当然足りない（無限ループになる）。uint16_tで十分足りるという想定
  for (uint16_t i = 2000; i < totalResponse.length(); ++i) { // だいたいこんなもんの位置
    String subString = totalResponse.substring(i);
    if (subString.startsWith("***")) {
      // uint8_tで足りるという想定。もし足りなかったら無限ループになる。
      for (uint8_t j = 3; j < subString.length(); ++j) {
        String subSubString = subString.substring(j);
        if (subSubString.startsWith("***")) {
          return subString.substring(3, j);
        }
      }
    }
  }
  return "";
}

int getNumOfHaiku() {
  HTTPClient http;
  http.begin(apiURL);
  http.GET();
  String result = getResultString(http.getString());
  http.end();
  if (result == "") {
    return 0;
  } else {
    return result.toInt();
  }
}

String getHaiku(uint8_t haikuNum) {
  HTTPClient http;
  String baseUrl = apiURL + "?haikuNum=";
  String url = baseUrl + String(haikuNum);
  http.begin(url);
  http.GET();
  String result = getResultString(http.getString());
  http.end();
  return result;
}

uint8_t current_haiku_num = 0;

void drawCurrentHaikuNum() {
  if (current_haiku_num == num_of_haiku) {
    draw_kana("+");
  } else {
    drawNumber(current_haiku_num);
  }
}

void setup() {
  M5.begin(false, false, true);
  // https://www.mgo-tec.com/blog-entry-trouble-shooting-esp32-wroom.html/7
  // https://twitter.com/mongonta555/status/1404304353884663813
  // どうやらWiFi.begin()を使うと、M5StackをはじめとしたM5シリーズで時々ボタンをオンにする誤判定が起こるようだ。
  // <driver/adc.h>をincludeして、以下の関数を呼び出すと良いらしい。
  adc_power_acquire();
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    draw_kana("tt");
  }
  num_of_haiku = getNumOfHaiku();
  Serial.println(num_of_haiku);
  Serial.println("start");
  drawCurrentHaikuNum();
}

bool long_pressed = false;
bool pressed_and_released = false;

void blinkCurrentKana() {
  draw_kana(current_kana);
  delay(200);
  draw_kana(" ");
  delay(200);
  draw_kana(current_kana);
  delay(200);
  draw_kana(" ");
  delay(200);
  draw_kana(current_kana);
}

uint8_t nextHaikuNum() {
  if (num_of_haiku < 9) {
    return (current_haiku_num + 1) % (num_of_haiku + 1);
  } else {
    return (current_haiku_num + 1) % num_of_haiku;
  }
}

void onSinglePressed() {
  switch (current_state) {
    case HaikuSelect:
      current_haiku_num = nextHaikuNum();
      drawCurrentHaikuNum();
      return;
    case GyoSelect:
      current_i = (current_i + 1) % gyo_length;
      current_kana = gyo[current_i];
      draw_kana(current_kana);
      return;
    case DanSelect:
      current_j = (current_j + 1) % current_dan_length;
      current_kana = current_dan[current_j];
      draw_kana(current_kana);
      return;
  }
}

void onLongPressed() {
  switch (current_state) {
    case HaikuSelect:
      if (current_haiku_num == num_of_haiku) {
        current_state = GyoSelect;
        current_kana = "a";
        current_haiku = "";
        current_i = 0;
        draw_kana(current_kana);
      } else {
        draw_kana("tt");
        String haiku = getHaiku(current_haiku_num);
        drawHaiku(haiku);
        drawNumber(current_haiku_num);
      }
      return;
    case GyoSelect:
      current_dan_length = dan_lengths[current_i];
      if (current_dan_length == 1) {
        if (current_i == gyo_length - 3) { // <- 戻る
          current_haiku = current_haiku.substring(0, current_haiku.length() - 1);
          current_i = 0;
          current_kana = gyo[current_i];
          blinkCurrentKana();
          return;
        } else if (current_i == gyo_length - 2) { // ch 完了
          draw_kana("tt");
          postHaiku(current_haiku);
          ++num_of_haiku;
          current_haiku_num = 0;
          current_state = HaikuSelect;
          drawCurrentHaikuNum();
        } else if (current_i == gyo_length - 1) { // xx キャンセル
          current_haiku_num = 0;
          current_state = HaikuSelect;
          drawCurrentHaikuNum();
        } else {
          current_haiku += dans[current_i][0];
          current_i = 0;
          current_kana = gyo[current_i];
          blinkCurrentKana();
        }
      } else {
        current_j = 0;
        current_state = DanSelect;
        current_dan = dans[current_i];
        blinkCurrentKana();
      }
      return;
    case DanSelect:
      current_haiku += current_dan[current_j];
      current_i = 0;
      current_state = GyoSelect;
      current_kana = gyo[current_i];
      blinkCurrentKana();
      return;
  }
}

void onDoublePressed() {
  switch (current_state) {
    case HaikuSelect:
      draw_kana("tt");
      deleteHaiku(current_haiku_num);
      --num_of_haiku;
      current_haiku_num = 0;
      drawCurrentHaikuNum();
      return;
    case GyoSelect:
    case DanSelect:
      drawHaiku(current_haiku);
      delay(1000);
      blinkCurrentKana();
      return;
  }
}

void loop() {
  M5.update();
  if (M5.Btn.pressedFor(500)) {
    long_pressed = true;
  }
  if (pressed_and_released) {
    if (M5.Btn.releasedFor(200)) {
      Serial.println("single pressed");
      onSinglePressed();
      pressed_and_released = false;
    }
  }
  if (M5.Btn.wasReleased()) {
    if (long_pressed) {
       Serial.println("long pressed");
       onLongPressed();
    } else {
      if (pressed_and_released) {
        Serial.println("double pressed");
        onDoublePressed();
        pressed_and_released = false;
      } else {
        Serial.println("pressed");
        pressed_and_released = true;
      }
    }
    long_pressed = false;
  }
}
