/// @file
/// Vault (SceneId_VaultRevisit), the town once the party has reached Kalay, on
/// the same map as Vault's first scene (vault.c). The world map still leads
/// into the first scene, but once FLAG_KALAY_REACHED is set its setup sends
/// the party straight on here (VAULT_REVISIT_FROM_VAULT); Vault Sanctum, Vault
/// Cave, the jail and the houses (vault_interiors_revisit.c) lead back here
/// directly. This scene replaces the first for the rest of the game.
///
/// The thieves the party caught have broken out of jail, and most townspeople
/// say so, or speak of Hammet and Lunpa, in plain message rows. Four speak
/// through event functions that remember who has spoken in place flags
/// (FLAG_WARRIOR_GUY_GREETED to FLAG_ASKED_ABOUT_JAIL), so that a Mind Read on
/// one not yet spoken to plays the conversation first. The old man's praise
/// also sets FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES, as the mayor's welcome
/// indoors does.
///
/// Searching the bell rings it, and a ring while
/// FLAG_VAULT_DJINNI_CALLED_BY_BELL is clear calls Sap, a Venus Djinni, out to
/// stand by the bell; spoken to, Sap joins the party. Coming in by a gate
/// clears the call, so Sap hides again until it has joined. The dog, given a
/// Bone, runs to a spot and jumps there three times
/// (FLAG_VAULT_DOG_SHOWN_PASSAGE, shared with vault.c); Reveal then shows the
/// hidden passage at that spot, whose cell (VAULT_CELL_CAVE_PASSAGE) leads
/// down into Vault Cave, and the cave mouth in the northeast cliff
/// (VAULT_CELL_CAVE_MOUTH) is a second way in. A wooden box holds a Nut, and
/// the well rumbles with water. The cells the events answer are enum
/// VaultCell (vault.h); the jail door brings the party in by
/// VAULT_JAIL_FROM_VAULT_REVISIT, where a lone prisoner sits in the thieves'
/// place.
///
/// The inn's roof is mended: the 3-by-3 block of cells Scene_Initialize
/// copies from (45, 41) to (8, 45) lies under map layer 2's window, on the
/// inn's roof, and draws the thatch whole where vault.c's town shows the hole
/// the fiery rock burnt in it (render probe vault-revisit, the town before
/// and after).
///
/// Open questions:
/// - XXX (no evidence): why Scene_Initialize, as vault.c's does, creates a
///   still object of sprite 223 (spr_223.png, a wooden crate) at (128, 420),
///   on the well, that shows nothing: its 32-by-64 sprite entry is in the
///   sprite list but its tiles in video memory are blank, it is neither solid
///   nor a target (ACTOR_NOT_TARGETED), and nothing refers to it (render probe
///   vault-revisit); nothing in the ROM or a render says what it was meant to
///   show.

#include "game_state.h"
#include "djinn_ids.h"
#include "window.h"
#include "unit/scene/vault_revisit.h"
#include "vault.h"
#include "unit/scene/common/vault_range.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
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

/// The townspeople, the dog and Sap, numbering gVaultRevisit_Placements' rows
/// from ACTOR_FIRST_PLACED.
enum VaultRevisitActor {
  /// vault.c's welcoming young man, who recognizes the party.
  /// ★ Named from the game: "You've returned to Vault!"
  /// (MsgVaultWarriorsWhoCaptured)
  ACTOR_WELCOMING_YOUNG_MAN = ACTOR_FIRST_PLACED,
  /// A woman with fair hair who brings the news from Kalay.
  /// ★ Named from the game: "You've probably already heard, but the bridge to
  /// Kalay has been fixed." (MsgVaultYouveProbablyAlreadyHeardButThe)
  ACTOR_WOMAN_ON_THE_FIXED_BRIDGE,
  /// A burly grey-haired man who worries about Hammet.
  /// ★ Named from the game: "I wonder what happened to Hammet when he went to
  /// Lunpa..." (MsgVaultIWonderWhatHappenedToHammet)
  ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET,
  /// A woman.
  /// ★ Named from the game: "There have been rumors of thieves from Lunpa
  /// running wild" (MsgVaultRumorsThievesLunpa)
  ACTOR_WOMAN_ON_LUNPAS_THIEVES,
  /// An old man with a cane who has talked with the mayor.
  /// ★ Named from the game: "I was just talking about you with our mayor"
  /// (MsgVaultTalkingMayorStrong)
  ACTOR_MAYORS_VISITOR,
  /// An old woman.
  /// ★ Named from the game: "If the volcano stays calm and the southern bridge
  /// stays fixed, we won't have any worries at all." (MsgVaultVolcanoStaysCalm)
  ACTOR_OLD_WOMAN_ON_THE_VOLCANO,
  /// vault.c's two children, walking round the well again; the boy greets the
  /// party. "Hey, it's that warrior guy!"
  /// ★ Named from what the player sees: a blond boy walking round the well
  /// (its render)
  ACTOR_BOY_ROUND_THE_WELL,
  /// Speaks of the grown-ups. "Grown-ups always get so scared"
  /// ★ Named from what the player sees: a girl with orange hair walking round
  /// the well (its render)
  ACTOR_GIRL_ROUND_THE_WELL,
  /// vault.c's strongman on watch. "Did you guys check out the jail?"
  /// ★ Named from the game: "Nobody's stealing anything on my watch!"
  /// (MsgVaultNobodysStealingAnything)
  ACTOR_STRONGMAN_ON_WATCH,
  /// A bearded man in a long robe who takes the thieves' escape calmly.
  /// ★ Named from the game: "The thieves escaped, but I think it's all right.
  /// It's all part of a divine plan." (MsgVaultTheThievesEscapedButIThink)
  ACTOR_VILLAGER_DIVINE_PLAN,
  /// ★ Named from what the player sees: a white dog sitting among the graves
  /// (its render)
  ACTOR_VAULT_DOG,
  /// vault.c's young man fixing the roof, standing where he did there, his work
  /// done.
  /// ★ Named from the game: "There, you see? The roof repairs are all
  /// finished." (MsgVaultThereYouSeeTheRoofRepairs)
  ACTOR_YOUNG_MAN_FIXING_THE_ROOF,
  /// Sap, a Venus Djinni, hiding until the bell calls it. The row before it,
  /// sprite 0xf8 placed at 0, 0, is never referred to.
  /// ★ Named from the game: "Sap" (MsgDjinnName)
  ACTOR_SAP = ACTOR_FIRST_PLACED + 13
};

enum { VAULT_PROXIMITY_REACTION_PAUSED = 2 };

/// Place flags, kept while the party stays in Vault's scenes: what the
/// townspeople have already said. A Mind Read before someone has spoken plays
/// the conversation first.
enum {
  /// ★ Named from the game: "Hey, it's that warrior guy!"
  /// (MsgVaultWarriorGuy)
  FLAG_WARRIOR_GUY_GREETED = FLAG_PLACE_TEMP_00,
  /// ★ Named from the game: "You're the warriors who captured the thieves"
  /// (MsgVaultWarriorsWhoCaptured)
  FLAG_WELCOMING_YOUNG_MAN_RECOGNIZED_PARTY = FLAG_PLACE_TEMP_05,
  /// ★ Named from the game: "You're all really strong"
  /// (MsgVaultTalkingMayorStrong)
  FLAG_PARTY_PRAISED = FLAG_PLACE_TEMP_06,
  /// ★ Named from the game: "I'm so glad you came back to Vault!"
  /// (MsgVaultWarriorGuy)
  FLAG_BOY_ROUND_THE_WELL_SPOKE = FLAG_PLACE_TEMP_07,
  /// ★ Named from the game: "did you guys check out the jail?"
  /// (MsgVaultGuysCheckJail)
  FLAG_ASKED_ABOUT_JAIL = FLAG_PLACE_TEMP_08
};

// Shared effect and proximity routines precede the scene's entry points.
#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_distance.h"
#include "common/vault_range.h"
#include "common/vault_nearby.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gVaultRevisit_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gVaultRevisit_Exits; }

/// Returns the villagers, dog, and Djinni placements.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gVaultRevisit_Placements;
}

/// Stands the actor still while showing the message already selected.
static void VaultRevisit_ShowStandingActorMessage(s32 actor) {
  Engine_EventStart();
  Engine_ActorSetAnimation(actor, ANIMATION_STAND);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

/// The party is recognized. "You're the warriors who captured the thieves a
/// while back, aren't you? You've returned to Vault!"
static void Event_RecognizeThiefCatchers(void) {
  Engine_EventStart();
  Engine_ActorFaceActor(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 2);
  Engine_GameFlagSet(FLAG_WELCOMING_YOUNG_MAN_RECOGNIZED_PARTY);
  Engine_EventSetMessage(MsgVaultWarriorsWhoCaptured);
  Engine_EventShowMessage(ACTOR_WELCOMING_YOUNG_MAN, 0);
  Engine_EventEnd();
}

/// There are rumors of thieves from Lunpa running wild, yet they have been
/// quiet.
static void Event_TalkOfLunpaThieves(void) {

  Engine_EventSetMessage(MsgVaultRumorsThievesLunpa);
  Engine_ActorFaceEachOther(ACTOR_WOMAN_ON_LUNPAS_THIEVES, ACTOR_PARTY_LEADER,
                            2);
  VaultRevisit_ShowStandingActorMessage(ACTOR_WOMAN_ON_LUNPAS_THIEVES);
}

/// The old man was just talking about the party with the mayor: they are so
/// strong, let those thieves attack. Sets FLAG_PARTY_PRAISED and
/// FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES (0x868).
static void Event_PraisePartyStrength(void) {

  Engine_EventStart();
  Engine_ActorFaceActor(ACTOR_MAYORS_VISITOR, ACTOR_PARTY_LEADER, 2);
  Engine_GameFlagSet(FLAG_PARTY_PRAISED);
  Engine_GameFlagSet(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  Engine_EventSetMessage(MsgVaultTalkingMayorStrong);
  Engine_EventShowMessage(ACTOR_MAYORS_VISITOR, 0);
  Engine_EventEnd();
}

/// If the volcano stays calm and the southern bridge stays fixed, there is
/// nothing to worry about.
static void Event_HopeVolcanoStaysCalm(void) {

  Engine_EventSetMessage(MsgVaultVolcanoStaysCalm);
  Engine_ActorFaceEachOther(ACTOR_OLD_WOMAN_ON_THE_VOLCANO, ACTOR_PARTY_LEADER,
                            2);
  VaultRevisit_ShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_VOLCANO);
}

/// The boy round the well stops circling. "Hey, it's that warrior guy!"
/// and "Yeah, and Ivan, too." the first time (FLAG_WARRIOR_GUY_GREETED), then
/// "I'm so glad you came back to Vault!" Bit 1 of the boy's timer keeps
/// Vault_UpdateWalkingVillager from moving it while it talks.
static void Event_GreetWarriorGuy(void) {
  struct FieldActor *actor = Actor_Get(ACTOR_BOY_ROUND_THE_WELL);
  u16 facing = actor->facing;
  s32 text;

  actor->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  Engine_EventStart();
  text = MsgVaultWarriorGuy;
  Engine_EventSetMessage(text);
  Engine_ActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, 0);
  Engine_ActorFaceEachOther(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  if (GameFlag_IsSet(FLAG_WARRIOR_GUY_GREETED) == 0) {
    Actor_ShowEmote(ACTOR_BOY_ROUND_THE_WELL,
                    EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Event_ShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
    Event_ShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
    GameFlag_Set(FLAG_WARRIOR_GUY_GREETED);
  }
  Engine_EventSetMessage(text + 2);
  Event_ShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
  actor->facing = facing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer = 1;
  GameFlag_Set(FLAG_BOY_ROUND_THE_WELL_SPOKE);
}

/// The girl round the well stops. "I wish Vault's elders were as brave as you!"
static void Event_WishEldersWereBrave(void) {
  struct FieldActor *actor;
  s16 originalFacing;

  actor = Object_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  originalFacing = actor->facing;
  actor->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  Engine_EventStart();
  Engine_EventSetMessage(MsgVaultWishVaultsElders);
  Engine_ActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, 0);
  Engine_ActorFaceEachOther(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Engine_EventShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 0, 10);
  actor->facing = originalFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer = 0;
}

/// The strongman on watch asks about the jail. "Did you guys check out the
/// jail?" Yes: how did the thugs break out? No: the thieves broke out and got
/// away.
static void Event_AskAboutJailbreak(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgVaultGuysCheckJail);
  Engine_ActorFaceEachOther(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
  Engine_EventOpenMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) != 0) {
    gEventWork->message += 1;
  }
  Engine_EventShowMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
  Engine_GameFlagSet(FLAG_ASKED_ABOUT_JAIL);
  Engine_EventEnd();
}

/// Mind Read on the welcoming young man before he has spoken: he turns and
/// recognizes the party.
static void Event_ReadMindAndRecognizeThiefCatchers(void) {
  Engine_EventStart();
  Engine_ActorRunRepeatedMotion(ACTOR_WELCOMING_YOUNG_MAN, 1);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 20);
  Engine_GameFlagSet(FLAG_WELCOMING_YOUNG_MAN_RECOGNIZED_PARTY);
  Engine_EventSetMessage(MsgVaultWarriorsWhoCaptured);
  Engine_EventShowMessageAndWait(ACTOR_WELCOMING_YOUNG_MAN, 0, 20);
  Engine_EventEnd();
}

/// Mind Read on the woman who talks of Lunpa's thieves: since Dodonpa took
/// over, Lunpa's outlaws are no better than thieves, and there must be a reason
/// they are so quiet.
static void Event_ReadMindOnDodonpa(void) {
  Engine_EventSetMessage(MsgVaultOnceDodonpaTook);
  Object_GetById(ACTOR_WOMAN_ON_LUNPAS_THIEVES)->paused = 1;
  VaultRevisit_ShowStandingActorMessage(ACTOR_WOMAN_ON_LUNPAS_THIEVES);
  Object_GetById(ACTOR_WOMAN_ON_LUNPAS_THIEVES)->paused = 0;
}

/// Mind Read before the old man has spoken: he turns and praises the party.
static void Event_ReadMindAndPraisePartyStrength(void) {
  Engine_EventStart();
  Engine_ActorRunRepeatedMotion(ACTOR_MAYORS_VISITOR, 1);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_MAYORS_VISITOR, ACTOR_PARTY_LEADER, 20);
  Engine_GameFlagSet(FLAG_PARTY_PRAISED);
  Engine_GameFlagSet(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  Engine_EventSetMessage(MsgVaultTalkingMayorStrong);
  Engine_EventShowMessageAndWait(ACTOR_MAYORS_VISITOR, 0, 20);
  Engine_EventEnd();
}

/// Mind Read: it would have been great to have everything solved before
/// winter.
static void Event_ReadMindOnWinter(void) {
  Engine_EventSetMessage(MsgVaultGreatEverythingSolved);
  Object_GetById(ACTOR_OLD_WOMAN_ON_THE_VOLCANO)->paused = 1;
  VaultRevisit_ShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_VOLCANO);
  Object_GetById(ACTOR_OLD_WOMAN_ON_THE_VOLCANO)->paused = 0;
}

/// Mind Read on the boy round the well: watching the party makes one feel
/// brave, or, before the boy has spoken, the greeting.
static void Event_ReadMindOnFeelingBrave(void) {
  Actor_Get(ACTOR_BOY_ROUND_THE_WELL)->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  Engine_EventStart();
  if (GameFlag_IsSet(FLAG_BOY_ROUND_THE_WELL_SPOKE) != 0) {
    Engine_EventSetMessage(MsgVaultWatchingGuysMakes);
    VaultRevisit_ShowStandingActorMessage(ACTOR_BOY_ROUND_THE_WELL);
  } else {
    Event_GreetWarriorGuy();
    GameFlag_Set(FLAG_BOY_ROUND_THE_WELL_SPOKE);
  }
  Engine_EventEnd();
  Actor_Get(ACTOR_BOY_ROUND_THE_WELL)->timer = 1;
}

/// Mind Read on the girl round the well: grown-ups always get so scared.
static void Event_ReadMindOnScaredGrownUps(void) {
  {
    struct FieldActor *actor = Object_GetById(ACTOR_GIRL_ROUND_THE_WELL);
    actor->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  }
  Engine_EventStart();
  Engine_EventSetMessage(MsgVaultGrownUpsAlways);
  VaultRevisit_ShowStandingActorMessage(ACTOR_GIRL_ROUND_THE_WELL);
  Engine_EventEnd();
  {
    s32 clear = 0;
    struct FieldActor *actor = Object_GetById(ACTOR_GIRL_ROUND_THE_WELL);
    actor->timer = clear;
  }
}

/// Mind Read on the strongman on watch: before he has asked about the jail he
/// asks now; afterwards, the thieves must know the secret of the jail's lock.
static void Event_ReadMindOnJailKey(void) {
  s32 requestHeard;
  s32 unpaused;

  requestHeard = GameFlag_IsSet(FLAG_ASKED_ABOUT_JAIL);
  if (requestHeard == 0) {
    Engine_EventStart();
    Actor_Get(ACTOR_STRONGMAN_ON_WATCH)->paused = 1;
    Engine_ActorSetAnimation(ACTOR_STRONGMAN_ON_WATCH, ANIMATION_STAND);
    Engine_ActorRunRepeatedMotion(ACTOR_STRONGMAN_ON_WATCH, 1);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgVaultGuysCheckJail);
    Engine_ActorFaceEachOther(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
    Event_OpenMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) != 0) {
      gEventWork->message++;
    }
    Event_ShowMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
    Actor_Get(ACTOR_STRONGMAN_ON_WATCH)->paused = requestHeard;
    Actor_SetBuiltinAction(ACTOR_STRONGMAN_ON_WATCH, 2);
    Engine_EventEnd();
    GameFlag_Set(FLAG_ASKED_ABOUT_JAIL);
  } else {
    Engine_EventSetMessage(MsgVaultNotLikeEasy);
    Actor_Get(ACTOR_STRONGMAN_ON_WATCH)->paused = 1;
    VaultRevisit_ShowStandingActorMessage(ACTOR_STRONGMAN_ON_WATCH);
    unpaused = 0;
    Actor_Get(ACTOR_STRONGMAN_ON_WATCH)->paused = unpaused;
  }
}

/// Gives the actor vertical velocity, moves it, and waits briefly for it to
/// settle.
static void VaultRevisit_JumpActorToAndWait(int actor, int x, int z,
                                            int velocityY) {
  struct FieldActor *jumpingActor = Object_GetById(actor);
  int frames;
  Engine_ActorSetSpeed(actor, PIXELS(3), PIXELS(3) / 2);
  jumpingActor->gravity = PIXELS(1) / 2;
  jumpingActor->bounce = 0;
  jumpingActor->velocityY = velocityY;
  Engine_ActorSetSpriteFlags(jumpingActor, 0);
  Engine_ActorMoveToAndWait(actor, x, z);
  Engine_ActorSetPosition(actor, x << 16, z << 16);
  for (frames = 60; frames != 0; --frames) {
    Engine_TaskWait(1);
    if ((s16)((u32)jumpingActor->velocityY >> 16) == 0)
      break;
  }
  Engine_ActorSetSpriteFlags(jumpingActor, 1);
  jumpingActor->gravity = PIXELS(1);
}

/// Searching the bell rings it (SE_100). The first ring since the party came in
/// from the world map makes Sap jump out of hiding and walk to the bell, at
/// (408, 120), and sets FLAG_VAULT_DJINNI_CALLED_BY_BELL.
static void Event_RingBell(void) {
  Engine_EventStart();
  Event_PlayCue(SE_100);
  Engine_EventWait(40);
  if (GameFlag_IsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) == 0) {
    Actor_SetAttachedEffect(ACTOR_SAP,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorJump(ACTOR_SAP, 4, 0);
    Engine_EventWait(12);
    Engine_ActorJump(ACTOR_SAP, 4, 0);
    Engine_EventWait(20);
    // Let the Djinni leap from hiding before it walks to the bell.
    VaultRevisit_JumpActorToAndWait(ACTOR_SAP, 392, 104, PIXELS(7));
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_SAP, 408, 104);
    Actor_WalkToAndWait(ACTOR_SAP, 408, 120);
    GameFlag_Set(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
  }
  Engine_EventEnd();
}

/// Walks the leader into a doorway during the sixteen-frame exit delay.
static void Vault_WalkThroughExit(s32 x, s32 z, s32 exitId) {
  s32 leaderId = ACTOR_PARTY_LEADER;

  Actor_SetSpeed(leaderId, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(leaderId, x, z);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(exitId);
}

/// The door of the gem owner's house opens and the party walks in.
/// It's VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHEAST, into the house of
/// the man whose precious stone a cat ate.
static void Event_EnterGemOwnersHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_GemOwnersHouseDoorCells, 56, 19);
  Vault_WalkThroughExit(408, 320,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHEAST);
}

/// The door of a child's home opens and the party walks in.
/// It's VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTH, into the house of the
/// child who doubts the thieves were from Lunpa.
static void Event_EnterChildsHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_ChildsHouseDoorCells, 50, 18);
  Vault_WalkThroughExit(312, 304,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTH);
}

/// The door of VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHWEST opens into
/// the inn.
static void Event_EnterInn(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_InnDoorCells, 44, 17);
  Vault_WalkThroughExit(216, 288,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHWEST);
}

/// The item shop's door opens and the party walks in.
/// It's VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_EAST, into the item shop,
/// where the couple who talk of Colosso also stand. The leader is
/// drawn at sprite priority 3 as he walks in, as at the weapon and armor shop
/// and the jail, and the collision of the doorway, a column of three cells at
/// (23, 12), is replaced so he can pass.
static void Event_EnterItemShop(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_ItemShopDoorCells, 54, 13);
  Map_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_EAST);
}

/// The weapon and armor shop's door opens and the party walks in.
/// It's VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTH, drawn like the item
/// shop's.
static void Event_EnterWeaponAndArmorShop(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_WeaponAndArmorShopDoorCells, 49, 10);
  Map_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTH);
}

/// The door of VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTHWEST opens into
/// the mayor's house.
static void Event_EnterMayorsHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_MayorsHouseDoorCells, 38, 6);
  Vault_WalkThroughExit(120, 144,
                        VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTHWEST);
}

// The remaining doorway helper retains its position in the overlay.
#include "common/vault_scene_cells.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

/// The jail door opens and the party walks in.
/// It's drawn behind the doorway as it goes in, to
/// VAULT_JAIL_FROM_VAULT_REVISIT, where the lone prisoner sits.
static void Event_EnterJail(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_JailDoorCells, 35, 9);
  Map_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, VAULT_REVISIT_TO_VAULT_JAIL);
}

// Shared entrance toggles precede the Djinni and dog events.
#include "common/vault_toggle.h"

/// Talking to Sap, a Venus Djinni: it joins the party.
static void Event_TalkToVenusDjinni(void) {
  Djinn_JoinParty(ACTOR_SAP, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP));
}

/// Jumps at the hidden passage, then pauses so the party can see the clue.
static inline void VaultRevisit_DogJumpAtPassage(void) {
  Engine_ActorJump(ACTOR_VAULT_DOG, 6, 0);
  Engine_EventWait(30);
}

/// Using a Bone on the dog: it takes the Bone, runs to the hidden passage at
/// (216, 408) and jumps there three times, then steps back and sets
/// FLAG_VAULT_DOG_SHOWN_PASSAGE, which places it there, a step back, on later
/// arrivals.
static void Event_GiveBoneToDog(void) {
  PartyInventory_Discard(ITEM_BONE);
  Engine_EventStart();
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_VAULT_DOG, 2);
  Actor_SetSpeed(ACTOR_VAULT_DOG, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_VAULT_DOG, 216, 408);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
  // Repeat the clue at the concealed passage, then step back.
  VaultRevisit_DogJumpAtPassage();
  VaultRevisit_DogJumpAtPassage();
  VaultRevisit_DogJumpAtPassage();
  Actor_WalkToAndWait(ACTOR_VAULT_DOG, 216, 392);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
  GameFlag_Set(FLAG_VAULT_DOG_SHOWN_PASSAGE);
  Engine_EventEnd();
}

/// Covers the hidden passage again, tiles and collision, when Reveal ends.
static void Event_HidePassage(void) {

  Map_CopyCellAttributes(41, 43, 1, 1, 13, 25);
  Map_CopyCellsTo(40, 42, 12, 22, 3, 3);
}

/// Reveal shows the hidden passage where the dog jumps: its tiles, and the
/// collision of cell (13, 25), which lets the party step onto trigger 40 and
/// down into Vault Cave.
static void Event_RevealPassage(void) {

  Map_CopyCellAttributes(37, 43, 1, 1, 13, 25);
  Map_CopyCellsTo(36, 42, 12, 22, 3, 3);
}

/// The dog barks; carrying a Bone before it has shown the passage lets the
/// party give it one: the main menu opens as the talk ends.
static void Event_TalkToDog(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgVaultRuffRrruff2);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
  Engine_ActorFaceEachOther(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(2);
  Engine_EventShowMessage(ACTOR_VAULT_DOG, 0);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, ANIMATION_STAND);

  if (PartyInventory_FindOwner(ITEM_BONE) != -1 &&
      Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0) {
    gEventWork->requestedMenu = 1;
  }

  Engine_EventEnd();
}

/// Describes the water heard at the bottom of the well.
static void Event_PeerIntoWell(void) {
  Engine_EventStart();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgVaultCanHearWaterRumblingDown, 1);
  Engine_EventEnd();
}

/// Returns the town's talk, search, Psynergy, and doorway events.
const struct SceneEvent *Scene_GetEvents(void) { return gVaultRevisit_Events; }

/// Patched editions keep Sap by the bell when the party keeps its position,
/// as on a return from battle.
static inline s32 VaultRevisit_ShouldKeepBellDjinniPosition(void) {
#if TBS_PATCH_1
  return Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION);
#else
  return 0;
#endif
}

/// Sets up Vault as the party finds it after reaching Kalay.
/// On arrival it restores the passage's collision (common/vault_toggle.h),
/// creates the still object of sprite 223 on the well that shows nothing (an
/// open question), mends the inn's roof, which vault.c leaves holed, and
/// hooks the two children round
/// the well to Vault_UpdateWalkingVillager. The dog waits at the hidden
/// passage once it has shown it. Coming in by a gate (ENTRANCE_DEFAULT to
/// VAULT_REVISIT_NORTH_UNUSED, forwarded from vault.c) before Sap has joined
/// clears FLAG_VAULT_DJINNI_CALLED_BY_BELL, so Sap is hidden again;
/// while it is called and has not joined, it stands by the bell. Coming out of
/// Vault Sanctum (VAULT_REVISIT_FROM_SANCTUM) clears FLAG_SHOW_LOCATION_NAME,
/// and coming up from Vault Cave (VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE) clears
/// FLAG_EXIT_ARRIVAL_PENDING, so the leader's arrival animation does
/// not play.
s32 Scene_Initialize(void) {

  struct FieldActor *actor;
  s32 mode;

  // Keep the bridge deck after a battle, and mend the inn's roof.
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00))
    Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  // A still object on the well that shows nothing (an open question).
  OverlayObject_CreateConfiguredObject(PIXELS(128), 0, PIXELS(420), 223);
  Map_CopyCellsTo(45, 41, 8, 45, 3, 3);
  Engine_TaskWait(1);
  // Resume the two walking villagers' proximity reactions.
  actor = Object_GetById(ACTOR_BOY_ROUND_THE_WELL);
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  {
    struct FieldActor *actor = Object_GetById(ACTOR_BOY_ROUND_THE_WELL);
    s32 mode = 1;
    actor->timer = mode;
  }
  mode = 0;
  actor = Object_GetById(ACTOR_GIRL_ROUND_THE_WELL);
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  Object_GetById(ACTOR_GIRL_ROUND_THE_WELL)->timer = mode;
  // Restore the dog's clue position and the bell's current Djinni position.
  if (Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE))
    Actor_SetPosition(ACTOR_VAULT_DOG, PIXELS(216), PIXELS(392));
  if (gGameState.entrance <= VAULT_REVISIT_NORTH_UNUSED &&
      !Engine_GameFlagIsSet(FLAG_DJINNI_0 + DJINNI_SAP) &&
      !VaultRevisit_ShouldKeepBellDjinniPosition())
    Engine_GameFlagClear(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
  if (Engine_GameFlagIsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) &&
      !Engine_GameFlagIsSet(FLAG_DJINNI_0 + DJINNI_SAP))
    Actor_SetPosition(ACTOR_SAP, PIXELS(408), PIXELS(120));
  if (gGameState.entrance == VAULT_REVISIT_FROM_SANCTUM)
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  if (gGameState.entrance == VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE)
    Engine_GameFlagClear(FLAG_EXIT_ARRIVAL_PENDING);
  return 0;
}

// The three scripts of gEffectScripts, as in vault.c.
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

const s32 *gEffectScripts[] = {
    gVaultRevisit_ShortEffectScript,
    gVaultRevisit_MediumEffectScript,
    gVaultRevisit_LongEffectScript,
};

s32 gVaultRevisit_BoyRoundTheWellScript[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 5),
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
    SC_ARG(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 5),
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

struct SceneEntrance gVaultRevisit_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
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
    {VAULT_REVISIT_FROM_VAULT_JAIL, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE, CONDITION_ALWAYS, 217, 0, 409,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH, CONDITION_ALWAYS, 420, 0, 136,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gVaultRevisit_Exits[] = {
    SCENE_EXITS(VaultRevisit),
    SCENE_EXIT(VAULT_REVISIT_TO_WORLD_MAP_SOUTH, WorldMap,
               WORLD_MAP_FROM_VAULT_SOUTH),
    SCENE_EXIT(VAULT_REVISIT_TO_WORLD_MAP_NORTH, WorldMap,
               WORLD_MAP_FROM_VAULT_NORTH),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHEAST,
               VaultInteriorsRevisit,
               VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHEAST),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTH,
               VaultInteriorsRevisit,
               VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTH),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHWEST,
               VaultInteriorsRevisit,
               VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHWEST),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_EAST,
               VaultInteriorsRevisit,
               VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_EAST),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTH,
               VaultInteriorsRevisit,
               VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTH),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTHWEST,
               VaultInteriorsRevisit,
               VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTHWEST),
    SCENE_EXIT(VAULT_REVISIT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_JAIL, VaultJail,
               VAULT_JAIL_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE, VaultCavePassage,
               VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_CAVE_MOUTH, VaultCaveMouth,
               VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gVaultRevisit_Placements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVaultRevisit_BoyRoundTheWellScript,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVaultRevisit_GirlRoundTheWellScript,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x5a, 0x34, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

u16 gVaultRevisit_GemOwnersHouseDoorCells[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 4), MAP_CELL_STEP(33, 52, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_ChildsHouseDoorCells[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 4), MAP_CELL_STEP(33, 50, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_InnDoorCells[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 4), MAP_CELL_STEP(33, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_ItemShopDoorCells[] = {
    MAP_CELL_STEP(42, 48, 2, 2, 4), MAP_CELL_STEP(40, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_WeaponAndArmorShopDoorCells[] = {
    MAP_CELL_STEP(42, 46, 2, 2, 4), MAP_CELL_STEP(40, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_MayorsHouseDoorCells[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 4), MAP_CELL_STEP(33, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_JailDoorCells[] = {
    MAP_CELL_STEP(35, 33, 2, 2, 4), MAP_CELL_STEP(33, 33, 2, 2, 4), MAP_CELL_STEPS_END,
};

struct SceneEvent gVaultRevisit_Events[] = {
    { EVENT_EXIT, 20, CONDITION_ALWAYS, VAULT_REVISIT_TO_WORLD_MAP_SOUTH },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, VAULT_REVISIT_TO_WORLD_MAP_NORTH },
    { EVENT_EXIT, VAULT_CELL_CAVE_PASSAGE, CONDITION_ALWAYS, VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE },
    { EVENT_EXIT, VAULT_CELL_CAVE_MOUTH, CONDITION_ALWAYS, VAULT_REVISIT_TO_VAULT_CAVE_MOUTH },
    { EVENT_TOUCH, VAULT_CELL_BRIDGE_END, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RaisePathToBridge) },
    { EVENT_TOUCH, VAULT_CELL_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_LowerPathUnderBridge) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VAULT_CELL_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGemOwnersHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VAULT_CELL_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterChildsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VAULT_CELL_INN_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInn) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VAULT_CELL_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterItemShop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VAULT_CELL_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWeaponAndArmorShop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VAULT_CELL_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterMayorsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VAULT_CELL_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VAULT_CELL_JAIL_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJail) },
    { EVENT_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RecognizeThiefCatchers) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_FIXED_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgVaultYouveProbablyAlreadyHeardButThe) },
    { EVENT_TALK, ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatHappenedToHammet) },
    { EVENT_TALK, ACTOR_WOMAN_ON_LUNPAS_THIEVES, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkOfLunpaThieves) },
    { EVENT_TALK, ACTOR_MAYORS_VISITOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PraisePartyStrength) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ON_THE_VOLCANO, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopeVolcanoStaysCalm) },
    { EVENT_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GreetWarriorGuy) },
    { EVENT_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WishEldersWereBrave) },
    { EVENT_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutJailbreak) },
    { EVENT_TALK, ACTOR_VILLAGER_DIVINE_PLAN, CONDITION_ALWAYS, MSGNO(MsgVaultTheThievesEscapedButIThink) },
    { EVENT_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDog) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultThereYouSeeTheRoofRepairs) },
    { EVENT_TALK, ACTOR_SAP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToVenusDjinni) },
    { SEARCH(SEARCH_UNNAMED), VAULT_CELL_BELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RingBell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_WELCOMING_YOUNG_MAN, FLAG_WELCOMING_YOUNG_MAN_RECOGNIZED_PARTY, EVENT_SCRIPT(Event_ReadMindAndRecognizeThiefCatchers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultICantBelieveTheyreHereWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_FIXED_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgVaultWhenThoseGuysFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultYeahSomeWarriorsFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_LUNPAS_THIEVES, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_MAYORS_VISITOR, FLAG_PARTY_PRAISED, EVENT_SCRIPT(Event_ReadMindAndPraisePartyStrength) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_VISITOR, CONDITION_ALWAYS, MSGNO(MsgVaultThoseStupidThievesEscapedAndWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_VOLCANO, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnWinter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_BOY_ROUND_THE_WELL, FLAG_BOY_ROUND_THE_WELL_SPOKE, EVENT_SCRIPT(Event_ReadMindOnFeelingBrave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnFeelingBrave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnScaredGrownUps) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_STRONGMAN_ON_WATCH, FLAG_ASKED_ABOUT_JAIL, EVENT_SCRIPT(Event_ReadMindOnJailKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnJailKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DIVINE_PLAN, CONDITION_ALWAYS, MSGNO(MsgVaultIKeepWonderingWasOurStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, IF_FLAG_SET(FLAG_VAULT_DOG_SHOWN_PASSAGE), MSGNO(MsgVaultCarefulSearchWillRevealPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, MSGNO(MsgVaultIfYouBringMeSomeBones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultThatsTheLastOfTheRoof) },
    { SEARCH(SEARCH_UNNAMED), VAULT_CELL_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PeerIntoWell) },
    { SEARCH(SEARCH_WOODEN_BOX), VAULT_CELL_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT_SCRIPT(Event_GiveBoneToDog) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealPassage) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HidePassage) },
    { SCENE_EVENTS_END },
};
// clang-format on
