#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"

extern char SceneId_Ov6A6Room1;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Actor_SetWideSprite(s32 id, s32 wide);
void Actor_SetMovementTypeById(s32 id, s32 callback);
void Func_02001080(void);
void Func_020010a0(void);
void Func_02001348(s32 id, s32 x, s32 z);
void Func_02001350(s32 id, s32 x, s32 z, u16 angle);

static __inline__ void Func_02001350_(s32 id, s32 x, s32 z, u16 angle) {
  /* FAKEMATCH(wrapper): forwarded inline, so each call builds its angle
   * first; as plain calls, 14 of 121 instructions differ in tla-en. */
  Func_02001350(id, x, z, angle);
}

s32 Func_020010c0(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1) {
    if (!GameFlag_Test(0x300))
      Actor_SetWideSprite(64, 0);
    if (!GameFlag_Test(0x301))
      Actor_SetWideSprite(65, 1);
    if (!GameFlag_Test(0x302))
      Actor_SetWideSprite(66, 1);
    Func_02001080();
  } else {
    Func_020010a0();
  }
  if (GameFlag_Test(0xa5f)) {
    Func_02001348(20, 0, 0);
    Func_02001350_(14, PIXELS(248), PIXELS(480), FACING_EAST);
    if (!GameFlag_Test(0x90a)) {
      Func_02001348(17, 0, 0);
      Func_02001348(16, 0, 0);
      Func_02001348(18, 0, 0);
    } else {
      Func_02001350_(16, PIXELS(459), PIXELS(254), FACING_NORTHEAST);
      Func_02001350_(18, PIXELS(442), PIXELS(203), FACING_WEST);
      Func_02001350_(17, PIXELS(490), PIXELS(188), FACING_SOUTHEAST);
      Actor_GetById(17)->action = Actor_GetById(17)->x >> 16;
      Actor_GetById(17)->unk66 = Actor_GetById(17)->z >> 16;
      Actor_SetMovementTypeById(17, 2);
    }
  }
  return 0;
}
