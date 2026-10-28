// Draft, differ 10 of 177 (was 49): overlay 653's Func_02002ec4 (the leader
// drops in with a burst of 17 effects) with this overlay's calls and 0xa3d7
// as the effects' scale. Left: as overlay 664's Func_0200505c (the count
// cleared between the options' palette and update there and here): the
// native clears the count after the options' stores and builds the
// velocity's address after the first call of each pass.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
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
#include "field_effect.h"
void Func_02003d78(s32 skip);
void Func_02003cd0(s32 id, s32 value);
void FieldEvent_Wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);

void Func_02000fc4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02003bc8(void);
void Func_02003c00(s32 a, s32 b, s32 c);
void Func_02003c08(void);
void Func_02003df8(s32 cue);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02003764(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;

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
  options.palette = 7;
  i = 0;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  for (; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Math_Cosine(i << 12);
    velocity[1] = 0;
    velocity[2] = Math_Sine(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14) >> 16;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
    Func_02000fc4(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                  velocity[2], 0x01090001, &options);
  }
  Func_02003df8(188);
  Func_02003cd0(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  Call3((void (*)())Func_02003c00, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02003c00, -1, -1, 0xe666);
  Func_02003c08();
  Func_02003cd0(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02003c30();
}
