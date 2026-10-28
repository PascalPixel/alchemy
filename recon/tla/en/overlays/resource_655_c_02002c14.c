// Draft, differ 121 of 185 (was 133): the leader drops in with actor 9 and
// gData_02004420, ending with flag 0x871. The calls whose constants the
// native builds afresh go through CallN. Left: register allocation.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
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
struct Actor *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
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

void FieldEvent_Wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Actor_SetAngleToward(s32 id, s32 target, s32 mode);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Func_020021b4(void);
void Func_02002bcc(void);
void Func_02003bc8(void);
void Func_02003bd0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003bd8(struct Actor *object);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c70(s32 id, s32 x, s32 z, s32 angle);
void Func_02003cc8(s32 id, s32 emote, s32 frames);
void Func_02003d00(s32 exit);
void Func_02003d78(s32 skip);

void Func_02002c14(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  struct Actor *helper = Actor_GetById(9);

  Func_02003c28();
  Func_02003d78(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02003bc8();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003df8(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  Func_02003df8(176);
  Func_020021b4();
  if (gData_02004420 != NULL) {
    Actor_SetPositionAndResetMotion(gData_02004420, 0xf80000, 0, 0x780000);
    wait(1);
    gData_02004420->speedLimit = 0x19999;
    gData_02004420->acceleration = 0x19999;
    gData_02004420->velocityY = 0x80000;
  }
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeedParameters(9, 0x19999, 0xcccc);
  leader->velocityY = 0x60000;
  helper->velocityY = 0x60000;
  Func_02003bd0(leader, 0xd80000, 0, 0x780000);
  Func_02003bd0(helper, 0x1180000, 0, 0x780000);
  if (gData_02004420 != NULL)
    Func_02003bd0(gData_02004420, 0xf80000, 0, 0x580000);
  Func_02003bd8(leader);
  FieldEvent_Wait(20);
  Func_02003cc8(9, 0x103, 0);
  Actor_ArmCallback(gSequenceParentWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 60);
  Call3((void (*)())Actor_SetPositionAndReset, 9, 296, 104);
  Call3((void (*)())Actor_SetPositionAndReset, 9, 296, 80);
  Func_02003c68(9, 0, 0);
  FieldEvent_Wait(20);
  if (gData_02004420 != NULL) {
    gData_02004420->motionFlags = 0;
    Call6((void (*)())Func_02003bf0, 0, 0, 1, 1, 15, 5);
    wait(1);
  }
  GameFlag_SetBit(0x871);
  Func_02003c30();
}
