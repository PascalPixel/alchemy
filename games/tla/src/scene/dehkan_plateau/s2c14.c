#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};
#include "event_work.h"

extern struct Actor *gData_02004420;

void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020021b4(void);
void Func_02002bcc(void);
void Func_02003bc8(void);

void Func_02002c14(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *helper = Actor_GetById(9);

  Func_02003c28();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003bc8();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  Sound_Play(176);
  Func_020021b4();
  if (gData_02004420 != NULL) {
    FieldActor_SetPositionAndResetMotion(gData_02004420, 0xf80000, 0, 0x780000);
    wait(1);
    gData_02004420->speedLimit = 0x19999;
    gData_02004420->acceleration = 0x19999;
    gData_02004420->velocityY = 0x80000;
  }
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  leader->velocityY = 0x60000;
  helper->velocityY = 0x60000;
  FieldActor_SetMoveTarget(leader, 0xd80000, 0, 0x780000);
  FieldActor_SetMoveTarget(helper, 0x1180000, 0, 0x780000);
  if (gData_02004420 != NULL)
    FieldActor_SetMoveTarget(gData_02004420, 0xf80000, 0, 0x580000);
  Script_WaitForEventTimeout(leader);
  Event_Wait(20);
  Func_080d47b4(9, 0x103, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 60);
  Actor_WalkToAndWait(9, 296, 104);
  Actor_WalkToAndWait(9, 296, 80);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Event_Wait(20);
  if (gData_02004420 != NULL) {
    gData_02004420->motionFlags = 0;
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 5);
    wait(1);
  }
  GameFlag_SetBit(0x871);
  Func_02003c30();
}
