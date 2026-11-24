/// @file
/// Mikasalla's houses and, as a second scene, the sanctums of Mikasalla, the
/// Apojii Islands and Yallam.
///
/// Behind the village's doors are the inn (the northeast door), where Galahad
/// waits in a linked game, the general store (northwest), the house of an old
/// man who keeps trying to climb Air's Rock (east) and the house of the family
/// of the explorer who founded the village (southwest). The same scene holds
/// the old couple's house on the East Tundaria Islet.

#include "shared/tables.h"
#include "sprite_ids.h"
#include "flags.h"
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
#include "system/main_routine_state.h"
#include "scene_ids.h"
#include "field/actor.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "field/field_sequence.h"
#include "text/msg_ids.h"

/// Set once Galahad has met the party and warned it of Isaac's enemies.
#define FLAG_MIKASALLA_GALAHAD_MET 0xa39
/// Set if the party told Galahad it knows Isaac.
#define FLAG_MIKASALLA_TOLD_GALAHAD_OF_ISAAC FLAG_AREA_TEMP_40
/// Set as Galahad meets the party, for this visit: he stays in the inn until
/// the party leaves the area.
#define FLAG_MIKASALLA_GALAHAD_MET_THIS_VISIT FLAG_AREA_TEMP_41

/// Mikasalla's houses and inn (MikasallaRoomsHouses), by the rows of
/// gMikasallaRooms_HousesPlacements.
enum MikasallaRoomsActorId {
  /// The family of the explorer who founded the village: his son, the son's
  /// wife, and the grandson who asks whether the leader is an explorer too.
  ACTOR_EXPLORERS_SON = ACTOR_FIRST_PLACED,
  ACTOR_EXPLORERS_DAUGHTER_IN_LAW,
  ACTOR_EXPLORERS_GRANDSON,
  /// An old man who keeps trying to climb Air's Rock, and his wife, worried
  /// that he will be hurt.
  ACTOR_OLD_MAN_ON_AIRS_ROCK,
  ACTOR_WORRIED_WIFE,
  /// The keeper of the general store: "We only have a small shop here."
  ACTOR_GENERAL_STOREKEEPER,
  /// A man at the inn: "There aren't any towns between here and Garoh."
  ACTOR_MAN_ON_THE_ROAD_TO_GAROH,
  /// A woman in a pink apron behind the inn's counter.
  ACTOR_INNKEEPER,
  /// An old couple by the fire, who talk of Tundaria.
  ACTOR_OLD_MAN_ON_TUNDARIA,
  ACTOR_OLD_WOMAN_BY_THE_FIRE,
  /// Galahad, in the inn in a linked game.
  ACTOR_GALAHAD
};

/// The sanctums: Mikasalla's, and those of the Apojii Islands and Yallam,
/// which this scene also holds; by the rows of
/// `gMikasallaRooms_SanctumPlacements`.
enum MikasallaRoomsSanctumActorId {
  /// The healers of Mikasalla's and the Apojii Islands' sanctums.
  ACTOR_MIKASALLA_HEALER = ACTOR_FIRST_PLACED,
  ACTOR_APOJII_HEALER,
  /// Two Apojii islanders tired of their island paradise.
  ACTOR_ISLANDER_SICK_OF_PARADISE,
  ACTOR_ISLANDER_LONGING_FOR_LAND,
  /// The healer of Yallam's sanctum.
  ACTOR_YALLAM_HEALER,
  /// Blank actors on the counters of Mikasalla's and the Apojii sanctums, so
  /// the healers answer from across them.
  ACTOR_MIKASALLA_COUNTER,
  ACTOR_APOJII_COUNTER
};

/// Mikasalla's shop and inn, as `Shop_Run` and `Inn_CheckIn` number them.
enum { SHOP_MIKASALLA = 13, MIKASALLA_INN = 4 };

extern struct ActorPlacement gMikasallaRooms_HousesPlacements[];
extern struct ActorPlacement gMikasallaRooms_SanctumPlacements[];
extern struct Trigger gMikasallaRooms_HousesTriggers[];
extern struct Trigger gMikasallaRooms_SanctumTriggers[];

void Shop_Run();

void Scene_FadeInAndClearFlag(void);

extern s32 gMikasallaRooms_DaughterInLawPaces[];
extern s32 gMikasallaRooms_WorriedWifePaces[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_MIKASALLA_ROOMS_SANCTUM)
    return gMikasallaRooms_SanctumPlacements;
  return gMikasallaRooms_HousesPlacements;
}

/// Felix casts Mind Read on the counter of Mikasalla's Sanctum and reads the
/// healer behind it: "I wonder if there are werewolves beyond those in
/// Garoh..."
s32 MikasallaRooms_ReadHealersMind(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaRoomsIWonderIfThere);
  // "I wonder if there are werewolves beyond those in Garoh... I also..."
  Event_ShowMessage(ACTOR_MIKASALLA_HEALER, 0);
  Event_End();
  return 0;
}

/// Felix casts Mind Read on the counter of the Apojii Islands' Sanctum and
/// reads the healer behind it: "Whenever visitors come out, the islanders start
/// to complain..."
s32 MikasallaRooms_ReadApojiiHealersMind(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId(
      (s32)MsgMikasallaRoomsWheneverVisitorsComeOutIslanders);
  // "Whenever visitors come out, the islanders start to complain about..."
  Event_ShowMessage(ACTOR_APOJII_HEALER, 0);
  Event_End();
  return 0;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_MIKASALLA_ROOMS_SANCTUM)
    return gMikasallaRooms_SanctumTriggers;
  return gMikasallaRooms_HousesTriggers;
}

void Sanctum_Run();

/// The boy in a blue shirt in the explorer's family's house, behind the
/// village's southwest door, asks Felix: "Hey, mister! Are you an explorer?"
/// Yes, and he wants to be one too someday; no, and he says his grandpa
/// travelled the whole world.
void MikasallaRooms_AskAreYouExplorer(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaRoomsHeyMisterAreYouExplorer);
  // "Hey, mister! Are you an explorer?"
  Event_OpenMessage(ACTOR_EXPLORERS_GRANDSON, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId((s32)MsgMikasallaRoomsHeyMisterAreYouExplorer + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId((s32)MsgMikasallaRoomsHeyMisterAreYouExplorer + 2);
  }
  Event_ShowMessage(ACTOR_EXPLORERS_GRANDSON, 0);
  Event_End();
}

/// Felix talks to the woman behind the counter of Mikasalla's general store:
/// facing her across it, he can shop; from elsewhere she says the shop used to
/// sell only items.
void MikasallaRooms_TalkToGeneralStorekeeper(s32 speaker) {
  u16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_Get(gMainRoutineState.leaderActorId)->facing);
  if (facing == FACING_NORTH) {
    Shop_Run(SHOP_MIKASALLA, speaker);
  } else {
    FieldEvent_SetMessageId((s32)MsgMikasallaRoomsWeOnlyHaveSmallShop);
    // "We only have a small shop here. I used to specialize in items... But..."
    Event_ShowMessage(speaker, 0);
  }
}

/// Felix talks to the woman in a pink apron behind the inn's counter: facing
/// her across it, he can stay the night; from elsewhere she bids the party
/// rest its feet.
void MikasallaRooms_TalkToInnkeeper(s32 speaker) {
  u16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_Get(gMainRoutineState.leaderActorId)->facing);
  if (facing == FACING_NORTH) {
    Inn_CheckIn(MIKASALLA_INN, speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaRoomsYouveComeLongWayFriends);
    // "You've come a long way, friends. Stay a while, and rest your feet."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the healer of Mikasalla's Sanctum: across the counter, he
/// can use the Sanctum; from elsewhere the healer asks whether he has been to
/// Garoh.
void MikasallaRooms_TalkToHealer(s32 speaker) {
  u16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_Get(gMainRoutineState.leaderActorId)->facing);
  if (facing == FACING_NORTH) {
    Sanctum_Run(speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaRoomsHaveYouBeenToGaroh);
    // "Have you been to Garoh?"
    Event_OpenMessage(speaker, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgMikasallaRoomsHaveYouBeenToGaroh + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgMikasallaRoomsHaveYouBeenToGaroh + 2);
    }
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the healer of the Apojii Islands' Sanctum: across the
/// counter, he can use the Sanctum; from elsewhere the healer says the little
/// island must seem boring to him.
void MikasallaRooms_TalkToApojiiHealer(s32 speaker) {
  u16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_Get(gMainRoutineState.leaderActorId)->facing);
  if (facing == FACING_NORTH) {
    Sanctum_Run(speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaRoomsThisLittleIslandMustSeem);
    // "This little island must seem quite boring to you sophisticated..."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the healer of Yallam's Sanctum: across the counter, he can
/// use the Sanctum; from elsewhere the healer talks of the far-off shores.
void MikasallaRooms_TalkToYallamHealer(s32 speaker) {
  u16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_Get(gMainRoutineState.leaderActorId)->facing);
  if (facing == FACING_NORTH) {
    Sanctum_Run(speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgEvenOutInFarOffShores);
    // "Even out in these far-off shores, people need the services and hope..."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Opens the rooms around the leader. In the houses, Galahad stands in the inn
/// in a game linked to one where Isaac won at Colosso, from his meeting with
/// the party until it leaves the area, and before the party is reunited. In
/// the sanctums, the healers of Mikasalla and the Apojii Islands answer from
/// across their counters, and the Apojii and Yallam sanctums, which this
/// scene holds, show no area name.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineState.next == SCENE_MIKASALLA_ROOMS_HOUSES) {
    if (gMainRoutineState.entrance ==
        MIKASALLA_ROOMS_HOUSES_FROM_EAST_TUNDARIA_ISLET)
      Scene_FadeInAndClearFlag();
    else if (!Event_GetFlag(FLAG_TRANSFERRED_WON_COLOSSO) ||
             (Event_GetFlag(FLAG_MIKASALLA_GALAHAD_MET) &&
              !Event_GetFlag(FLAG_MIKASALLA_GALAHAD_MET_THIS_VISIT)) ||
             Event_GetFlag(FLAG_REUNION))
      Actor_SetPosition(ACTOR_GALAHAD, 0, 0);
    else
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_GALAHAD), SPRITE_COLOR_GBR);
    return 0;
  }
  if (gMainRoutineState.next == SCENE_MIKASALLA_ROOMS_SANCTUM) {
    s32 entrance = gMainRoutineState.entrance;

    if (entrance == MIKASALLA_ROOMS_SANCTUM_FROM_MIKASALLA) {
      Actor_Get(ACTOR_MIKASALLA_HEALER)->collisionFlags |= ACTOR_NEAR_TALK;
      FieldActor_SetSpritePriority(ACTOR_MIKASALLA_HEALER, 2);
    } else if (entrance == MIKASALLA_ROOMS_SANCTUM_FROM_APOJII_ISLANDS) {
      Event_ClearFlag(FLAG_SHOW_AREA_NAME);
      Actor_Get(ACTOR_APOJII_HEALER)->collisionFlags |= ACTOR_NEAR_TALK;
      FieldActor_SetSpritePriority(ACTOR_APOJII_HEALER, 2);
    } else if (entrance == MIKASALLA_ROOMS_SANCTUM_FROM_YALLAM) {
      Event_ClearFlag(FLAG_SHOW_AREA_NAME);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// Arriving from the East Tundaria Islet, the rooms open around the leader and
// show no area name.
#include "shared/scene_fade_in_and_clear_flag.h"

void MikasallaRooms_MeetGalahad(void);

/// Felix talks to Galahad, the warrior in a golden helmet in Mikasalla's inn
/// in a linked game: the first time, he introduces himself; after that, he
/// thinks Isaac is in the Eastern Sea if the party said it knows him, and
/// otherwise says Isaac's enemies are gathering.
void MikasallaRooms_TalkToGalahad(void) {
  if (Event_GetFlag(FLAG_MIKASALLA_GALAHAD_MET)) {
    // "I think Isaac must be somewhere in the Eastern Sea right now..." or
    // "Isaac's enemies are gathering now, to destroy him and erase their
    // shame."
    FieldEvent_SetMessageId(
        Event_GetFlag(FLAG_MIKASALLA_TOLD_GALAHAD_OF_ISAAC)
            ? (s32)MsgMikasallaRoomsIThinkIsaacMust
            : (s32)MsgMikasallaRoomsIsaacsEnemiesAreGatheringNow);
    Event_ShowMessage(ACTOR_GALAHAD, 0);
  } else {
    MikasallaRooms_MeetGalahad();
  }
}

/// Felix casts Mind Read on Galahad in Mikasalla's inn: before they have met,
/// Galahad introduces himself; after, he fears for Isaac.
void MikasallaRooms_MindReadGalahad(void) {
  if (!Event_GetFlag(FLAG_MIKASALLA_GALAHAD_MET)) {
    MikasallaRooms_MeetGalahad();
  } else {
    FieldEvent_SetMessageId((s32)MsgMikasallaRoomsIFearWhatMightHappen);
    // "I fear what might happen if the three find Isaac before I can warn..."
    Event_ShowMessage(ACTOR_GALAHAD, 0);
  }
}

/// Galahad introduces himself, asks whether the party knows Isaac, and warns
/// that the three gladiators Isaac beat are seeking revenge.
void MikasallaRooms_MeetGalahad(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaRoomsForgiveMyManners);
  Actor_ShowEmote(ACTOR_GALAHAD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_GALAHAD, gMainRoutineState.leaderActorId, 0);
  // "Forgive my manners..."
  Event_ShowMessage(ACTOR_GALAHAD, 0);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetAnimation(ACTOR_GALAHAD, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(15);
  // "No, please... Do not fear me."
  Event_ShowMessage(ACTOR_GALAHAD, 0);
  Actor_ShowEmote(ACTOR_GALAHAD, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  // "I am the warrior Galahad. I am looking for someone, and I need your..."
  Event_OpenMessage(ACTOR_GALAHAD, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    // "You do? I must warn Sir Isaac that he is in danger most grievous......"
    Event_ShowMessage(ACTOR_GALAHAD, 0);
    Event_SetFlag(FLAG_MIKASALLA_TOLD_GALAHAD_OF_ISAAC);
  } else {
    gFieldState->message += 1;
    // "A pity... The one I seek won a battle at Colosso, in Tolbi. He..."
    Event_ShowMessage(ACTOR_GALAHAD, 0);
  }
  Event_SetFlag(FLAG_MIKASALLA_GALAHAD_MET);
  Event_SetFlag(FLAG_MIKASALLA_GALAHAD_MET_THIS_VISIT);
  Event_End();
}

// clang-format off
/// The explorer's son's wife paces her house, stopping to look east.
s32 gMikasallaRooms_DaughterInLawPaces[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(600, 0, 88),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(616, 0, 104),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// The worried wife paces her house, stopping to look east.
s32 gMikasallaRooms_WorriedWifePaces[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(360, 0, 88),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(376, 0, 104),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(MikasallaRoomsHouses),
    SCENE_EXIT_UNLESS(MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_NORTHEAST, Mikasalla,
                      MIKASALLA_FROM_MIKASALLA_ROOMS_HOUSES_NORTHEAST,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_NORTHWEST, Mikasalla,
                      MIKASALLA_FROM_MIKASALLA_ROOMS_HOUSES_NORTHWEST,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_EAST, Mikasalla,
                      MIKASALLA_FROM_MIKASALLA_ROOMS_HOUSES_EAST,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_SOUTHWEST, Mikasalla,
                      MIKASALLA_FROM_MIKASALLA_ROOMS_HOUSES_SOUTHWEST,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_ROOMS_HOUSES_TO_EAST_TUNDARIA_ISLET,
                      EastTundariaIslet,
                      EAST_TUNDARIA_ISLET_FROM_MIKASALLA_ROOMS_HOUSES,
                      EXIT_FLAG_NONE),
    SCENE_EXITS(MikasallaRoomsSanctum),
    SCENE_EXIT_UNLESS(MIKASALLA_ROOMS_SANCTUM_TO_MIKASALLA, Mikasalla,
                      MIKASALLA_FROM_MIKASALLA_ROOMS_SANCTUM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_ROOMS_SANCTUM_TO_APOJII_ISLANDS, ApojiiIslands,
                      APOJII_ISLANDS_FROM_MIKASALLA_ROOMS_SANCTUM,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_ROOMS_SANCTUM_TO_YALLAM, Yallam,
                      YALLAM_FROM_MIKASALLA_ROOMS_SANCTUM, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// Mikasalla's houses and inn (enum MikasallaRoomsActorId).
struct ActorPlacement gMikasallaRooms_HousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 152, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, gMikasallaRooms_DaughterInLawPaces, 616, 0, 104, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 136, FACING_NORTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 120, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, gMikasallaRooms_WorriedWifePaces, 360, 0, 88, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 408, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 664, 0, 376, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 376, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The three sanctums (enum MikasallaRoomsSanctumActorId).
struct ActorPlacement gMikasallaRooms_SanctumPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 147, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 466, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 520, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 536, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 472, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 472, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
/// The houses' and inn's talk and Mind Read, Galahad, and what is cooking.
struct Trigger gMikasallaRooms_HousesTriggers[] = {
    {TRIGGER_EXIT, MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_NORTHEAST, CONDITION_ALWAYS, MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_NORTHEAST},
    {TRIGGER_EXIT, MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_NORTHWEST, CONDITION_ALWAYS, MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_NORTHWEST},
    {TRIGGER_EXIT, MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_EAST, CONDITION_ALWAYS, MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_EAST},
    {TRIGGER_EXIT, MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_SOUTHWEST, CONDITION_ALWAYS, MIKASALLA_ROOMS_HOUSES_TO_MIKASALLA_SOUTHWEST},
    {TRIGGER_EXIT, MIKASALLA_ROOMS_HOUSES_TO_EAST_TUNDARIA_ISLET, CONDITION_ALWAYS, MIKASALLA_ROOMS_HOUSES_TO_EAST_TUNDARIA_ISLET},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_EXPLORERS_SON, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsIHeardSomeoneSayTheres)},
    {TRIGGER_TALK, ACTOR_EXPLORERS_DAUGHTER_IN_LAW, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsMyFatherInLaw)},
    {TRIGGER_TALK, ACTOR_EXPLORERS_GRANDSON, CONDITION_ALWAYS, EVENT(MikasallaRooms_AskAreYouExplorer)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsThoseWhoHaveClimbedAirs)},
    {TRIGGER_TALK, ACTOR_WORRIED_WIFE, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsMyHusbandHasTried)},
    {TRIGGER_TALK, ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS, EVENT(MikasallaRooms_TalkToGeneralStorekeeper)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_ROAD_TO_GAROH, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsThereArentAnyTownsBetween)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(MikasallaRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsUsedToBeYouCould)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_BY_THE_FIRE, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsYouMustBeColdDressed)},
    {TRIGGER_TALK, ACTOR_GALAHAD, CONDITION_ALWAYS, EVENT(MikasallaRooms_TalkToGalahad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EXPLORERS_SON, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsItMustBeNice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EXPLORERS_DAUGHTER_IN_LAW, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsThatBoySureAdmiresHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EXPLORERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsILoveExploringOutBehind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsWhatDoesUnseeableEvenLook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_WIFE, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsICantLetHimTry)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsIDontKnowMuchAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_ROAD_TO_GAROH, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsTheresNoReasonToGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsWhyAreTravelersAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsTundariaIsSouthernmostContinent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_BY_THE_FIRE, CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsAroundHereItDoesntMatter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GALAHAD, CONDITION_ALWAYS, EVENT(MikasallaRooms_MindReadGalahad)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaRoomsItsRoastChickenSoBig))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaRoomsFreshEggsOverEasySome))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaRoomsThatCheeseLooksFabulous))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaRoomsTheresFishIveNeverSeen))},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The three sanctums' healers, the islanders and the counters' Mind Read.
struct Trigger gMikasallaRooms_SanctumTriggers[] = {
    {TRIGGER_EXIT, MIKASALLA_ROOMS_SANCTUM_TO_MIKASALLA, CONDITION_ALWAYS,
     MIKASALLA_ROOMS_SANCTUM_TO_MIKASALLA},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, MIKASALLA_ROOMS_SANCTUM_TO_YALLAM, CONDITION_ALWAYS,
     MIKASALLA_ROOMS_SANCTUM_TO_YALLAM},
    {TRIGGER_EXIT, MIKASALLA_ROOMS_SANCTUM_TO_APOJII_ISLANDS, CONDITION_ALWAYS,
     MIKASALLA_ROOMS_SANCTUM_TO_APOJII_ISLANDS},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_MIKASALLA_HEALER, CONDITION_ALWAYS,
     EVENT(MikasallaRooms_TalkToHealer)},
    {TRIGGER_TALK, ACTOR_APOJII_HEALER, CONDITION_ALWAYS,
     EVENT(MikasallaRooms_TalkToApojiiHealer)},
    {TRIGGER_TALK, ACTOR_ISLANDER_SICK_OF_PARADISE, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaRoomsImSickOfLiving)},
    {TRIGGER_TALK, ACTOR_ISLANDER_LONGING_FOR_LAND, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaRoomsIdLoveToGo)},
    {TRIGGER_TALK, ACTOR_YALLAM_HEALER, CONDITION_ALWAYS,
     EVENT(MikasallaRooms_TalkToYallamHealer)},
    {TRIGGER_TALK, ACTOR_MIKASALLA_COUNTER, CONDITION_ALWAYS,
     EVENT(MikasallaRooms_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIKASALLA_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsIWonderIfThere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_APOJII_HEALER,
     CONDITION_ALWAYS,
     MSGNO(MsgMikasallaRoomsWheneverVisitorsComeOutIslanders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ISLANDER_SICK_OF_PARADISE,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsMaybeTheyllTakeMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ISLANDER_LONGING_FOR_LAND,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsIWonderWhatItsLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YALLAM_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaRoomsOnceThisWasThrivingVillage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIKASALLA_COUNTER,
     CONDITION_ALWAYS, EVENT(MikasallaRooms_ReadHealersMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_APOJII_COUNTER,
     CONDITION_ALWAYS, EVENT(MikasallaRooms_ReadApojiiHealersMind)},
    {TRIGGER_TABLE_END},
};
