#include "sea_of_time.h"

void Func_020018a8(s32 x, s32 z, s32 actor) {
  Object_GetById(actor)->collisionFlags |= 16;
  Func_02001fdc(actor, (x << 19) + 0x80000, (z << 19) + 0x80000);
  x >>= 1;
  z >>= 1;
  Func_02000388(1, x, z, 0xff00);
  Func_02000388(1, x + 1, z, 0xff00);
  Func_02000388(1, x, z + 1, 0xff00);
  Func_02000388(1, x + 1, z + 1, 0xff00);
}

void Func_02001928(s32 x, s32 z, s32 actor) {
  struct SeaOfTimeSpark spark;
  s32 px;
  s32 pz;
  s32 i;

  Func_02001fec(0x60000, 0xc000);
  px = (x << 19) + 0x80000;
  pz = (z << 19) + 0x80000;
  SeaOfTime_CamMove(px, -1, pz, 1);
  Func_02001ffc();
  Func_02002054(144);
  SeaOfTime_Tint(0, 0x100000, 0x10000);
  SeaOfTime_Tint(-1, -1, 0xe666);
  spark.scaleX = 0x13333;
  spark.scaleY = 0x18000;
  spark.mode = 1;
  spark.life = 276;
  Func_0200015c(px, 0, pz, 0, 0, 0, 0x1a0000, &spark);
  Object_GetById(actor)->scale = 0x1999;
  Func_020018a8(x, z, actor);
  for (i = 0; i != 10; i++) {
    Object_GetById(actor)->scale += 0x1999;
    FieldEvent_WaitTicks(1);
  }
  Object_GetById(actor)->scale = 0x10000;
  FieldEvent_WaitTicks(20);
}
