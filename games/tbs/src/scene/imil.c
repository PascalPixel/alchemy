/// @file
/// Imil, the snowbound village north of the Goma Range, near the lighthouse
/// that long ago held the Water of Hermes, and its houses and Sanctum.
///
/// Mia, the healer the party meets here, is of the Mercury Clan, who once
/// lived in Imil and guarded the lighthouse. She is the last of them.
///
/// An epidemic has struck the village. Until the party has beaten Saturos at
/// Mercury Lighthouse (FLAG_SATUROS_BEATEN_AT_MERCURY), the villagers are
/// sick or waiting for Mia. By that time the lighthouse is lit and the
/// fountain at its foot flows again.
///
/// Afterwards both scenes take their cured actor placements and lines, and
/// the villagers talk of Mia leaving with the party. Some lines also change
/// while she travels with them (FLAG_IN_PARTY(MIA)).
///
/// - Imil (SceneId_Imil), the village. A young man shivers and sneezes in the
///   snow, waiting for Mia, and a strongman counts out his exercises against
///   the cold.
///
///   Six doors (step triggers 51 to 56) open into the houses, the shops, the
///   inn and the Sanctum (SceneId_IceRooms). IMIL_TO_WORLD_MAP_SOUTH and
///   IMIL_TO_WORLD_MAP_NORTH lead out to the World Map, and the way north
///   (step trigger 4) leads into Imil Falls Cave. Coming back out of it
///   (IMIL_FROM_IMIL_FALLS_CAVE) the leader walks down into the village.
///
///   Two tasks run while the party is here. One makes smoke rise from the
///   five houses' chimneys, staggered over each second. The other turns the
///   leader's footprints in the snow off on the frozen river that runs down
///   from the falls and on the raised ground under the big tree, and on
///   elsewhere.
///
///   Stepping onto ice (cell behavior 90) slides the leader in the held
///   direction, turning toward it as he goes, until a blocked cell or one too
///   high stops him.
///
///   Move pushes the snowman only when cast from the snow, not from the
///   river's ice or the raised ground. Once it lands at cell (15, 7) it stays
///   there for the rest of the visit (a temporary flag).
///
///   Searching finds 9 coins, a Vial and a Lucky Medal.
///
/// - Ice rooms (SceneId_IceRooms), a sheet of rooms drawn in ice, all of them
///   Imil's: inside the houses, the shops and the inn, whose area name is
///   Imil. By ICE_ROOMS_FROM_IMIL_EAST_NORTHEAST the same scene is the Imil
///   Sanctum.
///
///   While the epidemic lasts the weapon and armor sellers are sick.
///   Searching their counters pans the camera to them, and they say the shop
///   is closed. The item seller's daughter minds that shop for her mother.
///
///   In the Sanctum Mia's two apprentices keep her place. Justin offers
///   healing across the counter, the first time saying he can act as the
///   party's healer (FLAG_IMIL_JUSTIN_OFFERED_HEALING). Megan asks whether
///   the party has visited the old couple who live near the entrance of town,
///   where Mia went (FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE).
///
///   In the old couple's house (ICE_ROOMS_HOUSE_SOUTHWEST) the old man moans
///   for help, and talking to him sends his wife hurrying to fetch Mia.
///   Either sets FLAG_IMIL_MIA_SENT_FOR, which changes the couple's lines and
///   where they stand.
///
///   Mia's healing is this scene's first event. It plays on entering the old
///   couple's house once Megan has told the party where Mia went, before
///   they have met her. The party finds her at the old man's bedside. She
///   heals him with Ply, asks who the party are, and the Mercury Lighthouse
///   flashes. `Was that... the lighthouse?`
///
///   She runs out, and the party follows her (FLAG_IMIL_MET_MIA). At the
///   lighthouse she goes on with them. Afterwards the old man lies in bed
///   feeling much better, and talking or Mind Read across the bed
///   (ACTOR_OLD_MANS_BED) reaches him.
///
///   Once Saturos is beaten the sellers are well and open their shops across
///   the counters.
///
///   The second event is Mia's farewell, the first time the party then
///   enters the Sanctum (FLAG_IMIL_MIA_SAID_FAREWELL). Her apprentices tell
///   her the lighthouse has been lit, and Mia tells them she will join
///   Isaac's quest to protect the world. They promise to protect Imil
///   themselves, as members of the Mercury Clan, and she leaves the village
///   in their hands, Megan and Justin.
///
///   Searching finds an Empty Bottle, a Sleep Bomb and Lucky Pepper. The
///   last comes from the oven with the stew, whose find always answers first,
///   so the stew's line is unused.

#include "types.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "field/sprite.h"
#include "system/input.h"
#include "system/main_routine_work.h"

extern s16 gImil_KeyHeadings[];
extern s16 gImil_SwayHeadings[];
extern s32 gImil_FreezingYoungManScript[];
extern s32 gImil_ShiverScript[];
extern s32 gImil_OldWomanHurryScript[];
extern s32 gImil_LeaderStepAsideScript[];
extern s32 gImil_BobScript[];
extern s32 gImil_SparkScript[];
extern s32 gImil_MiaCheckRoomScript[];
extern s32 gImil_MiaRunOutScript[];
extern s32 gImil_FaceTargetScript[];
extern s32 gImil_FollowMiaOutScript[];
extern struct SceneEntrance gImil_TownEntrances[];
extern struct SceneEntrance gImil_RoomEntrances[];
extern u32 gImil_Exits[];
extern struct ActorPlacement gImil_TownActorPlacements[];
extern struct ActorPlacement gImil_TownActorPlacementsCured[];
extern struct ActorPlacement gImil_RoomActorPlacements[];
extern struct Trigger gImil_TownTriggers[];
extern struct Trigger gImil_RoomTriggers[];
extern u16 gImil_SanctumDoorSteps[];
extern u16 gImil_RoomsEastDoorSteps[];
extern u16 gImil_InnDoorSteps[];
extern u16 gImil_HouseSouthwestDoorSteps[];
extern u16 gImil_HouseWestDoorSteps[];
extern u16 gImil_HouseNortheastDoorSteps[];
extern u16 *gImil_DoorCellSteps[];
extern s16 gImil_DoorCellPoints[][2];
extern u16 gImil_CellStepsA[];
extern u16 gImil_CellStepsB[];
/// The position (x, y, z) the unused arc and sway effects move around.
extern s32 gImil_ArcOrigin[];

#include "scene/walk_into_leader.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/heap_slots.h"
#include "text/msg_ids.h"
#include "field/field_effect.h"
#include "field/field_psynergy_work.h"
#include "field/event_inline.h"
#include "system/iwram_call.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "system/io_reg.h"
#include "field/map_grid_scroll.h"
#include "macros.h"
#include "flags.h"

/// The village's actors (Imil), numbering gImil_TownActorPlacements' rows,
/// and gImil_TownActorPlacementsCured's, from ACTOR_FIRST_PLACED.
enum ImilVillageActorId {
  /// A young man, shivering and sneezing.
  ACTOR_FREEZING_YOUNG_MAN = ACTOR_FIRST_PLACED,
  /// A small boy.
  ACTOR_GREETING_BOY,
  /// A strongman counting his exercises in the snow. "One, two, three, four..."
  ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
  /// Move pushes it.
  ACTOR_SNOWMAN
};

/// The actors of the houses and the Sanctum (IceRooms), numbering
/// gImil_RoomActorPlacements' rows from ACTOR_FIRST_PLACED.
///
/// The rows of Garet, Ivan and Mia take those characters' own slots.
enum IceRoomsActorId {
  /// The old couple's house.
  ACTOR_OLD_WOMAN = ACTOR_FIRST_PLACED,
  ACTOR_OLD_MAN,
  /// An unseen actor on the old man's bed. Once Mia has healed him, talking
  /// or Mind Read here reaches him.
  ACTOR_OLD_MANS_BED,
  /// An old man who shivers while the epidemic lasts.
  ACTOR_SICK_OLD_MAN,
  /// An old man.
  ACTOR_WEAPON_MERCHANT,
  /// An old man.
  ACTOR_ARMOR_MERCHANT,
  /// A little girl who minds the shop while her mother is sick.
  ACTOR_ITEM_MERCHANTS_DAUGHTER,
  /// A woman.
  ACTOR_ITEM_MERCHANT,
  ACTOR_INNKEEPER,
  /// A woman who waits on the inn's guests.
  ACTOR_INN_ATTENDANT,
  /// A young man who keeps the restaurant and offers its menu.
  ACTOR_RESTAURANT_OWNER,
  /// Mia's apprentices, a small boy who keeps the Sanctum and a little girl
  /// ("I will be a kind girl", MsgImilIWillBeAKindGirl).
  ///
  /// Her farewell names them together without saying which is which, so each
  /// takes the English name of the boy's or the girl's gender.
  ACTOR_JUSTIN,
  ACTOR_MEGAN,
  /// Invisible in the old couple's house.
  ///
  /// It is SPRITE_CRATE in SPRITE_COLOR_HIDDEN, every pixel transparent, so
  /// the player sees only the stack of logs in the room's cells. It is
  /// ACTOR_NOT_TARGETED, never a talk or Psynergy target.
  ///
  /// It is kept under the party as altin.c keeps its ground sprites. Vault's
  /// crate (log_rooms_a.c) uses the same sprite.
  ACTOR_INVISIBLE_CRATE
};

/// The village's own temporary flags, which last until the party leaves the
/// map.
enum ImilTownFlag {
  /// Whether the leader leaves footprints in the snow, from where he stands
  /// (Imil_UpdateFootprints). The one set says which he last did.
  FLAG_IMIL_NO_FOOTPRINTS = FLAG_SCENE_TEMP_00,
  FLAG_IMIL_FOOTPRINTS = FLAG_SCENE_TEMP_01,
  /// Move has pushed the snowman to its place, for the rest of the visit.
  FLAG_IMIL_SNOWMAN_MOVED = FLAG_SCENE_TEMP_03
};

/// Imil's shops, by their numbers in the shop table.
enum { SHOP_IMIL_WEAPONS = 10, SHOP_IMIL_ARMOR = 11, SHOP_IMIL_ITEMS = 12 };

/// Imil's inn, as Inn_CheckIn numbers the inns.
enum { IMIL_INN = 4 };

/// The weapon and armor counters' cell behaviors in the houses. Searching
/// one facing north reaches its seller.
enum IceRoomsCellBehavior {
  CELL_BEHAVIOR_ICE_ROOMS_WEAPON_COUNTER = 10,
  CELL_BEHAVIOR_ICE_ROOMS_ARMOR_COUNTER = 11
};

// The leader faces a counter: FACING_IS_NORTH, in the two spellings the sites
// match with, the shops' from the heading read as s16 and the inn's and the
// Sanctum's from the halfword. UB: the shops' form shifts a negative heading
// left.
#ifdef UBFIX
#define IMIL_FACES_COUNTER(facing) FACING_IS_NORTH((u16)(facing))
#else
#define IMIL_FACES_COUNTER(facing)                                             \
  ((u32)(((facing) << 16) + 0x5fff0000) <= 0x3ffe0000)
#endif
#define IMIL_FACES_COUNTER_HALFWORD(facing)                                    \
  ((u16)((facing) - (FACING_NORTHWEST + 1)) <=                                 \
   FACING_NORTHEAST - FACING_NORTHWEST - 2)

/// The item merchant lying in bed, only her head above the covers
/// (SPRITE_WOMAN_IN_GREEN_DRESS, woman_in_green_dress.png).
enum { IMIL_ANIMATION_LIE_IN_BED = 5 };

static void Imil_MeetMiaAtOldCouplesHouse(void);
static void Imil_MiaCastsPlyOnOldMan(void);
static void Imil_SpawnChimneySmoke(void);
static void Imil_UpdateFootprints(void);
static void Imil_PlaceMovedSnowman(void);
static void Imil_ArriveFromFallsCave(void);
static void Imil_MiaSaysFarewellToApprentices(void);
static void Imil_RunIceSlide(void);

/// A sick person shivers: a quick squash, then a pause.
static s32 Imil_UpdateShiver(struct Actor *actor) {
  switch (actor->timer) {
  case 6:
    actor->scaleX -= Q16_ONE / 4;
    actor->scaleY += Q16_ONE / 8;
    break;
  case 4:
    actor->scaleX += Q16_ONE / 8;
    actor->scaleY -= Q16_ONE / 16;
    break;
  case 2:
    actor->scaleX += Q16_ONE / 16;
    actor->scaleY -= Q16_ONE / 32;
    break;
  case 0:
    actor->scaleX += Q16_ONE / 16;
    actor->scaleY -= Q16_ONE / 32;
    if ((s16)actor->userValue != 0) {
      actor->timer = (u32)Random_Next() % 40 + 40;
    } else {
      actor->timer = (u32)Random_Next() % 20 + 20;
    }
    break;
  }
  actor->timer--;
  return 1;
}

#include "shared/actor_facing.c"

/// Imil is two scenes: the village, and its houses and Sanctum.
const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineWork.next == SCENE_ICE_ROOMS) {
    return gImil_RoomEntrances;
  }
  return gImil_TownEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gImil_Exits; }

/// Once Saturos is beaten at Mercury Lighthouse, the villagers are well again.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  struct ActorPlacement *script;

  if (gMainRoutineWork.next == (SCENE_ICE_ROOMS)) {
    script = gImil_RoomActorPlacements;
    Scene_AssignViewFlags(script);
    if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
      // A byte view: written as script[row].talkFacing, the row's address is
      // formed first.
      ((u8 *)script)[(ACTOR_RESTAURANT_OWNER - ACTOR_FIRST_PLACED) *
                         sizeof(struct ActorPlacement) +
                     offsetof(struct ActorPlacement, talkFacing)] =
          TALK_FACING_PARTY;
      script[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED].x = px(182);
      script[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED].z = px(564);
      script[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED].movement =
          MOVEMENT_TYPE_WANDER;
    }
    return script;
  }

  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    return gImil_TownActorPlacementsCured;
  }
  return gImil_TownActorPlacements;
}

/// Stepping onto the ice in Imil (cell behavior 90) starts a slide.
static void Imil_StartIceSlide(void) { Imil_RunIceSlide(); }

/// The village's, or the houses' and the Sanctum's, conversations and
/// Psynergy targets.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_ICE_ROOMS) {
    return gImil_RoomTriggers;
  }
  return gImil_TownTriggers;
}

/// In Imil, until Saturos is beaten at Mercury Lighthouse: the freezing young
/// man sneezes and shivers, waiting for Mia.
static void Imil_TalkToFreezingYoungMan(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Event_SetMessageId(MSGNO(MsgImilEvenIfMiaHealsUs));
  } else {
    Event_SetMessageId(MSGNO(MsgImilBrrrrrChooImFreezingMia));
  }
  {
    s32 val = 0;
    s32 speaker = ACTOR_FREEZING_YOUNG_MAN;
    Event_ShowMessage(speaker, val);
  }
  Event_End();
}

/// Mind Read on the freezing young man in Imil, until Saturos is beaten: Mia
/// should be here by now.
static void Imil_ReadFreezingYoungMansMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Event_SetMessageId(MSGNO(MsgImilCantUnderstandWhyAnyOne));
  } else {
    Event_SetMessageId(MSGNO(MsgImilMiaShouldHereByNow));
  }
  Event_ShowMessage(ACTOR_FREEZING_YOUNG_MAN, 0);
  Event_End();
}

/// A small boy in Imil, until Saturos is beaten, turns to the leader and asks
/// whether he is new in Imil.
static void Imil_GreetVisitor(void) {
  Event_Start();
  Actor_FaceActor(ACTOR_GREETING_BOY, ACTOR_PARTY_LEADER, 10);
  Event_SetMessageId(MSGNO(MsgImilHiNewInImil));
  Event_AskYesNo(ACTOR_GREETING_BOY, 0);
  Event_End();
}

/// The strongman who keeps in shape pauses his exercises to talk.
static void Imil_TalkToStrongmanKeepingInShape(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    Event_SetMessageId(MSGNO(MsgImilCountingAfterDeparture));
    Event_ShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
    Actor_SetAttachedEffect(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ANIM_STAND);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ACTOR_PARTY_LEADER, 20);
    Event_AskYesNo(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
    Actor_FaceDirection(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                        FACING_SOUTH_SOUTHEAST, 10);
    Actor_SetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                       STRONGMAN_ANIMATION_SQUATS);
  } else {
    Event_SetMessageId(MSGNO(MsgImilOneTwoThreeFour));
    Event_ShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
    Actor_SetAttachedEffect(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ANIM_STAND);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
    Actor_FaceDirection(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                        FACING_SOUTH_SOUTHEAST, 10);
    Actor_SetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                       STRONGMAN_ANIMATION_SQUATS);
  }

  Event_End();
}

/// The old man in the old couple's house moans for help, and his wife hurries
/// to fetch Mia (FLAG_IMIL_MIA_SENT_FOR).
static void Imil_TalkToOldMan(void) {
  struct Actor *oldMan;
  struct Actor *oldWoman;

  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    Event_Start();
    Actor_FaceActor(ACTOR_OLD_MAN, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessageId(MsgMercuryReallySayDie);
    // "Did I really say I was going to die from that disease?"
    // Yes: "Ha ha ha! I must have really felt sick to say that! I'm good..."
    // No: "That's right! The strong old man of Imil, that's me!"
    Event_AskYesNo(ACTOR_OLD_MAN, 0);
    Event_End();
  } else {
    if (GameFlag_Get(FLAG_IMIL_MET_MIA) != 0) {
      Event_Start();
      Actor_SetAnimation(ACTOR_OLD_MAN, OLD_MAN_WITH_CANE_ANIMATION_SIT_UP);
      MapGrid_PlayMapEdit(gImil_CellStepsA, 10, 69);
      Event_SetMessageId(MsgMercuryFeelMuchBetter);
      // "I feel much better, thanks to Mia. I should be fine now."
      Event_ShowMessage(ACTOR_OLD_MAN, 0);
      Actor_SetAnimation(ACTOR_OLD_MAN, OLD_MAN_WITH_CANE_ANIMATION_LIE_BACK);
      MapGrid_AnimateCells(gImil_CellStepsB, 10, 69);
      Event_End();
    } else {
      Event_Start();
      oldMan = Actor_Get(ACTOR_OLD_MAN);
      oldMan->timer = 10;
      Actor_SetScript(ACTOR_OLD_MAN, gImil_ShiverScript);
      Event_SetMessageId(MsgMercuryOoohHelpMe);
      // "Oooh... Help me..."
      Event_ShowMessage(ACTOR_OLD_MAN, 0);
      Actor_Stop(ACTOR_OLD_WOMAN);
      Actor_ShowEmote(ACTOR_OLD_WOMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
      Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_NORTH_NORTHEAST, 10);
      Actor_StartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
      // "Grandpa! Grandpa! Hold on!"
      Event_ShowMessageThenWait(ACTOR_OLD_WOMAN, 0, 20);
      Actor_SetScript(ACTOR_PARTY_LEADER, gImil_LeaderStepAsideScript);
      Actor_SetSpeed(ACTOR_OLD_WOMAN, 1.6);
      Actor_RunScriptAndWait(ACTOR_OLD_WOMAN, gImil_OldWomanHurryScript);
      Event_Wait(40);
      Actor_Jump(ACTOR_OLD_WOMAN, 2, 0);
      Actor_StartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
      Actor_SetAttachedEffect(ACTOR_OLD_WOMAN,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(60);
      // "Oh, no! This is terrible!"
      Event_ShowMessageThenWait(ACTOR_OLD_WOMAN, 0, 10);
      Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTH_SOUTHEAST, 20);
      Actor_StartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
      // "Get Mia... Please, get Mia!"
      Event_ShowMessage(ACTOR_OLD_WOMAN, 0);
      oldWoman = Actor_Get(ACTOR_OLD_WOMAN);
      oldWoman->collisionFlags ^= ACTOR_PUSHABLE;
      Import_GameFlagSet(FLAG_IMIL_MIA_SENT_FOR);
      Event_End();
    }
  }
}

/// Talking to the weapon seller of Imil, until Saturos is beaten: he asks
/// whether the party wants weapons.
static void Imil_AskAboutWeapons(void) {
  Event_Start();
  Event_SetMessageId(MSGNO(MsgImilDoWantWeapons));
  Event_AskYesNo(ACTOR_WEAPON_MERCHANT, 0);
  Event_End();
}

/// The restaurant owner of Imil, a young man, until Saturos is beaten, offers
/// the menu.
static void Imil_AskAboutRestaurantMenu(void) {
  Event_Start();
  Event_SetMessageId(MSGNO(MsgImilDoWantSeeRestaurantMenu));
  Event_AskYesNo(ACTOR_RESTAURANT_OWNER, 0);
  Event_End();
}

/// Megan asks whether the party has visited the old couple.
static void Imil_TalkToMegan(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Event_SetMessageId(MSGNO(MsgImilEveryoneCountsOnMiaThats));
    Event_ShowMessage(ACTOR_MEGAN, 0);
  } else {
    Event_SetMessageId(MsgImilHaveVisitedOldCoupleWho);
    // "Have you visited the old couple who live near the entrance..."
    // Yes: "Oh. That's too bad. That's where Mia was headed. You..."
    // No: "Mia went to their house. You might have seen her if you..."
    Event_AskYesNo(ACTOR_MEGAN, 0);
    Import_GameFlagSet(FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE);
    Import_GameFlagSet(FLAG_IMIL_MIA_SENT_FOR);
  }
  Event_End();
}

/// Mind Read on Megan in the Imil Sanctum, until Saturos is beaten: Mia had a
/// bad feeling.
static void Imil_ReadMegansMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Event_SetMessageId(MSGNO(MsgImilWeHaveDoWhateverWe));
  } else {
    Event_SetMessageId(MSGNO(MsgImilMiaWasSayingSheHas));
  }
  Event_ShowMessage(ACTOR_MEGAN, 0);
  Event_End();
}

/// The old woman of the old couple asks what happened at the lighthouse.
static void Imil_TalkToOldWoman(void) {
  Event_Start();
  Event_SetMessageId(MSGNO(MsgImilHappenedInLighthouseNortheast));
  Event_ShowMessage(ACTOR_OLD_WOMAN, 0);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTH_SOUTHEAST, 10);
  Event_End();
}

/// Mind Read on the old woman of the old couple's house, until Saturos is
/// beaten: about Mia's clan, her husband's exaggerating, or Mia running all
/// over town.
static void Imil_ReadOldWomanMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_IMIL_MET_MIA) != 0) {
    Event_SetMessageId(MSGNO(MsgImilMiaClanOnceLivedHere));
  } else if (GameFlag_Get(FLAG_IMIL_MIA_SENT_FOR) != 0) {
    Event_SetMessageId(MSGNO(MsgImilHesAlwaysExaggeratingThingsBut));
  } else {
    Event_SetMessageId(MSGNO(MsgImilMiaRunningAroundTownCaring));
  }
  Event_ShowMessage(ACTOR_OLD_WOMAN, 0);
  Event_End();
}

/// After Mia's visit, speaking across the bed in the old couple's house
/// reaches the old man.
static void Imil_TalkToOldManInBed(void) { Imil_TalkToOldMan(); }

/// After Mia's visit, Mind Read across the bed in the old couple's house
/// reaches the old man.
static void Imil_ReadOldManInBedMind(void) {
  Event_Start();
  Event_SetMessageId(MSGNO(MsgImilMiaIsAGoodGirlI));
  Event_ShowMessage(ACTOR_OLD_MANS_BED, 0);
  Event_End();
}

/// Mind Read on Justin in the Imil Sanctum, until Saturos is beaten: he
/// thinks the party are not bad people.
static void Imil_ReadJustinsMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Event_SetMessageId(MSGNO(MsgImilFeelLikeGrownUpWhen));
  } else {
    Event_SetMessageId(MSGNO(MsgImilTheseFolkOkayTheyDont));
  }
  Event_ShowMessage(ACTOR_JUSTIN, 0);
  Event_End();
}

/// Walking north into one of Imil's six doors (step triggers 51 to 56):
/// everyone stops, the door opens and the leader walks in.
static void Imil_OpenDoorAndExit(void) {
  struct FieldWork *work;
  struct Actor *actor;
  u32 i;
  s32 exit;

  work = gFieldWork;
  Event_Start();
  for (i = ACTOR_FIRST_PLACED; i <= 65; i++) {
    actor = Actor_Get(i);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  exit = (s16)(work->stepTrigger - 50);
  if (exit == IMIL_TO_ICE_ROOMS_NORTHEAST) {
    Sound_Play(SE_THUMP);
  } else {
    Sound_Play(SE_DOOR_OPEN);
  }
  {
    s32 x = gImil_DoorCellPoints[exit - 1][0];
    s32 y = gImil_DoorCellPoints[exit - 1][1];

    MapGrid_PlayMapEdit(gImil_DoorCellSteps[exit - 1], x, y);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (exit == IMIL_TO_ICE_ROOMS_NORTHEAST) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  } else {
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  Event_Wait(16);
  Import_EventSetExitRequest(exit);
  Event_End();
}

/// Imil's weapon shop.
static void Imil_TalkToWeaponMerchant(void) {
  s32 facing;

  facing = (s16)Actor_Get(ACTOR_PARTY_LEADER)->facing;
  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    if (IMIL_FACES_COUNTER(facing)) {
      Shop_Run(SHOP_IMIL_WEAPONS, ACTOR_WEAPON_MERCHANT);
      return;
    }
    Event_Start();
    Actor_FaceActor(ACTOR_WEAPON_MERCHANT, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessageId(MsgMercuryCatchUpLostOpportunity);
    // "I have to catch up on lost opportunity! It's time for business!"
    Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    Actor_FaceDirection(ACTOR_WEAPON_MERCHANT, FACING_SOUTH, 10);
    Event_End();
  } else {
    if (IMIL_FACES_COUNTER(facing)) {
      Event_Start();
      FieldCamera_SetSpeed(6);
      FieldCamera_MoveTo(426, CAMERA_KEEP, 492, 1);
      FieldCamera_WaitForMove();
      Event_Wait(20);
      Actor_RunScriptAndWait(ACTOR_WEAPON_MERCHANT, gImil_BobScript);
      Event_SetMessageId(MsgMercuryStoreClosedUntilWell);
      // "Our store is closed until we get well..."
      Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
      FieldCamera_MoveTo(426, CAMERA_KEEP, 616, 1);
      FieldCamera_WaitForMove();
      Event_End();
    }
  }
}

/// Imil's armor shop, as the weapon shop.
static void Imil_TalkToArmorMerchant(void) {
  s32 facing;

  facing = (s16)Actor_Get(ACTOR_PARTY_LEADER)->facing;
  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    if (IMIL_FACES_COUNTER(facing)) {
      Shop_Run(SHOP_IMIL_ARMOR, ACTOR_ARMOR_MERCHANT);
      return;
    }
    Event_Start();
    Actor_FaceActor(ACTOR_ARMOR_MERCHANT, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessageId(MsgMercuryShameStoreClosed);
    // "It's a shame the store was closed for something as minor as..."
    Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    Actor_FaceDirection(ACTOR_ARMOR_MERCHANT, FACING_SOUTH, 10);
    Event_End();
  } else {
    if (IMIL_FACES_COUNTER(facing)) {
      Event_Start();
      FieldCamera_SetSpeed(6);
      FieldCamera_MoveTo(426, CAMERA_KEEP, 492, 1);
      FieldCamera_WaitForMove();
      Event_Wait(20);
      Actor_RunScriptAndWait(ACTOR_ARMOR_MERCHANT, gImil_BobScript);
      Event_SetMessageId(MsgMercuryTooSickBusiness);
      // "Sorry, but I'm too sick to do business today. Try again..."
      Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
      FieldCamera_MoveTo(426, CAMERA_KEEP, 616, 1);
      FieldCamera_WaitForMove();
      Event_End();
    }
  }
}

/// Imil's item shop.
static void Imil_TalkToItemMerchant(void) {
  s32 facing;

  facing = (s16)Actor_Get(ACTOR_PARTY_LEADER)->facing;
  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    if (IMIL_FACES_COUNTER(facing)) {
      Shop_Run(SHOP_IMIL_ITEMS, ACTOR_ITEM_MERCHANT);
      return;
    }
    Event_Start();
    Actor_FaceActor(ACTOR_ITEM_MERCHANT, ACTOR_PARTY_LEADER, 0);
    Event_SetMessageId(MsgMercuryRecoveredThanksFountain);
    // "I'm fully recovered, thanks to the fountain! I can get back..."
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Actor_FaceDirection(ACTOR_ITEM_MERCHANT, FACING_SOUTH, 0);
    Event_End();
  } else {
    if (IMIL_FACES_COUNTER(facing)) {
      Event_Start();
      Event_SetMessageId(MsgMercuryMomsSickGetWhatever);
      // "Mom's sick, but I can get you whatever you need..."
      Event_ShowMessage(ACTOR_ITEM_MERCHANTS_DAUGHTER, 0);
      Shop_Run(SHOP_IMIL_ITEMS, ACTOR_ITEM_MERCHANTS_DAUGHTER);
      Event_End();
    } else {
      Actor_FaceActor(ACTOR_ITEM_MERCHANTS_DAUGHTER, ACTOR_PARTY_LEADER, 10);
      Event_SetMessageId(MsgMercuryDadLumberjack);
      // "My dad is a lumberjack. He's down south working in Kolima."
      Event_ShowMessage(ACTOR_ITEM_MERCHANTS_DAUGHTER, 0);
      Actor_FaceDirection(ACTOR_ITEM_MERCHANTS_DAUGHTER, FACING_SOUTH_SOUTHWEST,
                          10);
    }
  }
}

/// Across the counter of Imil's inn, the leader facing north, the inn opens.
static void Imil_TalkToInnkeeper(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (IMIL_FACES_COUNTER_HALFWORD(leader->facing)) {
    Inn_CheckIn(IMIL_INN, ACTOR_INNKEEPER);
  } else {
    Event_Start();
    Actor_FaceActor(ACTOR_INNKEEPER, ACTOR_PARTY_LEADER, 10);
    if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
      Event_SetMessageId(MSGNO(MsgImilItsAlmostTimeForLeave));
      Event_AskYesNo(ACTOR_INNKEEPER, 0);
    } else {
      Event_SetMessageId(MSGNO(MsgImilWhyHaveTwoGroupsTravelers));
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    }
    Actor_FaceDirection(ACTOR_INNKEEPER, FACING_SOUTH_SOUTHEAST, 10);
    Event_End();
  }
}

/// Justin offers the Sanctum's services across its counter.
static void Imil_TalkToJustin(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (IMIL_FACES_COUNTER_HALFWORD(leader->facing)) {
    Event_Start();
    if (Event_GetFlag(FLAG_IMIL_JUSTIN_OFFERED_HEALING) == 0) {
      Event_SetMessageId(MSGNO(MsgImilMayOnlyStudentButCan));
      Event_ShowMessage(ACTOR_JUSTIN, 0);
      Import_GameFlagSet(FLAG_IMIL_JUSTIN_OFFERED_HEALING);
    }
    Event_End();
    Sanctum_Run(ACTOR_JUSTIN);
  } else {
    Event_Start();
    if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
      Event_SetMessageId(MSGNO(MsgImilMiaGoingOnJourneyWith));
      Event_ShowMessage(ACTOR_JUSTIN, 0);
    } else if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
      Event_SetMessageId(MSGNO(MsgImilAmHealerWhileMiaOut));
      Event_ShowMessage(ACTOR_JUSTIN, 0);
    } else {
      Event_SetMessageId(MSGNO(MsgImilLookingForMia));
      (void)Event_AskYesNo(ACTOR_JUSTIN, 0);
      Actor_FaceDirection(ACTOR_JUSTIN, FACING_SOUTH_SOUTHEAST, 10);
    }
    Event_End();
  }
}

/// Sets up the scene as the party enters.
s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 entrance;

  if (gMainRoutineWork.next == SCENE_IMIL) {
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                       STRONGMAN_ANIMATION_SQUATS);
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      GameFlag_Clear(FLAG_IMIL_NO_FOOTPRINTS);
      GameFlag_Clear(FLAG_IMIL_FOOTPRINTS);
    }
    leader->timer = 0;
    leader->userValue = 0;
    Task_AddFrame(Imil_SpawnChimneySmoke, TASK_PASS_FRAME);
    Task_AddFrame(Imil_UpdateFootprints, TASK_PASS_FRAME);
    Actor_SetSpritePriority(ACTOR_SNOWMAN, 1);
    if (GameFlag_Get(FLAG_IMIL_SNOWMAN_MOVED)) {
      Imil_PlaceMovedSnowman();
    }
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == IMIL_FROM_IMIL_FALLS_CAVE) {
      Imil_ArriveFromFallsCave();
    }
  } else if (gMainRoutineWork.next == SCENE_ICE_ROOMS) {
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    entrance = gMainRoutineWork.entrance;
    if (entrance == ICE_ROOMS_HOUSE_SOUTHWEST) {
      Actor_SetSpriteColorById(ACTOR_INVISIBLE_CRATE, SPRITE_COLOR_HIDDEN);
      Actor_Get(ACTOR_INVISIBLE_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
      Actor_SetSpritePriority(ACTOR_INVISIBLE_CRATE, 1);
      if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        MapGrid_CopyCellAttributes(10, 7, 1, 1, 10, 8);
        MapGrid_CopyCellsTo(3, 125, 9, 69, 3, 3);
        MapGrid_CenterOnFollowedPoint();
        wait(1);
        Actor_SetMovementTypeById(ACTOR_OLD_WOMAN, 2);
        Actor_SetPosition(ACTOR_OLD_MANS_BED, 0, 0);
      } else if (GameFlag_Get(FLAG_IMIL_MIA_SENT_FOR) &&
                 GameFlag_Get(FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE)) {
        Actor_SetSpriteFlags(Actor_Get(ACTOR_OLD_MANS_BED), 0);
        Actor_SetPosition(ACTOR_OLD_MAN, 174, 164);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_OLD_MAN), 0);
        Actor_SetAnimation(ACTOR_OLD_MAN,
                           OLD_MAN_WITH_CANE_ANIMATION_LIE_IN_BED);
        Actor_SetPosition(ACTOR_OLD_WOMAN, 168, 152);
        Actor_Get(ACTOR_OLD_WOMAN)->facing = FACING_SOUTH_SOUTHEAST;
        if (!GameFlag_Get(FLAG_IMIL_MET_MIA)) {
          Imil_MeetMiaAtOldCouplesHouse();
        }
      } else {
        MapGrid_CopyCellAttributes(10, 7, 1, 1, 10, 8);
        MapGrid_CopyCellsTo(3, 125, 9, 69, 3, 3);
        MapGrid_CenterOnFollowedPoint();
        wait(1);
        if (GameFlag_Get(FLAG_IMIL_MIA_SENT_FOR)) {
          Actor_SetPosition(ACTOR_OLD_WOMAN, 149, 116);
          Actor_Get(ACTOR_OLD_WOMAN)->facing = FACING_EAST;
          Actor_Get(ACTOR_OLD_MAN)->userValue = 0;
          Actor_SetScript(ACTOR_OLD_MAN, gImil_ShiverScript);
        } else {
          Actor_SetMovementTypeById(ACTOR_OLD_WOMAN, 2);
        }
      }
    } else if (entrance == ICE_ROOMS_FROM_IMIL_WEST_SOUTHWEST) {
      if (!GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Actor_Get(ACTOR_SICK_OLD_MAN)->userValue = 1;
        Actor_SetScript(ACTOR_SICK_OLD_MAN, gImil_ShiverScript);
      }
    } else if (entrance == ICE_ROOMS_HOUSE_NORTHEAST) {
      if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Actor_SetPosition(ACTOR_WEAPON_MERCHANT, 364, 578);
        Actor_SetSpritePriority(ACTOR_WEAPON_MERCHANT, 2);
        Actor_Get(ACTOR_WEAPON_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
        MapGrid_CopyCellsTo(6, 125, 22, 88, 3, 3);
        Actor_SetPosition(ACTOR_ARMOR_MERCHANT, 492, 578);
        Actor_SetSpritePriority(ACTOR_ARMOR_MERCHANT, 2);
        Actor_Get(ACTOR_ARMOR_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
        MapGrid_CopyCellsTo(9, 125, 28, 88, 3, 3);
      } else {
        Actor_Get(ACTOR_WEAPON_MERCHANT)->scaleX = -Q16_ONE;
        Actor_SetSpriteFlags(Actor_Get(ACTOR_WEAPON_MERCHANT), 0);
        Actor_SetAnimation(ACTOR_WEAPON_MERCHANT,
                           OLD_MAN_WITH_CANE_ANIMATION_LIE_IN_BED);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_ARMOR_MERCHANT), 0);
        Actor_SetAnimation(ACTOR_ARMOR_MERCHANT,
                           OLD_MAN_WITH_CANE_ANIMATION_LIE_IN_BED);
      }
    } else if (entrance == ICE_ROOMS_HOUSE_WEST) {
      if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Actor_SetPosition(ACTOR_ITEM_MERCHANT, 460, 258);
        Actor_SetSpritePriority(ACTOR_ITEM_MERCHANT, 2);
        Actor_Get(ACTOR_ITEM_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
        Actor_SetPosition(ACTOR_ITEM_MERCHANTS_DAUGHTER, 408, 264);
        Actor_Get(ACTOR_ITEM_MERCHANTS_DAUGHTER)->facing =
            FACING_EAST_SOUTHEAST;
        MapGrid_CopyCellsTo(12, 125, 26, 70, 3, 3);
      } else {
        Actor_SetPosition(ACTOR_ITEM_MERCHANTS_DAUGHTER, 460, 258);
        Actor_SetSpritePriority(ACTOR_ITEM_MERCHANTS_DAUGHTER, 2);
        Actor_Get(ACTOR_ITEM_MERCHANTS_DAUGHTER)->collisionFlags |=
            ACTOR_NEAR_TALK;
        Actor_Get(ACTOR_ITEM_MERCHANT)->scaleX = -Q16_ONE;
        Actor_SetSpriteFlags(Actor_Get(ACTOR_ITEM_MERCHANT), 0);
        Actor_SetAnimation(ACTOR_ITEM_MERCHANT, IMIL_ANIMATION_LIE_IN_BED);
      }
    } else if (entrance == ICE_ROOMS_FROM_IMIL_EAST_NORTHEAST) {
      if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Actor_Get(ACTOR_MEGAN)->facing = FACING_SOUTH_SOUTHEAST;
        if (!GameFlag_Get(FLAG_IMIL_MIA_SAID_FAREWELL)) {
          Actor_SetPosition(ACTOR_MEGAN, 650, 161);
          Imil_MiaSaysFarewellToApprentices();
        } else {
          Actor_SetPosition(ACTOR_MEGAN, 644, 166);
        }
      }
    }
  }
  return 0;
}

/// Mia's healing, in the old couple's house in Imil: after hearing where Mia
/// went, the party finds her at the old man's bedside.
static void Imil_MeetMiaAtOldCouplesHouse(void) {
  const s32 *table;
  struct FieldWork *work;

  Event_Start();
  Actor_SetPosition(ACTOR_MIA, 182, 150);
  FieldCamera_MoveTo(141, CAMERA_KEEP, 221, 0);
  wait(1);
  FieldCamera_SetSpeed(0.3);
  FieldCamera_MoveTo(140, CAMERA_KEEP, 164, 1);
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 40;
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
  Actor_SetSpeed(ACTOR_GARET, 0.4);
  Actor_SetSpeed(ACTOR_IVAN, 0.4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Scene_PlaceAtLeader(ACTOR_GARET);
  Scene_PlaceAtLeader(ACTOR_IVAN);
  Actor_WalkTo(ACTOR_GARET, 150, 234);
  Actor_WalkToAndWait(ACTOR_IVAN, 134, 234);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  FieldActor_WatchAndStartScript(ACTOR_PARTY_LEADER,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_MIA,
                                 gImil_FaceTargetScript);
  FieldActor_WatchAndStartScript(
      ACTOR_GARET, ACTOR_WATCH_KEEP_MOTION | ACTOR_MIA, gImil_FaceTargetScript);
  FieldActor_WatchAndStartScript(
      ACTOR_IVAN, ACTOR_WATCH_KEEP_MOTION | ACTOR_MIA, gImil_FaceTargetScript);
  FieldCamera_WaitForMove();
  table = gImil_BobScript;
  Actor_SetScript(ACTOR_OLD_MAN, table);
  Event_Wait(40);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  // Mia checks on the old man while the party enters the house.
  Event_SetMessageId(MSGNO(MsgImilHowFeeling));
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetScript(ACTOR_OLD_MAN, table);
  Event_ShowMessageThenWait(ACTOR_OLD_MAN, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_EAST, 10);
  Actor_SetAnimationAndWait(ACTOR_OLD_WOMAN, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_OLD_WOMAN, 0, 40);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTH_SOUTHEAST, 20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 10);
  Actor_SetScript(ACTOR_OLD_MAN, table);

  Imil_MiaCastsPlyOnOldMan();
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  Event_ShowMessageThenWait(ACTOR_OLD_MAN, 0, 20);
  Actor_ShowEmote(ACTOR_OLD_WOMAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimation(ACTOR_OLD_MAN, OLD_MAN_WITH_CANE_ANIMATION_SIT_UP);
  MapGrid_AnimateCells(gImil_CellStepsA, 10, 69);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_OLD_MAN, 1);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_OLD_MAN, OLD_MAN_WITH_CANE_ANIMATION_LIE_BACK);
  MapGrid_AnimateCells(gImil_CellStepsB, 10, 69);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_OLD_WOMAN, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_OLD_WOMAN, 0, 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 10);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_OLD_WOMAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_Get(ACTOR_MIA)->timer = 0;
  Actor_SetScript(ACTOR_MIA, gImil_MiaCheckRoomScript);
  while (Actor_Get(ACTOR_MIA)->timer == 0) {
    wait(1);
  }
  FieldCamera_MoveTo(140, CAMERA_KEEP, 198, 1);
  Actor_WaitForScriptEndById(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(10);
  Event_ShowMessage(ACTOR_MIA, 0);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  // The lighthouse interrupts the visit; Mia leaves and the party follows.
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 20, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(10);
  wait(1);
  Sound_Play(SE_SHIMMER);
  wait(40);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  wait(60);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 10);
  Actor_SetSpeed(ACTOR_MIA, 2);
  Actor_WalkToAndWait(ACTOR_MIA, 202, 198);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_IVAN);
  Actor_SetSpeed(ACTOR_MIA, 3);
  Actor_Get(ACTOR_MIA)->timer = 0;
  Actor_SetScript(ACTOR_MIA, gImil_MiaRunOutScript);
  while (Actor_Get(ACTOR_MIA)->timer == 0) {
    wait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 4);
  Actor_SetSpeed(ACTOR_GARET, 4);
  Actor_SetSpeed(ACTOR_IVAN, 4);
  Sound_Play(SE_JUMP);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 132, 206);
  Actor_SetDestination(ACTOR_GARET, 136, 221);
  Actor_SetDestination(ACTOR_IVAN, 122, 238);
  Actor_WaitForScriptEndById(ACTOR_MIA);
  Event_Wait(80);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_IVAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  table = gImil_FollowMiaOutScript;
  Actor_SetScript(ACTOR_GARET, table);
  Actor_RunScriptAndWait(ACTOR_IVAN, table);
  Event_Wait(20);
  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  work->transitionFrames = 24;
  Import_GameFlagSet(FLAG_IMIL_MET_MIA);
  Event_End();
}

/// Unused: an update routine that moves an effect around its stored origin
/// until its timer runs out. No actor is given it.
UNUSED static void Imil_UpdateCountdownArc(struct Actor *actor) {
  // XXX (no evidence): what this and Imil_SwayAndSpark were for; no
  // actor is given either, so nothing in the game shows them in use.
  s32 buf[3];
  s32 *b;
  s32 n;
  s32 t;

  if (actor != NULL) {
    n = (u16)actor->timer - 1;
    actor->timer = n;
    t = (s16)n;
    if (t != 0) {
      b = buf;
      b[0] = gImil_ArcOrigin[0];
      b[1] = gImil_ArcOrigin[1] + px(8);
      b[2] = gImil_ArcOrigin[2];
      Vector_AddPolarOffset(px(t), (t << 11) + (s16)actor->userValue, b);
      actor->x.fixed = b[0];
      actor->y.fixed = b[1];
      actor->z.fixed = b[2];
    } else {
      Actor_Release(actor);
    }
  }
}

/// Unused: an update routine that sways an actor and now and then gives
/// off a rising spark. No actor is given it.
UNUSED static void Imil_SwayAndSpark(struct Actor *actor) {
  s32 pos[3];
  struct Actor *spark;
  s32 radius;

  actor->x.fixed =
      gImil_ArcOrigin[0] + Iwram_MulQ16(px(6), Trig_Sin(actor->timer << 10));
  actor->timer++;
  actor->timer = (actor->timer + 64) % 64;
  if (gFrameCount % 3 == 0) {
    SET_VECTOR(pos, actor->x.fixed, actor->y.fixed + px(2), actor->z.fixed);
    radius = Random_Next();
    Vector_AddPolarOffset(radius * 6, Random_Next(), pos);
    spark = FieldActor_Create(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
    if (spark != NULL) {
      spark->sprite->image.priority = 0;
      Actor_SetSpriteFlags(spark, 0);
      Actor_SelectAnimation(spark, 1);
      spark->scaleX = 3 * Q16_ONE / 5;
      spark->scaleY = 3 * Q16_ONE / 5;
      spark->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      spark->motionFlags = 0;
      Actor_SetSpriteColor(spark, SPRITE_COLOR_BLUE_TINT);
      Actor_StartScript(spark, gImil_SparkScript);
    }
  }
}

/// The old man bobs and speaks as Ply ends (its finishCallback).
static void Imil_OldManBobsAndSpeaks(void) {
  Actor_SetScript(ACTOR_OLD_MAN, gImil_BobScript);
  Event_ShowMessage(ACTOR_OLD_MAN, 0);
}

static void Imil_MiaCastsPlyOnOldMan(void) {
  struct FieldEffectWork *effectWork;

  FieldPsynergy_LoadResources(ABILITY_PLY, 1);
  effectWork = gFieldEffectWork;
  FieldPsynergy_SetupActorPair(ACTOR_MIA, ACTOR_OLD_MAN);
  effectWork->finishCallback = Imil_OldManBobsAndSpeaks;
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  FieldPsynergy_StopCasterUpdate();
  FieldPsynergy_End();
}

/// Resets a rising effect's motion, sprite priority, and initial scale.
static void Imil_InitializeRisingEffect(struct Actor *work) {
  struct Sprite *sprite;

  work->motionFlags = 0;
  work->timer = 0;
  work->priorityFlags &= ~1;

  sprite = work->sprite;
  sprite->image.priority = 1;

  Actor_SetSpriteColor(work, SPRITE_COLOR_BLUE_TINT);
  Actor_SetSpriteFlags(work, 0);

  work->scaleX = Q16_ONE / 2;
  work->scaleY = Q16_ONE / 2;
}

/// Moves and enlarges an ambient effect, then releases it when its timer
/// expires.
static void Imil_UpdateRisingEffect(struct Actor *actor) {
  struct FieldEffect *effect = (struct FieldEffect *)actor;

  effect->x += (s16)effect->spin << 8;
  effect->y += px(1) / 2;
  effect->scaleX += 3 * Q16_ONE / 100;
  effect->scaleY += 3 * Q16_ONE / 100;
  effect->spin += 2;
  if (--effect->countdown == 0) {
    Actor_Release(actor);
  }
}

/// Initializes a newly created ambient effect for its one-second rise.
static inline void Imil_StartRisingEffect(struct Actor *actor) {
  if (actor != NULL) {
    Imil_InitializeRisingEffect(actor);
    ((struct FieldEffect *)actor)->countdown = 60;
    ((struct FieldEffect *)actor)->update = Imil_UpdateRisingEffect;
    Actor_SelectAnimation(actor, 5);
  }
}

/// Smoke rises from the chimneys of Imil's five houses, staggered over each
/// second.
static void Imil_SpawnChimneySmoke(void) {
  struct Actor *actor;

  if (gFrameCount % 60 == 0) {
    actor = (struct Actor *)FieldActor_Create(SPRITE_DUST_PUFF, px(463), 0,
                                              px(292));
    Imil_StartRisingEffect(actor);
  }
  if ((gFrameCount + 30) % 60 == 0) {
    actor = (struct Actor *)FieldActor_Create(SPRITE_DUST_PUFF, px(320), px(32),
                                              px(356));
    Imil_StartRisingEffect(actor);
  }
  if ((gFrameCount + 10) % 60 == 0) {
    actor =
        (struct Actor *)FieldActor_Create(SPRITE_DUST_PUFF, px(118), 0, px(70));
    Imil_StartRisingEffect(actor);
  }
  if ((gFrameCount + 50) % 60 == 0) {
    actor = (struct Actor *)FieldActor_Create(SPRITE_DUST_PUFF, px(342), 0,
                                              px(124));
    Imil_StartRisingEffect(actor);
  }
  if ((gFrameCount + 80) % 60 == 0) {
    actor = (struct Actor *)FieldActor_Create(SPRITE_DUST_PUFF, px(431), 0,
                                              px(171));
    Imil_StartRisingEffect(actor);
  }
}

/// Turns the leader's footprints off in two stretches of Imil and back on
/// elsewhere, each frame while the party is in the village.
static void Imil_UpdateFootprints(void) {
  struct Actor *actor;
  s32 x;
  s32 column;
  s32 isSet;
  s32 zoneFlag = FLAG_IMIL_NO_FOOTPRINTS;
  s32 otherZoneFlag = FLAG_IMIL_FOOTPRINTS;
  struct MapGridScrollWork *grid;

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  x = actor->x.fixed;
  // In units of 8 pixels.
  column = x >> 19;
  if ((column >= 24 && column <= 31) ||
      ((u32)((actor->z.fixed >> 19) - 36) <= 9 && (u32)(column - 22) <= 9)) {
    isSet = GameFlag_Get(zoneFlag);
    if (isSet != 0)
      return;
    ((struct MapGridScrollWork *)gMapGridWork[0])->footprints = isSet;
    Import_GameFlagSet(zoneFlag);
    GameFlag_Clear(otherZoneFlag);
    return;
  }
  if (x > px(232) && actor->y.fixed > px(30) && actor->z.fixed > px(212)) {
    grid = gMapGridWork[0];
    grid->footprints = 0;
    Import_GameFlagSet(zoneFlag);
    GameFlag_Clear(otherZoneFlag);
    return;
  }
  isSet = GameFlag_Get(otherZoneFlag);
  if (isSet != 0)
    return;
  grid = gMapGridWork[0];
  grid->footprints = 1;
  Import_GameFlagSet(otherZoneFlag);
  GameFlag_Clear(zoneFlag);
  return;
}

/// Move on Imil's snowman, while the leader leaves footprints: once it is
/// at cell (15, 7) it stays there for the rest of the visit.
static void Imil_PlaceMovedSnowman(void) {
  s32 a;
  s32 b;

#if TBS_PATCH_3
  struct Actor *actor = Actor_Get(ACTOR_SNOWMAN);

  a = FIELD_CELL(actor->x.fixed);
  if (a != 15)
    return;
  b = FIELD_CELL(actor->z.fixed);
  if (b != 7)
    return;
  Import_GameFlagSet(FLAG_IMIL_SNOWMAN_MOVED);
  Actor_SetSpritePriority(ACTOR_SNOWMAN, 3);
#else
  Import_GameFlagSet(FLAG_IMIL_SNOWMAN_MOVED);
  Actor_SetSpritePriority(ACTOR_SNOWMAN, 3);
  a = 15;
  b = 7;
#endif
  MapGrid_CopyCellAttributes(15, 6, 1, 1, a, b);
}

/// Walking into the way north out of Imil (step trigger 4): the leader faces
/// north and goes into Imil Falls Cave (IMIL_TO_IMIL_FALLS_CAVE).
static void Imil_LeaveNorth(void) {
  struct Actor *work;

  Event_Start();
  work = Actor_Get(ACTOR_PARTY_LEADER);
  work->facing = FACING_NORTH;
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(IMIL_TO_IMIL_FALLS_CAVE);
}

/// Coming back out of Imil Falls Cave (IMIL_FROM_IMIL_FALLS_CAVE), the leader
/// walks down into the village.
static void Imil_ArriveFromFallsCave(void) {
  Event_Start();
  Event_StartScreenTransition();
  Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 0.1);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 232, 204);
  Event_End();
}

/// Mia's farewell, in the Imil Sanctum the first time the party comes once
/// Saturos is beaten: she says farewell to her two apprentices, Megan and
/// Justin, before leaving with the party.
static void Imil_MiaSaysFarewellToApprentices(void) {
  struct FieldWork *work;

  Event_Start();
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 690, 200);

  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(690, 0, 164, 1);
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 48;
  Event_StartScreenTransition();
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetSpeed(ACTOR_MIA, 0.6);
  Scene_PlaceAtLeader(ACTOR_MIA);
  Actor_WalkToAndWait(ACTOR_MIA, 673, 183);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_StartRepeatedMotion(ACTOR_JUSTIN, 2);
  Actor_RunRepeatedMotion(ACTOR_MEGAN, 2);
  Event_Wait(40);
  Event_SetMessageId(MSGNO(MsgImilMia));
  Event_ShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Actor_SetAttachedEffect(ACTOR_MEGAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MEGAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_JUSTIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST_NORTHEAST, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 40);
  Actor_RunRepeatedMotion(ACTOR_MEGAN, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_JUSTIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MEGAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_JUSTIN, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 80);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MEGAN, FACING_EAST_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JUSTIN, FACING_WEST_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_JUSTIN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MEGAN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetSpeed(ACTOR_MEGAN, 1);
  Actor_Get(ACTOR_MEGAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MEGAN, 656, 166);
  Event_Wait(1);
  Actor_Get(ACTOR_MEGAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 40);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_JUSTIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MEGAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_JUSTIN, 1);
  Event_ShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MEGAN, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 80);
  Actor_ShowEmote(ACTOR_JUSTIN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MEGAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimationAndWait(ACTOR_JUSTIN, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
  Actor_SetAnimation(ACTOR_MEGAN, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 40);
  Actor_SetAnimationAndWait(ACTOR_JUSTIN, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_MEGAN, ANIM_NOD);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 60);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 40);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 10);
  Actor_SetAnimation(ACTOR_JUSTIN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MEGAN, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_MIA, 688, 200);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_Get(ACTOR_MEGAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MEGAN, 644, 166);
  Event_Wait(1);
  /* FAKEMATCH(temporary): the flags set through a byte pointer to the actor;
   * as Actor_Get(ACTOR_MEGAN)->steeringFlags |=, 2 of 506 instructions differ
   * in tbs-en. */
  *((u8 *)Actor_Get(ACTOR_MEGAN) + offsetof(struct Actor, steeringFlags)) |=
      ACTOR_FACES_MOTION;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Import_GameFlagSet(FLAG_IMIL_MIA_SAID_FAREWELL);
  GameFlag_Clear(FLAG_IMIL_JUSTIN_OFFERED_HEALING);
  Event_End();
}

/// While sliding on Imil's ice, the leader turns toward the held direction.
static void Imil_TurnSlidingActor(struct Actor *updated) {
  struct Actor *actor;
  s32 z;
  s32 t;
  s32 d;
  u16 prev;
  s32 n;

  actor = updated;
  n = (u16)actor->timer;
  z = 0;
  t = ((s16 *)&actor->timer)[z];
  if (t != 0) {
    actor->timer = n - 1;
    return;
  }
  actor->steeringFlags = t;
  z = 1;
  d = gImil_KeyHeadings[(gKeysHeld >> 4) & 0xF];
  z = -z;
  if (d == z) {
    Actor_SelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
    return;
  }
  prev = actor->facing;
  d = (s16)(d - prev);
  if (d > FACING_STEP)
    d = FACING_STEP;
  if (d < -FACING_STEP)
    d = -FACING_STEP;
  actor->facing = prev + d;
  Actor_SelectAnimation(actor, ANIM_WALK);
  Actor_SetAnimationSpeed(actor, 48);
}

/// The leader slides along Imil's ice until something stops him.
static void Imil_RunIceSlide(void) {
  struct Actor *subject;
  struct FieldPosition probe;
  s32 heading;
  s32 startBehavior;
  s32 nextBehavior;
  s32 z;
  s32 x;
  u8 *layer;

  subject = Actor_Lookup(gMainRoutineWork.leaderActorId);

  for (;;) {
    heading = gImil_SwayHeadings[(gKeysHeld >> 4) & 15];
    if ((u16)heading == 0xffff) {
      return;
    }

    Event_Start();

    probe.x = (subject->x.fixed & FIELD_CELL_MASK) + px(8);
    probe.y = subject->y.fixed;
    probe.z = (subject->z.fixed & FIELD_CELL_MASK) + px(8);
    z = probe.z;
    x = probe.x;
    layer = &subject->layer;
    startBehavior = MapGrid_GetCellBehaviorAt(*layer, x, z);
    Vector_AddPolarOffset(px(16), heading, &probe);

    nextBehavior = MapGrid_GetCellBehaviorAt(*layer, probe.x, probe.z);
    if (nextBehavior == CELL_BEHAVIOR_WALL ||
        MapGrid_GetTerrainHeight(*layer, probe.x, probe.z) - subject->y.fixed >
            px(8)) {
      subject->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    subject->speedLimit = px(2);
    subject->acceleration = px(1) / 10;
    subject->timer = 0;
    FieldActor_SetMoveTargetFixed(subject, x, subject->y.fixed, z);
    Actor_SelectAnimation(subject, ANIM_WALK);
    Actor_SetAnimationSpeed(subject, 48);
    Actor_WaitForMove(subject);
    subject->update = Imil_TurnSlidingActor;

    for (;;) {
      FieldVector_AddPolarOffset(16, heading, &probe);
      if ((nextBehavior =
               MapGrid_GetCellBehaviorAt(*layer, probe.x, probe.z)) == 255) {
        break;
      }
      if (MapGrid_GetTerrainHeight(*layer, probe.x, probe.z) -
              subject->y.fixed >
          px(8)) {
        break;
      }
      x = probe.x;
      z = probe.z;
      subject->speedLimit = px(2);
      subject->acceleration = px(1) / 10;
      FieldActor_SetMoveTargetFixed(subject, probe.x, probe.y, probe.z);
      Actor_WaitForMove(subject);
      if (nextBehavior != startBehavior) {
        goto blocked;
      }
    }

    subject->speedLimit = px(2);
    subject->acceleration = px(1);
    FieldActor_SetMoveTargetFixed(subject, x, subject->y.fixed, z);
    Actor_WaitForMove(subject);
    wait(2);
  }

blocked:
  subject->update = NULL;
  subject->steeringFlags |= 1;

  subject->acceleration = px(1) / 4;

tail:
  wait(10);
  Event_End();
}

// clang-format off
s16 gImil_KeyHeadings[] = {
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
    FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, -1, -1, FACING_NORTH,
    FACING_SOUTH, -1, -1, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
};

s16 gImil_SwayHeadings[] = {
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
    FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
};

s32 gImil_FreezingYoungManScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_ADD(SW_SCALE_X, -5 * Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_ShiverScript[] = {
    SC_CALL(Imil_UpdateShiver),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_OldWomanHurryScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 192),
    SC_SYNC,
    SC_MOVE(136, 0, 136),
    SC_SYNC,
    SC_MOVE(149, 0, 116),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(1),
    SC_END,
};

s32 gImil_LeaderStepAsideScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(155, 0, 135),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, TRIG_TURN * 13 / 16),
    SC_END,
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(4),
    SC_END,
};

s32 gImil_BobScript[] = {
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(2, SC_TOP),
    SC_END,
};

s32 gImil_SparkScript[] = {
    SC_ADD(SW_Y, -px(2) / 5),
    SC_WAIT(1),
    SC_LOOP(72, SC_TOP),
    SC_RELEASE,
};

s32 gImil_MiaCheckRoomScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(177, 0, 136),
    SC_SYNC,
    SC_MOVE(161, 0, 124),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_MOVE(134, 0, 139),
    SC_SYNC,
    SC_MOVE(135, 0, 189),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gImil_MiaRunOutScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(165, 0, 207),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_MOVE(147, 0, 252),
    SC_SYNC,
    SC_MOVE(150, 0, 268),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gImil_FaceTargetScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_FollowMiaOutScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(132, 0, 206),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gImil_TownEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 288, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_EAST_UNUSED, CONDITION_ALWAYS, 480, 0, 280, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_WEST, CONDITION_ALWAYS, 312, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_NORTHEAST, CONDITION_ALWAYS, 424, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_ICE_ROOMS_EAST, CONDITION_ALWAYS, 456, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_ICE_ROOMS_NORTHEAST, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_ICE_ROOMS_WEST, CONDITION_ALWAYS, 120, 0, 208, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_ICE_ROOMS_SOUTHWEST, CONDITION_ALWAYS, 64, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_IMIL_FALLS_CAVE, CONDITION_ALWAYS, 229, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImil_RoomEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 136, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { ICE_ROOMS_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 144, 0, 242, FACING_NORTH, 0, 32, 52, 256, 280, 0 },
    { ICE_ROOMS_FROM_IMIL_WEST_SOUTHWEST, CONDITION_ALWAYS, 143, 0, 596, FACING_NORTH, 0, 42, 424, 256, 631, 0 },
    { ICE_ROOMS_HOUSE_WEST, CONDITION_ALWAYS, 399, 0, 322, FACING_NORTH, 0, 320, 58, 536, 360, 0 },
    { ICE_ROOMS_HOUSE_NORTHEAST, CONDITION_ALWAYS, 400, 0, 659, FACING_NORTH, 0, 307, 382, 545, 697, 0 },
    { ICE_ROOMS_INN, CONDITION_ALWAYS, 129, 0, 915, FACING_NORTH, 0, 50, 706, 350, 949, 0 },
    { ICE_ROOMS_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 446, 0, 825, FACING_SOUTH, 0, 402, 740, 670, 921, 0 },
    { ICE_ROOMS_FROM_IMIL_EAST_NORTHEAST, CONDITION_ALWAYS, 687, 0, 264, FACING_NORTH, 0, 594, 49, 806, 295, 0 },
    { ICE_ROOMS_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 182, 0, 772, FACING_SOUTH, 0, 50, 706, 350, 949, 0 },
    { SCENE_TABLE_END },
};

u32 gImil_Exits[] = {
    SCENE_EXITS(Imil),
    SCENE_EXIT(IMIL_TO_HOUSE_SOUTHWEST, IceRooms, ICE_ROOMS_HOUSE_SOUTHWEST),
    SCENE_EXIT(IMIL_TO_INN, IceRooms, ICE_ROOMS_INN),
    SCENE_EXIT(IMIL_TO_HOUSE_WEST, IceRooms, ICE_ROOMS_HOUSE_WEST),
    SCENE_EXIT(IMIL_TO_HOUSE_NORTHEAST, IceRooms, ICE_ROOMS_HOUSE_NORTHEAST),
    SCENE_EXIT(IMIL_TO_ICE_ROOMS_EAST, IceRooms, ICE_ROOMS_FROM_IMIL_WEST_SOUTHWEST),
    SCENE_EXIT(IMIL_TO_ICE_ROOMS_NORTHEAST, IceRooms, ICE_ROOMS_FROM_IMIL_EAST_NORTHEAST),
    SCENE_EXIT(IMIL_TO_IMIL_FALLS_CAVE, ImilFallsCave, IMIL_FALLS_CAVE_FROM_IMIL),
    SCENE_EXIT(IMIL_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_IMIL_SOUTH),
    SCENE_EXIT(IMIL_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_IMIL_NORTH),
    SCENE_EXITS(IceRooms),
    SCENE_EXIT(ICE_ROOMS_HOUSE_WEST_TO_IMIL, Imil, IMIL_FROM_HOUSE_WEST),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL_WEST_SOUTHWEST, Imil, IMIL_FROM_ICE_ROOMS_WEST),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL_EAST, Imil, IMIL_FROM_ICE_ROOMS_EAST),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL, Imil, IMIL_FROM_ICE_ROOMS_NORTHEAST),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL_SOUTH_SOUTHWEST, Imil, IMIL_FROM_HOUSE_NORTHEAST),
    SCENE_EXIT(ICE_ROOMS_INN_UPSTAIRS_TO_INN, IceRooms, ICE_ROOMS_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL_EAST_NORTHEAST, Imil, IMIL_FROM_ICE_ROOMS_SOUTHWEST),
    SCENE_EXIT(ICE_ROOMS_INN_TO_INN_UPSTAIRS, IceRooms, ICE_ROOMS_INN_UPSTAIRS_FROM_INN),
    SCENE_EXITS_END,
};

struct ActorPlacement gImil_TownActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (s32)gImil_FreezingYoungManScript,
        99, 0, 234, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        376, 0, 408, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        408, 0, 248, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SNOWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        262, 0, 124, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImil_TownActorPlacementsCured[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        99, 0, 234, FACING_EAST_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        376, 0, 408, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        408, 0, 248, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SNOWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        262, 0, 124, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImil_RoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        111, 0, 215, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        166, 0, 116, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        174, 0, 173, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        118, 0, 488, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        370, 0, 468, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        478, 0, 468, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        460, 0, 266, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        436, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        100, 0, 830, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        160, 0, 880, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        249, 0, 832, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        686, 0, 148, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        717, 0, 229, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        192, 0, 124, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gImil_TownTriggers[] = {
    { TRIGGER_EXIT, 60, CONDITION_ALWAYS, IMIL_TO_WORLD_MAP_SOUTH },
    { TRIGGER_EXIT, 61, CONDITION_ALWAYS, IMIL_TO_WORLD_MAP_NORTH },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SNOWMAN, FLAG_IMIL_NO_FOOTPRINTS, EVENT(Imil_PlaceMovedSnowman) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 51, CONDITION_ALWAYS, EVENT(Imil_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 52, CONDITION_ALWAYS, EVENT(Imil_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS, EVENT(Imil_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 54, CONDITION_ALWAYS, EVENT(Imil_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 55, CONDITION_ALWAYS, EVENT(Imil_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 56, CONDITION_ALWAYS, EVENT(Imil_OpenDoorAndExit) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Imil_LeaveNorth) },
    { TRIGGER_STEP, 90, CONDITION_ALWAYS, EVENT(Imil_StartIceSlide) },
    { TRIGGER_TALK, ACTOR_FREEZING_YOUNG_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_TalkToFreezingYoungMan) },
    { TRIGGER_TALK, ACTOR_FREEZING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgImilTheMercuryLighthouseIsShiningMia) },
    { TRIGGER_TALK, ACTOR_GREETING_BOY, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_GreetVisitor) },
    { TRIGGER_TALK, ACTOR_GREETING_BOY, CONDITION_ALWAYS, MSGNO(MsgImilImilWillBeFineDiseaseIs) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_KEEPING_IN_SHAPE, CONDITION_ALWAYS, EVENT(Imil_TalkToStrongmanKeepingInShape) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREEZING_YOUNG_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_ReadFreezingYoungMansMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREEZING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgImilMiaWasOnlyInImilTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_BOY, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWeveHadALotOfStrangers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_BOY, CONDITION_ALWAYS, MSGNO(MsgImilDontWorryAboutUsMiaWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_KEEPING_IN_SHAPE, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilPeopleGetSickBecauseTheyStay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_KEEPING_IN_SHAPE, CONDITION_ALWAYS, MSGNO(MsgImilNowThatIveLetMyGuard) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_IMIL_COINS, FIND_COINS(9) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_IMIL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_GRAVE), 102, FLAG_IMIL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

struct Trigger gImil_RoomTriggers[] = {
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, FLAG_IMIL_MIA_SENT_FOR, MSGNO(MsgImilTheresBeenAnEpidemicGoingAround) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilGetMiaHurry) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_TalkToOldWoman) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgImilThatFountainWaterReallyMadeGrandpa) },
    { TRIGGER_TALK, ACTOR_OLD_MAN, FLAG_IMIL_MIA_SENT_FOR, EVENT(Imil_TalkToOldMan) },
    { TRIGGER_TALK, ACTOR_OLD_MAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilItHurts) },
    { TRIGGER_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS, EVENT(Imil_TalkToOldMan) },
    { TRIGGER_TALK, ACTOR_OLD_MANS_BED, IF_FLAG_SET(FLAG_IMIL_MET_MIA), EVENT(Imil_TalkToOldManInBed) },
    { TRIGGER_TALK, ACTOR_SICK_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilCoughTheEpidemicHasHitAlmost) },
    { TRIGGER_TALK, ACTOR_SICK_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilItsNiceThatTheFountainCan) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_AskAboutWeapons) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(Imil_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWereClosedSorryIGetA) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(Imil_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIfYouWantToBuyAnything) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(Imil_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANTS_DAUGHTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANTS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgImilImSoHappyThatMommyIs) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(Imil_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_INN_ATTENDANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWereTheOnlyInnThatStays) },
    { TRIGGER_TALK, ACTOR_INN_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgImilThankGoodnessTheTownIsBack) },
    { TRIGGER_TALK, ACTOR_RESTAURANT_OWNER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_AskAboutRestaurantMenu) },
    { TRIGGER_TALK, ACTOR_RESTAURANT_OWNER, CONDITION_ALWAYS, MSGNO(MsgImilImGladTheBeaconHasBeen) },
    { TRIGGER_TALK, ACTOR_JUSTIN, CONDITION_ALWAYS, EVENT(Imil_TalkToJustin) },
    { TRIGGER_TALK, ACTOR_MEGAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_TalkToMegan) },
    { TRIGGER_TALK, ACTOR_MEGAN, CONDITION_ALWAYS, MSGNO(MsgImilIWillBeAKindGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_ReadOldWomanMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgImilGrandpasAllBetterNowThanksTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_OLD_MAN, FLAG_IMIL_MIA_SENT_FOR, EVENT(Imil_TalkToOldMan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilOoohIDontFeelSoGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilMiaIsAGoodGirlI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilNowMiaCanLeaveTheVillage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MANS_BED, IF_FLAG_SET(FLAG_IMIL_MET_MIA), EVENT(Imil_ReadOldManInBedMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SICK_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilHerPalmsShineWithABlue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SICK_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilThatBlueLightFromHerHands) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWhoAreThesePeopleTheyMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilCustomersAreHardToComeBy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWhyDoTheyAlwaysComeWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilTheCustomersAreBackIThought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIfOnlyWeHadSomeOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilMiaSaidItWouldntBeGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANTS_DAUGHTER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIWonderIfTheyAreHeaded) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANTS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgImilMyDaddysStillInKolimaIll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilSoManyCustomersItJustGoes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgImilMiaWillBeLeavingWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWintersAreToughHereSoIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgImilTheyCameToOurVillageAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RESTAURANT_OWNER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWeDontGetToEatMany) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RESTAURANT_OWNER, CONDITION_ALWAYS, MSGNO(MsgImilINeverToldMiaThatI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUSTIN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_ReadJustinsMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUSTIN, CONDITION_ALWAYS, MSGNO(MsgImilMiasGoingToDefendTheHonor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEGAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_ReadMegansMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEGAN, CONDITION_ALWAYS, MSGNO(MsgImilImSorryMiaButICant) },
    { EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_ICE_ROOMS_WEAPON_COUNTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_TalkToWeaponMerchant) },
    { EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_ICE_ROOMS_ARMOR_COUNTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(Imil_TalkToArmorMerchant) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_IMIL_EMPTY_BOTTLE, FIND_ITEM(ITEM_EMPTY_BOTTLE) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 101, FLAG_IMIL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 102, FLAG_IMIL_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItLooksLikePorridgeItIs)) },
    // Unused: this oven's item row answers first (see the maps README).
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItsSomeHotStewItWarms)) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, ICE_ROOMS_HOUSE_WEST_TO_IMIL },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL_WEST_SOUTHWEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL_SOUTH_SOUTHWEST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, ICE_ROOMS_INN_UPSTAIRS_TO_INN },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL_EAST_NORTHEAST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, ICE_ROOMS_INN_TO_INN_UPSTAIRS },
    { TRIGGER_TABLE_END },
};

u16 gImil_SanctumDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(40, 37, 4, 2, 5), MAPGRID_ANIMATION_STEP(40, 39, 4, 2, 5), MAPGRID_ANIMATION_STEPS_END,
    MAPGRID_ANIMATION_STEP(35, 35, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gImil_RoomsEastDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 37, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 37, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gImil_InnDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 43, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 43, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gImil_HouseSouthwestDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 39, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 39, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gImil_HouseWestDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(42, 48, 2, 2, 5), MAPGRID_ANIMATION_STEP(40, 48, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gImil_HouseNortheastDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(42, 46, 2, 2, 5), MAPGRID_ANIMATION_STEP(40, 46, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

/// Each door's opening, in the order of its exit (IMIL_TO_HOUSE_SOUTHWEST
/// first), as Imil_OpenDoorAndExit reads it.
u16 *gImil_DoorCellSteps[] = {
    gImil_HouseSouthwestDoorSteps,
    gImil_InnDoorSteps,
    gImil_HouseWestDoorSteps,
    gImil_HouseNortheastDoorSteps,
    gImil_RoomsEastDoorSteps,
    gImil_SanctumDoorSteps,
};

/// Cell position (x, y) for each exit.
s16 gImil_DoorCellPoints[][2] = {
    { 50, 24 }, { 57, 22 }, { 59, 13 }, { 54, 10 }, { 38, 8 }, { 34, 19 },
};
// clang-format on

u16 gImil_CellStepsA[] = {
    MAPGRID_ANIMATION_STEP(1, 125, 1, 2, 10),
    MAPGRID_ANIMATION_STEP(0, 125, 1, 2, 10),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gImil_CellStepsB[] = {
    MAPGRID_ANIMATION_STEP(1, 125, 1, 2, 10),
    MAPGRID_ANIMATION_STEP(2, 125, 1, 2, 10),
    MAPGRID_ANIMATION_STEPS_END,
};

s32 gImil_ArcOrigin[3];
