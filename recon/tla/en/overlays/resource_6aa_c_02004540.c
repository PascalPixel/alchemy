// Draft of overlay 6aa's Func_02004540, the twin of overlay 6a1's
// Func_02002010: its draft (resource_6a1_c_02002010.c) with this overlay's
// symbols: differ 2 of 81. Left: what that draft's header records.
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

extern struct JupiterLighthouseRoom8Puzzle Data_020068b4;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02004bf0(void);
void Func_020042f8(void);

void Func_02004540(s32 x, s32 z, s32 effect) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = GameFlag_GetByte(Data_020068b4.flag);
  s32 dz = GameFlag_GetByte(Data_020068b4.flag + 8);

  if (dx != 0 || dz != 0) {
    struct ObjectRuntime *marker;

    GameFlag_SetByte(Data_020068b4.flag, 0);
    GameFlag_SetByte(Data_020068b4.flag + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02004bf0();
    Func_020042f8();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}
