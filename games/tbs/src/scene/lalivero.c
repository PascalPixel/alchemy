/// @file Lalivero, the town by Babi Lighthouse, one map with the area name
/// Lalivero: SceneId_Lalivero. Its buildings are lalivero_interiors.c. The
/// story brings the party here twice, and Lalivero_GetObjectEventTemplates and
/// Lalivero_GetEvents choose the town's people and map events for each visit by
/// FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, set at Venus Lighthouse's aerie.
///
/// On the first visit the party comes in from the world map with Iodem, and
/// EventScript_WatchWoundedSoldiers plays once
/// (FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN): two of Tolbi's soldiers lie beaten in
/// the street and tell Iodem that Sheba's captors went to the ruins beneath
/// Babi Lighthouse, with Faran after them. The townsfolk talk of Sheba, and the
/// guard at the wharf lets no one by.
///
/// On the second visit the party comes back through Faran's house. Its first
/// step onto coord event 50 plays EventScript_IodemGivesBlackOrb
/// (FLAG_IODEM_FAREWELL_SEEN), in which Iodem comes out of the house and gives
/// the party the Black Orb that sails Babi's ship;
/// Lalivero_TalkAboutIdejimaShip adds a few lines when FLAG_SAW_SHIP_AT_IDEJIMA
/// is set. The wharf's guard then lets the party through
/// (FLAG_LALIVERO_WHARF_OPEN), coord event 51 turns it back toward the ship,
/// and a sign and three blocked cells close the north exit.
///
/// On both visits Torch, a Mars Djinni, waits until it joins;
/// EventScript_EnterDoor opens the door or the Sanctum's gate before the leader
/// leaves by the touched coord event's exit into SceneId_TimberRoomsB or
/// SceneId_TimberRoomsC; the world map lies east and west; and a jar, a
/// stone pillar and a chest hold a Water of Life, a Psy Crystal and a Warrior's
/// Helm.

#include "save_block.h"
#include "djinn_ids.h"
#include "unit/scene/lalivero.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "map_tables.h"
#include "field_service.h"
#include "field_effect.h"
#include "item_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "flags.h"
#include "heap.h"

static void Lalivero_TalkAboutIdejimaShip(void);

/// Entrances that set up the later visit, 90 as if the party had also
/// seen the ship at Idejima (FLAG_SAW_SHIP_AT_IDEJIMA). No exit leads to them
/// and the entrance table has no row for them (the aerie's exit leads
/// into Faran's house): only the debug map selector, which exits to any
/// entrance from 0 to 99, reaches them.
enum { ENTRANCE_FROM_AERIE = 90, ENTRANCE_FROM_AERIE_QUIETLY = 91 };

/// The placed object events, numbered in the order of
/// gLaliveroObjectEventTemplates. gLaliveroObjectEventTemplatesAfterLighthouse,
/// for the later visit, places the same people in the same rows, then the sign
/// and the Black Orb; its rows for Garet, Ivan and Mia take their own local ids
/// (FieldScript_SpawnObjectEventTemplates).
enum LaliveroLocalId {
  /// Tolbi's soldiers, lying beaten in the street on the first visit. "Uhhnn...
  /// We were taken by surprise..."; later "Ha ha! I thought they had done me in
  /// for sure, but I'm all better now."
  /// ★ Named from what the player sees: a soldier in a helmet lying in the
  /// street (its render)
  LOCALID_WEST_SOLDIER = LOCALID_FIRST_PLACED,
  /// "They were terrible! We were no match..."
  /// ★ Named from what the player sees: a second soldier lying in the street
  /// (its render)
  LOCALID_EAST_SOLDIER,
  /// A young man with brown hair who sobs for Sheba (gLalivero_CryingScript).
  /// "Ohh... They took Sheba and headed for the ruins."
  /// ★ Named from the game: "Ohh... They took"
  /// (MsgLaliveroOhhTheyTookShebaHeaded)
  LOCALID_VILLAGER_THEY_TOOK_SHEBA,
  /// A woman with fair hair. "Sheba is our only hope. If something happens to
  /// her, Lalivero is finished."; later asks whether the party is leaving
  /// Lalivero.
  /// ★ Named from the game: "is our only hope."
  /// (MsgLaliveroActorIsOurOnlyHopeIf)
  LOCALID_WOMAN_OUR_ONLY_HOPE,
  /// A man in a white turban. "This all happened because Tolbi took Sheba away
  /// from us..."
  /// ★ Named from the game: "This all happened because Tolbi took"
  /// (MsgLaliveroThisAllHappenedBecauseTolbiTook)
  LOCALID_VILLAGER_BLAMING_TOLBI,
  /// A woman in a green dress. "Faran followed Sheba. I'm sure he'll rescue
  /// her."
  /// ★ Named from the game: "I'm sure he'll rescue her."
  /// (MsgLaliveroFaranFollowedActorImSureHell)
  LOCALID_WOMAN_TRUSTING_FARAN,
  /// An old man with a cane.
  /// ★ Named from the game: "The rumbling at the ruins must have been a
  /// warning..." (MsgLaliveroTheRumblingAtTheRuinsMust)
  LOCALID_OLD_MAN_RUMBLING_WARNING,
  /// An old woman.
  /// ★ Named from the game: "There was an earthquake at the ruins before this
  /// happened." (MsgLaliveroThereWasAnEarthquakeAtThe)
  LOCALID_OLD_WOMAN_EARLIER_EARTHQUAKE,
  /// A small boy with fair hair. "If I were bigger, I would have saved Sheba
  /// myself!"
  /// ★ Named from the game: "If I were bigger, I would have saved"
  /// (MsgLaliveroIfIWereBiggerIWould)
  LOCALID_BOY_IF_I_WERE_BIGGER,
  /// A little girl with orange pigtails. "I bet Sheba can use her powers to
  /// escape them."
  /// ★ Named from the game: "can use her powers to escape them."
  /// (MsgLaliveroIBetActorCanUseHer)
  LOCALID_GIRL_SHEBAS_POWERS,
  /// A soldier who keeps the door to Babi's ship. "Babi's most precious
  /// belonging is kept here. No one may enter!"
  /// ★ Named from the game: "Babi's most precious belonging is kept here. No
  /// one may enter!" (MsgLaliveroBabisMostPreciousBelongingIsKept)
  LOCALID_WHARF_GUARD,
  /// Three plants, SPRITE_SPROUT, the sprout Growth grows at Alpine Crossing
  /// and Suhalla Gate. Here they stand in animation 3, block nothing and no
  /// map event answers them.
  /// ★ Named from what the player sees: small green sprouts on the edges of
  /// the rooftops (its render)
  LOCALID_FIRST_PLANT,
  /// Placed off the map until needed.
  /// ★ Named from the game: "Iodem of Tolbi"
  /// (MsgLaliveroIodemOfTolbiDoesntSeemToo)
  LOCALID_IODEM = LOCALID_FIRST_PLACED + 14,
  /// Torch, a Mars Djinni, placed until it joins.
  /// ★ Named from the game: "Torch" (MsgDjinnName)
  LOCALID_TORCH,
  /// Placed after Venus Lighthouse. "No trespassing. Danger beyond this point."
  /// ★ Named from the game: "No trespassing. Danger beyond this point."
  /// (MsgLaliveroNoTrespassingDangerBeyondThisPoint)
  LOCALID_NORTH_EXIT_SIGN,
  /// The orb Iodem holds out, drawn from its item icon.
  /// ★ Named from the game: "Black Orb" (MsgItemName)
  LOCALID_BLACK_ORB
};

/// The door into the Lalivero Sanctum, a gate two cells wide.
enum LaliveroMetatileBehavior { MB_LALIVERO_SANCTUM_GATE = 6 };

/// A cell of the wall along the town's south edge (MB_WALL), whose
/// attributes Lalivero_SetupMap copies onto cells the party may not cross.
enum { WALL_CELL_X = 20, WALL_CELL_Y = 23 };

/// A chest searched while facing the given way.
#define BG_EVENT_CHEST_FACING(facing)                                          \
  (BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | (facing))

/// An item icon's tiles: a 16-pixel square at 4 bits a pixel.
#define ICON_TILE_BYTES (16 * 16 / 2)

/// Lalivero, either visit: talking to Torch, a Mars Djinni, makes it join the
/// party.
static void EventScript_TalkToTorch(void) {
  Djinn_JoinParty(LOCALID_TORCH, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH));
}

const struct SceneEntrance *Lalivero_GetEntrances(void) {
  return gLaliveroEntrances;
}

const struct SceneExitRegion *Lalivero_GetRegions(void) { return NULL; }

const u32 *Lalivero_GetExits(void) { return gLaliveroExits; }

/// The townsfolk, who stand differently once the party is back from Venus
/// Lighthouse.
const struct ObjectEventTemplate *Lalivero_GetObjectEventTemplates(void) {
  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
    return gLaliveroObjectEventTemplatesAfterLighthouse;
  }
  return gLaliveroObjectEventTemplates;
}

/// Lalivero, the first visit: the young man mourning Sheba stops sobbing to say
/// she was taken to the ruins, faces north again and goes back to sobbing.
static void EventScript_TalkToVillagerTheyTookSheba(s32 localId) {
  void ObjectEvent_FaceDirection();

  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_Get(localId);
  objectEvent->scaleX = 0x10000;
  objectEvent = ObjectEvent_Get(localId);
  objectEvent->scaleY = 0x10000;
  FieldScript_SetMessageId(MsgLaliveroOhhTheyTookShebaHeaded);
  Script_ShowMessage(localId, 0);
  ObjectEvent_FaceDirection(localId, FACING_NORTH, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(localId, gLalivero_CryingScript);
}

/// Lalivero, after Venus Lighthouse: the woman with fair hair asks whether the
/// party is leaving Lalivero, and wishes it well either way.
static void EventScript_WomanAsksIfLeaving(s32 localId) {
  s32 message = MsgLaliveroLeavingLalivero;
  FieldScript_SetMessageId(message);
  FieldMessage_ShowAtObject(localId, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(message + 1);
  } else {
    FieldScript_SetMessageId(message + 2);
  }
  FieldScript_ShowMessageAndWait(localId, 0);
}

/// Lalivero, after Venus Lighthouse: the guard at the wharf's door ("I was
/// told to let you in if you showed up...") lets the party through and steps
/// aside (sets FLAG_LALIVERO_WHARF_OPEN).
static void EventScript_WharfGuardLetsPartyThrough(void) {

  GameFlag_Set(FLAG_LALIVERO_WHARF_OPEN);
  FieldScript_SetMessageId(MsgLaliveroWasToldLetInIf);
  FieldScript_ShowMessageAndWait(LOCALID_WHARF_GUARD, 0);
  ObjectEvent_SetSpeed(LOCALID_WHARF_GUARD, 0x10000, 0x8000);
  ObjectEvent_WalkByAndWait(LOCALID_WHARF_GUARD, -16, 0);
  ObjectMotion_ArmCallback(LOCALID_WHARF_GUARD, FACING_EAST, 0);
  FieldScript_WaitFrames(10);
}

/// The leader walks through one of Lalivero's doors. As the leader walks into a
/// doorway, the door (or the Sanctum's gate) swings open, the leader steps in
/// and the exit of the touched coord event is taken.
static void EventScript_EnterDoor(void) {
  s32 door = gFieldWork->coordEvent;
  s32 x = gLaliveroDoorCells[door][0];
  s32 y = gLaliveroDoorCells[door][1];

  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = OBJ_EVENT_FALLS;
  Sound_PlaySong(SE_DOOR_OPEN);
  if (door == MB_LALIVERO_SANCTUM_GATE) {
    MapGrid_AnimateCells(gLaliveroGateOpenSteps, (u16)x, (u16)y);
    ObjectEvent_WalkBy(LOCALID_PARTY_LEADER, 0, -16);
  } else {
    MapGrid_AnimateCells(gLaliveroDoorOpenSteps, (u16)x, (u16)y);
    ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -16);
  }
  FieldScript_WaitFrames(10);
  gFieldWork->transitionFrames = 16;
  Import_ScriptRequestExit(door);
}

/// Lalivero: a doorway with no door (coord events 8 and 11); leave by the
/// touched coord event's exit.
static void EventScript_EnterDoorway(void) {
  struct FieldWork *work = gFieldWork;
  s16 *trigger = (s16 *)&work->coordEvent;
  s32 exit = *trigger;

  work->transitionFrames = 16;
  Import_ScriptRequestExit(exit);
}

/// Lalivero, the first visit, arriving from the world map: the party and Iodem
/// find more of Tolbi's soldiers beaten in the street. Iodem asks whether they
/// fought a group of six; there were seven, the same group the soldiers had
/// been following, and one of them was Sheba, with the scholar Kraden
/// protecting her. They headed for the ruins beneath Babi Lighthouse, and
/// Faran, the master of Lalivero, followed them in. The soldiers send the party
/// on, and Iodem sets off for Babi Lighthouse.
static void EventScript_WatchWoundedSoldiers(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  FieldScript_SetMessageId(MsgLaliveroMoreTolbisSoldiersLayDefeated);
  // The party walks in from the south; the two soldiers lie on the ground.
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(248), PIXELS(424));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  Import_ObjectEventSetAnimation(LOCALID_WEST_SOLDIER, 0);
  Import_ObjectEventSetAnimation(LOCALID_EAST_SOLDIER, 0);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  // Iodem steps out from the party and looks them over.
  FieldMotion_LaunchFromFocusedObject(LOCALID_IODEM, 8, -16, FACING_NORTH);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_SetSpeed(LOCALID_IODEM, 0x10000, 0x8000);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -16);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_WEST_SOLDIER, 40);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_EAST_SOLDIER, 40);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_WEST_SOLDIER, 40);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  Script_OpenMessage(LOCALID_IODEM, 0);
  FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(30);
  // The soldiers know Iodem's voice: seven attackers took Sheba.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_WEST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_WEST_SOLDIER, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Import_ObjectEventSetAnimation(LOCALID_WEST_SOLDIER, ANIM_STAND);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_SOLDIER, 2);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_WEST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_EAST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_EAST_SOLDIER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_EAST_SOLDIER, 30);
  Import_ObjectEventSetAnimation(LOCALID_EAST_SOLDIER, ANIM_STAND);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_SOLDIER, 2);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_EAST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_EAST_SOLDIER, 20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_WEST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(20);
  // Iodem paces while they tell where the attackers went.
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  ObjectEvent_SetSpeed(LOCALID_IODEM, 0x1cccc, 0xe666);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 256, 360);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_EAST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_EAST_SOLDIER, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_EAST_SOLDIER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_IODEM, 0x19999, 0xcccc);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 256, 384);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_WEST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(LOCALID_IODEM, 0x13333, 0x9999);
  // Iodem resolves to follow; the soldiers send the party on.
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, 16);
  FieldScript_WaitFrames(10);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_WEST_SOLDIER, 40);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_EAST_SOLDIER, 40);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_EAST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_EAST_SOLDIER, 30);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_WEST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WEST_SOLDIER, 70);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  // Iodem walks back into the leader.
  ObjectEvent_SetSpeed(LOCALID_IODEM, 0x13333, 0x9999);
  Import_ObjectEventSetAnimation(LOCALID_IODEM, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_IODEM, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
  ObjectEvent_SetPosition(LOCALID_IODEM, 0, 0);
  FieldScript_WaitFrames(10);
  FieldScript_End();
}

/// Lalivero, coord event 50 after Venus Lighthouse, until Iodem has spoken
/// (FLAG_IODEM_FAREWELL_SEEN): Garet stops the leader, for Iodem asked them to
/// wait. Iodem comes out of Faran's house and tells the party that the misty
/// island they saw from the lighthouse may be Lemuria, which could save Babi's
/// life. The party promised Babi to find it, though it means first to find
/// Jenna, whom it is searching for, and Iodem gives it the Black Orb that
/// sails Babi's ship. Garet is glad to see the ocean at last.
static void EventScript_IodemGivesBlackOrb(void) {
  struct ObjectEvent *leader;

  GameFlag_Set(FLAG_IODEM_FAREWELL_SEEN);
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLaliveroWhereGoingIsaacIodemAsked);
  // Garet reminds the leader that Iodem asked them to wait; the answer
  // picks Mia's reply.
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 104, 376);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  FieldMotion_LaunchFromFocusedObject(LOCALID_GARET, -32, 0, FACING_EAST);
  FieldMotion_LaunchFromFocusedObject(LOCALID_MIA, -16, 16, FACING_NORTHEAST);
  ObjectMotion_LaunchFromFocusedObject(LOCALID_IVAN, 0, 16, FACING_NORTH);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  FieldScript_WaitFrames(30);
  Script_OpenMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  FieldScript_WaitFrames(10);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_MIA, 0);
    gFieldWork->message += 1;
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    gFieldWork->message += 1;
    Script_ShowMessage(LOCALID_MIA, 0);
  }
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(40);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  // The party lines up facing Iodem's door.
  ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, -16, 0);
  ObjectEvent_WalkBy(LOCALID_IVAN, 0, -16);
  ObjectEvent_WalkBy(LOCALID_MIA, 0, -8);
  ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 0, -16);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(80);
  // Iodem comes out: the door opens (cells 34, 0 drawn at 4, 18) and closes
  // behind him (cells 32, 0).
  ObjectEvent_SetSpeed(LOCALID_IODEM, 0xcccc, 0x6666);
  ObjectEvent_GetById(LOCALID_IODEM)->motionFlags = OBJ_EVENT_FALLS;
  ObjectMotion_SetActionVariant(LOCALID_IODEM, 2);
  MapGrid_CopyCellsLayered(34, 0, 1, 2, 4, 18);
  Script_PlaySong(SE_DOOR_OPEN);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetPosition(LOCALID_IODEM, PIXELS(72), PIXELS(312));
#if TBS_PATCH_2
  ObjectEvent_GetById(LOCALID_IODEM)->targetZ = OBJ_EVENT_NO_TARGET;
  ObjectEvent_SetPosition(LOCALID_IODEM, PIXELS(72), PIXELS(312));
#endif
  FieldScript_WaitFrames(20);
#if TBS_PATCH_2
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 72, 328);
#else
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, 16);
#endif
  // The door shuts.
  MapGrid_CopyCellIndexRect(32, 0, 1, 2, 4, 18);
  Script_PlaySong(SE_DOOR_CLOSE);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 16, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  FieldScript_WaitFrames(40);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  FieldScript_WaitFrames(20);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    gFieldWork->message += 1;
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    gFieldWork->message += 1;
    Script_ShowMessage(LOCALID_IODEM, 0);
  }
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(20);
  // Iodem takes out the Black Orb.
  ObjectEvent_SetPosition(LOCALID_BLACK_ORB, PIXELS(88), PIXELS(332));
  Script_ShowMessage(-1, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectMotion_Launch(LOCALID_MIA, 4, 13);
  ObjectMotion_Launch(LOCALID_MIA, 4, 30);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  // Arriving through the aerie, the party also talks about the ship they saw
  // at Idejima.
  if (GameFlag_Get(FLAG_SAW_SHIP_AT_IDEJIMA) != 0) {
    Lalivero_TalkAboutIdejimaShip();
  }
  FieldScript_SetMessageId(MsgLaliveroBabiToldMeShipAncients);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  // He holds the orb out, the leader takes it, and he steps back.
  ObjectEvent_SetSpeed(LOCALID_BLACK_ORB, 0xcccc, 0x6666);
  ObjectEvent_SetDestinationOffset(LOCALID_BLACK_ORB, 0, 16);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, 16);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetPosition(LOCALID_BLACK_ORB, 0, 0);
  gFieldWork->message += 1;
  PartyInventory_GiveItem(ITEM_BLACK_ORB, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_Get(LOCALID_IODEM)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -16);
  ObjectEvent_Get(LOCALID_IODEM)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH + 0x100, 0);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(50);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  FieldScript_WaitFrames(30);
  // Iodem goes back inside.
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, -16, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
  FieldScript_WaitFrames(20);
  MapGrid_CopyCellsLayered(34, 0, 1, 2, 4, 18);
  Script_PlaySong(SE_DOOR_OPEN);
  FieldScript_WaitFrames(10);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -16);
  ObjectEvent_SetPosition(LOCALID_IODEM, 0, 0);
  FieldScript_WaitFrames(10);
  // The door shuts.
  MapGrid_CopyCellIndexRect(32, 0, 1, 2, 4, 18);
  Script_PlaySong(SE_DOOR_CLOSE);
  FieldScript_WaitFrames(50);
  // The others are eager to sail, then walk back into the leader.
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(40);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetSpeed(LOCALID_GARET, 0x13333, 0x9999);
  ObjectEvent_SetSpeed(LOCALID_MIA, 0x13333, 0x9999);
  ObjectEvent_SetSpeed(LOCALID_IVAN, 0x13333, 0x9999);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_MIA, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
  ObjectEvent_SetPosition(LOCALID_MIA, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  FieldScript_WaitFrames(10);
  FieldScript_End();
}

/// The ship the party saw at Idejima was not Babi's, which is here in
/// Lalivero, but, Garet guesses, Saturos's.
static void Lalivero_TalkAboutIdejimaShip(void) {
  FieldScript_SetMessageId(MsgLaliveroThoughtShipWeSawAt);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 55);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  FieldScript_WaitFrames(65);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
}

/// Lalivero, coord event 51 after Venus Lighthouse: Garet turns the leader back
/// toward Babi's ship and the wharf the guard was keeping.
static void EventScript_TurnBackToTheShip(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLaliveroWhereGoingArentWeTaking);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 0, -16);
  FieldScript_End();
}

/// What the party can talk to, touch and search, which changes once they are
/// back from Venus Lighthouse.
const struct MapEvent *Lalivero_GetEvents(void) {
  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
    return gLaliveroEventsAfterLighthouse;
  }
  return gLaliveroEvents;
}

/// Sets Lalivero up as the party enters: the Black Orb Iodem will hold out,
/// the wounded soldiers of the first visit (and its event), or the sign at
/// the north exit after Venus Lighthouse.
s32 Lalivero_Initialize(void) {
  union ObjectEventOrEffect *fieldObject;
  struct Sprite *sprite;
  GlyphTransfer *icon;

  // Coming back from the lighthouse switches the town to its later state.
  if (gSaveBlock.entrance == ENTRANCE_FROM_AERIE) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
    GameFlag_Set(FLAG_SAW_SHIP_AT_IDEJIMA);
  }
  if (gSaveBlock.entrance == ENTRANCE_FROM_AERIE_QUIETLY) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
  }
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 24;

  // The three plants stand in animation 3 and block nothing.
  Import_ObjectEventSetAnimation(LOCALID_FIRST_PLANT, 3);
  ObjectEvent_Get(LOCALID_FIRST_PLANT)->collisionFlags = 0;
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_FIRST_PLANT), 0);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_PLANT + 1, 3);
  ObjectEvent_Get(LOCALID_FIRST_PLANT + 1)->collisionFlags = 0;
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_FIRST_PLANT + 1), 0);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_PLANT + 2, 3);
  ObjectEvent_Get(LOCALID_FIRST_PLANT + 2)->collisionFlags = 0;
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_FIRST_PLANT + 2), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_BLACK_ORB), 0);

  // The Black Orb Iodem hands over is drawn from its item icon, floating
  // 10 pixels up.
  fieldObject = (union ObjectEventOrEffect *)ObjectEvent_Get(LOCALID_BLACK_ORB);
  fieldObject->objectEvent.retainSpriteCache = 1;
  fieldObject->objectEvent.motionFlags = 0;
  sprite = fieldObject->objectEvent.sprite;
  fieldObject->objectEvent.y.fixed = PIXELS(10);
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
    ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_NORTH_EXIT_SIGN), 0);
    ObjectEvent_Get(LOCALID_NORTH_EXIT_SIGN)->collisionFlags = 0;
    MapGrid_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 14, 4);
    MapGrid_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 15, 4);
    MapGrid_CopyCellAttributes(WALL_CELL_X, WALL_CELL_Y, 1, 1, 16, 4);
    if (GameFlag_Get(FLAG_LALIVERO_WHARF_OPEN)) {
      ObjectEvent_SetPosition(LOCALID_WHARF_GUARD, PIXELS(56), PIXELS(184));
    }
  } else {
    // The first visit: the two soldiers lie on the ground, drawn under the
    // party with their own collision off, and the two cells each covers
    // become wall instead. The third cell copied, at x 78, lies outside the
    // town's 32 columns, where the camera never goes, and changes nothing.
    fieldObject =
        (union ObjectEventOrEffect *)ObjectEvent_Get(LOCALID_WEST_SOLDIER);
    fieldObject->objectEvent.collisionFlags = 0;
    fieldObject->objectEvent.priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    fieldObject->objectEvent.sprite->flags = 0;
    fieldObject->objectEvent.sprite->rotation = FACING_NORTH;
    fieldObject =
        (union ObjectEventOrEffect *)ObjectEvent_Get(LOCALID_EAST_SOLDIER);
    fieldObject->objectEvent.collisionFlags = 0;
    fieldObject->objectEvent.priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    fieldObject->objectEvent.sprite->flags = 0;
    fieldObject->objectEvent.sprite->rotation = FACING_SOUTH;
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 13, 23);
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 14, 23);
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 78, 23);
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 17, 23);
    MapGrid_CopyCellAttributes(20, 23, 1, 1, 18, 23);
    if ((gSaveBlock.entrance == LALIVERO_FROM_WORLD_MAP_EAST ||
         gSaveBlock.entrance == LALIVERO_FROM_WORLD_MAP_WEST) &&
        !Script_GetFlag(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN)) {
      GameFlag_Set(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN);
      // The woman and the girl who wander nearest the soldiers stay put during
      // the event.
      fieldObject = (union ObjectEventOrEffect *)ObjectEvent_Get(
          LOCALID_WOMAN_OUR_ONLY_HOPE);
      fieldObject->objectEvent.paused = 1;
      fieldObject = (union ObjectEventOrEffect *)ObjectEvent_Get(
          LOCALID_GIRL_SHEBAS_POWERS);
      fieldObject->objectEvent.paused = 1;
      EventScript_WatchWoundedSoldiers();
      fieldObject = (union ObjectEventOrEffect *)ObjectEvent_Get(
          LOCALID_WOMAN_OUR_ONLY_HOPE);
      fieldObject->objectEvent.paused = 0;
      fieldObject = (union ObjectEventOrEffect *)ObjectEvent_Get(
          LOCALID_GIRL_SHEBAS_POWERS);
      fieldObject->objectEvent.paused = 0;
    }
  }
  return 0;
}

/// The sobbing of LOCALID_VILLAGER_THEY_TOOK_SHEBA: the sprite squashes and
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
    {MAP_TABLE_END},
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
struct ObjectEventTemplate gLaliveroObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLaliveroObjectEventTemplatesAfterLighthouse[] = {
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
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
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

struct MapEvent gLaliveroEvents[] = {
    { MAP_EVENT_EXIT, 20, CONDITION_ALWAYS, LALIVERO_TO_WORLD_MAP_EAST },
    { MAP_EVENT_EXIT, 21, CONDITION_ALWAYS, LALIVERO_TO_WORLD_MAP_WEST },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoorway) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoorway) },
    { MAP_EVENT_TALK, LOCALID_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroUhhnnWeWereTakenBySurprise) },
    { MAP_EVENT_TALK, LOCALID_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyWereTerribleWeWereNo) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToVillagerTheyTookSheba) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorIsOurOnlyHopeIf) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisAllHappenedBecauseTolbiTook) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranFollowedActorImSureHell) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheRumblingAtTheRuinsMust) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroThereWasAnEarthquakeAtThe) },
    { MAP_EVENT_TALK, LOCALID_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfIWereBiggerIWould) },
    { MAP_EVENT_TALK, LOCALID_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroIBetActorCanUseHer) },
    { MAP_EVENT_TALK, LOCALID_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroBabisMostPreciousBelongingIsKept) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfOnlyTheyDidntHaveActor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyUsedFireOnUsThats) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroCursesWeCouldntStopActorAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhatDoTheyPlanToDo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeNeverShouldHaveLetTolbi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranStillDoesNotReturnWhat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeHaveSeenManyOmensOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheStonesWeStackedForThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroITriedToStepInFront) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyAllHaveALessonComing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWonderWhatBabisMostPrecious) },
    { MAP_EVENT_TALK, LOCALID_TORCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToTorch) },
    { BG_EVENT(BG_EVENT_JAR), 100, FLAG_LALIVERO_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { BG_EVENT(BG_EVENT_STONE_PILLAR), 101, FLAG_LALIVERO_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    // XXX (no evidence): why this chest opens only facing west, when
    // gLaliveroEventsAfterLighthouse opens it from any side; the chest and the
    // ground around it are the same on both visits.
    { BG_EVENT_CHEST_FACING(FACING_WEST), 102, FLAG_LALIVERO_WARRIORS_HELM, FIND_ITEM(ITEM_WARRIORS_HELM) },
    { MAP_EVENTS_END },
};

struct MapEvent gLaliveroEventsAfterLighthouse[] = {
    { MAP_EVENT_COORD, 50, FLAG_IODEM_FAREWELL_SEEN, EVENT_SCRIPT(EventScript_IodemGivesBlackOrb) },
    { MAP_EVENT_COORD, 51, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TurnBackToTheShip) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoorway) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoorway) },
    { MAP_EVENT_TALK, LOCALID_WHARF_GUARD, FLAG_LALIVERO_WHARF_OPEN, EVENT_SCRIPT(EventScript_WharfGuardLetsPartyThrough) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_WHARF_GUARD, FLAG_LALIVERO_WHARF_OPEN, EVENT_SCRIPT(EventScript_WharfGuardLetsPartyThrough) },
    { MAP_EVENT_TALK, LOCALID_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroHaHaIThoughtTheyHad) },
    { MAP_EVENT_TALK, LOCALID_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWillNeverLetMyGuard) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhereCouldActorBeNowIm) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WomanAsksIfLeaving) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroISawTheBeaconOfVenus) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheExplosionInTheVenusRuins) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroThePeninsulaBehindVenusLighthouseIs) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroICantBelieveYouEscapedVenus) },
    { MAP_EVENT_TALK, LOCALID_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfYouDefeatedThatEvilBunch) },
    { MAP_EVENT_TALK, LOCALID_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorJumpedIntoTheOceanFrom) },
    { MAP_EVENT_TALK, LOCALID_NORTH_EXIT_SIGN, CONDITION_ALWAYS, MSGNO(MsgLaliveroNoTrespassingDangerBeyondThisPoint) },
    { MAP_EVENT_TALK, LOCALID_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroApparentlyAShipHasBeenDocked) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIAmAllHealedNowBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWantedToBeAPart) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfActorUsedHerGreatPowers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroAfterEverythingThatsHappenedLaliveroNo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseHasBeenTheSymbol) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroIodemOfTolbiDoesntSeemToo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheOceanIsSoRoughSince) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseIsStillStandingStrong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroYouMustBeGreatWarriorsTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorWouldBeFineEvenAfter) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisSureSeemsLikeAStrange) },
    { MAP_EVENT_TALK, LOCALID_TORCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToTorch) },
    { BG_EVENT(BG_EVENT_JAR), 100, FLAG_LALIVERO_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { BG_EVENT(BG_EVENT_STONE_PILLAR), 101, FLAG_LALIVERO_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_LALIVERO_WARRIORS_HELM, FIND_ITEM(ITEM_WARRIORS_HELM) },
    { MAP_EVENTS_END },
};
// clang-format on
