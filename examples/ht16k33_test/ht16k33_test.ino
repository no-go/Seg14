// Alternating display updates.
//
// Hardware setup: attach four 14-segment displays to a HT16K33 at address 0x70.
#include <Seg14.h>

Seg14 segments(4, 0x70);

enum Letters {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z};

void setup() {
  segments.init();
  segments.setBrightness(15);  // Max brightness.
  //segments.writeStream(Seg14::getUpper(H));
  //segments.writeStream(Seg14::getLower(A));
  //segments.writeStream(Seg14::getLower(S));
  //segments.writeStream(Seg14::getLower(E));
  segments.print("+9\xB0\x43");
  delay(2000);
  segments.print("\x94=g\x81");
  delay(2000);
  
  segments.print("    ");
  delay(200);
  segments.print("    N");
  delay(200);
  segments.print("   Ne");
  delay(200);
  segments.print(" Ner");
  delay(200);
  segments.print("Nerd");
  delay(600);
  segments.print("erd!");
  delay(200);
  segments.print("rd!?");
  delay(200);
  segments.print("d!? ");
  delay(200);
  segments.print("!?  ");
  delay(200);
  segments.print("?   ");
  delay(200);
}

uint8_t i = 0;
void loop() {
  segments.write(Seg14::getUpper(i % 26), 0);
  segments.write(Seg14::getLower(i % 26), 1);
  segments.write(Seg14::getDecimal(i % 10), 2);
  segments.write(Seg14::getCharacter(i),3);
  ++i;
  delay(300);
}
