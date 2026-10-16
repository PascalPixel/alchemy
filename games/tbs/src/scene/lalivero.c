#include "game_state.h"
#include "unit/scene/lalivero.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "field_effect.h"
#include "item_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "sound_ids.h"

// Lalivero, the desert town below Babi Lighthouse.
//
// Two visits share this scene. On the first, the party and Iodem find
// Tolbi's soldiers beaten by Saturos's group, who took Sheba to the ruins
// under the lighthouse. After Venus Lighthouse, the party comes back through
// the aerie's exit and Iodem gives them the Black Orb that sails Babi's ship.

static void TalkToIodemAboutTheShip(void);

enum {
  ENTRANCE_WORLD_MAP_SOUTH = 20,
  ENTRANCE_WORLD_MAP_NORTH = 21,
  // Arriving from Venus Lighthouse's aerie: 90 also plays the talk about the
  // ship the party saw from the lighthouse.
  ENTRANCE_FROM_AERIE = 90,
  ENTRANCE_FROM_AERIE_QUIETLY = 91
};

enum {
  FLAG_RETURNED_FROM_LIGHTHOUSE = 0x9a7,
  FLAG_WOUNDED_SOLDIERS_SEEN = 0x9b8,
  FLAG_IODEM_FAREWELL_SEEN = 0x9ba,
  FLAG_WHARF_OPEN = 0x9bb,
  FLAG_SAW_SHIP_FROM_LIGHTHOUSE = 0x9bf
};

// The actors this scene's code moves, by their place in the placement tables.
enum {
  SOLDIER_WEST = 8, // Tolbi's soldiers, wounded on the first visit
  SOLDIER_EAST = 9,
  MOURNER = 10, // sobs for Sheba (gLaliveroMournerSobs)
  WANDERER_EAST = 11,
  WANDERER_WEST = 17,
  WHARF_GUARD = 18, // keeps the door to Babi's ship
  IODEM = 22,
  TORCH = 23,             // the Mars Djinni
  NORTH_EXIT_SPRITE = 24, // placed after Venus Lighthouse
  ACTOR_BLACK_ORB = 25
};

// The door at trigger 6 is a gate two cells wide.
enum { GATE_TRIGGER = 6 };

// Played as a door shuts, after SOUND_DOOR_OPEN.
enum { SOUND_DOOR_CLOSE = 159 };

enum {
  HEAP_ITEM_ICON = 17,
  ITEM_ICON_BUFFER_SIZE = 0x608,
  ITEM_ICON_TILES = 0x400,
  ITEM_ICON_TILE_BYTES = 128
};

// Torch, Mars Djinni 6, joins the party.
static void TalkToTorch(void) { Djinn_JoinParty(TORCH, 2, 6); }

// Where the party can enter Lalivero.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gLaliveroEntrances;
}

// Lalivero has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

// Where each exit leads.
const u32 *Scene_GetExits(void) { return gLaliveroExits; }

// The townsfolk, who stand differently once the party is back from Venus
// Lighthouse.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_LIGHTHOUSE) != 0) {
    return gLaliveroPlacementsAfterLighthouse;
  }
  return gLaliveroPlacements;
}

// The villager mourning Sheba stops sobbing to answer, faces north again and
// goes back to sobbing.
static void TalkToMourner(s32 actor) {
  void Actor_FaceDirection();

  struct FieldActor *object;

  object = Actor_Get(actor);
  object->scaleX = 0x10000;
  object = Actor_Get(actor);
  object->scaleY = 0x10000;
  Engine_EventSetMessage(MsgLaliveroOhhTheyTookShebaHeaded);
  Event_ShowMessage(actor, 0);
  Actor_FaceDirection(actor, FACING_NORTH, 0);
  Engine_EventWait(20);
  Actor_SetActionScript(actor, gLaliveroMournerSobs);
}

// After Venus Lighthouse: a villager asks whether the party is leaving
// Lalivero, and answers yes or no.
static void TalkToVillagerAskingIfLeaving(s32 actor) {
  s32 message = MsgLaliveroLeavingLalivero;
  Engine_EventSetMessage(message);
  Engine_EventOpenMessage(actor, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(message + 1);
  } else {
    Engine_EventSetMessage(message + 2);
  }
  Engine_EventShowMessage(actor, 0);
}

// Once Iodem has given the Black Orb, the guard at the wharf door lets the
// party through and steps aside.
static void TalkToWharfGuard(void) {

  Engine_GameFlagSet(FLAG_WHARF_OPEN);
  Engine_EventSetMessage(MsgLaliveroWasToldLetInIf);
  Engine_EventShowMessage(WHARF_GUARD, 0);
  Actor_SetSpeed(WHARF_GUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(WHARF_GUARD, -16, 0);
  Engine_ActorFaceDirection(WHARF_GUARD, FACING_EAST, 0);
  Engine_EventWait(10);
}

// The leader walks into a doorway: the door (or the gate) swings open, the
// leader steps in and the exit of the touched trigger is taken.
static void EnterDoor(void) {
  s32 door = gEventWork->touchedTrigger;
  s32 x = gLaliveroDoorCells[door][0];
  s32 y = gLaliveroDoorCells[door][1];

  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = ACTOR_FALLS;
  Engine_AudioPlayCue(SOUND_DOOR_OPEN);
  if (door == GATE_TRIGGER) {
    Map_AnimateCells(gLaliveroGateOpenSteps, (u16)x, (u16)y);
    Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    Map_AnimateCells(gLaliveroDoorOpenSteps, (u16)x, (u16)y);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  }
  Engine_EventWait(10);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(door);
}

// An exit with no door: leave by the touched trigger's exit.
static void TakeTouchedExit(void) {
  struct EventWork *work = gEventWork;
  s16 *trigger = (s16 *)&work->touchedTrigger;
  s32 exit = *trigger;

  work->transitionFrames = 16;
  Engine_EventRequestExit(exit);
}

// The first visit, arriving from the world map: the party and Iodem find
// Tolbi's soldiers beaten in the street. Seven attackers took Sheba to the
// ruins under Babi Lighthouse, and Faran followed them.
static void WatchWoundedSoldiers(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Engine_EventSetMessage(MsgLaliveroMoreTolbisSoldiersLayDefeated);
  // The party walks in from the south; the two soldiers lie on the ground.
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(248), PIXELS(424));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorSetAnimation(SOLDIER_WEST, 0);
  Engine_ActorSetAnimation(SOLDIER_EAST, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  // Iodem steps out from the party and looks them over.
  FieldMotion_LaunchFromFocusedObject(IODEM, 8, -16, FACING_NORTH);
  Engine_ActorWaitForMove(IODEM);
  Engine_EventWait(20);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 60);
  Actor_SetSpeed(IODEM, 0x10000, 0x8000);
  Actor_WalkByAndWait(IODEM, 0, -16);
  Engine_EventWait(10);
  Actor_FaceActor(IODEM, SOLDIER_WEST, 40);
  Actor_FaceActor(IODEM, SOLDIER_EAST, 40);
  Actor_FaceActor(IODEM, SOLDIER_WEST, 40);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_OpenMessage(IODEM, 0);
  Engine_EventChooseYesNo(0, 0);
  Engine_EventWait(30);
  // The soldiers know Iodem's voice: seven attackers took Sheba.
  Engine_ActorRunRepeatedMotion(SOLDIER_WEST, 2);
  Engine_EventWait(20);
  Event_ShowMessage(SOLDIER_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceActor(IODEM, SOLDIER_WEST, 30);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_ActorSetAnimation(SOLDIER_WEST, ANIM_STAND);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(SOLDIER_WEST, 2);
  Engine_EventWait(30);
  Event_ShowMessage(SOLDIER_WEST, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_EAST, 2);
  Engine_EventWait(20);
  Event_ShowMessage(SOLDIER_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceActor(IODEM, SOLDIER_EAST, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_EAST, 30);
  Engine_ActorSetAnimation(SOLDIER_EAST, ANIM_STAND);
  Engine_EventWait(10);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 8, 40);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_EAST, 2);
  Engine_EventWait(30);
  Event_ShowMessage(SOLDIER_EAST, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 50);
  Actor_FaceActor(IODEM, SOLDIER_EAST, 20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_WEST, 2);
  Engine_EventWait(20);
  Event_ShowMessage(SOLDIER_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventWait(20);
  // Iodem paces while they tell where the attackers went.
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 50);
  Actor_SetSpeed(IODEM, 0x1cccc, 0xe666);
  Actor_WalkToAndWait(IODEM, 256, 360);
  Engine_EventWait(10);
  Actor_FaceDirection(IODEM, FACING_EAST, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(IODEM, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(IODEM, FACING_NORTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_EAST, 2);
  Engine_EventWait(20);
  Event_ShowMessage(SOLDIER_EAST, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(IODEM, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(SOLDIER_EAST, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(SOLDIER_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(IODEM, 0x19999, 0xcccc);
  Actor_WalkToAndWait(IODEM, 256, 384);
  Actor_FaceDirection(IODEM, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_WEST, 2);
  Engine_EventWait(20);
  Event_ShowMessage(SOLDIER_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(IODEM, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_SetSpeed(IODEM, 0x13333, 0x9999);
  // Iodem resolves to follow; the soldiers send the party on.
  Actor_WalkByAndWait(IODEM, 0, 16);
  Engine_EventWait(10);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 1, 80);
  Actor_FaceActor(IODEM, SOLDIER_WEST, 40);
  Actor_FaceActor(IODEM, SOLDIER_EAST, 40);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_EAST, 2);
  Engine_EventWait(20);
  Event_ShowMessage(SOLDIER_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(IODEM, FACING_EAST, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_EAST, 30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_WEST, 2);
  Engine_EventWait(20);
  Event_ShowMessage(SOLDIER_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(IODEM, FACING_WEST, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_WEST, 70);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  // Iodem walks back into the leader.
  Actor_SetSpeed(IODEM, 0x13333, 0x9999);
  Engine_ActorSetAnimation(IODEM, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(IODEM, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IODEM);
  Actor_SetPosition(IODEM, 0, 0);
  Engine_EventWait(10);
  Engine_EventEnd();
}

// Trigger 50 after Venus Lighthouse, until Iodem has spoken: Garet stops
// the leader, Iodem comes out of his house and tells the party that the
// misty island they saw may be Lemuria, which could save Babi, and gives them
// the Black Orb that sails Babi's ship.
static void ReceiveBlackOrb(void) {
  struct FieldActor *leader;

  GameFlag_Set(FLAG_IODEM_FAREWELL_SEEN);
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLaliveroWhereGoingIsaacIodemAsked);
  // Garet reminds the leader that Iodem asked them to wait; the answer
  // picks Mia's reply.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 376);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  FieldMotion_LaunchFromFocusedObject(1, -32, 0, FACING_EAST);
  FieldMotion_LaunchFromFocusedObject(3, -16, 16, FACING_NORTHEAST);
  Motion_LaunchFromFocusedObject(2, 0, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(30);
  Event_OpenMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_EventWait(10);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessage(MIA, 0);
    gEventWork->message += 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    gEventWork->message += 1;
    Event_ShowMessage(MIA, 0);
  }
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 8, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  // The party lines up facing Iodem's door.
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -16, 0);
  Actor_WalkBy(IVAN, 0, -16);
  Actor_WalkBy(MIA, 0, -8);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(80);
  // Iodem comes out: the door opens (cells 34, 0 drawn at 4, 18) and closes
  // behind him (cells 32, 0).
  Actor_SetSpeed(IODEM, 0xcccc, 0x6666);
  Object_GetById(IODEM)->motionFlags = ACTOR_FALLS;
  Engine_ActorSetSpritePriority(IODEM, 2);
  Map_CopyCellsLayered(34, 0, 1, 2, 4, 18);
  Event_PlayCue(SOUND_DOOR_OPEN);
  Engine_EventWait(20);
  Actor_SetPosition(IODEM, PIXELS(72), PIXELS(312));
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
  Object_GetById(IODEM)->targetZ = ACTOR_NO_TARGET;
  Actor_SetPosition(IODEM, PIXELS(72), PIXELS(312));
  Engine_EventWait(20);
  Actor_WalkToAndWait(IODEM, 72, 328);
#else
  Engine_EventWait(20);
  Actor_WalkByAndWait(IODEM, 0, 16);
#endif
  Engine_MapCopyCellsLayered(32, 0, 1, 2, 4, 18);
  Event_PlayCue(SOUND_DOOR_CLOSE);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(40);
  Actor_WalkByAndWait(IODEM, 16, 0);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(40);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IODEM, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessage(IODEM, 0);
    gEventWork->message += 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IODEM, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    gEventWork->message += 1;
    Event_ShowMessage(IODEM, 0);
  }
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 6, 50);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(20);
  // Iodem takes out the Black Orb.
  Actor_SetPosition(ACTOR_BLACK_ORB, PIXELS(88), PIXELS(332));
  Event_ShowMessage(-1, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 40);
  Engine_ActorJump(MIA, 4, 13);
  Engine_ActorJump(MIA, 4, 30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IODEM, 0);
  // Arriving through the aerie, the party also talks about the ship they saw
  // from the lighthouse.
  if (GameFlag_IsSet(FLAG_SAW_SHIP_FROM_LIGHTHOUSE) != 0) {
    TalkToIodemAboutTheShip();
  }
  Engine_EventSetMessage(MsgLaliveroBabiToldMeShipAncients);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  // He holds the orb out, the leader takes it, and he steps back.
  Actor_SetSpeed(ACTOR_BLACK_ORB, 0xcccc, 0x6666);
  Actor_SetDestinationOffset(ACTOR_BLACK_ORB, 0, 16);
  Actor_WalkByAndWait(IODEM, 0, 16);
  Engine_EventWait(30);
  Actor_SetPosition(ACTOR_BLACK_ORB, 0, 0);
  gEventWork->message += 1;
  Engine_PartyGiveItem(ITEM_BLACK_ORB, 0);
  Engine_EventWait(10);
  Actor_Get(IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(IODEM, 0, -16);
  Actor_Get(IODEM)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(IODEM, 0x4100, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_NOD);
  Engine_EventWait(30);
  // Iodem goes back inside.
  Actor_WalkByAndWait(IODEM, -16, 0);
  Actor_FaceDirection(IODEM, FACING_NORTH, 0);
  Engine_EventWait(20);
  Map_CopyCellsLayered(34, 0, 1, 2, 4, 18);
  Event_PlayCue(SOUND_DOOR_OPEN);
  Engine_EventWait(10);
  Actor_WalkByAndWait(IODEM, 0, -16);
  Actor_SetPosition(IODEM, 0, 0);
  Engine_EventWait(10);
  Engine_MapCopyCellsLayered(32, 0, 1, 2, 4, 18);
  Event_PlayCue(SOUND_DOOR_CLOSE);
  Engine_EventWait(50);
  // The others are eager to sail, then walk back into the leader.
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_EventWait(10);
  Engine_EventEnd();
}

// The ship the party saw from the lighthouse was not Babi's but, Garet
// guesses, Saturos's.
static void TalkToIodemAboutTheShip(void) {
  Engine_EventSetMessage(MsgLaliveroThoughtShipWeSawAt);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 55);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(65);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
}

// Trigger 51 at the town exit: Garet turns the leader back toward Babi's
// ship and the wharf the guard was keeping.
static void TurnBackFromTheGate(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLaliveroWhereGoingArentWeTaking);
  Event_ShowMessage(GARET, 0);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Engine_EventEnd();
}

// What the party can talk to, touch and search, which changes once they are
// back from Venus Lighthouse.
const struct SceneEvent *Scene_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_LIGHTHOUSE) != 0) {
    return gLaliveroEventsAfterLighthouse;
  }
  return gLaliveroEvents;
}

// Sets Lalivero up as the party enters: the Black Orb prop, the wounded
// soldiers of the first visit (and its cutscene), or the north exit closed
// after Venus Lighthouse.
s32 Scene_Initialize(void) {
  union FieldObject *object;
  struct FieldSprite *sprite;
  u8 *icon;

  // Coming back from the lighthouse switches the town to its later state.
  if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
    GameFlag_Set(FLAG_RETURNED_FROM_LIGHTHOUSE);
    GameFlag_Set(FLAG_SAW_SHIP_FROM_LIGHTHOUSE);
  }
  if (gGameState.entrance == ENTRANCE_FROM_AERIE_QUIETLY) {
    GameFlag_Set(FLAG_RETURNED_FROM_LIGHTHOUSE);
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 24;

  // Three sprite 0xee props stand in animation 3 and block nothing.
  Engine_ActorSetAnimation(19, 3);
  Actor_Get(19)->collisionFlags = 0;
  Engine_ActorSetSpriteFlags(Actor_Get(19), 0);
  Engine_ActorSetAnimation(20, 3);
  Actor_Get(20)->collisionFlags = 0;
  Engine_ActorSetSpriteFlags(Actor_Get(20), 0);
  Engine_ActorSetAnimation(21, 3);
  Actor_Get(21)->collisionFlags = 0;
  Engine_ActorSetSpriteFlags(Actor_Get(21), 0);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_BLACK_ORB), 0);

  // The Black Orb Iodem hands over is drawn from its item icon, floating
  // 10 pixels up.
  object = (union FieldObject *)Actor_Get(ACTOR_BLACK_ORB);
  object->actor.unk5c = 1;
  object->actor.motionFlags = 0;
  sprite = object->actor.sprite;
  object->actor.y.fixed = PIXELS(10);
  sprite->partCount = 0;
  sprite->fullColor = 0;
  sprite->palette = 0;
  icon = Engine_HeapAllocate(HEAP_ITEM_ICON, ITEM_ICON_BUFFER_SIZE);
  Engine_ItemLoadIcon(ITEM_BLACK_ORB);
  Engine_VramLoad(sprite->vramBlock, ITEM_ICON_TILE_BYTES,
                  &icon[ITEM_ICON_TILES]);
  Engine_HeapRelease(HEAP_ITEM_ICON);

  if (GameFlag_IsSet(FLAG_RETURNED_FROM_LIGHTHOUSE)) {
    // Something now stands at the north exit, and its three cells take the
    // attributes of cell (20, 23).
    Engine_ActorSetSpriteFlags(Actor_Get(NORTH_EXIT_SPRITE), 0);
    Actor_Get(NORTH_EXIT_SPRITE)->collisionFlags = 0;
    Map_CopyCellAttributes(20, 23, 1, 1, 14, 4);
    Map_CopyCellAttributes(20, 23, 1, 1, 15, 4);
    Map_CopyCellAttributes(20, 23, 1, 1, 16, 4);
    if (GameFlag_IsSet(FLAG_WHARF_OPEN)) {
      Actor_SetPosition(WHARF_GUARD, PIXELS(56), PIXELS(184));
    }
  } else {
    // The first visit: the two soldiers lie on the ground, drawn under the
    // party, and cells beside them take the attributes of cell (20, 23). The
    // third cell, at x 78, is far from both soldiers.
    object = (union FieldObject *)Actor_Get(SOLDIER_WEST);
    object->actor.collisionFlags = 0;
    object->actor.priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    object->actor.sprite->flags = 0;
    object->actor.sprite->rotation = FACING_NORTH;
    object = (union FieldObject *)Actor_Get(SOLDIER_EAST);
    object->actor.collisionFlags = 0;
    object->actor.priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    object->actor.sprite->flags = 0;
    object->actor.sprite->rotation = FACING_SOUTH;
    Map_CopyCellAttributes(20, 23, 1, 1, 13, 23);
    Map_CopyCellAttributes(20, 23, 1, 1, 14, 23);
    Map_CopyCellAttributes(20, 23, 1, 1, 78, 23);
    Map_CopyCellAttributes(20, 23, 1, 1, 17, 23);
    Map_CopyCellAttributes(20, 23, 1, 1, 18, 23);
    if ((gGameState.entrance == ENTRANCE_WORLD_MAP_SOUTH ||
         gGameState.entrance == ENTRANCE_WORLD_MAP_NORTH) &&
        !GameFlag_IsSet(FLAG_WOUNDED_SOLDIERS_SEEN)) {
      GameFlag_Set(FLAG_WOUNDED_SOLDIERS_SEEN);
      // The two villagers who wander stay put during the cutscene.
      object = (union FieldObject *)Actor_Get(WANDERER_EAST);
      object->actor.paused = 1;
      object = (union FieldObject *)Actor_Get(WANDERER_WEST);
      object->actor.paused = 1;
      WatchWoundedSoldiers();
      object = (union FieldObject *)Actor_Get(WANDERER_EAST);
      object->actor.paused = 0;
      object = (union FieldObject *)Actor_Get(WANDERER_WEST);
      object->actor.paused = 0;
    }
  }
  return 0;
}

// The mourner's sobbing: the sprite squashes and stretches twice, then
// rests 80 frames.
s32 gLaliveroMournerSobs[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gLaliveroEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 72, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 152, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 200, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 296, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 360, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 432, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 72, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 104, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 360, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 440, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 424, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {20, CONDITION_ALWAYS, 248, 0, 416, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {21, CONDITION_ALWAYS, 248, 0, 64, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gLaliveroExits[] = {
    SCENE_EXITS(178),       SCENE_EXIT(1, 180, 30), SCENE_EXIT(2, 179, 1),
    SCENE_EXIT(3, 180, 23), SCENE_EXIT(4, 179, 6),  SCENE_EXIT(5, 179, 4),
    SCENE_EXIT(6, 179, 11), SCENE_EXIT(7, 180, 21), SCENE_EXIT(8, 180, 22),
    SCENE_EXIT(9, 179, 9),  SCENE_EXIT(10, 179, 2), SCENE_EXIT(11, 179, 10),
    SCENE_EXIT(20, 2, 30),  SCENE_EXIT(21, 2, 59),  SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gLaliveroPlacements[] = {
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(288), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, (s32)gLaliveroMournerSobs,
        PIXELS(248), 0, PIXELS(96), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(312), 0, PIXELS(280), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(304), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(440), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(200), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(160), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(400), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x5c, 0x5e, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLaliveroPlacementsAfterLighthouse[] = {
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(312), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(304), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(440), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(200), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(232), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(160), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(400), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x5c, 0x5e, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x112, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

s16 gLaliveroDoorCells[][2] = {
    { 0, 0 }, { 4, 9 }, { 9, 11 }, { 12, 8 }, { 18, 8 }, { 22, 8 }, { 26, 7 }, { 4, 18 },
    { 0, 0 }, { 22, 15 }, { 27, 18 }, { 0, 0 },
};

u16 gLaliveroDoorOpenSteps[] = {
    MAP_CELL_STEP(33, 0, 1, 2, 8), MAP_CELL_STEP(34, 0, 1, 2, 1), MAP_CELL_STEPS_END,
};

u16 gLaliveroGateOpenSteps[] = {
    MAP_CELL_STEP(34, 2, 2, 2, 8), MAP_CELL_STEP(36, 2, 2, 2, 1), MAP_CELL_STEPS_END,
};

struct SceneEvent gLaliveroEvents[] = {
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(TakeTouchedExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TakeTouchedExit) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroUhhnnWeWereTakenBySurprise) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyWereTerribleWeWereNo) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToMourner) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorIsOurOnlyHopeIf) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisAllHappenedBecauseTolbiTook) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranFollowedActorImSureHell) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheRumblingAtTheRuinsMust) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroThereWasAnEarthquakeAtThe) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfIWereBiggerIWould) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroIBetActorCanUseHer) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroBabisMostPreciousBelongingIsKept) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfOnlyTheyDidntHaveActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyUsedFireOnUsThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroCursesWeCouldntStopActorAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhatDoTheyPlanToDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeNeverShouldHaveLetTolbi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranStillDoesNotReturnWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeHaveSeenManyOmensOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheStonesWeStackedForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroITriedToStepInFront) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyAllHaveALessonComing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWonderWhatBabisMostPrecious) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToTorch) },
    { SEARCH(SEARCH_JAR), 100, 0xfb6, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SEARCH(SEARCH_STONE_PILLAR), 101, 0xfb7, FIND_ITEM(0xba) },
    { 0x8413, 102, 0xfb8, FIND_ITEM(0x97) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroEventsAfterLighthouse[] = {
    { EVENT_TOUCH, 50, FLAG_IODEM_FAREWELL_SEEN, EVENT_SCRIPT(ReceiveBlackOrb) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(TurnBackFromTheGate) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(TakeTouchedExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TakeTouchedExit) },
    { EVENT_TALK, 18, FLAG_WHARF_OPEN, EVENT_SCRIPT(TalkToWharfGuard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x412, FLAG_WHARF_OPEN, EVENT_SCRIPT(TalkToWharfGuard) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroHaHaIThoughtTheyHad) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWillNeverLetMyGuard) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhereCouldActorBeNowIm) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToVillagerAskingIfLeaving) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroISawTheBeaconOfVenus) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheExplosionInTheVenusRuins) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroThePeninsulaBehindVenusLighthouseIs) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroICantBelieveYouEscapedVenus) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfYouDefeatedThatEvilBunch) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorJumpedIntoTheOceanFrom) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgLaliveroNoTrespassingDangerBeyondThisPoint) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroApparentlyAShipHasBeenDocked) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroIAmAllHealedNowBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWantedToBeAPart) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfActorUsedHerGreatPowers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroAfterEverythingThatsHappenedLaliveroNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseHasBeenTheSymbol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroIodemOfTolbiDoesntSeemToo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheOceanIsSoRoughSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseIsStillStandingStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroYouMustBeGreatWarriorsTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorWouldBeFineEvenAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisSureSeemsLikeAStrange) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToTorch) },
    { SEARCH(SEARCH_JAR), 100, 0xfb6, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SEARCH(SEARCH_STONE_PILLAR), 101, 0xfb7, FIND_ITEM(0xba) },
    { SEARCH(SEARCH_CHEST), 102, 0xfb8, FIND_ITEM(0x97) },
    { SCENE_EVENTS_END },
};
// clang-format on
