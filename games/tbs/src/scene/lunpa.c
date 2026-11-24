/// @file
/// Lunpa, the town the righteous thief Lunpa founded, and the Lunpa Fortress
/// Gates above it. While Dodonpa, Donpa's son, holds Hammet in Lunpa Fortress,
/// his men keep the town sealed: two guards stand outside the shut gate in the
/// log palisade at the south end, where the road from the World Map ends, and
/// turn the party away. The way in is through West Lunpa Cave, whose gate Frost
/// opens; it comes out at the cave mouth in the town's north-west cliff (cell
/// (7, 4)).
///
/// Two areas share this overlay, each a scene of its own:
///
/// - Lunpa (SceneId_Lunpa): the town. Doors lead into three homes, the weapon
///   and armor shop, the item shop, the inn, the temple and the crate room
///   (RockRooms, rock_rooms.c). The road out of the top of the town (cells
///   (13, 0) to (16, 0)) climbs to the Lunpa Fortress Gates.
/// - Lunpa Fortress Gates (SceneId_LunpaFortressGates): the slope up to the
///   golden fortress door in the cliff. Two log palisades cross it, each with
///   a gap in the middle, and two guards stand in the yard between them, just
///   below the upper gap. The party slips past them under Cloak.
///
/// The getters choose by gMainRoutineState.next. The town's triggers also
/// choose by FLAG_DODONPA_LOCKED_UP, which Donpa's judgment in the fortress
/// sets. Before it the townspeople live under Dodonpa's men
/// (gLunpaSealedTriggers). After it they talk of the intruders in the
/// fortress (gLunpaReopenedTriggers). Both tables share the roads and doors
/// (LUNPA_ROADS_AND_DOORS) and Reveal's secrets (LUNPA_SECRETS).
///
/// Reveal turns the town grey and shows three secrets
/// (Lunpa_RevealSecrets): a blue puddle between the two log pillars below the
/// road to the gates, which Frost freezes into an ice pillar; a sparkle on the
/// cliff at the west edge of town (cell (3, 16)), the switch that opens the
/// passage to Lunpa's hidden room, searched from cell (3, 18) below it; and a
/// Psynergy Stone, found by searching cell (7, 16), south of the inn. Every
/// entry and the end of Reveal hide them again
/// (Lunpa_HideSecrets). Searching the sparkle opens a dark cave mouth in the
/// cliff beside it, and the passage can be walked into only while Reveal
/// shows it (Lunpa_EnterHiddenPassage). The room beyond (StoneRooms) holds
/// six chests.
///
/// At the gates, walking uncloaked into the yard before the guards
/// (CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND) makes them step into
/// the upper gap and warn the party off. An uncloaked leader in that gap, or
/// one still in the yard as Cloak fades, is caught and marched back down
/// through the lower palisade (Lunpa_GuardsCatchParty).
///
/// A party caught inside the fortress is thrown out by the guards and down
/// the slope (Lunpa_GuardsThrowIsaacOut). After the rescue it comes out of
/// the fortress door and Isaac sneaks past the guards under Cloak
/// (Lunpa_LeaderSneaksOut).
///
/// Every arrival at the gates clears the fortress's puzzle and beaten-guard
/// flags, so the fortress starts over.
///
/// Between Donpa's judgment and the meeting with Bunza outside West Lunpa
/// Cave (FLAG_LUNPA_CAVE_REUNION_SEEN), Garet stops Isaac from going back up
/// through the upper gap (Lunpa_GaretTurnsIsaacBack).

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "sounds.h"
#include "types.h"
#include "field/event.h"
#include "field/field_effect.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"
#include "scene/field_effect_spawn.h"
#include "system/main_routine_state.h"
#include "system/system_call.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gLunpaEntrances[];
extern struct SceneEntrance gGateEntrances[];
extern u32 gLunpaExits[];
extern u32 gGateExits[];
extern struct ActorPlacement gLunpaActorPlacements[];
extern struct ActorPlacement gGateActorPlacements[];
extern struct Trigger gLunpaSealedTriggers[];
extern struct Trigger gLunpaReopenedTriggers[];
extern struct Trigger gGateTriggers[];

// Lunpa's and the gates' entrances and exits are in scene_exits.h.

/// Unused: the exit of a third road out of the gates
/// (CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD), which no cell of the
/// gates' map has. gGateExits has no row for it either: Scene_TakeExit,
/// reaching the end of the gates' exits, would fall back to the event of
/// their first exit at ENTRANCE_DEFAULT, LunpaFortressGroundFloor's default
/// entrance.
enum { GATE_EXIT_THIRD = 3 };

/// The first of the rooms in RockRooms that Lunpa's doors lead into.
enum { ROOM_ENTRANCE_FIRST = 1 };

/// The RockRooms entrance a door's exit leads to.
#define ROOM_ENTRANCE(exit) ((exit) - LUNPA_TO_FIRST_HOME + ROOM_ENTRANCE_FIRST)

/// The cell behaviors Lunpa's triggers answer.
enum LunpaCellBehavior {
  CELL_BEHAVIOR_LUNPA_WORLD_MAP_ROAD = 1,
  CELL_BEHAVIOR_LUNPA_GATE_ROAD,
  CELL_BEHAVIOR_LUNPA_CAVE_ROAD,
  CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR,
  CELL_BEHAVIOR_LUNPA_SECOND_HOME_DOOR,
  CELL_BEHAVIOR_LUNPA_THIRD_HOME_DOOR,
  CELL_BEHAVIOR_LUNPA_ARMS_SHOP_DOOR,
  CELL_BEHAVIOR_LUNPA_ITEM_SHOP_DOOR,
  CELL_BEHAVIOR_LUNPA_INN_DOOR,
  CELL_BEHAVIOR_LUNPA_TEMPLE_DOOR,
  CELL_BEHAVIOR_LUNPA_CRATE_ROOM_DOOR,
  CELL_BEHAVIOR_LUNPA_HIDDEN_PASSAGE,
  CELL_BEHAVIOR_LUNPA_HIDDEN_SWITCH = 20,
  CELL_BEHAVIOR_LUNPA_WELL = 90,
  CELL_BEHAVIOR_LUNPA_GRAVE_SLEEP_BOMB = 100,
  CELL_BEHAVIOR_LUNPA_BARREL_NUT,
  CELL_BEHAVIOR_LUNPA_PSYNERGY_STONE
};

/// The cell behaviors the Lunpa Fortress Gates' triggers answer. The gates'
/// map has no cell of the third road, the fortress approach or the well: the
/// approach is raised by Lunpa_WatchForFortressApproach instead, and the other
/// two rows never fire.
enum LunpaFortressGatesCellBehavior {
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_DOOR = 1,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_LUNPA_ROAD,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND = 10,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH = 20,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_WELL = 90
};

/// The event the gates' guards raise on spotting the uncloaked party.
enum LunpaFortressGatesRaisedTrigger { RAISED_PARTY_SPOTTED = 91 };

/// Lunpa's doors that open as the leader walks in (gLunpaDoors), counted from
/// the first home's; at the temple's he goes through without first stepping
/// into the doorway.
enum {
  DOOR_COUNT = 8,
  DOOR_TEMPLE = CELL_BEHAVIOR_LUNPA_TEMPLE_DOOR -
      CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR
};

/// Lunpa's placed actors, numbered in the order of gLunpaActorPlacements
/// from ACTOR_FIRST_PLACED. The townspeople are named by their sprites and by
/// what they say before Donpa's judgment.
enum LunpaActorId {
  /// The blue puddle Reveal shows and Frost freezes into an ice pillar.
  ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  ACTOR_WOMAN_WITH_NO_FREEDOM,
  /// A stout man with brown hair (SPRITE_STOUT_MAN).
  ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders.
  ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
  /// An old woman with white hair (SPRITE_OLD_WOMAN).
  ACTOR_OLD_WOMAN_ON_DONPA,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR).
  ACTOR_BOY_ASKING_TO_SEE_DONPA,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who
  /// wanders.
  ACTOR_GIRL_ON_THE_CHEFS_MEALS,
  /// The two soldiers in spiked helmets (SPRITE_SOLDIER_IN_SPIKED_HELMET);
  /// this one keeps the road in from the World Map.
  ACTOR_WEST_GUARD,
  ACTOR_EAST_GUARD, ///< wants Dodonpa's authorization
  /// The hidden switch's sparkle, shown by Reveal.
  ACTOR_SWITCH_SPARKLE,
  /// The Nut that floats over the town, for Catch.
  ACTOR_FLOATING_NUT
};

/// The Lunpa Fortress Gates' placed actors, numbered in the order of
/// gGateActorPlacements from ACTOR_FIRST_PLACED. Garet's row, the third,
/// takes his own actor, GARET, as every row of a party member's sprite does.
enum GateActorId { ACTOR_LEFT_GUARD = ACTOR_FIRST_PLACED, ACTOR_RIGHT_GUARD };

/// How the switch's sparkle is stretched while Reveal shows it.
enum {
  SPARKLE_SCALE_X = 6 * FIXED_ONE / 5,
  SPARKLE_SCALE_Y = 3 * FIXED_ONE / 2
};

/// Where Retreat at the gates sends the party
/// (MainRoutineState.retreatEntrance).
enum { GATE_RETREAT_ENTRANCE = 10 };

/// The gate guards' lines when they catch the party uncloaked, in order.
enum CatchMessage {
  CATCH_LEFT_GUARD_CHALLENGES,
  CATCH_RIGHT_GUARD_WONDERS,
  CATCH_LEFT_GUARD_REFUSES_ENTRY,
  CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
};

/// The left guard's two lines once he has seen Hammet with the party.
enum RecognitionMessage { RECOGNITION_SEEN_THAT_MAN, RECOGNITION_IMPOSSIBLE };

/// The leader lying where the guards threw him down the steps.
enum { ANIM_SPRAWLED = 19 };

/// How high the ice pillar's top is, and a cell: 16 pixels each.
enum { PILLAR_TOP_HEIGHT = toFIXED(16), CELL_SIZE = toFIXED(16) };

/// The treasure spot of the town's Psynergy Stone.
enum { TREASURE_SPOT_PSYNERGY_STONE = 102 };

extern struct MapGridCellAnimation gLunpaDoors[];

static void Lunpa_WatchForUncloakedParty(void);

/// Unused: nothing in the scene reads them.
enum { PALETTE_TRANSLUCENT_EFFECT = 14, PALETTE_EFFECT = 15 };

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// Frost has frozen the hidden puddle into an ice pillar to stand on.
#define FLAG_LUNPA_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/// The town's secrets are hidden: Reveal is not shining.
#define FLAG_LUNPA_SECRETS_HIDDEN FLAG_SCENE_TEMP_01
/// The hidden switch has opened the passage to the hidden room.
#define FLAG_LUNPA_HIDDEN_PASSAGE_OPEN FLAG_SCENE_TEMP_02
/// The guards have stepped in front of the fortress door.
#define FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR FLAG_SCENE_TEMP_40
/// Cloak has been cast at the gates and has not yet faded.
#define FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST FLAG_SCENE_TEMP_41
/// The leader is going through the fortress door.
#define FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS FLAG_SCENE_TEMP_42
/// Garet's warning against going back up to the fortress is under way.
#define FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING FLAG_SCENE_TEMP_43
/// The guards have caught the party and are marching it down the steps.
#define FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY FLAG_SCENE_TEMP_44

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  s32 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_LUNPA) {
    return gLunpaEntrances;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_GATES) {
    return gGateEntrances;
  }
  return gLunpaEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) {
  s32 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_LUNPA) {
    return gLunpaExits;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_GATES) {
    return gGateExits;
  }
  return gLunpaExits;
}

/// The townsfolk of Lunpa, or the guards of the Lunpa Fortress Gates.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_LUNPA) {
    return gLunpaActorPlacements;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_GATES) {
    return gGateActorPlacements;
  }
  return gLunpaActorPlacements;
}

/// Catch on the Nut in the tree just west of Lunpa's shut south gate, outside
/// the palisade, brings it down to Isaac.
static void Lunpa_CatchFloatingNut(void) {
  Scene_CatchNut(ACTOR_FLOATING_NUT, FLAG_LUNPA_NUT_CAUGHT);
}

/// Makes the frozen puddle's cell, (14, 4), and the cells either side of it
/// into the ice pillar's top and sides, from copies kept at (17, 4) and
/// (15, 3).
static inline void Lunpa_ApplyFrozenPuddleCollision(void) {
  MapGrid_CopyCellAttributes(17, 4, 1, 1, 14, 4);
  MapGrid_CopyCellAttributes(15, 3, 1, 1, 15, 4);
  MapGrid_CopyCellAttributes(15, 3, 1, 1, 13, 4);
}

/// Frost on the blue puddle that Reveal shows between the two log pillars at
/// the top of Lunpa (cell (14, 4)) freezes it into an ice pillar to stand on.
static void Lunpa_FreezeHiddenPuddle(void) {
  struct Actor *pillar;

  pillar = Actor_Get(ACTOR_HIDDEN_PUDDLE);
  // DEAD: the leader looked up and dropped; the ROM keeps the call.
  Actor_Get(ACTOR_PARTY_LEADER);
  Lunpa_ApplyFrozenPuddleCollision();
  if (pillar != NULL) {
    Actor_SetSpriteFlags(pillar, 0);
    pillar->motionFlags = ACTOR_FALLS;
    pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
  GameFlag_Set(FLAG_LUNPA_PUDDLE_FROZEN);
}

/// Every frame while Reveal shows the puddle: once Isaac is up on the ice
/// pillar, it is drawn under him.
static void Lunpa_UpdateIcePillarDrawOrder(void) {
  if (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
    Actor_Get(ACTOR_HIDDEN_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Actor_Get(ACTOR_HIDDEN_PUDDLE)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Draws one picture of the hidden passage's cave mouth in the cliff at the
/// west edge of town, from a set of copies kept at (sourceColumn, sourceRow).
/// The second and third copies are the same mouth on the map's other layers.
static inline void Lunpa_DrawHiddenPassagePicture(s32 sourceColumn,
                                                  s32 sourceRow) {
  MapGrid_CopyCells(sourceColumn, sourceRow, 3, 4, 1, 14);
  MapGrid_CopyCells(sourceColumn + 3, sourceRow, 3, 4, 33, 14);
  MapGrid_CopyCells(sourceColumn + 6, sourceRow, 3, 4, 1, 46);
}

/// Casting Reveal in Lunpa turns the town grey and shows its secrets: the blue
/// puddle between the log pillars at the top of town, the sparkle of the
/// hidden switch on the west cliff (or, once it has been pressed, the cave
/// mouth it opened), and the Psynergy Stone's spot at cell (7, 16). Isaac
/// standing on that spot is first walked a step south off it.
static void Lunpa_RevealSecrets(void) {
  struct Actor *actor;
  s32 cellX;
  s32 cellZ;

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  cellX = actor->x.fixed / CELL_SIZE;
  cellZ = actor->z.fixed / CELL_SIZE;
  if (GameFlag_Get(FLAG_LUNPA_PSYNERGY_STONE) == 0) {
    if (cellX == 7 && cellZ == 16) {
      Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
    }
    TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
    // The stone's cell takes the attributes kept for it at (28, 31).
    MapGrid_CopyCellAttributes(28, 31, 1, 1, 7, 16);
  }
  // The puddle's cell on the middle layer (the map's columns from 32), and the
  // cells round it, (13, 3) to (15, 5), take the copies kept for Reveal.
  MapGrid_CopyCells(47, 4, 1, 1, 46, 4);
  MapGrid_CopyCellAttributes(34, 37, 3, 3, 13, 3);
  Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, 232, 72);
  Actor_Get(ACTOR_HIDDEN_PUDDLE)->y.fixed = 0;
#if TBS_PATCH_4
  if (GameFlag_Get(FLAG_LUNPA_PUDDLE_FROZEN)) {
    Lunpa_ApplyFrozenPuddleCollision();
  }
#endif
  // The opened passage replaces the sparkle; otherwise show the hidden switch.
  if (GameFlag_Get(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
    Lunpa_DrawHiddenPassagePicture(41, 49);
  } else {
    Actor_SetPosition(ACTOR_SWITCH_SPARKLE, 56, 268);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SWITCH_SPARKLE), 0);
    actor = Actor_Get(ACTOR_SWITCH_SPARKLE);
    if (actor != NULL) {
      actor->motionFlags = ACTOR_FLOATS_HIGH;
      actor->y.fixed = toFIXED(16);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->scaleX = SPARKLE_SCALE_X;
      actor->scaleY = SPARKLE_SCALE_Y;
    }
  }
  Task_AddFrame(Lunpa_UpdateIcePillarDrawOrder, TASK_PASS_FRAME);
  GameFlag_Clear(FLAG_LUNPA_SECRETS_HIDDEN);
}

/// As Reveal ends, and on every arrival in Lunpa, the puddle, the sparkle, the
/// cave mouth and the Psynergy Stone's spot are hidden again, and a frozen
/// puddle thaws.
static void Lunpa_HideSecrets(void) {
  // DEAD: the leader looked up and dropped; the ROM keeps the call.
  Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetPosition(ACTOR_SWITCH_SPARKLE, 0, 0);
  Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, 0, 0);
  // The puddle's cell and the cells round it as they are without it, from the
  // copies kept at (38, 38) and (37, 37), whose attributes (14, 2) takes too.
  MapGrid_CopyCells(38, 38, 1, 1, 46, 4);
  MapGrid_CopyCellAttributes(37, 37, 3, 3, 13, 3);
  MapGrid_CopyCellAttributes(37, 37, 1, 1, 14, 2);
  // The stone's cell plain again, as the cell east of it.
  MapGrid_CopyCellAttributes(8, 16, 1, 1, 7, 16);
  TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, 0, 0);
  // The cave mouth's lower cells, (1, 15) to (3, 16), shut again from the
  // copy kept at (32, 42).
  MapGrid_CopyCells(32, 42, 3, 2, 1, 15);
  GameFlag_Clear(FLAG_LUNPA_PUDDLE_FROZEN);
  Actor_SetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
  Actor_Get(ACTOR_HIDDEN_PUDDLE)->update = NULL;
  Actor_SetColorMode(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
  Task_Remove(Lunpa_UpdateIcePillarDrawOrder);
  GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
}

/// Searching the well in the middle of Lunpa (cells (16, 13) to (17, 14)),
/// Isaac peers in and hears frogs inside. The gates' table has a row for a
/// well too, but no cell there answers it.
static void Lunpa_SearchWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgLunpaWellFrogs, 1);
  Event_End();
}

/// The triggers of the Lunpa Fortress Gates, or the town's before and after
/// Donpa's judgment.
const struct Trigger *Scene_GetTriggers(void) {
  s32 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_LUNPA_FORTRESS_GATES) {
    // DEAD: the flag read and dropped; the ROM keeps the call.
    GameFlag_Get(FLAG_DODONPA_LOCKED_UP);
    return gGateTriggers;
  }
  if (sceneId == SCENE_LUNPA && GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    return gLunpaReopenedTriggers;
  }
  return gLunpaSealedTriggers;
}

/// Before Donpa's judgment, the guard on the left of Lunpa's shut south gate
/// winces and asks whether the party wants to enter Lunpa, and sends them back
/// where they came from whatever they answer.
static void Lunpa_TalkToWestGuard(void) {
  Event_Start();
  Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId(MsgLunpaWestGuardAsksAboutEntering);
  // "What's with you? You wanna enter Lunpa?"
  // Yes: "Well, you can't! Go back to where you came from!"
  // No: "You came all this way for nothing? That doesn't make any..."
  Event_AskYesNo(ACTOR_WEST_GUARD, 0);
  Event_End();
}

/// Before Donpa's judgment, the guard on the right of Lunpa's shut south gate
/// says no one may enter without Dodonpa's authorization. Once Hammet's
/// caravan has gone north to Lunpa (FLAG_LUNPA_GUARDS_SUSPECT_KALAY), he asks
/// instead whether the party is from Kalay, and then whether they have come to
/// save Hammet, and warns them off by their answers.
static void Lunpa_TalkToEastGuard(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
    Event_SetMessageId(MsgLunpaEastGuardDemandsAuthorization);
    // "This is Lunpa. You cannot enter without Dodonpa's authorization."
    Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  } else {
    Event_SetMessageId(MsgLunpaEastGuardAsksIfFrom);
    // "You're not from Kalay, are you?"
    Event_OpenMessage(ACTOR_EAST_GUARD, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      gFieldState->message++;
      // "You're not here to save Hammet, are you?"
      Event_OpenMessage(ACTOR_EAST_GUARD, 0);
      if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
        gFieldState->message++;
      }
    }
    Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  }
  Event_End();
}

/// Mind Read on the guard on the right of Lunpa's south gate. Before Donpa's
/// judgment he fears Dodonpa will blame the guards for any trouble outsiders
/// cause, or, once Hammet's caravan has come, trusts that the gate in the cave
/// opens only from the inside; after it he thinks the merchant is harmless.
static void Lunpa_ReadEastGuardsThoughts(void) {
  s32 thoughts;

  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) == 0) {
    thoughts = GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY);
    if (thoughts == 0) {
      // "If outsiders cause any trouble in the village, Dodonpa will..."
      thoughts = MsgLunpaEastGuardFearsBlame;
    } else {
      // "Even if they enter through the cave, they can't get into..."
      thoughts = MsgLunpaEastGuardTrustsCaveGate;
    }
    Event_SetMessageId(thoughts);
    Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  } else {
    Event_SetMessageId(MsgLunpaEastGuardThinksMerchantHarmless);
    // "That merchant was desperate to do business in the village...."
    Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  }
}

/// Before Donpa's judgment, a young man with brown hair near the inn asks
/// whether the party has been in Lunpa long, and takes a No to mean they are
/// new members of Dodonpa's gang.
static void Lunpa_AskAboutLengthOfStay(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaVillagerAAsksHowLong);
  // "Have you been in Lunpa long?"
  // Yes: "Hmm... Your face doesn't ring a bell. I guess you don't..."
  // No: "Oh, so you're a new member of Dodonpa's gang?"
  Event_AskYesNo(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, 0);
  Event_End();
}

/// Before Donpa's judgment, a stout man in Lunpa asks whether it is true that
/// Dodonpa kidnapped Hammet and extorted ransom for him.
static void Lunpa_AskAboutHammetsKidnapping(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaVillagerCAsksAboutKidnapping);
  // "Is it true that Dodonpa kidnapped Hammet and extorted ransom..."
  // Yes: "Then I guess Dodonpa's men were telling the truth."
  // No: "I didn't think so. Not even Dodonpa would do such a terrible..."
  Event_AskYesNo(ACTOR_MAN_ON_HAMMETS_KIDNAPPING, 0);
  Event_End();
}

/// Before Donpa's judgment, a small boy at the top of Lunpa asks whether the
/// party wants to see Donpa: he is in the fortress, or, on a No, impossible to
/// see now.
static void Lunpa_AskAboutSeeingDonpa(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaVillagerGAsksAboutDonpa);
  // "You want to see Donpa?"
  // Yes: "Donpa is in the fortress. You'll have to go there if you..."
  // No: "I didn't think so... It's impossible to see Donpa now."
  Event_AskYesNo(ACTOR_BOY_ASKING_TO_SEE_DONPA, 0);
  Event_End();
}

/// After Donpa's judgment, a woman with fair hair in Lunpa shivers and
/// complains that it has gotten colder.
static void Lunpa_ComplainAboutCold(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_WOMAN_WITH_NO_FREEDOM, 3);
  Event_SetMessageId(MsgLunpaVillagerBShivers);
  // "It's gotten colder... I'm shiverin' just standin' here."
  Event_ShowMessage(ACTOR_WOMAN_WITH_NO_FREEDOM, 0);
  Event_End();
}

/// After Donpa's judgment, a woman in a green dress in Lunpa offers to tell
/// what caused the commotion at the fortress: someone snuck in to save Hammet
/// and ran away.
static void Lunpa_AskAboutFortressCommotion(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaVillagerDAsksAboutCommotion);
  // "Do you want to know what caused the commotion at the fortress?"
  // Yes: "Someone snuck into the fortress to save the merchant Hammet!..."
  // No: "They're foolhardy to invade such a well- guarded fortress!..."
  Event_AskYesNo(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD, 0);
  Event_End();
}

/// Talking to the left guard at the Lunpa Fortress Gates under Cloak, he
/// could swear he just heard someone. Uncloaked, he grumbles that Dodonpa is
/// devious; after Donpa's judgment, until the meeting with Bunza, he thinks
/// he has seen the man with the party before, then decides it is impossible.
static void Lunpa_TalkToLeftGuard(void) {
  s32 recognitionMessageBase;

  if (gMainRoutineState.cloaked != 0) {
    Event_SetMessageId(MsgLunpaLeftGuardHearsSomeone);
  } else if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0 &&
             GameFlag_Get(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    recognitionMessageBase = MsgLunpaLeftGuardRecognizesHammet;
    Event_SetMessageId(recognitionMessageBase + RECOGNITION_SEEN_THAT_MAN);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Event_SetMessageId(recognitionMessageBase + RECOGNITION_IMPOSSIBLE);
    GameFlag_Set(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
  } else {
    Event_SetMessageId(MsgLunpaLeftGuardResentsDodonpa);
  }
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

/// Talking to the right guard at the Lunpa Fortress Gates, he boasts that not
/// even a mouse could get in while he is on watch. Under Cloak he answers his
/// partner that the place is kind of creepy; after Donpa's judgment he
/// wonders how the warriors from Kalay got in.
static void Lunpa_TalkToRightGuard(void) {
  if (gMainRoutineState.cloaked != 0) {
    Event_SetMessageId(MsgLunpaRightGuardFeelsCreepy);
  } else if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_SetMessageId(MsgLunpaRightGuardWondersHow);
  } else {
    Event_SetMessageId(MsgLunpaRightGuardBoasts);
  }
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/// Mind Read on the left guard at the Lunpa Fortress Gates: he thinks keeping
/// Hammet to squeeze money out of Kalay is genius. After Donpa's judgment,
/// until the meeting with Bunza, he turns to Isaac sure he has seen that man
/// with him before (later, that the man following them looks like that
/// merchant), then dismisses the thought.
static void Lunpa_ReadLeftGuardsThoughts(void) {
  s32 recognizedHammet;

  if (gMainRoutineState.cloaked == 0 &&
      GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0 &&
      GameFlag_Get(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    recognizedHammet = Event_GetFlag(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    if (recognizedHammet == 0) {
      gFieldState->psynergy = recognizedHammet;
      FieldPsynergy_StopMindReadWave();
      Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
      Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Event_SetMessageId(MsgLunpaLeftGuardRecognizesHammet);
      GameFlag_Set(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    } else {
      Event_SetMessageId(MsgLunpaLeftGuardGlimpsedMerchant);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Event_SetMessageId(MsgLunpaLeftGuardDismissesThought);
  } else {
    Event_SetMessageId(MsgLunpaLeftGuardThoughts);
  }
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

/// Mind Read on the right guard at the Lunpa Fortress Gates: you would have
/// to be a shadow to sneak past him. After Donpa's judgment he can't see how
/// anyone got by them.
static void Lunpa_ReadRightGuardsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_SetMessageId(MsgLunpaRightGuardReopenedThoughts);
  } else {
    Event_SetMessageId(MsgLunpaYoudShadowSneak);
  }
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/// Every frame at the Lunpa Fortress Gates between Donpa's judgment and the
/// meeting with Bunza: Isaac stepping into the upper palisade's gap (map row
/// 10) raises Garet's warning, Lunpa_GaretTurnsIsaacBack.
static void Lunpa_WatchForFortressApproach(void) {
  s32 leaderRow = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed / CELL_SIZE;

  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING) == 0 &&
      leaderRow == 10) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING);
    gFieldState->stepTrigger =
        CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH;
  }
}

/// After Donpa's judgment, walking up into the gap in the upper palisade at
/// the Lunpa Fortress Gates, Garet reminds Isaac they can't go in there, and
/// Isaac winces and steps back.
static void Lunpa_GaretTurnsIsaacBack(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_SetMessageId(MsgLunpaGaretRefusesToReturn);
  // "Isaac! Don't be stupid! We can't go in there, remember?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 100);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING);
  Event_End();
}

/// Walking north into one of Lunpa's doors, it swings open and Isaac goes
/// through into the home, shop, inn, temple or crate room behind it. The
/// townspeople stand still meanwhile.
static void Lunpa_EnterDoor(void) {
  struct FieldState *state;
  struct Actor *placedActor;
  u32 actorId;
  s32 doorIndex;

  state = gFieldState;
  Event_Start();
  // Freeze placed actors before opening the selected door and crossing
  // it.
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
    placedActor = Actor_Get(actorId);
    if (placedActor != NULL) {
      placedActor->motionFlags = 0;
    }
  }
  Sound_Play(SE_DOOR_OPEN);
  doorIndex = state->stepTrigger - CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR;
  MapGrid_AnimateCells(gLunpaDoors[doorIndex].steps, gLunpaDoors[doorIndex].x,
                       gLunpaDoors[doorIndex].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (doorIndex != DOOR_TEMPLE) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
    Event_Wait(10);
  }
  Import_EventSetExitRequest(state->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Walking north into the cave mouth the hidden switch opened in Lunpa's west
/// cliff (cell (2, 19)) takes Isaac into Lunpa's hidden room. It works only
/// while Reveal shows the mouth; otherwise nothing happens there.
static void Lunpa_EnterHiddenPassage(void) {
  if (GameFlag_Get(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
    if (GameFlag_Get(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
      Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
      Event_Wait(13);
      Import_EventSetExitRequest(LUNPA_TO_STONE_ROOMS);
    }
  }
}

/// Walking up to the golden door in the cliff at the top of the Lunpa
/// Fortress Gates (cells (9, 5) and (10, 5)), it opens and Isaac goes into
/// Lunpa Fortress.
static void Lunpa_EnterFortress(void) {
  GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS);
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -8);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_CopyCells(53, 4, 2, 2, 41, 4);
  Event_Wait(10);
  MapGrid_CopyCells(53, 6, 2, 2, 41, 4);
  Event_Wait(10);
  Import_EventSetExitRequest(
      LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

static void UNUSED Lunpa_TryJumpForward(void) { Field_TryJumpForward(); }

/// Unused: plays the treasure jingle while Reveal is shining. Nothing calls
/// it.
UNUSED static void Lunpa_PlayRevealCue(void) {
  if (GameFlag_Get(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
    Sound_Play(MUS_TREASURE_FOUND);
  }
}

/// Searching the sparkle Reveal shows on Lunpa's west cliff, from cell (3, 18)
/// below it, makes the rock crumble away into a dark cave mouth, the passage
/// to Lunpa's hidden room.
static void Lunpa_PressHiddenSwitch(void) {
  if (GameFlag_Get(FLAG_LUNPA_SECRETS_HIDDEN) != 0) {
    return;
  }
  if (Event_GetFlag(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
    return;
  }
  Actor_SetPosition(ACTOR_SWITCH_SPARKLE, 0, 0);
  Sound_Play(SE_HIDDEN_PASSAGE_OPEN);
  wait(1);
  Lunpa_DrawHiddenPassagePicture(32, 45);
  wait(10);
  Lunpa_DrawHiddenPassagePicture(41, 45);
  wait(10);
  Lunpa_DrawHiddenPassagePicture(50, 45);
  wait(10);
  Lunpa_DrawHiddenPassagePicture(32, 49);
  wait(10);
  Lunpa_DrawHiddenPassagePicture(41, 49);
  wait(10);
  GameFlag_Set(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN);
}

/// Copies the cell at (sourceColumn, sourceRow) over the three cells (7, 11)
/// to (9, 11) at the Lunpa Fortress Gates, just below the west half of the
/// upper palisade.
static inline void Lunpa_CopyGatewayCollision(s32 sourceColumn, s32 sourceRow) {
  MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 7, 11);
  MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 8, 11);
  MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 9, 11);
}

/// Walking uncloaked into the yard between the palisades at the Lunpa
/// Fortress Gates (cells (7, 11) to (14, 14)), the two guards cry out, step up
/// into the upper palisade's gap and warn the party not to come any closer to
/// the fortress. The three cells west of them are rewritten as Cloak's cast
/// does.
static void Lunpa_GuardsBlockGate(void) {
  if (gMainRoutineState.cloaked == 0) {
    Event_Start();
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 2);
    Actor_ShowEmote(ACTOR_RIGHT_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 15);
    Event_Wait(30);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 168);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 168);
    Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
    Actor_WaitForMoveById(ACTOR_RIGHT_GUARD);
    Actor_Stop(ACTOR_LEFT_GUARD);
    Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STILL);
    Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
    Actor_Stop(ACTOR_RIGHT_GUARD);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STILL);
    Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
    Event_SetMessageId(MsgLunpaGuardsWarnPartyAway);
    // "Don't come any closer to the fortress!"
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR);
    Lunpa_CopyGatewayCollision(6, 11);
    Event_End();
  }
}

static void UNUSED Lunpa_DoNothing(void) {}

/// Casting Cloak at the Lunpa Fortress Gates rewrites the three cells (7, 11)
/// to (9, 11) from the one at (6, 11); Lunpa_CloakFades puts them back.
static void Lunpa_CastCloak(void) {
  Lunpa_CopyGatewayCollision(6, 11);
  GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST);
}

/// When the guards at the Lunpa Fortress Gates spot the party, they turn and
/// shout who are you, wonder how anyone got past them, and march Isaac back
/// out through the gap in the lower palisade.
static void Lunpa_GuardsCatchParty(void) {
  struct Actor *leader;
  s32 warningMessageBase;

  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY) != 0) {
    return;
  }
  GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY);
  Event_Start();
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
  Actor_StartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // Warn the party before escorting everyone down the two sections of the gate.
  warningMessageBase = MsgLunpaGuardsCatchParty;
  Event_SetMessageId(warningMessageBase + CATCH_LEFT_GUARD_CHALLENGES);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_LEFT_GUARD, 2);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, 2);
  Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
  Event_Wait(35);
  Event_SetMessageId(warningMessageBase + CATCH_RIGHT_GUARD_WONDERS);
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
  Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_SetMessageId(warningMessageBase + CATCH_LEFT_GUARD_REFUSES_ENTRY);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
  Event_Wait(25);
  Event_SetMessageId(warningMessageBase + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
  Actor_WalkTo(ACTOR_LEFT_GUARD, leader->x.part[0].integer - 1,
               leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 216);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 200);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 200);
  Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
  Actor_WaitForMoveById(ACTOR_RIGHT_GUARD);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
  Event_Wait(12);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 272);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 256);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 256);
  Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
  Actor_WaitForMoveById(ACTOR_RIGHT_GUARD);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_End();
  Task_AddFrame(Lunpa_WatchForUncloakedParty, TASK_PASS_FRAME);
}

/// Puts back the three cells (7, 11) to (9, 11) that Cloak and the guards at
/// the door rewrite, from the cell at (7, 12).
static void Lunpa_ReopenGateway(void) { Lunpa_CopyGatewayCollision(7, 12); }

/// As Cloak fades at the Lunpa Fortress Gates, a party still in the yard
/// between the palisades is caught by the guards.
static void Lunpa_CloakFades(void) {
#if TBS_PATCH_1
  struct FieldState *state;
#endif
  struct Actor *leader;

#if TBS_PATCH_1
  state = gFieldState;
#endif
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST);
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader->x.fixed > toFIXED(104) && leader->x.fixed < toFIXED(240) &&
      leader->z.fixed > toFIXED(160) && leader->z.fixed < toFIXED(248)) {
    Task_Remove(Lunpa_WatchForUncloakedParty);
#if TBS_PATCH_1
    state->raisedTrigger = RAISED_PARTY_SPOTTED;
#else
    Lunpa_GuardsCatchParty();
#endif
  }
  Lunpa_ReopenGateway();
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY);
}

/// Every frame at the Lunpa Fortress Gates: an uncloaked Isaac in the gap of
/// the upper palisade is spotted by the guards.
static void Lunpa_WatchForUncloakedParty(void) {
  struct FieldState *state;
  struct Actor *leader;

  state = gFieldState;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (gMainRoutineState.cloaked == 0 &&
      (leader->x.fixed >= toFIXED(144) && leader->x.fixed <= toFIXED(176)) &&
      leader->z.fixed >= toFIXED(168) && leader->z.fixed < toFIXED(176)) {
    Task_Remove(Lunpa_WatchForUncloakedParty);
    state->raisedTrigger = RAISED_PARTY_SPOTTED;
  }
}

/// Every 16 frames while Isaac is dragged and thrown: a puff of dust at his
/// feet.
static void Lunpa_KickUpLeaderDust(void) {
  struct Actor *leader;
  s32 dustAngle;
  s32 dustVelocity[3];

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if ((gFrameCount & 15) == 0) {
    dustAngle = (Random_Below(52)) * 64 + 230;
    SET_VECTOR(dustVelocity, Trig_Cos(dustAngle) / 4, 0,
               Trig_Sin(dustAngle) / 2);
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      dustVelocity[0], dustVelocity[1], dustVelocity[2], 0,
                      NULL);
  }
}

/// Caught inside Lunpa Fortress, Isaac is dragged out of the door by the two
/// guards, struggling, down through both palisades, and thrown sprawling onto
/// the slope below. And don't come back, says one, and they return to their
/// posts.
static void Lunpa_GuardsThrowIsaacOut(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_PARTY_LEADER, 160, 128);
  Actor_SetPosition(ACTOR_LEFT_GUARD, 152, 112);
  Actor_SetPosition(ACTOR_RIGHT_GUARD, 168, 112);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#ifdef BUGFIX
  Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
#else
  // BUG: the town's guards, whose actor ids no actor at the gates holds,
  // so the gates' guards keep their headings.
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
#endif
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.8);
  Actor_SetSpeed(ACTOR_LEFT_GUARD, 1.8);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, 1.8);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 288);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 288);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 160, 296);
  Task_AddFrame(Lunpa_KickUpLeaderDust, TASK_PASS_FRAME);
  Event_Wait(1);
  Sound_Play(SE_SCUFFLE);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_LEFT_GUARD, 3);
  Actor_SetSpritePriority(ACTOR_RIGHT_GUARD, 3);
  Sound_Play(SE_SCUFFLE);
  Event_Wait(30);
  Actor_Get(ACTOR_LEFT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Sound_Play(SE_SCUFFLE);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
  Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
  Task_Remove(Lunpa_KickUpLeaderDust);
  // Throw the leader down the steps, then wait for the first landing.
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FALLS;
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = toFIXED(6);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityZ = toFIXED(6);
  Event_Wait(1);
  while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
    Event_Wait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
  Sound_Play(SE_LANDING_THUD);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Task_AddFrame(Lunpa_KickUpLeaderDust, TASK_PASS_FRAME);
  Event_Wait(2);
  // A smaller second bounce ends with the guards' final warning.
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = toFIXED(3);
  Event_Wait(1);
  while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
    Event_Wait(1);
  }
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Lunpa_KickUpLeaderDust);
  Event_Wait(50);
  Event_SetMessageId(MsgLunpaGuardForbidsReturn);
  // "And don't come back!"
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Actor_Get(ACTOR_LEFT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_LEFT_GUARD, 1);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, 1);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 144, 200);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 176, 200);
  Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
  Actor_WaitForMoveById(ACTOR_RIGHT_GUARD);
  Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
  Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Event_End();
}

/// After the rescue, Isaac comes out of the fortress door, casts Cloak in the
/// upper palisade's gap and slips round behind the guards, west along the
/// palisade and down the west side of the slope.
static void Lunpa_LeaderSneaksOut(void) {
  Event_Start();
  Event_StartScreenTransition();
  Actor_WalkTo(ACTOR_PARTY_LEADER, 152, 168);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_Wait(20);
  // Cast Cloak before following the route around the guards.
  FieldPsynergy_LoadResources(ABILITY_CLOAK, 1);
  FieldPsynergy_SetupActorPair(ACTOR_PARTY_LEADER, 0);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  FieldPsynergy_StopCasterUpdate();
  Actor_WalkTo(ACTOR_PARTY_LEADER, 144, 184);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 184);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 200);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 200);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 288);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 288);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_End();
}

/// Sets up Lunpa (secrets hidden, the Nut in its tree) or the Lunpa Fortress
/// Gates (the throw-out or the sneak-out, Garet's watch, the guards' watch and
/// the fortress started over).
s32 Scene_Initialize(void) {
  struct Actor *puddle;

  if (gMainRoutineState.next == SCENE_LUNPA) {
    gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Lunpa_HideSecrets();
    if (GameFlag_Get(FLAG_LUNPA_NUT_CAUGHT) == 0) {
      Scene_FloatNut(ACTOR_FLOATING_NUT);
    }
    puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    if (puddle != NULL) {
      Actor_SetSpriteFlags(puddle, 0);
    }
    GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
  }
  if (gMainRoutineState.next == SCENE_LUNPA_FORTRESS_GATES) {
    gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gMainRoutineState.retreatEntrance = GATE_RETREAT_ENTRANCE;
    if (gMainRoutineState.entrance == LUNPA_FORTRESS_GATES_AFTER_CAUGHT &&
        GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Lunpa_GuardsThrowIsaacOut();
    }
    if (gMainRoutineState.entrance ==
            LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA &&
        GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Lunpa_LeaderSneaksOut();
    }
    if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0 &&
        GameFlag_Get(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
      Task_AddFrame(Lunpa_WatchForFortressApproach, TASK_PASS_FRAME);
    }
#if TBS_PATCH_4
    if (gMainRoutineState.cloaked != 0)
      Lunpa_CastCloak();
#endif
    Task_AddFrame(Lunpa_WatchForUncloakedParty, TASK_PASS_FRAME);
    // The fortress's puzzles and beaten guards are reset.
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_MIDDLE_HALL_GUARD_BEATEN);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_DONPAS_DOOR_GUARD_BEATEN);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_CELL_KEY_ROOM_GUARD_BEATEN);
  }
  return 0;
}

#include "shared/floating_nut.c"

/// An entrance of the town, with its camera free, and of the Fortress gates,
/// its camera kept in their bounds.
#define LUNPA_ENTRANCE(entrance, x, z, facing)                                 \
  {(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, -1, -1, -1, -1, 0}
#define GATE_ENTRANCE(entrance, x, z, facing)                                  \
  {(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, 32, 48, 272, 344, 0}

struct SceneEntrance gLunpaEntrances[] = {
    LUNPA_ENTRANCE(ENTRANCE_DEFAULT, 172, 149, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WORLD_MAP, 224, 472, FACING_NORTH),
    LUNPA_ENTRANCE(LUNPA_FROM_LUNPA_FORTRESS_GATES, 232, 32, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WEST_LUNPA_CAVE, 120, 104, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_FIRST_HOME, 248, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_SECOND_HOME, 344, 232, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_THIRD_HOME, 344, 328, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WEAPON_AND_ARMOR_SHOP, 232, 360, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_ITEM_SHOP, 328, 120, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_INN, 104, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_TEMPLE, 48, 168, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_CRATE_ROOM, 216, 344, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_STONE_ROOMS, 40, 328, FACING_SOUTH),
    {SCENE_TABLE_END},
};

struct SceneEntrance gGateEntrances[] = {
    GATE_ENTRANCE(ENTRANCE_DEFAULT, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR, 160,
                  104, FACING_SOUTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA, 144, 104,
                  FACING_SOUTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_AFTER_CAUGHT, 160, 224, FACING_EAST),
    {SCENE_TABLE_END},
};

/// A door's exit into its room of RockRooms.
#define ROOM_EXIT(exit) SCENE_EXIT((exit), RockRooms, ROOM_ENTRANCE(exit))

u32 gLunpaExits[] = {
    SCENE_EXITS(Lunpa),
    SCENE_EXIT(LUNPA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LUNPA),
    SCENE_EXIT(LUNPA_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates,
               LUNPA_FORTRESS_GATES_FROM_LUNPA),
    SCENE_EXIT(LUNPA_TO_WEST_LUNPA_CAVE, WestLunpaCave,
               WEST_LUNPA_CAVE_FROM_LUNPA),
    ROOM_EXIT(LUNPA_TO_FIRST_HOME),
    ROOM_EXIT(LUNPA_TO_SECOND_HOME),
    ROOM_EXIT(LUNPA_TO_THIRD_HOME),
    ROOM_EXIT(LUNPA_TO_WEAPON_AND_ARMOR_SHOP),
    ROOM_EXIT(LUNPA_TO_ITEM_SHOP),
    ROOM_EXIT(LUNPA_TO_INN),
    ROOM_EXIT(LUNPA_TO_TEMPLE),
    ROOM_EXIT(LUNPA_TO_CRATE_ROOM),
    SCENE_EXIT(LUNPA_TO_STONE_ROOMS, StoneRooms, STONE_ROOMS_FROM_LUNPA),
    SCENE_EXITS_END,
};

u32 gGateExits[] = {
    SCENE_EXITS(LunpaFortressGates),
    SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR,
               LunpaFortressGroundFloor,
               LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES),
    SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA, Lunpa,
               LUNPA_FROM_LUNPA_FORTRESS_GATES),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gLunpaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      520, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      112, 0, 168, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      280, 0, 352, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      120, 0, 312, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      136, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      368, 0, 256, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      280, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      216, 0, 40, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      192, 0, 448, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      256, 0, 448, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      520, 0, 520, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_LUNPA_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
      120, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END, CONDITION_ALWAYS },
};

struct ActorPlacement gGateActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      144, 0, 200, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      176, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

/// A trigger for talking to an actor, for Mind Read on it, and for walking
/// north into a door.
#define TALK(actorId, value)                                                   \
  {TRIGGER_TALK, (actorId), CONDITION_ALWAYS, (value)}
#define MIND_READ(actorId, value)                                              \
  {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), (actorId), CONDITION_ALWAYS, (value)}
#define DOOR(trigger)                                                          \
  {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), (trigger), CONDITION_ALWAYS,       \
   EVENT(Lunpa_EnterDoor)}

/// The roads out of town and the doors, which every one of the town's trigger
/// tables holds.
#define LUNPA_ROADS_AND_DOORS                                                  \
  {TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_WORLD_MAP_ROAD, CONDITION_ALWAYS,         \
   LUNPA_TO_WORLD_MAP},                                                        \
      {TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_GATE_ROAD, CONDITION_ALWAYS,          \
       LUNPA_TO_LUNPA_FORTRESS_GATES},                                         \
      {TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_CAVE_ROAD, CONDITION_ALWAYS,          \
       LUNPA_TO_WEST_LUNPA_CAVE},                                              \
      DOOR(CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR),                               \
      DOOR(CELL_BEHAVIOR_LUNPA_SECOND_HOME_DOOR),                              \
      DOOR(CELL_BEHAVIOR_LUNPA_THIRD_HOME_DOOR),                               \
      DOOR(CELL_BEHAVIOR_LUNPA_ARMS_SHOP_DOOR),                                \
      DOOR(CELL_BEHAVIOR_LUNPA_ITEM_SHOP_DOOR),                                \
      DOOR(CELL_BEHAVIOR_LUNPA_INN_DOOR),                                      \
      DOOR(CELL_BEHAVIOR_LUNPA_TEMPLE_DOOR),                                   \
      DOOR(CELL_BEHAVIOR_LUNPA_CRATE_ROOM_DOOR), {                             \
    STEP_TRIGGER_WALKING_INTO(FACING_NORTH),                                   \
        CELL_BEHAVIOR_LUNPA_HIDDEN_PASSAGE, CONDITION_ALWAYS,                  \
        EVENT(Lunpa_EnterHiddenPassage)                                        \
  }

/// The hidden puddle and what Reveal shows, which the town's trigger tables
/// share.
#define LUNPA_SECRETS                                                          \
  {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_HIDDEN_PUDDLE,                      \
   FLAG_LUNPA_PUDDLE_FROZEN, EVENT(Lunpa_FreezeHiddenPuddle)},                 \
      {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,              \
       CONDITION_ALWAYS, EVENT(Lunpa_RevealSecrets)},                          \
      {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,             \
       CONDITION_ALWAYS, EVENT(Lunpa_HideSecrets)},                            \
      {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_HIDDEN_SWITCH,     \
       FLAG_LUNPA_SECRETS_HIDDEN, EVENT(Lunpa_PressHiddenSwitch)},             \
      {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_PSYNERGY_STONE,    \
       FLAG_LUNPA_PSYNERGY_STONE, FIND_PSYNERGY_STONE},                        \
      {EXAMINE_TRIGGER(EXAMINE_GRAVE), CELL_BEHAVIOR_LUNPA_GRAVE_SLEEP_BOMB,   \
       FLAG_LUNPA_GRAVE_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},               \
      {EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_LUNPA_BARREL_NUT,        \
       FLAG_LUNPA_BARREL_NUT, FIND_ITEM(ITEM_NUT)},                            \
      {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT,                   \
       FLAG_LUNPA_NUT_CAUGHT, EVENT(Lunpa_CatchFloatingNut)},                  \
      {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_WELL,              \
       CONDITION_ALWAYS, EVENT(Lunpa_SearchWell)},                             \
  {                                                                            \
    TRIGGER_TABLE_END                                                          \
  }

struct Trigger gLunpaSealedTriggers[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, EVENT(Lunpa_AskAboutLengthOfStay)),
    TALK(ACTOR_WOMAN_WITH_NO_FREEDOM, MSGNO(MsgLunpaVillageVillagerBSealed)),
    TALK(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
         EVENT(Lunpa_AskAboutHammetsKidnapping)),
    TALK(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
         MSGNO(MsgLunpaVillageVillagerDSealed)),
    TALK(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
         MSGNO(MsgLunpaVillageVillagerESealed)),
    TALK(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFSealed)),
    TALK(ACTOR_BOY_ASKING_TO_SEE_DONPA, EVENT(Lunpa_AskAboutSeeingDonpa)),
    TALK(ACTOR_GIRL_ON_THE_CHEFS_MEALS, MSGNO(MsgLunpaVillageVillagerHSealed)),
    TALK(ACTOR_WEST_GUARD, EVENT(Lunpa_TalkToWestGuard)),
    TALK(ACTOR_EAST_GUARD, EVENT(Lunpa_TalkToEastGuard)),
    MIND_READ(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
              MSGNO(MsgLunpaVillageVillagerASealedThoughts)),
    MIND_READ(ACTOR_WOMAN_WITH_NO_FREEDOM,
              MSGNO(MsgLunpaVillageVillagerBSealedThoughts)),
    MIND_READ(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
              MSGNO(MsgLunpaVillageVillagerCSealedThoughts)),
    MIND_READ(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
              MSGNO(MsgLunpaVillageVillagerDSealedThoughts)),
    MIND_READ(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
              MSGNO(MsgLunpaVillageVillagerESealedThoughts)),
    MIND_READ(ACTOR_OLD_WOMAN_ON_DONPA,
              MSGNO(MsgLunpaVillageVillagerFSealedThoughts)),
    MIND_READ(ACTOR_BOY_ASKING_TO_SEE_DONPA,
              MSGNO(MsgLunpaVillageVillagerGSealedThoughts)),
    MIND_READ(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
              MSGNO(MsgLunpaVillageVillagerHSealedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardSealedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, EVENT(Lunpa_ReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

struct Trigger gLunpaReopenedTriggers[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
         MSGNO(MsgLunpaVillageVillagerAReopened)),
    TALK(ACTOR_WOMAN_WITH_NO_FREEDOM, EVENT(Lunpa_ComplainAboutCold)),
    TALK(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
         MSGNO(MsgLunpaVillageVillagerCReopened)),
    TALK(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
         EVENT(Lunpa_AskAboutFortressCommotion)),
    TALK(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
         MSGNO(MsgLunpaVillageVillagerEReopened)),
    TALK(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFReopened)),
    TALK(ACTOR_BOY_ASKING_TO_SEE_DONPA,
         MSGNO(MsgLunpaVillageVillagerGReopened)),
    TALK(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
         MSGNO(MsgLunpaVillageVillagerHReopened)),
    TALK(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardReopened)),
    TALK(ACTOR_EAST_GUARD, MSGNO(MsgLunpaVillageEastGuardReopened)),
    MIND_READ(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
              MSGNO(MsgLunpaVillageVillagerAReopenedThoughts)),
    MIND_READ(ACTOR_WOMAN_WITH_NO_FREEDOM,
              MSGNO(MsgLunpaVillageVillagerBReopenedThoughts)),
    MIND_READ(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
              MSGNO(MsgLunpaVillageVillagerCReopenedThoughts)),
    MIND_READ(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
              MSGNO(MsgLunpaVillageVillagerDReopenedThoughts)),
    MIND_READ(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
              MSGNO(MsgLunpaVillageVillagerEReopenedThoughts)),
    MIND_READ(ACTOR_OLD_WOMAN_ON_DONPA,
              MSGNO(MsgLunpaVillageVillagerFReopenedThoughts)),
    MIND_READ(ACTOR_BOY_ASKING_TO_SEE_DONPA,
              MSGNO(MsgLunpaVillageVillagerGReopenedThoughts)),
    MIND_READ(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
              MSGNO(MsgLunpaVillageVillagerHReopenedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD,
              MSGNO(MsgLunpaVillageWestGuardReopenedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, EVENT(Lunpa_ReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

// clang-format off
struct Trigger gGateTriggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_LUNPA_ROAD, CONDITION_ALWAYS, LUNPA_FORTRESS_GATES_TO_LUNPA },
    // Unused: no cell of the gates' map has the third road's behavior.
    { TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD, CONDITION_ALWAYS, GATE_EXIT_THIRD },
    { PSYNERGY_ANYWHERE(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Lunpa_CastCloak) },
    { PSYNERGY_ANYWHERE(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(Lunpa_CloakFades) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_DOOR, FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS,
      EVENT(Lunpa_EnterFortress) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND, FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR,
      EVENT(Lunpa_GuardsBlockGate) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH, CONDITION_ALWAYS,
      EVENT(Lunpa_GaretTurnsIsaacBack) },
    { TRIGGER_RAISED, RAISED_PARTY_SPOTTED, CONDITION_ALWAYS, EVENT(Lunpa_GuardsCatchParty) },
    TALK(ACTOR_LEFT_GUARD, EVENT(Lunpa_TalkToLeftGuard)),
    TALK(ACTOR_RIGHT_GUARD, EVENT(Lunpa_TalkToRightGuard)),
    MIND_READ(ACTOR_LEFT_GUARD, EVENT(Lunpa_ReadLeftGuardsThoughts)),
    MIND_READ(ACTOR_RIGHT_GUARD, EVENT(Lunpa_ReadRightGuardsThoughts)),
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_WELL, CONDITION_ALWAYS, EVENT(Lunpa_SearchWell) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// Each door's opening, as a map grid animation.
u16 gLunpaDoorSteps[DOOR_COUNT][11] = {
    {MAPGRID_ANIMATION_STEP(38, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(40, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(42, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(44, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(46, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(48, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(46, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(48, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(50, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(52, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(50, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(52, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(42, 33, 4, 2, 5),
     MAPGRID_ANIMATION_STEP(38, 33, 4, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(54, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(56, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
};

/// Each door's opening animation and the map grid cell it plays at, by its
/// step trigger from CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR.
struct MapGridCellAnimation gLunpaDoors[] = {
    {gLunpaDoorSteps[0], 46, 9},  {gLunpaDoorSteps[1], 52, 12},
    {gLunpaDoorSteps[2], 52, 18}, {gLunpaDoorSteps[3], 45, 20},
    {gLunpaDoorSteps[4], 51, 5},  {gLunpaDoorSteps[5], 37, 9},
    {gLunpaDoorSteps[6], 33, 6},  {gLunpaDoorSteps[7], 44, 17},
};

/// Unused: a zero word nothing reads.
u32 gLunpaUnusedWord = 0;
