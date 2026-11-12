/// @file
/// Scene code of overlay 0x6a5, in ROM order, from s0198's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "version.h"
#include "map.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "call.h"
#include "system/main_routine_work.h"
#include "text/msg_ids.h"
#include "main_routine_work.h"
#include "scene_ids.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern u32 gFrameCount;
extern s32 gData_02006a24;
extern s32 gData_0200641c[];
extern s32 gData_02006470[];

u32 Random16Far(void);
void Func_02005764(struct Actor *object, const void *script);
void SceneActor_ParkRecord(struct Actor *object);
void Func_020000ec(void);

static inline void Event_PlayCue(s32 cue) { Sound_Play(cue); }

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second,
                                               s32 third) {
  Map_SetScreenShake(first, second, third);
}

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode) {
  BattleEffect_ApplyColorToTargetBuffer(value, mode);
}

static inline void ColorBuffer_Interpolate(s32 frames) {
  BattleEffect_StartBufferInterpolation(frames);
}

static inline struct Actor *Actor_Create(s32 type, s32 x, s32 y, s32 z) {
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
}

void Func_02000198(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 spawn;
  u32 rise;
  s32 angle;

  center = Actor_GetById(36);
  spawn = 0;
  switch (gData_02006a24) {
  case 0:
    Event_PlayCue(220);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    ColorBuffer_ApplyTarget(0x2063ff, 1);
    ColorBuffer_Interpolate(8);
    break;
  case 8:
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(8);
    break;
  case 16:
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    break;
  case 24:
    center->x = 152 << 17;
    center->y = -0x1680000;
    center->z = 164 << 16;
    center->scaleX = 0x10000;
    center->scaleY = 0x10000;
    SceneActor_ParkRecord(center);
    Actor_SetScript(36, gData_0200641c);
    break;
  case 25:
    gData_02006a24--;
    if (*(s32 *)&center->y > 0) {
      ColorBuffer_ApplyTarget(0x203210, 0);
      ColorBuffer_Interpolate(16);
      gData_02006a24++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(246);
      spawn = 1;
      center->y += 0x24000;
    }
    break;
  case 26:
    gData_02006a24--;
    if (*(volatile s32 *)&center->y > (160 << 14)) {
      ColorBuffer_ApplyTarget(0x10000, 0);
      ColorBuffer_Interpolate(40);
      gData_02006a24++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(246);
      spawn = 1;
      center->y += 0x24000;
    }
    break;
  case 27:
  case 28:
  case 29:
  case 30:
  case 31:
  case 32:
  case 33:
  case 34:
    spawn = 1;
    break;
  case 36:
    Event_PlayCue(187);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(12);
    break;
  case 48:
    FieldActor_Stop(36);
    Event_SetFlag(0x202);
    break;
  }
  if (spawn) {
    rise = ((u32)(Random16Far() * 80) >> 16) << 16;
    effect =
        Actor_Create(672, center->x, center->y - rise + 0xfff80000, center->z);
    if (effect != 0) {
      sprite = effect->animation;
      Func_02005764(effect, gData_02006470);
      Actor_SetSpriteColor(effect, 1);
      effect->motionFlags = 0;
      angle = Random16Far() & 0xffff000;
      effect->action = angle;
      effect->unk66 = 0;
      {
        u32 counter = Random16Far() >> 13;

        BYTE_AT(effect, 0x62) = counter;
      }
      effect->update = Func_020000ec;
      effect->speedLimit = Math_Sin((u32)(Random16Far() * 0xffff) >> 20) * 24;
      effect->speedLimit = center->speedLimit >> 16;
      sprite->nextFrame = 0;
      sprite->priority = 1;
    }
  }
  gData_02006a24++;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02005814();
s32 Func_0200581c();
extern u8 MsgYouReLate[];

void Func_0200045c(void) {
  s32 t0;
  s32 t2;

  t0 = Event_GetFlag(0xa23);
  if (t0 != 0) {
    Event_SetFlag(0x200);
    t2 = Event_GetFlag(0xa21);
    if (t2 == 0) {
      Func_02005814();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgYouReLate);
      Actor_ShowEmote(0x1c, 0x103, 0x1e);
      Event_ShowMessage(0x1c, 0);
      Actor_SetAnimation(27, 3);
      /* FAKEMATCH(dead): an empty once-only block, which ends the scheduling
       * block after the animation call, so its id (r0) is set before its mode
       * (r1); without it, 2 of 39 instructions differ in tla-en, the mode set
       * first. */
      do {
      } while (0);
      FieldEvent_Wait(20);
      Event_ShowMessage(0x1b, 0);
      Func_0200581c();
    }
  }
}

extern u32 gFrameCount;
extern const u8 gData_020068b0[];

u32 Random16Far(void);
void Func_02005764(struct Actor *object, const void *script);
void Func_02000090(void);

/// Every third frame, or every frame once flag 0x201 is set, spawns a particle
/// at a random height above object 31 that circles it (Func_02000090).
void Func_020004c8(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(0x201) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(31);
  if (Event_GetFlag(0x201) != 0) {
    u32 random;

    random = Random16Far();
    effect = Actor_Create(672, center->x,
                          (s32)((u32)(random << 8) >> 16 << 16) + center->y -
                              0x1c0000,
                          center->z);
  } else {
    u32 random;

    random = Random16Far();
    effect = Actor_Create(672, center->x,
                          (s32)((u32)(random << 6) >> 16 << 16) + center->y -
                              0x1c0000,
                          center->z);
  }
  if (effect != 0) {
    sprite = effect->animation;
    Func_02005764(effect, gData_020068b0);
    Actor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16Far() & 0xffff000;
    effect->action = angle;
    effect->unk66 = 0;
    effect->update = Func_02000090;
    effect->speedLimit =
        Math_Sin((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 1;
  }
}

/// The halves of a 16.16 coordinate. Read through an s16 pointer instead, the
/// high half's offset folds into the coordinate's (differ 11).
struct FixedHalves {
  u16 fraction;
  s16 whole;
};

/// What a field sprite's record points to.
struct SpriteRecord {
  u8 unk00[5];
  u8 unk05; ///< XXX: set to 1 here; the resource metadata code would say why.
};

extern u32 gFrameCount;

/// While the view's z is at most 177 pixels, sets object 36 at y 0x6f8000 and
/// full size on odd frames, or at y 0x688000 and 1.3 times the size on even
/// ones; beyond it, calls Actor_SetPosition(36, 0, 0).
void Func_020005b8(void) {
  struct Actor *object = Actor_Get(36);
  struct MapWork *map = Ram_HeapSlots->mapWork;

  Random16Far();
#if TLA_PATCH_1
  // From patch 1; the Japanese release goes on when object 36 is missing.
  if (object == NULL)
    return;
#endif
  if (((struct FixedHalves *)&map->viewZ)->whole <= 177) {
    ((struct SpriteRecord *)((struct FieldSprite *)Actor_Get(36)->animation)
         ->record)
        ->unk05 = 1;
    if (gFrameCount & 1) {
      Actor_SetPosition(36, PIXELS(512), PIXELS(264));
      Actor_Get(36)->y = 0x6f8000;
      Actor_Get(36)->scaleX = 0x10000;
      Actor_Get(36)->scaleY = 0x10000;
    } else {
      Actor_SetPosition(36, PIXELS(512), PIXELS(264));
      Actor_Get(36)->y = 0x688000;
      Actor_Get(36)->scaleX = 0x14ccc;
      Actor_Get(36)->scaleY = 0x14ccc;
    }
  } else {
    Actor_SetPosition(36, 0, 0);
  }
}

s32 Func_02005814();
s32 Func_0200581c();
s32 Func_02005894();
void Func_02005924();
s32 Actor_SetSingleChildFlags();
s32 PartyInventory_Remove();
void *HeapSlot_AllocFar();
s32 HeapSlot_Release();

extern u8 gData_02006338[];

void Func_0200065c(void) {
  s32 answer, state;
  struct Actor *actor4, *created, *actor37, *actor31, *actor32;
  struct FieldSprite *sprite;
  u8 *buffer;
  u8 *flags;

  if (GameFlag_Test(0xa23) != 0) {
    Func_02005814();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered((s32)MsgThereSeemsTo, 5);
    answer = Event_AskYesNoWithActorAnimation(4, 0);
    if (answer != 0) {
      Actor_SetSpeed(4, 0x10000, 0x8000);
      Actor_WalkToAndWait(4, 0x202, 0x135);
    } else {
      Event_SetFlag(0xa21);
      Event_SetFlag(0x8df);
      Event_ClearFlag(0xa25);
      Event_ClearFlag(0x161);
      Event_ClearFlag(0x144);
      Actor_FaceDirection(27, 0xd000, 0);
      Actor_FaceDirection(28, 0xd000, 0);
      Actor_SetSpeed(4, 0x10000, 0x8000);
      Actor_WalkToAndWait(4, 0x200, 0x126);
      Actor_FaceDirectionAndWait(4, 0xc000);
      FieldEvent_Wait(20);
      PaletteFade_ApplySource(0x10000, 0);
      PaletteFade_ApplyTarget(0x10003, 1);
      PaletteFade_Interpolate(20);
      System_Wait(20);
      Call1((void (*)())PartyInventory_Remove, 245);
      actor4 = Actor_GetById(4);
      created =
          FieldActor_Create(0x1e9, actor4->x, actor4->y + 0x80000, actor4->z);
      if (created != 0) {
        sprite = created->animation;
        sprite->unk1b[0] = 0;
        sprite->fullColor = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        created->priorityFlags &= 254;
        created->motionFlags = 0;
        created->unk5c[0] = 1;
        created->speedLimit = 0x6666;
        created->acceleration = 0xccc;
        buffer = HeapSlot_AllocFar(68, 0x608);
        Item_LoadIcon(246);
        Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
        Call1((void (*)())HeapSlot_Release, 68);
      }
      FieldActor_SetSpritePriority(37, 1);
      actor37 = Actor_GetById(37);
      actor37->x = actor4->x;
      actor37->y = 0x600000;
      actor37->z = actor4->z;
      actor37->motionFlags = 3;
      actor37->acceleration = 0xccc;
      actor37->speedLimit = 0x6666;
      actor37->scaleX = 0x2000;
      actor37->scaleY = 0x2000;
      FieldEvent_SetMessageId((s32)MsgThereSeemsTo + 1);
      Event_PlaySound(36);
      if (created != 0) {
        flags = &created->motionFlags;
        *flags = 0;
        created->gravity = 0x9999;
        created->bounce = 0xcccc;
        created->velocityY = 0x80000;
        FieldActor_SetMoveTarget(created, 0x2000000, 0x800000, 0x1260000);
        FieldActor_SetMoveTarget(actor37, 0x2000000, -1, 0x1260000);
        Func_02005894(7, gMainRoutineWork.currentOwner);
        System_Wait(1);
        Actor_SetSpeed(7, 0xcccc, 0x6666);
        Actor_WalkBy(7, 12, 0);
        Actor_WalkByAndWait(4, -12, 0);
        FieldActor_WaitForMoveById(7);
        FieldActor_Stop(7);
        System_Wait(1);
        Actor_SetAnimation(7, 1);
        System_Wait(1);
        Actor_FaceDirection(4, 0xc000, 0);
        Actor_FaceDirection(7, 0xc000, 0);
        Actor_WaitForMove(created);
        state = 4;
        *flags = state;
        FieldEvent_Wait(90);
        *flags = 0;
        FieldActor_SetMoveTarget(created, 0x2000000, 0x800000, 0x1080000);
        FieldActor_SetMoveTarget(actor37, 0x2000000, -1, 0x1080000);
        *flags = state;
        FieldEvent_Wait(60);
        *flags = 0;
        actor37->x = 0;
        actor37->targetX = 0x80000000;
        actor37->targetY = 0x80000000;
        actor37->targetZ = 0x80000000;

        actor37->y = 0;
        actor37->z = 0;
        actor37->velocityX = 0;
        actor37->velocityY = 0;
        actor37->velocityZ = 0;
        actor37->action = 0;
        Actor_WaitForMove(created);
        *flags = state;
        FieldEvent_Wait(90);
        *flags = 0;
        FieldActor_SetMoveTarget(created, 0x2000000, 0x5c0000, 0x1080000);
        Actor_WaitForMove(created);
        created->x = 0;
        created->targetX = 0x80000000;
        created->targetY = 0x80000000;
        created->targetZ = 0x80000000;
        created->y = 0;
        created->z = 0;
        created->velocityX = 0;
        created->velocityY = 0;
        created->velocityZ = 0;
        created->action = 0;
      }
      FieldEvent_Wait(40);
      PaletteFade_ApplyTarget(0x10000, 0);
      PaletteFade_Interpolate(20);
      System_Wait(20);
      Actor_WalkBy(4, 0, -16);
      Actor_WalkBy(7, 0, -16);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
      Actor_SetAnimation(7, 1);
      FieldEvent_Wait(50);
      Event_PlaySound(141);
      FieldEvent_Wait(30);
      MapGrid_SetShake(0x10000, 0x10000, 0x10000);
      FieldEvent_Wait(30);
      MapGrid_SetShake(0x20000, 0x20000, 0x10000);
      FieldEvent_Wait(30);
      Actor_GetById(0)->priorityFlags |= 1;
      Actor_GetById(1)->priorityFlags |= 1;
      Actor_GetById(2)->priorityFlags |= 1;
      Actor_GetById(3)->priorityFlags |= 1;
      Actor_GetById(4)->priorityFlags |= 1;
      Actor_GetById(5)->priorityFlags |= 1;
      Actor_GetById(7)->priorityFlags |= 1;
      Actor_GetById(6)->priorityFlags |= 1;
      Actor_GetById(30)->priorityFlags |= 1;
      Actor_GetById(27)->priorityFlags |= 1;
      Actor_GetById(28)->priorityFlags |= 1;
      Actor_GetById(31)->priorityFlags |= 1;
      Actor_GetById(32)->priorityFlags |= 1;
      FieldCamera_MoveTo(0x2000000, 0x600000, 0x1080000, 1);
      Func_02005924();
      FieldActor_SetSpritePriority(31, 1);
      FieldActor_SetSpritePriority(32, 1);
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
      FieldActor_SetSpritePriority(7, 1);
      FieldEvent_Wait(20);
      Actor_SetSingleChildFlags(Actor_GetById(31), 0);
      Actor_SetSpriteColor(Actor_GetById(31), 7);
      actor31 = Actor_GetById(31);
      actor31->scaleY = 0xffff0000;
      actor31->scaleX = 0x1999;
      actor31->motionFlags = 0;
      actor31->x = 0x2000000;
      actor31->y = 0x800000;
      actor31->z = 0x1080000;
      actor31->priorityFlags |= 2;
      Actor_SetSingleChildFlags(Actor_GetById(32), 0);
      Actor_SetSpriteColor(Actor_GetById(32), 7);
      actor32 = Actor_GetById(32);
      actor32->scaleY = 0xffff0000;
      actor32->scaleX = 0x1999;
      actor32->motionFlags = 0;
      actor32->x = 0x2000000;
      actor32->y = 0xa00000;
      actor32->z = 0x1080000;
      actor32->priorityFlags |= 2;
      Actor_SetScript(31, gData_02006338);
      Actor_SetScript(32, gData_02006338);
      Task_AddFrame((s32)Func_020004c8, 0x480);
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(10, 0, 0);
      Actor_SetPosition(23, 0, 0);
      Actor_SetPosition(24, 0, 0);
      Event_PlaySound(145);
      MapGrid_SetShake(0x60000, 0x60000, 0x10000);
      PaletteFade_ApplyTarget(0x4063ff, 0);
      PaletteFade_Interpolate(16);
      System_Wait(16);
      Event_PlaySound(63);
      System_Wait(180);
      PaletteFade_ApplyTarget(0x7fff, 0);
      PaletteFade_Interpolate(60);
      Event_PlaySound(141);
      System_Wait(60);
      FieldTask_Remove(Func_020004c8);
      Event_SetFlag(0x11a);
      FieldEvent_SetExitRequest(2);
    }
    Func_0200581c();
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200577c(void);

void Func_02005924(void);

void PartyInventory_Add(s32 item);
void *Unit_Get(s32 id);
void Func_020057ec(s32 id, s32 slot);
void Func_0200593c(s32 a, s32 b);
static __inline__ s32 Func_0200592c_(void) { return ((s32)Func_080d4568()); }
static __inline__ void Func_02005984_(s32 a) { Event_ResolvePendingActions(a); }
static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 b) {
  Camera_MoveWithinMapBounds(x, y, z, b);
}
static __inline__ void wait_(s32 frames) { wait(frames); }
static __inline__ void Func_020058c4_(const void *message) {
  Event_SetMessageId(message);
}
static __inline__ void Func_0200588c_(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}
static __inline__ void Func_020059f4_(s32 sound) { Sound_Play(sound); }
static __inline__ void Func_020057ac_(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}
static __inline__ void FieldEvent_Wait_(s32 frames) { Event_Wait(frames); }
static __inline__ void Func_02005914_(s32 a, s32 b) {
  Camera_SetSpeedParameters(a, b);
}
static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}
static __inline__ void Func_020058ec_(s32 id, s32 angle) {
  Actor_ArmCallbackAndRefresh(id, angle);
}
static __inline__ void Actor_OffsetPositionAndReset_(s32 id, s32 x, s32 z) {
  Actor_OffsetPositionAndReset(id, x, z);
}
static __inline__ void Actor_WaitForMoveById_(s32 id) {
  Actor_WaitForMoveById(id);
}
static __inline__ void Motion_SetVarCbAndRefresh_(s32 id, s32 variant) {
  Motion_SetVarCbAndRefresh(id, variant);
}
static __inline__ void Func_020058d4_(s32 id, s32 a, s32 b) {
  Event_ShowMessageThenWait(id, a, b);
}
static __inline__ void *Func_020058fc_(s32 id, s32 emote, s32 frames) {
  return ((void *)Func_080d47b4(id, emote, frames));
}
static __inline__ s32 FieldMessage_ShowAtActor_(s32 id, s32 flags) {
  return FieldMessage_ShowAtActor(id, flags);
}
static __inline__ s32 Event_AskYesNoWithActorAnimation_(s32 id, s32 flags) {
  return Event_AskYesNoWithActorAnimation(id, flags);
}
static __inline__ void Actor_CommitPositionAndActivate_(s32 id, s32 x, s32 z) {
  Actor_CommitPositionAndActivate(id, x, z);
}
static __inline__ struct Actor *Actor_GetById_(s32 id) {
  return Actor_GetById(id);
}
static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 x,
                                                           s32 z) {
  Actor_OffsetPositionAndResetMotion(id, x, z);
}
static __inline__ void FieldMessage_Show_(s32 message, s32 position) {
  FieldMessage_Show(message, position);
}
static __inline__ void PartyInventory_Add_(s32 item) {
  PartyInventory_Add(item);
}
static __inline__ void *Unit_Get_(s32 id) { return Unit_Get(id); }
static __inline__ void Func_020057ec_(s32 id, s32 slot) {
  Func_020057ec(id, slot);
}
static __inline__ void Func_02005904_(s32 id, s32 emote) {
  Func_080d489c(id, emote);
}
static __inline__ void Actor_StartRepeatedMotion_(s32 id, s32 motion) {
  Actor_StartRepeatedMotion(id, motion);
}
static __inline__ void GameFlag_SetBit_(s32 flag) { GameFlag_SetBit(flag); }
static __inline__ void Func_0200593c_(s32 a, s32 b) { Func_0200593c(a, b); }

struct UnitItems {
  u8 unk00[0xd8];
  u16 items[15];
};

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                               \
    FieldEvent_Wait(frames);                                                        \
  } while (0)

void Func_02000cd0(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 612 of 691 instructions differ in
   * tla-en. */
  s32 t2, t4, t75, t77, t78, t79, t80;
  s32 t101, t102, t135, t139, t140, t143, t144;
  s32 slot;
  s32 facing;

  Func_02005814();
  Func_02005984_(0);
  t2 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t2, s32, 428) = 0x100;
  FIELD_AT(t2, s32, 436) = 120;
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  t4 = Func_0200592c_();
  FIELD_AT(t4, u8, 85) = 0;
  Camera_MoveWithinMapBounds_(0x2000000, 0x600000, 0xf00000, 0);
  wait_(1);
  Func_0200577c();
  wait_(1);
  Func_020058c4_(MsgWeHaveIgnited);
  Func_0200588c_(4, 0x1fc0000, 0x1160000, facing = 0xc000);
  Func_0200588c_(7, 0x2140000, 0x1160000, facing);
  Func_020059f4_(141);
  Func_020057ac_(0x40000, 0x40000, 0x10000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057ac_(0x30000, 0x30000, 0x10000);
  FieldEvent_Wait_(60);
  Func_020057ac_(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait_(60);
  Func_020059f4_(0x186);
  Func_020057ac_(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Func_02005914_(0x6666, 0xccc);
  Camera_MoveWithinMapBounds_(0x2000000, 0x600000, 0x1280000, 1);
  Func_02005924();
  FieldEvent_Wait_(20);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 20);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 20);
  Actor_ArmCallback_(7, 0x4000, 0);
  Func_020058ec_(4, 0x4000);
  FieldEvent_Wait_(20);
  Actor_OffsetPositionAndReset_(4, 0, 8);
  Actor_OffsetPositionAndReset_(7, 0, 8);
  Actor_OffsetPositionAndReset_(27, 32, -16);
  Actor_OffsetPositionAndReset_(28, 48, -16);
  Actor_WaitForMoveById_(4);
  Actor_WaitForMoveById_(7);
  Actor_WaitForMoveById_(27);
  Actor_WaitForMoveById_(28);
  Actor_ArmCallback_(4, 0x2000, 0);
  Actor_ArmCallback_(7, 0x4000, 0);
  Actor_ArmCallback_(27, 0xa000, 0);
  Actor_ArmCallback_(28, 0xb000, 0);
  Motion_SetVarCbAndRefresh_(7, 2);
  FieldEvent_Wait_(5);
  Func_020059f4_(78);
  Func_020058d4_(7, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(28, 4, 33);
  Func_020059f4_(34);
  Func_020058d4_(0x101c, 0, 0);
  ((void *)Actor_ShowEmote(7, 0x100, 0));
  Func_020058fc_(4, 0x100, 30);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  Func_020058d4_(27, 0, 0);
  Actor_ArmCallback_(4, 0, 0);
  Func_020058ec_(7, 0x8000);
  FieldEvent_Wait_(10);
  ACTOR_SET_MODE_AND_WAIT(28, 4, 33);
  Func_020058d4_(0x101c, 0, 0);
  Actor_ArmCallback_(4, 0x2000, 0);
  Func_020058ec_(7, 0x4000);
  FieldEvent_Wait_(10);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  Func_020058d4_(27, 0, 0);
  ((void *)Actor_ShowEmote(7, 0x102, 30));
  Motion_SetVarCbAndRefresh_(28, 2);
  FieldEvent_Wait_(5);
  FieldMessage_ShowAtActor_(0x101c, 0);
  t75 = Event_AskYesNoWithActorAnimation_(4, 0);
  if (t75 != 0)
    goto L_02008fc4;
  Func_020058d4_(0x101c, 0, 0);
  t77 = (s32)Ram_HeapSlots->eventWork;
  t78 = FIELD_AT(t77, u16, 452);
  FIELD_AT(t77, u16, 452) = (t78 + 1);
  goto L_02008fe0;
L_02008fc4:
  t79 = (s32)Ram_HeapSlots->eventWork;
  t80 = FIELD_AT(t79, u16, 452);
  FIELD_AT(t79, u16, 452) = (t80 + 1);
  Func_020058d4_(0x101c, 0, 0);
L_02008fe0:
  Func_020058fc_(7, 0x100, 0);
  ((void *)Actor_ShowEmote(4, 0x100, 30));
  Actor_ArmCallback_(7, 0x8000, 0);
  Func_020058ec_(4, 0);
  FieldEvent_Wait_(10);
  Actor_CommitPositionAndActivate_(28, -16, -0x18);
  Actor_ArmCallback_(28, 0xd000, 0);
  Actor_ArmCallback_(4, 0x4000, 0);
  Func_020058fc_(7, 0x102, 0);
  Actor_GetById_(28)->actionFlags &= 0xfe;
  Actor_GetById_(28)->velocityY = 0x40000;
  Actor_OffsetPositionAndResetMotion_(28, 0, 24);
  Actor_WaitForMoveById_(28);
  Actor_GetById_(28)->actionFlags |= 1;
  Actor_ArmCallback_(28, 0xa000, 0);
  Actor_ArmCallback_(7, 0x4000, 0);
  Func_020058d4_(7, 0, 0);
  t101 = (s32)Ram_HeapSlots->eventWork;
  t102 = FIELD_AT(t101, u16, 452);
  FIELD_AT(t101, u16, 452) = (t102 + 1);
  FieldMessage_Show_(((t102 << 16) >> 16), 5);
  PartyInventory_Add_(224);
  for (slot = 0; slot < 15; slot++) {
    if ((((struct UnitItems *)Unit_Get_(4))->items[slot] & 0x1ff) == 224) {
      Func_020057ec_(4, slot);
      break;
    }
  }
  Motion_SetVarCbAndRefresh_(4, 2);
  FieldEvent_Wait_(5);
  Actor_OffsetPositionAndReset_(4, 0, 8);
  Actor_CommitPositionAndActivate_(27, -0x18, -8);
  Actor_GetById_(4)->actionFlags &= 0xfe;
  Actor_OffsetPositionAndResetMotion_(4, 0, -8);
  Func_020058fc_(28, 0x108, 30);
  Func_020058d4_(28, 0, 0);
  Actor_WaitForMoveById_(4);
  Actor_GetById_(4)->actionFlags &= 0xfe;
  ((void *)Actor_ShowEmote(4, 0x109, 30));
  Func_020058ec_(7, 0x8000);
  Func_020058d4_(7, 0, 0);
  Func_020058fc_(27, 0x108, 30);
  Actor_ArmCallback_(27, 0xd000, 0);
  Func_020058d4_(0x201b, 0, 0);
  Func_020058ec_(7, 0x4000);
  FieldEvent_Wait_(10);
  Func_02005904_(7, 0x102);
  Func_02005904_(gMainRoutineWork.currentOwner, 0x102);
  FieldEvent_Wait_(30);
  ((void *)Actor_ShowEmote(28, 0x108, 30));
  Actor_ArmCallback_(28, 0xb000, 0);
  Event_OpenMessage(28, 0);
  t135 = Event_AskYesNoWithActorAnimation_(4, 0);
  if (t135 != 0)
    goto L_02009236;
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  Func_020058d4_(28, 0, 0);
  t139 = (s32)Ram_HeapSlots->eventWork;
  t140 = FIELD_AT(t139, u16, 452);
  FIELD_AT(t139, u16, 452) = (t140 + 1);
  goto L_02009260;
L_02009236:
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  t143 = (s32)Ram_HeapSlots->eventWork;
  t144 = FIELD_AT(t143, u16, 452);
  FIELD_AT(t143, u16, 452) = (t144 + 1);
  Func_020058d4_(28, 0, 0);
L_02009260:
  Motion_SetVarCbAndRefresh_(7, 2);
  FieldEvent_Wait_(5);
  Func_020058d4_(7, 0, 0);
  Func_020058fc_(27, 0x109, 30);
  Actor_ArmCallback_(27, 0xd000, 0);
  Func_020058d4_(0x201b, 0, 0);
  ((void *)Actor_ShowEmote(4, 0x108, 30));
  Actor_ArmCallback_(28, 0xd000, 0);
  ACTOR_SET_MODE_AND_WAIT(28, 4, 33);
  Func_020058d4_(28, 0, 0);
  ((void *)Actor_ShowEmote(4, 0x101, 30));
  Actor_ArmCallback_(27, 0xb000, 0);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  Func_020058d4_(0x201b, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  Func_020058d4_(28, 0, 0);
  Actor_StartRepeatedMotion_(4, 2);
  Motion_SetVarCbAndRefresh_(7, 2);
  FieldEvent_Wait_(5);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  Func_020058d4_(0x201b, 0, 0);
  /* FAKEMATCH(dead): the byte store kept in a once-only block, whose end
   * holds the scene id load behind it, as s16e8.c does with the same store; as
   * the plain store, 4 of 691 instructions differ in tla-en. */
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov6a5, 10);
  Event_SetDefeatDestination((s32)&SceneId_Ov6a5, 10);
  GameFlag_SetBit_(0x11b);
  Func_0200593c_(13, 5);
  return;
}
