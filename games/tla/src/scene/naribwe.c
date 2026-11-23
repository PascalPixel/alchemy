/// @file
/// Naribwe, the desert village south of the Kibombo Mountains.
///
/// A fire burns in a ring of stones in the middle of the village. The inn is
/// the southeast hut, the weapon and armor shop the southwest one, and the
/// fortune-teller's hut stands at the top of the village between two burning
/// braziers, where for 20 coins he reads an item. Through the northwest houses
/// is a back yard, where Whirlwind blows away the weeds over a ladder up to a
/// ledge of totem poles, and Reveal there shows the Unicorn Ring's chest under
/// a rock pile. Once Piers has carried Gabomba's message, the villagers talk of
/// the smoke over the Kibombo Mountains.

#include "types.h"
#include "sprite_ids.h"
#include "field/field_work.h"
#include "flags.h"
#include "text/msg_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/sprite.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// The villager has asked where the party comes from.
#define FLAG_ASKED_WHERE_FROM 0x85e

/// The villager has asked about the Kibombo expedition.
#define FLAG_ASKED_ABOUT_EXPEDITION 0x85d

/// Whirlwind has scattered the weeds off the back yard's ladder, for this
/// visit.
#define FLAG_WEEDS_SCATTERED FLAG_SCENE_TEMP_01

/// The rope between the two posts, tied by Lash, for this visit.
#define FLAG_NARIBWE_ROPE_TIED FLAG_SCENE_TEMP_02

/// Naribwe, by the rows of the village's placements (the same before and
/// after the return from Kibombo); the last row goes to Isaac's party slot.
enum NaribweActorId {
  /// The fire in the ring of stones in the middle of the village.
  ACTOR_BONFIRE = ACTOR_FIRST_PLACED,
  /// The burning braziers either side of the fortune teller's hut.
  ACTOR_WEST_BRAZIER,
  ACTOR_EAST_BRAZIER,
  /// Unseen actors held 24 pixels over the braziers: the exclamation bubble's
  /// sheet, whose first animation draws nothing.
  ACTOR_OVER_WEST_BRAZIER,
  ACTOR_OVER_EAST_BRAZIER,
  /// "Don't tell me you just came back from Kibombo!"
  ACTOR_MAN_AMAZED_BY_THE_RETURN,
  /// "The Kibombo went on an expedition to the east, but they've already
  /// returned."
  ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION,
  /// "I would not venture far north of here if I were you..."
  ACTOR_MAN_WARNING_OF_THE_NORTH,
  /// "It's been dangerous out ever since the day the gems fell from the sky."
  ACTOR_WOMAN_WHO_SAW_THE_GEMS_FALL,
  /// "When Oeia was their witch doctor, the Kibombo were much more peaceful."
  ACTOR_OLD_MAN_ON_OEIA,
  /// "Can you tell me where you have come from?"
  ACTOR_WOMAN_ASKING_WHERE_FROM,
  /// "You don't plan on going to Kibombo, do you, mister?"
  ACTOR_BOY_ASKING_OF_KIBOMBO,
  /// "On the far side of the river lies a huge mountain we call Magma Rock."
  ACTOR_VILLAGER_ON_MAGMA_ROCK,
  /// "Ouch... Some big brute came through here looking for a woman named
  /// Menardi."
  ACTOR_OLD_MAN_HURT_BY_A_BRUTE,
  /// The item stall's keeper: "No one can see the future. Absolutely no one.
  /// Except for our fortune-teller."
  ACTOR_ITEM_SHOPKEEPER,
  /// The weeds over the back yard's ladder, which Whirlwind blows away.
  ACTOR_WEEDS,
  /// The post Lash throws the rope from, and the rope's unseen end up on the
  /// ledge.
  ACTOR_ROPE_POST,
  ACTOR_ROPE_END,
  /// "I am but a humble pilgrim..."
  ACTOR_PILGRIM,
  /// The rock pile over the Unicorn Ring's chest, which Reveal clears.
  ACTOR_ROCK_PILE
};

/// The Unicorn Ring's chest: the village's second scene item.
#define ACTOR_UNICORN_RING_CHEST (FIELD_TREASURE_ACTOR + 1)

/// An entrance no exit leads to: the village with no villagers about, where
/// Blaze on the fire throws its long jet of flame.
#define NARIBWE_FIRE_LIT_ENTRANCE 10
#include "shared/tables.h"

extern u32 gSceneExits[];

/// A door of Naribwe: how it opens, drawn over its cells, and where.
struct WarpEntry {
  const void *script;
  u16 x;
  u16 z;
};

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_End(void);
void Actor_DisableById(s32 id);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Shop_Run(s32 a, s32 b);
void Sanctum_Run(s32 a);

extern struct ActorPlacement gNaribwe_Placements[];
extern struct ActorPlacement gNaribwe_FireLitPlacements[];
extern struct ActorPlacement gNaribwe_PlacementsOnReturn[];
extern struct Trigger gNaribwe_Triggers[];
extern struct Trigger gNaribwe_TriggersOnReturn[];
extern struct WarpEntry gNaribwe_Doors[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.entrance == NARIBWE_FIRE_LIT_ENTRANCE)
    return gNaribwe_FireLitPlacements;
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gNaribwe_PlacementsOnReturn;
  return gNaribwe_Placements;
}

/// Felix casts Blaze at the fire in the ring of stones in the middle of the
/// village, and its jet of flame shoots up short, as anywhere else.
///
/// Before the cast, `blazeLongSpray` is set only when the party came in by
/// `NARIBWE_FIRE_LIT_ENTRANCE`, which no exit leads to; there the jet shoots
/// up long.
void Naribwe_PrepareBlaze(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s8 *flag;

  flag = &work->blazeLongSpray;
  *flag = 1;
  if (gMainRoutineWork.entrance != NARIBWE_FIRE_LIT_ENTRANCE)
    *flag = 0;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gNaribwe_TriggersOnReturn;
  return gNaribwe_Triggers;
}

/// The woman in a purple skirt just southwest of the fire tells Felix: "The
/// Kibombo went on an expedition to the east, but they've already returned,"
/// and asks him something. Once he has answered Yes, she says instead that the
/// Kibombo never move in such numbers without orders.
void Naribwe_AskAboutKibomboExpedition(void) {
  s32 msg;

  if (Event_GetFlag(FLAG_ASKED_ABOUT_EXPEDITION) == 0) {
    msg = (s32)MsgNaribweKibomboWentOnExpedition;
    Event_SetMessageId(msg);
    // "The Kibombo went on an expedition to the east, but they've already..."
    Event_OpenMessage(ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      Event_SetFlag(FLAG_ASKED_ABOUT_EXPEDITION);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, 0);
  } else {
    Event_SetMessageId((s32)MsgNaribweKibomboNeverMoveInSuch);
    // "The Kibombo never move in such large numbers without orders from..."
    Event_ShowMessage(ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, 0);
  }
}

/// The flag a Yes to "Can you tell me where you have come from?" sets, and
/// the actor the hot-weather line after it opens at.
// BUG: a Yes sets FLAG_ASKED_ABOUT_EXPEDITION, the expedition woman's flag,
// where FLAG_ASKED_WHERE_FROM was meant: she asks every time, the line on the
// heat after it is never shown (and would be at the expedition woman), and the
// expedition woman stops asking hers.
#if BUGFIX
#define WHERE_FROM_ASKED FLAG_ASKED_WHERE_FROM
#define AFTER_WHERE_FROM_SPEAKER ACTOR_WOMAN_ASKING_WHERE_FROM
#else
#define WHERE_FROM_ASKED FLAG_ASKED_ABOUT_EXPEDITION
#define AFTER_WHERE_FROM_SPEAKER ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION
#endif

/// The old woman in a headband east of the fire asks Felix: "Can you tell me
/// where you have come from?" She asks it again every time, because of a bug
/// (below); the line she was to say after, that it is always hot in Naribwe,
/// is never heard from her.
void Naribwe_AskWhereFrom(void) {
  s32 msg;

  if (Event_GetFlag(FLAG_ASKED_WHERE_FROM) == 0) {
    msg = (s32)MsgNaribweCanYouTellMeWhere;
    Event_SetMessageId(msg);
    // "Can you tell me where you have come from?"
    Event_OpenMessage(ACTOR_WOMAN_ASKING_WHERE_FROM, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      Event_SetFlag(WHERE_FROM_ASKED);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(ACTOR_WOMAN_ASKING_WHERE_FROM, 0);
  } else {
    Event_SetMessageId((s32)MsgNaribweItsAlwaysHotHere);
    // "It's always hot here in Naribwe. It doesn't matter if it's summer..."
    Event_ShowMessage(AFTER_WHERE_FROM_SPEAKER, 0);
  }
}

/// The boy with black hair just north of the fire asks Felix: "You don't plan
/// on going to Kibombo, do you, mister?"
void Naribwe_AskGoingToKibombo(void) {
  s32 msg = (s32)MsgNaribweYouDontPlanOnGoing;

  Event_SetMessageId(msg);
  Event_OpenMessage(ACTOR_BOY_ASKING_OF_KIBOMBO, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(ACTOR_BOY_ASKING_OF_KIBOMBO, 0);
}

/// Naribwe's shops, as Shop_Run numbers them.
enum { SHOP_NARIBWE_ITEMS = 16 };

/// Felix talks to the woman in a teal headband at the item stall in the south
/// of the village: facing her from the east, across her stall, he can shop;
/// from elsewhere she says: "No one can see the future. Absolutely no one.
/// Except for our fortune-teller."
void Naribwe_PraiseFortuneTeller(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing) ==
      FACING_WEST) {
    Shop_Run(SHOP_NARIBWE_ITEMS, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribweNoOneCanSeeFuture);
  // "No one can see the future. Absolutely no one. Except for our..."
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// Felix talks to the woman at the item stall once Piers has carried Gabomba's
/// message: across her stall, he can shop; from elsewhere she says strangers
/// should visit the fortune-teller's hut.
void Naribwe_PraiseFortuneTellerOnReturn(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing) ==
      FACING_WEST) {
    Shop_Run(SHOP_NARIBWE_ITEMS, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribweStrangersToOurTownWould);
  // "Strangers to our town would do well to visit the fortune-teller's hut."
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// Felix talks to the pilgrim in white below the fortune-teller's hut: facing
/// him from the south, he can use the Sanctum; from elsewhere the pilgrim says:
/// "I am but a humble pilgrim, serving the greater good in the heart of
/// Gondowan."
void Naribwe_TalkToPilgrim(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing) ==
      FACING_NORTH) {
    Sanctum_Run(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribweIAmButHumblePilgrim);
  // "I am but a humble pilgrim, serving the greater good in the heart of..."
  Event_ShowMessage(ACTOR_PILGRIM, 0);
  Event_End();
}

/// Felix talks to the pilgrim below the fortune-teller's hut once Piers has
/// carried Gabomba's message: from the south, he can use the Sanctum; from
/// elsewhere the pilgrim says how cold it grows to the south.
void Naribwe_TalkToPilgrimOnReturn(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing) ==
      FACING_NORTH) {
    Sanctum_Run(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribweItsStrangeAsHot);
  // "It's strange. As hot as it is here in Gondowan, if you head south..."
  Event_ShowMessage(ACTOR_PILGRIM, 0);
  Event_End();
}

/// Felix walks north into the door of the inn, the southeast hut, at cell
/// (18, 23), or of the fortune-teller's hut at the top of the village, at
/// cell (21, 8): everyone stops, the door opens, and he goes in.
///
/// Each door's cell carries the number of the exit it leaves by; the other
/// three houses' doorways are plain exits.
void Naribwe_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 *trigger;
  s32 index;
  u32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (id = ACTOR_FIRST_PLACED; id < FIELD_TREASURE_ACTOR; id++) {
    struct Actor *actor = Actor_GetById(id);

    if (actor != NULL) {
      u8 *flags = &actor->motionFlags;

      *flags = 0;
    }
  }
  trigger = &work->stepTrigger;
  index = *trigger;
  Event_PlaySound(SE_DOOR_OPEN);
  index -= NARIBWE_TO_NARIBWE_ROOMS_SOUTHEAST;
  MapGrid_AnimateCells(gNaribwe_Doors[index].script, gNaribwe_Doors[index].x,
                       gNaribwe_Doors[index].z);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Import_EventSetExitRequest(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Felix casts Whirlwind at the weeds in the back yard behind the northwest
/// houses: the leaves blow away and the ladder up to the ledge of totem poles
/// shows. The weeds stay gone until the party leaves the village.
void Naribwe_ScatterWeeds(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(FLAG_WEEDS_SCATTERED);
  Event_End();
}

/// Unused: writes the rope's post and end into the event work; nothing calls
/// it.
UNUSED void Naribwe_SetRopeWork(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->targetX = 0;
  work->targetY = ACTOR_ROPE_POST;
  work->targetZ = ACTOR_ROPE_END;
}

/// Felix casts Reveal anywhere in the village: on the ledge of totem poles
/// above the back yard, the rock pile is gone and the Unicorn Ring's chest
/// shows in its place.
void Naribwe_RevealChest(void) {
  Actor_SetPosition(ACTOR_ROCK_PILE, 0, 0);
  TreasureSpot_SetPosition(ACTOR_UNICORN_RING_CHEST, toFIXED(104),
                           toFIXED(104));
}

/// The Unicorn Ring's chest stays hidden under the rock pile on the ledge of
/// totem poles, as the village opens and again as Reveal wears off.
void Naribwe_HideChest(void) {
  TreasureSpot_SetActorPosition(ACTOR_UNICORN_RING_CHEST, 0, 0);
  Actor_SetPosition(ACTOR_ROCK_PILE, 104, 104);
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_WEST_BRAZIER), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_EAST_BRAZIER), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_WEEDS), 0);
  Actor_GetById(ACTOR_WEEDS)->scaleY = FIXED_ONE * 8 / 5;
  Actor_GetById(ACTOR_ROCK_PILE)->scaleX = FIXED_ONE / 2;
  Actor_GetById(ACTOR_ROCK_PILE)->scaleY = FIXED_ONE / 2;
  Naribwe_HideChest();
  if (Event_GetFlag(FLAG_WEEDS_SCATTERED))
    Actor_SetPosition(ACTOR_WEEDS, 0, 0);
  Rope_Initialize();
  Rope_Place(0, FLAG_NARIBWE_ROPE_TIED, ACTOR_ROPE_POST, ACTOR_ROPE_END);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROPE_POST), 0);
  Actor_DisableById(ACTOR_ROPE_END);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// Naribwe's data: its scripts, door, placements and triggers.

extern s32 gNaribwe_BrazierBurns[];
extern s32 gNaribwe_HoldOverBrazier[];

extern s32 gNaribwe_WeedsAnimation[];
extern s16 gNaribwe_DoorOpening[];

/// The village's placements, the same before and after the return with Piers.
#define DEFINE_NARIBWE_PLACEMENTS(name)                                        \
  struct ActorPlacement name[] = {                                             \
      ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS,                    \
                      MOVEMENT_TYPE_STAND, 232, 0, 246, FACING_SOUTH,          \
                      TALK_FACING_PARTY, 0),                                   \
      ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS,                        \
                      (s32)gNaribwe_BrazierBurns, 316, 0, 132, FACING_SOUTH,   \
                      TALK_FACING_PARTY, 0),                                   \
      ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS,                        \
                      (s32)gNaribwe_BrazierBurns, 372, 0, 132, FACING_SOUTH,   \
                      TALK_FACING_PARTY, 0),                                   \
      ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS,             \
                      (s32)gNaribwe_HoldOverBrazier, 316, 0, 132,              \
                      FACING_SOUTH, TALK_FACING_PARTY, 0),                     \
      ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS,             \
                      (s32)gNaribwe_HoldOverBrazier, 372, 0, 132,              \
                      FACING_SOUTH, TALK_FACING_PARTY, 0),                     \
      ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,                \
                      MOVEMENT_TYPE_STAND, 248, 0, 344, FACING_SOUTH,          \
                      TALK_FACING_PARTY_AND_BACK, 0),                          \
      ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,          \
                      MOVEMENT_TYPE_WANDER_NEAR, 200, 0, 296, FACING_EAST,     \
                      TALK_FACING_PARTY, 0),                                   \
      ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,            \
                      MOVEMENT_TYPE_STAND, 248, 0, 152, FACING_SOUTH,          \
                      TALK_FACING_PARTY_AND_BACK, 0),                          \
      ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS,           \
                      MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 376, FACING_NORTHEAST, \
                      TALK_FACING_PARTY, 0),                                   \
      ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,           \
                      MOVEMENT_TYPE_STAND, 248, 0, 88, FACING_SOUTHEAST,       \
                      TALK_FACING_PARTY_AND_BACK, 0),                          \
      ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS,        \
                      MOVEMENT_TYPE_STAND, 368, 0, 224, FACING_SOUTHEAST,      \
                      TALK_FACING_PARTY_AND_BACK, 0),                          \
      ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,            \
                      MOVEMENT_TYPE_WANDER, 200, 0, 200, FACING_NORTHEAST,     \
                      TALK_FACING_PARTY, 0),                                   \
      ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,        \
                      MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_SOUTHWEST,      \
                      TALK_FACING_PARTY_AND_BACK, 0),                          \
      ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,           \
                      MOVEMENT_TYPE_WANDER, 56, 0, 232, FACING_EAST,           \
                      TALK_FACING_PARTY, 0),                                   \
      ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS,       \
                      MOVEMENT_TYPE_STAND, 168, 0, 360, FACING_EAST,           \
                      TALK_FACING_PARTY_AND_BACK, 0),                          \
      ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS,                          \
                      (s32)gNaribwe_WeedsAnimation, 40, 0, 168, FACING_SOUTH,  \
                      TALK_FACING_KEEP, 0),                                    \
      ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,                 \
                      MOVEMENT_TYPE_STAND, 360, 0, 376, FACING_SOUTH,          \
                      TALK_FACING_KEEP, 0),                                    \
      ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS,                  \
                      MOVEMENT_TYPE_STAND, 392, 0, 376, FACING_SOUTH,          \
                      TALK_FACING_KEEP, 0),                                    \
      ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,    \
                      376, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK,   \
                      0),                                                      \
      ACTOR_PLACEMENT(SPRITE_ROCK_PILE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, \
                      104, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK,   \
                      0),                                                      \
      {SCENE_TABLE_END},                                                       \
  };

// clang-format off
/// Unused: a list of the pilgrim and FLAG_SCENE_TEMP_00, in a flag block's
/// pairs; nothing reads it.
s16 gNaribwe_UnusedList[] = {
    ACTOR_PILGRIM, FLAG_SCENE_TEMP_00, -1, 0,
};

/// The braziers burn.
s32 gNaribwe_BrazierBurns[] = {
    SC_ANIMATION(2),
    SC_END,
};

/// The unseen actors over the braziers: nothing drawn, held 24 pixels up.
s32 gNaribwe_HoldOverBrazier[] = {
    SC_ANIMATION(0),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(24)),
    SC_END,
};

s32 gNaribwe_WeedsAnimation[] = {
    SC_ANIMATION(3),
    SC_END,
};

/// How the inn's and the fortune-teller's doors open.
s16 gNaribwe_DoorOpening[] = {
    8, 42, 1, 2, 6, -1,
};

/// The inn's door and the fortune-teller's, which open as the party walks
/// through.
struct WarpEntry gNaribwe_Doors[] = {
    {gNaribwe_DoorOpening, 18, 51},
    {gNaribwe_DoorOpening, 21, 34},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 224, 0, 190, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Naribwe),
    SCENE_EXIT_UNLESS(NARIBWE_TO_NARIBWE_ROOMS_NORTHWEST, NaribweRooms, NARIBWE_ROOMS_FROM_NARIBWE_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_TO_NARIBWE_ROOMS_WEST, NaribweRooms, NARIBWE_ROOMS_FROM_NARIBWE_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_TO_NARIBWE_ROOMS_SOUTHWEST, NaribweRooms, NARIBWE_ROOMS_FROM_NARIBWE_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_TO_NARIBWE_ROOMS_SOUTHEAST, NaribweRooms, NARIBWE_ROOMS_FROM_NARIBWE_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_TO_NARIBWE_ROOMS_NORTHEAST, NaribweRooms, NARIBWE_ROOMS_FROM_NARIBWE_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_NARIBWE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_NARIBWE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The village's people until Gabomba's message is carried.
DEFINE_NARIBWE_PLACEMENTS(gNaribwe_Placements)

struct ActorPlacement gNaribwe_FireLitPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 246, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 316, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 316, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

/// The same people once it has been.
DEFINE_NARIBWE_PLACEMENTS(gNaribwe_PlacementsOnReturn)

struct Trigger gNaribwe_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, NARIBWE_TO_NARIBWE_ROOMS_NORTHWEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, NARIBWE_TO_NARIBWE_ROOMS_WEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, NARIBWE_TO_NARIBWE_ROOMS_SOUTHWEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     NARIBWE_TO_NARIBWE_ROOMS_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(Naribwe_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     NARIBWE_TO_NARIBWE_ROOMS_NORTHEAST, CONDITION_ALWAYS,
     EVENT(Naribwe_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, NARIBWE_TO_WORLD_MAP_SOUTH},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, NARIBWE_TO_WORLD_MAP_NORTH},
    {TRIGGER_TALK, ACTOR_MAN_AMAZED_BY_THE_RETURN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweWelcomeToNaribweItsReally)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, CONDITION_ALWAYS,
     EVENT(Naribwe_AskAboutKibomboExpedition)},
    {TRIGGER_TALK, ACTOR_MAN_WARNING_OF_THE_NORTH, CONDITION_ALWAYS,
     MSGNO(MsgNaribweIWouldNotVentureFar)},
    {TRIGGER_TALK, ACTOR_WOMAN_WHO_SAW_THE_GEMS_FALL, CONDITION_ALWAYS,
     MSGNO(MsgNaribweItsBeenDangerousOutEver)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_OEIA, CONDITION_ALWAYS,
     MSGNO(MsgNaribweWhenOeiaWasTheirWitch)},
    {TRIGGER_TALK, ACTOR_WOMAN_ASKING_WHERE_FROM, CONDITION_ALWAYS,
     EVENT(Naribwe_AskWhereFrom)},
    {TRIGGER_TALK, ACTOR_BOY_ASKING_OF_KIBOMBO, CONDITION_ALWAYS,
     EVENT(Naribwe_AskGoingToKibombo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_MAGMA_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgNaribweOnFarSideOfRiver)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_HURT_BY_A_BRUTE, CONDITION_ALWAYS,
     MSGNO(MsgNaribweWhenIClimbedKibomboMountains)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Naribwe_PraiseFortuneTeller)},
    {TRIGGER_TALK, ACTOR_PILGRIM, CONDITION_ALWAYS,
     EVENT(Naribwe_TalkToPilgrim)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AMAZED_BY_THE_RETURN,
     CONDITION_ALWAYS, MSGNO(MsgNaribweICanHardlyStandHeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, CONDITION_ALWAYS,
     MSGNO(MsgNaribweAkafubuIsAlmostCertainlyLooking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARNING_OF_THE_NORTH,
     CONDITION_ALWAYS, MSGNO(MsgNaribweItIsAwfulToNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_SAW_THE_GEMS_FALL,
     CONDITION_ALWAYS, MSGNO(MsgNaribweThatWasDayWindBlew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_OEIA,
     CONDITION_ALWAYS, MSGNO(MsgNaribweKibomboMagicWasOnceWidely)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_WHERE_FROM,
     CONDITION_ALWAYS, MSGNO(MsgNaribweWarriorWhoPassedThroughHere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_OF_KIBOMBO,
     CONDITION_ALWAYS, MSGNO(MsgNaribweToGetToKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_MAGMA_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgNaribweSometimesYouCanHearExplosions)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_HURT_BY_A_BRUTE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweIRememberWhenIFound)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweEvenIfIWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PILGRIM, CONDITION_ALWAYS,
     MSGNO(MsgNaribweAnywhereICanDoSome)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgNaribweTheresOnlyLittleBit))},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_BONFIRE, CONDITION_ALWAYS, EVENT(Naribwe_PrepareBlaze)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS, FLAG_WEEDS_SCATTERED,
     EVENT(Naribwe_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST, FLAG_NARIBWE_ROPE_TIED,
     0},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_RevealChest)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_HideChest)},
    {TRIGGER_TABLE_END},
};

struct Trigger gNaribwe_TriggersOnReturn[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, NARIBWE_TO_NARIBWE_ROOMS_NORTHWEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, NARIBWE_TO_NARIBWE_ROOMS_WEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, NARIBWE_TO_NARIBWE_ROOMS_SOUTHWEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     NARIBWE_TO_NARIBWE_ROOMS_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(Naribwe_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     NARIBWE_TO_NARIBWE_ROOMS_NORTHEAST, CONDITION_ALWAYS,
     EVENT(Naribwe_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, NARIBWE_TO_WORLD_MAP_SOUTH},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, NARIBWE_TO_WORLD_MAP_NORTH},
    {TRIGGER_TALK, ACTOR_MAN_AMAZED_BY_THE_RETURN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweDontTellMeYouJust)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, CONDITION_ALWAYS,
     MSGNO(MsgNaribweIDidntSeeAnyKibombo)},
    {TRIGGER_TALK, ACTOR_MAN_WARNING_OF_THE_NORTH, CONDITION_ALWAYS,
     MSGNO(MsgNaribweISawSmokeRising)},
    {TRIGGER_TALK, ACTOR_WOMAN_WHO_SAW_THE_GEMS_FALL, CONDITION_ALWAYS,
     MSGNO(MsgNaribweSomeSoldiersCameThroughHere)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_OEIA, CONDITION_ALWAYS,
     MSGNO(MsgNaribweSmokeComingFromKibomboMountains)},
    {TRIGGER_TALK, ACTOR_WOMAN_ASKING_WHERE_FROM, CONDITION_ALWAYS,
     MSGNO(MsgNaribweFortuneTellerPredictedThatKibombo)},
    {TRIGGER_TALK, ACTOR_BOY_ASKING_OF_KIBOMBO, CONDITION_ALWAYS,
     MSGNO(MsgNaribweManCameThroughWhileAgo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_MAGMA_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgNaribweIMentionedMagmaRock)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_HURT_BY_A_BRUTE, CONDITION_ALWAYS,
     MSGNO(MsgNaribweOuchSomeBigBruteCame)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Naribwe_PraiseFortuneTellerOnReturn)},
    {TRIGGER_TALK, ACTOR_PILGRIM, CONDITION_ALWAYS,
     EVENT(Naribwe_TalkToPilgrimOnReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AMAZED_BY_THE_RETURN,
     CONDITION_ALWAYS, MSGNO(MsgNaribweWhoWouldTravelToKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, CONDITION_ALWAYS,
     MSGNO(MsgNaribweImSoGladKibomboArent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARNING_OF_THE_NORTH,
     CONDITION_ALWAYS, MSGNO(MsgNaribweThatSmokesBeenRising)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_SAW_THE_GEMS_FALL,
     CONDITION_ALWAYS, MSGNO(MsgNaribweWhatKindOfName)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_OEIA,
     CONDITION_ALWAYS, MSGNO(MsgNaribweSignalsSaySomethingHasEnded)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_WHERE_FROM,
     CONDITION_ALWAYS, MSGNO(MsgNaribweFortuneTellerHasNeverBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_OF_KIBOMBO,
     CONDITION_ALWAYS, MSGNO(MsgNaribweIfManTheyWereLooking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_MAGMA_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgNaribweIDontThinkAnyoneCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_HURT_BY_A_BRUTE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweWhatHasGoneWrong)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweThoseWhoBecomeLostShould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PILGRIM, CONDITION_ALWAYS,
     MSGNO(MsgNaribweColdOfTundariaStiflesVirtually)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgNaribweTheresOnlyLittleBit))},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_BONFIRE, CONDITION_ALWAYS, EVENT(Naribwe_PrepareBlaze)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS, FLAG_WEEDS_SCATTERED,
     EVENT(Naribwe_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST, FLAG_NARIBWE_ROPE_TIED,
     0},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_RevealChest)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_HideChest)},
    {TRIGGER_TABLE_END},
};
