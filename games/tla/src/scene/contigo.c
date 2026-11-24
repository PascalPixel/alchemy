/// @file
/// Contigo, the village of the Jupiter clan north of Atteka Inlet, beside the
/// crater below Jupiter Lighthouse, and the rooms of its overlay: the houses,
/// shops, inn and Sanctum, the three game tents, and the Anemos geoglyph east
/// of the village, where the villagers build the Wings of Anemos in front of
/// the Anemos Sanctum.
///
/// On Felix's first visit the village is busy with the wings. Scoop in the
/// open gap of the big weed patch east of the village gate finds the Djinni
/// Salt, and Cyclone on the clump of weeds beside the house at the top of the
/// hill uncovers a Bramble Seed. The tent left of the inn holds Lucky Wheels,
/// and the two tents north of the Teleport circle Super Lucky Dice (west) and
/// Lucky Dice (east). Once Jupiter Lighthouse's beacon is lit, the villagers
/// are away transporting the wings and four doors are locked; Isaac's party
/// waits in the house at the top of the hill, and walking in starts the
/// reunion (Contigo_TheReunion). That house becomes Hamma's new home, and on
/// the party's first visit after the ship has its wings she talks with Ivan
/// (Contigo_ReturnToNewHome). In a linked game, Force on the tree stump
/// behind the inn frees the Djinni Shine. Late in the game, Reveal in front of
/// the Sanctum opens its middle door, and Teleport on the circle in the middle
/// of the village leads into the Anemos Inner Sanctum.

#include "types.h"
#include "macros.h"
#include "window/message_argument.h"
#include "item_ids.h"
#include "system/main_routine_state.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "text/msg_ids.h"
#include "scene/sparkle.h"
#include "graphics/palette_buffers.h"
#include "version.h"

/// Force has freed Shine from the tree stump it was stuck in.
#define FLAG_SHINE_FREED 0x9e7
/// Set as Shine, freed, joins the party; until then it waits where it landed.
#define FLAG_SHINE_CAUGHT 0x9e6
/// Set once Hamma has welcomed the party to her new home, after it set out
/// with the wings, and told Ivan why she kept from him that she is his sister
/// (Contigo_ReturnToNewHome).
#define FLAG_HAMMA_WELCOMED_PARTY 0x9d1

/// The village's temp flag: the camera follows the leader east past the
/// village gate as far as the weed patch, rather than stopping at the gate
/// (Contigo_WidenCameraBounds).
#define FLAG_WHOLE_VILLAGE_IN_VIEW FLAG_SCENE_TEMP_00

/// The map cells the village's, the geoglyph's and the houses' trigger rows
/// answer. A door's cell takes the number of the exit it leaves by, so those
/// rows use the exit's own name.
enum ContigoCellBehavior {
  /// The geoglyph's west edge, where the path from the village's east edge
  /// comes in.
  CELL_BEHAVIOR_GEOGLYPH_WEST_EDGE = 12,
  /// The doors at the head of the Sanctum's west, middle and east steps.
  CELL_BEHAVIOR_SANCTUM_WEST_DOOR = 13,
  CELL_BEHAVIOR_SANCTUM_MIDDLE_DOOR = 14,
  CELL_BEHAVIOR_SANCTUM_EAST_DOOR = 15,
  /// A door shut while the villagers are away with the wings
  /// (Contigo_LockDoors).
  CELL_BEHAVIOR_LOCKED_DOOR = 19,
  /// Across the three ways from the middle of the village down to its south
  /// part: the north line keeps the view west of the gate, the south one lets
  /// it follow the leader east.
  CELL_BEHAVIOR_VIEW_WEST_OF_GATE = 20,
  CELL_BEHAVIOR_VIEW_EAST_OF_GATE = 21,
  /// Just inside the door of Hamma's new home.
  CELL_BEHAVIOR_INSIDE_HAMMAS_DOOR = 20,
  /// The Sanctum's carved wall beside the translation sign.
  CELL_BEHAVIOR_CARVED_WALL = 22,
  /// The signs outside the three game tents.
  CELL_BEHAVIOR_LUCKY_WHEELS_SIGN = 24,
  CELL_BEHAVIOR_SUPER_LUCKY_DICE_SIGN = 25,
  CELL_BEHAVIOR_LUCKY_DICE_SIGN = 26,
  CELL_BEHAVIOR_TELEPORT_CIRCLE = 30,
  /// The open gap in the middle of the weed patch, where Salt hides.
  CELL_BEHAVIOR_WEED_PATCH_GAP = 32,
  /// The clumps of weeds about the village, and the five rows of the big
  /// patch east of the gate, from north to south.
  CELL_BEHAVIOR_WEEDS_WEST_OF_ITEM_SHOP = 39,
  CELL_BEHAVIOR_WEEDS_BEHIND_INN,
  CELL_BEHAVIOR_WEEDS_OVER_BRAMBLE_SEED,
  CELL_BEHAVIOR_WEEDS_WEST_OF_WEAPON_SHOP,
  CELL_BEHAVIOR_WEEDS_BY_GATE,
  CELL_BEHAVIOR_WEEDS_BELOW_LUCKY_WHEELS,
  CELL_BEHAVIOR_WEEDS_ON_CLIFF_WEST,
  CELL_BEHAVIOR_WEEDS_ON_CLIFF_EAST,
  CELL_BEHAVIOR_WEED_PATCH_TOP_ROW,
  CELL_BEHAVIOR_WEED_PATCH_SECOND_ROW,
  CELL_BEHAVIOR_WEED_PATCH_MIDDLE_ROW,
  CELL_BEHAVIOR_WEED_PATCH_FOURTH_ROW,
  CELL_BEHAVIOR_WEED_PATCH_BOTTOM_ROW,
  /// The pots and ovens in the houses: the bean soup, the corn bread and the
  /// cookies.
  CELL_BEHAVIOR_BEAN_SOUP = 200,
  CELL_BEHAVIOR_CORN_BREAD,
  CELL_BEHAVIOR_COOKIES
};

/// The tile row of Hamma's portrait as she and Isaac speak at once in the
/// reunion (Contigo_TheReunion): two rows higher from the first patch on.
#if !TLA_PATCH_1
#define REUNION_HAMMA_PORTRAIT_Y 12
#else
#define REUNION_HAMMA_PORTRAIT_Y 10
#endif

/// What the dice and wheels attendants say as the party comes back from a
/// game (Contigo_InitializeLuckyDice); the Spanish text numbers these lines
/// differently.
#if LANGUAGE == LANGUAGE_SPANISH
#define COIN_WON_MESSAGE ((s32)MsgContigoHeyYaDontHaveEnough + 1)
#define COIN_LOST_MESSAGE ((s32)MsgContigoHeyYaDontHaveEnough + 2)
#define READ_SIGN_MESSAGE ((s32)MsgContigoYaWonCoinTalkAbout)
#define CONGRATULATIONS_MESSAGE ((s32)MsgContigoYaLostCoinBetterLuck)
#define FIRST_PRIZE_MESSAGE ((s32)MsgContigoTheRulesAreListedOn)
#define NEXT_PRIZE_MESSAGE ((s32)MsgContigoCongratulations)
#define COME_AGAIN_MESSAGE ((s32)MsgContigoComeAgain)
#else
#define COIN_WON_MESSAGE ((s32)MsgContigoYaWonCoinTalkAbout)
#define COIN_LOST_MESSAGE ((s32)MsgContigoYaLostCoinBetterLuck)
#define READ_SIGN_MESSAGE ((s32)MsgContigoTheRulesAreListedOn)
#define CONGRATULATIONS_MESSAGE ((s32)MsgContigoCongratulations)
#define FIRST_PRIZE_MESSAGE ((s32)MsgContigoYouWonHereYouGo)
#define NEXT_PRIZE_MESSAGE ((s32)MsgContigoAndYouWonHereYou)
#define COME_AGAIN_MESSAGE ((s32)MsgContigoComeAgain)
#endif

/// Set as the party leaves the dice hall for one of its games, so that the
/// hall greets it on its way back by the game's entrance.
#define FLAG_LEFT_FOR_A_GAME 0x9e5

/// The dice hall's ways back from its games.
enum ContigoDiceHallEntrance {
  ENTRANCE_FROM_SUPER_LUCKY_DICE = 18,
  ENTRANCE_FROM_LUCKY_DICE,
  ENTRANCE_FROM_LUCKY_WHEELS
};

/// The village's people and Djinn (gContigo_VillagePlacements).
enum ContigoVillageActorId {
  /// "The Anemos blood in our veins often brings about strange..."
  ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND = ACTOR_FIRST_PLACED,
  /// "The beacon within Jupiter Lighthouse will soon be lit. I know this
  /// because of a strange premonition I had three years ago."
  ACTOR_WOMAN_FORESEEING_THE_BEACON,
  /// "It must be so exciting to travel around the world in a flying ship."
  ACTOR_MAN_ON_THE_FLYING_SHIP,
  /// "The people of Anemos possessed power over the wind."
  ACTOR_WOMAN_ON_THE_ANEMOS,
  /// "If a ship can rise up from the sea, why can't a city rise up from the
  /// ground?"
  ACTOR_OLD_MAN_ON_THE_RISEN_CITY,
  /// "The Anemos geoglyphs show a boat with wings..."
  ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS,
  /// The two children walking round the middle of the village: the boy,
  /// "Cities flying up into the sky? I don't buy that for a second!"
  /// (Contigo_TalkToDoubtingBoy), and the girl, "It wasn't what I expected,
  /// but seeing the ship take off was still amazing."
  /// (Contigo_TalkAboutTheShip)
  ///@{
  ACTOR_BOY_DOUBTING_THE_LEGENDS,
  ACTOR_GIRL_ON_THE_SHIP,
  ///@}
  /// The Djinn Salt (Venus) and Shine (Mars).
  ACTOR_SALT,
  ACTOR_SHINE
};

/// The shops and inn before the reunion
/// (gContigo_ShopsPlacementsBeforeReunion).
enum ContigoShopsActorId {
  /// "I guess the lighthouse was lit because Ivan and Hamma finally returned
  /// to Contigo."
  ACTOR_MAN_ON_IVANS_RETURN = ACTOR_FIRST_PLACED,
  /// "Ivan doesn't remember his mother..."
  ACTOR_WOMAN_ON_IVANS_MOTHER,
  /// "From the look on Ivan's..."; his thought: "Someday I'm gonna fight
  /// Ivan..."
  ACTOR_BOY_WHO_WOULD_FIGHT_IVAN,
  /// "Long ago, the people of Hesperia attacked Atteka."
  ACTOR_OLD_MAN_ON_HESPERIA,
  /// "Winter just ended, but it keeps getting colder. What's going on?"
  ACTOR_OLD_WOMAN_FEELING_THE_COLD,
  /// The woman behind the item shop's counter (Contigo_TalkToItemMerchant).
  ACTOR_ITEM_MERCHANT,
  /// "Soon, the siblings with the power of the Anemos will return."
  ACTOR_YOUNG_MAN_ON_THE_SIBLINGS,
  /// "The power of the Anemos is all but gone in Contigo."
  ACTOR_WOMAN_ON_THE_ANEMOS_POWER,
  /// The baby's brother: "I don't have any powers like the Anemos did, but my
  /// sister, Ahri, is different."
  ACTOR_BABYS_BROTHER,
  /// The baby whose furrowed brow brings the wind (Contigo_BabyThinks).
  ACTOR_BABY,
  /// The two men behind the counter of swords and shields: the weapon
  /// merchant, and the armor merchant, "Mining materials in the ruins is
  /// dangerous work, so my armor is selling well."
  ///@{
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  ///@}
  ACTOR_INNKEEPER,
  /// "All the work on the wings means that our inn is always full!"
  ACTOR_INN_WORKER,
  /// "Gotta make all the beds before everyone returns from Anemos
  /// Sanctum..."
  ACTOR_WOMAN_MAKING_THE_BEDS,
  /// "I must be the greatest chef in the world!"
  ACTOR_COOK,
  /// The workers on the wings, resting at the inn: "I worked myself half to
  /// death yesterday."
  ACTOR_WORN_OUT_WORKER,
  /// "The prophecies of the Anemos tell that someday soon..."
  ACTOR_WORKER_ON_THE_PROPHECIES,
  /// "I'm afraid I'm too old to help in the construction..."
  ACTOR_OLD_MAN_TOO_OLD_TO_HELP,
  /// "When is that miracle of the Anemos gonna hurry up..."
  ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE,
  /// The robed man behind the Sanctum's counter (Contigo_TalkToHealer).
  ACTOR_HEALER
};

/// Hamma and Kraden at the reunion and after it (gContigo_ShopsPlacements,
/// gContigo_ShopsPlacementsReunion).
enum ContigoReunionActorId { ACTOR_HAMMA = ACTOR_FIRST_PLACED, ACTOR_KRADEN };

/// The dice and wheels hall (gContigo_LuckyDicePlacements).
enum ContigoLuckyDiceActorId {
  /// A young man with fair hair (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR), who runs
  /// Super Lucky Dice (Contigo_OfferSuperLuckyDice).
  ACTOR_SUPER_LUCKY_DICE_ATTENDANT = ACTOR_FIRST_PLACED,
  /// Beside him, in his sprite, pointing the party to the rules: "If you want
  /// to learn about the rules to Super Lucky Dice, just check out the sign."
  ACTOR_SUPER_LUCKY_DICE_RULES_ATTENDANT,
  /// The players: "This kid's got unlucky written all over his face" (his
  /// thought); "If you lose the hi-lo game, you don't win anything...";
  /// "I almost always get at least one pair..."
  ///@{
  ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS,
  ACTOR_OLD_MAN_ON_THE_HI_LO_GAME,
  ACTOR_WOMAN_WHO_LOSES_AT_HI_LO,
  ///@}
  /// The sign of Super Lucky Dice's rules (Contigo_ReadSuperLuckyDiceRules).
  ACTOR_SUPER_LUCKY_DICE_RULES_SIGN,
  /// A young man with fair hair, in other colours, who runs Lucky Dice
  /// (Contigo_OfferLuckyDice).
  ACTOR_LUCKY_DICE_ATTENDANT,
  /// Beside him, in his sprite, pointing the party to the rules: "Read the
  /// sign if ya don't know the rules."
  ACTOR_LUCKY_DICE_RULES_ATTENDANT,
  /// Lucky Dice's two signs: how to play (Contigo_ReadLuckyDiceRules), and
  /// what pairs, triples and four of a kind win (Contigo_ReadLuckyDicePairs).
  ///@{
  ACTOR_LUCKY_DICE_RULES_SIGN,
  ACTOR_LUCKY_DICE_PAIRS_SIGN,
  ///@}
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN), who runs Lucky
  /// Wheels (Contigo_TalkToWheelsAttendant).
  ACTOR_LUCKY_WHEELS_ATTENDANT,
  /// Lucky Wheels' signs: its rules (Contigo_ReadLuckyWheelsRules), and its
  /// prizes (Contigo_ReadLuckyWheelsPrizes).
  ///@{
  ACTOR_LUCKY_WHEELS_RULES_SIGN,
  ACTOR_LUCKY_WHEELS_PRIZE_SIGN
  ///@}
};

/// The geoglyph, where the Wings of Anemos are built
/// (gContigo_GeoglyphPlacements).
enum ContigoGeoglyphActorId {
  /// "I've come a long way to see the flying boat."
  ACTOR_VISITOR_FROM_AFAR = ACTOR_FIRST_PLACED,
  /// "The wings are almost done."
  ACTOR_WOMAN_ON_THE_WINGS,
  /// The four workers building the wings: "Which part goes where? Does this
  /// metal feather connect to that wooden post?"
  ///@{
  ACTOR_WING_WORKER_A,
  ACTOR_WING_WORKER_B,
  ACTOR_WING_WORKER_C,
  ACTOR_WING_WORKER_D,
  ///@}
  /// The four men in long robes by the Sanctum's wall: "Have you taken a good
  /// look at the Anemos geoglyph?"; "I know that devoted pilgrims will
  /// appreciate having this translation."; "I want to get a good look at the
  /// inside of the sanctum, but I can't get this door open."; "The merchant
  /// Hammet took the gold in this room with him when he left Contigo."
  ///@{
  ACTOR_MAN_ON_THE_GEOGLYPH,
  ACTOR_MAN_WHO_TRANSLATED_THE_WALL,
  ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM,
  ACTOR_MAN_ON_HAMMET,
  ///@}
  /// The sign with a translation of the characters carved into the wall.
  ACTOR_SIGN,
  /// The wings' parts lying about: three frames, five metal feathers and
  /// seven loose pieces.
  ///@{
  ACTOR_WING_FRAME_A,
  ACTOR_WING_FRAME_B,
  ACTOR_WING_FRAME_C,
  ACTOR_METAL_FEATHER_A,
  ACTOR_METAL_FEATHER_B,
  ACTOR_METAL_FEATHER_C,
  ACTOR_METAL_FEATHER_D,
  ACTOR_METAL_FEATHER_E,
  ACTOR_LOOSE_PART_A,
  ACTOR_LOOSE_PART_B,
  ACTOR_LOOSE_PART_C,
  ACTOR_LOOSE_PART_D,
  ACTOR_LOOSE_PART_E,
  ACTOR_LOOSE_PART_F,
  ACTOR_LOOSE_PART_G
  ///@}
};
#include "field/event_inline.h"
#include "field/actor_script.h"
#include "fixed_math.h"

struct Actor;

void Actor_PlaceOnActor(s32 actor, s32 target);
void FieldCamera_WaitForArrival(void);
void Party_AddIsaacsParty(void);
void Contigo_OfferLuckyWheels(s32 mode);
void Contigo_OpenSanctumDoor(s32 sourceX, s32 sourceY, s32 doorX, s32 doorY);

#include "system/task.h"
#include "field/actor.h"
#include "field/scene_tables.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "flags.h"
#include "djinn_ids.h"
#include "shared/tables.h"
#include "system/field_effect.h"
#include "field/field_sequence_event.h"
#include "window/item_icon.h"
#include "field/event_commands.h"
#include "system/iwram_call.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "system/vector3.h"
#include "field/objdisp.h"
#include "field/field_psynergy_state.h"

/// The village's treasure actors after the Power Bread's (gSceneItems): the
/// Corn, and the Bramble Seed under the weeds north of the Teleport circle.
enum {
  ACTOR_CORN = FIELD_TREASURE_ACTOR + 1,
  ACTOR_BRAMBLE_SEED = FIELD_TREASURE_ACTOR + 2
};

extern const s32 *gFieldEffect_DurationScripts[];
extern struct ActorPlacement gContigo_VillagePlacements[];
extern struct ActorPlacement gContigo_GeoglyphPlacements[];
extern struct ActorPlacement gContigo_ShopsPlacementsBeforeReunion[];
extern struct ActorPlacement gContigo_ShopsPlacementsReunion[];
extern struct ActorPlacement gContigo_ShopsPlacements[];
extern struct ActorPlacement gContigo_LuckyDicePlacements[];
extern struct Trigger gContigo_VillageTriggers[];
extern struct Trigger gContigo_ShopsTriggers[];
extern struct Trigger gContigo_ShopsReunionTriggers[];

void Teleport_StartShimmer(void);

void Teleport_StopShimmer(void);

extern u32 gContigo_WheelsPrizes[];
/// The Lucky Wheels' icons that win prizes, shirts, boots, rings, then the
/// stars' and hearts' items, three prizes to each (gContigo_WheelsPrizes); the
/// Moon after them is wild, any of the five at random.
enum {
  WHEELS_PRIZE_ICONS = 5,
  WHEELS_ICON_MOON = WHEELS_PRIZE_ICONS,
  WHEELS_PRIZES_PER_ICON = 3
};
extern s32 gContigo_EnterSanctumDoorScript[];
extern u32 gContigo_RejoinLeaderScript[];
void Contigo_InitializeVillage(void);
void Contigo_InitializeGeoglyph(void);
void Contigo_InitializeShops(void);
void Contigo_InitializeLuckyDice(void);
extern u16 gTeleportCircle_PaletteSlots[];
extern u16 gTeleportCircle_Colors[];
extern u16 gTeleportCircle_ShimmerColors[];
extern u16 gTeleportCircle_ShimmerSlots[];
extern s32 gContigo_ShiverScript[];
extern u32 gLightRing_Tiles[];
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern u32 gContigo_DoubtingBoyScript[];
extern u32 gContigo_GirlOnTheShipScript[];
extern s32 gContigo_PuzzledWorkerScript[];

u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);

void Teleport_ArriveWhirling(void);

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

#include "shared/teleport_shimmer.c"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#include "text/msg_ids.h"

void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

/// Salt, popped up out of the gap in the weed patch east of the village gate,
/// joins the party: "Felix found the Venus Djinni Salt!"
void Contigo_SaltJoinsParty(void) {
  Djinn_JoinParty(ACTOR_SALT, DJINNI_SALT / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SALT));
}

/// Talking to Shine where it landed, beside the tree stump behind the inn,
/// once Force has freed it: the Mars Djinni joins the party.
void Contigo_FreedDjinniJoinsParty(void) {
  Djinn_JoinParty(ACTOR_SHINE, DJINNI_SHINE / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SHINE));
  GameFlag_Set(FLAG_SHINE_CAUGHT);
}

#include "shared/position_distance.h"

#include "shared/actor_is_facing_near.h"

void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 value);
s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// The update of the two children walking round the middle of the village:
/// each stands while it is talked to, while Mind Read shows its thoughts, or
/// while it faces the other child or the leader close by, and walks on
/// otherwise or once the leader is right by it.
s32 Contigo_PauseWhenFaced(struct Actor *self) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  u8 *state;
  u32 mode;
  struct FieldEffectState *effect;
  struct Actor *other;

  Actor_SetSpritePriorityDirect(
      self, BattleEffect_GetObjectPriority(
                Actor_GetById(gMainRoutineState.leaderActorId)));
  mode = ABILITY_ID(event->psynergy);
  state = &self->paused;
  if (mode == ABILITY_MIND_READ) {
    effect = Ram_HeapSlots->fieldEffectState;
    if ((s8)effect->messageShown != 0)
      goto stand;
  }
  if (self->riseEnabled == 1)
    goto stand;
  other = Actor_GetById(gMainRoutineState.leaderActorId);
  if (Position_Distance(&self->x.fixed, &other->x.fixed) <=
      CHILD_WALKS_ON_RANGE)
    goto walk;
  if (self->riseCounter == 1)
    other = Actor_GetById(ACTOR_GIRL_ON_THE_SHIP);
  else
    other = Actor_GetById(ACTOR_BOY_DOUBTING_THE_LEGENDS);
  if (Actor_IsFacingNear(self, other, CHILD_SEES_CHILD_RANGE) != 0)
    goto stand;
  if (Actor_IsFacingNear(self, Actor_GetById(gMainRoutineState.leaderActorId),
                         CHILD_SEES_LEADER_RANGE) != 0)
    goto stand;
walk:
  *state = 0;
  Actor_SelectAnimation(self, ANIM_WALK);
  return 0;
stand:
  *state = 1;
  Actor_SelectAnimation(self, ANIM_STAND);
  return 0;
}

/// A question mark over the worker wondering which part goes where, as he
/// looks from one part of the wings to another (gContigo_PuzzledWorkerScript).
s32 Contigo_ShowWorkerPuzzled(void) {
  FieldActor_ShowEmote(ACTOR_WING_WORKER_B, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gContigo_Placements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_CONTIGO)
    return gContigo_VillagePlacements;
  if (scene == SCENE_CONTIGO_GEOGLYPH)
    return gContigo_GeoglyphPlacements;
  if (scene == SCENE_CONTIGO_SHOPS) {
    if (gMainRoutineState.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH) {
      if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
        return gContigo_ShopsPlacements;
      if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 &&
          GameFlag_Get(FLAG_REUNION_AT_CONTIGO) == 0)
        return gContigo_ShopsPlacementsReunion;
    }
    return gContigo_ShopsPlacementsBeforeReunion;
  }
  if (scene == SCENE_CONTIGO_LUCKY_DICE)
    return gContigo_LuckyDicePlacements;
  return gContigo_Placements;
}

extern struct Trigger gContigo_Triggers[];
extern struct Trigger gContigo_GeoglyphTriggers[];
extern struct Trigger gContigo_LuckyDiceTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_CONTIGO)
    return gContigo_VillageTriggers;
  if (scene == SCENE_CONTIGO_GEOGLYPH)
    return gContigo_GeoglyphTriggers;
  if (scene == SCENE_CONTIGO_SHOPS) {
    if (gMainRoutineState.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH)
      return gContigo_ShopsReunionTriggers;
    return gContigo_ShopsTriggers;
  }
  if (scene == SCENE_CONTIGO_LUCKY_DICE)
    return gContigo_LuckyDiceTriggers;
  return gContigo_Triggers;
}

/// Felix examines the Sanctum's front wall between its west and middle steps,
/// behind the translation sign (cells (45, 18) and (46, 18)): "There are
/// characters carved into it", which he cannot make sense of.
void Contigo_ExamineWall(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix examined the wall. There are characters carved into it...."
  FieldMessage_Show((s32)MsgContigoFelixExaminedTheWallThere, 1);
  Event_End();
}

/// Felix reads the small sign in front of the Sanctum's carved wall: "The
/// characters from the wall are translated" on it.
void Contigo_ReadSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix examined the sign. The characters from the wall are translated..."
  FieldMessage_Show((s32)MsgContigoFelixExaminedTheSignThe, 1);
  Event_End();
}

/// Felix reads the sign outside the Lucky Wheels tent, left of the inn (cell
/// (4, 55)): "Turn the slot and get a prize! The lottery slots are over here."
void Contigo_ReadLuckyWheelsSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Turn the slot and get a prize! The lottery slots are over here."
  FieldMessage_Show((s32)MsgContigoTurnTheSlotAndGet, 1);
  Event_End();
}

/// Felix reads the sign west of the two tents north of the Teleport circle
/// (cell (11, 47)), which points to Super Lucky Dice in the west tent: "Drop
/// some cash and double up! The hi-lo dice are over here!"
void Contigo_ReadSuperLuckyDiceSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Drop some cash and double up! The hi-lo dice are over here!"
  FieldMessage_Show((s32)MsgContigoDropSomeCashAndDouble, 1);
  Event_End();
}

/// Felix reads the sign east of the two tents north of the Teleport circle
/// (cell (19, 47)), which points to Lucky Dice in the east tent: "You never
/// know what the dice will do next! Come and give our dice a toss!"
void Contigo_ReadLuckyDiceSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "You never know what the dice will do next! Come and give our dice a..."
  FieldMessage_Show((s32)MsgContigoYouNeverKnowWhatThe, 1);
  Event_End();
}

#include "shared/scene_show_entrance_locked.h"

/// Until Jupiter Lighthouse is lit, the young man a few steps inside the
/// village gate asks Felix whether he knows the legend of the people who could
/// control the wind and air, and answers his yes or no.
void Contigo_AskAboutWindLegend(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoDoYouKnowAnythingAbout);
  // "Do you know anything about a legend about these people who could..."
  Event_AskYesNoAndShowResponse(ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, 0);
  Event_End();
}

/// The update of a leaf in the baby's whirlwind: it circles on a random radius,
/// now and then pausing to spin faster, and is released after 101 frames.
void Contigo_WhirlLeaf(struct Actor *effect) {
  FIXED radius;
  s32 angle;
  FIXED dx;
  FIXED dz;

  // Random16 is 0 to 1 in 16.16, so 2 to 3 pixels.
  radius = Random16() + toFIXED(2);
  angle = effect->facing;
  dx = Iwram_MulFX(radius, Trig_Cos(angle));
  dz = Iwram_MulFX(radius, Trig_Sin(angle));
  effect->x.fixed += dx;
  effect->z.fixed += dz;
  // A small backwards drift, a 4096th of a turn a frame.
  effect->facing += -(TRIG_TURN / 4096);
  if (effect->turnFrames != 0) {
    effect->turnFrames--;
    effect->facing += TRIG_TURN / 32;
  } else if ((Random16() << 5) >> 16 == 0) {            // one chance in 32
    effect->turnFrames = ((Random16() << 4) >> 16) + 8; // 8 to 23 frames
  }
  if (++effect->swirlFrames == 101)
    Actor_Release(effect);
}

/// A task that spawns a leaf in the baby's whirlwind every eighth frame, flung
/// up to fall back as it circles.
void Contigo_RaiseLeaves(void) {
  if ((gFrameCount & 7) == 0) {
    struct Actor *actor =
        Actor_Spawn(SPRITE_LEAF, toFIXED(706), toFIXED(0.8), toFIXED(116));

    if (actor != NULL) {
      Actor_SetSpriteFlags(actor, 0);
      actor->update = Contigo_WhirlLeaf;
      actor->motionFlags = 0;
      actor->swirlFrames = -(s32)(Random_Below(16)) * 2 + 20;
      actor->turnFrames = 0;
      actor->gravity = toFIXED(1) / 10;
      actor->velocityY = toFIXED(3);
      actor->facing = Random16();
    }
  }
}

/// Talking to the baby in its cradle, in the house north of the weapon shop:
/// "The baby's forehead is furrowed, like it's thinking." The room turns dark
/// blue, a whirlwind grows over the cradle and leaves whirl up in it, then the
/// light comes back, the baby babbles "Babu bawaba bibaw.", and its brother
/// grumbles that the shabby old house is drafty.
void Contigo_BabyThinks(void) {
  struct Actor *effect;
  FIXED x = toFIXED(706);
  FIXED z = toFIXED(116);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
    // "The baby's forehead is furrowed, like it's thinking."
    FieldMessage_Show((s32)MsgContigoTheBabysForeheadIsFurrowedLike, 1);
  else
    // "The baby's forehead is furrowed, like it's thinking."
    FieldMessage_Show((s32)MsgContigoTheBabysForeheadIsFurrowed, 1);
  Actor_SetAnimationById(ACTOR_BABY, 3);
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(4, 4, 16), PALETTES_BG);
  PaletteFade_StartInterpolation(16);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_GetById(ACTOR_BABY), 2);
  effect = Import_ActorCreate(SPRITE_WHIRLWIND, x, 0, z);
  if (effect != 0) {
    Actor_SetSpriteFlags(effect, 0);
    Actor_SelectAnimation(effect, 2);
    effect->motionFlags = 0;
    effect->scaleX = FIXED_ONE / 10;
    effect->scaleY = FIXED_ONE / 10;
    for (i = 0; i < 28; i++) {
      effect->scaleX += FIXED_ONE / 40;
      effect->scaleY += FIXED_ONE / 40;
      wait(1);
    }
  }
  Task_Add(Contigo_RaiseLeaves, TASK_PASS_FRAME);
  Event_Wait(120);
  Task_Remove(Contigo_RaiseLeaves);
  if (effect != 0) {
    for (i = 0; i < 28; i++) {
      effect->scaleX -= FIXED_ONE / 40;
      effect->scaleY -= FIXED_ONE / 40;
      effect->y.fixed += toFIXED(1) / 2;
      wait(1);
    }
    Actor_Release(effect);
  }
  BattleEffect_SetObjectUpdateMode(Actor_GetById(ACTOR_BABY), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(16);
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
    // "Goo goo. Gaa gaa... Waaa!"
    Event_SetMessageId((s32)MsgContigoGooGooGaaGaaWaaa);
  else
    // "Babu bawaba bibaw."
    Event_SetMessageId((s32)MsgContigoBabuBawabaBibaw);
  Actor_RunRepeatedMotion(ACTOR_BABY, 2);
  Event_ShowMessageAndWait(ACTOR_BABY, 0);
  Actor_SetAnimationById(ACTOR_BABY, 1);
  FieldActor_ShowEmote(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS,
                       EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_FaceDirection(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, FACING_WEST, 20);
  // "It sure is drafty in this shabby old house."
  Event_ShowMessageAndWait(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_WOMAN_ON_THE_ANEMOS_POWER,
                                    FACING_EAST);
  // "When the Wings of Anemos are complete, let's fix the house!"
  Event_ShowMessageAndWait(ACTOR_WOMAN_ON_THE_ANEMOS_POWER, 0);
  Actor_SetAnimationById(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_WOMAN_ON_THE_ANEMOS_POWER, ANIM_NOD);
  Actor_FaceDirection(ACTOR_WOMAN_ON_THE_ANEMOS_POWER, FACING_NORTH_NORTHEAST,
                      0);
  Actor_FaceDirectionAndWait(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS,
                             FACING_SOUTH_SOUTHEAST);
  Event_End();
}

void Shop_Run(s32 stockRow, s32 keeperId);
void Sanctum_Run(s32 unitId);
void Inn_CheckIn(s32 innId, s32 actorId);

#include "shared/leader_is_facing_north.h"

/// Contigo's shops, as Shop_Run numbers them.
enum {
  SHOP_CONTIGO_WEAPONS = 26,
  SHOP_CONTIGO_ARMOR = 27,
  SHOP_CONTIGO_ITEMS = 28
};

/// Contigo's inn, as Inn_CheckIn numbers the inns.
enum { CONTIGO_INN = 12 };

/// Talking to the item merchant across her counter, in the west of the two
/// houses south of the Teleport circle, opens the item shop; from any other
/// side she talks of the wings.
void Contigo_TalkToItemMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_ITEMS, ACTOR_ITEM_MERCHANT);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      // "It's always such a letdown when you finish off a major task like..."
      Event_SetMessageId((s32)MsgContigoItsAlwaysSuchALetdown);
    else
      // "The villagers are working their hardest to complete the wings and..."
      Event_SetMessageId((s32)MsgContigoTheVillagersAreWorkingTheir);
    Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
    Event_End();
  }
}

/// Talking to the weapon merchant, the west of the two men behind the counter
/// in the shop east of the Teleport circle, across it opens his shop; from any
/// other side he talks of Atteka's traditions.
void Contigo_TalkToWeaponMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_WEAPONS, ACTOR_WEAPON_MERCHANT);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      // "A cannon? Yeah, I think I heard something about a town up in..."
      Event_SetMessageId((s32)MsgContigoACannonYeahIThink);
    else
      // "It was a longstanding tradition that people in Atteka would take a..."
      Event_SetMessageId((s32)MsgContigoItWasALongstandingTradition);
    Event_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
    Event_End();
  }
}

/// Talking to the armor merchant, the east of the two men behind the counter
/// in the shop east of the Teleport circle, across it opens his shop; from any
/// other side he talks of the mining in the ruins.
void Contigo_TalkToArmorMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_ARMOR, ACTOR_ARMOR_MERCHANT);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      // "Now that the wings are finished, I doubt anyone will be buying any..."
      Event_SetMessageId((s32)MsgContigoNowThatTheWingsAre);
    else
      // "Mining materials in the ruins is dangerous work, so my armor is..."
      Event_SetMessageId((s32)MsgContigoMiningMaterialsInTheRuins);
    Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
    Event_End();
  }
}

/// Talking to the innkeeper across the counter of the inn, right of the Lucky
/// Wheels tent, offers a night's rest; from any other side the innkeeper talks
/// of the visitors and the wings.
void Contigo_TalkToInnkeeper(void) {
  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(CONTIGO_INN, ACTOR_INNKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      // "All of our work and all of our celebration are finally at an end...."
      Event_SetMessageId((s32)MsgContigoAllOfOurWorkAnd);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT))
      // "You have to hurry! I think the ship from the geoglyphs is going to..."
      Event_SetMessageId((s32)MsgContigoYouHaveToHurryI);
    else
      // "We've had so many people coming to Contigo lately that we're full..."
      Event_SetMessageId((s32)MsgContigoWeveHadSoManyPeople);
    Event_ShowMessageAndWait(ACTOR_INNKEEPER, 0);
    Event_End();
  }
}

/// Talking to the healer across his counter, at the head of the red carpet in
/// the Sanctum north-east of the Teleport circle, offers the Sanctum's
/// healing; from any other side he talks of the miracle of the wings.
void Contigo_TalkToHealer(void) {
  if (Leader_IsFacingNorth()) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      // "It was a miracle that made all of the legends of the Anemos come..."
      Event_SetMessageId((s32)MsgContigoItWasAMiracleThat);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT))
      // "A flying ship... Surely, it must be a miracle. I'll have to study..."
      Event_SetMessageId((s32)MsgContigoAFlyingShipSurelyIt);
    else
      // "The real miracle will be if these wings actually make a boat fly...."
      Event_SetMessageId((s32)MsgContigoTheRealMiracleWillBe);
    Event_ShowMessageAndWait(ACTOR_HEALER, 0);
    Event_End();
  }
}

/// In the Super Lucky Dice tent, one of the players asks Felix whether he has
/// ever played Superdice before, and answers his yes or no.
void Contigo_AskAboutSuperDice(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoSoTellMePalHave);
  // "So, tell me, pal... Have you ever played Superdice before?"
  Event_AskYesNoAndShowResponse(ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, 0);
  Event_End();
}

/// Felix reads the rules sign in the Super Lucky Dice tent: four dice thrown at
/// once, what matching pairs and fours win, and the hi-lo game after a win.
void Contigo_ReadSuperLuckyDiceRules(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoSuperLuckyDiceTheRules;
  // "Super Lucky Dice The Rules of the Game."
  FieldMessage_Show(msg, 1);
  // "Throw all four dice onto the table in a single toss."
  FieldMessage_Show(msg + 1, 1);
  // "If two of the dice match, it's a push, and you win your money back."
  FieldMessage_Show(msg + 2, 1);
  // "If three of the dice match, you win back twice the bet you placed."
  FieldMessage_Show(msg + 3, 1);
  // "If you get two pairs of matching dice, you win three times the bet you..."
  FieldMessage_Show(msg + 4, 1);
  // "If all four dice match, it's a perfect toss! You win eight times your..."
  FieldMessage_Show(msg + 5, 1);
  // "If you get one pair, two pair, three of a kind, or a perfect four, you..."
  FieldMessage_Show(msg + 6, 1);
  // "You have to guess whether the next roll of the dice will be more or..."
  FieldMessage_Show(msg + 7, 1);
  // "If you win the hi-lo game, you double your winnings! You can do this..."
  FieldMessage_Show(msg + 8, 1);
  Event_End();
}

/// Felix reads the first of the two signs in the Lucky Dice tent: throw the
/// dice on the table and try to get the same numbers.
void Contigo_ReadLuckyDiceRules(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoLuckyDice;
  // "Lucky Dice"
  FieldMessage_Show(msg, 1);
  // "Throw the dice on the table and try to get the same numbers."
  FieldMessage_Show(msg + 1, 1);
  // "Prizes are based on the numbers rolled and where each die lands."
  FieldMessage_Show(msg + 2, 1);
  // "The more numbers that match, the more you win."
  FieldMessage_Show(msg + 3, 1);
  Event_End();
}

/// Felix reads the second sign in the Lucky Dice tent: what pairs, triples and
/// four of a kind win.
void Contigo_ReadLuckyDicePairs(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoLuckyDiceB;
  // "Lucky Dice"
  FieldMessage_Show(msg, 1);
  // "If two numbers match on either the dice or the table..."
  FieldMessage_Show(msg + 1, 1);
  // "it's called a pair, and you don't win or lose anything."
  FieldMessage_Show(msg + 2, 1);
  // "Two pair doubles your money. Triple matches triple your money."
  FieldMessage_Show(msg + 3, 1);
  // "Four of a kind is perfect, and you win five times your bet."
  FieldMessage_Show(msg + 4, 1);
  // "Plus, if your dice come up perfect on the Perfect Bonus spot on the..."
  FieldMessage_Show(msg + 5, 1);
  Event_End();
}

/// Talking to the young woman by the slot machine in the Lucky Wheels tent
/// offers a game for game tickets (Contigo_OfferLuckyWheels).
void Contigo_TalkToWheelsAttendant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Contigo_OfferLuckyWheels(0);
  Event_End();
}

/// Felix reads the rules sign in the Lucky Wheels tent: five pulls of the
/// lever, stars to hold a wheel, and more lines for more tickets.
void Contigo_ReadLuckyWheelsRules(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoLuckyWheelsRules;
  // "Lucky Wheels Rules"
  FieldMessage_Show(msg, 1);
  // "Pull the lever and try to line up matching icons to win big."
  FieldMessage_Show(msg + 1, 1);
  // "You have only 5 chances to pull the lever."
  FieldMessage_Show(msg + 2, 1);
  // "Press the star to keep a wheel from spinning and match other wheels."
  FieldMessage_Show(msg + 3, 1);
  // "You get 1 line for 1 game ticket, 3 lines for 2 tickets,"
  FieldMessage_Show(msg + 4, 1);
  // "5 lines for 3 tickets, and 7 lines for 4 tickets. Use all your tickets..."
  FieldMessage_Show(msg + 5, 1);
  Event_End();
}

/// Felix reads the prizes sign in the Lucky Wheels tent: shirts, rings and
/// boots win their kind, stars and hearts win items, and the Moon is wild.
void Contigo_ReadLuckyWheelsPrizes(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoLuckyWheelsPrizes;
  // "Lucky Wheels Prizes"
  FieldMessage_Show(msg, 1);
  // "Prizes are determined by which icons you match."
  FieldMessage_Show(msg + 1, 1);
  // "Shirts win undershirts, Rings win rings, Boots win boots,"
  FieldMessage_Show(msg + 2, 1);
  // "Stars and Hearts win you items..."
  FieldMessage_Show(msg + 3, 1);
  // The Spanish sign ends after four messages.
#if LANGUAGE != LANGUAGE_SPANISH
  // "And Moons are wild°they can win you anything."
  FieldMessage_Show(msg + 4, 1);
#endif
  Event_End();
}

/// On Felix's first visit, the man in a long robe between the Sanctum's middle
/// and east steps asks whether he has taken a good look at the Anemos
/// geoglyph, and answers his yes or no.
void Contigo_AskAboutGeoglyph(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoHaveYouTakenAGood);
  // "Have you taken a good look at the Anemos geoglyph?"
  Event_AskYesNoAndShowResponse(ACTOR_MAN_ON_THE_GEOGLYPH, 0);
  Event_End();
}

/// Talking to the boy walking about the middle of the village with a girl: he
/// talks of the geoglyphs and of the crater where the city of the Anemos is
/// said to have stood, and Mind Read hears that he will not believe it flew.
/// He turns to Felix to talk, but not for Mind Read, and turns back after.
void Contigo_TalkToDoubtingBoy(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_GetById(ACTOR_BOY_DOUBTING_THE_LEGENDS);
  s16 angle = actor->facing;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->riseEnabled = 1;
  if (state->psynergy == 0) {
    Actor_FaceActorById(ACTOR_BOY_DOUBTING_THE_LEGENDS,
                        gMainRoutineState.leaderActorId, 0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      // "I've seen a lot of weird things lately... Maybe I ought to listen..."
      Event_SetMessageId((s32)MsgContigoIveSeenALotOf);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0)
      // "That crater behind the village is supposed to be where the city of..."
      Event_SetMessageId((s32)MsgContigoThatCraterBehindTheVillage);
    else
      // "If you want to get a look at the Anemos geoglyphs, you ought to go..."
      Event_SetMessageId((s32)MsgContigoIfYouWantToGet);
  } else {
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      // "Wait... The city of Anemos flew up into the sky? All right, that's..."
      Event_SetMessageId((s32)MsgContigoWaitTheCityOfAnemos);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0)
      // "Cities flying up into the sky? I don't buy that for a second!"
      Event_SetMessageId((s32)MsgContigoCitiesFlyingUpIntoThe);
    else
      // "The Anemos geoglyphs are the most amazing carvings I've ever seen..."
      Event_SetMessageId((s32)MsgContigoTheAnemosGeoglyphsAreThe);
  }
  Event_ShowMessageAndWait(ACTOR_BOY_DOUBTING_THE_LEGENDS, 0);
  if (state->psynergy == 0)
    actor->facing = angle;
  actor->riseEnabled = 0;
  Event_End();
}

/// Talking to the girl walking about the middle of the village with the boy:
/// she talks of the wings the village built and, once they are gone, of seeing
/// the ship take off; Mind Read hears her thoughts on the same. She turns to
/// Felix to talk, but not for Mind Read, and turns back after.
void Contigo_TalkAboutTheShip(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_GetById(ACTOR_GIRL_ON_THE_SHIP);
  s16 angle = actor->facing;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->riseEnabled = 1;
  if (event->psynergy == 0) {
    Actor_FaceActorById(ACTOR_GIRL_ON_THE_SHIP, gMainRoutineState.leaderActorId,
                        0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      // "It wasn't what I expected, but seeing the ship take off was still..."
      Event_SetMessageId((s32)MsgContigoItWasntWhatIExpected);
    else
      // "Our people have labored for some time to build the Wings of the..."
      Event_SetMessageId((s32)MsgContigoOurPeopleHaveLaboredFor);
  } else {
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      // "All of those legends did nothing but build up people's..."
      Event_SetMessageId((s32)MsgContigoAllOfThoseLegendsDid);
    else
      // "We built the Wings of Anemos out of materials we found in the..."
      Event_SetMessageId((s32)MsgContigoWeBuiltTheWingsOf);
  }
  Event_ShowMessageAndWait(ACTOR_GIRL_ON_THE_SHIP, 0);
  if (event->psynergy == 0)
    actor->facing = angle;
  actor->riseEnabled = 0;
  Event_End();
}

void Teleport_PlaySparks(void);

/// Teleport cast on the carved stone circle in the middle of the village
/// (cells (16, 50) and (17, 50)): the village turns grey, the circle shimmers
/// and sparks of coloured light rise, and the party arrives on a circle like
/// it in the Anemos Inner Sanctum.
void Contigo_TeleportOut(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();

  gMainRoutineState.teleportState = 2;
  Event_SetDestination(SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST,
                       ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_FROM_CONTIGO);
  Event_Wait(40);
  Teleport_StopShimmer();
}

/// Cyclone scatters the clump of weeds just west of the item shop, the west of
/// the two houses south of the Teleport circle (cell (8, 56)), leaving bare
/// ground. Each clump's event copies bare ground from a cell nearby over the
/// clump's cells that Cyclone's whirl touched. Where the ground is raised, as
/// here, a clump is drawn some rows north of the cell it answers on.
void Contigo_ScatterWeedsWestOfItemShop(void) {
  FieldTileCopies_CopyTouchedCells(3, 50, 1, 1, 8, 54, 0);
}

/// Cyclone scatters the clump of weeds behind the inn, beside the tree stump
/// where Shine is stuck (cells (2, 45) and (3, 45)), leaving bare ground.
void Contigo_ScatterWeedsBehindInn(void) {
  FieldTileCopies_CopyTouchedCells(4, 43, 2, 1, 2, 43, 0);
}

/// The scene's own name for TileCopies_CopyTouchedCells, which the Bramble
/// Seed's clump calls.
static __inline__ void Contigo_CopyTouchedCells(s32 sourceX, s32 sourceZ,
                                                s32 width, s32 height,
                                                s32 windowX, s32 windowZ,
                                                s32 layer) {
  TileCopies_CopyTouchedCells(sourceX, sourceZ, width, height, windowX, windowZ,
                              layer);
}

/// Cyclone scatters the clump of weeds beside the east wall of the house at
/// the top of the hill (cell (17, 41)), and unless it was taken the Bramble
/// Seed lies uncovered there. If Felix stands on that very spot, he hops a
/// step north off it.
void Contigo_ScatterWeedsOverBrambleSeed(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  Contigo_CopyTouchedCells(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Get(FLAG_CONTIGO_BRAMBLE_SEED_TAKEN) == 0) {
    Actor_SetPosition(ACTOR_BRAMBLE_SEED, 280, 664);
    if (FIELD_CELL(leader->x.fixed) == 17 &&
        FIELD_CELL(leader->z.fixed) == 41) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineState.leaderActorId, 0, -16);
      leader->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone scatters the clump of weeds against the west wall of the weapon and
/// armor shop, east of the Teleport circle (cells (23, 49) and (23, 50)),
/// leaving bare ground.
void Contigo_ScatterWeedsWestOfWeaponShop(void) {
  FieldTileCopies_CopyTouchedCells(29, 35, 1, 2, 23, 47, 0);
}

/// Cyclone scatters the weeds just east of the village gate's east post (cells
/// (29, 59) and (30, 58) to (30, 60)), leaving bare ground.
void Contigo_ScatterWeedsByGate(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 2, 3, 29, 58, 0);
}

/// Cyclone scatters the clump of weeds below the Lucky Wheels tent, at the
/// edge of the slope (cell (3, 58)), leaving bare ground.
void Contigo_ScatterWeedsBelowLuckyWheels(void) {
  FieldTileCopies_CopyTouchedCells(3, 60, 1, 1, 3, 56, 0);
}

/// Cyclone scatters the western of the two clumps of weeds on top of the cliff
/// south of the item shop (cells (7, 60) and (8, 60)), leaving bare ground.
void Contigo_ScatterWeedsOnCliffWest(void) {
  FieldTileCopies_CopyTouchedCells(7, 62, 2, 1, 7, 58, 0);
}

/// Cyclone scatters the eastern of the two clumps of weeds on top of the cliff
/// south of the item shop (cells (10, 61) and (11, 61)), leaving bare ground.
void Contigo_ScatterWeedsOnCliffEast(void) {
  FieldTileCopies_CopyTouchedCells(10, 63, 2, 1, 10, 59, 0);
}

/// Cyclone scatters the top row of the big weed patch east of the village gate
/// (cells (40, 57) to (42, 57)), leaving bare ground. The patch's five rows
/// each have an event, and they and the weeds by the gate take their bare
/// ground from a cell between the gate and the patch.
void Contigo_ScatterWeedPatchTopRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 3, 1, 40, 57, 0);
}

/// Cyclone scatters the second row from the top of the big weed patch east of
/// the village gate (cells (39, 58) to (43, 58)), leaving bare ground.
void Contigo_ScatterWeedPatchSecondRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 58, 0);
}

/// Cyclone scatters the middle row of the big weed patch east of the village
/// gate, either side of the open gap where Salt hides (cells (39, 59),
/// (40, 59), (42, 59) and (43, 59)), leaving bare ground.
void Contigo_ScatterWeedPatchMiddleRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 59, 0);
}

/// Cyclone scatters the second row from the bottom of the big weed patch east
/// of the village gate (cells (39, 60) to (43, 60)), leaving bare ground.
void Contigo_ScatterWeedPatchFourthRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 60, 0);
}

/// Cyclone scatters the bottom row of the big weed patch east of the village
/// gate (cells (40, 61) to (42, 61)), leaving bare ground.
void Contigo_ScatterWeedPatchBottomRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 3, 1, 40, 61, 0);
}

/// Nothing visible happens when Felix walks south from the middle of the
/// village into its south part, on one of the three ways in: west of the item
/// shop, between it and the house beside it, or east of that house (cells
/// (5, 55) to (8, 55), (14, 55) and (18, 57) to (20, 57)). From then on the
/// view follows him east past the village gate to the big weed patch, where
/// it stopped just past the gate before. FLAG_WHOLE_VILLAGE_IN_VIEW marks the
/// wider view.
void Contigo_WidenCameraBounds(void) {
  Field_SetCameraBounds(0, toFIXED(512), toFIXED(880), toFIXED(1024));
  GameFlag_Set(FLAG_WHOLE_VILLAGE_IN_VIEW);
}

/// Nothing visible happens when Felix walks back north from the village's
/// south part into its middle, on one of the three ways (cells (4, 54) to
/// (8, 54), (14, 54) and (18, 56) to (20, 56)); the view stops at the village
/// gate again, and FLAG_WHOLE_VILLAGE_IN_VIEW is cleared.
void Contigo_NarrowCameraBounds(void) {
  Field_SetCameraBounds(0, toFIXED(512), toFIXED(512), toFIXED(1024));
  GameFlag_Clear(FLAG_WHOLE_VILLAGE_IN_VIEW);
}

/// The trigger Contigo_ScoopForSalt raises once Salt has popped up, which runs
/// Contigo_SaltJoinsParty.
enum ContigoRaisedTrigger { RAISED_SALT_FOUND = 200 };

/// Scoop cast in the open gap in the middle of the big weed patch east of the
/// village gate (cell (41, 59)): until the party has Salt, the Venus Djinni
/// pops up there, turns to Felix and joins the party.
void Contigo_ScoopForSalt(void) {
  if (!GameFlag_Get(FLAG_DJINNI(DJINNI_SALT))) {
    struct FieldState *state = Ram_HeapSlots->fieldState;

    Event_Start();
    Event_ResolvePendingActions(1);
    Actor_SetPosition(ACTOR_SALT, 664, 952);
    wait(1);
    Actor_FaceActorById(ACTOR_SALT, gMainRoutineState.leaderActorId, 0);
    state->raisedTrigger = RAISED_SALT_FOUND;
    Event_End();
  }
}

/// Felix casts Reveal anywhere at the geoglyph or the Sanctum's front: the
/// carved wall at the head of the Sanctum's main steps shows as a dark open
/// doorway, its middle door, and while Reveal lasts walking north into it
/// (cells (48, 19) and (49, 19)) leads inside, on the way to the Dragon Skin's
/// chest. The open door is the picture the unused
/// Contigo_EnterSanctumMiddleDoor would open it with; the doorway's exit cells
/// are kept out of reach at the top of the map, (48, 1), and copied into place.
void Contigo_RevealMiddleDoor(void) {
  MapGrid_CopyCellsTo(27, 4, 48, 12, 1, 2);
  MapGrid_CopyCellTilesIndicesRect(27, 6, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 1, 2, 1, 48, 19);
}

/// As Reveal fades in front of the Sanctum, its middle door shows shut again
/// and walking into it leads nowhere.
void Contigo_HideMiddleDoor(void) {
  MapGrid_CopyCellsTo(27, 8, 48, 12, 1, 2);
  MapGrid_CopyCellTilesIndicesRect(27, 10, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 0, 2, 1, 48, 19);
}

/// Force cast on the tree stump behind the inn, where Shine is stuck: the
/// ground shakes with a thump, a ring of dust bursts out of the stump, and
/// Shine leaps out and lands just north-west of it, Felix hopping aside if it
/// lands on him. Force comes only with the Orb of Force, carried over from The
/// Broken Seal in a linked game.
void Contigo_ForceFreesDjinni(void) {
  struct Actor *actor = Actor_GetById(ACTOR_SHINE);
  struct Actor *leader;
  struct EffectOptions options;
  FIXED velocity[3];
  s32 angle;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(ACTOR_SHINE, 56, 698);
  wait(1);
  options.priority = 2;
  Sound_Play(SE_THUMP);
  MapGrid_SetShake(1, 2, 1);
  for (i = 0; i <= DUST_RING_PUFFS; i++) {
    angle = i * (TRIG_TURN / DUST_RING_PUFFS);
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] / 3;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      velocity[0], velocity[1] + (toFIXED(1) / 10), velocity[2],
                      EFFECT_USE_PRIORITY, &options);
  }
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SHINE), SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHINE), 1);
  Actor_SetSpeed(ACTOR_SHINE, 3);
  actor->velocityY = toFIXED(12);
  Actor_MoveToAndWait(ACTOR_SHINE, 40, 680);
  Event_Wait(10);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (FIELD_CELL(actor->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(actor->z.fixed) == FIELD_CELL(leader->z.fixed)) {
    Actor_Jump(gMainRoutineState.leaderActorId, 6, 0);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 2);
    Actor_MoveToAndWait(gMainRoutineState.leaderActorId, 68, 680);
  }
  MapGrid_SetShake(0, 0, 0);
  Actor_TurnToFaceEachOther(gMainRoutineState.leaderActorId, ACTOR_SHINE, 0);
  Event_Wait(10);
  GameFlag_Set(FLAG_SHINE_FREED);
  Event_End();
}

/// Once Jupiter Lighthouse's beacon is lit, while the villagers are away
/// transporting the wings: the doors of the item shop, the house beside it,
/// the weapon and armor shop and the baby's house north of it show shut, and
/// trying one says it is locked (Scene_ShowEntranceLocked).
void Contigo_LockDoors(void) {
  MapGrid_CopyCellsTo(0, 1, 26, 42, 1, 2);
  MapGrid_CopyCellTilesIndicesRect(0, 1, 26, 48, 1, 2);
  MapGrid_CopyCellTilesIndicesRect(0, 1, 11, 54, 1, 2);
  MapGrid_CopyCellTilesIndicesRect(0, 1, 16, 54, 1, 2);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 26, 45);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 26, 51);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 11, 57);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 16, 57);
}

/// Lays a frame or feather of the wings flat on the ground, drawn behind.
void Contigo_PlaceWingPart(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  actor->collisionFlags = ACTOR_NOT_TARGETED;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(64);
}

/// Lays out the wings' parts by the geoglyph: each frame and feather shows
/// its own piece and lies flat, and the loose parts block the way.
void Contigo_SetUpWingParts(void) {
  Actor_SetAnimationById(ACTOR_WING_FRAME_A, 0);
  Actor_SetAnimationById(ACTOR_WING_FRAME_B, 1);
  Actor_SetAnimationById(ACTOR_WING_FRAME_C, 2);
  Actor_SetAnimationById(ACTOR_METAL_FEATHER_A, 0);
  Actor_SetAnimationById(ACTOR_METAL_FEATHER_B, 1);
  Actor_SetAnimationById(ACTOR_METAL_FEATHER_C, 2);
  Actor_SetAnimationById(ACTOR_METAL_FEATHER_D, 3);
  Actor_SetAnimationById(ACTOR_METAL_FEATHER_E, 4);
  Contigo_PlaceWingPart(ACTOR_WING_FRAME_A);
  Contigo_PlaceWingPart(ACTOR_WING_FRAME_B);
  Contigo_PlaceWingPart(ACTOR_WING_FRAME_C);
  Contigo_PlaceWingPart(ACTOR_METAL_FEATHER_A);
  Contigo_PlaceWingPart(ACTOR_METAL_FEATHER_B);
  Contigo_PlaceWingPart(ACTOR_METAL_FEATHER_C);
  Contigo_PlaceWingPart(ACTOR_METAL_FEATHER_D);
  Contigo_PlaceWingPart(ACTOR_METAL_FEATHER_E);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LOOSE_PART_A), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LOOSE_PART_B), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LOOSE_PART_C), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LOOSE_PART_D), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LOOSE_PART_E), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LOOSE_PART_F), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LOOSE_PART_G), 0);
  Actor_GetById(ACTOR_LOOSE_PART_A)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_LOOSE_PART_B)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_LOOSE_PART_C)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_LOOSE_PART_D)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_LOOSE_PART_E)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_LOOSE_PART_F)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_LOOSE_PART_G)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_SetAnimationById(ACTOR_LOOSE_PART_A, 0);
  Actor_SetAnimationById(ACTOR_LOOSE_PART_F, 0);
  Actor_SetAnimationById(ACTOR_LOOSE_PART_G, 0);
}

extern void MessageArgument_Push(s32 value, s32 kind);
void Event_LeaveFor(s32 scene, s32 entrance);

/// The Lucky Wheels attendant asks whether Felix will use his game tickets,
/// warns when the party has little room for prizes, and sends it to the slot
/// machine; `spoken` is nonzero as the party comes back from a game, when she
/// asks only whether to keep playing.
void Contigo_OfferLuckyWheels(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(ITEM_GAME_TICKET);
  s32 room;

  ObjectTable_Snapshot();
  if (spoken == 0) {
    s32 message = (s32)MsgContigoStepRightUpAndTry;

    Event_SetMessageId(message);
    // "Step right up and try your hand at lucky wheels!"
    Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    if (tickets == 0) {
      // "You gotta have game tickets to play."
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      return;
    }
    Event_SetMessageId(message + 2);
    MessageArgument_Push(tickets, MESSAGE_ARGUMENT_NUMBER);
    // "You have ... game ticket, Felix. Would you like to use these tickets?"
    FieldMessage_ShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        1) {
      // "The rules are listed on the sign. Take a look, take a look."
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      return;
    }
    room = PartyInventory_CountFreeSlots();
    if (room == 0) {
      Event_SetMessageId(message + 4);
      // "You can't carry any more items. Are you sure you want to play?"
      FieldMessage_ShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    } else if (room <= 6) {
      Event_SetMessageId(message + 5);
      // "If you win anything, you might not be able to take it with you..."
      FieldMessage_ShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    }
    if (room <= 6 && Event_AskYesNoWithActorAnimation(
                         gMainRoutineState.leaderActorId, 0) == 1) {
      Event_SetMessageId((s32)MsgContigoThenGetRidOfSome);
      // "Then get rid of some items and come again."
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      return;
    }
  } else {
    if (tickets == 0) {
      Event_SetMessageId((s32)MsgContigoTooBadComeBackAfter);
      // "Too bad... Come back after you get some tickets."
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      return;
    }
    Event_SetMessageId((s32)MsgContigoTooBadWouldYouLike);
    // "Too bad... Would you like to use your tickets to keep playing?"
    FieldMessage_ShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Event_SetMessageId((s32)MsgContigoComeAgain);
      // "Come again!"
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      return;
    }
  }
  Event_SetMessageId((s32)MsgContigoStepRightUpAndEnjoy);
  // "Step right up and enjoy yourself."
  Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  GameFlag_Set(FLAG_LEFT_FOR_A_GAME);
  wait(1);
  Event_LeaveFor(SEQUENCE_SLOTS, 0);
  Event_SetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE,
                                  ENTRANCE_FROM_LUCKY_WHEELS);
}

/// The prize for an icon matched on the Lucky Wheels: one of the icon's three,
/// never the one it gave last; the Moon picks an icon at random, and a negative
/// icon gives nothing.
s32 Contigo_SelectLuckyWheelsPrize(s32 icon) {
  struct MainRoutineState *state;
  s32 cursor;
  s32 variant;
  s32 index;

  if (icon < 0)
    return 0;
  if (icon == WHEELS_ICON_MOON)
    icon = (s32)(Random_Below(WHEELS_PRIZE_ICONS));
  state = &gMainRoutineState;
  cursor = state->wheelsPrizeCursors[icon];
  variant = (cursor + (s32)(Random_Below(2)) + 4) % WHEELS_PRIZES_PER_ICON;
  state->wheelsPrizeCursors[icon] = variant;
  index = icon * 2;

  return gContigo_WheelsPrizes[index + icon + variant];
}

/// The scene's own state in EWRAM (ewram.s), which each scene lays out its own
/// way.
extern u8 gSceneState[];

/// The dice games cost 10 coins for each level of the party's average level;
/// a win of 5,000 coins or more, or of 20,000, is greeted with a bigger
/// jingle.
enum {
  LUCKY_DICE_COINS_PER_LEVEL = 10,
  LUCKY_DICE_MEDIUM_WIN = 5000,
  LUCKY_DICE_LARGE_WIN = 20000
};

/// Talking to the young man running the Lucky Dice tent offers a game,
/// at 10 coins for each level of the party's average level: "Hey, buddy,
/// wanna try yer luck at the dice?"
void Contigo_OfferLuckyDice(void) {
  u32 price = Party_GetAverageLevel() * LUCKY_DICE_COINS_PER_LEVEL;

  Event_Start();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineState.coins < price) {
    Event_SetMessageId((s32)MsgContigoHeyYaDontHaveEnough);
    // "Hey! Ya don't have enough coins! Ya gotta pay to play."
    FieldMessage_ShowAtActor(ACTOR_LUCKY_DICE_ATTENDANT, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigoHeyBuddyWannaTryYer);
  MessageArgument_Push(price, MESSAGE_ARGUMENT_NUMBER);
  // "Hey, buddy, wanna try yer luck at the dice? It's coin to play." Yes: "OK!
  // Then take the dice." No: "No? Oh, well. Who's next?"
  FieldMessage_ShowAtActor(ACTOR_LUCKY_DICE_ATTENDANT, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    *(s32 *)gSceneState = gMainRoutineState.coins;
    /* FAKEMATCH(dead): tests of the coins, never above COIN_MAX, around a
     * store to price, which nothing reads again. */
    if (gMainRoutineState.coins > COIN_MAX) {
      if (gMainRoutineState.coins != 0)
        price = 0;
    }
    ObjectTable_Snapshot();
    // "OK! Then take the dice."
    Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
    GameFlag_Set(FLAG_LEFT_FOR_A_GAME);
    wait(1);
    Event_LeaveFor(SEQUENCE_DICE, 0);
    Event_SetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE,
                                    ENTRANCE_FROM_LUCKY_DICE);
  } else {
    gFieldState->message++;
    // "No? Oh, well. Who's next?"
    Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
  }
  Event_End();
}

/// Talking to the young man running the Super Lucky Dice tent offers a game at
/// the same price as Lucky Dice, which his offer does not say:
/// "Welcome, welcome, step right up! Care for a round of Super Lucky Dice?"
void Contigo_OfferSuperLuckyDice(void) {
  u32 price = Party_GetAverageLevel() * LUCKY_DICE_COINS_PER_LEVEL;

  Event_Start();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineState.coins < price) {
    // Shown at the Lucky Dice attendant, who stands in the next tent: the box
    // opens at the top of the screen as his welcome does, a little further
    // right.
    Event_SetMessageId((s32)MsgContigoSorryPalButYouveGotta);
    // "Sorry, pal, but you've gotta have more coins to play."
    FieldMessage_ShowAtActor(ACTOR_LUCKY_DICE_ATTENDANT, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigoWelcomeWelcomeStepRightUp);
  // "Welcome, welcome, step right up! Care for a round of Super Lucky Dice?"
  // Yes: "OK, then let 'em roll!" No: "Oh really? That's a shame!"
  FieldMessage_ShowAtActor(ACTOR_SUPER_LUCKY_DICE_ATTENDANT, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    *(s32 *)gSceneState = gMainRoutineState.coins;
    /* FAKEMATCH(dead): tests of the coins, never above COIN_MAX, around a
     * store to price, which nothing reads again. */
    if (gMainRoutineState.coins > COIN_MAX) {
      if (gMainRoutineState.coins != 0)
        price = 0;
    }
    ObjectTable_Snapshot();
    // "OK, then let 'em roll!"
    Event_ShowMessageAndWait(ACTOR_SUPER_LUCKY_DICE_ATTENDANT, 0);
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
    GameFlag_Set(FLAG_LEFT_FOR_A_GAME);
    wait(1);
    Event_LeaveFor(SEQUENCE_SUPER_DICE, 0);
    Event_SetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE,
                                    ENTRANCE_FROM_SUPER_LUCKY_DICE);
  } else {
    gFieldState->message++;
    // "Oh really? That's a shame!"
    Event_ShowMessageAndWait(ACTOR_SUPER_LUCKY_DICE_ATTENDANT, 0);
  }
  Event_End();
}

/// Draws an actor at seven tenths of its size, as the geoglyph's and the dice
/// hall's signs are drawn.
void Contigo_ShrinkActor(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  actor->scaleX = FIXED_ONE * 7 / 10;
  actor->scaleY = FIXED_ONE * 7 / 10;
  actor->radius = 8;
}

/// A thump, and the door at the head of the Sanctum's steps shows open; Felix
/// walks slowly in through it and the screen fades out.
void Contigo_OpenSanctumDoor(s32 sourceX, s32 sourceY, s32 doorX, s32 doorY) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_THUMP);
  {
    s32 width = 1;
    s32 height = 2;

    MapGrid_CopyCellTilesIndicesRect(sourceX, sourceY, doorX, doorY, width,
                                     height);
    MapGrid_CopyCellTilesIndicesRect(sourceX, sourceY + 2, doorX + 1, doorY,
                                     width, height);
  }
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_SetMovementTypeById(gMainRoutineState.leaderActorId,
                            gContigo_EnterSanctumDoorScript);
  wait(12);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Felix walks up the Sanctum's west steps into its door (cells (42, 20) and
/// (43, 20)): it opens with a thump, he walks in, and he arrives in a small
/// stone room of the Sanctum with dragon statues.
void Contigo_EnterSanctumWestDoor(void) {
  Contigo_OpenSanctumDoor(27, 0, 42, 13);
  Import_EventSetExitRequest(
      CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTH);
}

/// Unused: the same for the Sanctum's middle door, which Reveal opens as a
/// plain exit instead (Contigo_RevealMiddleDoor).
UNUSED void Contigo_EnterSanctumMiddleDoor(void) {
  Contigo_OpenSanctumDoor(27, 4, 48, 12);
  Import_EventSetExitRequest(
      CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTHEAST);
}

/// Felix walks up the Sanctum's east steps into its door (cells (54, 20) and
/// (55, 20)): it opens with a thump, he walks in, and he arrives in a small
/// stone room of the Sanctum with dragon statues.
void Contigo_EnterSanctumEastDoor(void) {
  Contigo_OpenSanctumDoor(27, 0, 54, 13);
  Import_EventSetExitRequest(
      CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_SOUTH);
}

extern u32 gContigo_JennaReunionScript[];
extern u32 gContigo_PiersReunionScript[];
extern u32 gContigo_ShebaReunionScript[];
extern u32 gContigo_KradenReunionScript[];
extern u32 gContigo_JoinLeaderScript[];
struct SparkleState gContigo_SparkleState BSS_DATA ALIGNED(16);
void Contigo_StartSparkle(struct Actor *actor, struct SparkleState *state);
void Contigo_StopSparkle(struct Actor *actor);

/// Walking into the house at the top of the hill once Jupiter Lighthouse's
/// beacon is lit: Isaac, Garet, Mia and Ivan are waiting, and the reunion
/// begins (Contigo_InitializeShops). The two parties make peace, Kraden tells
/// why Felix has been lighting the beacons, and Hamma comes to send them all
/// to Mars Lighthouse as one party.
void Contigo_TheReunion(void) {
  struct Actor *actor;
  s32 answer;
  s32 angle;

  actor = Actor_GetById(ACTOR_HAMMA);
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WatchAndSetScript(ACTOR_ISAAC, gMainRoutineState.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_GARET, gMainRoutineState.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_MIA, gMainRoutineState.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_IVAN, gMainRoutineState.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_HAMMA, gMainRoutineState.leaderActorId);
  Event_Wait(10);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 160, 382);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetMovementTypeById(ACTOR_JENNA, gContigo_JennaReunionScript);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gContigo_KradenReunionScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gContigo_ShebaReunionScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS, gContigo_PiersReunionScript);
  Actor_Stop(gMainRoutineState.leaderActorId);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  Actor_Stop(ACTOR_HAMMA);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Sound_Play(MUS_JOURNEY_TO_SAVE_THE_WORLD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Event_SetMessageId((s32)MsgGladYouKeptYourPromise);
  // "I'm glad you kept your promise, Felix."
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  // "All right, let's hear what he has to say for himself!"
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "What can we say, Isaac? Has Felix ever harmed you? Have we ever..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  // "Felix's been avoiding you, but he never had any intention of..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  // "And he definitely helped us out back at Jupiter Lighthouse."
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_SetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  // "Isaac's been worrying about Jenna nonstop since this nightmare..."
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "She was afraid that if we met, we would be forced to fight... She..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "I know that, now... But it doesn't explain why you're doing this..."
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Felix betrayed his hometown, Vale... That's why he hasn't been able..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(148, CAMERA_KEEP, 368, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 172, 342);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 146, 342);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Felix had hoped to play the villain alone, without getting Jenna or..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_Jump(ACTOR_GARET, 2, 10);
  // "He betrayed Vale? Kraden, what are you talking about?"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_GARET, 0);
  // "He conspired to steal the Elemental Stars and fire the beacons of..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "That's what Saturos and Menardi were trying to do..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_MIA, 0);
  // "Why were you helping them?"
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHWEST);
  // "Our parents' lives were at stake! We had to help!"
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Parents? But...your parents died three years ago, in that storm..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "That night, Saturos and his men had raided Sol Sanctum... The storm..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "Garet, you saw two strangers that night°Saturos and Menardi. They..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "They had failed to solve the mystery of Sol Sanctum. In doing so..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  // "Everyone thought that you were killed by that boulder, Felix. I..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "In fact, nobody was killed by the boulder that day!"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  // "So that means..."
  Event_ShowMessageThenWait(ACTOR_ISAAC, 0, 40);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Wait a minute, Kraden... Did you just say that nobody was killed by..."
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_SOUTH);
  // "But what about Isaac's dad? And Jenna and Felix's parents? I thought..."
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTH);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "I'm not so sure now..."
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_FaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Yes! If Felix survived, Kyle and the others may have as well..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirectionAndWait(ACTOR_MIA, FACING_SOUTHEAST);
  // "Why didn't you tell us? We could have worked together to save your..."
  FieldMessage_ShowAtActor(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  if (answer == 0) {
    Actor_WaitForAnimationChange(gMainRoutineState.leaderActorId);
    Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
    // "Except that we had only just begun our journey, and we simply..."
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    gFieldState->message++;
  } else {
    Actor_WaitForAnimationChange(gMainRoutineState.leaderActorId);
    gFieldState->message++;
    Actor_SetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    // "He's right. They were too powerful then... It would have been..."
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "There's more to it than that. He had another reason to light the..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Unless the lighthouse beacons are all lit, Weyard will eventually be..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 20);
  // "Destroyed? But...how? Why?"
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "King Hydros, ruler of Lemuria, says that our world is steadily shrinking."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_IVAN, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  // "Elemental energy drives the growth of civilization. Without it, we and..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 20);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "All that because the lighthouses aren't illuminated?"
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "According to my research, that seems to be the case."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "But...once the lighthouses are all lit, you said the world would end..."
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "That...might be true, too... But if we do nothing, the world will..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 80);
  // "Wait for the end of the world to come or wind up accidentally..."
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Nothing is certain. There is no way to prevent the world from reaching..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  // "However, we can fight to save the world from withering away due to the..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHEAST);
  // "And, Felix, you knew this? You were helping them because you knew what..."
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  if (answer == 0) {
    Actor_WaitForAnimationChange(gMainRoutineState.leaderActorId);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "You're a lot smarter than I remember you being..."
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    gFieldState->message++;
  } else {
    Actor_WaitForAnimationChange(gMainRoutineState.leaderActorId);
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    gFieldState->message++;
    // "You just figured it out along the way, huh? That's still pretty..."
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Why did you wait so long to tell me? I would have helped you..."
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "You wouldn't have done it before, Isaac... It would have meant..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Sound_Play(MUS_SILENCE);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HAMMA), SPRITE_COLOR_HIDDEN);
  angle = FACING_NORTH_NORTHWEST;
  Actor_SetHorizontalPositionAndAngleWithTerrain(ACTOR_HAMMA, toFIXED(160),
                                                 toFIXED(466), angle);
  wait(1);
  // "Now that we know all of this, shouldn't we be going?"
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_WatchAndSetScript(gMainRoutineState.leaderActorId, ACTOR_HAMMA);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_HAMMA);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_HAMMA);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_HAMMA);
  Actor_WatchAndSetScript(ACTOR_ISAAC, ACTOR_HAMMA);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_HAMMA);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_HAMMA);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_HAMMA);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HAMMA), SPRITE_COLOR_NORMAL);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(180, CAMERA_KEEP, 410, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(148, CAMERA_KEEP, 368, 1);
  Actor_SetSpeed(ACTOR_HAMMA, 0.8);
  Actor_WalkToAndWait(ACTOR_HAMMA, 146, 406);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "Hamma!"
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_Stop(gMainRoutineState.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_ISAAC);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  Sound_Play(MUS_REUNITING_WITH_HAMA);
  Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
  // "Hello again..."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Hamma! You...know Isaac? And his companions, too?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Event_Wait(10);
  // Isaac: "We met at the temple on the edge of the Lamakan Desert...", and
  // Hamma at once: "I'm Hamma, descendant of the Anemos."
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_ISAAC, 5, 5, 8, 10, ACTOR_HAMMA, 9,
                                    20, 18, REUNION_HAMMA_PORTRAIT_Y, 10);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
  FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_NOD);
  // "I was born in Contigo, and I inherited the power of the Anemos."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "Hey... We just got into Contigo, and we found out this is where Ivan..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  // "Yeah... We also learned that he's got a sister!"
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  // "Sister... You don't mean..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_HAMMA, FACING_EAST);
  // "That would be me, yes."
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_WalkToAndWait(ACTOR_IVAN, 130, 400);
  Actor_FaceDirectionById(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_WEST);
  // "Not now, Ivan... This isn't the time."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMA, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  // "Three lighthouses have been lit... The elements have been thrown out..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMA, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHEAST);
  // "Jupiter is growing stronger, and the north grows colder with every..."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  // "You must hurry to Mars Lighthouse. Ignite its beacon, before all of..."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  // "Mars Lighthouse? Agatio has the Mars Star. He's on his way to light it..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Contigo_StartSparkle(actor, &gContigo_SparkleState);
  Event_Wait(80);
  Contigo_StopSparkle(actor);
  Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  // "I doubt they will be able to light the Mars beacon..."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Why's that?"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Contigo_StartSparkle(actor, &gContigo_SparkleState);
  Event_Wait(80);
  Contigo_StopSparkle(actor);
  Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  // "There is a powerful force that does not want to see Mars rekindled."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAttachedEffect(ACTOR_ISAAC,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Actor_FaceDirectionAndRefreshById(ACTOR_HAMMA, angle);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  // "You will fail as well...unless you pool your strengths together and..."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_Jump(ACTOR_KRADEN, 4, 10);
  // "This is bad. We have to go...now!"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "I had hoped that you would feel that way..."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  // "I've prepared a gift for you, to aid you on your way to Mars Lighthouse."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_NOD);
  // "I must leave you now, but we will meet at the inlet."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveTo(180, CAMERA_KEEP, 410, 1);
  Actor_WalkToAndWait(ACTOR_HAMMA, 160, 466);
  Actor_SetPosition(ACTOR_HAMMA, 0, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(148, CAMERA_KEEP, 368, 1);
  FieldCamera_WaitForArrival();
  Scene_PlayMusic();
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "I'm sorry we left Venus Lighthouse without seeing you... I'm sorry for..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 20);
  Actor_FaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_ISAAC, FACING_EAST);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_Jump(ACTOR_JENNA, 2, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  // "Maybe when this is all over, we can all go on a trip together."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHWEST);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Hey, Jenna, you sure seem calm, considering that we're racing to meet..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "I'm just relieved that we've sorted our differences..."
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  // "Me too, Mia... We could not have stood divided against a common foe."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Yeah, I guess I'm a little happy that we're not going to have to beat..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  // "Listen, this is Felix's quest now... We're just doing what we can to..."
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
  // "Aren't you happy?"
  FieldMessage_ShowAtActor(ACTOR_JENNA, 0);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    // "Everything's finally come together... This is how it was meant to..."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    gFieldState->message++;
  } else {
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
    gFieldState->message++;
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // "Don't worry about the future, Felix... Enjoy this brief moment's..."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  }
  Actor_WalkToAndWait(ACTOR_KRADEN, 146, 368);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  // "But Hamma's waiting for us. Let's go to our ship!"
  Event_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetMovementTypeById(ACTOR_ISAAC, gContigo_JoinLeaderScript);
  Actor_SetMovementTypeById(ACTOR_GARET, gContigo_JoinLeaderScript);
  Actor_SetMovementTypeById(ACTOR_MIA, gContigo_JoinLeaderScript);
  Actor_SetMovementTypeById(ACTOR_IVAN, gContigo_JoinLeaderScript);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gContigo_JoinLeaderScript);
  Actor_SetMovementTypeById(ACTOR_JENNA, gContigo_JoinLeaderScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gContigo_JoinLeaderScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS, gContigo_JoinLeaderScript);
  Party_AddIsaacsParty();
  GameFlag_Set(FLAG_REUNION_AT_CONTIGO);
  Event_End();
}

extern u32 gContigo_LeaderVisitsScript[];

/// The party's first steps into the house at the top of the hill after the
/// ship has its wings (cells (7, 25) to (12, 25), just inside the door): Hamma
/// welcomes it to her new home, tells Ivan why she kept from him that she is
/// his sister, and warns that the world is falling into ruin unless Mars
/// Lighthouse is lit.
void Contigo_ReturnToNewHome(void) {
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0.8);
    FieldCamera_MoveTo(160, CAMERA_KEEP, 368, 1);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 158, 394);
    Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST, 0);
    Actor_PlaceOnActor(ACTOR_IVAN, gMainRoutineState.leaderActorId);
    wait(1);
    Actor_SetSpeed(ACTOR_IVAN, 0.8);
    Actor_WalkToAndWaitById(ACTOR_IVAN, 144, 374);
    Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
    FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    Event_SetMessageId((s32)MsgReturnedToMyNewHome);
    // "Ah, you've returned! What brings you to my new home?"
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    // "Hamma..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_SetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    // "You're my sister... Why didn't you tell me sooner? Why didn't you..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
    // "You play an important role in all this. If I'd told you, you would..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 40);
    Actor_WalkToAndWait(ACTOR_IVAN, 180, 364);
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_IVAN, 186, 388);
    Event_Wait(40);
    Actor_WalkToAndWait(ACTOR_IVAN, 146, 380);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "This place is so familiar... I feel like I've lived here my whole life."
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    // "Look at me, Ivan."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 40);
    // "You're too sentimental. You would have given up your quest to find..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    // "What are you saying, sister?"
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
    // "No, not sister... Do not think of me as your sister until you have..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_SetAttachedEffect(ACTOR_IVAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "W-Why?"
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "Our world is falling into ruin..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_PlaceOnActor(ACTOR_ISAAC, gMainRoutineState.leaderActorId);
    wait(1);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_SetSpeed(ACTOR_ISAAC, 1.2);
    Actor_WalkToAndWaitById(ACTOR_ISAAC, 168, 380);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
    // "What? Hamma, what do you mean? What's happening?"
    Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
    FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
    Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
    wait(1);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
    Actor_SetMovementTypeById(gMainRoutineState.leaderActorId,
                              gContigo_LeaderVisitsScript);
    Actor_SetSpeed(ACTOR_KRADEN, 0.8);
    Actor_WalkToAndWait(ACTOR_KRADEN, 182, 378);
    Actor_WalkToAndWaitById(ACTOR_KRADEN, 182, 360);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "Look, I don't want to interrupt your reunion or anything, but..."
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Actor_FaceDirectionById(ACTOR_HAMMA, FACING_EAST, 0);
    Actor_SetAttachedEffect(ACTOR_KRADEN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "I couldn't help overhearing you say something about the world..."
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
    // "Our world requires a delicate balance of elemental forces. That..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                         EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST);
    // "Without that balance, one element might be lost beneath the others..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    // "You're talking about Mars Lighthouse, right?"
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Actor_FaceDirectionAndRefreshById(ACTOR_HAMMA, FACING_EAST);
    Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 40);
    Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_EAST, 0);
    Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
    // "Isn't Agatio taking care of Mars Lighthouse? Everything should be..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST, 20);
    // "Agatio will not light Mars Lighthouse."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
    // "You don't think he can do it?"
    Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
    Actor_SetAttachedEffect(ACTOR_HAMMA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST);
    // "No. A great power stands ready to oppose them."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
    FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                         EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST);
    Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
    // "They lack the power to resist that force, and they will fall before it."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_SOUTHEAST,
                            40);
    FieldEffect_SetAttached(0, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(20);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
    // "But what will happen if Mars Lighthouse isn't lit quickly enough?"
    Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
    Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
    // "Our world will be plunged into ice."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    // "But what would make that happen?"
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST);
    // "Even I do not know the answer to that question."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
    Actor_FaceDirectionById(ACTOR_HAMMA, FACING_EAST, 80);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                         EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
    // "Ah... Yes, given the way that the elements interact, I see..."
    Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    // "Kraden, you understand?"
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
    // "Well, I'm only guessing... But I think it's a good guess."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_Jump(ACTOR_IVAN, 2, 10);
    // "Quit fooling around and tell me!"
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    // "The earth is nurturing and warm. Many creatures are reared in this..."
    FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        0) {
      Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
      Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
      // "I follow you..."
      Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
      gFieldState->message++;
    } else {
      Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
      gFieldState->message++;
      Actor_SetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
      // "It's OK, Felix... I don't really get it either, but let's see..."
      Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    }
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // "Now, think about the ocean. It is a cold place, is it not?"
    FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        0) {
      Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
      Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
      // "Yeah, it is awfully cold..."
      Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
      gFieldState->message++;
    } else {
      Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
      gFieldState->message++;
      Actor_SetAnimationById(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
      // "I don't know what point he's trying to make, but I have to agree..."
      Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
    }
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    // "Both lighthouses were lit, and a balance was reached."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
    // "But with only the lighthouse of wind lit, the balance has been..."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetAttachedEffect(ACTOR_IVAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "I get it... Wind cools things down, but it can't heat them up..."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    // "When you burn your hand on something hot, you blow on it to cool it..."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
    // "Right, yeah... I've always done that without thinking about it, but..."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
    FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // "When Mercury Lighthouse was lit, this didn't happen, though. I..."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    // "Water cannot cool things as swiftly as wind, perhaps."
    Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    // "Or maybe the combined power of water and wind is cooling things down..."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
    // "If what Hamma's saying is true, we'd better get moving again, Felix."
    Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
    Actor_SetAttachedEffect(ACTOR_IVAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
    // "I got too worked up about seeing my sister... Sorry."
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST);
    Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
    // "It's all right. I'm happy that you cared so much..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    // "All right, Felix! Let's head to Mars Lighthouse!"
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_SetSpeed(ACTOR_IVAN, 1.2);
    Actor_SetSpeed(ACTOR_ISAAC, 1.2);
    Actor_SetSpeed(ACTOR_KRADEN, 1.2);
    Actor_SetMovementTypeById(ACTOR_IVAN, gContigo_RejoinLeaderScript);
    Actor_SetMovementTypeById(ACTOR_ISAAC, gContigo_RejoinLeaderScript);
    Actor_SetMovementTypeAndWaitById(ACTOR_KRADEN, gContigo_RejoinLeaderScript);
    GameFlag_Set(FLAG_HAMMA_WELCOMED_PARTY);
    Event_End();
  }
}

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_CONTIGO)
    Contigo_InitializeVillage();
  else if (scene == SCENE_CONTIGO_GEOGLYPH)
    Contigo_InitializeGeoglyph();
  else if (scene == SCENE_CONTIGO_SHOPS)
    Contigo_InitializeShops();
  else if (scene == SCENE_CONTIGO_LUCKY_DICE)
    Contigo_InitializeLuckyDice();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// A task that draws the Corn at the leader's sprite priority.
void Contigo_KeepPriorityWithLeader(void) {
  if (Actor_GetById(ACTOR_CORN) != 0) {
    struct Actor *owner = Actor_GetById(gMainRoutineState.leaderActorId);

    Actor_SetSpritePriority(ACTOR_CORN,
                            ((struct FieldSprite *)owner->sprite)->priority);
  }
}

/// Sets up the village: the two children walking about its middle stop when
/// faced (Contigo_PauseWhenFaced); once Jupiter Lighthouse's beacon is lit,
/// until the ship has its wings, the girl and the old woman are gone and four
/// doors are locked; Shine waits where it landed until caught. Arriving by the
/// gate, or out of the two houses south of the Teleport circle or the Lucky
/// Wheels tent, the view follows Felix east past the gate to the weed patch;
/// then the Teleport circle's colours are set.
void Contigo_InitializeVillage(void) {
  struct Actor *actor = Actor_GetById(ACTOR_BOY_DOUBTING_THE_LEGENDS);

  Task_Add(Contigo_KeepPriorityWithLeader, TASK_PASS_FRAME);
  actor->riseCounter = 1;
  actor->riseEnabled = 0;
  actor->update = Contigo_PauseWhenFaced;
  if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 &&
      GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) == 0) {
    Actor_Stop(ACTOR_GIRL_ON_THE_SHIP);
    Actor_SetPosition(ACTOR_GIRL_ON_THE_SHIP, 0, 0);
    Actor_SetPosition(ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, 0, 0);
    Contigo_LockDoors();
  } else {
    actor = Actor_GetById(ACTOR_GIRL_ON_THE_SHIP);
    actor->riseCounter = 0;
    actor->riseEnabled = 0;
    actor->update = Contigo_PauseWhenFaced;
  }
  if (GameFlag_Get(FLAG_SHINE_FREED) != 0 &&
      GameFlag_Get(FLAG_SHINE_CAUGHT) == 0) {
    Actor_SetPosition(ACTOR_SHINE, 32, 708);
  } else {
    Actor_SetSpriteColor(Actor_GetById(ACTOR_SHINE), SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHINE), 0);
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    switch (gMainRoutineState.entrance) {
    case CONTIGO_FROM_WORLD_MAP:
    case CONTIGO_FROM_CONTIGO_SHOPS_SOUTH:
    case CONTIGO_FROM_CONTIGO_SHOPS_SOUTHWEST:
    case CONTIGO_FROM_CONTIGO_LUCKY_DICE_SOUTHWEST:
      GameFlag_Set(FLAG_WHOLE_VILLAGE_IN_VIEW);
      break;
    default:
      Field_SetCameraBounds(0, toFIXED(512), toFIXED(512), toFIXED(1024));
      break;
    }
  }
  if (GameFlag_Get(FLAG_WHOLE_VILLAGE_IN_VIEW) != 0) {
    Field_SetCameraBounds(0, toFIXED(512), toFIXED(880), toFIXED(1024));
    FieldCamera_FollowActor(gMainRoutineState.leaderActorId, 0);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  Palette_SetColorsFade(gTeleportCircle_PaletteSlots, gTeleportCircle_Colors,
                        gTeleportCircle_ShimmerColors,
                        gTeleportCircle_ShimmerSlots);
}

/// Sets up the geoglyph: until Jupiter Lighthouse's beacon is lit the wings'
/// parts lie on it among the workers; after, they and the workers are gone,
/// and the men in long robes too until the ship has its wings.
void Contigo_InitializeGeoglyph(void) {
  Contigo_ShrinkActor(ACTOR_SIGN);
  if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0) {
    MapGrid_CopyCellAttributes(44, 40, 8, 3, 44, 36);
    Actor_SetPosition(ACTOR_WING_FRAME_A, 0, 0);
    Actor_SetPosition(ACTOR_WING_FRAME_B, 0, 0);
    Actor_SetPosition(ACTOR_WING_FRAME_C, 0, 0);
    Actor_SetPosition(ACTOR_METAL_FEATHER_A, 0, 0);
    Actor_SetPosition(ACTOR_METAL_FEATHER_B, 0, 0);
    Actor_SetPosition(ACTOR_METAL_FEATHER_C, 0, 0);
    Actor_SetPosition(ACTOR_METAL_FEATHER_D, 0, 0);
    Actor_SetPosition(ACTOR_METAL_FEATHER_E, 0, 0);
    Actor_SetPosition(ACTOR_LOOSE_PART_A, 0, 0);
    Actor_SetPosition(ACTOR_LOOSE_PART_B, 0, 0);
    Actor_SetPosition(ACTOR_LOOSE_PART_C, 0, 0);
    Actor_SetPosition(ACTOR_LOOSE_PART_D, 0, 0);
    Actor_SetPosition(ACTOR_LOOSE_PART_E, 0, 0);
    Actor_SetPosition(ACTOR_LOOSE_PART_F, 0, 0);
    Actor_SetPosition(ACTOR_LOOSE_PART_G, 0, 0);
    Actor_SetPosition(ACTOR_VISITOR_FROM_AFAR, 0, 0);
    Actor_SetPosition(ACTOR_WOMAN_ON_THE_WINGS, 0, 0);
    Actor_SetPosition(ACTOR_WING_WORKER_B, 0, 0);
    Actor_SetPosition(ACTOR_WING_WORKER_C, 0, 0);
    Actor_SetPosition(ACTOR_WING_WORKER_D, 0, 0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) == 0) {
      Actor_SetPosition(ACTOR_WING_WORKER_A, 0, 0);
      Actor_SetPosition(ACTOR_MAN_ON_THE_GEOGLYPH, 0, 0);
      Actor_SetPosition(ACTOR_MAN_WHO_TRANSLATED_THE_WALL, 0, 0);
      Actor_SetPosition(ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, 0, 0);
      Actor_SetPosition(ACTOR_MAN_ON_HAMMET, 0, 0);
    } else {
      Actor_GetById(ACTOR_WING_WORKER_A)->facing = FACING_NORTH_NORTHEAST;
      wait(1);
    }
  } else {
    Contigo_SetUpWingParts();
  }
}

/// Sets up the houses: walking into the house at the top of the hill once the
/// beacon is lit starts the reunion; in the others the baby lies in its
/// cradle, the old man on Hesperia shivers once the ship has its wings, and
/// the workers resting at the inn are gone after the reunion.
void Contigo_InitializeShops(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (gMainRoutineState.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH) {
    if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 &&
        GameFlag_Get(FLAG_REUNION_AT_CONTIGO) == 0)
      Contigo_TheReunion();
  } else if (gMainRoutineState.entrance ==
             CONTIGO_SHOPS_FROM_ATTEKA_INLET_GEODE) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  } else {
    Actor_SetAnimationById(ACTOR_BABY, 2);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_BABY), 0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      Actor_SetMovementTypeById(ACTOR_OLD_MAN_ON_HESPERIA,
                                gContigo_ShiverScript);
    if (GameFlag_Get(FLAG_REUNION_AT_CONTIGO) != 0) {
      Actor_SetPosition(ACTOR_WORN_OUT_WORKER, 0, 0);
      Actor_SetPosition(ACTOR_WORKER_ON_THE_PROPHECIES, 0, 0);
      Actor_SetPosition(ACTOR_OLD_MAN_TOO_OLD_TO_HELP, 0, 0);
      Actor_SetPosition(ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE, 0, 0);
    }
  }
}

void ItemDisplay_ShowItem(s32 item, s32 count);

/// Sets up the dice hall: its signs drawn small and its attendants' colours,
/// and on the party's return from a game the coins won or lost, or the
/// prizes.
void Contigo_InitializeLuckyDice(void) {
  s32 coinChange;
  s8 *wonPrizes;
  s8 *prize;
  s32 item;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Contigo_ShrinkActor(ACTOR_SUPER_LUCKY_DICE_RULES_SIGN);
  Contigo_ShrinkActor(ACTOR_LUCKY_DICE_RULES_SIGN);
  Contigo_ShrinkActor(ACTOR_LUCKY_DICE_PAIRS_SIGN);
  Contigo_ShrinkActor(ACTOR_LUCKY_WHEELS_RULES_SIGN);
  Contigo_ShrinkActor(ACTOR_LUCKY_WHEELS_PRIZE_SIGN);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_LUCKY_DICE_ATTENDANT),
                       SPRITE_COLOR_GRB);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_LUCKY_DICE_RULES_ATTENDANT),
                       SPRITE_COLOR_RBG);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SUPER_LUCKY_DICE_ATTENDANT),
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SUPER_LUCKY_DICE_RULES_ATTENDANT),
                       SPRITE_COLOR_BGR);
  if (!Event_GetFlag(FLAG_LEFT_FOR_A_GAME))
    return;
  GameFlag_Clear(FLAG_LEFT_FOR_A_GAME);
  // The dice games come back here to report the change in the party's coins.
  if (gMainRoutineState.entrance == ENTRANCE_FROM_SUPER_LUCKY_DICE) {
    coinChange = gMainRoutineState.coins - *(s32 *)gSceneState;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (coinChange > 0) {
      if (coinChange >= LUCKY_DICE_LARGE_WIN)
        Sound_Play(MUS_SUPER_LUCKY);
      else if (coinChange >= LUCKY_DICE_MEDIUM_WIN)
        Sound_Play(MUS_VERY_LUCKY);
      else
        Sound_Play(MUS_LUCKY);
      Event_Wait(20);
      Event_SetMessageId(COIN_WON_MESSAGE);
      MessageArgument_Push(coinChange, MESSAGE_ARGUMENT_NUMBER);
      // "Ya won ... coin! Talk about lucky!"
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
      Sound_WaitForJingle();
    } else if (coinChange < 0) {
      Event_SetMessageId(COIN_LOST_MESSAGE);
      MessageArgument_Push(-coinChange, MESSAGE_ARGUMENT_NUMBER);
      // "Ya lost ... coin. Better luck next time."
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
    }
    Event_End();
  } else if (gMainRoutineState.entrance == ENTRANCE_FROM_LUCKY_DICE) {
    coinChange = gMainRoutineState.coins - *(s32 *)gSceneState;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (coinChange > 0) {
      if (coinChange >= LUCKY_DICE_LARGE_WIN)
        Sound_Play(MUS_SUPER_LUCKY);
      else if (coinChange >= LUCKY_DICE_MEDIUM_WIN)
        Sound_Play(MUS_VERY_LUCKY);
      else
        Sound_Play(MUS_LUCKY);
      Event_Wait(20);
      Event_SetMessageId(COIN_WON_MESSAGE);
      MessageArgument_Push(coinChange, MESSAGE_ARGUMENT_NUMBER);
      // "Ya won ... coin! Talk about lucky!"
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
      Sound_WaitForJingle();
    } else if (coinChange < 0) {
      Event_SetMessageId(COIN_LOST_MESSAGE);
      MessageArgument_Push(-coinChange, MESSAGE_ARGUMENT_NUMBER);
      // "Ya lost ... coin. Better luck next time."
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
    }
    Event_End();
  } else if (gMainRoutineState.entrance == ENTRANCE_FROM_LUCKY_WHEELS) {
    // Lucky Wheels comes back with the icons matched, for the attendant to
    // award a prize for each.
    wonPrizes = gMainRoutineState.wheelsPrizes;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    if (wonPrizes[0] == -1) {
      Contigo_OfferLuckyWheels(1);
    } else if (wonPrizes[0] == -2) {
      Event_SetMessageId(READ_SIGN_MESSAGE);
      // "The rules are listed on the sign. Take a look, take a look."
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    } else {
      Event_SetMessageId(CONGRATULATIONS_MESSAGE);
      // "Congratulations!"
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      if (wonPrizes[0] != -1) {
        for (prize = wonPrizes; *prize != -1; prize++) {
          if (prize == wonPrizes)
            Event_SetMessageId(FIRST_PRIZE_MESSAGE);
          else
            Event_SetMessageId(NEXT_PRIZE_MESSAGE);
          item = Contigo_SelectLuckyWheelsPrize(*prize);
          MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
          // "You won ...! Here you go.", then "And you won ...! Here you go."
          Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
          ItemDisplay_ShowItem(item,
                               ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
          PartyInventory_GiveItem(item, 0);
          Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
        }
      }
      gMainRoutineState.wheelsPrizes[0] = -2;
      Event_SetMessageId(COME_AGAIN_MESSAGE);
      // "Come again!"
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    }
    Event_End();
  }
}

void Actor_SetSpriteColorMode(struct Actor *, u32);
s32 BattleEffect_GetObjectDepth(struct Actor *actor);

/// The sparkle's update, played over Hamma as she speaks of Mars Lighthouse in
/// the reunion: two pairs of white half-rings rise around her, a pair starting
/// every 16 frames, until both have climbed RING_CLIMB pixels.
void Contigo_RaiseSparkle(struct Actor *actor) {
  struct Vector3 position;
  s32 x, z;
  FIXED sine;
  s32 i, j;
  struct SparkleState *state = (struct SparkleState *)actor->watchedActor;
  u8 *count = &actor->riseEnabled;
  struct EffectSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(actor,
                             (u8)((u32)((struct CounterBits *)count)->rest %
                                  (SPRITE_COLOR_BRG + 1)));
  if (actor->riseCounter == 0) {
    if ((*count & (RING_INTERVAL - 1)) == 0) {
      Sound_Play(SE_RISING_RINGS);
      state->heights[state->nextPair] = 0;
      state->nextPair ^= 1;
    }
  } else if (actor->riseCounter == 1) {
    if (state->heights[0] >= RING_CLIMB && state->heights[1] >= RING_CLIMB) {
      Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
      VramCache_FreeEntry(state->vramEntry);
      actor->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    // Its size: half a turn of sine over the climb.
    sine = Trig_Sin(state->heights[i] * (DEGtoANG(180) / RING_CLIMB));
    if (state->heights[i] >= 0) {
      if (state->heights[i] < RING_CLIMB) {
        position.x = actor->x.fixed;
        position.y = actor->y.fixed + toFIXED(state->heights[i]);
        position.z = actor->z.fixed;
        Camera_WorldToScreen(&position);
        x = position.x;
        z = position.z;
        sprite = &state->sprites[i * 2];
        for (j = 0; j < 2; j++) {
          sprite->x = x;
          sprite->y = z;
          sprite->scaleX = sine;
          sprite->scaleY = sine;
          if (j == 0) {
            sprite->depth = BattleEffect_GetObjectDepth(actor) - 1;
          } else {
            sprite->depth = BattleEffect_GetObjectDepth(actor) + 1;
            sprite->y -= toFIXED(1);
            sprite->scaleY = -sprite->scaleY;
          }
          EffectSprite_Draw(sprite);
          sprite++;
        }
        state->heights[i]++;
      }
    }
  }
}

/// Sets up the sparkle: loads its picture and its four 16 by 16 sprites, and
/// hands the object to its update, Contigo_RaiseSparkle.
void Contigo_StartSparkle(struct Actor *actor, struct SparkleState *state) {
  struct EffectSprite *sprite;
  s32 tile;
  s32 row;
  s32 col;

  state->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(state->vramEntry, 256, gLightRing_Tiles);
  state->nextPair = 0;
  state->heights[0] = RING_NOT_STARTED;
  state->heights[1] = RING_NOT_STARTED;
  state->actor = actor;
  actor->riseCounter = 0;
  actor->riseEnabled = 0;
  actor->update = Contigo_RaiseSparkle;
  tile = gVramCacheEntries[state->vramEntry].offset >> 5;
  actor->watchedActor = (struct Actor *)state;
  for (row = 0; row <= 1; row++) {
    for (col = 1, sprite = state->sprites + row + row; col >= 0; col--) {
      EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X16, tile);
      sprite->oam.fullColor = 0;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(actor);
      sprite++;
    }
  }
}

/// Ends the sparkle: its update finishes once both pairs have risen.
void Contigo_StopSparkle(struct Actor *actor) { actor->riseCounter = 1; }

// clang-format off
#include "shared/light_ring_tiles.c"

#include "shared/field_effect_duration_scripts.c"

/// The Lucky Wheels' prizes, three to each icon that wins them
/// (Contigo_SelectLuckyWheelsPrize).
u32 gContigo_WheelsPrizes[] = {
    ITEM_MYTHRIL_SHIRT, ITEM_SILK_SHIRT,  ITEM_RUNNING_SHIRT,
    ITEM_HYPER_BOOTS,   ITEM_QUICK_BOOTS, ITEM_FUR_BOOTS,
    ITEM_ADEPT_RING,    ITEM_WAR_RING,    ITEM_SLEEP_RING,
    ITEM_POTION,        ITEM_VIAL,        ITEM_NUT,
    ITEM_WATER_OF_LIFE, ITEM_PSY_CRYSTAL, ITEM_ELIXIR,
};

s32 gContigo_EnterSanctumDoorScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, -(toFIXED(1) / 2)),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

#define TELEPORT_CIRCLE_FIRST_ENTRY 89
#include "shared/teleport_circle_palette.c"

const s32 *gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};

u32 gContigo_DoubtingBoyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_MOVE(338, 0, 844),
    SC_SYNC,
    SC_MOVE(318, 0, 844),
    SC_SYNC,
    SC_MOVE(304, 0, 872),
    SC_SYNC,
    SC_MOVE(320, 0, 884),
    SC_SYNC,
    SC_MOVE(336, 0, 884),
    SC_SYNC,
    SC_MOVE(352, 0, 872),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gContigo_GirlOnTheShipScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_MOVE(318, 0, 884),
    SC_SYNC,
    SC_MOVE(338, 0, 884),
    SC_SYNC,
    SC_MOVE(352, 0, 872),
    SC_SYNC,
    SC_MOVE(338, 0, 844),
    SC_SYNC,
    SC_MOVE(318, 0, 844),
    SC_SYNC,
    SC_MOVE(304, 0, 872),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gContigo_JennaReunionScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(164, 0, 360),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gContigo_PiersReunionScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(190, 0, 352),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gContigo_ShebaReunionScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(190, 0, 390),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gContigo_KradenReunionScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(182, 0, 368),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/// The end of the reunion: each walks to the leader and goes into him, as the
/// two parties become one.
u32 gContigo_JoinLeaderScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(160, 0, 382),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
    // Unused: past the end.
    SC_ANIMATION(ANIM_WALK),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The old man on Hesperia shivering once the party has set out with the
/// wings: "Brrr...Brrr... It's been really cold since the lighthouse beacon was
/// lit!"
s32 gContigo_ShiverScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 5 / 32),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 128),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 128),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// In Hamma's new home the leader steps up to face her from the west.
u32 gContigo_LeaderVisitsScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(128, 0, 378),
    SC_SYNC,
    SC_MOVE(132, 0, 360),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

/// The end of the visit to Hamma's new home: Ivan, Isaac and Kraden walk to
/// the leader and go into him.
u32 gContigo_RejoinLeaderScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(132, 0, 359),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

/// The worker wondering which part goes where looks from one part of the
/// wings to another, and wonders (Contigo_ShowWorkerPuzzled).
s32 gContigo_PuzzledWorkerScript[] = {
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(120),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_CALL((s32)Contigo_ShowWorkerPuzzled),
    SC_WAIT(120),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 271, 0, 808, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Contigo),
    SCENE_EXIT(CONTIGO_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CONTIGO),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_SOUTH, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_SOUTH),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_SOUTHWEST, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_SOUTHWEST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_LUCKY_DICE_SOUTHWEST, ContigoLuckyDice, CONTIGO_LUCKY_DICE_FROM_CONTIGO_SOUTHWEST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_WEST, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_WEST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_LUCKY_DICE_NORTHEAST, ContigoLuckyDice, CONTIGO_LUCKY_DICE_FROM_CONTIGO_NORTHEAST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_LUCKY_DICE_EAST, ContigoLuckyDice, CONTIGO_LUCKY_DICE_FROM_CONTIGO_EAST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_EAST, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_EAST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_A, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_NORTHEAST_A),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_B, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_NORTHEAST_B),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_NORTH, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_NORTH),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_GEOGLYPH, ContigoGeoglyph, CONTIGO_GEOGLYPH_FROM_CONTIGO),
    SCENE_EXITS(ContigoGeoglyph),
    SCENE_EXIT(CONTIGO_GEOGLYPH_TO_CONTIGO, Contigo, CONTIGO_FROM_CONTIGO_GEOGLYPH),
    SCENE_EXIT(CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTH, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_CONTIGO_GEOGLYPH_NORTH),
    SCENE_EXIT(CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTHEAST, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_CONTIGO_GEOGLYPH_NORTHEAST),
    SCENE_EXIT(CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_SOUTH, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_CONTIGO_GEOGLYPH_SOUTH),
    SCENE_EXITS(ContigoShops),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_SOUTH, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_SOUTH),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_SOUTHWEST, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_SOUTHWEST),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_B, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_NORTHEAST_B),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_NORTH, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_NORTH),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_EAST, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_EAST),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_A, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_NORTHEAST_A),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_WEST, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_WEST),
    SCENE_EXIT(CONTIGO_SHOPS_TO_ATTEKA_INLET_GEODE, AttekaInletGeode, ATTEKA_INLET_GEODE_FROM_CONTIGO_SHOPS),
    SCENE_EXITS(ContigoLuckyDice),
    SCENE_EXIT(CONTIGO_LUCKY_DICE_TO_CONTIGO_NORTHEAST, Contigo, CONTIGO_FROM_CONTIGO_LUCKY_DICE_NORTHEAST),
    SCENE_EXIT(CONTIGO_LUCKY_DICE_TO_CONTIGO_EAST, Contigo, CONTIGO_FROM_CONTIGO_LUCKY_DICE_EAST),
    SCENE_EXIT(CONTIGO_LUCKY_DICE_TO_CONTIGO_SOUTHWEST, Contigo, CONTIGO_FROM_CONTIGO_LUCKY_DICE_SOUTHWEST),
    SCENE_EXITS_END,
};

struct ActorPlacement gContigo_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_VillagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 340, 0, 936, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 942, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 808, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 116, 0, 680, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 792, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 804, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, (s32)gContigo_DoubtingBoyScript, 352, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, (s32)gContigo_GirlOnTheShipScript, 304, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SALT), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_SHINE), MOVEMENT_TYPE_STAND, 56, 0, 706, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_GeoglyphPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 846, 0, 660, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 616, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 756, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, (s32)gContigo_PuzzledWorkerScript, 774, 0, 664, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 588, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 806, 0, 596, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 832, 0, 310, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 312, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 346, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 343, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 720, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WING_FRAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 720, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_FRAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 752, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_FRAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 784, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 776, 0, 590, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 808, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 744, 0, 614, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 808, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 826, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 700, 0, 604, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 668, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 758, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 858, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 782, 0, 678, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 669, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_ShopsPlacementsBeforeReunion[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 84, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 88, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 112, 0, 130, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 94, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 112, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 733, 0, 91, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 108, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 708, 0, 154, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_RED_BOW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 756, 0, 108, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 352, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 352, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 584, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 186, 0, 636, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 616, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 592, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 94, 0, 574, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 586, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 664, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 310, 0, 614, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 688, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_ShopsPlacementsReunion[] = {
    ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 382, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 366, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 354, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 398, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_ShopsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 158, 0, 352, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_LuckyDicePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 840, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 92, 0, 902, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 872, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 90, 0, 872, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 840, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 300, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 340, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 508, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 548, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gContigo_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_VillageTriggers[] = {
    {TRIGGER_EXIT, CONTIGO_TO_WORLD_MAP, CONDITION_ALWAYS, CONTIGO_TO_WORLD_MAP},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_SOUTH, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_SOUTH},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_SOUTHWEST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_SOUTHWEST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_LUCKY_DICE_SOUTHWEST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_LUCKY_DICE_SOUTHWEST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_WEST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_WEST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_LUCKY_DICE_NORTHEAST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_LUCKY_DICE_NORTHEAST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_LUCKY_DICE_EAST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_LUCKY_DICE_EAST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_EAST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_EAST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_A, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_A},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_B, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_B},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_NORTH, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_NORTH},
    {TRIGGER_EXIT, CONTIGO_TO_CONTIGO_GEOGLYPH, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_GEOGLYPH},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), CELL_BEHAVIOR_TELEPORT_CIRCLE, CONDITION_ALWAYS, EVENT(Contigo_TeleportOut)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_WEST_OF_ITEM_SHOP, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsWestOfItemShop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_BEHIND_INN, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsBehindInn)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_OVER_BRAMBLE_SEED, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsOverBrambleSeed)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_WEST_OF_WEAPON_SHOP, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsWestOfWeaponShop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_BY_GATE, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsByGate)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_BELOW_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsBelowLuckyWheels)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_ON_CLIFF_WEST, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsOnCliffWest)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_ON_CLIFF_EAST, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsOnCliffEast)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_TOP_ROW, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedPatchTopRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_SECOND_ROW, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedPatchSecondRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_MIDDLE_ROW, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedPatchMiddleRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_FOURTH_ROW, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedPatchFourthRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_BOTTOM_ROW, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedPatchBottomRow)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VIEW_WEST_OF_GATE, CONDITION_ALWAYS, EVENT(Contigo_NarrowCameraBounds)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VIEW_EAST_OF_GATE, CONDITION_ALWAYS, EVENT(Contigo_WidenCameraBounds)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SHINE, FLAG_SHINE_FREED, EVENT(Contigo_ForceFreesDjinni)},
    {TRIGGER_TALK, ACTOR_SHINE, IF_FLAG_SET(FLAG_SHINE_FREED), EVENT(Contigo_FreedDjinniJoinsParty)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, FLAG_JUPITER_BEACON_LIT, EVENT(Contigo_AskAboutWindLegend)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoAreYouLookingForIsaac)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, CONDITION_ALWAYS, MSGNO(MsgContigoIGotStuckDoingChores)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoTheAnemosBloodInOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheyLookLikeTheyreIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, CONDITION_ALWAYS, MSGNO(MsgContigoIDidntGetToSee)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORESEEING_THE_BEACON, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoTheBeaconWithinJupiterLighthouse)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORESEEING_THE_BEACON, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoEveryonesLeftTownForThe)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORESEEING_THE_BEACON, CONDITION_ALWAYS, MSGNO(MsgContigoEveryonesBeenGoingOnAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORESEEING_THE_BEACON, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoSurelySomeoneWillComeSoon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORESEEING_THE_BEACON, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoThePropheciesAreAllComing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORESEEING_THE_BEACON, CONDITION_ALWAYS, MSGNO(MsgContigoSomePeopleWereTalkingAbout)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_FLYING_SHIP, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIGuessThePeopleOf)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_FLYING_SHIP, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIsThatYourBoatIt)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_FLYING_SHIP, CONDITION_ALWAYS, MSGNO(MsgContigoItMustBeSoExciting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_FLYING_SHIP, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThePowerOfTheWind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_FLYING_SHIP, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoItsJustLikeTheLegends)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_FLYING_SHIP, CONDITION_ALWAYS, MSGNO(MsgContigoICouldntFlyInThe)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThePeopleOfAnemosPossessed)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoJupiterLighthouseHasBeenLit)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS, CONDITION_ALWAYS, MSGNO(MsgContigoHammaWasTellingUsThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThereWereTwoSiblingsBorn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWhatDoesItMeanWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS, CONDITION_ALWAYS, MSGNO(MsgContigoIHeardThatTheWorld)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RISEN_CITY, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoWhenThePowerOfAnemos)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RISEN_CITY, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheLegendsTellUsThat)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RISEN_CITY, CONDITION_ALWAYS, MSGNO(MsgContigoIfAShipCanRise)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RISEN_CITY, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIThinkJupiterLighthouseWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RISEN_CITY, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheAncientCityOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RISEN_CITY, CONDITION_ALWAYS, MSGNO(MsgContigoTheCityOfTheAnemos)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheAnemosGeoglyphsShowA)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, CONDITION_ALWAYS, MSGNO(MsgContigoISawTheShipFly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoItsHardToBelieveBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, CONDITION_ALWAYS, MSGNO(MsgContigoTheShipLookedLikeIt)},
    {TRIGGER_TALK, ACTOR_BOY_DOUBTING_THE_LEGENDS, CONDITION_ALWAYS, EVENT(Contigo_TalkToDoubtingBoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_DOUBTING_THE_LEGENDS, CONDITION_ALWAYS, EVENT(Contigo_TalkToDoubtingBoy)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_SHIP, CONDITION_ALWAYS, EVENT(Contigo_TalkAboutTheShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SHIP, CONDITION_ALWAYS, EVENT(Contigo_TalkAboutTheShip)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_GAP, CONDITION_ALWAYS, EVENT(Contigo_ScoopForSalt)},
    {TRIGGER_RAISED, RAISED_SALT_FOUND, CONDITION_ALWAYS, EVENT(Contigo_SaltJoinsParty)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_LUCKY_WHEELS_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyWheelsSign)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_SUPER_LUCKY_DICE_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadSuperLuckyDiceSign)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_LUCKY_DICE_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyDiceSign)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(Scene_ShowEntranceLocked)},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_GeoglyphTriggers[] = {
    {TRIGGER_EXIT, CELL_BEHAVIOR_GEOGLYPH_WEST_EDGE, CONDITION_ALWAYS, CONTIGO_GEOGLYPH_TO_CONTIGO},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SANCTUM_WEST_DOOR, CONDITION_ALWAYS, EVENT(Contigo_EnterSanctumWestDoor)},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_SANCTUM_MIDDLE_DOOR, CONDITION_ALWAYS, CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTHEAST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SANCTUM_EAST_DOOR, CONDITION_ALWAYS, EVENT(Contigo_EnterSanctumEastDoor)},
    {TRIGGER_TALK, ACTOR_VISITOR_FROM_AFAR, CONDITION_ALWAYS, MSGNO(MsgContigoIveComeALongWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VISITOR_FROM_AFAR, CONDITION_ALWAYS, MSGNO(MsgContigoTheGuysCarryingTheOther)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_WINGS, CONDITION_ALWAYS, MSGNO(MsgContigoTheWingsAreAlmostDone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_WINGS, CONDITION_ALWAYS, MSGNO(MsgContigoIveBeenReallyBusyLately)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_A, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheWingsWillBeFinished)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_A, CONDITION_ALWAYS, MSGNO(MsgContigoICantBelieveThatA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_A, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWhatDoWeDoIf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_A, CONDITION_ALWAYS, MSGNO(MsgContigoThatFlyingShipLookedSo)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_B, CONDITION_ALWAYS, MSGNO(MsgContigoWhichPartGoesWhereDoes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_B, CONDITION_ALWAYS, MSGNO(MsgContigoThereAreTooManyParts)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_C, CONDITION_ALWAYS, MSGNO(MsgContigoYouveGottaWatchOutCause)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_C, CONDITION_ALWAYS, MSGNO(MsgContigoWhenIStartGettingIrritated)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_D, CONDITION_ALWAYS, MSGNO(MsgContigoJustAFewMoreTouches)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_D, CONDITION_ALWAYS, MSGNO(MsgContigoHowLongHaveWeBeen)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GEOGLYPH, FLAG_SET_OUT_WITH_WINGS, EVENT(Contigo_AskAboutGeoglyph)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GEOGLYPH, CONDITION_ALWAYS, MSGNO(MsgContigoLookCloselyAtTheGeoglyph)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GEOGLYPH, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoICantEvenImagineHow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GEOGLYPH, CONDITION_ALWAYS, MSGNO(MsgContigoWeAllCameTogetherAnd)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_TRANSLATED_THE_WALL, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoThatSignHasATranslation)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_TRANSLATED_THE_WALL, CONDITION_ALWAYS, MSGNO(MsgContigoImConvincedTheresSomethingHidden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_TRANSLATED_THE_WALL, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIDontKnowIfAnyone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_TRANSLATED_THE_WALL, CONDITION_ALWAYS, MSGNO(MsgContigoAllRightSoWeveFollowed)},
    {TRIGGER_TALK, ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIWantToGetA)},
    {TRIGGER_TALK, ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgContigoAllTheAmazingThingsWeve)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWouldntThatBeTerribleIf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouTapOnThe)},
    {TRIGGER_TALK, ACTOR_MAN_ON_HAMMET, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheMerchantHammetTookThe)},
    {TRIGGER_TALK, ACTOR_MAN_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgContigoHesAnOddBoyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HAMMET, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoSomeoneToldMeThatHammet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgContigoIvanWasSupposedToBe)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_CARVED_WALL, CONDITION_ALWAYS, EVENT(Contigo_ExamineWall)},
    {TRIGGER_TALK, ACTOR_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadSign)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Contigo_RevealMiddleDoor)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Contigo_HideMiddleDoor)},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_ShopsTriggers[] = {
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_SOUTH, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_SOUTH},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_SOUTHWEST, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_SOUTHWEST},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_B, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_B},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_EAST, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_EAST},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_A, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_A},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_WEST, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_WEST},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_ATTEKA_INLET_GEODE, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_ATTEKA_INLET_GEODE},
    {TRIGGER_TALK, ACTOR_MAN_ON_IVANS_RETURN, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheSiblingsWhoInheritedThe)},
    {TRIGGER_TALK, ACTOR_MAN_ON_IVANS_RETURN, CONDITION_ALWAYS, MSGNO(MsgContigoIGuessTheLighthouseWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_IVANS_RETURN, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheFamiliesOfChildrenWho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_IVANS_RETURN, CONDITION_ALWAYS, MSGNO(MsgContigoHammaWillStayWithUs)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_IVANS_MOTHER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoImSureTheYoungerBrother)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_IVANS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgContigoIvanDoesntRememberHisMother)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_IVANS_MOTHER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIvansMotherLeftHimIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_IVANS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgContigoIWonderHowIvanFeels)},
    {TRIGGER_TALK, ACTOR_BOY_WHO_WOULD_FIGHT_IVAN, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoMyFatherDoesntApproveOf)},
    {TRIGGER_TALK, ACTOR_BOY_WHO_WOULD_FIGHT_IVAN, CONDITION_ALWAYS, MSGNO(MsgContigoFromTheLookOnIvans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WHO_WOULD_FIGHT_IVAN, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIWishICouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WHO_WOULD_FIGHT_IVAN, CONDITION_ALWAYS, MSGNO(MsgContigoSomedayImGonnaFightIvan)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_HESPERIA, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoLongAgoThePeopleOf)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_HESPERIA, CONDITION_ALWAYS, MSGNO(MsgContigoBrrrBrrrItsBeenReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HESPERIA, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoForALongTimeNow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HESPERIA, CONDITION_ALWAYS, MSGNO(MsgContigoJupiterLighthouseHasBeenCalled)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_FEELING_THE_COLD, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoALongTimeAgoThe)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_FEELING_THE_COLD, CONDITION_ALWAYS, MSGNO(MsgContigoWinterJustEndedButIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_FEELING_THE_COLD, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoEverybodyThinksThePeopleOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_FEELING_THE_COLD, CONDITION_ALWAYS, MSGNO(MsgContigoHammaSaysThatIfIvan)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(Contigo_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoOhHowIWantTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgContigoNowThatTheWingsAreDone)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoSoonTheSiblingsWithThe)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, CONDITION_ALWAYS, MSGNO(MsgContigoTheLighthouseHasBeenLit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIJustKnowIvanBecame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, CONDITION_ALWAYS, MSGNO(MsgContigoIvanHasThePowerOf)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS_POWER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoThePowerOfTheAnemos)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS_POWER, CONDITION_ALWAYS, MSGNO(MsgContigoItsUpToThePeople)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS_POWER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoATerribleMisfortuneFellOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS_POWER, CONDITION_ALWAYS, MSGNO(MsgContigoNowThatIThinkAbout)},
    {TRIGGER_TALK, ACTOR_BABYS_BROTHER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIDontHaveAnyPowers)},
    {TRIGGER_TALK, ACTOR_BABYS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgContigoMomAndDadHaveBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABYS_BROTHER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWheneverMySisterFurrowsHer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABYS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgContigoItsTimeToFixThose)},
    {TRIGGER_TALK, ACTOR_BABY, CONDITION_ALWAYS, EVENT(Contigo_BabyThinks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABY, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoBawabaItsHotIWant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABY, CONDITION_ALWAYS, MSGNO(MsgContigoGooItsGettingColderGagoo)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(Contigo_TalkToWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoOfCourseTheyreHereTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgContigoIdSureLikeToSee)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(Contigo_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoNothingElseAroundHereIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgContigoWellIfThereArentGoing)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(Contigo_TalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoItsGoodToHaveThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoOnceAllOurCustomersLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgContigoItsGoingToBeA)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoAllTheWorkOnThe)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoFirstTheLighthouseIsLit)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgContigoTheInnHasBecomeSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoOnceTheWingsAreFinished)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoICantBelieveItAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgContigoTheInnIsBoundTo)},
    {TRIGGER_TALK, ACTOR_WOMAN_MAKING_THE_BEDS, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoGottaMakeAllTheBeds)},
    {TRIGGER_TALK, ACTOR_WOMAN_MAKING_THE_BEDS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheBossSaidItWas)},
    {TRIGGER_TALK, ACTOR_WOMAN_MAKING_THE_BEDS, CONDITION_ALWAYS, MSGNO(MsgContigoGoshWeWereSoBusy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MAKING_THE_BEDS, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIveGotSoMuchTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MAKING_THE_BEDS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoAllTheseYearsIThought)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MAKING_THE_BEDS, CONDITION_ALWAYS, MSGNO(MsgContigoWhatAreWeGoingTo)},
    {TRIGGER_TALK, ACTOR_COOK, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIfIDontMakeDinner)},
    {TRIGGER_TALK, ACTOR_COOK, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheyveFinishedPuttingTheWings)},
    {TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgContigoNowsMyChanceToExperiment)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIMustBeTheGreatest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoItWasKindOfImpressive)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgContigoSeeingThatShipTakeFlight)},
    {TRIGGER_TALK, ACTOR_WORN_OUT_WORKER, CONDITION_ALWAYS, MSGNO(MsgContigoDidYouJustArriveIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORN_OUT_WORKER, CONDITION_ALWAYS, MSGNO(MsgContigoIWorkedMyselfHalfTo)},
    {TRIGGER_TALK, ACTOR_WORKER_ON_THE_PROPHECIES, CONDITION_ALWAYS, MSGNO(MsgContigoThePropheciesOfTheAnemos)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER_ON_THE_PROPHECIES, CONDITION_ALWAYS, MSGNO(MsgContigoTheWingsOfAnemosAre)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_TOO_OLD_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgContigoImAfraidImTooOld)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_TOO_OLD_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgContigoIThrewMyBackOut)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE, CONDITION_ALWAYS, MSGNO(MsgContigoWhenIsThatMiracleOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE, CONDITION_ALWAYS, MSGNO(MsgContigoIWantToSeeThe)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Contigo_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIfISeeTheWings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoPerhapsIfISeeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgContigoItWasAShockTo)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_BEAN_SOUP, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoTheresABrightRedSoup))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_CORN_BREAD, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoAhTheSmellOfFresh))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_COOKIES, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoContigosFamousForTheseCookies))},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_ShopsReunionTriggers[] = {
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_NORTH, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_NORTH},
    {TRIGGER_TALK, ACTOR_HAMMA, CONDITION_ALWAYS, MSGNO(MsgContigoNoMatterWhatHappensYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMA, CONDITION_ALWAYS, MSGNO(MsgContigoMyHeartIsWithYou)},
    {TRIGGER_STEP, CELL_BEHAVIOR_INSIDE_HAMMAS_DOOR, FLAG_HAMMA_WELCOMED_PARTY, EVENT(Contigo_ReturnToNewHome)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_BEAN_SOUP, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoTheresABrightRedSoup))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_CORN_BREAD, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoAhTheSmellOfFresh))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_COOKIES, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoContigosFamousForTheseCookies))},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_LuckyDiceTriggers[] = {
    {TRIGGER_EXIT, CONTIGO_LUCKY_DICE_TO_CONTIGO_NORTHEAST, CONDITION_ALWAYS, CONTIGO_LUCKY_DICE_TO_CONTIGO_NORTHEAST},
    {TRIGGER_EXIT, CONTIGO_LUCKY_DICE_TO_CONTIGO_EAST, CONDITION_ALWAYS, CONTIGO_LUCKY_DICE_TO_CONTIGO_EAST},
    {TRIGGER_EXIT, CONTIGO_LUCKY_DICE_TO_CONTIGO_SOUTHWEST, CONDITION_ALWAYS, CONTIGO_LUCKY_DICE_TO_CONTIGO_SOUTHWEST},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, EVENT(Contigo_OfferSuperLuckyDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUPER_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoWhenTheyLoseTheyCall)},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouWantToLearn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUPER_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoItsNotAToughGame)},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_RULES_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadSuperLuckyDiceRules)},
    {TRIGGER_TALK, ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, CONDITION_ALWAYS, EVENT(Contigo_AskAboutSuperDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, CONDITION_ALWAYS, MSGNO(MsgContigoThisKidsGotUnluckyWritten)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_HI_LO_GAME, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouLoseTheHi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_HI_LO_GAME, CONDITION_ALWAYS, MSGNO(MsgContigoItsToughToKnowWhen)},
    {TRIGGER_TALK, ACTOR_WOMAN_WHO_LOSES_AT_HI_LO, CONDITION_ALWAYS, MSGNO(MsgContigoIAlmostAlwaysGetAt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_LOSES_AT_HI_LO, CONDITION_ALWAYS, MSGNO(MsgContigoICanWinICan)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, EVENT(Contigo_OfferLuckyDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoWinningOrLosingIsPure)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoReadTheSignIfYa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoTheRulesForLuckyDice)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_RULES_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyDiceRules)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_PAIRS_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyDicePairs)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_ATTENDANT, CONDITION_ALWAYS, EVENT(Contigo_TalkToWheelsAttendant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_WHEELS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoHmmIWonderIfThese)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_RULES_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyWheelsRules)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_PRIZE_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyWheelsPrizes)},
    {TRIGGER_TABLE_END},
};
