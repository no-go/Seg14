#include "Seg14.h"

Seg14 segments(4, 0x70);

// helper for 4 digits
void format4digits(char * buf, double val, int8_t startpos = 0) {
  if (val<0) {
    buf[startpos++] = '-';
    val *= -1.0;
  }
  int16_t v;
  if (val >= 1000) {
    v = val/1000;
    buf[startpos++] = '0' + v;
    val = val - v*1000;
  }
  v = val/100;
  buf[startpos++] = '0' + v;
  val = val - v*100;
  v = val/10;
  buf[startpos++] = '0' + v;
  if (startpos < 4) {
    v = val - v*10;
    buf[startpos] = '0' + v;
  }
}
/** 
 *  print/formats a double value
 *  
 *  range1 -0.01 nano .. 999 Giga -> try to show 3 digits
 *  range2 -99 Giga .. 0.04 nano
 *  range3 1.001 .. 9999          -> sometimes 4 digits possible (+)
 *  range4 -.123 .. -999          -> sometimes 3 digits possible (-)
 *  
 *  @issue sometimes M or k ... has to be used, if number is negative
 *  
 *  @todo make this via loop shorter
 */ 
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
  while (expo>5) {
    if (val >= checker) {
      val /= checker/100.0;
      format4digits(buf, neg? -1*val : val);
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
  
  if (val >= 100000) {
    val /= 1000.0;
    format4digits(buf, neg? -1*val : val);
    
    if (neg) {
      buf[3] = 'M';
      segments.print(buf,0,0b00000001);
    } else {
      buf[3] = 'k';
      segments.print(buf,0,0b00000100);
    }
  } else if (val >= 10000) {
    val /= 100.0;
    format4digits(buf, neg? -1*val : val);
    buf[3] = 'k';
    if (neg) {
      segments.print(buf,0,0b00000100);
    } else {
      segments.print(buf,0,0b00000010);
    }
  } else if (val >= 1000) {
    format4digits(buf, neg? -1*val : val);
    if (neg) {
      buf[3] = 'k';
      segments.print(buf,0,0b00000010);
    } else {
      segments.print(buf,0,0b00001000);
    }
  } else if (val >= 100) {
    val *= 10.0;
    format4digits(buf, neg? -1*val : val);
    if (neg) {
      segments.print(buf,0,0b00001000);
    } else {
      segments.print(buf,0,0b00000100);
    }
  } else if (val >= 10) {
    val *= 100.0;
    format4digits(buf, neg? -1*val : val);
    if (neg) {
      segments.print(buf,0,0b00000100);
    } else {
      segments.print(buf,0,0b00000010);
    }
  } else if (val >= 1) {
    val *= 1000.0;
    format4digits(buf, neg? -1*val : val);
    if (neg) {
      segments.print(buf,0,0b00000010);
    } else {
      segments.print(buf,0,0b00000001);
    }
  } else if (val >= 0.1) {
    if (neg) {
      val *= 10000.0;
      format4digits(buf, -1*val);
    } else {
      val *= 1000.0;
      format4digits(buf, val, 1);
      buf[0] = '0';
    }
    segments.print(buf,0,0b00000001);
  } else if (val >= 0.01) {
    if (neg) {
      val *= 1000.0;
      format4digits(buf, -1*val);
      segments.print(buf,0,0b00000001);
    } else {
      val *= 10000.0;
      format4digits(buf, val);
      buf[3] = 'm';
      segments.print(buf,0,0b00000010);
    }
  } else if (val >= 0.001) {
    val *= 100000.0;
    format4digits(buf, neg? -1*val : val);
    buf[3] = 'm';
    if (neg) {
      segments.print(buf,0,0b00000010);
    } else {
      segments.print(buf,0,0b00000001);
    }

    
  } else if (val >= 0.0001) {
    if (neg) {
      val *= 1000000.0;
      format4digits(buf, -1*val);
      buf[3] = 'm';
      segments.print(buf,0,0b00000001);
    } else {
      val *= 1000000.0;
      format4digits(buf, val);
      buf[3] = 0xE6; // mu
      segments.print(buf,0,0b00000100);
    }
  } else if (val >= 0.00001) {
    val *= 10000000.0;
    format4digits(buf, neg? -1*val : val);
    buf[3] = 0xE6; // mu
    if (neg) {
      segments.print(buf,0,0b00000100);
    } else {
      segments.print(buf,0,0b00000010);
    }
  } else if (val >= 0.000001) {
    val *= 100000000.0;
    format4digits(buf, neg? -1*val : val);
    buf[3] = 0xE6; // mu
    if (neg) {
      segments.print(buf,0,0b00000010);
    } else {
      segments.print(buf,0,0b00000001);
    }

  } else if (val >= 0.0000001) {
    if (neg) {
      val *= 1000000000.0;
      format4digits(buf, -1*val);
      buf[3] = 0xE6; // mu
      segments.print(buf,0,0b00000001);
    } else {
      val *= 1000000000.0;
      format4digits(buf, val);
      buf[3] = 'n';
      segments.print(buf,0,0b00000100);
    }
  } else if (val >= 0.00000001) {
    val *= 10000000000.0;
    format4digits(buf, neg? -1*val : val);
    buf[3] = 'n';
    if (neg) {
      segments.print(buf,0,0b00000100);
    } else {
      segments.print(buf,0,0b00000010);
    }
  } else if (val >= 0.000000001) {
    val *= 100000000000.0;
    format4digits(buf, neg? -1*val : val);
    buf[3] = 'n';
    if (neg) {
      segments.print(buf,0,0b00000010);
    } else {
      segments.print(buf,0,0b00000001);
    }
  } else {
    if (neg) {
      val *= 1000000000000.0;
      format4digits(buf, -1*val);
    } else {
      val *= 100000000000.0;
      format4digits(buf, val);
    }
    buf[3] = 'n';
    segments.print(buf,0,0b00000001);
  }
  
}

void setup() {
  segments.init();
  segments.setBrightness(15);
}

void loop() {
  dblOut(segments, 432000000000.0);
  delay(1000);
  dblOut(segments, 0.00000000004321);
  delay(1000);
  dblOut(segments, -0.00000000004321);
  delay(1000);
}

