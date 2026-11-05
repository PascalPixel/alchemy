#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct HeapSlotWords {
  s32 unk00[8];
  u8 *map;
};

struct ScrollWork {
  s32 unk00[3];
  s32 height;
};

extern s32 Data_02006a1c;
extern s32 gData_02005a20[];

void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_0200577c(void);
void Func_020059f4(s32 sound);
struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);

void Func_02003c58(s32 mode) {
  struct ScrollWork *work = (struct ScrollWork *)(((struct HeapSlotWords *)Ram_HeapSlots)->map + 376);
  s32 speed = 0x18000;
  u32 count = 0;
  u32 i;
  s32 *ids = gData_02005a20;

  for (i = 0; i <= 4 && ids[i] != 64; i++) {
    Actor_GetById(ids[i])->motionFlags = 0;
    count++;
  }
  if (mode != 0) {
    Func_020059f4(223);
    i = 0;
    work->height += 0x18000;
    while (work->height < Data_02006a1c) {
      u32 j;

      for (j = 0; j < count; j++) {
        struct Actor *actor;

        Actor_GetById(ids[j])->z -= speed;
        actor = Actor_GetById(ids[j]);
        actor->targetZ = Actor_GetById(ids[j])->z;
      }
      if ((i & 3) == 3 && Data_02006a1c < speed * 15 * 2 + work->height)
        speed = speed * 15 / 16;
      if (speed <= 0xccb)
        speed = 0xccc;
      if (mode == 1 && i == 40)
        Camera_MoveWithinMapBounds(0x1200000, 0x680000, 0x1700000, 1);
      wait(1);
      i++;
      if (i > 119)
        break;
      work->height += speed;
    }
  }
  work->height = Data_02006a1c;
  Func_0200577c();
  wait(2);
}
