/// @file
/// Contigo, the village below Jupiter Lighthouse, in the rooms of this
/// overlay: its shops, inn and Sanctum (Contigo_TalkToWeaponMerchant,
/// Contigo_TalkToInnkeeper, Contigo_TalkToHealer), the bushes where Scoop
/// finds the Venus Djinni Salt, and the cycling colours of its water
/// (Contigo_StartSlowPaletteCycle).

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

/// Force has freed Shine from the ground it was stuck in.
#define FLAG_SHINE_FREED 0x9e7
/// Set as Shine, freed, joins the party; until then it waits where it landed.
#define FLAG_SHINE_CAUGHT 0x9e6
/// Set once the Bramble Seed in the bushes over Salt is picked up
/// (gSceneItems).
#define FLAG_CONTIGO_BRAMBLE_SEED_TAKEN 0xfc7
/// Set as Kraden comes out to greet the party in the house once it has set out
/// with the wings (Contigo_ReturnToNewHome).
#define FLAG_CONTIGO_KRADEN_GREETED_PARTY 0x9d1

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
  /// "Everyone's been going on about how amazing it was to watch the ship
  /// fly..."; once the beacon is lit, "The beacon within Jupiter Lighthouse
  /// will soon be lit."
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
  /// The boy who points the way (Contigo_TalkToWanderer).
  ACTOR_WANDERER,
  /// The girl who talks of the ship (Contigo_TalkAboutTheShip).
  ACTOR_GIRL_ON_THE_SHIP,
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
  ACTOR_ITEM_MERCHANT,
  /// "Soon, the siblings with the power of the Anemos will return."
  ACTOR_YOUNG_MAN_ON_THE_SIBLINGS,
  /// "The power of the Anemos is all but gone in Contigo."
  ACTOR_WOMAN_ON_THE_ANEMOS_POWER,
  /// The baby whose furrowed brow brings the wind (Contigo_BabyThinks).
  /// The baby's brother: "Mom and Dad have been walking around in a daze
  /// since the ship took off."
  ACTOR_BABYS_BROTHER,
  ACTOR_BABY,
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  ACTOR_INNKEEPER,
  /// "The inn has become so..."
  ACTOR_INN_WORKER,
  /// "Gosh, we were so busy..."; once the beacon is lit, "Gotta make all the
  /// beds..."
  ACTOR_WOMAN_MAKING_THE_BEDS,
  /// "Now's my chance to experiment..."
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
  ACTOR_HEALER
};

/// Hama and Kraden at the reunion and after it (gContigo_ShopsPlacements,
/// gContigo_ShopsPlacementsReunion).
enum ContigoReunionActorId { ACTOR_HAMA = ACTOR_FIRST_PLACED, ACTOR_KRADEN };

/// The dice and wheels hall (gContigo_LuckyDicePlacements).
enum ContigoLuckyDiceActorId {
  /// Who runs Super Lucky Dice, and who explains it beside him.
  ///@{
  ACTOR_SUPER_LUCKY_DICE_HOST = ACTOR_FIRST_PLACED,
  ACTOR_SUPER_LUCKY_DICE_GUIDE,
  ///@}
  /// Who runs Lucky Dice, and who points to its sign.
  ///@{
  /// The players: "This kid's got unlucky written all over his face" (his
  /// thought); "If you lose the hi-lo game, you don't win anything...";
  /// "I almost always get at least one pair..."
  ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS,
  ACTOR_OLD_MAN_ON_THE_HI_LO_GAME,
  ACTOR_WOMAN_WHO_LOSES_AT_HI_LO,
  /// The sign of Super Lucky Dice's rules.
  ACTOR_SUPER_LUCKY_DICE_SIGN,
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
  /// metal feather..."
  ///@{
  ACTOR_WING_WORKER_A,
  ACTOR_WING_WORKER_B,
  ACTOR_WING_WORKER_C,
  ACTOR_WING_WORKER_D,
  ///@}
  /// The four scholars in long robes studying the geoglyph.
  ///@{
  ACTOR_SCHOLAR_A,
  ACTOR_SCHOLAR_B,
  ACTOR_SCHOLAR_C,
  ACTOR_SCHOLAR_D,
  ///@}
  /// The sign with a translation of the geoglyph's characters.
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
void Contigo_OpenGameDoor(s32 a, s32 b, s32 c, s32 d);

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
#include "edition.h"
#include "version.h"
#include "system/vector3.h"

extern s32 gFieldEffect_DurationScripts[];
extern struct ActorPlacement gContigo_VillagePlacements[];
extern struct ActorPlacement gContigo_GeoglyphPlacements[];
extern struct ActorPlacement gContigo_ShopsPlacementsBeforeReunion[];
extern s16 gContigo_ShopsPlacementsReunion[];
extern struct ActorPlacement gContigo_ShopsPlacements[];
extern struct ActorPlacement gContigo_LuckyDicePlacements[];
extern struct Trigger gContigo_VillageTriggers[];
extern struct Trigger gContigo_ShopsTriggers[];
extern struct Trigger gContigo_ShopsReunionTriggers[];

void Contigo_StartSlowPaletteCycle(void);

void Contigo_StopPaletteCycle(void);

extern u32 gContigo_WheelsPrizes[];
/// The Lucky Wheels' icons that win prizes, shirts, boots, rings, then the
/// stars' and hearts' items, three prizes to each (gContigo_WheelsPrizes); the
/// Moon after them is wild, any of the five at random.
enum {
  WHEELS_PRIZE_ICONS = 5,
  WHEELS_ICON_MOON = WHEELS_PRIZE_ICONS,
  WHEELS_PRIZES_PER_ICON = 3
};
extern s32 gContigo_EnterGameDoorScript[];
extern u32 gContigo_HostScript[];
void Contigo_InitializeVillage(void);
void Contigo_InitializeGeoglyph(void);
void Contigo_InitializeShops(void);
void Contigo_InitializeLuckyDice(void);
extern s16 gContigo_PaletteSlots[];
extern s16 gContigo_PaletteColors[];
extern s16 gContigo_DimColors[];
extern u16 gContigo_CycleSlots[];
extern s32 gContigo_ShopsWatcherScript[];
extern s32 gContigo_SparkleTiles[];
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
void Contigo_TalkToWanderer(void);
void Contigo_TalkAboutTheShip(void);
void Contigo_ClearFirstBushes(void);
void Contigo_ClearSecondBushes(void);

void Contigo_ClearBushesBySalt(void);

/// Clears the fourth clump of bushes where Whirlwind touched it.
void Contigo_ClearFourthBushes(void);
void Contigo_ClearFifthBushes(void);
void Contigo_ClearSixthBushes(void);
void Contigo_ClearSeventhBushes(void);
void Contigo_ClearEighthBushes(void);
void Contigo_ClearFirstHedgeRow(void);
void Contigo_ClearSecondHedgeRow(void);
void Contigo_ClearThirdHedgeRow(void);
void Contigo_ClearFourthHedgeRow(void);
void Contigo_ClearFifthHedgeRow(void);
void Contigo_WidenCameraBounds(void);
void Contigo_NarrowCameraBounds(void);
void Contigo_ScoopForSalt(void);
void Contigo_RevealSteps(void);

void Contigo_ForceFreesDjinni(void);

void Contigo_SendToLuckyDice(void);
void Contigo_SendToSuperLuckyDice(void);
void Contigo_EnterLuckyWheels(void);
void Contigo_EnterSuperLuckyDice(void);
void Contigo_ReturnToNewHome(void);
extern u32 gContigo_ShortScript[];
extern u32 gContigo_MediumScript[];
extern u32 gContigo_LongScript[];
extern u32 gContigo_WandererScript[];
extern u32 gContigo_PacerScript[];
extern s32 gContigo_StatueScript[];

u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);

void Contigo_TeleportIn(void);

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Palette_SetColors(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->paletteFade;

  gPaletteSlots = slots;
  gPaletteColors = colours;
  gPaletteCycleColors = a;
  gPaletteCycleSlots = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  if (gMainRoutineWork.teleportState == 2)
    Contigo_TeleportIn();
}

/// Starts the slow cycle of the village's water colours, a step every 16
/// frames, as a VBlank task.
void Contigo_StartSlowPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

/// Stops the cycle of colours and leaves the scene's next transition a backdrop
/// fade.
void Contigo_StopPaletteCycle(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Remove(PaletteCycle_Fade);
}

/// Starts the fast cycle of colours, a step every 2 frames, which spins the
/// colours as the party teleports.
void Contigo_StartFastPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 2;
  gPaletteCycleTimer = 2;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

/// Counts a list of palette slots ended by 0xffff, less one.
s32 List_CountEntries(u16 *list) {
  s32 count = 0;

  while (list[count] != 0xffff)
    count++;
  count--;
  return count;
}

#include "shared/palette_cycle_fade.h"

/// The party's arrival by teleport from Anemos: the scene fades in through grey
/// while the colours spin and the whirl plays, then settles.
void Contigo_TeleportIn(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Contigo_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  wait(40);
  Contigo_StopPaletteCycle();
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

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect =
        ((struct FieldEffect *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    FIXED target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - FIXED_ONE) / frames;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

/// Salt, found in the bushes, joins the party.
void Contigo_SaltJoinsParty(void) {
  Djinn_JoinParty(ACTOR_SALT, DJINNI_SALT / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SALT));
}

/// Shine, freed from the ground by Force, joins the party.
void Contigo_FreedDjinniJoinsParty(void) {
  Djinn_JoinParty(ACTOR_SHINE, DJINNI_SHINE / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SHINE));
  GameFlag_Set(FLAG_SHINE_CAUGHT);
}

/// The distance in whole pixels between two positions of three 16.16
/// coordinates, as Lemuria's copy of it.
s32 Lemuria_Distance(s32 *a, s32 *b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a++ - *b++) >> 16;
  s32 dz = (*a - *b) >> 16;
  s32 dxsq = dx * dx;
  s32 dysq = dy * dy;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dysq + dzsq);
}

/// 1 when `other` is within `range` of `self` and `self` faces it, within one
/// eighth of a turn.
s32 Actor_IsFacingNear(struct Actor *self, struct Actor *other, s32 range) {
  s32 result = 0;
  s32 *to;
  s32 *from;

  to = &other->x.fixed;
  from = &self->x.fixed;
  if (Lemuria_Distance(to, from) < range) {
    u32 angle = (u16)Trig_Atan(other->z.fixed - self->z.fixed, *to - *from);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = self->facing & 0xf000;

    if (ahead == facing || right == facing || left == facing)
      result = 1;
  }
  return result;
}

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 value);
s32 BattleEffect_GetObjectPriority(struct Actor *object);

/// The update of the wanderer and the girl by the ship: each turns to the one
/// it talks with (ANIM_STAND's frames, near) while the leader or the other
/// villager stands close and faces it, or Mind Read is cast on it, and
/// otherwise looks away.
s32 Contigo_FaceTalker(struct Actor *self) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u8 *state;
  u32 mode;
  s8 *effect;
  struct Actor *other;

  Actor_SetSpritePriorityDirect(
      self, BattleEffect_GetObjectPriority(
                Actor_GetById(gMainRoutineWork.leaderActorId)));
  mode = 0x3ff & event->psynergy;
  state = &self->paused;
  if (mode == ABILITY_MIND_READ) {
    effect = Ram_HeapSlots->fieldEffectWork;
    if (effect[0x713] != 0)
      goto near;
  }
  if (self->riseEnabled == 1)
    goto near;
  other = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (Lemuria_Distance(&self->x.fixed, &other->x.fixed) <= 11)
    goto far;
  if (self->riseCounter == 1)
    other = Actor_GetById(ACTOR_GIRL_ON_THE_SHIP);
  else
    other = Actor_GetById(ACTOR_WANDERER);
  if (Actor_IsFacingNear(self, other, 18) != 0)
    goto near;
  if (Actor_IsFacingNear(self, Actor_GetById(gMainRoutineWork.leaderActorId),
                         26) != 0)
    goto near;
far:
  *state = 0;
  Actor_SelectAnimation(self, 2);
  return 0;
near:
  *state = 1;
  Actor_SelectAnimation(self, 1);
  return 0;
}

/// A question mark over the woman who talks of Anemos.
s32 Contigo_ShowSurprise(void) {
  FieldActor_ShowEmote(ACTOR_WOMAN_ON_THE_ANEMOS,
                       EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
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
    if (gMainRoutineWork.entrance == 4) {
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
    if (gMainRoutineWork.entrance == 4)
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

  radius = Random16() + toFIXED(2);
  angle = effect->facing;
  dx = Iwram_MulFX(radius, Trig_Cos(angle));
  dz = Iwram_MulFX(radius, Trig_Sin(angle));
  effect->x.fixed += dx;
  effect->z.fixed += dz;
  effect->facing -= 16;
  if (effect->userValue != 0) {
    effect->userValue--;
    effect->facing += TRIG_TURN / 32;
  } else if ((Random16() << 5) >> 16 == 0) {
    effect->userValue = ((Random16() << 4) >> 16) + 8;
  }
  if (++effect->timer == 101)
    Actor_Release(effect);
}

/// A task that spawns a leaf in the baby's whirlwind every eighth frame, flung
/// up to fall back as it circles.
void Contigo_RaiseLeaves(void) {
  if ((gFrameCount & 7) == 0) {
    struct Actor *object =
        Actor_Spawn(SPRITE_LEAF, toFIXED(706), 52428, toFIXED(116));

    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      object->update = Contigo_WhirlLeaf;
      object->motionFlags = 0;
      object->timer = -(s32)(Random_Below(16)) * 2 + 20;
      object->userValue = 0;
      object->gravity = toFIXED(1) / 10;
      object->velocityY = toFIXED(3);
      object->facing = Random16();
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
  BattleEffect_SetObjectUpdateMode(((struct Actor *)Actor_GetById(ACTOR_BABY)),
                                   2);
  effect = ((struct Actor *)Import_ActorCreate(SPRITE_WHIRLWIND, x, 0, z));
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
  Task_Add(Contigo_RaiseLeaves, TASK_PASS_VBLANK);
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
  BattleEffect_SetObjectUpdateMode(((struct Actor *)Actor_GetById(ACTOR_BABY)),
                                   0);
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

/// The item merchant: facing the counter opens the shop; otherwise he talks of
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
#if (LANGUAGE !=                                                               \
     LANGUAGE_SPANISH) /* The Spanish edition ends after four messages.        \
                        */
  FieldMessage_Show(msg + 4, 1);
#endif
  Event_End();
}

/// A scholar asks whether the party has looked at the Anemos geoglyph.
void Contigo_AskAboutGeoglyph(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoHaveYouTakenAGood);
  // "Have you taken a good look at the Anemos geoglyph?"
  Event_AskYesNoAndShowResponse(ACTOR_SCHOLAR_A, 0);
  Event_End();
}

/// The wanderer talks of the geoglyphs and of Anemos, turning to the leader
/// unless Mind Read is cast on him, and back after.
void Contigo_TalkToWanderer(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object = ((struct Actor *)Actor_GetById(ACTOR_WANDERER));
  s16 angle = object->facing;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->riseEnabled = 1;
  if (work->psynergy == 0) {
    Actor_FaceActorById(ACTOR_WANDERER, gMainRoutineWork.leaderActorId, 0);
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
  Event_ShowMessageAndWait(ACTOR_WANDERER, 0);
  if (work->psynergy == 0)
    object->facing = angle;
  object->riseEnabled = 0;
  Event_End();
}

/// The girl by the ship talks of the wings, turning to the leader unless Mind
/// Read is cast on her, and back after.
void Contigo_TalkAboutTheShip(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *object =
      ((struct Actor *)Actor_GetById(ACTOR_GIRL_ON_THE_SHIP));
  s16 angle = object->facing;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->riseEnabled = 1;
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
    object->facing = angle;
  object->riseEnabled = 0;
  Event_End();
}

void Teleport_PlaySparks(void);

/// Teleport from Contigo: the colours spin, the sparks play, and the party
/// leaves for the Anemos Inner Sanctum, to arrive by teleport.
void Contigo_TeleportOut(void) {
  Contigo_StartSlowPaletteCycle();
  Teleport_PlaySparks();

  gMainRoutineWork.teleportState = 2;
  Event_SetDestination(SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, 99);
  Event_Wait(40);
  Contigo_StopPaletteCycle();
}

/// Clears a clump of bushes where Whirlwind touched them, as each of the clumps
/// and hedge rows below does with its own cells.
void Contigo_ClearFirstBushes(void) {
  FieldTileCopies_CopyTouchedCells(3, 50, 1, 1, 8, 54, 0);
}

/// Clears the second clump of bushes where Whirlwind touched it.
void Contigo_ClearSecondBushes(void) {
  FieldTileCopies_CopyTouchedCells(4, 43, 2, 1, 2, 43, 0);
}

/// The scene's own name for TileCopies_CopyTouchedCells.
static __inline__ void Contigo_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d,
                                                s32 e, s32 f, s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

/// Clears the bushes over Salt; unless the Bramble Seed there was taken it is
/// shown, and a leader standing on its spot hops back.
void Contigo_ClearBushesBySalt(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Contigo_CopyTouchedCells(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Get(FLAG_CONTIGO_BRAMBLE_SEED_TAKEN) == 0) {
    Actor_SetPosition(66, 280, 664);
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

void Contigo_ClearFourthBushes(void) {
  FieldTileCopies_CopyTouchedCells(29, 35, 1, 2, 23, 47, 0);
}

/// Clears the fifth clump of bushes where Whirlwind touched it.
void Contigo_ClearFifthBushes(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 2, 3, 29, 58, 0);
}

/// Clears the sixth clump of bushes where Whirlwind touched it.
void Contigo_ClearSixthBushes(void) {
  FieldTileCopies_CopyTouchedCells(3, 60, 1, 1, 3, 56, 0);
}

/// Clears the seventh clump of bushes where Whirlwind touched it.
void Contigo_ClearSeventhBushes(void) {
  FieldTileCopies_CopyTouchedCells(7, 62, 2, 1, 7, 58, 0);
}

/// Clears the eighth clump of bushes where Whirlwind touched it.
void Contigo_ClearEighthBushes(void) {
  FieldTileCopies_CopyTouchedCells(10, 63, 2, 1, 10, 59, 0);
}

/// Clears the first hedge row where Whirlwind touched it.
void Contigo_ClearFirstHedgeRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 3, 1, 40, 57, 0);
}

/// Clears the second hedge row where Whirlwind touched it.
void Contigo_ClearSecondHedgeRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 58, 0);
}

/// Clears the third hedge row where Whirlwind touched it.
void Contigo_ClearThirdHedgeRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 59, 0);
}

/// Clears the fourth hedge row where Whirlwind touched it.
void Contigo_ClearFourthHedgeRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 5, 1, 39, 60, 0);
}

/// Clears the fifth hedge row where Whirlwind touched it.
void Contigo_ClearFifthHedgeRow(void) {
  FieldTileCopies_CopyTouchedCells(32, 55, 3, 1, 40, 61, 0);
}

/// Lets the camera follow the leader across the whole village, held by a scene
/// flag.
void Contigo_WidenCameraBounds(void) {
  Field_SetCameraBounds(0, toFIXED(512), toFIXED(880), toFIXED(1024));
  GameFlag_Set(FLAG_SCENE_TEMP_00);
}

/// Holds the camera to the village's western part again.
void Contigo_NarrowCameraBounds(void) {
  Field_SetCameraBounds(0, toFIXED(512), toFIXED(512), toFIXED(1024));
  GameFlag_Clear(FLAG_SCENE_TEMP_00);
}

/// Scoop by the bushes: until the party has Salt, Salt appears beside the
/// leader and turns to him.
void Contigo_ScoopForSalt(void) {
  if (!GameFlag_Get(FLAG_DJINNI(DJINNI_SALT))) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    Event_Start();
    Event_ResolvePendingActions(1);
    Actor_SetPosition(ACTOR_SALT, 664, 952);
    wait(1);
    Actor_FaceActorById(ACTOR_SALT, gMainRoutineWork.leaderActorId, 0);
    work->raisedTrigger = 200;
    Event_End();
  }
}

/// Draws the steps by the dice hall's door and makes them walkable.
void Contigo_RevealSteps(void) {
  MapGrid_CopyCellsTo(27, 4, 48, 12, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(27, 6, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 1, 2, 1, 48, 19);
}

/// Takes the steps away again.
void Contigo_HideSteps(void) {
  MapGrid_CopyCellsTo(27, 8, 48, 12, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(27, 10, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 0, 2, 1, 48, 19);
}

/// Force on the ground where Shine is stuck: the ground shakes, a ring of dust
/// bursts out, and Shine leaps free, the leader hopping aside if it lands on
/// him.
void Contigo_ForceFreesDjinni(void) {
  struct Actor *object = Actor_GetById(ACTOR_SHINE);
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
  for (i = 0; i <= 16; i++) {
    angle = i << 12;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] / 3;
    FieldEffect_Spawn(object->x.fixed, object->y.fixed, object->z.fixed,
                      velocity[0], velocity[1] + (toFIXED(1) / 10), velocity[2],
                      EFFECT_USE_PRIORITY, &options);
  }
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SHINE), SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHINE), 1);
  Actor_SetSpeed(ACTOR_SHINE, 3);
  object->velocityY = toFIXED(12);
  Actor_MoveToAndWait(ACTOR_SHINE, 40, 680);
  Event_Wait(10);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (FIELD_CELL(object->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(object->z.fixed) == FIELD_CELL(leader->z.fixed)) {
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

/// Opens the doors of the games' rooms once the beacon is lit.
void Contigo_OpenGamesRooms(void) {
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
  struct Actor *object = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  object->collisionFlags = ACTOR_NOT_TARGETED;
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  object->motionFlags = 0;
  object->y.fixed = toFIXED(64);
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

/// The dice game attendant: the fee is ten coins a level.
void Contigo_SendToLuckyDice(void) {
  u32 price = Party_GetAverageLevel() * 10;

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
    /* FAKEMATCH(pin): the scene state's address pinned to r3. */
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
  u32 price = Party_GetAverageLevel() * 10;

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
    /* FAKEMATCH(pin): the scene state's address pinned to r3. */
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
  struct Actor *object = Actor_GetById(id);

  object->scaleX = FIXED_ONE * 7 / 10;
  object->scaleY = FIXED_ONE * 7 / 10;
  object->radius = 8;
}

/// Opens a game's door in the dice hall, two cells of map edits, and walks the
/// leader in.
void Contigo_OpenGameDoor(s32 a, s32 b, s32 c, s32 d) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_THUMP);
  {
    s32 width = 1;
    s32 height = 2;

    MapGrid_CopyCellPictureIndicesRect(a, b, c, d, width, height);
    MapGrid_CopyCellPictureIndicesRect(a, b + 2, c + 1, d, width, height);
  }
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
                            gContigo_EnterGameDoorScript);
  wait(12);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Enters the Lucky Wheels' door.
void Contigo_EnterLuckyWheels(void) {
  Contigo_OpenGameDoor(27, 0, 42, 13);
  Import_EventSetExitRequest(22);
}

/// Enters the Lucky Dice's door.
void Contigo_EnterLuckyDice(void) {
  Contigo_OpenGameDoor(27, 4, 48, 12);
  Import_EventSetExitRequest(23);
}

/// Enters the Super Lucky Dice's door.
void Contigo_EnterSuperLuckyDice(void) {
  Contigo_OpenGameDoor(27, 0, 54, 13);
  Import_EventSetExitRequest(24);
}

extern u32 gContigo_JennaReunionScript[];
extern u32 gContigo_PiersReunionScript[];
extern u32 gContigo_ShebaReunionScript[];
extern u32 gContigo_KradenReunionScript[];
extern u32 gContigo_IsaacLeavesScript[];
struct SparkleWork gContigo_SparkleWork BSS_DATA ALIGNED(16);

/// The reunion scene. It opens with "Glad you kept your promise".
void Contigo_TheReunion(void) {
  struct Actor *object;
  s32 answer;
  s32 angle;

  object = ((struct Actor *)Actor_GetById(ACTOR_HAMA));
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WatchAndSetScript(ACTOR_ISAAC, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_GARET, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_MIA, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_IVAN, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_HAMA, gMainRoutineWork.leaderActorId);
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
  Actor_Stop(ACTOR_HAMA);
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
  Actor_SetSpriteColor(((struct Actor *)Actor_GetById(ACTOR_HAMA)),
                       SPRITE_COLOR_HIDDEN);
  angle = FACING_NORTH_NORTHWEST;
  Actor_SetHorizontalPositionAndAngleWithTerrain(ACTOR_HAMA, toFIXED(160),
                                                 toFIXED(466), angle);
  wait(1);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_HAMA);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_HAMA);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_HAMA);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_HAMA);
  Actor_WatchAndSetScript(ACTOR_ISAAC, ACTOR_HAMA);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_HAMA);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_HAMA);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_HAMA);
  Actor_SetSpriteColor(((struct Actor *)Actor_GetById(ACTOR_HAMA)),
                       SPRITE_COLOR_NORMAL);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(180, CAMERA_KEEP, 410, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(148, CAMERA_KEEP, 368, 1);
  Actor_SetSpeed(ACTOR_HAMA, 0.8);
  Actor_WalkToAndWait(ACTOR_HAMA, 146, 406);
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
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
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
#if !TLA_PATCH_1
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_ISAAC, 5, 5, 8, 10, 8, 9, 20, 18, 12,
                                    10);
#else
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_ISAAC, 5, 5, 8, 10, 8, 9, 20, 18, 10,
                                    10);
#endif
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
  Actor_SetAnimationById(ACTOR_HAMA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_HAMA, FACING_EAST);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMA, 0);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_WalkToAndWait(ACTOR_IVAN, 130, 400);
  Actor_FaceDirectionById(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMA, 0);
  FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetAnimationById(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_HAMA, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_NORTH_NORTHEAST);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_SetAnimationById(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Contigo_StartSparkle(object, &gContigo_SparkleWork);
  Event_Wait(80);
  Contigo_StopSparkle(object);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Contigo_StartSparkle(object, &gContigo_SparkleWork);
  Event_Wait(80);
  Contigo_StopSparkle(object);
  Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationById(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
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
  Actor_FaceDirectionAndRefreshById(ACTOR_HAMA, angle);
  Actor_SetAnimationById(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
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
  FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_SetAnimationById(ACTOR_HAMA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveTo(180, CAMERA_KEEP, 410, 1);
  Actor_WalkToAndWait(ACTOR_HAMA, 160, 466);
  Actor_SetPosition(ACTOR_HAMA, 0, 0);
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
  Actor_SetMovementTypeById(ACTOR_ISAAC, gContigo_IsaacLeavesScript);
  Actor_SetMovementTypeById(ACTOR_GARET, gContigo_IsaacLeavesScript);
  Actor_SetMovementTypeById(ACTOR_MIA, gContigo_IsaacLeavesScript);
  Actor_SetMovementTypeById(ACTOR_IVAN, gContigo_IsaacLeavesScript);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gContigo_IsaacLeavesScript);
  Actor_SetMovementTypeById(ACTOR_JENNA, gContigo_IsaacLeavesScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gContigo_IsaacLeavesScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS, gContigo_IsaacLeavesScript);
  Party_AddIsaacsParty();
  GameFlag_Set(FLAG_REUNITED_AT_CONTIGO);
  Event_End();
}

extern u32 gContigo_LeaderVisitsScript[];

/// Once the party has set out with the wings, stepping into the house brings
/// Kraden out to greet them, once.
void Contigo_ReturnToNewHome(void) {
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0.8);
    FieldCamera_MoveTo(160, CAMERA_KEEP, 368, 1);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 158, 394);
    Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 0);
    Actor_MoveToObject(ACTOR_IVAN, gMainRoutineWork.leaderActorId);
    wait(1);
    Actor_SetSpeed(ACTOR_IVAN, 0.8);
    Actor_WalkToAndWaitById(ACTOR_IVAN, 144, 374);
    Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
    FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    Event_SetMessageId((s32)MsgReturnedToMyNewHome);
    // "Ah, you've returned! What brings you to my new home?"
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    // "Hamma..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_SetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    // "You're my sister... Why didn't you tell me sooner? Why didn't you..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_SetAnimationById(ACTOR_HAMA, ANIM_SHAKE_HEAD);
    // "You play an important role in all this. If I'd told you, you would..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 40);
    Actor_WalkToAndWait(ACTOR_IVAN, 180, 364);
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_IVAN, 186, 388);
    Event_Wait(40);
    Actor_WalkToAndWait(ACTOR_IVAN, 146, 380);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "This place is so familiar... I feel like I've lived here my whole life."
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    // "Look at me, Ivan."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 40);
    // "You're too sentimental. You would have given up your quest to find..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    // "What are you saying, sister?"
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_SetAnimationById(ACTOR_HAMA, ANIM_SHAKE_HEAD);
    // "No, not sister... Do not think of me as your sister until you have..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_SetAttachedEffect(ACTOR_IVAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "W-Why?"
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "Our world is falling into ruin..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_MoveToObject(ACTOR_ISAAC, gMainRoutineWork.leaderActorId);
    wait(1);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_SetSpeed(ACTOR_ISAAC, 1.2);
    Actor_WalkToAndWaitById(ACTOR_ISAAC, 168, 380);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
    // "What? Hamma, what do you mean? What's happening?"
    Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
    FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST, 0);
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
    Actor_FaceDirectionById(ACTOR_HAMA, FACING_EAST, 0);
    Actor_SetAttachedEffect(ACTOR_KRADEN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "I couldn't help overhearing you say something about the world..."
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
    // "Our world requires a delicate balance of elemental forces. That..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMA, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                         EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST);
    // "Without that balance, one element might be lost beneath the others..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    // "You're talking about Mars Lighthouse, right?"
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Actor_FaceDirectionAndRefreshById(ACTOR_HAMA, FACING_EAST);
    Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 40);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
    Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
    // "Isn't Agatio taking care of Mars Lighthouse? Everything should be..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 20);
    // "Agatio will not light Mars Lighthouse."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
    // "You don't think he can do it?"
    Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
    Actor_SetAttachedEffect(ACTOR_HAMA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST);
    // "No. A great power stands ready to oppose them."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMA, 0);
    FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                         EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST);
    Actor_SetAnimationById(ACTOR_HAMA, ANIM_SHAKE_HEAD);
    // "They lack the power to resist that force, and they will fall before it."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
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
    FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST, 0);
    // "Our world will be plunged into ice."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMA, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    // "But what would make that happen?"
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST);
    // "Even I do not know the answer to that question."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
    Actor_FaceDirectionById(ACTOR_HAMA, FACING_EAST, 80);
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
    FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // "When Mercury Lighthouse was lit, this didn't happen, though. I..."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMA, 0);
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
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST);
    Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
    // "It's all right. I'm happy that you cared so much..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    // "All right, Felix! Let's head to Mars Lighthouse!"
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
    Actor_SetSpeed(ACTOR_IVAN, 1.2);
    Actor_SetSpeed(ACTOR_ISAAC, 1.2);
    Actor_SetSpeed(ACTOR_KRADEN, 1.2);
    Actor_SetMovementTypeById(ACTOR_IVAN, gContigo_HostScript);
    Actor_SetMovementTypeById(ACTOR_ISAAC, gContigo_HostScript);
    Actor_SetMovementTypeAndWaitById(ACTOR_KRADEN, gContigo_HostScript);
    GameFlag_Set(FLAG_CONTIGO_KRADEN_GREETED_PARTY);
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

/// A task that draws actor 65 at the leader's sprite priority.
void Contigo_KeepPriorityWithLeader(void) {
  if (Actor_GetById(65) != 0) {
    struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);

    Actor_SetSpritePriority(65,
                            ((struct FieldSprite *)owner->sprite)->priority);
  }
}

/// Sets up the village: the wanderer and the girl by the ship turn to whoever
/// talks to them (Contigo_FaceTalker), the girl and the old woman gone and the
/// games' rooms open once the beacon is lit; Shine waits where it landed until
/// caught; arriving by entrances 1 to 4, the camera follows the leader across
/// the whole village; then the water's colours are set.
void Contigo_InitializeVillage(void) {
  struct Actor *object = Actor_GetById(ACTOR_WANDERER);

  Task_Add(Contigo_KeepPriorityWithLeader, TASK_PASS_VBLANK);
  object->riseCounter = 1;
  object->riseEnabled = 0;
  object->update = Contigo_FaceTalker;
  if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 &&
      GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) == 0) {
    Actor_Stop(ACTOR_GIRL_ON_THE_SHIP);
    Actor_SetPosition(ACTOR_GIRL_ON_THE_SHIP, 0, 0);
    Actor_SetPosition(ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, 0, 0);
    Contigo_OpenGamesRooms();
  } else {
    object = Actor_GetById(ACTOR_GIRL_ON_THE_SHIP);
    object->riseCounter = 0;
    object->riseEnabled = 0;
    object->update = Contigo_FaceTalker;
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
    case 1:
    case 2:
    case 3:
    case 4:
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      break;
    default:
      Field_SetCameraBounds(0, toFIXED(512), toFIXED(512), toFIXED(1024));
      break;
    }
  }
  if (GameFlag_Get(FLAG_SCENE_TEMP_00) != 0) {
    Field_SetCameraBounds(0, toFIXED(512), toFIXED(880), toFIXED(1024));
    FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  Palette_SetColors(gContigo_PaletteSlots, gContigo_PaletteColors,
                    gContigo_DimColors, gContigo_CycleSlots);
}

/// Sets up the geoglyph: before the beacon is lit the wings' parts lie by it;
/// after, they and their workers are gone, the scholars too until the party
/// sets out.
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
      Actor_SetPosition(ACTOR_SCHOLAR_A, 0, 0);
      Actor_SetPosition(ACTOR_SCHOLAR_B, 0, 0);
      Actor_SetPosition(ACTOR_SCHOLAR_C, 0, 0);
      Actor_SetPosition(ACTOR_SCHOLAR_D, 0, 0);
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
  if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 &&
        GameFlag_Get(FLAG_REUNITED_AT_CONTIGO) == 0)
      Contigo_TheReunion();
  } else if (gMainRoutineWork.entrance == 21) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  } else {
    Actor_SetAnimationById(ACTOR_BABY, 2);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_BABY), 0);
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS) != 0)
      Actor_SetMovementTypeById(ACTOR_OLD_MAN_ON_HESPERIA,
                                gContigo_ShopsWatcherScript);
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
#if (LANGUAGE == LANGUAGE_SPANISH)
#define MSG_COINS_WON ((s32)MsgContigoHeyYaDontHaveEnough + 1)
#define MSG_COINS_LOST ((s32)MsgContigoHeyYaDontHaveEnough + 2)
#define MSG_READ_SIGN ((s32)MsgContigoYaWonCoinTalkAbout)
#define MSG_CONGRATULATIONS ((s32)MsgContigoYaLostCoinBetterLuck)
#define MSG_FIRST_PRIZE ((s32)MsgContigoTheRulesAreListedOn)
#define MSG_NEXT_PRIZE ((s32)MsgContigoCongratulations)
#define MSG_COME_AGAIN ((s32)MsgContigoComeAgain)
#else
#define MSG_COINS_WON ((s32)MsgContigoYaWonCoinTalkAbout)
#define MSG_COINS_LOST ((s32)MsgContigoYaLostCoinBetterLuck)
#define MSG_READ_SIGN ((s32)MsgContigoTheRulesAreListedOn)
#define MSG_CONGRATULATIONS ((s32)MsgContigoCongratulations)
#define MSG_FIRST_PRIZE ((s32)MsgContigoYouWonHereYouGo)
#define MSG_NEXT_PRIZE ((s32)MsgContigoAndYouWonHereYou)
#define MSG_COME_AGAIN ((s32)MsgContigoComeAgain)
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
  Actor_SetSpriteColor(((struct Actor *)Actor_GetById(ACTOR_LUCKY_DICE_HOST)),
                       SPRITE_COLOR_GRB);
  Actor_SetSpriteColor(((struct Actor *)Actor_GetById(ACTOR_LUCKY_DICE_GUIDE)),
                       SPRITE_COLOR_RBG);
  Actor_SetSpriteColor(
      ((struct Actor *)Actor_GetById(ACTOR_SUPER_LUCKY_DICE_HOST)),
      SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(
      ((struct Actor *)Actor_GetById(ACTOR_SUPER_LUCKY_DICE_GUIDE)),
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
      if (diff > 19999)
        Sound_Play(MUS_JINGLE_LARGE);
      else if (diff > 4999)
        Sound_Play(MUS_JINGLE_MEDIUM);
      else
        Sound_Play(MUS_JINGLE_SMALL);
      Event_Wait(20);
      Event_SetMessageId(MSG_COINS_WON);
      MessageArgument_Push(diff, MESSAGE_ARGUMENT_NUMBER);
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_HOST, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
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
      if (diff > 19999)
        Sound_Play(MUS_JINGLE_LARGE);
      else if (diff > 4999)
        Sound_Play(MUS_JINGLE_MEDIUM);
      else
        Sound_Play(MUS_JINGLE_SMALL);
      Event_Wait(20);
      Event_SetMessageId(MSG_COINS_WON);
      MessageArgument_Push(diff, MESSAGE_ARGUMENT_NUMBER);
      Event_ShowMessageAndWait(ACTOR_LUCKY_DICE_HOST, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
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
      Event_SetMessageId(MSG_READ_SIGN);
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    } else {
      Event_SetMessageId(MSG_CONGRATULATIONS);
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
      if (list[0] != -1) {
        prize = list;
        do {
          if (prize == list)
            Event_SetMessageId(MSG_FIRST_PRIZE);
          else
            Event_SetMessageId(MSG_NEXT_PRIZE);
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
      Event_SetMessageId(MSG_COME_AGAIN);
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    }
    Event_End();
  }
}

// The sparkle's update: raises two pairs of sprites over the object, each pair
// starting when its counter is cleared, until both counters pass 31.

void Actor_SetSpriteColorMode(struct Actor *, u32);
s32 BattleEffect_GetObjectDepth(struct Actor *object);

/// The sparkle's update: raises two pairs of sprites over the object, each pair
/// starting when its counter is cleared, until both counters pass 31.
void Contigo_RaiseSparkle(struct Actor *object) {
  struct Vector3 position;
  s32 x, z;
  FIXED sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedActor;
  u8 *count = &object->riseEnabled;
  struct EffectSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (object->riseCounter == 0) {
    if ((*count & 15) == 0) {
      Sound_Play(SE_RISING_RINGS);
      work->heights[work->nextPair] = 0;
      work->nextPair ^= 1;
    }
  } else if (object->riseCounter == 1) {
    if (work->heights[0] > 31 && work->heights[1] > 31) {
      Actor_SetSpriteColorMode(object, SPRITE_COLOR_NORMAL);
      VramCache_FreeEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Trig_Sin(work->heights[i] << 10);
    if (work->heights[i] >= 0) {
      if (work->heights[i] <= 31) {
        position.x = object->x.fixed;
        position.y = object->y.fixed + toFIXED(work->heights[i]);
        position.z = object->z.fixed;
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
            sprite->depth = BattleEffect_GetObjectDepth(object) - 1;
          } else {
            sprite->depth = BattleEffect_GetObjectDepth(object) + 1;
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
#undef MSG_COINS_WON
#undef MSG_COINS_LOST
#undef MSG_READ_SIGN
#undef MSG_CONGRATULATIONS
#undef MSG_FIRST_PRIZE
#undef MSG_NEXT_PRIZE
#undef MSG_COME_AGAIN

/// Sets up the sparkle: loads its picture and its four 16 by 16 sprites, and
/// hands the object to its update, Contigo_RaiseSparkle.
void Contigo_StartSparkle(struct Actor *object, struct SparkleWork *work) {
  struct EffectSprite *sprite;
  s32 tile;
  s32 row;
  s32 col;

  work->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(work->vramEntry, 256, gContigo_SparkleTiles);
  work->nextPair = 0;
  work->heights[0] = 999;
  work->heights[1] = 999;
  work->object = object;
  object->riseCounter = 0;
  object->riseEnabled = 0;
  object->update = Contigo_RaiseSparkle;
  tile = gVramCacheEntries[work->vramEntry].offset >> 5;
  object->watchedActor = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    for (col = 1, sprite = work->sprites + row + row; col >= 0; col--) {
      EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X16, tile);
      sprite->oam.fullColor = 0;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(object);
      sprite++;
    }
  }
}

/// Ends the sparkle: its update finishes once both pairs have risen.
void Contigo_StopSparkle(struct Actor *object) { object->riseCounter = 1; }

// clang-format off
s32 gContigo_SparkleTiles[] = {
    0, 0, 0, 0, 0, 0,
    0x70000000, (s32)0xf7700000, 0, 0, 0, 0,
    0x77770000, (s32)0xffff7770, -9, 0x7777ffff, 0, 0,
    0, 0, 0x7777, 0x777ffff, 0x7fffffff, (s32)0xffff7777,
    0, 0, 0, 0, 0, 0,
    7, 0x77f, -0x90000, -0x9000, 0x7ffff700, 0x7ffff70,
    0x7fff70, 0x7ffff7, 0x7fff7, 0x7fff7, 0x77ff, 119,
    0, 0, 0, 0, 0, 0,
    -0x890000, 0x77000000, 0, 0, 0, 0,
    0, 0, 0x7fff, 0x7ffff, 0x7ffff7, 0x7ffff70,
    0x7fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000, 0, 0,
    0, 0, 0, 0, 0x40000000, (s32)0xf4400000,
    0, 0, 0, 0, 0x44440000, (s32)0xffff4440,
    -12, 0x4444ffff, 0, 0, 0, 0,
    0x4444, 0x444ffff, 0x4fffffff, (s32)0xffff4444, 0, 0,
    0, 0, 0, 0, 4, 0x44f,
    -0xc0000, -0xc000, 0x4ffff400, 0x4ffff40, 0x4fff40, 0x4ffff4,
    0x4fff4, 0x4fff4, 0x44ff, 68, 0, 0,
    0, 0, 0, 0, -0xbc0000, 0x44000000,
    0, 0, 0, 0, 0, 0,
    0x4fff, 0x4ffff, 0x4ffff4, 0x4ffff40, 0x4fff400, 0x4ffff400,
    0x4fff4000, 0x4fff4000, (s32)0xe9000000, 233, (s32)0xeb000000, 235,
    (s32)0xedeb0000, 0xebed, (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xedeb0000, 0xebed, (s32)0xeb000000, 235, (s32)0xe9000000, 233,
    0xe900, 0xe90000, 0xebe9e9, (s32)0xe9e9eb00, (s32)0xedebeb00, 0xebebed,
    (s32)0xefed0000, 0xedef, (s32)0xefed0000, 0xedef, (s32)0xedebeb00, 0xebebed,
    0xebe9e9, (s32)0xe9e9eb00, 0xe900, 0xe90000, (s32)0xe9000000, 233,
    (s32)0xebe90000, 0xe9eb, (s32)0xedebe900, 0xe9ebed, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xedebe900, 0xe9ebed, (s32)0xebe90000, 0xe9eb,
    (s32)0xe9000000, 233,
};

u32 gContigo_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gContigo_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gContigo_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

/// The Lucky Wheels' prizes, three to each icon that wins them
/// (Contigo_PickPrize).
u32 gContigo_WheelsPrizes[] = {
    ITEM_MYTHRIL_SHIRT, ITEM_SILK_SHIRT,  ITEM_RUNNING_SHIRT,
    ITEM_HYPER_BOOTS,   ITEM_QUICK_BOOTS, ITEM_FUR_BOOTS,
    ITEM_ADEPT_RING,    ITEM_WAR_RING,    ITEM_SLEEP_RING,
    ITEM_POTION,        ITEM_VIAL,        ITEM_NUT,
    ITEM_WATER_OF_LIFE, ITEM_PSY_CRYSTAL, ITEM_ELIXIR,
};

s32 gContigo_EnterGameDoorScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, -0x8000),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

s16 gContigo_PaletteSlots[] = {
    89, 90, 91, 92, 93, 94, 95, -1,
};

s16 gContigo_PaletteColors[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gContigo_DimColors[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u16 gContigo_CycleSlots[] = {                                                               
      94, 89, 90, 91, 92, 93, 93, 94, 89, 90, 91, 92, 92, 93, 94, 89, 90, 91,  
      91, 92, 93, 94, 89, 90, 90, 91, 92, 93, 94, 89, 89, 90, 91, 92, 93, 94,  
  };

s32 gFieldEffect_DurationScripts[] = {
    (s32)gContigo_ShortScript,
    (s32)gContigo_MediumScript,
    (s32)gContigo_LongScript,
};

u32 gContigo_WandererScript[] = {                                                               
      39, 2,  22, 15, 0x10000, 22, 16, 0x6666, 4,  0x1520000, 0,  0x34c0000,     1,  4,      
      0x13e0000, 0,  0x34c0000, 1,  4,       0x1300000, 0,  0x3680000,     1,  4,  0x1400000, 0,      0x3740000, 1,      
      4,  0x1500000, 0,  0x3740000, 1,       4,  0x1600000, 0,      0x3680000, 1,  13, 0xffff, 0,          
  };

u32 gContigo_PacerScript[] = {                                                               
      39, 2,  22, 15, 0x10000, 22, 16, 0x6666, 4,  0x13e0000, 0,  0x3740000,     1,  4,      
      0x1520000, 0,  0x3740000, 1,  4,       0x1600000, 0,  0x3680000,     1,  4,  0x1520000, 0,      0x34c0000, 1,      
      4,  0x13e0000, 0,  0x34c0000, 1,       4,  0x1300000, 0,      0x3680000, 1,  13, 0xffff, 0,          
  };

u32 gContigo_JennaReunionScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(164, 0, 360),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gContigo_PiersReunionScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(190, 0, 352),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gContigo_ShebaReunionScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(190, 0, 390),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gContigo_KradenReunionScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(182, 0, 368),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gContigo_IsaacLeavesScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(160, 0, 382),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
    SC_ANIMATION(2),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gContigo_ShopsWatcherScript[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 5 / 32),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 2560),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 128),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 2560),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 128),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gContigo_LeaderVisitsScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(128, 0, 378),
    SC_SYNC,
    SC_MOVE(132, 0, 360),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

u32 gContigo_HostScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(132, 0, 359),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gContigo_StatueScript[] = {
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(120),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_CALL((s32)Contigo_ShowSurprise),
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
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 942, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 808, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 116, 0, 680, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 792, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 804, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, (s32)gContigo_WandererScript, 352, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, (s32)gContigo_PacerScript, 304, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SALT), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_SHINE), MOVEMENT_TYPE_STAND, 56, 0, 706, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_GeoglyphPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 846, 0, 660, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 616, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 756, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, (s32)gContigo_StatueScript, 774, 0, 664, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 588, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 806, 0, 596, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 832, 0, 310, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 312, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 346, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 343, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 720, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 84, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 88, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 112, 0, 130, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 94, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 112, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 733, 0, 91, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 108, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 708, 0, 154, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(161, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 756, 0, 108, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 352, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 352, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 584, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 186, 0, 636, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 616, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 592, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 94, 0, 574, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 586, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 664, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 310, 0, 614, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 688, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

s16 gContigo_ShopsPlacementsReunion[] = {
    59, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_NORTH, 2, 5, -1, 1, 0,
    0, 0, 0, 0, 0, 0, FACING_WEST, 2,
    7, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_WEST, 2, 6, -1, 1, 0,
    0, 0, 0, 0, 0, 0, FACING_WEST, 2,
    56, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_WEST, 2, 0, -1, 1, 0,
    0, 132, 0, 0, 0, 382, 0, 2,
    1, -1, 1, 0, 0, 128, 0, 0,
    0, 366, 0, 2, 3, -1, 1, 0,
    0, 112, 0, 0, 0, 354, 0, 2,
    2, -1, 1, 0, 0, 120, 0, 0,
    0, 398, 0, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

struct ActorPlacement gContigo_ShopsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 158, 0, 352, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_LuckyDicePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 840, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 92, 0, 902, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 872, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 90, 0, 872, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 840, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 300, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 340, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 508, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 548, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 39, CONDITION_ALWAYS, EVENT(Contigo_ClearFirstBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(Contigo_ClearSecondBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(Contigo_ClearBushesBySalt)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42, CONDITION_ALWAYS, EVENT(Contigo_ClearFourthBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43, CONDITION_ALWAYS, EVENT(Contigo_ClearFifthBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44, CONDITION_ALWAYS, EVENT(Contigo_ClearSixthBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45, CONDITION_ALWAYS, EVENT(Contigo_ClearSeventhBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 46, CONDITION_ALWAYS, EVENT(Contigo_ClearEighthBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 47, CONDITION_ALWAYS, EVENT(Contigo_ClearFirstHedgeRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 48, CONDITION_ALWAYS, EVENT(Contigo_ClearSecondHedgeRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 49, CONDITION_ALWAYS, EVENT(Contigo_ClearThirdHedgeRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 50, CONDITION_ALWAYS, EVENT(Contigo_ClearFourthHedgeRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 51, CONDITION_ALWAYS, EVENT(Contigo_ClearFifthHedgeRow)},
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
    {TRIGGER_TALK, ACTOR_WANDERER, CONDITION_ALWAYS, EVENT(Contigo_TalkToWanderer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WANDERER, CONDITION_ALWAYS, EVENT(Contigo_TalkToWanderer)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_SHIP, CONDITION_ALWAYS, EVENT(Contigo_TalkAboutTheShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SHIP, CONDITION_ALWAYS, EVENT(Contigo_TalkAboutTheShip)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(Contigo_ScoopForSalt)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Contigo_SaltJoinsParty)},
    {TRIGGER_EXAMINE, 24, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyWheelsSign)},
    {TRIGGER_EXAMINE, 25, CONDITION_ALWAYS, EVENT(Contigo_ReadLuckyDiceSign)},
    {TRIGGER_EXAMINE, 26, CONDITION_ALWAYS, EVENT(Contigo_ReadSuperDiceSign)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT(Scene_ShowEntranceLocked)},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_GeoglyphTriggers[] = {
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 21},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT(Contigo_EnterLuckyWheels)},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 14, CONDITION_ALWAYS, 23},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(Contigo_EnterSuperLuckyDice)},
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
    {TRIGGER_TALK, ACTOR_SCHOLAR_A, FLAG_SET_OUT_WITH_WINGS, EVENT(Contigo_AskAboutGeoglyph)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_A, CONDITION_ALWAYS, MSGNO(MsgContigoLookCloselyAtTheGeoglyph)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_A, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoICantEvenImagineHow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_A, CONDITION_ALWAYS, MSGNO(MsgContigoWeAllCameTogetherAnd)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_B, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoThatSignHasATranslation)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_B, CONDITION_ALWAYS, MSGNO(MsgContigoImConvincedTheresSomethingHidden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_B, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIDontKnowIfAnyone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_B, CONDITION_ALWAYS, MSGNO(MsgContigoAllRightSoWeveFollowed)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_C, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIWantToGetA)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_C, CONDITION_ALWAYS, MSGNO(MsgContigoAllTheAmazingThingsWeve)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_C, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWouldntThatBeTerribleIf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_C, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouTapOnThe)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_D, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheMerchantHammetTookThe)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_D, CONDITION_ALWAYS, MSGNO(MsgContigoHesAnOddBoyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_D, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoSomeoneToldMeThatHammet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_D, CONDITION_ALWAYS, MSGNO(MsgContigoIvanWasSupposedToBe)},
    {TRIGGER_EXAMINE, 22, CONDITION_ALWAYS, EVENT(Contigo_ExamineWall)},
    {TRIGGER_TALK, ACTOR_SIGN, CONDITION_ALWAYS, EVENT(Contigo_ReadSign)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Contigo_RevealSteps)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Contigo_HideSteps)},
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
    {TRIGGER_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgContigoNoMatterWhatHappensYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgContigoMyHeartIsWithYou)},
    {TRIGGER_STEP, 20, FLAG_CONTIGO_KRADEN_GREETED_PARTY, EVENT(Contigo_ReturnToNewHome)},
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
#undef MSG_COINS_WON
#undef MSG_COINS_LOST
#undef MSG_READ_SIGN
#undef MSG_CONGRATULATIONS
#undef MSG_FIRST_PRIZE
#undef MSG_NEXT_PRIZE
#undef MSG_COME_AGAIN
