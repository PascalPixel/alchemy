/// @file
/// The scene code of overlay 0x681 that its scene tables belong to, in ROM order; the rest of the overlay is in gaia_rock_inside/.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"
#include "version.h"
#include "ram_buffer.h"
#include "system/field_effect.h"
#include "field_sprite.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern char SceneId_Ov681Room5;
extern const u8 gData_02005458[];

const u8 *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5)
    return gData_02005458;
  return 0;
}

struct Actor;

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
void Actor_JumpDownLedge(s32 id, s32 a, s32 b);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02004eec_(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

void Func_0200036c(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  OverlayObject_PrepareObject(0x2c80000, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(0x2c80000, 0, 0x1ea0000, 286);
  Func_02004eec_(gMainRoutineWork.currentOwner, 6, 0x800);
  Event_PlaySound(310);
  Event_SetFlag(290);
  FieldEvent_SetExitRequest(8);
}

// Includes the shared Effect_AdvanceMotion in this scene.
// Neither it nor its caller Func_0200042c exists in the Japanese scene.
#if TLA_PATCH_1
#include "../field/field_effect/advance_motion.c"
#endif

void Event_Start(void);
void Map_CenterOnFollowedPoint(void);
s32 Math_Cosine(s32 angle);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
s32 Func_02004e64(void);

// The Japanese edition has no form of this function here.
#if TLA_PATCH_1
s32 Func_0200042c(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;
  s32 spawn = 1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  // A goto loop carries no loop notes: as a do, for or while loop, loop.c
  // moves the velocity array's address out of the loop.
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Math_Cosine(angle);
    /* FAKEMATCH(dead): spawn is always 1, a constant GCSE only folds after its
     * partial redundancy pass, so that pass leaves the velocity array's
     * address in the loop, built after the cosine call as the native has it;
     * as the plain body, 4 of 177 instructions differ in tla-en. */
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Math_Sin(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
      velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
      Func_0200015c(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                    velocity[2], 0x1090001, &options);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  return Func_02004e64();
}
#endif

const u32 *Scene_GetExits(void) { return gSceneExits; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02004d9c();
s32 Actor_SetAnimationSpeed();

s32 Func_020005dc(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  Func_02004d9c(t0, 3);
  Actor_SetAnimationSpeed(t0, 0);
  return 0;
}

s32 Func_020005f8(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  FIELD_AT(FIELD_AT(t0, s32, 80), u16, 18) =
      (FIELD_AT(FIELD_AT(t0, s32, 80), u16, 18) + 0x300);
  return 0;
}

extern const u8 gData_020050d0[];

void Actor_StartScript(struct Actor *object, const u8 *script);

s32 Func_02000610(void) {
  struct FieldSprite *sprite = Actor_Get(9)->sprite;

  sprite->rotation -= 0x300;
  return 0;
}

/// Randomly vary the scale.
void Func_0200062c(struct Actor *object) {
  u32 random = Random16Far();
  s32 scale = object->scaleX;
  s32 bias;
  s32 limit;

  bias = -0x800;
  scale += (random << 12) >> 16;
  scale += bias;
  object->scaleX = scale;
  object->scaleY = object->scaleY + ((Random16Far() << 12) >> 16) + bias;
  limit = 240;
  scale = object->scaleX;
  limit <<= 4;
  limit += 255;
  if (scale <= limit)
    object->scaleX = 0x2000;
  if (object->scaleY <= limit)
    object->scaleY = 0x2000;
}

s32 Func_02000678(void) {
  struct Actor *source = Actor_Get(9);
  struct Actor *object;
  struct FieldSprite *sprite;

  object = (FieldActor_Create(14, source->x + 0x80000,
                                               source->y - 0x20000, source->z));
  object->motionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Actor_StartScript(object, gData_020050d0);
  object->scaleX = 0x6666;
  object->scaleY = 0x6666;
  sprite = object->sprite;
  sprite->priority = 1;
  object->bounce = 0;
  object->unk4c = 0;
  object->update = (void (*)(void))Func_0200062c;
  return 0;
}

extern u8 gData_02005544[];

s32 Func_020006e0(void) {
  Actor_StartScript(((s32)Actor_Get(9)), (s32)gData_02005544);
  return 0;
}

extern u8 gData_0200559c[];

s32 Func_020006f8(void) {
  Actor_StartScript(((s32)Actor_Get(9)), (s32)gData_0200559c);
  Actor_SetAttachedEffect(0xa, 0x102);
  return 0;
}

extern u8 gData_020055c0[];

s32 Func_02000718(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  Actor_StartScript(t0, (s32)gData_020055c0);
  FIELD_AT(t0, s32, 8) = 0x1d80000;
  Actor_SetAnimationSpeed(t0, 0x10);
  Actor_SetAttachedEffect(0xa, 0x102);
  return 0;
}

extern u8 gData_02005610[];

s32 Func_02000748(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  Actor_StartScript(t0, (s32)gData_02005610);
  Actor_SetAnimationSpeed(t0, 0);
  Actor_SetAttachedEffect(0xa, 0x102);
  return 0;
}

#include "../field/field_effect/advance_motion_priority.c"
