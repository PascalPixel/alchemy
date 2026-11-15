/// @file
/// The Apojii Islands, the village at the edge of the world south of the
/// Eastern Sea, in three rooms of this overlay: the village, whose blue Aqua
/// Stone Douse wakes (ApojiiIslands_DouseAquaStone) so the Moai of Aqua Rock
/// move aside, the shore beyond it where Sand reaches past the rocks, and the
/// cliff where the Jupiter Djinni Haze waits (ApojiiIslands_HazeJoinsParty).
/// Its sea, falls and stone cycle their colours.
#include "flags.h"
#include "djinn_ids.h"
#include "field/field_sequence.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "scene/dust.h"

#define FLAG_APOJII_AQUA_STONE_DOUSED 0x95f
/// Set while the father who wished on the rock sees it answer Douse, so that
/// the scene finishes his words if it is left before he has spoken.
#define FLAG_FATHER_SEES_THE_STONE 0x95e
#include "types.h"
#include "system/io_write.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"

/// The village by the Aqua Stone (gApojiiIslands_VillagePlacements).
enum ApojiiIslandsVillageActorId {
  /// "This is Apojii, the easternmost island in the Eastern Sea!"
  ACTOR_WELCOMING_YOUNG_MAN = ACTOR_FIRST_PLACED,
  /// "Whatever you do, don't sail out to the eastern edge of the sea..."
  ACTOR_WOMAN_WARNING_OF_THE_EDGE,
  /// "The huge waterfall at the edge of the world is known as Gaia Falls."
  ACTOR_MAN_ON_GAIA_FALLS,
  /// "On Aqua Island, you can see the guardian moai."
  ACTOR_WOMAN_ON_THE_MOAI,
  /// "Have you any idea what it's like living on the edge of the world?"
  ACTOR_OLD_MAN_AT_THE_EDGE_OF_THE_WORLD,
  /// "I don't know what brings you to Apojii, but you'd better not be
  /// planning anything stupid."
  ACTOR_SUSPICIOUS_OLD_WOMAN,
  /// "When they come of age, all the men of Apojii go to Aqua Island. Will I,
  /// too?"
  ACTOR_BOY_COMING_OF_AGE,
  /// "Do you think if you fall off of Gaia Falls that you keep on falling
  /// forever and ever?"
  ACTOR_GIRL_ON_GAIA_FALLS,
  /// The father who wished on the rock: "What... What just happened? That
  /// rock couldn't have heard my wishes..."
  ACTOR_FATHER,
  /// The blue tear-shaped Aqua Stone Douse wakes.
  ACTOR_AQUA_STONE
};

/// The houses (gApojiiIslands_ShopsPlacements).
enum ApojiiIslandsHousesActorId {
  /// "There's this guy in town who spends his time at the spring, staring at
  /// the Aqua Stone."
  ACTOR_WOMAN_ON_THE_STARING_MAN = ACTOR_FIRST_PLACED,
  /// His son (ApojiiIslands_TalkToSon).
  ACTOR_SON,
  /// "I absolutely refuse to let my son swim in the ocean until he is an
  /// adult."
  ACTOR_STRICT_FATHER,
  /// "The currents around Apojii are just too strong to let children swim on
  /// their own."
  ACTOR_CAUTIOUS_MOTHER,
  /// "I'm not allowed to go into the ocean."
  ACTOR_BOY_KEPT_FROM_THE_OCEAN,
  /// "Hey, mister! Did you know that all the kids are calling you their
  /// hero?"
  ACTOR_GIRL_CALLING_FELIX_A_HERO,
  /// "That old Aqua Stone is supposed to be the key to getting onto Aqua
  /// Island."
  ACTOR_OLD_MAN_ON_THE_AQUA_STONE,
  /// "He's too old to try now, but my husband used to dream of scaling Aqua
  /// Rock."
  ACTOR_OLD_WOMAN_ON_HER_HUSBANDS_DREAM,
  ACTOR_MERCHANT,
  ACTOR_INNKEEPER,
  /// Who asks after the warriors (ApojiiIslands_AskAboutWarriors).
  ACTOR_INN_WORKER,
  /// "The temperature of the ocean has been rising."
  ACTOR_OLD_MAN_ON_THE_WARMING_SEA
};

/// Haze's islet (gApojiiIslands_HazePlacements).
enum ApojiiIslandsHazeActorId {
  /// The tightrope's posts.
  ///@{
  ACTOR_TIGHTROPE_POST_A = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPE_POST_B,
  ///@}
  ACTOR_HAZE
};

#include "system/iwram_call.h"
#include "system/io_reg.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "text/msg_ids.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Sprite_StartAnimation(void *animation, s32 value);
u8 *Sprite_AddAnimation(void *animation, s32 id);
void Irq_SetHandler(s32 a, s32 b, void *callback);
void Actor_StartScript(struct Actor *object, const void *data);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);
void Field_TryJumpForward(void);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Shop_Run(s32 a, s32 b);
void Inn_CheckIn(s32 a, s32 b);

void ApojiiIslands_CycleFallsColors(void);
void ApojiiIslands_CycleSeaColors(void);
void ApojiiIslands_CycleAquaStoneColors(void);
void ApojiiIslands_StopFlashing(struct Actor *object);
void Actor_FlickerChild(struct Actor *object);
void ApojiiIslands_FatherSeesTheStone(void);
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Scroll_StartHBlank(void);
void ApojiiIslands_InitializeHaze(void);
void ApojiiIslands_LeaveQuietly(void);
void ApojiiIslands_SpawnSplash(struct Actor *object);
s32 Actor_EmitMist(struct Actor *object);
void Leader_RiseFromWater(void);
void ApojiiIslands_DrawCellMarkers(void);
void ApojiiIslands_StartCellMarkers(void);
void ApojiiIslands_AddCellMarkers(u32 base, s32 a, s32 b);
void GroundParticles_Draw(void);
void ApojiiIslands_StartSparkles(void);
void ApojiiIslands_SetSparkleAnchor(s32 id);

extern s32 gApojiiIslands_SplitLine;
extern u16 gApojiiIslands_UpperScrollX;
extern u16 gApojiiIslands_LowerScrollX;
struct CellMarker {
  s32 x;
  s32 y;
  s32 z;
  s32 rest;
  u16 kind;
  u16 state;
  u32 oam[3];
};

struct CellMarkers {
  u16 block;
  u16 count;
  struct CellMarker markers[128];
};

extern struct CellMarkers gApojiiIslands_CellMarkers;
extern u32 gApojiiIslands_SplashScript[];
extern u32 gWaterDropScript[];

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  FieldCamera_MoveWithinMapBounds(x, a, z, b);
}

#include "shared/tables.h"
#include "scene_ids.h"
#include "field/call.h"
#include "system/io_write_queue.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/flag_call.h"
#include "field/ground_mask.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "scene/event_layer.h"
#include "system/task.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "version.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern struct ActorPlacement gApojiiIslands_Placements[];
extern struct ActorPlacement gApojiiIslands_VillagePlacements[];
extern struct ActorPlacement gApojiiIslands_ShopsPlacements[];
extern struct ActorPlacement gApojiiIslands_HazePlacements[];
extern s32 gApojiiIslands_StoneCycleFrame[];

extern struct Trigger gApojiiIslands_Triggers[];
extern struct Trigger gApojiiIslands_VillageTriggers[];
extern struct Trigger gApojiiIslands_ShopsTriggers[];
extern struct Trigger gApojiiIslands_HazeTriggers[];
void ApojiiIslands_SinkIntoSand(void);
void ApojiiIslands_RiseFromSand(void);
void ApojiiIslands_ResumeSeaColors(void);
void ApojiiIslands_ClearShopsBushes(void);
void ApojiiIslands_ClearShopsSmallBush(void);
void ApojiiIslands_EnterDoor(void);
void ApojiiIslands_JumpDownLedge(void);
void ApojiiIslands_JumpForward(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gApojiiIslands_PaceScript[];

extern u16 gApojiiIslands_StoneColors[16];

extern u8 Data_0202c001[];

void ApojiiIslands_HazeJoinsParty(void) {
  Djinn_JoinParty(10, DJINNI_HAZE / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_HAZE));
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_APOJII_ISLANDS)
    return gApojiiIslands_VillagePlacements;
  if (scene == SCENE_APOJII_ISLANDS_SHOPS)
    return gApojiiIslands_ShopsPlacements;
  if (scene == SCENE_APOJII_ISLANDS_HAZE)
    return gApojiiIslands_HazePlacements;
  return gApojiiIslands_Placements;
}

/// Sand takes the leader into the sand (and ApojiiIslands_RiseFromSand back
/// up).
void ApojiiIslands_SinkIntoSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 2;
}

void ApojiiIslands_RiseFromSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 0;
}

extern s32 gApojiiIslands_FallsCycleFrame;
extern u16 gApojiiIslands_FallsColors[];
extern u16 gApojiiIslands_FallsCycled[];
extern u16 gApojiiIslands_SprayColors[];
extern u16 gApojiiIslands_SprayCycled[];

void ApojiiIslands_CycleFallsColors(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 phase, k, offset;
  u16 *firstCopy, *secondCopy;

  if (work->psynergy != 0)
    return;
  if (work->revealActive != 0)
    return;
  if (gApojiiIslands_FallsCycleFrame == -1) {
    Iwram_CopyWords(gApojiiIslands_FallsColors, (void *)0x05000060, 32);
    Iwram_CopyWordsEntry(gApojiiIslands_FallsCycled, gApojiiIslands_FallsColors,
                         32);
    Iwram_CopyWordsEntry(gApojiiIslands_SprayColors, (void *)0x05000080, 32);
    Iwram_CopyWordsEntry(gApojiiIslands_SprayCycled, gApojiiIslands_SprayColors,
                         32);
    gApojiiIslands_FallsCycleFrame = 0;
  }
  phase = gApojiiIslands_FallsCycleFrame / 8 % 4;
  k = 0;
  firstCopy = gApojiiIslands_FallsCycled;
  secondCopy = gApojiiIslands_SprayCycled;
  for (; k <= 3; k++) {
    offset = (phase + k) % 4 * 2 + 24;
    *(u16 *)((u8 *)firstCopy + offset) = gApojiiIslands_FallsColors[12 + k];
    *(u16 *)((u8 *)secondCopy + offset) = gApojiiIslands_SprayColors[12 + k];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(3, 0), (u32)firstCopy, 0x84000008);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(4, 0), (u32)secondCopy, 0x84000008);
  gApojiiIslands_FallsCycleFrame++;
}

extern s32 gApojiiIslands_SeaCycleFrame;
extern u16 gApojiiIslands_SeaColors[];
extern u16 gApojiiIslands_SeaCycled[];
extern u16 gApojiiIslands_ShoreColors[];
extern u16 gApojiiIslands_ShoreCycled[];

/// The sea's and shore's colours cycle every eighth frame.
void ApojiiIslands_CycleSeaColors(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 busy, phase, k, j;

  if (work->psynergy != 0)
    return;
  busy = work->revealActive;
  if (busy != 0)
    return;
  if (gApojiiIslands_SeaCycleFrame == -1) {
    Iwram_CopyWords((s32)gApojiiIslands_SeaColors, 0x05000060, 32);
    Iwram_CopyWordsEntry(gApojiiIslands_SeaCycled, gApojiiIslands_SeaColors,
                         32);
    Iwram_CopyWordsEntry(gApojiiIslands_ShoreColors, (void *)0x050001a0, 32);
    Iwram_CopyWordsEntry(gApojiiIslands_ShoreCycled, gApojiiIslands_ShoreColors,
                         32);
    gApojiiIslands_SeaCycleFrame = busy;
  }
  phase = gApojiiIslands_SeaCycleFrame / 8 % 4;
  for (k = 0; k <= 3; k++) {
    /* FAKEMATCH(dead): the phase and the first colour in a once-only block. */
    do {
      j = (phase + k) % 4;
      gApojiiIslands_SeaCycled[12 + j] = gApojiiIslands_SeaColors[12 + k];
    } while (0);
    gApojiiIslands_ShoreCycled[12 + j] = gApojiiIslands_ShoreColors[12 + k];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(3, 0), (u32)gApojiiIslands_SeaCycled,
                      0x84000008);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(13, 0),
                      (u32)gApojiiIslands_ShoreCycled, 0x84000008);
  gApojiiIslands_SeaCycleFrame++;
}

void ApojiiIslands_PauseSeaColors(void) {
  Task_SuspendAll(ApojiiIslands_CycleSeaColors);
  System_Wait(3);
}

void ApojiiIslands_ResumeSeaColors(void) {
  Task_ResumeAll(ApojiiIslands_CycleSeaColors);
  System_Wait(3);
}

extern u16 gApojiiIslands_StoneCycled[16];

s32 __modsi3(s32 a, s32 b);

/// The woken Aqua Stone's colours cycle.
void ApojiiIslands_CycleAquaStoneColors(void) {
  s32 base;
  s32 i;

  if (gApojiiIslands_StoneCycleFrame[0] == -1) {
    Iwram_CopyWordsEntry(gApojiiIslands_StoneColors, (void *)0x05000080, 32);
    gApojiiIslands_StoneCycleFrame[0] = 0;
  }
  base = (gApojiiIslands_StoneCycleFrame[0] / 2) % 15;
  for (i = 0; i < 15; i++)
    gApojiiIslands_StoneCycled[(base + i) % 15 + 1] =
        gApojiiIslands_StoneColors[i + 1];
  IO_WRITE_QUEUE_PUSH((u32)&OBJ_PLTT_COLOR(14, 0),
                      (u32)gApojiiIslands_StoneCycled, 0x84000008);
  gApojiiIslands_StoneCycleFrame[0]++;
}

void ApojiiIslands_StopFlashing(struct Actor *object) {
  Actor_SetSpriteColorMode(object, 0);
  object->update = 0;
}

#include "shared/actor_flicker_child.h"

/* VIEW(s07a4): routine PaletteFade_SetTarget as s32 PaletteFade_SetTarget(s32
 * a, s32 b); event.h has void PaletteFade_SetTarget(s32 value, s32 mode); tried
 * as the definition's void PaletteFade_SetTarget(s32 value, s32 mode), which
 * the object already declares, ApojiiIslands_DouseAquaStone moves: 1 of 188
 * instructions in tla-ja (apojii_islands.c). */
extern s32 BattleEffect_ApplyColorToTargetBuffer__s07a4(s32 a, s32 b) __asm__(
    "PaletteFade_SetTarget");

static inline void ApojiiIslands_Flash(s32 strength, s32 mode) {
  BattleEffect_ApplyColorToTargetBuffer__s07a4(strength, mode);
}

/// An Apojii Islands scene inside an event: the actor takes Actor_FlickerChild
/// as its update, cues 78 and 21 play, a sparkle setup
/// (ApojiiIslands_StartSparkles) runs and four flashes of random strength
/// follow; then the screen blends in over six steps while the palette cycle
/// gives way to ApojiiIslands_CycleAquaStoneColors and the cell markers are
/// rebuilt (ApojiiIslands_AddCellMarkers). Flags 0x95f and 0x95e are set and
/// the camera pans through three points before cue 77.
void ApojiiIslands_DouseAquaStone(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->update = Actor_FlickerChild;
  Sound_Play(SOUND_FADE_OUT_BGM);
  wait(30);
  Sound_Play(MUS_AQUA_ROCK);
  ApojiiIslands_StartSparkles();
  ApojiiIslands_SetSparkleAnchor(ACTOR_AQUA_STONE);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  i = 3;
  /* FAKEMATCH(wrapper): both flashes forwarded through ApojiiIslands_Flash. */
  /* FAKEMATCH(signature): PaletteFade_SetTarget declared
   * returning s32, though its target returns nothing. */
flash:
  ApojiiIslands_Flash(Random16() & 0x7fff, 0);
  PaletteFade_StartInterpolation(15);
  wait(10);
  ApojiiIslands_Flash(0x10000, 0);
  PaletteFade_StartInterpolation(10);
  i--;
  wait(10);
  if (i >= 0)
    goto flash;
  ObjectTable_Snapshot();
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 |
                          BLDCNT_TGT2_BG3,
                      0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(0, 16), 0x20000);
  wait(1);
  Task_Remove(ApojiiIslands_CycleSeaColors);
  Task_Add(ApojiiIslands_CycleAquaStoneColors, TASK_PASS_VBLANK);
  ApojiiIslands_StartCellMarkers();
  ApojiiIslands_AddCellMarkers(33, 0, 0);
  i = 1;
blend:
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(i, BLEND_WEIGHT_MAX),
                      0x20000);
  wait(4);
  i++;
  if (i <= 6)
    goto blend;
  GameFlag_Set(FLAG_APOJII_AQUA_STONE_DOUSED);
  GameFlag_Set(FLAG_FATHER_SEES_THE_STONE);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(360, CAMERA_KEEP, 328, 1);
  Actor_ShowEmoteById(ACTOR_FATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(440, CAMERA_KEEP, 376, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(472, CAMERA_KEEP, 408, 1);
  Import_EventSetExitRequest(77);
}

/// "What... What just happened? That rock couldn't have heard my wishes..."
void ApojiiIslands_FatherSeesTheStone(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(ACTOR_AQUA_STONE)->update =
      (void (*)(void))ApojiiIslands_StopFlashing;
  FieldEvent_SetMessageId((s32)&MsgWhatWhatJust);
  Actor_FaceActor(gMainRoutineWork.leaderActorId, ACTOR_FATHER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_FaceDirection(ACTOR_FATHER, FACING_EAST, 0);
  Actor_SetAnimationAndWait(ACTOR_FATHER, 3);
  Actor_FaceDirection(ACTOR_FATHER, FACING_WEST, 0);
  Actor_SetAnimationAndWait(ACTOR_FATHER, 3);
  Actor_FaceDirection(ACTOR_FATHER, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(ACTOR_FATHER, 3);
  FieldActor_ShowEmote(ACTOR_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  // "What... What just happened?"
  Event_ShowMessage(ACTOR_FATHER, 0);
  FieldActor_Jump(ACTOR_FATHER, 2, 25);
  // "That rock couldn't have heard my wishes... That's impossible!"
  Event_ShowMessage(ACTOR_FATHER, 0);
  Actor_RunRepeatedMotion(ACTOR_FATHER, 1);
  // "It's got to be."
  Event_ShowMessage(ACTOR_FATHER, 0);
  Actor_SetAnimationAndWait(ACTOR_FATHER, 3);
  // "Well, OK then... Maybe we should try putting some more of our wishes..."
  Event_ShowMessage(ACTOR_FATHER, 0);
  Event_End();
  Event_ClearFlag(FLAG_FATHER_SEES_THE_STONE);
}

void ApojiiIslands_ClearFirstBush(void) {
  FieldTileCopies_CopyTouchedCells(68, 38, 39, 24, 68, 4, 1);
}

#define DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP(name, n1, n2, n3, n4, n5,    \
                                                  n6, n7, n8, n9, n10)         \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);          \
                                                                               \
    FieldTileCopies_CopyTouchedCells(n1, 38, n2, n3, n1, n4, 1);               \
    if (Event_GetFlag(n5) == 0) {                                              \
      Actor_SetPosition(n6, n7, n8);                                           \
      if (FIELD_CELL(leader->x.fixed) == n9 &&                                 \
          FIELD_CELL(leader->z.fixed) == n10) {                                \
        Event_Start();                                                         \
        FieldEvent_ResolvePendingActions(0);                                   \
        Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,                \
                                ATTACHED_EFFECT_IN_FRONT |                     \
                                    ATTACHED_EFFECT_ONCE);                     \
        Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 16, 0);     \
        leader->velocityY = toFIXED(6);                                        \
        FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);            \
        Event_End();                                                           \
      }                                                                        \
    }                                                                          \
  }

DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP(ApojiiIslands_ClearSecondBush, 68, 38,
                                          21, 4, 0xf9b, 65, 376, 376, 23, 23)

DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP(ApojiiIslands_ClearThirdBush, 68, 38,
                                          21, 4, 0xf9a, 64, 600, 408, 37, 25)

void ApojiiIslands_ClearShopsBushes(void) {
  FieldTileCopies_CopyTouchedCells(67, 38, 11, 6, 67, 8, 1);
}

DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP(ApojiiIslands_ClearShopsSmallBush, 67,
                                          11, 6, 8, 0xf9e, 64, 200, 152, 12, 9)

/// Stepping onto a door: the leader walks through it and the scene leaves by
/// the exit of its number.
void ApojiiIslands_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -6);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

s32 ApojiiIslands_TalkToSon(void) {
  u16 facing;

  facing = (s16)((Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) &
                 0xc000);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    Actor_FaceActor(ACTOR_SON, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)&MsgWhyDoesMy);
    // "Why does my father keep staring at that rock every day?"
    Event_ShowMessage(ACTOR_SON, 0);
    Actor_FaceDirection(ACTOR_SON, FACING_SOUTHEAST, 0);
  }
  Event_End();
  return 0;
}

s32 ApojiiIslands_MindReadSon(void) {
  u16 facing;

  facing = (s16)((Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) &
                 0xc000);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    FieldEvent_SetMessageId((s32)&MsgICanTEver);
    // "I can't ever tell what kind of mood Mom and Dad are in. At least..."
    Event_ShowMessage(ACTOR_SON, 0);
  }
  Event_End();
  return 0;
}

void ApojiiIslands_JumpDownLedge(void) {
  Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 1, 5);
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_APOJII_ISLANDS)
    return gApojiiIslands_VillageTriggers;
  if (scene == SCENE_APOJII_ISLANDS_SHOPS)
    return gApojiiIslands_ShopsTriggers;
  if (scene == SCENE_APOJII_ISLANDS_HAZE)
    return gApojiiIslands_HazeTriggers;
  return gApojiiIslands_Triggers;
}

/// The Apojii Islands' shops, as Shop_Run numbers them.
enum { SHOP_APOJII = 23 };

/// The Apojii Islands' inn, as Inn_CheckIn numbers the inns.
enum { APOJII_INN = 8 };

void ApojiiIslands_TalkToInnkeeper(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Inn_CheckIn(APOJII_INN, actor);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgWowYouLook);
  // "Wow! You look just like the guy who showed up in the fancy boat. He..."
  Event_ShowMessage(actor, 0);
  Event_End();
}

void ApojiiIslands_AskAboutWarriors(s32 actor) {
  s32 msg;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)&MsgYouGuysAren;
  FieldEvent_SetMessageId(msg);
  // "You guys aren't the warriors who came in that boat, are you?"
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
  Event_End();
}

void ApojiiIslands_TalkToMerchant(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Shop_Run(SHOP_APOJII, actor);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgItDBe);
  // "It'd be nice if we could import some items that our visitors would like."
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Scroll_HBlank(void) {
  u32 value;

  if (REG_VCOUNT < gApojiiIslands_SplitLine) {
    value = gApojiiIslands_UpperScrollX;
    REG_BG1HOFS = value;
    REG_BLDCNT = 0;
  } else {
    value = gApojiiIslands_LowerScrollX;
    Io_Write16(value, &REG_BG1HOFS);
    REG_BLDALPHA = BLEND_ALPHA(12, 16);
    REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  }
}

void Layer_TrackScroll(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[2];
  s16 y = FIXEDtoINT(layer->y);

  gApojiiIslands_SplitLine = 1448 - y;
  gApojiiIslands_UpperScrollX = FIXEDtoINT(layer->x);
  gApojiiIslands_LowerScrollX = (s16)FIXEDtoINT(layer->x) - (gFrameCount >> 2);
}

#include "shared/scroll_start_hblank.h"

/// The overlay's entry: sets the map mode and screen effect 0x202, then starts
/// each room's own setup.
s32 Scene_Initialize(void) {
  s32 *effect;
  s16 scene;
  struct MainRoutineWork *party;

  Map_SetUpperLevelWading(Ram_HeapSlots->mapGrid);
  effect = &gFieldWork->transition;
  *effect = 0x202;
  party = &gMainRoutineWork;
  scene = party->next;
  if (scene == SCENE_APOJII_ISLANDS_HAZE) {
    ApojiiIslands_InitializeHaze();
  } else if (scene == SCENE_APOJII_ISLANDS) {
    FieldTask_AddFrame(ApojiiIslands_CycleSeaColors, TASK_PASS_VBLANK);
    if (Event_GetFlag(FLAG_FATHER_SEES_THE_STONE))
      ApojiiIslands_FatherSeesTheStone();
  } else if (scene == SCENE_APOJII_ISLANDS_SHOPS) {
    *effect = 0x100;
    if (party->entrance == 6)
      ApojiiIslands_LeaveQuietly();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void ApojiiIslands_InitializeHaze(void) {
  Task_AddFrame(ApojiiIslands_CycleFallsColors, TASK_PASS_VBLANK);
  Rope_Initialize();
  Rope_Place(0, 0x200, 8, 9);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Scroll_StartHBlank();
  if (gMainRoutineWork.entrance == 3)
    Leader_RiseFromWater();
  if (gMainRoutineWork.entrance == 4)
    Leader_RiseFromWater();
  if (gMainRoutineWork.entrance == 5)
    Leader_RiseFromWater();
}

void ApojiiIslands_LeaveQuietly(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
}

void ApojiiIslands_JumpForward(void) { Field_TryJumpForward(); }

void ApojiiIslands_SpawnSplash(struct Actor *object) {
  struct Actor *child;
  struct FieldSprite *parent;
  struct FieldSprite *sprite;

  child = (FieldActor_Create(14, object->x.fixed, object->y.fixed + toFIXED(4),
                             object->z.fixed + toFIXED(4)));
  parent = object->sprite;
  if (child != NULL) {
    sprite = child->sprite;
    child->groundHeight = object->groundHeight;
    Actor_StartScript(child, gApojiiIslands_SplashScript);
    child->motionFlags = 0;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = parent->priority;
    }
  }
}

#include "shared/actor_emit_water_drop.h"

/// In a stream the current carries the leader downstream, splashing.
void ApojiiIslands_FlowWithStream(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 owner = gMainRoutineWork.leaderActorId;
  struct Actor *anchor = work->camera;
  struct Actor *leader = Actor_Get(owner);
  s32 position[3];
  s32 result;

  if (gMainRoutineWork.next != SCENE_AQUA_ROCK_MOAI ||
      leader->groundHeight != 0) {
    position[0] = leader->x.fixed;
    position[1] = leader->y.fixed;
    position[2] = leader->z.fixed + toFIXED(2);
    result = MapGrid_CheckCollision(leader, position);
    if ((gFrameCount & 4) == 0)
      ApojiiIslands_SpawnSplash(leader);
    if (result < 0) {
      Actor_SetAttachedEffect(owner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      FieldActor_SetMoveTargetFX(leader, leader->x.fixed, leader->y.fixed,
                                 leader->z.fixed + toFIXED(8));
      Actor_SelectAnimation(leader, 49);
      FieldActor_WaitForMove(leader);
      do {
        System_Wait(1);
      } while (leader->y.fixed != leader->groundHeight);
      ApojiiIslands_SpawnSplash(leader);
      Actor_SelectAnimation(leader, 49);
      System_Wait(3);
    } else {
      position[0] = leader->x.fixed;
      position[1] = leader->y.fixed;
      position[2] = leader->z.fixed + toFIXED(8);
      result = MapGrid_CheckCollision(leader, position);
      if (result <= 0) {
        position[0] = leader->x.fixed + (toFIXED(57) / 10);
        position[1] = leader->y.fixed;
        position[2] = leader->z.fixed + (toFIXED(57) / 10);
        result = MapGrid_CheckCollision(leader, position);
        if (result <= 0) {
          position[0] = leader->x.fixed - (toFIXED(57) / 10);
          position[1] = leader->y.fixed;
          position[2] = leader->z.fixed + (toFIXED(57) / 10);
          result = MapGrid_CheckCollision(leader, position);
          if (result <= 0) {
            anchor->z.fixed += toFIXED(2);
            leader->z.fixed += toFIXED(2);
            leader->facing = FACING_SOUTH;
          }
        }
      }
    }
  }
}

#include "shared/actor_show_parts.h"

/// The leader spins up and away to `entrance`.
void Leader_SpinAway(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Event_PlaySound(SE_SPLASH_SMALL);
  Actor_SelectAnimation(leader, 18);
  Event_PlaySound(SE_SPIN_AWAY);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y.fixed -= toFIXED(1) / 4;
    angle = leader->facing;
    leader->facing = angle + 0x1000;
    if ((i & 7) == 0)
      ApojiiIslands_SpawnSplash(Actor_Get(15));
    System_Wait(1);
  }
  Event_End();
  FieldEvent_SetExitRequest(entrance);
}

// Every other frame, emits particle 7 (5 on every other such frame) from the
// object, 26 pixels up, swaying with the frame count and drifting by a random
// amount.
#include "shared/actor_emit_mist.h"

#include "shared/leader_sink_away.h"

#include "shared/leader_rise_from_water.h"

#define COPY_PICTURE(from, to)                                                 \
  Dma_Set((void *)(from), (void *)(offset + (to)), DMA_COPY_WORDS(16), REG_DMA3)

void ApojiiIslands_DrawCellMarkers(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  // DEAD: viewY, subtracted from both height and z, cancels in dy; the ROM
  // keeps its load and its store to the stack.
  s32 viewY = map->origin[1];
  struct CellMarkers *table = &gApojiiIslands_CellMarkers;
  struct VramCacheEntry *entry = &gVramCacheEntries[table->block];
  u32 tile = entry->offset >> 5;
  u32 offset = entry->offset;
  struct CellMarker *record;
  s32 i;

  COPY_PICTURE(0x0600f000, 0x06010000);
  COPY_PICTURE(0x0600f400, 0x06010040);
  COPY_PICTURE(0x0600f040, 0x06010080);
  COPY_PICTURE(0x0600f440, 0x060100c0);
  COPY_PICTURE(0x0600f080, 0x06010100);
  COPY_PICTURE(0x0600f480, 0x06010140);
  COPY_PICTURE(0x0600f080, 0x06010180);
  COPY_PICTURE(0x0600f480, 0x060101c0);
  record = table->markers;
  for (i = 127; i >= 0; i--, record++) {
    s32 dx, dy, height, z, palette, flip, kind;
    s32 x, y, left, top, picture;
    u32 clear;
    u32 *attr;

    if (record->state == 0)
      continue;
    dx = record->x - viewX;
    height = record->y - viewY;
    z = record->z - viewZ - viewY;
    dy = z - height;
    if (record->y == record->rest && record->state != 2)
      palette = 0xec00;
    else
      palette = 0xe800;
    y = dy >> 16;
    clear = 0;
    x = dx >> 16;
    left = x - 8;
    top = y - 8;
    if ((u32)(x + 7) > 334)
      continue;
    if ((u32)(y + 39) > 286)
      continue;
    left &= 0x1ff;
    /* FAKEMATCH(dead): a self-update of Ram_HeapSlots->mapGrid after left's
     * mask. */
    Ram_HeapSlots->mapGrid += 0;
    top &= 255;
    attr = record->oam;
    kind = record->kind;
    picture = kind * 4;
    flip = 0;
    if (kind == 2)
      flip = 0x10000000;
    *attr++ = clear;
    *attr++ = top | (left << 16) | flip | 0x40000400;
    *attr = palette | (tile + picture);
    Oam_AddSpriteAtDepth(record->oam, 0);
  }
}

/// Clears the effect table, takes a free VRAM entry for it with a 0x200-byte
/// block and no source, then schedules ApojiiIslands_DrawCellMarkers.
void ApojiiIslands_StartCellMarkers(void) {
  Iwram_ClearWordsEntry(&gApojiiIslands_CellMarkers, 0x1004);
  gApojiiIslands_CellMarkers.block = VramCache_FindFreeEntry();
  Vram_Load(gApojiiIslands_CellMarkers.block, 0x200, 0);
  FieldTask_AddFrame(ApojiiIslands_DrawCellMarkers, TASK_PASS_VBLANK);
}

void ApojiiIslands_AddCellMarkers(u32 base, s32 a, s32 b) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *second = &map->layers[1];
  struct MapLayer *layer = &map->layers[0];
  struct MapCell *cells = second->cells;
  s32 left = layer->offsetX + map->minX;
  s32 top = layer->offsetY + map->minZ;
  struct CellMarkers *markers = &gApojiiIslands_CellMarkers;
  s32 width = FIELD_CELL(map->maxX - map->minX);
  s32 height = FIELD_CELL(map->maxZ - map->minZ);
  struct CellMarker *entry = &markers->markers[markers->count];
  s32 *slot = (s32 *)(Data_0202c001 + 0x1df);
  s32 origin = (FIELD_CELL(map->minZ) << 7) + FIELD_CELL(map->minX);
  struct MapCell *cell;
  s16 x, z;
  u32 value;

  *slot = a << 8;
  cells += origin;
  z = 0;
  if (z < height) {
  outer:
    cell = cells + ((u16)z << 7);
    x = 0;
    if (x < width) {
    inner:
      value = cell->collisionCode;
      if (value != 0 && value >= base && value < base + 4) {
        entry->x = ((u16)x << 20) + left + toFIXED(8);
        entry->z = ((u16)z << 20) + top + toFIXED(8);
        entry->rest = b << 19;
        entry->y = a << 19;
        entry->kind = value - base;
        entry->state = 1;
        entry++;
        markers->count++;
        // Layer 0's row base in its own local (written inline, 10 of 159
        // differ).
        {
          u8 *terrain = map->layers[0].terrainIds + origin;

          terrain[(u16)x + ((u16)z << 7)] = 120;
        }
      }
      x = (toFIXED(x) + toFIXED(1)) >> 16;
      cell++;
      if ((u16)x < width)
        goto inner;
    }
    z = (toFIXED(z) + toFIXED(1)) >> 16;
    if ((u16)z < height)
      goto outer;
  }
}

extern s16 gGroundParticleFrames[];

#include "shared/ground_particles_draw.h"

/// Starts the sparkles: 64 sprites on the ground at the origin, their tiles
/// decoded into VRAM, and GroundParticles_Draw scheduled at priority 0x480 to
/// move them; the overlay's copy of FieldEffect_InitializeSparkles.
void ApojiiIslands_StartSparkles(void) {
  struct GroundParticleWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, sizeof(struct GroundParticleWork));
  struct GroundParticle *p = work->particles;
  u8 *buf;
  u32 i;
  u32 size;
  volatile u32 zero;

  size = 0;
  zero = size;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              sizeof(struct GroundParticleWork) / 4,
          REG_DMA3);
  buf = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
  Decompress_ByteLz(gField_GroundTiles, buf);
  work->entry = VramCache_FindFreeEntry();
  work->tileBase = Vram_Load(work->entry, 0x300, buf);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  work->leader = (s32 *)size;
  i = 0;
loop:
  size = (u32)p;
  *(u32 *)size = 0;
  size += 4;
  *(u32 *)size = 0x40000400;
  size += 4;
  *(u32 *)size = 0xb400;
  p->posX = 0;
  p->posZ = 0;
  p->posY = toFIXED(Map_GetTerrainHeight(0, 0, 0));
  p->timer = (i & 15) + 1;
  i++;
  p++;
  if (i < 64)
    goto loop;
  Task_AddFrame(GroundParticles_Draw, TASK_PASS_VBLANK);
}

void ApojiiIslands_SetSparkleAnchor(s32 id) {
  struct GroundParticleWork *work = Ram_HeapSlots->weather;
  struct Actor *object = Actor_Get(id);

  work->leader = NULL;
  if (id != 0 && object != 0)
    work->leader = &object->x.fixed;
}

#include "text/msg_ids.h"

#if !TLA_PATCH_1
// The Japanese table holds 131 where later editions hold MsgFieldOff's number,
// which the Japanese text numbers alike: a changed value, from patch 1.
// XXX: is it a message at all; what reads gApojiiIslands_PaceScript would
// settle it.
#define PACE_SCRIPT_THIRD_WORD 131
#else
#define PACE_SCRIPT_THIRD_WORD MSGNO(MsgFieldOff)
#endif

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

u32 gApojiiIslands_SplashScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

u32 gWaterDropScript[] = {
    SC_ADD(SW_Y, 0x4000),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};

s16 gGroundParticleFrames[] = {                                                               
      -8,  -12,  20,  0,   1,    -8,  -12,  20,  0,    1,   -8,   -12,         
      20,  0,    1,   -8,  -12,  16,  0,    1,   -8,   -12, 16,   0,           
      1,   -8,   -12, 12,  0,    1,   -8,   -12, 8,    0,   1,    0,           
      -32, 0,    2,   2,   8,    -48, 0,    2,   2,    16,  -64,  0,           
      2,   2,    24,  -80, 0,    2,   2,    32,  -96,  0,   2,    2,           
      40,  -112, 0,   2,   2,    48,  -128, 0,   2,    2,   56,   -144,        
      0,   2,    2,   64,  -160, 0,   2,    2,   -2,   12,  -4,   12,          
      -6,  12,   -8,  8,   -10,  8,   -12,  8,   -14,  8,   -16,  8,           
      -19, 4,    -21, 4,   -24,  4,   -27,  4,   -30,  4,   -33,  4,           
      -36, 4,    -40, 4,   -44,  4,   -48,  0,   -52,  0,   -56,  0,           
      -60, 0,    -64, 0,   -68,  0,   -72,  0,   -76,  0,   -80,  0,           
      -85, 0,    -90, 0,   -95,  0,   -100, 0,   -110, 0,   -120, 0,           
  };

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gApojiiIslands_PaceScript[] = {
    SC_SET(SW_COLLISION, PACE_SCRIPT_THIRD_WORD),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_MOVE(296, 0, 136),
    SC_SYNC,
    SC_MOVE(296, 0, 104),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(60),
    SC_WAIT(10),
    SC_WAIT(60),
    SC_ANIMATION(2),
    SC_MOVE(296, 0, 136),
    SC_SYNC,
    SC_MOVE(280, 0, 136),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(60),
    SC_WAIT(10),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(ApojiiIslands),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_APOJII_ISLANDS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_HAZE_NORTH, ApojiiIslandsHaze, APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_HAZE_SOUTH, ApojiiIslandsHaze, APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_SOUTHEAST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_SOUTHWEST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_EAST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_WEST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_NORTHWEST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_MIKASALLA_INTERIORS_SANCTUM, MikasallaInteriorsSanctum, MIKASALLA_INTERIORS_SANCTUM_FROM_APOJII_ISLANDS, EXIT_FLAG_NONE),
    SCENE_EXIT(APOJII_ISLANDS_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_AQUA_ROCK_MOAI_NORTH),
    SCENE_EXITS(ApojiiIslandsShops),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHEAST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHWEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_EAST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_WEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_NORTHWEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_ISLETS_TURTLE, IsletsTurtle, ISLETS_TURTLE_FROM_APOJII_ISLANDS_SHOPS, EXIT_FLAG_NONE),
    SCENE_EXITS(ApojiiIslandsHaze),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_NORTH, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_HAZE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_SOUTH, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_HAZE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHWEST, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHEAST, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_NORTH, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gApojiiIslands_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gApojiiIslands_VillagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 168, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 88, FACING_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 504, 0, 200, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 264, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 152, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 216, 0, 376, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 104, 0, 344, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 280, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(383, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gApojiiIslands_ShopsPlacements[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 104, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, (s32)gApojiiIslands_PaceScript, 280, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 344, 0, 120, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 592, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 128, 0, 620, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gApojiiIslands_HazePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_HAZE), MOVEMENT_TYPE_STAND, 376, 0, 200, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gApojiiIslands_Triggers[] = {
    {TRIGGER_TABLE_END},
};

s32 gApojiiIslands_FallsCycleFrame = -1;

s32 gApojiiIslands_SeaCycleFrame = -1;

s32 gApojiiIslands_StoneCycleFrame[] = {
    -1,
};

struct Trigger gApojiiIslands_VillageTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT(ApojiiIslands_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 21, CONDITION_ALWAYS, EVENT(ApojiiIslands_JumpForward)},
    {TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgThisIsApojii)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARNING_OF_THE_EDGE, CONDITION_ALWAYS, MSGNO(MsgWhateverYouDo)},
    {TRIGGER_TALK, ACTOR_MAN_ON_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgTheHugeWaterfall)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_MOAI, CONDITION_ALWAYS, MSGNO(MsgOnAquaIsland)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_AT_THE_EDGE_OF_THE_WORLD, CONDITION_ALWAYS, MSGNO(MsgHaveYouAny)},
    {TRIGGER_TALK, ACTOR_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhat)},
    {TRIGGER_TALK, ACTOR_BOY_COMING_OF_AGE, CONDITION_ALWAYS, MSGNO(MsgWhenTheyCome)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgDoYouThink)},
    {TRIGGER_TALK, ACTOR_FATHER, CONDITION_ALWAYS, MSGNO(MsgIfThereS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgLookingOverGaia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARNING_OF_THE_EDGE, CONDITION_ALWAYS, MSGNO(MsgTheCurrentsNear)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgAllOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_MOAI, CONDITION_ALWAYS, MSGNO(MsgWhyWouldAnyone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_AT_THE_EDGE_OF_THE_WORLD, CONDITION_ALWAYS, MSGNO(MsgWellAtLeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgApojiiHasSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_COMING_OF_AGE, CONDITION_ALWAYS, MSGNO(MsgAquaIslandIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgIWonderWhere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER, CONDITION_ALWAYS, MSGNO(MsgAquaRockMust)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_AQUA_STONE, FLAG_APOJII_AQUA_STONE_DOUSED, EVENT(ApojiiIslands_DouseAquaStone)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(ApojiiIslands_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(ApojiiIslands_RiseFromSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(ApojiiIslands_PauseSeaColors)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(ApojiiIslands_ResumeSeaColors)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21, CONDITION_ALWAYS, EVENT(ApojiiIslands_ClearFirstBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22, CONDITION_ALWAYS, EVENT(ApojiiIslands_ClearSecondBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 23, CONDITION_ALWAYS, EVENT(ApojiiIslands_ClearThirdBush)},
    {TRIGGER_TABLE_END},
};

struct Trigger gApojiiIslands_ShopsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_STARING_MAN, CONDITION_ALWAYS, MSGNO(MsgThereSThis)},
    {TRIGGER_TALK, ACTOR_SON, CONDITION_ALWAYS, EVENT(ApojiiIslands_TalkToSon)},
    {TRIGGER_TALK, ACTOR_STRICT_FATHER, CONDITION_ALWAYS, MSGNO(MsgIAbsolutelyRefuse)},
    {TRIGGER_TALK, ACTOR_CAUTIOUS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTheCurrentsAround)},
    {TRIGGER_TALK, ACTOR_BOY_KEPT_FROM_THE_OCEAN, CONDITION_ALWAYS, MSGNO(MsgIMNotAllowed)},
    {TRIGGER_TALK, ACTOR_GIRL_CALLING_FELIX_A_HERO, CONDITION_ALWAYS, MSGNO(MsgHeyMisterDid)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_AQUA_STONE, CONDITION_ALWAYS, MSGNO(MsgThatOldAqua)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_HER_HUSBANDS_DREAM, CONDITION_ALWAYS, MSGNO(MsgHeSToo)},
    {TRIGGER_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, EVENT(ApojiiIslands_TalkToMerchant)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(ApojiiIslands_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, EVENT(ApojiiIslands_AskAboutWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_STARING_MAN, CONDITION_ALWAYS, MSGNO(MsgIJustWish)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SON, CONDITION_ALWAYS, EVENT(ApojiiIslands_MindReadSon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRICT_FATHER, CONDITION_ALWAYS, MSGNO(MsgISinkLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAUTIOUS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgPoorBoyHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_KEPT_FROM_THE_OCEAN, CONDITION_ALWAYS, MSGNO(MsgItSStupid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_CALLING_FELIX_A_HERO, CONDITION_ALWAYS, MSGNO(MsgWowHeSwam)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_AQUA_STONE, CONDITION_ALWAYS, MSGNO(MsgYouUnlockThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_HER_HUSBANDS_DREAM, CONDITION_ALWAYS, MSGNO(MsgHeAlwaysWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgNoOneShips)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgIMSure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgThoseGuysOn)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_WARMING_SEA, CONDITION_ALWAYS, MSGNO(MsgTheTemperatureOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_WARMING_SEA, CONDITION_ALWAYS, MSGNO(MsgLongAgoPoseidon)},
    {TRIGGER_TABLE_END},
};

struct Trigger gApojiiIslands_HazeTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 40, CONDITION_ALWAYS, EVENT(ApojiiIslands_JumpDownLedge)},
    {TRIGGER_TALK, ACTOR_HAZE, CONDITION_ALWAYS, EVENT(ApojiiIslands_HazeJoinsParty)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_TIGHTROPE_POST_A, FLAG_SCENE_TEMP_00, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21, CONDITION_ALWAYS, EVENT(ApojiiIslands_ClearShopsBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22, CONDITION_ALWAYS, EVENT(ApojiiIslands_ClearShopsSmallBush)},
    {TRIGGER_TABLE_END},
};
#undef COPY_PICTURE
#undef PACE_SCRIPT_THIRD_WORD
#undef DEFINE_APOJII_ISLANDS_SHIFT_CELLS_AND_HOP
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
