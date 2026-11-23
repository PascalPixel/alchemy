/// @file
/// Jupiter Lighthouse's aerie and its beacon: Karst and Agatio wait
/// (JupiterLighthouse_KarstAndAgatioWait), the party casts the Jupiter Star
/// into the well (JupiterLighthouse_CastTheJupiterStar) and the beacon is lit
/// (JupiterLighthouse_BeaconIsLit); Karst and Agatio are fought three times,
/// Jenna and Sheba coming to help (JupiterLighthouse_JennaJoinsTheFight,
/// JupiterLighthouse_ShebaJoinsTheFight), and Alex takes them down on the
/// elevator whether the party wins (JupiterLighthouse_WeBeatThem) or loses
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
#include "graphics/glyph.h"
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

/// The placed actors this file moves by number, besides the angels, the
/// archers and the lid (shared/jupiter_lighthouse_aerie.h). The party's rows
/// take their characters' numbers; the rest count on from 8.
enum {
  /// The chains hanging down the wall below the well (HangingVine_Place).
  ACTOR_WEST_CHAIN = 15,
  ACTOR_EAST_CHAIN,
  /// The six platforms floating over the shafts either side of the well, west
  /// to east.
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
  /// The bottom and top of the column of light that rises out of the well
  /// once the Jupiter Star is cast in (JupiterLighthouse_CastTheJupiterStar).
  ACTOR_BEAM_BOTTOM = 31,
  ACTOR_BEAM_TOP = 32,
  /// The lit beacon, a great swirling orb over the well.
  ACTOR_BEACON_ORB = 36,
  /// The Jupiter Star's shadow: an unseen marker whose shadow alone shows, at
  /// an eighth of its size, following the star from Felix to the well.
  ACTOR_STAR_SHADOW = 37
};

/// Set once Karst and Agatio have met Felix on his way up to the well, so that
/// they do not stop him again.
#define FLAG_KARST_AND_AGATIO_MET_FELIX FLAG_SCENE_TEMP_00
/// Sends up the column of light's particles every frame rather than every
/// third (JupiterLighthouse_SpawnBeaconParticle); nothing here sets it.
#define FLAG_BEACON_PARTICLES_EVERY_FRAME FLAG_SCENE_TEMP_01
/// Set once the unused JupiterLighthouse_RaiseBeaconLight has finished.
#define FLAG_BEACON_LIGHT_RISEN FLAG_SCENE_TEMP_02

/// The cells on the way up to the well where Karst and Agatio meet Felix, and
/// the row before the well where he can cast the Jupiter Star.
enum { CELL_BEFORE_KARST_AND_AGATIO = 100, CELL_BEFORE_THE_WELL };

/// The entrances the beacon's events arrive by: back from the World Map's
/// view of the lit beacon, and after each battle with Karst and Agatio.
enum {
  ENTRANCE_BEACON_LIT = 9,
  ENTRANCE_AFTER_FIRST_BATTLE,
  ENTRANCE_AFTER_SECOND_BATTLE,
  ENTRANCE_LAST_BATTLE_WON,
  ENTRANCE_LAST_BATTLE_LOST
};

/// An actor-script call: plays the jump sound and drops the actor's shadow as
/// it hops onto the next floating platform.
s32 JupiterLighthouse_JumpWithoutShadow(struct Actor *actor) {
  Event_PlaySound(SE_JUMP);
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// An actor-script call: gives the actor its shadow back once it has landed.
s32 JupiterLighthouse_ShowShadow(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

#include "../field/actor_park_record.c"

/// Turns a particle round the column of light's bottom (ACTOR_BEAM_BOTTOM) by
/// its timer, a thirty-second of a turn a frame, as far across as its speed
/// and two pixels deep.
void JupiterLighthouse_CircleBeacon(struct Actor *self) {
  struct Actor *centre = Actor_Get(ACTOR_BEAM_BOTTOM);
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
  self->timer -= DEGtoANG(45) / 4;
}

/// Turns a spark round the beacon's rising light (ACTOR_BEACON_ORB), wider
/// across than deep, its radius from its speed and its riseCounter.
void JupiterLighthouse_CircleBeaconWide(struct Actor *self) {
  struct Actor *centre = Actor_Get(ACTOR_BEACON_ORB);
  u16 angle = self->timer;
  FIXED cosine = Trig_Cos(angle);
  s32 radius = self->speedLimit + self->riseCounter;
  self->x.fixed = centre->x.fixed + cosine * (radius + 6);
  self->z.fixed = centre->z.fixed + Math_Sin(angle) * (self->riseCounter + 4);
  self->targetX = self->x.fixed;
  self->targetZ = self->z.fixed;
  self->timer -= DEGtoANG(45) / 4;
}

#include "../field/actor_step_angle.c"

#include "text/msg_ids.h"

struct CellTarget;
struct CellTargetEntry;

void JupiterLighthouse_KarstAndAgatioWait(void);

void SceneActor_ParkRecord(struct Actor *actor);

s32 gJupiterLighthouse_ElevatorLevel BSS_DATA;
/// Unused: nothing reads these bytes.
u8 gJupiterLighthouse_UnusedAfterElevatorLevel[4] BSS_DATA;
s32 gJupiterLighthouse_BeaconRiseFrame BSS_DATA;
s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
/// The cells CrackedTile_Step watches, and the cell it copies over the one the
/// leader steps on.
struct CellTarget gCrackedTiles BSS_DATA;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void Event_End(void);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern s32 gJupiterLighthouse_BeaconRisesScript[];
extern s32 gJupiterLighthouse_BeaconSparkScript[];

void FieldCamera_WaitForArrival(void);

/// Whom JupiterLighthouse_LowerElevator carries down, and where the camera
/// goes: Alex, Agatio and Karst with the camera moving south after them;
/// Isaac, the camera after him; Felix, carried all the way down; and Alex,
/// Agatio and Karst with the camera following Isaac.
enum ElevatorDescent {
  DESCENT_ALEX_AGATIO_KARST,
  DESCENT_ISAAC,
  DESCENT_FELIX,
  DESCENT_ALEX_AGATIO_KARST_FOLLOWING_ISAAC
};

/// How JupiterLighthouse_RaiseElevator brings the elevator back: put back at
/// once, or rising with the camera moving after it.
enum ElevatorAscent { ASCENT_AT_ONCE, ASCENT_RISING };

void JupiterLighthouse_LowerElevator(s32 mode);
void JupiterLighthouse_RaiseElevator(s32 mode);
void JupiterLighthouse_PlaceRaisedActors(void);

s32 HangingVine_Place();

void MapGrid_SetAttributeBAt();
s32 MapGrid_GetCellBehavior();

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

void Actor_StartScript(struct Actor *actor, const void *script);

/// Unused: one frame of the beacon's rise as The Broken Seal's Venus
/// Lighthouse aerie plays it, counted by gJupiterLighthouse_BeaconRiseFrame:
/// the tower shakes and flashes, the orb (ACTOR_BEACON_ORB) climbs from below
/// the map shedding sparks, and the screen fades to white;
/// FLAG_BEACON_LIGHT_RISEN is set once it is done. No event here starts it.
UNUSED void JupiterLighthouse_RaiseBeaconLight(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 spawnParticles;
  u32 randomHeight;
  s32 angle;

  center = Actor_GetById(ACTOR_BEACON_ORB);
  spawnParticles = 0;
  switch (gJupiterLighthouse_BeaconRiseFrame) {
  case 0:
    Event_PlaySound(SE_SHIMMER);
    MapGrid_SetShake(6, 6, 1);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 31, 24), PALETTES_BG);
    PaletteFade_Interpolate(8);
    break;
  case 8:
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
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
    gJupiterLighthouse_BeaconRiseFrame--;
    if (center->y.fixed > 0) {
      PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12),
                              PALETTES_ALL);
      PaletteFade_Interpolate(16);
      gJupiterLighthouse_BeaconRiseFrame++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(SE_GLITTER);
      spawnParticles = 1;
      center->y.fixed += toFIXED(9) / 4;
    }
    break;
  case 26:
    gJupiterLighthouse_BeaconRiseFrame--;
    if (center->y.fixed > toFIXED(40)) {
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_Interpolate(40);
      gJupiterLighthouse_BeaconRiseFrame++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(SE_GLITTER);
      spawnParticles = 1;
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
    spawnParticles = 1;
    break;
  case 36:
    Event_PlaySound(SE_FALLING_LIGHT);
    PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
    PaletteFade_Interpolate(12);
    break;
  case 48:
    FieldActor_Stop(ACTOR_BEACON_ORB);
    Event_SetFlag(FLAG_BEACON_LIGHT_RISEN);
    break;
  }
  if (spawnParticles) {
    randomHeight = toFIXED(Random_Below(80));
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               center->y.fixed - randomHeight + -toFIXED(8),
                               center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Actor_StartScript(effect, gJupiterLighthouse_BeaconSparkScript);
      FieldActor_SetSpriteColor(effect, SPRITE_COLOR_GRB);
      effect->motionFlags = 0;
      angle = Random16() & 0xffff000;
      effect->timer = angle;
      effect->userValue = 0;
      {
        u32 counter = Random16() >> 13;

        effect->riseCounter = counter;
      }
      effect->update = JupiterLighthouse_CircleBeaconWide;
      effect->speedLimit = Math_Sin((u32)(Random16() * 65535) >> 20) * 24;
      effect->speedLimit = FIXEDtoINT(center->speedLimit);
      sprite->flags = 0;
      sprite->priority = 1;
    }
  }
  gJupiterLighthouse_BeaconRiseFrame++;
}

/// Walking up towards the well in the aerie's centre (cells (33, 21) and
/// (34, 21)) once Isaac and Ivan have fallen, Felix is met by Karst and
/// Agatio waiting there: "You're late, Felix!" and "Now light the beacon!".
void JupiterLighthouse_KarstAndAgatioWait(void) {

  if (Event_GetFlag(FLAG_ISAAC_AND_IVAN_FELL)) {
    Event_SetFlag(FLAG_KARST_AND_AGATIO_MET_FELIX);
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

/// Every third frame, or every frame with FLAG_BEACON_PARTICLES_EVERY_FRAME,
/// sends up a particle from a random height on the first column
/// of light, circling it (JupiterLighthouse_CircleBeacon).
void JupiterLighthouse_SpawnBeaconParticle(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(FLAG_BEACON_PARTICLES_EVERY_FRAME) == 0 &&
      gFrameCount % 3 != 0)
    return;
  center = Actor_Get(ACTOR_BEAM_BOTTOM);
  if (Event_GetFlag(FLAG_BEACON_PARTICLES_EVERY_FRAME) != 0) {
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
    FieldActor_SetSpriteColor(effect, SPRITE_COLOR_GRB);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->timer = angle;
    effect->userValue = 0;
    effect->update = JupiterLighthouse_CircleBeacon;
    effect->speedLimit = Math_Sin((u32)(Random16() * 65535) >> 20) * 24 >> 16;
    sprite->flags = 0;
    sprite->priority = 1;
  }
}

/// The furthest south the view may lie, in pixels, with the lit beacon's light
/// drawn over the well.
#define BEACON_ORB_VIEW_Z_MAX 177

/// The lit beacon's light (ACTOR_BEACON_ORB) pulses over the well while the
/// view lies no further south than BEACON_ORB_VIEW_Z_MAX: full size on odd
/// frames, 1.3 times the size and 7 pixels lower on even ones. Further south
/// it is put away at the origin.
void JupiterLighthouse_PulseBeacon(void) {
  struct Actor *actor = Actor_Get(ACTOR_BEACON_ORB);
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  Random16();
#if TLA_PATCH_1
  // From patch 1; the Japanese release goes on when the light is missing.
  if (actor == NULL)
    return;
#endif
  if (FIXEDtoINT(map->viewZ) <= BEACON_ORB_VIEW_Z_MAX) {
    ((struct SpriteAnimation
          *)((struct FieldSprite *)Actor_Get(ACTOR_BEACON_ORB)->sprite)
         ->record)
        ->colorMode = SPRITE_COLOR_GRB;
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

s32 Actor_PlaceOnActor();
s32 PartyInventory_Remove(s32 item_id);

extern s32 gJupiterLighthouse_BeamOpenScript[];

/// Stepping up to the well in the aerie's centre (cells (29, 18) to (34, 18))
/// once Isaac and Ivan have fallen, Felix is asked whether to cast the
/// Elemental Star into its wide hole. Yes, and he throws the Jupiter Star in,
/// its light rises out of the well in two beams and the beacon is lit; the
/// lit beacon is shown from the world map, then the party returns to the aerie
/// beside it. No, and he steps back from the well.
///
/// The party returns by ENTRANCE_BEACON_LIT, where Karst and Agatio turn on it
/// (JupiterLighthouse_BeaconIsLit).
void JupiterLighthouse_CastTheJupiterStar(void) {
  s32 answer, state;
  struct Actor *felix, *star, *shadow, *bottom, *top;
  struct FieldSprite *sprite;
  GlyphTransfer *glyph;
  u8 *flags;

  if (GameFlag_Get(FLAG_ISAAC_AND_IVAN_FELL) != 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    // "There seems to be a wide hole for the Elemental Star. Cast the..."
    Message_ShowCentered((s32)MsgThereSeemsTo, 5);
    answer = Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
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
      PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
      PaletteFade_Interpolate(20);
      System_Wait(20);
      PartyInventory_Remove(ITEM_MYTHRIL_BAG_B);
      felix = Actor_GetById(ACTOR_FELIX);
      star = FieldActor_Create(SPRITE_UNSEEN_MARKER, felix->x.fixed,
                               felix->y.fixed + toFIXED(8), felix->z.fixed);
      if (star != 0) {
        sprite = star->sprite;
        sprite->countAndShadow[0] = 0;
        sprite->fullColor = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        star->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        star->motionFlags = 0;
        star->retainSpriteCache = 1;
        star->speedLimit = toFIXED(2) / 5;
        star->acceleration = toFIXED(1) / 20;
        glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
        Item_LoadIcon(ITEM_JUPITER_STAR);
        Vram_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
        HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
      }
      FieldActor_SetSpritePriority(ACTOR_STAR_SHADOW, 1);
      shadow = Actor_GetById(ACTOR_STAR_SHADOW);
      shadow->x.fixed = felix->x.fixed;
      shadow->y.fixed = toFIXED(96);
      shadow->z.fixed = felix->z.fixed;
      shadow->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
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
        Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
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
        state = ACTOR_FLOATS;
        *flags = state;
        FieldEvent_Wait(90);
        *flags = 0;
        FieldActor_SetMoveTarget(star, 512, 128, 264);
        FieldActor_SetMoveTargetFX(shadow, toFIXED(512), -1, toFIXED(264));
        *flags = state;
        FieldEvent_Wait(60);
        *flags = 0;
        shadow->x.fixed = 0;
        shadow->targetX = ACTOR_NO_TARGET;
        shadow->targetY = ACTOR_NO_TARGET;
        shadow->targetZ = ACTOR_NO_TARGET;

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
        star->targetX = ACTOR_NO_TARGET;
        star->targetY = ACTOR_NO_TARGET;
        star->targetZ = ACTOR_NO_TARGET;
        star->y.fixed = 0;
        star->z.fixed = 0;
        star->velocityX = 0;
        star->velocityY = 0;
        star->velocityZ = 0;
        star->timer = 0;
      }
      FieldEvent_Wait(40);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
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
      Actor_GetById(ACTOR_BEAM_BOTTOM)->priorityFlags |=
          ACTOR_PRIORITY_AUTOMATIC;
      Actor_GetById(ACTOR_BEAM_TOP)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      FieldCamera_MoveTo(512, 96, 264, 1);
      FieldCamera_WaitForArrival();
      FieldActor_SetSpritePriority(ACTOR_BEAM_BOTTOM, 1);
      FieldActor_SetSpritePriority(ACTOR_BEAM_TOP, 1);
      FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
      FieldActor_SetSpritePriority(ACTOR_PIERS, 1);
      FieldEvent_Wait(20);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEAM_BOTTOM), 0);
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_BEAM_BOTTOM),
                                SPRITE_COLOR_WHITE);
      bottom = Actor_GetById(ACTOR_BEAM_BOTTOM);
      bottom->scaleY = -FIXED_ONE;
      bottom->scaleX = FIXED_ONE / 10;
      bottom->motionFlags = 0;
      bottom->x.fixed = toFIXED(512);
      bottom->y.fixed = toFIXED(128);
      bottom->z.fixed = toFIXED(264);
      bottom->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEAM_TOP), 0);
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_BEAM_TOP),
                                SPRITE_COLOR_WHITE);
      top = Actor_GetById(ACTOR_BEAM_TOP);
      top->scaleY = -FIXED_ONE;
      top->scaleX = FIXED_ONE / 10;
      top->motionFlags = 0;
      top->x.fixed = toFIXED(512);
      top->y.fixed = toFIXED(160);
      top->z.fixed = toFIXED(264);
      top->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetScript(ACTOR_BEAM_BOTTOM, gJupiterLighthouse_BeamOpenScript);
      Actor_SetScript(ACTOR_BEAM_TOP, gJupiterLighthouse_BeamOpenScript);
      Task_AddFrame(JupiterLighthouse_SpawnBeaconParticle, TASK_PASS_FRAME);
      Actor_SetPosition(ACTOR_LEFT_ANGEL, 0, 0);
      Actor_SetPosition(ACTOR_RIGHT_ANGEL, 0, 0);
      Actor_SetPosition(ACTOR_LID, 0, 0);
      Actor_SetPosition(ACTOR_UNSEEN_LID, 0, 0);
      Event_PlaySound(SE_CRASH);
      MapGrid_SetShake(6, 6, 1);
      PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24),
                              PALETTES_ALL);
      PaletteFade_Interpolate(16);
      System_Wait(16);
      Event_PlaySound(MUS_ALCHEMY_UNLEASHED);
      System_Wait(180);
      PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
      PaletteFade_Interpolate(60);
      Event_PlaySound(SE_RUMBLE);
      System_Wait(60);
      FieldTask_Remove(JupiterLighthouse_SpawnBeaconParticle);
      Event_SetFlag(FLAG_KEEP_MUSIC_ONCE);
      FieldEvent_SetExitRequest(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_WORLD_MAP);
    }
    Event_End();
  }
}

struct Unit *Unit_Get(u32 unitId);
void Inventory_Equip(s32 id, s32 slot);

/// Back on the aerie beside the lit beacon, Karst and Agatio turn on Felix and
/// Piers, give back the Mythril Bag, and the first battle with them begins.
/// The party arrives by ENTRANCE_BEACON_LIT.
void JupiterLighthouse_BeaconIsLit(void) {
  struct FieldWork *work;
  s32 slot;
  s32 facing;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 120;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(512, 96, 240, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  FieldEvent_SetMessageId((s32)MsgWeHaveIgnited);
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(508), toFIXED(278),
                             facing = FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(532), toFIXED(278), facing);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(4, 4, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  MapGrid_SetShake(3, 3, 1);
  FieldEvent_Wait(60);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(512, 96, 296, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(20);
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
  FieldEvent_Wait(5);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  // "We have ignited the beacon... The third lighthouse is lit... Now let..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  Event_PlaySound(MUS_KARST_AND_AGATIO);
  // "I don't think so..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
  // "You see, you've betrayed us once already..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  // "We simply can't trust you anymore..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  // "And now that Jupiter Lighthouse is lit, I'm afraid we have little use..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  FieldEvent_Wait(5);
  // "Poor dear... You look puzzled. Would you like to know why we don't..."
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    // "I'd be happy to tell you. Mars Lighthouse is in Prox."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    // "Figured it out, have you? Yes, Mars Lighthouse is in Prox."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
  }
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_KARST, -16, -24);
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_Get(ACTOR_KARST)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_KARST)->velocityY = toFIXED(4);
  Actor_SetDestinationOffset(ACTOR_KARST, 0, 24);
  FieldActor_WaitForMoveById(ACTOR_KARST);
  Actor_Get(ACTOR_KARST)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  // "What are you doing!?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  // "They stole the Mars Star!"
  Message_ShowCentered(gFieldWork->message++, 5);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if ((Unit_Get(FELIX)->inventory[slot] & INVENTORY_ITEM_ID_MASK) ==
        ITEM_MYTHRIL_BAG) {
      Inventory_Equip(FELIX, slot);
      break;
    }
  }
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_Wait(5);
  Actor_WalkBy(ACTOR_FELIX, 0, 8);
  Actor_WalkByAndWait(ACTOR_AGATIO, -24, -8);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, -8);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Your job is done now."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  // "We're not done yet..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
  // "We've got the Mars Star! We don't need you anymore!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHWEST, 0);
  // "You know what that means, of course, don't you?"
  Event_OpenMessage(ACTOR_KARST, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
    // "Exactly! That means it's time to die, Felix!"
    FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
    gFieldWork->message++;
  } else {
    Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
    gFieldWork->message++;
    // "No? Well, how can I put this? It's time to die!"
    FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  }
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(5);
  // "But what will happen to Felix's parents?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
  // "We keep our promises! Once all four lighthouses have been lit, we..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHEAST, 0);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
  // "We would never do anything as terrible as breaking our word... as you..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHWEST, 0);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
  // "But if you're dead... Who will come to take them home from Prox?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
  Actor_AnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
  // "I guess they'll just have to spend their remaining years in the..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 2);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(5);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  // "Don't worry! They'll be fine! Now prepare for the end!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(
      (s32)&SceneId_JupiterLighthouseAerieBeacon, ENTRANCE_AFTER_FIRST_BATTLE);
  Event_SetDefeatDestination((s32)&SceneId_JupiterLighthouseAerieBeacon,
                             ENTRANCE_AFTER_FIRST_BATTLE);
  Event_SetFlag(FLAG_KEEP_MUSIC_HELD);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MOAPA, 5);
  return;
}

/// The HP the next battle's event takes from Felix when the party lost to
/// Karst and Agatio: more than the 1 HP the defeat revived him with
/// (Scene_ReturnFromSequence), so he goes into it downed.
#define DEFEAT_REVIVAL_HP_TAKEN 10

/// After the first battle with Karst and Agatio on the aerie, Jenna comes to
/// help, joins Felix, and the second battle begins. The party arrives by
/// ENTRANCE_AFTER_FIRST_BATTLE.
void JupiterLighthouse_JennaJoinsTheFight(void) {
  s32 northwest;
  s32 southeast;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  northwest = FACING_NORTHWEST;
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(512), toFIXED(312),
                             northwest);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(528), toFIXED(304),
                             northwest);
  southeast = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(504), toFIXED(304),
                             southeast);
  {
    Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(520), toFIXED(288),
                               southeast);
  }
  {
    Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(552), toFIXED(392),
                               FACING_NORTH);
  }
  Actor_SetAnimation(ACTOR_AGATIO, 9);
  Actor_SetAnimation(ACTOR_KARST, 8);
  Actor_SetAnimation(ACTOR_FELIX, 50);
  Actor_SetAnimation(ACTOR_PIERS, 12);
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
  Actor_FaceDirection(ACTOR_KARST, southeast, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, southeast);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, northwest);
  FieldEvent_Wait(10);
  // "Don't worry, Felix! I'm here to help!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_WalkBy(ACTOR_JENNA, -8, -24);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, southeast);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  Actor_FaceDirection(ACTOR_PIERS, northwest, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, northwest);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, northwest);
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
  if (gMainRoutineWork.sequenceResult == BATTLE_OUTCOME_LOST) {
    FieldUnit_AddHp(FELIX, -DEFEAT_REVIVAL_HP_TAKEN);
  }
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(
      (s32)&SceneId_JupiterLighthouseAerieBeacon, ENTRANCE_AFTER_SECOND_BATTLE);
  Event_SetDefeatDestination((s32)&SceneId_JupiterLighthouseAerieBeacon,
                             ENTRANCE_AFTER_SECOND_BATTLE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MOAPA, 6);
}

/// After the second battle on the aerie, Sheba comes to help, joins the party,
/// and the last battle with Karst and Agatio begins. The party arrives by
/// ENTRANCE_AFTER_SECOND_BATTLE.
void JupiterLighthouse_ShebaJoinsTheFight(void) {
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
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
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
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
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
  if (gMainRoutineWork.sequenceResult == BATTLE_OUTCOME_LOST)
    FieldUnit_AddHp(FELIX, -DEFEAT_REVIVAL_HP_TAKEN);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(
      (s32)&SceneId_JupiterLighthouseAerieBeacon, ENTRANCE_LAST_BATTLE_WON);
  Event_SetDefeatDestination((s32)&SceneId_JupiterLighthouseAerieBeacon,
                             ENTRANCE_LAST_BATTLE_LOST);
  Event_ClearFlag(FLAG_KEEP_MUSIC_HELD);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MOAPA, 7);
}

/// The caster's arcs of light reach a fallen actor and it stands up: Mia
/// healing Felix's party, or Alex healing Agatio and Karst.
void JupiterLighthouse_HealActor(s32 casterId, s32 targetId) {
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(casterId), 2);
  Event_PlaySound(SE_ASCEND);
  FieldEvent_Wait(60);
  BattleEffect_SetObjectUpdateMode(Actor_Get(casterId), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Actor_SetAnimation(targetId, ANIM_STAND);
}

extern s32 gJupiterLighthouse_JoinLeaderScript[];
extern s32 gJupiterLighthouse_WalkToPlatformsScript[];
extern s32 gJupiterLighthouse_HopWestOverPlatformsScript[];
extern s32 gJupiterLighthouse_HopEastOverPlatformsScript[];

/// When the party loses the last battle on the aerie, Alex arrives and takes
/// Karst and Agatio down on the elevator; Isaac's party comes up, Mia heals
/// Felix's party, and both parties leave on the elevator. The party arrives by
/// ENTRANCE_LAST_BATTLE_LOST.
void JupiterLighthouse_KarstAndAgatioRetreat(void) {
  s32 northwest;
  s32 southeast;
  s32 north;
  struct Actor *actor;

  struct FieldWork *work;

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
      member->hpGauge = Unit_ClampGauge((maxHp << UNIT_GAUGE_SHIFT) / maxHp);
      if (member->hpGauge == 0 && member->hp != 0)
        member->hpGauge = 1;
      member->ppGauge =
          Unit_ClampGauge((member->pp << UNIT_GAUGE_SHIFT) / member->ppMax);
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
  north = FACING_NORTH;
  northwest = FACING_NORTHWEST;
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(504), toFIXED(304),
                             northwest);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(528), toFIXED(288),
                             northwest);
  southeast = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(488), toFIXED(296),
                             southeast);
  Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(504), toFIXED(280),
                             southeast);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(528), toFIXED(304),
                             northwest);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(552), toFIXED(288), north);
  Actor_SetPositionAndFacing(ACTOR_ALEX, toFIXED(480), toFIXED(360), north);
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
  Actor_FaceDirectionAndWait(ACTOR_KARST, southeast);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, southeast);
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
  Actor_FaceDirection(ACTOR_KARST, southeast, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, southeast);
  FieldEvent_Wait(5);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, north);
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
  Actor_SetScript(ACTOR_AGATIO, gJupiterLighthouse_WalkToPlatformsScript);
  FieldEvent_Wait(30);
  Actor_SetScript(ACTOR_ALEX, gJupiterLighthouse_WalkToPlatformsScript);
  Actor_SetScript(ACTOR_KARST, gJupiterLighthouse_WalkToPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_ALEX);
  Actor_Get(ACTOR_ALEX)->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_AGATIO)->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_KARST)->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_ALEX)->motionFlags = ACTOR_FALLS;
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
  // "Felix! Jenna!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(512), toFIXED(328), north);
  JupiterLighthouse_LowerElevator(DESCENT_ALEX_AGATIO_KARST);
  Scene_PlayMusic();
  FieldEvent_Wait(10);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(536), toFIXED(368), north);
  Actor_SetPositionAndFacing(ACTOR_GARET, toFIXED(536), toFIXED(384), north);
  Actor_SetPositionAndFacing(ACTOR_IVAN, toFIXED(552), toFIXED(368), north);
  Actor_SetPositionAndFacing(ACTOR_MIA, toFIXED(552), toFIXED(352), north);
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
  Actor_AnimateThenWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 33);
  // "I had a really bad feeling after Sheba left. I hate it when I'm right..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_SOUTH);
  FieldEvent_Wait(10);
  // "Your power of prediction must be even stronger than you'd thought..."
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, northwest);
  FieldCamera_MoveTo(320, 96, 360, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  // "Blast! They must have taken the elevator to escape!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  FieldCamera_MoveTo(512, 96, 352, 1);
  FieldCamera_WaitForArrival();
  JupiterLighthouse_RaiseElevator(ASCENT_AT_ONCE);
  Actor_AnimateThenWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 33);
  // "How could they have done that to Jenna? Those monsters!"
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, northwest);
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
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, north);
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
  Actor_AnimateThenWait(ACTOR_MIA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_GARET, northwest);
  // "Don't let 'em die! Felix still has a lot of questions to answer..."
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, southeast);
  FieldEvent_Wait(10);
  // "If you save Felix, we'll tell you everything..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, north);
  Actor_WalkByAndWait(ACTOR_MIA, -48, 0);
  Actor_FaceDirection(ACTOR_MIA, north, 0);
  JupiterLighthouse_HealActor(ACTOR_MIA, ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, southeast, 0);
  actor = Actor_Get(ACTOR_FELIX);
  actor->scaleX = -actor->scaleX;
  Actor_WalkByAndWait(ACTOR_MIA, 40, 0);
  Actor_WalkByAndWait(ACTOR_MIA, 8, -16);
  Actor_FaceDirection(ACTOR_MIA, north, 0);
  JupiterLighthouse_HealActor(ACTOR_MIA, ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  actor = Actor_Get(ACTOR_SHEBA);
  actor->scaleX = -actor->scaleX;
  Actor_FaceDirection(ACTOR_MIA, northwest, 0);
  JupiterLighthouse_HealActor(ACTOR_MIA, ACTOR_PIERS);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  JupiterLighthouse_HealActor(ACTOR_MIA, ACTOR_JENNA);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, northwest);
  // "All right, Felix! We want to know what's going on!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, southeast, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "It's all quite complicated, Isaac. There are a number of extenuating..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "What are you talking about, Kraden?"
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_WalkTo(ACTOR_IVAN, 552, 320);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
  Actor_FaceDirection(ACTOR_GARET, north, 0);
  Actor_AnimateThenWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 33);
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
  Actor_AnimateThenWait(ACTOR_GARET, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, north);
  FieldEvent_Wait(50);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, northwest);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, southeast);
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    Actor_AnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
    FieldEvent_Wait(10);
    // "You're right. We can't fight right now. Let's head back to..."
    FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  } else {
    Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    Actor_AnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
    gFieldWork->message += 2;
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
    // "He's right, Felix. It's time we explained ourselves to them. We..."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    FieldEvent_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    FieldEvent_Wait(5);
    // "We'll be there, Isaac. I promise. I hope you can still trust me."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    Actor_AnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
    FieldEvent_Wait(10);
    // "I've never stopped trusting you, Jenna. I'll be waiting for you in..."
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
  Actor_SetScript(ACTOR_IVAN, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_MIA, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_GARET, gJupiterLighthouse_JoinLeaderScript);
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
  // Yes: "We'll be waiting for you... So don't think you can sneak off
  // again!"; No: "We'll be expecting you. Don't think you can sneak out
  // of this!..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_WalkToAndWait(ACTOR_ISAAC, 573, 358);
  Actor_WalkToAndWait(ACTOR_ISAAC, 584, 300);
  Actor_WalkToAndWait(ACTOR_ISAAC, 584, 252);
  Actor_WalkToAndWait(ACTOR_ISAAC, 616, 247);
  Actor_WalkToAndWait(ACTOR_ISAAC, 617, 312);
  Actor_Get(ACTOR_ISAAC)->motionFlags = ACTOR_FALLS;
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
  JupiterLighthouse_LowerElevator(DESCENT_ISAAC);
  FieldEvent_Wait(100);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  JupiterLighthouse_RaiseElevator(ASCENT_RISING);
  FieldEvent_Wait(60);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  FieldEvent_Wait(60);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_SHEBA, ANIM_NOD, 20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  actor = Actor_Get(ACTOR_FELIX);
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
  Actor_SetScript(ACTOR_PIERS, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_SHEBA, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_JENNA, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_KRADEN, gJupiterLighthouse_JoinLeaderScript);
  FieldActor_WaitForScriptEndById(ACTOR_PIERS);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  FieldActor_WaitForScriptEndById(ACTOR_JENNA);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, 0);
  Actor_SetScript(ACTOR_FELIX, gJupiterLighthouse_WalkToPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_ALEX)->motionFlags = ACTOR_FALLS;
  Actor_SetScript(ACTOR_FELIX, gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_WalkToAndWait(ACTOR_FELIX, 311, 256);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  JupiterLighthouse_LowerElevator(DESCENT_FELIX);
  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 24;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  JupiterLighthouse_HealParty(0);
  FieldEvent_SetExitRequest(
      JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ENTRANCE);
  Event_End();
  return;
}

/// When the party wins the last battle on the aerie, Alex heals Karst and
/// Agatio and takes them down on the elevator; Isaac's party comes up, and
/// both parties leave on the elevator. The party arrives by
/// ENTRANCE_LAST_BATTLE_WON.
void JupiterLighthouse_WeBeatThem(void) {
  s32 northwest;
  s32 southeast;
  s32 north;
  struct Actor *leader;
  struct FieldWork *work;

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
  northwest = FACING_NORTHWEST;
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(520), toFIXED(304),
                             northwest);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(536), toFIXED(288),
                             northwest);
  southeast = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_AGATIO, toFIXED(480), toFIXED(296),
                             southeast);
  Actor_SetPositionAndFacing(ACTOR_KARST, toFIXED(496), toFIXED(280),
                             southeast);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(544), toFIXED(304),
                             northwest);
  north = FACING_NORTH;
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(560), toFIXED(288), north);
  Actor_SetPositionAndFacing(ACTOR_ALEX, toFIXED(480), toFIXED(360), north);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(592), toFIXED(392),
                             FACING_EAST);
  Actor_SetAnimation(ACTOR_AGATIO, 10);
  Actor_SetAnimation(ACTOR_KARST, 9);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(520, 104, 312, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWeDidItWe);
  Actor_AnimateThenWait(ACTOR_PIERS, ANIM_SHAKE_HEAD, 33);
  // "We did it... We beat them..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
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
  Actor_FaceDirection(ACTOR_FELIX, northwest, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, northwest);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
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
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    // "No, Felix! Don't!"
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    gFieldWork->message = (gFieldWork->message + 1);
    // "You've made the better choice, Felix."
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  }
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveTo(520, 104, 352, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Event_PlaySound(MUS_ALEX);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  // "When did you get here, Alex?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  FieldCamera_MoveTo(520, 104, 312, 1);
  Actor_WalkToAndWait(ACTOR_ALEX, 480, 320);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
  // "If Karst is not bluffing... If you kill them, all of your efforts..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  // "Do you understand, Felix? If that happens, Isaac will suffer the same..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, north);
  Actor_WalkByAndWait(ACTOR_ALEX, 20, -20);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_WEST);
  JupiterLighthouse_HealActor(ACTOR_ALEX, ACTOR_AGATIO);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Alex! What are you doing!?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, north);
  JupiterLighthouse_HealActor(ACTOR_ALEX, ACTOR_KARST);
  Actor_WalkByAndWait(ACTOR_ALEX, -20, 20);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I have revived them."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_AGATIO, 2);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  FieldEvent_Wait(5);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Why!? Are you going to make us fight them again?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
  // "Don't worry, Jenna. Look at them... They can barely walk right now."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_AnimateThenWait(ACTOR_PIERS, ANIM_SHAKE_HEAD, 33);
  // "Still, that doesn't give you the right to..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "If you're not going to finish them off, you'll just be leaving them..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_SOUTH);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You're not going to help us defeat these brats?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_SOUTH);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You'd better not be expecting a big thank-you for this, Alex!"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Of course not, Karst. I would never ask for your gratitude."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  // "What's keeping Felix and the others?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_SOUTHWEST);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldActor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  Actor_FaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "We should be going before Isaac and the others arrive."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(5);
  // "If they're coming up, how do you plan on avoiding them on your way down?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Do not worry about us... We'll just take the elevator."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Now that the lighthouse has been lit, it should be fully operational..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, north);
  Actor_AnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KARST, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_KARST, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  EventCamera_FollowActor(ACTOR_AGATIO, 1);
  Actor_SetScript(ACTOR_ALEX, gJupiterLighthouse_WalkToPlatformsScript);
  FieldEvent_Wait(30);
  Actor_SetScript(ACTOR_AGATIO, gJupiterLighthouse_WalkToPlatformsScript);
  FieldEvent_Wait(30);
  Actor_SetScript(ACTOR_KARST, gJupiterLighthouse_WalkToPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_ALEX);
  Actor_Get(ACTOR_ALEX)->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_AGATIO)->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_KARST)->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_ALEX)->motionFlags = ACTOR_FALLS;
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
  // "Oh! You're looking better!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  {
    Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(512), toFIXED(328),
                               FACING_NORTH);
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
  JupiterLighthouse_LowerElevator(DESCENT_ALEX_AGATIO_KARST_FOLLOWING_ISAAC);
  Scene_PlayMusic();
  // "Felix, Jenna..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Isaac... Garet..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTH);
  FieldEvent_Wait(20);
  JupiterLighthouse_RaiseElevator(ASCENT_AT_ONCE);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Man! They got away! What a bunch of jerks!"
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  FieldEvent_Wait(5);
  // "All right, Felix! We want to know what's going on!"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTHEAST);
  // "It's all quite complicated, Isaac. There are a number of extenuating..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "What are you talking about, Kraden?"
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_WalkTo(ACTOR_IVAN, 552, 320);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTH);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_SOUTHWEST);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
  Actor_AnimateThenWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 33);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  // "Look, I don't think any of us are in any shape for another fight..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 2);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  FieldEvent_Wait(5);
  // "What do you mean, Ivan?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_SOUTHWEST);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_Wait(5);
  // "I'm just saying we should get ourselves back to Contigo and sort..."
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
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
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldEvent_Wait(60);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTH);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_GARET, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  FieldEvent_Wait(60);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    Actor_AnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
    FieldEvent_Wait(10);
    // "You're right. We can't fight right now. Let's head back to..."
    FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
    Actor_AnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
    FieldEvent_Wait(10);
  } else {
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    Actor_AnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
    gFieldWork->message = (gFieldWork->message + 2);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
    // "He's right, Felix. It's time we explained ourselves to them. We..."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    FieldEvent_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    FieldEvent_Wait(5);
    // "We'll be there, Isaac. I promise. I hope you can still trust me."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    Actor_AnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
    FieldEvent_Wait(10);
    // "I've never stopped trusting you, Jenna. I'll be waiting for you in..."
    FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_ISAAC);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_IVAN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_ISAAC);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_MIA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_ISAAC);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_SetScript(ACTOR_IVAN, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_MIA, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_GARET, gJupiterLighthouse_JoinLeaderScript);
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
  // Yes: "We'll be waiting for you... So don't think you can sneak off
  // again!"; No: "We'll be expecting you. Don't think you can sneak out
  // of this!..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 0);
  Actor_WalkToAndWait(ACTOR_ISAAC, 573, 358);
  Actor_WalkToAndWait(ACTOR_ISAAC, 584, 300);
  Actor_WalkToAndWait(ACTOR_ISAAC, 584, 252);
  Actor_WalkToAndWait(ACTOR_ISAAC, 616, 247);
  Actor_WalkToAndWait(ACTOR_ISAAC, 617, 312);
  Actor_Get(ACTOR_ISAAC)->motionFlags = ACTOR_FALLS;
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
  JupiterLighthouse_LowerElevator(DESCENT_ISAAC);
  FieldEvent_Wait(100);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  JupiterLighthouse_RaiseElevator(ASCENT_RISING);
  FieldEvent_Wait(60);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  FieldEvent_Wait(60);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_SHEBA, ANIM_NOD, 20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_Get(ACTOR_FELIX);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_PIERS, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_Get(ACTOR_FELIX);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_SHEBA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_Get(ACTOR_FELIX);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_Get(ACTOR_FELIX);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_SetScript(ACTOR_PIERS, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_SHEBA, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_JENNA, gJupiterLighthouse_JoinLeaderScript);
  Actor_SetScript(ACTOR_KRADEN, gJupiterLighthouse_JoinLeaderScript);
  FieldActor_WaitForScriptEndById(ACTOR_PIERS);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  FieldActor_WaitForScriptEndById(ACTOR_JENNA);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, 0);
  Actor_SetScript(ACTOR_FELIX, gJupiterLighthouse_WalkToPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_ALEX)->motionFlags = ACTOR_FALLS;
  Actor_SetScript(ACTOR_FELIX, gJupiterLighthouse_HopWestOverPlatformsScript);
  FieldActor_WaitForScriptEndById(ACTOR_FELIX);
  Actor_WalkToAndWait(ACTOR_FELIX, 311, 256);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  JupiterLighthouse_LowerElevator(DESCENT_FELIX);
  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 24;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(
      JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ENTRANCE);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Unused: writes to out the ids of the actors standing on the elevator, in
/// columns 4 to 8 and the three rows from 72 less the row of its scroll z, as
/// The Broken Seal's MercuryLighthouseAerie_FindLiftRiders finds its lift's.
UNUSED void JupiterLighthouse_FindElevatorRiders(s32 *out, s32 zArg) {
  s32 z = zArg;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 slot;

  z = 64 - FIELD_CELL(z);
  for (slot = 0; slot < FIELD_TREASURE_ACTOR; slot++) {
    struct Actor *actor = work->actors[slot];

    if (actor != NULL) {
      s32 column = FIELD_CELL(actor->x.fixed);
      s32 row = FIELD_CELL(actor->z.fixed);

      if (column >= 4 && column <= 8 && z + 8 <= row && row < z + 11)
        *out++ = slot;
    }
  }
}

/// Ends a list of the elevator's riders: the first id past the actors a scene
/// places.
#define ELEVATOR_RIDERS_END FIELD_TREASURE_ACTOR

extern s32 gJupiterLighthouse_FirstElevatorRiders[];
extern s32 gJupiterLighthouse_SecondElevatorRiders[];
extern s32 gJupiterLighthouse_ThirdElevatorRiders[];
extern s32 gJupiterLighthouse_FourthElevatorRiders[];
extern s32 gJupiterLighthouse_FloatingPlatforms[];

/// The frames the elevator takes to sink all the way, the frame of a ride at
/// which the camera sets off after it, and the frame at which a rise and every
/// descent but DESCENT_FELIX stop.
#define ELEVATOR_DOWN_FRAMES 228
#define ELEVATOR_CAMERA_FRAME 40
#define ELEVATOR_RIDE_FRAMES 120

/// The elevator, drawn on the map's overhead layer, sinks with the riders of
/// its descent, speeding up a tenth of a pixel every fourth frame.
void JupiterLighthouse_LowerElevator(s32 mode) {
  FIXED speed;
  u32 count;
  s32 *list;
  s32 *p;
  s32 *q;
  u32 i;
  u32 j;
  struct MapLayer *elevator;

  elevator = &((struct MapWork *)Ram_HeapSlots->mapGrid)
                  ->layers[MAPGRID_LAYER_OVERHEAD];
  speed = toFIXED(1) / 10;
  count = 0;
  gJupiterLighthouse_ElevatorLevel = elevator->offsetY;
  switch (mode) {
  case DESCENT_ALEX_AGATIO_KARST:
  case DESCENT_ALEX_AGATIO_KARST_FOLLOWING_ISAAC:
    list = gJupiterLighthouse_FirstElevatorRiders;
    break;
  case DESCENT_ISAAC:
    list = gJupiterLighthouse_SecondElevatorRiders;
    break;
  case DESCENT_FELIX:
    list = gJupiterLighthouse_ThirdElevatorRiders;
    break;
  default:
    list = gJupiterLighthouse_FourthElevatorRiders;
    break;
  }

  for (i = 0; i <= 4 && list[i] != ELEVATOR_RIDERS_END; i++) {
    Actor_Get(list[i])->motionFlags = 0;
    FieldActor_SetSpritePriority(list[i], 1);
    count++;
  }

  for (i = 0; i <= 5; i++) {
    Actor_Get(gJupiterLighthouse_FloatingPlatforms[i])->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    FieldActor_SetSpritePriority(gJupiterLighthouse_FloatingPlatforms[i], 0);
  }

  Event_PlaySound(SE_LIFT);

  for (i = 0; i < ELEVATOR_DOWN_FRAMES; i++) {
    elevator->offsetY -= speed;
    for (j = 0; j < count; j++) {
      struct Actor *actor;

      Actor_Get(list[j])->z.fixed += speed;
      actor = Actor_Get(list[j]);
      actor->targetZ = Actor_Get(list[j])->z.fixed;
    }
    if ((i & 3) == 3)
      speed += toFIXED(1) / 10;
    if (speed > toFIXED(3) / 2 - 1)
      speed = toFIXED(3) / 2;
    System_Wait(1);
    if (mode == DESCENT_ALEX_AGATIO_KARST) {
      if (i == ELEVATOR_CAMERA_FRAME)
        FieldCamera_MoveTo(512, 104, 304, 1);
      if (i == ELEVATOR_RIDE_FRAMES)
        break;
    } else if (mode == DESCENT_ISAAC) {
      if (i == ELEVATOR_CAMERA_FRAME)
        FieldCamera_MoveTo(512, 104, 352, 1);
      if (i == ELEVATOR_RIDE_FRAMES)
        break;
    } else if (mode == DESCENT_ALEX_AGATIO_KARST_FOLLOWING_ISAAC) {
      if (i == ELEVATOR_CAMERA_FRAME)
        EventCamera_FollowActor(ACTOR_ISAAC, 1);
      if (i == ELEVATOR_RIDE_FRAMES)
        break;
    }
  }

  for (i = 0; i <= 5; i++) {
    Actor_Get(gJupiterLighthouse_FloatingPlatforms[i])->priorityFlags &=
        ~ACTOR_PRIORITY_IGNORES_RISES;
    FieldActor_SetSpritePriority(gJupiterLighthouse_FloatingPlatforms[i], 1);
  }

  for (i = 0; i < count; i++) {
    Actor_SetPosition(list[i], 0, 0);
    Actor_Get(list[i])->motionFlags = 0;
  }
}

extern s32 gJupiterLighthouse_RisingElevatorRiders[];

/// The rising elevator keeps (ELEVATOR_DRAG - 1) / ELEVATOR_DRAG of its speed
/// every fourth frame once it is less than ELEVATOR_SLOWING_FRAMES frames'
/// travel from its level.
#define ELEVATOR_DRAG 16
#define ELEVATOR_SLOWING_FRAMES 30

/// The elevator rises back to where it was before it sank, slowing as it nears
/// it, or with ASCENT_AT_ONCE is only put back.
void JupiterLighthouse_RaiseElevator(s32 mode) {
  struct MapLayer *elevator = &((struct MapWork *)Ram_HeapSlots->mapGrid)
                                   ->layers[MAPGRID_LAYER_OVERHEAD];
  s32 speed = toFIXED(3) / 2;
  u32 count = 0;
  u32 i;
  s32 *ids = gJupiterLighthouse_RisingElevatorRiders;

  for (i = 0; i <= 4 && ids[i] != ELEVATOR_RIDERS_END; i++) {
    Actor_Get(ids[i])->motionFlags = 0;
    count++;
  }
  if (mode != ASCENT_AT_ONCE) {
    Event_PlaySound(SE_LIFT);
    i = 0;
    elevator->offsetY += toFIXED(3) / 2;
    while (elevator->offsetY < gJupiterLighthouse_ElevatorLevel) {
      u32 j;

      for (j = 0; j < count; j++) {
        struct Actor *actor;

        Actor_Get(ids[j])->z.fixed -= speed;
        actor = Actor_Get(ids[j]);
        actor->targetZ = Actor_Get(ids[j])->z.fixed;
      }
      if ((i & 3) == 3 &&
          gJupiterLighthouse_ElevatorLevel <
              speed * ELEVATOR_SLOWING_FRAMES + elevator->offsetY)
        speed = speed * (ELEVATOR_DRAG - 1) / ELEVATOR_DRAG;
      if (speed < toFIXED(1) / 20)
        speed = toFIXED(1) / 20;
      if (mode == ASCENT_RISING && i == ELEVATOR_CAMERA_FRAME)
        FieldCamera_MoveTo(288, 104, 368, 1);
      System_Wait(1);
      i++;
      if (i >= ELEVATOR_RIDE_FRAMES)
        break;
      elevator->offsetY += speed;
    }
  }
  elevator->offsetY = gJupiterLighthouse_ElevatorLevel;
  MapGrid_CenterOnFollowedPoint();
  System_Wait(2);
}

s32 Party_AddMember(s32 value);

/// Sets up the scene as the party enters: the floating platforms and the lid,
/// the beacon's light, and the event of the entrance taken (ENTRANCE_BEACON_LIT
/// to ENTRANCE_LAST_BATTLE_LOST).
s32 Scene_Initialize(void) {

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Call2(HangingVine_Place, ACTOR_WEST_CHAIN, 1);
  HangingVine_Place(ACTOR_EAST_CHAIN, 1);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_FLOATING_PLATFORM_A)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_B)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_C)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_D)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_E)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_F)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  if (!Event_GetFlag(FLAG_JUPITER_BEACON_LIT)) {
    JupiterLighthouse_PlaceRaisedActors();
  } else {
    Actor_Get(ACTOR_UNSEEN_LID)->scaleX = 0;
  }
  if (gMainRoutineWork.entrance !=
          JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A &&
      gMainRoutineWork.entrance !=
          JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B) {
    Actor_SetPosition(ACTOR_UNSEEN_LID, 0, 0);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_ORB), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_ORB), 0);
  Task_AddFrame(JupiterLighthouse_PulseBeacon, TASK_PASS_FRAME);
  if (gMainRoutineWork.entrance == ENTRANCE_BEACON_LIT) {
    JupiterLighthouse_BeaconIsLit();

    gMainRoutineWork.retreatScene = SCENE_JUPITER_LIGHTHOUSE_ENTRANCE;
    gMainRoutineWork.retreatEntrance =
        JUPITER_LIGHTHOUSE_ENTRANCE_FROM_WORLD_MAP;
  } else {
    if (gMainRoutineWork.entrance == ENTRANCE_AFTER_FIRST_BATTLE) {
      Party_AddMember(JENNA);
      JupiterLighthouse_JennaJoinsTheFight();
    } else {
      if (gMainRoutineWork.entrance == ENTRANCE_AFTER_SECOND_BATTLE) {
        Party_AddMember(SHEBA);
        JupiterLighthouse_ShebaJoinsTheFight();
      } else {
        if (gMainRoutineWork.entrance == ENTRANCE_LAST_BATTLE_WON) {
          JupiterLighthouse_WeBeatThem();
        } else {
          if (gMainRoutineWork.entrance == ENTRANCE_LAST_BATTLE_LOST) {
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

/// Sets the scene's arrival flags before the map is built. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame (gPaletteCycleFrame).
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

#include "shared/whirlwind_update.h"

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks_b.h"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

/// Takes `size` bytes from the heap for a picture's decompression.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

/// Sets the priority the lit actors' sprites are drawn at.
void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *gFieldEffect_DurationScripts[];
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

void CrackedTile_Update(void);

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

/// Who the elevator carries down, by JupiterLighthouse_LowerElevator's mode:
/// Alex, Agatio and Karst; Isaac; Felix; no one.
s32 gJupiterLighthouse_FirstElevatorRiders[] = {
    ACTOR_ALEX, ACTOR_AGATIO, ACTOR_KARST, ELEVATOR_RIDERS_END};
s32 gJupiterLighthouse_SecondElevatorRiders[] = {ACTOR_ISAAC,
                                                 ELEVATOR_RIDERS_END};
s32 gJupiterLighthouse_ThirdElevatorRiders[] = {ACTOR_FELIX,
                                                ELEVATOR_RIDERS_END};
s32 gJupiterLighthouse_FourthElevatorRiders[] = {ELEVATOR_RIDERS_END};
/// Who rides the elevator back up (JupiterLighthouse_RaiseElevator): no one.
s32 gJupiterLighthouse_RisingElevatorRiders[] = {ELEVATOR_RIDERS_END};

// clang-format off
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

/// Unused: turns its actor's facing back and forth, 9/128 of a turn at a time, and
/// ends. Nothing starts it.
LATE_DATA s32 gJupiterLighthouse_RockScript[] = {
    SC_WAIT(5),
    SC_ADD(SW_FACING, TRIG_TURN * 9 / 128),
    SC_WAIT(5),
    SC_ADD(SW_FACING, TRIG_TURN * 9 / 128),
    SC_WAIT(5),
    SC_ADD(SW_FACING, -TRIG_TURN * 9 / 128),
    SC_WAIT(2),
    SC_ADD(SW_FACING, -TRIG_TURN * 9 / 128),
    SC_WAIT(2),
    SC_ADD(SW_FACING, -TRIG_TURN * 9 / 128),
    SC_WAIT(5),
    SC_ADD(SW_FACING, -TRIG_TURN * 9 / 128),
    SC_WAIT(5),
    SC_ADD(SW_FACING, TRIG_TURN * 9 / 128),
    SC_WAIT(5),
    SC_ADD(SW_FACING, TRIG_TURN * 9 / 128),
    SC_END,
};

/// A party member walks into the leader it was sent to and is released there.
LATE_DATA s32 gJupiterLighthouse_JoinLeaderScript[] = {
    SC_SYNC,
    SC_RELEASE,
};

/// Walks west round the well to the western floating platforms.
LATE_DATA s32 gJupiterLighthouse_WalkToPlatformsScript[] = {
    SC_SPEED(1),
    SC_ACCELERATION(1),
    SC_ANIMATION(ANIM_WALK),
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
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Hops west over the three western floating platforms.
LATE_DATA s32 gJupiterLighthouse_HopWestOverPlatformsScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(3.5),
    SC_ACCELERATION(1),
    SC_MOVE(409, 32, 312),
    SC_SYNC,
    SC_WAIT(10),
    SC_MOVE(376, 32, 312),
    SC_SET(SW_MOTION, ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_CALL(JupiterLighthouse_JumpWithoutShadow),
    SC_SYNC,
    SC_CALL(JupiterLighthouse_ShowShadow),
    SC_WAIT(20),
    SC_MOVE(344, 32, 312),
    SC_SET(SW_MOTION, ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_WAIT(10),
    SC_CALL(JupiterLighthouse_JumpWithoutShadow),
    SC_SYNC,
    SC_CALL(JupiterLighthouse_ShowShadow),
    SC_WAIT(20),
    SC_MOVE(312, 32, 312),
    SC_SET(SW_MOTION, ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_CALL(JupiterLighthouse_JumpWithoutShadow),
    SC_SYNC,
    SC_CALL(JupiterLighthouse_ShowShadow),
    SC_WAIT(20),
    SC_MOVE(312, 32, 278),
    SC_SET(SW_MOTION, ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_CALL(JupiterLighthouse_JumpWithoutShadow),
    SC_SYNC,
    SC_CALL(JupiterLighthouse_ShowShadow),
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Hops east over the three eastern floating platforms.
LATE_DATA s32 gJupiterLighthouse_HopEastOverPlatformsScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(3.5),
    SC_ACCELERATION(1),
    SC_MOVE(616, 32, 312),
    SC_SYNC,
    SC_WAIT(10),
    SC_MOVE(648, 32, 312),
    SC_SET(SW_MOTION, ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_CALL(JupiterLighthouse_JumpWithoutShadow),
    SC_SYNC,
    SC_CALL(JupiterLighthouse_ShowShadow),
    SC_WAIT(20),
    SC_MOVE(681, 32, 312),
    SC_SET(SW_MOTION, ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_WAIT(10),
    SC_CALL(JupiterLighthouse_JumpWithoutShadow),
    SC_SYNC,
    SC_CALL(JupiterLighthouse_ShowShadow),
    SC_WAIT(20),
    SC_MOVE(712, 32, 312),
    SC_SET(SW_MOTION, ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_CALL(JupiterLighthouse_JumpWithoutShadow),
    SC_SYNC,
    SC_CALL(JupiterLighthouse_ShowShadow),
    SC_WAIT(20),
    SC_MOVE(712, 32, 264),
    SC_SET(SW_MOTION, ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_CALL(JupiterLighthouse_JumpWithoutShadow),
    SC_SYNC,
    SC_CALL(JupiterLighthouse_ShowShadow),
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// The column of light widens by a sixteenth of its picture's width a frame
/// for 32 frames. Past its end are the scripts that narrow the column and lift
/// it away, which nothing here runs.
LATE_DATA s32 gJupiterLighthouse_BeamOpenScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -7 * FIXED_ONE / 512),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -7 * FIXED_ONE / 512),
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

/// The lit beacon's orb breathes: narrower for four frames, wider for four,
/// again and again.
LATE_DATA s32 gJupiterLighthouse_BeaconRisesScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -3 * FIXED_ONE / 128),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 3 / 128),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A spark shed by the rising orb (JupiterLighthouse_RaiseBeaconLight): it
/// drops a pixel a frame for 16 frames and is released.
LATE_DATA s32 gJupiterLighthouse_BeaconSparkScript[] = {
    SC_SET(SW_SCALE_X, toFIXED(0.8)),
    SC_SET(SW_SCALE_Y, toFIXED(0.8)),
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseAerieBeacon),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ENTRANCE, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_AERIE_BEACON),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_WORLD_MAP, WorldMap, WORLD_MAP_JUPITER_BEACON),
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
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ANGEL_STATUE, FLAG_JUPITER_BEACON_LIT, MOVEMENT_TYPE_STAND, 472, 40, 256, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ANGEL_STATUE, FLAG_JUPITER_BEACON_LIT, MOVEMENT_TYPE_STAND, 552, 40, 256, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 880, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 312, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 344, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 376, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 680, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 712, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_BASIN, FLAG_JUPITER_BEACON_LIT, MOVEMENT_TYPE_CLEAR_CHILD, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BASIN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_CHILD, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(426, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(426, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AGATIO, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), MOVEMENT_TYPE_FACE_LEADER, 512, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), MOVEMENT_TYPE_FACE_LEADER, 480, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, IF_FLAG_SET(FLAG_JUPITER_BEACON_LIT), gJupiterLighthouse_BeaconRisesScript, 512, 8, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// A particle of the column of light (JupiterLighthouse_SpawnBeaconParticle):
/// it climbs a pixel a frame for 16 frames and is released. Past its end are
/// two more, which nothing here runs.
LATE_DATA s32 gJupiterLighthouse_BeaconParticleScript[] = {
    SC_SET(SW_SCALE_X, toFIXED(0.8)),
    SC_SET(SW_SCALE_Y, toFIXED(0.8)),
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
#if TLA_PATCH_1
    0,
#endif
    SC_SET(SW_SCALE_Y, toFIXED(1.4)),
    SC_SET(SW_SCALE_X, toFIXED(1.4)),
    SC_ANIMATION(4),
    SC_WAIT(30),
    SC_RELEASE,
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(8),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};
// clang-format on

#include "text/msg_ids.h"

void JupiterLighthouse_CastTheJupiterStar(void);

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B},
    {TRIGGER_STEP, CELL_BEFORE_KARST_AND_AGATIO,
     FLAG_KARST_AND_AGATIO_MET_FELIX,
     EVENT(JupiterLighthouse_KarstAndAgatioWait)},
    {TRIGGER_STEP, CELL_BEFORE_THE_WELL, FLAG_JUPITER_BEACON_LIT,
     EVENT(JupiterLighthouse_CastTheJupiterStar)},
    {TRIGGER_TALK, ACTOR_KARST, CONDITION_ALWAYS, MSGNO(MsgOnceYouVe)},
    {TRIGGER_TALK, ACTOR_AGATIO, CONDITION_ALWAYS, MSGNO(MsgOnceJupiterIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KARST, CONDITION_ALWAYS,
     MSGNO(MsgOnceJupiterIsLit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AGATIO, CONDITION_ALWAYS,
     MSGNO(MsgDoEvenThink)},
    {TRIGGER_TABLE_END},
};

/// The aerie's six floating platforms.
// clang-format off
LATE_DATA s32 gJupiterLighthouse_FloatingPlatforms[] = {
    ACTOR_FLOATING_PLATFORM_A, ACTOR_FLOATING_PLATFORM_B, ACTOR_FLOATING_PLATFORM_C,
    ACTOR_FLOATING_PLATFORM_D, ACTOR_FLOATING_PLATFORM_E, ACTOR_FLOATING_PLATFORM_F,
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA s32 gWhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
