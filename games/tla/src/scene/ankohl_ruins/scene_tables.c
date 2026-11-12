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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "ankohl_ruins.h"

extern const u8 gData_0200307c[];
extern const u8 gData_0200309c[];

const struct ActorPlacement *Func_02000344(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5)
    return (const struct ActorPlacement *)gData_0200307c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room8)
    return (const struct ActorPlacement *)gData_0200309c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "version.h"

void Func_02002e7c(s32 id, s32 a, s32 b);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_Start(s32 id, s32 a, s32 b) {
  Func_02002e7c(id, a, b);
}

void Func_02000384(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Start(gMainRoutineWork.currentOwner, 6, 0x1000);
  Event_PlaySound(0x136);
  Event_SetFlag(0x122);
  FieldEvent_SetExitRequest(13);
}

// Includes the shared Effect_AdvanceMotion in this scene.
// Neither it nor its caller Func_02000418 exists in the Japanese scene.
#if TLA_PATCH_1
#include "../../field/field_effect/advance_motion.c"
#endif

// The Japanese form is the older revision and is still on its listing.
#if TLA_PATCH_1

#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

struct LandingEffectOptions {
  s32 kind;
  s32 palette;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  s32 unk20;
  void (*update)();
};

struct LandingEventWork {
  u8 unk000[0x1ac];
  s32 field1ac;
};

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion();
void Func_02002e44(void);
void Func_02002dec(void);
void Func_02002e4c(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct LandingEffectOptions *options);

void Func_02000418(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct LandingEffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];

  Func_02002e44();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02002dec();
  System_Wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct LandingEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.scaleX = 0xa3d7;
  options.scaleY = 0xa3d7;
  i = 0;
loop:
  angle = i << 12;
  velocity[0] = Math_Cosine(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sin(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
  Func_0200015c(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                velocity[2], 0x1090001, &options);
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  ((struct LandingEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x204;
  Func_02002e4c();
}

#endif
