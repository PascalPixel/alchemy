/// @file
/// Mars Lighthouse's aerie, at the top of the last lighthouse: the party faces
/// the Doom Dragon (MarsLighthouseAerie_FaceDoomDragon) and learns who the
/// dragon was (MarsLighthouseAerie_FindWhoTheDragonWas), the three it was speak
/// to the party as they lie dying (MarsLighthouseAerie_TalkToJennasMother and
/// the talks after it), and the beacon is lit with the Mars Star
/// (MarsLighthouseAerie_LightBeacon) before the Golden Sun rises
/// (MarsLighthouseAerie_WatchGoldenSunRise).
///
/// The party climbs to the beacon in the storm with all eight and Kraden; a
/// voice calls them betrayers and the Doom Dragon rises. Beaten, it is three
/// people, Isaac's father and Felix and Jenna's parents, too weak to live
/// long; Kraden holds the children back while the Mercury Adepts try to heal
/// them. Then the voice asks for the Mars Star, the beacon is lit, and the
/// party leaves.

#include "flags.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "item_ids.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "scene/mars_lighthouse_aerie.h"
#include "scene/scene.h"
#include "field/object.h"
#include "field/effect_sprite.h"
#include "scene_ids.h"
#include "field/event_commands.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "graphics/glyph.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "system/io_reg.h"
#include "field/field_sprite.h"
#include "system/ram_buffer.h"
#include "field/call.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "shared/tables.h"
#include "window/item_icon.h"
#include "version.h"
#include "system/dma.h"
#include "system/task.h"
#include "system/vector3.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "scene/sparkle.h"
#include "window/window.h"
#include "text/msg_ids.h"

/// The voice has asked for the Mars Star; stepping back from the beacon asks
/// again.
#define FLAG_ASKED_FOR_MARS_STAR FLAG_SCENE_TEMP_02
/// The beacon's light has broken through: its orbs rise every frame.
#define FLAG_BEACON_BROKE_THROUGH FLAG_SCENE_TEMP_00

/// The aerie's actors, by the rows of its three actor placement tables; the
/// party's rows go to their party slots. Ids 8 to 16 and Kraden's are the same
/// in all three.
enum {
  /// The beacon's fire and its glow: SPRITE_BLUE_ORB on the beacon's table,
  /// placed over the beacon once FLAG_MARS_BEACON_BURNING is set.
  ACTOR_BEACON_FIRE = 8,
  ACTOR_BEACON_GLOW,
  /// Seven platforms (SPRITE_FLOATING_PLATFORM) along the aerie's east edge,
  /// floating before the battle.
  ACTOR_FIRST_PLATFORM,
  ACTOR_LAST_PLATFORM = ACTOR_FIRST_PLATFORM + 6,
  /// Before the battle (gMarsLighthouseAerie_Placements) and after it: the
  /// Doom Dragon and the Wise One, the voice that calls the party betrayers.
  ACTOR_DOOM_DRAGON,
  ACTOR_WISE_ONE,
  ACTOR_KRADEN
};

/// The cell behaviours the aerie's map marks its spots with, which its
/// trigger table answers.
enum MarsLighthouseAerieCellBehavior {
  /// The landing the party reaches the aerie on, where Teleport takes them
  /// back down.
  CELL_TELEPORT_LANDING = 1,
  /// The spot before the unlit beacon, where the voice speaks before the
  /// battle.
  CELL_BEFORE_BEACON = 10,
  /// Where Kraden stops Felix, west and east of the three as they lie dying.
  CELL_KRADEN_STOPS_WEST = 20,
  CELL_KRADEN_STOPS_EAST = 21,
  /// At the beacon, where the voice asks for the Mars Star, and the ring of
  /// cells round it that let it ask again.
  CELL_AT_BEACON = 22,
  CELL_ROUND_BEACON = 23
};

/// The entrances the aerie's own events arrive by.
enum {
  ENTRANCE_DRAGON_BEATEN = 2, ///< after the Doom Dragon's battle
  /// Straight to the beacon's lighting; no exit leads here.
  ENTRANCE_LIGHT_BEACON = 4
};

/// The exit that takes the party, faded to black, from the fallen dragon to
/// the beacon (MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_AERIE).
#define EXIT_TO_THE_BEACON 2

/// After the battle (gMarsLighthouseAerie_AfterBattlePlacements): the party
/// but the leader stand in on their smaller sprites (SPRITE_JENNA_SMALL and
/// the rest), the dragon's three heads fall, and under each lies one of the
/// three it was.
enum {
  ACTOR_JENNA_STAND_IN = 20,
  ACTOR_SHEBA_STAND_IN,
  ACTOR_PIERS_STAND_IN,
  ACTOR_ISAAC_STAND_IN,
  ACTOR_GARET_STAND_IN,
  ACTOR_MIA_STAND_IN,
  ACTOR_IVAN_STAND_IN,
  ACTOR_DRAGON_HEAD_A,
  ACTOR_DRAGON_HEAD_B,
  ACTOR_DRAGON_HEAD_C,
  ACTOR_FREED_JENNAS_MOTHER,
  ACTOR_FREED_JENNAS_FATHER,
  ACTOR_FREED_KYLE
};

/// At the beacon (gMarsLighthouseAerie_BeaconPlacements), entered from
/// MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_AERIE on:
/// the three lie dying, and an unseen actor beside each answers for them
/// when talked to.
enum {
  ACTOR_JENNAS_MOTHER_TALK_SPOT = 17,
  /// Unseen: "Will you cast in the Mars Star?"
  ACTOR_VOICE,
  ACTOR_JENNAS_MOTHER = 20, ///< "I...can't see you, but I sense..."
  ACTOR_JENNAS_FATHER,      ///< "I am so happy...to have one last chance..."
  ACTOR_KYLE,               ///< "Tell Dora...to live her life..."
  /// The beacon's beam, in five segments (SPRITE_PIPE_NORTH_SOUTH) from the
  /// bottom up, as at Venus Lighthouse's aerie.
  ACTOR_BEAM_BOTTOM,
  ACTOR_BEAM_LOWER,
  ACTOR_BEAM_MIDDLE,
  ACTOR_BEAM_UPPER,
  ACTOR_BEAM_TOP,
  /// Hamma, who speaks to the party from afar as the Golden Sun rises; until
  /// then her actor stands unseen beside Ivan as his talk spot.
  ACTOR_HAMMA,
  ACTOR_JENNAS_FATHER_TALK_SPOT,
  ACTOR_KYLE_TALK_SPOT
};
struct Trigger;

u32 Random16(void);
void MarsLighthouseAerie_LookAround(struct Actor *actor);
void MarsLighthouseAerie_StartTalkWhileHalted(s32 id);
void MarsLighthouseAerie_EndTalkWhileHalted(s32 id);
void Teleport_StartShimmer(void);
void Teleport_StopShimmer(void);
void PaletteCycle_Fade(void);
void Teleport_PlaySparks(void);

extern u8 MsgUhhhhnnnICan[];
extern u8 MsgKoffKoffYou[];
extern u8 MsgUnnnnIAm[];
extern u8 MsgIUnderstandYour[];
extern struct ActorPlacement gMarsLighthouseAerie_Placements[];
extern struct ActorPlacement gMarsLighthouseAerie_AfterBattlePlacements[];
extern struct ActorPlacement gMarsLighthouseAerie_BeaconPlacements[];
u32 gMarsLighthouseAerie_BeaconFlameFrame BSS_DATA;
u32 gMarsLighthouseAerie_FlickerFrame BSS_DATA;
u32 gMarsLighthouseAerie_BeaconFireFrames BSS_DATA;
u32 gMarsLighthouseAerie_FlickerRate BSS_DATA;
struct SparkleState gMarsLighthouseAerie_RisingRingsB BSS_DATA ALIGNED(16);
struct SparkleState gMarsLighthouseAerie_RisingRingsA BSS_DATA ALIGNED(16);
u32 gMarsLighthouseAerie_BeaconFireFrame BSS_DATA;
/// The teleport circle's shimmer, a palette cycle that PaletteCycle_Fade
/// steps.
u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);

void MarsLighthouseAerie_HaltSpeaker(s32 spot, s32 speaker);
void MarsLighthouseAerie_StartRisingRings(struct Actor *actor,
                                          struct SparkleState *state);

extern u8 MsgMarsLighthouseAerieDoNotLetYourSadness[];

s32 List_CountEntries(u16 *list);

void Map_CopyPalette(s32 scene, s32 a1);
void FieldCamera_WaitForArrival();
void Party_RestoreHpPp();
void Weather_StartRain(s32 withCue);

/// Each frame while waiting in the storm: now and then the actor looks
/// another way.
void MarsLighthouseAerie_LookAround(struct Actor *actor) {
  u8 *timer = &actor->sceneCounter;

  if (*timer != 0) {
    (*timer)--;
  } else {
    *timer = (Random_Below(80)) + 60;
    actor->facing = Random16();
  }
}

/// The actor flickers: hidden for two frames, shown for two.
void MarsLighthouseAerie_Flicker(struct Actor *actor) {
  if ((gFrameCount >> 1) & 1)
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
  else
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
}

/// The actor swells now and then and shrinks back, as if breathing.
void MarsLighthouseAerie_Throb(struct Actor *actor) {
  u8 *timer;
  s32 scale = actor->scaleX;

  if (scale > FIXED_ONE) {
    actor->scaleX = scale - FIXED_ONE / 32;
    actor->scaleY -= FIXED_ONE / 64;
  }
  timer = &actor->sceneCounter;
  if (*timer != 0) {
    (*timer)--;
  } else {
    *timer = *timer + Random_Below(80) + 40;
    actor->scaleX = FIXED_ONE * 9 / 8;
    actor->scaleY = FIXED_ONE * 17 / 16;
  }
}

/// A spark of the beacon's fire circles it as it rises.
void MarsLighthouseAerie_OrbitBeaconFire(struct Actor *actor) {
  struct Actor *anchor = Actor_GetById(ACTOR_BEACON_FIRE);
  u16 *pangle = (u16 *)&actor->timer;
  s32 angle = *pangle;
  FIXED cosine;
  FIXED sine;
  FIXED along;
  FIXED across;
  u8 *counter;

  cosine = Trig_Cos(angle);
  counter = &actor->sceneCounter;
  along = anchor->x.fixed + cosine * (actor->speedLimit + *counter + 6);
  actor->x.fixed = along;
  sine = Math_Sin(angle);
  across = anchor->z.fixed + sine * (*counter + 4);
  actor->z.fixed = across;
  actor->targetX = actor->x.fixed;
  actor->targetZ = across;
  *pangle -= TRIG_TURN / 32;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.entrance == ENTRANCE_DRAGON_BEATEN)
    return gMarsLighthouseAerie_AfterBattlePlacements;
  // From the fallen dragon, for the beacon's lighting, or back from Mercury
  // Lighthouse: the three entrances that arrive at the beacon.
  if ((u16)(gMainRoutineState.entrance - 3) <= 2)
    return gMarsLighthouseAerie_BeaconPlacements;
  return gMarsLighthouseAerie_Placements;
}

/// Teleport takes the party back down to the inscriptions room, in a shimmer of
/// sparks.
void Scene_LeaveByExit(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineState.teleportState = 2;
  FieldEvent_SetExitRequest(MARS_LIGHTHOUSE_AERIE_TO_MARS_LIGHTHOUSE_HALL);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern u16 gTeleportCircle_PaletteSlots[];
extern u16 gTeleportCircle_Colors[];
extern u16 gTeleportCircle_ShimmerColors[];
extern u16 gTeleportCircle_ShimmerSlots[];

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);
void MarsLighthouseAerie_SpawnBeaconFlames(void);

/// The beacon's colours and its fire, burning once the beacon is lit.
void MarsLighthouseAerie_SetUpBeacon(void) {
  struct Actor *actor;
  s32 i;

  Palette_SetColorsFade(gTeleportCircle_PaletteSlots, gTeleportCircle_Colors,
                        gTeleportCircle_ShimmerColors,
                        gTeleportCircle_ShimmerSlots);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  for (i = ACTOR_FIRST_PLATFORM; i <= ACTOR_LAST_PLATFORM; i++) {
    actor = Actor_Get(i);
    actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_BRG);
  }
  if (!Event_GetFlag(FLAG_MARS_BEACON_BURNING))
    return;
  Actor_SetPosition(ACTOR_BEACON_FIRE, 512, 264);
  Actor_SetPosition(ACTOR_BEACON_GLOW, 512, 271);
  System_Wait(1);
  FieldActor_SetSpritePriority(ACTOR_BEACON_FIRE, 1);
  actor = Actor_Get(ACTOR_BEACON_FIRE);
  actor->motionFlags = ACTOR_FLOATS;
  actor->y.fixed = toFIXED(112);
  Actor_SetSpriteColorMode(actor, SPRITE_COLOR_BRG);
  Actor_SetSpriteFlags(actor, 0);
  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  FieldActor_SetSpritePriority(ACTOR_BEACON_GLOW, 2);
  actor = Actor_Get(ACTOR_BEACON_GLOW);
  actor->motionFlags = ACTOR_FLOATS;
  actor->y.fixed = toFIXED(112);
  actor->scaleX = FIXED_ONE * 13 / 10;
  actor->scaleY = FIXED_ONE * 13 / 10;
  Actor_SetSpriteColorMode(actor, SPRITE_COLOR_BRG);
  Actor_SetSpriteFlags(actor, 0);
  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
  gMarsLighthouseAerie_BeaconFlameFrame = 0;
  FieldTask_AddFrame(MarsLighthouseAerie_SpawnBeaconFlames, TASK_PASS_FRAME);
}

/// Sets the scene up for the entrance the party arrived by: the Golden Sun
/// rising, the beacon's lighting, the three at the beacon, or the fallen
/// dragon.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  MarsLighthouseAerie_SetUpBeacon();
  Event_SetFlag(FLAG_IN_MARS_LIGHTHOUSE);
  if (gMainRoutineState.entrance ==
      MARS_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EAST_B) {
    REG_BLDALPHA = BLEND_ALPHA(8, 12);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    MarsLighthouseAerie_WatchGoldenSunRise();
  } else if (gMainRoutineState.entrance == ENTRANCE_LIGHT_BEACON) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Actor_SetAnimation(ACTOR_JENNAS_MOTHER, 8);
    Actor_SetAnimation(ACTOR_JENNAS_FATHER, 8);
    Actor_SetAnimation(ACTOR_KYLE, 7);
    Actor_SetPosition(ACTOR_KRADEN, 498, 320);
    Actor_SetPosition(ACTOR_GARET, 538, 320);
    Actor_SetPosition(gMainRoutineState.leaderActorId, 506, 280);
    System_Wait(1);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    EventCamera_GetActor()->motionFlags = 0;
    FieldCamera_MoveTo(512, 96, 312, 1);
    MapGrid_CenterOnFollowedPoint();
    System_Wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_Wait(40);
    MarsLighthouseAerie_LightBeacon();
    Event_End();
  } else if (gMainRoutineState.entrance ==
             MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_AERIE) {
    Actor_SetAnimation(ACTOR_JENNAS_MOTHER, 8);
    Actor_SetAnimation(ACTOR_JENNAS_FATHER, 8);
    Actor_SetAnimation(ACTOR_KYLE, 7);
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT) == 0)
      MarsLighthouseAerie_UrgeOnToBeacon();
  } else if (gMainRoutineState.entrance == ENTRANCE_DRAGON_BEATEN) {
    MarsLighthouseAerie_FindWhoTheDragonWas();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineState.entrance ==
          MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_HALL &&
      Event_GetFlag(FLAG_MARS_BEACON_LIT) == 0)
    Weather_StartRain(1);
  Map_CopyPalette(gMainRoutineState.mapId + MARS_LIGHTHOUSE_THAWED_MAPS,
                  (s32)BG_PLTT);
  return 0;
}

extern s32 gMarsLighthouseAerie_BeaconFlameScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);
void MarsLighthouseAerie_SwingBeaconFlame(struct Actor *actor);

/// A flame swings round the beacon's fire BEACON_FLAME_SWING_RADIUS pixels out,
/// and up to BEACON_FLAME_SWING_SPREAD farther, by the height it rose from.
#define BEACON_FLAME_SWING_RADIUS 28
#define BEACON_FLAME_SWING_SPREAD 24

/// Every sixteenth frame a flame rises from the beacon's fire.
void MarsLighthouseAerie_SpawnBeaconFlames(void) {
  struct Actor *anchor = Actor_Get(ACTOR_BEACON_FIRE);
  FIXED offset = toFIXED(Random_Below(48));
  struct Actor *actor;
  struct MarsLighthouseAerieSprite *sprite;
  u32 phase;
  s32 angle;

  if (anchor != NULL &&
      (phase = gMarsLighthouseAerie_BeaconFlameFrame & 15) == 0) {
    actor = FieldActor_Create(SPRITE_PARTICLE, anchor->x.fixed + toFIXED(8),
                              anchor->y.fixed + offset + toFIXED(8),
                              anchor->z.fixed);
    offset = INTtoFIXED(offset / toFIXED(6));
    if (actor != NULL) {

      sprite = actor->sprite;
      Actor_StartScript(actor, gMarsLighthouseAerie_BeaconFlameScript);
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
      actor->motionFlags = phase;
      angle = Random16() & 0xffff000;
      actor->timer = angle;
      actor->userValue = phase;
      actor->update = MarsLighthouseAerie_SwingBeaconFlame;
      actor->watchedActor = anchor;
      actor->speedLimit = FIXEDtoINT(Math_Sin((offset & 0xfffff) >> 4) *
                                     BEACON_FLAME_SWING_SPREAD);
      /* FAKEMATCH(dead): the flags written through their own address. */
      *(u8 *)&sprite->flags = 0;
      sprite->priority =
          ((struct MarsLighthouseAerieSprite *)anchor->sprite)->priority;
    }
  }
  gMarsLighthouseAerie_BeaconFlameFrame++;
}

/// A flame swings round the beacon's fire, behind it and then in front.
void MarsLighthouseAerie_SwingBeaconFlame(struct Actor *actor) {
  u16 *pangle = (u16 *)&actor->timer;
  u16 angle = *pangle;
  struct Actor *parent = actor->watchedActor;

  actor->x.fixed =
      parent->x.fixed +
      Trig_Cos(angle) * (actor->speedLimit + BEACON_FLAME_SWING_RADIUS);
  actor->z.fixed = (Math_Sin(angle) << 4) + toFIXED(264);
  actor->targetX = actor->x.fixed;
  actor->targetZ = actor->z.fixed;
  if (actor->z.fixed < toFIXED(264))
    ((struct FieldSprite *)actor->sprite)->priority = 2;
  else
    ((struct FieldSprite *)actor->sprite)->priority = 1;
  *pangle -= TRIG_TURN / 128;
}

/// The actor stands looking about.
void MarsLighthouseAerie_StartLookingAround(s32 id) {
  struct Actor *actor = Actor_Get(id);

  FieldActor_Stop(id);
  actor->sceneCounter = (Random_Below(40)) + 20;
  actor->update = (ObjectUpdate)MarsLighthouseAerie_LookAround;
}

/// The actor stops looking about and faces the beacon.
void MarsLighthouseAerie_StopLookingAround(s32 id) {
  Actor_Get(id)->update = NULL;
  Actor_FaceDirection(id, FACING_NORTH, 0);
}

s32 Actor_ReleaseById();
s32 Actor_PlaceOnActor();
extern s32 gMarsLighthouseAerie_JennaApproachScript[];
extern s32 gMarsLighthouseAerie_ShebaApproachScript[];
extern s32 gMarsLighthouseAerie_PiersApproachScript[];
extern s32 gMarsLighthouseAerie_IvanApproachScript[];
extern s32 gMarsLighthouseAerie_KradenApproachScript[];
extern s32 gMarsLighthouseAerie_VoiceFadeScript[];
extern s32 gMarsLighthouseAerie_DoomDragonRiseScript[];
extern s32 gMarsLighthouseAerie_LeaderReadyScript[];
extern s32 gMarsLighthouseAerie_IsaacReadyScript[];
extern s32 gMarsLighthouseAerie_JennaReadyScript[];
extern s32 gMarsLighthouseAerie_ShebaReadyScript[];
extern s32 gMarsLighthouseAerie_PiersReadyScript[];
extern s32 gMarsLighthouseAerie_IvanReadyScript[];
extern s32 gMarsLighthouseAerie_MiaReadyScript[];
extern s32 gMarsLighthouseAerie_GaretReadyScript[];

/// The party walks up to the unlit beacon in the storm (cells (32, 20) to
/// (34, 20)), and the voice calls them betrayers before the Doom Dragon's
/// battle.
void MarsLighthouseAerie_FaceDoomDragon(void) {
  struct MainRoutineState *state;
  u16 *textColor;
  struct Actor *actor;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  textColor = (u16 *)&CENTERED_TEXT_COLOR;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_WISE_ONE), SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_WISE_ONE, 512, 276);
  System_Wait(1);
  FieldActor_SetSpritePriority(ACTOR_SHEBA, 1);
  FieldActor_SetSpritePriority(ACTOR_JENNA, 1);
  FieldActor_SetSpritePriority(ACTOR_KRADEN, 1);
  FieldActor_SetSpritePriority(ACTOR_IVAN, 1);
  FieldActor_SetSpritePriority(ACTOR_MIA, 1);
  Event_PlaySound(MUS_SILENCE);
  FieldEvent_SetMessageId((s32)MsgMarsLighthouseAerieBetrayersYouHaveArrived);
  // "Betrayers, you have arrived!"
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 304, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 504, 320);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_PlaceOnActor(ACTOR_ISAAC, gMainRoutineState.leaderActorId);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_ISAAC, 1.6);
  Actor_WalkToAndWait(ACTOR_ISAAC, 520, 320);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_PlaceOnActor(ACTOR_GARET, ACTOR_ISAAC);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_WalkToAndWait(ACTOR_GARET, 536, 320);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  // "Who said that!?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_PlaceOnActor(ACTOR_MIA, ACTOR_GARET);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_MIA, 1.6);
  Actor_WalkToAndWait(ACTOR_MIA, 544, 328);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_NORTHWEST);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "The wind's too strong! I can't see anything!"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldActor_Stop(gMainRoutineState.leaderActorId);
  actor->sceneCounter = (Random_Below(20)) + 20;
  actor->update = (ObjectUpdate)MarsLighthouseAerie_LookAround;
  MarsLighthouseAerie_StartLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StartLookingAround(ACTOR_MIA);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_IVAN, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_SetSpeed(ACTOR_IVAN, 1.6);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetScript(ACTOR_JENNA, gMarsLighthouseAerie_JennaApproachScript);
  Actor_SetScript(ACTOR_SHEBA, gMarsLighthouseAerie_ShebaApproachScript);
  Actor_SetScript(ACTOR_PIERS, gMarsLighthouseAerie_PiersApproachScript);
  Actor_SetScript(ACTOR_KRADEN, gMarsLighthouseAerie_KradenApproachScript);
  Actor_RunScriptAndWait(ACTOR_IVAN, gMarsLighthouseAerie_IvanApproachScript);
  FieldEvent_Wait(10);
  Actor_ReleaseById(ACTOR_LAST_PLATFORM);
  Actor_ReleaseById(ACTOR_FIRST_PLATFORM + 5);
  Actor_ReleaseById(ACTOR_FIRST_PLATFORM + 4);
  Actor_ReleaseById(ACTOR_FIRST_PLATFORM + 3);
  Actor_ReleaseById(ACTOR_FIRST_PLATFORM + 2);
  Actor_ReleaseById(ACTOR_FIRST_PLATFORM + 1);
  Actor_ReleaseById(ACTOR_FIRST_PLATFORM);
  MarsLighthouseAerie_StartLookingAround(ACTOR_JENNA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_SHEBA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_PIERS);
  MarsLighthouseAerie_StartLookingAround(ACTOR_IVAN);
  MarsLighthouseAerie_StartLookingAround(ACTOR_KRADEN);
  FieldEvent_Wait(80);
  Actor_Get(ACTOR_SHEBA)->update = NULL;
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  // "I heard it too, but I don't see anyone else up here!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_Get(ACTOR_JENNA)->update = NULL;
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
  FieldActor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_Get(ACTOR_PIERS)->update = NULL;
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  FieldActor_Jump(ACTOR_PIERS, 4, 10);
  // "Felix! Quick! Cast the Mars Star in before anything has a chance to..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_Get(gMainRoutineState.leaderActorId)->update = NULL;
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_WEST);
  FieldActor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  MarsLighthouseAerie_StopLookingAround(ACTOR_ISAAC);
  MarsLighthouseAerie_StopLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StopLookingAround(ACTOR_KRADEN);
  MarsLighthouseAerie_StopLookingAround(ACTOR_IVAN);
  MarsLighthouseAerie_StopLookingAround(ACTOR_MIA);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(40);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 504, 292);
  Event_PlaySound(SE_WAVE);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(gMainRoutineState.leaderActorId, 2);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.2);
  Actor_MoveToAndWait(gMainRoutineState.leaderActorId, 504, 308);
  Actor_RunRepeatedMotion(gMainRoutineState.leaderActorId, 3);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_MoveToAndWait(gMainRoutineState.leaderActorId, 504, 320);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  *textColor = RGB_WHITE;
  System_Wait(1);
  // "So, you are still intent on lighting the beacon of Mars Lighthouse?"
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 40);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  Actor_WalkToAndWait(ACTOR_ISAAC, 520, 308);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  // "Who said that!?"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0, 20);
  // "Have you forgotten me so soon, Isaac? And you, Garet?"
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
  // "It seems to know you, Isaac... Do you have any idea what it is?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
  FieldActor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_PlaySound(MUS_CONFRONTATION);
  // "Then search your heart, boy!"
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTH);
  MarsLighthouseAerie_StartLookingAround(gMainRoutineState.leaderActorId);
  MarsLighthouseAerie_StartLookingAround(ACTOR_JENNA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_SHEBA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_PIERS);
  MarsLighthouseAerie_StartLookingAround(ACTOR_ISAAC);
  MarsLighthouseAerie_StartLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StartLookingAround(ACTOR_MIA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_IVAN);
  MarsLighthouseAerie_StartLookingAround(ACTOR_KRADEN);
  FieldEvent_Wait(40);
  MarsLighthouseAerie_StopLookingAround(ACTOR_KRADEN);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldActor_Jump(ACTOR_KRADEN, 4, 10);
  FieldActor_Jump(ACTOR_KRADEN, 6, 10);
  // "That voice..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  MarsLighthouseAerie_StopLookingAround(gMainRoutineState.leaderActorId);
  MarsLighthouseAerie_StopLookingAround(ACTOR_JENNA);
  MarsLighthouseAerie_StopLookingAround(ACTOR_SHEBA);
  MarsLighthouseAerie_StopLookingAround(ACTOR_PIERS);
  MarsLighthouseAerie_StopLookingAround(ACTOR_ISAAC);
  MarsLighthouseAerie_StopLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StopLookingAround(ACTOR_MIA);
  MarsLighthouseAerie_StopLookingAround(ACTOR_IVAN);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_WISE_ONE), SPRITE_COLOR_NORMAL);
  actor = Actor_Get(ACTOR_WISE_ONE);
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(ACTOR_WISE_ONE), toFIXED(512),
                                       toFIXED(120), toFIXED(208));
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 216, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  // "It's...the Wise One!"
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 304, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_ISAAC,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "Isaac, since when are you on a first-name basis with the Wise One?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
  // "It must have been... Isaac, what happened in Sol Sanctum after we..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  // "When Saturos and Menardi stole the Elemental Stars, they also..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "But...wait, Mt. Aleph did erupt! There was a huge explosion! We saw..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
  // "But it would have erupted with us still inside... There was no way..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  // "The Wise One held off the eruption until we could escape... He even..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC,
                    0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  // "I can't believe it... Nobody has enough power to do that..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHEAST);
  // "I know it sounds weird, but I was there, and it happened, so you'll..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_GARET,
                    0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  // "If it can do all that, this Wise One seems more like a god than an..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 296, 1);
  Actor_SetSpeed(ACTOR_WISE_ONE, 0.8);
  FieldActor_SetMoveTarget(actor, 512, 96, 276);
  FieldActor_WaitForMoveById(ACTOR_WISE_ONE);
  FieldEvent_Wait(10);
  // "I did not just save you. I also tasked you with recovering the four..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_RunRepeatedMotion(ACTOR_ISAAC, 1);
  Actor_WalkToAndWait(ACTOR_ISAAC, 520, 320);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 9);
  FieldEvent_Wait(20);
  // "Why have you disobeyed my command? Why have you come to light the..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  // "Because Prox will be destroyed if we don't! We can't let that happen..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 1);
  FieldEvent_Wait(10);
  // "Prox? They have brought this disaster upon themselves."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Are you saying we should just abandon them to die? What did they do..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 9);
  FieldEvent_Wait(20);
  // "The people of Prox have committed an unforgivable sin. They must pay..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  // "For lighting the lighthouses? Is that their sin? Does that warrant..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  // "If we don't light the beacon, Gaia Falls will eventually erode away..."
  Event_ShowMessage(ACTOR_MIA, 0);
  // "We have fought for so long to save all the people of our world, and..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldActor_Jump(ACTOR_SHEBA, 4, 10);
  // "Gaia Falls is growing. It's consuming more and more with each..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "How can you just allow the world to crumble into nothingness?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 11);
  FieldEvent_Wait(20);
  // "The seal needs to be broken! The world will be destroyed if it's not!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 12);
  Actor_WaitForAnimationChange(ACTOR_WISE_ONE);
  FieldEvent_Wait(20);
  // "You have learned far too much."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Wise One! You can't continue to protect the lighthouses! You know..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 10);
  FieldEvent_Wait(20);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 1);
  FieldEvent_Wait(10);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 10);
  FieldEvent_Wait(5);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 1);
  FieldEvent_Wait(20);
  // "It is your duty to protect all of Weyard! If Weyard is destroyed..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "Why won't you answer us, Wise One?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 9);
  FieldEvent_Wait(20);
  // "If Alchemy is unleashed, mankind may well destroy all of Weyard itself."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "But we can combine our strengths, ensure that Alchemy not be used..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 10);
  FieldEvent_Wait(20);
  // "It is inevitable. In time, one man will seek to rule over all. It is..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 9);
  FieldEvent_Wait(20);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 3);
  FieldEvent_Wait(10);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 6);
  FieldEvent_Wait(10);
  MarsLighthouseAerie_StartRisingRings(actor,
                                       &gMarsLighthouseAerie_RisingRingsA);
  FieldEvent_Wait(80);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 3);
  FieldEvent_Wait(10);
  // "And it shall come sooner than any of you think."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Why do you say that?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 3);
  FieldEvent_Wait(20);
  // "The Water Adept who climbs toward the peak of Mt. Aleph even as we..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 20);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 6);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_SWIRL);
  PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
  PaletteFade_Interpolate(40);
  FieldEvent_Wait(40);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_Interpolate(40);
  FieldEvent_Wait(40);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 3);
  FieldEvent_Wait(20);
  // "Alex is his name. Surely, you have not forgotten him?"
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  // "Alex!? What would he be doing on Mt. Aleph?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 4);
  FieldEvent_Wait(20);
  // "He understands far more than you do. He knows that when the four..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "But what would he gain from being there?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(20);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 13);
  FieldEvent_Wait(20);
  // "When the final beam of light reaches the peak of Mt. Aleph, the..."
  FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 20);
  // "The Golden Sun!? What is that? And what would Alex want with it?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 8);
  FieldEvent_Wait(180);
  // "When the four beams merge into one, they form a golden light..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  // "Is... Is that Alchemy? I mean, pure Alchemy made real, at the heart..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 1);
  FieldEvent_Wait(5);
  Actor_SetAnimation(ACTOR_WISE_ONE, 9);
  Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "And it's that light that gives shape to the Stone of Sages?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  MarsLighthouseAerie_EndRisingRings(actor);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 1);
  FieldEvent_Wait(20);
  // "This has been Alex's one true desire from the very start."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  FieldActor_Jump(ACTOR_PIERS, 4, 10);
  // "Alex planned all of this? Then he must have been after this power..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 10);
  FieldEvent_Wait(20);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 11);
  FieldEvent_Wait(5);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 1);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHEAST);
  // "We've been duped! He used us all! Oh, you'd better believe he's not..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_GARET,
                    0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "Alex... How could he do this? He's... He's one of my own people! I..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
  // "None of that matters right now. We still have to light the beacon...."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
  // "If we don't do it now, Prox will be destroyed! Felix... We have to..."
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                             0)) {
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
    FieldActor_Jump(ACTOR_SHEBA, 4, 10);
    // "You bet we do! The Wise One will have to deal with Alex on his own."
    Event_ShowMessage(ACTOR_SHEBA, 0);
    gFieldState->message++;
  } else {
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    FieldActor_Jump(ACTOR_SHEBA, 4, 10);
    gFieldState->message++;
    // "Felix! We don't have any choice! The Wise One will have to deal with..."
    Event_ShowMessage(ACTOR_SHEBA, 0);
  }
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 11);
  FieldEvent_Wait(20);
  // "I cannot interfere in the actions of mankind."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
  // "If you can't interfere, then how about getting out of our way so we..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimation(ACTOR_WISE_ONE, 8);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_WaitForAnimationChange(ACTOR_WISE_ONE);
  Actor_SetAnimation(ACTOR_WISE_ONE, 1);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "Ooo... Good one, Jenna!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
  // "I don't like doing exactly what Alex wants, but it's looking like..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
  // "Don't worry, Felix! Just throw the Mars Star into the beacon's well! Now!"
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                             0)) {
    Actor_WaitForAnimationChange(gMainRoutineState.leaderActorId);
    gFieldState->message++;
  } else {
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    // "The Wise One himself said he's not allowed to interfere! You're all..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  }
  state = &gMainRoutineState;
  Actor_FaceDirectionAndWait(state->leaderActorId, FACING_EAST);
  FieldActor_SetAnimationAndWait(state->leaderActorId, 3);
  Actor_FaceDirectionAndWait(state->leaderActorId, FACING_NORTH);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_SetSpeed(state->leaderActorId, 0.4);
  Actor_WalkToAndWait(state->leaderActorId, 504, 308);
  Event_PlaySound(SE_ASCEND);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 14);
  FieldEvent_Wait(10);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_BLINK);
  Actor_RunRepeatedMotion(state->leaderActorId, 2);
  Actor_SetSpeed(state->leaderActorId, 1.6);
  FieldActor_Jump(state->leaderActorId, 4, 0);
  Actor_Get(state->leaderActorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(state->leaderActorId, 504, 320);
  FieldEvent_Wait(1);
  Actor_Get(state->leaderActorId)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "Wise One! Didn't you just say that you aren't allowed to interfere..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 14);
  FieldEvent_Wait(10);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 10);
  FieldEvent_Wait(20);
  // "That is correct. I cannot stop you."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 9);
  FieldEvent_Wait(20);
  // "But...what if some miracle were to occur, one that prevented you from..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(state->leaderActorId, FACING_WEST, 40);
  // "Miracle? What are you talking about? What kind of miracle?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Event_PlaySound(SE_CRUMBLE);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_WISE_ONE), SPRITE_COLOR_WHITE);
  FieldEvent_Wait(20);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_WISE_ONE), SPRITE_COLOR_NORMAL);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(state->leaderActorId, FACING_NORTH, 20);
  // "If you can defeat a miracle, only then can you ignite the beacon's flame."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  // "The Wise One is up to something! Be wary, everyone! We don't know what..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  FieldActor_SetAnimation(state->leaderActorId, 3);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  FieldActor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  actor->update = (ObjectUpdate)MarsLighthouseAerie_Flicker;
  Actor_SetScript(ACTOR_WISE_ONE, gMarsLighthouseAerie_VoiceFadeScript);
  PaletteFade_ApplyTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
  PaletteFade_Interpolate(20);
  System_Wait(20);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_Interpolate(40);
  System_Wait(40);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetPosition(state->leaderActorId, 0, 0);
  Actor_SetPosition(ACTOR_ISAAC, 0, 0);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  System_Wait(1);
  actor = Actor_Get(ACTOR_DOOM_DRAGON);
  actor->scaleX = FIXED_ONE / 16;
  actor->scaleY = FIXED_ONE / 16;
  Actor_SetPosition(ACTOR_DOOM_DRAGON, 512, 280);
  System_Wait(1);
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(actor, toFIXED(512), toFIXED(48),
                                       toFIXED(280));
  System_Wait(1);
  actor->motionFlags = 0;
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_Interpolate(20);
  Event_PlaySound(MUS_ALEX);
  Actor_RunScriptAndWait(ACTOR_DOOM_DRAGON,
                         gMarsLighthouseAerie_DoomDragonRiseScript);
  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(40);
  Actor_SetSpeedAndAcceleration(ACTOR_DOOM_DRAGON, 3.2, 0.2);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  FieldActor_SetMoveTarget(actor, 544, 88, 280);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(10);
  FieldActor_WaitForMoveById(ACTOR_DOOM_DRAGON);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  FieldActor_SetMoveTarget(actor, 464, 88, 280);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(10);
  FieldActor_WaitForMoveById(ACTOR_DOOM_DRAGON);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  FieldActor_SetMoveTarget(actor, 576, 88, 280);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(10);
  FieldActor_WaitForMoveById(ACTOR_DOOM_DRAGON);
  Actor_SetSpeed(ACTOR_DOOM_DRAGON, 6.4);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  FieldActor_SetMoveTarget(actor, 508, 208, 288);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(10);
  FieldActor_WaitForMoveById(ACTOR_DOOM_DRAGON);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  REG_DISPCNT |= DISPCNT_BG1_ON | DISPCNT_BG2_ON;
  Actor_SetPosition(ACTOR_PIERS, 488, 320);
  Actor_SetPosition(state->leaderActorId, 504, 320);
  Actor_SetPosition(ACTOR_ISAAC, 520, 320);
  Actor_SetPosition(ACTOR_GARET, 536, 320);
  Actor_SetPosition(ACTOR_SHEBA, 480, 328);
  Actor_SetPosition(ACTOR_JENNA, 496, 328);
  Actor_SetPosition(ACTOR_KRADEN, 512, 328);
  Actor_SetPosition(ACTOR_IVAN, 528, 328);
  Actor_SetPosition(ACTOR_MIA, 544, 328);
  System_Wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_Interpolate(40);
  System_Wait(40);
  actor->gravity = toFIXED(1) / 4;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(30);
  Event_PlaySound(SE_BANE_ESCAPE);
  MapGrid_SetShake(2, 4, 1);
  FieldEvent_Wait(10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_DOOM_DRAGON, 1);
  Event_PlaySound(SE_BOSS_ROAR);
  FieldEvent_Wait(80);
  // "A three-headed dragon? That's your miracle?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "So you would have us fight for our future? Fine, then fight we shall!"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "What's he thinking? We already beat a two-headed dragon. How much..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  // "I don't care how many heads it has. Nothing's going to stop us now!"
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
  // "Let's do it! For Prox! For the future of Weyard!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_SetSpeed(state->leaderActorId, 1.6);
  Actor_SetSpeed(ACTOR_ISAAC, 1.6);
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_IVAN, 1.6);
  Actor_SetSpeed(ACTOR_MIA, 1.6);
  Actor_SetScript(ACTOR_PIERS, gMarsLighthouseAerie_PiersReadyScript);
  Actor_SetScript(state->leaderActorId, gMarsLighthouseAerie_LeaderReadyScript);
  Actor_SetScript(ACTOR_ISAAC, gMarsLighthouseAerie_IsaacReadyScript);
  Actor_SetScript(ACTOR_SHEBA, gMarsLighthouseAerie_ShebaReadyScript);
  Actor_SetScript(ACTOR_JENNA, gMarsLighthouseAerie_JennaReadyScript);
  Actor_SetScript(ACTOR_IVAN, gMarsLighthouseAerie_IvanReadyScript);
  Actor_SetScript(ACTOR_MIA, gMarsLighthouseAerie_MiaReadyScript);
  Actor_RunScriptAndWait(ACTOR_GARET, gMarsLighthouseAerie_GaretReadyScript);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Wait a second... Wasn't that two-headed dragon actually... So that..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHWEST);
  // "Felix! NO! You mustn't fight that dragon! It's°"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHWEST);
  FieldActor_Jump(ACTOR_GARET, 6, 10);
  // "Too late, Kraden! We can't get away from it now!"
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldActor_Jump(ACTOR_KRADEN, 2, 10);
  FieldActor_Jump(ACTOR_KRADEN, 4, 10);
  // "Felix, don't! Stop!!!"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  state->battleType = BATTLE_TYPE_NO_ESCAPE;
  FieldEvent_SetAfterBattleDestination(SCENE_MARS_LIGHTHOUSE_AERIE,
                                       ENTRANCE_DRAGON_BEATEN);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 1);
  return;
}

/// The Doom Dragon's parts flicker white, ever faster.
void MarsLighthouseAerie_FlickerDragon(struct Actor *actor) {
  if ((gMarsLighthouseAerie_FlickerFrame >> gMarsLighthouseAerie_FlickerRate) &
      1)
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
  else
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  gMarsLighthouseAerie_FlickerFrame++;
}

/// The three it was flicker white out of step with the dragon.
void MarsLighthouseAerie_FlickerParent(struct Actor *actor) {
  if ((gMarsLighthouseAerie_FlickerFrame >> gMarsLighthouseAerie_FlickerRate) &
      1)
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  else
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
}

extern s32 gMarsLighthouseAerie_ShebaToThosePeopleScript[];
extern s32 gMarsLighthouseAerie_JennaToThosePeopleScript[];
extern s32 gMarsLighthouseAerie_JennaTurnScript[];
extern s32 gMarsLighthouseAerie_LeaderTurnScript[];
extern s32 gMarsLighthouseAerie_IsaacTurnScript[];
extern s32 gMarsLighthouseAerie_GaretTurnScript[];
extern s32 gMarsLighthouseAerie_MiaTurnScript[];

/// The Doom Dragon falls, and the party finds who it was: Felix and Jenna's
/// parents and Isaac's father, Kyle, freed.
void MarsLighthouseAerie_FindWhoTheDragonWas(void) {
  s32 facing;
  struct Actor *actor, *mother, *father, *kyle;
  s32 leaderId;

  actor = Actor_Get(ACTOR_DOOM_DRAGON);
  mother = Actor_Get(ACTOR_FREED_JENNAS_MOTHER);
  father = Actor_Get(ACTOR_FREED_JENNAS_FATHER);
  kyle = Actor_Get(ACTOR_FREED_KYLE);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetAnimation(ACTOR_DOOM_DRAGON, 2);
  actor->motionFlags = 0;
  actor->x.fixed -= toFIXED(4);
  actor->z.fixed += toFIXED(16);
  Actor_SetAnimation(ACTOR_DRAGON_HEAD_B, 2);
  Actor_SetAnimation(ACTOR_DRAGON_HEAD_C, 3);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  Actor_SetAnimation(ACTOR_FREED_JENNAS_MOTHER, 7);
  Actor_SetAnimation(ACTOR_FREED_JENNAS_FATHER, 7);
  Actor_SetAnimation(ACTOR_FREED_KYLE, 6);
  leaderId = gMainRoutineState.leaderActorId;
  facing = FACING_NORTH;
  Actor_SetPositionAndFacing(leaderId, toFIXED(504), toFIXED(320), facing);
  System_Wait(1);
  PaletteFade_ApplySource(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_Interpolate(1);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_MOTHER)),
                            SPRITE_COLOR_HIDDEN);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_FATHER)),
                            SPRITE_COLOR_HIDDEN);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_KYLE)), SPRITE_COLOR_HIDDEN);
  System_Wait(1);
  FieldActor_SetSpritePriority(ACTOR_SHEBA_STAND_IN, 1);
  FieldActor_SetSpritePriority(ACTOR_JENNA_STAND_IN, 1);
  FieldActor_SetSpritePriority(ACTOR_KRADEN, 1);
  FieldActor_SetSpritePriority(ACTOR_IVAN_STAND_IN, 1);
  FieldActor_SetSpritePriority(ACTOR_MIA_STAND_IN, 1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gFieldState->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  FieldEvent_Wait(80);
  gMarsLighthouseAerie_FlickerFrame = 0;
  gMarsLighthouseAerie_FlickerRate = 4;
  Actor_Get(ACTOR_DOOM_DRAGON)->update =
      (ObjectUpdate)MarsLighthouseAerie_FlickerDragon;
  Actor_Get(ACTOR_DRAGON_HEAD_A)->update =
      (ObjectUpdate)MarsLighthouseAerie_FlickerDragon;
  Actor_Get(ACTOR_DRAGON_HEAD_B)->update =
      (ObjectUpdate)MarsLighthouseAerie_FlickerDragon;
  Actor_Get(ACTOR_DRAGON_HEAD_C)->update =
      (ObjectUpdate)MarsLighthouseAerie_FlickerDragon;
  mother->update = (ObjectUpdate)MarsLighthouseAerie_FlickerParent;
  father->update = (ObjectUpdate)MarsLighthouseAerie_FlickerParent;
  kyle->update = (ObjectUpdate)MarsLighthouseAerie_FlickerParent;
  Event_PlaySound(SE_DRAGON_FLICKER);
  System_Wait(10);
  Event_PlaySound(SE_DRAGON_FLICKER);
  System_Wait(10);
  Event_PlaySound(SE_DRAGON_FLICKER);
  System_Wait(10);
  Event_PlaySound(SE_DRAGON_FLICKER);
  System_Wait(10);
  Event_PlaySound(SE_DRAGON_FLICKER);
  System_Wait(40);
  gMarsLighthouseAerie_FlickerRate = 3;
  System_Wait(30);
  gMarsLighthouseAerie_FlickerRate = 2;
  System_Wait(20);
  Actor_SetPosition(ACTOR_DOOM_DRAGON, 0, 0);
  Actor_SetPosition(ACTOR_DRAGON_HEAD_A, 0, 0);
  Actor_SetPosition(ACTOR_DRAGON_HEAD_B, 0, 0);
  Actor_SetPosition(ACTOR_DRAGON_HEAD_C, 0, 0);
  Actor_Get(ACTOR_DOOM_DRAGON)->update = NULL;
  Actor_Get(ACTOR_DRAGON_HEAD_A)->update = NULL;
  Actor_Get(ACTOR_DRAGON_HEAD_B)->update = NULL;
  Actor_Get(ACTOR_DRAGON_HEAD_C)->update = NULL;
  mother->update = NULL;
  father->update = NULL;
  kyle->update = NULL;
  System_Wait(1);
  Event_PlaySound(SE_CURSE);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_MOTHER)),
                            SPRITE_COLOR_BLUE_TINT);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_FATHER)),
                            SPRITE_COLOR_BLUE_TINT);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_KYLE)),
                            SPRITE_COLOR_BLUE_TINT);
  FieldEvent_Wait(20);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_MOTHER)),
                            SPRITE_COLOR_NORMAL);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_FATHER)),
                            SPRITE_COLOR_NORMAL);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_KYLE)), SPRITE_COLOR_NORMAL);
  FieldEvent_Wait(20);
  mother->sceneCounter = (Random_Below(80)) + 40;
  father->sceneCounter = (Random_Below(80)) + 40;
  kyle->sceneCounter = (Random_Below(80)) + 40;
  mother->update = (ObjectUpdate)MarsLighthouseAerie_Throb;
  father->update = (ObjectUpdate)MarsLighthouseAerie_Throb;
  kyle->update = (ObjectUpdate)MarsLighthouseAerie_Throb;
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  FieldEvent_SetMessageId((s32)MsgMarsLighthouseAerieWhoAreThey);
  Actor_FaceDirectionAndWait(ACTOR_IVAN_STAND_IN, FACING_WEST);
  // "...Who are they?"
  Event_ShowMessage(ACTOR_IVAN_STAND_IN, 0);
  // "That's what I was trying to warn you about!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS_STAND_IN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC_STAND_IN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET_STAND_IN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA_STAND_IN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA_STAND_IN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN_STAND_IN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA_STAND_IN, FACING_SOUTH, 0);
  FieldCamera_SetSpeed(1.6);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(512, 80, 340, 1);
  FieldCamera_WaitForArrival();
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "It wasn't just the dragon you killed..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_PlaySound(MUS_THE_WISE_ONES_TRAP);
  Actor_SetAttachedEffect(ACTOR_GARET_STAND_IN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "I remember you warning us, but it was too late for us to stop..."
  Event_ShowMessage(ACTOR_GARET_STAND_IN, 0);
  // "Kraden, what do you know? Why were you trying to stop us from..."
  Event_ShowMessage(ACTOR_MIA_STAND_IN, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "It was the Wise One's final trap..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_IVAN_STAND_IN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Trap? What do you mean?"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN_STAND_IN, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "The Wise One knew he couldn't stop you, so he played a cruel..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_ISAAC_STAND_IN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_SetSpeed(ACTOR_ISAAC_STAND_IN, 1.6);
  Actor_WalkToAndWait(ACTOR_ISAAC_STAND_IN, 512, 328);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC_STAND_IN, FACING_SOUTHEAST);
  // "Kraden, I don't understand... What are you trying to tell us?"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC_STAND_IN, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA_STAND_IN, FACING_WEST);
  // "Sheba, let's see who those people are while they try to sort this out."
  Event_ShowMessage(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_JENNA_STAND_IN, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldActor_SetSpritePriority(ACTOR_SHEBA_STAND_IN, 2);
  Actor_Get(ACTOR_JENNA_STAND_IN)->sceneCounter = 0;
  Actor_Get(ACTOR_SHEBA_STAND_IN)->sceneCounter = 0;
  Actor_SetSpeed(ACTOR_SHEBA_STAND_IN, 0.8);
  Actor_SetSpeed(ACTOR_JENNA_STAND_IN, 0.8);
  Actor_SetScript(ACTOR_SHEBA_STAND_IN,
                  gMarsLighthouseAerie_ShebaToThosePeopleScript);
  Actor_SetScript(ACTOR_JENNA_STAND_IN,
                  gMarsLighthouseAerie_JennaToThosePeopleScript);
  Actor_SetSpeed(ACTOR_KRADEN, 2.4);
  Actor_WalkToAndWait(ACTOR_KRADEN, 540, 340);
  FieldActor_Jump(ACTOR_KRADEN, 4, 10);
  // "No, Jenna! Don't look! You mustn't look! It will only bring you pain..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  actor = Actor_Get(ACTOR_SHEBA_STAND_IN);
  do {
    System_Wait(1);
  } while (actor->sceneCounter == 0);
  Actor_FaceDirection(ACTOR_JENNA_STAND_IN, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_SOUTHEAST);
  // "What are you talking about, Kraden? They can't hurt us anymore..."
  Event_ShowMessage(ACTOR_SHEBA_STAND_IN, 0);
  FieldActor_SetSpritePriority(ACTOR_JENNA_STAND_IN, 2);
  FieldActor_SetSpritePriority(ACTOR_PIERS_STAND_IN, 2);
  FieldActor_ShowEmote(ACTOR_JENNA_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       0);
  Actor_SetSpeed(ACTOR_JENNA_STAND_IN, 1.6);
  Actor_WalkToAndWait(ACTOR_JENNA_STAND_IN, 490, 296);
  Actor_FaceDirection(ACTOR_SHEBA_STAND_IN, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_JENNA_STAND_IN, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_JENNA_STAND_IN, FACING_NORTHEAST, 20);
  Actor_SetSpeed(ACTOR_JENNA_STAND_IN, 0.2);
  Actor_Get(ACTOR_JENNA_STAND_IN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JENNA_STAND_IN, 488, 300);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_JENNA_STAND_IN)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_JENNA_STAND_IN, 2);
  Actor_Get(ACTOR_JENNA_STAND_IN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JENNA_STAND_IN, 486, 308);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_JENNA_STAND_IN)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(512, 96, 320, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_SHEBA_STAND_IN, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA_STAND_IN, FACING_NORTHEAST, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_EAST);
  // "What's the matter, Jenna?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SHEBA_STAND_IN, 0);
  Actor_FaceDirection(ACTOR_PIERS_STAND_IN, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC_STAND_IN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN_STAND_IN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA_STAND_IN, FACING_NORTHWEST, 0);
  // "It...can't be... How? How..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA_STAND_IN, 0);
  actor = Actor_Get(ACTOR_FREED_KYLE);
  actor->update = NULL;
  System_Wait(1);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  Actor_RunRepeatedMotion(ACTOR_FREED_KYLE, 2);
  // "Nnn... Uhnnn..."
  Event_ShowMessage(ACTOR_FREED_KYLE, 0);
  Actor_ShowEmote(ACTOR_ISAAC_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_ISAAC_STAND_IN, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_MIA_STAND_IN, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_MIA_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "What is it, Isaac?"
  Event_ShowMessage(ACTOR_MIA_STAND_IN, 0);
  Actor_FaceDirection(ACTOR_ISAAC_STAND_IN, FACING_EAST, 40);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC_STAND_IN, FACING_NORTH);
  FieldActor_StartRepeatedMotion(ACTOR_FREED_KYLE, 3);
  // "Uhhh...unnnn..."
  Event_ShowMessage(ACTOR_FREED_KYLE, 0);
  Actor_FaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_GARET_STAND_IN, FACING_SOUTHWEST);
  // "Isaac! I... I know that guy! That's your dad!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET_STAND_IN, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_SOUTHEAST);
  Actor_SetAttachedEffect(ACTOR_SHEBA_STAND_IN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Then...does that mean... the other two are..."
  Event_ShowMessage(ACTOR_SHEBA_STAND_IN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN_STAND_IN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA_STAND_IN, FACING_NORTH, 0);
  actor = Actor_Get(ACTOR_FREED_JENNAS_MOTHER);
  actor->update = NULL;
  System_Wait(1);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  actor = Actor_Get(ACTOR_FREED_JENNAS_FATHER);
  actor->update = NULL;
  System_Wait(1);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FREED_JENNAS_FATHER, 1);
  // "Hhnnnng..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_FREED_JENNAS_FATHER, 0);
  Actor_RunRepeatedMotion(ACTOR_FREED_JENNAS_MOTHER, 2);
  // "Hhhnnn..."
  Event_ShowMessage(ACTOR_FREED_JENNAS_MOTHER, 0);
  FieldActor_Jump(ACTOR_JENNA_STAND_IN, 2, 10);
  FieldActor_Jump(ACTOR_JENNA_STAND_IN, 4, 10);
  // "Mom... Dad..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA_STAND_IN, 0);
  Actor_SetSpeed(ACTOR_JENNA_STAND_IN, 2.4);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 2.4);
  Actor_SetSpeed(ACTOR_ISAAC_STAND_IN, 2.4);
  Actor_SetSpeed(ACTOR_GARET_STAND_IN, 2.4);
  Actor_SetScript(ACTOR_JENNA_STAND_IN, gMarsLighthouseAerie_JennaTurnScript);
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gMarsLighthouseAerie_LeaderTurnScript);
  Actor_SetScript(ACTOR_ISAAC_STAND_IN, gMarsLighthouseAerie_IsaacTurnScript);
  Actor_RunScriptAndWait(ACTOR_GARET_STAND_IN,
                         gMarsLighthouseAerie_GaretTurnScript);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_PIERS_STAND_IN, 1.6);
  Actor_WalkToAndWait(ACTOR_PIERS_STAND_IN, 528, 320);
  Actor_FaceDirection(ACTOR_PIERS_STAND_IN, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MIA_STAND_IN, FACING_NORTHWEST);
  // "I'll heal Jenna's parents! Quickly, Mia! Tend to Isaac's father!"
  Event_ShowMessage(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_STAND_IN, 0);
  Actor_SetSpeed(ACTOR_MIA_STAND_IN, 1.6);
  Actor_SetScript(ACTOR_MIA_STAND_IN, gMarsLighthouseAerie_MiaTurnScript);
  Actor_WalkToAndWait(ACTOR_PIERS_STAND_IN, 486, 308);
  Actor_FaceDirectionAndWait(ACTOR_PIERS_STAND_IN, FACING_NORTHWEST);
  Actor_SetSpeed(ACTOR_SHEBA_STAND_IN, 2.4);
  Actor_Get(ACTOR_SHEBA_STAND_IN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SHEBA_STAND_IN, 474, 316);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_SHEBA_STAND_IN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_SHEBA_STAND_IN, FACING_NORTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_PIERS_STAND_IN, 462, 280);
  Actor_FaceDirectionAndWait(ACTOR_PIERS_STAND_IN, FACING_EAST);
  FieldActor_SetSpritePriority(ACTOR_PIERS_STAND_IN, 1);
  MarsLighthouseAerie_StartRisingRings(Actor_Get(ACTOR_PIERS_STAND_IN),
                                       &gMarsLighthouseAerie_RisingRingsA);
  MarsLighthouseAerie_StartRisingRings(Actor_Get(ACTOR_MIA_STAND_IN),
                                       &gMarsLighthouseAerie_RisingRingsB);
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "If only... If only I'd realized it sooner..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  MarsLighthouseAerie_EndRisingRings(Actor_Get(ACTOR_PIERS_STAND_IN));
  MarsLighthouseAerie_EndRisingRings(Actor_Get(ACTOR_MIA_STAND_IN));
  FieldEvent_Wait(80);
  FieldEvent_SetExitRequest(EXIT_TO_THE_BEACON);
  return;
}

/// The actor stands where the party will pass, no longer in the way.
void MarsLighthouseAerie_StandAside(s32 id) {
  struct Actor *actor = Actor_Get(id);

  actor->collisionFlags = 0;
  actor->motionFlags = 0;
  FieldActor_SetSpritePriority(id, 1);
}

/// At the beacon, before it is lit: Piers and Mia can heal the three no
/// longer, Kraden tells Jenna they spent their last strength as the dragon,
/// the party turns on the Wise One, and Kraden urges Felix to light the
/// beacon.
void MarsLighthouseAerie_UrgeOnToBeacon(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 506, 280);
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldActor_SetSpritePriority(ACTOR_MIA, 1);
  MarsLighthouseAerie_StartRisingRings(Actor_Get(ACTOR_PIERS),
                                       &gMarsLighthouseAerie_RisingRingsA);
  MarsLighthouseAerie_StartRisingRings(Actor_Get(ACTOR_MIA),
                                       &gMarsLighthouseAerie_RisingRingsB);
  FieldEvent_Wait(120);
  MarsLighthouseAerie_EndRisingRings(Actor_Get(ACTOR_PIERS));
  MarsLighthouseAerie_EndRisingRings(Actor_Get(ACTOR_MIA));
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
  FieldEvent_SetMessageId(MsgMarsLighthouseAerieWhatsMatterPiersWhyDid);
  // "What's the matter, Piers? Why did you stop? Jenna's parents need you..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
  // "Don't give up, Mia. You can't! You have to save them..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "It's no use, Ivan... I'm tapped..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_MIA, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "I am, too... And even if I weren't, it's just too late..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  FieldActor_Jump(ACTOR_JENNA, 2, 10);
  FieldActor_Jump(ACTOR_JENNA, 4, 10);
  // "What are you saying!?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_WEST);
  // "They're not... They can't be..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "That's not what I'm saying, Jenna... I..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "I finally found them... I was going to be with them again... For the..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 40);
  // "Jenna..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "Please... It can't be..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetAnimation(ACTOR_JENNA, 5);
  Actor_MoveToAndWait(ACTOR_JENNA, 460, 288);
  Actor_SetAnimation(ACTOR_JENNA, 22);
  // "Mom... Wake up! It's me... It's Jenna! ...Don't leave me..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldCamera_SetSpeed(0.2);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 320, 1);
  FieldCamera_WaitForArrival();
  // "Jenna, you must prepare yourself for what comes next..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Being transformed into a dragon, fighting in that form... This..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 20);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
  // "What are you saying, Kraden?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "In fighting you, your parents were forced to use every last ounce of..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 20);
  // "They don't have the strength to..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Even if they had won the battle, they would not have survived. You..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 40);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 542, 328);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldCamera_SetSpeed(0.8);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(540, 76, 340, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
  Actor_WalkToAndWait(ACTOR_GARET, 542, 340);
  FieldActor_Jump(ACTOR_GARET, 2, 10);
  FieldActor_Jump(ACTOR_GARET, 4, 10);
  // "You monster!!!"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_GARET,
                    0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  FieldActor_Jump(ACTOR_GARET, 4, 10);
  // "Why did you do this? Why did you make us fight Jenna's parents?"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_GARET,
                    0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(478, 96, 312, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  FieldActor_Jump(ACTOR_SHEBA, 4, 10);
  // "You're no god! You're no protector! You're evil!"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA,
                    0);
  FieldCamera_MoveTo(480, CAMERA_KEEP, 276, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  // "You don't understand the pain you have caused, Wise One."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "You have no idea the damage done to a child who learns she has..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
  // "That's enough..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC,
                    0);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 312, 1);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  // "I knew what I was doing the moment I raised my sword."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0, 20);
  FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "We defied the Wise One in order to save the world. Our parents would..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
  // "Don't you think so, Felix?"
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    // "You're right. It hurts, but it's true. We didn't do this for..."
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
    gFieldState->message += 1;
  } else {
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    gFieldState->message += 1;
    // "I agree with my brother... But it does no one any good if we don't..."
    Event_ShowMessage(ACTOR_JENNA, 0);
  }
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_WalkToAndWait(ACTOR_GARET, 548, 320);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTHWEST);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  // "We still have a chance to save Prox."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "Perhaps we can't save your parents, but we can save countless others."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "Kyle and the others saved them, too... They sacrificed their lives so..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  // "I never imagined that my actions would help to save the world..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "Even though lighting the beacon may create wars and strife, I regret..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 498, 320);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "There's little time left, Felix... Use the Mars Star and light the..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_SOUTH);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  Event_SetFlag(FLAG_MARS_BEACON_LIT);
  Event_PlaySound(MUS_MARS_LIGHTHOUSE);
  Actor_SetSpeed(ACTOR_ISAAC, 0.8);
  Actor_WalkToAndWait(ACTOR_ISAAC, 540, 286);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
  MarsLighthouseAerie_StandAside(ACTOR_PIERS);
  MarsLighthouseAerie_StandAside(ACTOR_JENNA);
  MarsLighthouseAerie_StandAside(ACTOR_SHEBA);
  MarsLighthouseAerie_StandAside(ACTOR_KRADEN);
  MarsLighthouseAerie_StandAside(ACTOR_ISAAC);
  MarsLighthouseAerie_StandAside(ACTOR_MIA);
  MarsLighthouseAerie_StandAside(ACTOR_IVAN);
  MarsLighthouseAerie_StandAside(ACTOR_GARET);
  FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
  Event_SetFlag(FLAG_ASKED_FOR_MARS_STAR);
  MapGrid_CopyCellAttributes(39, 40, 18, 16, 23, 10);
  Party_RestoreHpPp(1);
  Party_RemoveMemberRestored(7);
  Party_RemoveMemberRestored(5);
  Party_RemoveMemberRestored(6);
  Party_RemoveMemberRestored(0);
  Party_RemoveMemberRestored(3);
  Party_RemoveMemberRestored(2);
  Party_RemoveMemberRestored(1);
  Actor_Get(ACTOR_JENNAS_MOTHER)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_Get(ACTOR_JENNAS_FATHER)->collisionFlags |= ACTOR_NOT_TARGETED;
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_HAMMA), SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_HAMMA, 568, 274);
  Actor_Get(ACTOR_JENNAS_MOTHER_TALK_SPOT)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(ACTOR_JENNAS_MOTHER_TALK_SPOT),
                                       toFIXED(492), toFIXED(96), toFIXED(284));
  Actor_Get(ACTOR_JENNAS_FATHER_TALK_SPOT)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(ACTOR_JENNAS_FATHER_TALK_SPOT),
                                       toFIXED(510), toFIXED(96), toFIXED(295));
  Actor_Get(ACTOR_KYLE_TALK_SPOT)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(ACTOR_KYLE_TALK_SPOT),
                                       toFIXED(540), toFIXED(96), toFIXED(300));
  System_Wait(1);
  Event_SetFlag(FLAG_CANNOT_SAVE_HERE);
  Event_SetFlag(FLAG_RETREAT_BLOCKED);
  Event_End();
}

/// An orb circles the beacon's beam as it rises.
void Orb_UpdateRising(struct Actor *self) {
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
  self->timer += -(TRIG_TURN / 32);
}

/// Every third frame, or every frame once the light has broken through, an orb
/// starts to rise round the beacon's beam.
void Orb_SpawnRising(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(FLAG_BEACON_BROKE_THROUGH) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(ACTOR_BEAM_BOTTOM);
  if (Event_GetFlag(FLAG_BEACON_BROKE_THROUGH) != 0) {
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
    Actor_StartScript(effect, gOrb_RisingScript);
    FieldActor_SetSpriteColor(effect, SPRITE_COLOR_GRB);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->timer = angle;
    effect->userValue = 0;
    effect->update = (ObjectUpdate)Orb_UpdateRising;
    effect->speedLimit = Math_Sin((u32)(Random16() * 65535) >> 20) * 24 >> 16;
    sprite->flags = 0;
    sprite->priority = 2;
  }
}

#include "../field/actor_stop_where_it_stands.c"

void FieldCamera_WaitForArrival();
void Teleport_PlayWhirl(void);
void Actor_Release(struct Actor *actor);

extern s32 gMarsLighthouseAerie_BeaconFireScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);

/// The beacon's fire: a flash and a quake, then it falls from above into the
/// beacon with glittering sparks.
void MarsLighthouseAerie_RaiseBeaconFire(void) {
  struct Actor *anchor = Actor_GetById(ACTOR_BEACON_FIRE);
  s32 spawn = 0;
  struct Actor *actor;
  struct MarsLighthouseAerieSprite *sprite;
  s32 angle;
  u8 phase;

  switch (gMarsLighthouseAerie_BeaconFireFrame) {
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
    anchor->x.fixed = toFIXED(512);
    anchor->y.fixed = -toFIXED(360);
    anchor->z.fixed = toFIXED(204);
    anchor->scaleX = FIXED_ONE;
    anchor->scaleY = FIXED_ONE;
    Actor_StopWhereItStands(anchor);
    Actor_SetScript(ACTOR_BEACON_FIRE, gMarsLighthouseAerie_BeaconFireScript);
    break;
  case 25:
    gMarsLighthouseAerie_BeaconFireFrame--;
    if (anchor->y.fixed > 0) {
      PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12),
                              PALETTES_ALL);
      PaletteFade_Interpolate(16);
      gMarsLighthouseAerie_BeaconFireFrame++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(SE_GLITTER);
      spawn = 1;
      anchor->y.fixed += toFIXED(9) / 4;
    }
    break;
  case 26:
    gMarsLighthouseAerie_BeaconFireFrame--;
    if (anchor->y.fixed > toFIXED(20)) {
      gMarsLighthouseAerie_BeaconFireFrame = 999;
      FieldTask_Remove(MarsLighthouseAerie_RaiseBeaconFire);
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(SE_GLITTER);
      anchor->y.fixed += toFIXED(9) / 4;
      spawn = 1;
    }
    break;
  }
  if (spawn) {
    actor = FieldActor_Create(SPRITE_PARTICLE, anchor->x.fixed,
                              anchor->y.fixed - ((Random_Below(80)) << 16) -
                                  toFIXED(8),
                              anchor->z.fixed);
    if (actor != NULL) {
      sprite = actor->sprite;
      Actor_StartScript(actor, gMarsLighthouseAerie_BeaconSparkScript);
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_GRB);
      actor->motionFlags = 0;
      angle = Random16() & 0xffff000;
      actor->timer = angle;
      actor->userValue = 0;
      phase = Random16() >> 13;
      actor->sceneCounter = phase;
      actor->update = MarsLighthouseAerie_OrbitBeaconFire;
      actor->speedLimit = Math_Sin(Random16() * 0xffff >> 20) * 24;
      actor->speedLimit = FIXEDtoINT(anchor->speedLimit);
      sprite->flags = 0;
      ((struct FieldSprite *)sprite)->priority = 1;
    }
  }
  if (++gMarsLighthouseAerie_BeaconFireFrame >
      gMarsLighthouseAerie_BeaconFireFrames)
    FieldTask_Remove(MarsLighthouseAerie_RaiseBeaconFire);
}

/// The actor steps back from the beam, to the left.
void MarsLighthouseAerie_StepBackLeft(s32 id) {
  struct Actor *actor = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(actor, actor->x.fixed - toFIXED(16),
                                       actor->y.fixed,
                                       actor->z.fixed + toFIXED(32));
}

/// The actor steps back from the beam, to the right.
void MarsLighthouseAerie_StepBackRight(s32 id) {
  struct Actor *actor = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(actor, actor->x.fixed + toFIXED(16),
                                       actor->y.fixed,
                                       actor->z.fixed + toFIXED(32));
}

/// The actor keeps to the ground as the beacon's beam rises.
void MarsLighthouseAerie_HoldStill(s32 id) {
  Actor_GetById(id)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
}

void MarsLighthouseAerie_StartLookingAround(s32 id);
void MarsLighthouseAerie_StopLookingAround(s32 id);
extern s32 gMarsLighthouseAerie_BeamNarrowScript[];

/// The Mars Star is cast into the beacon: the beam rises, the fire falls
/// into it and the beacon is lit.
void MarsLighthouseAerie_LightBeacon(void) {
  struct FieldState *later;
  struct MarsLighthouseAerieSprite *sprite;
  struct Actor *star;
  struct Actor *anchor;
  IconTransfer *icon;
  struct Actor *bottomBeam, *lowerBeam, *middleBeam;
  struct Actor *upperBeam, *topBeam;
  struct Actor *actor;

  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  Actor_SetPosition(ACTOR_HAMMA, 0, 0);
  Actor_SetPosition(ACTOR_JENNAS_FATHER_TALK_SPOT, 0, 0);
  Actor_SetPosition(ACTOR_KYLE_TALK_SPOT, 0, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(512, 96, 312, 1);
  FieldCamera_WaitForArrival();
  System_Wait(1);
  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  MapGrid_CopyCellAttributes(18, 40, 18, 16, 23, 10);
  MarsLighthouseAerie_HoldStill(ACTOR_PIERS);
  MarsLighthouseAerie_HoldStill(ACTOR_JENNA);
  MarsLighthouseAerie_HoldStill(ACTOR_SHEBA);
  MarsLighthouseAerie_HoldStill(ACTOR_KRADEN);
  MarsLighthouseAerie_HoldStill(ACTOR_ISAAC);
  MarsLighthouseAerie_HoldStill(ACTOR_MIA);
  MarsLighthouseAerie_HoldStill(ACTOR_IVAN);
  MarsLighthouseAerie_HoldStill(ACTOR_GARET);
  FieldEvent_AddPartyMember(0, 0);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 512, 280);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  Actor_RunRepeatedMotion(gMainRoutineState.leaderActorId, 2);
  FieldEvent_Wait(20);
  PartyInventory_Remove(ITEM_MYTHRIL_BAG_MARS_STAR);
  anchor = Actor_Get(ACTOR_FELIX);
  star = FieldActor_Create(SPRITE_UNSEEN_MARKER, anchor->x.fixed,
                           anchor->y.fixed + toFIXED(8), anchor->z.fixed);
  if (star != NULL) {
    sprite = (struct MarsLighthouseAerieSprite *)star->sprite;
    sprite->fullColor = 0;
    sprite->layerCount = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    star->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    star->motionFlags = 0;
    star->retainSpriteCache = 1;
    star->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteFlags(star, 0);
    star->speedLimit = toFIXED(2) / 5;
    star->acceleration = toFIXED(1) / 20;
    star->gravity = toFIXED(1) / 2;
    icon = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
    Item_LoadIcon(ITEM_MARS_STAR);
    Vram_Load(sprite->vramBlock, ICON_TILE_BYTES, icon->tiles);
    HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
    star->motionFlags = 0;
    FieldActor_SetMoveTarget(star, 512, 128, 280);
    FieldActor_WaitForMove(star);
    star->motionFlags = ACTOR_FLOATS;
    FieldEvent_Wait(60);
    star->motionFlags = 0;
    FieldActor_SetMoveTarget(star, 512, 128, 264);
    FieldActor_WaitForMove(star);
    FieldEvent_Wait(20);
    FieldActor_SetMoveTarget(star, 512, 80, 252);
    FieldActor_WaitForMove(star);
    FieldEvent_Wait(20);
    Actor_Release(star);
  }
  FieldEvent_Wait(40);
  FieldActor_SetSpritePriority(ACTOR_BEAM_BOTTOM, 2);
  FieldActor_SetSpritePriority(ACTOR_BEAM_LOWER, 2);
  FieldActor_SetSpritePriority(ACTOR_BEAM_MIDDLE, 2);
  FieldActor_SetSpritePriority(ACTOR_BEAM_UPPER, 2);
  FieldActor_SetSpritePriority(ACTOR_BEAM_TOP, 2);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(1, 1, 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_BOTTOM), SPRITE_COLOR_WHITE);
  bottomBeam = Actor_Get(ACTOR_BEAM_BOTTOM);
  bottomBeam->scaleX = FIXED_ONE / 10;
  bottomBeam->scaleY = -FIXED_ONE;
  bottomBeam->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(bottomBeam, toFIXED(512), toFIXED(64),
                                       toFIXED(204));
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_LOWER), SPRITE_COLOR_WHITE);
  lowerBeam = Actor_Get(ACTOR_BEAM_LOWER);
  lowerBeam->scaleX = FIXED_ONE / 10;
  lowerBeam->scaleY = -FIXED_ONE;
  lowerBeam->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(lowerBeam, toFIXED(512), toFIXED(96),
                                       toFIXED(204));
  Task_AddFrame(Orb_SpawnRising, TASK_PASS_FRAME);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 80);
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gMarsLighthouseAerie_BeamWidenScript);
  Actor_SetScript(ACTOR_BEAM_LOWER, gMarsLighthouseAerie_BeamWidenScript);
  Event_PlaySound(SE_CRASH);
  MapGrid_SetShake(6, 6, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_Interpolate(16);
  System_Wait(20);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(24);
  System_Wait(60);
  Event_PlaySound(SE_RUMBLE);
  Event_SetFlag(FLAG_BEACON_BROKE_THROUGH);
  Actor_Get(ACTOR_BEAM_BOTTOM)->y.fixed = -toFIXED(56);
  Actor_Get(ACTOR_BEAM_LOWER)->y.fixed = -toFIXED(24);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_MIDDLE), SPRITE_COLOR_WHITE);
  middleBeam = Actor_Get(ACTOR_BEAM_MIDDLE);
  middleBeam->scaleY = -FIXED_ONE;
  middleBeam->scaleX = (Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX);
  middleBeam->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(middleBeam, toFIXED(512), toFIXED(8),
                                       toFIXED(204));
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_UPPER), SPRITE_COLOR_WHITE);
  upperBeam = Actor_Get(ACTOR_BEAM_UPPER);
  upperBeam->scaleY = -FIXED_ONE;
  upperBeam->scaleX = (Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX);
  upperBeam->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(upperBeam, toFIXED(512), toFIXED(40),
                                       toFIXED(204));
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_TOP), SPRITE_COLOR_WHITE);
  topBeam = Actor_Get(ACTOR_BEAM_TOP);
  topBeam->scaleY = -FIXED_ONE;
  topBeam->scaleX = (Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX);
  topBeam->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(topBeam, toFIXED(512), toFIXED(72),
                                       toFIXED(204));
  MapGrid_CopyCellsTo(106, 36, 87, 7, 17, 23);
  MapGrid_CopyCellsTo(42, 98, 23, 69, 17, 23);
  MapGrid_CopyCellAttributes(0, 40, 18, 20, 23, 10);
  MarsLighthouseAerie_StepBackLeft(ACTOR_PIERS);
  MarsLighthouseAerie_StepBackLeft(ACTOR_JENNA);
  MarsLighthouseAerie_StepBackLeft(ACTOR_SHEBA);
  MarsLighthouseAerie_StepBackLeft(ACTOR_JENNAS_MOTHER);
  MarsLighthouseAerie_StepBackLeft(ACTOR_JENNAS_FATHER);
  MarsLighthouseAerie_StepBackLeft(ACTOR_KRADEN);
  MarsLighthouseAerie_StepBackRight(ACTOR_IVAN);
  MarsLighthouseAerie_StepBackRight(ACTOR_GARET);
  MarsLighthouseAerie_StepBackRight(ACTOR_KYLE);
  MarsLighthouseAerie_StepBackRight(ACTOR_ISAAC);
  MarsLighthouseAerie_StepBackRight(ACTOR_MIA);
  Actor_SetPositionAndFacing(gMainRoutineState.leaderActorId, toFIXED(484),
                             toFIXED(312), FACING_NORTHEAST);
  FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
  System_Wait(1);
  MapGrid_SetShake(4, 4, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gMarsLighthouseAerie_BeamNarrowScript);
  Actor_SetScript(ACTOR_BEAM_LOWER, gMarsLighthouseAerie_BeamNarrowScript);
  Actor_SetScript(ACTOR_BEAM_MIDDLE, gMarsLighthouseAerie_BeamNarrowScript);
  Actor_SetScript(ACTOR_BEAM_UPPER, gMarsLighthouseAerie_BeamNarrowScript);
  Actor_SetScript(ACTOR_BEAM_TOP, gMarsLighthouseAerie_BeamNarrowScript);
  System_Wait(120);
  MapGrid_SetShake(3, 3, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  MapGrid_SetShake(1, 1, 1);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_Get(ACTOR_BEAM_BOTTOM)->scaleY = FIXED_ONE / 50;
  Actor_SetScript(ACTOR_BEAM_LOWER, gMarsLighthouseAerie_BeamShootUpScript);
  Actor_SetScript(ACTOR_BEAM_MIDDLE, gMarsLighthouseAerie_BeamShootUpScript);
  Actor_SetScript(ACTOR_BEAM_UPPER, gMarsLighthouseAerie_BeamShootUpScript);
  Actor_RunScriptAndWait(ACTOR_BEAM_TOP,
                         gMarsLighthouseAerie_BeamShootUpScript);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_BOTTOM), SPRITE_COLOR_HIDDEN);
  FieldEvent_Wait(20);
  Actor_RunScriptAndWait(ACTOR_BEAM_BOTTOM,
                         gMarsLighthouseAerie_BeamRiseScript);
  FieldTask_Remove(Orb_SpawnRising);
  actor = Actor_Get(ACTOR_BEACON_FIRE);
  actor->motionFlags = 0;
  Actor_SetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEACON_FIRE), SPRITE_COLOR_WHITE);
  FieldActor_SetSpritePriority(ACTOR_BEACON_FIRE, 2);
  gMarsLighthouseAerie_BeaconFireFrame = 0;
  gMarsLighthouseAerie_BeaconFireFrames = 240;
  Task_AddFrame(MarsLighthouseAerie_RaiseBeaconFire, TASK_PASS_FRAME);
  do {
    System_Wait(1);
  } while (gMarsLighthouseAerie_BeaconFireFrame <= 100);
  Event_PlaySound(MUS_MERCURY_LIGHTHOUSE);
  FieldEvent_Wait(120);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  FieldEvent_SetMessageId((s32)MsgMarsLighthouseAerieBeaconIsLit);
  // "The beacon is lit!"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "And to think, I joined this quest hoping to prevent exactly this..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  // "And I began this quest as a prisoner, taken against my will."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "And if that tidal wave hadn't sent me far off course, I wouldn't..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  // "How many lives have been taken and changed forever just to light..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  // "Mom... Dad... Weyard is safe now."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0, 10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "You're right! I hear voices, too!"
  Message_ShowCentered(((s32)MsgMarsLighthouseAerieBeaconIsLit + 6), 1);
  FieldPsynergy_StopMindReadWave();
  FieldEvent_Wait(20);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "I told you... I told you I could hear them!"
  Message_ShowCentered(((s32)MsgMarsLighthouseAerieBeaconIsLit + 7), 1);
  FieldPsynergy_StopMindReadWave();
  gFieldState->message += 2;
  MarsLighthouseAerie_StartLookingAround(gMainRoutineState.leaderActorId);
  MarsLighthouseAerie_StartLookingAround(ACTOR_PIERS);
  MarsLighthouseAerie_StartLookingAround(ACTOR_JENNA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_SHEBA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_KRADEN);
  MarsLighthouseAerie_StartLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StartLookingAround(ACTOR_IVAN);
  MarsLighthouseAerie_StartLookingAround(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  // "Who said that? Where are you?!"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 0, 0, 10);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "We're in Imil! We're at the base of Mercury Lighthouse!"
  Message_ShowCentered(((s32)MsgMarsLighthouseAerieBeaconIsLit + 9), 1);
  FieldPsynergy_StopMindReadWave();
  MarsLighthouseAerie_StopLookingAround(ACTOR_MIA);
  Actor_FaceDirectionAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, FACING_SOUTH);
  /* FAKEMATCH(signature): Mia's hook cleared as a plain word, and the field
     state's address read as one. */
  *(s32 *)&Actor_Get(ACTOR_MIA)->update = 0;
  later = (struct FieldState *)((struct HeapWords *)Ram_HeapSlots)->fieldState;
  later->message++;
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "You... You can't be!"
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Event_PlaySound(SE_ASCEND);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(20);
  System_Wait(20);
  FieldEvent_SetExitRequest(MARS_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EAST_A);
  return;
}

void MarsLighthouseAerie_SpawnBeaconFlames(void);

/// All look up as the beam widens and the Golden Sun begins to rise.
void MarsLighthouseAerie_WatchGoldenSunRise(void) {
  s32 frame;
  struct Actor *actor;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_Get(gMainRoutineState.leaderActorId)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_PIERS)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_JENNA)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_SHEBA)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_KRADEN)->facing = FACING_SOUTH_SOUTHEAST;
  Actor_Get(ACTOR_ISAAC)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_GARET)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_IVAN)->facing = FACING_SOUTH;
  System_Wait(1);
  Actor_SetAnimation(ACTOR_JENNAS_MOTHER, 8);
  Actor_SetAnimation(ACTOR_JENNAS_FATHER, 8);
  Actor_SetAnimation(ACTOR_KYLE, 7);
  Actor_SetPosition(ACTOR_KRADEN, 498, 320);
  Actor_SetPosition(ACTOR_GARET, 538, 320);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 506, 280);
  System_Wait(1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(512, 96, 288, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHEAST);
  FieldEvent_SetMessageId((s32)MsgMarsLighthouseAerieWhatCouldHeBeDoing);
  // "What could he be doing?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  FieldActor_Jump(ACTOR_KRADEN, 4, 10);
  // "Of course! I understand..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 320, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "The Wise One said that when all four lighthouses have been lit, the..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, 3);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "When that happens, Mt. Aleph and the lighthouses will probably..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "So, the Wise One is warning people in dreams, telling them to seek..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "Why would he do that!? You saw what he did to us... He can't be up..."
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_HAMMA), SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_HAMMA, 512, 360);
  System_Wait(1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "Have you not learned? One's actions do not always reveal one's true..."
  Event_ShowMessage(ACTOR_HAMMA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  FieldActor_Jump(ACTOR_IVAN, 4, 10);
  // "Hamma... Sister..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Master Hamma! Did you receive a message, too?"
  Event_ShowMessage(ACTOR_ISAAC, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "Yes. I was called to Jupiter Lighthouse in a dream."
  Event_ShowMessage(ACTOR_HAMMA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 20);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, 3);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "You have completed your quest, but I see it comes with great loss......"
  Event_ShowMessage(ACTOR_HAMMA, 0);
  FieldPsynergy_StopMindReadWave();
  actor = Actor_Get(ACTOR_JENNA);
  actor->facing = FACING_SOUTHWEST;
  Actor_SetAnimation(ACTOR_JENNA, 22);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 2);
  // "Master Hamma... My parents..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "I know, Jenna, and I am sorry. But do not give up hope for them just..."
  Event_ShowMessage(ACTOR_HAMMA, 0);
  FieldPsynergy_StopMindReadWave();
  actor->facing = FACING_SOUTH;
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "What do you mean, Master Hamma?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "Appearances can be an illusion... The Wise One has a caring heart."
  Event_ShowMessage(ACTOR_HAMMA, 0);
  FieldPsynergy_StopMindReadWave();
  // "He forced us to fight Isaac's and Jenna's parents! What's so..."
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "If the Wise One were truly evil, he would not be warning me of..."
  Event_ShowMessage(ACTOR_HAMMA, 0);
  FieldPsynergy_StopMindReadWave();
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Well, I... I guess not... But then, what's he up to?"
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "We do not have time to discuss it right now."
  Event_ShowMessage(ACTOR_HAMMA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "She's right. If the Wise One said it's not safe to be near the..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "I am heading for safety, too. All of you must get away from the..."
  Event_ShowMessage(ACTOR_HAMMA, 0);
  FieldPsynergy_StopMindReadWave();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 296, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  // "Hurry, everyone! Let's go!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
  // "What about our parents?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
  // "We can't leave them behind..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  // "I understand how you feel, but we won't make it if we have to take..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "Make the decision, Felix! Should we take them with us?"
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_EAST);
  FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  Actor_WaitForAnimationChange(gMainRoutineState.leaderActorId);
  PaletteFade_ApplyTarget(PALETTE_TINT_MUTED, PALETTES_BG);
  PaletteFade_Interpolate(40);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  MarsLighthouseAerie_StartLookingAround(gMainRoutineState.leaderActorId);
  MarsLighthouseAerie_StartLookingAround(ACTOR_JENNA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_SHEBA);
  MarsLighthouseAerie_StartLookingAround(ACTOR_PIERS);
  MarsLighthouseAerie_StartLookingAround(ACTOR_KRADEN);
  MarsLighthouseAerie_StartLookingAround(ACTOR_ISAAC);
  MarsLighthouseAerie_StartLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StartLookingAround(ACTOR_IVAN);
  MarsLighthouseAerie_StartLookingAround(ACTOR_MIA);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 288, 1);
  FieldCamera_WaitForArrival();
  FieldTask_Remove(MarsLighthouseAerie_SpawnBeaconFlames);
  Event_PlaySound(SE_SPIN);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEACON_FIRE), SPRITE_COLOR_DARK);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEACON_GLOW), SPRITE_COLOR_DARK);
  FieldActor_SetSpritePriority(ACTOR_BEAM_BOTTOM, 2);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_BOTTOM), SPRITE_COLOR_WHITE);
  actor = Actor_Get(ACTOR_BEAM_BOTTOM);
  actor->scaleY = -FIXED_ONE;
  actor->scaleX = FIXED_ONE / 10;
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FieldActor_SetPositionAndResetMotion(actor, toFIXED(512), toFIXED(64),
                                       toFIXED(204));
  for (frame = 0; (u32)frame < 40; frame++) {
    if (frame == 20) {
      MapGrid_SetShake(2, 2, 1);
      FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                           EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      MarsLighthouseAerie_StopLookingAround(gMainRoutineState.leaderActorId);
      MarsLighthouseAerie_StopLookingAround(ACTOR_JENNA);
      MarsLighthouseAerie_StopLookingAround(ACTOR_SHEBA);
      MarsLighthouseAerie_StopLookingAround(ACTOR_PIERS);
      MarsLighthouseAerie_StopLookingAround(ACTOR_KRADEN);
      MarsLighthouseAerie_StopLookingAround(ACTOR_ISAAC);
      MarsLighthouseAerie_StopLookingAround(ACTOR_GARET);
      MarsLighthouseAerie_StopLookingAround(ACTOR_IVAN);
      MarsLighthouseAerie_StopLookingAround(ACTOR_MIA);
    }
    actor->scaleX += FIXED_ONE / 64;
    System_Wait(1);
  }
  Event_PlaySound(MUS_ALCHEMY_UNLEASHED);
  Actor_SetScript(ACTOR_BEAM_BOTTOM,
                  gMarsLighthouseAerie_WatchGoldenSunRiseScript);
  FieldEvent_Wait(80);
  FieldActor_Jump(ACTOR_GARET, 4, 10);
  FieldActor_Jump(ACTOR_GARET, 6, 10);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
  // "It's too dangerous... The Golden Sun is forming!"
  Event_ShowMessage(ACTOR_GARET, 0);
  // "A ray of light is emanating from the beacon... From the Mars Star."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHWEST);
  // "What will happen?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "The power of Alchemy will be unleashed upon the world!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_WEST);
  // "We must take Isaac's father and Jenna's parents and flee!"
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, 3);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 312, 1);
  Actor_SetPosition(ACTOR_BEACON_FIRE, 0, 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEACON_GLOW), SPRITE_COLOR_WHITE);
  FieldActor_SetSpritePriority(ACTOR_BEACON_GLOW, 1);
  Actor_Get(ACTOR_BEACON_GLOW)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Event_PlaySound(SE_SPIN);
  MapGrid_SetShake(3, 3, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
  PaletteFade_Interpolate(40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 80);
  FieldActor_Jump(ACTOR_GARET, 4, 0);
  Actor_SetSpeed(ACTOR_GARET, 3.2);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 542, 328);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  // "Stop it! Let go of my wrist!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldActor_Jump(ACTOR_ISAAC, 4, 10);
  // "Garet, get a grip! No one's touching you!"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  // "I'm serious! I..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "The beacon!"
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "Jenna... We have to move your parents!"
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldActor_Jump(ACTOR_JENNA, 4, 10);
  // "Mom! Dad!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "What's happening?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  FieldActor_Jump(ACTOR_KRADEN, 2, 10);
  FieldActor_Jump(ACTOR_KRADEN, 4, 10);
  // "There's no time for questions! Just carry them! Carry them and go!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_PlaySound(SE_STOP_EXIT_SE);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(1);
  Event_PlaySound(SE_SPIN);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(20);
  System_Wait(240);
  REG_DISPCNT = 0;
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_Interpolate(8);
  System_Wait(8);
  FieldEvent_SetExitRequest(MARS_LIGHTHOUSE_AERIE_TO_PROX_HOUSES);
  return;
}

/// Before the beacon is lit, Felix walks away south from the three as they
/// lie dying and reaches the head of the broad stairway down from the beacon's
/// platform on its west half (cells (29, 23) and (30, 23), map.txt's copy of
/// them at (45, 53) and (46, 53)). Kraden turns to him: "Do not let your
/// sadness overwhelm you, Felix! Go! Once the beacon has been ignited, you can
/// mourn as much as you need." and Felix walks back up onto the platform, to
/// (480, 348), south-west of them.
void MarsLighthouseAerie_HoldBackFromParents(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  FieldEvent_SetMessageId(MsgMarsLighthouseAerieDoNotLetYourSadness);
  // "Do not let your sadness overwhelm you, Felix! Go! Once the beacon..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 480, 348);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Event_End();
}

/// The same at the head of the stairway's east half (cells (33, 23) and
/// (34, 23), map.txt's copy of them at (49, 53) and (50, 53)): Kraden turns to
/// him, "Do not let your sadness overwhelm you, Felix! Go! Once the beacon has
/// been ignited, you can mourn as much as you need. I will protect you.", and
/// Felix walks back up onto the platform, to (544, 348), south-east of them.
void MarsLighthouseAerie_HoldBackFromParentsAgain(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  FieldEvent_SetMessageId(MsgMarsLighthouseAerieDoNotLetYourSadness);
  // "Do not let your sadness overwhelm you, Felix! Go! Once the beacon..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 544, 348);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Event_End();
}

/// Felix steps up to the beacon, and the voice asks: "Will you cast in the Mars
/// Star?" Yes lights the beacon (MarsLighthouseAerie_LightBeacon); no leaves
/// it to be asked again once he steps back.
void MarsLighthouseAerie_OfferToCastMarsStar(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetPosition(ACTOR_VOICE, 512, 232);
  System_Wait(1);
  FieldEvent_SetMessageId(MsgMarsLighthouseAerieWillYouCastInMars);
  // "Will you cast in the Mars Star?"
  Event_OpenMessage(ACTOR_VOICE, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    Actor_SetPosition(ACTOR_VOICE, 0, 0);
    System_Wait(1);
    MarsLighthouseAerie_LightBeacon();
  }
  Actor_SetPosition(ACTOR_VOICE, 0, 0);
  System_Wait(1);
  Event_SetFlag(FLAG_ASKED_FOR_MARS_STAR);
  Event_End();
}

/// Nothing visible happens as Felix steps back from the beacon without casting
/// in the Mars Star; the game lets the voice ask again when he steps up.
void MarsLighthouseAerie_ClearCastOffer(void) {
  GameFlag_Clear(FLAG_ASKED_FOR_MARS_STAR);
}

#if TLA_PATCH_1
/// From patch 1: the one Halt holds speaks haltingly, with a pause before
/// each character.
void MarsLighthouseAerie_StartTalkWhileHalted(s32 id) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;

  if (gMainRoutineState.haltedActor == id)
    state->haltingSpeech = 1;
}
#endif

#if TLA_PATCH_1

/// From patch 1: the talk over, Halt's hold on its speaker ends, and with it
/// the halting speech.
void MarsLighthouseAerie_EndTalkWhileHalted(s32 id) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;
  s32 held = gMainRoutineState.haltedActor;

  if (held == id) {
    state->haltingSpeech = 0;
    gMainRoutineState.haltedActor = -1;
    Actor_Get(held)->update = NULL;
    FieldActor_SetSpriteColor(Actor_Get(held), SPRITE_COLOR_NORMAL);
  }
}

#endif

/// Felix talks to Ivan, who stands by the three: "Felix, I understand your
/// loss..." From patch 1, if Halt holds him, he speaks haltingly.
void MarsLighthouseAerie_TalkToIvan(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
#if TLA_PATCH_1
  MarsLighthouseAerie_StartTalkWhileHalted(ACTOR_IVAN);
#endif
  Actor_FaceActor(ACTOR_IVAN, gMainRoutineState.leaderActorId, 0);
  FieldEvent_SetMessageId(MsgIUnderstandYour);
  // "Felix, I understand your loss... But remember, you and Jenna still..."
  Event_ShowMessage(ACTOR_IVAN, 0);
#if TLA_PATCH_1
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
  MarsLighthouseAerie_EndTalkWhileHalted(ACTOR_IVAN);
#else
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
#endif
  Event_End();
}

/// Felix talks to Jenna's mother as she lies dying: "Uhhhhnnn... Felix...
/// I...can't see you, but I sense that you're..."
void MarsLighthouseAerie_TalkToJennasMother(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
#if TLA_PATCH_1
  MarsLighthouseAerie_StartTalkWhileHalted(ACTOR_JENNAS_MOTHER);
#endif
  FieldEvent_SetMessageId(MsgUhhhhnnnICan);
  // "Uhhhhnnn... Felix... I...can't see you, but I sense that you're..."
  Event_ShowMessage(ACTOR_JENNAS_MOTHER, 0);
#if TLA_PATCH_1
  MarsLighthouseAerie_EndTalkWhileHalted(ACTOR_JENNAS_MOTHER);
#endif
  Event_End();
}

/// Felix talks to Kyle, Isaac's father, as he lies dying: "*Koff! Koff!*
/// Felix... You made it back...alive..."
void MarsLighthouseAerie_TalkToKyle(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
#if TLA_PATCH_1
  MarsLighthouseAerie_StartTalkWhileHalted(ACTOR_KYLE);
#endif
  FieldEvent_SetMessageId(MsgKoffKoffYou);
  // "*Koff! Koff!* Felix... You made it back...alive... Please, I must..."
  Event_ShowMessage(ACTOR_KYLE, 0);
#if TLA_PATCH_1
  MarsLighthouseAerie_EndTalkWhileHalted(ACTOR_KYLE);
#endif
  Event_End();
}

/// Felix talks to Jenna's father as he lies dying: "Unnnn... Felix?
/// ...Jenna? I am so happy...to have one last chance..."
void MarsLighthouseAerie_TalkToJennasFather(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
#if TLA_PATCH_1
  MarsLighthouseAerie_StartTalkWhileHalted(ACTOR_JENNAS_FATHER);
#endif
  FieldEvent_SetMessageId(MsgUnnnnIAm);
  // "Unnnn... Felix? ...Jenna? I am so happy...to have one last chance to..."
  Event_ShowMessage(ACTOR_JENNAS_FATHER, 0);
#if TLA_PATCH_1
  MarsLighthouseAerie_EndTalkWhileHalted(ACTOR_JENNAS_FATHER);
#endif
  Event_End();
}

#if TLA_PATCH_1

/// From patch 1: Halt on one of the three holds the speaker beside them.
void MarsLighthouseAerie_HaltSpeaker(s32 spot, s32 speaker) {
  struct Actor *found = Actor_Get(spot);
  struct Actor *actor;
  struct Actor *target;
  struct MainRoutineState *party = &gMainRoutineState;

  party->pausedColorMode = 0;
  actor = found;
  party->haltedActor = speaker;
  target = Actor_Get(speaker);
  target->update = actor->update;
  actor->update = NULL;
}
#endif

#if TLA_PATCH_1

/// From patch 1: Ivan is held where he stands while he talks.
void MarsLighthouseAerie_HaltIvan(void) {
  MarsLighthouseAerie_HaltSpeaker(ACTOR_HAMMA, ACTOR_IVAN);
}

/// From patch 1: Jenna's mother is held where she lies while she talks.
void MarsLighthouseAerie_HaltJennasMother(void) {
  MarsLighthouseAerie_HaltSpeaker(ACTOR_JENNAS_MOTHER_TALK_SPOT,
                                  ACTOR_JENNAS_MOTHER);
}

/// From patch 1: Kyle is held where he lies while he talks.
void MarsLighthouseAerie_HaltKyle(void) {
  MarsLighthouseAerie_HaltSpeaker(ACTOR_KYLE_TALK_SPOT, ACTOR_KYLE);
}

/// From patch 1: Jenna's father is held where he lies while he talks.
void MarsLighthouseAerie_HaltJennasFather(void) {
  MarsLighthouseAerie_HaltSpeaker(ACTOR_JENNAS_FATHER_TALK_SPOT,
                                  ACTOR_JENNAS_FATHER);
}
#endif

#include "shared/teleport_shimmer.c"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

/// Each frame of an actor's rings: a ring climbs RING_CLIMB pixels up the
/// actor, swelling and shrinking as it goes, and the next starts with a chime
/// RING_INTERVAL frames later, two at a time, while the actor's colours cycle.
/// Once stopped, the rings climb out and the actor's colours come back. The
/// Psynergy test's PsynergyTest_UpdateRings is the same routine.
void MarsLighthouseAerie_UpdateRisingRings(struct Actor *actor) {
  struct Vector3 position;
  s32 x, z;
  FIXED sine;
  s32 i, j;
  struct SparkleState *state = (struct SparkleState *)actor->watchedActor;
  u8 *count = &actor->sceneByte;
  struct EffectSprite *sprite;

  // Every other frame, the next of the normal colours and the five swaps.
  if (*count & 1)
    Actor_SetSpriteColorMode(actor,
                             (u8)((u32)((struct CounterBits *)count)->rest %
                                  (SPRITE_COLOR_BRG + 1)));
  if (actor->sceneCounter == 0) {
    if ((*count & (RING_INTERVAL - 1)) == 0) {
      Event_PlaySound(SE_RISING_RINGS);
      state->heights[state->nextPair] = 0;
      state->nextPair ^= 1;
    }
  } else if (actor->sceneCounter == 1) {
    if (state->heights[0] >= RING_CLIMB && state->heights[1] >= RING_CLIMB) {
      Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
      VramCache_FreeEntry(state->vramEntry);
      actor->update = NULL;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    // Its size: half a turn of sine over the climb.
    sine = Math_Sin(state->heights[i] * (DEGtoANG(180) / RING_CLIMB));
    if (state->heights[i] >= 0) {
      if (state->heights[i] < RING_CLIMB) {

        SET_POSITION(&position, actor->x.fixed,
                     actor->y.fixed + toFIXED(state->heights[i]),
                     actor->z.fixed);
        Camera_WorldToScreen(&position);
        x = position.x;
        z = position.z;
        sprite = &state->sprites[i * 2];
        for (j = 0; j <= 1; j++) {
          sprite->x = x;
          sprite->y = z;
          sprite->scaleX = sine;
          sprite->scaleY = sine;
          if (j == 0) {
            sprite->depth = Actor_GetSpriteDepth(actor) - 1;
          } else {
            sprite->depth = Actor_GetSpriteDepth(actor) + 1;
            sprite->y -= toFIXED(1);
            sprite->scaleY = -sprite->scaleY;
          }
          EffectSprite_Draw(sprite);
          sprite++;
        }
        state->heights[i]++;
      }
    }
  }
}

/// Starts an actor's rings, kept in `state`: loads their tiles and hooks
/// MarsLighthouseAerie_UpdateRisingRings to the actor.
void MarsLighthouseAerie_StartRisingRings(struct Actor *actor,
                                          struct SparkleState *state) {
  struct EffectSprite *sprite;
  s32 tile;
  s32 row;
  s32 col;
  s32 offset;

  state->vramEntry = VramCache_FindFreeEntry();
  Vram_Load(state->vramEntry, 8 * TILE_SIZE_4BPP,
            gMarsLighthouseAerie_RisingRingTiles);
  state->nextPair = 0;
  state->heights[0] = RING_NOT_STARTED;
  state->heights[1] = RING_NOT_STARTED;
  state->actor = actor;
  actor->sceneCounter = 0;
  actor->sceneByte = 0;
  actor->update = MarsLighthouseAerie_UpdateRisingRings;
  tile = gVramCacheEntries[state->vramEntry].offset / TILE_SIZE_4BPP;
  actor->watchedActor = (struct Actor *)state;
  for (row = 0; row <= 1; row++) {
    col = 1;
    offset = (row * 2) * sizeof(struct EffectSprite);
    sprite = (struct EffectSprite *)(offset + (s32)state);
    for (; col >= 0; col--) {
      EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X16, tile);
      sprite->oam.fullColor = 0;
      sprite->oam.palette = 0;
      sprite->oam.priority = Actor_GetSpritePriority(actor);
      sprite++;
    }
  }
}

/// Lets an actor's rings climb out.
void MarsLighthouseAerie_EndRisingRings(struct Actor *actor) {
  actor->sceneCounter = 1;
}

// clang-format off
/// The rings' tiles: half a ring of light, 32 by 16 pixels of 16 colours,
/// which MarsLighthouseAerie_UpdateRisingRings draws twice, the second
/// flipped, for the whole ring; only the first 8 tiles are loaded.
s32 gMarsLighthouseAerie_RisingRingTiles[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x70000000, 0xf7700000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x77770000, 0xffff7770, 0xfffffff7, 0x7777ffff,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00007777, 0x0777ffff, 0x7fffffff, 0xffff7777,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000007, 0x0000077f,
    0xfff70000, 0xffff7000, 0x7ffff700, 0x07ffff70,
    0x007fff70, 0x007ffff7, 0x0007fff7, 0x0007fff7,
    0x000077ff, 0x00000077, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0xff770000, 0x77000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00007fff, 0x0007ffff, 0x007ffff7, 0x07ffff70,
    0x07fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000,
    // Unused: the same half ring outlined in colour 4.
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x40000000, 0xf4400000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x44440000, 0xffff4440, 0xfffffff4, 0x4444ffff,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00004444, 0x0444ffff, 0x4fffffff, 0xffff4444,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000004, 0x0000044f,
    0xfff40000, 0xffff4000, 0x4ffff400, 0x04ffff40,
    0x004fff40, 0x004ffff4, 0x0004fff4, 0x0004fff4,
    0x000044ff, 0x00000044, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0xff440000, 0x44000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00004fff, 0x0004ffff, 0x004ffff4, 0x04ffff40,
    0x04fff400, 0x4ffff400, 0x4fff4000, 0x4fff4000,
    // Unused: three 8 by 8 sparkles of 256 colours.
    0xe9000000, 0x000000e9, 0xeb000000, 0x000000eb,
    0xedeb0000, 0x0000ebed, 0xeeedebe9, 0xe9ebedee,
    0xeeedebe9, 0xe9ebedee, 0xedeb0000, 0x0000ebed,
    0xeb000000, 0x000000eb, 0xe9000000, 0x000000e9,
    0x0000e900, 0x00e90000, 0x00ebe9e9, 0xe9e9eb00,
    0xedebeb00, 0x00ebebed, 0xefed0000, 0x0000edef,
    0xefed0000, 0x0000edef, 0xedebeb00, 0x00ebebed,
    0x00ebe9e9, 0xe9e9eb00, 0x0000e900, 0x00e90000,
    0xe9000000, 0x000000e9, 0xebe90000, 0x0000e9eb,
    0xedebe900, 0x00e9ebed, 0xeeedebe9, 0xe9ebedee,
    0xeeedebe9, 0xe9ebedee, 0xedebe900, 0x00e9ebed,
    0xebe90000, 0x0000e9eb, 0xe9000000, 0x000000e9,
};

#define TELEPORT_CIRCLE_FIRST_ENTRY 137
#include "shared/teleport_circle_palette.c"

s32 gMarsLighthouseAerie_BeaconFireScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -(FIXED_ONE * 3 / 128)),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 3 / 128),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gMarsLighthouseAerie_JennaApproachScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(496, 0, 328),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gMarsLighthouseAerie_ShebaApproachScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(480, 0, 328),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gMarsLighthouseAerie_PiersApproachScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(488, 0, 320),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gMarsLighthouseAerie_IvanApproachScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(528, 0, 328),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gMarsLighthouseAerie_KradenApproachScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(512, 0, 328),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gMarsLighthouseAerie_VoiceFadeScript[] = {
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(64, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gMarsLighthouseAerie_DoomDragonRiseScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 128),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 128),
    SC_ADD(SW_Y, toFIXED(1) / 8),
    SC_WAIT(1),
    SC_LOOP(128, SC_TOP),
    SC_END,
};

s32 gMarsLighthouseAerie_LeaderReadyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(500, 0, 306),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_IsaacReadyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(524, 0, 306),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_JennaReadyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(488, 0, 320),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_ShebaReadyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(472, 0, 304),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_PiersReadyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(464, 0, 280),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_IvanReadyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(538, 0, 320),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_MiaReadyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(552, 0, 304),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_GaretReadyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(560, 0, 280),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_ShebaToThosePeopleScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(472, 0, 320),
    SC_SYNC,
    SC_MOVE(472, 0, 304),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_SCENE_COUNTER, 1),
    SC_END,
};

s32 gMarsLighthouseAerie_JennaToThosePeopleScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(476, 0, 328),
    SC_SYNC,
    SC_MOVE(472, 0, 320),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_SCENE_COUNTER, 1),
    SC_END,
};

s32 gMarsLighthouseAerie_JennaTurnScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(486, 0, 286),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_LeaderTurnScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(504, 0, 300),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gMarsLighthouseAerie_IsaacTurnScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(528, 0, 292),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

// The same script as gMarsLighthouseAerie_GaretReadyScript, under another name.
s32 gMarsLighthouseAerie_GaretTurnScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(560, 0, 280),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_MiaTurnScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(548, 0, 292),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gMarsLighthouseAerie_BeamWidenScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
};

s32 gMarsLighthouseAerie_BeamNarrowScript[] = {
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
};

u32 gMarsLighthouseAerie_BeamShootUpScript[] = {
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gMarsLighthouseAerie_BeamRiseScript[] = {
    SC_ADD(SW_Y, toFIXED(2)),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gMarsLighthouseAerie_BeaconSparkScript[] = {
    SC_SET(SW_SCALE_X, toFIXED(0.8)),
    SC_SET(SW_SCALE_Y, toFIXED(0.8)),
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, -toFIXED(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gMarsLighthouseAerie_WatchGoldenSunRiseScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(8, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MarsLighthouseAerie),
    SCENE_EXIT(MARS_LIGHTHOUSE_AERIE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_AERIE),
    SCENE_EXIT(EXIT_TO_THE_BEACON, MarsLighthouseAerie, MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_AERIE),
    SCENE_EXIT(MARS_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EAST_A, MercuryLighthouse, MERCURY_LIGHTHOUSE_FROM_MARS_LIGHTHOUSE_AERIE_EAST_A),
    SCENE_EXIT(MARS_LIGHTHOUSE_AERIE_TO_PROX_HOUSES, ProxHouses, PROX_HOUSES_FROM_MARS_LIGHTHOUSE_AERIE),
    SCENE_EXITS_END,
};

struct ActorPlacement gMarsLighthouseAerie_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 584, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 616, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseAerie_AfterBattlePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 512, 0, 236, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 354, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 496, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 480, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 478, 0, 266, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 506, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 276, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_MOTHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 480, 0, 282, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 506, 0, 292, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 540, 0, 284, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseAerie_BeaconPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 584, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 616, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 458, 0, 288, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 476, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 482, 0, 276, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 529, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 538, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 274, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 566, 0, 286, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_MOTHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 480, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 502, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 548, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), CELL_TELEPORT_LANDING, CONDITION_ALWAYS, EVENT(Scene_LeaveByExit)},
    {TRIGGER_STEP, CELL_BEFORE_BEACON, FLAG_MARS_BEACON_LIT, EVENT(MarsLighthouseAerie_FaceDoomDragon)},
    {TRIGGER_TALK, ACTOR_ISAAC, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieOurQuestIsAlmostOver)},
    {TRIGGER_TALK, ACTOR_GARET, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIWishICouldConsole)},
    {TRIGGER_TALK, ACTOR_MIA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIfAlexSetThisAll)},
    {TRIGGER_TALK, ACTOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIThoughtIdGet)},
    {TRIGGER_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIDontKnowMyParents)},
    {TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieImSorryThingsHad)},
    {TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieBothKyleAndYourParents)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_HAMMA, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToIvan)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToJennasMother)},
    {TALK_TRIGGER_FACING(FACING_SOUTHWEST), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToJennasMother)},
    {TALK_TRIGGER_FACING(FACING_NORTHEAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToKyle)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToKyle)},
    {TALK_TRIGGER_FACING(FACING_SOUTHEAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToKyle)},
    {TRIGGER_TALK, ACTOR_JENNAS_FATHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToJennasFather)},
#if TLA_PATCH_1 /* From patch 1. */
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_HAMMA, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HaltIvan)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HaltJennasMother)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HaltKyle)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_JENNAS_FATHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HaltJennasFather)},
#endif
    {TRIGGER_STEP, CELL_KRADEN_STOPS_WEST, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HoldBackFromParents)},
    {TRIGGER_STEP, CELL_KRADEN_STOPS_EAST, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HoldBackFromParentsAgain)},
    {TRIGGER_STEP, CELL_AT_BEACON, FLAG_ASKED_FOR_MARS_STAR, EVENT(MarsLighthouseAerie_OfferToCastMarsStar)},
    {TRIGGER_STEP, CELL_ROUND_BEACON, IF_FLAG_SET(FLAG_ASKED_FOR_MARS_STAR), EVENT(MarsLighthouseAerie_ClearCastOffer)},
    {TRIGGER_TABLE_END},
};

s32 gMarsLighthouseAerie_BeaconFlameScript[] = {
    SC_SET(SW_SCALE_Y, toFIXED(1.4)),
    SC_SET(SW_SCALE_X, toFIXED(1.4)),
    SC_ANIMATION(4),
    SC_WAIT(30),
    SC_RELEASE,
};

#include "shared/orb_rising_script.c"
// clang-format on
