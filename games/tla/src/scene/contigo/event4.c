#include "contigo.h"

void Func_02001354(void) { Contigo_Tiles(29, 35, 1, 2, 23, 47, 0); }

void Func_02001374(void) { Contigo_Tiles(32, 55, 2, 3, 29, 58, 0); }

void Func_02001394(void) { Contigo_Tiles(3, 60, 1, 1, 3, 56, 0); }

void Func_020013b4(void) { Contigo_Tiles(7, 62, 2, 1, 7, 58, 0); }

void Func_020013d4(void) { Contigo_Tiles(10, 63, 2, 1, 10, 59, 0); }

void Func_020013f4(void) { Contigo_Tiles(32, 55, 3, 1, 40, 57, 0); }

void Func_02001414(void) { Contigo_Tiles(32, 55, 5, 1, 39, 58, 0); }

void Func_02001434(void) { Contigo_Tiles(32, 55, 5, 1, 39, 59, 0); }

void Func_02001454(void) { Contigo_Tiles(32, 55, 5, 1, 39, 60, 0); }

void Func_02001474(void) { Contigo_Tiles(32, 55, 3, 1, 40, 61, 0); }

void Func_02001494(void) {
  Contigo_CamSet(0, 0x2000000, 0x3700000, 0x4000000);
  GameFlag_SetBit(512);
}

void Func_020014b4(void) {
  Contigo_CamSet(0, 0x2000000, 0x2000000, 0x4000000);
  GameFlag_ClearBit(512);
}
