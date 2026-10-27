#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;
extern char SceneId_Ov681Room5;
extern u32 gData_02005970[];
extern u32 gData_020059a0[];
extern s32 gData_02005a54[];
extern u32 gData_02005afc[];
extern u32 gData_02005b8c[];
extern s32 gData_02005c7c[];

/* FAKEMATCH: the native reloads this counter for every use and masks it in the
 * register it was loaded into, as it does a volatile word; declared plain, the
 * mask is built in the constant's register (differ 12). */
extern volatile s32 gData_02005ee4;
extern const u8 gData_02005d6c[];
extern const u8 gData_02005e28[];
extern u8 gData_020023c4[];

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02004dac(struct ObjectRuntime *object, const u8 *script);
void Func_02005014(s32 cue);
struct ObjectRuntime *Object_GetById(u32 id);
u32 Func_02004904(void);
void Func_02004d9c(struct ObjectRuntime *object, s32 mode);
void GameFlag_SetBit(s32 flag);
void WaitTicks(s32 ticks);

/// Return the table for the current room.
const void *Func_020009e8(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov681Room2)
    return gData_02005a54;
  if (gPartyState.scene == (s32)&SceneId_Ov681Room3)
    return gData_02005afc;
  if (gPartyState.scene == (s32)&SceneId_Ov681Room4)
    return gData_02005b8c;
  if (gPartyState.scene == (s32)&SceneId_Ov681Room5)
    return gData_02005c7c;
  if (gPartyState.entrance > 8)
    return gData_020059a0;
  return gData_02005970;
}

void Func_02000a60(struct ObjectRuntime *object) {
  s32 value = 0;

  if ((gData_02005ee4 & 7) <= 0)
    value = 7;
  Animation_ApplyChildValues(object, value & 7);
  if ((gData_02005ee4 & 31) == 0) {
    object->scaleX = 0x10000;
    Func_02004dac(object, gData_02005e28);
    Func_02005014(106);
  }
  gData_02005ee4++;
}

void Func_02000aac(struct ObjectRuntime *object) {
  s32 value = 0;

  if ((gData_02005ee4 & 7) <= 0)
    value = 7;
  Animation_ApplyChildValues(object, value & 7);
  if ((gData_02005ee4 & 31) == 0) {
    object->scaleY = 0x10000;
    Func_02004dac(object, gData_02005d6c);
    Func_02005014(106);
  }
  gData_02005ee4++;
}

void Func_02000af8(s32 unused, u32 id) {
  struct ObjectRuntime *object = Object_GetById(id);
  u32 direction;
  /* FAKEMATCH: the state pointer pinned to r8 and loaded behind an empty asm,
   * with the direction read from r0 after it, loads first after the call;
   * without the r8 pin, 16 instructions differ in tla-en. */
  register s32 *state asm("r8");

  {
    register u32 ret asm("r0") = Func_02004904();
    s32 *address = (s32 *)(gData_020023c4 + 0x288);

    __asm__ volatile("" : : "l"(address));
    direction = ret;
    state = address;
  }
  object->collisionFlags = 0;
  Func_02004d9c(object, ((direction & 0xc000) >> 14) + 2);
  if (direction & 0x4000)
    object->update = (void (*)(void))Func_02000aac;
  else
    object->update = (void (*)(void))Func_02000a60;
  object->collisionFlags = 1;
  GameFlag_SetBit(*state + 0xa50);
  GameFlag_SetBit(0x200);
  WaitTicks(20);
}
