/// @file
/// Vale by day (SceneId_Vale), Isaac's village at the foot of Mt. Aleph,
/// outside the houses. A river runs down it from a waterfall in the north. In
/// the north are the Sanctum, a small stone shrine, and west of it the path to
/// the steps up Mt. Aleph. Isaac's house stands in the middle, east of the
/// river, with a ladder against its front; Garet's house is north-east of it,
/// below the cliff where Garet's training stone stands in his sister's
/// flowers. Two ways lead west out of the village to Kraden's cottage, and
/// puppies play in the southern one. In the south are the house of Jenna's
/// grandparents, a house on the west bank wrecked by the storm with its door
/// knocked off its hinges, the rope bridge across the river, and the way east
/// to the plaza.
///
/// The actor placements and triggers come in three sets, chosen by the story
/// flags: before the party leaves Vale, after it has left
/// (FLAG_VALE_PARTY_LEFT) and back from Kalay (FLAG_KALAY_REACHED). The
/// opening (VALE_FROM_TITLE) has a set of its own.
///
/// In story order:
///
/// - The day the story begins, three years after the storm (cottage_rooms_b.c),
///   which took Jenna's family and Isaac's father, Kyle. The game opens with
///   Vale_MendingTheRoof. Under the caption "Three years later...", Isaac is
///   mending the roof of his house while Dora, his mother, calls up to him.
///   Garet, who will travel with him, trains his Psynergy on his training
///   stone. Jenna comes up from the south, and the two speak of that day before
///   going to fetch Isaac to go to Kraden's. The party comes back in front of
///   Isaac's house (VALE_FROM_ROOF_MENDED), where Vale_RoofMended plays once.
///   Dora praises the roof and remembers Kyle, and Jenna and Garet tell her
///   they are going up Mt. Aleph with Kraden, the scholar who teaches them
///   Alchemy. For the rest of that visit Dora frets about the hole clumsy
///   Garet left.
/// - Vale before the party leaves. Until Kraden has planned the visit to Sol
///   Sanctum (FLAG_SANCTUM_VISIT_PLANNED), Jenna turns Isaac back at the steps
///   up Mt. Aleph. The first time Isaac takes the path there, a bearded man in
///   a long robe jumps out from the trees by the river to warn him against
///   sneaking up the sacred peak. He then walks a beat in front of the Sanctum
///   and along the path, and drives Isaac back whenever he catches him on it.
///   Three puppies play in the southern way to Kraden's cottage. Walking up to
///   them from either side sets them barking and Isaac turns back, until the
///   party has met Saturos and Menardi, two of the strangers staying at the
///   inn; then the puppies are gone. Villagers talk of the mayor, Garet's
///   grandfather, and of not going beyond Kraden's cottage. Garet's sister
///   scolds him for crushing her flowers, and Dora says Isaac can fix the hole
///   later.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT), Dora will not open
///   the door of Isaac's house to him ("My Isaac would keep his promise... Go
///   away."). The villagers speak of the tremors and of Kraden, who has not
///   come back.
/// - Back from Kalay (FLAG_KALAY_REACHED), Garet meets his sister by the
///   training stone and asks his little brother to protect the family while he
///   is gone. A young man asks whether Jenna is back, a woman feels another
///   tremor, and a man wonders why no travellers have come since the eruption.
///   Isaac's house opens again, and Dora lies ill inside (cottage_rooms_b.c).
///
/// In every set Catch takes the Nut from the tree on the ledge by the river
/// above the wrecked house (FLAG_VALE_NUT_CAUGHT), Isaac can push or Move
/// Garet's training stone a cell east or west, and he can climb the ladder onto
/// the roof of his house. The game also changes cells as Isaac steps on and off
/// the ladder's top and the rope bridges, which the player does not see
/// (FLAG_VALE_ON_ROOF and FLAG_VALE_ON_BRIDGE). After a battle
/// (FLAG_KEEP_PARTY_POSITION) the setup puts both back as they were.
///
/// The ways out:
///
/// - The steps in the north-west go up Mt. Aleph to Sol Sanctum's door
///   (SceneId_SolEntrance).
/// - Two ways out at the west edge lead to the part of Vale around Kraden's
///   cottage (SceneId_ValeKradensCottage), and one at the east edge in the
///   south to the plaza with the Psynergy Stone (SceneId_ValePlaza).
/// - The doors of Garet's house, the house north-west of Isaac's, the house of
///   Jenna's grandparents and the house south of theirs lead into the rooms of
///   SceneId_CottageRoomsA (cottage_rooms_a.c), and the door of Isaac's house
///   into SceneId_CottageRoomsB.
/// - The Sanctum's door leads into SceneId_Sanctum.
///
/// Two events no table reaches remain. They are Vale_UnusedRoofMended, a copy
/// of the roof's end, and Vale_UnusedWaterBurstsByTheRiver.
///
/// The man guarding Mt. Aleph stands still while a menu is open
/// (Vale_SynchronizeAlephGuardPause). The TBS_PATCH_4 editions also set Dora's
/// sprite priority before Kalay.
///
/// In the opening Garet trains his Psynergy on his training stone. His sprite
/// flashes white, then cycles through its colour modes with sparks about him.
/// Meanwhile the hand of Move pushes the stone west and sheds sparkles, and
/// the stone cycles its colour modes too.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_sequence.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/floating_nut.h"
#include "scene/actor_polar_target.h"
#include "scene/catch_nut.h"
#include "scene/event_psynergy.h"
#include "scene/import.h"
#include "scene/vale.h"
#include "scene/walk_into_leader.h"
#include "scene/golden_ring.h"
#include "sound/sound.h"
#include "system/heap_release.h"
#include "system/heap_slots.h"
#include "system/iwram_call.h"
#include "system/main_routine.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/task.h"
#include "system/vram_cache.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

extern s32 gVale_AlephGuardPatrolRoute[];
extern s32 gVale_LeaderHammers[];
extern s32 gVale_GaretWalksToTheHouse[];
extern s32 gVale_UnusedWaterGrows[];
extern s32 gVale_SparkleScript[];
extern struct SceneEntrance gVale_Entrances[];
extern u32 gVale_Exits[];
extern struct ActorPlacement gVale_ActorPlacements[];
extern struct ActorPlacement gVale_ActorPlacementsAfterLeaving[];
extern struct ActorPlacement gVale_ActorPlacementsAfterKalay[];
extern struct ActorPlacement gVale_RepairActorPlacements[];
extern struct Trigger gVale_Triggers[];
extern struct Trigger gVale_TriggersAfterLeaving[];
extern u16 gVale_SanctumDoorOpens[];
extern u16 gVale_IsaacsDoorOpens[];
extern u16 gVale_GrandparentsDoorOpens[];
extern u16 gVale_WestHouseDoorOpens[];
extern s32 gVale_FirstWarningLeaderPath[];
extern s32 gVale_FirstWarningGuardPath[];
extern s32 gVale_LeaderRunsBackFromThePath[];
extern s32 gVale_GuardComesDownThePath[];
extern s32 gVale_LeaderRunsBackBeforeTheDoor[];
extern s32 gVale_GuardStepsBeforeTheDoor[];

extern struct Trigger gVale_TriggersAfterKalay[];
extern s32 gVale_JennaWalksToTheHouse[];

/// Where Garet lands after his jump once Dora has gone in, at the roof's end
/// (Vale_RoofMended): two pixels further south in the TBS_PATCH_2
/// editions.
#if TBS_PATCH_2
#define VALE_GARET_REUNION_Z 829
#else
#define VALE_GARET_REUNION_Z 827
#endif

/// Where Dora stands by the house in the opening and before the party leaves:
/// two pixels further north in the TBS_PATCH_4 editions.
#if TBS_PATCH_4
#define VALE_DORA_Z 887
#else
#define VALE_DORA_Z 889
#endif

/// Where Garet's training stone stands in the west of the three cells it can
/// be pushed between below the cliff, (22, 36) to (24, 36), two pixels east of
/// each cell's middle.
#define PILLAR_WEST_X 362
#define PILLAR_MIDDLE_X 378 ///< in the middle column
#define PILLAR_EAST_X 394   ///< in the east column
#define PILLAR_Z 584        ///< in row 36

static void Vale_JennaSaysKradenIsWaiting(void);
static void Vale_AskAboutSneakingToAleph(const s32 *leaderActions,
                                         const s32 *guardActions);
static void Vale_ClimbUpRoofLadder(s32 actorId, s32 approachAnimationId,
                                   s32 departureAnimationId, s32 resumeMotion);
static void Vale_ClimbDownRoofLadder(s32 actorId, s32 approachAnimationId,
                                     s32 departureAnimationId,
                                     s32 resumeMotion);
static void Vale_PlaceOrHideBoards(s32 first, u32 count, s32 mode);
static void Vale_MendingTheRoof(void);
static void Vale_RoofMended(void);
static void Vale_UpdateGaretPaletteAndEffects(void);
static void Vale_UpdateLeaderPaletteAndEffects(void);
static void Vale_CycleOpeningPillarPalette(void);
static void Vale_CycleFirstBoardPalette(void);
static void Vale_CycleSecondBoardPalette(void);
static void Vale_CycleThirdBoardPalette(void);
static UNREAD_RESULT Vale_HaveGuardWatchTheLeader(struct Actor *actor);
static void Vale_TrailSparkles(struct Actor *actor);
static void Vale_VanishMoveHand(struct Actor *emitter);
static void Actor_SetTargetPositionFromMagnitudeAngle(struct Actor *actor,
                                                      s32 magnitude, s32 angle);
static void Vale_MovePillarBelowCliff(void);
static void Vale_PlayPsynergySparkleSound(void);
static void Vale_SynchronizeAlephGuardPause(void);
static void Vale_SetPillarPriorityByLeaderHeight(void);
static void Vale_FlashActorAndSpawnEffects(struct Actor *actor);
static void Vale_CycleActorPaletteAndSpawnEffects(struct Actor *actor);
static void Vale_CycleActorPalette(struct Actor *actor);

/// The placed actors of Vale by day, numbered in the order of
/// gVale_ActorPlacements and gVale_ActorPlacementsAfterLeaving, which place the
/// same people in the same rows.
///
/// A row whose sprite is a party member's (Garet's, Jenna's) takes that party
/// member's actor id instead (Event_SpawnActorPlacements), so the numbering
/// skips it.
enum ValeActorId {
  /// An old woman. "Mt. Aleph looks beautiful today. It's so peaceful." After
  /// the party has left, "Vale is saved thanks to the Wise One!"
  ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH = ACTOR_FIRST_PLACED,
  /// A woman in a green dress. "Vale stays pristine because of Mt. Aleph's
  /// purity."
  ACTOR_WOMAN_ON_ALEPHS_PURITY,
  /// A little girl with orange pigtails. "I'm jealous of you, Garet, because
  /// your grandpa's the mayor." (Vale_TalkAboutTheMayorAndTremors)
  ACTOR_GIRL_JEALOUS_OF_GARET,
  /// A young man with brown hair. "This place looks just like it did three
  /// years ago... after the storm..."
  ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM,
  /// A strongman by the river. "Isaac, watch out! Don't get too close to the
  /// river!"
  ACTOR_STRONGMAN_BY_THE_RIVER,
  /// A burly grey-haired man. "Vale must remain hidden. Psynergy's secrets
  /// mustn't fall into the wrong hands."
  ACTOR_MAN_KEEPING_VALE_HIDDEN,
  /// A woman with fair hair who stands by the way out to Kraden's cottage. "Do
  /// you need to go past here?", then "You mustn't go beyond Kraden's cottage."
  /// (Vale_WarnAboutGoingBeyondKradensCottage)
  ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN,
  /// The three puppies, small white dogs, removed once the party has met
  /// Saturos and Menardi. "Rrruff! Rrrruff!"
  ACTOR_FIRST_PUPPY,
  /// A young woman with an orange ponytail. "Garet, you make me so mad
  /// sometimes!" (Vale_ArgueWithGaretsSister) Garet calls her "Sis" back from
  /// Kalay.
  ACTOR_GARETS_SISTER = ACTOR_FIRST_PUPPY + 3,
  /// "You're in trouble now, Garet... Your sister's really mad!" After the
  /// party has left, "Please take care of my brother, Isaac."
  ACTOR_GARETS_BROTHER,
  /// Garet's training stone, a stone pillar below the cliff north of his
  /// house, which crushed his sister's flowers. Isaac pushes it
  /// (Vale_PushPillar) or moves it with Move (Vale_MovePillarBelowCliff).
  ACTOR_PILLAR,
  ACTOR_DORA, ///< Isaac's mother
  /// A man with long brown hair and a beard in a long robe. He jumps out to
  /// warn Isaac off the sacred peak, then keeps watching him
  /// (Vale_WarnAgainstSneakingUpMtAleph). "Mt. Aleph is a sacred peak. It is
  /// forbidden to climb it!"
  ACTOR_MAN_GUARDING_MT_ALEPH,
  /// The boards on the roof, SPRITE_ROOF_BOARDS, which Isaac moves into place
  /// with Psynergy at the roof's end (Vale_RoofMended).
  ACTOR_FIRST_BOARD,
  ACTOR_SECOND_BOARD,
  ACTOR_THIRD_BOARD,
  ACTOR_NUT ///< the Nut for Catch, until FLAG_VALE_NUT_CAUGHT is set
};

/// Back from Kalay, the placed actors of gVale_ActorPlacementsAfterKalay.
/// Garet's row takes his own actor id.
enum ValeAfterKalayActorId {
  /// Garet's sister. "Ah! It's been a while... You're back?"
  /// (Vale_GaretMeetsHisSister)
  ACTOR_GARETS_SISTER_AFTER_KALAY = ACTOR_FIRST_PLACED,
  /// "I will do my best to show everyone that I am strong enough to protect
  /// our family and Vale." (Vale_GaretAsksToProtectFamily)
  ACTOR_GARETS_BROTHER_AFTER_KALAY,
  /// The young man of ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM. "Is Jenna back
  /// yet?" (Vale_AskWhetherJennaHasReturned)
  ACTOR_YOUNG_MAN_ASKING_FOR_JENNA,
  /// The strongman of ACTOR_STRONGMAN_BY_THE_RIVER. "I still remember when
  /// Felix and I used to play here...*Sniff*"
  ACTOR_STRONGMAN_REMEMBERING_FELIX,
  /// Three sprites of water, CrossboneIsleFloodgate's water sprite
  /// SPRITE_SPLASH. Only Vale_UnusedWaterBurstsByTheRiver shows them. During a
  /// tremor each in turn grows from a tenth of its size at (266, 1230), north
  /// of ACTOR_STRONGMAN_REMEMBERING_FELIX, and vanishes
  /// (gVale_UnusedWaterGrows).
  ///
  /// The row after them places the woman with fair hair of
  /// ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN at the origin, and nothing names her.
  ACTOR_FIRST_WATER,
  /// An old woman. "Mt. Aleph's eruption was so powerful, it caused the
  /// continent to crack!"
  ACTOR_OLD_WOMAN_ON_THE_ERUPTION = ACTOR_FIRST_WATER + 4,
  /// A woman in a green dress. "Aghh! I felt another one!"
  /// (Vale_FeelAnotherTremor)
  ACTOR_WOMAN_FEELING_TREMORS,
  /// A little girl with orange pigtails. "They say the danger is gone, even
  /// though the earthquakes continue..."
  ACTOR_GIRL_DOUBTING_SAFETY,
  /// A burly grey-haired man. "I wonder why we haven't seen any travelers since
  /// the eruption..." (Vale_AskAboutTheLackOfTravelers)
  ACTOR_MAN_MISSING_TRAVELERS,
  ACTOR_NUT_AFTER_KALAY,
  ACTOR_PILLAR_AFTER_KALAY
};

/// The game's opening (Vale_MendingTheRoof), the placed actors of
/// gVale_RepairActorPlacements. Jenna's and Garet's rows take their own actor
/// ids.
enum ValeOpeningActorId {
  /// A woman in a green dress who meets Jenna on her way.
  ACTOR_OPENING_WOMAN = ACTOR_FIRST_PLACED,
  /// The pillar of ACTOR_PILLAR, which Garet trains his Psynergy on.
  ACTOR_OPENING_PILLAR,
  ACTOR_OPENING_DORA, ///< calls up to Isaac on the roof
  /// The boards on the roof, as ACTOR_FIRST_BOARD.
  ACTOR_OPENING_FIRST_BOARD,
  ACTOR_OPENING_SECOND_BOARD,
  ACTOR_OPENING_THIRD_BOARD,
  /// SPRITE_MOVE_HAND, the hand of Move (event_commands_catch.c). It pushes the
  /// pillar for Garet, shedding sparkles.
  ACTOR_OPENING_MOVE_HAND
};

/// What Vale's cells do when Isaac steps on them, walks into them or searches
/// them, by the trigger rows that answer them.
enum ValeCellBehavior {
  /// The Sanctum's door: cells (15, 27) and (16, 27).
  CELL_BEHAVIOR_VALE_SANCTUM_DOOR = 1,
  /// The steps up Mt. Aleph in the north-west: cells (5, 16) to (8, 16).
  CELL_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH,
  /// The northern way west to Kraden's cottage: cells (0, 38) to (0, 40).
  CELL_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN,
  /// The southern way west, where the puppies play: (0, 49) and (0, 50).
  CELL_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN,
  /// The way east to the plaza, in the south: cells (29, 86) and (29, 87).
  CELL_BEHAVIOR_VALE_WAY_TO_THE_PLAZA,
  /// The door of Garet's house: cell (25, 46).
  CELL_BEHAVIOR_VALE_GARETS_DOOR,
  /// The door of the house north-west of Isaac's: cell (16, 51).
  CELL_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR,
  /// The door of Isaac's house: cell (21, 56).
  CELL_BEHAVIOR_VALE_ISAACS_DOOR,
  /// The door of Jenna's grandparents' house: cell (20, 71).
  CELL_BEHAVIOR_VALE_GRANDPARENTS_DOOR,
  /// The door of the house south of theirs: cell (23, 78).
  CELL_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR,
  /// Unused: no cell carries it.
  CELL_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR,
  /// A cell beyond each end of the rope bridges, a step from the river.
  CELL_BEHAVIOR_VALE_OFF_BRIDGE = 14,
  /// The cells at each end of the rope bridges.
  CELL_BEHAVIOR_VALE_ONTO_BRIDGE,
  /// North of the puppies: cells (1, 47) to (5, 47) and (1, 48).
  CELL_BEHAVIOR_VALE_NORTH_OF_PUPPIES = 20,
  /// South of the puppies: cells (3, 53) and (4, 53).
  CELL_BEHAVIOR_VALE_SOUTH_OF_PUPPIES,
  /// The stuck door of the wrecked house on the west bank: cell (6, 74).
  CELL_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR = 24,
  /// West of Garet's training stone, pushing it east.
  CELL_BEHAVIOR_VALE_WEST_OF_TRAINING_STONE = 30,
  /// East of Garet's training stone, pushing it west.
  CELL_BEHAVIOR_VALE_EAST_OF_TRAINING_STONE,
  /// The north end of the path up the Sanctum's west side: cell (10, 23).
  CELL_BEHAVIOR_VALE_GUARD_JUMPS_OUT = 50,
  /// The path up the Sanctum's west side: cells (11, 23) to (13, 27), and
  /// (12, 30) and (13, 30) south of its foot.
  CELL_BEHAVIOR_VALE_PATH_UP_MT_ALEPH,
  /// The corner west of the Sanctum's front: cells (14, 28) and (14, 29).
  CELL_BEHAVIOR_VALE_BESIDE_THE_SANCTUM,
  /// The foot of the path: cells (12, 28) to (13, 29).
  CELL_BEHAVIOR_VALE_FOOT_OF_THE_PATH,
  /// The foot of the ladder against Isaac's house: cell (24, 55).
  CELL_BEHAVIOR_VALE_LADDER_FOOT = 100,
  /// The top of the ladder, on the roof: cell (24, 53) once the roof's cells
  /// are in.
  CELL_BEHAVIOR_VALE_LADDER_TOP
};

/// Which stretch of his beat the man guarding Mt. Aleph is on, kept in his
/// SW_TIMER (gVale_AlephGuardPatrolRoute). He catches Isaac sneaking only on
/// the first two.
enum ValeGuardBeat {
  /// Up and down the path along the Sanctum's west side.
  GUARD_ON_THE_PATH = 1,
  /// Along the front of the Sanctum, past its door.
  GUARD_BEFORE_THE_SANCTUM,
  /// Up the Sanctum's east side, behind it.
  GUARD_BEHIND_THE_SANCTUM
};

/// The leader's animation of hammering on the roof, beyond enum
/// ActorAnimation's.
enum { ANIM_HAMMER = 11 };

/// Animations of Garet's sprite beyond enum ActorAnimation, as its animation
/// table and frames under graphics/sprite draw them.
enum ValeAnimation {
  /// Garet throwing up his arms, then crouching, as he jumps in fright
  /// (garet.png).
  ANIM_GARET_STARTLED = 13
};

#include "shared/pushed_actor.c"

/// The roof of Isaac's house has just been mended. For the rest of the visit
/// Dora frets about the hole Garet left.
#define FLAG_VALE_ROOF_JUST_MENDED FLAG_SCENE_TEMP_02
/// The leader has climbed the ladder onto the roof of Isaac's house, whose
/// cells are changed so it can be walked.
#define FLAG_VALE_ON_ROOF FLAG_SCENE_TEMP_04
/// The man guarding Mt. Aleph is paused while a menu is open.
#define FLAG_VALE_MT_ALEPH_GUARD_PAUSED FLAG_SCENE_TEMP_41
/// Back from Kalay, Garet has met his sister.
#define FLAG_VALE_GARET_MET_SISTER FLAG_AREA_TEMP_03
/// Back from Kalay, Garet has asked his brother to protect the family while he
/// is gone.
#define FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY FLAG_AREA_TEMP_04
/// Unused: the water has burst up by the river and the strongman has remembered
/// Felix (Vale_UnusedWaterBurstsByTheRiver, which nothing reaches).
#define FLAG_VALE_UNUSED_WATER_BURST_SEEN FLAG_AREA_TEMP_05
/// Back from Kalay, the burly man has asked about the lack of travellers since
/// the eruption.
#define FLAG_VALE_ASKED_ABOUT_TRAVELERS FLAG_AREA_TEMP_07
/// Vale_RoofMended has played, in which Dora praises the roof and sends
/// the three off to Mt. Aleph.
#define FLAG_VALE_ROOF_MENDED_EVENT_PLAYED FLAG_AREA_TEMP_08
/// The stone pillar below the cliff stands in its west column.
#define FLAG_VALE_PILLAR_IN_WEST_COLUMN FLAG_AREA_TEMP_14
/// The stone pillar below the cliff stands in its middle column, where it
/// starts.
#define FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN FLAG_AREA_TEMP_15
/// The stone pillar below the cliff stands in its east column.
#define FLAG_VALE_PILLAR_IN_EAST_COLUMN FLAG_AREA_TEMP_16

const struct SceneEntrance *Scene_GetEntrances(void) { return gVale_Entrances; }

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gVale_Exits; }

/// Places the villagers for the story so far, or for the game's opening, when
/// Isaac is mending the roof (VALE_FROM_TITLE).
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.entrance == VALE_FROM_TITLE)
    return gVale_RepairActorPlacements;
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    return gVale_ActorPlacementsAfterKalay;
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0)
    return gVale_ActorPlacementsAfterLeaving;
  return gVale_ActorPlacements;
}

/// Catch on the Nut in the tree on the ledge by the river, above the house the
/// storm wrecked on the west bank, plucks it from the tree, and Isaac holds it
/// up and gets it.
///
/// The Nut hangs at cell (9, 63) and floats in it until FLAG_VALE_NUT_CAUGHT
/// is set (Scene_FloatNut, Scene_CatchNut).
static void Vale_CatchNut(void) {
  Scene_CatchNut(ACTOR_NUT, FLAG_VALE_NUT_CAUGHT);
}

/// Back from Kalay, Catch on the Nut in the tree on the ledge by the river,
/// above the wrecked house on the west bank, plucks it from the tree for Isaac,
/// if he did not take it before.
///
/// The placements back from Kalay number the Nut ACTOR_NUT_AFTER_KALAY.
static void Vale_CatchNutAfterKalay(void) {
  Scene_CatchNut(ACTOR_NUT_AFTER_KALAY, FLAG_VALE_NUT_CAUGHT);
}

/// Vale's triggers for the story so far: before the party leaves, after it has
/// left, or back from Kalay.
const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    return gVale_TriggersAfterKalay;
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0)
    return gVale_TriggersAfterLeaving;
  return gVale_Triggers;
}

/// A little girl with orange pigtails, west of Garet's training stone, tells
/// Garet she is jealous that his grandpa is the mayor and asks whether Isaac
/// is too. After the party has left she speaks of the tremors.
static void Vale_TalkAboutTheMayorAndTremors(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    Event_SetMessageId(MsgValeTheGroundStillShakes);
    // "The ground still shakes sometimes, but everyone says the..."
    Event_ShowMessage(ACTOR_GIRL_JEALOUS_OF_GARET, 0);
  } else {
    Event_SetMessageId(MsgValeYourGrandpaIsTheMayor);
    Actor_FaceEachOther(ACTOR_GIRL_JEALOUS_OF_GARET, ACTOR_PARTY_LEADER, 4);
    // "I'm jealous of you, Garet, because your grandpa's the mayor...."
    // Yes: "Yeah... When travelers come, you get to hear all about the..."
    // No: "Hmph... I can't believe you're not jealous, Isaac. I sure..."
    Event_AskYesNo(ACTOR_GIRL_JEALOUS_OF_GARET, 0);
  }
  Event_End();
}

/// A woman with fair hair at the northern way west to Kraden's cottage asks
/// whether Isaac needs to go past, and warns him not to go beyond Kraden's
/// cottage. After the party has left she says Kraden still has not come back.
///
/// She asks only the first time (FLAG_VALE_ASKED_IF_GOING_TO_KRADEN), and
/// after that gives the warning alone.
static void Vale_WarnAboutGoingBeyondKradensCottage(void) {
  s32 asked = FLAG_VALE_ASKED_IF_GOING_TO_KRADEN;

  Event_Start();
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    Event_SetMessageId(MsgValeKradenHasntComeBack);
    // "I guess Kraden still hasn't come back."
    Event_ShowMessage(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  } else if (GameFlag_Get(asked) == 0) {
    GameFlag_Set(asked);
    Event_SetMessageId(MsgValeDoYouNeedToGo);
    Actor_FaceEachOther(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, ACTOR_PARTY_LEADER,
                        4);
    // "Do you need to go past here?"
    // Yes: "You must be going to Kraden's cottage..."
    // No: "You mustn't go beyond Kraden's cottage. There are monsters..."
    Event_AskYesNo(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  } else {
    Event_SetMessageId(MsgValeDontGoBeyondKradensCottage);
    Actor_FaceEachOther(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, ACTOR_PARTY_LEADER,
                        4);
    // "You mustn't go beyond Kraden's cottage. There are monsters..."
    Event_ShowMessage(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  }
  Event_End();
}

/// Garet's sister, by his training stone, scolds him: "Your training stone
/// crushed my flowers!" Afterwards she only threatens him with what will
/// happen tonight.
static void Vale_ArgueWithGaretsSister(void) {
  Event_Start();
  if (Event_GetFlag(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS) == 0) {
    GameFlag_Set(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS);
    Event_SetMessageId(MsgValeYouMakeMeSoMad);
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_SISTER, 20);
    // "Garet, you make me so mad sometimes!"
    Event_ShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 6);
    Actor_FaceDirection(ACTOR_GARETS_SISTER, FACING_WEST, 30);
    Actor_Jump(ACTOR_GARETS_SISTER, 2, 20);
    // "Your training stone crushed my flowers!"
    Event_ShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 6);
    Actor_FaceEachOther(ACTOR_GARETS_SISTER, ACTOR_PARTY_LEADER, 10);
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    // "You're in for it tonight... Just wait and see!"
    Event_ShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  } else {
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_SetMessageId(MsgValeIllGetYouForMy);
    // "I'll get you for my flowers! Just see what happens...tonight!"
    Event_ShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 20);
  }
  Event_End();
}

/// Dora, by the door of Isaac's house, speaks of the hole in the roof before
/// the party leaves. On the morning the roof was mended she frets about the
/// hole clumsy Garet left; later she says it won't rain for some time, so
/// Isaac can fix it later.
static void Vale_DoraTalksAboutTheHole(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VALE_ROOF_JUST_MENDED) != 0) {
    Event_SetMessageId(MsgValeIToldGaretItWas);
  } else {
    Event_SetMessageId(MsgValeItWontRainForSome);
  }
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_End();
}

/// Back from Kalay, the young man by the river in the south of Vale asks Isaac
/// whether Jenna is back yet, and makes him promise to bring her home.
///
/// Mind Read plays the same the first time. After he has asked
/// (FLAG_VALE_ASKED_IF_JENNA_IS_BACK), talking gives his plea alone and Mind
/// Read a thought of his.
static void Vale_AskWhetherJennaHasReturned(void) {
  Event_Start();
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, 20);
  Event_SetMessageId(MsgValeIsJennaBackYet);
  // "Oh, it's Isaac, isn't it? Is Jenna back yet? Promise me,..."
  Event_ShowMessage(ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, 0);
  GameFlag_Set(FLAG_VALE_ASKED_IF_JENNA_IS_BACK);
  Event_End();
}

/// Back from Kalay, a woman in a green dress cries out as the ground shakes:
/// "Aghh! I felt another one!" The tremors keep her awake at night.
static void Vale_FeelAnotherTremor(void) {
  Event_Start();
  MapGrid_SetShake(3, 3, 1);
  wait(10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_SetMessageId(MsgValeIFeltAnotherOne);
  // "Aghh! I felt another one!"
  Event_ShowMessageThenWait(ACTOR_WOMAN_FEELING_TREMORS, 0, 20);
  Actor_FaceActor(ACTOR_WOMAN_FEELING_TREMORS, ACTOR_PARTY_LEADER, 20);
  // "They've been going on for a while. I can't even sleep at night!"
  Event_ShowMessage(ACTOR_WOMAN_FEELING_TREMORS, 0);
  Event_End();
}

/// Back from Kalay, a burly grey-haired man wonders why no travellers have
/// come since the eruption, and asks whether Isaac knows. "Yes" tells him the
/// bridge to the south was ruined.
///
/// Talking to him always plays this. Mind Read plays it too until he has asked
/// (FLAG_VALE_ASKED_ABOUT_TRAVELERS), and then reads a thought of his.
static void Vale_AskAboutTheLackOfTravelers(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_MAN_MISSING_TRAVELERS, 2);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_MAN_MISSING_TRAVELERS, ACTOR_PARTY_LEADER, 20);
  Event_SetMessageId(MsgValeNoTravelersSinceTheEruption);
  // "I wonder why we haven't seen any travelers since the..."
  // Yes: "Oh, the bridge to the south was ruined? That explains it."
  // No: "You don't know either? It's not like we banned visitors..."
  Event_AskYesNo(ACTOR_MAN_MISSING_TRAVELERS, 0);
  GameFlag_Set(FLAG_VALE_ASKED_ABOUT_TRAVELERS);
  Event_End();
}

/// Nothing visible happens as Isaac steps up to either end of the rope bridge
/// across the river in the south of Vale, at cells (8, 85) to (8, 88) on the
/// west bank or (18, 84) to (18, 87) on the east. The game lays the bridge's
/// planks into the cells over the river, so that he can walk across it.
///
/// Until then the cells under that bridge are the river's, which he cannot
/// walk on. The planks are copied from the bridge's own layer, cells (40, 84)
/// to (46, 87) of the map, onto cells (10, 84) to (16, 87), and
/// FLAG_VALE_ON_BRIDGE keeps them there after a battle. The ends of the rope
/// bridge west of Isaac's house, cells (5, 56) to (5, 57) and (13, 56) to
/// (13, 57), run this too, though that bridge is always open.
static void Vale_StepOntoBridge(void) {
  GameFlag_Set(FLAG_VALE_ON_BRIDGE);
  MapGrid_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

/// Nothing visible happens as Isaac steps off either end of the rope bridge
/// across the river in the south of Vale, a cell further from the river than
/// Vale_StepOntoBridge's cells. The game puts the river's cells back under the
/// bridge.
///
/// The river's cells are kept below the bridge's own, at cells (40, 89) to
/// (46, 92) of the map. The ends of the bridge west of Isaac's house run this
/// too.
static void Vale_StepOffBridge(void) {
  GameFlag_Clear(FLAG_VALE_ON_BRIDGE);
  MapGrid_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

/// Walking into the door of the Sanctum, the small stone shrine in the north
/// of Vale, at cells (15, 27) and (16, 27), opens it with a thump, and Isaac
/// walks inside.
static void Vale_EnterSanctum(void) {
  Sound_Play(SE_THUMP);
  MapGrid_AnimateCells(gVale_SanctumDoorOpens, 45, 11);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 257, 420);
  Import_EventSetExitRequest(VALE_TO_SANCTUM);
}

/// Climbing the steps up Mt. Aleph in the north-west of Vale, cells (5, 16) to
/// (8, 16), takes Isaac up to Sol Sanctum's door, once Kraden has planned the
/// visit. Before that, Jenna steps out to turn him back
/// (Vale_JennaSaysKradenIsWaiting).
///
/// Kraden plans the visit in his cottage (FLAG_SANCTUM_VISIT_PLANNED,
/// vale_kradens_cottage.c).
static void Vale_ApproachMtAleph(void) {
  if (GameFlag_Get(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
    Vale_JennaSaysKradenIsWaiting();
  } else {
    Sound_Play(SE_MAP_EXIT);
    Import_EventSetExitRequest(VALE_TO_SOL_ENTRANCE);
  }
}

/// Walking out of Vale by the northern of its two ways west, cells (0, 38) to
/// (0, 40), takes Isaac on to the part of the village around Kraden's
/// cottage.
static void Vale_ExitToValeKradensCottageNorth(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(VALE_TO_VALE_KRADENS_COTTAGE_NORTH);
}

/// Walking out of Vale by the southern of its two ways west, where the puppies
/// play, cells (0, 49) and (0, 50), takes Isaac on to the part of the village
/// around Kraden's cottage.
static void Vale_ExitToValeKradensCottageSouth(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(VALE_TO_VALE_KRADENS_COTTAGE_SOUTH);
}

/// Walking out of Vale by the path at its east edge in the south, cells
/// (29, 86) and (29, 87), takes Isaac on to the plaza with the Psynergy Stone
/// and the town gate.
static void Vale_ExitToValePlaza(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(VALE_TO_VALE_PLAZA);
}

/// Walking into the door of Garet's house, north-east of Isaac's, at cell
/// (25, 46), opens it, and Isaac walks in to Garet's family.
///
/// The door opens with the same pictures as Isaac's (gVale_IsaacsDoorOpens).
static void Vale_EnterGaretsHouse(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_IsaacsDoorOpens, 54, 32);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 406, 727);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_EAST);
}

/// Walking into the door of the house just north-west of Isaac's, at cell
/// (16, 51), opens it, and Isaac walks in to the villagers who talk of Kraden
/// and his visitors.
static void Vale_EnterCottageRoomsA(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 45, 39);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 262, 805);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_SOUTH);
}

/// Walking into the door of Isaac's house, at cell (21, 56), opens it, and he
/// walks in. After the party has left Vale and until it is back from Kalay,
/// Dora will not open it: she asks who is there, and does not believe it is
/// her Isaac.
static void Vale_EnterIsaacsHouse(void) {
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0 &&
      GameFlag_Get(FLAG_KALAY_REACHED) == 0) {
    Event_Start();
    Event_SetMessageId(MsgValeYouCantBeIsaac);
    // "Who are you? You can't be... You're not Isaac, are you?"
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      // "Isaac... That's a lie!"
      Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
      // "He promised not to return until he fulfills his mission. My..."
      Event_ShowMessage(ACTOR_DORA, 0);
    } else {
      gFieldState->message += 2;
      Event_Wait(40);
      Event_ShowMessage(ACTOR_DORA, 0);
    }
    Event_End();
  } else {
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gVale_IsaacsDoorOpens, 50, 44);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 340, 888);
    Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_B);
  }
}

/// Walking into the door of the house of Jenna's grandparents, in the south of
/// Vale east of the river, at cell (20, 71), opens it, and Isaac walks in.
static void Vale_EnterJennasGrandparentsHouse(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 49, 69);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 326, 1126);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_NORTH);
}

/// Walking into the door of the house south of Jenna's grandparents', at cell
/// (23, 78), opens it, and Isaac walks in to the young man who dreams of
/// seeing the world.
///
/// The room is the one cottage_rooms_a.c enters from
/// COTTAGE_ROOMS_A_FROM_VALE_WEST, after its place on that map.
static void Vale_EnterCottageRoomsAWest(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_WestHouseDoorOpens, 52, 76);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 1238);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_WEST);
}

/// Unused: walking into the door of the house the storm wrecked on the west
/// bank would have opened it and taken Isaac into the house of Jenna's
/// grandparents.
///
/// No cell of Vale's map, by day or on the stormy night, carries its cell
/// behaviour, and that door's own cell answers Vale_SearchTheLockedDoor
/// instead. vale_prologue.c keeps the same unused door
/// (ValePrologue_UnusedLeaveToCottageRoomsAWestByWestBankDoor).
static void Vale_UnusedEnterJennasGrandparentsHouseByWestBankDoor(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_IsaacsDoorOpens, 35, 74);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_NORTH_WEST_BANK_DOOR);
}

/// At the foot of the steps up Mt. Aleph, before Kraden has planned the visit,
/// Jenna steps out of Isaac and tells him "Kraden is waiting for us." He nods,
/// she steps back in, and he walks back down off the steps.
///
/// Jenna is put where Isaac stands, as the party is drawn as its leader alone,
/// and walks out from there (Scene_WalkIntoLeader takes her back).
static void Vale_JennaSaysKradenIsWaiting(void) {
  struct Actor *leader;
  struct Actor *jenna;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  jenna = Actor_Get(ACTOR_JENNA);
  Event_Start();
  jenna->x.fixed = leader->x.fixed;
  jenna->y.fixed = leader->y.fixed;
  jenna->z.fixed = leader->z.fixed;
  jenna->targetX = ACTOR_NO_TARGET;
  jenna->targetY = ACTOR_NO_TARGET;
  jenna->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(jenna, 0, 0, 0);
  jenna->groundHeight = leader->y.fixed;
  wait(1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_WalkToAndWait(ACTOR_JENNA, 110, 283);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 2);
  Event_SetMessageId(MsgValeKradenIsWaitingForUs);
  if (leader->x.fixed < jenna->x.fixed) {
    // "Kraden is waiting for us. We should go to his house."
    Event_ShowMessageThenWait(
        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 2);
  } else {
    // "Kraden is waiting for us. We should go to his house."
    Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 2);
  }
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(2);
  Scene_WalkIntoLeader(ACTOR_JENNA);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 110, 303);
  Event_End();
}

/// Walking towards the puppies in the southern way west out of Vale from the
/// ledge north of them, cells (1, 47) to (5, 47) and (1, 48), sets them
/// barking. The view slides south to show them at play below, a message reads
/// "There are some puppies playing over there. We should let them be.", and
/// the view comes back before Isaac walks away north.
///
/// Once the party has met Saturos and Menardi the puppies are gone, and
/// nothing happens here. The view follows a point of its own, moved 2 pixels
/// a frame for 40 frames, while Isaac stands still.
static void Vale_WatchThePuppies(void) {
  struct MapGridScrollState *grid;
  s32 *savedOrigin;
  struct Actor *leader;
  s32 frame;
  FIXED cameraPosition[3];

  if (GameFlag_Get(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    grid = gMapGridState[0];
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_Wait(2);
    Event_SetMessageId(MsgValeRrruffRrrruff);
    // "Rrruff! Rrrruff!"
    Event_ShowMessageThenWait(ACTOR_FIRST_PUPPY, 0, 2);
    // "Woof! Woof!"
    Event_ShowMessageThenWait(ACTOR_FIRST_PUPPY + 1, 0, 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = grid->origin;
    grid->origin = cameraPosition;
    for (frame = 0; frame != 40; frame++) {
      cameraPosition[2] += toFIXED(2);
      Event_Wait(1);
      MapGrid_CenterOnFollowedPoint();
    }
    Event_Wait(60);
    FieldMessage_Show(MsgValePuppiesPlayingOver, 1);
    frame = 0;
    Event_Wait(6);
    do {
      cameraPosition[2] -= toFIXED(2);
      Event_Wait(1);
      frame++;
      MapGrid_CenterOnFollowedPoint();
    } while (frame != 40);
    grid->origin = savedOrigin;
    Event_Wait(60);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 70, 741);
    Event_End();
  }
}

/// Walking up to the puppies in the southern way west out of Vale from the
/// south, at cells (3, 53) and (4, 53), sets them barking. A message reads
/// "There are some puppies playing over there. We should let them be." and
/// Isaac walks back south.
///
/// The view stays on him, as the puppies are already in sight from this side.
/// Once the party has met Saturos and Menardi the puppies are gone, and
/// nothing happens here.
static void Vale_TurnBackFromThePuppies(void) {
  s32 msg;

  if (GameFlag_Get(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
    msg = MsgValeRrruffRrrruff;
    Event_SetMessageId(msg);
    // "Rrruff! Rrrruff!"
    Event_ShowMessageThenWait(ACTOR_FIRST_PUPPY, 0, 2);
    // "Woof! Woof!"
    Event_ShowMessageThenWait(ACTOR_FIRST_PUPPY + 1, 0, 2);
    FieldMessage_Show(msg + 2, 1);
    Event_Wait(6);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 870);
    Event_End();
  }
}

/// Walking north into the foot of the ladder against the front of Isaac's
/// house, east of his door, at cell (24, 55), climbs him up the ladder and over
/// the eaves onto the roof.
///
/// The roof's cells are copied in from cells (49, 53) to (56, 56) of the map
/// onto cells (20, 50) to (27, 53), so that he can walk on it, and
/// FLAG_VALE_ON_ROOF keeps them there after a battle.
static void Vale_ClimbLadderOntoRoof(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Vale_ClimbUpRoofLadder(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB,
                         ANIM_LEADER_CLIMB_OVER_TOP, 1);
  GameFlag_Set(FLAG_VALE_ON_ROOF);
  Event_End();
}

/// Walking south off the roof of Isaac's house at the top of the ladder,
/// cell (24, 53) once he is on the roof, climbs him down the ladder to the
/// ground in front of the house.
///
/// The ground's cells come back from cells (49, 46) to (56, 49) of the map.
static void Vale_ClimbDownFromRoof(void) {
  Event_Start();
  Vale_ClimbDownRoofLadder(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_DOWN_FROM_TOP,
                           ANIM_LEADER_CLIMB, 1);
  GameFlag_Clear(FLAG_VALE_ON_ROOF);
  MapGrid_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  Event_End();
}

/// The first time Isaac takes the path up the Sanctum's west side towards the
/// steps up Mt. Aleph, at its north end (cell (10, 23)), a bearded man in a
/// long robe jumps out from the trees by the river and drives him back to the
/// Sanctum's door. "You're not planning on sneaking up to Mt. Aleph, are you?"
///
/// From then on the man walks his beat (gVale_AlephGuardPatrolRoute) and
/// watches for Isaac on the path (Vale_CatchSneakingOnThePath and the two after
/// it). FLAG_VALE_WARNED_AGAINST_MT_ALEPH keeps this from playing again.
static void Vale_WarnAgainstSneakingUpMtAleph(void) {
  struct Actor *guard;

  guard = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
  Event_Start();
  Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 2);
  Actor_Jump(ACTOR_PARTY_LEADER, 5, 0);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 215, 403);
  guard->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetPosition(ACTOR_MAN_GUARDING_MT_ALEPH, 166, 375);
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTHEAST, 20);
  guard->steeringFlags = (guard->steeringFlags ^ ACTOR_FACES_MOTION);
  Actor_SetSpeedAndAcceleration(ACTOR_MAN_GUARDING_MT_ALEPH, 2.5, 2.5);
  Actor_Jump(ACTOR_MAN_GUARDING_MT_ALEPH, 4, 0);
  Actor_WalkToAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, 202, 395);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTH_SOUTHEAST, 24);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetSpeedAndAcceleration(ACTOR_MAN_GUARDING_MT_ALEPH, 1.5, 1);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_FirstWarningLeaderPath);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_SetScript(ACTOR_MAN_GUARDING_MT_ALEPH, gVale_FirstWarningGuardPath);
  Actor_WaitForScriptEndById(ACTOR_PARTY_LEADER);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 474);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WaitForScriptEndById(ACTOR_MAN_GUARDING_MT_ALEPH);
  Actor_WalkToAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, 256, 456);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTH, 20);
  Actor_StartRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgValeNotSneakingUpMtAleph);
  // "You're not planning on sneaking up to Mt. Aleph, are you?"
  // Yes: "Mt. Aleph is a sacred peak. No one can climb it without..."
  // No: "Well, I'll overlook it this time... But I've got my eye on..."
  Event_AskYesNo(ACTOR_MAN_GUARDING_MT_ALEPH, 0);
  Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
      (void (*)(struct Actor *))Vale_HaveGuardWatchTheLeader;
  Actor_SetScript(ACTOR_MAN_GUARDING_MT_ALEPH, gVale_AlephGuardPatrolRoute);
  GameFlag_Set(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
  Event_End();
}

/// Walking up the path along the Sanctum's west side, cells (11, 23) to
/// (13, 27), or just south of its foot, cells (12, 30) and (13, 30), while the
/// man guarding Mt. Aleph is on that path catches Isaac: he runs back to the
/// Sanctum's door, the man follows him down, and asks again whether he is
/// sneaking up Mt. Aleph.
///
/// While the man is in front of the Sanctum or behind it, Isaac walks on
/// unseen.
static void Vale_CatchSneakingOnThePath(void) {
  struct Actor *guard = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
  if (GameFlag_Get(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (guard->timer == GUARD_ON_THE_PATH) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderRunsBackFromThePath,
                                   gVale_GuardComesDownThePath);
    }
  }
}

/// The man guarding Mt. Aleph catches Isaac sneaking: both start, Isaac
/// winces and runs back by `leaderActions`, the man follows him by
/// `guardActions` to stand before the Sanctum's door, and asks "You're not
/// planning on sneaking up to Mt. Aleph, are you?" before going back to his
/// beat.
static void Vale_AskAboutSneakingToAleph(const s32 *leaderActions,
                                         const s32 *guardActions) {
  const s32 *replyActions;
  struct Actor *guard;
  struct Actor *actor;

  replyActions = guardActions;
  guard = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
  Event_Start();
  Actor_StartRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetScript(ACTOR_PARTY_LEADER, leaderActions);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_RunScriptAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, replyActions);
  Actor_WaitForScriptEndById(ACTOR_PARTY_LEADER);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  guard->scaleX = FIXED_ONE;
  guard->scaleY = FIXED_ONE;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  Event_SetMessageId(MsgValeNotSneakingUpMtAleph);
  // "You're not planning on sneaking up to Mt. Aleph, are you?"
  // Yes: "Mt. Aleph is a sacred peak. No one can climb it without..."
  // No: "Well, I'll overlook it this time... But I've got my eye on..."
  Event_AskYesNo(ACTOR_MAN_GUARDING_MT_ALEPH, 0);
  Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
      (void (*)(struct Actor *))Vale_HaveGuardWatchTheLeader;
  Actor_SetScript(ACTOR_MAN_GUARDING_MT_ALEPH, gVale_AlephGuardPatrolRoute);
  Event_End();
}

/// Stepping into the corner just west of the Sanctum's front, cells (14, 28)
/// and (14, 29), while the man guarding Mt. Aleph stands or walks in front of
/// the Sanctum catches Isaac: he runs back before the Sanctum's door, and the
/// man steps up to him and asks again whether he is sneaking up Mt. Aleph.
static void Vale_CatchSneakingBesideTheSanctum(void) {
  struct Actor *guard = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
  if (GameFlag_Get(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (guard->timer == GUARD_BEFORE_THE_SANCTUM) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderRunsBackBeforeTheDoor,
                                   gVale_GuardStepsBeforeTheDoor);
    }
  }
}

/// Stepping onto the foot of the path along the Sanctum's west side, cells
/// (12, 28) to (13, 29), while the man guarding Mt. Aleph is on the path or in
/// front of the Sanctum catches Isaac: he runs back before the Sanctum's door,
/// and the man comes to him and asks again whether he is sneaking up Mt.
/// Aleph.
static void Vale_CatchSneakingAtThePathsFoot(void) {
  struct Actor *guard = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
  if (GameFlag_Get(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    s32 stretch;
    stretch = guard->timer;
    if (stretch == GUARD_ON_THE_PATH) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderRunsBackBeforeTheDoor,
                                   gVale_GuardComesDownThePath);
    } else if (stretch == GUARD_BEFORE_THE_SANCTUM) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderRunsBackBeforeTheDoor,
                                   gVale_GuardStepsBeforeTheDoor);
    }
  }
}

/// Unused: another cut of Vale_RoofMended, the end of the morning the
/// story begins.
///
/// No table names it. It plays the same lines as the event the game uses, `Good
/// job, Isaac.` and `I was devastated when Kyle died...` among them, but frames
/// them with its own camera and map grid cells, and sets
/// FLAG_VALE_ROOF_JUST_MENDED as that event does.
UNUSED static void Vale_UnusedRoofMended(void) {
  struct Actor *leader;
  struct Actor *actor;
  u8 leaderMotionFlags;
  s32 i;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  actor = FieldCamera_GetActor();
  actor->motionFlags = 0;
  FieldCamera_MoveTo(383, 160, 877, 0);
  Event_Wait(1);
  MapGrid_CenterOnFollowedPoint();
  MapGrid_CopyCellAttributes(49, 41, 7, 3, 20, 50);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leaderMotionFlags = leader->motionFlags;
  leader->motionFlags = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, 407, 690);
  Actor_SetPosition(ACTOR_DORA, 392, 896);
  Actor_SetPosition(ACTOR_GARET, 298, 736);
  Actor_SetPosition(ACTOR_JENNA, 298, 760);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Vale_PlaceOrHideBoards(ACTOR_FIRST_BOARD, 2, 1);
  gFieldState->transition = 0;
  gFieldState->transitionFrames = 32;
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_JENNA, 0.5);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  Actor_SetScript(ACTOR_JENNA, gVale_JennaWalksToTheHouse);
  Actor_SetScript(ACTOR_GARET, gVale_GaretWalksToTheHouse);

  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.3);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 404, 843);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_RBG);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, 0.3);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 402, 828);
  Event_Wait(80);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_FIRST_BOARD, 390, 842);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(200);
  MapGrid_CopyCellsTo(7, 102, 84, 41, 2, 1);

  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_RBG);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, 0.3);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 377, 828);
  Event_Wait(80);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_FIRST_BOARD, 0, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(200);
  MapGrid_CopyCellsTo(6, 102, 83, 41, 1, 1);

  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 20);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
  Actor_SetSpriteColorById(ACTOR_SECOND_BOARD, SPRITE_COLOR_RBG);
  Actor_SetSpeed(ACTOR_SECOND_BOARD, 0.3);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 390, 832);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 360, 842);
  Event_Wait(80);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_SECOND_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_SECOND_BOARD, 0, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(200);
  MapGrid_CopyCellsTo(5, 103, 82, 42, 1, 1);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;

  Event_SetMessageId(MsgValeGoodJob);
  Actor_Jump(ACTOR_DORA, 2, 20);
  // "Good job, Isaac."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 20);
  Vale_ClimbUpRoofLadder(ACTOR_DORA, DORA_ANIMATION_CLIMB,
                         DORA_ANIMATION_CLIMB_OVER_TOP, 0);
  Actor_SetSpeed(ACTOR_DORA, 0.3);
  Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    gFieldState->message++;
  }
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Event_SetMessageId(MsgValeWorkingYourselvesBone);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  // "You and Garet have been working yourselves to the bone..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
  // "Do you think things would be any different if you'd had..."
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldState->message++;
  }
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgValeDevastatedWhenKyle);
  // "I was devastated when Kyle died..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  // "I felt all alone..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 10);
  // "It's taken some time, but I'm better now."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  // "I still have you, Isaac."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  // "You're almost seventeen. You're so much like Kyle was when..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  // "I'm looking forward to seeing where life takes you."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);

  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(377, 160, 860, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_WalkTo(ACTOR_GARET, 369, 904);
  Actor_WalkToAndWait(ACTOR_JENNA, 392, 904);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Vale_ClimbUpRoofLadder(ACTOR_JENNA, ANIM_LEADER_CLIMB,
                         ANIM_LEADER_CLIMB_OVER_TOP, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 30);
  Actor_Jump(ACTOR_JENNA, 4, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 392, 843);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 40);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Vale_ClimbUpRoofLadder(ACTOR_GARET, ANIM_LEADER_CLIMB,
                         ANIM_LEADER_CLIMB_OVER_TOP, 0);
  Actor_SetSpeed(ACTOR_JENNA, 0.3);
  Actor_SetSpeed(ACTOR_GARET, 0.3);
  Actor_WalkTo(ACTOR_GARET, 392, 843);
  Actor_Get(ACTOR_JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JENNA, 408, 843);
  Event_Wait(1);
  Actor_Get(ACTOR_JENNA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_Jump(ACTOR_DORA, 4, 30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 40);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(100);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(373, 160, 837, 1);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 40);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldState->message++;
  }
  Event_Wait(40);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);

  Event_SetMessageId(MsgValeAsStubbornAsYourFather);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Event_Wait(30);
  // "You're just as stubborn as your father!"
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_Jump(ACTOR_DORA, 4, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Actor_SetAnimation(ACTOR_DORA, DORA_ANIMATION_HANDS_UP);
  Event_Wait(5);
  // "Ahhh!"
  // "Be careful, ma'am!"
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, ACTOR_GARET, 10,
                                    14, 4, 14, 0);
  actor = Actor_Get(ACTOR_DORA);
  actor->sprite->flags = 0;
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_DORA, 3);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
  Event_Wait(4);
  for (i = 0; i != 4; i++) {
    actor->z.fixed += toFIXED(3) / 2;
    actor->scaleY -= FIXED_ONE / 10;
    Event_Wait(1);
  }
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetSpeed(ACTOR_GARET, 3);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 374, 827);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Actor_SetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
  Actor_Jump(ACTOR_GARET, 2, 5);
  MapGrid_SetShake(0, 4, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_SetAnimation(ACTOR_DORA, DORA_ANIMATION_PEEK);
  actor->scaleY = FIXED_ONE / 2;
  Actor_SetPosition(ACTOR_DORA, 364, 811);
  for (i = 0; i != 5; i++) {
    actor->scaleY += FIXED_ONE / 10;
    Event_Wait(1);
  }
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  FieldCamera_SetSpeed(0.3);
  FieldCamera_MoveTo(372, 160, 859, 1);
  Actor_SetSpeed(ACTOR_DORA, 3);
  Actor_Jump(ACTOR_DORA, 6, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(30);
  actor = Actor_Get(ACTOR_DORA);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetSpeed(ACTOR_GARET, 4);
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_GARET, 403, 827);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 20);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 1);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
  Actor_Jump(ACTOR_GARET, 2, 5);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_SetShake(0, 4, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(ACTOR_JENNA, 0.3);
  Actor_WalkToAndWait(ACTOR_JENNA, 408, 855);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 70);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 398, 828);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(60);

  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_JENNA);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(ACTOR_JENNA, actor->x.part[0].integer + 16,
               actor->z.part[0].integer);
  Actor_WalkToAndWait(ACTOR_GARET, actor->x.part[0].integer + 16,
                      actor->z.part[0].integer - 16);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_JENNA, actor->x.part[0].integer,
                      actor->z.part[0].integer);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WalkToAndWait(ACTOR_GARET, actor->x.part[0].integer,
                      actor->z.part[0].integer);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_AddPartyMemberPair(GARET, JENNA);
  FieldCamera_MoveTo(377, 160, 887, 1);
  Vale_ClimbDownRoofLadder(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_DOWN_FROM_TOP,
                           ANIM_LEADER_CLIMB, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_Get(ACTOR_DORA)->steeringFlags |= ACTOR_FACES_MOTION;
  Vale_ClimbDownRoofLadder(ACTOR_DORA, DORA_ANIMATION_CLIMB_OVER_TOP,
                           DORA_ANIMATION_CLIMB, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForMove();
  Event_Wait(100);
  GameFlag_Set(FLAG_VALE_ROOF_JUST_MENDED);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  leader->motionFlags = leaderMotionFlags;
  Event_End();
}

/// Sets Vale up as the party arrives. Arriving from the title, the game's
/// opening plays, with Isaac mending the roof; arriving back in front of his
/// house after it, the end of that morning plays once.
///
/// Otherwise the setup floats the Nut in its tree until it is caught, stands
/// Garet's training stone in its column, sends the puppies away once the
/// party has met Saturos and Menardi, starts the man guarding Mt. Aleph on his
/// beat once he has warned Isaac, and after a battle puts back the roof's and
/// the bridge's cells. After the party has left and until it is back from
/// Kalay, Dora stands inside Isaac's house shrunk to nothing, so that she can
/// answer through the door (Vale_EnterIsaacsHouse).
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 set;

  if (gMainRoutineState.entrance == VALE_FROM_TITLE) {
    WindowColor_Apply(gMainRoutineState.windowHue,
                      gMainRoutineState.windowBrightness);
    Vale_MendingTheRoof();
  } else {
    if (!GameFlag_Get(FLAG_VALE_NUT_CAUGHT)) {
      if (!GameFlag_Get(FLAG_KALAY_REACHED)) {
        Scene_FloatNut(ACTOR_NUT);
      } else {
        Scene_FloatNut(ACTOR_NUT_AFTER_KALAY);
      }
    }
    // The hole Garet left in the roof when he jumped onto it, on map layer 2
    // (Vale_MendingTheRoof).
    MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
    MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
    actor = Actor_Get((GameFlag_Get(FLAG_KALAY_REACHED) != 0) + ACTOR_PILLAR);
    Actor_SetSpriteFlags(actor, 0);
    if (GameFlag_Get(FLAG_VALE_PILLAR_IN_WEST_COLUMN)) {
      actor->x.fixed = toFIXED(PILLAR_WEST_X);
    } else if (GameFlag_Get(FLAG_VALE_PILLAR_IN_EAST_COLUMN)) {
      actor->x.fixed = toFIXED(PILLAR_EAST_X);
    } else {
      actor->x.fixed = toFIXED(PILLAR_MIDDLE_X);
    }
    actor->z.fixed = toFIXED(PILLAR_Z);
    actor->y.fixed = toFIXED(192);
    Vale_MovePillarBelowCliff();
    actor->layer = 3;
    actor->motionFlags = 0;
    Task_AddFrame(Vale_SetPillarPriorityByLeaderHeight, TASK_PASS_FRAME);
#if TBS_PATCH_4
    if (GameFlag_Get(FLAG_KALAY_REACHED)) {
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        // The cells Vale_ClimbLadderOntoRoof copies as it sets the flag,
        // redrawn on arrival.
        if (GameFlag_Get(FLAG_VALE_ON_ROOF)) {
          MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (GameFlag_Get(FLAG_VALE_ON_BRIDGE)) {
          Vale_StepOntoBridge();
        }
      }
    } else {
#else
    if (!GameFlag_Get(FLAG_KALAY_REACHED)) {
#endif
      if (Event_GetFlag(FLAG_VALE_PARTY_LEFT)) {
        actor = Actor_Get(ACTOR_DORA);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_DORA), 0);
        actor->scaleX = FIXED_ONE / 100;
        actor->scaleY = FIXED_ONE / 100;
      }
      if (GameFlag_Get(FLAG_MET_SATUROS_AND_MENARDI)) {
        Actor_SetPosition(ACTOR_FIRST_PUPPY, 0, 0);
        Actor_SetPosition(ACTOR_FIRST_PUPPY + 1, 0, 0);
        Actor_SetPosition(ACTOR_FIRST_PUPPY + 2, 0, 0);
      }
      set = GameFlag_Get(FLAG_VALE_PARTY_LEFT);
      if (set == 0) {
        if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
          if (GameFlag_Get(FLAG_VALE_WARNED_AGAINST_MT_ALEPH)) {
            Actor_SetPosition(ACTOR_MAN_GUARDING_MT_ALEPH, 256, 456);
#ifdef UBFIX
            Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
                Vale_HaveGuardWatchTheLeader;
#else
            Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
                (void *)Vale_HaveGuardWatchTheLeader;
#endif
            Actor_SetScript(ACTOR_MAN_GUARDING_MT_ALEPH,
                            gVale_AlephGuardPatrolRoute);
          }
        } else {
          Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = set;
          GameFlag_Clear(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
        }
        if (gMainRoutineState.entrance != VALE_FROM_TITLE &&
            !GameFlag_Get(FLAG_KALAY_REACHED)) {
          Task_AddFrame(Vale_SynchronizeAlephGuardPause, TASK_PASS_FRAME);
        }
      }
      if (!Event_GetFlag(FLAG_VALE_ROOF_MENDED_EVENT_PLAYED) &&
          gMainRoutineState.entrance == VALE_FROM_ROOF_MENDED) {
        Vale_RoofMended();
        GameFlag_Set(FLAG_VALE_ROOF_MENDED_EVENT_PLAYED);
      }
#if TBS_PATCH_4
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        // The cells Vale_ClimbLadderOntoRoof copies as it sets the flag,
        // redrawn on arrival.
        if (GameFlag_Get(FLAG_VALE_ON_ROOF)) {
          MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (GameFlag_Get(FLAG_VALE_ON_BRIDGE)) {
          Vale_StepOntoBridge();
        }
      }
      {
        s32 priority;

        // DEAD: a byte shifted down 8 places is 0, so Dora's priority is 2;
        // the ROM keeps the byte's mask and shift.
        priority = ((u32)(u8)set >> 8) + 2;
        Actor_SetSpritePriority(ACTOR_DORA, priority);
      }
#endif
    }
#if !TBS_PATCH_4
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      // The cells Vale_ClimbLadderOntoRoof copies as it sets the flag, redrawn
      // on arrival.
      if (GameFlag_Get(FLAG_VALE_ON_ROOF)) {
        MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
      }
      if (GameFlag_Get(FLAG_VALE_ON_BRIDGE)) {
        Vale_StepOntoBridge();
      }
    }
#endif
    Scene_QueueMusic(SE_AMBIENT_WATER);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  return 0;
}

/// Back from Kalay, talking to Garet's sister by his training stone, or
/// reading her mind, brings Garet out to her. "It's been a while..." She
/// admits she was worried about him, and is sorry he is leaving again.
///
/// FLAG_VALE_GARET_MET_SISTER keeps this from playing twice.
static void Vale_GaretMeetsHisSister(void) {
  struct Actor *actor;
  s32 msg;

  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_FaceActor(ACTOR_GARETS_SISTER_AFTER_KALAY, ACTOR_PARTY_LEADER, 20);
  msg = MsgValeAh;
  Event_SetMessageId(msg);
  Actor_StartRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 2);
  // "Ah!"
  Event_ShowMessageThenWait(ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(398, CAMERA_KEEP, 582, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 420, 608);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY, FACING_SOUTH_SOUTHEAST,
                      0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  Actor_WalkToAndWait(ACTOR_GARET, 402, 608);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
  // "It's been a while..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY, FACING_SOUTH_SOUTHWEST,
                      20);
  Actor_SetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_NOD);
  // "You're back?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 2);
  // "Have you gone home to see Dad yet?"
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldState->message++;
    Actor_StartRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 1);
  }
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 40);
  Actor_ShowEmote(ACTOR_GARETS_SISTER_AFTER_KALAY,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_SetMessageId(msg + 6);
  // "...So have I."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(40);
  // "Sis..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 1);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY, FACING_NORTH_NORTHEAST,
                      20);
  // "Don't get me wrong. I... I was just worried you might get..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  // "Yeah, all right... Thanks, Sis."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 120);
  // "So, are you home for good? Or..."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "Not just yet..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_SHAKE_HEAD);
  // "You're leaving... again?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY, FACING_SOUTH_SOUTHWEST,
                      20);
  // "Don't push yourself too hard...when you leave again."
  Event_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_NOD);
  Scene_WalkIntoLeader(ACTOR_GARET);
  GameFlag_Set(FLAG_VALE_GARET_MET_SISTER);
  Event_End();
}

/// Back from Kalay, talking to Garet's little brother west of the door of
/// Garet's house, or reading his mind, brings Garet out to him. The boy
/// wonders that they are back already, and Garet tells him to protect the
/// family while he is gone.
///
/// FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY keeps this from playing twice.
static void Vale_GaretAsksToProtectFamily(void) {
  struct Actor *actor;

  Event_Start();
  FieldCamera_MoveTo(357, CAMERA_KEEP, 738, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 367, 745);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  Actor_WalkToAndWait(ACTOR_GARET, 346, 745);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
  Event_SetMessageId(MsgValeHowHaveYouBeen);
  // "Hey, how have you been?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_RunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 2);
  Actor_ShowEmote(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY, FACING_SOUTH_SOUTHEAST,
                      10);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY, FACING_SOUTH_SOUTHWEST,
                      10);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY, FACING_SOUTH_SOUTHEAST,
                      40);
  // "Oh, Garet! And Isaac!"
  Event_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 1);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY, FACING_SOUTH_SOUTHWEST,
                      10);
  // "What happened!? You're back already!?"
  Event_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  // "Already? What kind of way is that to greet us?"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  // "But-But they said you might never come back!"
  Event_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_SHAKE_HEAD);
  // "And it still seems like you're not back for good, like..."
  Event_ShowMessage(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "Really? Is that what it seems like?"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  // "Yeah... You and your friends... You all look like you are..."
  Event_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  // "You're smarter than you look."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 2);
  // "I have to shape up while you're gone, right? If not..."
  Event_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 20);
  // "Hey, the family'll be fine, as long as you're here! Am I..."
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  } else {
    gFieldState->message++;
  }
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 10);
  Event_SetMessageId(MsgValeEverProtectFamily);
  // "WHATEVER YOU DO, you must protect the family while I'm gone...."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  // "Don't push yourself too hard, OK?"
  Event_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Scene_WalkIntoLeader(ACTOR_GARET);
  GameFlag_Set(FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY);
  Event_End();
}

/// Unused: water bursts up by the river after a tremor. Back from Kalay, a
/// tremor shakes Vale and water bursts up three times north of the strongman by
/// the river who remembers Felix.
///
/// No table names it. He would have started, sniffled through `I still remember
/// when Felix and I used to play here...*Sniff*`, turned to the leader to say
/// that Felix must be protecting Jenna, and set area flag
/// FLAG_VALE_UNUSED_WATER_BURST_SEEN. His talk and Mind Read rows show the same
/// two lines.
UNUSED static void Vale_UnusedWaterBurstsByTheRiver(void) {
  const s32 *actions;

  Event_Start();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WATER), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WATER + 1), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WATER + 2), 0);
  Actor_SetAnimation(ACTOR_FIRST_WATER, 0);
  Actor_SetAnimation(ACTOR_FIRST_WATER + 1, 0);
  Actor_SetAnimation(ACTOR_FIRST_WATER + 2, 0);
  wait(20);
  MapGrid_SetShake(2, 2, 1);
  actions = gVale_UnusedWaterGrows;
  Actor_SetScript(ACTOR_FIRST_WATER, actions);
  wait(10);
  Actor_SetScript(ACTOR_FIRST_WATER + 1, actions);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  wait(20);
  Actor_RunScriptAndWait(ACTOR_FIRST_WATER + 2, actions);
  Actor_ShowEmote(ACTOR_STRONGMAN_REMEMBERING_FELIX,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_RunRepeatedMotion(ACTOR_STRONGMAN_REMEMBERING_FELIX, 2);
  Actor_FaceDirection(ACTOR_STRONGMAN_REMEMBERING_FELIX, FACING_NORTH_NORTHEAST,
                      10);
  Event_SetMessageId(MsgValeIUsedToPlayHere);
  // "I still remember when Felix and I used to play here...*Sniff*"
  Event_ShowMessageThenWait(ACTOR_STRONGMAN_REMEMBERING_FELIX, 0, 40);
  Actor_FaceActor(ACTOR_STRONGMAN_REMEMBERING_FELIX, ACTOR_PARTY_LEADER, 20);
  // "Felix must be protecting Jenna... I'm sure of it."
  Event_ShowMessage(ACTOR_STRONGMAN_REMEMBERING_FELIX, 0);
  Actor_FaceDirection(ACTOR_STRONGMAN_REMEMBERING_FELIX, FACING_WEST, 10);
  GameFlag_Set(FLAG_VALE_UNUSED_WATER_BURST_SEEN);
  Event_End();
}

/// Walks an actor to the foot of the ladder beside Isaac's house and climbs it
/// onto the roof, in `approachAnimationId` on the rungs and
/// `departureAnimationId` over the eaves.
static void Vale_ClimbUpRoofLadder(s32 actorId, s32 approachAnimationId,
                                   s32 departureAnimationId, s32 resumeMotion) {
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_Get(actorId);
  sprite = actor->sprite;
  Actor_SetSpeed(actorId, 1);
  Actor_WalkToAndWait(actorId, 392, 886);
  // The leader, not actorId, is turned north. The player sees no turn: Isaac
  // stays facing south on the roof while Dora climbs, and already faces north
  // when Jenna and Garet do.
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  actor->motionFlags = 0;
  sprite->flags = 0;
  Actor_SetAnimation(actorId, approachAnimationId);
  Actor_SetSpeed(actorId, 0.3);
  Actor_MoveToAndWait(actorId, 392, 875);
  Event_Wait(10);
  Actor_SetAnimation(actorId, departureAnimationId);
  Actor_SetSpeed(actorId, 1);
  Actor_MoveToAndWait(actorId, 392, 859);
  sprite->flags = SPRITE_FLAG_SHADOW;
  if (resumeMotion != 0) {
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  }
  Event_Wait(10);
  Actor_SetAnimation(actorId, ANIM_STAND);
}

/// Climbs an actor down the ladder beside Isaac's house from the roof, and
/// restores falling motion if requested.
static void Vale_ClimbDownRoofLadder(s32 actorId, s32 approachAnimationId,
                                     s32 departureAnimationId,
                                     s32 resumeMotion) {
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_Get(actorId);
  sprite = actor->sprite;
  Actor_SetSpeed(actorId, 1);
  Actor_WalkToAndWait(actorId, 392, 859);
  Actor_FaceDirection(actorId, FACING_NORTH, 10);
  actor->motionFlags = 0;
  sprite->flags = 0;
  Actor_SetAnimation(actorId, approachAnimationId);
  Actor_SetSpeed(actorId, 1);
  Actor_MoveToAndWait(actorId, 392, 875);
  Event_Wait(10);
  Actor_SetSpeed(actorId, 0.3);
  Actor_SetAnimation(actorId, departureAnimationId);
  Actor_MoveToAndWait(actorId, 392, 890);
  actor->velocityY = toFIXED(2);
  sprite->flags = SPRITE_FLAG_SHADOW;
  if (resumeMotion != 0) {
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  }
  Actor_SetAnimation(actorId, ANIM_STAND);
}

/// Lays `count` of the roof's boards, from actor `first` on, by the hole in the
/// roof, still and without shadows; with any `mode` but 0 it puts them out of
/// sight instead.
static void Vale_PlaceOrHideBoards(s32 first, u32 count, s32 mode) {
  s32 actorId = first;
  u32 i;

  if (mode == 0) {
    for (i = 0; i < count; i++) {
      struct Actor *actor = Actor_Get(actorId);

      actor->motionFlags = 0;
      Actor_SetSpriteFlags(actor, 0);
      actor->x.fixed = toFIXED(390);
      actor->y.fixed = toFIXED(160);
      actor->z.fixed = toFIXED(842);
      actorId++;
    }
    return;
  }

  for (i = 0; i < count; i++) {
    Actor_SetPosition(actorId, 0, 0);
    actorId++;
  }
}

/// The game's opening, three years after the storm: Isaac is mending the roof
/// of his house while Dora calls up to him, "Isaac! Haven't you finished
/// mending the roof yet?" Jenna comes up from the south past them, finds Garet
/// training his Psynergy on his training stone, and the two talk before
/// setting off to fetch Isaac.
///
/// It plays as Vale is set up from the title (VALE_FROM_TITLE), and ends by
/// taking the party back in front of Isaac's house (VALE_TO_ROOF_MENDED),
/// where Vale_RoofMended goes on.
static void Vale_MendingTheRoof(void) {
  u32 i;
  struct Actor *repairActor = NULL;
  struct Actor *leader;
  struct Actor *moveHand;
  s32 message;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  moveHand = Actor_Get(ACTOR_OPENING_MOVE_HAND);

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  (FieldCamera_GetActor())->motionFlags = 0;
  wait(1);
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  // The hole in the roof, three cells of its picture, which the boards mend
  // one by one at the roof's end from the whole pictures at (5, 102) to
  // (7, 102). The roof is on map layer 2, the roofs over the actors, whose
  // cells begin at the map's column 60: its cells (22, 41) to (24, 42) are the
  // map's (82, 41) to (84, 42).
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  repairActor = Actor_GetById(ACTOR_OPENING_FIRST_BOARD);
  repairActor->motionFlags = 0;
  repairActor->x.fixed = toFIXED(388);
  repairActor->y.fixed = toFIXED(160);
  repairActor->z.fixed = toFIXED(840);
  Actor_SetSpriteFlags(repairActor, 0);
  repairActor = Actor_GetById(ACTOR_OPENING_SECOND_BOARD);
  repairActor->motionFlags = 0;
  repairActor->y.fixed = toFIXED(160);
  repairActor->z.fixed = toFIXED(844);
  repairActor->x.fixed = toFIXED(388);
  Actor_SetSpriteFlags(repairActor, 0);
  repairActor = Actor_GetById(ACTOR_OPENING_THIRD_BOARD);
  repairActor->motionFlags = 0;
  repairActor->z.fixed = toFIXED(848);
  repairActor->y.fixed = toFIXED(160);
  repairActor->x.fixed = toFIXED(388);
  Actor_SetSpriteFlags(repairActor, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_OPENING_FIRST_BOARD), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_OPENING_SECOND_BOARD), 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  ActorSystem_SuspendProjectionAndShowSprites();
  message = MsgValeRepairCaption;
  FieldMessage_ShowCentered(message, 0, 0);
  ActorSystem_ResumeProjectionAndHideLayers();

  FieldCamera_MoveTo(339, 160, 1173, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  FieldCamera_SetSpeed(0.33);
  FieldCamera_MoveTo(296, 160, 921, 1);
  Actor_SetPosition(ACTOR_JENNA, 409, 1134);
  wait(1);
  Actor_SetSpeed(ACTOR_JENNA, 0.7);
  Actor_WalkTo(ACTOR_JENNA, 420, 1068);
  Scene_PlayMusic();
  gFieldState->transitionFrames = 60;
  Event_StartScreenTransition();
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_WalkToAndWait(ACTOR_JENNA, 341, 1064);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 359, 1033);
  Actor_SetSpeed(ACTOR_OPENING_WOMAN, 0.5);
  Actor_WalkTo(ACTOR_OPENING_WOMAN, 318, 947);
  Actor_SetAnimation(ACTOR_OPENING_WOMAN, ANIM_WALK);
  Actor_WalkToAndWait(ACTOR_JENNA, 412, 1033);
  Actor_WalkToAndWait(ACTOR_JENNA, 412, 1019);
  Actor_WalkToAndWait(ACTOR_JENNA, 374, 1008);
  Actor_WalkToAndWait(ACTOR_JENNA, 347, 955);
  Actor_WalkToAndWait(ACTOR_OPENING_WOMAN, 318, 947);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_OPENING_WOMAN, 40);
  Actor_RunRepeatedMotion(ACTOR_OPENING_WOMAN, 2);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_WalkTo(ACTOR_OPENING_WOMAN, 379, 1017);
  FieldCamera_SetSpeed(0.5);
  Actor_WalkToAndWait(ACTOR_JENNA, 333, 920);
  Actor_WalkToAndWait(ACTOR_JENNA, 299, 924);
  FieldCamera_WaitForMove();
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST_NORTHEAST, 30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(387, 160, 866, 1);
  FieldCamera_WaitForMove();
  Actor_Jump(ACTOR_OPENING_DORA, 2, 20);
  Event_SetMessageId(message + 1);
  // "Isaac! Haven't you finished mending the roof yet?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_OPENING_DORA, 0,
                                 10);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  Actor_FaceEachOther(ACTOR_OPENING_DORA, ACTOR_PARTY_LEADER, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_OPENING_DORA, 2);
  Event_Wait(40);
  // "You're almost done? OK! Keep at it!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_OPENING_DORA, 0, 40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  FieldCamera_FollowActor(ACTOR_JENNA, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHEAST, 10);
  Actor_WalkToAndWait(ACTOR_JENNA, 312, 759);
  Actor_WalkToAndWait(ACTOR_JENNA, 361, 760);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 40);
  // "Hyaa!"
  Event_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_JENNA, 4, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(40);
  // "Hee hee... Still training, Garet?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(396, CAMERA_KEEP, 588, 1);
  Actor_WalkToAndWait(ACTOR_JENNA, 456, 739);
  FieldCamera_WaitForMove();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);

  // Garet trains his Psynergy: the hand of Move pushes the pillar, shedding
  // sparkles.
  Scene_StartEventPsynergy();
  Actor_SetAnimation(ACTOR_GARET, ANIM_CAST_PSYNERGY);
  // "Hyaaaa!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  for (i = 0; i < 60; i++) {
    Vale_FlashActorAndSpawnEffects(Actor_Get(ACTOR_GARET));
    wait(1);
  }
  Actor_SetSpritePriority(ACTOR_GARET, 1);
  Task_AddFrame(Vale_UpdateGaretPaletteAndEffects, TASK_PASS_FRAME);
  Task_AddFrame(Vale_CycleOpeningPillarPalette, TASK_PASS_FRAME);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_OPENING_MOVE_HAND), 0);
  moveHand->motionFlags = 0;
  moveHand->x.fixed = toFIXED(428);
  moveHand->y.fixed = toFIXED(208);
  moveHand->z.fixed = toFIXED(584);
  moveHand->facing = FACING_WEST;
  moveHand->update = Vale_TrailSparkles;
  Event_Wait(4);
  Actor_SetSpeedAndAcceleration(ACTOR_OPENING_MOVE_HAND, 2, 2);
  FieldActor_SetMoveTarget(moveHand, 408, 208, 584);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_OPENING_PILLAR, 0.15);
  Actor_SetSpeed(ACTOR_OPENING_MOVE_HAND, 0.15);
  // DEAD: the pillar is looked up and dropped; the ROM keeps the call.
  Actor_Get(ACTOR_OPENING_PILLAR);
  FieldActor_SetMoveTarget(moveHand, 392, 208, 584);
  Actor_MoveToAndWait(ACTOR_OPENING_PILLAR, PILLAR_MIDDLE_X, PILLAR_Z);
  Event_Wait(20);
  // "You sure do put your heart into it, Garet!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
  moveHand->update = NULL;
  Actor_SetSpritePriority(ACTOR_GARET, 2);
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Task_Remove(Vale_UpdateGaretPaletteAndEffects);
  Task_Remove(Vale_CycleOpeningPillarPalette);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_GARET, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_OPENING_PILLAR, SPRITE_COLOR_NORMAL);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Vale_VanishMoveHand(moveHand);
  Scene_EndEventPsynergy();

  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_JENNA, 424, 624);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_JENNA, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  // "Oh, I'm just finishing up."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  // "That's good. It's time to go, and I was about to leave you behind."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  // "Say, what exactly happened to you two?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(10);
  // "The accident three years ago... Is that why you've been studying
  // Psynergy?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  // "I appreciate your concern, but...my family's never coming back, and
  // neither is Isaac's father"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_JENNA, 388, 604);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  // "I just want to forget that day. I wish everyone would..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
  // Garet: "That awful day... We went to get help, but they found us
  // unconscious..." and at once Jenna: "Well, let's get Isaac and go to
  // Kraden's..."
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_GARET, 1, 2, 25, 2, ACTOR_JENNA, 10,
                                    14, 4, 14, 0);
  Event_Wait(40);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "What, Garet?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Huh? You were about to say something, weren't you?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  // "Don't be weird, Garet!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST_SOUTHEAST, 40);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_GARET, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_JENNA, 380, 620);
  Event_Wait(10);
  // "Whatever..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  // "Jenna, are you mad at me?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_WalkToAndWait(ACTOR_GARET, 412, 604);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(10);
  // "I don't want to talk about it."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_Jump(ACTOR_JENNA, 4, 30);
  // "Come on!!!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  // "Huh?"
  Event_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  // "Isaac's waiting for us."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkTo(ACTOR_JENNA, 428, 628);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WaitForMoveById(ACTOR_JENNA);
  // "But, Jenna...!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_GARET,
                    0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_WalkToAndWait(ACTOR_JENNA, 428, 628);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  // "I'll leave you behind if you're too slow."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "Jenna..."
  Event_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_JENNA, 0.5);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  Actor_WalkTo(ACTOR_JENNA, 450, 750);
  Actor_WalkTo(ACTOR_GARET, 450, 750);
  Event_Wait(60);
  gFieldState->transitionFrames = 60;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(VALE_TO_ROOF_MENDED);
  Event_End();
}

/// The end of the opening, on the roof of Isaac's house: Isaac lifts the last
/// boards into place with his Psynergy, and Dora climbs the ladder to see.
/// "Good job, Isaac. You patched it nicely." She remembers Kyle. Jenna and
/// Garet climb up too and tell her they are going up Mt. Aleph with Kraden,
/// and at last they all climb back down.
///
/// It plays once, as Vale is set up from VALE_FROM_ROOF_MENDED
/// (FLAG_VALE_ROOF_MENDED_EVENT_PLAYED), and sets FLAG_VALE_ROOF_JUST_MENDED
/// for the rest of the visit.
static void Vale_RoofMended(void) {
  struct Actor *leader;
  struct Actor *actor;
  u32 i;
  struct Actor *dora;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  Actor_SetPosition(ACTOR_DORA, 392, 896);
  dora = Actor_Get(ACTOR_DORA);
  // DEAD: Dora is turned east and at once north; the ROM keeps both stores.
  dora->facing = FACING_EAST;
  dora->facing = FACING_NORTH;
  Actor_SetPosition(ACTOR_GARET, 298, 736);
  Actor_Get(ACTOR_GARET)->facing = FACING_SOUTH;
  Actor_SetPosition(ACTOR_JENNA, 298, 760);
  Actor_Get(ACTOR_JENNA)->facing = FACING_SOUTH;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  actor = Actor_Get(ACTOR_FIRST_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = toFIXED(388);
  actor->y.fixed = toFIXED(160);
  actor->z.fixed = toFIXED(840);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_SECOND_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = toFIXED(388);
  actor->y.fixed = toFIXED(160);
  actor->z.fixed = toFIXED(844);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_THIRD_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = toFIXED(388);
  actor->y.fixed = toFIXED(160);
  actor->z.fixed = toFIXED(848);
  Actor_SetSpriteFlags(actor, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  FieldCamera_MoveTo(383, 160, 877, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldState->transitionFrames = 32;
  Event_StartScreenTransition();
  Actor_SetSpeedFX(ACTOR_JENNA, toFIXED(1) / 2, FACING_SOUTH);
  Actor_SetSpeedFX(ACTOR_GARET, toFIXED(1) / 2, FACING_SOUTH);
  Actor_SetScript(ACTOR_JENNA, gVale_JennaWalksToTheHouse);
  Actor_SetScript(ACTOR_GARET, gVale_GaretWalksToTheHouse);
  Event_Wait(40);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.3);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 400, 840);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Scene_StartEventPsynergy();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  Task_AddFrame(Vale_PlayPsynergySparkleSound, TASK_PASS_FRAME);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    wait(1);
  }
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);

  Task_AddFrame(Vale_UpdateLeaderPaletteAndEffects, TASK_PASS_FRAME);
  Task_AddFrame(Vale_CycleFirstBoardPalette, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, 0.2);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 400, 826);
  Event_Wait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Vale_PlayPsynergySparkleSound);
  Task_Remove(Vale_UpdateLeaderPaletteAndEffects);
  Task_Remove(Vale_CycleFirstBoardPalette);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_FIRST_BOARD, 0, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(120);
  MapGrid_CopyCellsTo(7, 102, 84, 41, 2, 1);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  Task_AddFrame(Vale_PlayPsynergySparkleSound, TASK_PASS_FRAME);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    wait(1);
  }
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Task_AddFrame(Vale_UpdateLeaderPaletteAndEffects, TASK_PASS_FRAME);
  Task_AddFrame(Vale_CycleSecondBoardPalette, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_SECOND_BOARD, 0.2);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 377, 828);
  Event_Wait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Vale_PlayPsynergySparkleSound);
  Task_Remove(Vale_UpdateLeaderPaletteAndEffects);
  Task_Remove(Vale_CycleSecondBoardPalette);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_SECOND_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_SECOND_BOARD, 0, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(120);
  MapGrid_CopyCellsTo(6, 102, 83, 41, 1, 1);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  Task_AddFrame(Vale_PlayPsynergySparkleSound, TASK_PASS_FRAME);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    wait(1);
  }
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Task_AddFrame(Vale_UpdateLeaderPaletteAndEffects, TASK_PASS_FRAME);
  Task_AddFrame(Vale_CycleThirdBoardPalette, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_THIRD_BOARD, 0.2);
  Actor_MoveToAndWait(ACTOR_THIRD_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_THIRD_BOARD, 360, 837);
  Event_Wait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Vale_PlayPsynergySparkleSound);
  Task_Remove(Vale_UpdateLeaderPaletteAndEffects);
  Task_Remove(Vale_CycleThirdBoardPalette);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_THIRD_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_THIRD_BOARD, 0, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(120);
  Scene_EndEventPsynergy();
  MapGrid_CopyCellsTo(5, 103, 82, 42, 1, 1);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  Actor_Jump(ACTOR_DORA, 2, 20);
  Event_SetMessageId(MsgValeGoodJob);
  // "Good job, Isaac."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST_SOUTHEAST, 10);
  Vale_ClimbUpRoofLadder(ACTOR_DORA, DORA_ANIMATION_CLIMB,
                         DORA_ANIMATION_CLIMB_OVER_TOP, 0);
  Actor_SetSpeed(ACTOR_DORA, 0.3);
  Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  // "You patched it nicely."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  // "All right. You're all done."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  // "You're very reliable, Isaac...just like your father was."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  // "What is it, Isaac? Do you still think about the accident?"
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    gFieldState->message++;
  } else {
    Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  }
  // Yes: "I thought so... Isaac..." No: "I don't believe you..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Event_SetMessageId(MsgValeWorkingYourselvesBone);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  // "You and Garet have been working yourselves to the bone..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
  // "Do you think things would be any different if you'd had..."
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldState->message++;
  }
  // Yes: "That's what I thought." No: "Hmmm... I wonder..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_SetMessageId(MsgValeDevastatedWhenKyle);
  // "I was devastated when Kyle died..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  // "I felt all alone..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 10);
  // "It's taken some time, but I'm better now."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  // "I still have you, Isaac."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  // "You're almost seventeen. You're so much like Kyle was when..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  // "I'm looking forward to seeing where life takes you."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(377, 160, 860, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_WalkTo(ACTOR_GARET, 369, 904);
  Actor_WalkToAndWait(ACTOR_JENNA, 392, 904);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Vale_ClimbUpRoofLadder(ACTOR_JENNA, ANIM_LEADER_CLIMB,
                         ANIM_LEADER_CLIMB_OVER_TOP, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  // "MA'AAAAM!!!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST_SOUTHEAST, 20);
  Actor_Jump(ACTOR_JENNA, 4, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 392, 843);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  // "Hello, Jenna."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  // "Good morning."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  // "Yes, yes... Good morning."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Vale_ClimbUpRoofLadder(ACTOR_GARET, ANIM_LEADER_CLIMB,
                         ANIM_LEADER_CLIMB_OVER_TOP, 0);
  Actor_SetSpeed(ACTOR_JENNA, 0.3);
  Actor_SetSpeed(ACTOR_GARET, 0.3);
  Actor_WalkTo(ACTOR_GARET, 392, 843);
  Actor_Get(ACTOR_JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JENNA, 408, 843);
  Event_Wait(1);
  Actor_Get(ACTOR_JENNA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_Jump(ACTOR_DORA, 4, 30);
  // "Well, if it isn't Garet, too!"
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 20);
  // "Where do you all plan to go today?"
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // "We're going to Mt. Aleph with Kraden."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 20);
  // "Mountain climbing with Kraden, eh?"
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(80);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "Kids and their games..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  // "No! It's part of our studies..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(40);
  // "Ah, yes... Alchemy."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  // "Alchemy is the foundation of all Psynergy..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  // "And you all know Mt. Aleph is the greatest source of Alchemy."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  // "They say Kraden is the best teacher around."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(373, 160, 837, 1);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 10);
  // "Alchemy... I wonder what use it all is?"
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 40);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 20);
  // "Your mother would rather see you grow up to be a happy, ordinary man."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  // "But I suppose you still want to go..."
  Event_OpenMessage(ACTOR_DORA, 0);

  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldState->message++;
  }
  // Yes: "Well, if that won't persuade you, I guess nothing will!" No: "It's
  // nice of you to humor me, but I know you don't mean it."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Event_SetMessageId(MsgValeAsStubbornAsYourFather);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  // "You're just as stubborn as your father!"
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_Jump(ACTOR_DORA, 4, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Actor_SetAnimation(ACTOR_DORA, DORA_ANIMATION_HANDS_UP);
  Event_Wait(5);
  // "Ahhh!"
  // "Be careful, ma'am!"
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, ACTOR_GARET, 10,
                                    14, 4, 14, 0);
  Sound_Play(SE_RATTLE);
  actor = Actor_Get(ACTOR_DORA);
  actor->sprite->flags = 0;
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_DORA, 3);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
  Event_Wait(4);
  for (i = 0; i != 4; i++) {
    actor->z.fixed += toFIXED(3) / 2;
    actor->scaleY -= FIXED_ONE / 10;
    Event_Wait(1);
  }
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetSpeed(ACTOR_GARET, 3);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 374, VALE_GARET_REUNION_Z);
#if TBS_PATCH_2
  Actor_SetPosition(ACTOR_GARET, 374, VALE_GARET_REUNION_Z);
#endif
  // "Garet, don't!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Actor_SetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
  Actor_Jump(ACTOR_GARET, 2, 5);
  Sound_Play(SE_CRACK);
  MapGrid_SetShake(0, 4, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_SetAnimation(ACTOR_DORA, DORA_ANIMATION_PEEK);
  actor->scaleY = FIXED_ONE / 2;
  Actor_SetPosition(ACTOR_DORA, 364, 811);
  for (i = 0; i != 5; i++) {
    actor->scaleY += FIXED_ONE / 10;
    Event_Wait(1);
  }
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  // "Oh! That was exciting!"
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  FieldCamera_SetSpeed(0.3);
  FieldCamera_MoveTo(372, 160, 859, 1);
  Actor_SetSpeed(ACTOR_DORA, 3);
  Actor_Jump(ACTOR_DORA, 6, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  // "I'm sorry I scared you like that."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  // "Well!"
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // "I... I'm sorry, ma'am. I'll get out of your way right away!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetSpeed(ACTOR_GARET, 4);
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_GARET, 403, VALE_GARET_REUNION_Z);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 20);
  // "Wait! Not there!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 1);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  // "Huh? What's wrong with—"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
  Actor_Jump(ACTOR_GARET, 2, 5);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Sound_Play(SE_CRACK);
  MapGrid_SetShake(0, 4, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(ACTOR_JENNA, 0.3);
  Actor_WalkToAndWait(ACTOR_JENNA, 408, 855);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  // "Oh, never mind..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  // "But you'd better get going..."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  // "This old house will fall apart if you stay much longer."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  // "I'll get this fixed up. You don't have to hang around here."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  // "Thank you, ma'am."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Please, call me Dora."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  // "We'll get going right away!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Event_Wait(30);
  // "Wait, Garet... Just watch your step."
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 398, 828);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(60);
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_JENNA);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(ACTOR_JENNA, actor->x.part[0].integer + 16,
               actor->z.part[0].integer);
  Actor_WalkToAndWait(ACTOR_GARET, actor->x.part[0].integer + 16,
                      actor->z.part[0].integer - 16);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_JENNA, actor->x.part[0].integer,
                      actor->z.part[0].integer);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WalkToAndWait(ACTOR_GARET, actor->x.part[0].integer,
                      actor->z.part[0].integer);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_AddPartyMemberPair(GARET, JENNA);
  FieldCamera_MoveTo(377, 160, 887, 1);
  Vale_ClimbDownRoofLadder(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_DOWN_FROM_TOP,
                           ANIM_LEADER_CLIMB, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_Get(ACTOR_DORA)->steeringFlags |= ACTOR_FACES_MOTION;
  Vale_ClimbDownRoofLadder(ACTOR_DORA, DORA_ANIMATION_CLIMB_OVER_TOP,
                           DORA_ANIMATION_CLIMB, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForMove();
  Event_Wait(100);
  MapGrid_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  GameFlag_Set(FLAG_VALE_ROOF_JUST_MENDED);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#if TBS_PATCH_1
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->y.fixed = toFIXED(160);
  leader->targetY = ACTOR_NO_TARGET;
  leader->velocityY = 0;
#else
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
#endif
  Event_End();
}

#include "shared/event_psynergy.c"

/// Garet using his Psynergy on his training stone in the opening: his colours
/// shift and sparks fly about him (Vale_CycleActorPaletteAndSpawnEffects).
static void Vale_UpdateGaretPaletteAndEffects(void) {
  Vale_CycleActorPaletteAndSpawnEffects(Actor_Get(ACTOR_GARET));
}

/// Isaac using his Psynergy on the roof's boards at the end of the opening: his
/// colours shift and sparks fly about him.
static void Vale_UpdateLeaderPaletteAndEffects(void) {
  Vale_CycleActorPaletteAndSpawnEffects(Actor_Get(ACTOR_PARTY_LEADER));
}

/// Garet's training stone shifting colours while his Psynergy moves it in the
/// opening.
static void Vale_CycleOpeningPillarPalette(void) {
  Vale_CycleActorPalette(Actor_Get(ACTOR_OPENING_PILLAR));
}

/// The first board shifting colours while Isaac's Psynergy lifts it onto the
/// roof.
static void Vale_CycleFirstBoardPalette(void) {
  Vale_CycleActorPalette(Actor_Get(ACTOR_FIRST_BOARD));
}

/// The second board shifting colours while Isaac's Psynergy lifts it.
static void Vale_CycleSecondBoardPalette(void) {
  Vale_CycleActorPalette(Actor_Get(ACTOR_SECOND_BOARD));
}

/// The third board shifting colours while Isaac's Psynergy lifts it.
static void Vale_CycleThirdBoardPalette(void) {
  Vale_CycleActorPalette(Actor_Get(ACTOR_THIRD_BOARD));
}

/// The distance in whole pixels between two positions of 16.16 fixed-point x,
/// y and z.
// The same routine as Position_Distance, which this scene also holds through
// shared/pushed_actor.c, under another name.
static s32 Vale_DistanceInPixels(FIXED *first, FIXED *second) {
  s32 dx = FIXEDtoINT(*first++ - *second++);
  s32 dy = FIXEDtoINT(*first++ - *second++);
  s32 dz = FIXEDtoINT(*first - *second);

  return Iwram_Sqrt(dx * dx + dy * dy + dz * dz);
}

/// Whether the man guarding Mt. Aleph sees the target.
///
/// He sees it within range pixels and in the sixteenth of a turn he faces or
/// the one either side of it, or always when force is set.
///
/// Returns 1 when he is stopped.
static s32 Vale_TestFacing(struct Actor *actor, struct Actor *target, s32 range,
                           s32 force) {
  s32 result;
  u32 angle;
  u32 left;
  u32 right;
  u32 dir;

  result = 0;
  if (actor->paused == 1) {
    if (actor->sceneCounter == 0) {
      Actor_SelectAnimation(actor, ANIM_STAND);
      return 1;
    }
  }
  if (Vale_DistanceInPixels(&target->x.fixed, &actor->x.fixed) < range ||
      force != 0) {
    angle = (u16)Trig_Atan(target->z.fixed - actor->z.fixed,
                           target->x.fixed - actor->x.fixed);
    left = FACING_SIXTEENTH(angle - FACING_STEP);
    right = FACING_SIXTEENTH(angle + FACING_STEP);
    angle = FACING_SIXTEENTH(angle);
    dir = FACING_SIXTEENTH(actor->facing);
    if (angle == dir || right == dir || left == dir || force != 0) {
      actor->paused = 1;
      Actor_SelectAnimation(actor, ANIM_STAND);
      result = 1;
      actor->sceneCounter = result;
    } else {
      actor->paused = 0;
      Actor_SelectAnimation(actor, ANIM_WALK);
      actor->sceneCounter = 0;
    }
  } else {
    actor->paused = 0;
    Actor_SelectAnimation(actor, ANIM_WALK);
    actor->sceneCounter = 0;
  }
  return result;
}

/// The update routine of the man guarding Mt. Aleph. While he walks, he stops
/// when the leader comes within 18 pixels in front of him (Vale_TestFacing).
static UNREAD_RESULT Vale_HaveGuardWatchTheLeader(struct Actor *guard) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (guard->targetX == ACTOR_NO_TARGET && guard->targetZ == ACTOR_NO_TARGET)
    RETURN_UNREAD(0);
  Vale_TestFacing(guard, leader, 18, 0);
  RETURN_UNREAD(0);
}

/// The Move hand's update routine in the opening. Each frame it sheds a sparkle
/// (SPRITE_SPARKLE) just below and around it, with SE_ITEM_BREAK.
static void Vale_TrailSparkles(struct Actor *source) {
  s32 spawnPosition[3];
  s32 randomDistance;
  struct Actor *particle;
  SET_VECTOR(spawnPosition, source->x.fixed,
             source->y.fixed - (Random_Next() << 4) - toFIXED(8),
             source->z.fixed);
  randomDistance = Random_Next();
  Vector_AddPolarOffset(randomDistance * 48, Random_Next(), spawnPosition);
  particle = FieldActor_Create(SPRITE_SPARKLE, spawnPosition[0],
                               spawnPosition[1], spawnPosition[2]);
  if (particle != NULL) {
    particle->motionFlags = ACTOR_FALLS;
    particle->gravity = toFIXED(1) / 10;
    particle->wait = 12;
    Actor_SetSpriteFlags(particle, 0);
    Actor_SelectAnimation(particle, 0);
    Actor_StartScript(particle, gVale_SparkleScript);
    particle->sprite->image.priority = 1;
  }
  Sound_Play(SE_ITEM_BREAK);
}

/// The Move hand rises, spins and shrinks for 31 frames, bursts into eight
/// sparkles with SE_PSYNERGY_SPARKLE, and is put out of sight.
static void Vale_VanishMoveHand(struct Actor *emitter) {
  s32 frame;

  Sound_Play(SE_CAST);

  for (frame = 0; frame < 31; frame++) {
    emitter->y.fixed += toFIXED(1);
    emitter->facing = (u16)(emitter->facing + DEGtoANG(45));
    emitter->scaleX -= FIXED_ONE / 32;
    emitter->scaleY -= FIXED_ONE / 32;
    wait(1);
  }

  for (frame = 0; frame < 8; frame++) {
    struct Actor *particle = FieldActor_Create(
        SPRITE_SPARKLE, emitter->x.fixed, emitter->y.fixed, emitter->z.fixed);

    if (particle != NULL) {
      FIXED verticalSpeed;

      Actor_SetSpriteFlags(particle, 0);
      Actor_StartScript(particle, (const void *)&gVale_SparkleScript[1]);

      verticalSpeed = Random_Next() + toFIXED(1);
      particle->acceleration = FIXED_ONE;
      particle->speedLimit = verticalSpeed;
      particle->motionFlags = ACTOR_FALLS;
      particle->gravity = toFIXED(1) / 25;

      particle->velocityY = Random_Next() - Random_Next();

      Actor_SetTargetPositionFromMagnitudeAngle(
          particle, ((Random_Next() * 3) << 3) + toFIXED(8), Random_Next());
    }
  }

  Sound_Play(SE_PSYNERGY_SPARKLE);

  emitter->x.fixed = 0;
  emitter->y.fixed = 0;
  emitter->z.fixed = 0;
  emitter->targetX = ACTOR_NO_TARGET;
  emitter->targetY = ACTOR_NO_TARGET;
  emitter->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(emitter, 0, 0, 0);
}

DEFINE_SET_TARGET_POSITION_FROM_MAGNITUDE_ANGLE(static,
                                                FieldActor_SetMoveTargetFX)

/// Walking into Garet's training stone, below the cliff north of his house,
/// pushes it a cell: east from its west side, cells (22, 36) to (23, 36), or
/// west from its east side, cells (24, 36) to (25, 36). It moves only between
/// cells (22, 36) and (24, 36).
///
/// The push itself is the shared one (PushedActor_AdvancePair), and
/// Vale_MovePillarBelowCliff then settles the cells under it.
static void Vale_PushPillar(void) {
#if !TBS_PATCH_1
  // DEAD: the Japanese edition looks the pillar up and drops it; the ROM keeps
  // the calls.
  if (GameFlag_Get(FLAG_KALAY_REACHED))
    Actor_Get(ACTOR_PILLAR_AFTER_KALAY);
  else
    Actor_Get(ACTOR_PILLAR);
#endif
  MapGrid_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  PushedActor_AdvancePair();
  Vale_MovePillarBelowCliff();
}

/// Move on Garet's training stone, below the cliff north of his house, pushes
/// it a cell east or west with the hand of Move, between cells (22, 36) and
/// (24, 36).
///
/// The Psynergy moves the stone; this event then settles the row under it, as
/// it does after a push and when Vale is set up. The cells either side of the
/// stone push it, and its own cell is one Isaac jumps across
/// (CELL_BEHAVIOR_JUMP_FORWARD), as the cell west of the row is. The row's
/// cells are copied from cells (17, 0) to (19, 0) of the map and the stone's
/// own from cell (17, 1), and its column is kept in
/// FLAG_VALE_PILLAR_IN_WEST_COLUMN, _IN_MIDDLE_COLUMN or _IN_EAST_COLUMN.
static void Vale_MovePillarBelowCliff(void) {
  struct Actor *actor;
  s32 column;

  MapGrid_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  if (GameFlag_Get(FLAG_KALAY_REACHED))
    actor = Actor_Get(ACTOR_PILLAR_AFTER_KALAY);
  else
    actor = Actor_Get(ACTOR_PILLAR);
  if (actor == NULL)
    return;
  Event_ClearFlag(FLAG_VALE_PILLAR_IN_WEST_COLUMN);
  Event_ClearFlag(FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN);
  Event_ClearFlag(FLAG_VALE_PILLAR_IN_EAST_COLUMN);
  column = FIELD_CELL(actor->x.fixed);
  if (column == 22) {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, column, 36);
    GameFlag_Set(FLAG_VALE_PILLAR_IN_WEST_COLUMN);
  } else if (column == 23) {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, column, 36);
    GameFlag_Set(FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN);
  } else {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, 24, 36);
    GameFlag_Set(FLAG_VALE_PILLAR_IN_EAST_COLUMN);
  }
}

/// The task that plays SE_PSYNERGY_SPARKLE every 16 frames while Isaac's
/// Psynergy lifts a board at the roof's end.
static void Vale_PlayPsynergySparkleSound(void) {
  if ((gFrameCount & 15) == 0)
    Sound_Play(SE_PSYNERGY_SPARKLE);
}

/// Searching the door of the house the storm wrecked, on the west bank in the
/// south of Vale, at cell (6, 74), finds it stuck: "The door won't open. The
/// force of the impact must have knocked it off its hinges."
static void Vale_SearchTheLockedDoor(void) {
  Event_Start();
  FieldMessage_Show(MsgValeDoorWontOpen, 1);
  Event_End();
}

/// Keeps the man guarding Mt. Aleph standing still while a menu is open, and
/// lets him walk on when it closes.
///
/// It runs every frame from Vale's setup, before the party leaves, and
/// FLAG_VALE_MT_ALEPH_GUARD_PAUSED remembers that it stopped him.
static void Vale_SynchronizeAlephGuardPause(void) {
  s32 pauseFlag;

  if (Event_GetFlag(FLAG_VALE_MT_ALEPH_GUARD_PAUSED) != 0) {
    pauseFlag = GameFlag_Get(FLAG_MENU_OPEN);
    if (pauseFlag == 0) {
      Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = pauseFlag;
      GameFlag_Clear(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
    }
  } else {
    if (GameFlag_Get(FLAG_MENU_OPEN) != 0) {
      Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = 1;
      GameFlag_Set(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
    }
  }
}

/// Draws Garet's training stone under Isaac while he is up on the cliff above
/// it, and in front of him or behind as usual while he is down beside it.
///
/// It runs every frame. Isaac is above the stone when his height is over 200;
/// the stone's foot is at height 192.
static void Vale_SetPillarPriorityByLeaderHeight(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *pillar;
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    pillar = Actor_Get(ACTOR_PILLAR_AFTER_KALAY);
  else
    pillar = Actor_Get(ACTOR_PILLAR);
  if (pillar != NULL) {
    if (leader->y.fixed > toFIXED(200))
      pillar->priorityFlags =
          ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
    else
      pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
}

#include "shared/floating_nut.c"

/// Someone calling up Psynergy. They are drawn SPRITE_COLOR_WHITE for two
/// frames in four, with the World Map's attached sparks every 16 frames.
static void Vale_FlashActorAndSpawnEffects(struct Actor *actor) {
  if ((gFrameCount & 2) != 0)
    Actor_SetColorMode(actor, SPRITE_COLOR_WHITE);
  else
    Actor_SetColorMode(actor, SPRITE_COLOR_NORMAL);
  if ((gFrameCount & 0xf) == 0)
    Scene_SpawnGoldenRing(actor);
}

/// Someone using Psynergy.
static void Vale_CycleActorPaletteAndSpawnEffects(struct Actor *actor) {
  if ((gFrameCount & 1) != 0)
    Actor_SetColorMode(actor, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 0xf) == 0)
    Scene_SpawnGoldenRing(actor);
}

/// What Psynergy moves. It goes through colour modes SPRITE_COLOR_NORMAL to
/// SPRITE_COLOR_BRG in turn, a step every second frame.
static void Vale_CycleActorPalette(struct Actor *actor) {
  if ((gFrameCount & 1) != 0)
    Actor_SetColorMode(actor, (gFrameCount >> 1) % 6);
}

#include "shared/field_effect_vertical.c"
#include "shared/golden_ring.c"

#include "shared/pushed_actor_tables.c"

// clang-format off

/// The beat the man guarding Mt. Aleph walks once he has warned Isaac, looking
/// about at each stop: east along the front of the Sanctum and up behind it,
/// back before its door, then west and up the path along its west side, and
/// back. SW_TIMER says which stretch of it he is on (enum ValeGuardBeat).
s32 gVale_AlephGuardPatrolRoute[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.25),
    SC_SET(SW_TIMER, GUARD_BEFORE_THE_SANCTUM),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(296, 0, 456),
    SC_SYNC,
    SC_SET(SW_TIMER, GUARD_BEHIND_THE_SANCTUM),
    SC_MOVE(296, 0, 420),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(296, 0, 456),
    SC_SYNC,
    SC_SET(SW_TIMER, GUARD_BEFORE_THE_SANCTUM),
    SC_MOVE(256, 0, 456),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(15),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(60),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(212, 0, 456),
    SC_SYNC,
    SC_SET(SW_TIMER, GUARD_ON_THE_PATH),
    SC_MOVE(212, 0, 388),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(40),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(212, 0, 429),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(212, 0, 456),
    SC_SYNC,
    SC_SET(SW_TIMER, GUARD_BEFORE_THE_SANCTUM),
    SC_MOVE(256, 0, 456),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(40),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/// Isaac on the roof, his sprite stretching and settling with each blow of
/// the hammer.
s32 gVale_LeaderHammers[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 100),
    SC_WAIT(12),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_WAIT(12),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// Garet and Jenna walk to the front of Isaac's house to watch him on the roof.

s32 gVale_GaretWalksToTheHouse[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(300, 0, 880),
    SC_SYNC,
    SC_MOVE(300, 0, 904),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

s32 gVale_JennaWalksToTheHouse[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(300, 0, 904),
    SC_SYNC,
    SC_MOVE(321, 0, 904),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

/// A spout of water grows from a tenth of its size north of the strongman by
/// the river and vanishes (Vale_UnusedWaterBurstsByTheRiver).
s32 gVale_UnusedWaterGrows[] = {
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(266, 0, 1230),
    SC_SET(SW_SCALE_X, FIXED_ONE / 10),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 10),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 3 * FIXED_ONE / 50),
    SC_ADD(SW_SCALE_Y, 2 * FIXED_ONE / 25),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// A sparkle falls and vanishes after 10 frames. The burst's sparkles start at
/// its second word, past the first SC_RELEASE.
s32 gVale_SparkleScript[] = {
    SC_RELEASE,
    SC_SYNC,
    SC_SPEED(0.0625),
    SC_ANIMATION(ANIM_STILL),
    SC_WAIT(10),
    SC_RELEASE,
};

struct SceneEntrance gVale_Entrances[] = {
    { 0, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_SANCTUM, CONDITION_ALWAYS, 256, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, 38, 0, 636, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_KRADENS_COTTAGE_SOUTH, CONDITION_ALWAYS, 29, 0, 792, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_PLAZA, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, 406, 0, 743, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, 262, 0, 821, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_B, CONDITION_ALWAYS, 340, 0, 904, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, 326, 0, 1142, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, 374, 0, 1254, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_NORTH_WEST_BANK_DOOR, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_TITLE, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_ROOF_MENDED, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gVale_Exits[] = {
    SCENE_EXITS(Vale),
    SCENE_EXIT(VALE_TO_SOL_ENTRANCE, SolEntrance, SOL_ENTRANCE_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_KRADENS_COTTAGE_NORTH, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_VALE_KRADENS_COTTAGE_SOUTH, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_EAST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_EAST),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_SOUTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_B, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_WEST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_WEST),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_NORTH_WEST_BANK_DOOR, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_SANCTUM, Sanctum, SANCTUM_FROM_VALE),
    SCENE_EXIT(VALE_TO_ROOF_MENDED, Vale, VALE_FROM_ROOF_MENDED),
    SCENE_EXITS_END,
};

struct ActorPlacement gVale_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 908, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
        92, 0, 508, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        252, 0, 620, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        164, 0, 1236, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        268, 0, 1276, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        276, 0, 1044, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        19, 0, 614, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
        50, 0, 795, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
        65, 0, 800, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
        57, 0, 805, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        412, 0, 586, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        398, 0, 616, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PILLAR_MIDDLE_X, 0, PILLAR_Z, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        373, 0, VALE_DORA_Z, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        152, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVale_ActorPlacementsAfterLeaving[] = {
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 908, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
        92, 0, 508, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        252, 0, 620, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        164, 0, 1236, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        268, 0, 1276, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        276, 0, 1044, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        19, 0, 614, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
        50, 0, 795, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
        65, 0, 800, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
        57, 0, 805, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        412, 0, 586, FACING_WEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        398, 0, 616, FACING_NORTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PILLAR_MIDDLE_X, 0, PILLAR_Z, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        335, 0, 869, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        152, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVale_ActorPlacementsAfterKalay[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        412, 0, 586, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        356, 0, 727, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        164, 0, 1236, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        268, 0, 1276, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 908, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
        92, 0, 508, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        252, 0, 620, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        276, 0, 1044, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_STAND,
        152, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PILLAR_MIDDLE_X, 0, PILLAR_Z, FACING_EAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVale_RepairActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        409, 0, 1134, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        428, 0, 588, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        204, 0, 924, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PILLAR_EAST_X, 0, PILLAR_Z, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        373, 0, VALE_DORA_Z, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gVale_Triggers[] = {
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephLooksBeautifulTodayIts) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeValeStaysPristineBecauseOfMt) },
    { TRIGGER_TALK, ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, EVENT(Vale_TalkAboutTheMayorAndTremors) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeThisPlaceLooksJustLikeIt) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacWatchOutDontGetToo) },
    { TRIGGER_TALK, ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeValeMustRemainHiddenPsynergysSecrets) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_WarnAboutGoingBeyondKradensCottage) },
    { TRIGGER_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, EVENT(Vale_ArgueWithGaretsSister) },
    { TRIGGER_TALK, ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeYoureInTroubleNowGaretYour) },
    { TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS, EVENT(Vale_DoraTalksAboutTheHole) },
    { TRIGGER_TALK, ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephIsASacredPeak) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_SANCTUM_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterSanctum) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH, CONDITION_ALWAYS, EVENT(Vale_ApproachMtAleph) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageSouth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_WAY_TO_THE_PLAZA, CONDITION_ALWAYS, EVENT(Vale_ExitToValePlaza) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_GARETS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterGaretsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsA) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_ISAACS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterIsaacsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_GRANDPARENTS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterJennasGrandparentsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsAWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_UnusedEnterJennasGrandparentsHouseByWestBankDoor) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_ONTO_BRIDGE, CONDITION_ALWAYS, EVENT(Vale_StepOntoBridge) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_OFF_BRIDGE, CONDITION_ALWAYS, EVENT(Vale_StepOffBridge) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_NORTH_OF_PUPPIES, CONDITION_ALWAYS, EVENT(Vale_WatchThePuppies) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_SOUTH_OF_PUPPIES, CONDITION_ALWAYS, EVENT(Vale_TurnBackFromThePuppies) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_GUARD_JUMPS_OUT, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT(Vale_WarnAgainstSneakingUpMtAleph) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_PATH_UP_MT_ALEPH, CONDITION_ALWAYS, EVENT(Vale_CatchSneakingOnThePath) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_BESIDE_THE_SANCTUM, CONDITION_ALWAYS, EVENT(Vale_CatchSneakingBesideTheSanctum) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_FOOT_OF_THE_PATH, CONDITION_ALWAYS, EVENT(Vale_CatchSneakingAtThePathsFoot) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_LADDER_FOOT, CONDITION_ALWAYS, EVENT(Vale_ClimbLadderOntoRoof) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_VALE_LADDER_TOP, CONDITION_ALWAYS, EVENT(Vale_ClimbDownFromRoof) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, EVENT(Vale_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(Vale_MovePillarBelowCliff) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_VALE_WEST_OF_TRAINING_STONE, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_VALE_EAST_OF_TRAINING_STONE, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_SearchTheLockedDoor) },
    { TRIGGER_TABLE_END },
};

struct Trigger gVale_TriggersAfterLeaving[] = {
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeIsSavedThanksToThe) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeEverSinceTheTremorsStartedMt) },
    { TRIGGER_TALK, ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, EVENT(Vale_TalkAboutTheMayorAndTremors) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeSighNoLongerWillISee) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIDontBlameFelixIfOnly) },
    { TRIGGER_TALK, ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeTheMayorWasTooKindTo) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_WarnAboutGoingBeyondKradensCottage) },
    { TRIGGER_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreSoLuckyGaret) },
    { TRIGGER_TALK, ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseTakeCareOfMyBrother) },
    { TRIGGER_TALK, ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeGeeIWishICouldSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeMyBrothersStrengthIsHisBest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeImGladThatFelixIsAlive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeFelixWasAGoodPersonI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, MSGNO(MsgValePoorKradenWasTakenToSome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeWasSavedBecausePeopleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeICantBelieveAnyoneWouldClimb) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, MSGNO(MsgValeIJustDontFeelSafeKnowing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeThePeopleOfValeWillNever) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_SANCTUM_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterSanctum) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH, CONDITION_ALWAYS, EVENT(Vale_ApproachMtAleph) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageSouth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_WAY_TO_THE_PLAZA, CONDITION_ALWAYS, EVENT(Vale_ExitToValePlaza) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_GARETS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterGaretsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsA) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_ISAACS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterIsaacsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_GRANDPARENTS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterJennasGrandparentsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsAWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_UnusedEnterJennasGrandparentsHouseByWestBankDoor) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_ONTO_BRIDGE, CONDITION_ALWAYS, EVENT(Vale_StepOntoBridge) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_OFF_BRIDGE, CONDITION_ALWAYS, EVENT(Vale_StepOffBridge) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_NORTH_OF_PUPPIES, CONDITION_ALWAYS, EVENT(Vale_WatchThePuppies) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_SOUTH_OF_PUPPIES, CONDITION_ALWAYS, EVENT(Vale_TurnBackFromThePuppies) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_GUARD_JUMPS_OUT, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT(Vale_WarnAgainstSneakingUpMtAleph) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_PATH_UP_MT_ALEPH, CONDITION_ALWAYS, EVENT(Vale_CatchSneakingOnThePath) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_BESIDE_THE_SANCTUM, CONDITION_ALWAYS, EVENT(Vale_CatchSneakingBesideTheSanctum) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_FOOT_OF_THE_PATH, CONDITION_ALWAYS, EVENT(Vale_CatchSneakingAtThePathsFoot) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_LADDER_FOOT, CONDITION_ALWAYS, EVENT(Vale_ClimbLadderOntoRoof) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_VALE_LADDER_TOP, CONDITION_ALWAYS, EVENT(Vale_ClimbDownFromRoof) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, EVENT(Vale_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(Vale_MovePillarBelowCliff) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_VALE_WEST_OF_TRAINING_STONE, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_VALE_EAST_OF_TRAINING_STONE, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_SearchTheLockedDoor) },
    { TRIGGER_TABLE_END },
};

struct Trigger gVale_TriggersAfterKalay[] = {
    { TRIGGER_TALK, ACTOR_GARETS_SISTER_AFTER_KALAY, FLAG_VALE_GARET_MET_SISTER, EVENT(Vale_GaretMeetsHisSister) },
    { TRIGGER_TALK, ACTOR_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICanTellJustByLooking) },
    { TRIGGER_TALK, ACTOR_GARETS_BROTHER_AFTER_KALAY, FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY, EVENT(Vale_GaretAsksToProtectFamily) },
    { TRIGGER_TALK, ACTOR_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeIWillDoMyBestTo) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT(Vale_AskWhetherJennaHasReturned) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValePromiseMeIsaacPromiseMeYoull) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeIUsedToPlayHere) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephsEruptionWasSoPowerful) },
    { TRIGGER_TALK, ACTOR_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, EVENT(Vale_FeelAnotherTremor) },
    { TRIGGER_TALK, ACTOR_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheDangerIsGone) },
    { TRIGGER_TALK, ACTOR_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, EVENT(Vale_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_GARETS_SISTER_AFTER_KALAY, FLAG_VALE_GARET_MET_SISTER, EVENT(Vale_GaretMeetsHisSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreWorkingSoHard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_GARETS_BROTHER_AFTER_KALAY, FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY, EVENT(Vale_GaretAsksToProtectFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICouldGoWithYouIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT(Vale_AskWhetherJennaHasReturned) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValeYouDidntBringJennaWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeFelixMustBeProtectingJennaIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeTheEarthquakesArentCausedByEruptions) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, MSGNO(MsgValeAhhICantStandTheseEarthquakes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheSmokeFromMtAlephIsnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_MAN_MISSING_TRAVELERS, FLAG_VALE_ASKED_ABOUT_TRAVELERS, EVENT(Vale_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgValeHavingNoTravelersIsNoFun) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_SANCTUM_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterSanctum) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH, CONDITION_ALWAYS, EVENT(Vale_ApproachMtAleph) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageSouth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_WAY_TO_THE_PLAZA, CONDITION_ALWAYS, EVENT(Vale_ExitToValePlaza) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_GARETS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterGaretsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsA) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_ISAACS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterIsaacsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_GRANDPARENTS_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterJennasGrandparentsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsAWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_UnusedEnterJennasGrandparentsHouseByWestBankDoor) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_ONTO_BRIDGE, CONDITION_ALWAYS, EVENT(Vale_StepOntoBridge) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_OFF_BRIDGE, CONDITION_ALWAYS, EVENT(Vale_StepOffBridge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VALE_LADDER_FOOT, CONDITION_ALWAYS, EVENT(Vale_ClimbLadderOntoRoof) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_VALE_LADDER_TOP, CONDITION_ALWAYS, EVENT(Vale_ClimbDownFromRoof) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT_AFTER_KALAY, FLAG_VALE_NUT_CAUGHT, EVENT(Vale_CatchNutAfterKalay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_AFTER_KALAY, CONDITION_ALWAYS, EVENT(Vale_MovePillarBelowCliff) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_VALE_WEST_OF_TRAINING_STONE, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_VALE_EAST_OF_TRAINING_STONE, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vale_SearchTheLockedDoor) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// The doors' cell animations, from the layout's cells below the village, each
/// picture shown for two frames; vale_prologue.c has the same four kinds.
/// Each table takes the name of a door it opens. The two of Jenna's
/// grandparents' kind and the west house's show their one open picture twice,
/// where the stormy night's show two.
u16 gVale_SanctumDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};
u16 gVale_IsaacsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};
u16 gVale_GrandparentsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};
u16 gVale_WestHouseDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

// The man guarding Mt. Aleph drives Isaac back down the path to the Sanctum's
// door: at the first warning (Vale_WarnAgainstSneakingUpMtAleph), and each
// time he catches him on it again (Vale_AskAboutSneakingToAleph).

s32 gVale_FirstWarningLeaderPath[] = {
    SC_ANIMATION(ANIM_LEADER_RUN),
    SC_MOVE(199, 0, 446),
    SC_SYNC,
    SC_MOVE(215, 0, 472),
    SC_SYNC,
    SC_MOVE(239, 0, 461),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gVale_FirstWarningGuardPath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(199, 0, 446),
    SC_SYNC,
    SC_MOVE(215, 0, 472),
    SC_SYNC,
    SC_MOVE(239, 0, 461),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gVale_LeaderRunsBackFromThePath[] = {
    SC_SPEED(2),
    SC_ACCELERATION(2),
    SC_ANIMATION(ANIM_LEADER_RUN),
    SC_MOVE(224, 0, 458),
    SC_SYNC,
    SC_MOVE(256, 0, 458),
    SC_SYNC,
    SC_MOVE(256, 0, 474),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

s32 gVale_GuardComesDownThePath[] = {
    SC_SPEED(1.5),
    SC_ACCELERATION(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(216, 0, 458),
    SC_SYNC,
    SC_MOVE(256, 0, 456),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

s32 gVale_LeaderRunsBackBeforeTheDoor[] = {
    SC_SPEED(2),
    SC_ACCELERATION(2),
    SC_ANIMATION(ANIM_LEADER_RUN),
    SC_MOVE(256, 0, 458),
    SC_SYNC,
    SC_MOVE(256, 0, 474),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

s32 gVale_GuardStepsBeforeTheDoor[] = {
    SC_SPEED(1.5),
    SC_ACCELERATION(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(256, 0, 456),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};
