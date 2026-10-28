#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"

struct JupiterLighthouseRoom8Puzzle {
  s16 unk00;
  s16 flag;
};

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct ObjectRuntime *marker;
};

extern struct JupiterLighthouseRoom8Puzzle Data_0200354c;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002838(void);
void Func_02002450(void);

void Func_02002698(s32 x, s32 z, s32 effect) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = GameFlag_GetByte(Data_0200354c.flag);
  s32 dz = GameFlag_GetByte(Data_0200354c.flag + 8);

  if (dx != 0 || dz != 0) {
    struct ObjectRuntime *marker;

    GameFlag_SetByte(Data_0200354c.flag, 0);
    GameFlag_SetByte(Data_0200354c.flag + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02002838();
    Func_02002450();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}
