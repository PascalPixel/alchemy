#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

extern s16 gData_02001c30[];

// 4 フレームごとにパレット 7 と 8 の色 2-14 を 1 つずつ送り、色 0x7f
// を表から選ぶ
s32 Func_02000630(void) {
  if ((*Ram_FrameCount & 3) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x050000fc;
    index = 14;
  first:
    palette = (u16 *)0x050000e0;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)0x050000e2 = saved;
    saved = *(volatile u16 *)0x0500011c;
    index = 14;
  second:
    palette = (u16 *)0x05000100;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)0x05000102 = saved;
  }
  {
    s16 color = gData_02001c30[(*Ram_FrameCount >> 2) % 12];

    *(volatile u16 *)0x050000fe = color;
    return color;
  }
}

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_Test(s32 flag);
u8 *Object_GetById(s32 id);
void Scheduler_AddOrUpdateCallback(s32 (*callback)(void), s32 priority);
void ObjectDispatch_SetSingleChildFlags(u8 *object, s32 flags);
void Func_0200195c(s32 id, s32 x, s32 z);
void Func_02000048(void);

static __inline__ void Func_0200195c_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline loads the arguments in the native
   * order (differ 2 at each call with coordinates as a direct call). */
  Func_0200195c(id, x, z);
}

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constant, as
   * The Broken Seal's scene layer does; called directly the two tests of 0x96d
   * share it in r5 (differ 15). */
  return GameFlag_Test(flag);
}

s32 Func_020006bc(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 513;
  Scheduler_AddOrUpdateCallback(Func_02000630, 0xc80);
  if (gPartyState.entrance != 1)
    GameFlag_ClearBit(0x12f);
  else
    GameFlag_SetBit(0x12f);
  if (GameFlag_Test(0x961)) {
    Func_0200195c_(8, 136 << 16, 584 << 16);
    Func_0200195c_(9, 0, 0);
    Func_0200195c_(10, 0, 0);
  }
  if (GameFlag_Test_(0x96d))
    Func_0200195c_(12, 312 << 16, 184 << 16);
  if (gPartyState.entrance == 1 && !GameFlag_Test_(0x96d) &&
      GameFlag_Test(0x96c)) {
    *(void (**)(void))(Object_GetById(13) + 108) = Func_02000048;
    *(u16 *)(Object_GetById(13) + 100) = 0;
  }
  ObjectDispatch_SetSingleChildFlags(Object_GetById(13), 0);
  return 0;
}
