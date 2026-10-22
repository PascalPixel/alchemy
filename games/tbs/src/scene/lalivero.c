/// @file Lalivero, the town by Babi Lighthouse, one scene with the area name
/// Lalivero: SceneId_Lalivero. Its buildings are lalivero_interiors.c. The
/// story brings the party here twice, and Lalivero_GetPlacements and
/// Lalivero_GetEvents choose the town's people and events for each visit by
/// FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, set at Venus Lighthouse's aerie.
///
/// On the first visit the party comes in from the world map with Iodem, and
/// Event_WatchWoundedSoldiers plays once (FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN):
/// two of Tolbi's soldiers lie beaten in the street and tell Iodem that Sheba's
/// captors went to the ruins beneath Babi Lighthouse, with Faran after them.
/// The townsfolk talk of Sheba, and the guard at the wharf lets no one by.
///
/// On the second visit the party comes back through Faran's house. Its first
/// step onto trigger 50 plays Event_IodemGivesBlackOrb
/// (FLAG_IODEM_FAREWELL_SEEN), in which Iodem comes out of the house and gives
/// the party the Black Orb that sails Babi's ship;
/// Lalivero_TalkAboutIdejimaShip adds a few lines when FLAG_SAW_SHIP_AT_IDEJIMA
/// is set. The wharf's guard then lets the party through
/// (FLAG_LALIVERO_WHARF_OPEN), trigger 51 turns it back toward the ship, and a
/// sign and three blocked cells close the north exit.
///
/// On both visits Torch, a Mars Djinni, waits until it joins;
/// Event_EnterDoor opens the door or the Sanctum's gate before the leader
/// leaves by the touched trigger's exit into SceneId_LaliveroShops or
/// SceneId_LaliveroFaransHouse; the world map lies east and west; and a jar, a
/// stone pillar and a chest hold a Water of Life, a Psy Crystal and a Warrior's
/// Helm.
///
/// Open questions:
/// - XXX (no evidence): why the first visit's event table opens the Warrior's
///   Helm chest only facing west, from the foot of the vine east of it, when
///   the second opens it from any side; the chest and the ground around it are
///   the same on both visits (their renders), and nothing in the ROM records
///   the reason.

#include "game_state.h"
#include "djinn_ids.h"
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
#include "songs.h"
#include "flags.h"
#include "heap_state.h"

static void Lalivero_TalkAboutIdejimaShip(void);

/// Entrances that set up the later visit, 90 as if the party had also seen the
/// ship at Idejima (FLAG_SAW_SHIP_AT_IDEJIMA). No exit leads to them and the
/// entrance table has no row for them (the aerie's exit leads into Faran's
/// house): only the debug scene selector, which warps to any entrance from 0
/// to 99, reaches them.
enum { ENTRANCE_FROM_AERIE = 90, ENTRANCE_FROM_AERIE_QUIETLY = 91 };

/// The placed actors, numbered in the order of gLaliveroPlacements.
/// gLaliveroPlacementsAfterLighthouse, for the later visit, places the same
/// people in the same rows, then the sign and the Black Orb; its rows for
/// Garet, Ivan and Mia take their own actor ids (Event_SpawnObjectTable).
enum LaliveroActor {
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
  /// event answers them.
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

/// The door at trigger 6, into the Lalivero Sanctum, is a gate two cells wide.
enum { GATE_TRIGGER = 6 };

/// A cell of the wall along the town's south edge (MAP_CELL_WALL), whose
/// attributes Lalivero_SetupScene copies onto cells the party may not cross.
enum { WALL_CELL_X = 20, WALL_CELL_Y = 23 };

/// A chest searched while facing the given way.
#define SEARCH_CHEST_FACING(facing)                                            \
  (SEARCH(SEARCH_CHEST) | EVENT_TOUCH_FACING | (facing))

/// An item icon's tiles: a 16-pixel square at 4 bits a pixel.
#define ICON_TILE_BYTES (16 * 16 / 2)

/// Lalivero, either visit: talking to Torch, a Mars Djinni, makes it join the
/// party.
static void Event_TalkToTorch(void) {
  Djinn_JoinParty(ACTOR_TORCH, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH));
}

const struct SceneEntrance *Lalivero_GetEntrances(void) {
  return gLaliveroEntrances;
}

const struct SceneRegion *Lalivero_GetRegions(void) { return NULL; }

const u32 *Lalivero_GetExits(void) { return gLaliveroExits; }

/// The townsfolk, who stand differently once the party is back from Venus
/// Lighthouse.
const struct ScenePlacement *Lalivero_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
    return gLaliveroPlacementsAfterLighthouse;
  }
  return gLaliveroPlacements;
}

/// Lalivero, the first visit: the young man mourning Sheba stops sobbing to say
/// she was taken to the ruins, faces north again and goes back to sobbing.
static void Event_TalkToVillagerTheyTookSheba(s32 actor) {
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
  Actor_SetActionScript(actor, gLalivero_CryingScript);
}

/// Lalivero, after Venus Lighthouse: the woman with fair hair asks whether the
/// party is leaving Lalivero, and wishes it well either way.
static void Event_WomanAsksIfLeaving(s32 actor) {
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

/// Lalivero, after Venus Lighthouse: the guard at the wharf's door ("I was
/// told to let you in if you showed up...") lets the party through and steps
/// aside (sets FLAG_LALIVERO_WHARF_OPEN).
static void Event_WharfGuardLetsPartyThrough(void) {

  Engine_GameFlagSet(FLAG_LALIVERO_WHARF_OPEN);
  Engine_EventSetMessage(MsgLaliveroWasToldLetInIf);
  Engine_EventShowMessage(ACTOR_WHARF_GUARD, 0);
  Actor_SetSpeed(ACTOR_WHARF_GUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_WHARF_GUARD, -16, 0);
  Engine_ActorFaceDirection(ACTOR_WHARF_GUARD, FACING_EAST, 0);
  Engine_EventWait(10);
}

/// The leader walks through one of Lalivero's doors. As the leader walks into a
/// doorway, the door (or the Sanctum's gate) swings open, the leader steps in
/// and the exit of the touched trigger is taken.
static void Event_EnterDoor(void) {
  s32 door = gEventWork->touchedTrigger;
  s32 x = gLaliveroDoorCells[door][0];
  s32 y = gLaliveroDoorCells[door][1];

  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = ACTOR_FALLS;
  Engine_AudioPlayCue(SE_DOOR_OPEN);
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

/// Lalivero: a doorway with no door (triggers 8 and 11); leave by the touched
/// trigger's exit.
static void Event_TakeTouchedExit(void) {
  struct EventWork *work = gEventWork;
  s16 *trigger = (s16 *)&work->touchedTrigger;
  s32 exit = *trigger;

  work->transitionFrames = 16;
  Engine_EventRequestExit(exit);
}

/// Lalivero, the first visit, arriving from the world map: the party and Iodem
/// find more of Tolbi's soldiers beaten in the street. Iodem asks whether they
/// fought a group of six; there were seven, the same group the soldiers had
/// been following, and one of them was Sheba, with the scholar Kraden
/// protecting her. They headed for the ruins beneath Babi Lighthouse, and
/// Faran, the master of Lalivero, followed them in. The soldiers send the party
/// on, and Iodem sets off for Babi Lighthouse.
static void Event_WatchWoundedSoldiers(void) {
  struct FieldActor *leader;

  Engine_EventStart();
  Engine_EventSetMessage(MsgLaliveroMoreTolbisSoldiersLayDefeated);
  // The party walks in from the south; the two soldiers lie on the ground.
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(248), PIXELS(424));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorSetAnimation(ACTOR_WEST_SOLDIER, 0);
  Engine_ActorSetAnimation(ACTOR_EAST_SOLDIER, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  // Iodem steps out from the party and looks them over.
  FieldMotion_LaunchFromFocusedObject(ACTOR_IODEM, 8, -16, FACING_NORTH);
  Engine_ActorWaitForMove(ACTOR_IODEM);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetSpeed(ACTOR_IODEM, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
  Engine_EventWait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 40);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_OpenMessage(ACTOR_IODEM, 0);
  Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  // The soldiers know Iodem's voice: seven attackers took Sheba.
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_ActorSetAnimation(ACTOR_WEST_SOLDIER, ANIMATION_STAND);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_EAST_SOLDIER, 30);
  Engine_ActorSetAnimation(ACTOR_EAST_SOLDIER, ANIMATION_STAND);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_EventWait(20);
  // Iodem paces while they tell where the attackers went.
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetSpeed(ACTOR_IODEM, 0x1cccc, 0xe666);
  Actor_WalkToAndWait(ACTOR_IODEM, 256, 360);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_EAST_SOLDIER, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_IODEM, 0x19999, 0xcccc);
  Actor_WalkToAndWait(ACTOR_IODEM, 256, 384);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_IODEM, 0x13333, 0x9999);
  // Iodem resolves to follow; the soldiers send the party on.
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_EAST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_EAST_SOLDIER, 30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WEST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_SOLDIER, 70);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  // Iodem walks back into the leader.
  Actor_SetSpeed(ACTOR_IODEM, 0x13333, 0x9999);
  Engine_ActorSetAnimation(ACTOR_IODEM, ANIMATION_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_IODEM);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Engine_EventWait(10);
  Engine_EventEnd();
}

/// Lalivero, trigger 50 after Venus Lighthouse, until Iodem has spoken
/// (FLAG_IODEM_FAREWELL_SEEN): Garet stops the leader, for Iodem asked them to
/// wait. Iodem comes out of Faran's house and tells the party that the misty
/// island they saw from the lighthouse may be Lemuria, which could save Babi's
/// life. The party promised Babi to find it, though it means first to find
/// Jenna, whom it is searching for, and Iodem gives it the Black Orb that
/// sails Babi's ship. Garet is glad to see the ocean at last.
static void Event_IodemGivesBlackOrb(void) {
  struct FieldActor *leader;

  GameFlag_Set(FLAG_IODEM_FAREWELL_SEEN);
  Engine_EventStart();
  Engine_EventSetMessage(MsgLaliveroWhereGoingIsaacIodemAsked);
  // Garet reminds the leader that Iodem asked them to wait; the answer
  // picks Mia's reply.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 376);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  FieldMotion_LaunchFromFocusedObject(GARET, -32, 0, FACING_EAST);
  FieldMotion_LaunchFromFocusedObject(MIA, -16, 16, FACING_NORTHEAST);
  Motion_LaunchFromFocusedObject(IVAN, 0, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(30);
  Event_OpenMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_EventWait(10);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessage(MIA, 0);
    gEventWork->message += 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
    Engine_EventWait(20);
    gEventWork->message += 1;
    Event_ShowMessage(MIA, 0);
  }
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(30);
  // The party lines up facing Iodem's door.
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -16, 0);
  Actor_WalkBy(IVAN, 0, -16);
  Actor_WalkBy(MIA, 0, -8);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Engine_ActorSetAnimation(MIA, ANIMATION_STAND);
  Engine_ActorSetAnimation(IVAN, ANIMATION_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(80);
  // Iodem comes out: the door opens (cells 34, 0 drawn at 4, 18) and closes
  // behind him (cells 32, 0).
  Actor_SetSpeed(ACTOR_IODEM, 0xcccc, 0x6666);
  Object_GetById(ACTOR_IODEM)->motionFlags = ACTOR_FALLS;
  Engine_ActorSetSpritePriority(ACTOR_IODEM, 2);
  Map_CopyCellsLayered(34, 0, 1, 2, 4, 18);
  Event_PlayCue(SE_DOOR_OPEN);
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_IODEM, PIXELS(72), PIXELS(312));
#if TBS_PATCH_2
  Object_GetById(ACTOR_IODEM)->targetZ = ACTOR_NO_TARGET;
  Actor_SetPosition(ACTOR_IODEM, PIXELS(72), PIXELS(312));
#endif
  Engine_EventWait(20);
#if TBS_PATCH_2
  Actor_WalkToAndWait(ACTOR_IODEM, 72, 328);
#else
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
#endif
  // The door shuts.
  Engine_MapCopyCellsLayered(32, 0, 1, 2, 4, 18);
  Event_PlayCue(SE_159);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(40);
  Actor_WalkByAndWait(ACTOR_IODEM, 16, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
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
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gEventWork->message += 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_SHAKE_HEAD);
    Engine_EventWait(20);
    gEventWork->message += 1;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(20);
  // Iodem takes out the Black Orb.
  Actor_SetPosition(ACTOR_BLACK_ORB, PIXELS(88), PIXELS(332));
  Event_ShowMessage(-1, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ActorJump(MIA, 4, 13);
  Engine_ActorJump(MIA, 4, 30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  // Arriving through the aerie, the party also talks about the ship they saw
  // at Idejima.
  if (GameFlag_IsSet(FLAG_SAW_SHIP_AT_IDEJIMA) != 0) {
    Lalivero_TalkAboutIdejimaShip();
  }
  Engine_EventSetMessage(MsgLaliveroBabiToldMeShipAncients);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  // He holds the orb out, the leader takes it, and he steps back.
  Actor_SetSpeed(ACTOR_BLACK_ORB, 0xcccc, 0x6666);
  Actor_SetDestinationOffset(ACTOR_BLACK_ORB, 0, 16);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  Engine_EventWait(30);
  Actor_SetPosition(ACTOR_BLACK_ORB, 0, 0);
  gEventWork->message += 1;
  Engine_PartyGiveItem(ITEM_BLACK_ORB, 0);
  Engine_EventWait(10);
  Actor_Get(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
  Actor_Get(ACTOR_IODEM)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_IODEM, 0x4100, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_NOD);
  Engine_EventWait(30);
  // Iodem goes back inside.
  Actor_WalkByAndWait(ACTOR_IODEM, -16, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Engine_EventWait(20);
  Map_CopyCellsLayered(34, 0, 1, 2, 4, 18);
  Event_PlayCue(SE_DOOR_OPEN);
  Engine_EventWait(10);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Engine_EventWait(10);
  // The door shuts.
  Engine_MapCopyCellsLayered(32, 0, 1, 2, 4, 18);
  Event_PlayCue(SE_159);
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
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, ANIMATION_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIMATION_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIMATION_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_EventWait(10);
  Engine_EventEnd();
}

/// The ship the party saw at Idejima was not Babi's, which is here in
/// Lalivero, but, Garet guesses, Saturos's.
static void Lalivero_TalkAboutIdejimaShip(void) {
  Engine_EventSetMessage(MsgLaliveroThoughtShipWeSawAt);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 55);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(65);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
}

/// Lalivero, trigger 51 after Venus Lighthouse: Garet turns the leader back
/// toward Babi's ship and the wharf the guard was keeping.
static void Event_TurnBackToTheShip(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgLaliveroWhereGoingArentWeTaking);
  Event_ShowMessage(GARET, 0);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Engine_EventEnd();
}

/// What the party can talk to, touch and search, which changes once they are
/// back from Venus Lighthouse.
const struct SceneEvent *Lalivero_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
    return gLaliveroEventsAfterLighthouse;
  }
  return gLaliveroEvents;
}

/// Sets Lalivero up as the party enters: the Black Orb Iodem will hold out,
/// the wounded soldiers of the first visit (and its cutscene), or the sign at
/// the north exit after Venus Lighthouse.
s32 Lalivero_Initialize(void) {
  union FieldObject *fieldObject;
  struct AnimationObject *sprite;
  GlyphTransfer *icon;

  // Coming back from the lighthouse switches the town to its later state.
  if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
    GameFlag_Set(FLAG_SAW_SHIP_AT_IDEJIMA);
  }
  if (gGameState.entrance == ENTRANCE_FROM_AERIE_QUIETLY) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 24;

  // The three plants stand in animation 3 and block nothing.
  Engine_ActorSetAnimation(ACTOR_FIRST_PLANT, 3);
  Actor_Get(ACTOR_FIRST_PLANT)->collisionFlags = 0;
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_FIRST_PLANT), 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_PLANT + 1, 3);
  Actor_Get(ACTOR_FIRST_PLANT + 1)->collisionFlags = 0;
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_FIRST_PLANT + 1), 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_PLANT + 2, 3);
  Actor_Get(ACTOR_FIRST_PLANT + 2)->collisionFlags = 0;
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_FIRST_PLANT + 2), 0);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_BLACK_ORB), 0);

  // The Black Orb Iodem hands over is drawn from its item icon, floating
  // 10 pixels up.
  fieldObject = (union FieldObject *)Actor_Get(ACTOR_BLACK_ORB);
  fieldObject->actor.retainSpriteCache = 1;
  fieldObject->actor.motionFlags = 0;
  sprite = fieldObject->actor.sprite;
  fieldObject->actor.y.fixed = PIXELS(10);
  sprite->count = 0;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  icon = Engine_HeapAllocate(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Engine_ItemLoadIcon(ITEM_BLACK_ORB);
  Engine_VramLoad(sprite->slot, ICON_TILE_BYTES, icon->tiles);
  Engine_HeapRelease(HEAP_SLOT_GLYPH_TRANSFER);

  if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    // A sign now stands at the north exit, and the three open cells of the
    // gap in the north wall become wall, closing the way north.
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_NORTH_EXIT_SIGN), 0);
    Actor_Get(ACTOR_NORTH_EXIT_SIGN)->collisionFlags = 0;
    Map_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 14, 4);
    Map_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 15, 4);
    Map_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 16, 4);
    if (GameFlag_IsSet(FLAG_LALIVERO_WHARF_OPEN)) {
      Actor_SetPosition(ACTOR_WHARF_GUARD, PIXELS(56), PIXELS(184));
    }
  } else {
    // The first visit: the two soldiers lie on the ground, drawn under the
    // party with their own collision off, and the two cells each covers
    // become wall instead. The third cell copied, at x 78, lies outside the
    // town's 32 columns, where the camera never goes, and changes nothing.
    fieldObject = (union FieldObject *)Actor_Get(ACTOR_WEST_SOLDIER);
    fieldObject->actor.collisionFlags = 0;
    fieldObject->actor.priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    fieldObject->actor.sprite->flags = 0;
    fieldObject->actor.sprite->rotation = FACING_NORTH;
    fieldObject = (union FieldObject *)Actor_Get(ACTOR_EAST_SOLDIER);
    fieldObject->actor.collisionFlags = 0;
    fieldObject->actor.priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    fieldObject->actor.sprite->flags = 0;
    fieldObject->actor.sprite->rotation = FACING_SOUTH;
    Map_CopyCellAttributes(20, 23, 1, 1, 13, 23);
    Map_CopyCellAttributes(20, 23, 1, 1, 14, 23);
    Map_CopyCellAttributes(20, 23, 1, 1, 78, 23);
    Map_CopyCellAttributes(20, 23, 1, 1, 17, 23);
    Map_CopyCellAttributes(20, 23, 1, 1, 18, 23);
    if ((gGameState.entrance == LALIVERO_FROM_WORLD_MAP_EAST ||
         gGameState.entrance == LALIVERO_FROM_WORLD_MAP_WEST) &&
        !GameFlag_IsSet(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN)) {
      GameFlag_Set(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN);
      // The woman and the girl who wander nearest the soldiers stay put during
      // the cutscene.
      fieldObject = (union FieldObject *)Actor_Get(ACTOR_WOMAN_OUR_ONLY_HOPE);
      fieldObject->actor.paused = 1;
      fieldObject = (union FieldObject *)Actor_Get(ACTOR_GIRL_SHEBAS_POWERS);
      fieldObject->actor.paused = 1;
      Event_WatchWoundedSoldiers();
      fieldObject = (union FieldObject *)Actor_Get(ACTOR_WOMAN_OUR_ONLY_HOPE);
      fieldObject->actor.paused = 0;
      fieldObject = (union FieldObject *)Actor_Get(ACTOR_GIRL_SHEBAS_POWERS);
      fieldObject->actor.paused = 0;
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
    {LALIVERO_FROM_LALIVERO_FARANS_HOUSE_NORTHWEST, CONDITION_ALWAYS, 72, 0,
     184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_SHOPS_WEST, CONDITION_ALWAYS, 152, 0, 216,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_FARANS_HOUSE_NORTHEAST, CONDITION_ALWAYS, 200, 0,
     168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_SHOPS_NORTHWEST, CONDITION_ALWAYS, 296, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_SHOPS_NORTH, CONDITION_ALWAYS, 360, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_SHOPS_NORTHEAST, CONDITION_ALWAYS, 432, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_FARANS_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 72, 0,
     328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_FARANS_HOUSE_SOUTH, CONDITION_ALWAYS, 104, 0, 312,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_SHOPS_SOUTH, CONDITION_ALWAYS, 360, 0, 280,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_SHOPS_SOUTHEAST, CONDITION_ALWAYS, 440, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_LALIVERO_SHOPS_EAST_SOUTHEAST, CONDITION_ALWAYS, 424, 0, 264,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 248, 0, 416, FACING_NORTH,
     0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 248, 0, 64, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gLaliveroExits[] = {
    SCENE_EXITS(Lalivero),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_FARANS_HOUSE_NORTHWEST, LaliveroFaransHouse,
               LALIVERO_FARANS_HOUSE_FROM_LALIVERO_NORTHWEST),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_SHOPS_WEST, LaliveroShops,
               LALIVERO_SHOPS_FROM_LALIVERO_WEST),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_FARANS_HOUSE_NORTHEAST, LaliveroFaransHouse,
               LALIVERO_FARANS_HOUSE_FROM_LALIVERO_NORTHEAST),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_SHOPS_NORTHWEST, LaliveroShops,
               LALIVERO_SHOPS_FROM_LALIVERO_NORTHWEST),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_SHOPS_NORTH, LaliveroShops,
               LALIVERO_SHOPS_FROM_LALIVERO_NORTH),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_SHOPS_NORTHEAST, LaliveroShops,
               LALIVERO_SHOPS_FROM_LALIVERO_NORTHEAST),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_FARANS_HOUSE_SOUTHWEST, LaliveroFaransHouse,
               LALIVERO_FARANS_HOUSE_FROM_LALIVERO_SOUTHWEST),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_FARANS_HOUSE_SOUTH, LaliveroFaransHouse,
               LALIVERO_FARANS_HOUSE_FROM_LALIVERO_SOUTH),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_SHOPS_SOUTH, LaliveroShops,
               LALIVERO_SHOPS_FROM_LALIVERO_SOUTH),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_SHOPS_SOUTHEAST, LaliveroShops,
               LALIVERO_SHOPS_FROM_LALIVERO_SOUTHEAST),
    SCENE_EXIT(LALIVERO_TO_LALIVERO_SHOPS_EAST_SOUTHEAST, LaliveroShops,
               LALIVERO_SHOPS_FROM_LALIVERO_EAST_SOUTHEAST),
    SCENE_EXIT(LALIVERO_TO_WORLD_MAP_EAST, WorldMap,
               WORLD_MAP_FROM_LALIVERO_EAST),
    SCENE_EXIT(LALIVERO_TO_WORLD_MAP_WEST, WorldMap,
               WORLD_MAP_FROM_LALIVERO_WEST),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gLaliveroPlacements[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(288), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (s32)gLalivero_CryingScript,
        PIXELS(248), 0, PIXELS(96), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(312), 0, PIXELS(280), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(304), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(440), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(200), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(160), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(400), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
    { MARS_DJINNI, FLAG_DJINNI_0 + DJINNI_TORCH, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLaliveroPlacementsAfterLighthouse[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(312), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(304), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(440), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(200), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(232), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(160), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(400), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
    { MARS_DJINNI, FLAG_DJINNI_0 + DJINNI_TORCH, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
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
    { EVENT_EXIT, 20, CONDITION_ALWAYS, LALIVERO_TO_WORLD_MAP_EAST },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, LALIVERO_TO_WORLD_MAP_WEST },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit) },
    { EVENT_TALK, ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroUhhnnWeWereTakenBySurprise) },
    { EVENT_TALK, ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyWereTerribleWeWereNo) },
    { EVENT_TALK, ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToVillagerTheyTookSheba) },
    { EVENT_TALK, ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorIsOurOnlyHopeIf) },
    { EVENT_TALK, ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisAllHappenedBecauseTolbiTook) },
    { EVENT_TALK, ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranFollowedActorImSureHell) },
    { EVENT_TALK, ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheRumblingAtTheRuinsMust) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroThereWasAnEarthquakeAtThe) },
    { EVENT_TALK, ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfIWereBiggerIWould) },
    { EVENT_TALK, ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroIBetActorCanUseHer) },
    { EVENT_TALK, ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroBabisMostPreciousBelongingIsKept) },
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
    { EVENT_TALK, ACTOR_TORCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToTorch) },
    { SEARCH(SEARCH_JAR), 100, FLAG_LALIVERO_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SEARCH(SEARCH_STONE_PILLAR), 101, FLAG_LALIVERO_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { SEARCH_CHEST_FACING(FACING_WEST), 102, FLAG_LALIVERO_WARRIORS_HELM, FIND_ITEM(ITEM_WARRIORS_HELM) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroEventsAfterLighthouse[] = {
    { EVENT_TOUCH, 50, FLAG_IODEM_FAREWELL_SEEN, EVENT_SCRIPT(Event_IodemGivesBlackOrb) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TurnBackToTheShip) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit) },
    { EVENT_TALK, ACTOR_WHARF_GUARD, FLAG_LALIVERO_WHARF_OPEN, EVENT_SCRIPT(Event_WharfGuardLetsPartyThrough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_WHARF_GUARD, FLAG_LALIVERO_WHARF_OPEN, EVENT_SCRIPT(Event_WharfGuardLetsPartyThrough) },
    { EVENT_TALK, ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroHaHaIThoughtTheyHad) },
    { EVENT_TALK, ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWillNeverLetMyGuard) },
    { EVENT_TALK, ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhereCouldActorBeNowIm) },
    { EVENT_TALK, ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WomanAsksIfLeaving) },
    { EVENT_TALK, ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroISawTheBeaconOfVenus) },
    { EVENT_TALK, ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheExplosionInTheVenusRuins) },
    { EVENT_TALK, ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroThePeninsulaBehindVenusLighthouseIs) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroICantBelieveYouEscapedVenus) },
    { EVENT_TALK, ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfYouDefeatedThatEvilBunch) },
    { EVENT_TALK, ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorJumpedIntoTheOceanFrom) },
    { EVENT_TALK, ACTOR_NORTH_EXIT_SIGN, CONDITION_ALWAYS, MSGNO(MsgLaliveroNoTrespassingDangerBeyondThisPoint) },
    { EVENT_TALK, ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroApparentlyAShipHasBeenDocked) },
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
    { EVENT_TALK, ACTOR_TORCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToTorch) },
    { SEARCH(SEARCH_JAR), 100, FLAG_LALIVERO_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SEARCH(SEARCH_STONE_PILLAR), 101, FLAG_LALIVERO_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { SEARCH(SEARCH_CHEST), 102, FLAG_LALIVERO_WARRIORS_HELM, FIND_ITEM(ITEM_WARRIORS_HELM) },
    { SCENE_EVENTS_END },
};
// clang-format on
