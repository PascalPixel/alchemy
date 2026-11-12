/// @file
/// The scene code of overlay 0x69e that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_5/.

// ---- games/tla/src/scene/jupiter_lighthouse_room_5.c ----

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
  struct Actor *object;
  s32 i;

  object = Actor_Get(id);
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

void Func_020000c8(struct Actor *object) {
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

void Func_02000120(struct Actor *object) {
  u32 frameCount = gFrameCount;

  if (frameCount & 1)
    Animation_ApplyChildValues(object, (frameCount >> 1) % 6);
}

s32 Math_Cosine(u32 angle);

/// Circles the object around actor 23, as overlay 6a5's Func_02000090 circles
/// actor 31.
void Func_02000148(struct Actor *self) {
  struct Actor *centre = Actor_Get(23);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Math_Cosine(angle) * (self->speedLimit + 3);
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
  self->timer += -0x800;
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
  struct Actor *center;
  struct OrbitEffect *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(0x201) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(23);
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
  leader->timer = 0;
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
  leader->timer = 3;
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

s32 Resource_ResetEntry(u32 slot);
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
/* VIEW(jupiter_lighthouse_room_5): routine Func_02002a18 as void Func_02002a18(struct Actor *object, s32 value); jupiter_lighthouse_aerie.h has void Func_02002a18(void). */
extern void Func_02002a18__jupiter_lighthouse_room_5(struct Actor *object, s32 value) __asm__("Func_02002a18");

void Func_02000810(s32 actor, s32 target) {
  s32 i;

  Func_02002a00();
  Func_02002a18__jupiter_lighthouse_room_5(Actor_Get(actor), 2);
  Event_PlaySound(201);
  System_Wait(50);
  Event_PlaySound(82);
  for (i = 0; i < 120; i++) {
    {
      struct Actor *object = Actor_Get(target);
      u32 facing = object->facing;

      object->facing = facing + 0x1000;
    }
    Actor_SetSpriteColor(Actor_Get(target), (i & 4) ? 7 : 0);
    System_Wait(1);
  }
  Actor_Get(target)->facing = 0xc000;
  Actor_SetSpriteColor(Actor_Get(target), 0);
  Func_02002a18__jupiter_lighthouse_room_5(Actor_Get(actor), 0);
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

#include "shared/scene_set_arrival_flags.h"
#undef EVENT_WORK

// ---- games/tla/src/scene/jupiter_lighthouse_room_5/s0cb8.c ----
// Scene code of overlay 0x69e, in ROM order, from s0cb8's offset up to the
// next object the overlay links.

#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_020033dc;

void Func_02000cb8(s32 value) { gData_020033dc = value; }

extern s32 gData_020033d8;
extern u16 gData_02003404[];

void Func_02000cc4(void) {
  u16 colors[16];
  s32 phase = (gData_020033d8 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_020033dc != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    /* FAKEMATCH(temporary): the slot and the table address held in locals;
     * as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02003404 + offset),
     * 2 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02003404;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_020033d8++;
}

extern u16 gData_02003424[];
extern u16 gData_02003428[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002888();
void Func_02002a30();

void Func_02000d80(s32 mode) {
  gData_020033dc = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02003404, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02003424, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_020033d8 = zero;
    Task_Add(Func_02000cc4, 1152);
  } else {
    Dma_Set(gData_02003428, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_02000e04(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  {
    u8 *flags = &object->priorityFlags;
    /* FAKEMATCH(temporary): a local holding 128, so the bit is built in the
     * register it is ORed into; as *flags |= 128, 4 of 38 instructions differ
     * in tla-en. */
    s32 bit = 128;

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02002888(object->x, object->z, 0, (height >> 19) + 6);
  Func_02002a30(object->x, object->z, 0, 128);
}

#include "field_sprite.h"
#include "event_work.h"

void Func_020028b0(void);
void Func_020028b8(void);

void Func_02000e58(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 8 of 216 instructions differ in
   * tla-en. */
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Func_020028b0();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkd46 = i;

  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);

  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  angle = linked->facing;
  actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  FieldActor_SetMoveTarget(
      shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->y > actor->terrainHeight + 0x100000);
  }
  Event_PlaySound(127);
  i = 0;
  if (actor->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->velocityY != 0);
  }
  Actor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkd46 = 1;
  work->exit = 0;
  Func_020028b8();
}

s32 Func_02001044(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern const s32 gData_020033e0[];
extern s32 gData_02003400;

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02002a20(s32 x, s32 z, s32 layer);
void Func_020029b0(s32 *position);
/* VIEW(s0cb8): routine Math_Cosine as s32 Math_Cosine(s32 angle); jupiter_lighthouse_room_5 has s32 Math_Cosine(u32 angle). */
extern s32 Math_Cosine__s0cb8(s32 angle) __asm__("Math_Cosine");
/* VIEW(s0cb8): routine Func_02002820 as void Func_02002820(struct Actor *object, const s32 *script); jupiter_lighthouse_room_5 has void Func_02002820(struct OrbitEffect *object, const void *script). */
extern void Func_02002820__s0cb8(struct Actor *object, const s32 *script) __asm__("Func_02002820");

void Func_02001070(struct Actor *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (Func_02002a20(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_020029b0(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Func_02001044(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Math_Cosine__s0cb8(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block, so the speed loads first; as a plain call, 2 of 197
         * instructions differ in tla-en. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Func_02002820__s0cb8(self, gData_020033e0);
    Event_PlaySound(228);
    gData_02003400 = zero;
  }
}

// ---- games/tla/src/scene/jupiter_lighthouse_room_5/s1230.c ----
// Scene code of overlay 0x69e, in ROM order, from s1230's offset up to the next object the overlay links.

#include "call.h"
#include "field_effect.h"
#include "field_coordinate.h"

struct GustObject {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 unk30;
  u8 unk34[0x21];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(struct GustObject *object);
};

/* VIEW(s1230): routine Math_Cosine as s32 Math_Cosine(s32 angle); jupiter_lighthouse_room_5 has s32 Math_Cosine(u32 angle). */
extern s32 Math_Cosine__s1230(s32 angle) __asm__("Math_Cosine");
void Func_02002810(struct GustObject *object, s32 mode);
/* VIEW(s1230): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags); field.h has void Actor_SetSingleChildFlags(struct Actor *object, u32 flags). */
extern void Actor_SetSingleChildFlags__s1230(struct GustObject *object, u32 flags) __asm__("Actor_SetSingleChildFlags");
/* VIEW(s1230): routine Func_02001070 as void Func_02001070(struct GustObject *object); the definition has void Func_02001070(struct Actor *self). */
extern void Func_02001070__s1230(struct GustObject *object) __asm__("Func_02001070");
/* VIEW(s1230): routine Func_02001c88 as void Func_02001c88(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, struct EffectOptions *options); jupiter_lighthouse_room_5 has void Func_02001c88(s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 a5, s32 a6, struct EffectParams *params). */
extern void Func_02001c88__s1230(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options) __asm__("Func_02001c88");

struct GustObject *Func_02001230(struct GustObject *source) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 69 of 92 instructions differ in
   * tla-en. */
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Cosine__s1230(source->angle));
  position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                    position.z));
  Func_02002810(object, 2);
  Actor_SetSingleChildFlags__s1230(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02001070__s1230;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02001c88__s1230(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

s32 Func_02001308(struct Actor *object) {
  struct EffectOptions options;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase != 0)
    goto done;
  options.priority = 3;
  options.startScaleX = 0xb333;
  options.startScaleY = 0xb333;
  options.palette = 14;
  x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
  y = object->y + ((32 - (s32)((u32)(Random16Far() * 20) >> 16)) << 16);
  speed = (((s32)((u32)(Random16Far() * 5) >> 16) << 16) + 0x50000) / 10;
  Func_02001c88__s1230(x, y, object->z, 0, speed, phase,
                EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                    EFFECT_USE_START_SCALE,
                &options);
done:
  return 0;
}

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  u8 unk1c[0x34];
  struct FieldSprite *animation;
  u8 animationKind;
  u8 motionFlags;
};

/* VIEW(s1230): routine Animation_ApplyChildValues as void Animation_ApplyChildValues(struct PositionedActor *object, s32 value); jupiter_lighthouse_aerie.h has void Animation_ApplyChildValues(struct Actor *object, s32 value). */
extern void Animation_ApplyChildValues__s1230(struct PositionedActor *object, s32 value) __asm__("Animation_ApplyChildValues");

void Func_020013a8(void) {
  struct PositionedActor *actor9 = ((struct PositionedActor *)Actor_Get(9));
  struct PositionedActor *actor10 =
      ((struct PositionedActor *)Actor_Get(10));
  struct PositionedActor *actor23 =
      ((struct PositionedActor *)Actor_Get(23));
  struct FieldSprite *sprite = actor23->animation;

  Actor_SetPosition(24, 0x2000000, 0xf80000);
  Actor_SetPosition(23, 0x2000000, 0xf80000);
  Actor_SetPosition(9, 0x1d80000, 0x1000000);
  Actor_SetPosition(10, 0x2280000, 0x1000000);
  ((struct PositionedActor *)Actor_Get(24))->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (Event_GetFlag(0xa0a)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues__s1230(((struct PositionedActor *)Actor_Get(9)), 4);
    Animation_ApplyChildValues__s1230(((struct PositionedActor *)Actor_Get(11)),
                               4);
  }
  if (Event_GetFlag(0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues__s1230(((struct PositionedActor *)Actor_Get(10)),
                               4);
    Animation_ApplyChildValues__s1230(((struct PositionedActor *)Actor_Get(12)),
                               4);
  }
  if (Event_GetFlag(0xa0a) && Event_GetFlag(0xa0b)) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}

struct Spark {
  u8 unk00[0x0c];
  s32 dx;
  s32 dy;
  s32 duration;
  s32 age;
};

struct SparkSprite {
  u8 unk00[0x0c];
  s32 x;
  s32 z;
  u8 unk14[0x14];
};

/* VIEW(s1230): tag struct SparkWork as struct SparkWork__s1230; jupiter_lighthouse_room_5.c has another definition. */
struct SparkWork__s1230 {
  u8 unk0000[0x400];
  struct Spark sparks[96];
  struct SparkSprite sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
};

void Func_020029b0(s32 *position);
void Func_020029e8(struct SparkSprite *sprite);

void Func_02001538(void) {
  struct SparkWork__s1230 *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark = work->sparks;
  struct SparkSprite *sprite = work->sprites;
  s32 position[3];
  s32 duration;
  s32 i;

  for (i = 95; i >= 0; sprite++, i--, spark++) {
    duration = spark->duration;
    if (spark->age < 0 || spark->age > duration)
      continue;
    {
      s32 sine = Math_Sin((s32)((spark->age << 15) / duration));
      s32 *p = position;
      p[0] = work->from[0] +
             (work->to[0] - work->from[0]) * spark->age / duration +
             sine * spark->dx;
      sine = Math_Sin((s32)((spark->age << 15) / duration));
      p[1] = work->from[1] +
             (work->to[1] - work->from[1]) * spark->age / duration +
             sine * spark->dy;
      p[2] =
          work->from[2] + (work->to[2] - work->from[2]) * spark->age / duration;
      Func_020029b0(p);
      sprite->x = p[0];
      sprite->z = p[2];
      Func_020029e8(sprite);
      spark->age++;
    }
  }
}
