/// @file
/// Vault (VaultRevisit, scene 23), the town as the party finds it once it has
/// reached Kalay, on the same map as Vault's first scene (vault.c). The world
/// map still leads into that first scene, but once FLAG_REACHED_KALAY is set
/// (entering Kalay sets it) its setup sends the party straight on here by its
/// exit 14, to entrance 1 at the town's south edge whichever side it came
/// from; Vault Sanctum, Vault Cave and the houses (vault_interiors_revisit.c)
/// lead back here directly. This copy of the town replaces the first for the
/// rest of the game.
///
/// Much has changed since the party left. The three thieves it caught here,
/// who had stolen Hammet's Shaman's Rod, have broken out of jail; the bridge
/// to Kalay has been fixed, warriors from Kalay have come asking after Hammet,
/// who went to Lunpa, and a villager says Lunpa's outlaws have become no
/// better than thieves since Dodonpa took over. Most townspeople say this in
/// plain message rows. Four speak through event functions that remember who
/// has spoken in place flags (FLAG_SCENE_TEMP_00 + 0x100 to + 0x108, which
/// last until the party leaves Vault's group of scenes), so that a Mind Read
/// on one not yet spoken to plays the conversation first: the greeter
/// recognizes "the warriors who captured the thieves", the villager who was
/// talking with the mayor praises the party's strength, the watchman asks
/// whether it has checked the jail (a yes and a no get different replies), and
/// the first of two villagers who walk in circles together greets the party
/// and Ivan. The praise also sets FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES, as the
/// mayor's welcome indoors does.
///
/// Two secrets wait here. Searching the bell rings it, and a ring while
/// FLAG_VAULT_DJINNI_CALLED_BY_BELL is clear calls Sap, a Venus Djinni, out of
/// hiding to stand by the bell and sets the flag; spoken to, Sap joins the
/// party. Coming in from the world map clears the call, so Sap hides again
/// until it has joined. The dog, given a Bone (the girl at the inn hands them
/// out), runs to a spot and jumps there three times
/// (FLAG_VAULT_DOG_SHOWN_PASSAGE, shared with vault.c), and its thoughts then
/// hint at a passage underground: Reveal shows the passage at that spot, and
/// its trigger 40 leads down into Vault Cave (scene 96). Trigger 41 is a second
/// way into Vault Cave (scene 98). A wooden box holds a Nut, and the well
/// rumbles with water.
///
/// The town's other exits lead to the world map (exits 1 and 2), through six
/// doors into the houses, shops and inn (scene 24, each door's number its
/// entrance there), into Vault Sanctum (exit 11, the Sanctum's entrance 11)
/// and into the jail (exit 12, the jail's entrance 10, where a lone prisoner
/// sits in the thieves' place). Touch triggers 1 and 2 swap the collision of a
/// 4-by-2 block of cells (common/vault_toggle.h), as in vault.c.

#include "game_state.h"
#include "window.h"
#include "unit/scene/vault_revisit.h"
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

enum {
  ACTOR_GREETER = 8, ///< Vault's greeter. "You're the warriors who captured..."
  ACTOR_LUNPA_GOSSIP = 11, ///< rumors of thieves from Lunpa running wild
  /// Has talked with the mayor. "I was just talking about you with our mayor"
  ACTOR_MAYORS_VISITOR = 12,
  ACTOR_VOLCANO_WATCHER = 13, ///< if the volcano stays calm...
  /// The two who walk in circles. "Hey, it's that warrior guy!"
  ACTOR_FIRST_WALKER = 14,
  /// Speaks of the grown-ups. "Grown-ups always get so scared"
  ACTOR_SECOND_WALKER = 15,
  ACTOR_WATCHMAN = 16, ///< Vault's watchman. "Did you guys check out the jail?"
  ACTOR_VAULT_DOG = 18,
  ACTOR_VAULT_VENUS_DJINNI = 21, ///< Sap, hiding until the bell calls it
  VAULT_VENUS_DJINNI_INDEX = 4,  ///< Sap's index among the Venus Djinn
  VAULT_PROXIMITY_REACTION_PAUSED = 2
};

/// Place flags, kept while the party stays in Vault's scenes: what the
/// townspeople have already said. A Mind Read before someone has spoken plays
/// the conversation first.
enum {
  FLAG_WARRIOR_GUY_GREETED = FLAG_SCENE_TEMP_00 + 0x100,
  FLAG_GREETER_RECOGNIZED_PARTY = FLAG_SCENE_TEMP_00 + 0x105,
  FLAG_PARTY_PRAISED = FLAG_SCENE_TEMP_00 + 0x106,
  FLAG_WALKER_SPOKE = FLAG_SCENE_TEMP_00 + 0x107,
  FLAG_ASKED_ABOUT_JAIL = FLAG_SCENE_TEMP_00 + 0x108
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
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, ANIM_STAND);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

/// The party is recognized. "You're the warriors who captured the thieves a
/// while back, aren't you? You've returned to Vault!"
static void Event_RecognizeThiefCatchers(void) {
  Engine_EventBegin();
  Engine_ActorFaceActor(ACTOR_GREETER, ACTOR_PARTY_LEADER, 2);
  Engine_GameFlagSet(FLAG_GREETER_RECOGNIZED_PARTY);
  Engine_EventSetMessage(MsgVaultWarriorsWhoCaptured);
  Engine_EventShowMessage(ACTOR_GREETER, 0);
  Engine_EventEnd();
}

/// There are rumors of thieves from Lunpa running wild, yet they have been
/// quiet.
static void Event_TalkOfLunpaThieves(void) {

  Engine_EventSetMessage(MsgVaultRumorsThievesLunpa);
  Engine_ActorFaceEachOther(ACTOR_LUNPA_GOSSIP, ACTOR_PARTY_LEADER, 2);
  VaultRevisit_ShowStandingActorMessage(ACTOR_LUNPA_GOSSIP);
}

/// The villager was just talking about the party with the mayor: they are so
/// strong, let those thieves attack. Sets FLAG_PARTY_PRAISED and
/// FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES (0x868).
static void Event_PraisePartyStrength(void) {

  Engine_EventBegin();
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
  Engine_ActorFaceEachOther(ACTOR_VOLCANO_WATCHER, ACTOR_PARTY_LEADER, 2);
  VaultRevisit_ShowStandingActorMessage(ACTOR_VOLCANO_WATCHER);
}

/// The first of the two walkers stops circling. "Hey, it's that warrior guy!"
/// and "Yeah, and Ivan, too." the first time (FLAG_WARRIOR_GUY_GREETED), then
/// "I'm so glad you came back to Vault!" Bit 1 of the walker's timer keeps
/// Vault_UpdateWalkingVillager from moving it while it talks.
static void Event_GreetWarriorGuy(void) {
  struct FieldActor *actor = Actor_Get(ACTOR_FIRST_WALKER);
  u16 facing = actor->facing;
  s32 text;

  actor->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  Engine_EventBegin();
  text = MsgVaultWarriorGuy;
  Engine_EventSetMessage(text);
  Engine_ActorSetAnimation(ACTOR_FIRST_WALKER, 0);
  Engine_ActorFaceEachOther(ACTOR_FIRST_WALKER, ACTOR_PARTY_LEADER, 2);
  if (GameFlag_IsSet(FLAG_WARRIOR_GUY_GREETED) == 0) {
    Actor_ShowEmote(ACTOR_FIRST_WALKER, EMOTE_IN_FRONT, 60);
    Event_ShowMessageAndWait(ACTOR_FIRST_WALKER, 0, 10);
    Event_ShowMessageAndWait(ACTOR_FIRST_WALKER, 0, 10);
    GameFlag_Set(FLAG_WARRIOR_GUY_GREETED);
  }
  Engine_EventSetMessage(text + 2);
  Event_ShowMessageAndWait(ACTOR_FIRST_WALKER, 0, 10);
  actor->facing = facing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer = 1;
  GameFlag_Set(FLAG_WALKER_SPOKE);
}

/// The second walker stops. "I wish Vault's elders were as brave as you!"
static void Event_WishEldersWereBrave(void) {
  struct FieldActor *actor;
  s16 originalFacing;

  actor = Object_GetById(ACTOR_SECOND_WALKER);
  originalFacing = actor->facing;
  actor->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultWishVaultsElders);
  Engine_ActorSetAnimation(ACTOR_SECOND_WALKER, 0);
  Engine_ActorFaceEachOther(ACTOR_SECOND_WALKER, ACTOR_PARTY_LEADER, 2);
  Engine_EventShowMessageAndWait(ACTOR_SECOND_WALKER, 0, 10);
  actor->facing = originalFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer = 0;
}

/// The watchman asks about the jail. "Did you guys check out the jail?" Yes:
/// how did the thugs break out? No: the thieves broke out and got away.
static void Event_AskAboutJailbreak(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGuysCheckJail);
  Engine_ActorFaceEachOther(ACTOR_WATCHMAN, ACTOR_PARTY_LEADER, 2);
  Engine_EventOpenMessage(ACTOR_WATCHMAN, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message += 1;
  }
  Engine_EventShowMessage(ACTOR_WATCHMAN, 0);
  Engine_GameFlagSet(FLAG_ASKED_ABOUT_JAIL);
  Engine_EventEnd();
}

/// Mind Read on the greeter before he has spoken: he turns and recognizes the
/// party.
static void Event_ReadMindAndRecognizeThiefCatchers(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_GREETER, 1);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_GREETER, ACTOR_PARTY_LEADER, 20);
  Engine_GameFlagSet(FLAG_GREETER_RECOGNIZED_PARTY);
  Engine_EventSetMessage(MsgVaultWarriorsWhoCaptured);
  Engine_EventShowMessageAndWait(ACTOR_GREETER, 0, 20);
  Engine_EventEnd();
}

/// Mind Read on the villager who talks of Lunpa's thieves: since Dodonpa took
/// over, Lunpa's outlaws are no better than thieves, and there must be a reason
/// they are so quiet.
static void Event_ReadMindOnDodonpa(void) {
  Engine_EventSetMessage(MsgVaultOnceDodonpaTook);
  Object_GetById(ACTOR_LUNPA_GOSSIP)->paused = 1;
  VaultRevisit_ShowStandingActorMessage(ACTOR_LUNPA_GOSSIP);
  Object_GetById(ACTOR_LUNPA_GOSSIP)->paused = 0;
}

/// Mind Read before the villager has spoken: he turns and praises the party.
static void Event_ReadMindAndPraisePartyStrength(void) {
  Engine_EventBegin();
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
  Object_GetById(ACTOR_VOLCANO_WATCHER)->paused = 1;
  VaultRevisit_ShowStandingActorMessage(ACTOR_VOLCANO_WATCHER);
  Object_GetById(ACTOR_VOLCANO_WATCHER)->paused = 0;
}

/// Mind Read on the first walker: watching the party makes one feel brave, or,
/// before the walker has spoken, the greeting.
static void Event_ReadMindOnFeelingBrave(void) {
  Actor_Get(ACTOR_FIRST_WALKER)->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_WALKER_SPOKE) != 0) {
    Engine_EventSetMessage(MsgVaultWatchingGuysMakes);
    VaultRevisit_ShowStandingActorMessage(ACTOR_FIRST_WALKER);
  } else {
    Event_GreetWarriorGuy();
    GameFlag_Set(FLAG_WALKER_SPOKE);
  }
  Engine_EventEnd();
  Actor_Get(ACTOR_FIRST_WALKER)->timer = 1;
}

/// Mind Read on the second walker: grown-ups always get so scared.
static void Event_ReadMindOnScaredGrownUps(void) {
  {
    struct FieldActor *actor = Object_GetById(ACTOR_SECOND_WALKER);
    actor->timer |= VAULT_PROXIMITY_REACTION_PAUSED;
  }
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGrownUpsAlways);
  VaultRevisit_ShowStandingActorMessage(ACTOR_SECOND_WALKER);
  Engine_EventEnd();
  {
    s32 clear = 0;
    struct FieldActor *actor = Object_GetById(ACTOR_SECOND_WALKER);
    actor->timer = clear;
  }
}

/// Mind Read on the watchman: before he has asked about the jail he asks
/// now; afterwards, the thieves must know the secret of the jail's lock.
static void Event_ReadMindOnJailKey(void) {
  s32 requestHeard;
  s32 unpaused;

  requestHeard = GameFlag_IsSet(FLAG_ASKED_ABOUT_JAIL);
  if (requestHeard == 0) {
    Engine_EventBegin();
    Actor_Get(ACTOR_WATCHMAN)->paused = 1;
    Engine_ActorSetAnimation(ACTOR_WATCHMAN, ANIM_STAND);
    Engine_ActorRunRepeatedMotion(ACTOR_WATCHMAN, 1);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgVaultGuysCheckJail);
    Engine_ActorFaceEachOther(ACTOR_WATCHMAN, 0, 2);
    Event_OpenMessage(ACTOR_WATCHMAN, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
      gEventWork->message++;
    }
    Event_ShowMessage(ACTOR_WATCHMAN, 0);
    Actor_Get(ACTOR_WATCHMAN)->paused = requestHeard;
    Actor_SetBuiltinAction(ACTOR_WATCHMAN, 2);
    Engine_EventEnd();
    GameFlag_Set(FLAG_ASKED_ABOUT_JAIL);
  } else {
    Engine_EventSetMessage(MsgVaultNotLikeEasy);
    Actor_Get(ACTOR_WATCHMAN)->paused = 1;
    VaultRevisit_ShowStandingActorMessage(ACTOR_WATCHMAN);
    unpaused = 0;
    Actor_Get(ACTOR_WATCHMAN)->paused = unpaused;
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
  Engine_EventBegin();
  Event_PlayCue(SE_100);
  Engine_EventWait(40);
  if (GameFlag_IsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) == 0) {
    Actor_SetAttachedEffect(ACTOR_VAULT_VENUS_DJINNI, 0x102);
    Engine_ActorJump(ACTOR_VAULT_VENUS_DJINNI, 4, 0);
    Engine_EventWait(12);
    Engine_ActorJump(ACTOR_VAULT_VENUS_DJINNI, 4, 0);
    Engine_EventWait(20);
    // Let the Djinni leap from hiding before it walks to the bell.
    VaultRevisit_JumpActorToAndWait(ACTOR_VAULT_VENUS_DJINNI, 392, 104,
                                    PIXELS(7));
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_VAULT_VENUS_DJINNI, 408, 104);
    Actor_WalkToAndWait(ACTOR_VAULT_VENUS_DJINNI, 408, 120);
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

/// Door 5 opens into the house of the man whose precious stone a cat ate.
static void Event_EnterGemOwnersHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_Scene5Cells, 56, 19);
  Vault_WalkThroughExit(408, 320, 5);
}

/// Door 6 opens into the house of the child who doubts the thieves were from
/// Lunpa.
static void Event_EnterChildsHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_Scene6Cells, 50, 18);
  Vault_WalkThroughExit(312, 304, 6);
}

/// Door 7 opens into the inn.
static void Event_EnterInn(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_Scene7Cells, 44, 17);
  Vault_WalkThroughExit(216, 288, 7);
}

/// Door 8 opens into the item shop, where the couple who talk of Colosso also
/// stand. The leader is drawn at sprite priority 3 as he walks in, as at
/// doors 9 and 12, and the collision of the doorway, a column of three cells
/// at (23, 12), is replaced so he can pass.
static void Event_EnterItemShop(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_Scene8Cells, 54, 13);
  Map_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224, 8);
}

/// Door 9 opens into the weapon and armor shop, drawn like door 8.
static void Event_EnterWeaponAndArmorShop(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_Scene9Cells, 49, 10);
  Map_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176, 9);
}

/// Door 10 opens into the mayor's house.
static void Event_EnterMayorsHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_Scene10Cells, 38, 6);
  Vault_WalkThroughExit(120, 144, 10);
}

// The remaining doorway helper retains its position in the overlay.
#include "common/vault_scene_cells.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

/// The jail door opens and the party walks in, drawn behind the doorway, to
/// the jail's entrance 10, where the lone prisoner sits.
static void Event_EnterJail(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVaultRevisit_Scene12Cells, 35, 9);
  Map_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, 12);
}

// Shared entrance toggles precede the Djinni and dog events.
#include "common/vault_toggle.h"

/// Talking to Sap, a Venus Djinni: it joins the party.
static void Event_TalkToVenusDjinni(void) {
  Djinn_JoinParty(ACTOR_VAULT_VENUS_DJINNI, 0, VAULT_VENUS_DJINNI_INDEX);
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
  Engine_EventBegin();
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
/// party give it one (gEventWork->unk172).
static void Event_TalkToDog(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRuffRrruff2);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
  Engine_ActorFaceEachOther(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(2);
  Engine_EventShowMessage(ACTOR_VAULT_DOG, 0);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, 1);

  if (PartyInventory_FindOwner(ITEM_BONE) != -1 &&
      Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0) {
    gEventWork->unk172 = 1;
  }

  Engine_EventEnd();
}

/// Describes the water heard at the bottom of the well.
static void Event_PeerIntoWell(void) {
  Engine_EventBegin();
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

/// On arrival: restores the collision block of touch triggers 1 and 2, creates
/// configured object 223 at (128, 420) and copies a 3-by-3 block of cells from
/// (45, 41) to (8, 45), which vault.c does not (XXX: what the copy shows is not
/// known), and hooks the two walkers to Vault_UpdateWalkingVillager. The dog
/// waits at the hidden passage once it has shown it. Coming in from the world
/// map (entrances 0 to 2) before Sap has joined clears
/// FLAG_VAULT_DJINNI_CALLED_BY_BELL, so Sap is hidden again; while it is
/// called and has not joined, it stands by the bell. Coming out of Vault
/// Sanctum (entrance 11) clears FLAG_SHOW_LOCATION_NAME, and coming up from
/// Vault Cave (entrance 13) clears FLAG_EXIT_ARRIVAL_PENDING (0x120), so the
/// leader's arrival animation does not play.
s32 Scene_Initialize(void) {

  struct FieldActor *actor;
  s32 mode;

  // Restore the passage cells and the town's ambient effect.
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00))
    Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  OverlayObject_CreateConfiguredObject(PIXELS(128), 0, PIXELS(420), 223);
  Map_CopyCellsTo(45, 41, 8, 45, 3, 3);
  Engine_TaskWait(1);
  // Resume the two walking villagers' proximity reactions.
  actor = Object_GetById(ACTOR_FIRST_WALKER);
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  {
    struct FieldActor *actor = Object_GetById(ACTOR_FIRST_WALKER);
    s32 mode = 1;
    actor->timer = mode;
  }
  mode = 0;
  actor = Object_GetById(ACTOR_SECOND_WALKER);
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  Object_GetById(ACTOR_SECOND_WALKER)->timer = mode;
  // Restore the dog's clue position and the bell's current Djinni position.
  if (Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE))
    Actor_SetPosition(ACTOR_VAULT_DOG, PIXELS(216), PIXELS(392));
  if (gGameState.entrance <= 2 &&
      !Engine_GameFlagIsSet(FLAG_DJINNI_0 + VAULT_VENUS_DJINNI_INDEX) &&
      !VaultRevisit_ShouldKeepBellDjinniPosition())
    Engine_GameFlagClear(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
  if (Engine_GameFlagIsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) &&
      !Engine_GameFlagIsSet(FLAG_DJINNI_0 + VAULT_VENUS_DJINNI_INDEX))
    Actor_SetPosition(ACTOR_VAULT_VENUS_DJINNI, PIXELS(408), PIXELS(120));
  if (gGameState.entrance == VAULT_REVISIT_FROM_SANCTUM)
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  if (gGameState.entrance == VAULT_REVISIT_FROM_VAULT_CAVE_1)
    Engine_GameFlagClear(FLAG_EXIT_ARRIVAL_PENDING);
  return 0;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gVaultRevisit_Actor0[] = {
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

s32 gVaultRevisit_Actor1[] = {
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
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
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
    {12, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_CAVE_1, CONDITION_ALWAYS, 217, 0, 409,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT_CAVE_3, CONDITION_ALWAYS, 420, 0, 136,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gVaultRevisit_Exits[] = {
    SCENE_EXITS(VaultRevisit),
    SCENE_EXIT(1, WorldMap, WORLD_MAP_FROM_VAULT_SOUTH),
    SCENE_EXIT(2, WorldMap, 44),
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
    SCENE_EXIT(12, VaultJail, 10),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_CAVE_1, VaultCave1,
               VAULT_CAVE_1_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_CAVE_3, VaultCave3,
               VAULT_CAVE_3_FROM_VAULT_REVISIT),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gVaultRevisit_Placements[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, (s32)gVaultRevisit_Actor0,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x66, CONDITION_ALWAYS, (s32)gVaultRevisit_Actor1,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x5a, 0x34, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

u16 gVaultRevisit_Scene5Cells[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 4), MAP_CELL_STEP(33, 52, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene6Cells[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 4), MAP_CELL_STEP(33, 50, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene7Cells[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 4), MAP_CELL_STEP(33, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene8Cells[] = {
    MAP_CELL_STEP(42, 48, 2, 2, 4), MAP_CELL_STEP(40, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene9Cells[] = {
    MAP_CELL_STEP(42, 46, 2, 2, 4), MAP_CELL_STEP(40, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene10Cells[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 4), MAP_CELL_STEP(33, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene12Cells[] = {
    MAP_CELL_STEP(35, 33, 2, 2, 4), MAP_CELL_STEP(33, 33, 2, 2, 4), MAP_CELL_STEPS_END,
};

struct SceneEvent gVaultRevisit_Events[] = {
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, 14 },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnableAlternatePassageCells) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RestorePassageCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGemOwnersHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterChildsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInn) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterItemShop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWeaponAndArmorShop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterMayorsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJail) },
    { EVENT_TALK, ACTOR_GREETER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RecognizeThiefCatchers) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgVaultYouveProbablyAlreadyHeardButThe) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatHappenedToHammet) },
    { EVENT_TALK, ACTOR_LUNPA_GOSSIP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkOfLunpaThieves) },
    { EVENT_TALK, ACTOR_MAYORS_VISITOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PraisePartyStrength) },
    { EVENT_TALK, ACTOR_VOLCANO_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopeVolcanoStaysCalm) },
    { EVENT_TALK, ACTOR_FIRST_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GreetWarriorGuy) },
    { EVENT_TALK, ACTOR_SECOND_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WishEldersWereBrave) },
    { EVENT_TALK, ACTOR_WATCHMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutJailbreak) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgVaultTheThievesEscapedButIThink) },
    { EVENT_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDog) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgVaultThereYouSeeTheRoofRepairs) },
    { EVENT_TALK, ACTOR_VAULT_VENUS_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToVenusDjinni) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RingBell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, FLAG_GREETER_RECOGNIZED_PARTY, EVENT_SCRIPT(Event_ReadMindAndRecognizeThiefCatchers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETER, CONDITION_ALWAYS, MSGNO(MsgVaultICantBelieveTheyreHereWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVaultWhenThoseGuysFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultYeahSomeWarriorsFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUNPA_GOSSIP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40c, FLAG_PARTY_PRAISED, EVENT_SCRIPT(Event_ReadMindAndPraisePartyStrength) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_VISITOR, CONDITION_ALWAYS, MSGNO(MsgVaultThoseStupidThievesEscapedAndWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VOLCANO_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnWinter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40e, FLAG_WALKER_SPOKE, EVENT_SCRIPT(Event_ReadMindOnFeelingBrave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnFeelingBrave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnScaredGrownUps) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, FLAG_ASKED_ABOUT_JAIL, EVENT_SCRIPT(Event_ReadMindOnJailKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnJailKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultIKeepWonderingWasOurStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, IF_FLAG_SET(FLAG_VAULT_DOG_SHOWN_PASSAGE), MSGNO(MsgVaultCarefulSearchWillRevealPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, MSGNO(MsgVaultIfYouBringMeSomeBones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgVaultThatsTheLastOfTheRoof) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PeerIntoWell) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT_SCRIPT(Event_GiveBoneToDog) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealPassage) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HidePassage) },
    { SCENE_EVENTS_END },
};
// clang-format on
