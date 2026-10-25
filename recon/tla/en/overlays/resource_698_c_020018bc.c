// Draft, differ 2 of 81: overlay 698's copy of the function drafted in
// resource_6a1_c_02002010.c (overlay 6a1's Func_02002010, differ 2 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

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

extern struct JupiterLighthouseRoom8Puzzle gData_02002308;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001a24(void);
void Func_02001674(void);

void Func_020018bc(s32 x, s32 z, s32 effect) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = GameFlag_GetByte(gData_02002308.flag);
  s32 dz = GameFlag_GetByte(gData_02002308.flag + 8);

  if (dx != 0 || dz != 0) {
    struct ObjectRuntime *marker;

    GameFlag_SetByte(gData_02002308.flag, 0);
    GameFlag_SetByte(gData_02002308.flag + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02001a24();
    Func_02001674();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
        effect;
  }
}
