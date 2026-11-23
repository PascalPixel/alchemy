/// @file
/// Vault (SceneId_VaultRevisit), the town once the party has reached Kalay.
/// It has the same layout as Vault's first scene (vault.c), and replaces that
/// scene for the rest of the game.
///
/// The World Map still leads into the first scene. Once FLAG_KALAY_REACHED is
/// set, that scene's setup sends the party straight on here
/// (VAULT_REVISIT_FROM_VAULT). Vault Sanctum, Vault Cave, the jail and the
/// houses (log_rooms_b.c) lead back here directly.
///
/// The townspeople stand where they stood before, and the news is that the
/// thieves the party caught have broken out of jail, that the bridge to Kalay
/// is fixed, and that Hammet is still in Lunpa. Some speak in plain message
/// rows; the young man by the rope bridge, the woman in green, the old man by
/// the jail, the old woman by the inn, the two children round the well and
/// the strongman speak through events. Those who greet the party remember it
/// in area flags (FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED to
/// FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL), so that a Mind Read on one not yet
/// spoken to plays the greeting first. The old man's praise also sets
/// FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES, as the mayor's welcome indoors does.
///
/// This is where the walkthrough sends the player back for two things. Up on
/// the bell tower, ringing the bell startles Sap, a Venus Djinni, out of the
/// tree east of the tower onto the cliff top beside the cave mouth; the party
/// reaches it there through Vault Cave, and talking to it makes it join.
/// Coming into town from the World Map sends Sap back into its tree until it
/// has joined (FLAG_VAULT_SAP_CALLED_BY_BELL). And Reveal among the six
/// gravestones south of the inn shows a hole, the hidden passage down into
/// Vault Cave. The dog, given a Bone from the inn, runs to that spot and jumps
/// there three times as a hint (FLAG_VAULT_DOG_SHOWN_PASSAGE, shared with
/// vault.c). The cave mouth in the north-east cliff is the cave's other way
/// in.
///
/// The wooden box beside the mayor's house still holds a Nut if it was not
/// taken before, and the well rumbles with water. The cells the events answer
/// are enum VaultCellBehavior (vault.h).
///
/// The inn's roof is mended: Scene_Initialize copies a 3-by-3 block of cells
/// from (45, 41) to (8, 45), which draws the thatch whole where vault.c's town
/// shows the hole the fiery rock burnt in it.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "game/inventory.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/vault.h"
#include "scene/vault_range.h"
#include "sound/sound.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "window/window.h"
#include "text/msg_ids.h"

extern s32 gVault_BoyRoundTheWellRoute[];
extern s32 gVault_GirlRoundTheWellRoute[];
extern struct SceneEntrance gVaultRevisit_Entrances[];
extern u32 gVaultRevisit_Exits[];
extern struct ActorPlacement gVaultRevisit_ActorPlacements[];
extern u16 gVaultRevisit_GemOwnersHouseDoorCells[];
extern u16 gVaultRevisit_ChildsHouseDoorCells[];
extern u16 gVaultRevisit_InnDoorCells[];
extern u16 gVaultRevisit_ItemShopDoorCells[];
extern u16 gVaultRevisit_WeaponAndArmorShopDoorCells[];
extern u16 gVaultRevisit_MayorsHouseDoorCells[];
extern u16 gVaultRevisit_JailDoorCells[];
extern struct Trigger gVaultRevisit_Triggers[];

/// The townspeople, the dog and Sap, standing where vault.c places them.
enum VaultRevisitActorId {
  /// The young man with brown hair outside the gem owner's house in the east,
  /// by the rope bridge, who recognizes the party.
  ACTOR_WELCOMING_YOUNG_MAN = ACTOR_FIRST_PLACED,
  /// A woman with fair hair east of the six gravestones, who brings the news
  /// that the bridge to Kalay is fixed.
  ACTOR_WOMAN_ON_THE_FIXED_BRIDGE,
  /// A burly grey-haired man up on the bell tower, beside the bell, who
  /// worries about Hammet.
  ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET,
  /// A woman in a green dress who wanders north of the inn.
  ACTOR_WOMAN_ON_LUNPAS_THIEVES,
  /// An old man with a cane east of the jail's door, who has been talking
  /// with the mayor.
  ACTOR_MAYORS_VISITOR,
  /// An old woman who wanders east of the inn's door.
  ACTOR_OLD_WOMAN_ON_THE_VOLCANO,
  /// vault.c's two children, walking round the well again. The boy greets the
  /// party. "Hey, it's that warrior guy!"
  ACTOR_BOY_ROUND_THE_WELL,
  /// Wishes Vault's elders were as brave as the party.
  ACTOR_GIRL_ROUND_THE_WELL,
  /// The strongman who wanders near the west end of the rope bridge. "Did you
  /// guys check out the jail?"
  ACTOR_STRONGMAN_ON_WATCH,
  /// A bearded man in a long robe beside the Sanctum's door, who takes the
  /// thieves' escape calmly.
  ACTOR_VILLAGER_DIVINE_PLAN,
  /// The dog south of the six gravestones.
  ACTOR_VAULT_DOG,
  /// The young man at the foot of the ladder against the inn's west wall, the
  /// roof now mended.
  ACTOR_YOUNG_MAN_FIXING_THE_ROOF,
  /// Sap, a Venus Djinni, hiding in the tree east of the bell tower until the
  /// bell calls it. The row before it, SPRITE_IVY placed at 0, 0, is never
  /// referred to.
  ACTOR_SAP = ACTOR_FIRST_PLACED + 13
};

/// Area flags for what the townspeople have already said, kept while the
/// party stays in Vault's scenes. A Mind Read before someone has spoken plays
/// the conversation first.
enum {
  FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED = FLAG_AREA_TEMP_00,
  FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY = FLAG_AREA_TEMP_05,
  FLAG_VAULT_REVISIT_PARTY_PRAISED = FLAG_AREA_TEMP_06,
  FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE = FLAG_AREA_TEMP_07,
  FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL = FLAG_AREA_TEMP_08
};

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/actor_distance.c"
#include "shared/vault_range.c"
#include "shared/vault_nearby.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gVaultRevisit_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gVaultRevisit_Exits; }

/// The townspeople, the dog and Sap.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gVaultRevisit_ActorPlacements;
}

/// Stands the person still while they say the message already chosen.
static void VaultRevisit_ShowStandingActorMessage(s32 actorId) {
  Event_Start();
  Actor_SetAnimation(actorId, ANIM_STAND);
  Event_ShowMessage(actorId, 0);
  Event_End();
}

/// The young man outside the gem owner's house, by the rope bridge, turns to
/// Isaac and recognizes the party: "You're the warriors who captured the
/// thieves a while back, aren't you? You've returned to Vault!"
static void VaultRevisit_RecognizeWarriors(void) {
  Event_Start();
  Actor_FaceActor(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 2);
  GameFlag_Set(FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY);
  Event_SetMessageId(MsgVaultWarriorsWhoCaptured);
  // "You! You're the warriors who captured the thieves a while..."
  Event_ShowMessage(ACTOR_WELCOMING_YOUNG_MAN, 0);
  Event_End();
}

/// The woman in green who wanders north of the inn stops and turns to Isaac:
/// there have been rumors of thieves from Lunpa running wild, and she wonders
/// why they have been so quiet recently.
static void VaultRevisit_TalkOfLunpaThieves(void) {
  Event_SetMessageId(MsgVaultRumorsThievesLunpa);
  Actor_TurnToFaceEachOther(ACTOR_WOMAN_ON_LUNPAS_THIEVES, ACTOR_PARTY_LEADER,
                            2);
  VaultRevisit_ShowStandingActorMessage(ACTOR_WOMAN_ON_LUNPAS_THIEVES);
}

/// The old man with a cane east of the jail's door turns to Isaac: he was just
/// talking about the party with the mayor, and they are so strong that the
/// thieves are welcome to attack.
static void VaultRevisit_PraisePartyStrength(void) {
  Event_Start();
  Actor_FaceActor(ACTOR_MAYORS_VISITOR, ACTOR_PARTY_LEADER, 2);
  GameFlag_Set(FLAG_VAULT_REVISIT_PARTY_PRAISED);
  GameFlag_Set(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  Event_SetMessageId(MsgVaultTalkingMayorStrong);
  // "Oh, it's you... I was just talking about you with our mayor...."
  Event_ShowMessage(ACTOR_MAYORS_VISITOR, 0);
  Event_End();
}

/// The old woman who wanders east of the inn's door stops and turns to Isaac:
/// if the volcano stays calm and the southern bridge stays fixed, Vault won't
/// have any worries at all.
static void VaultRevisit_HopeVolcanoStaysCalm(void) {
  Event_SetMessageId(MsgVaultVolcanoStaysCalm);
  Actor_TurnToFaceEachOther(ACTOR_OLD_WOMAN_ON_THE_VOLCANO, ACTOR_PARTY_LEADER,
                            2);
  VaultRevisit_ShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_VOLCANO);
}

/// The boy walking round the well stops and turns to Isaac. The first time, an
/// exclamation mark pops up over him: "Hey, it's that warrior guy! Yeah, and
/// Ivan, too." Then, each time: "I'm so glad you came back to Vault!" He goes
/// back to walking round the well.
static void VaultRevisit_GreetWarriorGuy(void) {
  struct Actor *actor = Actor_Get(ACTOR_BOY_ROUND_THE_WELL);
  u16 facing = actor->facing;

  actor->timer |= VILLAGER_TALKED_TO;
  Event_Start();
  Event_SetMessageId(MsgVaultWarriorGuy);
  Actor_SetAnimation(ACTOR_BOY_ROUND_THE_WELL, ANIM_STILL);
  Actor_TurnToFaceEachOther(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  if (Event_GetFlag(FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED) == 0) {
    Actor_ShowEmote(ACTOR_BOY_ROUND_THE_WELL,
                    EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    // "Hey, it's that warrior guy!"
    Event_ShowMessageThenWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
    // "Yeah, and Ivan, too."
    Event_ShowMessageThenWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
    GameFlag_Set(FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED);
  }
  Event_SetMessageId(MsgVaultWarriorGuy + 2);
  // "I'm so glad you came back to Vault!"
  Event_ShowMessageThenWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
  actor->facing = facing;
  wait(1);
  Event_End();
  actor->timer = VILLAGER_IS_BOY;
  GameFlag_Set(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE);
}

/// The girl walking round the well stops and turns to Isaac: "I wish Vault's
/// elders were as brave as you!"
static void VaultRevisit_WishEldersWereBrave(void) {
  struct Actor *actor;
  s16 originalFacing;

  actor = Actor_Get(ACTOR_GIRL_ROUND_THE_WELL);
  originalFacing = actor->facing;
  actor->timer |= VILLAGER_TALKED_TO;
  Event_Start();
  Event_SetMessageId(MsgVaultWishVaultsElders);
  Actor_SetAnimation(ACTOR_GIRL_ROUND_THE_WELL, ANIM_STILL);
  Actor_TurnToFaceEachOther(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  // "I wish Vault's elders were as brave as you!"
  Event_ShowMessageThenWait(ACTOR_GIRL_ROUND_THE_WELL, 0, 10);
  actor->facing = originalFacing;
  wait(1);
  Event_End();
  actor->timer = 0;
}

/// The strongman by the rope bridge asks, "Oh, so did you guys check out the
/// jail?" If Isaac nods, he wonders how the thugs broke out, the bars being so
/// strong; if he shakes his head, the strongman tells him the thieves broke
/// out and got away.
static void VaultRevisit_AskAboutJail(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultGuysCheckJail);
  Actor_TurnToFaceEachOther(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
  // "Oh, so did you guys check out the jail?"
  Event_OpenMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    gFieldWork->message += 1;
  }
  Event_ShowMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
  GameFlag_Set(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
  Event_End();
}

/// Mind Read on the young man by the rope bridge before he has spoken: he
/// turns to Isaac and recognizes the party aloud, as when talked to.
static void VaultRevisit_ReadMindAndRecognizeWarriors(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_WELCOMING_YOUNG_MAN, 1);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 20);
  GameFlag_Set(FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY);
  Event_SetMessageId(MsgVaultWarriorsWhoCaptured);
  // "You! You're the warriors who captured the thieves a while..."
  Event_ShowMessageThenWait(ACTOR_WELCOMING_YOUNG_MAN, 0, 20);
  Event_End();
}

/// Mind Read on the woman in green north of the inn: "Once Dodonpa took over,
/// Lunpa's outlaws became no better than thieves. There's gotta be a reason
/// those guys are being so quiet."
static void VaultRevisit_ReadMindOnDodonpa(void) {
  Event_SetMessageId(MsgVaultOnceDodonpaTook);
  Actor_Get(ACTOR_WOMAN_ON_LUNPAS_THIEVES)->paused = 1;
  VaultRevisit_ShowStandingActorMessage(ACTOR_WOMAN_ON_LUNPAS_THIEVES);
  Actor_Get(ACTOR_WOMAN_ON_LUNPAS_THIEVES)->paused = 0;
}

/// Mind Read on the old man by the jail's door before he has spoken: he turns
/// to Isaac and praises the party aloud, as when talked to.
static void VaultRevisit_ReadMindAndPraisePartyStrength(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_MAYORS_VISITOR, 1);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_MAYORS_VISITOR, ACTOR_PARTY_LEADER, 20);
  GameFlag_Set(FLAG_VAULT_REVISIT_PARTY_PRAISED);
  GameFlag_Set(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  Event_SetMessageId(MsgVaultTalkingMayorStrong);
  // "Oh, it's you... I was just talking about you with our mayor...."
  Event_ShowMessageThenWait(ACTOR_MAYORS_VISITOR, 0, 20);
  Event_End();
}

/// Mind Read on the old woman east of the inn's door: "It would have been
/// great to have everything solved before winter came."
static void VaultRevisit_ReadMindOnWinter(void) {
  Event_SetMessageId(MsgVaultGreatEverythingSolved);
  Actor_Get(ACTOR_OLD_WOMAN_ON_THE_VOLCANO)->paused = 1;
  VaultRevisit_ShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_VOLCANO);
  Actor_Get(ACTOR_OLD_WOMAN_ON_THE_VOLCANO)->paused = 0;
}

/// Mind Read on the boy walking round the well: watching the party makes him
/// feel brave, and he hopes they'll tell him all about their travels
/// sometime. If he has not spoken to them yet, he greets them aloud instead.
static void VaultRevisit_ReadMindOnFeelingBrave(void) {
  Actor_Get(ACTOR_BOY_ROUND_THE_WELL)->timer |= VILLAGER_TALKED_TO;
  Event_Start();
  if (Event_GetFlag(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE) != 0) {
    Event_SetMessageId(MsgVaultWatchingGuysMakes);
    VaultRevisit_ShowStandingActorMessage(ACTOR_BOY_ROUND_THE_WELL);
  } else {
    VaultRevisit_GreetWarriorGuy();
    GameFlag_Set(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE);
  }
  Event_End();
  Actor_Get(ACTOR_BOY_ROUND_THE_WELL)->timer = VILLAGER_IS_BOY;
}

/// Mind Read on the girl walking round the well: "Grown-ups always get so
/// scared whenever anything happens. I get so tired of that..."
static void VaultRevisit_ReadMindOnScaredGrownUps(void) {
  Actor_Get(ACTOR_GIRL_ROUND_THE_WELL)->timer |= VILLAGER_TALKED_TO;
  Event_Start();
  Event_SetMessageId(MsgVaultGrownUpsAlways);
  VaultRevisit_ShowStandingActorMessage(ACTOR_GIRL_ROUND_THE_WELL);
  Event_End();
  Actor_Get(ACTOR_GIRL_ROUND_THE_WELL)->timer = 0;
}

/// Mind Read on the strongman by the rope bridge: it's not easy to grab the
/// key to that jail, so the thieves must know the secret of the lock, and
/// that must have been great! If he has not asked about the jail yet, he asks
/// aloud instead, then wanders off again.
static void VaultRevisit_ReadMindOnJailKey(void) {
  s32 requestHeard;

  requestHeard = Event_GetFlag(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
  if (requestHeard == 0) {
    Event_Start();
    Actor_Get(ACTOR_STRONGMAN_ON_WATCH)->paused = 1;
    Actor_SetAnimation(ACTOR_STRONGMAN_ON_WATCH, ANIM_STAND);
    Actor_RunRepeatedMotion(ACTOR_STRONGMAN_ON_WATCH, 1);
    Event_Wait(20);
    Event_SetMessageId(MsgVaultGuysCheckJail);
    Actor_TurnToFaceEachOther(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
    // "Oh, so did you guys check out the jail?"
    Event_OpenMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
      gFieldWork->message++;
    }
    Event_ShowMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
    Actor_Get(ACTOR_STRONGMAN_ON_WATCH)->paused = 0;
    Actor_SetMovementTypeById(ACTOR_STRONGMAN_ON_WATCH, MOVEMENT_TYPE_WANDER);
    Event_End();
    GameFlag_Set(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
  } else {
    Event_SetMessageId(MsgVaultNotLikeEasy);
    Actor_Get(ACTOR_STRONGMAN_ON_WATCH)->paused = 1;
    VaultRevisit_ShowStandingActorMessage(ACTOR_STRONGMAN_ON_WATCH);
    Actor_Get(ACTOR_STRONGMAN_ON_WATCH)->paused = 0;
  }
}

/// Jumps the actor to (x, z), launched upward at `velocityY`, and waits there
/// until it lands, a second at most.
static void VaultRevisit_JumpActorToAndWait(s32 actorId, s32 x, s32 z,
                                            FIXED velocityY) {
  struct Actor *jumpingActor = Actor_Get(actorId);
  s32 frames;
  Actor_SetSpeed(actorId, 3);
  jumpingActor->gravity = toFIXED(1) / 2;
  jumpingActor->bounce = 0;
  jumpingActor->velocityY = velocityY;
  Actor_SetSpriteFlags(jumpingActor, 0);
  Actor_MoveToAndWait(actorId, x, z);
  Actor_SetPosition(actorId, x, z);
  for (frames = FRAMES_PER_SECOND; frames != 0; --frames) {
    wait(1);
    if ((FIXEDtoINT(jumpingActor->velocityY)) == 0)
      break;
  }
  Actor_SetSpriteFlags(jumpingActor, SPRITE_FLAG_SHADOW);
  jumpingActor->gravity = toFIXED(1);
}

/// Up on the bell tower, Isaac rings the bell (cell (17, 8)). The first time
/// since the party came into town, Sap the Venus Djinni pops up, startled, on
/// top of the tree east of the tower, hops twice and leaps away north-east
/// onto the cliff top beside the cave mouth, where it waits.
static void VaultRevisit_RingBell(void) {
  Event_Start();
  Sound_Play(SE_BELL);
  Event_Wait(40);
  if (Event_GetFlag(FLAG_VAULT_SAP_CALLED_BY_BELL) == 0) {
    Actor_SetAttachedEffect(ACTOR_SAP,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_Jump(ACTOR_SAP, 4, 0);
    Event_Wait(12);
    Actor_Jump(ACTOR_SAP, 4, 0);
    Event_Wait(20);
    VaultRevisit_JumpActorToAndWait(ACTOR_SAP, 392, 104, toFIXED(7));
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_SAP, 408, 104);
    Actor_WalkToAndWait(ACTOR_SAP, 408, 120);
    GameFlag_Set(FLAG_VAULT_SAP_CALLED_BY_BELL);
  }
  Event_End();
}

#include "shared/vault_door_exit.c"

/// Isaac opens the door of the house in the east, by the rope bridge (cell
/// (25, 21)), the gem owner's, and goes in.
static void VaultRevisit_EnterGemOwnersHouse(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gVaultRevisit_GemOwnersHouseDoorCells, 56, 19);
  Vault_WalkThroughExit(408, 320, VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTHEAST);
}

/// Isaac opens the door of the house east of the inn (cell (19, 20)), a
/// child's home, and goes in.
static void VaultRevisit_EnterChildsHouse(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gVaultRevisit_ChildsHouseDoorCells, 50, 18);
  Vault_WalkThroughExit(312, 304, VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTH);
}

/// Isaac opens the inn's door, in the middle of town (cell (13, 19)), and goes
/// in.
static void VaultRevisit_EnterInn(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gVaultRevisit_InnDoorCells, 44, 17);
  Vault_WalkThroughExit(216, 288, VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTHWEST);
}

/// Isaac opens the item shop's door, under the jar sign in the cliff in the
/// east of town (cell (23, 15)), and goes in, vanishing into the doorway as
/// he walks: he is drawn behind the cliff from the door's first step.
static void VaultRevisit_EnterItemShop(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct Sprite *sprite = actor->sprite;

  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gVaultRevisit_ItemShopDoorCells, 54, 13);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224, VAULT_REVISIT_TO_LOG_ROOMS_B_EAST);
}

/// Isaac opens the weapon and armor shop's door, under the sword sign in the
/// cliff below the bell tower (cell (18, 12)), and goes in, as at the item
/// shop.
static void VaultRevisit_EnterWeaponAndArmorShop(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct Sprite *sprite = actor->sprite;

  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gVaultRevisit_WeaponAndArmorShopDoorCells, 49, 10);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176, VAULT_REVISIT_TO_LOG_ROOMS_B_NORTH);
}

/// Isaac opens the door of the mayor's house, the big house in the town's
/// north-west (cell (7, 10)), and goes in.
static void VaultRevisit_EnterMayorsHouse(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gVaultRevisit_MayorsHouseDoorCells, 38, 6);
  Vault_WalkThroughExit(120, 144, VAULT_REVISIT_TO_LOG_ROOMS_B_NORTHWEST);
}

#include "shared/vault_map_cells.c"

/// Isaac opens the jail's grey iron door, in the cliff below the mayor's house
/// (cell (4, 11)), and walks into the jail the thieves broke out of,
/// vanishing into the doorway as at the shops.
static void VaultRevisit_EnterJail(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct Sprite *sprite = actor->sprite;

  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gVaultRevisit_JailDoorCells, 35, 9);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, VAULT_REVISIT_TO_STONE_ROOMS);
}

#include "shared/vault_toggle.c"

/// On the cliff top beside the cave mouth, where the bell sent it, talking to
/// Sap, a Venus Djinni, makes it join the party.
static void VaultRevisit_SapJoins(void) {
  Djinn_JoinParty(ACTOR_SAP, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP));
}

/// The dog jumps once on the hidden passage's spot, then pauses.
static inline void VaultRevisit_DogJumpAtPassage(void) {
  Actor_Jump(ACTOR_VAULT_DOG, 6, 0);
  Event_Wait(30);
}

/// Isaac gives the dog south of the six gravestones a Bone. It trots
/// north to the middle of the gravestones (cell (13, 25)), turns south and
/// jumps there three times, then steps one cell north and sits facing the
/// spot, where Reveal will show the passage.
static void VaultRevisit_GiveBoneToDog(void) {
  PartyInventory_Discard(ITEM_BONE);
  Event_Start();
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_VAULT_DOG, 2);
  Actor_SetSpeed(ACTOR_VAULT_DOG, 0.8);
  Actor_WalkToAndWait(ACTOR_VAULT_DOG, 216, 408);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
  VaultRevisit_DogJumpAtPassage();
  VaultRevisit_DogJumpAtPassage();
  VaultRevisit_DogJumpAtPassage();
  Actor_WalkToAndWait(ACTOR_VAULT_DOG, 216, 392);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
  GameFlag_Set(FLAG_VAULT_DOG_SHOWN_PASSAGE);
  Event_End();
}

/// When Reveal wears off, the hole among the six gravestones (cell (13, 25))
/// is grass again and can no longer be walked into.
static void VaultRevisit_HidePassage(void) {
  MapGrid_CopyCellAttributes(41, 43, 1, 1, 13, 25);
  MapGrid_CopyCellsTo(40, 42, 12, 22, 3, 3);
}

/// Reveal, cast near the six gravestones south of the inn, shows a dark hole
/// in the middle of them (cell (13, 25)), where the dog jumps: the hidden
/// passage down into Vault Cave, which Isaac can walk into while the spell
/// lasts.
static void VaultRevisit_RevealPassage(void) {
  MapGrid_CopyCellAttributes(37, 43, 1, 1, 13, 25);
  MapGrid_CopyCellsTo(36, 42, 12, 22, 3, 3);
}

/// The dog south of the six gravestones turns to Isaac and barks. "Ruff!
/// Rrruff!!" If Isaac carries a Bone and the dog has not yet shown its spot,
/// the item menu opens so that he can give it one.
static void VaultRevisit_TalkToDog(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultRuffRrruffAfterCapture);
  Actor_SetAnimation(ACTOR_VAULT_DOG, ANIM_STILL);
  Actor_TurnToFaceEachOther(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 0);
  Event_Wait(2);
  // "Ruff! Rrruff!!"
  Event_ShowMessage(ACTOR_VAULT_DOG, 0);
  Actor_SetAnimation(ACTOR_VAULT_DOG, ANIM_STAND);

  if (PartyInventory_FindMember(ITEM_BONE) != -1 &&
      GameFlag_Get(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0) {
    gFieldWork->fieldMenu = 1;
  }

  Event_End();
}

/// Isaac peers into the well in the south-west of Vault (cells (7, 24) to
/// (8, 25)): "You can hear water rumbling down there."
static void VaultRevisit_PeerIntoWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgVaultCanHearWaterRumblingDown, 1);
  Event_End();
}

/// The town's events: its people, doors, bell, well and box, the dog's Bone
/// and Reveal among the gravestones.
const struct Trigger *Scene_GetTriggers(void) { return gVaultRevisit_Triggers; }

/// Patched editions keep Sap on the cliff top when the party keeps its
/// position, as on a return from battle.
static inline s32 VaultRevisit_ShouldKeepBellDjinniPosition(void) {
#if TBS_PATCH_1
  return GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
#else
  return 0;
#endif
}

/// Sets up Vault as the party finds it after reaching Kalay.
s32 Scene_Initialize(void) {
  if (GameFlag_Get(FLAG_VAULT_ON_BRIDGE))
    MapGrid_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  // A crate on the well, drawn hidden (SPRITE_COLOR_HIDDEN) and never a talk or
  // Psynergy target, as vault.c makes it. XXX (no evidence): what it is for;
  // nothing refers to it again, and every pixel of it is transparent.
  Actor_CreateConfigured(128, 0, 420, SPRITE_CRATE);
  MapGrid_CopyCellsTo(45, 41, 8, 45, 3, 3);
  wait(1);
  Actor_Get(ACTOR_BOY_ROUND_THE_WELL)->update =
      (void (*)(struct Actor *))Vault_UpdateWalkingVillager;
  Actor_Get(ACTOR_BOY_ROUND_THE_WELL)->timer = VILLAGER_IS_BOY;
  Actor_Get(ACTOR_GIRL_ROUND_THE_WELL)->update =
      (void (*)(struct Actor *))Vault_UpdateWalkingVillager;
  Actor_Get(ACTOR_GIRL_ROUND_THE_WELL)->timer = 0;
  if (GameFlag_Get(FLAG_VAULT_DOG_SHOWN_PASSAGE))
    Actor_SetPosition(ACTOR_VAULT_DOG, 216, 392);
  if (gMainRoutineWork.entrance <= VAULT_REVISIT_NORTH_UNUSED &&
      !GameFlag_Get(FLAG_DJINNI(DJINNI_SAP)) &&
      !VaultRevisit_ShouldKeepBellDjinniPosition())
    GameFlag_Clear(FLAG_VAULT_SAP_CALLED_BY_BELL);
  if (GameFlag_Get(FLAG_VAULT_SAP_CALLED_BY_BELL) &&
      !GameFlag_Get(FLAG_DJINNI(DJINNI_SAP)))
    Actor_SetPosition(ACTOR_SAP, 408, 120);
  if (gMainRoutineWork.entrance == VAULT_REVISIT_FROM_SANCTUM)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  if (gMainRoutineWork.entrance == VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE)
    GameFlag_Clear(FLAG_ARRIVAL_STAIRS_UP);
  return 0;
}

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "shared/vault_well_routes.c"

struct SceneEntrance gVaultRevisit_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_REVISIT_NORTH_UNUSED, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTHEAST, CONDITION_ALWAYS, 408, 0, 344,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTH, CONDITION_ALWAYS, 312, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTHWEST, CONDITION_ALWAYS, 216, 0, 312,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_EAST, CONDITION_ALWAYS, 376, 0, 248,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_NORTH, CONDITION_ALWAYS, 296, 0, 200,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_NORTHWEST, CONDITION_ALWAYS, 120, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_SANCTUM, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_STONE_ROOMS, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH,
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
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTHEAST, LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTH, LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTH),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTHWEST, LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_EAST, LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_EAST),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_NORTH, LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTH),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_NORTHWEST, LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTHWEST),
    SCENE_EXIT(VAULT_REVISIT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_STONE_ROOMS, StoneRooms,
               STONE_ROOMS_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE, VaultCavePassage,
               VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_CAVE_MOUTH, VaultCaveMouth,
               VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gVaultRevisit_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        381, 0, 342, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 396, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 150, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 204, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        124, 0, 196, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 300, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVault_BoyRoundTheWellRoute,
        132, 0, 380, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVault_GirlRoundTheWellRoute,
        132, 0, 420, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        332, 0, 412, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        92, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 440, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SAP), MOVEMENT_TYPE_STAND,
        344, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
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

struct Trigger gVaultRevisit_Triggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_VAULT_SOUTH_GATE, CONDITION_ALWAYS, VAULT_REVISIT_TO_WORLD_MAP_SOUTH },
    { TRIGGER_EXIT, CELL_BEHAVIOR_VAULT_NORTH_GATE, CONDITION_ALWAYS, VAULT_REVISIT_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, CELL_BEHAVIOR_VAULT_HIDDEN_PASSAGE, CONDITION_ALWAYS, VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE },
    { TRIGGER_EXIT, CELL_BEHAVIOR_VAULT_CLIFF_CAVE_MOUTH, CONDITION_ALWAYS, VAULT_REVISIT_TO_VAULT_CAVE_MOUTH },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_BRIDGE_END, CONDITION_ALWAYS, EVENT(Vault_RaisePathToBridge) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT(Vault_LowerPathUnderBridge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(VaultRevisit_EnterGemOwnersHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(VaultRevisit_EnterChildsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_INN_DOOR, CONDITION_ALWAYS, EVENT(VaultRevisit_EnterInn) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT(VaultRevisit_EnterItemShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT(VaultRevisit_EnterWeaponAndArmorShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(VaultRevisit_EnterMayorsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT(Vault_OpenDoorwayAndEnter) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_DOOR_TO_JAIL, CONDITION_ALWAYS, EVENT(VaultRevisit_EnterJail) },
    { TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT(VaultRevisit_RecognizeWarriors) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_FIXED_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgVaultYouveProbablyAlreadyHeardButThe) },
    { TRIGGER_TALK, ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatHappenedToHammet) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_LUNPAS_THIEVES, CONDITION_ALWAYS, EVENT(VaultRevisit_TalkOfLunpaThieves) },
    { TRIGGER_TALK, ACTOR_MAYORS_VISITOR, CONDITION_ALWAYS, EVENT(VaultRevisit_PraisePartyStrength) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_VOLCANO, CONDITION_ALWAYS, EVENT(VaultRevisit_HopeVolcanoStaysCalm) },
    { TRIGGER_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(VaultRevisit_GreetWarriorGuy) },
    { TRIGGER_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(VaultRevisit_WishEldersWereBrave) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(VaultRevisit_AskAboutJail) },
    { TRIGGER_TALK, ACTOR_VILLAGER_DIVINE_PLAN, CONDITION_ALWAYS, MSGNO(MsgVaultTheThievesEscapedButIThink) },
    { TRIGGER_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(VaultRevisit_TalkToDog) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultThereYouSeeTheRoofRepairs) },
    { TRIGGER_TALK, ACTOR_SAP, CONDITION_ALWAYS, EVENT(VaultRevisit_SapJoins) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_BELL, CONDITION_ALWAYS, EVENT(VaultRevisit_RingBell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_WELCOMING_YOUNG_MAN, FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY, EVENT(VaultRevisit_ReadMindAndRecognizeWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultICantBelieveTheyreHereWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_FIXED_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgVaultWhenThoseGuysFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultYeahSomeWarriorsFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_LUNPAS_THIEVES, CONDITION_ALWAYS, EVENT(VaultRevisit_ReadMindOnDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_MAYORS_VISITOR, FLAG_VAULT_REVISIT_PARTY_PRAISED, EVENT(VaultRevisit_ReadMindAndPraisePartyStrength) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_VISITOR, CONDITION_ALWAYS, MSGNO(MsgVaultThoseStupidThievesEscapedAndWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_VOLCANO, CONDITION_ALWAYS, EVENT(VaultRevisit_ReadMindOnWinter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_BOY_ROUND_THE_WELL, FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE, EVENT(VaultRevisit_ReadMindOnFeelingBrave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(VaultRevisit_ReadMindOnFeelingBrave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(VaultRevisit_ReadMindOnScaredGrownUps) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_STRONGMAN_ON_WATCH, FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL, EVENT(VaultRevisit_ReadMindOnJailKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(VaultRevisit_ReadMindOnJailKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DIVINE_PLAN, CONDITION_ALWAYS, MSGNO(MsgVaultIKeepWonderingWasOurStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, IF_FLAG_SET(FLAG_VAULT_DOG_SHOWN_PASSAGE), MSGNO(MsgVaultCarefulSearchWillRevealPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, MSGNO(MsgVaultIfYouBringMeSomeBones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultThatsTheLastOfTheRoof) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_WELL, CONDITION_ALWAYS, EVENT(VaultRevisit_PeerIntoWell) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT(VaultRevisit_GiveBoneToDog) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(VaultRevisit_RevealPassage) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(VaultRevisit_HidePassage) },
    { TRIGGER_TABLE_END },
};
// clang-format on
