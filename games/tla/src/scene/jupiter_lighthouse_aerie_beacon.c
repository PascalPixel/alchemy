/// @file
/// The scene code of overlay 0x6a5, in ROM order.

#include "types.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "field_event.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "system/heap_release.h"
#include "actor.h"
#include "field_sprite.h"
#include "version.h"
#include "map.h"
#include "system/ram_buffer.h"
#include "call.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "owner_state.h"
#include "debug.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/vector3.h"
#include "shared/whirlwind.h"
#include "system/field_effect.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "shared/cracked_tile.h"
#include "module/field/field_effect.h"
#include "macros.h"
#include "shared/tables.h"
#include "objcmd.h"

s32 Func_02000038(s32 a0) {
  Event_PlaySound(0x98);
  Actor_SetSpriteFlags(a0, 0);
  return 0;
}

s32 Func_02000050(s32 a0) {
  Actor_SetSpriteFlags(a0, 1);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "../field/object/park_record.c"

void Func_02000090(struct Actor *self) {
  struct Actor *centre = Actor_Get(31);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Trig_Cos(angle) * (self->speedLimit + 3);
  sine = Math_Sin(angle);
  z = centre->z;
  x = self->x;
  z += sine * 2;
  self->z = z;
  self->targetX = x;
  self->targetZ = z;
  self->timer += -0x800;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// Circles object 36, wider across than deep, by the actor's speed and its
/// byte at 98.
// As ☀️'s SceneEffect_UpdateCounterDrivenOrbit does.
void Func_020000ec(struct Actor *self) {
  struct Actor *centre = Actor_Get(36);
  u16 angle = self->timer;
  s32 cosine = Trig_Cos(angle);
  s32 radius = self->speedLimit + BYTE_AT(self, 98);
  self->x = centre->x + cosine * (radius + 6);
  self->z = centre->z + Math_Sin(angle) * (BYTE_AT(self, 98) + 4);
  self->targetX = self->x;
  self->targetZ = self->z;
  self->timer += -0x800;
}

#include "../field/object/step_angle.c"

#undef BYTE_AT
// @file
// Scene code of overlay 0x6a5, in ROM order.

// ---- games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s0198.c ----

#include "text/msg_ids.h"

/// What The Lost Age's code declares for the files that use it, written once.

struct CellTarget;
struct CellTargetEntry;

void Func_0200045c(void);

void SceneActor_ParkRecord(struct Actor *actor);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
extern void Actor_SelectAnimation(struct FieldEffect *effect, s32 mode);

/// The cells CrackedTiles_Step watches, and the cell it copies over the one the
/// leader steps on.
extern struct CellTarget gCrackedTiles;

void Event_End(void);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

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

struct UnitItems {
  u8 unk00[0xd8];
  u16 items[15];
};

struct SceneActor {
  u8 pad0[16];
  s32 unk_10;
  u8 pad14[35 - 20];
  u8 unk_23;
  u8 pad24[64 - 36];
  s32 unk_40;
  u8 pad44[85 - 68];
  u8 unk_55;
};

struct ScrollWork {
  s32 unk00[3];
  s32 height;
};

/// The object this update drives: an Actor with its own bytes over the callback
/// word.
struct Object6a5 {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 speed;
  u8 unk34[0x1c];
  s8 *sprite;
  u8 unk54[0x0e];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(void);
};

/// The scene's event work, as far as this update reads it.
struct EventWork6a5 {
  u8 unk000[346];
  s16 unk15a;
  u8 unk15c[2];
  s16 unk15e;
  u8 unk160[6];
  s16 unk166;
  s16 unk168;
  s16 unk16a;
};

extern s32 gData_02006a24;

extern s32 gData_0200641c[];
extern s32 gData_02006470[];

void FieldCamera_WaitForArrival(void);

void Func_02003a6c(s32 mode);
void Func_02003c58(s32 mode);

extern s32 Data_02006a1c;

s32 HangingVine_Place();

extern u16 gPaletteCycleColors[];
extern u16 gLighthousePalette13[];
extern u16 gLighthouseUnlitColors[];

void MapGrid_SetAttributeBAt();
s32 MapGrid_GetCellBehavior();

extern s32 Data_02006a28;

void MapGrid_SetCellHeight(void);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Actor_StartScript(struct Actor *object, const void *script);
/* VIEW(s0198): routine Func_020000ec as this part declared it; the joined file
 * has another. */
void Func_020000ec__s0198(void) __asm__("Func_020000ec");
#define Func_020000ec Func_020000ec__s0198

static inline void Event_PlayCue(s32 cue) { Sound_Play(cue); }

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second,
                                               s32 third) {
  MapGrid_SetScreenShake(first, second, third);
}

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode) {
  BattleEffect_ApplyColorToTargetBuffer(value, mode);
}

static inline void ColorBuffer_Interpolate(s32 frames) {
  PaletteFade_StartInterpolation(frames);
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
    rise = ((u32)(Random16() * 80) >> 16) << 16;
    effect = FieldActor_Create(672, center->x, center->y - rise + 0xfff80000,
                               center->z);
    if (effect != 0) {
      sprite = effect->sprite;
      Actor_StartScript(effect, gData_02006470);
      FieldActor_SetSpriteColor(effect, 1);
      effect->motionFlags = 0;
      angle = Random16() & 0xffff000;
      effect->timer = angle;
      effect->unk66 = 0;
      {
        u32 counter = Random16() >> 13;

        BYTE_AT(effect, 0x62) = counter;
      }
      effect->update = Func_020000ec;
      effect->speedLimit = Math_Sin((u32)(Random16() * 0xffff) >> 20) * 24;
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

/* VIEW(s0198): routine Event_End as s32 Event_End();
 * module/scene/jupiter_lighthouse_aerie_beacon.h has void Event_End(void); not
 * tried: Event_End's bound declaration was not found. */
extern s32 Func_0200581c__s0198() __asm__("Event_End");

void Func_0200045c(void) {
  s32 t0;
  s32 t2;

  t0 = Event_GetFlag(0xa23);
  if (t0 != 0) {
    Event_SetFlag(0x200);
    t2 = Event_GetFlag(0xa21);
    if (t2 == 0) {
      Event_Start();
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
      Func_0200581c__s0198();
    }
  }
}

/* VIEW(s0198): routine gData_020068b0 as this part declared it; the joined file
 * has another. */
extern const u8 gData_020068b0__s0198[] __asm__("gData_020068b0");
#define gData_020068b0 gData_020068b0__s0198

/* VIEW(s0198): routine Func_02000090 as this part declared it; the joined file
 * has another. */
void Func_02000090__s0198(void) __asm__("Func_02000090");
#define Func_02000090 Func_02000090__s0198

// Every third frame, or every frame once flag 0x201 is set, spawns a particle
// at a random height above object 31 that circles it (Func_02000090).
void Func_020004c8(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(0x201) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(31);
  /* FAKEMATCH(wrapper): both effects created through Actor_Create above, which
   * evaluates the position before the type; as direct calls of
   * Import_ActorCreate, 2 of 94 instructions differ in tla-en. */
  if (Event_GetFlag(0x201) != 0) {
    u32 random;

    random = Random16();
    effect = FieldActor_Create(672, center->x,
                               (s32)((u32)(random << 8) >> 16 << 16) +
                                   center->y - 0x1c0000,
                               center->z);
  } else {
    u32 random;

    random = Random16();
    effect = FieldActor_Create(672, center->x,
                               (s32)((u32)(random << 6) >> 16 << 16) +
                                   center->y - 0x1c0000,
                               center->z);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gData_020068b0);
    FieldActor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->timer = angle;
    effect->unk66 = 0;
    effect->update = Func_02000090;
    effect->speedLimit = Math_Sin((u32)(Random16() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 1;
  }
}

// While the view's z is at most 177 pixels, sets object 36 at y 0x6f8000 and
// full size on odd frames, or at y 0x688000 and 1.3 times the size on even
// ones; beyond it, calls Actor_SetPosition(36, 0, 0).
void Func_020005b8(void) {
  struct Actor *object = Actor_Get(36);
  struct MapWork *map = Ram_HeapSlots->mapWork;

  Random16();
#if TLA_PATCH_1
  // From patch 1; the Japanese release goes on when object 36 is missing.
  if (object == NULL)
    return;
#endif
  if (((struct FixedHalves *)&map->viewZ)->whole <= 177) {
    ((struct SpriteRecord *)((struct FieldSprite *)Actor_Get(36)->sprite)
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

s32 Actor_MoveToObject();
s32 PartyInventory_Remove(s32 item_id);

/* VIEW(s0198): routine gData_02006338 as this part declared it; the joined file
 * has another. */
extern u8 gData_02006338__s0198[] __asm__("gData_02006338");
#define gData_02006338 gData_02006338__s0198

void Func_0200065c(void) {
  s32 answer, state;
  struct Actor *actor4, *created, *actor37, *actor31, *actor32;
  struct FieldSprite *sprite;
  u8 *buffer;
  u8 *flags;

  if (GameFlag_Get(0xa23) != 0) {
    Event_Start();
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
        sprite = created->sprite;
        sprite->unk1b[0] = 0;
        sprite->fullColor = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        created->priorityFlags &= 254;
        created->motionFlags = 0;
        created->unk5c[0] = 1;
        created->speedLimit = 0x6666;
        created->acceleration = 0xccc;
        buffer = HeapSlot_Alloc(68, 0x608);
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
        Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
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
        FieldActor_WaitForMove(created);
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
        actor37->timer = 0;
        FieldActor_WaitForMove(created);
        *flags = state;
        FieldEvent_Wait(90);
        *flags = 0;
        FieldActor_SetMoveTarget(created, 0x2000000, 0x5c0000, 0x1080000);
        FieldActor_WaitForMove(created);
        created->x = 0;
        created->targetX = 0x80000000;
        created->targetY = 0x80000000;
        created->targetZ = 0x80000000;
        created->y = 0;
        created->z = 0;
        created->velocityX = 0;
        created->velocityY = 0;
        created->velocityZ = 0;
        created->timer = 0;
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
      FieldCamera_WaitForArrival();
      FieldActor_SetSpritePriority(31, 1);
      FieldActor_SetSpritePriority(32, 1);
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
      FieldActor_SetSpritePriority(7, 1);
      FieldEvent_Wait(20);
      Actor_SetSpriteFlags(Actor_GetById(31), 0);
      FieldActor_SetSpriteColor(Actor_GetById(31), 7);
      actor31 = Actor_GetById(31);
      actor31->scaleY = 0xffff0000;
      actor31->scaleX = 0x1999;
      actor31->motionFlags = 0;
      actor31->x = 0x2000000;
      actor31->y = 0x800000;
      actor31->z = 0x1080000;
      actor31->priorityFlags |= 2;
      Actor_SetSpriteFlags(Actor_GetById(32), 0);
      FieldActor_SetSpriteColor(Actor_GetById(32), 7);
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
    Func_0200581c__s0198();
  }
}

void *Unit_Get(s32 owner);
void Inventory_Equip(s32 id, s32 slot);
static __inline__ s32 Func_0200592c_(void) {
  return ((s32)FieldCamera_GetActor());
}
static __inline__ void Func_02005984_(s32 a) { Event_ResolvePendingActions(a); }
static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 b) {
  FieldCamera_MoveWithinMapBounds(x, y, z, b);
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
  MapGrid_SetScreenShake(a, b, c);
}
static __inline__ void FieldEvent_Wait_(s32 frames) { Event_Wait(frames); }
static __inline__ void Func_02005914_(s32 a, s32 b) {
  Camera_SetSpeedParameters(a, b);
}
static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}
static __inline__ void Func_020058ec_(s32 id, s32 angle) {
  Actor_FaceDirectionById(id, angle);
}
static __inline__ void Actor_OffsetPositionAndReset_(s32 id, s32 x, s32 z) {
  Actor_WalkByById(id, x, z);
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
  return ((void *)Actor_ShowEmoteById(id, emote, frames));
}
static __inline__ s32 FieldMessage_ShowAtActor_(s32 id, s32 flags) {
  return FieldMessage_ShowAtActor(id, flags);
}
static __inline__ s32 Event_AskYesNoWithActorAnimation_(s32 id, s32 flags) {
  return Event_AskYesNoWithActorAnimation(id, flags);
}
static __inline__ void Actor_CommitPositionAndActivate_(s32 id, s32 x, s32 z) {
  Actor_WalkByAndWaitById(id, x, z);
}
static __inline__ struct Actor *Actor_GetById_(s32 id) {
  return Actor_GetById(id);
}
static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 x,
                                                           s32 z) {
  Actor_SetDestinationOffsetById(id, x, z);
}
static __inline__ void FieldMessage_Show_(s32 message, s32 position) {
  FieldMessage_Show(message, position);
}
static __inline__ void PartyInventory_Add_(s32 item) {
  PartyInventory_Add(item);
}
static __inline__ void *Unit_Get_(s32 id) { return Unit_Get(id); }
static __inline__ void Func_020057ec_(s32 id, s32 slot) {
  Inventory_Equip(id, slot);
}
static __inline__ void Func_02005904_(s32 id, s32 emote) {
  FieldEffect_SetAttached(id, emote);
}
static __inline__ void Actor_StartRepeatedMotion_(s32 id, s32 motion) {
  Actor_StartRepeatedMotion(id, motion);
}
static __inline__ void GameFlag_SetBit_(s32 flag) { GameFlag_Set(flag); }
static __inline__ void Func_0200593c_(s32 a, s32 b) {
  Field_RequestBattleFromEncounterTable(a, b);
}

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                              \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

void Func_02000cd0(void) {
  s32 t2, t4, t75, t77, t78, t79, t80;
  s32 t101, t102, t135, t139, t140, t143, t144;
  s32 slot;
  s32 facing;

  Event_Start();
  Func_02005984_(0);
  t2 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t2, s32, 428) = 0x100;
  FIELD_AT(t2, s32, 436) = 120;
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  t4 = Func_0200592c_();
  FIELD_AT(t4, u8, 85) = 0;
  Camera_MoveWithinMapBounds_(0x2000000, 0x600000, 0xf00000, 0);
  wait_(1);
  Map_CenterOnFollowedPoint();
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
  FieldMapGrid_WaitForShakeToSettle();
  Func_02005914_(0x6666, 0xccc);
  Camera_MoveWithinMapBounds_(0x2000000, 0x600000, 0x1280000, 1);
  FieldCamera_WaitForArrival();
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
  gMainRoutineWork.battleRule24b = 2;
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_Ov6a5, 10);
  Event_SetDefeatDestination((s32)&SceneId_Ov6a5, 10);
  GameFlag_SetBit_(0x11b);
  Func_0200593c_(13, 5);
  return;
}

void Func_02001388(void) {
  s32 fast;
  s32 slow;
  s32 t24;
  s32 t25;
  s32 t26;
  s32 t27;
  s32 t66;
  s32 t67;
  s32 t68;
  s32 t69;
  s32 t70;
  s32 t20;
  s32 t21;
  s32 t22;
  s32 t23;
  s32 t33;
  s32 t34;
  s32 t35;
  s32 t36;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  fast = 0xa000;
  Actor_SetPositionAndFacing(4, 0x2000000, 0x1380000, fast);
  Actor_SetPositionAndFacing(7, 0x2100000, 0x1300000, fast);
  slow = 0x2000;
  Actor_SetPositionAndFacing(0x1b, 0x1f80000, 0x1300000, slow);
  {
    Actor_SetPositionAndFacing(0x1c, 0x2080000, 0x1200000, slow);
  }
  {
    s32 north = 0xc000;
    Actor_SetPositionAndFacing(5, 0x2280000, 0x1880000, north);
  }
  Actor_SetAnimation(0x1b, 9);
  Actor_SetAnimation(0x1c, 8);
  Actor_SetAnimation(4, 0x32);
  Actor_SetAnimation(7, 0xc);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgBlank5);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  FieldActor_ShowEmote(4, 0x100, 0);
  FieldActor_ShowEmote(7, 0x100, 0);
  FieldActor_ShowEmote(0x1c, 0x100, 0);
  FieldActor_ShowEmote(0x1b, 0x100, 0x1e);
  t20 = (s32)Actor_Get(4);
  FIELD_AT((t20 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t20 + 0x5a), u8, 0));
  t21 = (s32)Actor_Get(7);
  FIELD_AT((t21 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t21 + 0x5a), u8, 0));
  t22 = (s32)Actor_Get(0x1b);
  FIELD_AT((t22 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t22 + 0x5a), u8, 0));
  t23 = (s32)Actor_Get(0x1c);
  FIELD_AT((t23 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t23 + 0x5a), u8, 0));
  t24 = (s32)Actor_Get(4);
  FIELD_AT(t24, s32, 40) = 0x30000;
  t25 = (s32)Actor_Get(7);
  FIELD_AT(t25, s32, 40) = 0x30000;
  t26 = (s32)Actor_Get(0x1b);
  FIELD_AT(t26, s32, 40) = 0x30000;
  t27 = (s32)Actor_Get(0x1c);
  FIELD_AT(t27, s32, 40) = 0x30000;
  Actor_SetDestinationOffset(4, 8, 4);
  Actor_SetDestinationOffset(7, 8, 4);
  Actor_SetDestinationOffset(0x1b, -16, -8);
  Actor_SetDestinationOffset(0x1c, -16, -8);
  FieldActor_WaitForMoveById(4);
  t33 = (s32)Actor_Get(4);
  FIELD_AT((t33 + 0x5a), u8, 0) = (FIELD_AT((t33 + 0x5a), u8, 0) | 1);
  t34 = (s32)Actor_Get(7);
  FIELD_AT((t34 + 0x5a), u8, 0) = (FIELD_AT((t34 + 0x5a), u8, 0) | 1);
  t35 = (s32)Actor_Get(0x1b);
  FIELD_AT((t35 + 0x5a), u8, 0) = (FIELD_AT((t35 + 0x5a), u8, 0) | 1);
  t36 = (s32)Actor_Get(0x1c);
  FIELD_AT((t36 + 0x5a), u8, 0) = (1 | FIELD_AT((t36 + 0x5a), u8, 0));
  EventCamera_FollowActor(4, 1);
  Actor_WalkToAndWait(5, 0x228, 0x158);
  Actor_ShowEmote(5, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirectionAndWait(7, 0x4000);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(7, 0, 0);
  Actor_FaceDirectionAndWait(0x1c, 0x6000);
  Actor_ShowEmote(0x1c, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0);
  Actor_FaceDirection(0x1c, slow, 0);
  Actor_FaceDirectionAndWait(0x1b, slow);
  Actor_FaceDirectionAndWait(5, fast);
  FieldEvent_Wait(0xa);
  Event_ShowMessage(5, 0);
  Actor_WalkBy(5, -8, -24);
  Actor_FaceDirectionAndWait(4, slow);
  /* FAKEMATCH(dead): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 349 instructions differ
   * in tla-en. */
  do {
    Actor_SetAnimation(4, 3);
  } while (0);
  /* FAKEMATCH(dead): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 349 instructions differ
   * in tla-en. */
  do {
    FieldEvent_Wait(0x14);
  } while (0);
  Actor_FaceDirection(7, fast, 0);
  Actor_FaceDirectionAndWait(4, fast);
  Actor_FaceDirectionAndWait(5, fast);
  FieldEvent_Wait(0xa);
  Actor_SetDestinationOffset(4, -16, -8);
  Actor_SetDestinationOffset(7, -16, -8);
  Actor_SetDestinationOffset(5, -16, -8);
  Actor_SetDestinationOffset(0x1b, 0x10, 8);
  Actor_SetDestinationOffset(0x1c, 0x10, 8);
  t66 = (s32)Actor_Get(4);
  FIELD_AT(t66, s32, 40) = 0x40000;
  t67 = (s32)Actor_Get(7);
  FIELD_AT(t67, s32, 40) = 0x40000;
  t68 = (s32)Actor_Get(5);
  FIELD_AT(t68, s32, 40) = 0x40000;
  t69 = (s32)Actor_Get(0x1b);
  FIELD_AT(t69, s32, 40) = 0x40000;
  t70 = (s32)Actor_Get(0x1c);
  FIELD_AT(t70, s32, 40) = 0x40000;
  FieldEvent_Wait(4);
  if (gMainRoutineWork.sequenceResult == -1) {
    FieldUnit_AddHp(4, -10);
  }
  gMainRoutineWork.battleRule24b = 2;
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_Ov6a5, 0xb);
  Event_SetDefeatDestination((s32)&SceneId_Ov6a5, 0xb);
  Call2(Field_RequestBattleFromEncounterTable, 0xd, 6);
}

void Func_020016e8(void) {
  s32 northwest = FACING_NORTHWEST;
  s32 southeast;
  s32 north;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPositionAndFacing(4, PIXELS(512), PIXELS(312), northwest);
  Actor_SetPositionAndFacing(7, PIXELS(528), PIXELS(304), northwest);
  southeast = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(27, PIXELS(504), PIXELS(304), southeast);
  Actor_SetPositionAndFacing(28, PIXELS(520), PIXELS(288), southeast);
  Actor_SetPositionAndFacing(5, PIXELS(528), PIXELS(320), northwest);
  north = FACING_NORTH;
  Actor_SetPositionAndFacing(6, PIXELS(544), PIXELS(384), north);
  FieldCamera_MoveTo(PIXELS(520), PIXELS(104), PIXELS(312), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgBlank6);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_SetAnimation(27, 9);
  Actor_SetAnimation(28, 8);
  Actor_SetAnimation(4, 50);
  Actor_SetAnimation(7, 12);
  Actor_SetAnimation(5, 27);
  Actor_ShowEmote(4, 0x100, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(28, 0x100, 0);
  ((void *)Actor_ShowEmote(27, 0x100, 30));
  Actor_Get(4)->actionFlags &= ~1;
  Actor_Get(7)->actionFlags &= ~1;
  Actor_Get(5)->actionFlags &= ~1;
  Actor_Get(27)->actionFlags &= ~1;
  Actor_Get(28)->actionFlags &= ~1;
  Actor_Get(4)->velocityY = PIXELS(3);
  Actor_Get(7)->velocityY = PIXELS(3);
  Actor_Get(5)->velocityY = PIXELS(3);
  Actor_Get(27)->velocityY = PIXELS(3);
  Actor_Get(28)->velocityY = PIXELS(3);
  Actor_SetDestinationOffset(4, 8, 4);
  Actor_SetDestinationOffset(7, 8, 4);
  Actor_SetDestinationOffset(5, 8, 4);
  Actor_SetDestinationOffset(27, -16, -8);
  Actor_SetDestinationOffset(28, -16, -8);
  FieldActor_WaitForMoveById(4);
  Actor_Get(4)->actionFlags |= 1;
  Actor_Get(7)->actionFlags |= 1;
  Actor_Get(5)->actionFlags |= 1;
  Actor_Get(27)->actionFlags |= 1;
  Actor_Get(28)->actionFlags |= 1;
  EventCamera_FollowActor(4, 1);
  Actor_WalkToAndWait(6, 552, 344);
  Actor_ShowEmote(6, 0x102, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  ((void *)Actor_ShowEmote(28, 0x102, 30));
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(27, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(6, north);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_WalkBy(6, 0, -24);
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirectionAndWait(4, southeast);
  Actor_FaceDirection(6, northwest, 0);
  /* FAKEMATCH(dead): the three calls kept in a once-only block; as the
   * plain calls, 18 of 400 instructions differ in tla-en (the angles' copies
   * into r3 in five of the six placements, and the argument order of the
   * calls around the block). */
  do {
    Actor_SetAnimation(4, 3);
    FieldEvent_Wait(20);
    Actor_FaceDirection(7, northwest, 0);
  } while (0);
  Actor_FaceDirectionAndWait(4, northwest);
  Actor_FaceDirectionAndWait(5, northwest);
  FieldEvent_Wait(10);
  Actor_SetDestinationOffset(4, -16, -8);
  Actor_SetDestinationOffset(7, -16, -8);
  Actor_SetDestinationOffset(5, -16, -8);
  Actor_SetDestinationOffset(6, -16, -8);
  Actor_SetDestinationOffset(27, 16, 8);
  Actor_SetDestinationOffset(28, 16, 8);
  Actor_Get(4)->velocityY = PIXELS(4);
  Actor_Get(7)->velocityY = PIXELS(4);
  Actor_Get(5)->velocityY = PIXELS(4);
  Actor_Get(6)->velocityY = PIXELS(4);
  Actor_Get(27)->velocityY = PIXELS(4);
  Actor_Get(28)->velocityY = PIXELS(4);
  FieldEvent_Wait(4);
  if (gMainRoutineWork.sequenceResult == -1)
    FieldUnit_AddHp(4, -10);
  gMainRoutineWork.battleRule24b = 2;
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_Ov6a5, 12);
  Event_SetDefeatDestination((s32)&SceneId_Ov6a5, 13);
  Event_ClearFlag(0x11b);
  Field_RequestBattleFromEncounterTable(13, 7);
}

void Func_02001ac4(s32 a0, s32 a1) {
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(((s32)Actor_Get(a0)), 2);
  Event_PlaySound(0xc9);
  FieldEvent_Wait(60);
  BattleEffect_SetObjectUpdateMode(((s32)Actor_Get(a0)), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Actor_SetAnimation(a1, 1);
}

/* VIEW(s0198): routine gData_02005ff0 as this part declared it; the joined file
 * has another. */
extern u8 gData_02005ff0__s0198[] __asm__("gData_02005ff0");
#define gData_02005ff0 gData_02005ff0__s0198
/* VIEW(s0198): routine gData_02005ff8 as this part declared it; the joined file
 * has another. */
extern u8 gData_02005ff8__s0198[] __asm__("gData_02005ff8");
#define gData_02005ff8 gData_02005ff8__s0198
/* VIEW(s0198): routine gData_02006088 as this part declared it; the joined file
 * has another. */
extern u8 gData_02006088__s0198[] __asm__("gData_02006088");
#define gData_02006088 gData_02006088__s0198
/* VIEW(s0198): routine gData_020061e0 as this part declared it; the joined file
 * has another. */
extern u8 gData_020061e0__s0198[] __asm__("gData_020061e0");
#define gData_020061e0 gData_020061e0__s0198

void Func_02001bc4(void) {
  s32 fast;
  s32 slow;
  s32 mid;
  struct Actor *actor;
  s32 two, v_r5, t302, t310, t311, t438;

  void Func_02001b04(s32 clearStatus) {
    s32 count;
    s32 n;
    struct BattleUnit *member;
    s16 maxHp;

    count = Party_CountMembers();
    for (n = 0; n < count; n++) {
      struct MainRoutineWork *party = &gMainRoutineWork;

      member = Unit_Get(party->activeOwners[n]);
      maxHp = member->hp = member->maxHp;
      member->hpGauge = Vitals_Clamp((maxHp << 14) / maxHp);
      if (member->hpGauge == 0 && member->hp != 0)
        member->hpGauge = 1;
      member->ppGauge = Vitals_Clamp((member->pp << 14) / member->maxPp);
      if (member->ppGauge == 0 && member->pp != 0)
        member->ppGauge = 1;
      if (clearStatus == 1) {
        member->poison = 0;
        member->haunt = 0;
      }
    }
  }

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(0)->priorityFlags |= 1;
  Actor_Get(1)->priorityFlags |= 1;
  Actor_Get(2)->priorityFlags |= 1;
  Actor_Get(3)->priorityFlags |= 1;
  Actor_Get(4)->priorityFlags |= 1;
  Actor_Get(5)->priorityFlags |= 1;
  Actor_Get(7)->priorityFlags |= 1;
  Actor_Get(6)->priorityFlags |= 1;
  Actor_Get(30)->priorityFlags |= 1;
  Actor_Get(27)->priorityFlags |= 1;
  Actor_Get(28)->priorityFlags |= 1;
  mid = 0xc000;
  fast = 0xa000;
  Actor_SetPositionAndFacing(4, 0x1f80000, 0x1300000, fast);
  Actor_SetPositionAndFacing(7, 0x2100000, 0x1200000, fast);
  slow = 0x2000;
  Actor_SetPositionAndFacing(27, 0x1e80000, 0x1280000, slow);
  Actor_SetPositionAndFacing(28, 0x1f80000, 0x1180000, slow);
  Actor_SetPositionAndFacing(5, 0x2100000, 0x1300000, fast);
  Actor_SetPositionAndFacing(6, 0x2280000, 0x1200000, mid);
  Actor_SetPositionAndFacing(29, 0x1e00000, 0x1680000, mid);
  Actor_SetPositionAndFacing(30, 0x2500000, 0x1880000, 0);
  Actor_SetAnimation(5, 19);
  Actor_SetAnimation(6, 19);
  Actor_SetAnimation(4, 19);
  Actor_SetAnimation(7, 13);
  actor = Actor_Get(4);
  actor->scaleX = -actor->scaleX;
  actor = Actor_Get(6);
  actor->scaleX = -actor->scaleX;
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWasnTThe);
  Actor_SetAnimation(27, 9);
  Actor_SetAnimation(28, 8);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  Actor_FaceDirectionAndWait(27, 0xe000);
  FieldActor_ShowEmote(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 0);
  Actor_RunRepeatedMotion(4, 2);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(28, slow);
  Actor_FaceDirectionAndWait(27, slow);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(27, 0x102, 0);
  FieldActor_ShowEmote(28, 0x102, 30);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  FieldEvent_ShowMessageThenWait(27, 0, 0);
  FieldActor_ShowEmote(28, 0x107, 30);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  Event_PlaySound(78);
  Actor_FaceDirection(27, 0x4000, 0);
  Actor_FaceDirectionAndWait(28, 0x4000);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1580000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Event_PlaySound(39);
  FieldActor_ShowEmote(27, 0x107, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  FieldActor_ShowEmote(28, 0x107, 30);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  Actor_ShowEmote(29, 0x108, 30);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  Actor_SetAnimation(28, 1);
  Actor_FaceDirection(28, 0x6000, 0);
  Actor_FaceDirectionAndWait(27, 0xe000);
  FieldEvent_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  Actor_FaceDirectionAndWait(29, 0);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  FieldActor_StartRepeatedMotion(28, 2);
  Actor_RunRepeatedMotion(27, 2);
  Actor_FaceDirection(28, slow, 0);
  Actor_FaceDirectionAndWait(27, slow);
  FieldEvent_Wait(5);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  Actor_FaceDirectionAndWait(29, mid);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  FieldActor_ShowEmote(28, 0x102, 30);
  Actor_FaceDirection(28, 0x4000, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 3, 20);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  Actor_FaceDirectionAndWait(28, 0x6000);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, 0xe000);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  FieldEvent_ShowMessageThenWait(27, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  Actor_SetAnimation(27, 3);
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  FieldEvent_Wait(10);
  EventCamera_FollowActor(27, 1);
  Actor_SetScript(27, (s32)gData_02005ff8);
  FieldEvent_Wait(30);
  Actor_SetScript(29, (s32)gData_02005ff8);
  Actor_SetScript(28, (s32)gData_02005ff8);
  FieldActor_RefreshSelectorById(29);
  Actor_Get(29)->priorityFlags |= 64;
  Actor_Get(27)->priorityFlags |= 64;
  Actor_Get(28)->priorityFlags |= 64;
  Actor_Get(29)->motionFlags = 2;
  Actor_SetScript(29, (s32)gData_02006088);
  FieldActor_RefreshSelectorById(27);
  Actor_SetScript(27, (s32)gData_02006088);
  FieldActor_RefreshSelectorById(28);
  Actor_SetScript(28, (s32)gData_02006088);
  FieldActor_RefreshSelectorById(29);
  Actor_WalkToAndWait(29, 0x137, 0x100);
  Actor_FaceDirection(29, 0x4000, 0);
  FieldActor_RefreshSelectorById(27);
  Actor_WalkToAndWait(27, 0x14c, 0x10a);
  Actor_FaceDirection(27, 0x4000, 0);
  FieldActor_RefreshSelectorById(28);
  Actor_WalkToAndWait(28, 0x122, 0x10a);
  Actor_FaceDirection(28, 0x4000, 0);
  Actor_Get(29)->priorityFlags |= 32;
  Actor_Get(27)->priorityFlags |= 32;
  Actor_Get(28)->priorityFlags |= 32;
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_PlaySound(78);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  Actor_SetPositionAndFacing(30, 0x2000000, 0x1480000, mid);
  Func_02003a6c(0);
  Scene_PlayMusic();
  FieldEvent_Wait(10);
  Actor_SetPositionAndFacing(0, 0x2180000, 0x1700000, mid);
  Actor_SetPositionAndFacing(1, 0x2180000, 0x1800000, mid);
  Actor_SetPositionAndFacing(2, 0x2280000, 0x1700000, mid);
  Actor_SetPositionAndFacing(3, 0x2280000, 0x1600000, mid);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  EventCamera_FollowActor(0, 1);
  Actor_WalkTo(0, 0x218, 0x150);
  Actor_WalkTo(1, 0x218, 0x160);
  Actor_WalkTo(2, 0x228, 0x150);
  Actor_WalkTo(3, 0x228, 0x140);
  FieldActor_WaitForMoveById(0);
  FieldActor_WaitForMoveById(1);
  FieldActor_WaitForMoveById(2);
  FieldActor_WaitForMoveById(3);
  Actor_FaceDirectionAndWait(30, 0x3000);
  FieldEvent_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(30, 4, 33);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(2, 4, 33);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_FaceDirectionAndWait(3, 0x4000);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(3, 0, 0);
  Actor_FaceDirectionAndWait(0, fast);
  FieldCamera_MoveTo(0x1400000, 0x600000, 0x1680000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  FieldCamera_MoveTo(0x2000000, 0x600000, 0x1600000, 1);
  FieldCamera_WaitForArrival();
  Func_02003c58(0);
  ACTOR_SET_MODE_AND_WAIT(1, 4, 33);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  Actor_FaceDirectionAndWait(2, fast);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_ShowEmote(3, 0x102, 30);
  FieldEvent_ShowMessageThenWait(3, 0, 0);
  Actor_RunRepeatedMotion(4, 2);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(30, mid);
  Actor_RunRepeatedMotion(30, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(5, 2);
  FieldEvent_Wait(5);
  FieldActor_StartRepeatedMotion(7, 2);
  FieldEvent_Wait(5);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(0, 0xe000);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(3, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(1, fast);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  Actor_FaceDirectionAndWait(30, slow);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  Actor_FaceDirectionAndWait(30, mid);
  Actor_WalkByAndWait(3, -0x30, 0);
  Actor_FaceDirection(3, mid, 0);
  Call2(Func_02001ac4, 3, 4);
  Actor_FaceDirection(4, slow, 0);
  actor = Actor_Get(4);
  actor->scaleX = -actor->scaleX;
  Actor_WalkByAndWait(3, 40, 0);
  Actor_WalkByAndWait(3, 8, -16);
  Actor_FaceDirection(3, mid, 0);
  Call2(Func_02001ac4, 3, 6);
  Actor_FaceDirection(6, 0x4000, 0);
  actor = Actor_Get(6);
  actor->scaleX = -actor->scaleX;
  Actor_FaceDirection(3, fast, 0);
  Call2(Func_02001ac4, 3, 7);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Call2(Func_02001ac4, 3, 5);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_RunRepeatedMotion(0, 2);
  Actor_FaceDirectionAndWait(0, fast);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_FaceDirection(30, slow, 0);
  FieldActor_ShowEmote(30, 0x109, 30);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  Actor_ShowEmote(1, 0x107, 30);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  Actor_ShowEmote(2, 0x109, 30);
  Actor_WalkTo(2, 0x228, 0x140);
  Actor_FaceDirectionAndWait(2, 0x8000);
  Actor_FaceDirection(1, mid, 0);
  ACTOR_SET_MODE_AND_WAIT(2, 4, 33);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_FaceDirectionAndWait(0, 0xe000);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_FaceDirectionAndWait(2, 0x6000);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_FaceEachOther(5, 6, 0);
  Actor_FaceEachOther(7, 4, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(30, 0);
  ACTOR_SET_MODE_AND_WAIT(30, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(30, 0);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  Actor_FaceDirection(3, 0x6000, 0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldEvent_Wait(50);
  ACTOR_SET_MODE_AND_WAIT(1, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(0, mid);
  FieldEvent_Wait(50);
  Actor_SetAnimation(3, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(6, 3);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(0, fast);
  Actor_FaceDirectionAndWait(4, slow);
  t302 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t302 != 0)
    goto L_0200a678;
  ACTOR_SET_MODE_AND_WAIT(0, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  goto L_0200a6fe;
L_0200a678:
  Actor_ShowEmote(0, 0x107, 30);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 33);
  t310 = (s32)Ram_HeapSlots->eventWork;
  t311 = FIELD_AT(t310, u16, 452);
  FIELD_AT(t310, u16, 452) = (t311 + 2);
  Actor_FaceDirectionAndWait(5, 0x8000);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirectionAndWait(5, 0x4000);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(0, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
L_0200a6fe:
  Actor_SetAnimation(2, 2);
  actor = Actor_Get(0);
  if (actor == 0)
    goto L_0200a71e;
  Actor_SetDestination(2, FIELD_AT(actor, s16, 10), FIELD_AT(actor, s16, 18));
L_0200a71e:
  Actor_SetAnimation(3, 2);
  actor = Actor_Get(0);
  if (actor == 0)
    goto L_0200a73e;
  Actor_SetDestination(3, FIELD_AT(actor, s16, 10), FIELD_AT(actor, s16, 18));
L_0200a73e:
  Actor_SetAnimation(1, 2);
  actor = Actor_Get(0);
  if (actor == 0)
    goto L_0200a75e;
  Actor_SetDestination(1, FIELD_AT(actor, s16, 10), FIELD_AT(actor, s16, 18));
L_0200a75e:
  Actor_SetScript(2, (s32)gData_02005ff0);
  Actor_SetScript(3, (s32)gData_02005ff0);
  Actor_SetScript(1, (s32)gData_02005ff0);
  FieldActor_RefreshSelectorById(2);
  FieldActor_RefreshSelectorById(3);
  FieldActor_RefreshSelectorById(1);
  EventCamera_FollowActor(0, 1);
  FieldActor_WatchAndSetScript(4, 0);
  FieldActor_WatchAndSetScript(7, 0);
  FieldActor_WatchAndSetScript(5, 0);
  FieldActor_WatchAndSetScript(6, 0);
  FieldActor_WatchAndSetScript(30, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(0, 0x18000, 0xc000);
  Actor_WalkToAndWait(0, 0x21f, 0x16b);
  FieldActor_WaitForMoveById(0);
  Actor_FaceDirectionAndWait(0, 0xc000);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_WalkToAndWait(0, 0x23d, 0x166);
  Actor_WalkToAndWait(0, 0x248, 0x12c);
  Actor_WalkToAndWait(0, 0x248, 252);
  Actor_WalkToAndWait(0, 0x268, 247);
  Actor_WalkToAndWait(0, 0x269, 0x138);
  Actor_Get(0)->motionFlags = 2;
  Actor_SetScript(0, (s32)gData_020061e0);
  FieldActor_RefreshSelectorById(0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldEvent_Wait(30);
  Actor_Get(0)->priorityFlags |= 32;
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(30, 0, 0);
  Func_02003a6c(1);
  FieldEvent_Wait(100);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(30, 0x8000, 0);
  Func_02003c58(1);
  FieldEvent_Wait(60);
  EventCamera_FollowActor(4, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Actor_FaceDirectionAndWait(4, 0);
  FieldEvent_Wait(60);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(4, 0xe000);
  Actor_SetAnimation(7, 3);
  ACTOR_SET_MODE_AND_WAIT(6, 3, 20);
  Actor_SetAnimation(7, 2);
  actor = Actor_Get(4);
  two = 2;
  v_r5 = (s32)gData_02005ff0;
  if (actor == 0)
    goto L_0200a980;
  Actor_SetDestination(7, FIELD_AT(actor, s16, 10), FIELD_AT(actor, s16, 18));
L_0200a980:
  Actor_SetAnimation(6, 2);
  actor = Actor_Get(4);
  if (actor == 0)
    goto L_0200a9a0;
  Actor_SetDestination(6, FIELD_AT(actor, s16, 10), FIELD_AT(actor, s16, 18));
L_0200a9a0:
  Actor_SetAnimation(5, 2);
  actor = Actor_Get(4);
  if (actor == 0)
    goto L_0200a9c0;
  Actor_SetDestination(5, FIELD_AT(actor, s16, 10), FIELD_AT(actor, s16, 18));
L_0200a9c0:
  Actor_SetAnimation(30, 2);
  actor = Actor_Get(4);
  if (actor == 0)
    goto L_0200a9e0;
  Actor_SetDestination(30, FIELD_AT(actor, s16, 10), FIELD_AT(actor, s16, 18));
L_0200a9e0:
  Actor_SetScript(7, v_r5);
  Actor_SetScript(6, v_r5);
  Actor_SetScript(5, v_r5);
  Actor_SetScript(30, v_r5);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(30);
  EventCamera_FollowActor(4, 1);
  Actor_WalkByAndWait(4, -16, 0);
  Actor_SetScript(4, (s32)gData_02005ff8);
  FieldActor_RefreshSelectorById(4);
  Actor_Get(4)->priorityFlags |= 64;
  Actor_Get(29)->motionFlags = two;
  Actor_SetScript(4, (s32)gData_02006088);
  FieldActor_RefreshSelectorById(4);
  Actor_WalkToAndWait(4, 0x137, 0x100);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003a6c(2);
  t438 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t438, s32, 428) = 0x200;
  FIELD_AT(t438, s32, 436) = 24;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001b04(0);
  FieldEvent_SetExitRequest(1);
  Func_0200581c__s0198();
  return;
}

static __inline__ void W_Func_020058ec(s32 a0, s32 a1) {
  Actor_FaceDirectionById(a0, a1);
}

static __inline__ void W_Func_02005914(s32 a0, s32 a1) {
  Camera_SetSpeedParameters(a0, a1);
}
static __inline__ void W_Actor_ArmCallback(s32 a0, s32 a1, s32 a2) {
  Actor_ArmCallback(a0, a1, a2);
}
static __inline__ void W_Actor_EnableActionAndSetCallback(s32 a0, s32 a1) {
  Actor_SetMovementTypeById(a0, a1);
}

// Every Actor_SetModeById of mode 3 or 4 that is followed by its wait goes
// through this macro: its loop notes put the argument loads in the listing's
// order, and all 20 uses match through it.
#define SET_MODE_AND_WAIT(id, mode, frames)                                    \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                              \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

void Func_02002ae4(void) {
  s32 fast;
  s32 slow;
  s32 mid;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t5;
  s32 t6;
  s32 t7;
  s32 t8;
  s32 t9;
  s32 t10;
  s32 t11;
  s32 t12;
  s32 t13;
  s32 t67;
  s32 t166;
  s32 t167;
  s32 t168;
  s32 t169;
  s32 t184;
  s32 t185;
  s32 t186;
  s32 t269;
  s32 t292;
  s32 t295;
  s32 t298;
  s32 t325;
  s32 t330;
  s32 t358;
  s32 t361;
  s32 t364;
  s32 t367;
  s32 t381;
  s32 t382;
  s32 work;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  t2 = (s32)Actor_Get(0);
  FIELD_AT((t2 + 0x23), u8, 0) = (FIELD_AT((t2 + 0x23), u8, 0) | 1);
  t3 = (s32)Actor_Get(1);
  FIELD_AT((t3 + 0x23), u8, 0) = (FIELD_AT((t3 + 0x23), u8, 0) | 1);
  t4 = (s32)Actor_Get(2);
  FIELD_AT((t4 + 0x23), u8, 0) = (FIELD_AT((t4 + 0x23), u8, 0) | 1);
  t5 = (s32)Actor_Get(3);
  FIELD_AT((t5 + 0x23), u8, 0) = (FIELD_AT((t5 + 0x23), u8, 0) | 1);
  t6 = (s32)Actor_Get(4);
  FIELD_AT((t6 + 0x23), u8, 0) = (FIELD_AT((t6 + 0x23), u8, 0) | 1);
  t7 = (s32)Actor_Get(5);
  FIELD_AT((t7 + 0x23), u8, 0) = (FIELD_AT((t7 + 0x23), u8, 0) | 1);
  t8 = (s32)Actor_Get(7);
  FIELD_AT((t8 + 0x23), u8, 0) = (FIELD_AT((t8 + 0x23), u8, 0) | 1);
  t9 = (s32)Actor_Get(6);
  FIELD_AT((t9 + 0x23), u8, 0) = (FIELD_AT((t9 + 0x23), u8, 0) | 1);
  t10 = (s32)Actor_Get(0x1e);
  FIELD_AT((t10 + 0x23), u8, 0) = (FIELD_AT((t10 + 0x23), u8, 0) | 1);
  t11 = (s32)Actor_Get(0x1b);
  FIELD_AT((t11 + 0x23), u8, 0) = (FIELD_AT((t11 + 0x23), u8, 0) | 1);
  t12 = (s32)Actor_Get(0x1c);
  FIELD_AT((t12 + 0x23), u8, 0) = (FIELD_AT((t12 + 0x23), u8, 0) | 1);
  t13 = (s32)Actor_Get(0x1d);
  FIELD_AT((t13 + 0x23), u8, 0) = (1 | FIELD_AT((t13 + 0x23), u8, 0));
  fast = 0xa000;
  Actor_SetPositionAndFacing(4, 0x2080000, 0x1300000, fast);
  Actor_SetPositionAndFacing(7, 0x2180000, 0x1200000, fast);
  slow = 0x2000;
  Actor_SetPositionAndFacing(0x1b, 0x1e00000, 0x1280000, slow);
  Actor_SetPositionAndFacing(0x1c, 0x1f00000, 0x1180000, slow);
  Actor_SetPositionAndFacing(5, 0x2200000, 0x1300000, fast);
  mid = 0xc000;
  Actor_SetPositionAndFacing(6, 0x2300000, 0x1200000, mid);
  Actor_SetPositionAndFacing(0x1d, 0x1e00000, 0x1680000, mid);
  Actor_SetPositionAndFacing(0x1e, 0x2500000, 0x1880000, 0);
  Actor_SetAnimation(0x1b, 0xa);
  Actor_SetAnimation(0x1c, 9);
  W_Func_02005914(0x10000, slow);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWeDidItWe);
  SET_MODE_AND_WAIT(7, 4, 0x21);
  FieldEvent_ShowMessageThenWait(7, 0, 0);
  SET_MODE_AND_WAIT(5, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirectionAndWait(6, 0x6000);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_RunRepeatedMotion(0x1b, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0);
  Actor_RunRepeatedMotion(0x1c, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirectionAndWait(5, 0x8000);
  FieldEvent_Wait(0x28);
  W_Actor_ArmCallback(4, fast, 0);
  Actor_FaceDirectionAndWait(5, fast);
  FieldEvent_Wait(0x14);
  SET_MODE_AND_WAIT(5, 4, 0x21);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_RunRepeatedMotion(0x1b, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0);
  Actor_FaceDirection(4, 0xe000, 0);
  Actor_FaceDirection(7, 0x6000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirectionAndWait(6, 0x6000);
  FieldEvent_Wait(0x1e);
  Actor_RunRepeatedMotion(0x1c, 2);
  Event_OpenMessage(0x1c, 0);
  Actor_FaceDirection(6, 0x6000, 0);
  Actor_FaceDirectionAndWait(5, 0x8000);
  Actor_FaceDirection(4, 0x8000, 0);
  t67 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t67 == 0) {
    FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
    FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  }
  Event_PlaySound(0x4e);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1600000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(0xa);
  Event_PlaySound(0x27);
  FieldActor_ShowEmote(5, 0x100, 0x1e);
  Actor_FaceDirection(5, 0x6000, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1380000, 1);
  Actor_WalkToAndWait(0x1d, 0x1e0, 0x140);
  SET_MODE_AND_WAIT(0x1d, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_FaceDirectionAndWait(0x1d, 0);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  FieldActor_ShowEmote(7, 0x109, 0);
  FieldActor_ShowEmote(5, 0x109, 0);
  FieldActor_ShowEmote(6, 0x109, 0);
  FieldActor_ShowEmote(4, 0x109, 0x1e);
  Actor_FaceDirectionAndWait(0x1d, mid);
  Actor_WalkByAndWait(0x1d, 0x14, -20);
  Actor_FaceDirectionAndWait(0x1d, 0x8000);
  Call2(Func_02001ac4, 0x1d, 0x1b);
  FieldActor_ShowEmote(6, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_FaceDirectionAndWait(0x1d, mid);
  Call2(Func_02001ac4, 0x1d, 0x1c);
  Actor_WalkByAndWait(0x1d, -20, 0x14);
  Actor_FaceDirectionAndWait(0x1d, 0);
  FieldActor_ShowEmote(0x1d, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  FieldActor_StartRepeatedMotion(0x1b, 2);
  Actor_RunRepeatedMotion(0x1c, 2);
  FieldEvent_Wait(5);
  FieldActor_ShowEmote(5, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  SET_MODE_AND_WAIT(0x1d, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  SET_MODE_AND_WAIT(7, 4, 0x21);
  FieldEvent_ShowMessageThenWait(7, 0, 0);
  FieldActor_ShowEmote(0x1d, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_FaceDirectionAndWait(0x1b, 0x4000);
  FieldActor_ShowEmote(0x1b, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0);
  Actor_FaceDirectionAndWait(0x1c, 0x4000);
  FieldActor_ShowEmote(0x1c, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0);
  FieldActor_ShowEmote(0x1d, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0);
  Actor_FaceDirection(0x1b, 0xe000, 0);
  Actor_FaceDirectionAndWait(0x1c, 0x6000);
  FieldEvent_Wait(0xa);
  FieldActor_ShowEmote(0x1c, 0x102, 0);
  FieldActor_ShowEmote(0x1b, 0x102, 0x1e);
  Actor_FaceDirectionAndWait(0x1d, 0);
  Actor_FaceDirection(0x1c, 0x4000, 0);
  FieldEvent_Wait(0xa);
  SET_MODE_AND_WAIT(0x1d, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  FieldActor_ShowEmote(0x1d, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirectionAndWait(6, 0x6000);
  FieldEvent_Wait(0xa);
  SET_MODE_AND_WAIT(0x1d, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirectionAndWait(0x1d, mid);
  SET_MODE_AND_WAIT(0x1d, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirection(0x1c, 0x6000, 0);
  Actor_FaceDirectionAndWait(0x1b, 0xe000);
  FieldEvent_Wait(0x14);
  Actor_SetAnimation(0x1c, 3);
  SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  EventCamera_FollowActor(0x1b, 1);
  Actor_SetScript(0x1d, gData_02005ff8);
  FieldEvent_Wait(0x1e);
  Actor_SetScript(0x1b, gData_02005ff8);
  FieldEvent_Wait(0x1e);
  Actor_SetScript(0x1c, gData_02005ff8);
  FieldActor_RefreshSelectorById(0x1d);
  t166 = (s32)Actor_Get(0x1d);
  FIELD_AT((t166 + 0x23), u8, 0) = (FIELD_AT((t166 + 0x23), u8, 0) | 0x40);
  t167 = (s32)Actor_Get(0x1b);
  FIELD_AT((t167 + 0x23), u8, 0) = (FIELD_AT((t167 + 0x23), u8, 0) | 0x40);
  t168 = (s32)Actor_Get(0x1c);
  FIELD_AT((t168 + 0x23), u8, 0) = (0x40 | FIELD_AT((t168 + 0x23), u8, 0));
  t169 = (s32)Actor_Get(0x1d);
  FIELD_AT((t169 + 0x55), u8, 0) = 2;
  Actor_SetScript(0x1d, gData_02006088);
  FieldActor_RefreshSelectorById(0x1b);
  Actor_SetScript(0x1b, gData_02006088);
  FieldActor_RefreshSelectorById(0x1c);
  Actor_SetScript(0x1c, gData_02006088);
  FieldActor_RefreshSelectorById(0x1d);
  Actor_WalkToAndWait(0x1d, 0x137, 0x100);
  Actor_FaceDirection(0x1d, 0x4000, 0);
  FieldActor_RefreshSelectorById(0x1b);
  Actor_WalkToAndWait(0x1b, 0x14c, 0x10a);
  Actor_FaceDirection(0x1b, 0x4000, 0);
  FieldActor_RefreshSelectorById(0x1c);
  Actor_WalkToAndWait(0x1c, 0x122, 0x10a);
  Actor_FaceDirection(0x1c, 0x4000, 0);
  t184 = (s32)Actor_Get(0x1d);
  FIELD_AT((t184 + 0x23), u8, 0) = (FIELD_AT((t184 + 0x23), u8, 0) | 0x20);
  t185 = (s32)Actor_Get(0x1b);
  FIELD_AT((t185 + 0x23), u8, 0) = (FIELD_AT((t185 + 0x23), u8, 0) | 0x20);
  t186 = (s32)Actor_Get(0x1c);
  FIELD_AT((t186 + 0x23), u8, 0) = (0x20 | FIELD_AT((t186 + 0x23), u8, 0));
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_PlaySound(0x4e);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0);
  {
    s32 speed = 0xc000;
    Actor_SetPositionAndFacing(0x1e, 0x2000000, 0x1480000, speed);
  }
  Actor_SetPositionAndFacing(0, 0x2180000, 0x1500000, 0xc000);
  Actor_SetPositionAndFacing(1, 0x2180000, 0x1600000, 0xc000);
  Actor_SetPositionAndFacing(2, 0x2280000, 0x1600000, 0xc000);
  Actor_SetPositionAndFacing(3, 0x2280000, 0x1700000, 0xc000);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  Actor_FaceDirection(7, 0x4000, 0);
  Func_02003a6c(3);
  Scene_PlayMusic();
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_ShowEmote(4, 0x102, 0x1e);
  FieldActor_ShowEmote(5, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirectionAndWait(1, 0x8000);
  Actor_FaceDirectionAndWait(1, 0xc000);
  FieldEvent_Wait(0x14);
  Func_02003c58(0);
  FieldActor_ShowEmote(1, 0x103, 0x1e);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  Actor_FaceDirectionAndWait(0, 0xa000);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  FieldActor_ShowEmote(0x1e, 0x109, 0x1e);
  Actor_FaceDirectionAndWait(0x1e, 0x2000);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0);
  FieldActor_ShowEmote(1, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  FieldActor_ShowEmote(2, 0x109, 0x1e);
  Actor_WalkTo(2, 0x228, 0x140);
  Actor_FaceDirectionAndWait(2, 0xc000);
  Actor_FaceDirectionAndWait(2, 0x6000);
  Actor_FaceDirectionAndWait(2, 0x8000);
  SET_MODE_AND_WAIT(2, 4, 0x21);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_RunRepeatedMotion(0, 2);
  Actor_FaceDirectionAndWait(0, 0xe000);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_FaceDirectionAndWait(2, 0x6000);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0x6000, 0);
  Actor_FaceDirection(7, 0x6000, 0);
  Actor_FaceDirection(4, 0xe000, 0);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1e, 0);
  SET_MODE_AND_WAIT(0x1e, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1e, 0);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldEvent_Wait(0x3c);
  W_Func_020058ec(1, 0xc000);
  Actor_SetAnimation(3, 3);
  SET_MODE_AND_WAIT(1, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0, 0xe000);
  FieldEvent_Wait(0x3c);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  SET_MODE_AND_WAIT(5, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0, 0xc000);
  FieldEvent_Wait(0x1e);
  Actor_FaceDirectionAndWait(4, 0x4000);
  t269 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t269 == 0) {
    SET_MODE_AND_WAIT(0, 3, 0x14);
    FieldEvent_Wait(0xa);
    FieldEvent_ShowMessageThenWait(0, 0, 0);
    SET_MODE_AND_WAIT(0, 3, 0x14);
    FieldEvent_Wait(0xa);
  } else {
    FieldActor_ShowEmote(0, 0x107, 0x1e);
    SET_MODE_AND_WAIT(5, 4, 0x21);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 2);
    Actor_FaceDirectionAndWait(5, 0x8000);
    FieldEvent_ShowMessageThenWait(5, 0, 0);
    Actor_FaceDirectionAndWait(5, 0x4000);
    FieldEvent_Wait(0xa);
    Actor_RunRepeatedMotion(5, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(5, 0, 0);
    SET_MODE_AND_WAIT(0, 3, 0x14);
    FieldEvent_Wait(0xa);
    FieldEvent_ShowMessageThenWait(0, 0, 0);
  }
  Actor_SetAnimation(2, 2);
  t292 = (s32)Actor_Get(0);
  if (t292 != 0) {
    Actor_SetDestination(2, FIELD_AT(t292, s16, 10), FIELD_AT(t292, s16, 18));
  }
  Actor_SetAnimation(3, 2);
  t295 = (s32)Actor_Get(0);
  if (t295 != 0) {
    Actor_SetDestination(3, FIELD_AT(t295, s16, 10), FIELD_AT(t295, s16, 18));
  }
  Actor_SetAnimation(1, 2);
  t298 = (s32)Actor_Get(0);
  if (t298 != 0) {
    Actor_SetDestination(1, FIELD_AT(t298, s16, 10), FIELD_AT(t298, s16, 18));
  }
  Actor_SetScript(2, gData_02005ff0);
  W_Actor_EnableActionAndSetCallback(3, (s32)gData_02005ff0);
  Actor_SetScript(1, gData_02005ff0);
  FieldActor_RefreshSelectorById(2);
  FieldActor_RefreshSelectorById(3);
  FieldActor_RefreshSelectorById(1);
  EventCamera_FollowActor(0, 1);
  FieldActor_WatchAndSetScript(4, 0);
  FieldActor_WatchAndSetScript(7, 0);
  FieldActor_WatchAndSetScript(5, 0);
  FieldActor_WatchAndSetScript(6, 0);
  FieldActor_WatchAndSetScript(0x1e, 0);
  FieldEvent_Wait(0xa);
  Actor_SetSpeed(0, 0x18000, 0xc000);
  Actor_WalkToAndWait(0, 0x21f, 0x16b);
  Actor_FaceDirectionAndWait(0, 0xc000);
  FieldEvent_Wait(0xa);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_WalkToAndWait(0, 0x23d, 0x166);
  Actor_WalkToAndWait(0, 0x248, 0x12c);
  Actor_WalkToAndWait(0, 0x248, 0xfc);
  Actor_WalkToAndWait(0, 0x268, 0xf7);
  Actor_WalkToAndWait(0, 0x269, 0x138);
  t325 = (s32)Actor_Get(0);
  FIELD_AT((t325 + 0x55), u8, 0) = 2;
  Actor_SetScript(0, gData_020061e0);
  FieldActor_RefreshSelectorById(0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldEvent_Wait(0x1e);
  t330 = (s32)Actor_Get(0);
  FIELD_AT((t330 + 0x23), u8, 0) = (0x20 | FIELD_AT((t330 + 0x23), u8, 0));
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(0x1e, 0, 0);
  Func_02003a6c(1);
  FieldEvent_Wait(0x64);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(0x1e, 0x8000, 0);
  Func_02003c58(1);
  FieldEvent_Wait(0x3c);
  EventCamera_FollowActor(4, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(0x3c);
  Actor_FaceDirectionAndWait(4, 0);
  FieldEvent_Wait(0x3c);
  SET_MODE_AND_WAIT(5, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(4, 0xe000);
  Actor_SetAnimation(7, 3);
  SET_MODE_AND_WAIT(6, 3, 0x14);
  Actor_SetAnimation(7, 2);
  t358 = (s32)Actor_Get(4);
  if (t358 != 0) {
    Actor_SetDestination(7, FIELD_AT(t358, s16, 10), FIELD_AT(t358, s16, 18));
  }
  Actor_SetAnimation(6, 2);
  t361 = (s32)Actor_Get(4);
  if (t361 != 0) {
    Actor_SetDestination(6, FIELD_AT(t361, s16, 10), FIELD_AT(t361, s16, 18));
  }
  Actor_SetAnimation(5, 2);
  t364 = (s32)Actor_Get(4);
  if (t364 != 0) {
    Actor_SetDestination(5, FIELD_AT(t364, s16, 10), FIELD_AT(t364, s16, 18));
  }
  Actor_SetAnimation(0x1e, 2);
  t367 = (s32)Actor_Get(4);
  if (t367 != 0) {
    Actor_SetDestination(0x1e, FIELD_AT(t367, s16, 10),
                         FIELD_AT(t367, s16, 18));
  }
  Actor_SetScript(7, gData_02005ff0);
  Actor_SetScript(6, gData_02005ff0);
  Actor_SetScript(5, gData_02005ff0);
  Actor_SetScript(0x1e, gData_02005ff0);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(0x1e);
  EventCamera_FollowActor(4, 1);
  Actor_WalkByAndWait(4, -16, 0);
  Actor_SetScript(4, gData_02005ff8);
  FieldActor_RefreshSelectorById(4);
  t381 = (s32)Actor_Get(4);
  FIELD_AT((t381 + 0x23), u8, 0) = (FIELD_AT((t381 + 0x23), u8, 0) | 0x40);
  t382 = (s32)Actor_Get(0x1d);
  FIELD_AT((t382 + 0x55), u8, 0) = 2;
  Actor_SetScript(4, gData_02006088);
  FieldActor_RefreshSelectorById(4);
  Actor_WalkToAndWait(4, 0x137, 0x100);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(0xa);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003a6c(2);
  work = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT((work + 0x1ac), s32, 0) = 0x200;
  FIELD_AT((work + 0x1b4), s32, 0) = 0x18;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
  Func_0200581c__s0198();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

// Writes to out the ids of objects 0 to 63 standing in columns 4 to 8 and in
// the three rows from 72 less the row of z. Nothing in this overlay calls it.
// As ☀️'s MercuryLighthouseAerie_CollectBandSlots does.
void Func_02003a28(s32 *out, s32 zArg) {
  s32 z = zArg;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  u32 slot;

  z = 64 - (z >> 20);
  for (slot = 0; slot <= 63; slot++) {
    struct Actor *object = work->objects[slot];

    if (object != NULL) {
      s32 column = object->x >> 20;
      s32 row = object->z >> 20;

      if (column >= 4 && column <= 8 && z + 8 <= row && row < z + 11)
        *out++ = slot;
    }
  }
}

struct SceneWork {
  u8 pad0[12];
  s32 unk_0c;
};
/* VIEW(s0198): tag struct FieldWork as struct FieldWork__s0198; field_work.h
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s0198.c:2045:
 * dereferencing pointer to incomplete type. */
struct FieldWork__s0198 {
  u8 pad0[0x178];
  struct SceneWork unk_178;
};
struct Iwram {
  u8 pad0[32];
  struct FieldWork__s0198 *field;
};
/* VIEW(s0198): routine gData_020059fc as this part declared it; the joined file
 * has another. */
extern s32 gData_020059fc__s0198[] __asm__("gData_020059fc");
#define gData_020059fc gData_020059fc__s0198
/* VIEW(s0198): routine gData_02005a0c as this part declared it; the joined file
 * has another. */
extern s32 gData_02005a0c__s0198[] __asm__("gData_02005a0c");
#define gData_02005a0c gData_02005a0c__s0198
/* VIEW(s0198): routine gData_02005a14 as this part declared it; the joined file
 * has another. */
extern s32 gData_02005a14__s0198[] __asm__("gData_02005a14");
#define gData_02005a14 gData_02005a14__s0198
/* VIEW(s0198): routine gData_02005a1c as this part declared it; the joined file
 * has another. */
extern s32 gData_02005a1c__s0198[] __asm__("gData_02005a1c");
#define gData_02005a1c gData_02005a1c__s0198
/* VIEW(s0198): routine gData_020069dc as this part declared it; the joined file
 * has another. */
extern s32 gData_020069dc__s0198[] __asm__("gData_020069dc");
#define gData_020069dc gData_020069dc__s0198

void Func_02003a6c(s32 mode) {
  s32 speed;
  u32 count;
  s32 *list;
  s32 *p;
  s32 *q;
  u32 i;
  u32 j;
  struct SceneWork *work;

  work = &((struct Iwram *)Ram_HeapSlots)->field->unk_178;
  speed = 0x1999;
  count = 0;
  Data_02006a1c = work->unk_0c;
  switch (mode) {
  case 0:
  case 3:
    list = gData_020059fc;
    break;
  case 1:
    list = gData_02005a0c;
    break;
  case 2:
    list = gData_02005a14;
    break;
  default:
    list = gData_02005a1c;
    break;
  }

  for (i = 0; i <= 4 && list[i] != 64; i++) {
    ((struct SceneActor *)Actor_Get(list[i]))->unk_55 = 0;
    FieldActor_SetSpritePriority(list[i], 1);
    count++;
  }

  for (i = 0; i <= 5; i++) {
    ((struct SceneActor *)Actor_Get(gData_020069dc[i]))->unk_23 |= 32;
    FieldActor_SetSpritePriority(gData_020069dc[i], 0);
  }

  Event_PlaySound(223);

  for (i = 0; i <= 227; i++) {
    work->unk_0c -= speed;
    for (j = 0; j < count; j++) {
      struct SceneActor *actor;

      ((struct SceneActor *)Actor_Get(list[j]))->unk_10 += speed;
      actor = ((struct SceneActor *)Actor_Get(list[j]));
      actor->unk_40 = ((struct SceneActor *)Actor_Get(list[j]))->unk_10;
    }
    if ((i & 3) == 3)
      speed += 0x1999;
    if (speed > 0x17fff)
      speed = 0x18000;
    System_Wait(1);
    if (mode == 0) {
      if (i == 40)
        FieldCamera_MoveTo(128 << 18, 208 << 15, 152 << 17, 1);
      if (i == 120)
        break;
    } else if (mode == 1) {
      if (i == 40)
        FieldCamera_MoveTo(128 << 18, 208 << 15, 176 << 17, 1);
      if (i == 120)
        break;
    } else if (mode == 3) {
      if (i == 40)
        EventCamera_FollowActor(0, 1);
      if (i == 120)
        break;
    }
  }

  for (i = 0; i <= 5; i++) {
    ((struct SceneActor *)Actor_Get(gData_020069dc[i]))->unk_23 &= 223;
    FieldActor_SetSpritePriority(gData_020069dc[i], 1);
  }

  for (i = 0; i < count; i++) {
    Actor_SetPosition(list[i], 0, 0);
    ((struct SceneActor *)Actor_Get(list[i]))->unk_55 = 0;
  }
}

/* VIEW(s0198): tag struct HeapSlotWords as struct HeapSlotWords__s0198;
 * module/scene/scene.h has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s0198.c:2132: structure
 * has no member named `map'. */
struct HeapSlotWords__s0198 {
  s32 unk00[8];
  u8 *map;
};

/* VIEW(s0198): routine gData_02005a20 as this part declared it; the joined file
 * has another. */
extern s32 gData_02005a20__s0198[] __asm__("gData_02005a20");
#define gData_02005a20 gData_02005a20__s0198

void Func_02003c58(s32 mode) {
  struct ScrollWork *work =
      (struct ScrollWork
           *)(((struct HeapSlotWords__s0198 *)Ram_HeapSlots)->map + 376);
  s32 speed = 0x18000;
  u32 count = 0;
  u32 i;
  s32 *ids = gData_02005a20;

  for (i = 0; i <= 4 && ids[i] != 64; i++) {
    Actor_Get(ids[i])->motionFlags = 0;
    count++;
  }
  if (mode != 0) {
    Event_PlaySound(223);
    i = 0;
    work->height += 0x18000;
    while (work->height < Data_02006a1c) {
      u32 j;

      for (j = 0; j < count; j++) {
        struct Actor *actor;

        Actor_Get(ids[j])->z -= speed;
        actor = Actor_Get(ids[j]);
        actor->targetZ = Actor_Get(ids[j])->z;
      }
      if ((i & 3) == 3 && Data_02006a1c < speed * 15 * 2 + work->height)
        speed = speed * 15 / 16;
      if (speed <= 0xccb)
        speed = 0xccc;
      if (mode == 1 && i == 40)
        FieldCamera_MoveTo(0x1200000, 0x680000, 0x1700000, 1);
      System_Wait(1);
      i++;
      if (i > 119)
        break;
      work->height += speed;
    }
  }
  work->height = Data_02006a1c;
  Map_CenterOnFollowedPoint();
  System_Wait(2);
}

s32 Party_AddMember(s32 value);

s32 Scene_Initialize(void) {
  s32 t11;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t5;
  s32 t6;
  s32 t7;
  s32 t8;
  s32 t9;
  s32 t25;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
  Call2(HangingVine_Place, 0xf, 1);
  HangingVine_Place(0x10, 1);
  t2 = Actor_Get(gMainRoutineWork.currentOwner);
  FIELD_AT((t2 + 0x23), u8, 0) = (0x40 | FIELD_AT((t2 + 0x23), u8, 0));
  t3 = Actor_Get(17);
  FIELD_AT((t3 + 0x23), u8, 0) = (FIELD_AT((t3 + 0x23), u8, 0) | 0x80);
  t4 = Actor_Get(18);
  FIELD_AT((t4 + 0x23), u8, 0) = (FIELD_AT((t4 + 0x23), u8, 0) | 0x80);
  t5 = Actor_Get(19);
  FIELD_AT((t5 + 0x23), u8, 0) = (FIELD_AT((t5 + 0x23), u8, 0) | 0x80);
  t6 = Actor_Get(20);
  FIELD_AT((t6 + 0x23), u8, 0) = (FIELD_AT((t6 + 0x23), u8, 0) | 0x80);
  t7 = Actor_Get(21);
  FIELD_AT((t7 + 0x23), u8, 0) = (FIELD_AT((t7 + 0x23), u8, 0) | 0x80);
  t8 = Actor_Get(22);
  FIELD_AT((t8 + 0x23), u8, 0) = (0x80 | FIELD_AT((t8 + 0x23), u8, 0));
  t9 = Event_GetFlag(0xa21);
  if (t9 == 0) {
    Func_02004644();
  } else {
    t11 = Actor_Get(24);
    FIELD_AT(t11, s32, 24) = 0;
  }
  if ((u32)((FIELD_AT(&gMainRoutineWork, u16, 482) - 1) << 16) > 0x10000) {
    Actor_SetPosition(24, 0, 0);
  }
  Actor_SetSpriteFlags(Actor_Get(36), 0);
  Actor_SetSpriteFlags(Actor_Get(36), 0);
  Task_AddFrame((s32)Func_020005b8, 0x480);
  if (gMainRoutineWork.entrance == 9) {
    Func_02000cd0();

    gMainRoutineWork.retreatScene = (s32)MsgRobe;
    gMainRoutineWork.retreatEntrance = 1;
  } else {
    if (gMainRoutineWork.entrance == 0xa) {
      Party_AddMember(5);
      Func_02001388();
    } else {
      if (gMainRoutineWork.entrance == 0xb) {
        Party_AddMember(6);
        Func_020016e8();
      } else {
        if (gMainRoutineWork.entrance == 0xc) {
          Func_02002ae4();
        } else {
          if (gMainRoutineWork.entrance == 0xd) {
            Func_02001bc4();
          } else {
            t25 = Event_GetFlag(0xa21);
            if (t25 != 0) {
              Actor_SetPosition(27, 0, 0);
              Actor_SetPosition(28, 0, 0);
            }
          }
        }
      }
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"
#undef FIELD_AT
#undef ACTOR_SET_MODE_AND_WAIT
#undef SET_MODE_AND_WAIT

/* VIEW(s3f54): routine gPaletteCyclePausesInEvents as this part declared it;
 * the joined file has another. */
// ---- games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s3f54.c ----
extern s32
    gPaletteCyclePausesInEvents__s3f54 __asm__("gPaletteCyclePausesInEvents");
#define gPaletteCyclePausesInEvents gPaletteCyclePausesInEvents__s3f54

void Func_02003f54(s32 a0) { gPaletteCyclePausesInEvents = a0; }

/* VIEW(s3f54): routine gPaletteCycleFrame as this part declared it; the joined
 * file has another. */
extern s32 gPaletteCycleFrame__s3f54 __asm__("gPaletteCycleFrame");
#define gPaletteCycleFrame gPaletteCycleFrame__s3f54

// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame,
                          gPaletteCyclePausesInEvents, gPaletteCycleColors)

/* VIEW(s0198): routine MapGrid_SetCellHeight as void MapGrid_SetCellHeight();
 * module/scene/scene.h has void MapGrid_SetCellHeight(void); not tried:
 * MapGrid_SetCellHeight's bound declaration was not found. */
extern void Func_020057c4__s0198() __asm__("MapGrid_SetCellHeight");

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gLighthousePalette13, PaletteCycle_Step,
                                        gLighthouseUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_020057c4__s0198,
                             MapGrid_SetAttributeBAt)

void Func_020040f4(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkd46 = i;

  actor->motionFlags = 4;
  Actor_SetSpriteFlags(actor, 0);

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
  Actor_SetSpriteFlags(actor, 1);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  shadow = EventCamera_GetActor();
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
  FieldActor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkd46 = 1;
  work->exit = 0;
  Func_0200581c__s0198();
}

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

/* VIEW(s3f54): routine Actor_StartScript as void Actor_StartScript(struct
 * Object6a5 *object, const void *script); s0198 has void
 * Actor_StartScript(struct Actor *object, const void *script); not tried:
 * Actor_StartScript's bound declaration was not found. */
extern void
Func_02005764__s3f54(struct Object6a5 *object,
                     const void *script) __asm__("Actor_StartScript");
/* VIEW(s3f54): routine gData_020069fc as this part declared it; the joined file
 * has another. */
extern u8 gData_020069fc__s3f54[] __asm__("gData_020069fc");
#define gData_020069fc gData_020069fc__s3f54

void Whirlwind_Update(struct Object6a5 *object) {
  struct EventWork6a5 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Vector3 position;
  s32 distance;
  s32 near;
  s32 timer;
  u8 *flags;

  if (work->unk166 != 0 || work->unk15a != 0 || work->unk15e != 0 ||
      work->unk168 != 0) {
    Actor_SetAnimationSpeed(object, 0);
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  timer = object->unk64 + 1;
  object->unk64 = timer;
  if ((timer & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *from = leader->sprite;
    s8 *to = object->sprite;
    s32 priority = from[9] & 12;
    to[9] = (to[9] & ~12) | priority;
  }
  if (MapGrid_GetCellBehavior(object->x, object->z, 2) != 255) {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Camera_WorldToScreen(&position);
    if (position.x <= 0x1100000) {
      if (position.x >= -0x200000) {
        if (object->unk62 == 0) {
          distance = leader->y - object->y;
          if (distance < 0)
            distance = object->y - leader->y;
          near = 0;
          if (Position_FlatDistance(&leader->x, &object->x) <= 12 &&
              distance < 0xc0000)
            near = 1;
          if (near != 0 && Event_GetFlag(0x104) == 0) {
            leader->watchedObject = (struct Actor *)object;
            leader->facing = object->angle;
            flags = &leader->priorityFlags;
            *flags &= ~1;
            work->unk16a = 200;
            gMainRoutineWork.renderMode = 0;
            object->unk62 = 1;
            object->unk63 = 1;
          }
        }
        object->x += IwramMulQ16(Trig_Cos(object->angle), object->speed);
        object->z += IwramMulQ16(Math_Sin(object->angle), object->speed);
        return;
      }
    }
  }
  object->unk63 = 0;
  object->update = 0;
  Func_02005764__s3f54(object, gData_020069fc);
  Event_PlaySound(228);
  Data_02006a28 = 0;
}

// ---- scene/jupiter_lighthouse_aerie_beacon/s44cc.c ----

// Scene code of overlay 0x6a5, in ROM order.

extern u8 gLightSpritesLarge[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);
/* VIEW(s44cc): routine FieldEffect_Spawn as this part declared it; the joined
 * file has another. */
void FieldEffect_Spawn__s44cc(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__s44cc

DEFINE_WHIRLWIND_SPAWN_UPPER(Whirlwind_SpawnUpper, Actor_SelectAnimation,
                             Whirlwind_Update, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS_B(Actor_EmitRisingSparksB, FieldEffect_Spawn)

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

static __inline__ void Func_02001c10_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

void Func_02004644(void) {
  struct Actor *left = Actor_GetById(9);
  struct Actor *right = Actor_GetById(10);
  struct Actor *rider = Actor_GetById(23);
  struct FieldSprite *sprite = rider->sprite;

  Func_02001c10_(24, 0x2000000, 0xf80000);
  Func_02001c10_(23, 0x2000000, 0xf80000);
  Func_02001c10_(9, 0x1d80000, 0x1000000);
  Func_02001c10_(10, 0x2280000, 0x1000000);
  Actor_GetById(24)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y = rider->terrainHeight;
  left->motionFlags = 0;
  left->y = left->terrainHeight;
  right->motionFlags = 0;
  right->y = right->terrainHeight;
  if (GameFlag_Test_(0xa0a)) {
    left->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += -0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(9), 4);
    Actor_SetSpriteColorMode(Actor_GetById(11), 4);
  }
  if (GameFlag_Test_(0xa0b)) {
    right->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(10), 4);
    Actor_SetSpriteColorMode(Actor_GetById(12), 4);
  }
  if (GameFlag_Test_(0xa0a) && GameFlag_Test_(0xa0b)) {
    left->y += 0x199900;
    right->y += 0x199900;
    rider->y += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
  }
}

/* VIEW(s44cc): variable gLightSpritesLarge as s16 gLightSpritesLarge;
 * module/scene/scene.h has u8 gLightSpritesLarge[]; tried as the rest of the
 * object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s0198.c:2552:
 * `gLightSpritesLarge' has an incomplete type. */
extern s16 gLightSpritesLarge__s44cc __asm__("gLightSpritesLarge");

/* VIEW(s44cc): routine Oam_AddSpriteAtDepthClamped as this part declared it;
 * the joined file has another. */
void Oam_AddSpriteAtDepthClamped__s44cc(
    struct LightOam *oam, s32 depth) __asm__("Oam_AddSpriteAtDepthClamped");
#define Oam_AddSpriteAtDepthClamped Oam_AddSpriteAtDepthClamped__s44cc

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge__s44cc, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge__s44cc,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge__s44cc,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge__s44cc,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD_COUNTED(LightSprites_AddCounted, gLightSpriteActors,
                                 gLightSpriteCount)

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#undef BYTE_AT
#undef FIELD_AT
#undef ACTOR_SET_MODE_AND_WAIT
#undef SET_MODE_AND_WAIT
#undef Func_020000ec
#undef gData_020068b0
#undef Func_02000090
#undef gData_02006338
#undef gData_02005ff0
#undef gData_02005ff8
#undef gData_02006088
#undef gData_020061e0
#undef gData_020059fc
#undef gData_02005a0c
#undef gData_02005a14
#undef gData_02005a1c
#undef gData_020069dc
#undef gData_02005a20
#undef gPaletteCyclePausesInEvents
#undef gPaletteCycleFrame
#undef gData_020069fc
#undef FieldEffect_Spawn
#undef Oam_AddSpriteAtDepthClamped
// @file
// Scene code of overlay 0x6a5, in ROM order.

/* VIEW(s4bd4): routine Actor_StartScript as this part declared it; the joined
 * file has another. */
extern void
Actor_StartScript__s4bd4(struct FieldEffect *effect,
                         const s32 *script) __asm__("Actor_StartScript");
#define Actor_StartScript Actor_StartScript__s4bd4
extern const s32 *gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

void CrackedTiles_Update(void);

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gCrackedTiles, CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

DEFINE_CRACKED_TILE_FALL_EVENT(CrackedTile_FallEvent, Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef Actor_StartScript
// @file
// Scene code of overlay 0x6a5, in ROM order.

/* VIEW(s539c): routine FieldEffect_Spawn as this part declared it; the joined
 * file has another. */
void FieldEffect_Spawn__s539c(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__s539c

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 use_setter) {
  FieldCamera_MoveWithinMapBounds(x, y, z, use_setter);
}

void CrackedTile_DropIn(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  for (i = 0; i <= 16; i++) {
    s32 angle = i << 12;

    velocity.x = Trig_Cos(angle);
    velocity.y = 0;
    velocity.z = Math_Sin(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16() * 3 << 14) >> 16) - 0x6000;
    velocity.z += ((u32)(Random16() * 3 << 13) >> 16) - 0x3000;
    FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity.x, velocity.y,
                      velocity.z, 0x1090001, &options);
  }
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  Func_020057ac_(0x50000, 0x50000, 0x10000);
  Func_020057ac_(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(s539c): routine CrackedTile_FallEvent as this part declared it; the
 * joined file has another. */
void CrackedTile_FallEvent__s539c(
    struct CellTarget *work,
    struct CellTargetEntry *entry) __asm__("CrackedTile_FallEvent");
#define CrackedTile_FallEvent CrackedTile_FallEvent__s539c

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_FallEvent)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)

// clang-format off
u32 gData_020059fc[] = {
    29, 27, 28, 64,
};
// clang-format on

u32 gData_02005a0c[] = {
    0,
    64,
};

u32 gData_02005a14[] = {
    4,
    64,
};

u32 gData_02005a1c[] = {
    64,
};

u32 gData_02005a20[] = {
    64,
};

#undef FieldEffect_Spawn
#undef CrackedTile_FallEvent

/* VIEW(data): routine Func_02000038 as this part declared it; the joined file
 * has another. */
void Func_02000038__data(void) __asm__("Func_02000038");
#define Func_02000038 Func_02000038__data
/* VIEW(data): routine Func_02000050 as this part declared it; the joined file
 * has another. */
void Func_02000050__data(void) __asm__("Func_02000050");
#define Func_02000050 Func_02000050__data

// clang-format off
LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

LATE_DATA s32 gData_02005f10[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
    SC_WAIT(5),
    SC_ADD(SW_FACING, 0x1200),
    SC_WAIT(5),
    SC_ADD(SW_FACING, 0x1200),
    SC_WAIT(5),
    SC_ADD(SW_FACING, -0x1200),
    SC_WAIT(2),
    SC_ADD(SW_FACING, -0x1200),
    SC_WAIT(2),
    SC_ADD(SW_FACING, -0x1200),
    SC_WAIT(5),
    SC_ADD(SW_FACING, -0x1200),
    SC_WAIT(5),
    SC_ADD(SW_FACING, 0x1200),
    SC_WAIT(5),
    SC_ADD(SW_FACING, 0x1200),
    SC_END,
};

LATE_DATA u32 gData_02005ff0[] = {
    1, 38,
};

LATE_DATA u32 gData_02005ff8[] = {
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_ACCEL, 0x10000),
    SC_ARG(2),
    SC_MOVE(PIXELS(480), PIXELS(72), PIXELS(360)),
    SC_SYNC,
    SC_MOVE(PIXELS(456), PIXELS(72), PIXELS(360)),
    SC_SYNC,
    SC_MOVE(PIXELS(440), PIXELS(72), PIXELS(256)),
    SC_SYNC,
    SC_MOVE(PIXELS(408), PIXELS(72), PIXELS(256)),
    SC_SYNC,
    SC_MOVE(PIXELS(409), PIXELS(32), PIXELS(312)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

LATE_DATA DEFINE_G_DATA_02006088(gData_02006088, 409, 376, 344, 312, 278)

LATE_DATA DEFINE_G_DATA_02006088(gData_020061e0, 616, 648, 681, 712, 264)

LATE_DATA s32 gData_02006338[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
    SC_ADD(SW_Y, PIXELS(4)),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
    SC_ADD(SW_Y, PIXELS(2)),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA DEFINE_G_DATA_02005B54(gData_0200641c)

LATE_DATA s32 gData_02006470[] = {
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_ARG(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(265),
    SCENE_EXIT(1, 251, 5),
    SCENE_EXIT(2, 2, 72),
    SCENE_EXIT(11, 259, 1),
    SCENE_EXIT(12, 259, 2),
    SCENE_EXIT(13, 259, 3),
    SCENE_EXIT(14, 262, 4),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    {0, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {1, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {2, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {3, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 1},
    {339, 2593, 1, PIXELS(472), PIXELS(40), PIXELS(256), FACING_WEST, 2, 0},
    {339, 2593, 1, PIXELS(552), PIXELS(40), PIXELS(256), FACING_EAST, 2, 1},
    {340, CONDITION_ALWAYS, 1, PIXELS(144), 0, PIXELS(200), FACING_EAST, 2, 0},
    {340, CONDITION_ALWAYS, 1, PIXELS(880), 0, PIXELS(200), FACING_WEST, 2, 1},
    {428, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {428, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {302, CONDITION_ALWAYS, 7, PIXELS(312), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 0},
    {302, CONDITION_ALWAYS, 7, PIXELS(344), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(376), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(648), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(680), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(712), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {425, 2593, 11, PIXELS(512), 0, PIXELS(248), FACING_EAST, 2, 0},
    {425, CONDITION_ALWAYS, 11, PIXELS(512), 0, PIXELS(248), FACING_EAST, 2, 1},
    {426, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {426, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_WEST, 2, 0},
    {33, 6691, 6, PIXELS(512), 0, PIXELS(328), FACING_SOUTH, 2, 0},
    {23, 6691, 6, PIXELS(480), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {57, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {303, 6689, (s32)gData_0200641c, PIXELS(512), PIXELS(8), PIXELS(264), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

LATE_DATA s32 gData_020068b0[] = {
    22, 9, 0xcccc, 22, 10, 0xcccc,
    39, 1, (s32)0x80010000, 23, 6, 0x10000,
    0, 1, 13, 16, (s32)0xc0010000, 2,
    0, 0, 0, 0, 1, 38,
#if TLA_PATCH_1
    0,
#endif
    22, 10, 0x16666, 22, 9, 0x16666,
    39, 4, 0, 30, 38, 22,
    26, 0, 23, 6, 0x40000, 0,
    8, 3, 0, 1, 17,
};
// clang-format on

#include "text/msg_ids.h"

void Func_0200065c(void);

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 100, 0x200, EVENT(Func_0200045c)},
    {TRIGGER_STEP, 101, 0xa21, EVENT(Func_0200065c)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgOnceYouVe)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgOnceJupiterIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     MSGNO(MsgOnceJupiterIsLit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS,
     MSGNO(MsgDoEvenThink)},
    {TRIGGER_TABLE_END},
};

// clang-format off
LATE_DATA u32 gData_020069dc[] = {
    17, 18, 19, 20, 21, 22,
};

LATE_DATA s32 gPaletteCycleFrame[] = {
    -1,
};

LATE_DATA u32 gPaletteCyclePausesInEvents[] = {
    1,
};

LATE_DATA u32 gData_020069fc[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gData_02005f10,
};
#undef Func_02000038
#undef Func_02000050
