/// @file
/// Jupiter Lighthouse outside its aerie, where the party finds Isaac's: Garet
/// hangs from the ledge and Mia is held while Isaac and Ivan fight Karst and
/// Agatio (JupiterLighthouse_StartTheFight), the party arrives to watch
/// (JupiterLighthouse_PartyArrivesAtTheFight), and Isaac and Ivan fall
/// (JupiterLighthouse_IsaacAndIvanFall) and are hurt
/// (JupiterLighthouse_IsaacAndIvanAreHurt). Its whirlwinds and blocks lead on.
#include "flags.h"
#include "types.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "field_event.h"
#include "field/event_inline.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "actor.h"

void Leader_CenterInCell(void);
#include "system/io_reg.h"
#include "system/dma.h"
#include "system/io_write.h"
#include "call.h"
#include "field_sprite.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "graphics/resource.h"
#include "scene_ids.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "system/heap_release.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "objcmd.h"
#include "macros.h"

struct CellTarget;
struct CellTargetEntry;

void PaletteCycle_SetPausesInEvents(s32 value);

extern u32 gJupiterLighthouse_GaretHangsScript[];
extern u32 gJupiterLighthouse_MiaScript[];
extern u32 gJupiterLighthouse_FelixLooksAroundScript[];
extern u32 gJupiterLighthouse_KradenArrivesWestScript[];
extern u32 gJupiterLighthouse_KradenArrivesEastScript[];
extern u32 gJupiterLighthouse_PiersArrivesWestScript[];
extern u32 gJupiterLighthouse_JennaArrivesEastScript[];
extern u32 gJupiterLighthouse_ShebaArrivesWestScript[];

extern u32 gJupiterLighthouse_KarstStepsUpScript[];
extern u32 gJupiterLighthouse_AgatioTurnsScript[];
extern u32 gJupiterLighthouse_AgatioFacesSouthScript[];
extern u32 gJupiterLighthouse_KarstFacesEastScript[];
extern u32 gJupiterLighthouse_IsaacFacesWestScript[];
extern u32 gJupiterLighthouse_IvanFacesNorthScript[];

extern u32 gJupiterLighthouse_PiersStepsBackScript[];

extern u32 gJupiterLighthouse_FelixWaitsScript[];
extern u32 gJupiterLighthouse_KradenWaitsScript[];
extern u32 gJupiterLighthouse_PiersStepsUpScript[];
extern u32 gJupiterLighthouse_ShebaStepsUpScript[];
extern u32 gJupiterLighthouse_JennaStepsUpScript[];

extern u32 gJupiterLighthouse_KarstLeavesScript[];
extern u32 gJupiterLighthouse_KarstFliesOffScript[];

void JupiterLighthouse_LoadPalettes(s32 mode);

void JupiterLighthouse_RideUpperWhirlwind(void);

void JupiterLighthouse_RideWhirlwind(void);

void JupiterLighthouse_OpenFloor(void);

void JupiterLighthouse_CloseFloor(void);

void Whirlwind_CarryLeader(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

extern struct CellTarget gCrackedTiles;

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void CrackedTile_Fall(struct CellTargetHead *table,
                      struct CellTargetEntry *entry);

s32 Event_ClearInvalidPackedValues();
void HangingVine_Place(s32 id, s32 mode);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void MapGrid_SetAttributeBAt();
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

#include "text/msg_ids.h"

s32 JupiterLighthouse_ClearScriptedActorSpriteFlags(s32 object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gWhirlwindRiding;

struct GustObject *Whirlwind_SpawnUpper(struct GustObject *source);

void JupiterLighthouse_RideUpperWhirlwind(void) {
  if (gWhirlwindRiding == 0) {
    Whirlwind_SpawnUpper((struct GustObject *)Actor_Get(24));
    gWhirlwindRiding = 1;
  }
}

void JupiterLighthouse_RideWhirlwind(void) {
  if (gWhirlwindRiding == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode == 10) {
      struct Actor *object = Actor_Get(party->currentOwner);

      party->renderMode = 12;
      WORD_AT(object, 48) = 0;
      WORD_AT(object, 52) = 0;
      Whirlwind_SpawnUpper((struct GustObject *)Actor_Get(25));
      gWhirlwindRiding = 1;
    }
  }
}

s32 Actor_EmitRisingSparks(struct Actor *object);

void JupiterLighthouse_FlashSky(void) {

  u16 *buffer;
  u16 *p;
  u32 i;

  {
    u8 *enabled = &gCpuLoadDisplayEnabled;

    *enabled = 0;
  }
  REG_DISPCNT &= ~0x100;
  Io_Write16(0x400, &REG_BG0CNT);
  buffer = Heap_Alloc(0x800);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                      buffer);
  Dma_Set(buffer, (void *)(0x06000000), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)0x06002000, 0x800);
  buffer = Heap_Alloc(128);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningMap),
                      buffer);
  p = buffer;
  for (i = 0; i != 64; i++)
    *p++ &= 0xfcff;
  p = buffer;
  for (i = 0; i != 16; i++) {
    Dma_Set(p, (void *)(0x06002000 + (i << 6)), DMA_COPY_WORDS(2), REG_DMA3);
    p += 4;
  }
  Heap_Release(buffer);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f41;
  gBgScroll[0].x = -104;
  gBgScroll[0].y = 16;
  System_Wait(30);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= 0x100;
  Event_PlaySound(SE_CLATTER);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 2);
  FieldActor_Jump(4, 6, 0);
  Actor_Get(4)->actionFlags &= ~1;
  Actor_WalkBy(4, -16, 0);
  Actor_Get(20)->update = Actor_EmitRisingSparks;
  for (i = 0; i != 32; i++) {
    REG_BLDALPHA = (16 - (i >> 1)) | 0x1000;
    System_Wait(1);
  }
  MapGrid_CopyCellsLayered(104, 42, 5, 11, 61, 42);
  MapGrid_CopyCellsLayered(106, 52, 1, 1, 63, 84);
  FieldMapGrid_SetCellBehavior(130 << 19, 160 << 16, 2, 236);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                         gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  REG_DISPCNT |= 0x100;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  System_Wait(1);
  gCpuLoadDisplayEnabled = 1;
  MapGrid_SetShake(0, 0, 0);
}

extern void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);

void JupiterLighthouse_StartMovingBlock(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  Call4(MapGrid_SetCellHeight, object->x, object->z, 0, -8);
}

static __inline__ void JupiterLighthouse_SetCellHeight(s32 x, s32 z, s32 a,
                                                       s32 b) {
  MapGrid_SetCellHeight(x, z, a, b);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  /* FAKEMATCH(wrapper): forwarded inline. */
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

void JupiterLighthouse_FinishMovingBlock(s32 unused, s32 id) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(id);
  s32 x;
  s32 i;

  if (object->x >> 20 == 63 && object->z >> 20 == 15) {
    Event_SetFlag(2572);
    object->motionFlags = 3;
    System_Wait(2);
    i = 0;
    if (object->y > object->terrainHeight || object->velocityY != 0) {
      do {
        System_Wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight || object->velocityY != 0);
    }
    object->motionFlags = 0;
    Event_PlaySound(SE_HEAVY_IMPACT);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(254 << 18, -1, 232 << 16, 1);
    FieldCamera_WaitForArrival();
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 1);
    x = leader->x >> 20;
    if (x == 64) {
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1032, 232);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 232);
    } else if (x == 63) {
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 232);
    }
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 248);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
    PaletteFade_ApplySource(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 0);
    PaletteFade_Interpolate(60);
    FieldEvent_Wait(50);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x6000);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x2000);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x6000);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x2000);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
    FieldEvent_Wait(20);
    FieldActor_ShowEmote(gMainRoutineWork.currentOwner,
                         EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
    FieldEvent_Wait(40);
    PaletteCycle_SetPausesInEvents(0);
    JupiterLighthouse_FlashSky();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
    FieldActor_StartRepeatedMotion(4, 3);
    Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(40);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(3);
    Camera_MoveWithinMapBounds_(131 << 19, 0, 174 << 16, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    object->update = 0;
    Event_PlaySound(MUS_080);
    Sound_WaitForJingle();
    FieldEvent_Wait(10);
    Camera_MoveWithinMapBounds_(246 << 18, -0x400000, 248 << 16, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    PaletteCycle_SetPausesInEvents(1);
    Event_End();
  }
  JupiterLighthouse_SetCellHeight(object->x, object->z, 0, -6);
}

/// Opens the floor under (49, 103) and (49, 106) once (flag 514, until 2591).
/// Object 23, standing on the ground at x cell 49 to 52 with flag 2573 clear,
/// falls through and sets 2573: from cells 50 and 51 it drops, lands with the
/// screen shaken and slides off sideways; from 49 and 52 it simply drops.
void JupiterLighthouse_OpenFloor(void) {
  struct Actor *object = Actor_GetById(23);
  s32 spot = 0;
  s32 x;
  s32 i;

  if (Event_GetFlag(2591))
    return;
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_CHEST_OPEN);
  /* FAKEMATCH(wrapper): forwarded through Camera_MoveWithinMapBounds_ above. */
  Camera_MoveWithinMapBounds_(194 << 18, -1, px(208), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(6);
  Sound_Play(SE_FLOOR_SWITCH);
  MapGrid_CopyCellsLayered(49, 103, 4, 3, 49, 76);
  Event_Wait(6);
  MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
  x = object->x >> 20;
  if (x == 49)
    spot = 1;
  if (x == 50)
    spot = 2;
  if (x == 51)
    spot = 3;
  if (x == 52)
    spot = 4;
  if (object->y != 0)
    spot = 0;
  if (Event_GetFlag(2573))
    spot = 0;
  if (spot != 0)
    GameFlag_Set(2573);
  if ((u32)(spot - 2) <= 1) {
    FieldCamera_SetSpeed(4);
    EventCamera_FollowActor(23, 1);
    FieldCamera_WaitForArrival();
    Sound_Play(SE_310);
    object->gravity = px(1) / 2;
    object->motionFlags = 2;
    object->terrainHeight = px(-80);
    i = 0;
    if (object->y > object->terrainHeight) {
      do {
        System_Wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight);
    }
    MapGrid_SetShake(3, 3, 0.95);
    Sound_Play(SE_HEAVY_IMPACT);
    object->gravity = px(1);
    object->velocityY = px(6);
    Actor_SetSpeed(23, 1.5);
    if (spot == 2)
      Actor_SetDestinationOffset(23, -32, 0);
    else
      Actor_SetDestinationOffset(23, 32, 0);
    Event_Wait(7);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    object->gravity = px(1) / 2;
    object->terrainHeight = px(-240);
    Event_Wait(80);
    MapGrid_SetShake(5, 5, 1);
    Sound_Play(SE_CLATTER);
    Event_Wait(20);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(10);
  } else if (spot == 1 || spot == 4) {
    FieldActor_SetSpritePriority(23, 1);
    FieldCamera_SetSpeed(4);
    EventCamera_FollowActor(23, 1);
    FieldCamera_WaitForArrival();
    Sound_Play(SE_310);
    object->gravity = px(1) / 2;
    object->motionFlags = 2;
    object->terrainHeight = px(-240);
    Event_Wait(20);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    Event_Wait(60);
    MapGrid_SetShake(5, 5, 1);
    Sound_Play(SE_CLATTER);
    Event_Wait(20);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(10);
  }
  Event_Wait(20);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

void JupiterLighthouse_CloseFloor(void) {
  if (Event_GetFlag(0xa1f) != 0)
    return;
  Event_ClearFlag(FLAG_SCENE_TEMP_02);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_CHEST_CLOSE);
  FieldCamera_MoveTo(776, CAMERA_KEEP, 208, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(6);
  MapGrid_CopyCellsLayered(49, 103, 4, 3, 49, 76);
  FieldEvent_Wait(6);
  Event_PlaySound(SE_IMPACT);
  MapGrid_CopyCellsLayered(49, 100, 4, 3, 49, 76);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Talking to one of the party: each answers with its own message.
void JupiterLighthouse_TalkToReunitedParty(s32 actor) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  switch (actor) {
  case 0:
    Actor_RunRepeatedMotion(0, 2);
    FieldEvent_SetMessageId(MsgNoWhyAre);
    Event_ShowMessage(0, 0);
    break;
  case 2:
    Actor_RunRepeatedMotion(2, 2);
    FieldEvent_SetMessageId(MsgDonTWorry);
    Event_ShowMessage(2, 0);
    break;
  case 28:
    FieldEvent_SetMessageId(MsgWeHaveTo);
    Event_ShowMessage(28, 0);
    break;
  case 5:
    FieldEvent_SetMessageId(MsgIDonT);
    Event_ShowMessage(5, 0);
    break;
  case 6:
    Actor_SetAnimation(6, 4);
    FieldEvent_Wait(33);
    FieldEvent_SetMessageId(MsgYouVeBeen);
    Event_ShowMessage(6, 0);
    break;
  }
  Event_End();
}

s32 JupiterLighthouse_SwayGaret(void) {
  struct Actor *object = Actor_Get(1);

  if (object != NULL) {
    struct FieldSprite *sprite = object->sprite;

    if ((gFrameCount & 127) == 0)
      Actor_SetAttachedEffect(29,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    if (sprite != NULL) {
      *(sprite->unk14 + 3) = 30;
      *(sprite->unk14 + 2) = 252;
      sprite->rotation = Math_Sin(gFrameCount * 728) / 128 - 0x800;
    }
  }
  return 1;
}

s32 JupiterLighthouse_PlayRandomBlowSound(void) {
  switch (Random16() & 7) {
  case 0:
    Event_PlaySound(SE_RECOVERY);
    break;
  case 1:
    Event_PlaySound(SE_POWER_UP);
    break;
  case 2:
    Event_PlaySound(SE_HEAVY_IMPACT);
    break;
  case 3:
  case 5:
  case 6:
  default:
    Event_PlaySound(SE_IMPACT);
    break;
  case 4:
  case 7:
    Event_PlaySound(SE_CAST);
    break;
  }
  return 0;
}

s32 JupiterLighthouse_ClearActionBit(u8 *object) {
  object[90] &= 0xfe;
  return 0;
}

extern u32 gJupiterLighthouse_IsaacFightsScript[];
extern u32 gJupiterLighthouse_KarstFightsScript[];
extern u32 gJupiterLighthouse_IvanFightsScript[];
extern u32 gJupiterLighthouse_AgatioFightsScript[];

/// Places an actor at (x, z), in pixels, facing a direction.
#define PLACE(id, x, z, facing)                                                \
  {                                                                            \
    s32 facing_ = (facing);                                                    \
    Actor_SetPositionAndFacing(id, (x) << 16, (z) << 16, facing_);             \
  }

/// Sets four actors' speeds, places them and starts their action scripts.
void JupiterLighthouse_StartTheFight(void) {
  Actor_SetSpeed(26, 3);
  Actor_SetSpeed(27, 3);
  Actor_SetSpeed(0, 3);
  Actor_SetSpeed(2, 3);
  PLACE(0, 776, 202, FACING_WEST);
  PLACE(2, 731, 232, FACING_NORTH);
  PLACE(27, 731, 213, FACING_SOUTH);
  PLACE(26, 737, 202, FACING_EAST);
  Actor_SetScript(0, gJupiterLighthouse_IsaacFightsScript);
  Actor_SetScript(2, gJupiterLighthouse_IvanFightsScript);
  Actor_SetScript(26, gJupiterLighthouse_KarstFightsScript);
  Actor_SetScript(27, gJupiterLighthouse_AgatioFightsScript);
  Actor_SetScript(3, gJupiterLighthouse_MiaScript);
}

void JupiterLighthouse_PlaceForTheFight(void) {
  PLACE(2, 757, 216, FACING_SOUTHEAST);
  PLACE(0, 761, 219, FACING_SOUTHEAST);
  PLACE(3, 816, 285, FACING_SOUTHEAST);
  PLACE(1, 824, 302, FACING_NORTH);
  PLACE(29, 821, 302, FACING_NORTH);
  Actor_SetSpriteFlags(Actor_Get(1), 0);
  Actor_SetScript(1, gJupiterLighthouse_GaretHangsScript);
  {
    s32 priority = 0x480;

    FieldTask_AddFrame((void (*)(void))JupiterLighthouse_SwayGaret, priority);
  }
  Actor_SetAnimation(0, 38);
  Actor_SetAnimation(2, 31);
  Actor_SetAnimation(3, 9);
}

extern u32 gJupiterLighthouse_FelixArrivesWestScript[];
extern u32 gJupiterLighthouse_FelixArrivesEastScript[];
extern u32 gJupiterLighthouse_PiersArrivesEastScript[];
extern u32 gJupiterLighthouse_JennaArrivesWestScript[];
extern u32 gJupiterLighthouse_ShebaArrivesEastScript[];
extern u32 gJupiterLighthouse_AgatioStepsUpScript[];

static __inline__ struct Actor *I_Func_020048cc(s32 actor, s32 emote,
                                                s32 frames) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants. */
  return (struct Actor *)Actor_ShowEmoteById(actor, emote, frames);
}

/// The scene in which actor 4 meets 5, 6, 7 and 28 and the group is placed for
/// the arrival; arg selects one of its two layouts.
void JupiterLighthouse_PartyArrivesAtTheFight(s32 arg) {
  struct Actor *object;
  s32 angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgAhhhh2);
  Event_PlaySound(SE_CHEST_OPEN);
  FieldEvent_Wait(26);
  Event_PlaySound(SE_FLOOR_SWITCH);
  Actor_SetScript(4, gJupiterLighthouse_FelixLooksAroundScript);
  if (arg != 0) {
    FieldEvent_ShowTwoMessagesAndWait(3, 20, 9, 15, 8, 1, 20, 2, 15, 1, 25);
    Event_PlaySound(SE_310);
    FieldEvent_Wait(30);
    Event_PlaySound(SE_CLATTER);
    FieldEvent_ShowTwoMessagesAndWait(3, 21, 9, 16, 8, 1, 21, 2, 16, 1, 0);
    FieldEvent_ShowTwoMessagesAndWait(0, 20, 9, 15, 8, 2, 20, 2, 15, 1, 0);
  } else {
    FieldEvent_ShowTwoMessagesAndWait(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 25);
    Event_PlaySound(SE_310);
    FieldEvent_Wait(30);
    Event_PlaySound(SE_CLATTER);
    FieldEvent_ShowTwoMessagesAndWait(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 0);
    FieldEvent_ShowTwoMessagesAndWait(0, 5, 9, 0, 8, 2, 5, 2, 0, 1, 0);
  }
  FieldActor_Stop(4);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldActor_LaunchFromLeader(7, 16, 0, FACING_WEST);
  FieldActor_LaunchFromLeader(28, -16, 0, FACING_EAST);
  FieldActor_LaunchFromLeader(6, 16, 24, FACING_WEST);
  FieldActor_LaunchFromLeader(5, -16, 24, FACING_EAST);
  EventCamera_FollowActor(5, 1);
  FieldActor_WaitForMoveById(5);
  (struct Actor *)Actor_ShowEmote(28, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST + FACING_STEP, 0);
  (struct Actor *)Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(5, FACING_NORTH + FACING_STEP, 0);
  Event_OpenMessage(7, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_SetAnimation(28, 3);
    FieldEvent_Wait(20);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(28, 0, 5);
    (gFieldWork->message)++;
  } else {
    Actor_SetAnimation(28, 4);
    FieldEvent_Wait(33);
    (gFieldWork->message)++;
    FieldEvent_ShowMessageThenWait(28, 0, 5);
  }
  Actor_RunRepeatedMotion(5, 2);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  (struct Actor *)Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  (struct Actor *)Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(4, 1.2);
  Actor_SetScript(4, arg != 0 ? gJupiterLighthouse_FelixArrivesWestScript
                              : gJupiterLighthouse_FelixArrivesEastScript);
  FieldEvent_Wait(30);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(28, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(28, 3);
  FieldEvent_Wait(30);
  Actor_SetSpeed(5, 1);
  Actor_SetSpeed(7, 1);
  Actor_SetSpeed(6, 1);
  Actor_SetSpeed(28, 0.8);
  Actor_SetScript(28, arg != 0 ? gJupiterLighthouse_KradenArrivesWestScript
                               : gJupiterLighthouse_KradenArrivesEastScript);
  Actor_SetScript(7, arg != 0 ? gJupiterLighthouse_PiersArrivesWestScript
                              : gJupiterLighthouse_PiersArrivesEastScript);
  Actor_SetScript(5, arg != 0 ? gJupiterLighthouse_JennaArrivesWestScript
                              : gJupiterLighthouse_JennaArrivesEastScript);
  Actor_SetScript(6, arg != 0 ? gJupiterLighthouse_ShebaArrivesWestScript
                              : gJupiterLighthouse_ShebaArrivesEastScript);
  FieldCamera_SetSpeed(0.5);
  if (arg != 0) {
    FieldCamera_MoveTo(677, -64, 287, 1);
    FieldCamera_WaitForArrival();
  } else {
    FieldCamera_MoveTo(950, -64, 287, 1);
    FieldCamera_WaitForArrival();
  }
  FieldActor_RefreshSelectorById(4);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(28);
  FieldEvent_Wait(20);
  angle = 0x2000;
  Actor_SetPositionAndFacing(2, px(741), px(232), angle);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(800, 0, 270, 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(MUS_039);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_SetAnimation(3, 0);
  Actor_FaceDirectionAndWait(3, FACING_NORTHWEST);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_FaceDirectionAndWait(3, angle);
  Actor_SetAnimation(3, 9);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_RunRepeatedMotion(1, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(1, 0, 5);
  FieldActor_StartRepeatedMotion(3, 3);
  Actor_SetAttachedEffect(3, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_RunRepeatedMotion(1, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(1, 0, 5);
  Actor_Get(3)->actionFlags &= 0xfe;
  Actor_SetSpeed(3, 0.1);
  Actor_SetDestinationOffset(3, 4, 2);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  Actor_SetDestinationOffset(3, -2, 2);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  Actor_SetDestinationOffset(3, 2, 1);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  Actor_SetDestinationOffset(3, -2, -1);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  FieldEvent_Wait(40);
  Actor_SetDestinationOffset(3, 0, -2);
  FieldActor_WaitForMoveById(3);
  Actor_SetSpeed(3, 2);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    Actor_SetAnimation(3, 4);
  } while (0);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_Get(3)->actionFlags |= 1;
  Actor_SetDestinationOffset(3, -2, -8);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 1);
  Actor_FaceDirectionAndWait(3, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_SetAnimation(0, 1);
  Actor_SetAnimation(2, 1);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  I_Func_020048cc(0, 258, 30);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_ShowMessageThenWait(0, 0, 5);
  } while (0);
  Actor_SetAnimation(2, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetScript(3, gJupiterLighthouse_MiaScript);
  Actor_SetPositionAndFacing(26, px(665), px(128), angle);
  Actor_SetPositionAndFacing(27, px(641), px(128), angle);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 256, 0);
  I_Func_020048cc(2, 256, 30);
  Actor_FaceDirection(0, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(2, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(720, 0, 210, 1);
  Actor_SetSpeedFixed(26, 0x4000, angle);
  Actor_SetSpeedFixed(27, 0x4000, angle);
  Actor_WalkTo(26, 714, 193);
  Actor_WalkTo(27, 698, 209);
  Actor_SetScript(26, gJupiterLighthouse_KarstStepsUpScript);
  Actor_SetScript(27, gJupiterLighthouse_AgatioStepsUpScript);
  FieldEvent_Wait(20);
  I_Func_020048cc(2, 258, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  Event_PlaySound(MUS_034);
  I_Func_020048cc(27, 264, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(26, 263, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 263, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  FieldActor_RefreshSelectorById(26);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldActor_RefreshSelectorById(27);
  } while (0);
  Actor_SetAnimation(27, 4);
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(2, 256, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 263, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(2, 257, 0);
  I_Func_020048cc(0, 257, 30);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  (struct Actor *)Actor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  Actor_FaceDirection(2, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_ShowMessageThenWait(0, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(2, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  Actor_FaceDirectionAndWait(26, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 265, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(26, angle);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(2, 263, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(27, 264, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  I_Func_020048cc(26, 264, 30);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  Actor_SetAnimation(27, 4);
  Actor_SetAnimation(26, 4);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  I_Func_020048cc(2, 263, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 256, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  (struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirectionAndWait(0, angle);
  Actor_SetAnimation(0, 38);
  FieldCamera_MoveTo(784, 0, 274, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  FieldCamera_MoveTo(720, 0, 210, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(0, 1);
  Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_FaceEachOther(26, 27, 0);
  FieldEvent_Wait(10);
  I_Func_020048cc(26, 264, 0);
  (struct Actor *)Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  (struct Actor *)Actor_ShowEmote(2, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  {
    /* FAKEMATCH(pin): the angle pinned to r1. */
    register s32 a1 asm("r1") = angle;
    s32 flags = 0;

    /* FAKEMATCH(barrier): r1 and r2 are set before r0. */
    __asm__("" : : "r"(a1), "r"(flags));
    Actor_FaceDirection(26, a1, flags);
  }
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    Actor_FaceDirectionAndWait(27, angle);
  } while (0);
  Actor_SetAnimation(26, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 4);
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(2, 259, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 264, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  I_Func_020048cc(27, 264, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(2, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 265, 30);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 4);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(27, FACING_NORTHWEST);
  FieldEvent_Wait(20);
  (struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_SetSpeed(27, 2);
  Actor_WalkByAndWait(27, -24, -24);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, FACING_NORTH);
  FieldEvent_Wait(10);
  (struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(27, angle);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(26, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowTwoMessagesAndWait(0, 20, 7, 15, 6, 2, 17, 13, 12, 12, 0);
  Actor_FaceDirection(0, FACING_NORTHWEST, 0);
  Actor_FaceDirection(2, FACING_NORTHWEST, 0);
  (struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(26, 263, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(26, angle);
  FieldEvent_Wait(10);
  I_Func_020048cc(26, 264, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  (struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_WalkBy(27, 24, 24);
  Actor_SetScript(27, gJupiterLighthouse_AgatioTurnsScript);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  FieldActor_RefreshSelectorById(27);
  Actor_FaceEachOther(26, 27, 30);
  Actor_SetAnimation(26, 3);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    Actor_SetAnimation(27, 3);
  } while (0);
  FieldEvent_Wait(20);
  FieldEvent_Wait(30);
  Actor_SetSpeed(26, 3);
  Actor_SetSpeed(27, 3);
  Actor_SetSpeed(0, 3);
  Actor_SetSpeed(2, 3);
  Actor_WalkTo(26, 737, 202);
  Actor_WalkTo(27, 731, 213);
  Actor_SetScript(27, gJupiterLighthouse_AgatioFacesSouthScript);
  Actor_SetScript(26, gJupiterLighthouse_KarstFacesEastScript);
  FieldEvent_Wait(5);
  Actor_WalkTo(0, 776, 202);
  Actor_WalkTo(2, 731, 232);
  Actor_SetScript(0, gJupiterLighthouse_IsaacFacesWestScript);
  Actor_SetScript(2, gJupiterLighthouse_IvanFacesNorthScript);
  FieldActor_RefreshSelectorById(0);
  FieldActor_RefreshSelectorById(2);
  FieldActor_RefreshSelectorById(26);
  FieldActor_RefreshSelectorById(27);
  FieldEvent_Wait(40);
  JupiterLighthouse_StartTheFight();
  FieldEvent_Wait(200);
  EventCamera_FollowActor(4, 1);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  I_Func_020048cc(6, 265, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  (struct Actor *)Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  (struct Actor *)Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(28, FACING_SOUTH);
  FieldEvent_Wait(10);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    FieldEvent_ShowMessageThenWait(28, 0, 5);
  } while (0);
  Actor_SetAnimation(4, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceActor(5, 4, 0);
  Actor_FaceActor(7, 4, 0);
  Actor_FaceActor(6, 4, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  /* FAKEMATCH(dead): the statement kept in a once-only block. */
  do {
    Actor_SetAnimation(6, 3);
  } while (0);
  Actor_SetAnimation(5, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetAnimation(7, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(7, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_SetAnimation(6, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(6, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_SetAnimation(5, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(5, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_SetAnimation(28, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(28, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_SetScript(7, gJupiterLighthouse_PiersStepsBackScript);
  Actor_SetScript(6, gJupiterLighthouse_PiersStepsBackScript);
  Actor_SetScript(5, gJupiterLighthouse_PiersStepsBackScript);
  Actor_SetScript(28, gJupiterLighthouse_PiersStepsBackScript);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(28);
  Event_End();
}

static __inline__ void JupiterLighthouseRoomM_AddHp(s32 owner, s32 delta) {
  /* FAKEMATCH(wrapper): forwarding through the inline reloads -10000 from the
   * pool at each call. */
  Unit_AddHp(owner, delta);
}

static __inline__ void JupiterLighthouseRoomM_AddPp(s32 owner, s32 delta) {
  /* FAKEMATCH(wrapper): forwarding through the inline reloads -10000 from the
   * pool at each call. */
  Unit_AddPp(owner, delta);
}

/// Places actors 0, 2, 27 and 26 and shows an apology; then the party is left
/// with owners 2 and 0, whose first values and PP drop by 10000. "I'm sorry"
void JupiterLighthouse_IsaacAndIvanAreHurt(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PLACE(0, 776, 202, FACING_WEST);
  PLACE(2, 731, 232, FACING_NORTHWEST);
  PLACE(27, 731, 213, FACING_SOUTH);
  PLACE(26, 737, 202, FACING_EAST);
  Actor_SetAnimation(0, 1);
  Actor_SetAnimation(2, 1);
  Actor_SetAnimation(26, 1);
  Actor_SetAnimation(27, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgIMSorry);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  (struct Actor *)Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldCamera_MoveTo(705, 0, 177, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Actor_SetSpriteFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 32);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHWEST);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  (struct Actor *)Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_WalkBy(26, 24, 0);
  FieldEvent_Wait(20);
  do {
    /* FAKEMATCH(dead): the store kept in a once-only block, whose end ends
     * CSE's block. */
    gMainRoutineWork.battleType = BATTLE_TYPE_LOST;
  } while (0);
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_JupiterLighthouseRoomM,
                                       77);
  Event_SetDefeatDestination((s32)&SceneId_JupiterLighthouseRoomM, 77);
  Party_RemoveMember(4);
  Party_RemoveMember(7);
  Party_RemoveMember(5);
  Party_RemoveMember(6);
  Party_RemoveMember(2);
  Party_RemoveMember(0);
  Party_AddMember(2);
  Party_AddMember(0);
  Party_RemoveMember(3);
  Party_RemoveMember(1);
  JupiterLighthouseRoomM_AddHp(2, -10000);
  JupiterLighthouseRoomM_AddHp(0, -10000);
  Unit_Calculate(0);
  Unit_Calculate(2);
  FieldUnit_AddHp(0, 173);
  JupiterLighthouseRoomM_AddPp(2, -10000);
  Unit_AddPp(0, -10000);
  gMainRoutineWork.currentOwner = 0;
  Field_RequestBattleFromEncounterTable(102, 4);
}

void JupiterLighthouse_PlaceTheHurt(void) {
  PLACE(0, 761, 219, FACING_NORTHWEST);
  PLACE(2, 741, 232, FACING_NORTHWEST);
  Actor_SetSpriteFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 39);
  Actor_SetSpriteFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 32);
  PLACE(5, 761, 203, FACING_SOUTH);
  PLACE(6, 725, 232, FACING_EAST);
  PLACE(28, 729, 203, FACING_SOUTHEAST);
  Actor_SetScript(3, gJupiterLighthouse_MiaScript);
}

#define EVENT_WORK gFieldWork

s32 Inventory_Equip();

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                              \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

static __inline__ void Walk(s32 id, s32 speed, s32 frames) {
  /* FAKEMATCH(wrapper): the walk forwarded inline. */
  ((struct Actor *)Actor_ShowEmoteById(id, speed, frames));
}

void JupiterLighthouse_IsaacAndIvanFall(void) {
  s32 index;
  s32 offset;
  s32 answer;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(737, 0, 209, 0);
  gMainRoutineWork.currentOwner = 4;
  Party_RemoveMember(0);
  Party_RemoveMember(2);
  Party_RemoveMember(5);
  Party_RemoveMember(6);
  Party_RemoveMember(7);
  Party_RemoveMember(3);
  Party_RemoveMember(1);
  Party_AddMember(4);
  Actor_SetSpeed(4, 1);
  {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(4, px(664), px(88), angle);
  }
  {
    s32 angle = 0x2000;

    Actor_SetPositionAndFacing(27, px(717), px(208), angle);
  }
  Actor_SetPositionAndFacing(26, px(737), px(195), FACING_SOUTHEAST);
  {
    s32 angle = 0xa000;

    Actor_SetPositionAndFacing(0, px(761), px(219), angle);
  }
  Actor_SetPositionAndFacing(2, px(741), px(232), FACING_NORTHWEST);
  Actor_SetScript(3, (s32)gJupiterLighthouse_MiaScript);
  Actor_SetSpriteFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 0x27);
  Actor_SetSpriteFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 0x20);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWeDidItAgatio);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_SetAnimation(26, 8);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(27, FACING_EAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  FieldActor_StartRepeatedMotion(0, 2);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  Walk(26, 0x100, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(27, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_SetAnimation(26, 1);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(26, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(20);
  ACTOR_SET_MODE_AND_WAIT(26, 3, 20);
  FieldEvent_Wait(10);
  EventCamera_GetActor()->motionFlags = 0;
  EventCamera_FollowActor(4, 1);
  FieldEvent_Wait(20);
  FieldActor_LaunchFromLeader(6, 0, 0, 0x4000);
  EventCamera_FollowActor(6, 1);
  FieldActor_LaunchFromLeader(7, 16, 0, 0x4000);
  FieldActor_LaunchFromLeader(28, -16, 24, 0x2000);
  FieldActor_LaunchFromLeader(5, -16, 0, 0x2000);
  Actor_WalkByAndWait(4, 16, 24);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  ((struct Actor *)Actor_ShowEmote(28, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30));
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirection(4, FACING_WEST, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 35);
  Actor_SetAnimation(4, 3);
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  FieldEvent_Wait(10);
  Actor_WalkTo(4, 683, 186);
  Actor_WalkTo(28, 667, 202);
  Actor_SetScript(4, (s32)gJupiterLighthouse_FelixWaitsScript);
  Actor_SetScript(28, (s32)gJupiterLighthouse_KradenWaitsScript);
  Actor_FaceDirection(26, FACING_WEST, 0);
  Actor_FaceDirection(27, FACING_WEST, 0);
  Walk(6, 0x102, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 33);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceActor(5, 7, 0);
  Actor_FaceActor(6, 7, 0);
  Actor_FaceActor(7, 5, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 20);
  FieldEvent_Wait(20);
  Actor_WalkBy(7, 0, 40);
  Actor_WalkBy(6, 0, 40);
  Actor_WalkBy(5, 0, 40);
  Actor_SetScript(7, (s32)gJupiterLighthouse_PiersStepsUpScript);
  Actor_SetScript(6, (s32)gJupiterLighthouse_ShebaStepsUpScript);
  Actor_SetScript(5, (s32)gJupiterLighthouse_JennaStepsUpScript);
  EventCamera_FollowActor(4, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Walk(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Walk(27, 0x107, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(26, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(26, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(26, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  ((struct Actor *)Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30));
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(6, 4, 33);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  ((struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SCOWL, 30));
  Event_OpenMessage(27, 0);
  Actor_FaceActor(28, 4, 0);
  Actor_FaceActor(5, 4, 0);
  Actor_FaceActor(6, 4, 0);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(10);
  answer = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    ((struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_WINCE, 30));
    FieldEvent_ShowMessageThenWait(27, 0, 5);
    EVENT_WORK->message++;
  } else {
    ((struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_WINCE, 30));
    EVENT_WORK->message++;
    FieldEvent_ShowMessageThenWait(27, 0, 5);
  }
  Actor_FaceActor(7, 26, 0);
  ((struct Actor *)Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SCOWL, 30));
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(26, 4, 33);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 33);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceEachOther(26, 0x1b, 0);
  FieldActor_RefreshSelectorById(26);
  FieldActor_RefreshSelectorById(27);
  Walk(26, 0x105, 0);
  ((struct Actor *)Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30));
  Actor_FaceActor(28, 27, 0);
  ACTOR_SET_MODE_AND_WAIT(28, 4, 33);
  Actor_FaceActor(26, 28, 0);
  Actor_FaceActor(27, 28, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  FieldEvent_Wait(60);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Walk(26, 0x108, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Walk(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  FieldEvent_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(26, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(27, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceActor(7, 4, 0);
  Actor_FaceEachOther(6, 5, 0);
  Actor_FaceEachOther(4, 0x1c, 0);
  FieldActor_RefreshSelectorById(4);
  FieldActor_RefreshSelectorById(28);
  Walk(4, 0x108, 0);
  Walk(6, 0x108, 0);
  Walk(5, 0x108, 0);
  Walk(7, 0x108, 0);
  ((struct Actor *)Actor_ShowEmote(28, EMOTE_IN_FRONT | EMOTE_SMILE, 40));
  Actor_FaceActor(4, 26, 0);
  Actor_FaceActor(5, 26, 0);
  Actor_FaceActor(6, 26, 0);
  Actor_FaceActor(7, 26, 0);
  Actor_FaceActor(28, 26, 0);
  ACTOR_SET_MODE_AND_WAIT(26, 3, 20);
  FieldEvent_Wait(10);
  Event_OpenMessage(26, 0);
  answer = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    ACTOR_SET_MODE_AND_WAIT(26, 4, 33);
    FieldEvent_ShowMessageThenWait(26, 0, 5);
    EVENT_WORK->message++;
  } else {
    ACTOR_SET_MODE_AND_WAIT(26, 4, 33);
    EVENT_WORK->message++;
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  }
  Actor_FaceEachOther(28, 4, 0);
  FieldActor_RefreshSelectorById(28);
  FieldEvent_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  Actor_FaceEachOther(4, 0x1c, 0);
  FieldActor_RefreshSelectorById(4);
  FieldEvent_Wait(10);
  ((struct Actor *)Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_WINCE, 30));
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  FieldEvent_Wait(40);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 20);
  FieldEvent_Wait(30);
  Actor_FaceEachOther(26, 0x1b, 0);
  FieldActor_RefreshSelectorById(26);
  FieldActor_RefreshSelectorById(27);
  Walk(27, 0x108, 0);
  Walk(26, 0x108, 30);
  Actor_FaceActor(26, 4, 0);
  Actor_FaceActor(4, 27, 0);
  Actor_WalkToAndWait(27, 699, 202);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Walk(0, 0x100, 0);
  Walk(4, 0x100, 0);
  Walk(2, 0x100, 0);
  Walk(5, 0x100, 0);
  ((struct Actor *)Actor_ShowEmote(28, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30));
  ((struct Actor *)Actor_ShowEmote(28, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30));
  Actor_FaceDirection(28, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  ((struct Actor *)Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_SMILE, 30));
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceEachOther(4, 7, 0);
  Actor_FaceEachOther(6, 5, 0);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldEvent_Wait(40);
  Actor_FaceActor(4, 26, 0);
  Actor_FaceActor(7, 26, 0);
  Actor_FaceActor(6, 26, 0);
  Actor_FaceActor(5, 26, 0);
  ((struct Actor *)Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_SCOWL, 30));
  Actor_FaceDirection(27, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_SetSpeed(7, 4);
  Actor_SetSpeed(27, 2);
  Actor_WalkBy(7, 8, 24);
  Actor_WalkByAndWait(27, 8, 4);
  FieldActor_WaitForMoveById(7);
  FieldEvent_Wait(5);
  ((struct Actor *)Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30));
  Actor_FaceActor(26, 7, 0);
  Actor_FaceActor(27, 7, 0);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_FaceActor(26, 0, 0);
  Actor_FaceActor(27, 0, 0);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(4, FACING_SOUTHEAST);
  FieldEvent_Wait(20);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 20);
  FieldEvent_Wait(10);
  FieldActor_WatchAndSetScript(26, 4);
  FieldActor_WatchAndSetScript(27, 4);
  Actor_WalkByAndWait(4, 5, 32);
  Actor_WalkByAndWait(4, 56, -2);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(0, 2);
  FieldEvent_Wait(4);
  Actor_RunRepeatedMotion(4, 2);
  PartyInventory_Remove(0xdf);
  PartyInventory_Add(245);
  Message_ShowCentered((s16)EVENT_WORK->message++, 5);
  index = 0;
  offset = 0xd8;
  for (index = 0; index <= 14; offset += 2, index++) {
    if ((*(u16 *)((u8 *)Unit_Get(4) + offset) & 0x1ff) == 245) {
      Inventory_Equip(4, index);
      break;
    }
  }
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceActor(4, 27, 0);
  FieldEvent_Wait(20);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 20);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 2);
  Actor_SetSpeed(26, 0.5);
  Actor_SetSpeed(27, 0.5);
  Actor_SetSpeed(4, 0.5);
  Actor_SetScript(26, (s32)gJupiterLighthouse_KarstLeavesScript);
  FieldEvent_Wait(40);
  Actor_SetDestinationOffset(7, -16, 0);
  FieldEvent_Wait(10);
  Actor_SetScript(27, (s32)gJupiterLighthouse_KarstLeavesScript);
  FieldEvent_Wait(110);
  Actor_SetScript(4, (s32)gJupiterLighthouse_KarstLeavesScript);
  FieldActor_RefreshSelectorById(26);
  Actor_SetScript(26, (s32)gJupiterLighthouse_KarstFliesOffScript);
  FieldActor_RefreshSelectorById(27);
  Actor_SetScript(27, (s32)gJupiterLighthouse_KarstFliesOffScript);
  FieldActor_RefreshSelectorById(4);
  Actor_SetAnimation(4, 1);
  FieldActor_StartRepeatedMotion(7, 2);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirection(28, FACING_NORTH, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  EventCamera_FollowActor(6, 1);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_Wait(10);
  ((struct Actor *)Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 30));
  ((struct Actor *)Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30));
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  ((struct Actor *)Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30));
  ((struct Actor *)Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30));
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 20);
  FieldEvent_Wait(10);
  Actor_SetSpeed(7, 1);
  Actor_SetScript(7, (s32)gJupiterLighthouse_KarstLeavesScript);
  FieldActor_RefreshSelectorById(7);
  Party_AddMember(7);
  EventCamera_FollowActor(4, 1);
  Actor_SetPosition(7, 0, 0);
  FieldEvent_Wait(20);
#if TLA_PATCH_1
  Event_ClearInvalidPackedValues();
#endif
  Actor_WalkToAndWait(4, 664, 70);
  Event_End();
}

/// Sets up Jupiter Lighthouse room M: objects 8 to 15 take mode 1, objects 24
/// and 25 stand at heights 0 and 32, the cells flags 514 and 2591 opened are
/// stamped again, and with 2591 set the arrival is chosen by flags 2595, 2593
/// and 2596 and the entrance.
s32 Scene_Initialize(void) {
  struct Actor *object;

  gFieldWork->transition = 516;
  HangingVine_Place(8, 1);
  HangingVine_Place(9, 1);
  HangingVine_Place(10, 1);
  HangingVine_Place(11, 1);
  HangingVine_Place(12, 1);
  HangingVine_Place(13, 1);
  HangingVine_Place(14, 1);
  HangingVine_Place(15, 1);
  Rope_Initialize();
  Rope_Place(0, 512, 21, 22);
  gWhirlwindRiding = 0;
  object = Actor_Get(24);
  object->motionFlags = 0;
  object->y = 0;
  object->terrainHeight = 0;
  object = Actor_Get(25);
  object->motionFlags = 0;
  object->y = px(32);
  object->terrainHeight = px(32);
  if (Event_GetFlag(FLAG_SCENE_TEMP_02)) {
    MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
    MapGrid_CopyCellAttributes(49, 45, 4, 1, 49, 44);
  }
  if (Event_GetFlag(2591)) {
    MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
    MapGrid_CopyCellAttributes(49, 45, 4, 1, 49, 44);
    Actor_SetPosition(23, 0, 0);
    if (Event_GetFlag(2595)) {
      if (!Event_GetFlag(2593)) {
        JupiterLighthouse_PlaceForTheFight();
        JupiterLighthouse_PlaceTheHurt();
      }
    } else if (!Event_GetFlag(2596)) {
      if (gMainRoutineWork.entrance == 3) {
        JupiterLighthouse_PlaceForTheFight();
        Event_SetFlag(2596);
        JupiterLighthouse_PartyArrivesAtTheFight(gMainRoutineWork.entrance ==
                                                 3);
      }
    } else {
      JupiterLighthouse_PlaceForTheFight();
      if (gMainRoutineWork.entrance == 5) {
        JupiterLighthouse_IsaacAndIvanAreHurt();
      } else if (gMainRoutineWork.entrance == 77) {
        JupiterLighthouse_IsaacAndIvanFall();
        Event_SetFlag(2595);
        Event_ClearFlag(2596);
      } else {
        JupiterLighthouse_StartTheFight();
      }
    }
  }
  Actor_Get(23)->motionFlags = 0;
  if (Event_GetFlag(2573))
    Actor_SetPosition(23, 0, 0);
  if (gMainRoutineWork.entrance == 99)
    JupiterLighthouse_IsaacAndIvanFall();
  return 0;
}

void LightSprites_InitC();
void LightSprites_Add();

/// The scene's map preparation.
s32 Scene_PrepareMap(void) {
  struct Actor *object;

  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(1);
  LightSprites_InitC();
  LightSprites_Add(20, 0);
  object = Actor_Get(20);
  if (Event_GetFlag(0xa0c)) {
    Actor_SetPosition(20, 1016, 248);
    MapGrid_CopyCellsLayered(104, 42, 5, 11, 61, 42);
    MapGrid_CopyCellsLayered(106, 52, 1, 1, 63, 84);
    FieldMapGrid_SetCellBehavior(px(1040), px(160), 2, 236);
  }
  object->motionFlags = 0;
  {
    s32 zero = 0;

    /* FAKEMATCH(barrier): the last call's zero is built before the position
     * loads. */
    __asm__("" : : "r"(zero));
    MapGrid_SetCellHeight(object->x, object->z, zero, -6);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }
extern u16 gPaletteCycleColors[];

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update.h"

void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void Whirlwind_Update(struct Actor *self);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

struct GustObject *Whirlwind_SpawnUpper(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(SE_222);
  position.x = source->x + Math_MulQ16(0x80000, Trig_Cos(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                   position.z));
  Import_ActorSelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Whirlwind_Update;
  object->speedLimit = px(3) / 2;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y, position.z - px(6), 0, 0, 0, 0x30000,
                    &options);
  return object;
}

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

extern s16 gLightSpritesLarge;

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#undef WORD_AT
#undef PLACE
#undef EVENT_WORK
#undef ACTOR_SET_MODE_AND_WAIT
extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
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
  Import_ActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
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
    Import_ActorSelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect/advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"

// clang-format off
u32 gJupiterLighthouse_KarstStepsUpScript[] = {
    1, 39, 1, 17,
};

u32 gJupiterLighthouse_AgatioStepsUpScript[] = {
    1, 39, 1, 17,
};

u32 gJupiterLighthouse_AgatioTurnsScript[] = {
    1, 39, 1, 17,
};

u32 gJupiterLighthouse_AgatioFacesSouthScript[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

u32 gJupiterLighthouse_KarstFacesEastScript[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_END,
};

u32 gJupiterLighthouse_IsaacFacesWestScript[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

u32 gJupiterLighthouse_IvanFacesNorthScript[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

u32 gJupiterLighthouse_FelixWaitsScript[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};

u32 gJupiterLighthouse_KradenWaitsScript[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1),
    SC_END,
};

u32 gJupiterLighthouse_PiersStepsUpScript[] = {                                                               
      SC_SYNC,    SC_MOVE_XZ(px(699), px(170)), SC_SYNC,                
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTHEAST), SC_ARG(1),              
      SC_END,                                                                  
  };

u32 gJupiterLighthouse_ShebaStepsUpScript[] = {                                                               
      SC_SYNC,    SC_MOVE_XZ(px(679), px(170)), SC_SYNC,                
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTHEAST), SC_ARG(1),              
      SC_END,                                                                  
  };

u32 gJupiterLighthouse_JennaStepsUpScript[] = {                                                               
      SC_SYNC,    SC_MOVE_XZ(px(659), px(170)), SC_SYNC,                
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTHEAST), SC_ARG(1),              
      SC_END,                                                                  
  };
// clang-format on

extern u32 gFieldEffect_LongScript[];
extern s32 gJupiterLighthouse_ShowGustScript[];

// clang-format off
LATE_DATA u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

LATE_DATA u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

LATE_DATA u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

LATE_DATA s32 gJupiterLighthouse_ShowGustScript[] = {
    SC_CALL(JupiterLighthouse_ClearScriptedActorSpriteFlags),
    SC_END,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseRoomM),         SCENE_EXIT(1, JupiterLighthouseRoom3, 1),    SCENE_EXIT(3, JupiterLighthouseRoom3, 3), 
      SCENE_EXIT(4, JupiterLighthouseRoom3, 4),  SCENE_EXIT(5, JupiterLighthouseRoom5, 5),    SCENE_EXIT(6, JupiterLighthouseRoom5, 6),  
      SCENE_EXIT(7, JupiterLighthouseRoom8, 7),   SCENE_EXIT(8, JupiterLighthouseRoom7, 8), SCENE_EXIT(9, JupiterLighthouseRoom4, 9), 
      SCENE_EXIT(10, JupiterLighthouseRoom4, 10), SCENE_EXITS_END,                                
  };

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1464, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1608, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 648, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 680, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 936, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 952, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 968, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 1144, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 1064, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 200, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 1016, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(1, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(2, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(3, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(4, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(33, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(23, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, (s32)gJupiterLighthouse_ShowGustScript, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
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
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RideUpperWhirlwind)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RideWhirlwind)},
    {TRIGGER_STEP, 32, 0x202, EVENT(JupiterLighthouse_OpenFloor)},
    {TRIGGER_STEP, 33, IF_FLAG_SET(0x202), EVENT(JupiterLighthouse_CloseFloor)},
    {TRIGGER_TALK, 0, IF_FLAG_SET(0xa23), EVENT(JupiterLighthouse_TalkToReunitedParty)},
    {TRIGGER_TALK, 2, IF_FLAG_SET(0xa23), EVENT(JupiterLighthouse_TalkToReunitedParty)},
    {TRIGGER_TALK, 6, IF_FLAG_SET(0xa23), EVENT(JupiterLighthouse_TalkToReunitedParty)},
    {TRIGGER_TALK, 5, IF_FLAG_SET(0xa23), EVENT(JupiterLighthouse_TalkToReunitedParty)},
    {TRIGGER_TALK, 28, IF_FLAG_SET(0xa23), EVENT(JupiterLighthouse_TalkToReunitedParty)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 20, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {TRIGGER_PUSH, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {TRIGGER_PUSHED, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 21, 0x200, 0},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Whirlwind_CarryLeader)},
    {TRIGGER_TABLE_END},
};

LATE_DATA u32 gJupiterLighthouse_GaretHangsScript[] = {
#if !TLA_PATCH_1
    39, 14, 22, 26, 0, 23,
#else
    39, 14, 22, 26, 0, 22,
#endif
    6, 0xc0000, 17,
};

LATE_DATA u32 gJupiterLighthouse_MiaScript[] = {
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(9),
    SC_WAIT(120),
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(120),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_IsaacFightsScript[] = {
    SC_ARG(2),
    SC_CALL(JupiterLighthouse_ClearActionBit),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(60),
    SC_MOVE_XZ(px(737), px(202)),
    SC_ARG(2),
    SC_WAIT(4),
    SC_MOVE_XZ(px(776), px(202)),
    SC_CALL(JupiterLighthouse_PlayRandomBlowSound),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_KarstFightsScript[] = {
    SC_ARG(2),
    SC_CALL(JupiterLighthouse_ClearActionBit),
    SC_SYNC,
    SC_ARG(9),
    SC_WAIT(60),
    SC_MOVE_XZ(px(776), px(202)),
    SC_ARG(2),
    SC_WAIT(4),
    SC_MOVE_XZ(px(737), px(202)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_IvanFightsScript[] = {
    SC_CALL(JupiterLighthouse_ClearActionBit),
    SC_MOVE_XZ(px(731), px(213)),
    SC_ARG(2),
    SC_WAIT(4),
    SC_MOVE_XZ(px(731), px(232)),
    SC_CALL(JupiterLighthouse_PlayRandomBlowSound),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(53),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_AgatioFightsScript[] = {
    SC_ARG(2),
    SC_CALL(JupiterLighthouse_ClearActionBit),
    SC_MOVE_XZ(px(731), px(232)),
    SC_WAIT(4),
    SC_MOVE_XZ(px(731), px(213)),
    SC_SYNC,
    SC_ARG(8),
    SC_WAIT(53),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA u32 gJupiterLighthouse_FelixLooksAroundScript[] = {                                                               
      SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(20),                        
      SC_SET(SW_FACING, FACING_SOUTHWEST), SC_WAIT(20),                        
      SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(20),                        
      SC_SET(SW_FACING, FACING_SOUTHEAST), SC_WAIT(20),                        
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

LATE_DATA u32 gJupiterLighthouse_FelixArrivesWestScript[] = {
    SC_ARG(5),
    SC_MOVE_BY(0, 0, px(32)),
    SC_SYNC,
    SC_MOVE_XZ(px(744), px(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_FelixArrivesEastScript[] = {
    SC_ARG(5),
    SC_MOVE_BY(px(-8), 0, px(32)),
    SC_SYNC,
    SC_MOVE_XZ(px(896), px(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_KradenArrivesWestScript[] = {                                                               
      SC_ARG(2),                                                              
      SC_MOVE_XZ(px(740), px(312)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),                           
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_KradenArrivesEastScript[] = {                                                               
      SC_ARG(2),                                                              
      SC_MOVE_XZ(px(900), px(312)),                                     
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),                       
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_PiersArrivesWestScript[] = {                                                               
      SC_ARG(5),                                                              
      SC_MOVE_XZ(px(724), px(312)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),                           
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_PiersArrivesEastScript[] = {                                                               
      SC_ARG(5),                                                              
      SC_MOVE_XZ(px(916), px(312)),                                     
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),                       
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_JennaArrivesWestScript[] = {
    SC_ARG(5),
    SC_MOVE_XZ(px(677), px(287)),
    SC_SYNC,
    SC_ARG(5),
    SC_MOVE_XZ(px(740), px(344)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_JennaArrivesEastScript[] = {
    SC_ARG(5),
    SC_MOVE_XZ(px(950), px(287)),
    SC_SYNC,
    SC_ARG(5),
    SC_MOVE_XZ(px(900), px(344)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_ShebaArrivesWestScript[] = {                                                               
      SC_ARG(5),                                                              
      SC_MOVE_XZ(px(708), px(328)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),                           
      SC_END,                                                                  
  };

LATE_DATA u32 gJupiterLighthouse_ShebaArrivesEastScript[] = {
    SC_ARG(5),
    SC_MOVE_BY(px(-8), 0, 0),
    SC_SYNC,
    SC_ARG(5),
    SC_MOVE_XZ(px(924), px(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_PiersStepsBackScript[] = {
    1, 38,
};

LATE_DATA u32 gJupiterLighthouse_KarstLeavesScript[] = {
    SC_ARG(2),
    SC_MOVE_XZ(px(706), px(172)),
    SC_SYNC,
    SC_MOVE_XZ(px(682), px(151)),
    SC_SYNC,
    SC_MOVE_XZ(px(682), px(120)),
    SC_SYNC,
    SC_MOVE_XZ(px(664), px(106)),
    SC_SYNC,
    SC_MOVE_XZ(px(664), px(100)),
    SC_SYNC,
    SC_END,
};

LATE_DATA u32 gJupiterLighthouse_KarstFliesOffScript[] = {
    SC_MOVE_XZ(px(664), px(80)),
    SC_SYNC,
    SC_FREE,
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
