#include "Seg14.h"

Seg14 segments(4, 0x70);

void format3digits(char * buf, double val) {
  int8_t pos = 0;
  if (val<0) {
    buf[pos++] = '-';
    val *= -1.0;
  }
  int8_t v = val/100;
  buf[pos++] = '0' + v;
  val = val - v*100;
  v = val/10;
  buf[pos++] = '0' + v;
  v = val - v*10;
  buf[pos++] = '0' + v;
}

void dblOut(Seg14 const & seg, double val) {
  char buf[4] = "    ";
  uint8_t v = 0;
  int8_t expo = 11;
  double checker = 100000000000.0;
  bool neg = false;
  if (val < 0) {
    neg = true;
    val *= -1.0;
  }
  while (expo>0) {
    if (val >= checker) {
      val /= checker/100.0;
      format3digits(buf, neg? -1*val : val);
      if (expo>8) {
        buf[3] = 'G';
      } else if (expo>5) {
        buf[3] = 'M';
      }
      
      if (neg) {
        segments.print(buf,0,0b00000010>>(expo%3));
      } else {
        segments.print(buf,0,0b00000001>>(expo%3));
      }
      return;
    }
    checker /= 10.0;
    expo--;
  }
  /*
  if (val > 0.0) {
    // 123.G
    if (val >= 100000000000.0) {
      val /= 1000000000.0;
      format3digits(buf, val);
      buf[3] = 'G';
      segments.print(buf,0,0b00000100);
    }
    // 12.3G
    else if (val >= 10000000000.0) {
      val /= 100000000.0;
      format3digits(buf, val);
      buf[3] = 'G';
      segments.print(buf,0,0b00000010);
    }
    // 1.23G
    else if (val >= 1000000000.0) {
      val /= 10000000.0;
      format3digits(buf, val);
      buf[3] = 'G';
      segments.print(buf,0,0b00000001);
    }
    // 123.M
    else if (val >= 100000000.0) {
      val /= 1000000.0;
      format3digits(buf, val);
      buf[3] = 'M';
      segments.print(buf,0,0b00000100);
    }
    // 12.3M
    else if (val >= 10000000.0) {
      val /= 100000.0;
      format3digits(buf, val);
      buf[3] = 'M';
      segments.print(buf,0,0b00000010);
    }
    // 1.23M
    // 123.k
    // 12.3k
    // 1234.
    // 123.4
    // 12.34
    // 1.234
    // 123.m
    // 12.3m
    // 1.23m
    // 123.u
    // 12.3u
    // 1.23u
    // 123.n
    // 12.3n
    // 1.23n
    // .123n
    
  } else {
      
    // -12.G
    // -1.2G
    // -.12G
    // -12.M
    // -1.2M
    // -.12M
    // -12.k
    // -1.2k
    // -123.
    // -12.3
    // -1.23
    // -.123
    // -.012
    // -1.2m
    // -12.u
    // -1.2u
    // -.12u
    // -12.n
    // -1.2n
    // -.12n
  }
  */
}

void setup() {
  segments.init();
  segments.setBrightness(15);
  dblOut(segments, -2555111.0);
  //segments.print("No ?");
}

void loop() {
}

