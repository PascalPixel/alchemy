#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "actor.h"
#include "text/msg_ids.h"

struct Actor *Actor_GetById(s32 id);
void Func_020002a8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000300(s32 message);
void Func_02000310(s32 actor, s32 a, s32 b);
void Func_02000318(s32 actor, s32 a);
void Func_02000328(s32 a, s32 b);
void Func_02000338(void);
void Func_02000340(s32 skip);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);

static inline void ObjArm(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(actor, angle, frames);
}

static inline void MapCopyAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020002a8(a, b, c, d, e, f);
}

static inline void ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ResetAndSetPositionInMode2(actor, x, z);
}

static inline s32 FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

void Func_020000c8(void) {
  if (FlagCheck(0x200) == 0) {
    Func_020002b8();
    Func_02000340(0);
    if (FlagCheck(0x301) == 0) {
      Func_02000328(0x18000, 0x3000);
      Camera_MoveWithinMapBounds(0xa40000, -1, 0x1040000, 1);
      ObjPlace(gAdventure.currentOwner, 158, 316);
    }
    Actor_GetById(8)->actionFlags &= ~1;
    Actor_GetById(9)->actionFlags &= ~1;
    Actor_ResetAndSetPositionInMode2(9, 148, 252);
    Actor_SetPositionAndReset(8, 170, 252);
    Actor_WaitForMoveById(9);
    Actor_GetById(8)->actionFlags |= 1;
    Actor_GetById(9)->actionFlags |= 1;
    ObjArm(8, 0x4000, 0);
    ObjArm(9, 0x4000, 0);
    Actor_WaitForMoveById(gAdventure.currentOwner);
    Actor_ArmCallback(gAdventure.currentOwner, 0xc000, 0);
    FlagSet(0x200);
    Func_02000338();
    MapCopyAttr(11, 9, 4, 1, 8, 14);
    if (FlagCheck(0x301) == 0) {
      Func_02000300((s32)MsgMadraNeedEldersPermission);
      FieldEvent_WaitFrames(10);
      Motion_SetVarCbAndRefresh(8, 1);
      Func_02000310(8, 0, 5);
      Motion_SetModeAndWaitAnimation(9, 3);
      Func_02000318(9, 0);
      FlagSet(0x301);
      Func_020002c0();
    }
  }
}
