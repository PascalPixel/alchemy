/// @file Lalivero, the town by Babi Lighthouse, one scene with the area name
/// Lalivero: SceneId_Lalivero. Its buildings are lalivero_interiors.c. The
/// story brings the party here twice, and Lalivero_GetActorPlacements and
/// Lalivero_GetTriggers choose the town's people and triggers for each visit by
/// FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, set at Venus Lighthouse's aerie.
///
/// On the first visit the party comes in from the World Map with Iodem, and
/// Lalivero_WatchWoundedSoldiers plays once
/// (FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN): two of Tolbi's soldiers lie beaten in
/// the street and tell Iodem that Sheba's captors went to the ruins beneath
/// Babi Lighthouse, with Faran after them. The townsfolk talk of Sheba, and the
/// guard at the wharf lets no one by.
///
/// On the second visit the party comes back through Faran's house. Its first
/// step onto step trigger 50 plays Lalivero_IodemGivesBlackOrb
/// (FLAG_IODEM_FAREWELL_SEEN), in which Iodem comes out of the house and gives
/// the party the Black Orb that sails Babi's ship;
/// Lalivero_TalkAboutIdejimaShip adds a few lines when FLAG_SAW_SHIP_AT_IDEJIMA
/// is set. The wharf's guard then lets the party through
/// (FLAG_LALIVERO_WHARF_OPEN), step trigger 51 turns it back toward the ship,
/// and a sign and three blocked cells close the north exit.
///
/// On both visits Torch, a Mars Djinni, waits until it joins;
/// Lalivero_EnterDoor opens the door or the Sanctum's gate before the leader
/// leaves by the touched step trigger's exit into SceneId_TimberRoomsB or
/// SceneId_TimberRoomsC; the World Map lies east and west; and a jar, a
/// stone pillar and a chest hold a Water of Life, a Psy Crystal and a Warrior's
/// Helm.

#include "adventure.h"
#include "djinn_ids.h"
#include "unit/scene/lalivero.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event_layer.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "field_effect.h"
#include "item_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "songs.h"
#include "flags.h"
#include "heap.h"

static void Lalivero_TalkAboutIdejimaShip(void);

/// Entrances that set up the later visit, 90 as if the party had also
/// seen the ship at Idejima (FLAG_SAW_SHIP_AT_IDEJIMA). No exit leads to them
/// and the entrance table has no row for them (the aerie's exit leads
/// into Faran's house): only the debug scene selector, which exits to any
/// entrance from 0 to 99, reaches them.
enum { ENTRANCE_FROM_AERIE = 90, ENTRANCE_FROM_AERIE_QUIETLY = 91 };

/// The placed actors, numbered in the order of
/// gLaliveroActorPlacements. gLaliveroActorPlacementsAfterLighthouse,
/// for the later visit, places the same people in the same rows, then the sign
/// and the Black Orb; its rows for Garet, Ivan and Mia take their own actor ids
/// (Event_SpawnActorPlacements).
enum LaliveroActorId {
  /// Tolbi's soldiers, lying beaten in the street on the first visit. "Uhhnn...
  /// We were taken by surprise..."; later "Ha ha! I thought they had done me in
  /// for sure, but I'm all better now."
  /// ★ Named from what the player sees: a soldier in a helmet lying in the
  /// street (its render)
  ACTOR_WEST_SOLDIER = ACTOR_FIRST_PLACED,
  /// "They were terrible! We were no match..."
  /// ★ Named from what the player sees: a second soldier lying in the street
  /// (its render)
  ACTOR_EAST_SOLDIER,
  /// A young man with brown hair who sobs for Sheba (gLalivero_CryingScript).
  /// "Ohh... They took Sheba and headed for the ruins."
  /// ★ Named from the game: "Ohh... They took"
  /// (MsgLaliveroOhhTheyTookShebaHeaded)
  ACTOR_VILLAGER_THEY_TOOK_SHEBA,
  /// A woman with fair hair. "Sheba is our only hope. If something happens to
  /// her, Lalivero is finished."; later asks whether the party is leaving
  /// Lalivero.
  /// ★ Named from the game: "is our only hope."
  /// (MsgLaliveroActorIsOurOnlyHopeIf)
  ACTOR_WOMAN_OUR_ONLY_HOPE,
  /// A man in a white turban. "This all happened because Tolbi took Sheba away
  /// from us..."
  /// ★ Named from the game: "This all happened because Tolbi took"
  /// (MsgLaliveroThisAllHappenedBecauseTolbiTook)
  ACTOR_VILLAGER_BLAMING_TOLBI,
  /// A woman in a green dress. "Faran followed Sheba. I'm sure he'll rescue
  /// her."
  /// ★ Named from the game: "I'm sure he'll rescue her."
  /// (MsgLaliveroFaranFollowedActorImSureHell)
  ACTOR_WOMAN_TRUSTING_FARAN,
  /// An old man with a cane.
  /// ★ Named from the game: "The rumbling at the ruins must have been a
  /// warning..." (MsgLaliveroTheRumblingAtTheRuinsMust)
  ACTOR_OLD_MAN_RUMBLING_WARNING,
  /// An old woman.
  /// ★ Named from the game: "There was an earthquake at the ruins before this
  /// happened." (MsgLaliveroThereWasAnEarthquakeAtThe)
  ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE,
  /// A small boy with fair hair. "If I were bigger, I would have saved Sheba
  /// myself!"
  /// ★ Named from the game: "If I were bigger, I would have saved"
  /// (MsgLaliveroIfIWereBiggerIWould)
  ACTOR_BOY_IF_I_WERE_BIGGER,
  /// A little girl with orange pigtails. "I bet Sheba can use her powers to
  /// escape them."
  /// ★ Named from the game: "can use her powers to escape them."
  /// (MsgLaliveroIBetActorCanUseHer)
  ACTOR_GIRL_SHEBAS_POWERS,
  /// A soldier who keeps the door to Babi's ship. "Babi's most precious
  /// belonging is kept here. No one may enter!"
  /// ★ Named from the game: "Babi's most precious belonging is kept here. No
  /// one may enter!" (MsgLaliveroBabisMostPreciousBelongingIsKept)
  ACTOR_WHARF_GUARD,
  /// Three plants, SPRITE_SPROUT, the sprout Growth grows at Alpine Crossing
  /// and Suhalla Gate. Here they stand in animation 3, block nothing and no
  /// trigger answers them.
  /// ★ Named from what the player sees: small green sprouts on the edges of
  /// the rooftops (its render)
  ACTOR_FIRST_PLANT,
  /// Placed off the map until needed.
  /// ★ Named from the game: "Iodem of Tolbi"
  /// (MsgLaliveroIodemOfTolbiDoesntSeemToo)
  ACTOR_IODEM = ACTOR_FIRST_PLACED + 14,
  /// Torch, a Mars Djinni, placed until it joins.
  /// ★ Named from the game: "Torch" (MsgDjinnName)
  ACTOR_TORCH,
  /// Placed after Venus Lighthouse. "No trespassing. Danger beyond this point."
  /// ★ Named from the game: "No trespassing. Danger beyond this point."
  /// (MsgLaliveroNoTrespassingDangerBeyondThisPoint)
  ACTOR_NORTH_EXIT_SIGN,
  /// The orb Iodem holds out, drawn from its item icon.
  /// ★ Named from the game: "Black Orb" (MsgItemName)
  ACTOR_BLACK_ORB
};

/// The door into the Lalivero Sanctum, a gate two cells wide.
enum LaliveroCellBehavior { CELL_BEHAVIOR_LALIVERO_SANCTUM_GATE = 6 };

/// A cell of the wall along the town's south edge (CELL_BEHAVIOR_WALL), whose
/// attributes Lalivero_SetupMap copies onto cells the party may not cross.
enum { WALL_CELL_X = 20, WALL_CELL_Y = 23 };

/// A chest searched while facing the given way.
#define EXAMINE_CHEST_FACING(facing)                                           \
  (EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | (facing))

/// An item icon's tiles: a 16-pixel square at 4 bits a pixel.
#define ICON_TILE_BYTES (16 * 16 / 2)

/// Lalivero, either visit: talking to Torch, a Mars Djinni, makes it join the
/// party.
static void Lalivero_TalkToTorch(void) {
  Djinn_JoinParty(ACTOR_TORCH, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH));
}

const struct SceneEntrance *Lalivero_GetEntrances(void) {
  return gLaliveroEntrances;
}

const struct SceneExitRegion *Lalivero_GetRegions(void) { return NULL; }

const u32 *Lalivero_GetExits(void) { return gLaliveroExits; }

/// The townsfolk, who stand differently once the party is back from Venus
/// Lighthouse.
const struct ActorPlacement *Lalivero_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
    return gLaliveroActorPlacementsAfterLighthouse;
  }
  return gLaliveroActorPlacements;
}

/// Lalivero, the first visit: the young man mourning Sheba stops sobbing to say
/// she was taken to the ruins, faces north again and goes back to sobbing.
static void Lalivero_TalkToVillagerTheyTookSheba(s32 actorId) {
  void Actor_FaceDirection();

  struct Actor *actor;

  actor = Actor_Get(actorId);
  actor->scaleX = 0x10000;
  actor = Actor_Get(actorId);
  actor->scaleY = 0x10000;
  Event_SetMessageId(MsgLaliveroOhhTheyTookShebaHeaded);
  Event_ShowMessage(actorId, 0);
  Actor_FaceDirection(actorId, FACING_NORTH, 0);
  Event_WaitFrames(20);
  Actor_SetScript(actorId, gLalivero_CryingScript);
}

/// Lalivero, after Venus Lighthouse: the woman with fair hair asks whether the
/// party is leaving Lalivero, and wishes it well either way.
static void Lalivero_WomanAsksIfLeaving(s32 actorId) {
  s32 message = MsgLaliveroLeavingLalivero;
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(10);
    Event_SetMessageId(message + 1);
  } else {
    Event_SetMessageId(message + 2);
  }
  Event_ShowMessageAndWait(actorId, 0);
}

/// Lalivero, after Venus Lighthouse: the guard at the wharf's door ("I was
/// told to let you in if you showed up...") lets the party through and steps
/// aside (sets FLAG_LALIVERO_WHARF_OPEN).
static void Lalivero_WharfGuardLetsPartyThrough(void) {

  GameFlag_Set(FLAG_LALIVERO_WHARF_OPEN);
  Event_SetMessageId(MsgLaliveroWasToldLetInIf);
  Event_ShowMessageAndWait(ACTOR_WHARF_GUARD, 0);
  Actor_SetSpeed(ACTOR_WHARF_GUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_WHARF_GUARD, -16, 0);
  Actor_ArmCallback(ACTOR_WHARF_GUARD, FACING_EAST, 0);
  Event_WaitFrames(10);
}

/// The leader walks through one of Lalivero's doors. As the leader walks into a
/// doorway, the door (or the Sanctum's gate) swings open, the leader steps in
/// and the exit of the touched step trigger is taken.
static void Lalivero_EnterDoor(void) {
  s32 door = gFieldWork->stepTrigger;
  s32 x = gLaliveroDoorCells[door][0];
  s32 y = gLaliveroDoorCells[door][1];

  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = ACTOR_FALLS;
  Sound_PlaySong(SE_DOOR_OPEN);
  if (door == CELL_BEHAVIOR_LALIVERO_SANCTUM_GATE) {
    MapGrid_AnimateCells(gLaliveroGateOpenSteps, (u16)x, (u16)y);
    Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    MapGrid_AnimateCells(gLaliveroDoorOpenSteps, (u16)x, (u16)y);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  }
  Event_WaitFrames(10);
  gFieldWork->transitionFrames = 16;
  Import_EventSetExitRequest(door);
}

/// Lalivero: a doorway with no door (step triggers 8 and 11); leave by the
/// touched step trigger's exit.
static void Lalivero_EnterDoorway(void) {
  struct FieldWork *work = gFieldWork;
  s16 *trigger = (s16 *)&work->stepTrigger;
  s32 exit = *trigger;

  work->transitionFrames = 16;
  Import_EventSetExitRequest(exit);
}

/// Lalivero, the first visit, arriving from the World Map: the party and Iodem
/// find more of Tolbi's soldiers beaten in the street. Iodem asks whether they
/// fought a group of six; there were seven, the same group the soldiers had
/// been following, and one of them was Sheba, with the scholar Kraden
/// protecting her. They headed for the ruins beneath Babi Lighthouse, and
/// Faran, the master of Lalivero, followed them in. The soldiers send the party
/// on, and Iodem sets off for Babi Lighthouse.
static void Lalivero_WatchWoundedSoldiers(void) {
  struct Actor *leader;

  Event_Start();
  Event_SetMessageId(MsgLaliveroMoreTolbisSoldiersLayDefeated);
  // The party walks in from the south; the two soldiers lie on the ground.
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(248), PIXELS(424));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Import_ActorSetAnimation(ACTOR_WEST_SOLDIER, 0);
  Import_ActorSetAnimation(ACTOR_EAST_SOLDIER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  // Iodem steps out from the party and looks them over.
  FieldActor_LaunchFromLeader(ACTOR_IODEM, 8, -16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_IODEM);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetSpeed(ACTOR_IODEM, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 40);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Event_OpenMessage(ACTOR_IODEM, 0);
  Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(30);
  // The soldiers know Iodem's voice: seven attackers took Sheba.
  Actor_RunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 30);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Import_ActorSetAnimation(ACTOR_WEST_SOLDIER, ANIM_STAND);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_EAST_SOLDIER, 30);
  Import_ActorSetAnimation(ACTOR_EAST_SOLDIER, ANIM_STAND);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Event_WaitFrames(20);
  // Iodem paces while they tell where the attackers went.
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetSpeed(ACTOR_IODEM, 0x1cccc, 0xe666);
  Actor_WalkToAndWait(ACTOR_IODEM, 256, 360);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_EAST_SOLDIER, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_IODEM, 0x19999, 0xcccc);
  Actor_WalkToAndWait(ACTOR_IODEM, 256, 384);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetSpeed(ACTOR_IODEM, 0x13333, 0x9999);
  // Iodem resolves to follow; the soldiers send the party on.
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  Event_WaitFrames(10);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_EAST_SOLDIER, 30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_SOLDIER, 70);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  // Iodem walks back into the leader.
  Actor_SetSpeed(ACTOR_IODEM, 0x13333, 0x9999);
  Import_ActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IODEM);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Event_WaitFrames(10);
  Event_End();
}

/// Lalivero, step trigger 50 after Venus Lighthouse, until Iodem has spoken
/// (FLAG_IODEM_FAREWELL_SEEN): Garet stops the leader, for Iodem asked them to
/// wait. Iodem comes out of Faran's house and tells the party that the misty
/// island they saw from the lighthouse may be Lemuria, which could save Babi's
/// life. The party promised Babi to find it, though it means first to find
/// Jenna, whom it is searching for, and Iodem gives it the Black Orb that
/// sails Babi's ship. Garet is glad to see the ocean at last.
static void Lalivero_IodemGivesBlackOrb(void) {
  struct Actor *leader;

  GameFlag_Set(FLAG_IODEM_FAREWELL_SEEN);
  Event_Start();
  Event_SetMessageId(MsgLaliveroWhereGoingIsaacIodemAsked);
  // Garet reminds the leader that Iodem asked them to wait; the answer
  // picks Mia's reply.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 376);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -32, 0, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_MIA, -16, 16, FACING_NORTHEAST);
  Actor_LaunchFromLeader(ACTOR_IVAN, 0, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_WaitFrames(30);
  Event_OpenMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Event_WaitFrames(10);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    gFieldWork->message += 1;
  } else {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    gFieldWork->message += 1;
    Event_ShowMessage(ACTOR_MIA, 0);
  }
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(40);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  // The party lines up facing Iodem's door.
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -16, 0);
  Actor_WalkBy(ACTOR_IVAN, 0, -16);
  Actor_WalkBy(ACTOR_MIA, 0, -8);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(80);
  // Iodem comes out: the door opens (cells 34, 0 drawn at 4, 18) and closes
  // behind him (cells 32, 0).
  Actor_SetSpeed(ACTOR_IODEM, 0xcccc, 0x6666);
  Actor_GetById(ACTOR_IODEM)->motionFlags = ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_IODEM, 2);
  MapGrid_CopyCellsLayered(34, 0, 1, 2, 4, 18);
  Event_PlaySong(SE_DOOR_OPEN);
  Event_WaitFrames(20);
  Actor_SetPosition(ACTOR_IODEM, PIXELS(72), PIXELS(312));
#if TBS_PATCH_2
  Actor_GetById(ACTOR_IODEM)->targetZ = ACTOR_NO_TARGET;
  Actor_SetPosition(ACTOR_IODEM, PIXELS(72), PIXELS(312));
#endif
  Event_WaitFrames(20);
#if TBS_PATCH_2
  Actor_WalkToAndWait(ACTOR_IODEM, 72, 328);
#else
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
#endif
  // The door shuts.
  MapGrid_CopyCellIndexRect(32, 0, 1, 2, 4, 18);
  Event_PlaySong(SE_DOOR_CLOSE);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(40);
  Actor_WalkByAndWait(ACTOR_IODEM, 16, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_LinkPair(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_LinkPair(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_WaitFrames(40);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message += 1;
  } else {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    gFieldWork->message += 1;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  // Iodem takes out the Black Orb.
  Actor_SetPosition(ACTOR_BLACK_ORB, PIXELS(88), PIXELS(332));
  Event_ShowMessage(-1, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_Jump(ACTOR_MIA, 4, 13);
  Actor_Jump(ACTOR_MIA, 4, 30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  // Arriving through the aerie, the party also talks about the ship they saw
  // at Idejima.
  if (GameFlag_Get(FLAG_SAW_SHIP_AT_IDEJIMA) != 0) {
    Lalivero_TalkAboutIdejimaShip();
  }
  Event_SetMessageId(MsgLaliveroBabiToldMeShipAncients);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  // He holds the orb out, the leader takes it, and he steps back.
  Actor_SetSpeed(ACTOR_BLACK_ORB, 0xcccc, 0x6666);
  Actor_SetDestinationOffset(ACTOR_BLACK_ORB, 0, 16);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  Event_WaitFrames(30);
  Actor_SetPosition(ACTOR_BLACK_ORB, 0, 0);
  gFieldWork->message += 1;
  PartyInventory_GiveItem(ITEM_BLACK_ORB, 0);
  Event_WaitFrames(10);
  Actor_Get(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
  Actor_Get(ACTOR_IODEM)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH + 0x100, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(50);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  // Iodem goes back inside.
  Actor_WalkByAndWait(ACTOR_IODEM, -16, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Event_WaitFrames(20);
  MapGrid_CopyCellsLayered(34, 0, 1, 2, 4, 18);
  Event_PlaySong(SE_DOOR_OPEN);
  Event_WaitFrames(10);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Event_WaitFrames(10);
  // The door shuts.
  MapGrid_CopyCellIndexRect(32, 0, 1, 2, 4, 18);
  Event_PlaySong(SE_DOOR_CLOSE);
  Event_WaitFrames(50);
  // The others are eager to sail, then walk back into the leader.
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(40);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_GARET, 0x13333, 0x9999);
  Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
  Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Event_WaitFrames(10);
  Event_End();
}

/// The ship the party saw at Idejima was not Babi's, which is here in
/// Lalivero, but, Garet guesses, Saturos's.
static void Lalivero_TalkAboutIdejimaShip(void) {
  Event_SetMessageId(MsgLaliveroThoughtShipWeSawAt);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 55);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Event_WaitFrames(65);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
}

/// Lalivero, step trigger 51 after Venus Lighthouse: Garet turns the leader
/// back toward Babi's ship and the wharf the guard was keeping.
static void Lalivero_TurnBackToTheShip(void) {
  Event_Start();
  Event_SetMessageId(MsgLaliveroWhereGoingArentWeTaking);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Event_End();
}

/// What the party can talk to, touch and search, which changes once they are
/// back from Venus Lighthouse.
const struct Trigger *Lalivero_GetTriggers(void) {
  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
    return gLaliveroTriggersAfterLighthouse;
  }
  return gLaliveroTriggers;
}

/// Sets Lalivero up as the party enters: the Black Orb Iodem will hold out,
/// the wounded soldiers of the first visit (and its event), or the sign at
/// the north exit after Venus Lighthouse.
s32 Lalivero_Initialize(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  GlyphTransfer *icon;

  // Coming back from the lighthouse switches the town to its later state.
  if (gAdventure.entrance == ENTRANCE_FROM_AERIE) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
    GameFlag_Set(FLAG_SAW_SHIP_AT_IDEJIMA);
  }
  if (gAdventure.entrance == ENTRANCE_FROM_AERIE_QUIETLY) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
  }
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 24;

  // The three plants stand in animation 3 and block nothing.
  Import_ActorSetAnimation(ACTOR_FIRST_PLANT, 3);
  Actor_Get(ACTOR_FIRST_PLANT)->collisionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_PLANT), 0);
  Import_ActorSetAnimation(ACTOR_FIRST_PLANT + 1, 3);
  Actor_Get(ACTOR_FIRST_PLANT + 1)->collisionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_PLANT + 1), 0);
  Import_ActorSetAnimation(ACTOR_FIRST_PLANT + 2, 3);
  Actor_Get(ACTOR_FIRST_PLANT + 2)->collisionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_PLANT + 2), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLACK_ORB), 0);

  // The Black Orb Iodem hands over is drawn from its item icon, floating
  // 10 pixels up.
  actor = (struct Actor *)Actor_Get(ACTOR_BLACK_ORB);
  actor->retainSpriteCache = 1;
  actor->motionFlags = 0;
  sprite = actor->sprite;
  actor->y.fixed = PIXELS(10);
  sprite->count = 0;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  icon = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Icon_DrawItem(ITEM_BLACK_ORB);
  VramCache_Load(sprite->slot, ICON_TILE_BYTES, icon->tiles);
  FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);

  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    // A sign now stands at the north exit, and the three open cells of the
    // gap in the north wall become wall, closing the way north.
    Actor_SetSpriteFlags(Actor_Get(ACTOR_NORTH_EXIT_SIGN), 0);
    Actor_Get(ACTOR_NORTH_EXIT_SIGN)->collisionFlags = 0;
    MapGrid_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 14, 4);
    MapGrid_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 15, 4);
    MapGrid_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 16, 4);
    if (GameFlag_Get(FLAG_LALIVERO_WHARF_OPEN)) {
      Actor_SetPosition(ACTOR_WHARF_GUARD, PIXELS(56), PIXELS(184));
    }
  } else {
    // The first visit: the two soldiers lie on the ground, drawn under the
    // party with their own collision off, and the two cells each covers
    // become wall instead. The third cell copied, at x 78, lies outside the
    // town's 32 columns, where the camera never goes, and changes nothing.
    actor = (struct Actor *)Actor_Get(ACTOR_WEST_SOLDIER);
    actor->collisionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->sprite->flags = 0;
    actor->sprite->rotation = FACING_NORTH;
    actor = (struct Actor *)Actor_Get(ACTOR_EAST_SOLDIER);
    actor->collisionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->sprite->flags = 0;
    actor->sprite->rotation = FACING_SOUTH;
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 13, 23);
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 14, 23);
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 78, 23);
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 17, 23);
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 18, 23);
    if ((gAdventure.entrance == LALIVERO_FROM_WORLD_MAP_EAST ||
         gAdventure.entrance == LALIVERO_FROM_WORLD_MAP_WEST) &&
        !Event_GetFlag(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN)) {
      GameFlag_Set(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN);
      // The woman and the girl who wander nearest the soldiers stay put during
      // the event.
      actor = (struct Actor *)Actor_Get(ACTOR_WOMAN_OUR_ONLY_HOPE);
      actor->paused = 1;
      actor = (struct Actor *)Actor_Get(ACTOR_GIRL_SHEBAS_POWERS);
      actor->paused = 1;
      Lalivero_WatchWoundedSoldiers();
      actor = (struct Actor *)Actor_Get(ACTOR_WOMAN_OUR_ONLY_HOPE);
      actor->paused = 0;
      actor = (struct Actor *)Actor_Get(ACTOR_GIRL_SHEBAS_POWERS);
      actor->paused = 0;
    }
  }
  return 0;
}

/// The sobbing of ACTOR_VILLAGER_THEY_TOOK_SHEBA: the sprite squashes and
/// stretches twice, then rests 80 frames.
s32 gLalivero_CryingScript[] = {
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
    {LALIVERO_FROM_TIMBER_ROOMS_C_NORTHWEST, CONDITION_ALWAYS, 72, 0, 184,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_WEST, CONDITION_ALWAYS, 152, 0, 216,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_C_NORTHEAST, CONDITION_ALWAYS, 200, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_NORTHWEST, CONDITION_ALWAYS, 296, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_NORTH, CONDITION_ALWAYS, 360, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_NORTHEAST, CONDITION_ALWAYS, 432, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_C_SOUTHWEST, CONDITION_ALWAYS, 72, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_C_SOUTH, CONDITION_ALWAYS, 104, 0, 312,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_SOUTH, CONDITION_ALWAYS, 360, 0, 280,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_SOUTHEAST, CONDITION_ALWAYS, 440, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_EAST_SOUTHEAST, CONDITION_ALWAYS, 424, 0, 264,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 248, 0, 416, FACING_NORTH,
     0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 248, 0, 64, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gLaliveroExits[] = {
    SCENE_EXITS(Lalivero),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_C_NORTHWEST, TimberRoomsC,
               TIMBER_ROOMS_C_FROM_LALIVERO_NORTHWEST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_WEST, TimberRoomsB,
               TIMBER_ROOMS_B_FROM_LALIVERO_WEST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_C_NORTHEAST, TimberRoomsC,
               TIMBER_ROOMS_C_FROM_LALIVERO_NORTHEAST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_NORTHWEST, TimberRoomsB,
               TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_NORTH, TimberRoomsB,
               TIMBER_ROOMS_B_FROM_LALIVERO_NORTH),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_NORTHEAST, TimberRoomsB,
               TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_C_SOUTHWEST, TimberRoomsC,
               TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_C_SOUTH, TimberRoomsC,
               TIMBER_ROOMS_C_FROM_LALIVERO_SOUTH),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_SOUTH, TimberRoomsB,
               TIMBER_ROOMS_B_FROM_LALIVERO_SOUTH),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_SOUTHEAST, TimberRoomsB,
               TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_EAST_SOUTHEAST, TimberRoomsB,
               TIMBER_ROOMS_B_FROM_LALIVERO_EAST_SOUTHEAST),
    SCENE_EXIT(LALIVERO_TO_WORLD_MAP_EAST, WorldMap,
               WORLD_MAP_FROM_LALIVERO_EAST),
    SCENE_EXIT(LALIVERO_TO_WORLD_MAP_WEST, WorldMap,
               WORLD_MAP_FROM_LALIVERO_WEST),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gLaliveroActorPlacements[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(288), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (s32)gLalivero_CryingScript,
        PIXELS(248), 0, PIXELS(96), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(312), 0, PIXELS(280), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(304), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(440), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(200), 0, PIXELS(184), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(160), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(400), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACING_KEEP, 0 },
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_TORCH), MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(136), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gLaliveroActorPlacementsAfterLighthouse[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(112), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(312), 0, PIXELS(296), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(304), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(440), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(200), 0, PIXELS(184), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(232), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(160), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(400), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACING_KEEP, 0 },
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_TORCH), MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(136), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

s16 gLaliveroDoorCells[][2] = {
    { 0, 0 }, { 4, 9 }, { 9, 11 }, { 12, 8 }, { 18, 8 }, { 22, 8 }, { 26, 7 }, { 4, 18 },
    { 0, 0 }, { 22, 15 }, { 27, 18 }, { 0, 0 },
};

u16 gLaliveroDoorOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(33, 0, 1, 2, 8), MAPGRID_ANIMATION_STEP(34, 0, 1, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};

u16 gLaliveroGateOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(34, 2, 2, 2, 8), MAPGRID_ANIMATION_STEP(36, 2, 2, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};

struct Trigger gLaliveroTriggers[] = {
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, LALIVERO_TO_WORLD_MAP_EAST },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, LALIVERO_TO_WORLD_MAP_WEST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoorway) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoorway) },
    { TRIGGER_TALK, ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroUhhnnWeWereTakenBySurprise) },
    { TRIGGER_TALK, ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyWereTerribleWeWereNo) },
    { TRIGGER_TALK, ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, EVENT(Lalivero_TalkToVillagerTheyTookSheba) },
    { TRIGGER_TALK, ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorIsOurOnlyHopeIf) },
    { TRIGGER_TALK, ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisAllHappenedBecauseTolbiTook) },
    { TRIGGER_TALK, ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranFollowedActorImSureHell) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheRumblingAtTheRuinsMust) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroThereWasAnEarthquakeAtThe) },
    { TRIGGER_TALK, ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfIWereBiggerIWould) },
    { TRIGGER_TALK, ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroIBetActorCanUseHer) },
    { TRIGGER_TALK, ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroBabisMostPreciousBelongingIsKept) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfOnlyTheyDidntHaveActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyUsedFireOnUsThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroCursesWeCouldntStopActorAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhatDoTheyPlanToDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeNeverShouldHaveLetTolbi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranStillDoesNotReturnWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeHaveSeenManyOmensOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheStonesWeStackedForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroITriedToStepInFront) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyAllHaveALessonComing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWonderWhatBabisMostPrecious) },
    { TRIGGER_TALK, ACTOR_TORCH, CONDITION_ALWAYS, EVENT(Lalivero_TalkToTorch) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_LALIVERO_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { EXAMINE_TRIGGER(EXAMINE_STONE_PILLAR), 101, FLAG_LALIVERO_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    // XXX (no evidence): why this chest opens only facing west, when
    // gLaliveroTriggersAfterLighthouse opens it from any side; the chest and the
    // ground around it are the same on both visits.
    { EXAMINE_CHEST_FACING(FACING_WEST), 102, FLAG_LALIVERO_WARRIORS_HELM, FIND_ITEM(ITEM_WARRIORS_HELM) },
    { TRIGGERS_END },
};

struct Trigger gLaliveroTriggersAfterLighthouse[] = {
    { TRIGGER_STEP, 50, FLAG_IODEM_FAREWELL_SEEN, EVENT(Lalivero_IodemGivesBlackOrb) },
    { TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(Lalivero_TurnBackToTheShip) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoorway) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoor) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Lalivero_EnterDoorway) },
    { TRIGGER_TALK, ACTOR_WHARF_GUARD, FLAG_LALIVERO_WHARF_OPEN, EVENT(Lalivero_WharfGuardLetsPartyThrough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_WHARF_GUARD, FLAG_LALIVERO_WHARF_OPEN, EVENT(Lalivero_WharfGuardLetsPartyThrough) },
    { TRIGGER_TALK, ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroHaHaIThoughtTheyHad) },
    { TRIGGER_TALK, ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWillNeverLetMyGuard) },
    { TRIGGER_TALK, ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhereCouldActorBeNowIm) },
    { TRIGGER_TALK, ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, EVENT(Lalivero_WomanAsksIfLeaving) },
    { TRIGGER_TALK, ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroISawTheBeaconOfVenus) },
    { TRIGGER_TALK, ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheExplosionInTheVenusRuins) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroThePeninsulaBehindVenusLighthouseIs) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroICantBelieveYouEscapedVenus) },
    { TRIGGER_TALK, ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfYouDefeatedThatEvilBunch) },
    { TRIGGER_TALK, ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorJumpedIntoTheOceanFrom) },
    { TRIGGER_TALK, ACTOR_NORTH_EXIT_SIGN, CONDITION_ALWAYS, MSGNO(MsgLaliveroNoTrespassingDangerBeyondThisPoint) },
    { TRIGGER_TALK, ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroApparentlyAShipHasBeenDocked) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIAmAllHealedNowBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWantedToBeAPart) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfActorUsedHerGreatPowers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroAfterEverythingThatsHappenedLaliveroNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseHasBeenTheSymbol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroIodemOfTolbiDoesntSeemToo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheOceanIsSoRoughSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseIsStillStandingStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroYouMustBeGreatWarriorsTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorWouldBeFineEvenAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisSureSeemsLikeAStrange) },
    { TRIGGER_TALK, ACTOR_TORCH, CONDITION_ALWAYS, EVENT(Lalivero_TalkToTorch) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_LALIVERO_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { EXAMINE_TRIGGER(EXAMINE_STONE_PILLAR), 101, FLAG_LALIVERO_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_LALIVERO_WARRIORS_HELM, FIND_ITEM(ITEM_WARRIORS_HELM) },
    { TRIGGERS_END },
};
// clang-format on
