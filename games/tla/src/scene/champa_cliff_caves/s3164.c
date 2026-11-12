#include "scene.h"
#include "task.h"

struct ChampaCliffCavesWork {
  u8 tiles[0x1000];
  s32 originX;
  s32 originY;
  s32 originZ;
  u8 spriteA[40];
  u8 spriteB[40];
  u16 entryA;
  u16 vramA;
  u16 entryB;
  u16 vramB;
  s32 field1064;
  s32 field1068;
  s32 field106c;
  s32 field1070;
  u16 field1074;
  u8 unk1076[2];
};

struct Vec3 {
  s32 x;
  s32 y;
  s32 z;
};

void *HeapSlot_AllocFar(s32 slot, s32 size);
void wait(s32 frames);
void Func_02002c88(void);
void Func_02002de4(void);

void Func_02003164(const struct Vec3 *origin) {
  struct ChampaCliffCavesWork *work =
      HeapSlot_AllocFar(220, sizeof(struct ChampaCliffCavesWork));
  s32 state;

  work->originX = origin->x;
  work->originY = origin->y;
  work->originZ = origin->z;
  Func_02002c88();
  work->field106c = 0;
  work->field1070 = 0;
  work->field1074 = 0;
  {
    s32 priority = 0x480;

    Task_Add(Func_02002de4, priority);
  }
  Sound_Play(139);
  state = 0;
  do {
    if (state == 0) {
      work->field1064 += 0x1000;
      if (work->field1064 > 0x14ccb) {
        work->field1064 = 0x14ccc;
        state = 999;
      }
    }
    wait(1);
  } while (state != 999);
}
