// 26 October 2026, under -mthumb-slow-loads: differ 4 of 102: the call's
// constants in another order; alchemy permute (5 to 6 minutes) and an empty asm
// or one-pass block after any one statement do not move it.
// Draft, differ 6 of 102: the leader steps through the opened cell and
// leaves by exit 23. Left: two constants built in another order.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct ObjectRuntime *object, struct ObjectRuntime *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct ObjectRuntime *object, s32 mode);
void Func_02003ba8(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct ObjectRuntime *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct ObjectRuntime *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
void Effect_AdvanceMotionAtLeaderPriority(struct ObjectRuntime *object);

/// The effect fields of an ObjectRuntime, as field_effect.h names them.
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
  void (*update)(struct ObjectRuntime *object);
};
#include "event_work.h"

extern struct ObjectRuntime *gData_02004420;

void FieldEvent_WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 mode);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void Func_020021b4(void);
void Func_02002bcc(void);
void Func_02003bc8(void);
void Func_02003bd0(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02003bd8(struct ObjectRuntime *object);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c70(s32 id, s32 x, s32 z, s32 angle);
void Func_02003cc8(s32 id, s32 emote, s32 frames);
void Func_02003d00(s32 exit);
void Func_02003d78(s32 skip);

void Func_02002a5c(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  u32 i;

  Func_02003c28();
  Func_02003d78(0);
  Call4((void (*)())Func_02003c70, gAdventure.currentOwner, 0xf80000, 0x880000,
        0xc000);
  Func_02003be0(65, 64, 15, 8, 1, 1);
  GameFlag_SetBit(0x245);
  Func_02003df8(161);
  Func_020021b4();
  ObjectMotion_SetAngleToward(9, gAdventure.currentOwner, 0);
  Func_02003cc8(9, 0x100, 20);
  Func_02002bcc();
  FieldEvent_WaitFrames(20);
  leader->angle = 0x4000;
  leader->motionFlags = 0;
  ObjectMotion_SetActionVariant(9, 3);
  ObjectMotion_SetActionVariant(gAdventure.currentOwner, 3);
  Object_SetModeById(gAdventure.currentOwner, 28);
  FieldEvent_WaitFrames(16);
  for (i = 0; i <= 39;) {
    if (i == 5)
      Func_02003df8(204);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    i++;
    leader->y += -0x9999;
    WaitFrames(1);
  }
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003d00(23);
}
