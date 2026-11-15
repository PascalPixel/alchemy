/// @file
/// Contigo, the village below Jupiter Lighthouse, in the rooms of this
/// overlay: its shops, inn and Sanctum (Contigo_TalkToWeaponMerchant,
/// Contigo_TalkToInnkeeper, Contigo_TalkToHealer), the bushes where Scoop
/// finds the Venus Djinni Salt, and the cycling colours of its water
/// (Contigo_StartSlowPaletteCycle).

#include "types.h"
#include "window/message_argument.h"
#include "item_ids.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "types.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "text/msg_ids.h"

/// The village's people and Djinn (gContigo_VillagePlacements).
enum ContigoVillageActorId {
  /// "The Anemos blood in our veins often brings about strange..."
  ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND = ACTOR_FIRST_PLACED,
  /// "The people of Anemos possessed power over the wind."
  ACTOR_WOMAN_ON_THE_ANEMOS = ACTOR_FIRST_PLACED + 3,
  /// "The Anemos geoglyphs show a boat with wings..."
  ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS = ACTOR_FIRST_PLACED + 5,
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
  /// "Long ago, the people of Hesperia attacked Atteka."
  ACTOR_OLD_MAN_ON_HESPERIA = ACTOR_FIRST_PLACED + 3,
  ACTOR_ITEM_MERCHANT = ACTOR_FIRST_PLACED + 5,
  /// "Soon, the siblings with the power of the Anemos will return."
  ACTOR_YOUNG_MAN_ON_THE_SIBLINGS,
  /// "The power of the Anemos is all but gone in Contigo."
  ACTOR_WOMAN_ON_THE_ANEMOS_POWER,
  /// The baby whose furrowed brow brings the wind (Contigo_BabyThinks).
  ACTOR_BABY = ACTOR_FIRST_PLACED + 9,
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  ACTOR_INNKEEPER,
  /// The workers on the wings, resting at the inn: "I worked myself half to
  /// death yesterday."
  ACTOR_WORN_OUT_WORKER = ACTOR_FIRST_PLACED + 16,
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
  ACTOR_LUCKY_DICE_HOST = ACTOR_FIRST_PLACED + 6,
  ACTOR_LUCKY_DICE_GUIDE,
  ///@}
  ACTOR_LUCKY_WHEELS_ATTENDANT = ACTOR_FIRST_PLACED + 10
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
  ACTOR_FIRST_WING_WORKER,
  /// The four scholars in long robes studying the geoglyph.
  ACTOR_FIRST_SCHOLAR = ACTOR_FIRST_WING_WORKER + 4,
  /// The sign with a translation of the geoglyph's characters.
  ACTOR_SIGN = ACTOR_FIRST_SCHOLAR + 4,
  /// The wings' parts lying about: three frames, five metal feathers and
  /// seven loose pieces.
  ACTOR_FIRST_WING_FRAME,
  ACTOR_FIRST_METAL_FEATHER = ACTOR_FIRST_WING_FRAME + 3,
  ACTOR_FIRST_LOOSE_PART = ACTOR_FIRST_METAL_FEATHER + 5
};
#include "field/event_inline.h"
#include "field/actor_script.h"
#include "fixed_math.h"

struct ContigoObject;

void Actor_MoveToObject(s32 actor, s32 target);
void FieldCamera_WaitForArrival(void);
void Party_AddIsaacsParty(void);
void Contigo_SendToLuckyWheels(s32 mode);
void Contigo_OpenGameDoor(s32 a, s32 b, s32 c, s32 d);
void TileCopies_CopyTouchedCells(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 mode);

static inline void Contigo_Tiles(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 mode) {
  TileCopies_CopyTouchedCells(srcX, srcY, width, height, destX, destY, mode);
}
#include "system/task.h"
#include "field/actor.h"
#include "field/scene_tables.h"
#include "scene/scene.h"
#include "flags.h"
#include "djinn_ids.h"
#include "shared/tables.h"
#include "system/field_effect.h"
#include "field/field_sequence_event.h"
#include "window/item_icon.h"
#include "field/event_commands.h"
#include "field/field_bank.h"
#include "game/game_bank.h"
#include "system/system_bank.h"
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

extern u32 gContigo_Topics[];
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

extern u16 *gPaletteSlots;
extern u16 *gPaletteCycleColors;

void Contigo_TeleportIn(void);

extern u16 gPaletteCycleBright;
extern u16 gPaletteCycleTimer;
extern u16 gPaletteCycleDelay;
extern u16 gPaletteCycleLength;
extern u16 gPaletteCycleIndex;

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

extern u16 *gPaletteColors;
extern u16 *gPaletteCycleSlots;

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
  PaletteFade_SetTarget(0x10000, 0);
  if (gMainRoutineWork.teleportState == 2)
    Contigo_TeleportIn();
}

void Contigo_StartSlowPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

void Contigo_StopPaletteCycle(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = 256;
  Task_Remove(PaletteCycle_Fade);
}

void Contigo_StartFastPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 2;
  gPaletteCycleTimer = 2;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

s32 List_CountEntries(u16 *list) {
  s32 count = 0;

  while (list[count] != 0xffff)
    count++;
  count--;
  return count;
}

#include "shared/palette_cycle_fade.h"

void Teleport_PlayWhirl(void);

void Contigo_TeleportIn(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_SetTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  gFieldWork->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Contigo_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  wait(40);
  Contigo_StopPaletteCycle();
  PaletteFade_SetTarget(0x10000, 1);
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

/* VIEW(contigo): tag struct TalkActor as this part has it; the joined file has
 * another; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/contigo.c:839: redefinition of `struct
 * TalkActor'. */
#define TalkActor TalkActor__contigo
/* VIEW(contigo): tag struct ContigoEventWork as this part has it; the joined
 * file has another; tried with the definition the rest of the object has, it
 * does not compile in tla-ja: games/tla/src/scene/contigo.c:2108: redefinition
 * of `struct ContigoEventWork'. */
#define ContigoEventWork ContigoEventWork__contigo
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
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
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
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
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

void Contigo_SaltJoinsParty(void) {
  Djinn_JoinParty(ACTOR_SALT, DJINNI_SALT / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SALT));
}

void Contigo_FreedDjinniJoinsParty(void) {
  Djinn_JoinParty(ACTOR_SHINE, DJINNI_SHINE / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SHINE));
  GameFlag_Set(2534);
}

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
    u32 angle = (u16)ArcTan2(other->z.fixed - self->z.fixed, *to - *from);
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
  if (mode == 141) {
    effect = Ram_HeapSlots->fieldEffectWork;
    if (effect[0x713] != 0)
      goto near;
  }
  if (self->blockMarked == 1)
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
      if (GameFlag_Get(0x9b2) != 0)
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

void Contigo_ExamineWall(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix examined the wall. There are characters carved into it...."
  FieldMessage_Show((s32)MsgContigoFelixExaminedTheWallThere, 1);
  Event_End();
}

void Contigo_ReadSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix examined the sign. The characters from the wall are translated..."
  FieldMessage_Show((s32)MsgContigoFelixExaminedTheSignThe, 1);
  Event_End();
}

void Contigo_ReadLuckyWheelsSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Turn the slot and get a prize! The lottery slots are over here."
  FieldMessage_Show((s32)MsgContigoTurnTheSlotAndGet, 1);
  Event_End();
}

void Contigo_ReadLuckyDiceSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Drop some cash and double up! The hi-lo dice are over here!"
  FieldMessage_Show((s32)MsgContigoDropSomeCashAndDouble, 1);
  Event_End();
}

void Contigo_ReadSuperDiceSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "You never know what the dice will do next! Come and give our dice a..."
  FieldMessage_Show((s32)MsgContigoYouNeverKnowWhatThe, 1);
  Event_End();
}

#include "shared/scene_show_entrance_locked.h"

void Contigo_AskAboutWindLegend(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoDoYouKnowAnythingAbout);
  // "Do you know anything about a legend about these people who could..."
  Event_AskYesNoAndShowResponse(8, 0);
  Event_End();
}

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[80];
  u16 timer;
  s16 pause;
};

s32 Trig_Cos(s32 angle);

void Contigo_CircleGlow(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) {
    effect->pause = ((Random16() << 4) >> 16) + 8;
  }
  if (++effect->timer == 101)
    Actor_Release(effect);
}

void Contigo_RaiseMotes(void) {
  if ((gFrameCount & 7) == 0) {
    struct Actor *object = Actor_Spawn(308, 0x02c20000, 52428, 0x740000);

    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      object->update = Contigo_CircleGlow;
      object->motionFlags = 0;
      object->timer = -(s32)((u32)(Random16() * 16) >> 16) * 2 + 20;
      object->userValue = 0;
      object->gravity = 0x1999;
      object->velocityY = px(3);
      object->facing = Random16();
    }
  }
}

struct ContigoGlow {
  u8 unk00[0x0c];
  s32 y;
  u8 unk10[8];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[0x35];
  u8 motionFlags;
};

/// The glow that grows over actor 17 and sinks away again, then the scene goes
/// on.
void Contigo_BabyThinks(void) {
  struct ContigoGlow *effect;
  s32 x = px(706);
  s32 z = px(116);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(0x9b2))
    // "The baby's forehead is furrowed, like it's thinking."
    FieldMessage_Show((s32)MsgContigoTheBabysForeheadIsFurrowedLike, 1);
  else
    // "The baby's forehead is furrowed, like it's thinking."
    FieldMessage_Show((s32)MsgContigoTheBabysForeheadIsFurrowed, 1);
  Actor_SetAnimationById(ACTOR_BABY, 3);
  PaletteFade_SetSource(0x10000, 0);
  PaletteFade_SetTarget(0x204084, 1);
  PaletteFade_StartInterpolation(16);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(
      ((struct ContigoObject *)Actor_GetById(ACTOR_BABY)), 2);
  effect = ((struct ContigoGlow *)Import_ActorCreate(280, x, 0, z));
  if (effect != 0) {
    Actor_SetSpriteFlags(effect, 0);
    Actor_SelectAnimation(effect, 2);
    effect->motionFlags = 0;
    effect->scaleX = 0x1999;
    effect->scaleY = 0x1999;
    i = 0;
  grow:
    effect->scaleX += 0x666;
    effect->scaleY += 0x666;
    wait(1);
    i++;
    if (i < 28)
      goto grow;
  }
  Task_Add(Contigo_RaiseMotes, TASK_PASS_VBLANK);
  Event_Wait(120);
  Task_Remove(Contigo_RaiseMotes);
  if (effect != 0) {
    i = 0;
  shrink:
    effect->scaleX += 0xfffff99a;
    effect->scaleY += 0xfffff99a;
    effect->y += 0x8000;
    wait(1);
    i++;
    if (i < 28)
      goto shrink;
    Actor_Release(effect);
  }
  BattleEffect_SetObjectUpdateMode(
      ((struct ContigoObject *)Actor_GetById(ACTOR_BABY)), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  PaletteFade_SetTarget(0x10000, 1);
  PaletteFade_StartInterpolation(16);
  if (GameFlag_Get(0x9b2))
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
  Actor_SetAnimationById(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, 3);
  Actor_SetAnimationAndWait(ACTOR_WOMAN_ON_THE_ANEMOS_POWER, 3);
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

void Contigo_TalkToItemMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_ITEMS, 13);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
      Event_SetMessageId((s32)MsgContigoItsAlwaysSuchALetdown);
    else
      Event_SetMessageId((s32)MsgContigoTheVillagersAreWorkingTheir);
    // "The villagers are working their hardest to complete the wings and..."
    Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
    Event_End();
  }
}

void Contigo_TalkToWeaponMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_WEAPONS, 18);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
      Event_SetMessageId((s32)MsgContigoACannonYeahIThink);
    else
      Event_SetMessageId((s32)MsgContigoItWasALongstandingTradition);
    // "It was a longstanding tradition that people in Atteka would take a..."
    Event_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
    Event_End();
  }
}

void Contigo_TalkToArmorMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_CONTIGO_ARMOR, 19);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
      Event_SetMessageId((s32)MsgContigoNowThatTheWingsAre);
    else
      Event_SetMessageId((s32)MsgContigoMiningMaterialsInTheRuins);
    // "Mining materials in the ruins is dangerous work, so my armor is..."
    Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
    Event_End();
  }
}

void Contigo_TalkToInnkeeper(void) {
  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(CONTIGO_INN, 20);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
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

void Contigo_TalkToHealer(void) {
  if (Leader_IsFacingNorth()) {
    Sanctum_Run(28);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
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

void Contigo_AskAboutSuperDice(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoSoTellMePalHave);
  // "So, tell me, pal... Have you ever played Superdice before?"
  Event_AskYesNoAndShowResponse(10, 0);
  Event_End();
}

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

void Contigo_TalkToWheelsAttendant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Contigo_SendToLuckyWheels(0);
  Event_End();
}

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

void Contigo_AskAboutGeoglyph(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigoHaveYouTakenAGood);
  // "Have you taken a good look at the Anemos geoglyph?"
  Event_AskYesNoAndShowResponse(14, 0);
  Event_End();
}

/// The bytes of an actor this handler uses. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Contigo_FaceTalker reads it
};

void Contigo_TalkToWanderer(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct TalkActor *object =
      ((struct TalkActor *)Actor_GetById(ACTOR_WANDERER));
  s16 angle = object->angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->psynergy == 0) {
    Actor_FaceActorById(ACTOR_WANDERER, gMainRoutineWork.leaderActorId, 0);
    if (GameFlag_Get(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigoIveSeenALotOf);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0)
      Event_SetMessageId((s32)MsgContigoThatCraterBehindTheVillage);
    else
      Event_SetMessageId((s32)MsgContigoIfYouWantToGet);
  } else {
    if (GameFlag_Get(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigoWaitTheCityOfAnemos);
    else if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0)
      Event_SetMessageId((s32)MsgContigoCitiesFlyingUpIntoThe);
    else
      Event_SetMessageId((s32)MsgContigoTheAnemosGeoglyphsAreThe);
  }
  Event_ShowMessageAndWait(ACTOR_WANDERER, 0);
  if (work->psynergy == 0)
    object->angle = angle;
  object->unk63 = 0;
  Event_End();
}

struct ContigoObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x5b];
  u8 unk63;
};

void Contigo_TalkAboutTheShip(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct ContigoObject *object =
      ((struct ContigoObject *)Actor_GetById(ACTOR_GIRL_ON_THE_SHIP));
  s16 angle = object->angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->unk63 = 1;
  if (event->psynergy == 0) {
    Actor_FaceActorById(ACTOR_GIRL_ON_THE_SHIP, gMainRoutineWork.leaderActorId,
                        0);
    if (GameFlag_Get(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigoItWasntWhatIExpected);
    else
      Event_SetMessageId((s32)MsgContigoOurPeopleHaveLaboredFor);
  } else {
    if (GameFlag_Get(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigoAllOfThoseLegendsDid);
    else
      Event_SetMessageId((s32)MsgContigoWeBuiltTheWingsOf);
  }
  Event_ShowMessageAndWait(ACTOR_GIRL_ON_THE_SHIP, 0);
  if (event->psynergy == 0)
    object->angle = angle;
  object->unk63 = 0;
  Event_End();
}

void Teleport_PlaySparks(void);

void Contigo_TeleportOut(void) {
  Contigo_StartSlowPaletteCycle();
  Teleport_PlaySparks();

  gMainRoutineWork.teleportState = 2;
  Event_SetDestination(SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, 99);
  Event_Wait(40);
  Contigo_StopPaletteCycle();
}

void Contigo_ClearFirstBushes(void) { Contigo_Tiles(3, 50, 1, 1, 8, 54, 0); }

void Contigo_ClearSecondBushes(void) { Contigo_Tiles(4, 43, 2, 1, 2, 43, 0); }

static __inline__ void Contigo_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d,
                                                s32 e, s32 f, s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

void Contigo_ClearBushesBySalt(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Contigo_CopyTouchedCells(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Get(0xfc7) == 0) {
    Actor_SetPosition(66, 280, 664);
    if (FIELD_CELL(leader->x.fixed) == 17 &&
        FIELD_CELL(leader->z.fixed) == 41) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -16);
      leader->velocityY = px(6);
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

void Contigo_ClearFourthBushes(void) { Contigo_Tiles(29, 35, 1, 2, 23, 47, 0); }

void Contigo_ClearFifthBushes(void) { Contigo_Tiles(32, 55, 2, 3, 29, 58, 0); }

void Contigo_ClearSixthBushes(void) { Contigo_Tiles(3, 60, 1, 1, 3, 56, 0); }

void Contigo_ClearSeventhBushes(void) { Contigo_Tiles(7, 62, 2, 1, 7, 58, 0); }

void Contigo_ClearEighthBushes(void) { Contigo_Tiles(10, 63, 2, 1, 10, 59, 0); }

void Contigo_ClearFirstHedgeRow(void) {
  Contigo_Tiles(32, 55, 3, 1, 40, 57, 0);
}

void Contigo_ClearSecondHedgeRow(void) {
  Contigo_Tiles(32, 55, 5, 1, 39, 58, 0);
}

void Contigo_ClearThirdHedgeRow(void) {
  Contigo_Tiles(32, 55, 5, 1, 39, 59, 0);
}

void Contigo_ClearFourthHedgeRow(void) {
  Contigo_Tiles(32, 55, 5, 1, 39, 60, 0);
}

void Contigo_ClearFifthHedgeRow(void) {
  Contigo_Tiles(32, 55, 3, 1, 40, 61, 0);
}

void Contigo_WidenCameraBounds(void) {
  Field_SetCameraBounds(0, 0x2000000, 0x3700000, 0x4000000);
  GameFlag_Set(FLAG_SCENE_TEMP_00);
}

void Contigo_NarrowCameraBounds(void) {
  Field_SetCameraBounds(0, 0x2000000, 0x2000000, 0x4000000);
  GameFlag_Clear(FLAG_SCENE_TEMP_00);
}

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

void Contigo_RevealSteps(void) {
  MapGrid_CopyCellsTo(27, 4, 48, 12, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(27, 6, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 1, 2, 1, 48, 19);
}

void Contigo_HideSteps(void) {
  MapGrid_CopyCellsTo(27, 8, 48, 12, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(27, 10, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 0, 2, 1, 48, 19);
}

void Contigo_ForceFreesDjinni(void) {
  struct Actor *object = Actor_GetById(ACTOR_SHINE);
  struct Actor *leader;
  struct EffectOptions options;
  s32 velocity[3];
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
                      velocity[0], velocity[1] + 0x1999, velocity[2], 0x20000,
                      &options);
  }
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SHINE), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHINE), 1);
  Actor_SetSpeed(ACTOR_SHINE, 3);
  object->velocityY = px(12);
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
  Actor_TurnToFaceEachOther(gMainRoutineWork.leaderActorId, 17, 0);
  Event_Wait(10);
  GameFlag_Set(2535);
  Event_End();
}

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
  object->collisionFlags = 8;
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  object->y.fixed = px(64);
}

/// Lays out the wings' parts by the geoglyph: each frame and feather shows
/// its own piece and lies flat, and the loose parts block the way.
void Contigo_SetUpWingParts(void) {
  Actor_SetAnimationById(ACTOR_FIRST_WING_FRAME, 0);
  Actor_SetAnimationById(ACTOR_FIRST_WING_FRAME + 1, 1);
  Actor_SetAnimationById(ACTOR_FIRST_WING_FRAME + 2, 2);
  Actor_SetAnimationById(ACTOR_FIRST_METAL_FEATHER, 0);
  Actor_SetAnimationById(ACTOR_FIRST_METAL_FEATHER + 1, 1);
  Actor_SetAnimationById(ACTOR_FIRST_METAL_FEATHER + 2, 2);
  Actor_SetAnimationById(ACTOR_FIRST_METAL_FEATHER + 3, 3);
  Actor_SetAnimationById(ACTOR_FIRST_METAL_FEATHER + 4, 4);
  Contigo_PlaceWingPart(ACTOR_FIRST_WING_FRAME);
  Contigo_PlaceWingPart(ACTOR_FIRST_WING_FRAME + 1);
  Contigo_PlaceWingPart(ACTOR_FIRST_WING_FRAME + 2);
  Contigo_PlaceWingPart(ACTOR_FIRST_METAL_FEATHER);
  Contigo_PlaceWingPart(ACTOR_FIRST_METAL_FEATHER + 1);
  Contigo_PlaceWingPart(ACTOR_FIRST_METAL_FEATHER + 2);
  Contigo_PlaceWingPart(ACTOR_FIRST_METAL_FEATHER + 3);
  Contigo_PlaceWingPart(ACTOR_FIRST_METAL_FEATHER + 4);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_LOOSE_PART), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_LOOSE_PART + 1), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_LOOSE_PART + 2), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_LOOSE_PART + 3), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_LOOSE_PART + 4), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_LOOSE_PART + 5), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_LOOSE_PART + 6), 0);
  Actor_GetById(ACTOR_FIRST_LOOSE_PART)->collisionFlags |= 8;
  Actor_GetById(ACTOR_FIRST_LOOSE_PART + 1)->collisionFlags |= 8;
  Actor_GetById(ACTOR_FIRST_LOOSE_PART + 2)->collisionFlags |= 8;
  Actor_GetById(ACTOR_FIRST_LOOSE_PART + 3)->collisionFlags |= 8;
  Actor_GetById(ACTOR_FIRST_LOOSE_PART + 4)->collisionFlags |= 8;
  Actor_GetById(ACTOR_FIRST_LOOSE_PART + 5)->collisionFlags |= 8;
  Actor_GetById(ACTOR_FIRST_LOOSE_PART + 6)->collisionFlags |= 8;
  Actor_SetAnimationById(ACTOR_FIRST_LOOSE_PART, 0);
  Actor_SetAnimationById(ACTOR_FIRST_LOOSE_PART + 5, 0);
  Actor_SetAnimationById(ACTOR_FIRST_LOOSE_PART + 6, 0);
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
  GameFlag_Set(0x9e5);
  wait(1);
  Event_LeaveFor(508, 0);
  Event_SetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE, 20);
}

struct ContigoTopicState {
  u8 unk000[340];
  s8 cursor[6];
};

s32 Contigo_PickTopic(s32 topic) {
  struct ContigoTopicState *state;
  s32 cursor;
  s32 variant;
  s32 index;

  if (topic < 0)
    return 0;
  if (topic == 5)
    topic = (s32)((u32)(Random16() * 5) >> 16);
  state = (struct ContigoTopicState *)&gMainRoutineWork;
  cursor = state->cursor[topic];
  variant = (cursor + (s32)((u32)(Random16() * 2) >> 16) + 4) % 3;
  state->cursor[topic] = variant;
  index = topic * 2;

  return gContigo_Topics[index + topic + variant];
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
    GameFlag_Set(0x9e5);
    wait(1);
    Event_LeaveFor(509, 0);
    Event_SetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE, 19);
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
    GameFlag_Set(0x9e5);
    wait(1);
    Event_LeaveFor(SEQUENCE_SUPER_DICE, 0);
    Event_SetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE, 18);
  } else {
    gFieldWork->message++;
    // "Oh really? That's a shame!"
    Event_ShowMessageAndWait(ACTOR_SUPER_LUCKY_DICE_HOST, 0);
  }
  Event_End();
}

void Contigo_ShrinkActor(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->radius = 8;
}

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

void Contigo_EnterLuckyWheels(void) {
  Contigo_OpenGameDoor(27, 0, 42, 13);
  Import_EventSetExitRequest(22);
}

void Contigo_EnterLuckyDice(void) {
  Contigo_OpenGameDoor(27, 4, 48, 12);
  Import_EventSetExitRequest(23);
}

void Contigo_EnterSuperLuckyDice(void) {
  Contigo_OpenGameDoor(27, 0, 54, 13);
  Import_EventSetExitRequest(24);
}

extern u32 gContigo_JennaReunionScript[];
extern u32 gContigo_PiersReunionScript[];
extern u32 gContigo_ShebaReunionScript[];
extern u32 gContigo_KradenReunionScript[];
extern u32 gContigo_IsaacLeavesScript[];
extern const u8 gContigo_SparkleWork[];

/// The reunion scene. It opens with "Glad you kept your promise".
void Contigo_TheReunion(void) {
  struct ContigoObject *object;
  s32 answer;
  /* FAKEMATCH(pin): the leader pinned to r5, reloaded after each yes/no prompt
   * and copied to r0 in each branch. */
  register s32 owner __asm__("r5");
  s32 angle;

  object = ((struct ContigoObject *)Actor_GetById(ACTOR_HAMA));
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
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
  Actor_SetAnimationById(ACTOR_KRADEN, 4);
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
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
  Actor_SetAnimationById(ACTOR_KRADEN, 4);
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
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
  owner = gMainRoutineWork.leaderActorId;
  if (answer == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationById(ACTOR_IVAN, ANIM_NOD);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    gFieldWork->message++;
  } else {
    Actor_WaitForAnimationChange(owner);
    gFieldWork->message++;
    Actor_SetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 80);
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, 4);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHEAST);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | 1, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
  owner = gMainRoutineWork.leaderActorId;
  if (answer == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    gFieldWork->message++;
  } else {
    Actor_WaitForAnimationChange(owner);
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    gFieldWork->message++;
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessageAndWait(ACTOR_ISAAC, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationById(ACTOR_KRADEN, 4);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Sound_Play(MUS_SILENCE);
  Actor_SetSpriteColor(((struct ContigoObject *)Actor_GetById(ACTOR_HAMA)), 15);
  angle = FACING_NORTH_NORTHWEST;
  Actor_SetHorizontalPositionAndAngleWithTerrain(ACTOR_HAMA, px(160), px(466),
                                                 angle);
  wait(1);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, 8);
  Actor_WatchAndSetScript(ACTOR_JENNA, 8);
  Actor_WatchAndSetScript(ACTOR_PIERS, 8);
  Actor_WatchAndSetScript(ACTOR_SHEBA, 8);
  Actor_WatchAndSetScript(ACTOR_ISAAC, 8);
  Actor_WatchAndSetScript(ACTOR_GARET, 8);
  Actor_WatchAndSetScript(ACTOR_MIA, 8);
  Actor_WatchAndSetScript(ACTOR_IVAN, 8);
  Actor_SetSpriteColor(((struct ContigoObject *)Actor_GetById(ACTOR_HAMA)), 0);
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
  Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
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
  Actor_SetAnimationById(ACTOR_HAMA, 3);
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
  Actor_SetAnimationById(ACTOR_HAMA, 4);
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
  Actor_SetAnimationById(ACTOR_HAMA, 4);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Contigo_StartSparkle(object, gContigo_SparkleWork);
  Event_Wait(80);
  Contigo_StopSparkle(object);
  Actor_SetAnimationAndWait(ACTOR_HAMA, 4);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Contigo_StartSparkle(object, gContigo_SparkleWork);
  Event_Wait(80);
  Contigo_StopSparkle(object);
  Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationById(ACTOR_HAMA, 4);
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
  Actor_SetAnimationById(ACTOR_HAMA, 4);
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
  Actor_SetAnimationById(ACTOR_HAMA, 3);
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
    Actor_SetAnimationById(ACTOR_KRADEN, 3);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    gFieldWork->message++;
  } else {
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
    gFieldWork->message++;
    Actor_SetAnimationById(ACTOR_KRADEN, 4);
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

void Contigo_ReturnToNewHome(void) {
  if (GameFlag_Get(0x9b2)) {
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
    Actor_SetAnimationById(ACTOR_HAMA, 4);
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
    Actor_SetAnimationById(ACTOR_HAMA, 4);
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
    Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
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
    Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
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
    Actor_SetAnimationById(ACTOR_HAMA, 4);
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
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
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
    Actor_SetAnimationById(ACTOR_KRADEN, 4);
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
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
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
    Actor_SetAnimationById(ACTOR_KRADEN, 3);
    // "When you burn your hand on something hot, you blow on it to cool it..."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetAnimationById(ACTOR_ISAAC, ANIM_NOD);
    // "Right, yeah... I've always done that without thinking about it, but..."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
    FieldActor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // "When Mercury Lighthouse was lit, this didn't happen, though. I..."
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
    Actor_SetAnimationById(ACTOR_KRADEN, 3);
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
    Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
    // "It's all right. I'm happy that you cared so much..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
    Actor_SetAnimationById(ACTOR_KRADEN, 3);
    // "All right, Felix! Let's head to Mars Lighthouse!"
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
    Actor_SetSpeed(ACTOR_IVAN, 1.2);
    Actor_SetSpeed(ACTOR_ISAAC, 1.2);
    Actor_SetSpeed(ACTOR_KRADEN, 1.2);
    Actor_SetMovementTypeById(ACTOR_IVAN, gContigo_HostScript);
    Actor_SetMovementTypeById(ACTOR_ISAAC, gContigo_HostScript);
    Actor_SetMovementTypeAndWaitById(ACTOR_KRADEN, gContigo_HostScript);
    GameFlag_Set(2513);
    Event_End();
  }
}

struct SpriteAttributes {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
};

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

void Contigo_KeepPriorityWithLeader(void) {
  if (Actor_GetById(65) != 0) {
    struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);

    Actor_SetSpritePriority(
        65, ((struct SpriteAttributes *)owner->sprite)->priority);
  }
}

void MapGrid_CenterOnFollowedPoint(void);

/// Schedules Contigo_KeepPriorityWithLeader and gives objects 14 and 15 their
/// flag bytes and Contigo_FaceTalker as their update (object 15 brought in and
/// placed while flag 0xa21 is set and 0x9b2 clear), places or hides object 17
/// by flags 0x9e7 and 0x9e6, sets flag 0x200 on entrances 1 to 4 unless flag
/// 0x109 is set, and while flag 0x200 is set has the camera follow the leader;
/// it ends with Palette_SetColors.
void Contigo_InitializeVillage(void) {
  struct Actor *object = Actor_GetById(ACTOR_WANDERER);
  s32 zero;

  Task_Add(Contigo_KeepPriorityWithLeader, TASK_PASS_VBLANK);
  /* FAKEMATCH(temporary): the second byte's 0 built after the first byte's
   * address. */
  zero = 0;
  object->riseCounter = 1;
  object->blockMarked = zero;
  *(void **)((u8 *)object + 108) = Contigo_FaceTalker;
  if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 && GameFlag_Get(0x9b2) == 0) {
    Actor_Stop(ACTOR_GIRL_ON_THE_SHIP);
    Actor_SetPosition(ACTOR_GIRL_ON_THE_SHIP, 0, 0);
    Actor_SetPosition(ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, 0, 0);
    Contigo_OpenGamesRooms();
  } else {
    object = Actor_GetById(ACTOR_GIRL_ON_THE_SHIP);
    object->riseCounter = 0;
    object->blockMarked = 0;
    *(void **)((u8 *)object + 108) = Contigo_FaceTalker;
  }
  if (GameFlag_Get(0x9e7) != 0 && GameFlag_Get(0x9e6) == 0) {
    Actor_SetPosition(ACTOR_SHINE, 32, 708);
  } else {
    Actor_SetSpriteColor(Actor_GetById(ACTOR_SHINE), 15);
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
      Field_SetCameraBounds(0, 0x2000000, 0x2000000, 0x4000000);
      break;
    }
  }
  if (GameFlag_Get(FLAG_SCENE_TEMP_00) != 0) {
    Field_SetCameraBounds(0, 0x2000000, 0x3700000, 0x4000000);
    FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  Palette_SetColors(gContigo_PaletteSlots, gContigo_PaletteColors,
                    gContigo_DimColors, gContigo_CycleSlots);
}

void Contigo_InitializeGeoglyph(void) {
  Contigo_ShrinkActor(18);
  if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0) {
    MapGrid_CopyCellAttributes(44, 40, 8, 3, 44, 36);
    Actor_SetPosition(ACTOR_FIRST_WING_FRAME, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_WING_FRAME + 1, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_WING_FRAME + 2, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_METAL_FEATHER, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_METAL_FEATHER + 1, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_METAL_FEATHER + 2, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_METAL_FEATHER + 3, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_METAL_FEATHER + 4, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_LOOSE_PART, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_LOOSE_PART + 1, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_LOOSE_PART + 2, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_LOOSE_PART + 3, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_LOOSE_PART + 4, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_LOOSE_PART + 5, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_LOOSE_PART + 6, 0, 0);
    Actor_SetPosition(ACTOR_VISITOR_FROM_AFAR, 0, 0);
    Actor_SetPosition(ACTOR_WOMAN_ON_THE_WINGS, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_WING_WORKER + 1, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_WING_WORKER + 2, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_WING_WORKER + 3, 0, 0);
    if (GameFlag_Get(0x9b2) == 0) {
      Actor_SetPosition(ACTOR_FIRST_WING_WORKER, 0, 0);
      Actor_SetPosition(ACTOR_FIRST_SCHOLAR, 0, 0);
      Actor_SetPosition(ACTOR_FIRST_SCHOLAR + 1, 0, 0);
      Actor_SetPosition(ACTOR_FIRST_SCHOLAR + 2, 0, 0);
      Actor_SetPosition(ACTOR_FIRST_SCHOLAR + 3, 0, 0);
    } else {
      Actor_GetById(ACTOR_FIRST_WING_WORKER)->facing = FACING_NORTH_NORTHEAST;
      wait(1);
    }
  } else {
    Contigo_SetUpWingParts();
  }
}

struct ContigoEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Contigo_InitializeShops(void) {
  gFieldWork->transition = 0x209;
  if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT) != 0 &&
        GameFlag_Get(FLAG_REUNITED_AT_CONTIGO) == 0)
      Contigo_TheReunion();
  } else if (gMainRoutineWork.entrance == 21) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  } else {
    Actor_SetAnimationById(ACTOR_BABY, 2);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_BABY), 0);
    if (GameFlag_Get(0x9b2) != 0)
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

struct ContigoPrizes {
  u8 unk000[0x14c];
  s8 list[8];
};

void ItemDisplay_ShowItem(s32 item, s32 count);

void Contigo_InitializeLuckyDice(void) {
  s32 diff;
  s8 *list;
  s8 *prize;
  s32 item;

  gFieldWork->transition = 0x209;
  Contigo_ShrinkActor(13);
  Contigo_ShrinkActor(16);
  Contigo_ShrinkActor(17);
  Contigo_ShrinkActor(19);
  Contigo_ShrinkActor(20);
  Actor_SetSpriteColor(
      ((struct ContigoObject *)Actor_GetById(ACTOR_LUCKY_DICE_HOST)), 1);
  Actor_SetSpriteColor(
      ((struct ContigoObject *)Actor_GetById(ACTOR_LUCKY_DICE_GUIDE)), 2);
  Actor_SetSpriteColor(
      ((struct ContigoObject *)Actor_GetById(ACTOR_SUPER_LUCKY_DICE_HOST)), 0);
  Actor_SetSpriteColor(
      ((struct ContigoObject *)Actor_GetById(ACTOR_SUPER_LUCKY_DICE_GUIDE)), 3);
  if (!Event_GetFlag(0x9e5))
    return;
  GameFlag_Clear(0x9e5);
  if (gMainRoutineWork.entrance == 18) {
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
  } else if (gMainRoutineWork.entrance == 19) {
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
  } else if (gMainRoutineWork.entrance == 20) {
    list = ((struct ContigoPrizes *)&gMainRoutineWork)->list;
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
      next:
        if (prize == list)
          Event_SetMessageId(MSG_FIRST_PRIZE);
        else
          Event_SetMessageId(MSG_NEXT_PRIZE);
        item = Contigo_PickTopic(*prize);
        MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
        Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
        ItemDisplay_ShowItem(item, 3);
        PartyInventory_GiveItem(item, 0);
        Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
        prize++;
        if (*prize != -1)
          goto next;
      }
      ((struct ContigoPrizes *)&gMainRoutineWork)->list[0] = -2;
      Event_SetMessageId(MSG_COME_AGAIN);
      Event_ShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    }
    Event_End();
  }
}

// The sparkle's update: raises two pairs of sprites over the object, each pair
// starting when its counter is cleared, until both counters pass 31.

struct SparkleSprite {
  u8 unk00[12];
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 unk18;
  u8 unk1c[2];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkleWork {
  struct SparkleSprite sprites[4];
  s16 unka0[2];
  s16 vramEntry;
  s16 unka6;
  struct Actor *object;
};

struct CounterBits {
  u8 low : 1;
  u8 rest : 7;
};

void Actor_SetSpriteColorMode(struct Actor *, u32);
s32 BattleEffect_GetObjectDepth(struct Actor *object);

/// The sparkle's update: raises two pairs of sprites over the object, each pair
/// starting when its counter is cleared, until both counters pass 31.
void Contigo_RaiseSparkle(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedActor;
  u8 *count = (u8 *)object + 99;
  struct SparkleSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (object->riseCounter == 0) {
    if ((*count & 15) == 0) {
      Sound_Play(SE_598);
      work->unka0[work->unka6] = 0;
      work->unka6 ^= 1;
    }
  } else if (object->riseCounter == 1) {
    if (work->unka0[0] > 31 && work->unka0[1] > 31) {
      Actor_SetSpriteColorMode(object, 0);
      VramCache_FreeEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Trig_Sin(work->unka0[i] << 10);
    if (work->unka0[i] >= 0) {
      if (work->unka0[i] <= 31) {
        position.x = object->x.fixed;
        position.y = object->y.fixed + (work->unka0[i] << 16);
        position.z = object->z.fixed;
        Camera_WorldToScreen(&position);
        x = position.x;
        z = position.z;
        sprite = &work->sprites[i * 2];
        for (j = 0; j < 2; j++) {
          sprite->unk0c = x;
          sprite->unk10 = z;
          sprite->unk14 = sine;
          sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) - 1;
          } else {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) + 1;
            sprite->unk10 -= 0x10000;
            sprite->unk18 = -sprite->unk18;
          }
          EffectSprite_Draw(sprite);
          sprite++;
        }
        work->unka0[i]++;
      }
    }
  }
}
#undef BYTE_AT
#undef MSG_COINS_WON
#undef MSG_COINS_LOST
#undef MSG_READ_SIGN
#undef MSG_CONGRATULATIONS
#undef MSG_FIRST_PRIZE
#undef MSG_NEXT_PRIZE
#undef MSG_COME_AGAIN

// The sparkle's setup, Contigo_StopSparkle, which sets byte 0x62 of an object,
// and the overlay's data.

/* VIEW(s3e54): tag struct SparkleSprite as struct SparkleSprite__s3e54;
 * contigo.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:2410: structure has no member named `blend'. */
struct SparkleSprite__s3e54 {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 blend : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x1e];
};

/* VIEW(s3e54): tag struct SparkleWork as struct SparkleWork__s3e54; contigo.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja: games/tla/src/scene/contigo.c:2402:
 * incompatible types in assignment. */
struct SparkleWork__s3e54 {
  struct SparkleSprite__s3e54 sprites[4];
  u16 unka0;
  u16 unka2;
  s16 vramEntry;
  u16 unka6;
  struct Actor *object;
};

/// Sets up the sparkle: loads its picture and its four 16 by 16 sprites, and
/// hands the object to its update, Contigo_RaiseSparkle.
void Contigo_StartSparkle(struct Actor *object,
                          struct SparkleWork__s3e54 *work) {
  struct SparkleSprite__s3e54 *sprite;
  s32 tile;
  s32 row;
  s32 col;

  work->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(work->vramEntry, 256, gContigo_SparkleTiles);
  work->unka6 = 0;
  work->unka0 = 999;
  work->unka2 = 999;
  work->object = object;
  object->riseCounter = 0;
  object->blockMarked = 0;
  object->update = Contigo_RaiseSparkle;
  tile = gVramCacheEntries[work->vramEntry].offset >> 5;
  object->watchedActor = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    for (col = 1, sprite = work->sprites + row + row; col >= 0; col--) {
      EffectSprite_Init(sprite, 16, 16, 0x80004000, tile);
      sprite->blend = 0;
      sprite->palette = 0;
      sprite->priority = BattleEffect_GetObjectPriority(object);
      sprite++;
    }
  }
}

/* VIEW(s3e54): tag struct ContigoObject as struct ContigoObject__s3e54;
 * contigo.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:2434: structure has no member named `unk62'. */
struct ContigoObject__s3e54 {
  u8 unk00[0x62];
  u8 unk62;
};

void Contigo_StopSparkle(struct ContigoObject__s3e54 *object) {
  object->unk62 = 1;
}

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

u32 gContigo_Topics[] = {
    250, 251, 252, 256, 257, 258,
    262, 263, 264, 183, 182, 181,
    189, 186, 188,
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
    SC_ADD(SW_SCALE_X, -Q16_ONE * 5 / 32),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE * 5 / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 2560),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 128),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 2560),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 128),
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
    SCENE_EXIT(1, WorldMap, 54),
    SCENE_EXIT(2, ContigoShops, 1),
    SCENE_EXIT(3, ContigoShops, 2),
    SCENE_EXIT(4, ContigoLuckyDice, 10),
    SCENE_EXIT(5, ContigoShops, 7),
    SCENE_EXIT(6, ContigoLuckyDice, 8),
    SCENE_EXIT(7, ContigoLuckyDice, 9),
    SCENE_EXIT(8, ContigoShops, 5),
    SCENE_EXIT(9, ContigoShops, 6),
    SCENE_EXIT(10, ContigoShops, 3),
    SCENE_EXIT(11, ContigoShops, 4),
    SCENE_EXIT(12, ContigoGeoglyph, 21),
    SCENE_EXITS(ContigoGeoglyph),
    SCENE_EXIT(21, Contigo, 12),
    SCENE_EXIT(22, AnemosInnerSanctumEntrance, 3),
    SCENE_EXIT(23, AnemosInnerSanctumEntrance, 1),
    SCENE_EXIT(24, AnemosInnerSanctumEntrance, 4),
    SCENE_EXITS(ContigoShops),
    SCENE_EXIT(1, Contigo, 2),
    SCENE_EXIT(2, Contigo, 3),
    SCENE_EXIT(3, Contigo, 10),
    SCENE_EXIT(4, Contigo, 11),
    SCENE_EXIT(5, Contigo, 8),
    SCENE_EXIT(6, Contigo, 9),
    SCENE_EXIT(7, Contigo, 5),
    SCENE_EXIT(21, AttekaInletGeode, 3),
    SCENE_EXITS(ContigoLuckyDice),
    SCENE_EXIT(8, Contigo, 6),
    SCENE_EXIT(9, Contigo, 7),
    SCENE_EXIT(10, Contigo, 4),
    SCENE_EXITS_END,
};

struct ActorPlacement gContigo_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_VillagePlacements[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 340, 0, 936, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 942, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 808, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(107, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 116, 0, 680, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 792, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 804, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, (s32)gContigo_WandererScript, 352, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(138, CONDITION_ALWAYS, (s32)gContigo_PacerScript, 304, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SALT), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_SHINE), MOVEMENT_TYPE_STAND, 56, 0, 706, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_GeoglyphPlacements[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 846, 0, 660, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 616, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 756, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, (s32)gContigo_StatueScript, 774, 0, 664, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 588, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 806, 0, 596, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 832, 0, 310, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 312, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 346, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 343, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 720, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 668, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 758, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 858, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 782, 0, 678, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(680, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 669, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_ShopsPlacementsBeforeReunion[] = {
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 84, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(107, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 88, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 112, 0, 130, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 94, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(110, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 112, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 733, 0, 91, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 108, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 708, 0, 154, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(161, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 756, 0, 108, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 352, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 352, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(134, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 584, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(135, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 186, 0, 636, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 616, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(211, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 592, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 94, 0, 574, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 586, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 664, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 310, 0, 614, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 688, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
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
    ACTOR_PLACEMENT(59, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 158, 0, 352, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(2, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_LuckyDicePlacements[] = {
    ACTOR_PLACEMENT(236, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(236, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 840, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 92, 0, 902, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 872, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(107, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 90, 0, 872, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(236, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(236, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 840, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 300, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 340, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(228, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 508, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 548, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 17, 0x9e7, EVENT(Contigo_ForceFreesDjinni)},
    {TRIGGER_TALK, 17, IF_FLAG_SET(0x9e7), EVENT(Contigo_FreedDjinniJoinsParty)},
    {TRIGGER_TALK, 8, FLAG_JUPITER_BEACON_LIT, EVENT(Contigo_AskAboutWindLegend)},
    {TRIGGER_TALK, 8, 0x9b2, MSGNO(MsgContigoAreYouLookingForIsaac)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgContigoIGotStuckDoingChores)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoTheAnemosBloodInOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9b2, MSGNO(MsgContigoTheyLookLikeTheyreIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigoIDidntGetToSee)},
    {TRIGGER_TALK, 9, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoTheBeaconWithinJupiterLighthouse)},
    {TRIGGER_TALK, 9, 0x9b2, MSGNO(MsgContigoEveryonesLeftTownForThe)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgContigoEveryonesBeenGoingOnAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoSurelySomeoneWillComeSoon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x9b2, MSGNO(MsgContigoThePropheciesAreAllComing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgContigoSomePeopleWereTalkingAbout)},
    {TRIGGER_TALK, 10, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIGuessThePeopleOf)},
    {TRIGGER_TALK, 10, 0x9b2, MSGNO(MsgContigoIsThatYourBoatIt)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgContigoItMustBeSoExciting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThePowerOfTheWind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9b2, MSGNO(MsgContigoItsJustLikeTheLegends)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgContigoICouldntFlyInThe)},
    {TRIGGER_TALK, 11, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThePeopleOfAnemosPossessed)},
    {TRIGGER_TALK, 11, 0x9b2, MSGNO(MsgContigoJupiterLighthouseHasBeenLit)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgContigoHammaWasTellingUsThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThereWereTwoSiblingsBorn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x9b2, MSGNO(MsgContigoWhatDoesItMeanWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgContigoIHeardThatTheWorld)},
    {TRIGGER_TALK, 12, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoWhenThePowerOfAnemos)},
    {TRIGGER_TALK, 12, 0x9b2, MSGNO(MsgContigoTheLegendsTellUsThat)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgContigoIfAShipCanRise)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIThinkJupiterLighthouseWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x9b2, MSGNO(MsgContigoTheAncientCityOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgContigoTheCityOfTheAnemos)},
    {TRIGGER_TALK, 13, 0x9b2, MSGNO(MsgContigoTheAnemosGeoglyphsShowA)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgContigoISawTheShipFly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9b2, MSGNO(MsgContigoItsHardToBelieveBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgContigoTheShipLookedLikeIt)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Contigo_TalkToWanderer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT(Contigo_TalkToWanderer)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Contigo_TalkAboutTheShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT(Contigo_TalkAboutTheShip)},
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
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgContigoIveComeALongWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigoTheGuysCarryingTheOther)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgContigoTheWingsAreAlmostDone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgContigoIveBeenReallyBusyLately)},
    {TRIGGER_TALK, 10, 0x9b2, MSGNO(MsgContigoTheWingsWillBeFinished)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgContigoICantBelieveThatA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9b2, MSGNO(MsgContigoWhatDoWeDoIf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgContigoThatFlyingShipLookedSo)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgContigoWhichPartGoesWhereDoes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgContigoThereAreTooManyParts)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgContigoYouveGottaWatchOutCause)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgContigoWhenIStartGettingIrritated)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgContigoJustAFewMoreTouches)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgContigoHowLongHaveWeBeen)},
    {TRIGGER_TALK, 14, 0x9b2, EVENT(Contigo_AskAboutGeoglyph)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgContigoLookCloselyAtTheGeoglyph)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9b2, MSGNO(MsgContigoICantEvenImagineHow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgContigoWeAllCameTogetherAnd)},
    {TRIGGER_TALK, 15, 0x9b2, MSGNO(MsgContigoThatSignHasATranslation)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgContigoImConvincedTheresSomethingHidden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x9b2, MSGNO(MsgContigoIDontKnowIfAnyone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgContigoAllRightSoWeveFollowed)},
    {TRIGGER_TALK, 16, 0x9b2, MSGNO(MsgContigoIWantToGetA)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgContigoAllTheAmazingThingsWeve)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9b2, MSGNO(MsgContigoWouldntThatBeTerribleIf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouTapOnThe)},
    {TRIGGER_TALK, 17, 0x9b2, MSGNO(MsgContigoTheMerchantHammetTookThe)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgContigoHesAnOddBoyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x9b2, MSGNO(MsgContigoSomeoneToldMeThatHammet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgContigoIvanWasSupposedToBe)},
    {TRIGGER_EXAMINE, 22, CONDITION_ALWAYS, EVENT(Contigo_ExamineWall)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Contigo_ReadSign)},
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
    {TRIGGER_TALK, 8, 0x9b2, MSGNO(MsgContigoTheSiblingsWhoInheritedThe)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgContigoIGuessTheLighthouseWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9b2, MSGNO(MsgContigoTheFamiliesOfChildrenWho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigoHammaWillStayWithUs)},
    {TRIGGER_TALK, 9, 0x9b2, MSGNO(MsgContigoImSureTheYoungerBrother)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgContigoIvanDoesntRememberHisMother)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x9b2, MSGNO(MsgContigoIvansMotherLeftHimIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgContigoIWonderHowIvanFeels)},
    {TRIGGER_TALK, 10, 0x9b2, MSGNO(MsgContigoMyFatherDoesntApproveOf)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgContigoFromTheLookOnIvans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9b2, MSGNO(MsgContigoIWishICouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgContigoSomedayImGonnaFightIvan)},
    {TRIGGER_TALK, 11, 0x9b2, MSGNO(MsgContigoLongAgoThePeopleOf)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgContigoBrrrBrrrItsBeenReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x9b2, MSGNO(MsgContigoForALongTimeNow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgContigoJupiterLighthouseHasBeenCalled)},
    {TRIGGER_TALK, 12, 0x9b2, MSGNO(MsgContigoALongTimeAgoThe)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgContigoWinterJustEndedButIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x9b2, MSGNO(MsgContigoEverybodyThinksThePeopleOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgContigoHammaSaysThatIfIvan)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Contigo_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9b2, MSGNO(MsgContigoOhHowIWantTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgContigoNowThatTheWingsAreDone)},
    {TRIGGER_TALK, 14, 0x9b2, MSGNO(MsgContigoSoonTheSiblingsWithThe)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgContigoTheLighthouseHasBeenLit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9b2, MSGNO(MsgContigoIJustKnowIvanBecame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgContigoIvanHasThePowerOf)},
    {TRIGGER_TALK, 15, 0x9b2, MSGNO(MsgContigoThePowerOfTheAnemos)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgContigoItsUpToThePeople)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x9b2, MSGNO(MsgContigoATerribleMisfortuneFellOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgContigoNowThatIThinkAbout)},
    {TRIGGER_TALK, 16, 0x9b2, MSGNO(MsgContigoIDontHaveAnyPowers)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgContigoMomAndDadHaveBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9b2, MSGNO(MsgContigoWheneverMySisterFurrowsHer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgContigoItsTimeToFixThose)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Contigo_BabyThinks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x9b2, MSGNO(MsgContigoBawabaItsHotIWant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgContigoGooItsGettingColderGagoo)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Contigo_TalkToWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x9b2, MSGNO(MsgContigoOfCourseTheyreHereTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgContigoIdSureLikeToSee)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Contigo_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x9b2, MSGNO(MsgContigoNothingElseAroundHereIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgContigoWellIfThereArentGoing)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Contigo_TalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoItsGoodToHaveThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x9b2, MSGNO(MsgContigoOnceAllOurCustomersLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgContigoItsGoingToBeA)},
    {TRIGGER_TALK, 21, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoAllTheWorkOnThe)},
    {TRIGGER_TALK, 21, 0x9b2, MSGNO(MsgContigoFirstTheLighthouseIsLit)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgContigoTheInnHasBecomeSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoOnceTheWingsAreFinished)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x9b2, MSGNO(MsgContigoICantBelieveItAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgContigoTheInnIsBoundTo)},
    {TRIGGER_TALK, 22, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoGottaMakeAllTheBeds)},
    {TRIGGER_TALK, 22, 0x9b2, MSGNO(MsgContigoTheBossSaidItWas)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgContigoGoshWeWereSoBusy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIveGotSoMuchTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0x9b2, MSGNO(MsgContigoAllTheseYearsIThought)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgContigoWhatAreWeGoingTo)},
    {TRIGGER_TALK, 23, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIfIDontMakeDinner)},
    {TRIGGER_TALK, 23, 0x9b2, MSGNO(MsgContigoTheyveFinishedPuttingTheWings)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgContigoNowsMyChanceToExperiment)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIMustBeTheGreatest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, 0x9b2, MSGNO(MsgContigoItWasKindOfImpressive)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgContigoSeeingThatShipTakeFlight)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgContigoDidYouJustArriveIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgContigoIWorkedMyselfHalfTo)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgContigoThePropheciesOfTheAnemos)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgContigoTheWingsOfAnemosAre)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgContigoImAfraidImTooOld)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgContigoIThrewMyBackOut)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgContigoWhenIsThatMiracleOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgContigoIWantToSeeThe)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, EVENT(Contigo_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIfISeeTheWings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, 0x9b2, MSGNO(MsgContigoPerhapsIfISeeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgContigoItWasAShockTo)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoTheresABrightRedSoup))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoAhTheSmellOfFresh))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoContigosFamousForTheseCookies))},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_ShopsReunionTriggers[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgContigoNoMatterWhatHappensYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigoMyHeartIsWithYou)},
    {TRIGGER_STEP, 20, 0x9d1, EVENT(Contigo_ReturnToNewHome)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoTheresABrightRedSoup))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoAhTheSmellOfFresh))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoContigosFamousForTheseCookies))},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_LuckyDiceTriggers[] = {
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Contigo_SendToSuperLuckyDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigoWhenTheyLoseTheyCall)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouWantToLearn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgContigoItsNotAToughGame)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Contigo_ExplainSuperDice)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Contigo_AskAboutSuperDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgContigoThisKidsGotUnluckyWritten)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouLoseTheHi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgContigoItsToughToKnowWhen)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgContigoIAlmostAlwaysGetAt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgContigoICanWinICan)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Contigo_SendToLuckyDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgContigoWinningOrLosingIsPure)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgContigoReadTheSignIfYa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgContigoTheRulesForLuckyDice)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Contigo_ExplainLuckyDice)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Contigo_ExplainLuckyDiceAgain)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Contigo_TalkToWheelsAttendant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgContigoHmmIWonderIfThese)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Contigo_ExplainLuckyWheels)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Contigo_ListLuckyWheelsPrizes)},
    {TRIGGER_TABLE_END},
};
#undef BYTE_AT
#undef MSG_COINS_WON
#undef MSG_COINS_LOST
#undef MSG_READ_SIGN
#undef MSG_CONGRATULATIONS
#undef MSG_FIRST_PRIZE
#undef MSG_NEXT_PRIZE
#undef MSG_COME_AGAIN
#undef ContigoEventWork
#undef TalkActor
