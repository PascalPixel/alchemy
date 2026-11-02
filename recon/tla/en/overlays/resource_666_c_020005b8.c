// Matches: differ 0 (tla-ja, 2026-11-01)
// The Japanese edition's map setup for this scene: the listing's section sits
// under .ifdef TLA_EDITION_JA, so measure with --edition tla-ja; tla-en has its
// own adopted form in games/tla/src/scene/alhafra_interiors/s05b8.c, which is
// guarded out of Japanese. This is the #else of that guard: the same body
// without flag 0x200 by the leader's position and without placing actors 31
// and 32. Named shape pairing with overlay 68F's Func_02002b04 is a false pair:
// that is a 17-spark spawn loop and shares nothing with this.
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "edition.h"

extern u8 gData_02001c90[], gData_02001c60[];
struct Actor *Actor_GetById(u32 id);
void Func_02000bac(s32 mode);
void Func_02000bb4();
void Func_02000578(void);
void Func_02000598(void);
s32 GameFlag_Test(s32 flag);
void Func_02000aec();

s32 Func_020005b8(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_GetById(16)->collisionFlags |= 4;
  Actor_GetById(16)->collisionFlags |= 16;
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpritePriority(31, 3);
  Actor_SetSpritePriority(14, 3);
  Actor_SetSpritePriority(15, 3);
  Func_02000bac(0);
  Call4(Func_02000bb4, (s32)gData_02001c90, 8, 15, 0x40000000);
  Call4(Func_02000bb4, (s32)gData_02001c60, 8, 15, 0x40000000);
  *(s32 *)(gData_02001c60 + 24) = 0x18000;
  Actor_GetById(14)->update = Func_02000578;
  Actor_GetById(15)->update = Func_02000598;
  if (GameFlag_Test(0x8a7)) {
    if (!GameFlag_Test(0x823) || GameFlag_Test(0x903)) {
      Call3(Func_02000aec, 31, 0, 0);
      Call3(Func_02000aec, 32, 0, 0);
    }
  } else if (GameFlag_Test(0x8ab)) {
    if (GameFlag_Test(0x971))
      Call3(Func_02000aec, 31, 0, 0);
  } else {
    Actor_SetSpritePriority(24, 2);
    Actor_SetSpritePriority(19, 3);
    Actor_SetSpritePriority(28, 2);
    Actor_SetSpritePriority(25, 3);
  }
  return 0;
}
