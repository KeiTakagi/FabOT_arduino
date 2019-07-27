//
// FabOT 電光掲示板
// このプログラムはFabOT用の電光掲示板のプログラムになります。
// FabOTの活動につきましてはこちら(https://fablab-shinagawa.org/column/whatisot.html)
//
// LedMatrixにつきましてはこちら（http://blog.digit-parts.com/archives/51872590.html）
//
// Copyright (c) 2019 Kei Takagi
// Released under the MIT license
// https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
//
//

#include <MsTimer2.h>

#define WAIT0  500
#define WAIT1  50
#define WAIT2  8
#define WAIT3  2
#define WAIT4  1
#define WAIT5  4
#define WAIT6  20
#define WAIT9  2500

#define SIN1   4
#define SIN2   5
#define SIN3   6
#define CLOCK  7
#define LATCH  8
#define ENABLE 9


uint16_t LOGO[32] = {
  0x0030, 0x01e8, 0x0f0c, 0x7004, 0x8062, 0x9e32, 0xa331, 0x6a99,
  0x2ad9, 0x20f6, 0x6f80, 0xc060, 0x9c10, 0x55d0, 0x4d70, 0x6380,
  0x38e0, 0x2018, 0x4f04, 0x48e4, 0x2622, 0x23d2, 0x3006, 0xdc4c,
  0x8390, 0x8088, 0x7008, 0x1e08, 0x0388, 0x0088, 0x00d8, 0x0070
};
uint16_t buf[32];
int8_t Sync = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("START");
  Led32x16Init();

  for (uint8_t i = 0; i < 32; i++) {
    buf[i] = LOGO[i];
  }
  MsTimer2::set(1, Led32x16Display);     // 1ms毎にLed32x16Display( )割込み関数を呼び出す
  MsTimer2::start();                     // タイマー割り込み開始
  Sync = 0;
  digitalWrite(ENABLE, LOW);
}

void loop() {
  long randNo1;
  long randNo2;
  long randNo3;
  int i;
  for ( i = 0; i < 32; i++) buf[i] = 0x00;
  delay(WAIT1);
  /*
    s_roll2();
    delay(WAIT9);
    e_roll2();
    delay(WAIT9);
  */

  randNo1 = random(0, 11);
  randNo2 = random(0, 11);
  randNo3 = random(0, 20);
  switch (randNo1) {
    case 0:
      s_up();
      break;
    case 1:
      s_down();
      break;
    case 2:
      s_left();
      break;
    case 3:
      s_right();
      break;
    case 4:
      s_up2();
      break;
    case 5:
      s_down2();
      break;
    case 6:
      s_left2();
      break;
    case 7:
      s_right2();
      break;
    case 8:
      s_randomLogo();
      break;
    case 9:
      s_roll();
      break;
    case 10:
      s_roll2();
      break;
    default:
      s_up();
      break;
  }

  delay(WAIT9);

  if (randNo3 == 0) {

    for (int j = 0; j < 10; j++) {
      for ( i = 0; i < 30; i++) buf[i] = LOGO[i + 2];
      buf[30] = 0x00;
      buf[31] = 0x00;
      delay(WAIT6);
      for ( i = 0; i < 28; i++) buf[i] = LOGO[i + 4];
      buf[28] = 0x00;
      buf[29] = 0x00;
      delay(WAIT6);
      for ( i = 0; i < 30; i++) buf[i] = LOGO[i + 2];
      delay(WAIT6);
      for ( i = 0; i < 32; i++) buf[i] = LOGO[i];
      delay(WAIT6);
      for ( i = 0; i < 30; i++) buf[i + 2] = LOGO[i];
      buf[0] = 0x00;
      buf[1] = 0x00;
      delay(WAIT6);
      for ( i = 0; i < 30; i++) buf[i + 4] = LOGO[i];
      buf[2] = 0x00;
      buf[3] = 0x00;
      delay(WAIT6);
      for ( i = 0; i < 30; i++) buf[i + 2] = LOGO[i];
      delay(WAIT6);
      for ( i = 0; i < 32; i++) buf[i] = LOGO[i];
      delay(WAIT6);
    }
    delay(WAIT1);

  }

  switch (randNo2) {
    case 0:
      e_up();
      break;
    case 1:
      e_down();
      break;
    case 2:
      e_left();
      break;
    case 3:
      e_right();
      break;
    case 4:
      e_up2();
      break;
    case 5:
      e_down2();
      break;
    case 6:
      e_left2();
      break;
    case 7:
      e_right2();
      break;
    case 8:
      e_randomLogo();
      break;
    case 9:
      e_roll();
      break;
    case 10:
      e_roll2();
      break;
    default:
      e_up();
      break;
  }

  for ( i = 0; i < 32; i++) buf[i] = 0x00;
  delay(WAIT0);
}

void logo() {
  int i = 0;
  for ( i = 0; i < 32; i++) {
    buf[i] = LOGO[i];
  }
}

//-------------------------------------------------------

void s_roll2() {
  int i = 0, j = 0, x = 0, y = 0;
  word mask;

  for ( j = 7; j >= 0; j--) {
    x = j;
    y = j;
    //左上→右上
    for ( i = j; i < (32 - j); i++) {
      mask = 0x0001 << y;
      buf[x] |= (LOGO[x] & mask);
      x++;
      delay(WAIT5);
    }
    //右上→右下
    y++;
    x--;
    for ( i = j; i < (15 - j); i++) {
      mask = 0x0001 << y;
      buf[x] |= (LOGO[x] & mask);
      y++;
      delay(WAIT5);
    }
    //右下→左下
    y--;
    x--;
    for ( i = 31 - j; i > j; i--) {
      mask = 0x0001 << y;
      buf[x] |= (LOGO[x] & mask);
      x--;
      delay(WAIT5);
    }
    //左下→左上
    y--;
    x++;
    for ( i = (16 - j); i > j ; i--) {
      mask = 0x0001 << y;
      buf[x] |= (LOGO[x] & mask);
      y--;
      delay(WAIT5);
    }
  }
}

void e_roll2() {
  int i = 0, j = 0, x = 0, y = 0;
  word mask;

  for ( j = 7; j >= 0; j--) {
    x = j;
    y = j;
    //左上→右上
    for ( i = j; i < (32 - j); i++) {
      mask = 0x0001 << y;
      buf[x] &= ~(LOGO[x] & mask);
      x++;
      delay(WAIT5);
    }
    //右上→右下
    y++;
    x--;
    for ( i = j; i < (15 - j); i++) {
      mask = 0x0001 << y;
      buf[x] &= ~(LOGO[x] & mask);
      y++;
      delay(WAIT5);
    }
    //右下→左下
    y--;
    x--;
    for ( i = 31 - j; i > j; i--) {
      mask = 0x0001 << y;
      buf[x] &= ~(LOGO[x] & mask);
      x--;
      delay(WAIT5);
    }
    //左下→左上
    y--;
    x++;
    for ( i = (16 - j); i > j ; i--) {
      mask = 0x0001 << y;
      buf[x] &= ~(LOGO[x] & mask);
      y--;
      delay(WAIT5);
    }
  }
}

void s_roll() {
  int i = 0, j = 0, x = 0, y = 0;
  word mask;

  for ( j = 0; j < 8; j++) {
    x = j;
    y = j;
    //左上→右上
    for ( i = j; i < (32 - j); i++) {
      mask = 0x0001 << y;
      buf[x] |= (LOGO[x] & mask);
      x++;
      delay(WAIT5);
    }
    //右上→右下
    y++;
    x--;
    for ( i = j; i < (15 - j); i++) {
      mask = 0x0001 << y;
      buf[x] |= (LOGO[x] & mask);
      y++;
      delay(WAIT5);
    }
    //右下→左下
    y--;
    x--;
    for ( i = 31 - j; i > j; i--) {
      mask = 0x0001 << y;
      buf[x] |= (LOGO[x] & mask);
      x--;
      delay(WAIT5);
    }
    //左下→左上
    y--;
    x++;
    for ( i = (16 - j); i > j ; i--) {
      mask = 0x0001 << y;
      buf[x] |= (LOGO[x] & mask);
      y--;
      delay(WAIT5);
    }
  }
}

void e_roll() {
  int i = 0, j = 0, x = 0, y = 0;
  word mask;

  for ( j = 0; j < 8; j++) {
    x = j;
    y = j;
    //左上→右上
    for ( i = j; i < (32 - j); i++) {
      mask = 0x0001 << y;
      buf[x] &= ~(LOGO[x] & mask);
      x++;
      delay(WAIT5);
    }
    //右上→右下
    y++;
    x--;
    for ( i = j; i < (15 - j); i++) {
      mask = 0x0001 << y;
      buf[x] &= ~(LOGO[x] & mask);
      y++;
      delay(WAIT5);
    }
    //右下→左下
    y--;
    x--;
    for ( i = 31 - j; i > j; i--) {
      mask = 0x0001 << y;
      buf[x] &= ~(LOGO[x] & mask);
      x--;
      delay(WAIT5);
    }
    //左下→左上
    y--;
    x++;
    for ( i = (16 - j); i > j ; i--) {
      mask = 0x0001 << y;
      buf[x] &= ~(LOGO[x] & mask);
      y--;
      delay(WAIT5);
    }
  }
}

void s_randomLogo() {
  int i = 0, j = 0;
  word mask;
  for ( j = 0; j < 3000; j++) {
    mask = 0x0001 << random(0, 16);
    i = random(0, 32);
    buf[i] |= (LOGO[i] & mask);
    delay(WAIT4);
  }
  for ( i = 0; i < 32; i++)buf[i] = LOGO[i];
}

void e_randomLogo() {
  int i = 0, j = 0;
  word mask;
  for ( j = 0; j < 3000; j++) {
    mask = 0x0001 << random(0, 16);
    i = random(0, 32);
    buf[i] &= ~(LOGO[i] & mask);
    delay(WAIT4);
  }
  for ( i = 0; i < 32; i++)buf[i] = 0x00;
}

void s_left2() {
  int i = 0, j = 0;
  for (j = 0; j < 32; j++) {
    for (i = j; i < 32; i++) {
      buf[i] = LOGO[j];
      delay(WAIT3);
    }
  }
}

void e_left2() {
  int i = 0, j = 0;
  for (j = 0; j < 32; j++) {
    for (i = j; i >= 0 ; i--) {
      buf[i] = LOGO[j];
      delay(WAIT3);
    }
  }
}

void s_right2() {
  int i = 0, j = 0, k = 0;
  for (k = 32; k > 0; k--) {
    for (j = 0; j <= k; j++) {
      for (i = 0; i < j; i++) {
        buf[i] = LOGO[k - 1];
      }
      delay(WAIT3);
    }
  }
}

void e_right2() {
  int i = 0, j = 0, k = 0;
  for (j = 32; j > 0; j--) {
    for (i = j; i < 32; i++) {
      buf[i] = LOGO[j];
    }
    delay(WAIT1);
  }
}

void s_left() {
  int i = 0, j = 0, k = 0;
  for (j = 31; j >= 0; j--) {
    k = 0;
    for (i = j; i < 32; i++) {
      buf[i] = LOGO[k++];
    }
    delay(WAIT1);
  }
}

void e_left() {
  int i = 0, j = 0, k = 0;
  for (j = 32; j > 0; j--) {
    k = 32 - j;
    for (i = 0; i < j; i++) {
      buf[i] = LOGO[k++];
    }
    if (i < 32)buf[i] = 0x00;
    delay(WAIT1);
  }
}

void s_right() {
  int i = 0, j = 0;
  for (j = 0; j <= 32; j++) {
    for (i = 0; i < j; i++) {
      buf[i] = LOGO[32 - j + i];
    }
    delay(WAIT1);
  }
}

void e_right() {
  int i = 0, j = 0;
  for (j = 1; j < 32; j++) {
    for (i = 0; i < 32 - j; i++) {
      buf[j + i] = LOGO[i];
    }
    buf[j - 1] = 0x00;
    delay(WAIT1);
  }
}

void s_up2() {
  int i = 0, j = 0;
  for (i = 0; i < 32; i++) {
    for (j = 16; j >= 0; j--) {
      buf[i] = LOGO[i] << j;
      delay(WAIT2);
    }
  }
}

void e_up2() {
  int i = 0, j = 0;
  for (i = 0; i < 32; i++) {
    for (j = 0; j < 17; j++) {
      buf[i] = LOGO[i] >> j;
      delay(WAIT2);
    }
  }
}


void s_down2() {
  int i = 0, j = 0;
  for (i = 0; i < 32; i++) {
    for (j = 16; j >= 0; j--) {
      buf[i] = LOGO[i] >> j;
      delay(WAIT2);
    }
  }
}

void e_down2() {
  int i = 0, j = 0;
  for (i = 0; i < 32; i++) {
    for (j = 0; j < 17; j++) {
      buf[i] = LOGO[i] << j;
      delay(WAIT2);
    }
  }
}

void s_down() {
  int i = 0, j = 0;
  for (j = 16; j >= 0; j--) {
    for (i = 0; i < 32; i++) {
      buf[i] = LOGO[i] >> j;
    }
    delay(WAIT1);
  }
}

void e_down() {
  int i = 0, j = 0;
  for (j = 0; j < 17; j++) {
    for (i = 0; i < 32; i++) {
      buf[i] = LOGO[i] << j;
    }
    delay(WAIT1);
  }
}

void e_up() {
  int i = 0, j = 0;
  for (j = 0; j < 17; j++) {
    for (i = 0; i < 32; i++) {
      buf[i] = LOGO[i] >> j;
    }
    delay(WAIT1);
  }
}

void s_up() {
  int i = 0, j = 0;
  for (j = 16; j >= 0; j--) {
    for (i = 0; i < 32; i++) {
      buf[i] = LOGO[i] << j;
    }
    delay(WAIT1);
  }
}

void Led32x16Init() {
  pinMode(SIN1,   OUTPUT);
  pinMode(SIN2,   OUTPUT);
  pinMode(SIN3,   OUTPUT);
  pinMode(CLOCK,  OUTPUT);
  pinMode(LATCH,  OUTPUT);
  pinMode(ENABLE, OUTPUT);
}

void Led32x16Display() {
  int8_t j;
  Sync < 16 ? Sync++ : Sync = 0;
  for (j = 0; j < 16; j++) {
    digitalWrite(SIN1, ( 0x8000 >> Sync >> j ) & 0x0001);
    digitalWrite(SIN2, (*(buf + 15 - j) >> 15 - Sync) & 0x0001 );
    digitalWrite(SIN3, (*(buf + 31 - j) >> 15 - Sync) & 0x0001 );
    digitalWrite(CLOCK, LOW);
    digitalWrite(CLOCK, HIGH);
  }
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
}
