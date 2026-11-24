/// @file
/// Inside Madra's buildings: the inn, the round sanctum, the houses, the
/// mayor's house with the Black Crystal on its mantel, the jail where Piers is
/// held, and, as a second scene, the left cavern home in the cliff. Walking
/// into the jail the first time, Felix sees Shin accuse Piers of being one of
/// the Champa and Piers, angered, lift him on a shaft of ice. Later the old
/// couple who love mushrooms trade Char, the Mars Djinni, for the Healing
/// Fungus from the Gondowan Cliffs, and when Felix brings Piers home the
/// mayor's household welcomes him.

#include "shared/tables.h"
#include "sprite_ids.h"
#include "flags.h"
#include "item_ids.h"
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
#include "scene/scene.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "version.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"
#include "field/actor_script.h"
#include "djinn_ids.h"
#include "text/msg_ids.h"

/// Set once the old man of the mushroom house has been given the Healing
/// Fungus and has traded Char for it.
#define FLAG_MADRA_HEALING_FUNGUS_GIVEN 0x98a
/// Set as the Healing Fungus is given, for this visit: the couple look forward
/// to dinner until the party leaves the area.
#define FLAG_MADRA_FUNGUS_FOR_DINNER FLAG_AREA_TEMP_10
/// Set once the old man has been offered the Laughing Fungus on this visit.
#define FLAG_MADRA_LAUGHING_FUNGUS_OFFERED FLAG_SCENE_TEMP_10
/// Set once the man focusing on making ice has groaned on this visit.
#define FLAG_MADRA_FOCUSING_MAN_GROANED FLAG_SCENE_TEMP_11

/// Madra's houses, numbering the rows gMadraRooms_Placements and
/// gMadraRooms_PlacementsOnReturn share.
enum MadraRoomsActorId {
  /// A couple who fear what lies south in Gondowan, and then the Kibombo.
  ACTOR_OLD_MAN_WARY_OF_GONDOWAN = ACTOR_FIRST_PLACED,
  ACTOR_WOMAN_WARY_OF_GONDOWAN,
  /// The old man who loves the rare mushrooms of the Gondowan Cliffs, in the
  /// house south of the market.
  ACTOR_MUSHROOM_LOVER,
  /// His wife, who cooks them.
  ACTOR_MUSHROOM_COOK,
  /// Two men curious about the strange ship grounded in the cove to the east.
  ACTOR_MAN_ON_THE_STRANGE_SHIP,
  ACTOR_MAN_ON_THE_GROUNDED_SHIP,
  /// The elder's household: who keeps the jewel on his mantel with Milady,
  /// his daughter, the old woman of the house, and his two grandchildren.
  ACTOR_JEWEL_KEEPER,
  ACTOR_ELDERS_DAUGHTER,
  ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE,
  ACTOR_ELDERS_GRANDSON,
  ACTOR_ELDERS_GRANDDAUGHTER,
  /// The sanctum's healer and the two women who pray there.
  ACTOR_HEALER,
  ACTOR_WOMAN_OF_THE_SANCTUM,
  ACTOR_OLD_WOMAN_OF_THE_SANCTUM,
  ACTOR_INNKEEPER,
  /// Who helps at the inn, sure it is safe from the Champa.
  ACTOR_INN_HELPER,
  /// A girl staying at the inn, who hid under her bed in the raid.
  ACTOR_GIRL_STAYING_AT_THE_INN,
  /// Char, the Mars Djinni the mushroom couple keep.
  ACTOR_CHAR,
  /// A blank actor on the elder's mantel that shows the Black Crystal
  /// (MadraRooms_PlaceBlackCrystal).
  ACTOR_BLACK_CRYSTAL
};

/// The rest of gMadraRooms_Placements.
enum MadraRoomsTravelersActorId {
  /// Two travelers at the inn, the first slept through the pirate raid; his
  /// partner wears the same sprite in other colours.
  ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID = ACTOR_BLACK_CRYSTAL + 1,
  ACTOR_TRAVELERS_PARTNER,
  /// The two warriors on their way to Colosso in Tolbi, there once
  /// FLAG_TRANSFERRED_WON_COLOSSO is set: "I'm Morgan, and this here's Dekka."
  ACTOR_MORGAN,
  ACTOR_DEKKA
};

/// The rest of gMadraRooms_PlacementsOnReturn, when the party brings Piers
/// home. The rows for Piers, Jenna and Sheba go to their party slots.
enum MadraRoomsReturnActorId {
  /// The elder, who held Piers' jewel, and Madra's mayor.
  ACTOR_ELDER = ACTOR_BLACK_CRYSTAL + 1,
  ACTOR_MAYOR,
  /// Unused: two more rows with Kraden's sprite, out of sight.
  ACTOR_UNUSED_FIRST_KRADEN,
  ACTOR_UNUSED_SECOND_KRADEN,
  ACTOR_KRADEN,
  ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN,
  ACTOR_TRAVELERS_PARTNER_ON_RETURN
};

/// The jail's actors, by the rows of gMadraRooms_JailPlacements; Piers' row
/// goes to his party slot.
enum MadraRoomsJailActorId {
  /// The puddle Piers freezes into a shaft of ice.
  ACTOR_JAIL_PUDDLE = ACTOR_FIRST_PLACED,
  /// Shin, who threatens Piers, and his friend, who accuses him of being one
  /// of the Champa.
  ACTOR_SHIN,
  ACTOR_SHINS_FRIEND,
  /// Kraden, who jumps out from behind Felix.
  ACTOR_JAIL_KRADEN,
  ACTOR_SECOND_JAIL_PUDDLE,
  /// Once FLAG_GABOMBA_MESSAGE_CARRIED is set: who saw Isaac's party hear of
  /// Felix, and a man who has been focusing for days to make ice as Piers
  /// did.
  ACTOR_WITNESS_ON_ISAACS_PARTY,
  ACTOR_MAN_FOCUSING_ON_ICE
};

/// The left cavern home (MadraRoomsCavernHomes), by the rows of
/// gMadraRooms_SecondRoomsPlacements.
enum MadraRoomsCavernHomeActorId {
  /// A couple who talk of Alhafra, and later of a plan to make money.
  ACTOR_MAN_ON_ALHAFRA = ACTOR_FIRST_PLACED,
  ACTOR_WOMAN_ON_ALHAFRA,
  /// A woman lying ill in bed until Briggs is caught.
  ACTOR_SISTER_IN_BED,
  /// Her sister, glad she is better.
  ACTOR_HER_SISTER
};

/// The cells, (44, 5) and (44, 6) a few steps in from the mayor's door, whose
/// step trigger runs MadraRooms_WelcomePiersHome.
enum { CELL_BEHAVIOR_MADRA_ROOMS_WELCOME = 30 };

/// Madra's inn, as Inn_CheckIn numbers the inns.
enum { MADRA_INN = 1 };

extern s32 gMadraRooms_ShinLifted[];
extern s32 gMadraRooms_ShinFalls[];
void MadraRooms_SayFarewellAgain(void);

void MadraRooms_AskToStayLonger(void);

extern s32 gMadraRooms_OldManWalksToBackRoom[];
extern s32 gMadraRooms_WifeComesBack[];
extern s32 gMadraRooms_OldManComesBack[];

extern s32 gMadraRooms_CharFollows[];

/// Counts one more weird answer Felix gives (Event_CountAnswer, through the
/// overlay's veneer).
void MadraRooms_CountAnswer();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMadraRooms_Placements[];
extern struct ActorPlacement gMadraRooms_PlacementsOnReturn[];
extern struct ActorPlacement gMadraRooms_JailPlacements[];
extern struct ActorPlacement gMadraRooms_SecondRoomsPlacements[];
extern struct Trigger gMadraRooms_TriggersBeforeRaid[];
extern struct Trigger gMadraRooms_JailTriggers[];
extern struct Trigger gMadraRooms_SecondRoomsTriggers[];
extern struct Trigger gMadraRooms_TriggersAfterRaid[];
extern struct Trigger gMadraRooms_TriggersOnReturn[];

void Event_End(void);
void Sanctum_Run(s32 actor);
void Inn_CheckIn(s32 slot, s32 actor);
void MadraRooms_ShinAngersPiers(void);

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_MADRA_ROOMS_CAVERN_HOMES)
    return gMadraRooms_SecondRoomsPlacements;
  if (gMainRoutineState.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
    return gMadraRooms_JailPlacements;
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gMadraRooms_PlacementsOnReturn;
  return gMadraRooms_Placements;
}

/// Talking to the auburn-haired woman in the house by the town's south-west
/// corner: "Indra is in direct contact with southern Gondowan... Will we be
/// safe?" Told yes, she says Felix is only trying to make her feel better;
/// told no, she tells him to stop getting folks worked up.
void MadraRooms_AskWillWeBeSafe(void) {
  s32 msg;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  msg = (s32)MsgMadraRoomsIndraIsInDirectContact;
  FieldEvent_SetMessageId(msg);
  // "Indra is in direct contact with southern Gondowan... Will we be safe?"
  Event_OpenMessage(ACTOR_WOMAN_WARY_OF_GONDOWAN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(ACTOR_WOMAN_WARY_OF_GONDOWAN, 0);
  Event_End();
}

/// Talking to Madra's innkeeper from across the counter on the inn's ground
/// floor lets the party stay the night. From any other side he talks of the
/// week's troubles, then of the Kibombo's wicked magic, and when Felix brings
/// Piers home of the dead fish on the shore, which the later editions ask as a
/// question.
void MadraRooms_TalkToInnkeeper(void) {
  u16 facing = Actor_Get(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(MADRA_INN, ACTOR_INNKEEPER);
  } else {
#if !TLA_PATCH_1
    // Before patch 1 there is no choice prompt.
    if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
      FieldEvent_SetMessageId((s32)MsgMadraRoomsAreYouWorriedAboutAll);
    else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
      FieldEvent_SetMessageId((s32)MsgMadraRoomsIveHeardThatKibomboWield);
    else
      FieldEvent_SetMessageId(
          (s32)MsgMadraRoomsEarthquakesPirateAttacksWhatWeek);
    // "Earthquakes, pirate attacks... What a week!"
    Event_ShowMessage(ACTOR_INNKEEPER, 0);
#else
    if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED)) {
      s32 msg = (s32)MsgMadraRoomsAreYouWorriedAboutAll;

      FieldEvent_SetMessageId(msg);
      Event_OpenMessage(ACTOR_INNKEEPER, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(),
                                                0) == 0) {
        FieldEvent_Wait(10);
        FieldEvent_SetMessageId(msg + 1);
      } else {
        FieldEvent_Wait(20);
        FieldEvent_SetMessageId(msg + 2);
      }
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    } else {
      if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
        FieldEvent_SetMessageId((s32)MsgMadraRoomsIveHeardThatKibomboWield);
      else
        FieldEvent_SetMessageId(
            (s32)MsgMadraRoomsEarthquakesPirateAttacksWhatWeek);
      // "Earthquakes, pirate attacks... What a week!"
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    }
#endif
  }
}

/// Talking to the healer in the round sanctum from across his counter opens the
/// sanctum's services; from any other side he talks of the town's trials, then
/// of the Kibombo, and later of the warming sea.
void MadraRooms_TalkToHealer(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
      FieldEvent_SetMessageId(
          (s32)MsgMadraRoomsIveNoticedLatelyThatTemperature);
    else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
      FieldEvent_SetMessageId((s32)MsgMadraRoomsKibomboAreTribeFromGondowan);
    else
      FieldEvent_SetMessageId((s32)MsgMadraRoomsSurelyThisTownIsEnduring);
    // "Surely this town is enduring some sort of test... We must not fail..."
    Event_ShowMessage(actor, 0);
  }
}

/// Felix goes to touch the Black Crystal on the mantel in the mayor's house:
/// the auburn-haired woman by the table hops up, turns to him and cries "Don't
/// touch that gem! The elder gave us strict orders to protect it!"
void MadraRooms_GuardElderGem(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_FaceActor(ACTOR_JEWEL_KEEPER, ACTOR_FELIX, 0);
  FieldActor_Jump(ACTOR_JEWEL_KEEPER, 6, 20);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_JEWEL_KEEPER, 0);
  FieldEvent_SetMessageId((s32)MsgMadraRoomsDontTouchThatGemElder);
  // "Don't touch that gem! The elder gave us strict orders to protect it!"
  Event_ShowMessage(ACTOR_JEWEL_KEEPER, 0);
  Actor_FaceDirection(ACTOR_JEWEL_KEEPER, FACING_SOUTHEAST, 0);
  Event_End();
}

/// Talking to the old man who loves the rare mushrooms of the Gondowan
/// Cliffs, in the house south of the market: he wonders how to climb the cliffs
/// to get one, and once he has the Healing Fungus, he looks forward to dinner,
/// then says he ate too much.
void MadraRooms_TalkToMushroomLover(void) {
  s32 msg;

  if (Event_GetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
    if (Event_GetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER))
      msg = (s32)MsgMadraRoomsICantWaitForDinner;
    else
      msg = (s32)MsgMadraRoomsThatMushroomWasGreat;
    FieldEvent_SetMessageId(msg);
    // "That mushroom was great, but I ate way too much. I don't ever want..."
    Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
  } else {
    if (Event_GetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED)) {
      FieldEvent_SetMessageId((s32)MsgMadraRoomsThanksForBringing);
      // "Thanks for bringing this by, but it's just not the type of..."
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraRoomsHowAreWeToClimb);
      // "How are we to climb the cliffs between here and Gondowan? If..."
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    }
    // The field menu opens as he finishes: the player's chance to use a fungus
    // on him from the items (the USE_ITEM_ON_ACTOR triggers).
    gFieldState->fieldMenu = 1;
  }
}

/// Talking to the mushroom lover's wife, in the same house: she tells of her
/// husband's love of the rare mushrooms of the Gondowan Cliffs, begs the party
/// to find the right one once he has turned down the Laughing Fungus, and
/// after the Healing Fungus talks of cooking it.
void MadraRooms_TalkToMushroomCook(void) {
  if (Event_GetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
    if (Event_GetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
      FieldEvent_SetMessageId((s32)MsgMadraRoomsThisHeresEnoughOfMushroom);
      // "This here's enough of a mushroom that we'll be eating it for days!"
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraRoomsActuallyIDidntCookMushrooms);
      // "Actually, I didn't cook the mushrooms properly that night. But..."
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    }
  } else if (Event_GetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED)) {
    FieldEvent_SetMessageId((s32)MsgMadraRoomsPleaseWontYouPleaseTry);
    // "Please... Won't you please try to find the mushrooms my husband..."
    Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraRoomsMyHusbandRelishesThoseRare);
    // "My husband relishes those rare mushrooms growing in the Gondowan..."
    Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
  }
}

/// Casting Mind Read on the mushroom lover: he thinks of the mushrooms he
/// longs for, and once he has the Healing Fungus, of dinner.
void MadraRooms_MindReadMushroomLover(void) {
  if (Event_GetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
    if (Event_GetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
      FieldEvent_SetMessageId((s32)MsgMadraRoomsMoreIThinkAboutDinner);
      // "The more I think about dinner tonight, the more I drool..."
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraRoomsIDontRememberMushroomsLike);
      // "I don't remember mushrooms like that ever tasting bad..."
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    }
  } else {
    s32 flag = Event_GetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
    s32 msg = (s32)MsgMadraRoomsImDyingForSome;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    }
  }
}

/// Casting Mind Read on the mushroom lover's wife: she thinks of her husband
/// growing old, and once he has the Healing Fungus, of cooking it.
void MadraRooms_MindReadMushroomCook(void) {
  if (Event_GetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
    if (Event_GetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
      FieldEvent_SetMessageId((s32)MsgMadraRoomsTheseMushroomsWillMakeAny);
      // "These mushrooms will make any meal taste amazing!"
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraRoomsIfMyHusbandFindsOut);
      // "If my husband finds out I messed up the recipe, he'll be so sad..."
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    }
  } else {
    s32 flag = Event_GetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
    s32 msg = (s32)MsgMadraRoomsMyPoorHusbandsGettingOld;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    }
  }
}

/// Talking to the man in the jail who has been trying for days to make ice as
/// Piers did, once Felix brings Piers home: the first time on a visit he
/// strains and groans "Uhhhnnn... UHHHNNN!", and afterwards he wonders how
/// anyone makes ice just by thinking.
void MadraRooms_TalkToManFocusingOnIce(void) {
  if (!Event_GetFlag(FLAG_MADRA_FOCUSING_MAN_GROANED)) {
    Event_SetFlag(FLAG_MADRA_FOCUSING_MAN_GROANED);
    FieldEvent_SetMessageId((s32)MsgMadraUhhhnnnUhhhnnn);
    Actor_WalkByAndWait(ACTOR_MAN_FOCUSING_ON_ICE, -12, 0);
    FieldEvent_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_MAN_FOCUSING_ON_ICE, 2);
    FieldEvent_Wait(20);
    // "Uhhhnnn... UHHHNNN!"
    Event_ShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
    FieldActor_ShowEmote(ACTOR_MAN_FOCUSING_ON_ICE,
                         EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(ACTOR_MAN_FOCUSING_ON_ICE, 12, 0);
    Actor_FaceActor(ACTOR_MAN_FOCUSING_ON_ICE, ACTOR_FELIX, 0);
    // "They told me that you make Psynergy by focusing your mind... I've..."
    Event_ShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraRoomsHowCanAnyoneJustCreate);
    Actor_FaceActor(ACTOR_MAN_FOCUSING_ON_ICE, ACTOR_FELIX, 0);
    // "How can anyone just create ice by thinking about it? I mean, imagine..."
    Event_ShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_MADRA_ROOMS_CAVERN_HOMES)
    return gMadraRooms_SecondRoomsTriggers;
  if (gMainRoutineState.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
    return gMadraRooms_JailTriggers;
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gMadraRooms_TriggersOnReturn;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gMadraRooms_TriggersAfterRaid;
  return gMadraRooms_TriggersBeforeRaid;
}

/// The Black Crystal sits on the mantel in the mayor's house until the Kibombo
/// steal it in their raid (FLAG_BRIGGS_CAUGHT), after which it is gone.
void MadraRooms_PlaceBlackCrystal(void) {
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    Actor_SetPosition(ACTOR_BLACK_CRYSTAL, 888, 8);
  } else {
    Actor_SetItemIcon(Actor_Get(ACTOR_BLACK_CRYSTAL), ITEM_BLACK_CRYSTAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_BLACK_CRYSTAL), 0);
  }
}

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is.
static inline void MadraRooms_PlaceFacing(s32 actorId, s32 x, s32 z,
                                          u16 facing) {
  Actor_SetPositionAndFacing(actorId, x, z, facing);
}

/// Sets up the jail: the first time Felix walks in, Shin angers Piers
/// (MadraRooms_ShinAngersPiers); afterwards Shin and his friend are gone until
/// the Kibombo's raid, when they are back in the jail.
void MadraRooms_SetUpJail(void) {
  if (!Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    gFieldState->psynergyDrawFlags =
        PSYNERGY_DRAW_NO_SCENE_SPRITE | PSYNERGY_DRAW_FIXED_PRIORITY;
    if (!Event_GetFlag(FLAG_PIERS_INTERROGATED)) {
      MadraRooms_ShinAngersPiers();
    } else {
      Actor_SetPosition(ACTOR_SHIN, 0, 0);
      Actor_SetPosition(ACTOR_SHINS_FRIEND, 0, 0);
    }
  } else if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED)) {
    MadraRooms_PlaceFacing(ACTOR_SHIN, toFIXED(515), toFIXED(841),
                           FACING_NORTHEAST);
    MadraRooms_PlaceFacing(ACTOR_SHINS_FRIEND, toFIXED(472), toFIXED(877),
                           FACING_NORTH);
    Actor_SetPosition(ACTOR_JAIL_PUDDLE, 0, 0);
  } else {
    MadraRooms_PlaceFacing(ACTOR_SHIN, toFIXED(515), toFIXED(841),
                           FACING_NORTHWEST);
    MadraRooms_PlaceFacing(ACTOR_SHINS_FRIEND, toFIXED(472), toFIXED(877),
                           FACING_NORTH);
    Actor_SetPosition(ACTOR_JAIL_PUDDLE, 0, 0);
  }
}

extern s32 gMadraRooms_ShinAndFriendRunAway[];
void FieldCamera_WaitForArrival();
void FieldPsynergy_LoadResources();
void FieldPsynergy_SetupActorPair();
void BattleEffect_StopMainActor();

/// Sets up Madra's buildings as the party walks in: Morgan and Dekka at the inn
/// only for a party that won Colosso in The Broken Seal, the jail, the sister
/// lying ill in bed in the left cavern home until Briggs is caught, and the
/// Black Crystal.
s32 Scene_Initialize(void) {
  struct FieldSprite *sprite;
  struct FieldState *event;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineState.next == SCENE_MADRA_ROOMS_HOUSES) {
    event = Ram_HeapSlots->fieldState;
    event->psynergyDrawFlags =
        PSYNERGY_DRAW_NO_SCENE_SPRITE | PSYNERGY_DRAW_FIXED_PRIORITY;
    if (!Event_GetFlag(FLAG_TRANSFERRED_WON_COLOSSO)) {
      Actor_SetPosition(ACTOR_MORGAN, 0, 0);
      Actor_SetPosition(ACTOR_DEKKA, 0, 0);
    } else {
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_MORGAN), SPRITE_COLOR_BGR);
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_DEKKA), SPRITE_COLOR_BRG);
    }
    if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
      MapGrid_CopyCellsLayered(2, 110, 16, 10, 16, 64);
      MapGrid_CopyCellAttributes(2, 46, 16, 10, 16, 0);
    }
  }
  if (gMainRoutineState.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
    MadraRooms_SetUpJail();
  if (gMainRoutineState.next == SCENE_MADRA_ROOMS_CAVERN_HOMES) {
    if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
      Actor_SetPosition(ACTOR_SISTER_IN_BED, 0, 0);
    } else {
      Actor_SetSpriteRotationById(ACTOR_SISTER_IN_BED, TRIG_TURN * 3 / 4);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SISTER_IN_BED), 0);
      // Lying under the covers: her sprite turned, its bottom 8 rows sunk.
      ((struct FieldSprite *)Actor_Get(ACTOR_SISTER_IN_BED)->sprite)->flags |=
          SPRITE_FLAG_SUNK;
      sprite = Actor_Get(ACTOR_SISTER_IN_BED)->sprite;
      sprite->sunkRows = 8;
      ((struct FieldSprite *)Actor_Get(ACTOR_SISTER_IN_BED)->sprite)->dirty = 1;
    }
  }
  MadraRooms_PlaceBlackCrystal();
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_TRAVELERS_PARTNER_ON_RETURN),
                              SPRITE_COLOR_RBG);
  else
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_TRAVELERS_PARTNER),
                              SPRITE_COLOR_RBG);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Turns an actor's sprite a thirty-second of a turn, a frame of Shin falling
/// over (gMadraRooms_ShinFalls).
s32 MadraRooms_RotateSprite(struct Actor *actor) {
  struct FieldSprite *sprite;

  sprite = actor->sprite;
  if (actor != NULL && sprite != NULL)
    sprite->rotation -= TRIG_TURN / 32;
  return 0;
}

/// Unused: stands an actor's sprite upright again; no script calls it.
UNUSED s32 MadraRooms_ResetSpriteRotation(struct Actor *actor) {
  struct FieldSprite *sprite;

  sprite = actor->sprite;
  if (actor != NULL && sprite != NULL)
    sprite->rotation = 0;
  return 0;
}

/// The party enters the jail as Shin and his friend accuse Piers of being one
/// of the Champa, and sees Piers, angered, lift Shin on a shaft of ice.
void MadraRooms_ShinAngersPiers(void) {
  s32 i;
  struct Actor *felix;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetAnimation(ACTOR_JAIL_PUDDLE, 1);
  Actor_SetPosition(ACTOR_FELIX, 400, 888);
  FieldCamera_MoveTo(408, CAMERA_KEEP, 848, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  FieldEvent_SetMessageId((s32)MsgMadraRoomsAdmitItYoureOne);
  // "Admit it! You're one of the Champa, aren't you, Piers?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 396, 869);
  FieldActor_LaunchFromLeader(ACTOR_JAIL_KRADEN, 25, 19, FACING_NORTH);
  FieldActor_WaitForScriptEndById(ACTOR_JAIL_KRADEN);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JAIL_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JAIL_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 820, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Speak! Or let your silence condemn you!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHIN, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "I told you already... I'm not a Champa."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SHIN, 2);
  // "Then where did you come from?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHIN, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  Actor_WalkByAndWait(ACTOR_PIERS, 0, -16);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "The heart of the Eastern Sea... If I told you where, you'd never..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldActor_ShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_Get(ACTOR_SHIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  for (i = 0; i < 3; i++) {
    Actor_WalkByAndWait(ACTOR_SHIN, 0, -8);
    Event_PlaySound(SE_HEAVY_IMPACT);
    Actor_WalkByAndWait(ACTOR_SHIN, 0, 8);
  }
  Actor_Get(ACTOR_SHIN)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Look at me when I'm talking to you! Are you trying to mock me!?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHIN, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "I implore you, do not anger me."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Oh, do you? So what's going to happen if I make you angry?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHIN, 0, 10);
  Actor_FaceActor(ACTOR_SHINS_FRIEND, ACTOR_SHIN, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
  // "Hey, Shin... Maybe you should go easy on him..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
  Actor_FaceActor(ACTOR_SHIN, ACTOR_SHINS_FRIEND, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SHIN, 2);
  // "Hey, it wasn't your girlfriend who got hurt!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHIN, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "I'm sorry she was injured, but I cannot°"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldActor_ShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHIN, FACING_NORTH);
  // "I don't want your sympathy, freak! I want you to get angry for me!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHIN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  // "Stop, please... Even my patience has its limits."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "Hey, don't you want to see what he'll do?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHIN, 0, 10);
  Actor_ShowEmote(ACTOR_SHINS_FRIEND, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "I think we should just stop..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
  // "If my words will not cool your temper, then..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_PIERS, 0, 16);
  FieldPsynergy_LoadResources(ABILITY_FROST, 1);
  FieldPsynergy_SetupActorPair(ACTOR_PIERS, ACTOR_JAIL_PUDDLE);
  FieldPsynergy_StartCast();
  Actor_ShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(ACTOR_SHIN, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_SHINS_FRIEND, ACTOR_PIERS, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHIN), 0);
  Actor_SetScript(ACTOR_SHIN, gMadraRooms_ShinLifted);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
  // "Oh! That was Psynergy!"
  FieldEvent_ShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
  FieldActor_Stop(ACTOR_SHIN);
  Actor_Get(ACTOR_SHIN)->scaleX = FIXED_ONE;
  Actor_Get(ACTOR_SHIN)->scaleY = FIXED_ONE;
  FieldEvent_Wait(4);
  Actor_Get(ACTOR_SHIN)->motionFlags = ACTOR_FALLS;
  Actor_Get(ACTOR_SHIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestinationOffset(ACTOR_SHIN, -8, 0);
  Actor_SetScript(ACTOR_SHIN, gMadraRooms_ShinFalls);
  FieldActor_WaitForScriptEndById(ACTOR_SHIN);
  Event_PlaySound(SE_LANDING_THUD);
  FieldEvent_Wait(60);
  Actor_Get(ACTOR_SHIN)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldActor_Jump(ACTOR_SHIN, 6, 0);
  FieldEvent_Wait(3);
  Actor_SetSpriteRotationById(ACTOR_SHIN, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHIN), 1);
  Actor_FaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHINS_FRIEND, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHINS_FRIEND, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_SHINS_FRIEND, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       30);
  Actor_RunRepeatedMotion(ACTOR_SHIN, 2);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_SHIN, 6, 20);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_SHIN, ACTOR_PIERS, 0);
  FieldEvent_Wait(20);
  Actor_Get(ACTOR_SHINS_FRIEND)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SHIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_SHIN, -16, 16);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_SHIN, 6, 20);
  FieldEvent_Wait(20);
  Actor_Get(ACTOR_SHIN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetScript(ACTOR_SHIN, gMadraRooms_ShinAndFriendRunAway);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_SHIN);
  FieldActor_WatchAndSetScript(ACTOR_JAIL_KRADEN, ACTOR_SHIN);
  // "Waaaaaaaaaahh!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHIN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_SHINS_FRIEND, ACTOR_PIERS, 0);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_SHINS_FRIEND);
  FieldActor_WatchAndSetScript(ACTOR_JAIL_KRADEN, ACTOR_SHINS_FRIEND);
  // "Did you do that?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "What did your friend expect?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_SHINS_FRIEND, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_WalkByAndWait(ACTOR_SHINS_FRIEND, -16, 16);
  // "You monster!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 1);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "I... I am no monster."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_SHINS_FRIEND, -16, 8);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
  FieldEvent_Wait(20);
  // "Help!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
  Actor_Get(ACTOR_SHINS_FRIEND)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetScript(ACTOR_SHINS_FRIEND, gMadraRooms_ShinAndFriendRunAway);
  FieldEvent_Wait(40);
  FieldActor_Stop(ACTOR_FELIX);
  FieldActor_Stop(ACTOR_JAIL_KRADEN);
  FieldActor_WaitForScriptEndById(ACTOR_SHINS_FRIEND);
  Actor_SetPosition(ACTOR_SHIN, 0, 0);
  Actor_SetPosition(ACTOR_SHINS_FRIEND, 0, 0);
  Actor_SetAnimation(ACTOR_JAIL_PUDDLE, 6);
  Actor_SetSpriteColorById(ACTOR_JAIL_PUDDLE, SPRITE_COLOR_NORMAL);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_WaitForAnimationChange(8);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceEachOther(ACTOR_JAIL_KRADEN, ACTOR_FELIX, 0);
  Actor_RunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
  FieldEvent_Wait(20);
  // "This man... He must be an Adept!" Yes: "Of course! That was clearly
  // Psynergy he just used..." No: "You're right... We only know what we..."
  Event_OpenMessage(ACTOR_JAIL_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
    // "Of course! That was clearly Psynergy he just used..."
    FieldEvent_ShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
    gFieldState->message += 1;
  } else {
    Actor_SetAnimationAndWait(ACTOR_JAIL_KRADEN, ANIM_SHAKE_HEAD);
    gFieldState->message += 1;
    // "You're right... We only know what we just saw."
    FieldEvent_ShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
  }
  Actor_RunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Perhaps we should try to learn a little more about him."
  FieldEvent_ShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JAIL_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_JAIL_KRADEN, ANIM_WALK);
  felix = Actor_Get(ACTOR_FELIX);
  if (felix != NULL)
    Actor_SetDestination(ACTOR_JAIL_KRADEN, felix->x.part[0].integer,
                         felix->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_JAIL_KRADEN);
  Actor_SetPosition(ACTOR_JAIL_KRADEN, 0, 0);
  Event_SetFlag(FLAG_PIERS_INTERROGATED);
  Event_End();
}

/// Talking to Piers in his cell in Madra's jail: "Ah... When will that kindly
/// elder return and end my imprisonment? I have no time to wait, but I don't
/// want to have to hurt anyone..."
void MadraRooms_TalkToPiers(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgMadraRoomsAhWhenWillThatKindly);
  Actor_FaceDirection(ACTOR_JAIL_KRADEN, FACING_SOUTH, 0);
  // "Ah... When will that kindly elder return and end my imprisonment? I..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Event_End();
}

/// Casting Mind Read on Piers in his cell: he regrets using his Psynergy in
/// anger, then senses someone using Psynergy and looks about for them, and
/// wonders when he will be freed.
void MadraRooms_MindReadPiers(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgMadraRoomsWhyDidIUse);
  // "Why did I use my Psynergy in anger? I must maintain control... My..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "I...sense someone... using Psynergy..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "That person... Could he be°no, he is not of my people..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  // "When will I be freed?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Event_End();
}

/// Unused: Piers' Frost at the jail's puddle alone, as
/// MadraRooms_ShinAngersPiers plays it; nothing calls it.
UNUSED void MadraRooms_PiersUsesPsynergy(void) {
  FieldPsynergy_LoadResources(ABILITY_FROST, 1);
  FieldPsynergy_SetupActorPair(ACTOR_PIERS, ACTOR_JAIL_PUDDLE);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

extern s32 gMadraRooms_WifeWalksToBackRoom[];
extern s32 gMadraRooms_LeaderStepsAside[];

/// Felix gives the old mushroom lover the Healing Fungus from the Gondowan
/// Cliffs: "Oh, my... Is... Is that..." He and his wife go to their back room
/// and come back with Char, the Mars Djinni, who joins the party.
void MadraRooms_GiveHealingFungus(void) {
  struct Actor *actor;

  Event_SetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN);
  Event_SetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER);
  PartyInventory_Remove(ITEM_HEALING_FUNGUS);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetSpeed(ACTOR_MUSHROOM_COOK, 0.4);
  Actor_WalkTo(ACTOR_MUSHROOM_COOK, 410, 109);
  actor = Actor_Get(ACTOR_FELIX);
  if (actor->z.fixed > toFIXED(95))
    Actor_WalkToAndWait(ACTOR_FELIX, actor->x.fixed / FIXED_ONE, 95);
  Actor_WalkToAndWait(ACTOR_FELIX, 362, 95);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldActor_WaitForMoveById(ACTOR_MUSHROOM_COOK);
  FieldEvent_SetMessageId((s32)MsgMadraRoomsOhMy);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Oh, my... Is... Is that..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
  Actor_ShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 3);
  Actor_WalkToAndWait(ACTOR_MUSHROOM_COOK, 378, 109);
  Actor_FaceDirection(ACTOR_MUSHROOM_COOK, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "Is that one of his most favorite mushrooms!?"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Actor_FaceEachOther(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Did you climb all the way up those cliffs just to get that mushroom..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "That's the nicest thing anyone has ever done for me. I have to thank..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_MUSHROOM_LOVER, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "But what could I have that a warrior like you would want?"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
  FieldEvent_Wait(20);
  // "That's a tricky one..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  FieldActor_Stop(ACTOR_CHAR);
  Actor_SetSpeed(ACTOR_MUSHROOM_LOVER, 0.5);
  Actor_SetSpeed(ACTOR_MUSHROOM_COOK, 0.5);
  Actor_SetSpeed(ACTOR_CHAR, 0.5);
  Actor_SetScript(ACTOR_MUSHROOM_COOK, gMadraRooms_WifeWalksToBackRoom);
  Actor_SetScript(ACTOR_FELIX, gMadraRooms_LeaderStepsAside);
  FieldEvent_Wait(60);
  Actor_SetScript(ACTOR_MUSHROOM_LOVER, gMadraRooms_OldManWalksToBackRoom);
  FieldActor_WaitForScriptEndById(ACTOR_MUSHROOM_LOVER);
  FieldEvent_Wait(120);
  Actor_SetScript(ACTOR_MUSHROOM_COOK, gMadraRooms_WifeComesBack);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_MUSHROOM_COOK);
  FieldEvent_Wait(60);
  Actor_SetScript(ACTOR_MUSHROOM_LOVER, gMadraRooms_OldManComesBack);
  Actor_SetScript(ACTOR_CHAR, gMadraRooms_CharFollows);
  FieldActor_WaitForScriptEndById(ACTOR_MUSHROOM_LOVER);
  FieldActor_WaitForScriptEndById(ACTOR_MUSHROOM_COOK);
  FieldActor_WaitForScriptEndById(ACTOR_CHAR);
  Actor_RunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
  FieldEvent_Wait(20);
  // "I don't know if you want a pet, but this is all we can offer. Take..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Djinn_JoinParty(ACTOR_CHAR, DJINNI_CHAR / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_CHAR));
  gFieldState->message += 1;
  Actor_SetAnimation(ACTOR_FELIX, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MUSHROOM_LOVER, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MUSHROOM_LOVER, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  // "I can't wait for dinner tonight. Batter-fried mushrooms! Mmm!"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MUSHROOM_COOK, ANIM_NOD);
  FieldEvent_Wait(20);
  // "This here's enough of a mushroom that we'll be eating it for days!"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Event_End();
}

/// Felix gives the old mushroom lover the Laughing Fungus: he tells his wife
/// "No, dear... They look similar, but these aren't quite right." and turns
/// it down.
void MadraRooms_GiveLaughingFungus(void) {
  struct Actor *felix;

  Event_SetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  felix = Actor_Get(ACTOR_FELIX);
  if (felix->z.fixed > toFIXED(95))
    Actor_WalkToAndWait(ACTOR_FELIX, felix->x.fixed / FIXED_ONE, 95);
  Actor_WalkToAndWait(ACTOR_FELIX, 362, 95);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_SetMessageId((s32)MsgMadraRoomsOhMy);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Oh, my... Is... Is that..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
  Actor_ShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 3);
  Actor_FaceActor(ACTOR_MUSHROOM_COOK, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  // "Is that one of his most favorite mushrooms!?"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 5);
  FieldEvent_SetMessageId((s32)MsgMadraRoomsNoDearTheyLookSimilar);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirectionAndWait(ACTOR_MUSHROOM_LOVER, FACING_EAST);
  FieldEvent_Wait(10);
  // "No, dear... They look similar, but these aren't quite right."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
  Actor_FaceEachOther(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldEvent_Wait(20);
  // "Thanks for bringing this by, but it's just not the type of mushroom..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
  Actor_FaceActor(ACTOR_MUSHROOM_COOK, ACTOR_FELIX, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
  FieldEvent_Wait(5);
  // "Please... Won't you please try to find the mushrooms my husband..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 5);
  Event_End();
}

/// A party member walks into Felix and vanishes, as the party gathers up to
/// leave the mayor's house.
static inline void MadraRooms_WalkIntoLeader(s32 actorId) {
  struct Actor *leader;

  Actor_SetSpeed(actorId, 1.2);
  Actor_SetAnimation(actorId, ANIM_WALK);
  leader = Actor_Get(ACTOR_FELIX);
  if (leader != NULL)
    Actor_SetDestination(actorId, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  FieldActor_WaitForMoveById(actorId);
  Actor_SetPosition(actorId, 0, 0);
}

/// Walking into the mayor's house when Felix brings Piers home (cells (44, 5)
/// and (44, 6), a few steps in from the door), the mayor cries "Felix! You're
/// back!" and his household welcomes Piers home, asking after his jewel and
/// apologising for his imprisonment.
void MadraRooms_WelcomePiersHome(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgMadraRoomsFelixYoureBack);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(ACTOR_MAYOR, ACTOR_FELIX, 0);
  // "Felix! You're back!"
  FieldEvent_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_FaceActor(ACTOR_ELDERS_DAUGHTER, ACTOR_FELIX, 0);
  Actor_FaceDirection(ACTOR_ELDERS_GRANDSON, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceActor(ACTOR_ELDERS_GRANDDAUGHTER, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_JEWEL_KEEPER, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_ELDERS_DAUGHTER, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(ACTOR_FELIX, 701, 93);
  Actor_WalkBy(ACTOR_FELIX, -32, 0);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -8, 32, FACING_WEST);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_SOUTH_SOUTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -8, 16, FACING_SOUTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, -24, 16, FACING_SOUTHWEST);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "And you've brought Piers with you!"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ELDER, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ELDER, 2);
  FieldEvent_Wait(20);
  // "Oh! Piers!"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Piers, I must apologize for the trouble we've caused you. I'm sorry..."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_FaceEachOther(ACTOR_PIERS, ACTOR_FELIX, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Don't be. Everything worked out for the best."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldActor_Jump(ACTOR_ELDERS_GRANDDAUGHTER, 6, 20);
  FieldEvent_Wait(20);
  // "Piers, you said you were going to go to Gondowan. Did you make it?"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "Yes. In fact, we have just returned."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "And your jewel..." Yes: "You actually got it back from the Kibombo?
  // Incredible..." No: "You didn't recover it from the Kibombo?"
  Event_OpenMessage(ACTOR_MAYOR, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "You actually got it back from the Kibombo? Incredible..."
    FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
    Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    FieldEvent_Wait(20);
    // "I was able to recover it with the help of Felix and his friends."
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "That's wonderful! I'm so relieved! I was worried that you might..."
    FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
    gFieldState->message += 3;
  } else {
    Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    MadraRooms_CountAnswer();
    gFieldState->message += 3;
    // "You didn't recover it from the Kibombo?"
    FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
    Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    FieldEvent_Wait(20);
    // "That's not true, sir. Felix and his companions helped me recover it."
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    FieldActor_StartRepeatedMotion(ACTOR_ELDERS_DAUGHTER, 3);
    Actor_SetAttachedEffect(ACTOR_ELDERS_DAUGHTER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(50);
    // "Please! Don't startle me like that! I was afraid you might never..."
    FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
  }
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  // "That's why we came back... To let you know that we've recovered it..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  if (Event_GetFlag(FLAG_PIERS_SETTING_SAIL) != 0)
    MadraRooms_SayFarewellAgain();
  else
    MadraRooms_AskToStayLonger();
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  MadraRooms_WalkIntoLeader(ACTOR_KRADEN);
  MadraRooms_WalkIntoLeader(ACTOR_SHEBA);
  MadraRooms_WalkIntoLeader(ACTOR_PIERS);
  MadraRooms_WalkIntoLeader(ACTOR_JENNA);
  Event_SetFlag(FLAG_MADRA_FAREWELL_SAID);
  Event_End();
}

/// The welcome's end when Piers has already agreed to sail for Lemuria: the
/// household asks "Ah, so you'll be leaving again, will you?"
void MadraRooms_SayFarewellAgain(void) {
  FieldEvent_SetMessageId((s32)MsgMadraRoomsAhSoYoullBeLeaving);
  // "Ah, so you'll be leaving again, will you?"
  FieldEvent_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_FaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "Are you sure you must leave so soon, Piers?"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "I may yet return here again, but I'm afraid I must be going soon."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "You seem to be quite busy."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  // "It is their youth that allows them to be so busy."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "But we'll be back, I promise. Maybe we'll be able to visit at a more..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
  FieldEvent_Wait(20);
  // "I'd like that..."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDDAUGHTER, ANIM_NOD);
  FieldEvent_Wait(20);
  // "So would I!"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
}

/// The welcome's end when Piers has not yet agreed to sail: the household asks
/// the party to stay a little longer now that the jewel is recovered, and
/// Piers explains why he cannot.
void MadraRooms_AskToStayLonger(void) {
  FieldEvent_SetMessageId((s32)MsgMadraRoomsIfYouveAlreadyRecovered);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
  FieldEvent_Wait(20);
  // "If you've already recovered your jewel, why don't you stay here for..."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  // "Unfortunately, we can't..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_JENNA, 0);
  FieldEvent_Wait(20);
  // "Hey, have you seen the girl out front?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ELDERS_GRANDSON, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_ELDERS_GRANDSON, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "We must go...to the island that is my home."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_PIERS, 0);
  // "Piers, are you... Are you sure?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
  FieldEvent_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "After what we've been through, I feel I've learned a few things."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_PIERS, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Like what!?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_SHEBA, 0);
  FieldEvent_Wait(20);
  // "Like...the fact that you truly are good people."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  // "And that you have gone to great lengths for no other reason than to..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "I have come to realize that I, too, must do something to help where..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Piers..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
  // "I have made my decision."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
  FieldEvent_Wait(20);
  // "And the sooner we depart, the better, right, Kraden?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  // "We shall set sail, Felix! To my ship! To the Eastern Sea!"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Event_SetFlag(FLAG_PIERS_SETTING_SAIL);
  Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "Do you truly mean to leave so soon, Piers?"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "I may yet again have reason to visit your town, but... yes, I must go."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "You seem quite busy."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  // "It is their youth that allows them to be so busy."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  // "But we'll be back, I promise. Maybe we'll be able to visit at a more..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
  FieldEvent_Wait(20);
  // "I'd like that..."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDDAUGHTER, ANIM_NOD);
  FieldEvent_Wait(20);
  // "So would I!"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
}

// clang-format off
/// The old couple's walks into their back room and out again, Char hopping
/// after them, as they fetch the Djinni for the Healing Fungus.
s32 gMadraRooms_WifeWalksToBackRoom[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(356, 72),
    SC_SYNC,
    SC_MOVE(324, 0, 73),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

/// Felix's script as the old man passes: one step aside.
s32 gMadraRooms_LeaderStepsAside[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(339, 111),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

// The same script as gMadraRooms_WifeWalksToBackRoom, under another name.
/// The mushroom lover's script off to the back room to fetch Char.
s32 gMadraRooms_OldManWalksToBackRoom[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(356, 72),
    SC_SYNC,
    SC_MOVE(324, 0, 73),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

/// The mushroom cook's script back to her place.
s32 gMadraRooms_WifeComesBack[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 0, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 109),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/// The mushroom lover's script back from the back room with Char.
s32 gMadraRooms_OldManComesBack[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 0, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 97),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/// Char's script once the old man gives him to the party: he follows Felix.
s32 gMadraRooms_CharFollows[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 24, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 97),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(MadraRoomsHouses),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, MadraRoomsHouses, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, MadraRoomsHouses, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, MadraRoomsHouses, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, MadraRoomsHouses, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHEAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, MadraRoomsHouses, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, MadraRoomsHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, MadraRoomsHouses, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, MadraRoomsHouses, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHEAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(MadraRoomsCavernHomes),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST, Madra, MADRA_FROM_MADRA_ROOMS_CAVERN_HOMES_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_ROOMS_CAVERN_HOMES_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// Madra's houses, shops, inn and sanctum until the party returns with Piers
/// (enum MadraRoomsActorId, then MadraRoomsTravelersActorId).
struct ActorPlacement gMadraRooms_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 184, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 109, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 96, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 615, 0, 311, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 364, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 348, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 556, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 624, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 648, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 604, 0, 548, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 432, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 518, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CHAR), MOVEMENT_TYPE_WANDER, 368, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 676, 0, 762, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 730, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 847, 0, 711, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 906, 0, 760, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// Madra's houses once the party returns with Piers (enum
/// MadraRoomsActorId, then MadraRoomsReturnActorId).
struct ActorPlacement gMadraRooms_PlacementsOnReturn[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 184, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 109, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 96, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 88, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 620, 0, 152, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 152, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 552, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 624, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 648, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 604, 0, 548, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 432, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 518, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CHAR), MOVEMENT_TYPE_WANDER, 368, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 614, 0, 111, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 636, 0, 111, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 676, 0, 762, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 730, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The jail (enum MadraRoomsJailActorId).
struct ActorPlacement gMadraRooms_JailPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 522, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 504, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 362, 0, 847, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MOVEMENT_TYPE_STAND, 394, 0, 863, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MOVEMENT_TYPE_STAND, 396, 0, 848, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The left cavern home (enum MadraRoomsCavernHomeActorId).
struct ActorPlacement gMadraRooms_SecondRoomsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 128, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 192, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 356, 0, 109, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, IF_FLAG_SET(FLAG_BRIGGS_CAUGHT), MOVEMENT_TYPE_STAND, 357, 0, 131, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The houses' talk and Mind Read before the Kibombo raid. The exits without a
/// name lead from one of the map's rooms to another, 3 to 4, 6 to 7, 11 to 12
/// and 13 to 14 and back; 16 and 17 have no row in gSceneExits.
struct Trigger gMadraRooms_TriggersBeforeRaid[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsLandsToSouthOfGondowan)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, EVENT(MadraRooms_AskWillWeBeSafe)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWaveBigEnoughToMove)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEverythingHappenedSoSuddenlyEarth)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheySayThatBoatBelongs)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsBoatThatRanAground)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMiladyAndIHaveBeen)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherWasOpposed)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenElderGetsBack)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsDadAndGrandpaHaveGone)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWheneverDadGoesOnTrips)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeMustPassThisTest)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWaveThatMovesVeryEarth)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurInnIsQuiteSafe)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeThoseChampaDont)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(MadraRooms_GuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsISleptRightThroughPirate)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImGladMyPartner)},
    {TRIGGER_TALK, ACTOR_MORGAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWereHeadingToTolbi)},
    {TRIGGER_TALK, ACTOR_DEKKA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHeyIfYouSeeGladiator)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAreWarlikeButThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheySayThereArePeople)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfMythicalSeaCreatureHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEverythingsOnFloorAndAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThatBoatsFarSturdier)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsItDidntEvenHaveAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWishIKnewWhy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsUntilMyFatherReturnsIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHeIntendsToFindBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfDadHasToFight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeHeBringsMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHowCouldOneExplainContinent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfThisIsTestPlease)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsRaidWasJustAnotherPart)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsNewLandBridgesBetweenContinents)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsActuallyIThink)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsNoOneWasSafe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWasHidingUnder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeWereFineBecauseOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MORGAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsGladiatorWhoTakesPrize)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEKKA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeFoughtInColossoLast)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {TRIGGER_TABLE_END},
};

/// The jail's talk and Mind Read, by the story flags.
struct Trigger gMadraRooms_JailTriggers[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_WEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_WEST},
    {TRIGGER_TALK, ACTOR_SHIN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsOhPiersAfterWay)},
    {TRIGGER_TALK, ACTOR_SHINS_FRIEND, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsShinsDevelopedTerribleFear)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TALK_TRIGGER_FACING(FACING_NORTHWEST), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TRIGGER_TALK, ACTOR_MAN_FOCUSING_ON_ICE, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), EVENT(MadraRooms_TalkToManFocusingOnIce)},
    {TRIGGER_TALK, ACTOR_SHIN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersWasntOneOfChampa)},
    {TRIGGER_TALK, ACTOR_SHINS_FRIEND, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSoThatPiersGuyWasnt)},
    {TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToPiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsEvenSoundOfNamePiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHINS_FRIEND, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsIUnderstandWhyShinsScared)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgSoThisFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FOCUSING_ON_ICE, IF_FLAG_SET(FLAG_MADRA_FOCUSING_MAN_GROANED), MSGNO(MsgMadraIKnewIShouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FOCUSING_ON_ICE | TRIGGER_SUBJECT_OWN_EFFECT, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), EVENT(MadraRooms_TalkToManFocusingOnIce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThereWasSomethingReallyFrightening)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHINS_FRIEND, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThatPiersWasStrangeFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS, CONDITION_ALWAYS, EVENT(MadraRooms_MindReadPiers)},
    {TRIGGER_TABLE_END},
};

/// The left cavern home's talk and Mind Read, by the story flags.
struct Trigger gMadraRooms_SecondRoomsTriggers[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST, CONDITION_ALWAYS, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST},
    {TRIGGER_EXIT, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST, CONDITION_ALWAYS, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsInAllThisChaosEveryone)},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsSoBriggsAndHisPirates)},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheresTalkOfMightyTravelers)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsMadraAndAlhafraOff)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsItWasSureSneaky)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSinceThingsAreFinallyCalming)},
    {TRIGGER_TALK, ACTOR_SISTER_IN_BED, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOhhOhhh)},
    {TRIGGER_TALK, ACTOR_HER_SISTER, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsImSoHappy)},
    {TRIGGER_TALK, ACTOR_HER_SISTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveToldMySisterNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsWaitSecondIllBetBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsNowThatBriggsHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhatIfWeJustTraveled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsAlhafraHasLongBeenTrade)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsItSoundsLikeOsenia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsBusinessPlanOurPeopleHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SISTER_IN_BED, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEyesShiningInDarknessNo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HER_SISTER, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsIWasSoScared)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HER_SISTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsShinsNormallySoKind)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingTandooriChicken))},
    {TRIGGER_TABLE_END},
};

/// The houses' talk and Mind Read once the Kibombo have raided Madra.
struct Trigger gMadraRooms_TriggersAfterRaid[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAreVeryWarlikeTribe)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImGladKibombosAttackWasnt)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboApparentlyUsedRopes)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyHusbandPursuedKibomboAfter)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersLeftThatBoat)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantStopThinkingAbout)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAllKibomboWantedWhenThey)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherWasHolding)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersWentToKibombo)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfMyFatherdBeen)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherHasntComeHome)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfThatTidalWaveHadnt)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThemKibomboPossessEvilPowers)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThingsHaveBeenBusyHere)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHidUnderMyBed)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(MadraRooms_GuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWatchedEntireAttack)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfYouWantToKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurBestGuess)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhatCanWeDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfIWereJustLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsInHisYouthMyHusband)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSomePeopleInTownSaw)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWonderIfThatShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenMyHusbandHears)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWontEverForgetLook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenPiersLeftHeSaid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsDadAlwaysKnowsWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeHesJustLate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopePowerOfOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEveryoneSaysThemKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWereLuckyKibomboDidntUse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheresNothingWorseThanWicked)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfItWereMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAfterICrawledUnderBed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHadToGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAttackedInWeeHours)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {TRIGGER_TABLE_END},
};

/// The houses' talk and Mind Read once the party returns with Piers, the
/// welcome, and the mushroom trade.
struct Trigger gMadraRooms_TriggersOnReturn[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_ROOMS_WELCOME, FLAG_MADRA_FAREWELL_SAID, EVENT(MadraRooms_WelcomePiersHome)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurSalesmanSaidKibombosCeremony)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWonderIfKibomboHave)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToMushroomLover)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToMushroomCook)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSameSailorsCameThroughRecently)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOneOfSailorsIsaac)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIKnowYouWent)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeJustHaveToFinish)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImSoRelievedPiersGot)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantBelievePiersActually)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsGondowanIsSuchAwfulDangerous)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsILikeYouGuysLot)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyDaddyWentToGondowan)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOceansBeenWarmingUpLot)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenOceanWarmsEasternSea)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(MadraRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHasSomeoneBeenTelling)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOneOfOurGuestsKept)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(MadraRooms_GuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIGuessThatLot)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveHeardThatRisingOcean)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsContinentShiftMakesItFeel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSalesmanThatJustCameBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, EVENT(MadraRooms_MindReadMushroomLover)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, EVENT(MadraRooms_MindReadMushroomCook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOarsIUnderstandHeyIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeveHadLotOfStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAllOfThisCouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhyWontMayorOfAlhafra)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantBelieveItDespite)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIFeltLittleResponsible)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersSaidHeWasGoing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWishTheydTellMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImSoGladDaddyGot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIKnewTemperaturesWereRising)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurNewShipIsSpectacular)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSeafaringTownsKnowManyMore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveNeverSeenSingleCustomer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSomeoneMustHaveSeenMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsItMustBeVeryStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfIHadBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHmIWonderIfTidal)},
    {USE_ITEM_ON_ACTOR(ITEM_HEALING_FUNGUS), ACTOR_MUSHROOM_LOVER, FLAG_MADRA_HEALING_FUNGUS_GIVEN, EVENT(MadraRooms_GiveHealingFungus)},
    {USE_ITEM_ON_ACTOR(ITEM_LAUGHING_FUNGUS), ACTOR_MUSHROOM_LOVER, FLAG_MADRA_HEALING_FUNGUS_GIVEN, EVENT(MadraRooms_GiveLaughingFungus)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {TRIGGER_TABLE_END},
};

/// Shin and then his friend run out of the jail, crying for help.
s32 gMadraRooms_ShinAndFriendRunAway[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(408, 0, 872),
    SC_SYNC,
    SC_MOVE(408, 0, 904),
    SC_SYNC,
    SC_MOVE(408, 0, 920),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Shin falls over, a quarter turn in eight frames.
s32 gMadraRooms_ShinFalls[] = {
    SC_CALL(MadraRooms_RotateSprite),
    SC_WAIT(1),
    SC_CALL(MadraRooms_RotateSprite),
    SC_WAIT(1),
    SC_CALL(MadraRooms_RotateSprite),
    SC_WAIT(1),
    SC_CALL(MadraRooms_RotateSprite),
    SC_WAIT(1),
    SC_CALL(MadraRooms_RotateSprite),
    SC_WAIT(1),
    SC_CALL(MadraRooms_RotateSprite),
    SC_WAIT(1),
    SC_CALL(MadraRooms_RotateSprite),
    SC_WAIT(1),
    SC_CALL(MadraRooms_RotateSprite),
    SC_WAIT(1),
    SC_END,
};

/// Piers' shaft of ice lifts Shin and holds him bobbing in the air.
s32 gMadraRooms_ShinLifted[] = {
    SC_SET(SW_MOTION, 0),
    SC_WAIT(100),
    SC_WAIT(36),
    SC_ADD(SW_Y, toFIXED(3)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(5)),
    SC_ADD(SW_X, toFIXED(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(9),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ADD(SW_X, toFIXED(1)),
    SC_ADD(SW_Y, toFIXED(5)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_ADD(SW_X, toFIXED(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ADD(SW_X, toFIXED(1)),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
// clang-format on
