#include "anemos_inner_sanctum_entrance.h"

extern const u8 gData_02001c1c[];
extern const u8 gData_02001c28[];

const u8 *Func_02000714(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov6b1Room1)
    return gData_02001c28;
  return gData_02001c1c;
}

struct AnemosInnerSanctumEntranceEventWork {
  u8 unk000[0x1ac];
  s32 route;
};

void Func_02000408(void);
void Func_02000b18(void);
void Func_02000ba4(s32 id, s32 value);
void Func_02000bd4(u16 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Clear(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_ClearBit(flag);
}

static __inline__ void SetObjectValue(s32 id, s32 value) {
  /* FAKEMATCH: the value pinned to r1: ⚓️ loads it before the id (movs r1; movs
   * r0); as a direct call or a plain forwarding inline the id comes first
   * (differ 10). */
  register s32 v asm("r1") = value;

  Func_02000ba4(id, v);
}

/// Sets the event route to 0x204. In the first room, gives objects 9 to 14 their
/// values and priority bits, and under flag 0x200 runs Func_02000494. Under flag
/// 0x201, runs a screen transition and Func_02000408, then clears the flag.
s32 Func_02000740(void) {
  ((struct AnemosInnerSanctumEntranceEventWork *)Ram_HeapSlots->eventWork)
      ->route = 0x204;
  if (gPartyState.scene == (s32)&SceneId_Ov6b1Room1) {
    Func_02000b18();
    Func_02000bd4(2);
    SetObjectValue(9, 0);
    SetObjectValue(10, 1);
    SetObjectValue(11, 2);
    SetObjectValue(12, 3);
    SetObjectValue(13, 128);
    SetObjectValue(14, 129);
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 0x40;
    Object_GetById(9)->priorityFlags |= 0xc0;
    Object_GetById(10)->priorityFlags |= 0x80;
    Object_GetById(11)->priorityFlags |= 0x80;
    Object_GetById(12)->priorityFlags |= 0x80;
    Object_GetById(13)->priorityFlags |= 0x80;
    Object_GetById(14)->priorityFlags |= 0x80;
    if (Flag_Test(0x200)) {
      Func_02000be0(1);
      Func_02000494();
    }
  }
  if (Flag_Test(0x201)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000408();
    Flag_Clear(0x201);
  }
  return 0;
}
