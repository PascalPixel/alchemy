/// @file
/// Mars Lighthouse's aerie, at the top of the last lighthouse: the party faces
/// the Doom Dragon (MarsLighthouseAerie_FaceDoomDragon) and learns who the
/// dragon was (MarsLighthouseAerie_FindWhoTheDragonWas), the parents it held
/// speak to their children (MarsLighthouseAerie_TalkToJennasMother and its
/// siblings), and the beacon is lit with the Mars Star
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
#include "field/field_sequence_event.h"
#include "scene/mars_lighthouse_aerie.h"
#include "scene/scene.h"
#include "scene/shared.h"
#include "scene_ids.h"
#include "field/event_commands.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
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

/// Set as the party leaves the Mercury Lighthouse at the end: the beacon's
/// fire burns at the aerie from then on.
#define FLAG_MARS_BEACON_BURNING 0xa3c
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
  /// Doom Dragon (sprite 182) and the Wise One (sprite 311), the voice that
  /// calls the party betrayers.
  ACTOR_DOOM_DRAGON,
  ACTOR_WISE_ONE,
  ACTOR_KRADEN
};

/// After the battle (gMarsLighthouseAerie_AfterBattlePlacements): the party
/// but the leader stand in on sprites 48 to 54, the dragon's three heads
/// (sprite 183) fall, and under each lies one of the three it was.
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

/// At the beacon (gMarsLighthouseAerie_BeaconPlacements), entered by 3 to 5:
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
  /// Hama, who speaks to the party from afar as the Golden Sun rises; until
  /// then her actor stands unseen beside Ivan as his talk spot.
  ACTOR_HAMA,
  ACTOR_JENNAS_FATHER_TALK_SPOT,
  ACTOR_KYLE_TALK_SPOT
};
#ifndef TLA_MARS_LIGHTHOUSE_AERIE_H
#define TLA_MARS_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/scene_tables.h"

struct Trigger;

u32 Random16(void);
void MarsLighthouseAerie_LookAround(struct Actor *object);
void MarsLighthouseAerie_BeginTalkWhileHalted(s32 id);
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
extern u32 gMarsLighthouseAerie_FlickerFrame;
extern u32 gMarsLighthouseAerie_FlickerRate;

void MarsLighthouseAerie_HaltSpeaker(s32 spot, s32 speaker);

extern u8 MsgMarsLighthouseAerieDoNotLetYourSadness[];
extern u16 gPaletteCycle_Flash;
extern u16 gPaletteCycle_Countdown;
extern u16 gPaletteCycle_Period;
extern u16 *gPaletteCycle_Slots;
extern u16 gPaletteCycle_Count;
extern u16 gPaletteCycle_Step;

s32 List_CountEntries(u16 *list);

#endif

#include "text/msg_ids.h"

extern u32 gMarsLighthouseAerie_BeaconFlameFrame;
void Map_CopyPalette(s32 scene, s32 a1);
void FieldCamera_WaitForArrival();
extern u8 gMarsLighthouseAerie_RisingRingsA[];
extern u8 gMarsLighthouseAerie_RisingRingsB[];
void Party_RestoreHpPp();
void Weather_StartRain(struct Actor *object);

/// Each frame while waiting in the storm: now and then the actor looks
/// another way.
void MarsLighthouseAerie_LookAround(struct Actor *object) {
  u8 *timer = &object->riseCounter;

  if (*timer != 0) {
    (*timer)--;
  } else {
    *timer = (Random_Below(80)) + 60;
    object->facing = Random16();
  }
}

/// The actor flickers white every other frame.
void MarsLighthouseAerie_FlickerWhite(struct Actor *object) {
  if ((gFrameCount >> 1) & 1)
    FieldActor_SetSpriteColor(object, 15);
  else
    FieldActor_SetSpriteColor(object, 0);
}

/// The actor swells now and then and shrinks back, as if breathing.
void MarsLighthouseAerie_Throb(struct Actor *object) {
  u8 *timer;
  s32 scale = object->scaleX;

  if (scale > 0x10000) {
    object->scaleX = scale - 0x800;
    object->scaleY -= 0x400;
  }
  timer = &object->riseCounter;
  if (*timer != 0) {
    (*timer)--;
  } else {
    u32 r = Random16();

    *timer = *timer + ((((r << 2) + r) * 16) >> 16) + 40;
    object->scaleX = Q16_ONE * 9 / 8;
    object->scaleY = Q16_ONE * 17 / 16;
  }
}

/// A spark of the beacon's fire circles it as it rises.
void MarsLighthouseAerie_OrbitBeaconFire(struct Actor *object) {
  struct Actor *anchor = Actor_GetById(ACTOR_BEACON_FIRE);
  u16 *pangle = (u16 *)&object->timer;
  s32 angle = *pangle;
  s32 cosine;
  s32 sine;
  s32 along;
  s32 across;
  u8 *counter;

  cosine = Trig_Cos(angle);
  counter = &object->riseCounter;
  {
    s32 radius = object->speedLimit + *counter;
    struct Actor *a;

    a = anchor;
    along = a->x.fixed + cosine * (radius + 6);
  }
  object->x.fixed = along;
  sine = Math_Sin(angle);
  across = anchor->z.fixed + sine * (*counter + 4);
  object->z.fixed = across;
  object->targetX = object->x.fixed;
  object->targetZ = across;
  *pangle -= 0x800;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct Trigger *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.entrance == 2)
    return (const struct Trigger *)gMarsLighthouseAerie_AfterBattlePlacements;
  if ((u16)(gMainRoutineWork.entrance - 3) <= 2)
    return (const struct Trigger *)gMarsLighthouseAerie_BeaconPlacements;
  return (const struct Trigger *)gMarsLighthouseAerie_Placements;
}

/// Leaves by exit 1 after Teleport_StartShimmer and Teleport_PlaySparks, then
/// runs Teleport_StopShimmer.
void Scene_LeaveByExit(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(1);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern s16 gMarsLighthouseAerie_FadeSlots[];
extern s16 gMarsLighthouseAerie_FadeColorsFrom[];
extern s16 gMarsLighthouseAerie_FadeColorsTo[];
extern u32 gMarsLighthouseAerie_FadeOrder[];

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);
void MarsLighthouseAerie_SpawnBeaconFlames(void);

/// The beacon's colours and its fire, burning once the beacon is lit.
void MarsLighthouseAerie_SetUpBeacon(void) {
  struct Actor *object;
  s32 i;

  Palette_SetColorsFade(
      gMarsLighthouseAerie_FadeSlots, gMarsLighthouseAerie_FadeColorsFrom,
      gMarsLighthouseAerie_FadeColorsTo, gMarsLighthouseAerie_FadeOrder);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 0x40;
  for (i = 10; i <= 16; i++) {
    object = Actor_Get(i);
    object->priorityFlags |= 0x80;
    Actor_SetSpriteColorMode(object, 5);
  }
  if (!Event_GetFlag(FLAG_MARS_BEACON_BURNING))
    return;
  Actor_SetPosition(ACTOR_BEACON_FIRE, 512, 264);
  Actor_SetPosition(ACTOR_BEACON_GLOW, 512, 271);
  System_Wait(1);
  FieldActor_SetSpritePriority(ACTOR_BEACON_FIRE, 1);
  object = Actor_Get(ACTOR_BEACON_FIRE);
  object->motionFlags = ACTOR_FLOATS;
  object->y.fixed = px(112);
  Actor_SetSpriteColorMode(object, 5);
  Actor_SetSpriteFlags(object, 0);
  object->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(ACTOR_BEACON_GLOW, 2);
  object = Actor_Get(ACTOR_BEACON_GLOW);
  object->motionFlags = ACTOR_FLOATS;
  object->y.fixed = px(112);
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Actor_SetSpriteColorMode(object, 5);
  Actor_SetSpriteFlags(object, 0);
  object->priorityFlags |= 0x20;
  object->priorityFlags |= 2;
  Actor_SetBlendMode(object, 1);
  gMarsLighthouseAerie_BeaconFlameFrame = 0;
  FieldTask_AddFrame(MarsLighthouseAerie_SpawnBeaconFlames, TASK_PASS_VBLANK);
}

/// Sets the scene up for the entrance the party arrived by (5 to 2).
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  MarsLighthouseAerie_SetUpBeacon();
  Event_SetFlag(FLAG_IN_MARS_LIGHTHOUSE);
  if (gMainRoutineWork.entrance == 5) {
    /* FAKEMATCH(pin): the blend word pinned to r3 as a halfword loads from the
     * pool before the register address. */
    register u16 alpha asm("r3") = 0xc08;

    REG_BLDALPHA = alpha;
    REG_BLDCNT = 0x3f10;
    MarsLighthouseAerie_WatchGoldenSunRise();
  } else if (gMainRoutineWork.entrance == 4) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetAnimation(ACTOR_JENNAS_MOTHER, 8);
    Actor_SetAnimation(ACTOR_JENNAS_FATHER, 8);
    Actor_SetAnimation(ACTOR_KYLE, 7);
    Actor_SetPosition(ACTOR_KRADEN, 498, 320);
    Actor_SetPosition(ACTOR_GARET, 538, 320);
    Actor_SetPosition(gMainRoutineWork.leaderActorId, 506, 280);
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
  } else if (gMainRoutineWork.entrance == 3) {
    Actor_SetAnimation(ACTOR_JENNAS_MOTHER, 8);
    Actor_SetAnimation(ACTOR_JENNAS_FATHER, 8);
    Actor_SetAnimation(ACTOR_KYLE, 7);
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT) == 0)
      MarsLighthouseAerie_UrgeOnToBeacon();
  } else if (gMainRoutineWork.entrance == 2) {
    MarsLighthouseAerie_FindWhoTheDragonWas();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.entrance == 1 &&
      Event_GetFlag(FLAG_MARS_BEACON_LIT) == 0)
    Weather_StartRain(1);
  Map_CopyPalette(gMainRoutineWork.mapId +
                      (SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE - SCENE_PROX),
                  0x5000000);
  return 0;
}

extern u32 gMarsLighthouseAerie_BeaconFlameScript[];

void Actor_StartScript(struct Actor *object, const u8 *script);
void MarsLighthouseAerie_SwingBeaconFlame(struct Actor *object);

/// Every sixteenth frame a flame rises from the beacon's fire.
void MarsLighthouseAerie_SpawnBeaconFlames(void) {
  struct Actor *anchor = Actor_Get(ACTOR_BEACON_FIRE);
  s32 offset = (Random_Below(48)) << 16;
  struct Actor *object;
  struct MarsLighthouseAerieSprite *sprite;
  u32 phase;
  s32 angle;

  if (anchor != NULL &&
      (phase = gMarsLighthouseAerie_BeaconFlameFrame & 15) == 0) {
    object =
        FieldActor_Create(SPRITE_PARTICLE, anchor->x.fixed + px(8),
                          anchor->y.fixed + offset + px(8), anchor->z.fixed);
    offset = (offset / 0x60000) << 16;
    if (object != NULL) {
      u8 zero;

      sprite = object->sprite;
      Actor_StartScript(object, gMarsLighthouseAerie_BeaconFlameScript);
      FieldActor_SetSpriteColor(object, 10);
      object->motionFlags = phase;
      angle = Random16() & 0xffff000;
      object->timer = angle;
      object->userValue = phase;
      object->update = MarsLighthouseAerie_SwingBeaconFlame;
      zero = 0;
      object->watchedActor = anchor;
      object->speedLimit = Math_Sin((offset & 0xfffff) >> 4) * 24 >> 16;
      ((u8 *)sprite)[26] = zero;
      sprite->layer =
          ((struct MarsLighthouseAerieSprite *)anchor->sprite)->layer;
    }
  }
  gMarsLighthouseAerie_BeaconFlameFrame++;
}

/// A flame swings round the beacon's fire, behind it and then in front.
void MarsLighthouseAerie_SwingBeaconFlame(struct Actor *object) {
  u16 *pangle = (u16 *)&object->timer;
  u16 angle = *pangle;
  struct Actor *parent = object->watchedActor;

  object->x.fixed =
      parent->x.fixed + Trig_Cos(angle) * (object->speedLimit + 28);
  object->z.fixed = (Math_Sin(angle) << 4) + 0x1080000;
  object->targetX = object->x.fixed;
  object->targetZ = object->z.fixed;
  if (object->z.fixed < px(264))
    ((struct FieldSprite *)object->sprite)->priority = 2;
  else
    ((struct FieldSprite *)object->sprite)->priority = 1;
  *pangle -= 0x200;
}

/// The actor stands looking about.
void MarsLighthouseAerie_StartLookingAround(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_Stop(id);
  object->riseCounter = (Random_Below(40)) + 20;
  object->update = (void (*)(void))MarsLighthouseAerie_LookAround;
}

/// The actor stops looking about and faces the beacon.
void MarsLighthouseAerie_StopLookingAround(s32 id) {
  Actor_Get(id)->update = NULL;
  Actor_FaceDirection(id, FACING_NORTH, 0);
}

struct SparkleWork;
s32 Actor_ReleaseById();
s32 Actor_MoveToObject();
extern u32 gMarsLighthouseAerie_JennaApproachScript[];
extern u32 gMarsLighthouseAerie_ShebaApproachScript[];
extern u32 gMarsLighthouseAerie_PiersApproachScript[];
extern u32 gMarsLighthouseAerie_IvanApproachScript[];
extern u32 gMarsLighthouseAerie_KradenApproachScript[];
extern s32 gMarsLighthouseAerie_VoiceFadeScript[];
extern u32 gMarsLighthouseAerie_DoomDragonRiseScript[];
extern u32 gMarsLighthouseAerie_LeaderReadyScript[];
extern u32 gMarsLighthouseAerie_IsaacReadyScript[];
extern u32 gMarsLighthouseAerie_JennaReadyScript[];
extern u32 gMarsLighthouseAerie_ShebaReadyScript[];
extern u32 gMarsLighthouseAerie_PiersReadyScript[];
extern u32 gMarsLighthouseAerie_IvanReadyScript[];
extern u32 gMarsLighthouseAerie_MiaReadyScript[];
extern u32 gMarsLighthouseAerie_GaretReadyScript[];

/// The party reaches the beacon in the storm, and the voice calls them
/// betrayers before the Doom Dragon's battle.
void MarsLighthouseAerie_FaceDoomDragon(void) {
  u8 *motionFlags;
  u16 *dispcnt;
  struct MainRoutineWork *work;
  u16 *palette;
  struct Actor *actor;

  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  palette = (u16 *)&OBJ_PLTT_COLOR(0, 15);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_WISE_ONE), 15);
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
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 304, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 504, 320);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_MoveToObject(ACTOR_ISAAC, gMainRoutineWork.leaderActorId);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_ISAAC, 1.6);
  Actor_WalkToAndWait(ACTOR_ISAAC, 520, 320);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_MoveToObject(ACTOR_GARET, ACTOR_ISAAC);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_WalkToAndWait(ACTOR_GARET, 536, 320);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  // "Who said that!?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_MoveToObject(ACTOR_MIA, ACTOR_GARET);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_MIA, 1.6);
  Actor_WalkToAndWait(ACTOR_MIA, 544, 328);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_NORTHWEST);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "The wind's too strong! I can't see anything!"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldActor_Stop(gMainRoutineWork.leaderActorId);
  actor->riseCounter = (Random_Below(20)) + 20;
  actor->update = (void (*)(void))MarsLighthouseAerie_LookAround;
  MarsLighthouseAerie_StartLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StartLookingAround(ACTOR_MIA);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_IVAN, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_SetSpeed(ACTOR_IVAN, 1.6);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetScript(ACTOR_JENNA, (s32)gMarsLighthouseAerie_JennaApproachScript);
  Actor_SetScript(ACTOR_SHEBA, (s32)gMarsLighthouseAerie_ShebaApproachScript);
  Actor_SetScript(ACTOR_PIERS, (s32)gMarsLighthouseAerie_PiersApproachScript);
  Actor_SetScript(ACTOR_KRADEN, (s32)gMarsLighthouseAerie_KradenApproachScript);
  Actor_RunScriptAndWait(ACTOR_IVAN,
                         (s32)gMarsLighthouseAerie_IvanApproachScript);
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
  Actor_Get(gMainRoutineWork.leaderActorId)->update = NULL;
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
  FieldActor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, 3);
  MarsLighthouseAerie_StopLookingAround(ACTOR_ISAAC);
  MarsLighthouseAerie_StopLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StopLookingAround(ACTOR_KRADEN);
  MarsLighthouseAerie_StopLookingAround(ACTOR_IVAN);
  MarsLighthouseAerie_StopLookingAround(ACTOR_MIA);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(40);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 504, 292);
  Event_PlaySound(SE_WAVE);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(gMainRoutineWork.leaderActorId, 2);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.2);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 504, 308);
  Actor_RunRepeatedMotion(gMainRoutineWork.leaderActorId, 3);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 504, 320);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags |= 1;
  *palette = 0x7fff;
  System_Wait(1);
  // "So, you are still intent on lighting the beacon of Mars Lighthouse?"
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
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
  MarsLighthouseAerie_StartLookingAround(gMainRoutineWork.leaderActorId);
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
  MarsLighthouseAerie_StopLookingAround(gMainRoutineWork.leaderActorId);
  MarsLighthouseAerie_StopLookingAround(ACTOR_JENNA);
  MarsLighthouseAerie_StopLookingAround(ACTOR_SHEBA);
  MarsLighthouseAerie_StopLookingAround(ACTOR_PIERS);
  MarsLighthouseAerie_StopLookingAround(ACTOR_ISAAC);
  MarsLighthouseAerie_StopLookingAround(ACTOR_GARET);
  MarsLighthouseAerie_StopLookingAround(ACTOR_MIA);
  MarsLighthouseAerie_StopLookingAround(ACTOR_IVAN);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_WISE_ONE), 0);
  actor = Actor_Get(ACTOR_WISE_ONE);
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(ACTOR_WISE_ONE), px(512),
                                       px(120), px(208));
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
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
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
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 20);
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
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
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
                                       (s32)gMarsLighthouseAerie_RisingRingsA);
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
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 20);
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
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
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
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
  // "If we don't do it now, Prox will be destroyed! Felix... We have to..."
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                             0)) {
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
    FieldActor_Jump(ACTOR_SHEBA, 4, 10);
    Event_ShowMessage(ACTOR_SHEBA, 0);
    gFieldWork->message++;
  } else {
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    FieldActor_Jump(ACTOR_SHEBA, 4, 10);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_SHEBA, 0);
  }
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 11);
  FieldEvent_Wait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimation(ACTOR_WISE_ONE, 8);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_WaitForAnimationChange(ACTOR_WISE_ONE);
  Actor_SetAnimation(ACTOR_WISE_ONE, 1);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                             0)) {
    Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
    gFieldWork->message++;
  } else {
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  }
  work = &gMainRoutineWork;
  Actor_FaceDirectionAndWait(work->leaderActorId, FACING_EAST);
  FieldActor_SetAnimationAndWait(work->leaderActorId, 3);
  Actor_FaceDirectionAndWait(work->leaderActorId, FACING_NORTH);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_SetSpeed(work->leaderActorId, 0.4);
  Actor_WalkToAndWait(work->leaderActorId, 504, 308);
  Event_PlaySound(SE_ASCEND);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 14);
  FieldEvent_Wait(10);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, 0x100);
  Actor_RunRepeatedMotion(work->leaderActorId, 2);
  Actor_SetSpeed(work->leaderActorId, 1.6);
  FieldActor_Jump(work->leaderActorId, 4, 0);
  Actor_Get(work->leaderActorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(work->leaderActorId, 504, 320);
  FieldEvent_Wait(1);
  Actor_Get(work->leaderActorId)->steeringFlags |= 1;
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 14);
  FieldEvent_Wait(10);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 10);
  FieldEvent_Wait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  FieldActor_SetAnimation(ACTOR_WISE_ONE, 9);
  FieldEvent_Wait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(work->leaderActorId, FACING_WEST, 40);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Event_PlaySound(SE_CRUMBLE);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_WISE_ONE), 7);
  FieldEvent_Wait(20);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_WISE_ONE), 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(work->leaderActorId, FACING_NORTH, 20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  FieldActor_SetAnimation(work->leaderActorId, 3);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  FieldActor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  actor->update = (void (*)(void))MarsLighthouseAerie_FlickerWhite;
  Actor_SetScript(ACTOR_WISE_ONE, (s32)gMarsLighthouseAerie_VoiceFadeScript);
  PaletteFade_ApplyTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
  PaletteFade_Interpolate(20);
  System_Wait(20);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_Interpolate(40);
  System_Wait(40);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetPosition(work->leaderActorId, 0, 0);
  Actor_SetPosition(ACTOR_ISAAC, 0, 0);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  System_Wait(1);
  actor = Actor_Get(ACTOR_DOOM_DRAGON);
  actor->scaleX = 0x1000;
  actor->scaleY = 0x1000;
  Actor_SetPosition(ACTOR_DOOM_DRAGON, 512, 280);
  System_Wait(1);
  motionFlags = &actor->motionFlags;
  *motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(actor, px(512), px(48), px(280));
  System_Wait(1);
  *motionFlags = 0;
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_Interpolate(20);
  dispcnt = (u16 *)&REG_DISPCNT;
  Event_PlaySound(MUS_ALEX);
  Actor_RunScriptAndWait(ACTOR_DOOM_DRAGON,
                         (s32)gMarsLighthouseAerie_DoomDragonRiseScript);
  *dispcnt = *dispcnt & 0xf9ff;
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
  *dispcnt |= 0x600;
  Actor_SetPosition(ACTOR_PIERS, 488, 320);
  Actor_SetPosition(work->leaderActorId, 504, 320);
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
  actor->gravity = px(1) / 4;
  *motionFlags = 3;
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
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_SetSpeed(work->leaderActorId, 1.6);
  Actor_SetSpeed(ACTOR_ISAAC, 1.6);
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_IVAN, 1.6);
  Actor_SetSpeed(ACTOR_MIA, 1.6);
  Actor_SetScript(ACTOR_PIERS, (s32)gMarsLighthouseAerie_PiersReadyScript);
  Actor_SetScript(work->leaderActorId,
                  (s32)gMarsLighthouseAerie_LeaderReadyScript);
  Actor_SetScript(ACTOR_ISAAC, (s32)gMarsLighthouseAerie_IsaacReadyScript);
  Actor_SetScript(ACTOR_SHEBA, (s32)gMarsLighthouseAerie_ShebaReadyScript);
  Actor_SetScript(ACTOR_JENNA, (s32)gMarsLighthouseAerie_JennaReadyScript);
  Actor_SetScript(ACTOR_IVAN, (s32)gMarsLighthouseAerie_IvanReadyScript);
  Actor_SetScript(ACTOR_MIA, (s32)gMarsLighthouseAerie_MiaReadyScript);
  Actor_RunScriptAndWait(ACTOR_GARET,
                         (s32)gMarsLighthouseAerie_GaretReadyScript);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHWEST);
  FieldActor_Jump(ACTOR_GARET, 6, 10);
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldActor_Jump(ACTOR_KRADEN, 2, 10);
  FieldActor_Jump(ACTOR_KRADEN, 4, 10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  work->battleType = BATTLE_TYPE_NO_ESCAPE;
  FieldEvent_SetAfterBattleDestination(SCENE_MARS_LIGHTHOUSE_AERIE, 2);
  Field_RequestBattleFromEncounterTable(102, 1);
  return;
}

/// The Doom Dragon's parts flicker white, ever faster.
void MarsLighthouseAerie_FlickerDragon(struct Actor *object) {
  if ((gMarsLighthouseAerie_FlickerFrame >> gMarsLighthouseAerie_FlickerRate) &
      1)
    FieldActor_SetSpriteColor(object, 15);
  else
    FieldActor_SetSpriteColor(object, 0);
  gMarsLighthouseAerie_FlickerFrame++;
}

/// The three it was flicker white out of step with the dragon.
void MarsLighthouseAerie_FlickerParent(struct Actor *object) {
  if ((gMarsLighthouseAerie_FlickerFrame >> gMarsLighthouseAerie_FlickerRate) &
      1)
    FieldActor_SetSpriteColor(object, 0);
  else
    FieldActor_SetSpriteColor(object, 15);
}

extern u32 gMarsLighthouseAerie_ShebaToBodiesScript[];
extern u32 gMarsLighthouseAerie_JennaToBodiesScript[];
extern u32 gMarsLighthouseAerie_JennaTurnScript[];
extern u32 gMarsLighthouseAerie_LeaderTurnScript[];
extern u32 gMarsLighthouseAerie_IsaacTurnScript[];
extern u32 gMarsLighthouseAerie_GaretTurnScript[];
extern u32 gMarsLighthouseAerie_MiaTurnScript[];

void MarsLighthouseAerie_FindWhoTheDragonWas(void) {
  s32 facing;
  s32 leader, leaderAgain;
  struct Actor *actor, *mother, *father, *kyle;
  s32 leaderId;

  actor = Actor_Get(ACTOR_DOOM_DRAGON);
  mother = Actor_Get(ACTOR_FREED_JENNAS_MOTHER);
  father = Actor_Get(ACTOR_FREED_JENNAS_FATHER);
  kyle = Actor_Get(ACTOR_FREED_KYLE);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(ACTOR_DOOM_DRAGON, 2);
  actor->motionFlags = 0;
  actor->x.fixed -= px(4);
  actor->z.fixed += px(16);
  Actor_SetAnimation(ACTOR_DRAGON_HEAD_B, 2);
  Actor_SetAnimation(ACTOR_DRAGON_HEAD_C, 3);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  Actor_SetAnimation(ACTOR_FREED_JENNAS_MOTHER, 7);
  Actor_SetAnimation(ACTOR_FREED_JENNAS_FATHER, 7);
  Actor_SetAnimation(ACTOR_FREED_KYLE, 6);
  leader = (s32)&gMainRoutineWork;
  leader += offsetof(struct MainRoutineWork, leaderActorId);
  leaderId = *(s32 *)leader;
  facing = FACING_NORTH;
  Actor_SetPositionAndFacing(leaderId, px(504), px(320), facing);
  System_Wait(1);
  PaletteFade_ApplySource(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_Interpolate(1);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_MOTHER)), 15);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_FATHER)), 15);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_KYLE)), 15);
  System_Wait(1);
  FieldActor_SetSpritePriority(ACTOR_SHEBA_STAND_IN, 1);
  FieldActor_SetSpritePriority(ACTOR_JENNA_STAND_IN, 1);
  FieldActor_SetSpritePriority(ACTOR_KRADEN, 1);
  FieldActor_SetSpritePriority(ACTOR_IVAN_STAND_IN, 1);
  FieldActor_SetSpritePriority(ACTOR_MIA_STAND_IN, 1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  FieldEvent_Wait(80);
  *(s32 *)&gMarsLighthouseAerie_FlickerFrame = 0;
  *(s32 *)&gMarsLighthouseAerie_FlickerRate = 4;
  Actor_Get(ACTOR_DOOM_DRAGON)->update =
      (void (*)(void))MarsLighthouseAerie_FlickerDragon;
  Actor_Get(ACTOR_DRAGON_HEAD_A)->update =
      (void (*)(void))MarsLighthouseAerie_FlickerDragon;
  Actor_Get(ACTOR_DRAGON_HEAD_B)->update =
      (void (*)(void))MarsLighthouseAerie_FlickerDragon;
  Actor_Get(ACTOR_DRAGON_HEAD_C)->update =
      (void (*)(void))MarsLighthouseAerie_FlickerDragon;
  mother->update = (void (*)(void))MarsLighthouseAerie_FlickerParent;
  father->update = (void (*)(void))MarsLighthouseAerie_FlickerParent;
  kyle->update = (void (*)(void))MarsLighthouseAerie_FlickerParent;
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
  *(s32 *)&gMarsLighthouseAerie_FlickerRate = 3;
  System_Wait(30);
  *(s32 *)&gMarsLighthouseAerie_FlickerRate = 2;
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
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_MOTHER)), 9);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_FATHER)), 9);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_KYLE)), 9);
  FieldEvent_Wait(20);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_MOTHER)), 0);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_JENNAS_FATHER)), 0);
  FieldActor_SetSpriteColor((Actor_Get(ACTOR_FREED_KYLE)), 0);
  FieldEvent_Wait(20);
  mother->riseCounter = (Random_Below(80)) + 40;
  father->riseCounter = (Random_Below(80)) + 40;
  kyle->riseCounter = (Random_Below(80)) + 40;
  mother->update = (void (*)(void))MarsLighthouseAerie_Throb;
  father->update = (void (*)(void))MarsLighthouseAerie_Throb;
  kyle->update = (void (*)(void))MarsLighthouseAerie_Throb;
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
  Actor_FaceDirection(*(s32 *)leader, FACING_SOUTHEAST, 0);
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
  Event_PlaySound(MUS_DOOM_DRAGON_REVEALED);
  Actor_SetAttachedEffect(ACTOR_GARET_STAND_IN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "I remember you warning us, but it was too late for us to stop..."
  Event_ShowMessage(ACTOR_GARET_STAND_IN, 0);
  // "Kraden, what do you know? Why were you trying to stop us from..."
  Event_ShowMessage(ACTOR_MIA_STAND_IN, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  // "It was the Wise One's final trap..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_IVAN_STAND_IN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Trap? What do you mean?"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN_STAND_IN, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 4);
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
  Actor_Get(ACTOR_JENNA_STAND_IN)->riseCounter = 0;
  Actor_Get(ACTOR_SHEBA_STAND_IN)->riseCounter = 0;
  Actor_SetSpeed(ACTOR_SHEBA_STAND_IN, 0.8);
  Actor_SetSpeed(ACTOR_JENNA_STAND_IN, 0.8);
  Actor_SetScript(ACTOR_SHEBA_STAND_IN,
                  (s32)gMarsLighthouseAerie_ShebaToBodiesScript);
  Actor_SetScript(ACTOR_JENNA_STAND_IN,
                  (s32)gMarsLighthouseAerie_JennaToBodiesScript);
  Actor_SetSpeed(ACTOR_KRADEN, 2.4);
  Actor_WalkToAndWait(ACTOR_KRADEN, 540, 340);
  FieldActor_Jump(ACTOR_KRADEN, 4, 10);
  // "No, Jenna! Don't look! You mustn't look! It will only bring you pain..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  actor = Actor_Get(ACTOR_SHEBA_STAND_IN);
L_0200a47a:
  System_Wait(1);
  if (actor->riseCounter == 0)
    goto L_0200a47a;
  Actor_FaceDirection(ACTOR_JENNA_STAND_IN, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_SOUTHEAST);
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
  Actor_Get(ACTOR_JENNA_STAND_IN)->steeringFlags &= 254;
  Actor_WalkToAndWait(ACTOR_JENNA_STAND_IN, 488, 300);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_JENNA_STAND_IN)->steeringFlags |= 1;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_JENNA_STAND_IN, 2);
  Actor_Get(ACTOR_JENNA_STAND_IN)->steeringFlags &= 254;
  Actor_WalkToAndWait(ACTOR_JENNA_STAND_IN, 486, 308);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_JENNA_STAND_IN)->steeringFlags |= 1;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(512, 96, 320, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_SHEBA_STAND_IN, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA_STAND_IN, FACING_NORTHEAST, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_EAST);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SHEBA_STAND_IN, 0);
  Actor_FaceDirection(ACTOR_PIERS_STAND_IN, FACING_NORTH, 0);
  leaderAgain = (s32)&gMainRoutineWork;
  leaderAgain += offsetof(struct MainRoutineWork, leaderActorId);
  Actor_FaceDirection(*(s32 *)leaderAgain, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC_STAND_IN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN_STAND_IN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA_STAND_IN, FACING_NORTHWEST, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA_STAND_IN, 0);
  actor = Actor_Get(ACTOR_FREED_KYLE);
  actor->update = NULL;
  System_Wait(1);
  actor->scaleX = 0x10000;
  actor->scaleY = 0x10000;
  Actor_RunRepeatedMotion(ACTOR_FREED_KYLE, 2);
  Event_ShowMessage(ACTOR_FREED_KYLE, 0);
  Actor_ShowEmote(ACTOR_ISAAC_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_ISAAC_STAND_IN, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_MIA_STAND_IN, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_MIA_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(ACTOR_MIA_STAND_IN, 0);
  Actor_FaceDirection(ACTOR_ISAAC_STAND_IN, FACING_EAST, 40);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC_STAND_IN, FACING_NORTH);
  FieldActor_StartRepeatedMotion(ACTOR_FREED_KYLE, 3);
  Event_ShowMessage(ACTOR_FREED_KYLE, 0);
  Actor_FaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_GARET_STAND_IN, FACING_SOUTHWEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET_STAND_IN, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_SOUTHEAST);
  Actor_SetAttachedEffect(ACTOR_SHEBA_STAND_IN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessage(ACTOR_SHEBA_STAND_IN, 0);
  Actor_FaceDirection(*(s32 *)leaderAgain, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN_STAND_IN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA_STAND_IN, FACING_NORTH, 0);
  actor = Actor_Get(ACTOR_FREED_JENNAS_MOTHER);
  actor->update = NULL;
  System_Wait(1);
  actor->scaleX = 0x10000;
  actor->scaleY = 0x10000;
  actor = Actor_Get(ACTOR_FREED_JENNAS_FATHER);
  actor->update = NULL;
  System_Wait(1);
  actor->scaleX = 0x10000;
  actor->scaleY = 0x10000;
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FREED_JENNAS_FATHER, 1);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_FREED_JENNAS_FATHER, 0);
  Actor_RunRepeatedMotion(ACTOR_FREED_JENNAS_MOTHER, 2);
  Event_ShowMessage(ACTOR_FREED_JENNAS_MOTHER, 0);
  FieldActor_Jump(ACTOR_JENNA_STAND_IN, 2, 10);
  FieldActor_Jump(ACTOR_JENNA_STAND_IN, 4, 10);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA_STAND_IN, 0);
  Actor_SetSpeed(ACTOR_JENNA_STAND_IN, 2.4);
  Actor_SetSpeed(*(s32 *)leaderAgain, 2.4);
  Actor_SetSpeed(ACTOR_ISAAC_STAND_IN, 2.4);
  Actor_SetSpeed(ACTOR_GARET_STAND_IN, 2.4);
  Actor_SetScript(ACTOR_JENNA_STAND_IN,
                  (s32)gMarsLighthouseAerie_JennaTurnScript);
  Actor_SetScript(*(s32 *)leaderAgain,
                  (s32)gMarsLighthouseAerie_LeaderTurnScript);
  Actor_SetScript(ACTOR_ISAAC_STAND_IN,
                  (s32)gMarsLighthouseAerie_IsaacTurnScript);
  Actor_RunScriptAndWait(ACTOR_GARET_STAND_IN,
                         (s32)gMarsLighthouseAerie_GaretTurnScript);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_PIERS_STAND_IN, 1.6);
  Actor_WalkToAndWait(ACTOR_PIERS_STAND_IN, 528, 320);
  Actor_FaceDirection(ACTOR_PIERS_STAND_IN, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MIA_STAND_IN, FACING_NORTHWEST);
  Event_ShowMessage(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_STAND_IN, 0);
  Actor_SetSpeed(ACTOR_MIA_STAND_IN, 1.6);
  Actor_SetScript(ACTOR_MIA_STAND_IN, (s32)gMarsLighthouseAerie_MiaTurnScript);
  Actor_WalkToAndWait(ACTOR_PIERS_STAND_IN, 486, 308);
  Actor_FaceDirectionAndWait(ACTOR_PIERS_STAND_IN, FACING_NORTHWEST);
  Actor_SetSpeed(ACTOR_SHEBA_STAND_IN, 2.4);
  Actor_Get(ACTOR_SHEBA_STAND_IN)->steeringFlags &= 254;
  Actor_WalkToAndWait(ACTOR_SHEBA_STAND_IN, 474, 316);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_SHEBA_STAND_IN)->steeringFlags |= 1;
  Actor_FaceDirection(ACTOR_SHEBA_STAND_IN, FACING_NORTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_PIERS_STAND_IN, 462, 280);
  Actor_FaceDirectionAndWait(ACTOR_PIERS_STAND_IN, FACING_EAST);
  FieldActor_SetSpritePriority(ACTOR_PIERS_STAND_IN, 1);
  MarsLighthouseAerie_StartRisingRings((Actor_Get(ACTOR_PIERS_STAND_IN)),
                                       (s32)gMarsLighthouseAerie_RisingRingsA);
  MarsLighthouseAerie_StartRisingRings((Actor_Get(ACTOR_MIA_STAND_IN)),
                                       (s32)gMarsLighthouseAerie_RisingRingsB);
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  MarsLighthouseAerie_EndRisingRings(Actor_Get(ACTOR_PIERS_STAND_IN));
  MarsLighthouseAerie_EndRisingRings(Actor_Get(ACTOR_MIA_STAND_IN));
  FieldEvent_Wait(80);
  FieldEvent_SetExitRequest(2);
  return;
}

/// The actor stands where the party will pass, no longer in the way.
void MarsLighthouseAerie_StandAside(s32 id) {
  struct Actor *object = Actor_Get(id);

  object->collisionFlags = 0;
  object->motionFlags = 0;
  FieldActor_SetSpritePriority(id, 1);
}

/// The scene that opens with MsgMarsLighthouseAerieWhatsMatterPiersWhyDid;
/// either answer to its prompt steps the event's message number.
void MarsLighthouseAerie_UrgeOnToBeacon(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 506, 280);
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldActor_SetSpritePriority(ACTOR_MIA, 1);
  MarsLighthouseAerie_StartRisingRings((s32)Actor_Get(ACTOR_PIERS),
                                       (s32)gMarsLighthouseAerie_RisingRingsA);
  MarsLighthouseAerie_StartRisingRings((s32)Actor_Get(ACTOR_MIA),
                                       (s32)gMarsLighthouseAerie_RisingRingsB);
  FieldEvent_Wait(120);
  MarsLighthouseAerie_EndRisingRings((s32)Actor_Get(ACTOR_PIERS));
  MarsLighthouseAerie_EndRisingRings((s32)Actor_Get(ACTOR_MIA));
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
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
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
  Actor_SetAnimation(ACTOR_JENNA, 0x16);
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
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  // "Even if they had won the battle, they would not have survived. You..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST, 0);
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
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  // "I knew what I was doing the moment I raised my sword."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0, 20);
  {
    s32 third = 0x28;
    s32 second = 0x105;
    s32 actor;

    actor = gMainRoutineWork.leaderActorId;
    FieldActor_ShowEmote(actor, second, third);
  }
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "We defied the Wise One in order to save the world. Our parents would..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
  {
    s32 flags = 0;

    Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, flags);
  }
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                            0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
    gFieldWork->message += 1;
  } else {
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    gFieldWork->message += 1;
    Event_ShowMessage(ACTOR_JENNA, 0);
  }
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_WalkToAndWait(ACTOR_GARET, 548, 320);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTHWEST);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 498, 320);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
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
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
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
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_HAMA), 0xf);
  Actor_SetPosition(ACTOR_HAMA, 568, 274);
  Actor_Get(ACTOR_JENNAS_MOTHER_TALK_SPOT)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(ACTOR_JENNAS_MOTHER_TALK_SPOT),
                                       px(492), px(96), px(284));
  Actor_Get(ACTOR_JENNAS_FATHER_TALK_SPOT)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(ACTOR_JENNAS_FATHER_TALK_SPOT),
                                       px(510), px(96), px(295));
  Actor_Get(ACTOR_KYLE_TALK_SPOT)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(ACTOR_KYLE_TALK_SPOT), px(540),
                                       px(96), px(300));
  System_Wait(1);
  Event_SetFlag(FLAG_CANNOT_SAVE_HERE);
  Event_SetFlag(FLAG_RETREAT_BLOCKED);
  Event_End();
}

/// An orb circles the beacon's beam as it rises.
void Orbs_UpdateRising(struct Actor *self) {
  struct Actor *centre = Actor_Get(ACTOR_BEAM_BOTTOM);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

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

/// Every third frame, or every frame once the light has broken through, an orb
/// starts to rise round the beacon's beam.
void Orbs_SpawnRising(void) {
  struct Actor *center;
  struct OrbitEffect *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(FLAG_BEACON_BROKE_THROUGH) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(ACTOR_BEAM_BOTTOM);
  if (Event_GetFlag(FLAG_BEACON_BROKE_THROUGH) != 0) {
    u32 random;

    random = Random16();
    effect = (struct OrbitEffect *)FieldActor_Create(
        0x2a0, center->x.fixed,
        (s32)((u32)(random << 8) >> 16 << 16) + center->y.fixed - px(28),
        center->z.fixed);
  } else {
    u32 random;

    random = Random16();
    effect = (struct OrbitEffect *)FieldActor_Create(
        0x2a0, center->x.fixed,
        (s32)((u32)(random << 6) >> 16 << 16) + center->y.fixed - px(28),
        center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gOrbs_RisingScript);
    FieldActor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->angle = angle;
    effect->unk66 = 0;
    effect->update = (void (*)(struct OrbitEffect *))Orbs_UpdateRising;
    effect->radius = Math_Sin((u32)(Random16() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 2;
  }
}

#include "../field/actor_park_record.c"

#include "text/msg_ids.h"

void BattleEffect_GetObjectPriority(void);
void MarsLighthouseAerie_HaltIvan(void);
void FieldCamera_WaitForArrival();
void Teleport_PlayWhirl(void);

extern s32 gMarsLighthouseAerie_BeaconFireScript[];

void Actor_StartScript(struct Actor *object, const u8 *script);

/// The beacon's fire: a flash and a quake, then it falls from above into the
/// beacon with glittering sparks.
void MarsLighthouseAerie_RaiseBeaconFire(void) {
  struct Actor *anchor = Actor_GetById(ACTOR_BEACON_FIRE);
  s32 spawn = 0;
  struct Actor *object;
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
    anchor->x.fixed = px(512);
    anchor->y.fixed = -px(360);
    anchor->z.fixed = px(204);
    anchor->scaleX = Q16_ONE;
    anchor->scaleY = Q16_ONE;
    SceneActor_ParkRecord(anchor);
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
      anchor->y.fixed += px(9) / 4;
    }
    break;
  case 26:
    gMarsLighthouseAerie_BeaconFireFrame--;
    if (anchor->y.fixed > px(20)) {
      gMarsLighthouseAerie_BeaconFireFrame = 999;
      FieldTask_Remove(MarsLighthouseAerie_RaiseBeaconFire);
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(SE_GLITTER);
      anchor->y.fixed += px(9) / 4;
      spawn = 1;
    }
    break;
  }
  if (spawn) {
    object = FieldActor_Create(SPRITE_PARTICLE, anchor->x.fixed,
                               anchor->y.fixed - ((Random_Below(80)) << 16) -
                                   0x80000,
                               anchor->z.fixed);
    if (object != NULL) {
      u8 zero;
      s32 mask;

      sprite = object->sprite;
      Actor_StartScript(object, gMarsLighthouseAerie_BeaconSparkScript);
      FieldActor_SetSpriteColor(object, 1);
      object->motionFlags = 0;
      angle = Random16() & 0xffff000;
      object->timer = angle;
      zero = 0;
      object->userValue = 0;
      phase = Random16() >> 13;
      object->riseCounter = phase;
      object->update = MarsLighthouseAerie_OrbitBeaconFire;
      object->speedLimit = Math_Sin(Random16() * 0xffff >> 20) * 24;
      object->speedLimit = anchor->speedLimit >> 16;
      sprite->unk1a = zero;
      mask = -13;
      ((u8 *)sprite)[9] = (((u8 *)sprite)[9] & mask) | 4;
    }
  }
  if (++gMarsLighthouseAerie_BeaconFireFrame >
      gMarsLighthouseAerie_BeaconFireFrames)
    FieldTask_Remove(MarsLighthouseAerie_RaiseBeaconFire);
}

/// The actor steps back from the beam, to the left.
void MarsLighthouseAerie_StepBackLeft(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(object, object->x.fixed - px(16),
                                       object->y.fixed,
                                       object->z.fixed + px(32));
}

/// The actor steps back from the beam, to the right.
void MarsLighthouseAerie_StepBackRight(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(object, object->x.fixed + px(16),
                                       object->y.fixed,
                                       object->z.fixed + px(32));
}

/// The actor keeps to the ground as the beacon's beam rises.
void MarsLighthouseAerie_HoldStill(s32 id) {
  Actor_GetById(id)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
}

void MarsLighthouseAerie_StartLookingAround(s32 id);
void MarsLighthouseAerie_StopLookingAround(s32 id);
extern s32 gMarsLighthouseAerie_BeamNarrowScript[];

void MarsLighthouseAerie_LightBeacon(void) {
  struct AerieEventWork *work, *later;
  struct OverlaySprite *sprite;
  struct Actor *created, *object;
  s32 zero, firstBeam, tiles, glowTask, message;
  struct Actor *anchor, *spawned;
  s32 y, x, z, block;
  struct Actor *bottomBeam, *lowerBeam, *middleBeam;
  struct Actor *upperBeam, *topBeam;
  s32 owner;
  struct Actor *actor;

  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  Actor_SetPosition(ACTOR_HAMA, 0, 0);
  Actor_SetPosition(ACTOR_JENNAS_FATHER_TALK_SPOT, 0, 0);
  Actor_SetPosition(ACTOR_KYLE_TALK_SPOT, 0, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(512, 96, 312, 1);
  FieldCamera_WaitForArrival();
  System_Wait(1);
  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = 0x3f10;
  MapGrid_CopyCellAttributes(18, 40, 18, 16, 23, 10);
  MarsLighthouseAerie_HoldStill(ACTOR_PIERS);
  MarsLighthouseAerie_HoldStill(ACTOR_JENNA);
  MarsLighthouseAerie_HoldStill(ACTOR_SHEBA);
  firstBeam = 23;
  zero = 0;
  MarsLighthouseAerie_HoldStill(ACTOR_KRADEN);
  MarsLighthouseAerie_HoldStill(ACTOR_ISAAC);
  MarsLighthouseAerie_HoldStill(ACTOR_MIA);
  MarsLighthouseAerie_HoldStill(ACTOR_IVAN);
  MarsLighthouseAerie_HoldStill(ACTOR_GARET);
  Event_PrepareActorAndApplyValue(0, 0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 512, 280);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  Actor_RunRepeatedMotion(gMainRoutineWork.leaderActorId, 2);
  FieldEvent_Wait(20);
  PartyInventory_Remove(ITEM_MYTHRIL_BAG_MARS_STAR);
  anchor = Actor_Get(ACTOR_FELIX);
  y = anchor->y.fixed;
  x = anchor->x.fixed;
  y += 0x80000;
  z = anchor->z.fixed;
  spawned = FieldActor_Create(0x1e9, x, y, z);
  created = spawned;
  object = spawned;
  if (created != 0) {
    sprite = (struct OverlaySprite *)object->sprite;
    sprite->unk5_5 = 0;
    sprite->unk1b = zero;
    sprite->unk9_4 = 0;
    sprite->unk9_2 = 2;
    object->priorityFlags &= 254;
    object->motionFlags = zero;
    object->retainSpriteCache = 1;
    object->priorityFlags |= 2;
    Actor_SetSpriteFlags(created, 0);
    object->speedLimit = 0x6666;
    object->acceleration = 0xccc;
    object->gravity = px(1) / 2;
    block = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Item_LoadIcon(ITEM_MARS_STAR);
    tiles = (block + 0x400);
    Vram_Load(sprite->vramBlock, 128, tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
    object->motionFlags = zero;
    FieldActor_SetMoveTarget(object, 512, 128, 280);
    FieldActor_WaitForMove(object);
    object->motionFlags = ACTOR_FLOATS;
    FieldEvent_Wait(60);
    object->motionFlags = zero;
    FieldActor_SetMoveTarget(object, 512, 128, 264);
    FieldActor_WaitForMove(object);
    FieldEvent_Wait(20);
    FieldActor_SetMoveTarget(object, 512, 80, 252);
    FieldActor_WaitForMove(object);
    FieldEvent_Wait(20);
    Actor_Release(object);
  }
  FieldEvent_Wait(40);
  FieldActor_SetSpritePriority(ACTOR_BEAM_BOTTOM, 2);
  FieldActor_SetSpritePriority(ACTOR_BEAM_LOWER, 2);
  FieldActor_SetSpritePriority(ACTOR_BEAM_MIDDLE, 2);
  FieldActor_SetSpritePriority(ACTOR_BEAM_UPPER, 2);
  FieldActor_SetSpritePriority(ACTOR_BEAM_TOP, 2);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(1, 1, 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_BOTTOM), 7);
  bottomBeam = Actor_Get(ACTOR_BEAM_BOTTOM);
  bottomBeam->scaleX = 0x1999;
  bottomBeam->scaleY = 0xffff0000;
  bottomBeam->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(bottomBeam, px(512), px(64), px(204));
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_LOWER), 7);
  lowerBeam = Actor_Get(ACTOR_BEAM_LOWER);
  lowerBeam->scaleX = 0x1999;
  lowerBeam->scaleY = 0xffff0000;
  lowerBeam->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(lowerBeam, px(512), px(96), px(204));
  Task_AddFrame(Orbs_SpawnRising, TASK_PASS_VBLANK);
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
  Actor_Get(ACTOR_BEAM_BOTTOM)->y.fixed = -px(56);
  Actor_Get(ACTOR_BEAM_LOWER)->y.fixed = -px(24);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_MIDDLE), 7);
  middleBeam = Actor_Get(ACTOR_BEAM_MIDDLE);
  middleBeam->scaleY = 0xffff0000;
  middleBeam->scaleX = (Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX);
  middleBeam->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(middleBeam, px(512), px(8), px(204));
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_UPPER), 7);
  upperBeam = Actor_Get(ACTOR_BEAM_UPPER);
  upperBeam->scaleY = 0xffff0000;
  upperBeam->scaleX = (Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX);
  upperBeam->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(upperBeam, px(512), px(40), px(204));
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_TOP), 7);
  topBeam = Actor_Get(ACTOR_BEAM_TOP);
  topBeam->scaleY = 0xffff0000;
  topBeam->scaleX = (Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX);
  topBeam->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(topBeam, px(512), px(72), px(204));
  MapGrid_CopyCellsTo(106, 36, 87, 7, 17, firstBeam);
  MapGrid_CopyCellsTo(42, 98, 23, 69, 17, firstBeam);
  MapGrid_CopyCellAttributes(0, 40, 18, 20, firstBeam, 10);
  MarsLighthouseAerie_StepBackLeft(ACTOR_PIERS);
  MarsLighthouseAerie_StepBackLeft(ACTOR_JENNA);
  MarsLighthouseAerie_StepBackLeft(ACTOR_SHEBA);
  glowTask = (s32)Orbs_SpawnRising;
  MarsLighthouseAerie_StepBackLeft(ACTOR_JENNAS_MOTHER);
  MarsLighthouseAerie_StepBackLeft(ACTOR_JENNAS_FATHER);
  MarsLighthouseAerie_StepBackLeft(ACTOR_KRADEN);
  MarsLighthouseAerie_StepBackRight(ACTOR_IVAN);
  MarsLighthouseAerie_StepBackRight(ACTOR_GARET);
  MarsLighthouseAerie_StepBackRight(ACTOR_KYLE);
  MarsLighthouseAerie_StepBackRight(ACTOR_ISAAC);
  MarsLighthouseAerie_StepBackRight(ACTOR_MIA);
  owner = gMainRoutineWork.leaderActorId;
  {
    s32 facing = FACING_NORTHEAST;
    Actor_SetPositionAndFacing(owner, px(484), px(312), facing);
  }
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
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
  Actor_Get(ACTOR_BEAM_BOTTOM)->scaleY = 0x51e;
  Actor_SetScript(ACTOR_BEAM_LOWER, gMarsLighthouseAerie_BeamShootUpScript);
  Actor_SetScript(ACTOR_BEAM_MIDDLE, gMarsLighthouseAerie_BeamShootUpScript);
  Actor_SetScript(ACTOR_BEAM_UPPER, gMarsLighthouseAerie_BeamShootUpScript);
  Actor_RunScriptAndWait(ACTOR_BEAM_TOP,
                         gMarsLighthouseAerie_BeamShootUpScript);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_BOTTOM), 15);
  FieldEvent_Wait(20);
  Actor_RunScriptAndWait(ACTOR_BEAM_BOTTOM,
                         gMarsLighthouseAerie_BeamRiseScript);
  FieldTask_Remove(glowTask);
  actor = Actor_Get(ACTOR_BEACON_FIRE);
  actor->motionFlags = zero;
  Actor_SetBlendMode(actor, 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEACON_FIRE), 7);
  FieldActor_SetSpritePriority(ACTOR_BEACON_FIRE, 2);
  *(s32 *)(s32)&gMarsLighthouseAerie_BeaconFireFrame = zero;
  *(s32 *)(s32)&gMarsLighthouseAerie_BeaconFireFrames = 240;
  Task_AddFrame(MarsLighthouseAerie_RaiseBeaconFire, TASK_PASS_VBLANK);
L_0200bcea:
  System_Wait(1);
  if ((u32)(*(s32 *)(s32)&gMarsLighthouseAerie_BeaconFireFrame) <= (u32)100)
    goto L_0200bcea;
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
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 3);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
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
  Message_ShowCentered(((s32)MsgMarsLighthouseAerieBeaconIsLit + 6), 1);
  FieldPsynergy_StopMindReadWave();
  FieldEvent_Wait(20);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  Message_ShowCentered(((s32)MsgMarsLighthouseAerieBeaconIsLit + 7), 1);
  FieldPsynergy_StopMindReadWave();
  work = Ram_HeapSlots->fieldWork;
  work->step.value = work->stepRead + 2;
  MarsLighthouseAerie_StartLookingAround(gMainRoutineWork.leaderActorId);
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
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 0, 0, 10);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  message = ((s32)MsgMarsLighthouseAerieBeaconIsLit + 9);
  Message_ShowCentered(message, 1);
  FieldPsynergy_StopMindReadWave();
  MarsLighthouseAerie_StopLookingAround(ACTOR_MIA);
  Actor_FaceDirectionAndWait(24579, FACING_SOUTH);
  ((struct OverlayActor *)Actor_Get(ACTOR_MIA))->update = NULL;
  // Read as a word, so the store to the actor before it may alias.
  later =
      (struct AerieEventWork *)((struct HeapWords *)Ram_HeapSlots)->eventWord;
  later->stepRead = later->stepRead + 1;
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Event_PlaySound(SE_ASCEND);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(20);
  System_Wait(20);
  FieldEvent_SetExitRequest(3);
  return;
}

void MarsLighthouseAerie_SpawnBeaconFlames(void);

void MarsLighthouseAerie_WatchGoldenSunRise(void) {
  s32 *leader, *leaderInLoop, *leaderAfterLoop, i, south;
  s32 southSoutheast, actor;
  s32 t7;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    leader = &party->leaderActorId;
  }
  south = FACING_SOUTH;
  ((struct Actor *)(Actor_Get(*leader)))->facing = south;
  ((struct Actor *)Actor_Get(ACTOR_PIERS))->facing = south;
  ((struct Actor *)Actor_Get(ACTOR_JENNA))->facing = south;
  ((struct Actor *)Actor_Get(ACTOR_SHEBA))->facing = south;
  t7 = Actor_Get(ACTOR_KRADEN);
  southSoutheast = FACING_SOUTH_SOUTHEAST;
  ((struct Actor *)t7)->facing = southSoutheast;
  ((struct Actor *)Actor_Get(ACTOR_ISAAC))->facing = south;
  ((struct Actor *)Actor_Get(ACTOR_GARET))->facing = south;
  ((struct Actor *)Actor_Get(ACTOR_IVAN))->facing = south;
  System_Wait(1);
  Actor_SetAnimation(ACTOR_JENNAS_MOTHER, 8);
  Actor_SetAnimation(ACTOR_JENNAS_FATHER, 8);
  Actor_SetAnimation(ACTOR_KYLE, 7);
  Actor_SetPosition(ACTOR_KRADEN, 498, 320);
  Actor_SetPosition(ACTOR_GARET, 538, 320);
  Actor_SetPosition(*leader, 506, 280);
  System_Wait(1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  ((struct Actor *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_MoveTo(512, 96, 288, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(*leader, FACING_EAST, 0);
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
  Actor_FaceDirection(ACTOR_PIERS, south, 0);
  Actor_FaceDirection(*leader, south, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 320, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  // "The Wise One said that when all four lighthouses have been lit, the..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(*leader, 3);
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "Why would he do that!? You saw what he did to us... He can't be up..."
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_HAMA), 15);
  Actor_SetPosition(ACTOR_HAMA, 512, 360);
  System_Wait(1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "Have you not learned? One's actions do not always reveal one's true..."
  Event_ShowMessage(ACTOR_HAMA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirection(ACTOR_IVAN, south, 0);
  FieldActor_Jump(ACTOR_IVAN, 4, 10);
  // "Hamma... Sister..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(*leader, south, 0);
  Actor_FaceDirection(ACTOR_PIERS, south, 0);
  Actor_FaceDirection(ACTOR_JENNA, south, 0);
  Actor_FaceDirection(ACTOR_SHEBA, south, 0);
  Actor_FaceDirection(ACTOR_KRADEN, southSoutheast, 0);
  Actor_FaceDirection(ACTOR_GARET, south, 0);
  Actor_FaceDirection(ACTOR_MIA, south, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, south);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Master Hamma! Did you receive a message, too?"
  Event_ShowMessage(ACTOR_ISAAC, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "Yes. I was called to Jupiter Lighthouse in a dream."
  Event_ShowMessage(ACTOR_HAMA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirection(*leader, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 20);
  Actor_SetAnimation(*leader, 3);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_FaceDirection(*leader, south, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, south);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "You have completed your quest, but I see it comes with great loss......"
  Event_ShowMessage(ACTOR_HAMA, 0);
  FieldPsynergy_StopMindReadWave();
  actor = Actor_Get(ACTOR_JENNA);
  ((struct Actor *)actor)->facing = FACING_SOUTHWEST;
  Actor_SetAnimation(ACTOR_JENNA, 22);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 2);
  // "Master Hamma... My parents..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "I know, Jenna, and I am sorry. But do not give up hope for them just..."
  Event_ShowMessage(ACTOR_HAMA, 0);
  FieldPsynergy_StopMindReadWave();
  ((struct Actor *)actor)->facing = south;
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "What do you mean, Master Hamma?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "Appearances can be an illusion... The Wise One has a caring heart."
  Event_ShowMessage(ACTOR_HAMA, 0);
  FieldPsynergy_StopMindReadWave();
  // "He forced us to fight Isaac's and Jenna's parents! What's so..."
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "If the Wise One were truly evil, he would not be warning me of..."
  Event_ShowMessage(ACTOR_HAMA, 0);
  FieldPsynergy_StopMindReadWave();
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Well, I... I guess not... But then, what's he up to?"
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "We do not have time to discuss it right now."
  Event_ShowMessage(ACTOR_HAMA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "She's right. If the Wise One said it's not safe to be near the..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
  // "I am heading for safety, too. All of you must get away from the..."
  Event_ShowMessage(ACTOR_HAMA, 0);
  FieldPsynergy_StopMindReadWave();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 296, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, southSoutheast, 40);
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
  Actor_FaceDirectionAndWait(*leader, FACING_EAST);
  FieldEvent_AskYesNoWithActorAnimation(*leader, 0);
  Actor_WaitForAnimationChange(*leader);
  PaletteFade_ApplyTarget(PALETTE_TINT_MUTED, PALETTES_BG);
  PaletteFade_Interpolate(40);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  MarsLighthouseAerie_StartLookingAround(*leader);
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
  FieldTask_Remove((s32)MarsLighthouseAerie_SpawnBeaconFlames);
  Event_PlaySound(SE_SPIN);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEACON_FIRE), 12);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEACON_GLOW), 12);
  FieldActor_SetSpritePriority(ACTOR_BEAM_BOTTOM, 2);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEAM_BOTTOM), 7);
  actor = Actor_Get(ACTOR_BEAM_BOTTOM);
  ((struct Actor *)actor)->scaleY = 0xffff0000;
  ((struct Actor *)actor)->scaleX = 0x1999;
  ((struct Actor *)actor)->motionFlags = 0;
  ((struct Actor *)actor)->priorityFlags |= 2;
  FieldActor_SetPositionAndResetMotion(actor, px(512), px(64), px(204));
  i = 0;
L_0200c66c:
  if (i == 20) {
    MapGrid_SetShake(2, 2, 1);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;
      leaderInLoop = &party->leaderActorId;
    }
    FieldActor_ShowEmote(*leaderInLoop, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    MarsLighthouseAerie_StopLookingAround(*leaderInLoop);
    MarsLighthouseAerie_StopLookingAround(ACTOR_JENNA);
    MarsLighthouseAerie_StopLookingAround(ACTOR_SHEBA);
    MarsLighthouseAerie_StopLookingAround(ACTOR_PIERS);
    MarsLighthouseAerie_StopLookingAround(ACTOR_KRADEN);
    MarsLighthouseAerie_StopLookingAround(ACTOR_ISAAC);
    MarsLighthouseAerie_StopLookingAround(ACTOR_GARET);
    MarsLighthouseAerie_StopLookingAround(ACTOR_IVAN);
    MarsLighthouseAerie_StopLookingAround(ACTOR_MIA);
  }
  ((struct Actor *)actor)->scaleX = ((((struct Actor *)actor)->scaleX) + 0x400);
  System_Wait(1);
  i = (i + 1);
  if ((u32)i <= (u32)39)
    goto L_0200c66c;
  Event_PlaySound(MUS_BEACON_LIT);
  Actor_SetScript(ACTOR_BEAM_BOTTOM,
                  (s32)gMarsLighthouseAerie_WatchGoldenSunRiseScript);
  FieldEvent_Wait(80);
  FieldActor_Jump(ACTOR_GARET, 4, 10);
  FieldActor_Jump(ACTOR_GARET, 6, 10);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHWEST);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    leaderAfterLoop = &party->leaderActorId;
  }
  Actor_FaceDirection(*leaderAfterLoop, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_FaceDirection(*leaderAfterLoop, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_WEST);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirection(*leaderAfterLoop, FACING_EAST, 0);
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
  Actor_SetAnimation(*leaderAfterLoop, 3);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 312, 1);
  Actor_SetPosition(ACTOR_BEACON_FIRE, 0, 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BEACON_GLOW), 7);
  FieldActor_SetSpritePriority(ACTOR_BEACON_GLOW, 1);
  ((struct Actor *)Actor_Get(ACTOR_BEACON_GLOW))->priorityFlags &= 253;
  Event_PlaySound(SE_SPIN);
  MapGrid_SetShake(3, 3, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
  PaletteFade_Interpolate(40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(*leaderAfterLoop, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 80);
  FieldActor_Jump(ACTOR_GARET, 4, 0);
  Actor_SetSpeed(ACTOR_GARET, 3.2);
  Actor_Get(ACTOR_GARET)->steeringFlags &= 254;
  Actor_WalkToAndWait(ACTOR_GARET, 542, 328);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_GARET)->steeringFlags |= 1;
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
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldActor_Jump(ACTOR_JENNA, 4, 10);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  FieldActor_Jump(ACTOR_KRADEN, 2, 10);
  FieldActor_Jump(ACTOR_KRADEN, 4, 10);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_PlaySound(SE_STOP_MUSIC_PLAYER_5);
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
  FieldEvent_SetExitRequest(4);
  return;
}

void MarsLighthouseAerie_HoldBackFromParents(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  FieldEvent_SetMessageId(MsgMarsLighthouseAerieDoNotLetYourSadness);
  // "Do not let your sadness overwhelm you, Felix! Go! Once the beacon..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 480, 348);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Event_End();
}

void MarsLighthouseAerie_HoldBackFromParentsAgain(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  FieldEvent_SetMessageId(MsgMarsLighthouseAerieDoNotLetYourSadness);
  // "Do not let your sadness overwhelm you, Felix! Go! Once the beacon..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 544, 348);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Event_End();
}

void MarsLighthouseAerie_OfferToCastMarsStar(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(ACTOR_VOICE, 512, 232);
  System_Wait(1);
  FieldEvent_SetMessageId(MsgMarsLighthouseAerieWillYouCastInMars);
  // "Will you cast in the Mars Star?"
  Event_OpenMessage(ACTOR_VOICE, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
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

/// Stepping back from the beacon: the voice will ask again.
void MarsLighthouseAerie_ClearCastOffer(void) {
  GameFlag_Clear(FLAG_ASKED_FOR_MARS_STAR);
}

// From patch 1, as is MarsLighthouseAerie_EndTalkWhileHalted.
#if TLA_PATCH_1
/// From patch 1: talking to the one Halt holds keeps the hold during the talk.
void MarsLighthouseAerie_BeginTalkWhileHalted(s32 id) {
  u8 *work = Ram_HeapSlots->windowWork;

  if (gMainRoutineWork.haltedActor == id)
    work[0x138a] = 1;
}
#endif

// From patch 1; the Japanese overlay has no such function.
#if TLA_PATCH_1

/// A halfword held in its own local: its load, the pool's mask and its store
/// keep their halfword forms.
struct HeldHalfword {
  u16 value;
};

/// From patch 1: the talk over, Halt's hold on its speaker ends.
void MarsLighthouseAerie_EndTalkWhileHalted(s32 id) {
  u8 *work = Ram_HeapSlots->windowWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  s32 held = party->haltedActor;
  /* FAKEMATCH(temporary): the word held in a halfword struct. */
  struct HeldHalfword raw = *(struct HeldHalfword *)&party->haltedActor;
  struct HeldHalfword ones;

  if (held == id) {
    s32 zero = 0;

    work[0x138a] = zero;
    ones.value = 0xffff;
    party->haltedActor = raw.value | ones.value;
    Actor_Get(held)->update = (void *)zero;
    FieldActor_SetSpriteColor(Actor_Get(held), 0);
  }
}

#endif

void MarsLighthouseAerie_TalkToIvan(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  MarsLighthouseAerie_BeginTalkWhileHalted(ACTOR_IVAN);
#endif
  Actor_FaceActor(ACTOR_IVAN, gMainRoutineWork.leaderActorId, 0);
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

void MarsLighthouseAerie_TalkToJennasMother(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  MarsLighthouseAerie_BeginTalkWhileHalted(ACTOR_JENNAS_MOTHER);
#endif
  FieldEvent_SetMessageId(MsgUhhhhnnnICan);
  // "Uhhhhnnn... Felix... I...can't see you, but I sense that you're..."
  Event_ShowMessage(ACTOR_JENNAS_MOTHER, 0);
#if TLA_PATCH_1
  MarsLighthouseAerie_EndTalkWhileHalted(ACTOR_JENNAS_MOTHER);
#endif
  Event_End();
}

void MarsLighthouseAerie_TalkToKyle(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  MarsLighthouseAerie_BeginTalkWhileHalted(ACTOR_KYLE);
#endif
  FieldEvent_SetMessageId(MsgKoffKoffYou);
  // "*Koff! Koff!* Felix... You made it back...alive... Please, I must..."
  Event_ShowMessage(ACTOR_KYLE, 0);
#if TLA_PATCH_1
  MarsLighthouseAerie_EndTalkWhileHalted(ACTOR_KYLE);
#endif
  Event_End();
}

void MarsLighthouseAerie_TalkToJennasFather(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  MarsLighthouseAerie_BeginTalkWhileHalted(ACTOR_JENNAS_FATHER);
#endif
  FieldEvent_SetMessageId(MsgUnnnnIAm);
  // "Unnnn... Felix? ...Jenna? I am so happy...to have one last chance to..."
  Event_ShowMessage(ACTOR_JENNAS_FATHER, 0);
#if TLA_PATCH_1
  MarsLighthouseAerie_EndTalkWhileHalted(ACTOR_JENNAS_FATHER);
#endif
  Event_End();
}

// From patch 1; the Japanese overlay has no such function.
#if TLA_PATCH_1

/// From patch 1: Halt on one of the three holds the speaker beside them.
void MarsLighthouseAerie_HaltSpeaker(s32 spot, s32 speaker) {
  struct Actor *found = Actor_Get(spot);
  struct Actor *object;
  struct Actor *target;
  struct MainRoutineWork *party = &gMainRoutineWork;

  party->pausedPose = 0;
  object = found;
  party->haltedActor = speaker;
  target = Actor_Get(speaker);
  target->update = object->update;
  object->update = NULL;
}
#endif

// These four, their rows, and MarsLighthouseAerie_HaltSpeaker are from patch 1.
#if TLA_PATCH_1

void MarsLighthouseAerie_HaltIvan(void) {
  MarsLighthouseAerie_HaltSpeaker(ACTOR_HAMA, ACTOR_IVAN);
}

void MarsLighthouseAerie_HaltJennasMother(void) {
  MarsLighthouseAerie_HaltSpeaker(ACTOR_JENNAS_MOTHER_TALK_SPOT,
                                  ACTOR_JENNAS_MOTHER);
}

void MarsLighthouseAerie_HaltKyle(void) {
  MarsLighthouseAerie_HaltSpeaker(ACTOR_KYLE_TALK_SPOT, ACTOR_KYLE);
}

void MarsLighthouseAerie_HaltJennasFather(void) {
  MarsLighthouseAerie_HaltSpeaker(ACTOR_JENNAS_FATHER_TALK_SPOT,
                                  ACTOR_JENNAS_FATHER);
}
#endif

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->paletteFade;

  gPaletteCycle_Slots = slots;
  gPaletteCycle_FlashColors = colours;
  gPaletteCycle_Colors = a;
  gPaletteCycle_Order = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  if (gMainRoutineWork.teleportState == 2)
    Teleport_ArriveWhirling();
}

void Teleport_StartShimmer(void) {
  gPaletteCycle_Step = 0;
  gPaletteCycle_Count = List_CountEntries(gPaletteCycle_Slots);
  gPaletteCycle_Period = 15;
  gPaletteCycle_Countdown = 16;
  gPaletteCycle_Flash = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

void Teleport_StopShimmer(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldTask_Remove(PaletteCycle_Fade);
}

/// Starts the circle's quick shimmer.
void Teleport_StartFastShimmer(void) {
  gPaletteCycle_Step = 0;
  gPaletteCycle_Count = List_CountEntries(gPaletteCycle_Slots);
  gPaletteCycle_Period = 2;
  gPaletteCycle_Countdown = 2;
  gPaletteCycle_Flash = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

#include "shared/list_count_entries.h"

void PaletteCycle_Fade(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gPaletteCycle_Countdown != 0) {
    if (--gPaletteCycle_Countdown == 0) {
      step = 1;
      gPaletteCycle_Countdown = gPaletteCycle_Period;
      if (gPaletteCycle_Period > 2)
        gPaletteCycle_Period--;
    }
  }
  if (step == 0)
    return;
  src = gPaletteCycle_Colors;
  for (i = 0; i < gPaletteCycle_Count; i++) {
    dst =
        &BG_PLTT[gPaletteCycle_Order[gPaletteCycle_Step * gPaletteCycle_Count +
                                     i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    SET_RGB(*dst, red, green, blue);
  }
  dst = &BG_PLTT[gPaletteCycle_Slots[gPaletteCycle_Count]];
  if (gPaletteCycle_Flash != 0)
    src = gPaletteCycle_FlashColors + gPaletteCycle_Count * 3;
  else
    src = gPaletteCycle_Colors + gPaletteCycle_Count * 3;
  red = *src++;
  green = *src++;
  blue = *src++;
  SET_RGB(*dst, red, green, blue);
  gPaletteCycle_Flash ^= 1;
  if (++gPaletteCycle_Step == gPaletteCycle_Count)
    gPaletteCycle_Step = 0;
}

/// Arriving by Teleport: the screen comes up from white as the circle whirls.
void Teleport_ArriveWhirling(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Teleport_StartFastShimmer();
  Teleport_PlayWhirl();
  System_Wait(40);
  Teleport_StopShimmer();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

/// Rings of light rise round the actor, two at a time, until told to end.
void MarsLighthouseAerie_UpdateRisingRings(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedActor;
  u8 *count = (u8 *)object + 99;
  struct SparkleSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (((u8 *)object)[98] == 0) {
    if ((*count & 15) == 0) {
      Event_PlaySound(SE_RISING_RINGS);
      work->unka0[work->unka6] = 0;
      work->unka6 ^= 1;
    }
  } else if (((u8 *)object)[98] == 1) {
    if (work->unka0[0] > 31 && work->unka0[1] > 31) {
      Actor_SetSpriteColorMode(object, 0);
      VramCache_FreeEntry(work->vramEntry);
      object->update = NULL;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Math_Sin(work->unka0[i] << 10);
    if (work->unka0[i] >= 0) {
      if (work->unka0[i] <= 31) {
        struct Vector3 *p = &position;

        p->x = object->x.fixed;
        p->y = object->y.fixed + (work->unka0[i] << 16);
        p->z = object->z.fixed;
        Camera_WorldToScreen(p);
        x = p->x;
        j = 0;
        z = p->z;
        sprite = &work->sprites[i * 2];
        for (; j <= 1; j++) {
          sprite->unk0c = x;
          sprite->unk10 = z;
          sprite->unk14 = sine;
          sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) - 1;
          } else {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) + 1;
            sprite->unk10 -= 0x10000;
            sprite->unk18 = -sprite->unk18;
          }
          EffectSprite_Draw(sprite);
          sprite++;
        }
        work->unka0[i]++;
      }
    }
  }
}
#undef A

/* VIEW(s53e4): tag struct SparkleSprite as struct SparkleSprite__s53e4; s3370.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_aerie.c:2992: structure has no member
 * named `flag'. */
struct SparkleSprite__s53e4 {
  u8 unk00[5];
  u8 attr5Low : 5;
  u8 flag : 1;
  u8 attr5High : 2;
  u8 unk06[2];
  u8 unk08;
  u8 unk09 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[0x1e];
};

/* VIEW(s53e4): tag struct SparkleWork as struct SparkleWork__s53e4; s3370.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_aerie.c:2983:
 * incompatible types in assignment. */
struct SparkleWork__s53e4 {
  struct SparkleSprite__s53e4 sprites[4];
  u16 unka0;
  u16 unka2;
  s16 vramEntry;
  u16 unka6;
  struct Actor *object;
};

/* VIEW(s3370): routine BattleEffect_GetObjectPriority as s32
 * BattleEffect_GetObjectPriority(struct Actor *object); scene/scene.h
 * has void BattleEffect_GetObjectPriority(void); tried as the definition's s32
 * BattleEffect_GetObjectPriority(struct Actor *object), which the object
 * already declares, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_aerie.c:3002: too many arguments to
 * function `BattleEffect_GetObjectPriority'. */
extern s32 BattleEffect_GetObjectPriority__s3370(struct Actor *object) __asm__(
    "BattleEffect_GetObjectPriority");

/// Rings of light begin to rise round the actor.
void MarsLighthouseAerie_StartRisingRings(struct Actor *object,
                                          struct SparkleWork__s53e4 *work) {
  struct SparkleSprite__s53e4 *sprite;
  s32 tile;
  s32 row;
  s32 col;
  s32 offset;

  work->vramEntry = VramCache_FindFreeEntry();
  Vram_Load(work->vramEntry, 256, gMarsLighthouseAerie_RisingRingTiles);
  work->unka6 = 0;
  work->unka0 = 999;
  work->unka2 = 999;
  work->object = object;
  ((u8 *)object)[0x62] = 0;
  ((u8 *)object)[0x63] = 0;
  object->update = MarsLighthouseAerie_UpdateRisingRings;
  tile = gVramCacheEntries[work->vramEntry].offset >> 5;
  object->watchedActor = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    col = 1;
    offset = (row * 2) * sizeof(struct SparkleSprite__s53e4);
    sprite = (struct SparkleSprite__s53e4 *)(offset + (s32)work);
    for (; col >= 0; col--) {
      EffectSprite_Init(sprite, 16, 16, 0x80004000, tile);
      sprite->flag = 0;
      sprite->palette = 0;
      sprite->priority = BattleEffect_GetObjectPriority__s3370(object) & 3;
      sprite++;
    }
  }
}

/// The rings round the actor rise out and stop.
void MarsLighthouseAerie_EndRisingRings(struct Actor *object) {
  object->riseCounter = 1;
}

#include "text/msg_ids.h"

void Scene_LeaveByExit(void);
void MarsLighthouseAerie_FaceDoomDragon(void);
void MarsLighthouseAerie_HaltKyle(void);

// clang-format off
s32 gMarsLighthouseAerie_RisingRingTiles[] = {
    0, 0, 0, 0, 0, 0,
    0x70000000, (s32)0xf7700000, 0, 0, 0, 0,
    0x77770000, (s32)0xffff7770, -9, 0x7777ffff, 0, 0,
    0, 0, 0x7777, 0x777ffff, 0x7fffffff, (s32)0xffff7777,
    0, 0, 0, 0, 0, 0,
    7, 0x77f, -0x90000, -0x9000, 0x7ffff700, 0x7ffff70,
    0x7fff70, 0x7ffff7, 0x7fff7, 0x7fff7, 0x77ff, 119,
    0, 0, 0, 0, 0, 0,
    -0x890000, 0x77000000, 0, 0, 0, 0,
    0, 0, 0x7fff, 0x7ffff, 0x7ffff7, 0x7ffff70,
    0x7fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000, 0, 0,
    0, 0, 0, 0, 0x40000000, (s32)0xf4400000,
    0, 0, 0, 0, 0x44440000, (s32)0xffff4440,
    -12, 0x4444ffff, 0, 0, 0, 0,
    0x4444, 0x444ffff, 0x4fffffff, (s32)0xffff4444, 0, 0,
    0, 0, 0, 0, 4, 0x44f,
    -0xc0000, -0xc000, 0x4ffff400, 0x4ffff40, 0x4fff40, 0x4ffff4,
    0x4fff4, 0x4fff4, 0x44ff, 68, 0, 0,
    0, 0, 0, 0, -0xbc0000, 0x44000000,
    0, 0, 0, 0, 0, 0,
    0x4fff, 0x4ffff, 0x4ffff4, 0x4ffff40, 0x4fff400, 0x4ffff400,
    0x4fff4000, 0x4fff4000, (s32)0xe9000000, 233, (s32)0xeb000000, 235,
    (s32)0xedeb0000, 0xebed, (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xedeb0000, 0xebed, (s32)0xeb000000, 235, (s32)0xe9000000, 233,
    0xe900, 0xe90000, 0xebe9e9, (s32)0xe9e9eb00, (s32)0xedebeb00, 0xebebed,
    (s32)0xefed0000, 0xedef, (s32)0xefed0000, 0xedef, (s32)0xedebeb00, 0xebebed,
    0xebe9e9, (s32)0xe9e9eb00, 0xe900, 0xe90000, (s32)0xe9000000, 233,
    (s32)0xebe90000, 0xe9eb, (s32)0xedebe900, 0xe9ebed, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xedebe900, 0xe9ebed, (s32)0xebe90000, 0xe9eb,
    (s32)0xe9000000, 233,
};

s16 gMarsLighthouseAerie_FadeSlots[] = {
    137, 138, 139, 140, 141, 142, 143, -1,
};

s16 gMarsLighthouseAerie_FadeColorsFrom[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gMarsLighthouseAerie_FadeColorsTo[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gMarsLighthouseAerie_FadeOrder[] = {
    0x89008e, 0x8b008a, 0x8d008c, 0x8e008d, 0x8a0089, 0x8c008b,
    0x8d008c, 0x89008e, 0x8b008a, 0x8c008b, 0x8e008d, 0x8a0089,
    0x8b008a, 0x8d008c, 0x89008e, 0x8a0089, 0x8c008b, 0x8e008d,
};

s32 gMarsLighthouseAerie_BeaconFireScript[] = {                                                               
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

u32 gMarsLighthouseAerie_JennaApproachScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(496, 0, 328), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gMarsLighthouseAerie_ShebaApproachScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(480, 0, 328), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gMarsLighthouseAerie_PiersApproachScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(488, 0, 320), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gMarsLighthouseAerie_IvanApproachScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(528, 0, 328), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gMarsLighthouseAerie_KradenApproachScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(512, 0, 328), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

s32 gMarsLighthouseAerie_VoiceFadeScript[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(64, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gMarsLighthouseAerie_DoomDragonRiseScript[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 128),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 128),
    SC_ADD(SW_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(128, SC_TOP),
    SC_END,
};

u32 gMarsLighthouseAerie_LeaderReadyScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(500, 0, 306),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_IsaacReadyScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(524, 0, 306),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_JennaReadyScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(488, 0, 320),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_ShebaReadyScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(472, 0, 304),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_PiersReadyScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(464, 0, 280), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(10), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(10), SC_END,    
  };

u32 gMarsLighthouseAerie_IvanReadyScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(538, 0, 320),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_MiaReadyScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(552, 0, 304),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_GaretReadyScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(560, 0, 280),                   
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_WEST),                         
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_ShebaToBodiesScript[] = {
    39, 2, 4, 0x1d80000, 0, 0x1400000,
    1, 4, 0x1d80000, 0, 0x1300000, 1,
    39, 1, 22, 39, 1, 17,
};

u32 gMarsLighthouseAerie_JennaToBodiesScript[] = {
    39, 2, 4, 0x1dc0000, 0, 0x1480000,
    1, 4, 0x1d80000, 0, 0x1400000, 1,
    39, 1, 22, 39, 1, 17,
};

u32 gMarsLighthouseAerie_JennaTurnScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(486, 0, 286),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_LeaderTurnScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(504, 0, 300), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gMarsLighthouseAerie_IsaacTurnScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(528, 0, 292), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gMarsLighthouseAerie_GaretTurnScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(560, 0, 280),                   
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_WEST),                         
          SC_WAIT(10), SC_END,                                                 
  };

u32 gMarsLighthouseAerie_MiaTurnScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(548, 0, 292),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

s32 gMarsLighthouseAerie_BeamWidenScript[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
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
    SC_ADD(SW_Y, px(4)),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gMarsLighthouseAerie_BeamRiseScript[] = {
    SC_ADD(SW_Y, px(2)),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gMarsLighthouseAerie_BeaconSparkScript[] = {
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, -px(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gMarsLighthouseAerie_WatchGoldenSunRiseScript[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(8, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {                                                        
      SCENE_EXITS(MarsLighthouseAerie),       SCENE_EXIT(1, MarsLighthouseRoomMInscriptions, 1), SCENE_EXIT(2, MarsLighthouseAerie, 3),     
      SCENE_EXIT(3, MercuryLighthouse, 2), SCENE_EXIT(4, ProxHouses, 20), SCENE_EXITS_END,           
  };

struct ActorPlacement gMarsLighthouseAerie_Placements[] = {
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 584, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 616, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 648, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 648, 16, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 648, 16, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 648, 16, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(182, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(311, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(182, CONDITION_ALWAYS, 7, 512, 0, 236, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(311, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 354, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 496, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 480, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, 7, 478, 0, 266, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, 7, 506, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, 7, 536, 0, 276, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(61, CONDITION_ALWAYS, 7, 480, 0, 282, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(60, CONDITION_ALWAYS, 7, 506, 0, 292, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(62, CONDITION_ALWAYS, 7, 540, 0, 284, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 458, 0, 288, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 476, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 482, 0, 276, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 529, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 538, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 274, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 566, 0, 286, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(61, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 480, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(60, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 502, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(62, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 548, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 1, CONDITION_ALWAYS, EVENT(Scene_LeaveByExit)},
    {TRIGGER_STEP, 10, FLAG_MARS_BEACON_LIT, EVENT(MarsLighthouseAerie_FaceDoomDragon)},
    {TRIGGER_TALK, ACTOR_ISAAC, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieOurQuestIsAlmostOver)},
    {TRIGGER_TALK, ACTOR_GARET, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIWishICouldConsole)},
    {TRIGGER_TALK, ACTOR_MIA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIfAlexSetThisAll)},
    {TRIGGER_TALK, ACTOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIThoughtIdGet)},
    {TRIGGER_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIDontKnowMyParents)},
    {TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieImSorryThingsHad)},
    {TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieBothKyleAndYourParents)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_HAMA, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToIvan)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToJennasMother)},
    {TALK_TRIGGER_FACING(FACING_SOUTHWEST), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToJennasMother)},
    {TALK_TRIGGER_FACING(FACING_NORTHEAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToKyle)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToKyle)},
    {TALK_TRIGGER_FACING(FACING_SOUTHEAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToKyle)},
    {TRIGGER_TALK, ACTOR_JENNAS_FATHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToJennasFather)},
#if TLA_PATCH_1 /* From patch 1. */
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_HAMA, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HaltIvan)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HaltJennasMother)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HaltKyle)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_JENNAS_FATHER_TALK_SPOT, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HaltJennasFather)},
#endif
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HoldBackFromParents)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HoldBackFromParentsAgain)},
    {TRIGGER_STEP, 22, FLAG_ASKED_FOR_MARS_STAR, EVENT(MarsLighthouseAerie_OfferToCastMarsStar)},
    {TRIGGER_STEP, 23, IF_FLAG_SET(FLAG_ASKED_FOR_MARS_STAR), EVENT(MarsLighthouseAerie_ClearCastOffer)},
    {TRIGGER_TABLE_END},
};

u32 gMarsLighthouseAerie_BeaconFlameScript[] = {
    SC_SET(SW_SCALE_Y, 0x16666),
    SC_SET(SW_SCALE_X, 0x16666),
    SC_ANIMATION(4),
    SC_WAIT(30),
    SC_RELEASE,
};

u32 gOrbs_RisingScript[] = {                                                               
      SC_SET(SW_SCALE_X, 0xcccc),                                              
      SC_SET(SW_SCALE_Y, 0xcccc),                                              
      SC_ANIMATION(1),                                                               
      SC_LABEL(1),                                                             
      SC_ADD(SW_Y, px(1)),                                                 
      SC_WAIT(1),                                                              
      SC_LOOP(16, SC_TO(1)),                                                   
      SC_POSITION(0, 0, 0),                                                    
      SC_WAIT(1),                                                              
      SC_RELEASE,                                                                 
  };
// clang-format on
