/// @file
/// Jupiter Lighthouse's aerie and its beacon: Karst and Agatio wait
/// (JupiterLighthouse_KarstAndAgatioWait), the party casts the Jupiter Star
/// into the well (JupiterLighthouse_CastTheJupiterStar) and the beacon is lit
/// (JupiterLighthouse_BeaconIsLit); Karst and Agatio are fought
/// (JupiterLighthouse_FightKarstAndAgatio) and retreat
/// (JupiterLighthouse_KarstAndAgatioRetreat).
#include "types.h"
#include "item_ids.h"
#include "flags.h"
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_write.h"
#include "field/scene_tables.h"
#include "field/field_event.h"
#include "system/heap_release.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "version.h"
#include "field/map_grid.h"
#include "system/ram_buffer.h"
#include "field/call.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "field/cell_target.h"
#include "scene/scene.h"
#include "game/owner_state.h"
#include "debug/debug.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/vector3.h"
#include "system/field_effect.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "field/field_effect.h"
#include "macros.h"
#include "shared/tables.h"
#include "field/actor_script.h"

/// Plays an actor's animation and lets frames pass before the event goes on.
#define Actor_AnimateThenWait(id, animation, frames)                           \
  do {                                                                         \
    Actor_SetAnimation(id, animation);                                         \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

/// The placed actors this file moves by number. The party's rows take their
/// characters' numbers; the rest count on from 8 (a render of the beacon with
/// FLAG_ISAAC_AND_IVAN_FELL set shows Agatio's sheet at 27, Karst's at 28).
enum {
  /// The angels either side of the lid over the well, the archer statues that
  /// energize them, and the lid (two rows sharing the stone basin sheet).
  ACTOR_LEFT_ANGEL = 9,
  ACTOR_RIGHT_ANGEL,
  ACTOR_LEFT_ARCHER,
  ACTOR_RIGHT_ARCHER,
  ACTOR_LID_A = 23,
  ACTOR_LID_B,
  /// The six platforms floating over the well, west to east.
  ACTOR_FLOATING_PLATFORM_A = 17,
  ACTOR_FLOATING_PLATFORM_B = 18,
  ACTOR_FLOATING_PLATFORM_C = 19,
  ACTOR_FLOATING_PLATFORM_D = 20,
  ACTOR_FLOATING_PLATFORM_E = 21,
  ACTOR_FLOATING_PLATFORM_F = 22,
  ACTOR_AGATIO = 27,
  ACTOR_KARST = 28,
  ACTOR_ALEX = 29,
  ACTOR_KRADEN = 30,
  /// The two columns of light that drop into the well when the Jupiter Star
  /// is cast (JupiterLighthouse_CastTheJupiterStar).
  ACTOR_BEAM_A = 31,
  ACTOR_BEAM_B = 32,
  /// The beacon's light, rising once it is lit
  /// (gJupiterLighthouse_BeaconRisesScript).
  ACTOR_BEACON_ORB = 36,
  /// The Jupiter Star's shadow, an eighth of a sprite-489 point, following it
  /// down to the well.
  ACTOR_STAR_SHADOW = 37
};

s32 JupiterLighthouse_JumpAndHide(s32 a0) {
  Event_PlaySound(SE_JUMP);
  Actor_SetSpriteFlags(a0, 0);
  return 0;
}

s32 JupiterLighthouse_ShowSprite(s32 a0) {
  Actor_SetSpriteFlags(a0, 1);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

#include "../field/actor_park_record.c"

void JupiterLighthouse_CircleBeacon(struct Actor *self) {
  struct Actor *centre = Actor_Get(ACTOR_BEAM_A);
  u16 angle = self->timer;
  FIXED sine;
  FIXED x;
  FIXED z;

  self->x.fixed = centre->x.fixed + Trig_Cos(angle) * (self->speedLimit + 3);
  sine = Math_Sin(angle);
  z = centre->z.fixed;
  x = self->x.fixed;
  z += sine * 2;
  self->z.fixed = z;
  self->targetX = x;
  self->targetZ = z;
  self->timer += -0x800;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// Circles Alex (actor 36, sprite 57 as in room 5), wider across than deep, by
/// the actor's speed.
// As ☀️'s SceneEffect_UpdateCounterDrivenOrbit does.
void JupiterLighthouse_CircleBeaconWide(struct Actor *self) {
  struct Actor *centre = Actor_Get(ACTOR_BEACON_ORB);
  u16 angle = self->timer;
  FIXED cosine = Trig_Cos(angle);
  s32 radius = self->speedLimit + BYTE_AT(self, 98);
  self->x.fixed = centre->x.fixed + cosine * (radius + 6);
  self->z.fixed = centre->z.fixed + Math_Sin(angle) * (BYTE_AT(self, 98) + 4);
  self->targetX = self->x.fixed;
  self->targetZ = self->z.fixed;
  self->timer += -0x800;
}

#include "../field/actor_step_angle.c"

#undef BYTE_AT
#include "text/msg_ids.h"

struct CellTarget;
struct CellTargetEntry;

void JupiterLighthouse_KarstAndAgatioWait(void);

void SceneActor_ParkRecord(struct Actor *actor);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[16];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// The cells CrackedTiles_Step watches, and the cell it copies over the one the
/// leader steps on.
extern struct CellTarget gCrackedTiles;

void Event_End(void);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

/// The halves of a 16.16 coordinate.
struct FixedHalves {
  u16 decimal;
  s16 integer;
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

/// The whirlwind as its update reads it: struct Actor's fields under a type of
/// its own, so that its stores and the leader's are not taken to alias (as
/// struct Actor, the facing's store moves).
struct WhirlwindActor {
  u8 unused[6]; ///< the script and its cursor, not read here
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unusedAfterZ[0x1c];
  s32 speedLimit;
  u8 unusedAfterSpeed[0x1c];
  s8 *sprite;
  u8 unusedAfterSprite[0x0e];
  u8 riding;   ///< Actor.riseCounter: the leader rides it
  u8 carrying; ///< Actor.riseEnabled: it carries the leader still
  u16 timer;   ///< Actor.timer, frames it has turned
  u8 unusedAfterTimer[6];
  void (*update)(void);
};

extern s32 gJupiterLighthouse_BeaconStage;

extern s32 gJupiterLighthouse_BeaconRisesScript[];
extern s32 gJupiterLighthouse_BeaconSparkScript[];

void FieldCamera_WaitForArrival(void);

void JupiterLighthouse_LiftObjectsInBeam(s32 mode);
void JupiterLighthouse_ScrollBeam(s32 mode);

extern s32 gJupiterLighthouse_BeamLevel;

s32 HangingVine_Place();

extern u16 gPaletteCycleColors[];
extern u16 gLighthousePalette13[];
extern u16 gLighthouseUnlitColors[];

void MapGrid_SetAttributeBAt();
s32 MapGrid_GetCellBehavior();

extern s32 gJupiterLighthouse_WhirlwindRiding;

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Actor_StartScript(struct Actor *object, const void *script);

static inline void Event_PlayCue(s32 cue) { Sound_Play(cue); }

void JupiterLighthouse_RaiseBeaconLight(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 spawn;
  u32 rise;
  s32 angle;

  center = Actor_GetById(ACTOR_BEACON_ORB);
  spawn = 0;
  switch (gJupiterLighthouse_BeaconStage) {
  case 0:
    Event_PlayCue(220);
    MapGrid_SetShake(6, 6, 1);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 31, 24), 1);
    PaletteFade_Interpolate(8);
    break;
  case 8:
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
    PaletteFade_Interpolate(8);
    break;
  case 16:
    MapGrid_SetShake(1, 1, 1);
    break;
  case 24:
    center->x.fixed = toFIXED(304);
    center->y.fixed = -toFIXED(360);
    center->z.fixed = toFIXED(164);
    center->scaleX = FIXED_ONE;
    center->scaleY = FIXED_ONE;
    SceneActor_ParkRecord(center);
    Actor_SetScript(ACTOR_BEACON_ORB, gJupiterLighthouse_BeaconRisesScript);
    break;
  case 25:
    gJupiterLighthouse_BeaconStage--;
    if (*(s32 *)&center->y.fixed > 0) {
      PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), 0);
      PaletteFade_Interpolate(16);
      gJupiterLighthouse_BeaconStage++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(246);
      spawn = 1;
      center->y.fixed += toFIXED(9) / 4;
    }
    break;
  case 26:
    gJupiterLighthouse_BeaconStage--;
    if (*(volatile s32 *)&center->y.fixed > (160 << 14)) {
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
      PaletteFade_Interpolate(40);
      gJupiterLighthouse_BeaconStage++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(246);
      spawn = 1;
      center->y.fixed += toFIXED(9) / 4;
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
    PaletteFade_ApplyTarget(RGB_WHITE, 0);
    PaletteFade_Interpolate(12);
    break;
  case 48:
    FieldActor_Stop(ACTOR_BEACON_ORB);
    Event_SetFlag(FLAG_SCENE_TEMP_02);
    break;
  }
  if (spawn) {
    rise = toFIXED(Random_Below(80));
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               center->y.fixed - rise + -toFIXED(8),
                               center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Actor_StartScript(effect, gJupiterLighthouse_BeaconSparkScript);
      FieldActor_SetSpriteColor(effect, 1);
      effect->motionFlags = 0;
      angle = Random16() & 0xffff000;
      effect->timer = angle;
      effect->userValue = 0;
      {
        u32 counter = Random16() >> 13;

        effect->riseCounter = counter;
      }
      effect->update = JupiterLighthouse_CircleBeaconWide;
      effect->speedLimit = Math_Sin((u32)(Random16() * 0xffff) >> 20) * 24;
      effect->speedLimit = center->speedLimit >> 16;
      sprite->nextFrame = 0;
      sprite->priority = 1;
    }
  }
  gJupiterLighthouse_BeaconStage++;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void JupiterLighthouse_KarstAndAgatioWait(void) {

  if (Event_GetFlag(FLAG_ISAAC_AND_IVAN_FELL)) {
    Event_SetFlag(FLAG_SCENE_TEMP_00);
    if (!Event_GetFlag(FLAG_JUPITER_BEACON_LIT)) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgYouReLate);
      Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
      // "You're late, Felix!"
      Event_ShowMessage(ACTOR_KARST, 0);
      Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
      // "Now light the beacon!"
      Event_ShowMessage(ACTOR_AGATIO, 0);
      Event_End();
    }
  }
}

extern s32 gJupiterLighthouse_BeaconParticleScript[];

/// Every third frame, or every frame once flag 0x201 is set, spawns a particle
/// at a random height above object 31 that circles it
/// (JupiterLighthouse_CircleBeacon).
void JupiterLighthouse_SpawnBeaconParticle(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(FLAG_SCENE_TEMP_01) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(ACTOR_BEAM_A);
  if (Event_GetFlag(FLAG_SCENE_TEMP_01) != 0) {
    u32 random;

    random = Random16();
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               (s32)((u32)(random << 8) >> 16 << 16) +
                                   center->y.fixed - toFIXED(28),
                               center->z.fixed);
  } else {
    u32 random;

    random = Random16();
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               (s32)((u32)(random << 6) >> 16 << 16) +
                                   center->y.fixed - toFIXED(28),
                               center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gJupiterLighthouse_BeaconParticleScript);
    FieldActor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->timer = angle;
    effect->userValue = 0;
    effect->update = JupiterLighthouse_CircleBeacon;
    effect->speedLimit = Math_Sin((u32)(Random16() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 1;
  }
}

/// While the view's z is at most 177 pixels, sets object 36 at y 0x6f8000 and
/// full size on odd frames, or at y 0x688000 and 1.3 times the size on even
/// ones; beyond it, calls Actor_SetPosition(ACTOR_BEACON_ORB, 0, 0).
void JupiterLighthouse_PulseBeacon(void) {
  struct Actor *object = Actor_Get(ACTOR_BEACON_ORB);
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  Random16();
#if TLA_PATCH_1
  // From patch 1; the Japanese release goes on when object 36 is missing.
  if (object == NULL)
    return;
#endif
  if (((struct FixedHalves *)&map->viewZ)->integer <= 177) {
    ((struct SpriteRecord *)((struct FieldSprite *)Actor_Get(ACTOR_BEACON_ORB)
                                 ->sprite)
         ->record)
        ->unk05 = 1;
    if (gFrameCount & 1) {
      Actor_SetPosition(ACTOR_BEACON_ORB, 512, 264);
      Actor_Get(ACTOR_BEACON_ORB)->y.fixed = toFIXED(223) / 2;
      Actor_Get(ACTOR_BEACON_ORB)->scaleX = FIXED_ONE;
      Actor_Get(ACTOR_BEACON_ORB)->scaleY = FIXED_ONE;
    } else {
      Actor_SetPosition(ACTOR_BEACON_ORB, 512, 264);
      Actor_Get(ACTOR_BEACON_ORB)->y.fixed = toFIXED(209) / 2;
      Actor_Get(ACTOR_BEACON_ORB)->scaleX = FIXED_ONE * 13 / 10;
      Actor_Get(ACTOR_BEACON_ORB)->scaleY = FIXED_ONE * 13 / 10;
    }
  } else {
    Actor_SetPosition(ACTOR_BEACON_ORB, 0, 0);
  }
}

s32 Actor_MoveToObject();
s32 PartyInventory_Remove(s32 item_id);

extern s32 gJupiterLighthouse_StarFallsScript[];

void JupiterLighthouse_CastTheJupiterStar(void) {
  s32 answer, state;
  struct Actor *felix, *star, *shadow, *beamA, *beamB;
  struct FieldSprite *sprite;
  u8 *buffer;
  u8 *flags;

  if (GameFlag_Get(FLAG_ISAAC_AND_IVAN_FELL) != 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    // "There seems to be a wide hole for the Elemental Star. Cast the..."
    Message_ShowCentered((s32)MsgThereSeemsTo, 5);
    answer = Event_AskYesNoWithActorAnimation(4, 0);
    if (answer != 0) {
      Actor_SetSpeed(ACTOR_FELIX, 1);
      Actor_WalkToAndWait(ACTOR_FELIX, 514, 309);
    } else {
      Event_SetFlag(FLAG_JUPITER_BEACON_LIT);
      Event_SetFlag(FLAG_SHIP_FLIES);
      Event_ClearFlag(FLAG_JUPITER_LIGHTHOUSE_ALEX_HEALED_THE_PARTY);
      Event_ClearFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
      Event_ClearFlag(FLAG_RETREAT_BLOCKED);
      Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
      Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHEAST, 0);
      Actor_SetSpeed(ACTOR_FELIX, 1);
      Actor_WalkToAndWait(ACTOR_FELIX, 512, 294);
      Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
      FieldEvent_Wait(20);
      PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
      PaletteFade_Interpolate(20);
      System_Wait(20);
      PartyInventory_Remove(ITEM_MYTHRIL_BAG_B);
      felix = Actor_GetById(ACTOR_FELIX);
      star = FieldActor_Create(0x1e9, felix->x.fixed,
                               felix->y.fixed + toFIXED(8), felix->z.fixed);
      if (star != 0) {
        sprite = star->sprite;
        sprite->unk1b[0] = 0;
        sprite->fullColor = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        star->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        star->motionFlags = 0;
        star->retainSpriteCache = 1;
        star->speedLimit = toFIXED(2) / 5;
        star->acceleration = toFIXED(1) / 20;
        buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
        Item_LoadIcon(ITEM_JUPITER_STAR);
        Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
        HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
      }
      FieldActor_SetSpritePriority(ACTOR_STAR_SHADOW, 1);
      shadow = Actor_GetById(ACTOR_STAR_SHADOW);
      shadow->x.fixed = felix->x.fixed;
      shadow->y.fixed = toFIXED(96);
      shadow->z.fixed = felix->z.fixed;
      shadow->motionFlags = 3;
      shadow->acceleration = toFIXED(1) / 20;
      shadow->speedLimit = toFIXED(2) / 5;
      shadow->scaleX = FIXED_ONE / 8;
      shadow->scaleY = FIXED_ONE / 8;
      FieldEvent_SetMessageId((s32)MsgThereSeemsTo + 1);
      Event_PlaySound(MUS_CONFRONTATION);
      if (star != 0) {
        flags = &star->motionFlags;
        *flags = 0;
        star->gravity = toFIXED(3) / 5;
        star->bounce = FIXED_ONE * 4 / 5;
        star->velocityY = toFIXED(8);
        FieldActor_SetMoveTarget(star, 512, 128, 294);
        FieldActor_SetMoveTargetFX(shadow, toFIXED(512), -1, toFIXED(294));
        Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
        System_Wait(1);
        Actor_SetSpeed(ACTOR_PIERS, 0.8);
        Actor_WalkBy(ACTOR_PIERS, 12, 0);
        Actor_WalkByAndWait(ACTOR_FELIX, -12, 0);
        FieldActor_WaitForMoveById(ACTOR_PIERS);
        FieldActor_Stop(ACTOR_PIERS);
        System_Wait(1);
        Actor_SetAnimation(ACTOR_PIERS, ANIM_STAND);
        System_Wait(1);
        Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
        FieldActor_WaitForMove(star);
        state = 4;
        *flags = state;
        FieldEvent_Wait(90);
        *flags = 0;
        FieldActor_SetMoveTarget(star, 512, 128, 264);
        FieldActor_SetMoveTargetFX(shadow, toFIXED(512), -1, toFIXED(264));
        *flags = state;
        FieldEvent_Wait(60);
        *flags = 0;
        shadow->x.fixed = 0;
        shadow->targetX = TARGET_UNSET;
        shadow->targetY = TARGET_UNSET;
        shadow->targetZ = TARGET_UNSET;

        shadow->y.fixed = 0;
        shadow->z.fixed = 0;
        shadow->velocityX = 0;
        shadow->velocityY = 0;
        shadow->velocityZ = 0;
        shadow->timer = 0;
        FieldActor_WaitForMove(star);
        *flags = state;
        FieldEvent_Wait(90);
        *flags = 0;
        FieldActor_SetMoveTarget(star, 512, 92, 264);
        FieldActor_WaitForMove(star);
        star->x.fixed = 0;
        star->targetX = TARGET_UNSET;
        star->targetY = TARGET_UNSET;
        star->targetZ = TARGET_UNSET;
        star->y.fixed = 0;
        star->z.fixed = 0;
        star->velocityX = 0;
        star->velocityY = 0;
        star->velocityZ = 0;
        star->timer = 0;
      }
      FieldEvent_Wait(40);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
      PaletteFade_Interpolate(20);
      System_Wait(20);
      Actor_WalkBy(ACTOR_FELIX, 0, -16);
      Actor_WalkBy(ACTOR_PIERS, 0, -16);
      FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
      Actor_SetAnimation(ACTOR_PIERS, ANIM_STAND);
      FieldEvent_Wait(50);
      Event_PlaySound(SE_RUMBLE);
      FieldEvent_Wait(30);
      MapGrid_SetShake(1, 1, 1);
      FieldEvent_Wait(30);
      MapGrid_SetShake(2, 2, 1);
      FieldEvent_Wait(30);
      Actor_GetById(ACTOR_ISAAC)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_JENNA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_PIERS)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_SHEBA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_KRADEN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_AGATIO)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_KARST)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_BEAM_A)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_BEAM_B)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      FieldCamera_MoveTo(512, 96, 264, 1);
      FieldCamera_WaitForArrival();
      FieldActor_SetSpritePriority(ACTOR_BEAM_A, 1);
      FieldActor_SetSpritePriority(ACTOR_BEAM_B, 1);
      FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
      FieldActor_SetSpritePriority(ACTOR_PIERS, 1);
      FieldEvent_Wait(20);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEAM_A), 0);
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_BEAM_A), 7);
      beamA = Actor_GetById(ACTOR_BEAM_A);
      beamA->scaleY = -FIXED_ONE;
      beamA->scaleX = FIXED_ONE / 10;
      beamA->motionFlags = 0;
      beamA->x.fixed = toFIXED(512);
      beamA->y.fixed = toFIXED(128);
      beamA->z.fixed = toFIXED(264);
      beamA->priorityFlags |= 2;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEAM_B), 0);
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_BEAM_B), 7);
      beamB = Actor_GetById(ACTOR_BEAM_B);
      beamB->scaleY = -FIXED_ONE;
      beamB->scaleX = FIXED_ONE / 10;
      beamB->motionFlags = 0;
      beamB->x.fixed = toFIXED(512);
      beamB->y.fixed = toFIXED(160);
      beamB->z.fixed = toFIXED(264);
      beamB->priorityFlags |= 2;
      Actor_SetScript(ACTOR_BEAM_A, gJupiterLighthouse_StarFallsScript);
      Actor_SetScript(ACTOR_BEAM_B, gJupiterLighthouse_StarFallsScript);
      Task_AddFrame((s32)JupiterLighthouse_SpawnBeaconParticle,
                    TASK_PASS_VBLANK);
      Actor_SetPosition(ACTOR_LEFT_ANGEL, 0, 0);
      Actor_SetPosition(ACTOR_RIGHT_ANGEL, 0, 0);
      Actor_SetPosition(ACTOR_LID_A, 0, 0);
      Actor_SetPosition(ACTOR_LID_B, 0, 0);
      Event_PlaySound(SE_CRASH);
      MapGrid_SetShake(6, 6, 1);
      PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), 0);
      PaletteFade_Interpolate(16);
      System_Wait(16);
      Event_PlaySound(MUS_BEACON_LIT);
      System_Wait(180);
      PaletteFade_ApplyTarget(RGB_WHITE, 0);
      PaletteFade_Interpolate(60);
      Event_PlaySound(SE_RUMBLE);
      System_Wait(60);
      FieldTask_Remove(JupiterLighthouse_SpawnBeaconParticle);
      Event_SetFlag(FLAG_KEEP_MUSIC_ONCE);
      FieldEvent_SetExitRequest(2);
    }
    Event_End();
  }
}

struct Unit *Unit_Get(u32 unitId);
void Inventory_Equip(s32 id, s32 slot);
static __inline__ s32 JupiterLighthouse_GetCamera(void) {
  return ((s32)FieldCamera_GetActor());
}
static __inline__ void JupiterLighthouse_ResolvePending(s32 a) {
  Event_ResolvePendingActions(a);
}
static __inline__ void wait_(s32 frames) { wait(frames); }
static __inline__ void JupiterLighthouse_SetMessage(const void *message) {
  Event_SetMessageId(message);
}
static __inline__ void JupiterLighthouse_PlaySound(s32 sound) {
  Sound_Play(sound);
}
static __inline__ void FieldEvent_Wait_(s32 frames) { Event_Wait(frames); }
static __inline__ void *JupiterLighthouse_ShowEmote(s32 id, s32 emote,
                                                    s32 frames) {
  return ((void *)Actor_ShowEmoteById(id, emote, frames));
}
static __inline__ void PartyInventory_Add_(s32 item) {
  PartyInventory_Add(item);
}
static __inline__ void *Unit_Get_(s32 id) { return Unit_Get(id); }
static __inline__ void JupiterLighthouse_Equip(s32 id, s32 slot) {
  Inventory_Equip(id, slot);
}
static __inline__ void GameFlag_SetBit_(s32 flag) { GameFlag_Set(flag); }
static __inline__ void JupiterLighthouse_RequestBattle(s32 a, s32 b) {
  Field_RequestBattleFromEncounterTable(a, b);
}

void JupiterLighthouse_BeaconIsLit(void) {
  s32 t2, t4, t77, t78, t79, t80;
  s32 t101, t102, t139, t140, t143, t144;
  s32 slot;
  s32 facing;

  Event_Start();
  JupiterLighthouse_ResolvePending(0);
  t2 = (s32)Ram_HeapSlots->fieldWork;
  FIELD_AT(t2, s32, 428) = 0x100;
  FIELD_AT(t2, s32, 436) = 120;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  t4 = JupiterLighthouse_GetCamera();
  FIELD_AT(t4, u8, 85) = 0;
  FieldCamera_MoveTo(512, 96, 240, 0);
  wait_(1);
  MapGrid_CenterOnFollowedPoint();
  wait_(1);
  JupiterLighthouse_SetMessage(MsgWeHaveIgnited);
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(508), toFIXED(278),
                             facing = FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(532), toFIXED(278), facing);
  JupiterLighthouse_PlaySound(141);
  MapGrid_SetShake(4, 4, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  MapGrid_SetShake(3, 3, 1);
  FieldEvent_Wait_(60);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait_(60);
  JupiterLighthouse_PlaySound(0x186);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(512, 96, 296, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait_(20);
  Actor_AnimateThenWait(7, 3, 20);
  Actor_AnimateThenWait(4, 3, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait_(20);
  Actor_WalkBy(ACTOR_FELIX, 0, 8);
  Actor_WalkBy(ACTOR_PIERS, 0, 8);
  Actor_WalkBy(ACTOR_AGATIO, 32, -16);
  Actor_WalkBy(ACTOR_KARST, 48, -16);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  FieldActor_WaitForMoveById(ACTOR_AGATIO);
  FieldActor_WaitForMoveById(ACTOR_KARST);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHWEST, 0);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait_(5);
  JupiterLighthouse_PlaySound(78);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  JupiterLighthouse_PlaySound(34);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
  ((void *)Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0));
  JupiterLighthouse_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  FieldEvent_Wait_(10);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldEvent_Wait_(10);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  ((void *)Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30));
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  FieldEvent_Wait_(5);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
  }
  JupiterLighthouse_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ((void *)Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                           30));
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  FieldEvent_Wait_(10);
  Actor_WalkByAndWait(ACTOR_KARST, -16, -24);
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  JupiterLighthouse_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_Get(ACTOR_KARST)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_KARST)->velocityY = toFIXED(4);
  Actor_SetDestinationOffset(ACTOR_KARST, 0, 24);
  FieldActor_WaitForMoveById(ACTOR_KARST);
  Actor_Get(ACTOR_KARST)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  Message_ShowCentered(gFieldWork->message++, 5);
  PartyInventory_Add_(ITEM_MYTHRIL_BAG);
  for (slot = 0; slot < 15; slot++) {
    if ((((struct UnitItems *)Unit_Get_(4))->items[slot] & 0x1ff) == 224) {
      JupiterLighthouse_Equip(4, slot);
      break;
    }
  }
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_Wait_(5);
  Actor_WalkBy(ACTOR_FELIX, 0, 8);
  Actor_WalkByAndWait(ACTOR_AGATIO, -24, -8);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, -8);
  JupiterLighthouse_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  ((void *)Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30));
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  JupiterLighthouse_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldEvent_Wait_(10);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait_(30);
  ((void *)Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30));
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHWEST, 0);
  Event_OpenMessage(ACTOR_KARST, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
    gFieldWork->message++;
  } else {
    Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
    gFieldWork->message++;
    FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  }
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait_(5);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  JupiterLighthouse_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                              30);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
  ((void *)Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SMILE, 30));
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHEAST, 0);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  ((void *)Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30));
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHWEST, 0);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 2);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait_(5);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(
      (s32)&SceneId_JupiterLighthouseAerieBeacon, 10);
  Event_SetDefeatDestination((s32)&SceneId_JupiterLighthouseAerieBeacon, 10);
  GameFlag_SetBit_(FLAG_KEEP_MUSIC_HELD);
  JupiterLighthouse_RequestBattle(13, 5);
  return;
}

void JupiterLighthouse_FightKarstAndAgatio(void) {
  s32 fast;
  s32 slow;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  fast = FACING_NORTHWEST;
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(512), toFIXED(312), fast);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(528), toFIXED(304), fast);
  slow = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(504), toFIXED(304), slow);
  {
    Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(520), toFIXED(288), slow);
  }
  {
    s32 north = FACING_NORTH;
    Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(552), toFIXED(392), north);
  }
  Actor_SetAnimation(ACTOR_AGATIO, 9);
  Actor_SetAnimation(ACTOR_KARST, 8);
  Actor_SetAnimation(ACTOR_FELIX, 0x32);
  Actor_SetAnimation(ACTOR_PIERS, 0xc);
  FieldCamera_MoveTo(520, 104, 312, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgCallJenna);
  // "Felix!"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_PIERS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_AGATIO)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_KARST)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_FELIX)->velocityY = toFIXED(3);
  Actor_Get(ACTOR_PIERS)->velocityY = toFIXED(3);
  Actor_Get(ACTOR_AGATIO)->velocityY = toFIXED(3);
  Actor_Get(ACTOR_KARST)->velocityY = toFIXED(3);
  Actor_SetDestinationOffset(ACTOR_FELIX, 8, 4);
  Actor_SetDestinationOffset(ACTOR_PIERS, 8, 4);
  Actor_SetDestinationOffset(ACTOR_AGATIO, -16, -8);
  Actor_SetDestinationOffset(ACTOR_KARST, -16, -8);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_PIERS)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_AGATIO)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_KARST)->steeringFlags |= ACTOR_FACES_MOTION;
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  Actor_WalkToAndWait(ACTOR_JENNA, 552, 344);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "I thought you were taking too long, so I came to check on you...."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldEvent_Wait(10);
  // "They say they don't need us anymore..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_SOUTHWEST);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Agatio! Felix's sister isn't supposed to be here..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  // "What do you want me to do? We can't just leave now! We have to..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_FaceDirection(ACTOR_KARST, slow, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, slow);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, fast);
  FieldEvent_Wait(10);
  // "Don't worry, Felix! I'm here to help!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_WalkBy(ACTOR_JENNA, -8, -24);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, slow);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  Actor_FaceDirection(ACTOR_PIERS, fast, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, fast);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, fast);
  FieldEvent_Wait(10);
  Actor_SetDestinationOffset(ACTOR_FELIX, -16, -8);
  Actor_SetDestinationOffset(ACTOR_PIERS, -16, -8);
  Actor_SetDestinationOffset(ACTOR_JENNA, -16, -8);
  Actor_SetDestinationOffset(ACTOR_AGATIO, 16, 8);
  Actor_SetDestinationOffset(ACTOR_KARST, 16, 8);
  Actor_Get(ACTOR_FELIX)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_PIERS)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_JENNA)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_AGATIO)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_KARST)->velocityY = toFIXED(4);
  FieldEvent_Wait(4);
  if (gMainRoutineWork.sequenceResult == -1) {
    FieldUnit_AddHp(4, -10);
  }
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(
      (s32)&SceneId_JupiterLighthouseAerieBeacon, 11);
  Event_SetDefeatDestination((s32)&SceneId_JupiterLighthouseAerieBeacon, 11);
  Field_RequestBattleFromEncounterTable(0xd, 6);
}

void JupiterLighthouse_AfterTheFight(void) {
  s32 northwest = FACING_NORTHWEST;
  s32 southeast;
  s32 north;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(512), toFIXED(312),
                             northwest);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(528), toFIXED(304),
                             northwest);
  southeast = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(504), toFIXED(304),
                             southeast);
  Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(520), toFIXED(288),
                             southeast);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(528), toFIXED(320),
                             northwest);
  north = FACING_NORTH;
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(544), toFIXED(384), north);
  FieldCamera_MoveTo(520, 104, 312, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgCallSheba);
  // "Jenna!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_SetAnimation(ACTOR_AGATIO, 9);
  Actor_SetAnimation(ACTOR_KARST, 8);
  Actor_SetAnimation(ACTOR_FELIX, 50);
  Actor_SetAnimation(ACTOR_PIERS, 12);
  Actor_SetAnimation(ACTOR_JENNA, 27);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ((void *)Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                           30));
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_PIERS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_AGATIO)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_KARST)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_FELIX)->velocityY = toFIXED(3);
  Actor_Get(ACTOR_PIERS)->velocityY = toFIXED(3);
  Actor_Get(ACTOR_JENNA)->velocityY = toFIXED(3);
  Actor_Get(ACTOR_AGATIO)->velocityY = toFIXED(3);
  Actor_Get(ACTOR_KARST)->velocityY = toFIXED(3);
  Actor_SetDestinationOffset(ACTOR_FELIX, 8, 4);
  Actor_SetDestinationOffset(ACTOR_PIERS, 8, 4);
  Actor_SetDestinationOffset(ACTOR_JENNA, 8, 4);
  Actor_SetDestinationOffset(ACTOR_AGATIO, -16, -8);
  Actor_SetDestinationOffset(ACTOR_KARST, -16, -8);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_PIERS)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_JENNA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_AGATIO)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_KARST)->steeringFlags |= ACTOR_FACES_MOTION;
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  Actor_WalkToAndWait(ACTOR_SHEBA, 552, 344);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "I got the feeling something was wrong, so I rushed up here. Just in..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(5);
  // "Thank you, Sheba! I'm not looking forward to fighting, but I'm glad..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  ((void *)Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30));
  // "Another one? Agatio, we're rapidly being outnumbered!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  FieldEvent_Wait(5);
  // "It's too late to flee now! We have to keep fighting!"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, north);
  FieldEvent_Wait(10);
  // "Isaac and the others are safe! Now let's take care of these two rats!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_WalkBy(ACTOR_SHEBA, 0, -24);
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, southeast);
  Actor_FaceDirection(ACTOR_SHEBA, northwest, 0);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  Actor_FaceDirection(ACTOR_PIERS, northwest, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, northwest);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, northwest);
  FieldEvent_Wait(10);
  Actor_SetDestinationOffset(ACTOR_FELIX, -16, -8);
  Actor_SetDestinationOffset(ACTOR_PIERS, -16, -8);
  Actor_SetDestinationOffset(ACTOR_JENNA, -16, -8);
  Actor_SetDestinationOffset(ACTOR_SHEBA, -16, -8);
  Actor_SetDestinationOffset(ACTOR_AGATIO, 16, 8);
  Actor_SetDestinationOffset(ACTOR_KARST, 16, 8);
  Actor_Get(ACTOR_FELIX)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_PIERS)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_JENNA)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_SHEBA)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_AGATIO)->velocityY = toFIXED(4);
  Actor_Get(ACTOR_KARST)->velocityY = toFIXED(4);
  FieldEvent_Wait(4);
  if (gMainRoutineWork.sequenceResult == -1)
    FieldUnit_AddHp(4, -10);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(
      (s32)&SceneId_JupiterLighthouseAerieBeacon, 12);
  Event_SetDefeatDestination((s32)&SceneId_JupiterLighthouseAerieBeacon, 13);
  Event_ClearFlag(FLAG_KEEP_MUSIC_HELD);
  Field_RequestBattleFromEncounterTable(13, 7);
}

void JupiterLighthouse_ArcTo(s32 a0, s32 a1) {
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(((s32)Actor_Get(a0)), 2);
  Event_PlaySound(SE_ASCEND);
  FieldEvent_Wait(60);
  BattleEffect_SetObjectUpdateMode(((s32)Actor_Get(a0)), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Actor_SetAnimation(a1, 1);
}

extern u32 gJupiterLighthouse_IvanLeavesScript[];
extern u32 gJupiterLighthouse_WalkToPlatformsScript[];
extern s32 gJupiterLighthouse_HopWestOverPlatformsScript[];
extern s32 gJupiterLighthouse_HopEastOverPlatformsScript[];

void JupiterLighthouse_KarstAndAgatioRetreat(void) {
  s32 fast;
  s32 slow;
  s32 mid;
  struct Actor *actor;
  s32 two, v_r5, t310, t311, t438;

  void JupiterLighthouse_HealParty(s32 clearStatus) {
    s32 count;
    s32 n;
    struct Unit *member;
    s16 maxHp;

    count = Party_CountMembers();
    for (n = 0; n < count; n++) {
      struct MainRoutineWork *party = &gMainRoutineWork;

      member = Unit_Get(party->party[n]);
      maxHp = member->hp = member->hpMax;
      member->hpGauge = Vitals_Clamp((maxHp << 14) / maxHp);
      if (member->hpGauge == 0 && member->hp != 0)
        member->hpGauge = 1;
      member->ppGauge = Vitals_Clamp((member->pp << 14) / member->ppMax);
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
  Actor_Get(ACTOR_ISAAC)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_JENNA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_PIERS)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_SHEBA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_KRADEN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_AGATIO)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_KARST)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  mid = FACING_NORTH;
  fast = FACING_NORTHWEST;
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(504), toFIXED(304), fast);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(528), toFIXED(288), fast);
  slow = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(488), toFIXED(296), slow);
  Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(504), toFIXED(280), slow);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(528), toFIXED(304), fast);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(552), toFIXED(288), mid);
  Actor_SetPositionAndFacing(ACTOR_ALEX, toFIXED(480), toFIXED(360), mid);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(592), toFIXED(392),
                             FACING_EAST);
  Actor_SetAnimation(ACTOR_JENNA, 19);
  Actor_SetAnimation(ACTOR_SHEBA, 19);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_PIERS, 13);
  actor = Actor_Get(ACTOR_FELIX);
  actor->scaleX = -actor->scaleX;
  actor = Actor_Get(ACTOR_SHEBA);
  actor->scaleX = -actor->scaleX;
  FieldCamera_MoveTo(520, 104, 312, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWasnTThe);
  Actor_SetAnimation(ACTOR_AGATIO, 9);
  Actor_SetAnimation(ACTOR_KARST, 8);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  FieldEvent_Wait(5);
  // "Isaac wasn't the only surprise. They were all far more powerful than..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Don't worry, Karst! We'll win in the end..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(ACTOR_KARST, slow);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, slow);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  // "This one moved... I can't believe they're still alive! The pests!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
  // "If we stay here much longer, Isaac and the others will come! We..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "I know... But treason deserves death! We must finish them!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  // "There's no time for petty grudges!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_SOUTH);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(520, 104, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Event_PlaySound(MUS_ALEX);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Alex! You..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
  // "You no longer consider me a friend? I do not care."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "It goes beyond that, Alex! You've betrayed us, and treason deserves..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Yes, I heard you the first time. But shouldn't you heal yourselves..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_SetAnimation(ACTOR_KARST, ANIM_STAND);
  Actor_FaceDirection(ACTOR_KARST, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
  // "After fighting both Isaac and Felix, you're in no condition to fight me."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  // "And if Isaac and the others come, what chance will you have?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  // "What's keeping Felix and the others?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KARST, 2);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  Actor_FaceDirection(ACTOR_KARST, slow, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, slow);
  FieldEvent_Wait(5);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, mid);
  // "There's no time to talk!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
  // "Then what should we do?"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
  // "We take the elevator! With the beacon lit, it should be fully..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_SOUTHWEST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  // "We have no choice. Let's go."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Good! You're listening to reason! Now follow me!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_SetAnimation(ACTOR_AGATIO, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  EventCamera_FollowActor(ACTOR_AGATIO, 1);
  Actor_SetScript(ACTOR_AGATIO, (s32)gJupiterLighthouse_WalkToPlatformsScript);
  FieldEvent_Wait(30);
  Actor_SetScript(ACTOR_ALEX, (s32)gJupiterLighthouse_WalkToPlatformsScript);
  Actor_SetScript(ACTOR_KARST, (s32)gJupiterLighthouse_WalkToPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_ALEX);
  Actor_Get(ACTOR_ALEX)->priorityFlags |= 64;
  Actor_Get(ACTOR_AGATIO)->priorityFlags |= 64;
  Actor_Get(ACTOR_KARST)->priorityFlags |= 64;
  Actor_Get(ACTOR_ALEX)->motionFlags = 2;
  Actor_SetScript(ACTOR_ALEX,
                  (s32)gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  Actor_SetScript(ACTOR_AGATIO,
                  (s32)gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  Actor_SetScript(ACTOR_KARST,
                  (s32)gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_ALEX);
  Actor_WalkToAndWait(ACTOR_ALEX, 311, 256);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  Actor_WalkToAndWait(ACTOR_AGATIO, 332, 266);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_SOUTH, 0);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  Actor_WalkToAndWait(ACTOR_KARST, 290, 266);
  Actor_FaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
  Actor_Get(ACTOR_ALEX)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_AGATIO)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_KARST)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  // "Felix! Jenna!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(512), toFIXED(328), mid);
  JupiterLighthouse_LiftObjectsInBeam(0);
  Scene_PlayMusic();
  FieldEvent_Wait(10);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(536), toFIXED(368), mid);
  Actor_SetPositionAndFacing(ACTOR_GARET, toFIXED(536), toFIXED(384), mid);
  Actor_SetPositionAndFacing(ACTOR_IVAN, toFIXED(552), toFIXED(368), mid);
  Actor_SetPositionAndFacing(ACTOR_MIA, toFIXED(552), toFIXED(352), mid);
  // "What's happened?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  EventCamera_FollowActor(ACTOR_ISAAC, 1);
  Actor_WalkTo(ACTOR_ISAAC, 536, 336);
  Actor_WalkTo(ACTOR_GARET, 536, 352);
  Actor_WalkTo(ACTOR_IVAN, 552, 336);
  Actor_WalkTo(ACTOR_MIA, 552, 320);
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  FieldActor_WaitForMoveById(ACTOR_GARET);
  FieldActor_WaitForMoveById(ACTOR_IVAN);
  FieldActor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  // "It seems they fought with Agatio and Karst..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(2, 4, 33);
  // "I had a really bad feeling after Sheba left. I hate it when I'm right..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_SOUTH);
  FieldEvent_Wait(10);
  // "Your power of prediction must be even stronger than you'd thought..."
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, fast);
  FieldCamera_MoveTo(320, 96, 360, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  // "Blast! They must have taken the elevator to escape!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  FieldCamera_MoveTo(512, 96, 352, 1);
  FieldCamera_WaitForArrival();
  JupiterLighthouse_ScrollBeam(0);
  Actor_AnimateThenWait(1, 4, 33);
  // "How could they have done that to Jenna? Those monsters!"
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, fast);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_Wait(5);
  // "Felix tried to help us... They must have attacked him because he..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "So, does that mean Felix isn't really our enemy?"
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, mid);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(5);
  // "Felix! You're alive!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(5);
  FieldActor_StartRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(5);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  // "Mia, he needs healing, quickly! There's still time!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_AnimateThenWait(3, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_GARET, fast);
  // "Don't let 'em die! Felix still has a lot of questions to answer..."
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, slow);
  FieldEvent_Wait(10);
  // "If you save Felix, we'll tell you everything..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, mid);
  Actor_WalkByAndWait(ACTOR_MIA, -48, 0);
  Actor_FaceDirection(ACTOR_MIA, mid, 0);
  JupiterLighthouse_ArcTo(3, 4);
  Actor_FaceDirection(ACTOR_FELIX, slow, 0);
  actor = Actor_Get(ACTOR_FELIX);
  actor->scaleX = -actor->scaleX;
  Actor_WalkByAndWait(ACTOR_MIA, 40, 0);
  Actor_WalkByAndWait(ACTOR_MIA, 8, -16);
  Actor_FaceDirection(ACTOR_MIA, mid, 0);
  JupiterLighthouse_ArcTo(3, 6);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  actor = Actor_Get(ACTOR_SHEBA);
  actor->scaleX = -actor->scaleX;
  Actor_FaceDirection(ACTOR_MIA, fast, 0);
  JupiterLighthouse_ArcTo(3, 7);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  JupiterLighthouse_ArcTo(3, 5);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, fast);
  // "All right, Felix! We want to know what's going on!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, slow, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "It's all quite complicated, Isaac. There are a number of extenuating..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "What are you talking about, Kraden?"
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_WalkTo(ACTOR_IVAN, 552, 320);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
  Actor_FaceDirection(ACTOR_GARET, mid, 0);
  Actor_AnimateThenWait(2, 4, 33);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  // "Look, I don't think any of us are in any shape for another fight..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(5);
  // "What do you mean, Ivan?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_SOUTHWEST);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_Wait(5);
  // "I'm just saying we should get ourselves back to Contigo and sort..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_SHEBA, 0);
  Actor_FaceEachOther(ACTOR_PIERS, ACTOR_FELIX, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  // "That's a sound plan, Ivan... I fully agree."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldEvent_Wait(50);
  Actor_AnimateThenWait(1, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, mid);
  FieldEvent_Wait(50);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(5, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, fast);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, slow);
  if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    Actor_AnimateThenWait(0, 3, 20);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  } else {
    Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    Actor_AnimateThenWait(5, 4, 33);
    gFieldWork->message += 2;
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    FieldEvent_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    Actor_AnimateThenWait(0, 3, 20);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_ISAAC);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_ISAAC);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_ISAAC);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetScript(ACTOR_IVAN, (s32)gJupiterLighthouse_IvanLeavesScript);
  Actor_SetScript(ACTOR_MIA, (s32)gJupiterLighthouse_IvanLeavesScript);
  Actor_SetScript(ACTOR_GARET, (s32)gJupiterLighthouse_IvanLeavesScript);
  FieldActor_WaitForScriptEndById(ACTOR_IVAN);
  FieldActor_WaitForScriptEndById(ACTOR_MIA);
  FieldActor_WaitForScriptEndById(ACTOR_GARET);
  EventCamera_FollowActor(ACTOR_ISAAC, 1);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, 0);
  FieldActor_WatchAndSetScript(ACTOR_PIERS, 0);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, 0);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, 0);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_ISAAC, 1.5);
  Actor_WalkToAndWait(ACTOR_ISAAC, 543, 363);
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_WalkToAndWait(ACTOR_ISAAC, 573, 358);
  Actor_WalkToAndWait(ACTOR_ISAAC, 584, 300);
  Actor_WalkToAndWait(ACTOR_ISAAC, 584, 252);
  Actor_WalkToAndWait(ACTOR_ISAAC, 616, 247);
  Actor_WalkToAndWait(ACTOR_ISAAC, 617, 312);
  Actor_Get(ACTOR_ISAAC)->motionFlags = 2;
  Actor_SetScript(ACTOR_ISAAC,
                  (s32)gJupiterLighthouse_HopEastOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_ISAAC);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldEvent_Wait(30);
  Actor_Get(ACTOR_ISAAC)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  JupiterLighthouse_LiftObjectsInBeam(1);
  FieldEvent_Wait(100);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  JupiterLighthouse_ScrollBeam(1);
  FieldEvent_Wait(60);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  FieldEvent_Wait(60);
  Actor_AnimateThenWait(5, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_AnimateThenWait(6, 3, 20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
  two = 2;
  v_r5 = (s32)gJupiterLighthouse_IvanLeavesScript;
  if (actor != 0) {
    Actor_SetDestination(ACTOR_PIERS, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_SHEBA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_JENNA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetScript(ACTOR_PIERS, v_r5);
  Actor_SetScript(ACTOR_SHEBA, v_r5);
  Actor_SetScript(ACTOR_JENNA, v_r5);
  Actor_SetScript(ACTOR_KRADEN, v_r5);
  FieldActor_WaitForScriptEndById(ACTOR_PIERS);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  FieldActor_WaitForScriptEndById(ACTOR_JENNA);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, 0);
  Actor_SetScript(ACTOR_FELIX, (s32)gJupiterLighthouse_WalkToPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= 64;
  Actor_Get(ACTOR_ALEX)->motionFlags = two;
  Actor_SetScript(ACTOR_FELIX,
                  (s32)gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_WalkToAndWait(ACTOR_FELIX, 311, 256);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  JupiterLighthouse_LiftObjectsInBeam(2);
  t438 = (s32)Ram_HeapSlots->fieldWork;
  FIELD_AT(t438, s32, 428) = 0x200;
  FIELD_AT(t438, s32, 436) = 24;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  JupiterLighthouse_HealParty(0);
  FieldEvent_SetExitRequest(1);
  Event_End();
  return;
}

static __inline__ void W_Actor_EnableActionAndSetCallback(s32 a0, s32 a1) {
  Actor_SetMovementTypeById(a0, a1);
}

#define SET_MODE_AND_WAIT(id, mode, frames)                                    \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                              \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

void JupiterLighthouse_WeBeatThem(void) {
  s32 fast;
  FIXED slow;
  s32 mid;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t5;
  s32 t6;
  s32 t7;
  s32 t8;
  s32 t11;
  s32 t292;
  s32 t295;
  s32 t298;
  s32 t358;
  s32 t361;
  s32 t364;
  s32 t367;
  s32 work;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(ACTOR_ISAAC)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_JENNA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_PIERS)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_SHEBA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_KRADEN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_AGATIO)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_KARST)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_ALEX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  fast = FACING_NORTHWEST;
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(520), toFIXED(304), fast);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(536), toFIXED(288), fast);
  slow = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(480), toFIXED(296), slow);
  Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(496), toFIXED(280), slow);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(544), toFIXED(304), fast);
  mid = FACING_NORTH;
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(560), toFIXED(288), mid);
  Actor_SetPositionAndFacing(ACTOR_ALEX, toFIXED(480), toFIXED(360), mid);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(592), toFIXED(392),
                             FACING_EAST);
  Actor_SetAnimation(ACTOR_AGATIO, 0xa);
  Actor_SetAnimation(ACTOR_KARST, 9);
  FieldCamera_SetSpeedFX(0x10000, slow);
  FieldCamera_MoveTo(520, 104, 312, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWeDidItWe);
  SET_MODE_AND_WAIT(7, 4, 33);
  // "We did it... We beat them..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  SET_MODE_AND_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  // "Barely..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  FieldEvent_Wait(10);
  // "What should we do with them?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  FieldEvent_Wait(5);
  // "Finish us!"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  FieldEvent_Wait(5);
  // "But think: what happens to your parents if neither we nor Saturos..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, fast, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, fast);
  FieldEvent_Wait(20);
  SET_MODE_AND_WAIT(5, 4, 33);
  // "Are you saying your people will kill our parents if you don't return..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  FieldEvent_Wait(5);
  // "Well then, I suppose you'd better finish us off, like I said."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  // "What's the matter? Are you going to do it or not?"
  Event_OpenMessage(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    gFieldWork->message = (gFieldWork->message + 1);
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  }
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveTo(520, 104, 352, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Event_PlaySound(MUS_ALEX);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  FieldCamera_MoveTo(520, 104, 312, 1);
  Actor_WalkToAndWait(ACTOR_ALEX, 480, 320);
  SET_MODE_AND_WAIT(ACTOR_ALEX, 4, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, mid);
  Actor_WalkByAndWait(ACTOR_ALEX, 20, -20);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_WEST);
  JupiterLighthouse_ArcTo(0x1d, 0x1b);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, mid);
  JupiterLighthouse_ArcTo(0x1d, 0x1c);
  Actor_WalkByAndWait(ACTOR_ALEX, -20, 20);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_AGATIO, 2);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  FieldEvent_Wait(5);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  SET_MODE_AND_WAIT(ACTOR_ALEX, 4, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  SET_MODE_AND_WAIT(7, 4, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_SOUTH);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_SOUTH);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_SOUTHWEST);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  Actor_FaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  SET_MODE_AND_WAIT(ACTOR_ALEX, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  FieldEvent_Wait(10);
  SET_MODE_AND_WAIT(ACTOR_ALEX, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, mid);
  SET_MODE_AND_WAIT(ACTOR_ALEX, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KARST, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_KARST, ANIM_NOD);
  SET_MODE_AND_WAIT(ACTOR_AGATIO, 3, 20);
  FieldEvent_Wait(10);
  EventCamera_FollowActor(ACTOR_AGATIO, 1);
  Actor_SetScript(ACTOR_ALEX, gJupiterLighthouse_WalkToPlatformsScript);
  FieldEvent_Wait(30);
  Actor_SetScript(ACTOR_AGATIO, gJupiterLighthouse_WalkToPlatformsScript);
  FieldEvent_Wait(30);
  Actor_SetScript(ACTOR_KARST, gJupiterLighthouse_WalkToPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_ALEX);
  Actor_Get(ACTOR_ALEX)->priorityFlags |= 0x40;
  Actor_Get(ACTOR_AGATIO)->priorityFlags |= 0x40;
  Actor_Get(ACTOR_KARST)->priorityFlags |= 0x40;
  Actor_Get(ACTOR_ALEX)->motionFlags = 2;
  Actor_SetScript(ACTOR_ALEX, gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  Actor_SetScript(ACTOR_AGATIO, gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  Actor_SetScript(ACTOR_KARST, gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_ALEX);
  Actor_WalkToAndWait(ACTOR_ALEX, 311, 256);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
  FieldActor_WaitForScriptEndById(ACTOR_AGATIO);
  Actor_WalkToAndWait(ACTOR_AGATIO, 332, 266);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_SOUTH, 0);
  FieldActor_WaitForScriptEndById(ACTOR_KARST);
  Actor_WalkToAndWait(ACTOR_KARST, 290, 266);
  Actor_FaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
  Actor_Get(ACTOR_ALEX)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_AGATIO)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_KARST)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  {
    s32 speed = 0xc000;
    Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(512), toFIXED(328), speed);
  }
  Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(536), toFIXED(336),
                             FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_GARET, toFIXED(536), toFIXED(352),
                             FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_IVAN, toFIXED(552), toFIXED(352),
                             FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_MIA, toFIXED(552), toFIXED(368),
                             FACING_NORTH);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  JupiterLighthouse_LiftObjectsInBeam(3);
  Scene_PlayMusic();
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTH);
  FieldEvent_Wait(20);
  JupiterLighthouse_ScrollBeam(0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTHEAST);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_WalkTo(ACTOR_IVAN, 552, 320);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTH);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_SOUTHWEST);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
  SET_MODE_AND_WAIT(2, 4, 33);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_SOUTHWEST);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  SET_MODE_AND_WAIT(ACTOR_KRADEN, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldEvent_Wait(60);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTH);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  SET_MODE_AND_WAIT(1, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  FieldEvent_Wait(60);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  SET_MODE_AND_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    SET_MODE_AND_WAIT(0, 3, 20);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
    SET_MODE_AND_WAIT(0, 3, 20);
    FieldEvent_Wait(10);
  } else {
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    SET_MODE_AND_WAIT(5, 4, 33);
    gFieldWork->message = (gFieldWork->message + 2);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    FieldEvent_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    SET_MODE_AND_WAIT(0, 3, 20);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  t292 = (s32)Actor_Get(ACTOR_ISAAC);
  if (t292 != 0) {
    Actor_SetDestination(ACTOR_IVAN, ((struct Actor *)t292)->x.part[0].integer,
                         ((struct Actor *)t292)->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  t295 = (s32)Actor_Get(ACTOR_ISAAC);
  if (t295 != 0) {
    Actor_SetDestination(ACTOR_MIA, ((struct Actor *)t295)->x.part[0].integer,
                         ((struct Actor *)t295)->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  t298 = (s32)Actor_Get(ACTOR_ISAAC);
  if (t298 != 0) {
    Actor_SetDestination(ACTOR_GARET, ((struct Actor *)t298)->x.part[0].integer,
                         ((struct Actor *)t298)->z.part[0].integer);
  }
  Actor_SetScript(ACTOR_IVAN, gJupiterLighthouse_IvanLeavesScript);
  W_Actor_EnableActionAndSetCallback(3,
                                     (s32)gJupiterLighthouse_IvanLeavesScript);
  Actor_SetScript(ACTOR_GARET, gJupiterLighthouse_IvanLeavesScript);
  FieldActor_WaitForScriptEndById(ACTOR_IVAN);
  FieldActor_WaitForScriptEndById(ACTOR_MIA);
  FieldActor_WaitForScriptEndById(ACTOR_GARET);
  EventCamera_FollowActor(ACTOR_ISAAC, 1);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, 0);
  FieldActor_WatchAndSetScript(ACTOR_PIERS, 0);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, 0);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, 0);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_ISAAC, 1.5);
  Actor_WalkToAndWait(ACTOR_ISAAC, 543, 363);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_WalkToAndWait(ACTOR_ISAAC, 573, 358);
  Actor_WalkToAndWait(ACTOR_ISAAC, 584, 300);
  Actor_WalkToAndWait(ACTOR_ISAAC, 584, 252);
  Actor_WalkToAndWait(ACTOR_ISAAC, 616, 247);
  Actor_WalkToAndWait(ACTOR_ISAAC, 617, 312);
  Actor_Get(ACTOR_ISAAC)->motionFlags = 2;
  Actor_SetScript(ACTOR_ISAAC, gJupiterLighthouse_HopEastOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_ISAAC);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldEvent_Wait(30);
  Actor_Get(ACTOR_ISAAC)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  JupiterLighthouse_LiftObjectsInBeam(1);
  FieldEvent_Wait(100);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  JupiterLighthouse_ScrollBeam(1);
  FieldEvent_Wait(60);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  FieldEvent_Wait(60);
  SET_MODE_AND_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  SET_MODE_AND_WAIT(6, 3, 20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  t358 = (s32)Actor_Get(ACTOR_FELIX);
  if (t358 != 0) {
    Actor_SetDestination(ACTOR_PIERS, ((struct Actor *)t358)->x.part[0].integer,
                         ((struct Actor *)t358)->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  t361 = (s32)Actor_Get(ACTOR_FELIX);
  if (t361 != 0) {
    Actor_SetDestination(ACTOR_SHEBA, ((struct Actor *)t361)->x.part[0].integer,
                         ((struct Actor *)t361)->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  t364 = (s32)Actor_Get(ACTOR_FELIX);
  if (t364 != 0) {
    Actor_SetDestination(ACTOR_JENNA, ((struct Actor *)t364)->x.part[0].integer,
                         ((struct Actor *)t364)->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  t367 = (s32)Actor_Get(ACTOR_FELIX);
  if (t367 != 0) {
    Actor_SetDestination(ACTOR_KRADEN,
                         ((struct Actor *)t367)->x.part[0].integer,
                         ((struct Actor *)t367)->z.part[0].integer);
  }
  Actor_SetScript(ACTOR_PIERS, gJupiterLighthouse_IvanLeavesScript);
  Actor_SetScript(ACTOR_SHEBA, gJupiterLighthouse_IvanLeavesScript);
  Actor_SetScript(ACTOR_JENNA, gJupiterLighthouse_IvanLeavesScript);
  Actor_SetScript(ACTOR_KRADEN, gJupiterLighthouse_IvanLeavesScript);
  FieldActor_WaitForScriptEndById(ACTOR_PIERS);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  FieldActor_WaitForScriptEndById(ACTOR_JENNA);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, 0);
  Actor_SetScript(ACTOR_FELIX, gJupiterLighthouse_WalkToPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= 0x40;
  Actor_Get(ACTOR_ALEX)->motionFlags = 2;
  Actor_SetScript(ACTOR_FELIX, gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_WalkToAndWait(ACTOR_FELIX, 311, 256);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  JupiterLighthouse_LiftObjectsInBeam(2);
  work = (s32)Ram_HeapSlots->fieldWork;
  FIELD_AT((work + 0x1ac), s32, 0) = 0x200;
  FIELD_AT((work + 0x1b4), s32, 0) = 0x18;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Writes to out the ids of objects 0 to 63 standing in columns 4 to 8 and in
/// the three rows from 72 less the row of z. Nothing in this overlay calls it.
/// As ☀️'s MercuryLighthouseAerie_CollectBandSlots does.
void JupiterLighthouse_FindObjectsInBeam(s32 *out, s32 zArg) {
  s32 z = zArg;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 slot;

  z = 64 - FIELD_CELL(z);
  for (slot = 0; slot <= 63; slot++) {
    struct Actor *object = work->actors[slot];

    if (object != NULL) {
      s32 column = FIELD_CELL(object->x.fixed);
      s32 row = FIELD_CELL(object->z.fixed);

      if (column >= 4 && column <= 8 && z + 8 <= row && row < z + 11)
        *out++ = slot;
    }
  }
}

extern s32 gJupiterLighthouse_FirstBeamObjects[];
extern s32 gJupiterLighthouse_SecondBeamObjects[];
extern s32 gJupiterLighthouse_ThirdBeamObjects[];
extern s32 gJupiterLighthouse_FourthBeamObjects[];
extern s32 gJupiterLighthouse_BeamObjects[];

void JupiterLighthouse_LiftObjectsInBeam(s32 mode) {
  FIXED speed;
  u32 count;
  s32 *list;
  s32 *p;
  s32 *q;
  u32 i;
  u32 j;
  struct MapLayer *beam;

  beam = &Ram_HeapSlots->mapGrid->layers[2];
  speed = 0x1999;
  count = 0;
  gJupiterLighthouse_BeamLevel = beam->offsetY;
  switch (mode) {
  case 0:
  case 3:
    list = gJupiterLighthouse_FirstBeamObjects;
    break;
  case 1:
    list = gJupiterLighthouse_SecondBeamObjects;
    break;
  case 2:
    list = gJupiterLighthouse_ThirdBeamObjects;
    break;
  default:
    list = gJupiterLighthouse_FourthBeamObjects;
    break;
  }

  for (i = 0; i <= 4 && list[i] != 64; i++) {
    Actor_Get(list[i])->motionFlags = 0;
    FieldActor_SetSpritePriority(list[i], 1);
    count++;
  }

  for (i = 0; i <= 5; i++) {
    Actor_Get(gJupiterLighthouse_BeamObjects[i])->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    FieldActor_SetSpritePriority(gJupiterLighthouse_BeamObjects[i], 0);
  }

  Event_PlaySound(SE_LIFT);

  for (i = 0; i <= 227; i++) {
    beam->offsetY -= speed;
    for (j = 0; j < count; j++) {
      struct Actor *actor;

      Actor_Get(list[j])->z.fixed += speed;
      actor = Actor_Get(list[j]);
      actor->targetZ = Actor_Get(list[j])->z.fixed;
    }
    if ((i & 3) == 3)
      speed += 0x1999;
    if (speed > 0x17fff)
      speed = toFIXED(3) / 2;
    System_Wait(1);
    if (mode == 0) {
      if (i == 40)
        FieldCamera_MoveTo(512, 104, 304, 1);
      if (i == 120)
        break;
    } else if (mode == 1) {
      if (i == 40)
        FieldCamera_MoveTo(512, 104, 352, 1);
      if (i == 120)
        break;
    } else if (mode == 3) {
      if (i == 40)
        EventCamera_FollowActor(ACTOR_ISAAC, 1);
      if (i == 120)
        break;
    }
  }

  for (i = 0; i <= 5; i++) {
    Actor_Get(gJupiterLighthouse_BeamObjects[i])->priorityFlags &=
        ~ACTOR_PRIORITY_IGNORES_RISES;
    FieldActor_SetSpritePriority(gJupiterLighthouse_BeamObjects[i], 1);
  }

  for (i = 0; i < count; i++) {
    Actor_SetPosition(list[i], 0, 0);
    Actor_Get(list[i])->motionFlags = 0;
  }
}

extern s32 gJupiterLighthouse_BeamRiders[];

void JupiterLighthouse_ScrollBeam(s32 mode) {
  struct MapLayer *beam = &Ram_HeapSlots->mapGrid->layers[2];
  s32 speed = toFIXED(3) / 2;
  u32 count = 0;
  u32 i;
  s32 *ids = gJupiterLighthouse_BeamRiders;

  for (i = 0; i <= 4 && ids[i] != 64; i++) {
    Actor_Get(ids[i])->motionFlags = 0;
    count++;
  }
  if (mode != 0) {
    Event_PlaySound(SE_LIFT);
    i = 0;
    beam->offsetY += toFIXED(3) / 2;
    while (beam->offsetY < gJupiterLighthouse_BeamLevel) {
      u32 j;

      for (j = 0; j < count; j++) {
        struct Actor *actor;

        Actor_Get(ids[j])->z.fixed -= speed;
        actor = Actor_Get(ids[j]);
        actor->targetZ = Actor_Get(ids[j])->z.fixed;
      }
      if ((i & 3) == 3 &&
          gJupiterLighthouse_BeamLevel < speed * 15 * 2 + beam->offsetY)
        speed = speed * 15 / 16;
      if (speed <= 0xccb)
        speed = 0xccc;
      if (mode == 1 && i == 40)
        FieldCamera_MoveTo(288, 104, 368, 1);
      System_Wait(1);
      i++;
      if (i > 119)
        break;
      beam->offsetY += speed;
    }
  }
  beam->offsetY = gJupiterLighthouse_BeamLevel;
  MapGrid_CenterOnFollowedPoint();
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

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Call2(HangingVine_Place, 0xf, 1);
  HangingVine_Place(16, 1);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 0x40;
  Actor_Get(ACTOR_FLOATING_PLATFORM_A)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FLOATING_PLATFORM_B)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FLOATING_PLATFORM_C)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FLOATING_PLATFORM_D)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FLOATING_PLATFORM_E)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FLOATING_PLATFORM_F)->priorityFlags |= 0x80;
  if (!Event_GetFlag(FLAG_JUPITER_BEACON_LIT)) {
    JupiterLighthouse_EnergizeAngels();
  } else {
    t11 = Actor_Get(ACTOR_LID_B);
    FIELD_AT(t11, s32, 24) = 0;
  }
  if ((u32)((FIELD_AT(&gMainRoutineWork, u16, 482) - 1) << 16) > 0x10000) {
    Actor_SetPosition(ACTOR_LID_B, 0, 0);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_ORB), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_ORB), 0);
  Task_AddFrame((s32)JupiterLighthouse_PulseBeacon, TASK_PASS_VBLANK);
  if (gMainRoutineWork.entrance == 9) {
    JupiterLighthouse_BeaconIsLit();

    gMainRoutineWork.retreatScene = (s32)MsgRobe;
    gMainRoutineWork.retreatEntrance = 1;
  } else {
    if (gMainRoutineWork.entrance == 0xa) {
      Party_AddMember(5);
      JupiterLighthouse_FightKarstAndAgatio();
    } else {
      if (gMainRoutineWork.entrance == 0xb) {
        Party_AddMember(6);
        JupiterLighthouse_AfterTheFight();
      } else {
        if (gMainRoutineWork.entrance == 0xc) {
          JupiterLighthouse_WeBeatThem();
        } else {
          if (gMainRoutineWork.entrance == 0xd) {
            JupiterLighthouse_KarstAndAgatioRetreat();
          } else {
            if (Event_GetFlag(FLAG_JUPITER_BEACON_LIT)) {
              Actor_SetPosition(ACTOR_AGATIO, 0, 0);
              Actor_SetPosition(ACTOR_KARST, 0, 0);
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

extern s32 gPaletteCyclePausesInEvents;

void JupiterLighthouse_SetCycleFrame(s32 a0) {
  gPaletteCyclePausesInEvents = a0;
}

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

void JupiterLighthouse_LoadPalettes(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gLighthousePalette13, (void *)&BG_PLTT_COLOR(13, 0),
                         32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    PaletteCycle_SetFrame(0);
    Task_Add(PaletteCycle_Step, TASK_PASS_VBLANK);
  } else {
    Dma_Set(gLighthouseUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}

void Actor_RaiseFromCell(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->groundHeight;
  object->bounce = 0;
  object->y.fixed = height + toFIXED(32);
  object->priorityFlags |= 128;
  height =
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
  MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0,
                        (height >> 19) + 6);
  MapGrid_SetAttributeBAt(object->x.fixed, object->z.fixed, 0, 128);
}

void JupiterLighthouse_RideTheBeam(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *linked = actor->watchedActor;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->glowHeld = i;

  actor->motionFlags = 4;
  Actor_SetSpriteFlags(actor, 0);

  while (linked->riseEnabled != 0) {
    actor->x.fixed = linked->x.fixed;
    actor->y.fixed = linked->y.fixed + i;
    actor->z.fixed = linked->z.fixed;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->glow = 1;
  work->glowHeld = 1;
  angle = linked->facing;
  actor->x.fixed = (((linked->x.fixed >> 19) & ~1) + (angle >> 13) - 1) << 19;
  actor->z.fixed = (((linked->z.fixed >> 19) & ~1) - ((angle >> 13) & 2) + 1)
                   << 19;
  actor->velocityY = toFIXED(6);
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSpriteFlags(actor, 1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  shadow = EventCamera_GetActor();
  shadow->speedLimit = toFIXED(8);
  shadow->acceleration = toFIXED(1);
  shadow->motionFlags = 0;
  FieldActor_SetMoveTargetFX(
      shadow, actor->x.fixed,
      Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed), actor->z.fixed);
  i = 0;
  if (actor->y.fixed > actor->groundHeight + toFIXED(16)) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->y.fixed > actor->groundHeight + toFIXED(16));
  }
  Event_PlaySound(SE_LANDING_THUD);
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
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  work->glowHeld = 1;
  work->stepTrigger = 0;
  Event_End();
}

#include "shared/position_flat_distance.h"

extern u32 gJupiterLighthouse_WhirlwindFadesScript[];

void Whirlwind_Update(struct WhirlwindActor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Vector3 position;
  FIXED distance;
  s32 near;
  s32 timer;
  u8 *flags;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(object, 0);
    return;
  }
  Actor_SetAnimationSpeed(object, ANIMATION_STEP_NORMAL);
  timer = object->timer + 1;
  object->timer = timer;
  if ((timer & 31) == 31)
    Event_PlaySound(SE_WATER_LEVEL);
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
    if (position.x <= toFIXED(272)) {
      if (position.x >= -toFIXED(32)) {
        if (object->riding == 0) {
          distance = leader->y.fixed - object->y;
          if (distance < 0)
            distance = object->y - leader->y.fixed;
          near = 0;
          if (Position_FlatDistance(&leader->x.fixed, &object->x) <= 12 &&
              distance < toFIXED(12))
            near = 1;
          if (near != 0 && Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
            leader->watchedActor = (struct Actor *)object;
            leader->facing = object->facing;
            flags = &leader->priorityFlags;
            *flags &= ~1;
            work->raisedTrigger = 200;
            gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
            object->riding = 1;
            object->carrying = 1;
          }
        }
        object->x += IwramMulFX(Trig_Cos(object->facing), object->speedLimit);
        object->z += IwramMulFX(Math_Sin(object->facing), object->speedLimit);
        return;
      }
    }
  }
  object->carrying = 0;
  object->update = 0;
  Actor_StartScript(object, gJupiterLighthouse_WhirlwindFadesScript);
  Event_PlaySound(SE_SPARKLING_PASSAGE);
  gJupiterLighthouse_WhirlwindRiding = 0;
}

void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn_upper.h"

#include "shared/actor_emit_rising_sparks_b.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

void JupiterLighthouse_EnergizeAngels(void) {
  struct Actor *left = Actor_GetById(ACTOR_LEFT_ANGEL);
  struct Actor *right = Actor_GetById(ACTOR_RIGHT_ANGEL);
  struct Actor *rider = Actor_GetById(ACTOR_LID_A);
  struct FieldSprite *sprite = rider->sprite;

  Actor_SetPosition(ACTOR_LID_B, 512, 248);
  Actor_SetPosition(ACTOR_LID_A, 512, 248);
  Actor_SetPosition(ACTOR_LEFT_ANGEL, 472, 256);
  Actor_SetPosition(ACTOR_RIGHT_ANGEL, 552, 256);
  Actor_GetById(ACTOR_LID_B)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y.fixed = rider->groundHeight;
  left->motionFlags = 0;
  left->y.fixed = left->groundHeight;
  right->motionFlags = 0;
  right->y.fixed = right->groundHeight;
  if (GameFlag_Test_(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    left->y.fixed += 0x66640;
    rider->y.fixed += 0x1eb80;
    rider->x.fixed += -0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(ACTOR_LEFT_ANGEL), 4);
    Actor_SetSpriteColorMode(Actor_GetById(ACTOR_LEFT_ARCHER), 4);
  }
  if (GameFlag_Test_(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    right->y.fixed += 0x66640;
    rider->y.fixed += 0x1eb80;
    rider->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(ACTOR_RIGHT_ANGEL), 4);
    Actor_SetSpriteColorMode(Actor_GetById(ACTOR_RIGHT_ARCHER), 4);
  }
  if (GameFlag_Test_(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      GameFlag_Test_(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    left->y.fixed += 0x199900;
    right->y.fixed += 0x199900;
    rider->y.fixed += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
  }
}

extern s16 gLightSpritesLarge;

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef BYTE_AT
#undef FIELD_AT
#undef ACTOR_SET_MODE_AND_WAIT
#undef SET_MODE_AND_WAIT
extern const s32 *gFieldEffect_DurationScripts[];
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

void CrackedTiles_Update(void);

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);
#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"

// clang-format off
s32 gJupiterLighthouse_FirstBeamObjects[] = {
    29, 27, 28, 64,
};
// clang-format on

s32 gJupiterLighthouse_SecondBeamObjects[] = {
    0,
    64,
};

s32 gJupiterLighthouse_ThirdBeamObjects[] = {
    4,
    64,
};

s32 gJupiterLighthouse_FourthBeamObjects[] = {
    64,
};

s32 gJupiterLighthouse_BeamRiders[] = {
    64,
};

// clang-format off
LATE_DATA u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

LATE_DATA u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

LATE_DATA s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
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

LATE_DATA u32 gJupiterLighthouse_IvanLeavesScript[] = {
    1, 38,
};

LATE_DATA u32 gJupiterLighthouse_WalkToPlatformsScript[] = {
    SC_SPEED(1),
    SC_ACCELERATION(1),
    SC_ANIMATION(2),
    SC_MOVE(480, 72, 360),
    SC_SYNC,
    SC_MOVE(456, 72, 360),
    SC_SYNC,
    SC_MOVE(440, 72, 256),
    SC_SYNC,
    SC_MOVE(408, 72, 256),
    SC_SYNC,
    SC_MOVE(409, 32, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

LATE_DATA s32 gJupiterLighthouse_HopWestOverPlatformsScript[] = {                                                               
      SC_ANIMATION(2),                                                               
      SC_SPEED(3.5),                                               
      SC_ACCELERATION(1),                                               
      SC_MOVE(409, 32, 312),                            
      SC_SYNC,                                                                 
      SC_WAIT(10),                                                             
      SC_MOVE(376, 32, 312),                            
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_VELOCITY_Y, toFIXED(5)),                                        
      SC_CALL(JupiterLighthouse_JumpAndHide),                                                  
      SC_SYNC,                                                                 
      SC_CALL(JupiterLighthouse_ShowSprite),                                                  
      SC_WAIT(20),                                                             
      SC_MOVE(344, 32, 312),                            
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_VELOCITY_Y, toFIXED(5)),                                        
      SC_WAIT(10),                                                             
      SC_CALL(JupiterLighthouse_JumpAndHide),                                                  
      SC_SYNC,                                                                 
      SC_CALL(JupiterLighthouse_ShowSprite),                                                  
      SC_WAIT(20),                                                             
      SC_MOVE(312, 32, 312),                            
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_VELOCITY_Y, toFIXED(5)),                                        
      SC_CALL(JupiterLighthouse_JumpAndHide),                                                  
      SC_SYNC,                                                                 
      SC_CALL(JupiterLighthouse_ShowSprite),                                                  
      SC_WAIT(20),                                                             
      SC_MOVE(312, 32, 278),                             
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_VELOCITY_Y, toFIXED(5)),                                        
      SC_CALL(JupiterLighthouse_JumpAndHide),                                                  
      SC_SYNC,                                                                 
      SC_CALL(JupiterLighthouse_ShowSprite),                                                  
      SC_ANIMATION(1),                                                               
      SC_END,                                                                  
  };

LATE_DATA s32 gJupiterLighthouse_HopEastOverPlatformsScript[] = {                                                               
      SC_ANIMATION(2),                                                               
      SC_SPEED(3.5),                                               
      SC_ACCELERATION(1),                                               
      SC_MOVE(616, 32, 312),                            
      SC_SYNC,                                                                 
      SC_WAIT(10),                                                             
      SC_MOVE(648, 32, 312),                            
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_VELOCITY_Y, toFIXED(5)),                                        
      SC_CALL(JupiterLighthouse_JumpAndHide),                                                  
      SC_SYNC,                                                                 
      SC_CALL(JupiterLighthouse_ShowSprite),                                                  
      SC_WAIT(20),                                                             
      SC_MOVE(681, 32, 312),                            
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_VELOCITY_Y, toFIXED(5)),                                        
      SC_WAIT(10),                                                             
      SC_CALL(JupiterLighthouse_JumpAndHide),                                                  
      SC_SYNC,                                                                 
      SC_CALL(JupiterLighthouse_ShowSprite),                                                  
      SC_WAIT(20),                                                             
      SC_MOVE(712, 32, 312),                            
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_VELOCITY_Y, toFIXED(5)),                                        
      SC_CALL(JupiterLighthouse_JumpAndHide),                                                  
      SC_SYNC,                                                                 
      SC_CALL(JupiterLighthouse_ShowSprite),                                                  
      SC_WAIT(20),                                                             
      SC_MOVE(712, 32, 264),                             
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_VELOCITY_Y, toFIXED(5)),                                        
      SC_CALL(JupiterLighthouse_JumpAndHide),                                                  
      SC_SYNC,                                                                 
      SC_CALL(JupiterLighthouse_ShowSprite),                                                  
      SC_ANIMATION(1),                                                               
      SC_END,                                                                  
  };

LATE_DATA s32 gJupiterLighthouse_StarFallsScript[] = {
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
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
    SC_ADD(SW_Y, toFIXED(2)),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA s32 gJupiterLighthouse_BeaconRisesScript[] = {                                                               
      SC_LABEL(1),                                                             
      SC_ADD(SW_SCALE_X, -1536),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(4, SC_TO(1)),                                                    
      SC_LABEL(2),                                                             
      SC_ADD(SW_SCALE_X, 0x600),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(4, SC_TO(2)),                                                    
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

LATE_DATA s32 gJupiterLighthouse_BeaconSparkScript[] = {
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseAerieBeacon),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ENTRANCE, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_AERIE_BEACON),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_JUPITER_LIGHTHOUSE_AERIE_BEACON),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_C, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_AERIE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(339, FLAG_JUPITER_BEACON_LIT, MOVEMENT_TYPE_STAND, 472, 40, 256, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(339, FLAG_JUPITER_BEACON_LIT, MOVEMENT_TYPE_STAND, 552, 40, 256, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(340, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(340, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 880, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 312, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 344, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 376, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 680, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 712, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(425, FLAG_JUPITER_BEACON_LIT, 11, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(425, CONDITION_ALWAYS, 11, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(426, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(426, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(33, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), MOVEMENT_TYPE_FACE_LEADER, 512, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), MOVEMENT_TYPE_FACE_LEADER, 480, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, IF_FLAG_SET(FLAG_JUPITER_BEACON_LIT), (s32)gJupiterLighthouse_BeaconRisesScript, 512, 8, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA s32 gJupiterLighthouse_BeaconParticleScript[] = {
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

void JupiterLighthouse_CastTheJupiterStar(void);

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 100, FLAG_SCENE_TEMP_00,
     EVENT(JupiterLighthouse_KarstAndAgatioWait)},
    {TRIGGER_STEP, 101, FLAG_JUPITER_BEACON_LIT,
     EVENT(JupiterLighthouse_CastTheJupiterStar)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgOnceYouVe)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgOnceJupiterIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     MSGNO(MsgOnceJupiterIsLit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS,
     MSGNO(MsgDoEvenThink)},
    {TRIGGER_TABLE_END},
};

// clang-format off
LATE_DATA s32 gJupiterLighthouse_BeamObjects[] = {
    17, 18, 19, 20, 21, 22,
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA u32 gJupiterLighthouse_WhirlwindFadesScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
