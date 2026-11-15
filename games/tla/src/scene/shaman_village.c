/// @file
/// The Shaman Village, whose people will not talk to outsiders: Moapa, shown
/// the Shaman's Rod, sends the party to Trial Road, where Sheba's Whirlwind
/// clears the sand before the race to the summit; its houses, inn, shop and
/// sanctum, Aroma the Djinni, and the lettering that counts the race down.
#include "../field/actor_set_sprite_priority.c"
#include "resource_ids.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "system/io_write.h"

/// The trial grounds' actors (SceneId_ShamanVillageTrialGrounds), numbering the
/// rows of gShamanVillageTrialGroundsActorPlacements; Moapa, his guards and
/// Kraden keep the same rows in the village's table
/// (gShamanVillageVillageActorPlacements).
enum ShamanVillageTrialActorId {
  /// The six torches about the start of Trial Road (SPRITE_TORCH).
  ACTOR_FIRST_TORCH = ACTOR_FIRST_PLACED,
  ACTOR_LAST_TORCH = ACTOR_FIRST_TORCH + 5,
  /// The sand Whirlwind blows off the stone of Yegelos.
  ACTOR_SAND_MOUND,
  /// Moapa, head of the Shaman tribe: "I cannot lose. I will not lose."
  ACTOR_MOAPA,
  /// His two guards: "Just ahead lies Trial Road, where Hoabna and Yegelos
  /// fought their greatest battles."
  ACTOR_WEST_GUARD,
  ACTOR_EAST_GUARD,
  ACTOR_KRADEN
};

/// Moapa in his house (SceneId_ShamanVillageInn), the fourth row of
/// gShamanVillageInnActorPlacements, with the counter and the innkeeper after.
enum ShamanVillageInnActorId {
  ACTOR_MOAPA_AT_HOME = ACTOR_FIRST_PLACED + 3,
  ACTOR_INN_COUNTER,
  ACTOR_INNKEEPER
};

/// The rest of Moapa's house: the three men who ask the party's names, and the
/// two who plan a picnic and their cook, there once Moapa is beaten.
enum {
  ACTOR_MOAPAS_MAN_A = ACTOR_FIRST_PLACED,
  ACTOR_MOAPAS_MAN_B,
  ACTOR_MOAPAS_MAN_C,
  ACTOR_PICNIC_SHAMAN_A = ACTOR_FIRST_PLACED + 9,
  ACTOR_PICNIC_SHAMAN_B,
  ACTOR_INN_COOK
};

/// The village's shaman who asks the party to look over the mountain.
#define ACTOR_MOUNTAIN_SIDE_SHAMAN (ACTOR_FIRST_PLACED + 5)
/// The sanctum's two shamans after the priest, there once Moapa is beaten.
enum {
  ACTOR_SANCTUM_SHAMAN_A = ACTOR_FIRST_PLACED + 1,
  ACTOR_SANCTUM_SHAMAN_B
};

/// The shop's keeper (SceneId_ShamanVillageShop).
#define ACTOR_SHOPKEEPER (ACTOR_FIRST_PLACED + 4)
/// The sanctum's priest (SceneId_ShamanVillageSanctum).
#define ACTOR_PRIEST ACTOR_FIRST_PLACED
/// Aroma's room (SceneId_ShamanVillageAroma): the pillar Move drops, the
/// fortune teller and her flame, and Aroma, the Jupiter Djinni.
#define ACTOR_AROMA_ROOM_PILLAR (ACTOR_FIRST_PLACED + 6)
/// The small flame before the fortune teller, which flares as she hops.
#define ACTOR_FORTUNE_TELLERS_FLAME (ACTOR_FIRST_PLACED + 8)
#define ACTOR_FORTUNE_TELLER (ACTOR_FIRST_PLACED + 9)
#define ACTOR_AROMA (ACTOR_FIRST_PLACED + 10)
#include "djinn_ids.h"

/// Set once Sheba has cast Whirlwind on the purple rock for Moapa's first test
/// and blown the sand away.
#define FLAG_SHAMAN_VILLAGE_SAND_CLEARED 0x94c
/// Set once Move has dropped the pillar onto cell (32, 11) as a bridge.
#define FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED 0x94e
/// Set as Moapa comes out of his cabin to meet the party
/// (ShamanVillage_MeetMoapa).
#define FLAG_SHAMAN_VILLAGE_MOAPA_MET 0x94f
/// Set once Moapa has told the party the rules of the trial: the Shaman's Rod
/// and eight pieces of equipment.
#define FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD 0x9e0
/// Set once the party has shown Moapa the Shaman's Rod.
#define FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN 0x9e1
/// Set once Moapa has asked the party's names after the battle; he remembers
/// them from then on.
#define FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES 0x9ed
/// Set as the party comes back from beating Moapa on the Trial Road (entrance
/// 30); the inn and the sanctum serve the party from then on.
#define FLAG_MOAPA_BEATEN 0x9fe
/// Set once the fortune-teller has read the party's fortune, for this visit.
#define FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD FLAG_AREA_TEMP_01

#include "../field/field_effect_move.c"

#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "system/field_effect.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/event.h"
#include "scene/scene.h"
#include "graphics/resource.h"
#include "window/window.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "game/item.h"
#include "window/item_icon.h"
#include "system/task.h"
#include "system/affine.h"
#include "graphics/vram_tab.h"
#include "system/io_write_queue.h"
#include "edition.h"
#include "system/heap_release.h"
#include "system/input.h"

struct ItemDefinition;

extern s16 gShamanVillage_WaveLines[];

void ShamanVillage_WaveWhirlwindLines(void);

extern u32 gShamanVillage_KradenStandsByRodScript[];
extern u32 gShamanVillage_PiersStandsByRodScript[];
extern u32 gShamanVillage_ShebaStandsByRodScript[];
extern u32 gShamanVillage_MoapaLeavesHouseScript[];
extern u32 gShamanVillage_OtherGuardLeavesHouseScript[];
extern u32 gShamanVillage_RejoinLeaderAtHouseScript[];

struct ItemDefinition *Item_GetDirect(s32 item);

extern u32 gShamanVillage_LetteringSprites[];

extern s16 gShamanVillage_LetteringVramEntry[];

extern s16 gShamanVillage_LetteringWait;
extern s16 *gShamanVillage_LetteringCursor;
extern s32 gShamanVillage_LetteringX;
extern s16 gShamanVillage_LetteringSpeed;
extern s16 gShamanVillage_LetteringSpeedFrames;
extern s16 gShamanVillage_LetteringSpeedFrame;
extern s16 gShamanVillage_LetteringScale;
extern s16 gShamanVillage_LetteringScaleFrom;
extern s16 gShamanVillage_LetteringScaleFrames;
extern s16 gShamanVillage_LetteringScaleFrame;
extern s16 gShamanVillage_LetteringBlend;
extern s16 gShamanVillage_LetteringBlendFrom;
extern s16 gShamanVillage_LetteringBlendFrames;
extern s16 gShamanVillage_LetteringBlendFrame;
extern s16 gShamanVillage_LetteringMode;
extern s16 gShamanVillage_LetteringTileOffset;

/// The lettering scripts' commands (ShamanVillage_RunLettering): each is
/// followed by its values; START's are the x and the speed, SPEED's, SCALE's
/// and BLEND's the new value and the frames to reach it, WAIT's the frames.
/// The lettering's sprites: affine, drawn at twice their size so turning never
/// clips them, and blended; palette 15, priority 1.
#define LETTERING_OBJ_MODE                                                     \
  (OBJ_AFFINE | OBJ_DOUBLE_SIZE | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT))
#define LETTERING_ATTRIBUTES ((15 << 12) | OBJ_PRIORITY(1))

enum LetteringCommand {
  LETTERING_END = -1,
  LETTERING_SCALE = 0x1000,
  LETTERING_BLEND = 0x2000,
  LETTERING_SPEED = 0x3000,
  LETTERING_START = 0x4000,
  LETTERING_WAIT = 0x7fff
};

extern s16 gShamanVillage_LetteringSlideScript[];
extern s16 gShamanVillage_LetteringCountScript[];
extern s16 gShamanVillage_LetteringGoScript[];
extern s16 gShamanVillage_LetteringThrobScript[];
extern u32 gShamanVillage_LetteringThrobFadeScript[];
extern u8 gShamanVillage_LetteringPalettes[];

void ShamanVillage_CastWhirlwindOnSand(void);
extern u32 gShamanVillage_KradenWatchesShebaScript[];
extern u32 gShamanVillage_LeaderWatchesShebaScript[];

extern u32 gShamanVillage_JennaWatchesShebaScript[];

extern u32 gShamanVillage_PiersWatchesShebaScript[];
extern u32 gShamanVillage_HeadForTrialRoadScript[];
extern u32 gShamanVillage_RejoinLeaderAtSandScript[];
void ShamanVillage_SetUpTrialGrounds(void);
void ShamanVillage_SetUpAromaRoom(void);
void ShamanVillage_SetUpVillage(void);

void ShamanVillage_SetUpInn(void);

void ShamanVillage_SetUpSanctum(void);

void Party_RestoreHpPp(s32 mode);

void ShamanVillage_TalkToGuard(void);

void ShamanVillage_TellOutsidersToLeave(void);

void ShamanVillage_WarnOfMoapasPower(void);

void ShamanVillage_SendToMoapa(void);

void ShamanVillage_TalkToPriest(void);

void ShamanVillage_AskAboutOtherSide(void);

void ShamanVillage_TalkToMoapa(void);

void ShamanVillage_TalkToShopkeeper(void);

void ShamanVillage_TellFortune(void);

void ShamanVillage_OfferToGiveUp(void);

void ShamanVillage_ChallengeMoapa(void);
void ShamanVillage_TakeLeftPath(void);

void ShamanVillage_TakeRightPath(void);

void ShamanVillage_DropPillar(void);

void ShamanVillage_MeetMoapa(void);

void ShamanVillage_TalkToInnkeeper(void);

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Import_ActorSetMode((struct Actor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Import_ActorStartScript(
      (struct Actor *)effect,
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
    FieldActor_SetSpriteColor(effect, options->palette);
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
    Import_ActorSetMode((struct Actor *)effect, 1);
    Import_ActorStartScript((struct Actor *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#ifndef TLA_SHAMAN_VILLAGE_H
#define TLA_SHAMAN_VILLAGE_H

#include "types.h"
#include "system/system.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"

void Event_End(void);
void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);
void Import_ActorSetMode(struct Actor *object, s32 mode);
void Actor_Release(struct Actor *object);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
void Actor_SetBlendMode(struct Actor *object, u32 value);
void FieldPsynergy_RunHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldCamera_WaitForArrival(void);

void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Event_StartScreenTransition(void);

void Actor_MoveToObject(s32 actor, s32 target);

void BattleEffect_StartArcEffect(void);
void BattleEffect_EndArcEffect(void);
void BattleEffect_WaitForArcGroups(void);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 value);
void Scene_PlayMusic(void);
void InventorySnapshot_Save(void);

// ---- this overlay

void ShamanVillage_BlowAwaySand(void);
void ShamanVillage_HopFortuneTeller(void);
void ShamanVillage_StartLettering(s32 a, s32 b);
void ShamanVillage_LoadLettering(s32 a);
void ShamanVillage_EnterDoor(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f);
s32 ShamanVillage_IsFacingCounter(void);
void ShamanVillage_ExplainTrialRules(void);
void ShamanVillage_StepBackFromTrial(void);
void ShamanVillage_CheckTrialEntry(void);
void ShamanVillage_CountDownToStart(void);

extern s32 gShamanVillage_EnterDoorScript[];
extern s32 gShamanVillage_HopFortuneTellerScript[];
extern struct SceneEntrance gShamanVillage_Entrances[];
extern struct SceneEntrance gShamanVillageShopEntrances[];
extern struct SceneEntrance gShamanVillageSanctumEntrances[];

#endif

void ShamanVillage_MeetAroma(void) {
  Djinn_JoinParty(ACTOR_AROMA, DJINNI_AROMA / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_AROMA));
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SHAMAN_VILLAGE_SHOP)
    return gShamanVillageShopEntrances;
  if (scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
    return gShamanVillageSanctumEntrances;
  return gShamanVillage_Entrances;
}

#include "field/field.h"

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gShamanVillage_Placements[];
extern struct ActorPlacement gShamanVillageTrialGroundsActorPlacements[];
extern struct ActorPlacement gShamanVillageAromaRoomActorPlacements[];

extern struct ActorPlacement gShamanVillageVillageActorPlacements[];
extern struct ActorPlacement gShamanVillageInnActorPlacements[];
extern struct ActorPlacement gShamanVillageShopActorPlacements[];
extern struct ActorPlacement gShamanVillageSanctumActorPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
    return gShamanVillageTrialGroundsActorPlacements;
  if (scene == SCENE_SHAMAN_VILLAGE_AROMA)
    return gShamanVillageAromaRoomActorPlacements;
  if (scene == SCENE_SHAMAN_VILLAGE) {
    if (Event_GetFlag(FLAG_MOAPA_BEATEN))
      gShamanVillageVillageActorPlacements[0].talkFacing =
          TALK_FACING_PARTY_AND_BACK;
    return gShamanVillageVillageActorPlacements;
  }
  if (scene == SCENE_SHAMAN_VILLAGE_INN)
    return gShamanVillageInnActorPlacements;
  if (scene == SCENE_SHAMAN_VILLAGE_SHOP)
    return gShamanVillageShopActorPlacements;
  if (scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
    return gShamanVillageSanctumActorPlacements;
  return gShamanVillage_Placements;
}

/// Fills BG0's line scroll table with a jittered sine wave and streams it
/// to BG0HOFS by HBlank DMA 0.
void ShamanVillage_WaveWhirlwindLines(void) {
  s16 *line = gShamanVillage_WaveLines;
  s32 i;
  s16 *source;
  volatile u16 *scroll;
  volatile u16 *dma;

  for (i = 0; i != 160; i++, line++) {
    s32 range = (160 - i) >> 2;

    *line = (Iwram_MulQ16(Trig_Sin((i + *(u8 *)&gFrameCount) << 8),
                          (144 - i) << 10) >>
             11) -
            4;
    *line += Random16() % (range * 2) - range;
    if (*line > 56)
      *line -= 56;
    else if (*line < -64)
      *line += 64;
  }
  line = gShamanVillage_WaveLines;
  scroll = (volatile u16 *)REG_BG0HOFS;
  *scroll = *line;
  dma = REG_DMA0;
  dma[5] &= 0xc5ff; // stop the start timing and the repeat
  dma[5] &= 0x7fff; // stop the channel
  source = line + 1;
  (void)dma[5];
  Dma_Set(source, scroll, 0xa2600001, (volatile u32 *)dma);
}

void ShamanVillage_ScrollSandLayer(void) {
  s32 *view = (s32 *)((u8 *)Ram_HeapSlots->mapGrid + 376);

  view[2] -= 0x20000;

  view[3] += 0x20000;
  if (view[3] > 0x4100000) {
    view[2] = 0;
    view[3] = 0x4000000;
  }
}

void ShamanVillage_FlickerRock(struct Actor *object) {
  s32 bit = (((u32)gFrameCount & 3) >> *((u8 *)object + 99)) & 1;

  FieldActor_SetSpriteColor(object, (u8)(bit * *((u8 *)object + 98)));
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

/// Circles the object around the centre in words 68 and 76, twice as wide
/// as it is deep, the radius growing with its height, one step of 256 per
/// lap, and blinks the part attribute every other frame.
void Effect_CircleAndBlink(struct Actor *object) {
  s32 radius = object->y.fixed;
  s32 step = WORD_AT(object, 48);

  radius >>= 2;
  radius += 0xc0000;
  object->x.fixed = WORD_AT(object, 68) +
                    Iwram_MulQ16(Trig_Cos((step & 255) << 11), radius) * 2;
  object->z.fixed =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Trig_Sin((WORD_AT(object, 48) & 255) << 11), radius);
  object->y.fixed += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  FieldActor_SetSpriteColor(object, ((gFrameCount & 2) >> 1) * 9);
}
#undef WORD_AT

/// Swings an object about x = 168: left of it the object speeds up rightward
/// and grows, right of it leftward and shrinks. It takes sprite priority 1
/// while it moves right, 2 otherwise, and spins.
void ShamanVillage_SwirlSandGrain(struct Actor *object) {
  object->x.fixed += object->bounce;
  object->y.fixed += object->gravity;
  object->z.fixed += *(s32 *)((u8 *)object + 76);
  if (FIELD_PIXEL(object->x.fixed) < 168) {
    object->bounce += Q16_ONE * 3 / 2;
    object->scaleY = object->scaleX += 0xccc;
  } else {
    object->bounce -= Q16_ONE * 3 / 2;
    object->scaleY = object->scaleX -= 0x51e;
  }
  if (object->bounce > 0)
    Scene_SetActorSpritePriority(object, 1);
  else
    Scene_SetActorSpritePriority(object, 2);
  ((struct FieldSprite *)object->sprite)->rotation += 0x1800;
}

extern u32 gFieldEffect_MediumScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void ShamanVillage_ScrollSandLayer(void);

#define OBJ14_BYTE(offset) (((u8 *)Actor_GetById(14))[offset])

void ShamanVillage_BlowAwaySand(void) {
  u16 *buffer;
  u16 *cell;
  struct Actor *spark;
  u32 i;
  u32 j;

  buffer = Heap_Alloc(0x680);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                      buffer);
  Dma_Set(buffer, BG_SCREEN_BLOCK(29), DMA_COPY_WORDS(208), REG_DMA3);
  Dma_Set(buffer + 0x1a0, (void *)0x0600ec00, DMA_COPY_WORDS(208), REG_DMA3);
  Heap_Release(buffer);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_SetSpritePriority(ACTOR_SAND_MOUND, 1);
  Io_Write16(BGCNT_SCREEN_BASE(4) | BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(1),
             &REG_BG0CNT);
  REG_BG0CNT &= ~3;
  REG_BG0CNT |= 1;
  Dma_Fill16(16, (void *)0x06002000, 0x500);
  buffer = Heap_Alloc(576);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap),
                      buffer);
  i = 0;
  cell = buffer;
mask:
  {
    s32 t = (*cell + 0xff40) & 0xfff;
    *cell = t | 0x1000;
  }
  i++;
  cell++;
  if (i != 288)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x0600200f + (i << 6)), DMA_COPY_WORDS(8), REG_DMA3);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  OBJ14_BYTE(98) = 9;
  OBJ14_BYTE(99) = 1;
  Actor_GetById(ACTOR_SAND_MOUND)->update = ShamanVillage_FlickerRock;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(
          0x2a0, Actor_GetById(ACTOR_SAND_MOUND)->x.fixed,
          Actor_GetById(ACTOR_SAND_MOUND)->y.fixed,
          Actor_GetById(ACTOR_SAND_MOUND)->z.fixed);
      ((struct SpriteAttr2High *)spark->sprite)->priority =
          (((s8 *)Actor_GetById(ACTOR_SAND_MOUND)->sprite)[9] & 12) >> 2;
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = px(3) / 2;
      spark->riseBaseY = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Import_ActorSetMode((struct Actor *)spark, 7);
      Import_ActorStartScript((struct Actor *)spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, 2);
      spark->update = Effect_CircleAndBlink;
    }
    Event_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  Task_Add(ShamanVillage_WaveWhirlwindLines, TASK_PASS_FRAME);
  Event_PlaySound(SE_RUSH);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_SetShake(1, 1, 1);
  i = 0;
rise:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16() & 7;
  i++;
  Event_Wait(1);
  if (i != 64)
    goto rise;
  Event_Wait(60);
  MapGrid_SetShake(3, 3, 1);
  REG_BLDALPHA = 0x1000;
  REG_BLDCNT = 0x3f42;
  MapGrid_CopyCellsTo(2, 83, 36, 64, 16, 16);
  MapGrid_CopyCellsTo(64, 64, 2, 83, 16, 16);
  REG_BG1CNT &= ~3;
  Task_Add(ShamanVillage_ScrollSandLayer, TASK_PASS_VBLANK);
  i = 0;
swirl:
  REG_BLDALPHA = ((16 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16() & 7;
  i++;
  Event_Wait(1);
  if (i != 68)
    goto swirl;
  {
    s32 width = 13;
    s32 height = 11;

    MapGrid_CopyCellsTo(45, 90, 4, 19, width, height);
  }
  Io_Write16(BGCNT_SCREEN_BASE(6) | BGCNT_PRIORITY(2), &REG_BG2CNT);
  Dma_Set(BG_SCREEN_BLOCK(5), BG_SCREEN_BLOCK(6), DMA_COPY_WORDS(512),
          REG_DMA3);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0xa44;
  i = 0;
stage:
  MapGrid_CopyCellsTo((i - (i / 4) * 4) * 14 + 45, (i / 4) * 12 + 90, 4, 19, 13,
                      11);
  j = 0;
frame:
  if ((j & 3) == 0) {
    s32 velocity[3];
    struct EffectOptions options;
    s32 *v = velocity;

    options.priority = 1;
    options.update = ShamanVillage_SwirlSandGrain;
    v[0] = 0;
    v[2] = -0x20000;
    v[1] = 0;
    {
      s32 x = ((Random16() & 127) << 16) + (208 << 15);

      FieldEffect_Spawn(x, 0, (472 - (Random16() & 127)) << 16, v[0], v[1],
                        v[2], 0x1020001, &options);
    }
  }
  REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
  OBJ14_BYTE(98) = Random16() & 7;
  wait(1);
  j++;
  if (j != 64)
    goto frame;
  Dma_Set(BG_SCREEN_BLOCK(5), BG_SCREEN_BLOCK(6), DMA_COPY_WORDS(512),
          REG_DMA3);
  REG_BLDALPHA = 16;
  i++;
  if (i != 8)
    goto stage;
  {
    s32 control = 0x60a;

    REG_BG2CNT = control;
  }
  REG_DISPCNT &= ~DISPCNT_BG2_ON;
  {
    s32 width = 9;
    s32 height = 11;

    MapGrid_CopyCellsTo(47, 19, 38, 19, width, height);
  }
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f42;
  i = 0;
clear:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  OBJ14_BYTE(98) = Random16() & 7;
  i++;
  Event_Wait(1);
  if (i != 68)
    goto clear;
  Task_Remove(ShamanVillage_ScrollSandLayer);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  MapGrid_CopyCellsTo(36, 64, 2, 83, 16, 16);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  OBJ14_BYTE(98) = 6;
  i = 0;
fade:
  {
    u32 q = i >> 2;

    REG_BLDALPHA = (q << 8) | (16 - q);
  }
  Event_Wait(1);
  i++;
  if (i != 65)
    goto fade;
  Task_Remove(ShamanVillage_WaveWhirlwindLines);
  {
    s32 zero = 0;

    OBJ14_BYTE(99) = zero;
    wait(30);
    Actor_GetById(ACTOR_SAND_MOUND)->update = (void *)zero;
    FieldActor_SetSpriteColor(Actor_GetById(ACTOR_SAND_MOUND), 6);
    MapGrid_WaitForShakeToSettle();
    Event_Wait(60);
    REG_BLDCNT = zero;
  }
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                         gMainRoutineWork.windowBrightness);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags &=
      ~ACTOR_PRIORITY_IGNORES_RISES;
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 1;
  Actor_GetById(ACTOR_SAND_MOUND)->priorityFlags |= 1;
  REG_DISPCNT |= DISPCNT_BG2_ON;
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  {
    s32 width = 6;
    s32 height = 29;

    MapGrid_CopyCellAttributes(1, 5, 9, 1, width, height);
  }
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED);
}
#undef OBJ14_BYTE

extern struct Trigger gShamanVillage_Triggers[];
extern struct Trigger gShamanVillageTrialGroundsTriggers[];
extern struct Trigger gShamanVillageAromaRoomTriggers[];
extern struct Trigger gShamanVillageVillageTriggers[];
extern struct Trigger gShamanVillageInnTriggers[];
extern struct Trigger gShamanVillageShopTriggers[];
extern struct Trigger gShamanVillageSanctumTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
    return gShamanVillageTrialGroundsTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE_AROMA)
    return gShamanVillageAromaRoomTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE)
    return gShamanVillageVillageTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE_INN)
    return gShamanVillageInnTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE_SHOP)
    return gShamanVillageShopTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
    return gShamanVillageSanctumTriggers;
  return gShamanVillage_Triggers;
}

#include "shared/scene_show_entrance_locked.h"

void ShamanVillage_StopAtClosedEntrance(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "The entrance is closed. You probably shouldn't go in."
  FieldMessage_Show((s32)MsgShamanEntranceClosed, 1);
  Event_End();
}

void ShamanVillage_ReadTrialRoadSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Trial Road None may enter without permission from Moapa."
  FieldMessage_Show((s32)MsgShamanTrialRoadPermissionSign, 1);
  Event_End();
}

/// Moapa's guard, actor 17, offers his rules again.
void ShamanVillage_TalkToGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanGuardRepeatRulesBeforeTrial);
  // "Do you need to hear Moapa's explanation again?" Yes: "Thick-witted
  // outsiders." No: "Then you must leave Shaman Village and forget..."
  Event_OpenMessage(ACTOR_EAST_GUARD, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 246, 292);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 204, 292);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 182, 280);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
    // "Thick-witted outsiders."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Event_SetFlag(FLAG_SCENE_TEMP_00);
    ShamanVillage_ExplainTrialRules();
  } else {
    gFieldWork->message++;
    // "Then you must leave Shaman Village and forget about the Hover Jade."
    Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  }
  Event_End();
}

/// Moapa's other guard, actor 16, offers his rules again.
void ShamanVillage_TalkToOtherGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanOtherGuardRepeatRulesBeforeTrial);
  // "Do you need to hear Moapa's explanation again?" Yes: "Thick-witted
  // outsiders." No: "As long as you remain in our village, my..."
  Event_OpenMessage(ACTOR_WEST_GUARD, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 182, 280);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
    // "Thick-witted outsiders."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Event_SetFlag(FLAG_SCENE_TEMP_00);
    ShamanVillage_ExplainTrialRules();
  } else {
    gFieldWork->message++;
    // "As long as you remain in our village, my people are not free to do..."
    Event_ShowMessage(ACTOR_WEST_GUARD, 0);
  }
  Event_End();
}

void ShamanVillage_TellOutsidersToLeave(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanOutsidersLeaveTown);
  // "What's wrong? Nothing to say? Then what are you standing around for?..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  work->fieldMenu = 1;
  Event_End();
}

void ShamanVillage_WarnOfMoapasPower(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanFearMoapaPower);
  // "If you anger Moapa, you'll be in for a little taste of his awesome..."
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  work->fieldMenu = 1;
  Event_End();
}

void ShamanVillage_SendToMoapa(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanSpeakToMoapa);
  // "If there's something you want to say, say it to Moapa, not to us. We..."
  Event_ShowMessage(ACTOR_WEST_GUARD, 0);
  work->fieldMenu = 1;
  Event_End();
}

/// Whether the leader faces the counter, north: the innkeeper, priest and
/// shopkeeper serve only across it.
s32 ShamanVillage_IsFacingCounter(void) {
  u16 turn;
  s32 facing;

  turn =
      ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))->facing +
      0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = FACING_EAST;
  return facing;
}

u32 Inn_QuoteRoomPrice(s32 shop);
void Inn_PlaySleep(s32 roomPrice);

/// The Shaman Village's shops, as Shop_Run numbers them.
enum { SHOP_SHAMAN_VILLAGE = 29 };

/// The Shaman Village's inn, as Inn_CheckIn numbers the inns.
enum { SHAMAN_VILLAGE_INN = 11 };

/// Across the counter the inn's card asks for coins until Moapa is beaten, and
/// the innkeeper serves after.
void ShamanVillage_TalkToInnkeeper(void) {
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    if (ShamanVillage_IsFacingCounter()) {
      Inn_CheckIn(SHAMAN_VILLAGE_INN, 13);
    } else {
      Event_Start();
      Event_ResolvePendingActions(0);
      Event_SetMessageId((s32)MsgShamanVisitorsHaveTraveledFar);
      Event_AskYesNo(ACTOR_INNKEEPER, 0);
      Event_End();
    }
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (ShamanVillage_IsFacingCounter()) {
      u32 price = Inn_QuoteRoomPrice(11);

      Actor_SetAnimation(ACTOR_INNKEEPER, 5);
      Event_Wait(30);
      Event_SetMessageId((s32)MsgShamanInnPaymentPrompt);
      MessageArgument_Push(price, MESSAGE_ARGUMENT_NUMBER);
      MessageArgument_Push(price, MESSAGE_ARGUMENT_NUMBER);
      // "The card reads "Put ... coins on the counter, and you can stay."..."
      Event_OpenMessage(ACTOR_INN_COUNTER, 0);
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
          0) {
        if ((u32)gMainRoutineWork.coins < price) {
          Actor_SetAnimation(ACTOR_INNKEEPER, 6);
          Event_Wait(20);
          Actor_SetAnimation(ACTOR_INNKEEPER, 5);
          Event_Wait(30);
          Event_SetMessageId((s32)MsgShamanInnPaymentPrompt + 3);
          // "There's something written on the card: "If you don't have any..."
          Event_ShowMessage(ACTOR_INN_COUNTER, 0);
        } else {
          Inn_PlaySleep(price);
        }
      }
      Actor_SetAnimation(ACTOR_INNKEEPER, 6);
    } else {
      Event_SetMessageId((s32)MsgShamanSilentVillager);
      // "... ..."
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    }
    Event_End();
  }
}

/// Across the counter the sanctum; elsewhere the priest talks only once Moapa
/// is beaten.
void ShamanVillage_TalkToPriest(void) {
  if (ShamanVillage_IsFacingCounter()) {
    Sanctum_Run(8);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (Event_GetFlag(FLAG_MOAPA_BEATEN))
      Event_SetMessageId((s32)MsgShamanMoapaDefeatIncredible);
    else
      Event_SetMessageId((s32)MsgShamanTribeForbidsTalking);
    // "Nothing personal, but nobody in town will talk to you. It may seem..."
    Event_ShowMessage(ACTOR_PRIEST, 0);
    Event_End();
  }
}

void ShamanVillage_AskAboutOtherSide(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCheckOtherMountainSide);
  Event_AskYesNo(ACTOR_MOUNTAIN_SIDE_SHAMAN, 0);
  Event_End();
}

/// After the battle Moapa asks the party's names once, and then remembers them.
void ShamanVillage_TalkToMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES)) {
    Event_SetMessageId((s32)MsgShamanMoapaNeverForgetsVictor);
    // "Felix, the hero of Contigo! I shall never forget you, the only one..."
    Event_ShowMessage(ACTOR_MOAPA_AT_HOME, 0);
  } else {
    Event_SetMessageId((s32)MsgShamanMoapaAsksVictorsNames);
    // "You have defeated me, and yet I still don't know your names... Would..."
    Event_OpenMessage(ACTOR_MOAPA_AT_HOME, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        1) {
      // "A warrior who does not share his name is a humble warrior indeed."
      Event_ShowMessage(ACTOR_MOAPA_AT_HOME, 0);
    } else {
      gFieldWork->message++;
      // "Felix, Jenna, Sheba, and Piers. I shall remember those names. We..."
      Event_ShowMessage(ACTOR_MOAPA_AT_HOME, 0);
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES);
    }
  }
  Event_End();
}

/// Across the counter the shop; elsewhere a welcome for the hero.
void ShamanVillage_TalkToShopkeeper(void) {
  if (ShamanVillage_IsFacingCounter()) {
    Shop_Run(SHOP_SHAMAN_VILLAGE, 12);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanWelcomeContigoHero);
    // "Everyone is depressed that Moapa lost his big fight. They should be..."
    Event_ShowMessage(ACTOR_SHOPKEEPER, 0);
    Event_End();
  }
}

void ShamanVillage_HopFortuneTeller(void) {
  Actor_Stop(ACTOR_FORTUNE_TELLER);
  wait(1);
  Actor_Jump(ACTOR_FORTUNE_TELLER, 4, 20);
  Event_PlaySound(SE_SPLASH);
  wait(2);
  Actor_RunScriptAndWait(ACTOR_FORTUNE_TELLERS_FLAME,
                         gShamanVillage_HopFortuneTellerScript);
}

/// The fortune-teller, actor 17, reads the party's fortune once a visit to the
/// area.
void ShamanVillage_TellFortune(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD)) {
    if (Event_GetFlag(0xa21))
      Event_SetMessageId((s32)MsgShamanFortuneAvoidNortheast);
    else
      Event_SetMessageId((s32)MsgShamanFortuneAvoidSouth);
    // "You should absolutely not go south. Do you hear me? I'm serious..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  } else {
    Event_PlaySound(MUS_SILENCE);
    if (Event_GetFlag(0xa21))
      Event_SetMessageId((s32)MsgShamanFortuneOfferAfterTrial);
    else
      Event_SetMessageId((s32)MsgShamanFortuneOfferBeforeTrial);
    // "So, you'd like to hear your fortune, would you?"
    Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        1) {
      // "That's probably good. The things I see are not for everyone to hear."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    } else {
      gFieldWork->message++;
      // "Well, you caught me in a good mood... I'll do it!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      Event_PlaySound(MUS_KIBOMBO);
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5);
      // "Hoolabaloo!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      ShamanVillage_HopFortuneTeller();
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 6);
      // "Ballabahoo!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      ShamanVillage_HopFortuneTeller();
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5);
      // "Hoolabaloola!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      ShamanVillage_HopFortuneTeller();
      Event_Wait(20);
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 1);
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 3);
      // "I can see it clearly... Your life is thick with danger..."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      Actor_SetAttachedEffect(ACTOR_FORTUNE_TELLER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(40);
      // "Ooo... Wow, that was a strong one... Listen, don't go south. Bad..."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      Actor_RunRepeatedMotion(ACTOR_FORTUNE_TELLER, 1);
      // "Oh, it's terrible! Awful beyond words! No more, no more!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD);
    }
    Scene_PlayMusic();
  }
  Event_End();
}

void ShamanVillage_ExplainTrialRules(void) {
  if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
    Event_SetMessageId((s32)MsgShamanTrialRulesAreSimple);
  } else {
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.2);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 182, 280);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
    Event_PlaySound(MUS_TRIAL_ROAD);
    Actor_FaceDirection(ACTOR_MOAPA, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_WEST, 0);
    Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
    Event_SetMessageId((s32)MsgShamanListenCarefully);
    // "Listen carefully to me."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
    FieldCamera_SetSpeed(3.2);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 182, 1);
    FieldCamera_WaitForArrival();
    // "This cave provides you with an opportunity to practice your skills."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Actor_SetAttachedEffect(ACTOR_MOAPA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "Legends say that Yegelos and Hoabna once fought here. These columns..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
    // "They raced one another to the summit..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    FieldCamera_MoveTo(178, CAMERA_KEEP, 220, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(10);
    FieldCamera_MoveTo(270, CAMERA_KEEP, 220, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    // "And there, they fought with all their might on the peak of the..."
    Event_ShowMessage(ACTOR_MOAPA, 0);
    FieldCamera_MoveTo(226, CAMERA_KEEP, 280, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
    // "In honor of their great battle, the leaders of Shaman created this..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_SOUTHWEST);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 3);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1.6);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1.6);
  Actor_WalkTo(ACTOR_WEST_GUARD, 184, 256);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 264, 256);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTH, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 182, 1);
  Actor_WalkTo(ACTOR_WEST_GUARD, 184, 128);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 264, 128);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 1);
  wait(1);
  Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
  Actor_SetPosition(ACTOR_EAST_GUARD, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD);
  Event_SetFlag(0x11b);
  Import_EventSetExitRequest(10);
}

void ShamanVillage_OfferToGiveUp(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(182, CAMERA_KEEP, 304, 1);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgShamanGiveUpTrialPrompt);
  // "Have you decided to give up this foolishness once and for all?" Yes: "You
  // have chosen wisely, and for your wisdom..." No: "Choose the road of your
  // liking. It matters..."
  Event_OpenMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    // "You have chosen wisely, and for your wisdom, you must leave Shaman..."
    Event_ShowMessage(ACTOR_MOAPA, 0);
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 368, 1);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 168, 368);
    Event_ClearFlag(FLAG_SCENE_TEMP_01);
  } else {
    gFieldWork->message++;
    // "Choose the road of your liking. It matters not to us."
    Event_ShowMessage(ACTOR_MOAPA, 0);
    FieldCamera_SetSpeed(1.2);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 280, 1);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 168, 280);
  }
  Event_End();
}

void ShamanVillage_ChallengeMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 182, 280);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
  Event_SetMessageId((s32)MsgShamanChallengeTrialRoadPrompt);
  // "Do you want to challenge me on Trial Road?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      1) {
    // "You have chosen wisely, and for your wisdom, you must leave Shaman..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    gFieldWork->message++;
    // "Do you need to hear Moapa's explanation again?"
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        0) {
      Event_PlaySound(MUS_TRIAL_ROAD);
      // "Thick-witted outsiders."
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
      Event_SetFlag(FLAG_SCENE_TEMP_00);
      ShamanVillage_ExplainTrialRules();
    } else {
      gFieldWork->message++;
      ShamanVillage_CheckTrialEntry();
    }
  }
  Event_End();
}

extern u32 gShamanVillage_WalkToLeftStartScript[];
extern u32 gShamanVillage_WalkToRightStartScript[];

/// The leader takes the left path and Moapa the right, after the countdown.
void ShamanVillage_TakeLeftPath(void) {
  struct Actor *owner;
  struct Actor *shaman;
  struct Actor *target;

  owner = ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
  shaman = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(ACTOR_WEST_GUARD);
  Actor_Stop(ACTOR_EAST_GUARD);
  Actor_Stop(ACTOR_MOAPA);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_GUARD, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_WEST_GUARD, 3);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1.6);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 2);
  target = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  if (target != NULL)
    Actor_SetDestination(ACTOR_WEST_GUARD, ((s16 *)target)[5],
                         ((s16 *)target)[9]);
  Actor_WaitForMoveById(ACTOR_WEST_GUARD);
  Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 3);
  Event_SetMessageId((s32)MsgShamanGuardsTakeOtherPath);
  // "If you have chosen, then we shall take the other path."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceActor(gMainRoutineWork.leaderActorId, ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 2);
  target = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  if (target != NULL)
    Actor_SetDestination(ACTOR_EAST_GUARD, ((s16 *)target)[5],
                         ((s16 *)target)[9]);
  Actor_WaitForMoveById(ACTOR_EAST_GUARD);
  Actor_SetPosition(ACTOR_EAST_GUARD, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  // "On the count of three, the challenge begins!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 200, 1);
  owner->riseEnabled = 0;
  shaman->riseEnabled = 0;
  Actor_SetScript(gMainRoutineWork.leaderActorId,
                  gShamanVillage_WalkToLeftStartScript);
  Actor_SetScript(ACTOR_MOAPA, gShamanVillage_WalkToRightStartScript);
  do {
    wait(1);
  } while (owner->riseEnabled == 0 || shaman->riseEnabled == 0);
  ShamanVillage_CountDownToStart();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 168, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_SetSpeed(ACTOR_MOAPA, 1.6);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 184, 128);
  Actor_WalkToAndWait(ACTOR_MOAPA, 264, 128);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  wait(1);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
  Actor_SetPosition(ACTOR_MOAPA, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  InventorySnapshot_Save();
  Import_EventSetExitRequest(11);
}

/// The leader takes the right path and Moapa the left, after the countdown.
void ShamanVillage_TakeRightPath(void) {
  struct Actor *owner;
  struct Actor *shaman;
  struct Actor *target;

  owner = ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
  shaman = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(ACTOR_WEST_GUARD);
  Actor_Stop(ACTOR_EAST_GUARD);
  Actor_Stop(ACTOR_MOAPA);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1.6);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 2);
  target = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  if (target != NULL)
    Actor_SetDestination(ACTOR_EAST_GUARD, ((s16 *)target)[5],
                         ((s16 *)target)[9]);
  Actor_WaitForMoveById(ACTOR_EAST_GUARD);
  Actor_SetPosition(ACTOR_EAST_GUARD, 0, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 3);
  Event_SetMessageId((s32)MsgShamanGuardsTakeOtherPath);
  // "If you have chosen, then we shall take the other path."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceActor(gMainRoutineWork.leaderActorId, ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  Actor_SetAnimationAndWait(ACTOR_WEST_GUARD, 3);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 2);
  target = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  if (target != NULL)
    Actor_SetDestination(ACTOR_WEST_GUARD, ((s16 *)target)[5],
                         ((s16 *)target)[9]);
  Actor_WaitForMoveById(ACTOR_WEST_GUARD);
  Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  // "On the count of three, the challenge begins!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 200, 1);
  owner->riseEnabled = 0;
  shaman->riseEnabled = 0;
  Actor_SetScript(ACTOR_MOAPA, gShamanVillage_WalkToLeftStartScript);
  Actor_SetScript(gMainRoutineWork.leaderActorId,
                  gShamanVillage_WalkToRightStartScript);
  do {
    wait(1);
  } while (owner->riseEnabled == 0 || shaman->riseEnabled == 0);
  ShamanVillage_CountDownToStart();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 168, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_SetSpeed(ACTOR_MOAPA, 1.6);
  Actor_WalkTo(ACTOR_MOAPA, 184, 128);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 128);
  Actor_SetAnimation(ACTOR_MOAPA, 1);
  wait(1);
  Actor_SetPosition(ACTOR_MOAPA, 0, 0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  InventorySnapshot_Save();
  Import_EventSetExitRequest(12);
}

/// Move on the pillar, actor 14, to cell (32, 11): it drops into place as a
/// bridge.
void ShamanVillage_DropPillar(void) {
  struct Actor *object =
      ((struct Actor *)Actor_GetById(ACTOR_AROMA_ROOM_PILLAR));

  if (FIELD_CELL(object->x.fixed) == 32 && FIELD_CELL(object->z.fixed) == 11) {
    Event_Start();
    Event_ResolvePendingActions(0);
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    do {
      wait(1);
    } while (object->velocityY != 0);
    Event_PlaySound(SE_THUMP);
    wait(10);
    object->motionFlags = 0;
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(1);
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED);
    Event_End();
  }
}

void ShamanVillage_MeetMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  Event_Wait(20);
  Actor_SetPosition(ACTOR_MOAPA, 120, 194);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2.4);
  Actor_Jump(gMainRoutineWork.leaderActorId, 4, 0);
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->steeringFlags &= ~1;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 120, 220);
  Event_Wait(1);
  {
    u8 *flags = &((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
                     ->steeringFlags;

    *flags |= 1;
  }
  Actor_SetSpeed(ACTOR_MOAPA, 0.4);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 208);
  Actor_MoveToObject(ACTOR_WEST_GUARD, ACTOR_MOAPA);
  Actor_MoveToObject(ACTOR_EAST_GUARD, ACTOR_MOAPA);
  wait(1);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 0.8);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 0.8);
  Actor_WalkTo(ACTOR_WEST_GUARD, 104, 212);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 136, 212);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 1);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
  Actor_StartRepeatedMotion(ACTOR_MOAPA, 1);
  Event_SetMessageId((s32)MsgShamanMoapaGiveMeRoom);
  // "This is my house. If you're looking for Moapa, here I am. Back up...."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldCamera_SetSpeed(0.8);
  ((struct Actor *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_MoveTo(120, 50, 224, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 120, 256);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.4);
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->steeringFlags &= ~1;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 120, 264);
  Event_Wait(1);
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->steeringFlags |= 1;
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "Who have we here? Strangers in Shaman Village? What is your purpose..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOAPA, 0);
  Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 1);
  FieldActor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "Shaman Village does not appreciate intruders."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_GUARD, 0);
  Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 1);
  FieldActor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "You're not welcome here, outlander. Leave now, and never come back."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WEST_GUARD, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "If there's something you want to say, hurry up and say it. I'll..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOAPA, 0);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_MET);
  Event_End();
}

#undef gShamanVillageVillageActorPlacements
#include "shared/drift_spawn.c"

/// The Shaman Village's events after Moapa's house: the rod, the trial's
/// rules and paths, the race's lettering, and each room's set-up and tables.

/// The Shaman's Rod (item 65) drawn as an actor over the leader.
struct Actor *ShamanVillage_CreateRodIcon(void) {
  struct Actor *object =
      ((struct Actor *)Import_ActorCreate(489, px(120), px(64), px(264)));
  u8 *buffer = NULL;

  if (object == NULL)
    return NULL;
  {
    struct FieldSprite *sprite = object->sprite;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = 0;
    object->retainSpriteCache = 1;
    sprite->nextFrame = 0;
    sprite->unk1b[0] = 0;
    buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(ITEM_SHAMANS_ROD);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  return object;
}

extern u32 gShamanVillage_JennaStandsByRodScript[];
extern u32 gShamanVillage_GuardLeavesHouseScript[];

void ShamanVillage_ShowShamansRod(void) {
  struct Actor *rod;
  struct Actor *object;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  angle = FACING_NORTH;
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))->facing =
      angle;
  wait(1);
  FieldCamera_SetSpeed(0.8);
  ((struct Actor *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_MoveTo(120, 40, 246, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.2);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 120, 264);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 28);
  rod = ShamanVillage_CreateRodIcon();
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetSpeed(ACTOR_MOAPA, 2.4);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 242);
  Event_PlaySound(MUS_TUNDARIA_TOWER);
  Event_SetMessageId((s32)MsgShamanRecognizeShamanRod);
  // "That's the Shaman's Rod!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Event_Wait(20);
  if (rod != NULL)
    Actor_Release(rod);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, angle);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetScript(ACTOR_KRADEN, gShamanVillage_KradenStandsByRodScript);
  Actor_SetScript(ACTOR_PIERS, gShamanVillage_PiersStandsByRodScript);
  Actor_SetScript(ACTOR_JENNA, gShamanVillage_JennaStandsByRodScript);
  Actor_SetScript(ACTOR_SHEBA, gShamanVillage_ShebaStandsByRodScript);
  FieldActor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1.2);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 136, 232);
  // "We thought it was lost forever... It once belonged to the great Hoabna."
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1.2);
  Actor_WalkToAndWait(ACTOR_WEST_GUARD, 104, 232);
  // "Hoabna's staff was a gift from his great friend in Contigo, Yegelos...."
  Event_ShowMessage(ACTOR_WEST_GUARD, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "He vowed to care for the sacred treasure of Contigo, the Hover..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Have you outlanders come to claim the Hover Jade?"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "I suppose we have, yes."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_MOAPA, 0.8);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 252);
  Event_Wait(40);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_WalkToAndWait(ACTOR_MOAPA, 136, 252);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_WalkToAndWait(ACTOR_MOAPA, 104, 252);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_WalkToAndWait(ACTOR_MOAPA, 152, 252);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_WalkToAndWait(ACTOR_MOAPA, 88, 252);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 40);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 252);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 242);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_MOAPA, 4);
  // "That's a shame. We'll never give the Hover Jade to you!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_Jump(ACTOR_KRADEN, 4, 0);
  // "But...we brought you the Shaman's Rod... What's stopping you from..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Hoabna handed the Hover Jade down from generation to generation for..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 4);
  // "We are to return it to the descendants of Yegelos. And you are..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "That's preposterous! We returned your rod! We've come so far already!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTHWEST, 0);
  FieldActor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "What difference does it make if we're from Contigo or not!?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  FieldActor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Make no mistake about it. We are the intended recipients of the..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTHEAST, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Is there some way we can get our hands on the Hover Jade?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "No. You ask the impossible."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_SOUTHWEST);
  // "What about...the test? Would that be a bad idea, Moapa?"
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 20);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 3);
  // "You know, Trial Road? Only Yegelos could walk that path."
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  {
    s32 y = 0x500000;
    s32 z = 0xf20000;

    DriftScene_SpawnObject(0x740000, y, z, 0xe000);
    Event_Wait(10);
    DriftScene_SpawnObject(0x7c0000, y, z, 0x2000);
    Event_Wait(30);
    FieldActor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                         20);
    Actor_FaceDirectionAndWait(ACTOR_WEST_GUARD, FACING_EAST);
    Event_ShowMessage(ACTOR_WEST_GUARD, 0);
    Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
    FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
    Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
    Event_ShowMessage(ACTOR_MOAPA, 0);
    Actor_SetAnimation(ACTOR_MOAPA, 3);
    Event_ShowMessage(ACTOR_MOAPA, 0);
    FieldCamera_SetSpeed(0.4);
    FieldCamera_MoveToFixed(px(180), 0x200000, z, 1);
  }
  object = ((struct Actor *)Actor_GetById(ACTOR_WEST_GUARD));
  object->riseEnabled = 0;
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_MOAPA);
  Actor_SetSpeed(ACTOR_MOAPA, 1.6);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1.6);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1.6);
  Actor_SetScript(ACTOR_MOAPA, gShamanVillage_MoapaLeavesHouseScript);
  Event_Wait(20);
  Actor_SetScript(ACTOR_EAST_GUARD, gShamanVillage_GuardLeavesHouseScript);
  Actor_SetScript(ACTOR_WEST_GUARD, gShamanVillage_OtherGuardLeavesHouseScript);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  do {
    wait(1);
  } while (object->riseEnabled == 0);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  wait(1);
  Scene_PlayMusic();
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(120, CAMERA_KEEP, 260, 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(ACTOR_MOAPA, 0, 0);
  Actor_SetPosition(ACTOR_EAST_GUARD, 0, 0);
  Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_SetScript(ACTOR_SHEBA, gShamanVillage_RejoinLeaderAtHouseScript);
  Actor_SetScript(ACTOR_JENNA, gShamanVillage_RejoinLeaderAtHouseScript);
  Actor_SetScript(ACTOR_PIERS, gShamanVillage_RejoinLeaderAtHouseScript);
  Actor_RunScriptAndWait(ACTOR_KRADEN,
                         gShamanVillage_RejoinLeaderAtHouseScript);
  Event_Wait(20);
  Event_SetFlag(FLAG_SCENE_TEMP_02);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN);
  Event_End();
}

void ShamanVillage_ShowItemToMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowMeSomething);
  // "If you're here offering charity, I won't have it... Or did you want..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Event_End();
}

void ShamanVillage_ShowItemToGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCannotBuyGift);
  // "Yes, yes. That's very nice. Did you want to sell it? Because I don't..."
  Event_ShowMessage(ACTOR_WEST_GUARD, 0);
  Event_End();
}

void ShamanVillage_ShowItemToOtherGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowItToMoapa);
  // "What makes you think I would be interested in this? Show it to Moapa..."
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  Event_End();
}

void ShamanVillage_GreetFortuneSeeker(void) {
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    Event_PlaySound(MUS_SILENCE);
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanFortuneGreeting);
    Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5);
    // "Hoolabaloo!"
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 20);
    ShamanVillage_HopFortuneTeller();
    Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 1);
    Event_SetFlag(FLAG_SCENE_TEMP_03);
    Event_End();
  }
}

/// Growth on the sprout, actor 21: its cells become climbable, toward the
/// Spirit Gloves' chest (flag 0x205).
void ShamanVillage_GrowVine(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 2, 17);
  Event_SetFlag(FLAG_SCENE_TEMP_05);
}

void ShamanVillage_StepBackFromTrial(void) {
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 168, 304);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 168, 368);
  Event_ClearFlag(FLAG_SCENE_TEMP_01);
}

#define EVENT_WORK gFieldWork

// Forwards to Party_ListMembers; tagged where it is used.
/// Moapa takes the challenge only from a party with the Shaman's Rod and eight
/// pieces of equipment.
void ShamanVillage_CheckTrialEntry(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u8 remaining;
  u16 *items;
  s16 *owner;

  Event_SetMessageId((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindMember(ITEM_SHAMANS_ROD) == -1)
    goto refuse;
  count = FieldParty_ListMembers(owners);
  owner = owners;
  remaining = 8;
  for (i = 0; i < count; i++) {
    // slot holds the owner's id until the item loop reuses it.
    slot = *owner++;
    items = Unit_Get(slot)->inventory;
    for (slot = 0; slot < 15; slot++) {
      s32 item = items[slot];
      if (item == 0)
        break;
      if ((u8)(Item_GetDirect(item)->type - 1) <= 3)
        remaining--;
      if (remaining == 0)
        break;
    }
    if (remaining == 0)
      break;
  }
  EVENT_WORK->message++;
  if (remaining != 0) {
  refuse:
    FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    EVENT_WORK->message++;
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Actor_WatchAndSetScript(ACTOR_WEST_GUARD, gMainRoutineWork.leaderActorId);
    Actor_WatchAndSetScript(ACTOR_EAST_GUARD, gMainRoutineWork.leaderActorId);
    Actor_WatchAndSetScript(ACTOR_MOAPA, gMainRoutineWork.leaderActorId);
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD);
    Event_SetFlag(FLAG_SCENE_TEMP_01);
  }
}

// The portrait mode task: reads its command script, eases the portrait's scale,
// blend and position, and queues its sprites.

struct SpriteView {
  u32 words[3];
};

extern s16 gShamanVillage_LetteringSpeedFrom;

/// The portrait mode task: reads its command script, eases the portrait's
/// scale, blend and position, and queues its sprites.
void ShamanVillage_RunLettering(void) {
  u32 *write = gShamanVillage_LetteringSprites;
  struct SpriteView *sprite = (struct SpriteView *)write;
  s32 tile =
      gVramCacheEntries[gShamanVillage_LetteringVramEntry[0]].offset >> 5;
  u32 flags;
  s32 blend, scale, pos;
  s32 matrix, i, x, y, left;
  u32 work[2];

commands:
  if (gShamanVillage_LetteringWait == 0) {
    switch (*gShamanVillage_LetteringCursor++) {
    case LETTERING_START:
      gShamanVillage_LetteringX = *gShamanVillage_LetteringCursor++ << 8;
      gShamanVillage_LetteringSpeed = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrames = 0;
      break;
    case LETTERING_SPEED:
      gShamanVillage_LetteringSpeedFrom = gShamanVillage_LetteringSpeed;
      gShamanVillage_LetteringSpeed = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrame = 0;
      break;
    case LETTERING_SCALE:
      gShamanVillage_LetteringScaleFrom = gShamanVillage_LetteringScale;
      gShamanVillage_LetteringScale = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringScaleFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringScaleFrame = 0;
      break;
    case LETTERING_BLEND:
      gShamanVillage_LetteringBlendFrom = gShamanVillage_LetteringBlend;
      gShamanVillage_LetteringBlend = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringBlendFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringBlendFrame = 0;
      break;
    case LETTERING_WAIT:
      gShamanVillage_LetteringWait = *gShamanVillage_LetteringCursor++;
      break;
    case LETTERING_END:
      Task_Remove(ShamanVillage_RunLettering);
      VramCache_FreeEntry(gShamanVillage_LetteringVramEntry[0]);
      return;
    }
    goto commands;
  }
  {
    s32 duration;
    gShamanVillage_LetteringWait--;
    duration = gShamanVillage_LetteringScaleFrames;
    if (duration == 0) {
      scale = gShamanVillage_LetteringScale;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringScaleFrom;
      target = gShamanVillage_LetteringScale;
      progress = ++gShamanVillage_LetteringScaleFrame;
      scale = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringScaleFrames = 0;
    }
  }
  {
    s32 duration;
    duration = gShamanVillage_LetteringBlendFrames;
    if (duration == 0) {
      blend = gShamanVillage_LetteringBlend;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringBlendFrom;
      target = gShamanVillage_LetteringBlend;
      progress = ++gShamanVillage_LetteringBlendFrame;
      blend = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringBlendFrames = 0;
    }
  }
  {
    u32 packed;
    u32 *t;
    s32 duration;
    duration = gShamanVillage_LetteringSpeedFrames;
    if (duration == 0) {
      pos = gShamanVillage_LetteringSpeed;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringSpeedFrom;
      target = gShamanVillage_LetteringSpeed;
      progress = ++gShamanVillage_LetteringSpeedFrame;
      pos = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringSpeedFrames = 0;
    }
    t = work;
    packed = scale << 16;
    {
      u32 low, word;
      t[1] &= 0xffff0000;
      low = packed >> 16;
      word = work[0];
      word &= 0xffff0000;
      word |= low;
      word &= 0xffff;
      word |= low << 16;
      work[0] = word;
    }
    matrix = (s16)OamMatrix_AllocFromTransform((struct AffineTransform *)t);
  }
  gShamanVillage_LetteringX += pos;
  pos = gShamanVillage_LetteringX / 256;
  switch (gShamanVillage_LetteringMode) {
  case 1: {
    for (i = 0; i < 4; i++) {
      y = 56;
      flags = OBJ_SIZE_32X16;
      x = pos + scale * (i * 32 - 48) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = OBJ_X(x) | y | flags | (matrix << 25) | LETTERING_OBJ_MODE;
        *write++ = LETTERING_ATTRIBUTES | tile;
        Oam_AddSpriteAtDepth(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
  case 3: {
    for (i = 0; i < 2; i++) {
      y = 48;
      flags = OBJ_SIZE_32X16;
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = OBJ_X(x) | y | flags | (matrix << 25) | LETTERING_OBJ_MODE;
        *write++ =
            LETTERING_ATTRIBUTES | (tile + gShamanVillage_LetteringTileOffset);
        Oam_AddSpriteAtDepth(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
#if LANGUAGE == LANGUAGE_FRENCH
  case 4: {
    for (i = 0; i < 2; i++) {
      y = 64;
      flags = OBJ_SIZE_32X16;
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = OBJ_X(x) | y | flags | (matrix << 25) | LETTERING_OBJ_MODE;
        *write++ =
            LETTERING_ATTRIBUTES | (tile + gShamanVillage_LetteringTileOffset);
        Oam_AddSpriteAtDepth(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
#else
  case 4:
    y = 48;
    flags = OBJ_SIZE_64X32;
    left = pos + 56;
    if ((u32)(pos + 120) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = OBJ_X(x) | y | flags | (matrix << 25) | LETTERING_OBJ_MODE;
      *write++ =
          LETTERING_ATTRIBUTES | (tile + gShamanVillage_LetteringTileOffset);
      Oam_AddSpriteAtDepth(sprite, 236);
    }
    break;
#endif
  case 2:
    y = 48;
    flags = OBJ_SIZE_32X32;
    left = pos + 88;
    if ((u32)(pos + 152) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = OBJ_X(x) | y | flags | (matrix << 25) | LETTERING_OBJ_MODE;
      *write++ =
          LETTERING_ATTRIBUTES | (tile + gShamanVillage_LetteringTileOffset);
      Oam_AddSpriteAtDepth(sprite, 236);
    }
    break;
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

/// Starts the mode task: keeps the mode and its parameter, schedules
/// ShamanVillage_RunLettering and picks the script the mode runs, then rewinds
/// the task's timer and position. The countdown starts only modes 2, the
/// numbers, and 4, the start.
void ShamanVillage_StartLettering(s32 modeArg, s32 paramArg) {
  s32 handler;

  gShamanVillage_LetteringMode = modeArg;
  gShamanVillage_LetteringTileOffset = paramArg << 4;

  Task_Add(ShamanVillage_RunLettering, TASK_PASS_VBLANK);

  handler = (s32)gShamanVillage_LetteringSlideScript;
  if (modeArg == 2) {
    handler = (s32)gShamanVillage_LetteringCountScript;
  } else if (modeArg == 4) {
    handler = (s32)gShamanVillage_LetteringGoScript;
  } else if (modeArg == 3) {
    if (paramArg != 0) {
      handler = (s32)gShamanVillage_LetteringThrobScript;
    } else {
      handler = (s32)gShamanVillage_LetteringThrobFadeScript;
    }
  }

  gShamanVillage_LetteringWait = 0;
  gShamanVillage_LetteringCursor = (s16 *)handler;
  gShamanVillage_LetteringSpeed = 0;
  gShamanVillage_LetteringSpeedFrames = 0;
  gShamanVillage_LetteringX = 0;
}

// The European editions' lettering sheet has one more palette row.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_PALETTE_ROWS 6
#else
#define LETTERING_PALETTE_ROWS 5
#endif

/// The decoded lettering sheet: its palette rows, then eight graphics of 32
/// tiles each.
struct LetteringSheet {
  u8 palettes[LETTERING_PALETTE_ROWS][32];
  u8 graphics[8][1024];
};

/// Decodes the lettering sheet, sends one graphic's palette to OBJ palette 15
/// and loads its tiles into a VRAM block of its own. Graphic 8 has a palette of
/// its own and graphic 4's tiles.
void ShamanVillage_LoadLettering(s32 graphic) {
  // BUG: this holds seven graphics of the sheet's eight, so decoding the sheet
  // writes 1024 bytes past the block.
#ifdef BUGFIX
  struct LetteringSheet *sheet = Heap_AllocEwram(sizeof(struct LetteringSheet));
#else
  struct LetteringSheet *sheet =
      Heap_AllocEwram((224 + LETTERING_PALETTE_ROWS) * 32);
#endif
  struct DmaRegisters volatile *dma;
  s32 palette;

  if (gShamanVillage_LetteringVramEntry[0] == -1)
    gShamanVillage_LetteringVramEntry[0] = VramCache_FindFreeEntry();
  palette = gShamanVillage_LetteringPalettes[graphic];
  if (graphic == 8)
    graphic = 4;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LetteringBanner),
                      sheet);
  Dma_Set(sheet->palettes[0] + palette, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 32 / 4, REG_DMA3);
  Vram_Load(gShamanVillage_LetteringVramEntry[0], sizeof(sheet->graphics[0]),
            sheet->graphics[graphic]);
  dma = (struct DmaRegisters volatile *)REG_DMA3;
  while ((s32)dma->control < 0)
    ;
  Heap_Release(sheet);
}

/// The lettering banner's three counts and the start, before the race up Trial
/// Road.
void ShamanVillage_CountDownToStart(void) {
  gShamanVillage_LetteringSlideScript[15] = 180;
  Event_Wait(30);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  ShamanVillage_LoadLettering(5);
  ShamanVillage_StartLettering(2, 0);
  Event_PlaySound(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  ShamanVillage_StartLettering(2, 1);
  Event_PlaySound(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  ShamanVillage_LoadLettering(6);
  ShamanVillage_StartLettering(2, 0);
  Event_PlaySound(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  ShamanVillage_LoadLettering(7);
  ShamanVillage_StartLettering(4, 0);
  Event_PlaySound(SE_COLOSSO_GO);
  Event_PlaySound(MUS_TRIAL_ROAD);
}

/// Moapa's first test: Sheba steps up and casts Whirlwind on the purple rock,
/// and the sand is gone.
void ShamanVillage_ShebaClearsSand(void) {
  u8 *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *object = ((struct Actor *)Actor_GetById(ACTOR_WEST_GUARD));

  s32 result;
  s32 owner;
  s32 angle;
  u8 *done;

  work[34] = 0;
  work[35] = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(MUS_SILENCE);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 528, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 168, 536);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_SetScript(gMainRoutineWork.leaderActorId,
                  gShamanVillage_LeaderWatchesShebaScript);
  Actor_SetScript(ACTOR_JENNA, gShamanVillage_JennaWatchesShebaScript);
  Actor_SetScript(ACTOR_KRADEN, gShamanVillage_KradenWatchesShebaScript);
  Actor_RunScriptAndWait(ACTOR_PIERS, gShamanVillage_PiersWatchesShebaScript);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
  Event_SetMessageId((s32)MsgShamanCheerForCompanion);
  // "You can do it, Sheba!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "Leave it to me, Jenna. This will be a breeze."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_SHEBA, 0.4);
  Actor_WalkToAndWait(ACTOR_SHEBA, 168, 524);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTH, 0);
  FieldCamera_SetSpeed(1.6);
  ShamanVillage_CastWhirlwindOnSand();
  Event_PlaySound(MUS_TUNDARIA_TOWER);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 540, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_MOAPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "I don't believe it. They did it! The sand vanished!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAttachedEffect(ACTOR_EAST_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "The legends... They were all true!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_GUARD, 0);
  Actor_SetAttachedEffect(ACTOR_WEST_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "She's the one! She must have come from Contigo!"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_WEST_GUARD, 0,
                                 20);
  Actor_RunRepeatedMotion(ACTOR_MOAPA, 1);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  // "Her?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
  // "She's the one that did it? But she's...just a girl!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_Jump(ACTOR_SHEBA, 4, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "Well, you wanted the sand to vanish, so..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  // "Can we have the Hover Jade now?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_MOAPA, 4);
  // "Nope, you still can't have it."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldActor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
  // "What? Why not?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  // "This was just a test to see if you earned the right to take the test..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "So, there's more that they have to do before you'll give them the stone?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
  // "You will have to reach the end of Trial Road."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 1);
  // "The girls, too? Surely you can't make them..."
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 1);
  // "I thought that only the chosen hero could travel the road..."
  Event_ShowMessage(ACTOR_WEST_GUARD, 0);
  FieldActor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
  // "What, are you saying I can't be the chosen hero?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAttachedEffect(ACTOR_MOAPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "The heroes have always been men, ever since the time of Hoabna. It..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_Jump(ACTOR_SHEBA, 2, 10);
  Actor_Jump(ACTOR_SHEBA, 4, 10);
  // "I don't like your attitude, mister! I can be every bit as heroic as..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_NORTHWEST);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 4);
  // "That may be true, but you must respect our customs."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_GUARD, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_GUARD, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 4);
  // "It's the same in Contigo, you know. If you don't like it, take it up..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WEST_GUARD, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 3);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 560, 1);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_GUARD, FACING_SOUTH);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1);
  Actor_WalkTo(ACTOR_WEST_GUARD, 120, 560);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 216, 560);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 1);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_GUARD, FACING_EAST);
  Actor_WalkTo(ACTOR_WEST_GUARD, 152, 560);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 184, 560);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 1);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
  Actor_SetSpeed(ACTOR_MOAPA, 1);
  Actor_SetAnimation(ACTOR_MOAPA, 2);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 2);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 2);
  Actor_SetDestinationOffset(ACTOR_MOAPA, 0, 16);
  Actor_SetDestinationOffset(ACTOR_EAST_GUARD, 0, 16);
  Actor_SetDestinationOffset(ACTOR_WEST_GUARD, 0, 16);
  Actor_WaitForMoveById(ACTOR_MOAPA);
  Actor_SetAnimation(ACTOR_MOAPA, 1);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 1);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 1);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  // "Hold it, hold it... Moapa, what were you just saying about Trial Road?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTHWEST, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "If we reach the end of Trial Road, won't our names join the ranks of..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "Kraden's right... It shouldn't matter if a girl does it... All that..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  // "If you give us the chance, we will prove it to you."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "Unless you're afraid that a girl like me might become your town's..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 4);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_SOUTHWEST);
  // "Have they earned the right? Shall we let them onto Trial Road?"
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  Actor_FaceDirectionAndWait(ACTOR_WEST_GUARD, FACING_SOUTHEAST);
  // "I don't like the way these outsiders were talking to me."
  Event_ShowMessage(ACTOR_WEST_GUARD, 0);
  Actor_SetAttachedEffect(ACTOR_MOAPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(ACTOR_EAST_GUARD, FACING_WEST);
  Actor_Jump(ACTOR_EAST_GUARD, 4, 10);
  // "I think they need to learn exactly what it takes to be a hero around..."
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_GUARD, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_WEST_GUARD, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_WEST_GUARD, FACING_SOUTHEAST);
  // "Yeah, if these guys are so stubborn, let's give them a shot! They'll..."
  Event_ShowMessage(ACTOR_WEST_GUARD, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  // "You said it yourselves. We're too stubborn to quit."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "Yeah. If you want us "outlanders" to leave, you'd better give us a..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAttachedEffect(ACTOR_MOAPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_WEST_GUARD, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "Trial Road is a difficult one... When you reach the top, a terrible..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 4);
  // "It's difficult enough for me. I expect you will find it quite..."
  Event_OpenMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  result = Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
  done = &object->riseEnabled;
  owner = gMainRoutineWork.leaderActorId;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 4);
    Event_ShowMessage(ACTOR_MOAPA, 0);
  } else {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 4);
    EVENT_WORK->message++;
    Event_ShowMessage(ACTOR_MOAPA, 0);
    Actor_SetAnimation(ACTOR_MOAPA, 3);
    Event_ShowMessage(ACTOR_MOAPA, 0);
  }
  Actor_WalkToAndWait(ACTOR_MOAPA, 212, 582);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 500, 1);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_MOAPA);
  *done = 0;
  Actor_SetScript(ACTOR_MOAPA, gShamanVillage_HeadForTrialRoadScript);
  Event_Wait(20);
  Actor_SetScript(ACTOR_EAST_GUARD, gShamanVillage_HeadForTrialRoadScript);
  Actor_SetScript(ACTOR_WEST_GUARD, gShamanVillage_HeadForTrialRoadScript);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  do {
    wait(1);
  } while (*done == 0);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_SHEBA);
  Scene_PlayMusic();
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 540, 1);
  Event_Wait(20);
  angle = FACING_SOUTH;
  Actor_SetPositionAndFacing(ACTOR_MOAPA, px(224), px(280), angle);
  Actor_SetPositionAndFacing(ACTOR_WEST_GUARD, px(208), px(264), angle);
  Actor_SetPositionAndFacing(ACTOR_EAST_GUARD, px(240), px(264), angle);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_SHEBA);
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))->update =
      NULL;
  ((struct Actor *)Actor_GetById(ACTOR_KRADEN))->update = NULL;
  ((struct Actor *)Actor_GetById(ACTOR_JENNA))->update = NULL;
  ((struct Actor *)Actor_GetById(ACTOR_PIERS))->update = NULL;
  ((struct Actor *)Actor_GetById(ACTOR_SHEBA))->update = NULL;
  wait(1);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetScript(ACTOR_PIERS, gShamanVillage_RejoinLeaderAtSandScript);
  Actor_SetScript(ACTOR_SHEBA, gShamanVillage_RejoinLeaderAtSandScript);
  Actor_SetScript(ACTOR_JENNA, gShamanVillage_RejoinLeaderAtSandScript);
  Actor_RunScriptAndWait(ACTOR_KRADEN, gShamanVillage_RejoinLeaderAtSandScript);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED);
  Event_End();
}

void ShamanVillage_CastWhirlwindOnSand(void) {
  Actor_SetSpritePriority(ACTOR_SHEBA, 1);
  Event_PlaySound(SE_PSYNERGY_SPARKLE);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(((struct Actor *)Actor_GetById(ACTOR_SHEBA)),
                                   2);
  Event_Wait(40);
  BattleEffect_SetObjectUpdateMode(((struct Actor *)Actor_GetById(ACTOR_SHEBA)),
                                   0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  ShamanVillage_BlowAwaySand();
  Actor_SetSpritePriority(ACTOR_SHEBA, 2);
}

void ShamanVillage_EnterDoor(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  s32 cell;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (mode == 2)
    Event_PlaySound(SE_THUMP);
  else
    Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(a, b, c, d, mode, f);
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  if (mode == 1) {
    object = ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
    cell = FIELD_PIXEL(object->x.fixed);
    Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x.fixed,
                        object->y.fixed, object->z.fixed);
    FieldActor_WaitForMove(object);
    object->facing = FACING_NORTH;
  }
  Actor_SetScript(gMainRoutineWork.leaderActorId,
                  gShamanVillage_EnterDoorScript);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

void ShamanVillage_EnterDoorA(void) {
  ShamanVillage_EnterDoor(43, 40, 8, 22, 1, 2);
}

void ShamanVillage_EnterDoorB(void) {
  ShamanVillage_EnterDoor(43, 42, 4, 26, 1, 2);
}

void ShamanVillage_EnterDoorC(void) {
  ShamanVillage_EnterDoor(43, 42, 27, 21, 1, 2);
}

void ShamanVillage_EnterDoorD(void) {
  ShamanVillage_EnterDoor(43, 40, 28, 17, 1, 2);
}

void ShamanVillage_EnterDoorE(void) {
  ShamanVillage_EnterDoor(43, 42, 10, 8, 1, 2);
}

void ShamanVillage_EnterMoapasHouse(void) {
  if (Event_GetFlag(FLAG_SCENE_TEMP_02)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_PlaySound(SE_MAP_EXIT);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(13);
  } else {
    ShamanVillage_EnterDoor(43, 40, 7, 6, 1, 2);
  }
}

/// Back at Trial Road's foot, Moapa offers the trial again.
void ShamanVillage_RestartTrial(void) {
  s32 angle;

  Event_ClearFlag(0x11b);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
  Actor_SetPosition(ACTOR_EAST_GUARD, 0, 0);
  ((struct Actor *)Actor_GetById(ACTOR_MOAPA))->facing = FACING_NORTH;
  FieldCamera_MoveTo(224, CAMERA_KEEP, 182, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  angle = FACING_SOUTH;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPositionAndFacing(ACTOR_WEST_GUARD, px(184), px(128), angle);
  Actor_SetPositionAndFacing(ACTOR_EAST_GUARD, px(264), px(128), angle);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1.6);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1.6);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
  Actor_WalkTo(ACTOR_WEST_GUARD, 184, 256);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 264, 256);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_WalkTo(ACTOR_WEST_GUARD, 208, 264);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 240, 264);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WEST_GUARD, angle, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, angle, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgShamanMustFinishTrialRoad);
  // "In order to reach the battle at the summit, you cannot fail on Trial..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 3);
  // "Do you understand?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      1) {
    // "Would you like me to explain it to you again?"
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        0) {
      Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
      Event_PlaySound(MUS_TRIAL_ROAD);
      // "Do all outlanders share your thickwittedness?"
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
      Event_SetFlag(FLAG_SCENE_TEMP_00);
      ShamanVillage_ExplainTrialRules();
      return;
    }
    EVENT_WORK->message++;
  } else {
    EVENT_WORK->message += 2;
  }
  // "Then you understand the treacherous path of Trial Road. Will you..."
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      1) {
    // "You have chosen wisely, and for your wisdom, you must leave Shaman..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    EVENT_WORK->message++;
    // "You have heard the rules, and you wish to continue. You have been..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_CheckTrialEntry();
  }
  Scene_PlayMusic();
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_End();
}

void ShamanVillage_AdviseQuitting(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanMoapaAdvisesLeaving);
  ((struct Actor *)Actor_GetById(ACTOR_MOAPA))->facing = FACING_WEST;
  ((struct Actor *)Actor_GetById(ACTOR_WEST_GUARD))->facing = FACING_WEST;
  ((struct Actor *)Actor_GetById(ACTOR_EAST_GUARD))->facing = FACING_WEST;
  FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  // "It would be wise of you to quit. Even if you reach the top, you..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  ShamanVillage_StepBackFromTrial();
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_End();
}

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
    ShamanVillage_SetUpTrialGrounds();
  else if (scene == SCENE_SHAMAN_VILLAGE_AROMA)
    ShamanVillage_SetUpAromaRoom();
  else if (scene == SCENE_SHAMAN_VILLAGE)
    ShamanVillage_SetUpVillage();
  else if (scene == SCENE_SHAMAN_VILLAGE_INN)
    ShamanVillage_SetUpInn();
  else if (scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
    ShamanVillage_SetUpSanctum();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void ShamanVillage_SetUpTrialGrounds(void) {
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED)) {
    MapGrid_CopyCellsTo(47, 19, 38, 19, 9, 11);
    MapGrid_CopyCellAttributes(1, 5, 9, 1, 6, 29);
    FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(ACTOR_SAND_MOUND)),
                              6);
  } else if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 angle = FACING_SOUTH;

    Actor_SetPositionAndFacing(ACTOR_MOAPA, px(168), px(568), angle);
    Actor_SetPositionAndFacing(ACTOR_EAST_GUARD, px(216), px(536), angle);
    Actor_SetPositionAndFacing(ACTOR_WEST_GUARD, px(120), px(536), angle);
    wait(1);
  }
  Actor_SetAnimation(ACTOR_FIRST_TORCH, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 1, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 2, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 3, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 4, 2);
  Actor_SetAnimation(ACTOR_LAST_TORCH, 2);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 11)
      ShamanVillage_RestartTrial();
    else if (entrance == 9)
      ShamanVillage_AdviseQuitting();
  }
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    Actor_SetPosition(ACTOR_MOAPA, 0, 0);
    Actor_SetPosition(ACTOR_EAST_GUARD, 0, 0);
    Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
    MapGrid_CopyCellAttributes(10, 12, 3, 1, 10, 13);
    MapGrid_CopyCellAttributes(15, 12, 3, 1, 15, 13);
    MapGrid_CopyCellAttributes(9, 20, 3, 1, 9, 21);
  }
}

void ShamanVillage_SetUpAromaRoom(void) {
  struct Actor *object;

  Actor_SetAnimation(ACTOR_FIRST_TORCH, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 1, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 2, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 3, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 4, 2);
  Actor_SetAnimation(ACTOR_LAST_TORCH, 2);
  ((struct Actor *)Actor_GetById(ACTOR_AROMA_ROOM_PILLAR))->motionFlags = 0;
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED)) {
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_AROMA_ROOM_PILLAR)), px(520),
        px(32), px(184));
    wait(1);
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(10);
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN)) {
    MapGrid_CopyCellsTo(64, 0, 76, 2, 3, 3);
    MapGrid_CopyCellAttributes(12, 5, 3, 1, 12, 6);
  }
  if (Event_GetFlag(0x94d))
    MapGrid_CopyCellsTo(48, 40, 101, 11, 1, 2);
  else
    Actor_SetPosition(ACTOR_FORTUNE_TELLER, 0, 0);
  object = ((struct Actor *)Actor_GetById(ACTOR_FORTUNE_TELLERS_FLAME));
  object->collisionFlags = ACTOR_NOT_TARGETED;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  Actor_SetSpriteFlags(
      ((struct Actor *)Actor_GetById(ACTOR_FORTUNE_TELLERS_FLAME)), 0);
}

void ShamanVillage_DriftSmoke(struct Actor *object) {
  s32 step;

  object->x.fixed += object->timer << 8;

  object->y.fixed += 0x8000;

  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->timer += 2;
  if (--*(s32 *)&object->watchedActor == 0)
    Actor_Release(object);
}

void ShamanVillage_SpawnSmoke(s32 x, s32 y, s32 z) {
  struct Actor *object = ((struct Actor *)Import_ActorCreate(285, x, y, z));

  if (object != NULL) {
    object->retainSpriteCache = 2;
    object->motionFlags = 0;
    object->timer = 0;
    object->priorityFlags &= ~1;
    FieldActor_SetSpriteColor(object, 7);
    ((struct FieldSprite *)object->sprite)->priority = 1;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteFlags(object, 0);
    object->scaleX = Q16_ONE / 2;
    object->scaleY = Q16_ONE / 2;
    *(s32 *)&object->watchedActor = 60;
    object->update = ShamanVillage_DriftSmoke;
    Import_ActorSetMode(object, 5);
    Actor_SetBlendMode(object, 1);
  }
}

void ShamanVillage_PuffChimneySmoke(void) {
  if ((gFrameCount & 63) == 0) {
    ShamanVillage_SpawnSmoke(0x480000, 0x420000, 0x1780000);
    ShamanVillage_SpawnSmoke(0x680000, 0x620000, 0x980000);
  }
}

#define DEFINE_LEADER_LAYER_TASK(name, n1, n2)                                 \
  void name(void) {                                                            \
    struct FieldSprite *sprite =                                               \
        Actor_GetById(gMainRoutineWork.leaderActorId)->sprite;                 \
    s32 variant = sprite->priority;                                            \
                                                                               \
    Actor_SetSpritePriority(n1, variant);                                      \
    Actor_SetSpritePriority(n2, variant);                                      \
  }

DEFINE_LEADER_LAYER_TASK(ShamanVillage_LayerVillageActors, 21, 14)

struct IoReg16 {
  u16 value;
};

void ShamanVillage_SetUpVillage(void) {
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
      Actor_SetPosition(ACTOR_WEST_GUARD, 104, 200);
      Actor_SetPosition(ACTOR_EAST_GUARD, 136, 200);
    } else if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_MET) &&
               !Event_GetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN)) {
      Actor_SetPosition(ACTOR_MOAPA, 120, 208);
      Actor_SetPosition(ACTOR_WEST_GUARD, 104, 212);
      Actor_SetPosition(ACTOR_EAST_GUARD, 136, 212);
      wait(1);
    }
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_02))
    MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  if (Event_GetFlag(0x94d)) {
    s32 width = 1;
    s32 height = 2;

    MapGrid_CopyCellsTo(48, 40, 77, 21, width, height);
    MapGrid_CopyCellsTo(48, 40, 81, 25, width, height);
  }
  Rope_Initialize();
  Rope_Place(0, 0x204, 19, 20);
  if (Event_GetFlag(FLAG_SCENE_TEMP_05))
    ShamanVillage_GrowVine();
  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  FieldTask_AddFrame(ShamanVillage_LayerVillageActors, TASK_PASS_VBLANK);
  FieldTask_AddFrame(ShamanVillage_PuffChimneySmoke, TASK_PASS_VBLANK);
}

void ShamanVillage_SetUpInn(void) {
  u32 i;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  ((struct Actor *)Actor_GetById(ACTOR_INN_COUNTER))->collisionFlags =
      ACTOR_NOT_TARGETED;
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    s32 angle = FACING_SOUTH_SOUTHEAST;

    Actor_SetPositionAndFacing(ACTOR_MOAPAS_MAN_A, px(88), px(92), angle);
    Actor_SetPosition(ACTOR_MOAPA_AT_HOME, 156, 72);
  } else {
    Actor_SetPosition(ACTOR_PICNIC_SHAMAN_A, 0, 0);
    Actor_SetPosition(ACTOR_PICNIC_SHAMAN_B, 0, 0);
    Actor_SetPosition(ACTOR_INN_COOK, 0, 0);
  }
  if ((gMainRoutineWork.entrance == 30 || gMainRoutineWork.entrance == 31) &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Party_RestoreHpPp(1);
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, 41);
    Actor_SetSpriteFlags(
        ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId)), 0);
    wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    i = 0;
    goto test_stand;
  next_stand:
    i++;
  test_stand:
    if (i < 60) {
      wait(1);
      if (gInput.held == 0)
        goto next_stand;
    }
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, 42);
    Event_Wait(10);
    i = 0;
    goto test_turn;
  next_turn:
    i++;
  test_turn:
    if (i < 30) {
      wait(1);
      if (gInput.held == 0)
        goto next_turn;
    }
    Actor_Jump(gMainRoutineWork.leaderActorId, 4, 0);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 813, 120);
    Actor_SetSpriteFlags(
        ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId)), 1);
    Event_End();
    if (gMainRoutineWork.entrance == 30) {
      Event_SetFlag(FLAG_MOAPA_BEATEN);
      Event_SetFlag(0x94d);
    }
  }
}

DEFINE_LEADER_LAYER_TASK(ShamanVillage_LayerSanctumActors, 9, 10)

void ShamanVillage_SetUpSanctum(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    Task_Add(ShamanVillage_LayerSanctumActors, TASK_PASS_VBLANK);
  } else {
    Actor_SetPosition(ACTOR_SANCTUM_SHAMAN_A, 0, 0);
    Actor_SetPosition(ACTOR_SANCTUM_SHAMAN_B, 0, 0);
  }
}

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s16 gShamanVillage_LetteringCountScript[] = {
    LETTERING_SCALE, 512, 1,
    LETTERING_BLEND, 0, 1,
    LETTERING_SCALE, 256, 6,
    LETTERING_BLEND, 16, 6,
    LETTERING_WAIT, 30,
    LETTERING_BLEND, 0, 30,
    LETTERING_WAIT, 30,
    LETTERING_END,
};

/// Unused: the scripts of modes 3 and 1, which nothing in this overlay starts.
s16 gShamanVillage_LetteringThrobScript[] = {
    LETTERING_SCALE, 128, 1,
    LETTERING_BLEND, 0, 1,
    LETTERING_SCALE, 256, 6,
    LETTERING_BLEND, 16, 6,
    LETTERING_WAIT, 60,
    LETTERING_SCALE, 384, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 240, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 368, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 224, 6,
    LETTERING_WAIT, 6,
    LETTERING_BLEND, 0, 30,
    LETTERING_SCALE, 352, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 208, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 336, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 192, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 320, 6,
    LETTERING_WAIT, 6,
    LETTERING_END,
};

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SHAMAN_GRAPHIC7_PALETTE (5 * 32)
#else
#define SHAMAN_GRAPHIC7_PALETTE (2 * 32)
#endif

u8 gShamanVillage_LetteringPalettes[] = {
    0, 32, 32, 32, 96, 64, 64, SHAMAN_GRAPHIC7_PALETTE, 128, 0,
};
// clang-format on

// clang-format off
s32 gShamanVillage_EnterDoorScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, -0x8000),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

u32 gShamanVillage_WalkToLeftStartScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(184, 0, 216),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_SET(SW_RISE_ENABLED, 1),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_WalkToRightStartScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(264, 0, 216),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_SET(SW_RISE_ENABLED, 1),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_KradenStandsByRodScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_PiersStandsByRodScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_JennaStandsByRodScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(104, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_ShebaStandsByRodScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(88, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_MoapaLeavesHouseScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 120),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gShamanVillage_GuardLeavesHouseScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 120),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gShamanVillage_OtherGuardLeavesHouseScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(104, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 120),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_RISE_ENABLED, 1),
    SC_END,
};

u32 gShamanVillage_RejoinLeaderAtHouseScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_END,
};

u32 gShamanVillage_KradenWatchesShebaScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 552),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_LeaderWatchesShebaScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(184, 0, 536),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_JennaWatchesShebaScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 536),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_PiersWatchesShebaScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(200, 0, 552),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_HeadForTrialRoadScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(216, 0, 564),
    SC_SYNC,
    SC_MOVE(216, 0, 470),
    SC_SYNC,
    SC_MOVE(168, 0, 434),
    SC_SYNC,
    SC_MOVE(168, 0, 380),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_RISE_ENABLED, 1),
    SC_END,
};

u32 gShamanVillage_RejoinLeaderAtSandScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(184, 0, 536),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gShamanVillage_HopFortuneTellerScript[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, 0x4000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 8),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_END,
};

s16 gShamanVillage_LetteringVramEntry[] = {
    -1,
};

// Scale for each edition.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define SHAMAN_GRAPHIC1_SCALE 384
#define SHAMAN_GRAPHIC4_START_SCALE 512
#define SHAMAN_GRAPHIC4_SMALL_SCALE 384
#define SHAMAN_GRAPHIC4_LARGE_SCALE 512
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SHAMAN_GRAPHIC1_SCALE 256
#define SHAMAN_GRAPHIC4_START_SCALE 384
#define SHAMAN_GRAPHIC4_SMALL_SCALE 256
#define SHAMAN_GRAPHIC4_LARGE_SCALE 384
#else
#define SHAMAN_GRAPHIC1_SCALE 256
#define SHAMAN_GRAPHIC4_START_SCALE 512
#define SHAMAN_GRAPHIC4_SMALL_SCALE 256
#define SHAMAN_GRAPHIC4_LARGE_SCALE 384
#endif

s16 gShamanVillage_LetteringSlideScript[] = {
    LETTERING_START, -188, 0x800,
    LETTERING_SCALE, SHAMAN_GRAPHIC1_SCALE, 1,
    LETTERING_BLEND, 16, 1,
    LETTERING_WAIT, 14,
    LETTERING_SPEED, 0, 20,
    LETTERING_WAIT, 60,
    LETTERING_SPEED, 0x800, 20,
    LETTERING_WAIT, 60,
    LETTERING_END,
};

s16 gShamanVillage_LetteringGoScript[] = {
    LETTERING_SCALE, SHAMAN_GRAPHIC4_START_SCALE, 1,
    LETTERING_BLEND, 0, 1,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_SMALL_SCALE, 6,
    LETTERING_BLEND, 16, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_SMALL_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_SMALL_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_BLEND, 0, 30,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_SMALL_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_SMALL_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, SHAMAN_GRAPHIC4_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_END,
};
// clang-format on

#include "text/msg_ids.h"

void ShamanVillage_MeetAroma(void);
void Scene_ShowEntranceLocked(void);
void ShamanVillage_StopAtClosedEntrance(void);
void ShamanVillage_ReadTrialRoadSign(void);
void ShamanVillage_TalkToOtherGuard(void);

// clang-format off
u32 gShamanVillage_LetteringThrobFadeScript[] = {
    0x801000, 0x20000001, 0x10000, 0x1001000, 0x20000006, 0x60010,
    0x3c7fff, 0x1801000, 0x7fff0006, 0x10000006, 0x60110, 0x67fff,
    0x1901000, 0x7fff0006, 0x10000006, 0x60120, 0x67fff, 0x2000,
    0x1000001e, 0x601a0, 0x67fff, 0x1301000, 0x7fff0006, 0x10000006,
    0x601b0, 0x67fff, 0x1401000, 0x7fff0006, 0x10000006, 0x601c0,
    0x67fff, 0xffff,
};

struct SceneEntrance gShamanVillage_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gShamanVillageShopEntrances[] = {
    {15, CONDITION_ALWAYS, 544, 0, 680, FACING_NORTH, 0, 420, 560, 660, 720, 0},
    {16, CONDITION_ALWAYS, 752, 0, 680, FACING_NORTH, 0, 630, 560, 870, 720, 0},
    {17, CONDITION_ALWAYS, 544, 0, 904, FACING_NORTH, 0, 420, 780, 660, 940, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gShamanVillageSanctumEntrances[] = {
    {6, CONDITION_ALWAYS, 720, 0, 544, FACING_NORTH, 0, 600, 380, 840, 600, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(ShamanVillageTrialGrounds),
    SCENE_EXIT(1, TrialRoadStages, 31),
    SCENE_EXIT(2, TrialRoadStages, 32),
    SCENE_EXIT(3, ShamanVillageAroma, 1),
    SCENE_EXIT(10, TrialRoadStages, 50),
    SCENE_EXIT(11, TrialRoadStages, 31),
    SCENE_EXIT(12, TrialRoadStages, 32),
    SCENE_EXITS(ShamanVillageAroma),
    SCENE_EXIT(1, ShamanVillageTrialGrounds, 3),
    SCENE_EXIT(2, ShamanVillage, 8),
    SCENE_EXIT(3, ShamanVillage, 11),
    SCENE_EXIT(4, ShamanVillageShop, 17),
    SCENE_EXIT(5, ShamanVillage, 14),
    SCENE_EXIT(6, ShamanVillage, 9),
    SCENE_EXITS(ShamanVillage),
    SCENE_EXIT(1, WorldMap, 58),
    SCENE_EXIT(2, ShamanVillageShop, 15),
    SCENE_EXIT(3, ShamanVillageShop, 16),
    SCENE_EXIT(4, ShamanVillageInn, 6),
    SCENE_EXIT(5, ShamanVillageInn, 8),
    SCENE_EXIT(6, ShamanVillageInn, 11),
    SCENE_EXIT(7, ShamanVillageInn, 13),
    SCENE_EXIT(8, ShamanVillageAroma, 2),
    SCENE_EXIT(9, ShamanVillageAroma, 6),
    SCENE_EXIT(10, ShamanVillageSanctum, 6),
    SCENE_EXIT(11, ShamanVillageAroma, 3),
    SCENE_EXIT(12, ShamanVillageInn, 3),
    SCENE_EXIT(13, ShamanVillageInn, 1),
    SCENE_EXIT(14, ShamanVillageAroma, 5),
    SCENE_EXITS(ShamanVillageInn),
    SCENE_EXIT(1, ShamanVillage, 13),
    SCENE_EXIT(2, ShamanVillageInn, 4),
    SCENE_EXIT(3, ShamanVillage, 12),
    SCENE_EXIT(4, ShamanVillageInn, 2),
    SCENE_EXIT(5, ShamanVillageInn, 9),
    SCENE_EXIT(6, ShamanVillage, 4),
    SCENE_EXIT(7, ShamanVillageInn, 10),
    SCENE_EXIT(8, ShamanVillage, 5),
    SCENE_EXIT(9, ShamanVillageInn, 5),
    SCENE_EXIT(10, ShamanVillageInn, 7),
    SCENE_EXIT(11, ShamanVillage, 6),
    SCENE_EXIT(12, ShamanVillageInn, 14),
    SCENE_EXIT(13, ShamanVillage, 7),
    SCENE_EXIT(14, ShamanVillageInn, 12),
    SCENE_EXITS(ShamanVillageShop),
    SCENE_EXIT(15, ShamanVillage, 2),
    SCENE_EXIT(16, ShamanVillage, 3),
    SCENE_EXIT(17, ShamanVillageAroma, 4),
    SCENE_EXITS(ShamanVillageSanctum),
    SCENE_EXIT(6, ShamanVillage, 10),
    SCENE_EXITS_END,
};

struct ActorPlacement gShamanVillage_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillageTrialGroundsActorPlacements[] = {
    {305, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_EAST, 2, 2},
    {382, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_EAST, 2, 2},
    ACTOR_PLACEMENT(41, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 224, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(40, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 264, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(40, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 264, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillageAromaRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 120, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 120, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 7, 536, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 336, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(46, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_AROMA), MOVEMENT_TYPE_STAND, 680, 0, 248, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillageVillageActorPlacements[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 428, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 248, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 364, 0, 248, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(107, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 254, 0, 434, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 434, 0, 446, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(138, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 412, 0, 230, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(41, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(40, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(40, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, 7, 40, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillageInnActorPlacements[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 90, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 110, 0, 172, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(138, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 126, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(41, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 264, 0, 381, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(134, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 381, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(135, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 140, 0, 388, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 694, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 144, 0, 658, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 164, 0, 432, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 460, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 114, 0, 364, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillageShopActorPlacements[] = {
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 678, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 508, 0, 656, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 542, 0, 628, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(138, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 550, 0, 640, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(111, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 752, 0, 648, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 724, 0, 628, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillageSanctumActorPlacements[] = {
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 720, 0, 466, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 746, 0, 516, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 700, 0, 524, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gShamanVillage_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillageTrialGroundsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST),
     526, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     EVENT(ShamanVillage_ShebaClearsSand)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14 | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_SHAMAN_VILLAGE_SAND_CLEARED, 0},
    {TRIGGER_STEP, 10, FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD,
     EVENT(ShamanVillage_ExplainTrialRules)},
    {TRIGGER_STEP, 10, 0x201, EVENT(ShamanVillage_ChallengeMoapa)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(ShamanVillage_OfferToGiveUp)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(ShamanVillage_TakeLeftPath)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(ShamanVillage_TakeRightPath)},
    {TRIGGER_TALK, 15, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanYegelosSandChallenge)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanChooseEitherRoad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanRodWasStolen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaCannotLose)},
    {TRIGGER_TALK, 17, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanTrialRoadAncientBattle)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanNeedContigoOrShamanPower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaGreatestWarrior)},
    {TRIGGER_TALK, 16, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanAncientHeroesVanishedSand)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToOtherGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanSandChallengeWillFail)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanJadeNeedsYegelosHeir)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillageAromaRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     4, 0x194d, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(ShamanVillage_MeetAroma)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14,
     FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED,
     EVENT(ShamanVillage_DropPillar)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 22, 0x94d,
     EVENT(ShamanVillage_StopAtClosedEntrance)},
    {TRIGGER_EXAMINE, 10, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ReadTrialRoadSign)},
    {TRIGGER_STEP, 11, 0x203, EVENT(ShamanVillage_GreetFortuneSeeker)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 17, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TellFortune)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0xa21,
     MSGNO(MsgShamanCloudyFortuneBeforeTrial)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgShamanCloudyFortuneAfterTrial)},
    {TRIGGER_TALK, 15, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanNewLegendaryBattle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanAncestorsDistrustOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanContigoHeroVictorySurprise)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillageVillageTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     2, 0x194d, 2},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     3, 0x194d, 3},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(ShamanVillage_EnterDoorA)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(ShamanVillage_EnterDoorB)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, 0x194d,
     EVENT(ShamanVillage_EnterDoorC)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, 0x194d,
     EVENT(ShamanVillage_EnterDoorD)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT(ShamanVillage_EnterDoorE)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 13, FLAG_SHAMAN_VILLAGE_MOAPA_MET,
     EVENT(ShamanVillage_MeetMoapa)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS,
     EVENT(ShamanVillage_EnterMoapasHouse)},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 19, 0x204, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 21, 0x205,
     EVENT(ShamanVillage_GrowVine)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 20, 0x94d,
     EVENT(Scene_ShowEntranceLocked)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 22, 0x94d,
     EVENT(ShamanVillage_StopAtClosedEntrance)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 21, FLAG_SHAMAN_VILLAGE_MOAPA_MET,
     EVENT(ShamanVillage_MeetMoapa)},
    {TRIGGER_TALK, 8, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaImpressedByChallenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanWaitForOutsidersToLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaPraisesOpponents)},
    {TRIGGER_TALK, 9, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanRespectForeignWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanChiefForbidsTalkingToOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgShamanDoubtMoapaDefeat)},
    {TRIGGER_TALK, 10, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanVictoryProvesSkill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanWhoInvitedOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanHopeForStrongerShamanChild)},
    {TRIGGER_TALK, 11, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanForeignersCanBeHeroes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanRefuseToTalkToOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanWorldShouldWelcomeStrangers)},
    {TRIGGER_TALK, 12, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     MSGNO(MsgShamanHoabnaPromiseFulfilled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanTrustContigoFriends)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgShamanRemainFriendsWithContigo)},
    {TRIGGER_TALK, ACTOR_MOUNTAIN_SIDE_SHAMAN, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_MOUNTAIN_SIDE_SHAMAN, CONDITION_ALWAYS,
     EVENT(ShamanVillage_AskAboutOtherSide)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOUNTAIN_SIDE_SHAMAN,
     FLAG_MOAPA_BEATEN, MSGNO(MsgShamanDistrustFriendlyOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOUNTAIN_SIDE_SHAMAN,
     CONDITION_ALWAYS, MSGNO(MsgShamanWishToBreakSandWalls)},
    {TRIGGER_TALK, 14, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgShamanChosenFewHavePower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanShutDoorsAgainstOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanOutskirtsHaveDifferentPowers)},
    {TRIGGER_TALK, ACTOR_MOAPA, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TellOutsidersToLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA, CONDITION_ALWAYS,
     MSGNO(MsgShamanOutsidersAreNotContigo)},
    {TRIGGER_TALK, ACTOR_EAST_GUARD, FLAG_MOAPA_BEATEN,
     EVENT(ShamanVillage_WarnOfMoapasPower)},
    {TRIGGER_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgShamanGuardPromisesRematchVictory)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanContigoTempleTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgShamanClimbingTrialRoadIsHard)},
    {TRIGGER_TALK, ACTOR_WEST_GUARD, FLAG_MOAPA_BEATEN,
     EVENT(ShamanVillage_SendToMoapa)},
    {TRIGGER_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgShamanVillagersBlameGuardDefeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanMoapaSparedOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgShamanGuardBlamesMountainClimb)},
    {USE_ITEM_ON_ACTOR(ITEM_SHAMANS_ROD), 15,
     FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN,
     EVENT(ShamanVillage_ShowShamansRod)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), 15, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ShowItemToMoapa)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), 16, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ShowItemToGuard)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), 17, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ShowItemToOtherGuard)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillageInnTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_TALK, ACTOR_MOAPAS_MAN_A, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_MOAPAS_MAN_A, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaWantsToKnowNames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_MAN_A,
     FLAG_MOAPA_BEATEN, MSGNO(MsgShamanMoapaLeftDoorOpen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_MAN_A, CONDITION_ALWAYS,
     MSGNO(MsgShamanShameNotKnowingVictors)},
    {TRIGGER_TALK, ACTOR_MOAPAS_MAN_B, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_MOAPAS_MAN_B, CONDITION_ALWAYS,
     MSGNO(MsgShamanFormalBattleNeedsNames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_MAN_B,
     FLAG_MOAPA_BEATEN, MSGNO(MsgShamanFatherNeverLoses)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_MAN_B, CONDITION_ALWAYS,
     MSGNO(MsgShamanFatherLostToRudeOutsiders)},
    {TRIGGER_TALK, ACTOR_MOAPAS_MAN_C, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_MOAPAS_MAN_C, CONDITION_ALWAYS,
     MSGNO(MsgShamanNamesDoNotMatter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_MAN_C,
     FLAG_MOAPA_BEATEN, MSGNO(MsgShamanFirstForeignVisitors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_MAN_C, CONDITION_ALWAYS,
     MSGNO(MsgShamanNamesBeforeBattleOldFashioned)},
    {TRIGGER_TALK, ACTOR_MOAPA_AT_HOME, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToMoapa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA_AT_HOME,
     FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES,
     MSGNO(MsgShamanVictorsDeserveRespect)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA_AT_HOME,
     CONDITION_ALWAYS, MSGNO(MsgShamanVictorsNamesOnTrialColumns)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanPayingStrangersMayStay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaMiserableAfterDefeat)},
    {TRIGGER_TALK, 14, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanForgiveUnkindVillagers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanMoapaAllowsStrangersToStay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanVillagersWorryTooMuch)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanFinallyOpenLockedDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanQuietWhileLockedInside)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanMissedTrialRoadChallenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanHidingLawPreventedWatching)},
    {TRIGGER_TALK, ACTOR_PICNIC_SHAMAN_A, CONDITION_ALWAYS,
     MSGNO(MsgShamanEnjoyGettingOutside)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PICNIC_SHAMAN_A,
     CONDITION_ALWAYS, MSGNO(MsgShamanChildrenIgnoreTradition)},
    {TRIGGER_TALK, ACTOR_PICNIC_SHAMAN_B, CONDITION_ALWAYS,
     MSGNO(MsgShamanPlanPicnicMeal)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PICNIC_SHAMAN_B,
     CONDITION_ALWAYS, MSGNO(MsgShamanEagerForDinner)},
    {TRIGGER_TALK, ACTOR_INN_COOK, CONDITION_ALWAYS,
     MSGNO(MsgShamanSimpleMeatPotatoesBeans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_COOK, CONDITION_ALWAYS,
     MSGNO(MsgShamanSaltAndPepperOnly)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillageShopTriggers[] = {
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanAncientHesperiaAttekaWar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanNewContigoHeroStronger)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaYegelosDecideWar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgShamanDoubtShamanSuperiority)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanTownsChooseChampions)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanGrandparentsUpsetByDefeat)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanYegelosBraveJourney)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanGrandparentsTiredOfWarStories)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToShopkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgShamanHusbandWorriesAboutWeapons)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgShamanWarehouseOwnerFeelsResponsible)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgShamanWeaponsDidNotCauseLoss)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillageSanctumTriggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToPriest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanVisitorsMustFindMoapa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgShamanFearRenewedContigoWar)},
    {TRIGGER_TALK, ACTOR_SANCTUM_SHAMAN_A, CONDITION_ALWAYS,
     MSGNO(MsgShamanThreatenContigoRematch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_SHAMAN_A,
     CONDITION_ALWAYS, MSGNO(MsgShamanMoapaMustHaveBeenHungry)},
    {TRIGGER_TALK, ACTOR_SANCTUM_SHAMAN_B, CONDITION_ALWAYS,
     MSGNO(MsgShamanPrayForShamanWarrior)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_SHAMAN_B,
     CONDITION_ALWAYS, MSGNO(MsgShamanPrayForContigoConqueror)},
    {TRIGGER_TABLE_END},
};

u32 gDriftScene_SpawnScript[] = {
    SC_WAIT(46),
    SC_RELEASE,
};
#undef TLA_SHAMAN_VILLAGE_H
#undef WORD_AT
#undef OBJ14_BYTE
#undef gShamanVillageVillageActorPlacements
