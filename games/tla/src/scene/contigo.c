/// @file
/// Contigo, the village below Jupiter Lighthouse, and the rooms of its
/// overlay: the village with its Teleport circle, its weeds for Cyclone and
/// the Djinn Salt and Shine; its houses, shops, inn and Sanctum, where Isaac's
/// party and Felix's are reunited (Contigo_TheReunion); the hall of Lucky
/// Dice, Super Lucky Dice and Lucky Wheels; and the Anemos geoglyph, where the
/// villagers build the wings, below the Anemos Sanctum's front.

#include "types.h"
#include "macros.h"
#include "window/message_argument.h"
#include "item_ids.h"
#include "system/main_routine_work.h"
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

/// The village's temp flag: the camera follows the leader across the whole
/// village, rather than its western part alone (Contigo_WidenCameraBounds).
#define FLAG_WHOLE_VILLAGE_IN_VIEW FLAG_SCENE_TEMP_00

/// The tile row of Hamma's portrait as she and Isaac speak at once in the
/// reunion (Contigo_TheReunion): two rows higher from the first patch on.
#if !TLA_PATCH_1
#define REUNION_HAMMA_PORTRAIT_Y 12
#else
#define REUNION_HAMMA_PORTRAIT_Y 10
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
  /// Who runs Super Lucky Dice, and who explains it beside him.
  ///@{
  ACTOR_SUPER_LUCKY_DICE_HOST = ACTOR_FIRST_PLACED,
  ACTOR_SUPER_LUCKY_DICE_GUIDE,
  ///@}
  /// The players: "This kid's got unlucky written all over his face" (his
  /// thought); "If you lose the hi-lo game, you don't win anything...";
  /// "I almost always get at least one pair..."
  ///@{
  ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS,
  ACTOR_OLD_MAN_ON_THE_HI_LO_GAME,
  ACTOR_WOMAN_WHO_LOSES_AT_HI_LO,
  ///@}
  /// The sign of Super Lucky Dice's rules.
  ACTOR_SUPER_LUCKY_DICE_SIGN,
  /// Who runs Lucky Dice, and who points to its sign.
  ///@{
  ACTOR_LUCKY_DICE_HOST,
  ACTOR_LUCKY_DICE_GUIDE,
  ///@}
  /// Lucky Dice's two signs of its rules.
  ///@{
  ACTOR_LUCKY_DICE_SIGN,
  ACTOR_SECOND_LUCKY_DICE_SIGN,
  ///@}
  ACTOR_LUCKY_WHEELS_ATTENDANT,
  /// Lucky Wheels' signs: its rules, and its prizes.
  ///@{
  ACTOR_LUCKY_WHEELS_SIGN,
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

void Actor_MoveToObject(s32 actor, s32 target);
void FieldCamera_WaitForArrival(void);
void Party_AddIsaacsParty(void);
void Contigo_SendToLuckyWheels(s32 mode);
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
#include "field/field_psynergy_work.h"

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
extern u16 gContigo_PaletteSlots[];
extern u16 gContigo_PaletteColors[];
extern u16 gContigo_ShimmerColors[];
extern u16 gContigo_CycleSlots[];
extern s32 gContigo_ShiverScript[];
extern u32 gContigo_SparkleTiles[];
void Contigo_FreedDjinniJoinsParty(void);
void Contigo_ExamineWall(void);
void Contigo_ReadSign(void);

void Contigo_ReadLuckyWheelsSign(void);

void Contigo_ReadLuckyDiceSign(void);
void Scene_ShowEntranceLocked(void);
void Contigo_AskAboutWindLegend(void);
void Contigo_BabyThinks(void);
void Contigo_TalkToWeaponMerchant(void);
void Contigo_TalkToArmorMerchant(void);

void Contigo_TalkToInnkeeper(void);

void Contigo_AskAboutSuperDice(void);
void Contigo_ExplainSuperDice(void);
void Contigo_ExplainLuckyDice(void);
void Contigo_ExplainLuckyDiceAgain(void);
void Contigo_TalkToWheelsAttendant(void);
void Contigo_ExplainLuckyWheels(void);
void Contigo_ListLuckyWheelsPrizes(void);
void Contigo_AskAboutGeoglyph(void);
void Contigo_TalkToDoubtingBoy(void);
void Contigo_TalkAboutTheShip(void);
void Contigo_ScatterFirstWeeds(void);
void Contigo_ScatterSecondWeeds(void);

void Contigo_ScatterWeedsOverBrambleSeed(void);

void Contigo_ScatterFourthWeeds(void);
void Contigo_ScatterFifthWeeds(void);
void Contigo_ScatterSixthWeeds(void);
void Contigo_ScatterSeventhWeeds(void);
void Contigo_ScatterEighthWeeds(void);
void Contigo_ScatterFirstWeedRow(void);
void Contigo_ScatterSecondWeedRow(void);
void Contigo_ScatterThirdWeedRow(void);
void Contigo_ScatterFourthWeedRow(void);
void Contigo_ScatterFifthWeedRow(void);
void Contigo_WidenCameraBounds(void);
void Contigo_NarrowCameraBounds(void);
void Contigo_ScoopForSalt(void);
void Contigo_RevealMiddleDoor(void);

void Contigo_ForceFreesDjinni(void);

void Contigo_SendToLuckyDice(void);
void Contigo_SendToSuperLuckyDice(void);
void Contigo_EnterSanctumWestDoor(void);
void Contigo_EnterSanctumEastDoor(void);
void Contigo_ReturnToNewHome(void);
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

/// Writes each listed background colour from its red, green and blue, keeps
/// the lists for the Teleport circle's shimmer, and saves the palettes as the
/// fade's source; arriving by Teleport, the party then arrives on the circle.
void Palette_SetColors(u16 *slots, u16 *colours, u16 *cycleColours,
                       u16 *cycleSlots) {
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;

  gPaletteSlots = slots;
  gPaletteColors = colours;
  gPaletteCycleColors = cycleColours;
  gPaletteCycleSlots = cycleSlots;
  while (*slots != PALETTE_LIST_END && *colours != PALETTE_LIST_END) {
    int index = *slots++;
    int red = *colours++;
    int green = *colours++;
    int blue = *colours++;
    SET_RGB(BG_PLTT[index], red, green, blue);
  }
  Dma_Set((const void *)BG_PLTT, fade->palette,
          DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
  Dma_Set(
      (const void *)OBJ_PLTT, &fade->palette[PALETTE_FADE_BG_COLORS],
      DMA_COPY_WORDS((ARRAY_COUNT(fade->palette) - PALETTE_FADE_BG_COLORS) / 2),
      REG_DMA3);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  if (gMainRoutineWork.teleportState == 2)
    Teleport_ArriveWhirling();
}

/// Starts the shimmer at its slow pace, as Teleport is cast: the first step
/// after 16 frames, each after a frame less, down to two.
void Teleport_StartShimmer(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_FRAME);
}

/// Stops the shimmer, leaving the next transition a backdrop fade.
void Teleport_StopShimmer(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Remove(PaletteCycle_Fade);
}

/// Starts the shimmer at a step every two frames, as the party arrives.
void Teleport_StartFastShimmer(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 2;
  gPaletteCycleTimer = 2;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_FRAME);
}

#include "shared/list_count_entries.h"

#include "shared/palette_cycle_fade.h"

/// The party arrives by Teleport on the circle: the village fades in grey
/// while the circle shimmers and the whirl plays, then its colours come back.
void Teleport_ArriveWhirling(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Teleport_StartFastShimmer();
  Teleport_PlayWhirl();
  wait(40);
  Teleport_StopShimmer();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#include "text/msg_ids.h"

void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

/// Salt, dug up in the patch of weeds, joins the party.
void Contigo_SaltJoinsParty(void) {
  Djinn_JoinParty(ACTOR_SALT, DJINNI_SALT / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SALT));
}

/// Shine, freed from the tree stump by Force, joins the party.
void Contigo_FreedDjinniJoinsParty(void) {
  Djinn_JoinParty(ACTOR_SHINE, DJINNI_SHINE / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SHINE));
  GameFlag_Set(FLAG_SHINE_CAUGHT);
}

#include "shared/position_distance.h"

/// 1 when `other` is nearer `self` than `range` pixels and lies in the
/// sixteenth of a turn `self` faces or in one either side of it.
s32 Actor_IsFacingNear(struct Actor *self, struct Actor *other, s32 range) {
  s32 result = 0;
  s32 *to;
  s32 *from;

  to = &other->x.fixed;
  from = &self->x.fixed;
  if (Position_Distance(to, from) < range) {
    u32 angle = (u16)Trig_Atan(other->z.fixed - self->z.fixed, *to - *from);
    u32 left = FACING_SIXTEENTH(angle - FACING_STEP);
    u32 right = FACING_SIXTEENTH(angle + FACING_STEP);
    u32 ahead = FACING_SIXTEENTH(angle);
    u32 facing = FACING_SIXTEENTH(self->facing);

    if (ahead == facing || right == facing || left == facing)
      result = 1;
  }
  return result;
}

void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 value);
s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// The update of the two children walking round the middle of the village:
/// each stands while it is talked to, while Mind Read shows its thoughts, or
/// while it faces the other child or the leader close by, and walks on
/// otherwise or once the leader is right by it.
s32 Contigo_PauseWhenFaced(struct Actor *self) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u8 *state;
  u32 mode;
  struct FieldEffectWork *effect;
  struct Actor *other;

  Actor_SetSpritePriorityDirect(
      self, BattleEffect_GetObjectPriority(
                Actor_GetById(gMainRoutineWork.leaderActorId)));
  mode = ABILITY_ID(event->psynergy);
  state = &self->paused;
  if (mode == ABILITY_MIND_READ) {
    effect = Ram_HeapSlots->fieldEffectWork;
    if ((s8)effect->messageShown != 0)
      goto stand;
  }
  if (self->riseEnabled == 1)
    goto stand;
  other = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (Position_Distance(&self->x.fixed, &other->x.fixed) <=
      CHILD_WALKS_ON_RANGE)
    goto walk;
  if (self->riseCounter == 1)
    other = Actor_GetById(ACTOR_GIRL_ON_THE_SHIP);
  else
    other = Actor_GetById(ACTOR_BOY_DOUBTING_THE_LEGENDS);
  if (Actor_IsFacingNear(self, other, CHILD_SEES_CHILD_RANGE) != 0)
    goto stand;
  if (Actor_IsFacingNear(self, Actor_GetById(gMainRoutineWork.leaderActorId),
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
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_CONTIGO)
    return gContigo_VillagePlacements;
  if (scene == SCENE_CONTIGO_GEOGLYPH)
    return gContigo_GeoglyphPlacements;
  if (scene == SCENE_CONTIGO_SHOPS) {
    if (gMainRoutineWork.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH) {
      if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
        return gContigo_ShopsPlacements;
      if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 &&
          GameFlag_Get(FLAG_REUNITED_AT_CONTIGO) == 0)
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
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_CONTIGO)
    return gContigo_VillageTriggers;
  if (scene == SCENE_CONTIGO_GEOGLYPH)
    return gContigo_GeoglyphTriggers;
  if (scene == SCENE_CONTIGO_SHOPS) {
    if (gMainRoutineWork.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH)
      return gContigo_ShopsReunionTriggers;
    return gContigo_ShopsTriggers;
  }
  if (scene == SCENE_CONTIGO_LUCKY_DICE)
    return gContigo_LuckyDiceTriggers;
  return gContigo_Triggers;
}

/// Examining the wall by the geoglyph: characters are carved into it.
void Contigo_ExamineWall(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix examined the wall. There are characters carved into it...."
  FieldMessage_Show((s32)MsgContigoFelixExaminedTheWallThere, 1);
  Event_End();
}

/// Reading the sign that translates the wall's characters.
void Contigo_ReadSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix examined the sign. The characters from the wall are translated..."
  FieldMessage_Show((s32)MsgContigoFelixExaminedTheSignThe, 1);
  Event_End();
}

/// Reading the sign to the Lucky Wheels.
void Contigo_ReadLuckyWheelsSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Turn the slot and get a prize! The lottery slots are over here."
  FieldMessage_Show((s32)MsgContigoTurnTheSlotAndGet, 1);
  Event_End();
}

/// Reading the sign to the Lucky Dice.
void Contigo_ReadLuckyDiceSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Drop some cash and double up! The hi-lo dice are over here!"
  FieldMessage_Show((s32)MsgContigoDropSomeCashAndDouble, 1);
  Event_End();
}

/// Reading the sign to the Super Lucky Dice.
void Contigo_ReadSuperDiceSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "You never know what the dice will do next! Come and give our dice a..."
  FieldMessage_Show((s32)MsgContigoYouNeverKnowWhatThe, 1);
  Event_End();
}

#include "shared/scene_show_entrance_locked.h"

/// A young man asks whether the party knows the legend of the people who could
/// fly on the wind.
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

/// Examining the baby: its brow furrows, a whirlwind grows over it and leaves
/// whirl up in it while the room dims, then it babbles and its family talk of
/// the house.
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
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(4, 4, 16), 1);
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
    i = 0;
    do {
      effect->scaleX += FIXED_ONE / 40;
      effect->scaleY += FIXED_ONE / 40;
      wait(1);
      i++;
    } while (i < 28);
  }
  Task_Add(Contigo_RaiseLeaves, TASK_PASS_FRAME);
  Event_Wait(120);
  Task_Remove(Contigo_RaiseLeaves);
  if (effect != 0) {
    i = 0;
    do {
      effect->scaleX -= FIXED_ONE / 40;
      effect->scaleY -= FIXED_ONE / 40;
      effect->y.fixed += toFIXED(1) / 2;
      wait(1);
      i++;
    } while (i < 28);
    Actor_Release(effect);
  }
  BattleEffect_SetObjectUpdateMode(Actor_GetById(ACTOR_BABY), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(16);
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
    Event_SetMessageId((s32)MsgContigoGooGooGaaGaaWaaa);
  else
    Event_SetMessageId((s32)MsgContigoBabuBawabaBibaw);
  Actor_RunRepeatedMotion(ACTOR_BABY, 2);
  // "Babu bawaba bibaw."
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

void Shop_Run(s32 a, s32 b);
void Sanctum_Run(s32 a);
void Inn_CheckIn(s32 a, s32 b);

#include "shared/leader_is_facing_north.h"

/// Contigo's shops, as Shop_Run numbers them.
enum {
  SHOP_CONTIGO_WEAPONS = 26,
  SHOP_CONTIGO_ARMOR = 27,
  SHOP_CONTIGO_ITEMS = 28
};

/// Contigo's inn, as Inn_CheckIn numbers the inns.
enum { CONTIGO_INN = 12 };

/// The item merchant: facing the counter opens the shop; otherwise she talks of
/// the wings.
void Contigo_TalkToItemMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_ITEMS, ACTOR_ITEM_MERCHANT);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      Event_SetMessageId((s32)MsgContigoItsAlwaysSuchALetdown);
    else
      Event_SetMessageId((s32)MsgContigoTheVillagersAreWorkingTheir);
    // "The villagers are working their hardest to complete the wings and..."
    Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
    Event_End();
  }
}

/// The weapon merchant: facing the counter opens the shop; otherwise he talks
/// of Atteka's traditions.
void Contigo_TalkToWeaponMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_WEAPONS, ACTOR_WEAPON_MERCHANT);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      Event_SetMessageId((s32)MsgContigoACannonYeahIThink);
    else
      Event_SetMessageId((s32)MsgContigoItWasALongstandingTradition);
    // "It was a longstanding tradition that people in Atteka would take a..."
    Event_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
    Event_End();
  }
}

/// The armor merchant: facing the counter opens the shop; otherwise she talks
/// of the mining in the ruins.
void Contigo_TalkToArmorMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_ARMOR, ACTOR_ARMOR_MERCHANT);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      Event_SetMessageId((s32)MsgContigoNowThatTheWingsAre);
    else
      Event_SetMessageId((s32)MsgContigoMiningMaterialsInTheRuins);
    // "Mining materials in the ruins is dangerous work, so my armor is..."
    Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
    Event_End();
  }
}

/// The innkeeper: facing the counter checks in; otherwise she talks of the
/// visitors and the wings.
void Contigo_TalkToInnkeeper(void) {
  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(CONTIGO_INN, ACTOR_INNKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      Event_SetMessageId((s32)MsgContigoAllOfOurWorkAnd);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT))
      Event_SetMessageId((s32)MsgContigoYouHaveToHurryI);
    else
      Event_SetMessageId((s32)MsgContigoWeveHadSoManyPeople);
    // "We've had so many people coming to Contigo lately that we're full..."
    Event_ShowMessageAndWait(ACTOR_INNKEEPER, 0);
    Event_End();
  }
}

/// The healer: facing the counter opens the Sanctum; otherwise he talks of the
/// miracle of the wings.
void Contigo_TalkToHealer(void) {
  if (Leader_IsFacingNorth()) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      Event_SetMessageId((s32)MsgContigoItWasAMiracleThat);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT))
      Event_SetMessageId((s32)MsgContigoAFlyingShipSurelyIt);
    else
      Event_SetMessageId((s32)MsgContigoTheRealMiracleWillBe);
    // "The real miracle will be if these wings actually make a boat fly...."
    Event_ShowMessageAndWait(ACTOR_HEALER, 0);
    Event_End();
  }
}

/// A player in the dice hall asks whether the party has played Super Lucky Dice
/// before.
void Contigo_AskAboutSuperDice(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoSoTellMePalHave);
  // "So, tell me, pal... Have you ever played Superdice before?"
  Event_AskYesNoAndShowResponse(ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, 0);
  Event_End();
}

/// Reading the sign of Super Lucky Dice's rules.
void Contigo_ExplainSuperDice(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoSuperLuckyDiceTheRules;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  FieldMessage_Show(msg + 6, 1);
  FieldMessage_Show(msg + 7, 1);
  FieldMessage_Show(msg + 8, 1);
  Event_End();
}

/// Reading the first sign of Lucky Dice's rules.
void Contigo_ExplainLuckyDice(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoLuckyDice;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  Event_End();
}

/// Reading the second sign of Lucky Dice's rules.
void Contigo_ExplainLuckyDiceAgain(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoLuckyDiceB;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  Event_End();
}

/// Talking to the Lucky Wheels attendant (Contigo_SendToLuckyWheels).
void Contigo_TalkToWheelsAttendant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Contigo_SendToLuckyWheels(0);
  Event_End();
}

/// Reading the sign of Lucky Wheels' rules.
void Contigo_ExplainLuckyWheels(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoLuckyWheelsRules;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  Event_End();
}

/// Reading the sign of Lucky Wheels' prizes.
void Contigo_ListLuckyWheelsPrizes(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigoLuckyWheelsPrizes;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  // The Spanish sign ends after four messages.
#if LANGUAGE != LANGUAGE_SPANISH
  FieldMessage_Show(msg + 4, 1);
#endif
  Event_End();
}

/// A man in a long robe asks whether the party has looked at the Anemos
/// geoglyph.
void Contigo_AskAboutGeoglyph(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoHaveYouTakenAGood);
  // "Have you taken a good look at the Anemos geoglyph?"
  Event_AskYesNoAndShowResponse(ACTOR_MAN_ON_THE_GEOGLYPH, 0);
  Event_End();
}

/// The boy walking with the girl talks of the geoglyphs and of the city of the
/// Anemos, which he will not believe flew, turning to the leader unless Mind
/// Read is cast on him, and back after.
void Contigo_TalkToDoubtingBoy(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_GetById(ACTOR_BOY_DOUBTING_THE_LEGENDS);
  s16 angle = actor->facing;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->riseEnabled = 1;
  if (work->psynergy == 0) {
    Actor_FaceActorById(ACTOR_BOY_DOUBTING_THE_LEGENDS,
                        gMainRoutineWork.leaderActorId, 0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      Event_SetMessageId((s32)MsgContigoIveSeenALotOf);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0)
      Event_SetMessageId((s32)MsgContigoThatCraterBehindTheVillage);
    else
      Event_SetMessageId((s32)MsgContigoIfYouWantToGet);
  } else {
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      Event_SetMessageId((s32)MsgContigoWaitTheCityOfAnemos);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0)
      Event_SetMessageId((s32)MsgContigoCitiesFlyingUpIntoThe);
    else
      Event_SetMessageId((s32)MsgContigoTheAnemosGeoglyphsAreThe);
  }
  Event_ShowMessageAndWait(ACTOR_BOY_DOUBTING_THE_LEGENDS, 0);
  if (work->psynergy == 0)
    actor->facing = angle;
  actor->riseEnabled = 0;
  Event_End();
}

/// The girl by the ship talks of the wings, turning to the leader unless Mind
/// Read is cast on her, and back after.
void Contigo_TalkAboutTheShip(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_GetById(ACTOR_GIRL_ON_THE_SHIP);
  s16 angle = actor->facing;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->riseEnabled = 1;
  if (event->psynergy == 0) {
    Actor_FaceActorById(ACTOR_GIRL_ON_THE_SHIP, gMainRoutineWork.leaderActorId,
                        0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      Event_SetMessageId((s32)MsgContigoItWasntWhatIExpected);
    else
      Event_SetMessageId((s32)MsgContigoOurPeopleHaveLaboredFor);
  } else {
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      Event_SetMessageId((s32)MsgContigoAllOfThoseLegendsDid);
    else
      Event_SetMessageId((s32)MsgContigoWeBuiltTheWingsOf);
  }
  Event_ShowMessageAndWait(ACTOR_GIRL_ON_THE_SHIP, 0);
  if (event->psynergy == 0)
    actor->facing = angle;
  actor->riseEnabled = 0;
  Event_End();
}

void Teleport_PlaySparks(void);

/// Teleport on the circle: it shimmers, the sparks play, and the party leaves
/// for the Anemos Inner Sanctum, to arrive there by Teleport.
void Contigo_TeleportOut(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();

  gMainRoutineWork.teleportState = 2;
  Event_SetDestination(SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST,
                       ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_FROM_CONTIGO);
  Event_Wait(40);
  Teleport_StopShimmer();
}

/// Cyclone scatters a clump of weeds, as each clump and each row of the weed
/// patch south-east of the village below does with its own cells.
void Contigo_ScatterFirstWeeds(void) {
  FieldTileCopies_CopyTouchedCells(3, 50, 1, 1, 8, 54, 0);
}

/// Cyclone scatters the second clump of weeds.
void Contigo_ScatterSecondWeeds(void) {
  FieldTileCopies_CopyTouchedCells(4, 43, 2, 1, 2, 43, 0);
}

/// The scene's own name for TileCopies_CopyTouchedCells.
static __inline__ void Contigo_CopyTouchedCells(s32 sourceX, s32 sourceZ,
                                                s32 width, s32 height,
                                                s32 windowX, s32 windowZ,
                                                s32 layer) {
  TileCopies_CopyTouchedCells(sourceX, sourceZ, width, height, windowX, windowZ,
                              layer);
}

/// Cyclone scatters the weeds over the Bramble Seed: unless it was taken, the
/// seed is shown, and a leader standing on its spot hops back off it.
void Contigo_ScatterWeedsOverBrambleSeed(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Contigo_CopyTouchedCells(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Get(FLAG_CONTIGO_BRAMBLE_SEED_TAKEN) == 0) {
    Actor_SetPosition(ACTOR_BRAMBLE_SEED, 280, 664);
    if (FIELD_CELL(leader->x.fixed) == 17 &&
        FIELD_CELL(leader->z.fixed) == 41) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -16);
      leader->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone scatters the fourth clump of weeds.
void Contigo_ScatterFourthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(29, 35, 1, 2, 23, 47, 0);
}

/// Cyclone scatters the fifth clump of weeds.
void Contigo_ScatterFifthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 2, 3, 29, 58, 0);
}

/// Cyclone scatters the sixth clump of weeds.
void Contigo_ScatterSixthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(3, 60, 1, 1, 3, 56, 0);
}

/// Cyclone scatters the seventh clump of weeds.
void Contigo_ScatterSeventhWeeds(void) {
  FieldTileCopies_CopyTouchedCells(7, 62, 2, 1, 7, 58, 0);
}

/// Cyclone scatters the eighth clump of weeds.
void Contigo_ScatterEighthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(10, 63, 2, 1, 10, 59, 0);
}

/// Cyclone scatters the first row of the weed patch.
void Contigo_ScatterFirstWeedRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 3, 1, 40, 57, 0);
}

/// Cyclone scatters the second row of the weed patch.
void Contigo_ScatterSecondWeedRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 58, 0);
}

/// Cyclone scatters the third row of the weed patch.
void Contigo_ScatterThirdWeedRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 59, 0);
}

/// Cyclone scatters the fourth row of the weed patch.
void Contigo_ScatterFourthWeedRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 60, 0);
}

/// Cyclone scatters the fifth row of the weed patch.
void Contigo_ScatterFifthWeedRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 3, 1, 40, 61, 0);
}

/// Lets the camera follow the leader across the whole village, held by a scene
/// flag.
void Contigo_WidenCameraBounds(void) {
  Field_SetCameraBounds(0, toFIXED(512), toFIXED(880), toFIXED(1024));
  GameFlag_Set(FLAG_WHOLE_VILLAGE_IN_VIEW);
}

/// Holds the camera to the village's western part again.
void Contigo_NarrowCameraBounds(void) {
  Field_SetCameraBounds(0, toFIXED(512), toFIXED(512), toFIXED(1024));
  GameFlag_Clear(FLAG_WHOLE_VILLAGE_IN_VIEW);
}

/// The trigger Scoop raises once Salt has appeared: it joins the party.
enum { RAISED_SALT_FOUND = 200 };

/// Scoop in the gap in the middle of the weed patch: until the party has Salt,
/// Salt appears there and turns to the leader.
void Contigo_ScoopForSalt(void) {
  if (!GameFlag_Get(FLAG_DJINNI(DJINNI_SALT))) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    Event_Start();
    Event_ResolvePendingActions(1);
    Actor_SetPosition(ACTOR_SALT, 664, 952);
    wait(1);
    Actor_FaceActorById(ACTOR_SALT, gMainRoutineWork.leaderActorId, 0);
    work->raisedTrigger = RAISED_SALT_FOUND;
    Event_End();
  }
}

/// Reveal at the Sanctum's front: redraws the head of its middle steps and
/// puts the way in to the Anemos Inner Sanctum's entrance (cell code 14, kept
/// out of reach at the top of the map) at their foot.
void Contigo_RevealMiddleDoor(void) {
  MapGrid_CopyCellsTo(27, 4, 48, 12, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(27, 6, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 1, 2, 1, 48, 19);
}

/// As Reveal fades, puts the middle steps and the cells at their foot back.
void Contigo_HideMiddleDoor(void) {
  MapGrid_CopyCellsTo(27, 8, 48, 12, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(27, 10, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 0, 2, 1, 48, 19);
}

/// Force on the tree stump where Shine is stuck: the ground shakes, a ring of
/// dust bursts out, and Shine leaps free, the leader hopping aside if it lands
/// on him.
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
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (FIELD_CELL(actor->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(actor->z.fixed) == FIELD_CELL(leader->z.fixed)) {
    Actor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 68, 680);
  }
  MapGrid_SetShake(0, 0, 0);
  Actor_TurnToFaceEachOther(gMainRoutineWork.leaderActorId, ACTOR_SHINE, 0);
  Event_Wait(10);
  GameFlag_Set(FLAG_SHINE_FREED);
  Event_End();
}

/// Once the beacon is lit, while everyone has left town with the wings: shuts
/// the doors of two houses, the item shop and the weapon and armor shop, and
/// locks them (cell code 19, Scene_ShowEntranceLocked).
void Contigo_LockDoors(void) {
  MapGrid_CopyCellsTo(0, 1, 26, 42, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(0, 1, 26, 48, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(0, 1, 11, 54, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(0, 1, 16, 54, 1, 2);
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

extern void MessageArgument_Push(s32 value, s32 digits);
void Event_LeaveFor(s32 scene, s32 entrance);

/// The lucky wheels attendant: checks the game tickets and free item slots,
/// then sends the player in.
void Contigo_SendToLuckyWheels(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(ITEM_GAME_TICKET);
  s32 room;

  ObjectTable_Snapshot();
  if (spoken == 0) {
    s32 message = (s32)MsgContigoStepRightUpAndTry;

    Event_SetMessageId(message);
    Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    if (tickets == 0) {
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      return;
    }
    Event_SetMessageId(message + 2);
    MessageArgument_Push(tickets, MESSAGE_ARGUMENT_NUMBER);
    FieldMessage_ShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        1) {
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      return;
    }
    room = PartyInventory_CountFreeSlots();
    if (room == 0) {
      Event_SetMessageId(message + 4);
      FieldMessage_ShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    } else if (room <= 6) {
      Event_SetMessageId(message + 5);
      FieldMessage_ShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    }
    if (room <= 6 && Event_AskYesNoWithActorAnimation(
                         gMainRoutineWork.leaderActorId, 0) == 1) {
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
s32 Contigo_PickPrize(s32 icon) {
  struct MainRoutineWork *state;
  s32 cursor;
  s32 variant;
  s32 index;

  if (icon < 0)
    return 0;
  if (icon == WHEELS_ICON_MOON)
    icon = (s32)(Random_Below(WHEELS_PRIZE_ICONS));
  state = &gMainRoutineWork;
  cursor = state->wheelsPrizeCursors[icon];
  variant = (cursor + (s32)(Random_Below(2)) + 4) % WHEELS_PRIZES_PER_ICON;
  state->wheelsPrizeCursors[icon] = variant;
  index = icon * 2;

  return gContigo_WheelsPrizes[index + icon + variant];
}

extern u8 gSceneState[];

/// The dice games cost this many coins for each level of the party's average
/// level; a win of LUCKY_DICE_MEDIUM_WIN coins or more, or of
/// LUCKY_DICE_LARGE_WIN, is greeted with a bigger jingle.
enum {
  LUCKY_DICE_COINS_PER_LEVEL = 10,
  LUCKY_DICE_MEDIUM_WIN = 5000,
  LUCKY_DICE_LARGE_WIN = 20000
};

/// The dice game attendant: the fee is ten coins a level.
void Contigo_SendToLuckyDice(void) {
  u32 price = Party_GetAverageLevel() * LUCKY_DICE_COINS_PER_LEVEL;

  Event_Start();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineWork.coins < price) {
    Event_SetMessageId((s32)MsgContigoHeyYaDontHaveEnough);
    // "Hey! Ya don't have enough coins! Ya gotta pay to play."
    FieldMessage_ShowAtActor(ACTOR_LUCKY_DICE_HOST, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigoHeyBuddyWannaTryYer);
  MessageArgument_Push(price, MESSAGE_ARGUMENT_NUMBER);
  // "Hey, buddy, wanna try yer luck at the dice? It's coin to play." Yes: "OK!
  // Then take the dice." No: "No? Oh, well. Who's next?"
  FieldMessage_ShowAtActor(ACTOR_LUCKY_DICE_HOST, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    /* FAKEMATCH(pin): the scene state's address fixed to a register. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gMainRoutineWork.coins;
    ObjectTable_Snapshot();
    // "OK! Then take the dice."
    Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_HOST, 0);
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
    GameFlag_Set(FLAG_LEFT_FOR_A_GAME);
    wait(1);
    Event_LeaveFor(SEQUENCE_DICE, 0);
    Event_SetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE,
                                    ENTRANCE_FROM_LUCKY_DICE);
  } else {
    gFieldWork->message++;
    // "No? Oh, well. Who's next?"
    Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_HOST, 0);
  }
  Event_End();
}

/// The Super Lucky Dice attendant: the same check without the fee shown.
void Contigo_SendToSuperLuckyDice(void) {
  u32 price = Party_GetAverageLevel() * LUCKY_DICE_COINS_PER_LEVEL;

  Event_Start();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineWork.coins < price) {
    Event_SetMessageId((s32)MsgContigoSorryPalButYouveGotta);
    // "Sorry, pal, but you've gotta have more coins to play."
    FieldMessage_ShowAtActor(ACTOR_LUCKY_DICE_HOST, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigoWelcomeWelcomeStepRightUp);
  // "Welcome, welcome, step right up! Care for a round of Super Lucky Dice?"
  // Yes: "OK, then let 'em roll!" No: "Oh really? That's a shame!"
  FieldMessage_ShowAtActor(ACTOR_SUPER_LUCKY_DICE_HOST, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    /* FAKEMATCH(pin): the scene state's address fixed to a register. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gMainRoutineWork.coins;
    ObjectTable_Snapshot();
    // "OK, then let 'em roll!"
    Event_ShowMessageAndWait(ACTOR_SUPER_LUCKY_DICE_HOST, 0);
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
    GameFlag_Set(FLAG_LEFT_FOR_A_GAME);
    wait(1);
    Event_LeaveFor(SEQUENCE_SUPER_DICE, 0);
    Event_SetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE,
                                    ENTRANCE_FROM_SUPER_LUCKY_DICE);
  } else {
    gFieldWork->message++;
    // "Oh really? That's a shame!"
    Event_ShowMessageAndWait(ACTOR_SUPER_LUCKY_DICE_HOST, 0);
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

/// Opens one of the doors at the head of the Sanctum's steps, two cells of map
/// edits, and walks the leader in through it.
void Contigo_OpenSanctumDoor(s32 sourceX, s32 sourceY, s32 doorX, s32 doorY) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_THUMP);
  {
    s32 width = 1;
    s32 height = 2;

    MapGrid_CopyCellPictureIndicesRect(sourceX, sourceY, doorX, doorY, width,
                                       height);
    MapGrid_CopyCellPictureIndicesRect(sourceX, sourceY + 2, doorX + 1, doorY,
                                       width, height);
  }
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
                            gContigo_EnterSanctumDoorScript);
  wait(12);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Walking into the Sanctum's west door: it opens onto the Anemos Inner
/// Sanctum's entrance.
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

/// Walking into the Sanctum's east door: it opens onto the Anemos Inner
/// Sanctum's entrance.
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
struct SparkleWork gContigo_SparkleWork BSS_DATA ALIGNED(16);
void Contigo_StartSparkle(struct Actor *actor, struct SparkleWork *work);
void Contigo_StopSparkle(struct Actor *actor);

/// The reunion scene. It opens with "Glad you kept your promise".
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
  Actor_WatchAndSetScript(ACTOR_ISAAC, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_GARET, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_MIA, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_IVAN, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_HAMMA, gMainRoutineWork.leaderActorId);
  Event_Wait(10);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 160, 382);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetMovementTypeById(ACTOR_JENNA, gContigo_JennaReunionScript);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gContigo_KradenReunionScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gContigo_ShebaReunionScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS, gContigo_PiersReunionScript);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  Actor_Stop(ACTOR_HAMMA);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Sound_Play(MUS_WORLD_MAP_REUNITED);
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
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
  if (answer == 0) {
    Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
    Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    gFieldWork->message++;
  } else {
    Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
    gFieldWork->message++;
    Actor_SetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
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
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 20);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 80);
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHEAST);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | 1, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
  if (answer == 0) {
    Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    gFieldWork->message++;
  } else {
    Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    gFieldWork->message++;
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
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
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_HAMMA);
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
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_ISAAC);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  Sound_Play(MUS_REUNION);
  Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_ISAAC, 5, 5, 8, 10, ACTOR_HAMMA, 9,
                                    20, 18, REUNION_HAMMA_PORTRAIT_Y, 10);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
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
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_HAMMA, FACING_EAST);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_WalkToAndWait(ACTOR_IVAN, 130, 400);
  Actor_FaceDirectionById(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMA, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMA, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHEAST);
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Contigo_StartSparkle(actor, &gContigo_SparkleWork);
  Event_Wait(80);
  Contigo_StopSparkle(actor);
  Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Contigo_StartSparkle(actor, &gContigo_SparkleWork);
  Event_Wait(80);
  Contigo_StopSparkle(actor);
  Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAttachedEffect(ACTOR_ISAAC,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
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
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_Jump(ACTOR_KRADEN, 4, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_NOD);
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
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHWEST);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
  FieldMessage_ShowAtActor(ACTOR_JENNA, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    gFieldWork->message++;
  } else {
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
    gFieldWork->message++;
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  }
  Actor_WalkToAndWait(ACTOR_KRADEN, 146, 368);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
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
  GameFlag_Set(FLAG_REUNITED_AT_CONTIGO);
  Event_End();
}

extern u32 gContigo_LeaderVisitsScript[];

/// The party's first step into Hamma's house after it set out with the wings:
/// she welcomes it to her new home, tells Ivan why she kept from him that she
/// is his sister, and warns that the world is falling into ruin unless Mars
/// Lighthouse is lit.
void Contigo_ReturnToNewHome(void) {
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0.8);
    FieldCamera_MoveTo(160, CAMERA_KEEP, 368, 1);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 158, 394);
    Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST, 0);
    Actor_MoveToObject(ACTOR_IVAN, gMainRoutineWork.leaderActorId);
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
    Actor_MoveToObject(ACTOR_ISAAC, gMainRoutineWork.leaderActorId);
    wait(1);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_SetSpeed(ACTOR_ISAAC, 1.2);
    Actor_WalkToAndWaitById(ACTOR_ISAAC, 168, 380);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
    // "What? Hamma, what do you mean? What's happening?"
    Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
    FieldActor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
    Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
    wait(1);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
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
    FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
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
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 40);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
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
    FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
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
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST,
                            40);
    FieldEffect_SetAttached(0, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(20);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
    // "But what will happen if Mars Lighthouse isn't lit quickly enough?"
    Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
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
    FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
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
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        0) {
      Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
      Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
      // "I follow you..."
      Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
      gFieldWork->message++;
    } else {
      Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
      gFieldWork->message++;
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
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        0) {
      Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
      Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
      // "Yeah, it is awfully cold..."
      Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
      gFieldWork->message++;
    } else {
      Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
      gFieldWork->message++;
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
    Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
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
  s32 scene = gMainRoutineWork.next;

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
    struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);

    Actor_SetSpritePriority(ACTOR_CORN,
                            ((struct FieldSprite *)owner->sprite)->priority);
  }
}

/// Sets up the village: the two walking children stop when faced
/// (Contigo_PauseWhenFaced); once the beacon is lit, until the party sets out
/// with the wings, the girl and the old woman are gone and four doors are
/// locked; Shine waits where it landed until caught; arriving by four ways in
/// from the south, the camera follows the leader across the whole village;
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
    switch (gMainRoutineWork.entrance) {
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
    FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  Palette_SetColors(gContigo_PaletteSlots, gContigo_PaletteColors,
                    gContigo_ShimmerColors, gContigo_CycleSlots);
}

/// Sets up the geoglyph: before the beacon is lit the wings' parts lie by it;
/// after, they and their workers are gone, the men in long robes too until the
/// party sets out.
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

/// Sets up the shops' room: the reunion with Isaac's party on its entrance, or
/// the baby and the villagers by the progress of the story.
void Contigo_InitializeShops(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (gMainRoutineWork.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH) {
    if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 &&
        GameFlag_Get(FLAG_REUNITED_AT_CONTIGO) == 0)
      Contigo_TheReunion();
  } else if (gMainRoutineWork.entrance ==
             CONTIGO_SHOPS_FROM_ATTEKA_INLET_GEODE) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  } else {
    Actor_SetAnimationById(ACTOR_BABY, 2);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_BABY), 0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      Actor_SetMovementTypeById(ACTOR_OLD_MAN_ON_HESPERIA,
                                gContigo_ShiverScript);
    if (GameFlag_Get(FLAG_REUNITED_AT_CONTIGO) != 0) {
      Actor_SetPosition(ACTOR_WORN_OUT_WORKER, 0, 0);
      Actor_SetPosition(ACTOR_WORKER_ON_THE_PROPHECIES, 0, 0);
      Actor_SetPosition(ACTOR_OLD_MAN_TOO_OLD_TO_HELP, 0, 0);
      Actor_SetPosition(ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE, 0, 0);
    }
  }
}

// The lines the prize counter speaks: the Spanish text numbers them
// differently.
#if LANGUAGE == LANGUAGE_SPANISH
#define COINS_WON_MESSAGE ((s32)MsgContigoHeyYaDontHaveEnough + 1)
#define COINS_LOST_MESSAGE ((s32)MsgContigoHeyYaDontHaveEnough + 2)
#define READ_SIGN_MESSAGE ((s32)MsgContigoYaWonCoinTalkAbout)
#define CONGRATULATIONS_MESSAGE ((s32)MsgContigoYaLostCoinBetterLuck)
#define FIRST_PRIZE_MESSAGE ((s32)MsgContigoTheRulesAreListedOn)
#define NEXT_PRIZE_MESSAGE ((s32)MsgContigoCongratulations)
#define COME_AGAIN_MESSAGE ((s32)MsgContigoComeAgain)
#else
#define COINS_WON_MESSAGE ((s32)MsgContigoYaWonCoinTalkAbout)
#define COINS_LOST_MESSAGE ((s32)MsgContigoYaLostCoinBetterLuck)
#define READ_SIGN_MESSAGE ((s32)MsgContigoTheRulesAreListedOn)
#define CONGRATULATIONS_MESSAGE ((s32)MsgContigoCongratulations)
#define FIRST_PRIZE_MESSAGE ((s32)MsgContigoYouWonHereYouGo)
#define NEXT_PRIZE_MESSAGE ((s32)MsgContigoAndYouWonHereYou)
#define COME_AGAIN_MESSAGE ((s32)MsgContigoComeAgain)
#endif

void ItemDisplay_ShowItem(s32 item, s32 count);

/// Sets up the dice hall: its small hosts and their colours, and on the party's
/// return from a game the coins won or lost and the prizes.
void Contigo_InitializeLuckyDice(void) {
  s32 diff;
  s8 *list;
  s8 *prize;
  s32 item;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Contigo_ShrinkActor(ACTOR_SUPER_LUCKY_DICE_SIGN);
  Contigo_ShrinkActor(ACTOR_LUCKY_DICE_SIGN);
  Contigo_ShrinkActor(ACTOR_SECOND_LUCKY_DICE_SIGN);
  Contigo_ShrinkActor(ACTOR_LUCKY_WHEELS_SIGN);
  Contigo_ShrinkActor(ACTOR_LUCKY_WHEELS_PRIZE_SIGN);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_LUCKY_DICE_HOST), SPRITE_COLOR_GRB);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_LUCKY_DICE_GUIDE), SPRITE_COLOR_RBG);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SUPER_LUCKY_DICE_HOST),
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SUPER_LUCKY_DICE_GUIDE),
                       SPRITE_COLOR_BGR);
  if (!Event_GetFlag(FLAG_LEFT_FOR_A_GAME))
    return;
  GameFlag_Clear(FLAG_LEFT_FOR_A_GAME);
  if (gMainRoutineWork.entrance == ENTRANCE_FROM_SUPER_LUCKY_DICE) {
    diff = gMainRoutineWork.coins - *(s32 *)gSceneState;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (diff > 0) {
      if (diff >= LUCKY_DICE_LARGE_WIN)
        Sound_Play(MUS_JINGLE_LARGE);
      else if (diff >= LUCKY_DICE_MEDIUM_WIN)
        Sound_Play(MUS_JINGLE_MEDIUM);
      else
        Sound_Play(MUS_JINGLE_SMALL);
      Event_Wait(20);
      Event_SetMessageId(COINS_WON_MESSAGE);
      MessageArgument_Push(diff, MESSAGE_ARGUMENT_NUMBER);
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_HOST, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(COINS_LOST_MESSAGE);
      MessageArgument_Push(-diff, MESSAGE_ARGUMENT_NUMBER);
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_HOST, 0);
    }
    Event_End();
  } else if (gMainRoutineWork.entrance == ENTRANCE_FROM_LUCKY_DICE) {
    diff = gMainRoutineWork.coins - *(s32 *)gSceneState;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (diff > 0) {
      if (diff >= LUCKY_DICE_LARGE_WIN)
        Sound_Play(MUS_JINGLE_LARGE);
      else if (diff >= LUCKY_DICE_MEDIUM_WIN)
        Sound_Play(MUS_JINGLE_MEDIUM);
      else
        Sound_Play(MUS_JINGLE_SMALL);
      Event_Wait(20);
      Event_SetMessageId(COINS_WON_MESSAGE);
      MessageArgument_Push(diff, MESSAGE_ARGUMENT_NUMBER);
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_HOST, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(COINS_LOST_MESSAGE);
      MessageArgument_Push(-diff, MESSAGE_ARGUMENT_NUMBER);
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_HOST, 0);
    }
    Event_End();
  } else if (gMainRoutineWork.entrance == ENTRANCE_FROM_LUCKY_WHEELS) {
    list = gMainRoutineWork.wheelsPrizes;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    if (list[0] == -1) {
      Contigo_SendToLuckyWheels(1);
    } else if (list[0] == -2) {
      Event_SetMessageId(READ_SIGN_MESSAGE);
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    } else {
      Event_SetMessageId(CONGRATULATIONS_MESSAGE);
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      if (list[0] != -1) {
        prize = list;
        do {
          if (prize == list)
            Event_SetMessageId(FIRST_PRIZE_MESSAGE);
          else
            Event_SetMessageId(NEXT_PRIZE_MESSAGE);
          item = Contigo_PickPrize(*prize);
          MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
          Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
          ItemDisplay_ShowItem(item, 3);
          PartyInventory_GiveItem(item, 0);
          Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
          prize++;
        } while (*prize != -1);
      }
      gMainRoutineWork.wheelsPrizes[0] = -2;
      Event_SetMessageId(COME_AGAIN_MESSAGE);
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    }
    Event_End();
  }
}

void Actor_SetSpriteColorMode(struct Actor *, u32);
s32 BattleEffect_GetObjectDepth(struct Actor *actor);

/// The sparkle's update: raises two pairs of sprites over the actor, a pair
/// starting every 16 frames, until both have climbed RING_CLIMB pixels.
void Contigo_RaiseSparkle(struct Actor *actor) {
  struct Vector3 position;
  s32 x, z;
  FIXED sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)actor->watchedActor;
  u8 *count = &actor->riseEnabled;
  struct EffectSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(actor,
                             (u8)((u32)((struct CounterBits *)count)->rest %
                                  (SPRITE_COLOR_BRG + 1)));
  if (actor->riseCounter == 0) {
    if ((*count & (RING_INTERVAL - 1)) == 0) {
      Sound_Play(SE_RISING_RINGS);
      work->heights[work->nextPair] = 0;
      work->nextPair ^= 1;
    }
  } else if (actor->riseCounter == 1) {
    if (work->heights[0] >= RING_CLIMB && work->heights[1] >= RING_CLIMB) {
      Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
      VramCache_FreeEntry(work->vramEntry);
      actor->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    // Its size: half a turn of sine over the climb.
    sine = Trig_Sin(work->heights[i] * (DEGtoANG(180) / RING_CLIMB));
    if (work->heights[i] >= 0) {
      if (work->heights[i] < RING_CLIMB) {
        position.x = actor->x.fixed;
        position.y = actor->y.fixed + toFIXED(work->heights[i]);
        position.z = actor->z.fixed;
        Camera_WorldToScreen(&position);
        x = position.x;
        z = position.z;
        sprite = &work->sprites[i * 2];
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
        work->heights[i]++;
      }
    }
  }
}
#undef COINS_WON_MESSAGE
#undef COINS_LOST_MESSAGE
#undef READ_SIGN_MESSAGE
#undef CONGRATULATIONS_MESSAGE
#undef FIRST_PRIZE_MESSAGE
#undef NEXT_PRIZE_MESSAGE
#undef COME_AGAIN_MESSAGE

/// Sets up the sparkle: loads its picture and its four 16 by 16 sprites, and
/// hands the object to its update, Contigo_RaiseSparkle.
void Contigo_StartSparkle(struct Actor *actor, struct SparkleWork *work) {
  struct EffectSprite *sprite;
  s32 tile;
  s32 row;
  s32 col;

  work->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(work->vramEntry, 256, gContigo_SparkleTiles);
  work->nextPair = 0;
  work->heights[0] = RING_NOT_STARTED;
  work->heights[1] = RING_NOT_STARTED;
  work->actor = actor;
  actor->riseCounter = 0;
  actor->riseEnabled = 0;
  actor->update = Contigo_RaiseSparkle;
  tile = gVramCacheEntries[work->vramEntry].offset >> 5;
  actor->watchedActor = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    for (col = 1, sprite = work->sprites + row + row; col >= 0; col--) {
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
/// The sparkle's picture at 4 bits a pixel, eight rows of a tile to every two
/// lines: a white arc, half a ring 32 by 16, which Contigo_StartSparkle loads
/// for its four sprites. Unused: the arc again in colour 4, and three 8 by 8
/// sparkles at 8 bits a pixel (a four-pointed star, a cross and a diamond).
u32 gContigo_SparkleTiles[] = {
    0, 0, 0, 0,
    0, 0, 0x70000000, 0xf7700000,
    0, 0, 0, 0,
    0x77770000, 0xffff7770, 0xfffffff7, 0x7777ffff,
    0, 0, 0, 0,
    0x00007777, 0x0777ffff, 0x7fffffff, 0xffff7777,
    0, 0, 0, 0,
    0, 0, 0x00000007, 0x0000077f,
    0xfff70000, 0xffff7000, 0x7ffff700, 0x07ffff70,
    0x007fff70, 0x007ffff7, 0x0007fff7, 0x0007fff7,
    0x000077ff, 0x00000077, 0, 0,
    0, 0, 0, 0,
    0xff770000, 0x77000000, 0, 0,
    0, 0, 0, 0,
    0x00007fff, 0x0007ffff, 0x007ffff7, 0x07ffff70,
    0x07fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000,
    // Unused: the arc in colour 4.
    0, 0, 0, 0,
    0, 0, 0x40000000, 0xf4400000,
    0, 0, 0, 0,
    0x44440000, 0xffff4440, 0xfffffff4, 0x4444ffff,
    0, 0, 0, 0,
    0x00004444, 0x0444ffff, 0x4fffffff, 0xffff4444,
    0, 0, 0, 0,
    0, 0, 0x00000004, 0x0000044f,
    0xfff40000, 0xffff4000, 0x4ffff400, 0x04ffff40,
    0x004fff40, 0x004ffff4, 0x0004fff4, 0x0004fff4,
    0x000044ff, 0x00000044, 0, 0,
    0, 0, 0, 0,
    0xff440000, 0x44000000, 0, 0,
    0, 0, 0, 0,
    0x00004fff, 0x0004ffff, 0x004ffff4, 0x04ffff40,
    0x04fff400, 0x4ffff400, 0x4fff4000, 0x4fff4000,
    // Unused: the three sparkles, two words a row.
    0xe9000000, 0x000000e9, 0xeb000000, 0x000000eb,
    0xedeb0000, 0x0000ebed, 0xeeedebe9, 0xe9ebedee,
    0xeeedebe9, 0xe9ebedee, 0xedeb0000, 0x0000ebed,
    0xeb000000, 0x000000eb, 0xe9000000, 0x000000e9,
    0x0000e900, 0x00e90000, 0x00ebe9e9, 0xe9e9eb00,
    0xedebeb00, 0x00ebebed, 0xefed0000, 0x0000edef,
    0xefed0000, 0x0000edef, 0xedebeb00, 0x00ebebed,
    0x00ebe9e9, 0xe9e9eb00, 0x0000e900, 0x00e90000,
    0xe9000000, 0x000000e9, 0xebe90000, 0x0000e9eb,
    0xedebe900, 0x00e9ebed, 0xeeedebe9, 0xe9ebedee,
    0xeeedebe9, 0xe9ebedee, 0xedebe900, 0x00e9ebed,
    0xebe90000, 0x0000e9eb, 0xe9000000, 0x000000e9,
};

#include "shared/field_effect_duration_scripts.c"

/// The Lucky Wheels' prizes, three to each icon that wins them
/// (Contigo_PickPrize).
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

/// The Teleport circle's colours (shared/teleport_shimmer.c): their slots, the
/// colours they rest at, the shimmer's colours, and the slots each step of the
/// shimmer paints those into, so that the colours run round the circle.
u16 gContigo_PaletteSlots[] = {
    89, 90, 91, 92, 93, 94, 95, PALETTE_LIST_END,
};

u16 gContigo_PaletteColors[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, PALETTE_LIST_END,
};

u16 gContigo_ShimmerColors[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, PALETTE_LIST_END,
};

u16 gContigo_CycleSlots[] = {
    94, 89, 90, 91, 92, 93,
    93, 94, 89, 90, 91, 92,
    92, 93, 94, 89, 90, 91,
    91, 92, 93, 94, 89, 90,
    90, 91, 92, 93, 94, 89,
    89, 90, 91, 92, 93, 94,
};

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
    ACTOR_PLACEMENT(422, CONDITION_ALWAYS, 7, 720, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(422, CONDITION_ALWAYS, 7, 752, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(422, CONDITION_ALWAYS, 7, 784, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(423, CONDITION_ALWAYS, 7, 776, 0, 590, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(423, CONDITION_ALWAYS, 7, 808, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(423, CONDITION_ALWAYS, 7, 744, 0, 614, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(423, CONDITION_ALWAYS, 7, 808, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(423, CONDITION_ALWAYS, 7, 826, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 700, 0, 604, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 668, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 758, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 858, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 782, 0, 678, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 669, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
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
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 30, CONDITION_ALWAYS, EVENT(Contigo_TeleportOut)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 39, CONDITION_ALWAYS, EVENT(Contigo_ScatterFirstWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(Contigo_ScatterSecondWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(Contigo_ScatterWeedsOverBrambleSeed)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42, CONDITION_ALWAYS, EVENT(Contigo_ScatterFourthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43, CONDITION_ALWAYS, EVENT(Contigo_ScatterFifthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44, CONDITION_ALWAYS, EVENT(Contigo_ScatterSixthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45, CONDITION_ALWAYS, EVENT(Contigo_ScatterSeventhWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 46, CONDITION_ALWAYS, EVENT(Contigo_ScatterEighthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 47, CONDITION_ALWAYS, EVENT(Contigo_ScatterFirstWeedRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 48, CONDITION_ALWAYS, EVENT(Contigo_ScatterSecondWeedRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 49, CONDITION_ALWAYS, EVENT(Contigo_ScatterThirdWeedRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 50, CONDITION_ALWAYS, EVENT(Contigo_ScatterFourthWeedRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 51, CONDITION_ALWAYS, EVENT(Contigo_ScatterFifthWeedRow)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Contigo_NarrowCameraBounds)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(Contigo_WidenCameraBounds)},
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(Contigo_ScoopForSalt)},
    {TRIGGER_RAISED, RAISED_SALT_FOUND, CONDITION_ALWAYS, EVENT(Contigo_SaltJoinsParty)},
    {TRIGGER_EXAMINE, 24, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyWheelsSign)},
    {TRIGGER_EXAMINE, 25, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyDiceSign)},
    {TRIGGER_EXAMINE, 26, CONDITION_ALWAYS, EVENT(Contigo_ReadSuperDiceSign)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT(Scene_ShowEntranceLocked)},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_GeoglyphTriggers[] = {
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 21},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT(Contigo_EnterSanctumWestDoor)},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 14, CONDITION_ALWAYS, 23},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(Contigo_EnterSanctumEastDoor)},
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
    {TRIGGER_EXAMINE, 22, CONDITION_ALWAYS, EVENT(Contigo_ExamineWall)},
    {TRIGGER_TALK, ACTOR_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadSign)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Contigo_RevealMiddleDoor)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Contigo_HideMiddleDoor)},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_ShopsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
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
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoTheresABrightRedSoup))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoAhTheSmellOfFresh))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoContigosFamousForTheseCookies))},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_ShopsReunionTriggers[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, ACTOR_HAMMA, CONDITION_ALWAYS, MSGNO(MsgContigoNoMatterWhatHappensYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMA, CONDITION_ALWAYS, MSGNO(MsgContigoMyHeartIsWithYou)},
    {TRIGGER_STEP, 20, FLAG_HAMMA_WELCOMED_PARTY, EVENT(Contigo_ReturnToNewHome)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoTheresABrightRedSoup))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoAhTheSmellOfFresh))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoContigosFamousForTheseCookies))},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_LuckyDiceTriggers[] = {
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_HOST, CONDITION_ALWAYS, EVENT(Contigo_SendToSuperLuckyDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUPER_LUCKY_DICE_HOST, CONDITION_ALWAYS, MSGNO(MsgContigoWhenTheyLoseTheyCall)},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_GUIDE, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouWantToLearn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUPER_LUCKY_DICE_GUIDE, CONDITION_ALWAYS, MSGNO(MsgContigoItsNotAToughGame)},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ExplainSuperDice)},
    {TRIGGER_TALK, ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, CONDITION_ALWAYS, EVENT(Contigo_AskAboutSuperDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, CONDITION_ALWAYS, MSGNO(MsgContigoThisKidsGotUnluckyWritten)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_HI_LO_GAME, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouLoseTheHi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_HI_LO_GAME, CONDITION_ALWAYS, MSGNO(MsgContigoItsToughToKnowWhen)},
    {TRIGGER_TALK, ACTOR_WOMAN_WHO_LOSES_AT_HI_LO, CONDITION_ALWAYS, MSGNO(MsgContigoIAlmostAlwaysGetAt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_LOSES_AT_HI_LO, CONDITION_ALWAYS, MSGNO(MsgContigoICanWinICan)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_HOST, CONDITION_ALWAYS, EVENT(Contigo_SendToLuckyDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_HOST, CONDITION_ALWAYS, MSGNO(MsgContigoWinningOrLosingIsPure)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_GUIDE, CONDITION_ALWAYS, MSGNO(MsgContigoReadTheSignIfYa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_GUIDE, CONDITION_ALWAYS, MSGNO(MsgContigoTheRulesForLuckyDice)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ExplainLuckyDice)},
    {TRIGGER_TALK, ACTOR_SECOND_LUCKY_DICE_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ExplainLuckyDiceAgain)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_ATTENDANT, CONDITION_ALWAYS, EVENT(Contigo_TalkToWheelsAttendant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_WHEELS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoHmmIWonderIfThese)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ExplainLuckyWheels)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_PRIZE_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ListLuckyWheelsPrizes)},
    {TRIGGER_TABLE_END},
};
#undef COINS_WON_MESSAGE
#undef COINS_LOST_MESSAGE
#undef READ_SIGN_MESSAGE
#undef CONGRATULATIONS_MESSAGE
#undef FIRST_PRIZE_MESSAGE
#undef NEXT_PRIZE_MESSAGE
#undef COME_AGAIN_MESSAGE
