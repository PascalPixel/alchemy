#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

void Func_02001d74(void);
void Func_02001ccc(s32 scene, s32 entrance);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);
void Func_02001d64(s32 x, s32 z, s32 layer, s32 value);

void Func_02000538(void) {
  struct MainRoutineWork *party;

  Func_02001d74();
  Func_02001ccc((s32)&SceneId_Ov68b, 2);
  party = &gMainRoutineWork;

  party->unk2c2 = 2;
}

void Func_02000560(void) {
  s32 x;
  s32 z;
  s32 flag;
  s32 value;
  s32 old;

  flag = 0x900;
  /* FAKEMATCH(barrier): the empty asm on flag, so the flag's high part is
   * shifted before the two coordinates start; as the plain constant, the shift
   * comes after them, 2 of 33 instructions differ in tla-en. */
  asm volatile("" : "+l"(flag));
  x = 0x3580000;
  z = 0x3f80000;
  Event_SetFlag(flag + 0x63);
  value = Func_02001d5c(x, z, 0);
  /* FAKEMATCH(temporary): the first z copied to old, so the second z starts
   * before the first store call's arguments; as z reassigned after that call,
   * its shift comes late, 2 of 33 instructions differ in tla-en. */
  old = z;
  z = 0x4080000;
  Func_02001d64(x, old, 0, value | 64);
  Func_02001d64(x, z, 0, Func_02001d5c(x, z, 0) | 64);
}
