/// @file
/// The caves in Champa's cliff, where the sailors and their families live, in
/// four scenes of this overlay climbing the cliff level by level: the lowest
/// caves, entered from the village by the mouth Briggs's sailors watch
/// (SceneId_ChampaCliffCavesBriggs); the homes above them
/// (ChampaCliffCavesHouses); the level with Obaba's door and Briggs's home
/// (ChampaCliffCavesObaba); and at the top Obaba's forge room
/// (ChampaCliffCavesAvimander). Before Briggs is home, knocking at Obaba's
/// door only brings her out to ask after her grandson
/// (ChampaCliffCaves_ObabaAsksForBriggs). Once he has fled up here from
/// Felix, walking into the forge room finds Briggs with her, and she throws
/// her salamander into the forge to come out as Avimander
/// (ChampaCliffCaves_FightAvimander). Beaten, she hears what Briggs has done
/// (ChampaCliffCaves_ObabaMournsSalamander), and afterwards forges the three
/// prongs the party brings her into the Trident of Ankohl
/// (ChampaCliffCaves_ObabaForgesTrident), which Reveal's path across the pit
/// reaches (ChampaCliffCaves_RevealPath).
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "resource_ids.h"
#include "types.h"
#include "system/io_reg.h"
#include "item_ids.h"
#include "flags.h"
#include "field/actor_script.h"

/// Set once Avimander is beaten and Obaba has heard the truth about Briggs.
#define FLAG_CHAMPA_AVIMANDER_BEATEN 0x97e
/// The party hands Obaba the prongs one by one: set as she sees the first, the
/// second, and as she forges the three into the Trident.
#define FLAG_OBABA_SAW_FIRST_PRONG 0x975
#define FLAG_OBABA_SAW_SECOND_PRONG 0x976
#define FLAG_TRIDENT_FORGED 0x977
/// The Trident has been picked up from Obaba's forge.
#define FLAG_TRIDENT_TAKEN 0x978
/// Set as Briggs first stops the party at Avimander's room ("Hold it right
/// there!"); on a later visit he greets them with "Back for more?".
#define FLAG_CHAMPA_BRIGGS_STOPPED_PARTY 0x974

/// Where the party comes back to the Avimander's room after losing to it
/// (Event_SetDefeatDestination) or beating it, the second numbered 99 as The
/// Broken Seal's Altin Peak numbers its own; the party then stands in the forge
/// room by its way in from the passage west of it (ENTRANCE_FORGE_ROOM, which
/// exit 22 leads to).
enum {
  ENTRANCE_FORGE_ROOM = 23,
  ENTRANCE_AFTER_DEFEAT = 98,
  ENTRANCE_AFTER_AVIMANDER_BATTLE = 99
};

/// The map cells the caves' step and examine triggers answer: Obaba's door,
/// whose cell takes the number of the exit into her home, the knock at it,
/// and the way into her forge room from the passage west of it.
enum ChampaCliffCavesCellBehavior {
  CELL_BEHAVIOR_OBABAS_DOOR = 15,
  CELL_BEHAVIOR_FORGE_ROOM_WAY_IN = 30,
  CELL_BEHAVIOR_KNOCK_AT_OBABAS_DOOR = 40
};

/// Avimander's battle: slot 5 of ENCOUNTER_ZONE_SERPENT, between the
/// Serpent's and Poseidon's.
enum { ENCOUNTER_SLOT_AVIMANDER = 5 };

/// The five colours of Obaba's forge pit, from colour 2 of a background
/// palette: dark blue as the room shows them (palette 7), and the pale ones
/// (palette 6) and the fiery yellow to orange ones (palette 13) the map keeps
/// for the pit to fade to.
enum {
  FORGE_PALETTE = 7,
  FORGE_PALE_PALETTE = 6,
  FORGE_FIERY_PALETTE = 13,
  FORGE_FIRST_COLOR = 2,
  FORGE_COLORS = 5
};

/// The cell column of the path Reveal shows across Obaba's forge pit, to the
/// Trident floating over it, and the four cells from its north rim it covers.
enum { FORGE_PATH_CELL_X = 41, FORGE_PATH_CELL_Z = 13, FORGE_PATH_CELLS = 4 };

#include "field/scene_tables.h"

/// Obaba's forge room (SceneId_ChampaCliffCavesAvimander), numbering
/// gChampaCliffCaves_AvimanderPlacementsBriggsHome; Obaba is actor 8 at her
/// door as well (SceneId_ChampaCliffCavesObaba).
enum ChampaCliffCavesObabasRoomActorId {
  ACTOR_OBABA = ACTOR_FIRST_PLACED,
  /// The sailor across the room from the forge: "That pit is Obaba's forge."
  /// Once Briggs is back he turns his back on the party
  /// (ChampaCliffCaves_SailorTurnsAway).
  ACTOR_FORGE_ROOM_SAILOR,
  /// The prong or the Trident shown as an item icon in Obaba's hands, and the
  /// forged Trident floating over the forge.
  ACTOR_HELD_ITEM,
  ACTOR_BRIGGS,
  /// Obaba's salamander, which she throws into her forge.
  ACTOR_SALAMANDER,
  ACTOR_AVIMANDER, ///< the salamander grown into Avimander
  ACTOR_CHAUCHA,
  ACTOR_KRADEN
};

/// Briggs's home once Avimander is beaten
/// (gChampaCliffCaves_ObabaPlacementsAfterAvimander).
enum ChampaCliffCavesBriggsHomeActorId {
  /// Briggs, lying in bed: "Once I'm over my cold, I'm setting out to get
  /// more jewels and make amends with Madra and Daila."
  ACTOR_BRIGGS_IN_BED = ACTOR_FIRST_PLACED,
  /// Eoleo, "Babu ba bubabu...", and Chaucha by the bed: "I wonder what's wrong
  /// with Briggs."
  ACTOR_EOLEO = ACTOR_FIRST_PLACED + 2,
  ACTOR_CHAUCHA_BY_THE_BED
};

/// The level with Obaba's door until Briggs is home
/// (gChampaCliffCaves_ObabaPlacements): Obaba behind her door, and the man
/// outside it, "Obaba's not seeing anyone right now."
enum ChampaCliffCavesObabasDoorActorId {
  ACTOR_MAN_AT_OBABAS_DOOR = ACTOR_FIRST_PLACED + 1
};

/// The two sailors on Obaba's level once Briggs has fled up here
/// (gChampaCliffCaves_ObabaPlacementsBriggsHome): "Leave the women and
/// children alone." and "Did you really think you could come storming into
/// Champa, Briggs's home, without a fight?"
enum ChampaCliffCavesObabasLevelSailorActorId {
  ACTOR_SAILOR_GUARDING_THE_FAMILIES = ACTOR_FIRST_PLACED,
  ACTOR_SAILOR_SPOILING_FOR_A_FIGHT
};

/// The lowest caves' women (gChampaCliffCaves_BriggsPlacements), and once
/// Avimander is beaten Briggs's crew home from the sea
/// (gChampaCliffCaves_BriggsPlacementsAfterAvimander).
enum ChampaCliffCavesLowestActorId {
  /// "All of our men went to sea with Briggs, even my sweetheart!"
  ACTOR_WOMAN_MISSING_HER_SWEETHEART = ACTOR_FIRST_PLACED,
  /// "The Champa sailors who don't live in the cliffs are no better than
  /// cowards."
  ACTOR_WOMAN_SCORNING_HARBOR_SAILORS,
  /// "We're going to wait until Briggs feels better before we go out to sea
  /// again."
  ACTOR_SAILOR_WAITING_FOR_BRIGGS,
  /// "There were plenty of treasures on that island in the middle of the
  /// sea."
  ACTOR_SAILOR_ON_THE_ISLAND_TREASURE,
  /// "We really want to make amends."
  ACTOR_SAILOR_WANTING_AMENDS,
  /// "It's all my fault. If I hadn't messed with that switch, we'd have gotten
  /// everything."
  ACTOR_SAILOR_WHO_HIT_THE_SWITCH
};

/// The lowest caves once Briggs has fled up here
/// (gChampaCliffCaves_BriggsPlacementsBriggsHome).
enum ChampaCliffCavesLowestBriggsHomeActorId {
  /// "Why do you have to be so stubborn? Quit following us, you big bully!"
  ACTOR_SAILOR_CALLING_FELIX_A_BULLY = ACTOR_FIRST_PLACED,
  /// The sailor who gives in (ChampaCliffCaves_SailorSurrenders).
  ACTOR_SAILOR_WHO_SURRENDERS,
  /// The woman in her doorway (ChampaCliffCaves_AskToComeIn).
  ACTOR_WOMAN_IN_HER_DOORWAY
};

/// The homes above the lowest caves (gChampaCliffCaves_HousesPlacements): a
/// sailor's family and the man Briggs left on watch; once Briggs has fled up
/// here two of his sailors are there instead of the watchman and the wife
/// (gChampaCliffCaves_HousesPlacementsBriggsHome).
enum ChampaCliffCavesHomesActorId {
  /// "Before he left, Briggs ordered me to keep watch in his absence."
  ACTOR_MAN_KEEPING_WATCH = ACTOR_FIRST_PLACED,
  /// "My husband wants our children to be sailors like him, but I simply won't
  /// have it."
  ACTOR_SAILORS_WIFE,
  /// "Someday, I want to be a sailor, like my father before me."
  ACTOR_SAILORS_SON,
  /// "Mom's had it pretty rough. I hope I meet a sailor with a future..."
  ACTOR_SAILORS_DAUGHTER,
  /// "I'd rather fight than go back to jail. Bring it on, sissyboy!"
  ACTOR_SAILOR_RATHER_FIGHTING = ACTOR_MAN_KEEPING_WATCH,
  /// "You're Felix, aren't you? You've come to bring in Briggs, haven't you?"
  ACTOR_SAILOR_WHO_KNOWS_FELIX = ACTOR_SAILORS_WIFE
};
#include "system/main_routine_state.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "field/scene_call.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "graphics/resource.h"
#include "system/system.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "scene_ids.h"
#include "version.h"

#include "field/call.h"
#include "field/actor.h"
#include "field/flag_call.h"
#include "system/dma.h"
#include "system/task.h"
#include "macros.h"
#include "text/msg_ids.h"

/// Obaba's door opening or closing: the map edit that draws it and the cell
/// it plays at.
struct DoorEdit {
  const s16 *frames;
  u16 x;
  u16 z;
};

extern LATE_DATA struct DoorEdit gChampaCliffCaves_OpenDoor;
extern LATE_DATA struct DoorEdit gChampaCliffCaves_CloseDoor;

void ChampaCliffCaves_OpenDoorAndEnter(const struct DoorEdit *edit, s32 door,
                                       s32 mode);
void Event_PickUpItem(s32 a, s32 b, s32 c);
void Actor_WatchActor(struct Actor *actor, struct Actor *target);

extern struct ActorPlacement gChampaCliffCaves_BriggsPlacements[];
extern struct ActorPlacement gChampaCliffCaves_HousesPlacements[];
extern struct ActorPlacement gChampaCliffCaves_BriggsPlacementsBriggsHome[];
extern struct ActorPlacement gChampaCliffCaves_HousesPlacementsBriggsHome[];
extern struct ActorPlacement gChampaCliffCaves_ObabaPlacementsBriggsHome[];
extern struct ActorPlacement gChampaCliffCaves_BriggsPlacementsAfterAvimander[];
extern struct ActorPlacement gChampaCliffCaves_HousesPlacementsAfterAvimander[];
extern struct ActorPlacement gChampaCliffCaves_ObabaPlacementsAfterAvimander[];
extern struct Trigger gChampaCliffCaves_HousesTriggers[];
extern struct Trigger gChampaCliffCaves_ObabaTriggers[];
extern struct Trigger gChampaCliffCaves_AvimanderTriggers[];

void ChampaCliffCaves_AskIfBriggsLearned(void);
void ChampaCliffCaves_ObabaMournsSalamander(void);
void ChampaCliffCaves_HandPieceToObaba(s32 value);
void ChampaCliffCaves_ObabaStepsForward(s32 mode);

void ChampaCliffCaves_FlareAndDieDown(void);
void ChampaCliffCaves_ObabaSeesAWeapon(s32 item);
void ChampaCliffCaves_ObabaAsksForSecondPiece(s32 item);
void ChampaCliffCaves_StartFlash(s32 id, s32 mode);

/// The state of the fire over Obaba's forge (heap slot HEAP_SLOT_MENU): the
/// pictures of the glow, a glowing disc, and the column of fire, their sprites
/// at the forge's spot, and their scales, each one's height trailing its
/// width.
struct ChampaCliffCavesState {
  u8 tiles[64 * 64]; ///< the larger picture, the glow's 64 by 64 pixels
  s32 position[3];
  struct EffectSprite glow;
  struct EffectSprite column;
  s16 glowEntry;
  u16 glowVram;
  s16 columnEntry;
  u16 columnVram;
  FIXED glowScale;
  FIXED glowTrailingScale;
  FIXED columnScale;
  FIXED columnTrailingScale;
  s16 frame;
};

/// The state the forge fire's steps end on.
enum { GLOW_DONE = 999 };

/// The forge fire's pictures are shaded in colours 224 to 255. Their
/// brightest sixteen, of sprite palette 15, are moved onto palette 6, so that
/// with palette 14's they are the 32 colours ChampaCliffCaves_DrawForgeFire
/// cycles, at levels about these middles: full red, green 20 and blue 3.
enum {
  GLOW_PICTURE_ROW = 15,
  GLOW_CYCLED_ROW = 6,
  GLOW_COLORS = 2 * PALETTE_COLORS,
  GLOW_GREEN_MIDDLE = 20,
  GLOW_BLUE_MIDDLE = 3
};

/// A 256-colour pixel's palette row (its high four bits) and its colour in
/// that row.
#define PIXEL_PALETTE_ROW_MASK 0xf0
#define PIXEL_ROW_COLOR_MASK 0xf

void ChampaCliffCaves_LoadGlow(void);
void ChampaCliffCaves_DrawForgeFire(void);
void ChampaCliffCaves_LoadColumn(void);

extern s16 gChampaCliffCaves_OpenDoorFrames[];
extern s16 gChampaCliffCaves_CloseDoorFrames[];

void FieldEffect_StartBlankOverlay(s32 a);

/// Defines an update that grows an actor by step (a 16.16 scale) a frame until
/// it is full size.
#define DEFINE_CHAMPA_CLIFF_CAVES_GROW(name, step)                             \
  void name(struct Actor *actor) {                                             \
    actor->scaleX += step;                                                     \
    if (actor->scaleX >= FIXED_ONE) {                                          \
      actor->scaleX = FIXED_ONE;                                               \
      actor->update = 0;                                                       \
    }                                                                          \
    actor->scaleY = actor->scaleX;                                             \
  }

/// Each frame the Trident, thrown up from the rising column, grows by a
/// sixteenth of its size until it is full size.
DEFINE_CHAMPA_CLIFF_CAVES_GROW(ChampaCliffCaves_GrowThrown, FIXED_ONE / 16)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gChampaCliffCaves_ObabaPlacements[];
extern struct ActorPlacement gChampaCliffCaves_AvimanderPlacements[];
extern struct ActorPlacement gChampaCliffCaves_AvimanderPlacementsBriggsHome[];
extern struct ActorPlacement
    gChampaCliffCaves_AvimanderPlacementsAfterAvimander[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene;

  if (GameFlag_Get(FLAG_CHAMPA_AVIMANDER_BEATEN)) {
    scene = gMainRoutineState.next;
    if (scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
      return gChampaCliffCaves_BriggsPlacementsAfterAvimander;
    if (scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
      return gChampaCliffCaves_HousesPlacementsAfterAvimander;
    if (scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
      return gChampaCliffCaves_ObabaPlacementsAfterAvimander;
    if (scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
      return gChampaCliffCaves_AvimanderPlacementsAfterAvimander;
  } else {
    if (GameFlag_Get(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
      scene = gMainRoutineState.next;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
        return gChampaCliffCaves_BriggsPlacementsBriggsHome;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
        return gChampaCliffCaves_HousesPlacementsBriggsHome;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
        return gChampaCliffCaves_ObabaPlacementsBriggsHome;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
        return gChampaCliffCaves_AvimanderPlacementsBriggsHome;
    } else {
      scene = gMainRoutineState.next;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
        return gChampaCliffCaves_BriggsPlacements;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
        return gChampaCliffCaves_HousesPlacements;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
        return gChampaCliffCaves_ObabaPlacements;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
        return gChampaCliffCaves_AvimanderPlacements;
    }
  }
  return gChampaCliffCaves_BriggsPlacements;
}

/// Walking into a door in the cliff caves: it opens with a sound, Felix walks
/// through (lined up on the door first when `mode` is 0, straight on when 1)
/// and the screen goes to the room behind it.
void ChampaCliffCaves_OpenDoorAndEnter(const struct DoorEdit *edit, s32 door,
                                       s32 mode) {
  s32 id;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(edit->frames, edit->x, edit->z);
  Actor_GetById(gMainRoutineState.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_WALK);
  id = gMainRoutineState.leaderActorId;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Walking north into Obaba's door (cell (11, 26)) once Avimander is beaten
/// opens it, and Felix walks into the home behind it, where Briggs lies in
/// bed.
void ChampaCliffCaves_EnterDoor(s32 door) {
  ChampaCliffCaves_OpenDoorAndEnter(&gChampaCliffCaves_OpenDoor, door, 0);
}

/// Walking into or knocking at Obaba's door, at the west end of its passage
/// (cells (11, 25) and (11, 26)), before Avimander is beaten: Felix steps up
/// beside it and she opens it, asks "Has my grandson returned? Has my Briggs
/// come home?", answers the yes ("You lie!") or the no, and shuts herself in
/// again.
void ChampaCliffCaves_ObabaAsksForBriggs(void) {
  const struct DoorEdit *edit;
  s32 msg;

  edit = &gChampaCliffCaves_OpenDoor;
  Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 168, 424);
  Actor_FaceDirectionAndRefreshById(gMainRoutineState.leaderActorId,
                                    FACING_EAST);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(edit->frames, edit->x, edit->z);
  Actor_GetById(ACTOR_OBABA)->motionFlags = 0;
  Actor_SetPosition(ACTOR_OBABA, 184, 408);
  Actor_SetSpeed(ACTOR_OBABA, 1.8);
  Actor_WalkByAndWaitById(ACTOR_OBABA, 0, 16);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  msg = (s32)MsgHasMyGrandson;
  Event_SetMessageId(msg);
  // "Has my grandson returned? Has my Briggs come home?" Yes: "You lie! If
  // Briggs had truly returned..." No: "I asked not to be disturbed until my
  // grandson has returned home!..."
  FieldMessage_ShowAtActor(ACTOR_OBABA, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_OBABA, 0);
  Actor_SetSpeed(ACTOR_OBABA, 0.8);
  Actor_WalkByAndWait(ACTOR_OBABA, 0, -14);
  Actor_SetPosition(ACTOR_OBABA, 0, 0);
  edit = &gChampaCliffCaves_CloseDoor;
  MapGrid_PlayMapEdit(edit->frames, edit->x, edit->z);
  Sound_Play(SE_DOOR_OPEN);
}

/// Talking to the woman standing in the doorway of her home at the west end of
/// the lowest caves, once Briggs is back: "What, you want to come in?" A yes
/// gets "As if! I'm a nice girl...", a no "You're wasting my time."
void ChampaCliffCaves_AskToComeIn(s32 actor) {
  Event_SetMessageId((s32)MsgWhatYouWant);
  // "What, you want to come in?" Yes: "As if! I'm a nice girl..." No: "Yeah?
  // So what do you want? You're wasting my time."
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId((s32)MsgWhatYouWant + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId((s32)MsgWhatYouWant + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

/// Talking to the second of the two sailors on the east side of the lowest
/// caves, once Briggs is back: he gives in ("I'll go back to
/// Alhafra...but only if Briggs is going") and keeps his eyes on Felix.
void ChampaCliffCaves_SailorSurrenders(s32 actorId) {
  struct Actor *actor;

  Event_SetMessageId((s32)MsgAllRightI);
  // "All right, I surrender! I'll go back to Alhafra...but only if Briggs..."
  Event_ShowMessageAndWait(actorId, 0);
  actor = Actor_GetById(actorId);
  Actor_WatchActor(actor, Actor_GetById(gMainRoutineState.leaderActorId));
}

/// Talking to the sailor in Obaba's forge room, once Briggs is back: "Aw, come
/// on! I'm not doing anything! Leave me alone!", and he turns his back on the
/// party, squashing and stretching from then on (MOVEMENT_TYPE_SQUASH).
void ChampaCliffCaves_SailorTurnsAway(s32 actor) {
  Event_SetMessageId((s32)MsgAwComeOn);
  // "Aw, come on! I'm not doing anything! Leave me alone!"
  Event_ShowMessageAndWait(actor, 0);
  Actor_FaceDirectionAndWait(actor, FACING_NORTH_NORTHEAST);
  Actor_SetMovementType(Actor_GetById(actor), MOVEMENT_TYPE_SQUASH);
}

/// Reveal in Obaba's forge room shows a path hidden across the forge pit,
/// north to the forged Trident floating over it. The path's cells, kept
/// further down the map, are copied there with their behaviours.
void ChampaCliffCaves_RevealPath(void) {
  MapGrid_CopyCellTilesCellsRect(41, 97, 1, FORGE_PATH_CELLS, FORGE_PATH_CELL_X,
                                 77);
  Map_CopyCellAttributeRect(41, 33, 1, FORGE_PATH_CELLS, FORGE_PATH_CELL_X,
                            FORGE_PATH_CELL_Z);
}

/// When Reveal wears off, the pit's own cells cover the path again, and a
/// leader standing on it is set down on the pit's south rim.
void ChampaCliffCaves_HidePath(void) {
  struct Actor *actor;
  s32 x;
  s32 z;

  MapGrid_CopyCellTilesCellsRect(50, 97, 1, FORGE_PATH_CELLS, FORGE_PATH_CELL_X,
                                 77);
  Map_CopyCellAttributeRect(46, 33, 1, FORGE_PATH_CELLS, FORGE_PATH_CELL_X,
                            FORGE_PATH_CELL_Z);
  actor = Actor_GetById(gMainRoutineState.leaderActorId);
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  // On the path, or one row past it on the south rim (row 17), where he is
  // set down.
  if (gMainRoutineState.next == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER &&
      x == FORGE_PATH_CELL_X &&
      (u32)(z - FORGE_PATH_CELL_Z) <= FORGE_PATH_CELLS)
    Actor_SetPosition(gMainRoutineState.leaderActorId, 664, 280);
}

extern struct Trigger gChampaCliffCaves_BriggsTriggers[];

/// Talking to the forged Trident floating over Obaba's forge pit, reached by
/// the path Reveal shows, takes it.
void ChampaCliffCaves_PickUpTrident(s32 a) {
  Event_PickUpItem(a, ITEM_TRIDENT, FLAG_TRIDENT_TAKEN);
}

const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
    return gChampaCliffCaves_BriggsTriggers;
  if (scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
    return gChampaCliffCaves_HousesTriggers;
  if (scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
    return gChampaCliffCaves_ObabaTriggers;
  if (scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
    return gChampaCliffCaves_AvimanderTriggers;
  return gChampaCliffCaves_BriggsTriggers;
}

s32 Scene_Initialize(void) {
  struct MainRoutineState *party = &gMainRoutineState;

#if !TLA_PATCH_1
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
#else
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
#endif
  if (party->next != SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
    Actor_GetById(party->leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  FieldEffect_StartBlankOverlay(0);
  if (party->next == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER) {
    if (party->entrance == ENTRANCE_AFTER_DEFEAT)
      ChampaCliffCaves_AskIfBriggsLearned();
    if (party->entrance == ENTRANCE_AFTER_AVIMANDER_BATTLE) {
      party->returnEntrance = ENTRANCE_FORGE_ROOM;
      party->entrance = ENTRANCE_FORGE_ROOM;
      ChampaCliffCaves_ObabaMournsSalamander();
    }
    if (GameFlag_Get(FLAG_TRIDENT_FORGED) &&
        !GameFlag_Get(FLAG_TRIDENT_TAKEN)) {
      FieldActor_ShowItemIconById(ACTOR_HELD_ITEM, ITEM_TRIDENT);
      Actor_SetPosition(ACTOR_HELD_ITEM, 664, 232);
      Actor_GetById(ACTOR_HELD_ITEM)->motionFlags = ACTOR_FLOATS;
    }
    if (GameFlag_Get(FLAG_TRIDENT_FORGED))
      Actor_SetPosition(ACTOR_OBABA, 664, 200);
  } else if (party->next == SCENE_CHAMPA_CLIFF_CAVES_OBABA) {
    if (GameFlag_Get(FLAG_CHAMPA_AVIMANDER_BEATEN)) {
      Actor_SetAnimationById(ACTOR_BRIGGS_IN_BED, 11);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BRIGGS_IN_BED), 0);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Felix walks up to Obaba's counter, holds the prong up over it, leaves it
/// there and steps back.
void ChampaCliffCaves_HandPieceToObaba(s32 actor) {
  Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 648, 200);
  Actor_FaceDirectionAndRefreshById(gMainRoutineState.leaderActorId,
                                    FACING_EAST);
  Event_Wait(5);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId,
                            ANIM_LEADER_SPRING);
  Event_Wait(7);
  Actor_ShowItemIconById(ACTOR_HELD_ITEM, actor);
  Actor_SetPosition(ACTOR_HELD_ITEM, 648, 196);
  Actor_GetById(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(gMainRoutineState.leaderActorId, -16, -8);
  Event_Wait(1);
  Actor_GetById(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Actor_FaceDirectionAndRefreshById(gMainRoutineState.leaderActorId,
                                    FACING_EAST);
  Event_Wait(10);
}

/// Obaba steps up to her counter and takes the prong from it, and unless she
/// is to forge it at once steps back.
void ChampaCliffCaves_ObabaStepsForward(s32 value) {
  Actor_SetSpeed(ACTOR_OBABA, 0.8);
  Actor_GetById(ACTOR_OBABA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(ACTOR_OBABA, 0, 6);
  Event_Wait(1);
  Actor_GetById(ACTOR_OBABA)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  Actor_SetAnimationById(ACTOR_OBABA, 8);
  Event_Wait(15);
  Actor_SetPosition(ACTOR_HELD_ITEM, 0, 0);
  Event_Wait(15);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  if (value == 0) {
    Actor_GetById(ACTOR_OBABA)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkByAndWait(ACTOR_OBABA, 0, -6);
    Event_Wait(1);
    Actor_GetById(ACTOR_OBABA)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_Wait(10);
  }
}

/// The forge pit's glow changes colour, its five colours fading from one set to
/// another over the given number of frames, a step a frame.
void ChampaCliffCaves_FadeColors(u16 *from, u16 *to, s32 frames) {
  s32 step;

  for (step = 0; step <= frames; step++) {
    s32 offset;
    s32 left;

    for (offset = 0, left = 0; left < FORGE_COLORS; left++) {
      s32 red, green, blue, toRed, toGreen, toBlue;

      red = RGB_RED(*(u16 *)(offset + (s32)from));
      green = RGB_GREEN(*(u16 *)(offset + (s32)from));
      blue = RGB_BLUE(*(u16 *)(offset + (s32)from));
      toRed = RGB_RED(*(u16 *)(offset + (s32)to));
      toGreen = RGB_GREEN(*(u16 *)(offset + (s32)to));
      toBlue = RGB_BLUE(*(u16 *)(offset + (s32)to));
      red += step * (toRed - red) / frames;
      green += step * (toGreen - green) / frames;
      blue += step * (toBlue - blue) / frames;
      SET_RGB(*(u16 *)((u32)&BG_PLTT_COLOR(FORGE_PALETTE, FORGE_FIRST_COLOR) +
                       offset),
              red, green, blue);
      offset += 2;
    }
    wait(1);
  }
}

/// Obaba is handed the first prong: she knows it for part of the trident of
/// Ankohl and offers to keep it until the party brings the other two.
void ChampaCliffCaves_ObabaSeesAWeapon(s32 actor) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgThisLooksLike);
  ChampaCliffCaves_HandPieceToObaba(actor);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "This looks like a weapon!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "It looks like part of a trident, but it's only one third of the..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "Why... Could it be? Is this the legendary trident of Ankohl?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_OBABA, 2);
  Event_Wait(8);
  // "I'll hold on to this for you. It won't do you any good like this..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  ChampaCliffCaves_ObabaStepsForward(0);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "You seem unsure... You don't think I'll pull a fast one on you, do you?"
  // Yes: "Kids today. So mistrustful." No: "Ah, that's nice. Don't you worry!"
  FieldMessage_ShowAtActor(ACTOR_OBABA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    // "Kids today. So mistrustful."
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    gFieldState->message++;
  } else {
    Event_Wait(35);
    gFieldState->message++;
    // "Ah, that's nice. Don't you worry!"
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  }
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
  // "If you bring me the other two pieces, I'll be happy to rebuild it..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_End();
}

/// Obaba is handed the second prong and keeps it with the first.
void ChampaCliffCaves_ObabaAsksForSecondPiece(s32 actor) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgAhHaveYou);
  ChampaCliffCaves_HandPieceToObaba(actor);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "Ah, have you found the second piece, dear?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "It's magnificent!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  ChampaCliffCaves_ObabaStepsForward(0);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  // "I'll hold on to this as well. Wouldn't want it getting lost, now..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_End();
}

#include "text/msg_ids.h"

/// The trident's three prongs, in the order Obaba throws them into the forge.
struct TridentProngs {
  s32 items[3];
};

extern struct TridentProngs gChampaCliffCaves_Prongs;

/// Obaba is handed the last prong and throws the three into her forge, whose
/// pit flashes pale at each, then flares, shakes the room and lifts out the
/// Trident to float over it.
void ChampaCliffCaves_ObabaForgesTrident(s32 value) {
  struct Actor *thrown = Actor_GetById(ACTOR_HELD_ITEM);
  u16 dark[FORGE_COLORS];
  u16 pale[FORGE_COLORS];
  s32 glow[3];
  s32 i;
  struct Actor *actor;
  u16 *palette;

  glow[0] = toFIXED(662);
  glow[1] = 0;
  glow[2] = toFIXED(238);
  palette = (u16 *)&BG_PLTT_COLOR(FORGE_PALETTE, FORGE_FIRST_COLOR);
  dark[0] = *palette++;
  dark[1] = *palette++;
  dark[2] = *palette++;
  dark[3] = *palette++;
  dark[4] = *palette;
  palette = (u16 *)&BG_PLTT_COLOR(FORGE_PALE_PALETTE, FORGE_FIRST_COLOR);
  pale[0] = *palette++;
  pale[1] = *palette++;
  pale[2] = *palette++;
  pale[3] = *palette++;
  pale[4] = *palette;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgWellDoneYou);
  ChampaCliffCaves_HandPieceToObaba(value);
  Actor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Well done! You found all three pieces of the trident!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "That means I can begin work reforging the ancient trident of Ankohl!..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  ChampaCliffCaves_ObabaStepsForward(1);
  Actor_WalkToAndWait(ACTOR_OBABA, 664, 200);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  // "We'll just toss all three pieces in here, shall we?" Yes: "Just leave it
  // to me..." No: "There's nothing to be afraid of, dear. Just..."
  FieldMessage_ShowAtActor(ACTOR_OBABA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    // "Just leave it to me..."
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    gFieldState->message++;
  } else {
    Event_Wait(40);
    gFieldState->message++;
    // "There's nothing to be afraid of, dear. Just leave it to me."
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  }
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  FieldCamera_MoveTo(664, CAMERA_KEEP, 232, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  // "Heart of the earth, fury of the forge's flames, grant me power!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  for (i = 0; i <= 2; i++) {
    struct TridentProngs prongs;
    FIXED landing[3];
    prongs = gChampaCliffCaves_Prongs;
    Actor_SetAnimationById(ACTOR_OBABA, 9);
    Event_Wait(3);
    Actor_ShowItemIconById(ACTOR_HELD_ITEM, prongs.items[i]);
    Actor_SetPosition(ACTOR_HELD_ITEM, 668, 200);
    thrown->gravity = toFIXED(1) / 4;
    Actor_Jump(ACTOR_HELD_ITEM, 4, 2);
    Actor_SetSpritePriority(ACTOR_HELD_ITEM, 1);
    Actor_SetSpeed(ACTOR_HELD_ITEM, 1);
    Actor_SetDestinationOffsetById(ACTOR_HELD_ITEM, 0, 40);
    Sound_Play(SE_FALL);
    Event_Wait(34);
    landing[0] = thrown->x.fixed;
    landing[1] = thrown->y.fixed;
    landing[2] = thrown->z.fixed;
    Actor_SetPosition(ACTOR_HELD_ITEM, 0, 0);
    Event_Wait(20);
    Sound_Play(SE_HEAVY_IMPACT);
    ChampaCliffCaves_FadeColors(dark, pale, 12);
    ChampaCliffCaves_FadeColors(pale, dark, 48);
    Event_Wait(10);
  }
  actor = Actor_GetById(gMainRoutineState.leaderActorId);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(gMainRoutineState.leaderActorId, 12, 12);
  Event_Wait(1);
  Actor_GetById(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Actor_FaceDirectionAndRefreshById(gMainRoutineState.leaderActorId,
                                    FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId,
                         ANIM_LEADER_LEAN_BACK);
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(60);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "That's funny... Nothing happened."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  Event_Wait(20);
  Sound_Play(SE_SHIMMER);
  ChampaCliffCaves_FadeColors(dark, pale, 48);
  Actor_StartRepeatedMotion(gMainRoutineState.leaderActorId, 3);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Call2((void (*)())Field_StartColorTransition, 16, RGB(16, 4, 4));
  MapGrid_SetShake(2, 2, 1);
  ChampaCliffCaves_LightForge(glow);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Actor_Jump(ACTOR_FELIX, 6, 0);
  Sound_Play(SE_TABLET_RISE);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_FELIX, -12, -12);
  Event_Wait(1);
  Actor_GetById(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_OBABA, 3);
  FieldEffect_SetAttached(ACTOR_OBABA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(30);
  // "What's going on?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  MapGrid_SetShake(4, 4, 1);
  ChampaCliffCaves_RaiseColumn(glow);
  Sound_Play(SE_TABLET_RISE);
  Event_Wait(10);
  Actor_Jump(ACTOR_OBABA, 6, 15);
  Actor_Jump(ACTOR_OBABA, 6, 23);
  Sound_Play(SE_TABLET_RISE);
  Event_Wait(10);
  // "Something's happening! Is that...the legendary trident?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  FieldActor_ShowItemIconById(ACTOR_HELD_ITEM, ITEM_TRIDENT);
  Actor_SetPosition(ACTOR_HELD_ITEM, 664, 232);
  Actor_SetSpritePriority(ACTOR_HELD_ITEM, 1);
  thrown->motionFlags = ACTOR_FALLS;
  thrown->y.fixed = toFIXED(24);
  thrown->update = ChampaCliffCaves_GrowThrown;
  thrown->groundHeight = 0;
  thrown->scaleX = 0;
  thrown->scaleY = 0;
  Actor_Jump(ACTOR_HELD_ITEM, 5, 0);
  Field_StartPaletteTransition(8);
  ChampaCliffCaves_FlareAndDieDown();
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  while (thrown->y.fixed > 0)
    wait(1);
  thrown->gravity = toFIXED(1);
  thrown->y.fixed = 0;
  thrown->motionFlags = ACTOR_FLOATS;
  Scene_PlayMusic();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_OBABA, 2);
  Event_Wait(5);
  // "After all these years, the trident is complete again. But what's..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  // "Regardless, the trident has been fixed. It belongs to you now..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_HELD_ITEM, 2);
  Event_End();
}

/// Talking to Obaba at her counter: she takes the next prong the party carries,
/// right, left then center, or talks of the trident when it has none. From
/// east of the forge path's column the field menu opens instead.
void ChampaCliffCaves_ObabaOffersHelp(s32 actor) {
  s32 item;
  // DEAD: each owner is stored and never read; the ROM keeps the copies of
  // the first two.
  s32 owner;

  if (FIELD_CELL(Actor_GetById(gMainRoutineState.leaderActorId)->x.fixed) <=
      FORGE_PATH_CELL_X)
    goto first;
  gFieldState->fieldMenu = 1;
  return;
remove:
  PartyInventory_Remove(item);
  goto chosen;
last:
  item = ITEM_CENTER_PRONG;
  if ((owner = PartyInventory_FindMember(item)) != -1)
    goto remove;
  item = -1;
  goto chosen;
first:
  item = ITEM_RIGHT_PRONG;
  if ((owner = PartyInventory_FindMember(item)) != -1)
    goto remove;
  item++;
  if ((owner = PartyInventory_FindMember(item)) == -1)
    goto last;
  PartyInventory_Remove(item);
chosen:
  if (item != -1) {
    if (!Event_GetFlag(FLAG_OBABA_SAW_FIRST_PRONG)) {
      GameFlag_Set(FLAG_OBABA_SAW_FIRST_PRONG);
      ChampaCliffCaves_ObabaSeesAWeapon(item);
    } else if (!Event_GetFlag(FLAG_OBABA_SAW_SECOND_PRONG)) {
      GameFlag_Set(FLAG_OBABA_SAW_SECOND_PRONG);
      ChampaCliffCaves_ObabaAsksForSecondPiece(item);
    } else if (!Event_GetFlag(FLAG_TRIDENT_FORGED)) {
      GameFlag_Set(FLAG_TRIDENT_FORGED);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(208), 0, -4);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(224), 0, -4);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(240), 0, -4);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(256), 0, -4);
      ChampaCliffCaves_ObabaForgesTrident(item);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(208), 0, 0);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(224), 0, 0);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(240), 0, 0);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(256), 0, 0);
    }
  } else {
    Actor_FaceActorById(actor, gMainRoutineState.leaderActorId, 5);
    // "I mean it. If there's ever anything you need, just call on me.", then
    // "The legend says that anyone who holds the whole lance has nothing to
    // fear from the sea.", "Yes, there's no doubt about it. Those must truly
    // be fragments of the legendary trident." and "The trident seems to have
    // some amazing power..."
    if (!GameFlag_Get(FLAG_OBABA_SAW_FIRST_PRONG))
      Event_SetMessageId((s32)MsgIMeanIt);
    else if (!GameFlag_Get(FLAG_OBABA_SAW_SECOND_PRONG))
      Event_SetMessageId((s32)MsgTheLegendSaysThat);
    else if (!GameFlag_Get(FLAG_TRIDENT_FORGED))
      Event_SetMessageId((s32)MsgYesThereS);
    else
      Event_SetMessageId((s32)MsgTheTridentSeems);
    Event_ShowMessageAndWait(actor, 0);
    Actor_FaceDirection(actor, FACING_SOUTH_SOUTHWEST, 0);
  }
}

/// Each frame Avimander, leaping out of the forge's fire, grows by three sixty-
/// fourths of its size, from a quarter to full size.
DEFINE_CHAMPA_CLIFF_CAVES_GROW(ChampaCliffCaves_GrowThrownSlowly,
                               FIXED_ONE * 3 / 64)

/// Walking east into Obaba's forge room from the passage (cells (36, 12) to
/// (36, 18)) once Briggs has fled up here: "Hold it right there!" Briggs
/// stops the party, and Obaba, for her great-grandson Eoleo's sake, tells Felix
/// he had better leave; if he will not, she throws her salamander into the
/// forge, whose pit turns fiery, and it climbs out grown into Avimander to
/// fight. Coming back after a defeat, Briggs greets the party with "Back for
/// more?".
void ChampaCliffCaves_FightAvimander(void) {
  u16 dark[FORGE_COLORS];
  u16 fiery[FORGE_COLORS];
  struct Vector3 position;
  const u16 *color;
  struct Actor *actor = Actor_GetById(ACTOR_AVIMANDER);

  if (Flag_IsSet(FLAG_CHAMPA_AVIMANDER_BEATEN))
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 584, 248);
  if (Flag_IsSet(FLAG_CHAMPA_BRIGGS_STOPPED_PARTY)) {
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_EAST);
    Event_SetMessageId((s32)MsgBackForMore);
    // "Back for more?"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Sound_Play(MUS_CONFRONTATION);
    FieldCamera_MoveTo(664, CAMERA_KEEP, 256, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
    // "I wouldn't be doing this if it weren't for Eoleo, you know."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "I'm sorry. You look like quite a nice boy, but I just can't allow..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  } else {
    Flag_Set(FLAG_CHAMPA_BRIGGS_STOPPED_PARTY);
    Event_SetMessageId((s32)MsgHoldItRight);
    // "Hold it right there!"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Sound_Play(MUS_CONFRONTATION);
    Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_EAST, 0);
    FieldCamera_MoveTo(664, CAMERA_KEEP, 256, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(5);
    Actor_Jump(ACTOR_BRIGGS, 6, 15);
    Actor_Jump(ACTOR_BRIGGS, 6, 23);
    // "So, Felix, you've come."
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Don't assume I'll go so easy on you this time!"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
    Event_Wait(10);
    // "Is that boy him? The "mighty warrior" you told me about?"
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
    Event_Wait(10);
    // "I told you he didn't look like much, Grandma..."
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Weren't you even listening to me?"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
    // "All you told me is that his name is Felix and that he is a "mighty..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
    // "He doesn't look like the sort of person who's interested in stealing..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(30);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
    Event_Wait(10);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "But that's not the point, Grandma!"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
    Event_Wait(5);
    // "If you don't destroy him now, he's going to take me far away, and..."
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "Oh, now he doesn't look like the kind of boy who would do that..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
    Event_Wait(10);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 45);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS,
                               FACING_SOUTH_SOUTHEAST + FACING_NUDGE);
    Event_Wait(10);
    // "If he takes me away, poor Eoleo will be so sad! What do you say..."
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
    Event_Wait(10);
    // "Oh, that's just not fair. You know I can't deny my darling..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
    Event_Wait(5);
    Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
    Event_Wait(5);
    // "I don't see why I should be the one to get you out of this mess, but..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
    Event_Wait(5);
    // "Grandma, please!"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(5);
  }
  Event_SetMessageId((s32)MsgOhAllRightListen);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_NORTH_NORTHWEST);
  Event_Wait(20);
  Actor_GetById(ACTOR_OBABA)->facing = FACING_SOUTH_SOUTHWEST;
  Actor_SetAnimationById(ACTOR_OBABA, 5);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_OBABA, 6);
  Actor_SetPosition(ACTOR_SALAMANDER, 724, 252);
  Actor_GetById(ACTOR_SALAMANDER)->scaleX = FIXED_ONE * 6 / 5;
  Actor_GetById(ACTOR_SALAMANDER)->scaleY = FIXED_ONE * 6 / 5;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SALAMANDER), 0);
  // "Oh, all right. Listen, you have no idea what you're getting into. I
  // think you'd better leave." Yes: "Ah, you've made a good decision..." No:
  // "Stubborn, are you? Ah, well..."
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Scene_PlayMusic();
    Event_Wait(15);
    Actor_SetPosition(ACTOR_SALAMANDER, 0, 0);
    Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
    Actor_GetById(ACTOR_OBABA)->facing = FACING_WEST;
    // "Ah, you've made a good decision. You seem like a reasonable boy."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
    gFieldState->message++;
    Actor_WalkByAndWaitById(ACTOR_FELIX, -16, 0);
    Event_Wait(5);
  } else {
    Event_Wait(25);
    gFieldState->message++;
    // "Stubborn, are you? Ah, well. If that's the way it's going to be, get
    // ready for your medicine!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Heart of the earth, fury of the forge's flames, grant me power!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
    Sound_Play(SOUND_FADE_OUT_BGM);
    FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(208), 0, -4);
    FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(224), 0, -4);
    FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(240), 0, -4);
    FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(256), 0, -4);
    actor->gravity = toFIXED(1) / 4;
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = -toFIXED(999);
    Actor_Jump(ACTOR_SALAMANDER, 10, 0);
    Sound_Play(SE_ENEMY_CRY);
    Actor_SetSpritePriority(ACTOR_SALAMANDER, 1);
    Actor_SetSpeed(ACTOR_SALAMANDER, 3);
    Actor_WalkToById(ACTOR_SALAMANDER, 644, 248);
    Event_Wait(21);
    Actor_SetPosition(ACTOR_SALAMANDER, 0, 0);
    Event_Wait(50);
    MapGrid_SetShake(2, 2, 1);
    Event_Wait(70);
    Sound_Play(SE_BOSS_ROAR);
    Event_Wait(25);
    color = (const u16 *)&BG_PLTT_COLOR(FORGE_PALETTE, FORGE_FIRST_COLOR);
    dark[0] = *color++;
    dark[1] = *color++;
    dark[2] = *color++;
    dark[3] = *color++;
    dark[4] = *color++;
    color = (const u16 *)&BG_PLTT_COLOR(FORGE_FIERY_PALETTE, FORGE_FIRST_COLOR);
    fiery[0] = *color++;
    fiery[1] = *color++;
    fiery[2] = *color++;
    fiery[3] = *color++;
    fiery[4] = *color++;
    ChampaCliffCaves_FadeColors(dark, fiery, 48);
    Event_Wait(20);
    Sound_Play(SE_TABLET_RISE);
    position.x = toFIXED(662);
    position.y = 0;
    position.z = toFIXED(238);
    ChampaCliffCaves_StartForgeFire(&position);
    Sound_Play(SE_BEAST_CRY);
    Actor_SetPosition(ACTOR_AVIMANDER, 664, 248);
    actor->gravity = toFIXED(1);
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = 0;
    actor->y.fixed = toFIXED(32);
    actor->scaleX = FIXED_ONE / 4;
    actor->update = (s32)ChampaCliffCaves_GrowThrownSlowly;
    actor->scaleY = FIXED_ONE / 4;
    Actor_Jump(ACTOR_AVIMANDER, 11, 0);
    Sound_Play(SE_BEAST_CRY);
    Actor_SetSpeed(ACTOR_AVIMANDER, 3.2);
    Actor_WalkToAndWait(ACTOR_AVIMANDER, 584, 248);
    Event_SetDefeatDestination(SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER,
                               ENTRANCE_AFTER_DEFEAT);
    Event_SetAfterBattleDestination(SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER,
                                    ENTRANCE_AFTER_AVIMANDER_BATTLE);
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SERPENT,
                                          ENCOUNTER_SLOT_AVIMANDER);
  }
  Event_End();
}

/// Coming to after losing to Avimander, in the forge room: Briggs asks Felix
/// whether he has learned his lesson, Obaba scolds, and the party is sent back
/// down the passage.
void ChampaCliffCaves_AskIfBriggsLearned(void) {
  struct FieldState *state;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgHaveYouLearned);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 568, 248);
  Actor_SetPosition(ACTOR_OBABA, 584, 224);
  Actor_SetPosition(ACTOR_AVIMANDER, 600, 240);
  Actor_SetPosition(ACTOR_BRIGGS, 584, 264);
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, 41);
  FieldCamera_FollowActor(gMainRoutineState.leaderActorId, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Have you learned your lesson, Felix?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "Ah, such a bright lad. Now then, don't you ever try to take my..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 20);
  state = Ram_HeapSlots->fieldState;
  state->transitionFrames = 40;
  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetDestination((s16)gMainRoutineState.restartScene,
                       (s16)gMainRoutineState.restartEntrance);
}

extern u32 gChampaCliffCaves_ChaseToNorthScript[];
extern u32 gChampaCliffCaves_ChaseToEastScript[];
extern u32 gChampaCliffCaves_ChaseToWestScript[];
extern u32 gChampaCliffCaves_ChaseToSouthScript[];

/// After the party beats Avimander, in the forge room: Obaba mourns her
/// salamander ("My salamander! You... beat my salamander...") and leaves
/// Briggs to fight for himself, the party tells her he is a pirate, Chaucha
/// comes to tell why he did it, and Briggs and Chaucha go home
/// (FLAG_CHAMPA_AVIMANDER_BEATEN).
void ChampaCliffCaves_ObabaMournsSalamander(void) {
  const u8 *scriptA, *scriptB, *scriptC, *scriptD;
  struct Actor *leader;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgMySalamanderYou);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 568, 256);
  Actor_SetPosition(ACTOR_AVIMANDER, 592, 240);
  Actor_SetAnimationById(ACTOR_AVIMANDER, 3);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  FieldCamera_MoveTo(656, CAMERA_KEEP, 248, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  // "My salamander! You... beat my salamander..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_AVIMANDER, 2);
  Event_Wait(5);
  ChampaCliffCaves_StartFlash(ACTOR_AVIMANDER, 1);
  Actor_SetSpeed(ACTOR_OBABA, 1);
  Actor_WalkByAndWait(ACTOR_OBABA, -12, -18);
  Actor_WalkByAndWait(ACTOR_OBABA, -10, -14);
  Actor_WalkByAndWait(ACTOR_OBABA, -18, -12);
  Actor_WalkToAndWait(ACTOR_OBABA, 664, 200);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  // "I thought he was unbeatable... How could this be?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 0);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 8);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 16);
  Actor_WalkByAndWait(ACTOR_OBABA, -8, 16);
  Actor_WalkToAndWait(ACTOR_OBABA, 608, 248);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  Event_Wait(5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "How did you do that? What... What are you?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetSpeed(ACTOR_BRIGGS, 1.7);
  Actor_WalkByAndWait(ACTOR_BRIGGS, -16, 0);
  Event_Wait(5);
  // "Grandma, what's going on!? I thought you were going to protect me!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
  Event_Wait(5);
  // "What do you mean, what am I doing? I've done everything I can!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_Jump(ACTOR_BRIGGS, 6, 15);
  Actor_Jump(ACTOR_BRIGGS, 6, 23);
  // "Grandma!!! He's going to take me away! He's a bad guy! You have to..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "I'm sorry, Briggs, but you'll have to fight him yourself."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(38);
  // "Wha-What!?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "You heard me. I'm through pampering you. You have to get out of this..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(37);
  // "But...Grandma... Look at him! I mean, look what he can do!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST - FACING_NUDGE);
  Event_Wait(25);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
  Event_Wait(10);
  // "I know, dear, but there's nothing I can do to help. You don't want..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "But...what about poor Eoleo?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "*Sigh* All right, all right."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 0);
  // "Um. Excuse me... There seems to be a small misunderstanding."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST - FACING_NUDGE);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, 0, -16, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -16, -16, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, -16, FACING_EAST);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Event_Wait(10);
  FieldCamera_MoveTo(640, CAMERA_KEEP, 248, 1);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "Yeah! I don't like standing here listening to a pirate call us the..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_OBABA, 3);
  Actor_SetAttachedEffect(ACTOR_OBABA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "Pirate! My Briggs?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  // "He stole a ship from Alhafra and escaped from their jail! A ship we..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
  Event_Wait(5);
  // "Briggs, is this true?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  FieldEffect_SetAttached(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(35);
  // "Aw... But, Grandma, they..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "We should tell you, we're not here to take Briggs back to Alhafra."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Event_Wait(5);
  // "So you've decided you don't want to run errands for the mayor of..." Yes:
  // "Good. That greedy bag of hot air can do his..." No: "Felix, I know you
  // can't mean that. We didn't..."
  FieldMessage_ShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    Actor_FaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_SOUTHEAST);
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Event_Wait(10);
    // "Good. That greedy bag of hot air can do his own dirty work, for all..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
    gFieldState->message++;
  } else {
    Event_Wait(35);
    Actor_FaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_SOUTHEAST);
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    gFieldState->message++;
    // "Felix, I know you can't mean that. We didn't come to Champa for that..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
  }
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_EAST);
  // "Briggs... Piracy? Why?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetSpeed(ACTOR_OBABA, 2.8);
  Actor_SetSpeed(ACTOR_BRIGGS, 3.4);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  scriptA = gChampaCliffCaves_ChaseToNorthScript;
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptA);
  Event_Wait(20);
  Actor_WalkToById(ACTOR_BRIGGS, 664, 290);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  scriptB = gChampaCliffCaves_ChaseToEastScript;
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptB);
  Actor_WaitForMoveById(ACTOR_BRIGGS);
  scriptC = gChampaCliffCaves_ChaseToWestScript;
  Actor_SetMovementTypeById(ACTOR_BRIGGS, scriptC);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  scriptD = gChampaCliffCaves_ChaseToSouthScript;
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptD);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_SetMovementTypeById(ACTOR_BRIGGS, scriptA);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  Actor_FaceDirection(ACTOR_OBABA, FACING_NORTH_NORTHWEST, 0);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
  // "Grandma, it's not what you think! These guys, they..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "Briggs, you just hush up!!! ...It all makes sense now. The gems, the..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "What do you mean? What makes sense?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "They're telling the truth! And you, Briggs! You've been lying to me..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptC);
  Event_Wait(20);
  Actor_WaitForMoveById(ACTOR_BRIGGS);
  Actor_SetMovementTypeById(ACTOR_BRIGGS, scriptB);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptA);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_SetMovementTypeById(ACTOR_BRIGGS, scriptD);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  // "You're going to take their word over your own grandson's?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "You got that right! I should have known better than to believe you..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  // "He only did it for Champa!"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_SetSpritePriority(ACTOR_FELIX, 3);
  Actor_SetSpritePriority(ACTOR_JENNA, 3);
  Actor_SetSpritePriority(ACTOR_SHEBA, 3);
  Actor_SetSpritePriority(ACTOR_PIERS, 3);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  FieldCamera_MoveTo(568, CAMERA_KEEP, 248, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_CHAUCHA, 2);
  Actor_GetById(ACTOR_CHAUCHA)->facing = FACING_SOUTH_SOUTHEAST;
  Actor_GetById(ACTOR_CHAUCHA)->motionFlags = 0;
  Actor_SetPosition(ACTOR_CHAUCHA, 520, 200);
  Actor_SetSpeed(ACTOR_CHAUCHA, 1);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 0, 16);
  Actor_FaceDirectionById(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
  // "Chaucha!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_WatchAndSetScript(ACTOR_FELIX, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_OBABA, ACTOR_CHAUCHA);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 0, 16);
  Actor_SetSpritePriority(ACTOR_CHAUCHA, 3);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 0, 32);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 8, 8);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 64, 0);
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
  FieldCamera_MoveTo(600, CAMERA_KEEP, 256, 1);
  Actor_SetSpeed(ACTOR_BRIGGS, 1);
  Actor_WalkByAndWait(ACTOR_BRIGGS, -32, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, -16, -16);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "You came!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  // "I heard Felix had shown up looking for you. I was worried..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_OBABA, 2);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_OBABA, 1);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 0);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 8);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 16);
  Actor_WalkByAndWait(ACTOR_OBABA, -8, 8);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  // "What did you mean, Briggs did it for Champa?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
  Event_Wait(15);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "All of that food... Stolen goods?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
  // "No, we paid for everthing we brought back with us."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "But you stole a boat to do it, didn't you?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  // "Grandma, we found those jewels ourselves, and we paid Alhafra well..."
  // Yes: "Your story has the ring of truth to it." No: "So he's lying again?"
  FieldMessage_ShowAtActor(ACTOR_BRIGGS, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_OBABA, 2);
    Event_Wait(5);
    // "Your story has the ring of truth to it."
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndRefreshById(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(10);
    Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Thank you, Felix. Your honesty speaks well of you."
    Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    gFieldState->message += 2;
  } else {
    Event_Wait(35);
    gFieldState->message += 2;
    // "So he's lying again?"
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndRefreshById(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(5);
    Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Dang it, Felix, I'm not lying!"
    Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Actor_Jump(ACTOR_BRIGGS, 6, 15);
    Actor_Jump(ACTOR_BRIGGS, 6, 23);
  }
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
  Event_Wait(15);
  Actor_ShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "We...were pirates, for a short while. Until we found the jewels."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
  Event_Wait(5);
  // "But if not for our efforts, Champa would have starved away long ago."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 5);
  FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "But that doesn't excuse resorting to piracy!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  // "Hey, it's not like we were doing it for the money! We wanted to save..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
  Event_Wait(5);
  // "Those jewels we found are very valuable. We intend to repay each..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
  Event_Wait(5);
  // "Isn't that true?"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What's the matter with you!? You said on the boat that we would..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  FieldEffect_SetAttached(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "Oh yeah... You're right... We really ought to try to make amends for..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
  Event_Wait(10);
  // "That's the truth, Grandmother. Can you find it in your heart to..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  // "All right, all right. I'm too old for grudges. But what about Felix?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  // "What do you say, Felix? Can you forgive my husband?"
  FieldMessage_ShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0);
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(15);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_Wait(10);
    // "I agree, Felix. If each town is repaid what was stolen, I see no..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 5);
    gFieldState->message++;
  } else {
    Event_Wait(35);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_Wait(10);
    gFieldState->message++;
    // "I don't know, Felix. We didn't come to capture Briggs. Why should we..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 5);
  }
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
  Event_Wait(10);
  // "Do you hear that? They forgive you, too. That means you can leave..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "I... I don't know what to say..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "You know, you look a little strange..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  // "Oh... Oh, really?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 15);
  Actor_ShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "You haven't caught a cold, have you? Do you have a fever?"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
  Event_Wait(5);
  // "What are you talking about?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
  // "No, I'm positive... You look flush, too... You'd better lie down..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  // "She...worries about me."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  // "Hurry up and get into bed so you can get some rest!"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, -8, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, -8, 0);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, -56, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
  Event_Wait(10);
  // "Oh, for goodness' sake! You don't want to give them your cold, do..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(5);
  Actor_Jump(ACTOR_BRIGGS, 6, 20);
  Actor_WatchAndSetScript(ACTOR_FELIX, ACTOR_BRIGGS);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_BRIGGS);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_BRIGGS);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_BRIGGS);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_BRIGGS);
  Actor_SetSpeed(ACTOR_BRIGGS, 2);
  Actor_WalkByAndWaitById(ACTOR_BRIGGS, -80, 0);
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_CHAUCHA, 3);
  Actor_SetSpritePriority(ACTOR_BRIGGS, 3);
  Actor_SetSpeed(ACTOR_BRIGGS, 1);
  Actor_WalkBy(ACTOR_CHAUCHA, 0, -64);
  Actor_WalkByAndWait(ACTOR_BRIGGS, -16, 0);
  Actor_WalkByById(ACTOR_BRIGGS, 0, -64);
  Event_Wait(15);
  Actor_SetSpritePriority(ACTOR_CHAUCHA, 2);
  Actor_SetSpritePriority(ACTOR_BRIGGS, 2);
  Actor_WaitForMoveById(ACTOR_CHAUCHA);
  Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
  Actor_WaitForMoveById(ACTOR_BRIGGS);
  Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Event_Wait(60);
  Actor_WalkByAndWaitById(ACTOR_OBABA, 0, 16);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
  // "So...what brings you to Champa?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
  Event_Wait(10);
  // "I mean, you didn't come here to catch Briggs, from what I gathered..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "Ah, well... To be young and foolish again. If you ever need to..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetSpeed(ACTOR_BRIGGS, 0.7);
  Actor_SetMovementTypeById(ACTOR_OBABA, gChampaCliffCaves_ChaseToNorthScript);
  Event_Wait(30);
  Actor_Stop(ACTOR_OBABA);
  Actor_WalkToAndWait(ACTOR_OBABA, 648, 188);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_FELIX, 2);
  Actor_GetById(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_JENNA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_SHEBA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_PIERS, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  GameFlag_Set(FLAG_CHAMPA_AVIMANDER_BEATEN);
  Event_End();
}

/// ChampaCliffCaves_StartFlash's explosion: its tiles, decompressed into the
/// heap, and the sprite that shows them.
struct FlashState {
  u8 tiles[1024];
  struct EffectSprite sprite;
};

/// The explosion's sprite palette, and the tiles of each of its 16 by 16
/// pictures.
enum { FLASH_PALETTE = 15, FLASH_PICTURE_TILES = 16 * 16 / 64 };

/// Someone or something vanishes in a burst: it spins (`mode` 0) or not, is
/// hidden, and the burst plays over it, twice the size in mode 1; then it is
/// taken off the map.
void ChampaCliffCaves_StartFlash(s32 id, s32 mode) {
  struct FlashState *state =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct FlashState));
  struct Actor *actor = Actor_GetById(id);
  s32 slot;
  s32 tile;
  struct EffectSprite *sprite;
  FIXED position[3];
  s32 frame;
  FIXED z;

  if (mode == 0) {
    for (frame = 0; frame < 16; frame++) {
      s32 angle = actor->facing;
      angle += DEGtoANG(45);
      actor->facing = angle;
      wait(1);
    }
  }
  wait(2);
  actor->scaleX = 0;
  Actor_SetSpriteFlags(actor, 0);
  Dma_Set(Field_ExplosionPalette, (void *)&OBJ_PLTT_COLOR(FLASH_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION),
                      state);
  slot = VramCache_FindFreeEntry();
  tile = Vram_Load(slot, sizeof(state->tiles), state);
  sprite = &state->sprite;
  FieldEffectSprite_Init(sprite, 8, 16, OBJ_SIZE_16X16, tile);
  sprite->depth = OAM_DEPTH_WINDOW;
  sprite->oam.priority = 1;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = FLASH_PALETTE;
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = z = position[2];
  if (mode == 1) {
    sprite->scaleX = FIXED_ONE * 2;
    sprite->scaleY = FIXED_ONE * 2;
    sprite->y = z + toFIXED(10);
  }
  Sound_Play(SE_CRASH);
  for (frame = 0; frame <= 71; frame++) {
    sprite = &state->sprite;
    switch (frame) {
    case 6:
      sprite->oam.index = tile + 1 * FLASH_PICTURE_TILES;
      break;
    case 12:
      sprite->oam.index = tile + 2 * FLASH_PICTURE_TILES;
      break;
    case 22:
      sprite->oam.index = tile + 3 * FLASH_PICTURE_TILES;
      break;
    case 32:
      sprite->oam.index = tile + 4 * FLASH_PICTURE_TILES;
      break;
    case 42:
      sprite->oam.index = tile + 5 * FLASH_PICTURE_TILES;
      break;
    case 52:
      sprite->oam.index = tile + 6 * FLASH_PICTURE_TILES;
      break;
    case 62:
      sprite->oam.index = tile + 7 * FLASH_PICTURE_TILES;
      break;
    }
    EffectSprite_Draw(sprite);
    wait(1);
  }
  Actor_SetPosition(id, 0, 0);
  VramCache_FreeEntry(slot);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// Loads the forge's glow, a glowing disc, into the fire's state, its picture
/// moved onto the colours the fire cycles, and sets its sprite at no size.
void ChampaCliffCaves_LoadGlow(void) {
  struct ChampaCliffCavesState *state = Ram_HeapSlots->menuState;
  struct EffectSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_GLOW_DISC), state);
  for (i = 0, tile = state->tiles; i < (s32)sizeof(state->tiles); tile++, i++) {
    if ((*tile & PIXEL_PALETTE_ROW_MASK) == GLOW_PICTURE_ROW * PALETTE_COLORS)
      *tile = (*tile & PIXEL_ROW_COLOR_MASK) + GLOW_CYCLED_ROW * PALETTE_COLORS;
  }
  entry = VramCache_FindFreeEntry();
  vram = VramCache_Load(entry, 64 * 64, state);
  state->glowEntry = entry;
  state->glowVram = vram;
  sprite = &state->glow;
  FieldEffectSprite_Init(sprite, 31, 31, OBJ_SIZE_64X64, vram);
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = 2;
  sprite->depth = 1;
  state->glowScale = 0;
  state->glowTrailingScale = 0;
}

/// Loads the forge's column of fire into the fire's state as
/// ChampaCliffCaves_LoadGlow loads the glow.
void ChampaCliffCaves_LoadColumn(void) {
  struct ChampaCliffCavesState *state = Ram_HeapSlots->menuState;
  struct EffectSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FIRE_COLUMN), state);
  for (i = 0, tile = state->tiles; i < (s32)sizeof(state->tiles); i++, tile++) {
    if ((*tile & PIXEL_PALETTE_ROW_MASK) == GLOW_PICTURE_ROW * PALETTE_COLORS)
      *tile = (*tile & PIXEL_ROW_COLOR_MASK) + GLOW_CYCLED_ROW * PALETTE_COLORS;
  }
  entry = VramCache_FindFreeEntry();
  vram = VramCache_Load(entry, 32 * 64, state);
  state->columnEntry = entry;
  state->columnVram = vram;
  sprite = &state->column;
  FieldEffectSprite_Init(sprite, 15, 47, OBJ_SIZE_32X64, vram);
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 1;
  sprite->oam.priority = 1;
  sprite->depth = 1;
  state->columnScale = 0;
  state->columnTrailingScale = 0;
}

/// The height the glow and the column are drawn at: 31/40 of the width they
/// had the frame before.
#define FIRE_HEIGHT_SCALE(widthScale) ((widthScale) * 31 / 40)

/// Each frame while Obaba's forge burns, its fire shimmers and pulses: the
/// fire's 32 colours ripple through waves of green and blue, a 32nd of a turn
/// apart (the first sixteen onto colours 111 down to 96, the rest onto 239
/// down to 224); the glow's and the column's sprites are placed at the fire's
/// spot at their sizes; and each one's height is set to trail its width
/// (FIRE_HEIGHT_SCALE).
void ChampaCliffCaves_DrawForgeFire(void) {
  struct ChampaCliffCavesState *state = Ram_HeapSlots->menuState;
  s32 position[3];
  struct EffectSprite *sprite;
  s32 i;

  for (i = 0; i < GLOW_COLORS; i++) {
    s32 red = RGB_LEVEL_MAX;
    s32 green = Trig_Sin((state->frame + i) * (TRIG_TURN / 32)) * 6;
    s32 blue = Trig_Sin((state->frame / 2 + i) * (TRIG_TURN / 32) +
                        TRIG_TURN * 9 / 32) *
               3;
    s32 color;
    green = FIXEDtoINT(green);
    blue = FIXEDtoINT(blue);
    green += GLOW_GREEN_MIDDLE;
    blue += GLOW_BLUE_MIDDLE;
    if (i < PALETTE_COLORS) {
      color = RGB(red, green, blue);
      ((u16 *)OBJ_PLTT)[GLOW_CYCLED_ROW * PALETTE_COLORS + PALETTE_COLORS - 1 -
                        i] = color;
    } else {
      color = RGB(red, green, blue);
      ((u16 *)OBJ_PLTT)[GLOW_PICTURE_ROW * PALETTE_COLORS + PALETTE_COLORS - 1 -
                        i] = color;
    }
  }
  position[0] = state->position[0];
  position[1] = state->position[1];
  position[2] = state->position[2];
  sprite = &state->glow;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = state->glowScale;
  sprite->scaleY = state->glowTrailingScale;
  EffectSprite_Draw(sprite);
  position[0] = state->position[0];
  position[1] = state->position[1];
  position[2] = state->position[2];
  sprite = &state->column;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = state->columnScale;
  sprite->scaleY = state->columnTrailingScale;
  EffectSprite_Draw(sprite);
  state->glowTrailingScale = FIRE_HEIGHT_SCALE(state->glowScale);
  state->columnTrailingScale = FIRE_HEIGHT_SCALE(state->columnScale);
  state->frame++;
}

/// The forge's fire at a spot: the glow grows, holds, and the column rises
/// through it, while the room shakes.
void ChampaCliffCaves_StartForgeFire(const s32 *position) {
  struct ChampaCliffCavesState *champaCliffCavesState =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct ChampaCliffCavesState));
  s32 frames;
  s32 state;

  champaCliffCavesState->position[0] = position[0];
  champaCliffCavesState->position[1] = position[1];
  champaCliffCavesState->position[2] = position[2];
  ChampaCliffCaves_LoadGlow();
  ChampaCliffCaves_LoadColumn();
  champaCliffCavesState->frame = 0;
  frames = 0;
  Task_Add(ChampaCliffCaves_DrawForgeFire, TASK_PASS_FRAME);
  MapGrid_SetShake(4, 4, 1);
  state = 0;
  do {
    switch (state) {
    case 0:
      if ((champaCliffCavesState->glowScale += FIXED_ONE * 3 / 128) >=
          FIXED_ONE * 13 / 10) {
        champaCliffCavesState->glowScale = FIXED_ONE * 13 / 10;
        state = 1;
        frames = -1;
      }
      break;
    case 1:
      if (frames == 10) {
        state = 2;
        frames = -1;
      }
      break;
    case 2:
      if ((champaCliffCavesState->columnScale += FIXED_ONE / 64) >=
          FIXED_ONE * 2 / 5) {
        champaCliffCavesState->columnScale = FIXED_ONE * 2 / 5;
        state = 3;
        frames = -1;
      }
      break;
    case 3:
      if ((champaCliffCavesState->columnScale += FIXED_ONE * 3 / 16) >=
          FIXED_ONE * 13 / 10) {
        champaCliffCavesState->columnScale = FIXED_ONE * 13 / 10;
        state = GLOW_DONE;
      }
      break;
    }
    System_Wait(1);
    frames++;
  } while (state != GLOW_DONE);
}

/// Unused: puts out the forge's fire: the column and then the glow shrink
/// away and the fire's state is released.
UNUSED void ChampaCliffCaves_PutOutForgeFire(void) {
  struct ChampaCliffCavesState *champaCliffCavesState =
      Ram_HeapSlots->menuState;
  FIXED position[3];
  s32 state;
  s32 frames;

  position[0] = champaCliffCavesState->position[0];
  position[1] = champaCliffCavesState->position[1] - toFIXED(32);
  state = 0;
  frames = 0;
  position[2] = champaCliffCavesState->position[2];
  do {
    switch (state) {
    case 0:
      if ((champaCliffCavesState->columnScale -= FIXED_ONE / 16) < 0) {
        champaCliffCavesState->columnScale = 0;
        frames = -1;
        state = 1;
      }
      break;
    case 1:
      if ((champaCliffCavesState->glowScale -= FIXED_ONE / 32) < 0) {
        champaCliffCavesState->glowScale = 0;
        state = 2;
        frames = -1;
      }
      break;
    case 2:
      if (frames == 1)
        state = GLOW_DONE;
      break;
    }
    wait(1);
    frames++;
  } while (state != GLOW_DONE);
  Task_Remove(ChampaCliffCaves_DrawForgeFire);
  MapGrid_SetShake(0, 0, 0);
  VramCache_FreeEntry(champaCliffCavesState->glowEntry);
  VramCache_FreeEntry(champaCliffCavesState->columnEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// The forge lights at a spot: the glow alone grows to full size.
void ChampaCliffCaves_LightForge(const struct Vector3 *origin) {
  struct ChampaCliffCavesState *champaCliffCavesState =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct ChampaCliffCavesState));
  s32 state;

  champaCliffCavesState->position[0] = origin->x;
  champaCliffCavesState->position[1] = origin->y;
  champaCliffCavesState->position[2] = origin->z;
  ChampaCliffCaves_LoadGlow();
  champaCliffCavesState->columnScale = 0;
  champaCliffCavesState->columnTrailingScale = 0;
  champaCliffCavesState->frame = 0;
  Task_Add(ChampaCliffCaves_DrawForgeFire, TASK_PASS_FRAME);
  Sound_Play(SE_DJINNI_SET);
  state = 0;
  do {
    if (state == 0) {
      champaCliffCavesState->glowScale += FIXED_ONE / 16;
      if (champaCliffCavesState->glowScale >= FIXED_ONE * 13 / 10) {
        champaCliffCavesState->glowScale = FIXED_ONE * 13 / 10;
        state = GLOW_DONE;
      }
    }
    wait(1);
  } while (state != GLOW_DONE);
}

/// The column of fire rises through the glow already burning.
void ChampaCliffCaves_RaiseColumn(void) {
  struct ChampaCliffCavesState *champaCliffCavesState =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct ChampaCliffCavesState));
  s32 state;

  ChampaCliffCaves_LoadColumn();
  Task_Add(ChampaCliffCaves_DrawForgeFire, TASK_PASS_FRAME);
  Sound_Play(SE_CLATTER);
  state = 0;
  do {
    switch (state) {
    case 0:
      champaCliffCavesState->columnScale += FIXED_ONE / 16;
      if (champaCliffCavesState->columnScale >= FIXED_ONE * 2 / 5) {
        champaCliffCavesState->columnScale = FIXED_ONE * 2 / 5;
        state = 1;
      }
      break;
    case 1:
      champaCliffCavesState->columnScale += FIXED_ONE * 3 / 16;
      if (champaCliffCavesState->columnScale >= FIXED_ONE * 13 / 10) {
        champaCliffCavesState->columnScale = FIXED_ONE * 13 / 10;
        state = GLOW_DONE;
      }
      break;
    }
    wait(1);
  } while (state != GLOW_DONE);
}

/// The forge's last flare: the column flares to twice its size, then it and the
/// glow die away and the fire's state is released.
void ChampaCliffCaves_FlareAndDieDown(void) {
  struct ChampaCliffCavesState *champaCliffCavesState =
      Ram_HeapSlots->menuState;
  s32 state = 0;
  s32 time = 0;

  do {
    switch (state) {
    case 0:
      if ((champaCliffCavesState->columnScale += FIXED_ONE / 16) >=
          FIXED_ONE * 2) {
        champaCliffCavesState->columnScale = FIXED_ONE * 2;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if ((champaCliffCavesState->columnScale -= FIXED_ONE / 16) <
          FIXED_ONE * 13 / 10) {
        champaCliffCavesState->columnScale = FIXED_ONE * 13 / 10;
        state = 2;
        time = -1;
      }
      /* fall through */
    case 2:
      if ((champaCliffCavesState->columnScale -= FIXED_ONE / 16) < 0)
        champaCliffCavesState->columnScale = 0;
      if ((champaCliffCavesState->glowScale -= FIXED_ONE / 16) < 0) {
        champaCliffCavesState->glowScale = 0;
        state++;
        time = -1;
      }
      break;
    case 3:
      if (time == 1)
        state = GLOW_DONE;
      break;
    }
    wait(1);
    time++;
  } while (state != GLOW_DONE);
  Task_Remove(ChampaCliffCaves_DrawForgeFire);
  VramCache_FreeEntry(champaCliffCavesState->glowEntry);
  VramCache_FreeEntry(champaCliffCavesState->columnEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// The three prongs Obaba throws into her forge, in turn.
struct TridentProngs gChampaCliffCaves_Prongs = {
    {ITEM_RIGHT_PRONG, ITEM_LEFT_PRONG, ITEM_CENTER_PRONG}};

#undef DEFINE_CHAMPA_CLIFF_CAVES_GROW

#include "text/msg_ids.h"
#include "field/effect_sprite.h"

// clang-format off
LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(ChampaCliffCavesBriggs),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA, Champa, CHAMPA_FROM_CHAMPA_CLIFF_CAVES_BRIGGS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA_CLIFF_CAVES_HOUSES, ChampaCliffCavesHouses, CHAMPA_CLIFF_CAVES_HOUSES_FROM_CHAMPA_CLIFF_CAVES_BRIGGS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ChampaCliffCavesBriggs, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ChampaCliffCavesBriggs, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ChampaCliffCavesBriggs, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, ChampaCliffCavesBriggs, 5, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesHouses),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA, Champa, CHAMPA_FROM_CHAMPA_CLIFF_CAVES_HOUSES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, ChampaCliffCavesHouses, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, ChampaCliffCavesHouses, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, ChampaCliffCavesHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, ChampaCliffCavesHouses, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_BRIGGS, ChampaCliffCavesBriggs, CHAMPA_CLIFF_CAVES_BRIGGS_FROM_CHAMPA_CLIFF_CAVES_HOUSES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_OBABA, ChampaCliffCavesObaba, CHAMPA_CLIFF_CAVES_OBABA_FROM_CHAMPA_CLIFF_CAVES_HOUSES, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesObaba),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_HOUSES, ChampaCliffCavesHouses, CHAMPA_CLIFF_CAVES_HOUSES_FROM_CHAMPA_CLIFF_CAVES_OBABA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, ChampaCliffCavesObaba, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, ChampaCliffCavesObaba, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, ChampaCliffCavesObaba, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(18, ChampaCliffCavesObaba, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA, Champa, CHAMPA_FROM_CHAMPA_CLIFF_CAVES_OBABA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_AVIMANDER, ChampaCliffCavesAvimander, CHAMPA_CLIFF_CAVES_AVIMANDER_FROM_CHAMPA_CLIFF_CAVES_OBABA, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesAvimander),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_AVIMANDER_TO_CHAMPA_CLIFF_CAVES_OBABA, ChampaCliffCavesObaba, CHAMPA_CLIFF_CAVES_OBABA_FROM_CHAMPA_CLIFF_CAVES_AVIMANDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(22, ChampaCliffCavesAvimander, ENTRANCE_FORGE_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(23, ChampaCliffCavesAvimander, 22, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_BriggsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 568,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 224,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_HousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 192, 0, 184, FACING_SOUTH_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 488, 0, 248, FACING_NORTH_NORTHEAST,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_ObabaPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 200, 0, 424, FACING_SOUTH_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_AvimanderPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 616, 0, 216, FACING_SOUTH_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_BriggsPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 344, 0, 552, FACING_WEST,
                        TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_SQUASH, 344, 0, 568,
                        FACING_WEST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 504,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_HousesPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312,
            0, 728, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328,
            0, 728, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                        TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 248,
                        FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_ObabaPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520,
            0, 392, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184,
            0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_AvimanderPlacementsBriggsHome[] = {
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 736, 0, 248, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_SQUASH, 216, 0, 464, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 752, 0, 264, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SALAMANDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AVIMANDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement
    gChampaCliffCaves_BriggsPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 568,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0,
                        224, FACING_SOUTH, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 208,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 600, 0, 216,
                        FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_WANDER, 576, 0, 248, FACING_SOUTH,
                        TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 504, 0, 264, FACING_EAST,
                        TALK_FACING_PARTY, 0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement
    gChampaCliffCaves_HousesPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 192, 0, 184,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                        TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 248, FACING_EAST,
                        TALK_FACING_PARTY, 0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement
    gChampaCliffCaves_ObabaPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        560, 0, 704, FACING_SOUTH, TALK_FACING_KEEP, 0),
        ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        480, 0, 704, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_AvimanderPlacementsAfterAvimander[] = {
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 188, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 464, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Obaba's door opening and closing, as MAPGRID_ANIMATION_STEP rows: two steps
/// of six frames each.
LATE_DATA s16 gChampaCliffCaves_OpenDoorFrames[] = {
    1, 64, 1, 2, 6, 2, 64, 1, 2, 6, -1,
};
LATE_DATA s16 gChampaCliffCaves_CloseDoorFrames[] = {
    1, 64, 1, 2, 6, 0, 64, 1, 2, 6, -1,
};

/// Obaba's door opening and closing.
LATE_DATA struct DoorEdit gChampaCliffCaves_OpenDoor = {
    gChampaCliffCaves_OpenDoorFrames, 11, 88};
LATE_DATA struct DoorEdit gChampaCliffCaves_CloseDoor = {
    gChampaCliffCaves_CloseDoorFrames, 11, 88};

LATE_DATA struct Trigger gChampaCliffCaves_BriggsTriggers[] = {
    {TRIGGER_EXIT, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA_CLIFF_CAVES_HOUSES, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA_CLIFF_CAVES_HOUSES},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_WOMAN_MISSING_HER_SWEETHEART, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgArenTYou)},
    {TRIGGER_TALK, ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgIThinkThose)},
    {TRIGGER_TALK, ACTOR_SAILOR_WAITING_FOR_BRIGGS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeReGoing)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_ISLAND_TREASURE, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgThereWerePlenty)},
    {TRIGGER_TALK, ACTOR_SAILOR_WANTING_AMENDS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeReallyWant)},
    {TRIGGER_TALK, ACTOR_SAILOR_WHO_HIT_THE_SWITCH, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgItSAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MISSING_HER_SWEETHEART,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgThatSureComes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SCORNING_HARBOR_SAILORS,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgAllThoseSailors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WAITING_FOR_BRIGGS,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgSupposeWeWent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_ISLAND_TREASURE,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsIsJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WANTING_AMENDS,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIWasAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_HIT_THE_SWITCH,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgAtLeastWe)},
    {TRIGGER_TALK, ACTOR_SAILOR_CALLING_FELIX_A_BULLY, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgWhyDoYou)},
    {TRIGGER_TALK, ACTOR_SAILOR_WHO_SURRENDERS, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(ChampaCliffCaves_SailorSurrenders)},
    {TRIGGER_TALK, ACTOR_WOMAN_IN_HER_DOORWAY, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(ChampaCliffCaves_AskToComeIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_CALLING_FELIX_A_BULLY,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgMaybeIfI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_SURRENDERS,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSAfter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_HER_DOORWAY,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgToughOrNot)},
    {TRIGGER_TALK, ACTOR_WOMAN_MISSING_HER_SWEETHEART, CONDITION_ALWAYS, MSGNO(MsgAllOfOurMen)},
    {TRIGGER_TALK, ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, CONDITION_ALWAYS, MSGNO(MsgTheChampaSailors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MISSING_HER_SWEETHEART, CONDITION_ALWAYS,
     MSGNO(MsgBriggsIsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, CONDITION_ALWAYS,
     MSGNO(MsgIfYouLive)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gChampaCliffCaves_HousesTriggers[] = {
    {TRIGGER_EXIT, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_BRIGGS, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_BRIGGS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_OBABA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_OBABA},
    {TRIGGER_TALK, ACTOR_MAN_KEEPING_WATCH, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWhyDidYou)},
    {TRIGGER_TALK, ACTOR_SAILORS_WIFE, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeHaveSo)},
    {TRIGGER_TALK, ACTOR_SAILORS_SON, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgBriggsCameDown)},
    {TRIGGER_TALK, ACTOR_SAILORS_DAUGHTER, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgEoleoStillWon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_WATCH,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIJustKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_WIFE,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsSaysHe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsSNever)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgEoleoIsYounger)},
    {TRIGGER_TALK, ACTOR_SAILOR_RATHER_FIGHTING, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgIDRatherFight)},
    {TRIGGER_TALK, ACTOR_SAILOR_WHO_KNOWS_FELIX, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgYouReAren)},
    {TRIGGER_TALK, ACTOR_SAILORS_SON, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgWhySEveryone)},
    {TRIGGER_TALK, ACTOR_SAILORS_DAUGHTER, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgIfYouSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_RATHER_FIGHTING,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgItWouldBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_KNOWS_FELIX,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsSaidThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatSHe)},
    {TRIGGER_TALK, ACTOR_MAN_KEEPING_WATCH, CONDITION_ALWAYS, MSGNO(MsgBeforeHeLeft)},
    {TRIGGER_TALK, ACTOR_SAILORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgMyHusbandWants)},
    {TRIGGER_TALK, ACTOR_SAILORS_SON, CONDITION_ALWAYS, MSGNO(MsgSomedayIWant)},
    {TRIGGER_TALK, ACTOR_SAILORS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMomSHad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_WATCH, CONDITION_ALWAYS,
     MSGNO(MsgBriggsMayBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_WIFE, CONDITION_ALWAYS,
     MSGNO(MsgWhenIMarried)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON, CONDITION_ALWAYS,
     MSGNO(MsgHasMomForgotten)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER, CONDITION_ALWAYS,
     MSGNO(MsgEoleoIsThe)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gChampaCliffCaves_ObabaTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_OBABAS_DOOR,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     EVENT(ChampaCliffCaves_EnterDoor)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_HOUSES, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_HOUSES},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_OBABAS_DOOR, CONDITION_ALWAYS,
     EVENT(ChampaCliffCaves_ObabaAsksForBriggs)},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_AVIMANDER, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_AVIMANDER},
    {TRIGGER_TALK, ACTOR_BRIGGS_IN_BED, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgOnceIM)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_CHAUCHA_BY_THE_BED,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIWonderWhatS)},
    {TRIGGER_TALK, ACTOR_EOLEO, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgBabuBaBubabu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_IN_BED,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIMCertain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHAUCHA_BY_THE_BED,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgThereIsAbsolutely)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEO,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgOneOfPapa)},
    {TRIGGER_TALK, ACTOR_SAILOR_GUARDING_THE_FAMILIES, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgLeaveTheWomen)},
    {TRIGGER_TALK, ACTOR_SAILOR_SPOILING_FOR_A_FIGHT, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgDidYouReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_GUARDING_THE_FAMILIES,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatAreThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_SPOILING_FOR_A_FIGHT,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhyWouldBriggs)},
    {TRIGGER_TALK, ACTOR_MAN_AT_OBABAS_DOOR, CONDITION_ALWAYS, MSGNO(MsgObabaSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_OBABAS_DOOR, CONDITION_ALWAYS,
     MSGNO(MsgIVeNeverSeen)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_KNOCK_AT_OBABAS_DOOR, FLAG_CHAMPA_AVIMANDER_BEATEN,
     EVENT(ChampaCliffCaves_ObabaAsksForBriggs)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gChampaCliffCaves_AvimanderTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_AVIMANDER_TO_CHAMPA_CLIFF_CAVES_OBABA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_AVIMANDER_TO_CHAMPA_CLIFF_CAVES_OBABA},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_STEP, CELL_BEHAVIOR_FORGE_ROOM_WAY_IN, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(ChampaCliffCaves_FightAvimander)},
    {TRIGGER_TALK, ACTOR_OBABA, CONDITION_ALWAYS,
     EVENT(ChampaCliffCaves_ObabaOffersHelp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_OBABA_SAW_FIRST_PRONG,
     MSGNO(MsgNowIVe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_OBABA_SAW_SECOND_PRONG,
     MSGNO(MsgAllOfMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_TRIDENT_FORGED,
     MSGNO(MsgSoonILl)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, CONDITION_ALWAYS,
     MSGNO(MsgAllOfMyTraining)},
    {TRIGGER_TALK, ACTOR_HELD_ITEM, CONDITION_ALWAYS, EVENT(ChampaCliffCaves_PickUpTrident)},
    {TRIGGER_TALK, ACTOR_FORGE_ROOM_SAILOR, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgYouGuysDon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgMustBeIncredibly)},
    {TRIGGER_TALK, ACTOR_FORGE_ROOM_SAILOR, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(ChampaCliffCaves_SailorTurnsAway)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsIsPretty)},
    {TRIGGER_TALK, ACTOR_FORGE_ROOM_SAILOR, CONDITION_ALWAYS, MSGNO(MsgThatPitIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR, CONDITION_ALWAYS,
     MSGNO(MsgIWonderHow)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(ChampaCliffCaves_RevealPath)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(ChampaCliffCaves_HidePath)},
    {TRIGGER_TABLE_END},
};

/// Obaba chasing Briggs round her forge: the four quarters of the way round,
/// each walked to its end.
LATE_DATA u32 gChampaCliffCaves_ChaseToNorthScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(616, 0, 220),
    SC_SYNC,
    SC_MOVE(632, 0, 206),
    SC_SYNC,
    SC_MOVE(648, 0, 200),
    SC_SYNC,
    SC_MOVE(664, 0, 198),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gChampaCliffCaves_ChaseToEastScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(680, 0, 200),
    SC_SYNC,
    SC_MOVE(694, 0, 206),
    SC_SYNC,
    SC_MOVE(710, 0, 216),
    SC_SYNC,
    SC_MOVE(720, 0, 232),
    SC_SYNC,
    SC_MOVE(720, 0, 246),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gChampaCliffCaves_ChaseToSouthScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(710, 0, 264),
    SC_SYNC,
    SC_MOVE(698, 0, 278),
    SC_SYNC,
    SC_MOVE(664, 0, 290),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gChampaCliffCaves_ChaseToWestScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(632, 0, 280),
    SC_SYNC,
    SC_MOVE(616, 0, 266),
    SC_SYNC,
    SC_MOVE(608, 0, 248),
    SC_SYNC,
    SC_MOVE(608, 0, 230),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};
// clang-format on
