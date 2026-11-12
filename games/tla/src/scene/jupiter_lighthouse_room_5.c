/// @file
/// The scene code of overlay 0x69e that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_5/.

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
#include "field_event.h"
#include "event_inline.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "macros.h"
#include "task.h"
#include "frame.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

extern u8 gData_02002fb8[];
void Func_020029d0();

void Func_02000054(void) { Func_020029d0(gData_02002fb8); }

void Func_02000e04(s32 id);
void Func_020029d8(void);

void Func_02000064(s32 unused, s32 id) {
  struct FieldActor *object;
  s32 i;

  object = ((struct FieldActor *)Actor_Get(id));
  if (id == 11) {
    if (!Event_GetFlag(0xa00) || object->x >> 20 != 13 || object->z >> 20 != 34)
      return;
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      System_Wait(1);
    }
    Func_02000e04(id);
    Event_SetFlag(0xa09);
  } else {
    Func_020029d8();
  }
}

u32 __umodsi3();
void Animation_ApplyChildValues();

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x19];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

void Func_020000c8(struct FieldActor *object) {
  struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;
  struct SpritePictures *pictures = sprite->pictures;

  switch (object->timer & 3) {
  case 0:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 2;
    break;
  case 1:
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 2:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 3:
    pictures->picture = 0;
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  }
  object->timer++;
}

void Func_02000120(struct FieldActor *object) {
  u32 frameCount = gFrameCount;

  if (frameCount & 1)
    Animation_ApplyChildValues(object, (frameCount >> 1) % 6);
}

s32 Math_Cosine(u32 angle);

/// Circles the object around actor 23, as overlay 6a5's Func_02000090 circles
/// actor 31.
void Func_02000148(struct FieldActor *self) {
  struct FieldActor *centre = ((struct FieldActor *)Actor_Get(23));
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Math_Cosine(angle) * (self->speed + 3);
  sine = Math_Sin(angle);
  z = centre->z;
  x = self->x;
  z += sine * 2;
  self->z = z;
  self->targetX = x;
  /* FAKEMATCH(dead): an empty once-only block, so the target z store stays
   * ahead of the timer load; without it, 6 of 40 instructions differ in
   * tla-en, the store falling behind (placed by alchemy permute). */
  do {
  } while (0);
  self->targetZ = z;
  *(s16 *)&self->timer += -0x800;
}

struct OrbitEffect {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 radius;
  u8 unk34[0x1c];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[0x0e];
  u16 angle;
  u16 unk66;
  u8 unk68[4];
  void (*update)(struct OrbitEffect *effect);
};

extern u32 gData_02003214[];

u32 Random16Far(void);
void Func_02002820(struct OrbitEffect *object, const void *script);

static inline struct OrbitEffect *CreateOrbitEffect(s32 type, s32 x, s32 y,
                                                    s32 z) {
  /* FAKEMATCH(wrapper): the effect created through an inline, which
   * evaluates the position before the type; as a direct call of
   * Import_ActorCreate, 4 of 94 instructions differ in tla-en. */
  return ((struct OrbitEffect *)Import_ActorCreate(type, x, y, z));
}

/// Every third frame, or every frame once flag 0x201 is set, starts a particle
/// circling actor 23 at a random height above it.
void Func_020001a4(void) {
  struct FieldActor *center;
  struct OrbitEffect *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(0x201) == 0 && gFrameCount % 3 != 0)
    return;
  center = ((struct FieldActor *)Actor_Get(23));
  if (Event_GetFlag(0x201) != 0) {
    u32 random;

    random = Random16Far();
    effect = CreateOrbitEffect(0x2a0, center->x,
                               (s32)((u32)(random << 8) >> 16 << 16) +
                                   center->y - 0x1c0000,
                               center->z);
  } else {
    u32 random;

    random = Random16Far();
    effect = CreateOrbitEffect(0x2a0, center->x,
                               (s32)((u32)(random << 6) >> 16 << 16) +
                                   center->y - 0x1c0000,
                               center->z);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Func_02002820(effect, gData_02003214);
    Actor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16Far() & 0xffff000;
    effect->angle = angle;
    effect->unk66 = 0;
    effect->update = (void (*)(struct OrbitEffect *))Func_02000148;
    effect->radius = Math_Sin((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 1;
  }
}

extern s32 gData_02003274;
extern s32 gData_02003278;

void Func_02001650();

void Func_02000294(void) {
  if (gData_02003278 != 0)
    Event_PlaySound(201);
  Func_02001650(gData_02003274);
}

extern const u8 gData_02002f70[];
extern const u8 gData_02002f94[];

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);

void Func_02001718(const s32 *from, const s32 *to);
void Func_02000d80(s32 value);
void Func_0200185c(void);

static __inline__ void Bring(s32 id) {
  struct Actor *object;

  Actor_SetSpritePriority(id, 2);
  Actor_SetPartAttribute(Actor_GetById(id), 7);
  object = Actor_GetById(id);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  object->scaleY = 0xffff0000;
  object->scaleX = 0x1999;
  Actor_SetMovementTypeById(id, gData_02002f70);
}

/// The event when the leader stands on cell (42, 41): sets flag 2560, turns the
/// leader (Actor_ArmCallbackAndRefresh), plays its hooks and sounds 220 and
/// 178, starts the spark ring from above the leader to (680, 0, 574)
/// (Func_02001718) and a pulse callback (Func_02000294) that quickens over 20
/// steps, sounds 390 and 144 with the screen shaking, objects 23 and 24 brought
/// in on their action scripts, flashes, the room's cells copied open
/// (Func_0802b450) and Func_02000d80(2); then the callbacks stop, objects 13 to
/// 16 rise 2 pixels a frame for 16 frames, take their next state
/// (Func_02000e04) and sound 80 plays.
void Func_020002b8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *a = Actor_GetById(13);
  struct Actor *b = Actor_GetById(14);
  struct Actor *c = Actor_GetById(15);
  struct Actor *d = Actor_GetById(16);
  s32 from[3];
  s32 to[3];
  s32 i;

  if (leader->x >> 20 != 42 || leader->z >> 20 != 41)
    return;
  Event_SetFlag(2560);
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  FieldEvent_Wait(30);
  leader->update = Func_02000120;
  FieldEvent_Wait(30);
  Event_PlaySound(220);
  Actor_SetAnimationSpeed(leader, 64);
  FieldEvent_Wait(50);
  Event_PlaySound(178);
  leader->action = 0;
  leader->update = Func_020000c8;
  FieldEvent_Wait(30);
  from[0] = leader->x;
  from[1] = leader->y + 0x140000;
  from[2] = leader->z;
  to[0] = 170 << 18;
  to[1] = 0;
  to[2] = 0x23e0000;
  Func_02001718(from, to);
  gData_02003274 = 0;
  gData_02003278 = 1;
  Task_AddFrame(Func_02000294, 0x480);
  for (i = 0; i < 20; i++) {
    gData_02003274 = 0;
    System_Wait((20 - i) / 2);
    gData_02003274 = 1;
    System_Wait(1);
  }
  Event_PlaySound(390);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  gData_02003274 = 1;
  FieldEvent_Wait(90);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  gData_02003274 = 3;
  gData_02003278 = 0;
  FieldEvent_Wait(40);
  Bring(23);
  Bring(24);
  Task_AddFrame(Func_020001a4, 0x480);
  Event_PlaySound(144);
  MapGrid_SetShake(0x60000, 0x60000, 0x10000);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x4063ff, 0);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  Event_PlaySound(144);
  System_Wait(30);
  Event_PlaySound(144);
  System_Wait(30);
  Event_PlaySound(144);
  System_Wait(30);
  MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
  MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
  Func_02000d80(2);
  Event_PlaySound(144);
  System_Wait(30);
  leader->action = 3;
  System_Wait(1);
  leader->update = 0;
  Actor_SetAnimationSpeed(leader, 16);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  Actor_SetScript(23, gData_02002f94);
  Actor_SetScript(24, gData_02002f94);
  gData_02003274 = 2;
  FieldEvent_Wait(30);
  gData_02003274 = 1;
  FieldEvent_Wait(30);
  gData_02003274 = 0;
  FieldActor_RefreshSelectorById(23);
  FieldActor_RefreshSelectorById(24);
  Actor_GetById(23)->scaleX = 0;
  Actor_GetById(24)->scaleX = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(30);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(30);
  MapGrid_SetShake(0, 0, 0x10000);
  FieldTask_Remove(Func_02000294);
  Func_0200185c();
  FieldEvent_Wait(30);
  a->motionFlags = 0;
  b->motionFlags = 0;
  c->motionFlags = 0;
  d->motionFlags = 0;
  for (i = 0; i < 16; i++) {
    a->y += PIXELS(2);
    b->y += PIXELS(2);
    c->y += PIXELS(2);
    d->y += PIXELS(2);
    System_Wait(1);
  }
  Func_02000e04(13);
  Func_02000e04(14);
  Func_02000e04(15);
  Func_02000e04(16);
  Event_PlaySound(80);
  Sound_WaitForJingle();
}

struct SparkActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct EffectParams {
  s32 count;
  s32 kind;
  s32 spread;
  s32 rise;
  u8 unk10[8];
  u16 tile;
  u16 unk1a;
  u8 unk1c[12];
};

struct SparkWork {
  u8 unk0000[0x1d9c];
  s16 resource;
};

extern s32 gData_020033dc;

void Resource_ResetEntry(s32 entry);
void Func_02001538(void);
void Func_020028b0(void);
void Func_020028b8(void);
void Func_02001c88(s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 a5, s32 a6,
                   struct EffectParams *params);

void Func_020006ac(void) {
  if (!Event_GetFlag(0xa0e)) {
    Event_SetFlag(0xa0e);
    Func_020028b0();
    FieldEvent_ResolvePendingActions(0);
    PaletteFade_ApplySource(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 1);
    PaletteFade_Interpolate(30);
    Event_WaitTransitionFrames();
    Actor_SetAnimation(0, 1);
    FieldEvent_Wait(30);
    FieldMessage_ShowCentered((s32)MsgFieldShowAnemosPower, 0, -2);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(30);
    Func_020028b8();
  }
}

void Func_02000724(void) { GameFlag_ClearBit(0xa0e); }

/// Actor 22 rises in a shower of sparks: one every other frame for 32 frames.
void Func_02000734(void) {
  struct EffectParams params;
  struct EffectParams *p;
  struct SparkActor *actor;
  u32 i;
  s32 x;
  s32 y;
  u32 random;
  u32 random2;
  u32 scaled;

  actor = ((struct SparkActor *)Actor_Get(22));
  Event_PlaySound(190);
  Actor_SetSingleChildFlags(((struct SparkActor *)Actor_Get(22)), 0);
  p = &params;
  p->count = 1;
  p->kind = 5;
  p->tile = 0x2a0;
  p->spread = 0x6666;
  p->rise = 0x30000;
  for (i = 0; i <= 31; i++) {
    FieldEvent_Wait(1);
    if (!(i & 1)) {
      random = Random16Far();
      // random * 24, written in steps around the two barriers below, so x
      // loads between the multiply's add and its last shift.
      scaled = random << 1;
      scaled += random;
      x = actor->x;
      x += (((scaled << 3) >> 16) << 16);
      x += -0xc0000;
      random2 = Random16Far();
      y = actor->y;
      Func_02001c88(x, y + (((random2 << 5) >> 16) << 16) + 0x200000, actor->z,
                    0, -0x40000, 0, 0x1b0000, p);
    }
    if (i == 20)
      Actor_SetSpriteColorById(22, 0x100);
  }
  Actor_SetSpriteColorById(22, 0);
  Actor_SetSingleChildFlags(((struct SparkActor *)Actor_Get(22)), 1);
}

void Func_02002a00(void);
void Func_02002a08(void);
void Func_02002a10(void);
void Func_02002a18(struct FieldActor *object, s32 value);

void Func_02000810(s32 actor, s32 target) {
  s32 i;

  Func_02002a00();
  Func_02002a18(((struct FieldActor *)Actor_Get(actor)), 2);
  Event_PlaySound(201);
  System_Wait(50);
  Event_PlaySound(82);
  for (i = 0; i < 120; i++) {
    {
      struct FieldActor *object = ((struct FieldActor *)Actor_Get(target));
      u32 facing = object->facing;

      object->facing = facing + 0x1000;
    }
    Actor_SetSpriteColor(((struct FieldActor *)Actor_Get(target)), (i & 4) ? 7 : 0);
    System_Wait(1);
  }
  ((struct FieldActor *)Actor_Get(target))->facing = 0xc000;
  Actor_SetSpriteColor(((struct FieldActor *)Actor_Get(target)), 0);
  Func_02002a18(((struct FieldActor *)Actor_Get(actor)), 0);
  Func_02002a10();
  Func_02002a08();
  Actor_SetAnimation(target, 1);
}

struct JupiterLighthouseRoom5EventWork {
  u8 unk000[0x1c4];
  u16 message;
};

#define EVENT_WORK                                                             \
  ((struct JupiterLighthouseRoom5EventWork *)Ram_HeapSlots->eventWork)

void Func_020028a0(s32 value);

/// Once flag 0xa24 is set and neither 0xa23 nor 0xa22 is, actor 4 turns
/// about, actor 22 rises in sparks and talks with it, then joins it; flags
/// 0xa25, 0x161 and 0x144 are set.
void Func_020008a4(void) {
  s32 angle;

  if (!Event_GetFlag(0xa24))
    return;
  if (Event_GetFlag(0xa23))
    return;
  if (Event_GetFlag(0xa22))
    return;
  Event_SetFlag(0xa22);
  Func_020028b0();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgFieldWaitingForCompanion);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_WalkToAndWait(4, 68, 280);
  Actor_WalkToAndWait(4, 68, 272);
  Actor_FaceDirectionAndWait(4, FACING_WEST);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_SetSingleChildFlags(((void *)Actor_Get(22)), 0);
  Actor_SetSpriteColor(((void *)Actor_Get(22)), 15);
  angle = 0x4000;
  Actor_SetPositionAndFacing(22, PIXELS(64), PIXELS(225),
                                                 angle);
  Func_02000734();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(PIXELS(68), -1, PIXELS(240), 1);
  Event_OpenMessage(22, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_SetAnimationAndWait(22, 4);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    EVENT_WORK->message++;
  } else {
    Actor_SetAnimationAndWait(22, 3);
    FieldEvent_Wait(10);
    EVENT_WORK->message++;
    FieldEvent_ShowMessageThenWait(22, 0, 5);
  }
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_ShowEmote(22, 265, 30);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_SetAnimationAndWait(22, 4);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(PIXELS(68), -1, PIXELS(256), 1);
  Actor_WalkByAndWait(22, 0, 16);
  FieldEvent_Wait(30);
  Func_02000810(22, 4);
  Func_020028a0(1);
  Message_ShowCentered((s16)EVENT_WORK->message++, 5);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Event_SetFlag(0xa25);
  Event_SetFlag(0x161);
  Event_SetFlag(0x144);
  Func_020028b8();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

struct JupiterLighthouseRoom5EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

void Func_020029c8(void *data);

/// Scene preparation: the event value 0x204, actor 22 placed while flag 0xa22
/// is set (cleared once 0xa23 is), actors 23 and 24 placed, and with flag
/// 0xa00 two more and the particle callback.
s32 Func_02000ae8(void) {
  struct Actor *object;
  s32 angle;

  ((struct JupiterLighthouseRoom5EventWork1ac *)Ram_HeapSlots->eventWork)
      ->value = 0x204;
  if (Event_GetFlag(0xa23))
    Event_ClearFlag(0xa22);
  if (Event_GetFlag(0xa22)) {
    angle = 0x4000;
    Actor_SetPositionAndFacing(22, PIXELS(64), PIXELS(225), angle);
  }
  Func_020029c8(gData_02002fb8);
  Actor_SetActionCallback(Actor_Get(64), 9);
  object = Actor_Get(23);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_POSITION(object, 0x2a80000, 0x200000, 0x2440000);
  Actor_SetSingleChildFlags(Actor_Get(23), 0);
  object = Actor_Get(24);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_POSITION(object, 0x2a80000, 0x400000, 0x2440000);
  Actor_SetSingleChildFlags(Actor_Get(24), 0);
  if (Event_GetFlag(0xa00)) {
    MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
    MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
    Task_AddFrame(Func_020001a4, TASK_PASS_VBLANK);
  }
  return 0;
}

/// Scene setup: the arrival flags and palette, the leader's priority bit 0x40,
/// and the actors flags 0xa00 and 0xa09 bring in.
s32 Func_02000bf0(void) {
  Scene_SetArrivalFlags(0);
  Func_02000d80(0);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  if (Event_GetFlag(0xa00))
    MapGrid_SetCellBehavior(0xd00000, 0x2a00000, 2, 236);
  if (Event_GetFlag(0xa00)) {
    Func_02000e04(10);
    Func_02000e04(12);
    Func_02000e04(13);
    Func_02000e04(14);
    Func_02000e04(15);
    Func_02000e04(16);
  }
  if (Event_GetFlag(0xa09)) {
    Actor_SetPosition(11, PIXELS(216), PIXELS(552));
    Func_02000e04(11);
  }
  return 0;
}
