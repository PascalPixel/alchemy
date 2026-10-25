/// @file
/// Vault (MapId_VaultRevisit), the town once the party has reached Kalay, on
/// the same layout as Vault's first map (vault.c). The world map still leads
/// into the first map, but once FLAG_KALAY_REACHED is set its setup sends the
/// party straight on here (VAULT_REVISIT_FROM_VAULT); Vault Sanctum, Vault
/// Cave, the jail and the houses (vault_interiors_revisit.c) lead back here
/// directly. This map replaces the first for the rest of the game.
///
/// The thieves the party caught have broken out of jail, and most townspeople
/// say so, or speak of Hammet and Lunpa, in plain message rows. Four speak
/// through scripts that remember who has spoken in map section flags
/// (FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED to
/// FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL), so that a Mind Read on one not yet
/// spoken to plays the conversation first. The old man's praise also sets
/// FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES, as the mayor's welcome indoors does.
///
/// Searching the bell rings it, and a ring while FLAG_VAULT_SAP_CALLED_BY_BELL
/// is clear calls Sap, a Venus Djinni, out to stand by the bell; spoken to, Sap
/// joins the party. Coming in by a gate clears the call, so Sap hides again
/// until it has joined. The dog, given a Bone, runs to a spot and jumps there
/// three times (FLAG_VAULT_DOG_SHOWN_PASSAGE, shared with vault.c); Reveal then
/// shows the hidden passage at that spot, whose cell (MB_VAULT_HIDDEN_PASSAGE)
/// leads down into Vault Cave, and the cave mouth in the northeast cliff
/// (MB_VAULT_CLIFF_CAVE_MOUTH) is a second way in. A wooden box holds a Nut,
/// and the well rumbles with water. The cells the events answer are enum
/// VaultMetatileBehavior (vault.h); the jail door brings the party in by
/// STONE_ROOMS_FROM_VAULT_REVISIT, where a lone prisoner sits in the thieves'
/// place.
///
/// The inn's roof is mended: the 3-by-3 block of cells Map_Initialize copies
/// from (45, 41) to (8, 45) lies under map grid layer 2's window, on the inn's
/// roof, and draws the thatch whole where vault.c's town shows the hole the
/// fiery rock burnt in it (render probe vault-revisit, the town before and
/// after).

#include "save_block.h"
#include "djinn_ids.h"
#include "window.h"
#include "unit/map/vault_revisit.h"
#include "vault.h"
#include "unit/map/common/vault_range.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "object_event.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "iwram_call.h"
#include "tbs_edition.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "call.h"
#include "flags.h"
#include "songs.h"

/// The townspeople, the dog and Sap, numbering
/// gVaultRevisit_ObjectEventTemplates' rows from LOCALID_FIRST_PLACED.
enum VaultRevisitLocalId {
  /// vault.c's welcoming young man, who recognizes the party.
  /// ★ Named from the game: "You've returned to Vault!"
  /// (MsgVaultWarriorsWhoCaptured)
  LOCALID_WELCOMING_YOUNG_MAN = LOCALID_FIRST_PLACED,
  /// A woman with fair hair who brings the news from Kalay.
  /// ★ Named from the game: "You've probably already heard, but the bridge to
  /// Kalay has been fixed." (MsgVaultYouveProbablyAlreadyHeardButThe)
  LOCALID_WOMAN_ON_THE_FIXED_BRIDGE,
  /// A burly grey-haired man who worries about Hammet.
  /// ★ Named from the game: "I wonder what happened to Hammet when he went to
  /// Lunpa..." (MsgVaultIWonderWhatHappenedToHammet)
  LOCALID_VILLAGER_WONDERING_ABOUT_HAMMET,
  /// A woman.
  /// ★ Named from the game: "There have been rumors of thieves from Lunpa
  /// running wild" (MsgVaultRumorsThievesLunpa)
  LOCALID_WOMAN_ON_LUNPAS_THIEVES,
  /// An old man with a cane who has talked with the mayor.
  /// ★ Named from the game: "I was just talking about you with our mayor"
  /// (MsgVaultTalkingMayorStrong)
  LOCALID_MAYORS_VISITOR,
  /// An old woman.
  /// ★ Named from the game: "If the volcano stays calm and the southern bridge
  /// stays fixed, we won't have any worries at all." (MsgVaultVolcanoStaysCalm)
  LOCALID_OLD_WOMAN_ON_THE_VOLCANO,
  /// vault.c's two children, walking round the well again; the boy greets the
  /// party. "Hey, it's that warrior guy!"
  /// ★ Named from what the player sees: a blond boy walking round the well
  /// (its render)
  LOCALID_BOY_ROUND_THE_WELL,
  /// Speaks of the grown-ups. "Grown-ups always get so scared"
  /// ★ Named from what the player sees: a girl with orange hair walking round
  /// the well (its render)
  LOCALID_GIRL_ROUND_THE_WELL,
  /// vault.c's strongman on watch. "Did you guys check out the jail?"
  /// ★ Named from the game: "Nobody's stealing anything on my watch!"
  /// (MsgVaultNobodysStealingAnything)
  LOCALID_STRONGMAN_ON_WATCH,
  /// A bearded man in a long robe who takes the thieves' escape calmly.
  /// ★ Named from the game: "The thieves escaped, but I think it's all right.
  /// It's all part of a divine plan." (MsgVaultTheThievesEscapedButIThink)
  LOCALID_VILLAGER_DIVINE_PLAN,
  /// ★ Named from what the player sees: a white dog sitting among the graves
  /// (its render)
  LOCALID_VAULT_DOG,
  /// vault.c's young man fixing the roof, standing where he did there, his work
  /// done.
  /// ★ Named from the game: "There, you see? The roof repairs are all
  /// finished." (MsgVaultThereYouSeeTheRoofRepairs)
  LOCALID_YOUNG_MAN_FIXING_THE_ROOF,
  /// Sap, a Venus Djinni, hiding until the bell calls it. The row before it,
  /// sprite 0xf8 placed at 0, 0, is never referred to.
  /// ★ Named from the game: "Sap" (MsgDjinnName)
  LOCALID_SAP = LOCALID_FIRST_PLACED + 13
};

enum { VAULT_PROXIMITY_REACTION_PAUSED = 2 };

/// Map section flags, kept while the party stays in Vault's maps: what the
/// townspeople have already said. A Mind Read before someone has spoken plays
/// the conversation first.
enum {
  /// ★ Named from the game: "Hey, it's that warrior guy!"
  /// (MsgVaultWarriorGuy)
  FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED = FLAG_MAPSEC_TEMP_00,
  /// ★ Named from the game: "You're the warriors who captured the thieves"
  /// (MsgVaultWarriorsWhoCaptured)
  FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY = FLAG_MAPSEC_TEMP_05,
  /// ★ Named from the game: "You're all really strong"
  /// (MsgVaultTalkingMayorStrong)
  FLAG_VAULT_REVISIT_PARTY_PRAISED = FLAG_MAPSEC_TEMP_06,
  /// ★ Named from the game: "I'm so glad you came back to Vault!"
  /// (MsgVaultWarriorGuy)
  FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE = FLAG_MAPSEC_TEMP_07,
  /// ★ Named from the game: "did you guys check out the jail?"
  /// (MsgVaultGuysCheckJail)
  FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL = FLAG_MAPSEC_TEMP_08
};

// Shared effect and proximity routines precede the map's entry points.
#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_distance.h"
#include "common/vault_range.h"
#include "common/vault_nearby.h"

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gVaultRevisit_WarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gVaultRevisit_Warps; }

/// Returns the villagers, dog, and Djinni object event templates.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  return gVaultRevisit_ObjectEventTemplates;
}

/// Stands the object event still while showing the message already selected.
static void VaultRevisit_ShowStandingObjectEventMessage(s32 localId) {
  FieldScript_Start();
  Import_ObjectEventSetAnimation(localId, ANIM_STAND);
  FieldScript_ShowMessageAndWait(localId, 0);
  FieldScript_End();
}

/// The party is recognized. "You're the warriors who captured the thieves a
/// while back, aren't you? You've returned to Vault!"
static void EventScript_RecognizeThiefCatchers(void) {
  FieldScript_Start();
  ObjectMotion_SetAngleToward(LOCALID_WELCOMING_YOUNG_MAN, LOCALID_PARTY_LEADER,
                              2);
  GameFlag_Set(FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY);
  FieldScript_SetMessageId(MsgVaultWarriorsWhoCaptured);
  FieldScript_ShowMessageAndWait(LOCALID_WELCOMING_YOUNG_MAN, 0);
  FieldScript_End();
}

/// There are rumors of thieves from Lunpa running wild, yet they have been
/// quiet.
static void EventScript_TalkOfLunpaThieves(void) {

  FieldScript_SetMessageId(MsgVaultRumorsThievesLunpa);
  ObjectEvent_LinkPair(LOCALID_WOMAN_ON_LUNPAS_THIEVES, LOCALID_PARTY_LEADER,
                       2);
  VaultRevisit_ShowStandingObjectEventMessage(LOCALID_WOMAN_ON_LUNPAS_THIEVES);
}

/// The old man was just talking about the party with the mayor: they are so
/// strong, let those thieves attack. Sets FLAG_VAULT_REVISIT_PARTY_PRAISED and
/// FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES (0x868).
static void EventScript_PraisePartyStrength(void) {

  FieldScript_Start();
  ObjectMotion_SetAngleToward(LOCALID_MAYORS_VISITOR, LOCALID_PARTY_LEADER, 2);
  GameFlag_Set(FLAG_VAULT_REVISIT_PARTY_PRAISED);
  GameFlag_Set(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  FieldScript_SetMessageId(MsgVaultTalkingMayorStrong);
  FieldScript_ShowMessageAndWait(LOCALID_MAYORS_VISITOR, 0);
  FieldScript_End();
}

/// If the volcano stays calm and the southern bridge stays fixed, there is
/// nothing to worry about.
static void EventScript_HopeVolcanoStaysCalm(void) {

  FieldScript_SetMessageId(MsgVaultVolcanoStaysCalm);
  ObjectEvent_LinkPair(LOCALID_OLD_WOMAN_ON_THE_VOLCANO, LOCALID_PARTY_LEADER,
                       2);
  VaultRevisit_ShowStandingObjectEventMessage(LOCALID_OLD_WOMAN_ON_THE_VOLCANO);
}

/// The boy round the well stops circling. "Hey, it's that warrior guy!" and
/// "Yeah, and Ivan, too." the first time
/// (FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED), then "I'm so glad you came back to
/// Vault!" Bit 1 of the boy's timer keeps Vault_UpdateWalkingVillager from
/// moving it while it talks.
static void EventScript_GreetWarriorGuy(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_Get(LOCALID_BOY_ROUND_THE_WELL);
  u16 facing = objectEvent->facing;
  s32 text;

  objectEvent->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  FieldScript_Start();
  text = MsgVaultWarriorGuy;
  FieldScript_SetMessageId(text);
  Import_ObjectEventSetAnimation(LOCALID_BOY_ROUND_THE_WELL, 0);
  ObjectEvent_LinkPair(LOCALID_BOY_ROUND_THE_WELL, LOCALID_PARTY_LEADER, 2);
  if (Script_GetFlag(FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED) == 0) {
    ObjectEvent_ShowEmote(LOCALID_BOY_ROUND_THE_WELL,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Script_ShowMessageAndWait(LOCALID_BOY_ROUND_THE_WELL, 0, 10);
    Script_ShowMessageAndWait(LOCALID_BOY_ROUND_THE_WELL, 0, 10);
    GameFlag_Set(FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED);
  }
  FieldScript_SetMessageId(text + 2);
  Script_ShowMessageAndWait(LOCALID_BOY_ROUND_THE_WELL, 0, 10);
  objectEvent->facing = facing;
  WaitFrames(1);
  FieldScript_End();
  objectEvent->timer = 1;
  GameFlag_Set(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE);
}

/// The girl round the well stops. "I wish Vault's elders were as brave as you!"
static void EventScript_WishEldersWereBrave(void) {
  struct ObjectEvent *objectEvent;
  s16 originalFacing;

  objectEvent = ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL);
  originalFacing = objectEvent->facing;
  objectEvent->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultWishVaultsElders);
  Import_ObjectEventSetAnimation(LOCALID_GIRL_ROUND_THE_WELL, 0);
  ObjectEvent_LinkPair(LOCALID_GIRL_ROUND_THE_WELL, LOCALID_PARTY_LEADER, 2);
  FieldScript_ShowMessageThenWait(LOCALID_GIRL_ROUND_THE_WELL, 0, 10);
  objectEvent->facing = originalFacing;
  WaitFrames(1);
  FieldScript_End();
  objectEvent->timer = 0;
}

/// The strongman on watch asks about the jail. "Did you guys check out the
/// jail?" Yes: how did the thugs break out? No: the thieves broke out and got
/// away.
static void EventScript_AskAboutJailbreak(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultGuysCheckJail);
  ObjectEvent_LinkPair(LOCALID_STRONGMAN_ON_WATCH, LOCALID_PARTY_LEADER, 2);
  FieldMessage_ShowAtObject(LOCALID_STRONGMAN_ON_WATCH, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) !=
      0) {
    gFieldWork->message += 1;
  }
  FieldScript_ShowMessageAndWait(LOCALID_STRONGMAN_ON_WATCH, 0);
  GameFlag_Set(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
  FieldScript_End();
}

/// Mind Read on the welcoming young man before he has spoken: he turns and
/// recognizes the party.
static void EventScript_ReadMindAndRecognizeThiefCatchers(void) {
  FieldScript_Start();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WELCOMING_YOUNG_MAN, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAngleToward(LOCALID_WELCOMING_YOUNG_MAN, LOCALID_PARTY_LEADER,
                              20);
  GameFlag_Set(FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY);
  FieldScript_SetMessageId(MsgVaultWarriorsWhoCaptured);
  FieldScript_ShowMessageThenWait(LOCALID_WELCOMING_YOUNG_MAN, 0, 20);
  FieldScript_End();
}

/// Mind Read on the woman who talks of Lunpa's thieves: since Dodonpa took
/// over, Lunpa's outlaws are no better than thieves, and there must be a reason
/// they are so quiet.
static void EventScript_ReadMindOnDodonpa(void) {
  FieldScript_SetMessageId(MsgVaultOnceDodonpaTook);
  ObjectEvent_GetById(LOCALID_WOMAN_ON_LUNPAS_THIEVES)->paused = 1;
  VaultRevisit_ShowStandingObjectEventMessage(LOCALID_WOMAN_ON_LUNPAS_THIEVES);
  ObjectEvent_GetById(LOCALID_WOMAN_ON_LUNPAS_THIEVES)->paused = 0;
}

/// Mind Read before the old man has spoken: he turns and praises the party.
static void EventScript_ReadMindAndPraisePartyStrength(void) {
  FieldScript_Start();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MAYORS_VISITOR, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAngleToward(LOCALID_MAYORS_VISITOR, LOCALID_PARTY_LEADER, 20);
  GameFlag_Set(FLAG_VAULT_REVISIT_PARTY_PRAISED);
  GameFlag_Set(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  FieldScript_SetMessageId(MsgVaultTalkingMayorStrong);
  FieldScript_ShowMessageThenWait(LOCALID_MAYORS_VISITOR, 0, 20);
  FieldScript_End();
}

/// Mind Read: it would have been great to have everything solved before
/// winter.
static void EventScript_ReadMindOnWinter(void) {
  FieldScript_SetMessageId(MsgVaultGreatEverythingSolved);
  ObjectEvent_GetById(LOCALID_OLD_WOMAN_ON_THE_VOLCANO)->paused = 1;
  VaultRevisit_ShowStandingObjectEventMessage(LOCALID_OLD_WOMAN_ON_THE_VOLCANO);
  ObjectEvent_GetById(LOCALID_OLD_WOMAN_ON_THE_VOLCANO)->paused = 0;
}

/// Mind Read on the boy round the well: watching the party makes one feel
/// brave, or, before the boy has spoken, the greeting.
static void EventScript_ReadMindOnFeelingBrave(void) {
  ObjectEvent_Get(LOCALID_BOY_ROUND_THE_WELL)->timer |=
      VAULT_PROXIMITY_REACTION_PAUSED;
  FieldScript_Start();
  if (Script_GetFlag(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE) != 0) {
    FieldScript_SetMessageId(MsgVaultWatchingGuysMakes);
    VaultRevisit_ShowStandingObjectEventMessage(LOCALID_BOY_ROUND_THE_WELL);
  } else {
    EventScript_GreetWarriorGuy();
    GameFlag_Set(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE);
  }
  FieldScript_End();
  ObjectEvent_Get(LOCALID_BOY_ROUND_THE_WELL)->timer = 1;
}

/// Mind Read on the girl round the well: grown-ups always get so scared.
static void EventScript_ReadMindOnScaredGrownUps(void) {
  {
    struct ObjectEvent *objectEvent =
        ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL);
    objectEvent->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  }
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultGrownUpsAlways);
  VaultRevisit_ShowStandingObjectEventMessage(LOCALID_GIRL_ROUND_THE_WELL);
  FieldScript_End();
  {
    s32 clear = 0;
    struct ObjectEvent *objectEvent =
        ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL);
    objectEvent->timer = clear;
  }
}

/// Mind Read on the strongman on watch: before he has asked about the jail he
/// asks now; afterwards, the thieves must know the secret of the jail's lock.
static void EventScript_ReadMindOnJailKey(void) {
  s32 requestHeard;
  s32 unpaused;

  requestHeard = Script_GetFlag(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
  if (requestHeard == 0) {
    FieldScript_Start();
    ObjectEvent_Get(LOCALID_STRONGMAN_ON_WATCH)->paused = 1;
    Import_ObjectEventSetAnimation(LOCALID_STRONGMAN_ON_WATCH, ANIM_STAND);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_STRONGMAN_ON_WATCH, 1);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgVaultGuysCheckJail);
    ObjectEvent_LinkPair(LOCALID_STRONGMAN_ON_WATCH, LOCALID_PARTY_LEADER, 2);
    Script_OpenMessage(LOCALID_STRONGMAN_ON_WATCH, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) !=
        0) {
      gFieldWork->message++;
    }
    Script_ShowMessage(LOCALID_STRONGMAN_ON_WATCH, 0);
    ObjectEvent_Get(LOCALID_STRONGMAN_ON_WATCH)->paused = requestHeard;
    ObjectEvent_SetBuiltinAction(LOCALID_STRONGMAN_ON_WATCH, 2);
    FieldScript_End();
    GameFlag_Set(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
  } else {
    FieldScript_SetMessageId(MsgVaultNotLikeEasy);
    ObjectEvent_Get(LOCALID_STRONGMAN_ON_WATCH)->paused = 1;
    VaultRevisit_ShowStandingObjectEventMessage(LOCALID_STRONGMAN_ON_WATCH);
    unpaused = 0;
    ObjectEvent_Get(LOCALID_STRONGMAN_ON_WATCH)->paused = unpaused;
  }
}

/// Gives the object event vertical velocity, moves it, and waits briefly for it
/// to settle.
static void VaultRevisit_JumpObjectEventToAndWait(int localId, int x, int z,
                                                  int velocityY) {
  struct ObjectEvent *jumpingObjectEvent = ObjectEvent_GetById(localId);
  int frames;
  ObjectMotion_SetSpeedParameters(localId, PIXELS(3), PIXELS(3) / 2);
  jumpingObjectEvent->gravity = PIXELS(1) / 2;
  jumpingObjectEvent->bounce = 0;
  jumpingObjectEvent->velocityY = velocityY;
  ObjectEvent_SetSpriteFlags(jumpingObjectEvent, 0);
  ObjectMotion_SetPositionAndCommit(localId, x, z);
  Import_ObjectEventSetPosition(localId, x << 16, z << 16);
  for (frames = 60; frames != 0; --frames) {
    WaitFrames(1);
    if ((s16)((u32)jumpingObjectEvent->velocityY >> 16) == 0)
      break;
  }
  ObjectEvent_SetSpriteFlags(jumpingObjectEvent, 1);
  jumpingObjectEvent->gravity = PIXELS(1);
}

/// Searching the bell rings it (SE_BELL). The first ring since the party came
/// in from the world map makes Sap jump out of hiding and walk to the bell, at
/// (408, 120), and sets FLAG_VAULT_SAP_CALLED_BY_BELL.
static void EventScript_RingBell(void) {
  FieldScript_Start();
  Script_PlaySong(SE_BELL);
  FieldScript_WaitFrames(40);
  if (Script_GetFlag(FLAG_VAULT_SAP_CALLED_BY_BELL) == 0) {
    ObjectEvent_SetAttachedEffect(LOCALID_SAP, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
    ObjectMotion_Launch(LOCALID_SAP, 4, 0);
    FieldScript_WaitFrames(12);
    ObjectMotion_Launch(LOCALID_SAP, 4, 0);
    FieldScript_WaitFrames(20);
    // Let the Djinni leap from hiding before it walks to the bell.
    VaultRevisit_JumpObjectEventToAndWait(LOCALID_SAP, 392, 104, PIXELS(7));
    FieldScript_WaitFrames(20);
    ObjectEvent_WalkToAndWait(LOCALID_SAP, 408, 104);
    ObjectEvent_WalkToAndWait(LOCALID_SAP, 408, 120);
    GameFlag_Set(FLAG_VAULT_SAP_CALLED_BY_BELL);
  }
  FieldScript_End();
}

/// Walks the leader into a doorway during the sixteen-frame warp delay.
static void Vault_WalkThroughExit(s32 x, s32 z, s32 warpNumber) {
  s32 leaderId = LOCALID_PARTY_LEADER;

  ObjectEvent_SetSpeed(leaderId, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkTo(leaderId, x, z);
  gFieldWork->transitionFrames = 16;
  Import_ScriptRequestWarp(warpNumber);
}

/// The door of the gem owner's house opens and the party walks in.
/// It's VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHEAST, into the house of
/// the man whose precious stone a cat ate.
static void EventScript_EnterGemOwnersHouse(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVaultRevisit_GemOwnersHouseDoorCells, 56,
                                   19);
  Vault_WalkThroughExit(408, 320,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHEAST);
}

/// The door of a child's home opens and the party walks in.
/// It's VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTH, into the house of the
/// child who doubts the thieves were from Lunpa.
static void EventScript_EnterChildsHouse(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVaultRevisit_ChildsHouseDoorCells, 50, 18);
  Vault_WalkThroughExit(312, 304,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTH);
}

/// The door of VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHWEST opens into
/// the inn.
static void EventScript_EnterInn(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVaultRevisit_InnDoorCells, 44, 17);
  Vault_WalkThroughExit(216, 288,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHWEST);
}

/// The item shop's door opens and the party walks in.
/// It's VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_EAST, into the item shop,
/// where the couple who talk of Colosso also stand. The leader is
/// drawn at sprite priority 3 as he walks in, as at the weapon and armor shop
/// and the jail, and the metatile behavior of the doorway, a column of three
/// cells at (23, 12), is replaced so he can pass.
static void EventScript_EnterItemShop(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct Sprite *sprite = objectEvent->sprite;

  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVaultRevisit_ItemShopDoorCells, 54, 13);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_EAST);
}

/// The weapon and armor shop's door opens and the party walks in.
/// It's VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTH, drawn like the item
/// shop's.
static void EventScript_EnterWeaponAndArmorShop(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct Sprite *sprite = objectEvent->sprite;

  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVaultRevisit_WeaponAndArmorShopDoorCells,
                                   49, 10);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTH);
}

/// The door of VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTHWEST opens into
/// the mayor's house.
static void EventScript_EnterMayorsHouse(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVaultRevisit_MayorsHouseDoorCells, 38, 6);
  Vault_WalkThroughExit(120, 144,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTHWEST);
}

// The remaining doorway helper retains its position in the overlay.
#include "common/vault_map_cells.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

/// The jail door opens and the party walks in.
/// It's drawn behind the doorway as it goes in, to
/// STONE_ROOMS_FROM_VAULT_REVISIT, where the lone prisoner sits.
static void EventScript_EnterJail(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct Sprite *sprite = objectEvent->sprite;

  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVaultRevisit_JailDoorCells, 35, 9);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, VAULT_REVISIT_TO_STONE_ROOMS);
}

// Shared entrance toggles precede the Djinni and dog events.
#include "common/vault_toggle.h"

/// Talking to Sap, a Venus Djinni: it joins the party.
static void EventScript_SapJoins(void) {
  Djinn_JoinParty(LOCALID_SAP, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP));
}

/// Jumps at the hidden passage, then pauses so the party can see the clue.
static inline void VaultRevisit_DogJumpAtPassage(void) {
  ObjectMotion_Launch(LOCALID_VAULT_DOG, 6, 0);
  FieldScript_WaitFrames(30);
}

/// Using a Bone on the dog: it takes the Bone, runs to the hidden passage at
/// (216, 408) and jumps there three times, then steps back and sets
/// FLAG_VAULT_DOG_SHOWN_PASSAGE, which places it there, a step back, on later
/// arrivals.
static void EventScript_GiveBoneToDog(void) {
  PartyInventory_Discard(ITEM_BONE);
  FieldScript_Start();
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_VAULT_DOG, 2);
  ObjectEvent_SetSpeed(LOCALID_VAULT_DOG, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_VAULT_DOG, 216, 408);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_VAULT_DOG, FACING_SOUTH, 20);
  // Repeat the clue at the concealed passage, then step back.
  VaultRevisit_DogJumpAtPassage();
  VaultRevisit_DogJumpAtPassage();
  VaultRevisit_DogJumpAtPassage();
  ObjectEvent_WalkToAndWait(LOCALID_VAULT_DOG, 216, 392);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_VAULT_DOG, FACING_SOUTH, 20);
  GameFlag_Set(FLAG_VAULT_DOG_SHOWN_PASSAGE);
  FieldScript_End();
}

/// Covers the hidden passage again, tiles and metatile behaviors, when Reveal
/// ends.
static void EventScript_HidePassage(void) {

  MapGrid_CopyCellAttributes(41, 43, 1, 1, 13, 25);
  MapGrid_CopyCellsTo(40, 42, 12, 22, 3, 3);
}

/// Reveal shows the hidden passage where the dog jumps: its tiles, and the
/// metatile behavior of cell (13, 25), which lets the party step onto metatile
/// behavior 40 and down into Vault Cave.
static void EventScript_RevealPassage(void) {

  MapGrid_CopyCellAttributes(37, 43, 1, 1, 13, 25);
  MapGrid_CopyCellsTo(36, 42, 12, 22, 3, 3);
}

/// The dog barks; carrying a Bone before it has shown the passage lets the
/// party give it one: the main menu opens as the talk ends.
static void EventScript_TalkToDog(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultRuffRrruff2);
  Import_ObjectEventSetAnimation(LOCALID_VAULT_DOG, 0);
  ObjectEvent_LinkPair(LOCALID_VAULT_DOG, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(2);
  FieldScript_ShowMessageAndWait(LOCALID_VAULT_DOG, 0);
  Import_ObjectEventSetAnimation(LOCALID_VAULT_DOG, ANIM_STAND);

  if (PartyInventory_FindMember(ITEM_BONE) != -1 &&
      GameFlag_Get(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0) {
    gFieldWork->menu = 1;
  }

  FieldScript_End();
}

/// Describes the water heard at the bottom of the well.
static void EventScript_PeerIntoWell(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgVaultCanHearWaterRumblingDown, 1);
  FieldScript_End();
}

/// Returns the town's talk, bg, Psynergy, and doorway events.
const struct MapEvent *Map_GetEvents(void) { return gVaultRevisit_Events; }

/// Patched editions keep Sap by the bell when the party keeps its position,
/// as on a return from battle.
static inline s32 VaultRevisit_ShouldKeepBellDjinniPosition(void) {
#if TBS_PATCH_1
  return GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
#else
  return 0;
#endif
}

/// Sets up Vault as the party finds it after reaching Kalay.
/// On arrival it restores the passage's metatile behaviors
/// (common/vault_toggle.h), creates the still object of sprite 223 on the well
/// that shows nothing, mends the inn's roof, which vault.c
/// leaves holed, and hooks the two children round the well to
/// Vault_UpdateWalkingVillager. The dog waits at the hidden passage once it has
/// shown it. Coming in by a gate (WARP_DEST_DEFAULT to
/// VAULT_REVISIT_NORTH_UNUSED, forwarded from vault.c) before Sap has joined
/// clears FLAG_VAULT_SAP_CALLED_BY_BELL, so Sap is hidden again; while it is
/// called and has not joined, it stands by the bell. Coming out of Vault
/// Sanctum (VAULT_REVISIT_FROM_SANCTUM) clears FLAG_SHOW_LOCATION_NAME, and
/// coming up from Vault Cave (VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE) clears
/// FLAG_WARP_ARRIVAL_STAIRS_UP, so the leader's arrival animation does not
/// play.
s32 Map_Initialize(void) {

  struct ObjectEvent *objectEvent;
  s32 mode;

  // Keep the bridge deck after a battle, and mend the inn's roof.
  if (GameFlag_Get(FLAG_VAULT_ON_BRIDGE))
    MapGrid_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  // A still object on the well that shows nothing. XXX (no evidence): what it
  // was meant to show; its tiles in video memory are blank and nothing refers
  // to it.
  ObjectEvent_CreateConfiguredObject(PIXELS(128), 0, PIXELS(420), 223);
  MapGrid_CopyCellsTo(45, 41, 8, 45, 3, 3);
  WaitFrames(1);
  // Resume the two walking villagers' proximity reactions.
  objectEvent = ObjectEvent_GetById(LOCALID_BOY_ROUND_THE_WELL);
#ifdef UBFIX
  objectEvent->update = Vault_UpdateWalkingVillager;
#else
  objectEvent->update =
      (void (*)(union ObjectEventOrEffect *))Vault_UpdateWalkingVillager;
#endif
  {
    struct ObjectEvent *objectEvent =
        ObjectEvent_GetById(LOCALID_BOY_ROUND_THE_WELL);
    s32 mode = 1;
    objectEvent->timer = mode;
  }
  mode = 0;
  objectEvent = ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL);
#ifdef UBFIX
  objectEvent->update = Vault_UpdateWalkingVillager;
#else
  objectEvent->update =
      (void (*)(union ObjectEventOrEffect *))Vault_UpdateWalkingVillager;
#endif
  ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL)->timer = mode;
  // Restore the dog's clue position and the bell's current Djinni position.
  if (GameFlag_Get(FLAG_VAULT_DOG_SHOWN_PASSAGE))
    ObjectEvent_SetPosition(LOCALID_VAULT_DOG, PIXELS(216), PIXELS(392));
  if (gSaveBlock.warpDest <= VAULT_REVISIT_NORTH_UNUSED &&
      !GameFlag_Get(FLAG_DJINNI(DJINNI_SAP)) &&
      !VaultRevisit_ShouldKeepBellDjinniPosition())
    GameFlag_Clear(FLAG_VAULT_SAP_CALLED_BY_BELL);
  if (GameFlag_Get(FLAG_VAULT_SAP_CALLED_BY_BELL) &&
      !GameFlag_Get(FLAG_DJINNI(DJINNI_SAP)))
    ObjectEvent_SetPosition(LOCALID_SAP, PIXELS(408), PIXELS(120));
  if (gSaveBlock.warpDest == VAULT_REVISIT_FROM_SANCTUM)
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  if (gSaveBlock.warpDest == VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE)
    GameFlag_Clear(FLAG_WARP_ARRIVAL_STAIRS_UP);
  return 0;
}

// The three scripts of gFieldEffect_DurationScripts, as in vault.c.
s32 gVaultRevisit_ShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gVaultRevisit_MediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gVaultRevisit_LongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gVaultRevisit_ShortEffectScript,
    gVaultRevisit_MediumEffectScript,
    gVaultRevisit_LongEffectScript,
};

s32 gVaultRevisit_BoyRoundTheWellScript[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_LABEL(1),
    SC_MOVE(PIXELS(136), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(392)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gVaultRevisit_GirlRoundTheWellScript[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_LABEL(1),
    SC_MOVE(PIXELS(120), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(408)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

struct MapWarpDest gVaultRevisit_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_REVISIT_NORTH_UNUSED, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTHEAST, CONDITION_ALWAYS,
     408, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTH, CONDITION_ALWAYS, 312, 0,
     328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTHWEST, CONDITION_ALWAYS,
     216, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_EAST, CONDITION_ALWAYS, 376, 0,
     248, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_NORTH, CONDITION_ALWAYS, 296, 0,
     200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_NORTHWEST, CONDITION_ALWAYS,
     120, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_SANCTUM, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_STONE_ROOMS, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE, CONDITION_ALWAYS, 217, 0, 409,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH, CONDITION_ALWAYS, 420, 0, 136,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gVaultRevisit_Warps[] = {
    MAP_WARPS(VaultRevisit),
    MAP_WARP(VAULT_REVISIT_TO_WORLD_MAP_SOUTH, WorldMap,
             WORLD_MAP_FROM_VAULT_SOUTH),
    MAP_WARP(VAULT_REVISIT_TO_WORLD_MAP_NORTH, WorldMap,
             WORLD_MAP_FROM_VAULT_NORTH),
    MAP_WARP(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHEAST,
             VaultInteriorsRevisit,
             VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHEAST),
    MAP_WARP(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTH,
             VaultInteriorsRevisit,
             VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTH),
    MAP_WARP(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHWEST,
             VaultInteriorsRevisit,
             VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHWEST),
    MAP_WARP(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_EAST,
             VaultInteriorsRevisit,
             VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_EAST),
    MAP_WARP(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTH,
             VaultInteriorsRevisit,
             VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTH),
    MAP_WARP(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTHWEST,
             VaultInteriorsRevisit,
             VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTHWEST),
    MAP_WARP(VAULT_REVISIT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT_REVISIT),
    MAP_WARP(VAULT_REVISIT_TO_STONE_ROOMS, StoneRooms,
             STONE_ROOMS_FROM_VAULT_REVISIT),
    MAP_WARP(VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE, VaultCavePassage,
             VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT),
    MAP_WARP(VAULT_REVISIT_TO_VAULT_CAVE_MOUTH, VaultCaveMouth,
             VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gVaultRevisit_ObjectEventTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVaultRevisit_BoyRoundTheWellScript,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVaultRevisit_GirlRoundTheWellScript,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { 0x5a, 0x34, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

u16 gVaultRevisit_GemOwnersHouseDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 4), MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

u16 gVaultRevisit_ChildsHouseDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 4), MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

u16 gVaultRevisit_InnDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 4), MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

u16 gVaultRevisit_ItemShopDoorCells[] = {
    MAPGRID_ANIMATION_STEP(42, 48, 2, 2, 4), MAPGRID_ANIMATION_STEP(40, 48, 2, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

u16 gVaultRevisit_WeaponAndArmorShopDoorCells[] = {
    MAPGRID_ANIMATION_STEP(42, 46, 2, 2, 4), MAPGRID_ANIMATION_STEP(40, 46, 2, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

u16 gVaultRevisit_MayorsHouseDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 4), MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

u16 gVaultRevisit_JailDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 33, 2, 2, 4), MAPGRID_ANIMATION_STEP(33, 33, 2, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

struct MapEvent gVaultRevisit_Events[] = {
    { MAP_EVENT_WARP, 20, CONDITION_ALWAYS, VAULT_REVISIT_TO_WORLD_MAP_SOUTH },
    { MAP_EVENT_WARP, 21, CONDITION_ALWAYS, VAULT_REVISIT_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_WARP, MB_VAULT_HIDDEN_PASSAGE, CONDITION_ALWAYS, VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE },
    { MAP_EVENT_WARP, MB_VAULT_CLIFF_CAVE_MOUTH, CONDITION_ALWAYS, VAULT_REVISIT_TO_VAULT_CAVE_MOUTH },
    { MAP_EVENT_COORD, MB_VAULT_BRIDGE_END, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RaisePathToBridge) },
    { MAP_EVENT_COORD, MB_VAULT_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_LowerPathUnderBridge) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterGemOwnersHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterChildsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_INN_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterInn) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterItemShop) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterWeaponAndArmorShop) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterMayorsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_DOOR_TO_JAIL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterJail) },
    { MAP_EVENT_TALK, LOCALID_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RecognizeThiefCatchers) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_THE_FIXED_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgVaultYouveProbablyAlreadyHeardButThe) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_WONDERING_ABOUT_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatHappenedToHammet) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_LUNPAS_THIEVES, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkOfLunpaThieves) },
    { MAP_EVENT_TALK, LOCALID_MAYORS_VISITOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PraisePartyStrength) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_THE_VOLCANO, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopeVolcanoStaysCalm) },
    { MAP_EVENT_TALK, LOCALID_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_GreetWarriorGuy) },
    { MAP_EVENT_TALK, LOCALID_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WishEldersWereBrave) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutJailbreak) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_DIVINE_PLAN, CONDITION_ALWAYS, MSGNO(MsgVaultTheThievesEscapedButIThink) },
    { MAP_EVENT_TALK, LOCALID_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToDog) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultThereYouSeeTheRoofRepairs) },
    { MAP_EVENT_TALK, LOCALID_SAP, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SapJoins) },
    { BG_EVENT(BG_EVENT_SILENT), MB_VAULT_BELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RingBell) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_WELCOMING_YOUNG_MAN, FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY, EVENT_SCRIPT(EventScript_ReadMindAndRecognizeThiefCatchers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultICantBelieveTheyreHereWhat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_THE_FIXED_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgVaultWhenThoseGuysFromKalayCame) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_WONDERING_ABOUT_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultYeahSomeWarriorsFromKalayCame) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_LUNPAS_THIEVES, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnDodonpa) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_MAYORS_VISITOR, FLAG_VAULT_REVISIT_PARTY_PRAISED, EVENT_SCRIPT(EventScript_ReadMindAndPraisePartyStrength) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAYORS_VISITOR, CONDITION_ALWAYS, MSGNO(MsgVaultThoseStupidThievesEscapedAndWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_THE_VOLCANO, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnWinter) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_BOY_ROUND_THE_WELL, FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE, EVENT_SCRIPT(EventScript_ReadMindOnFeelingBrave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnFeelingBrave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnScaredGrownUps) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_STRONGMAN_ON_WATCH, FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL, EVENT_SCRIPT(EventScript_ReadMindOnJailKey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnJailKey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_DIVINE_PLAN, CONDITION_ALWAYS, MSGNO(MsgVaultIKeepWonderingWasOurStatue) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VAULT_DOG, IF_FLAG_SET(FLAG_VAULT_DOG_SHOWN_PASSAGE), MSGNO(MsgVaultCarefulSearchWillRevealPassage) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VAULT_DOG, CONDITION_ALWAYS, MSGNO(MsgVaultIfYouBringMeSomeBones) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultThatsTheLastOfTheRoof) },
    { BG_EVENT(BG_EVENT_SILENT), MB_VAULT_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PeerIntoWell) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), MB_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { USE_ITEM_ON_OBJ_EVENT(ITEM_BONE), LOCALID_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT_SCRIPT(EventScript_GiveBoneToDog) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealPassage) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HidePassage) },
    { MAP_EVENTS_END },
};
// clang-format on
