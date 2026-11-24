/// @file
/// The Apojii Islands, at the edge of the world in the Eastern Sea, in three
/// rooms of this overlay: the village round the spring where the Aqua Stone
/// stands, which Douse wakes (ApojiiIslands_DouseAquaStone) to carry the party
/// across the sea to Aqua Rock; its houses, with the inn and the general
/// store, and beside them the Sea of Time Islet's house; and the islet among
/// the falls where the Jupiter Djinni Haze waits
/// (ApojiiIslands_HazeJoinsParty). The village's sea and the islet's falls
/// cycle their colours.
#include "flags.h"
#include "djinn_ids.h"
#include "field/field_sequence.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "scene/dust.h"

/// Set once Douse has woken the Aqua Stone.
#define FLAG_APOJII_AQUA_STONE_DOUSED 0x95f
/// Set by Douse on the Aqua Stone, which leaves for the World Map: back in the
/// village, the father who wished on the stone sees it answer
/// (ApojiiIslands_FatherSeesTheStone), and that clears it.
#define FLAG_FATHER_SEES_THE_STONE 0x95e
/// Set once the treasures buried under the weeds are found (gSceneItems): the
/// village's Mint and Herb and the islet's Bramble Seed.
#define FLAG_APOJII_ISLANDS_MINT_TAKEN 0xf9a
#define FLAG_APOJII_ISLANDS_HERB_TAKEN 0xf9b
#define FLAG_APOJII_ISLANDS_BRAMBLE_SEED_TAKEN 0xf9e
/// Haze's islet: set once Lash has tied the rope between its posts.
#define FLAG_HAZE_ROPE_TIED FLAG_SCENE_TEMP_00

/// The cell behaviours the village's and the islet's trigger tables answer:
/// the bank of the stream north of the hut by the Aqua Stone's pool, which
/// Felix jumps across walking north; the weeds Cyclone scatters; the weeds
/// over a buried treasure, the village's Herb and Mint and the islet's Bramble
/// Seed; and the edge of Haze's ledge, which Felix jumps down walking south.
enum {
  CELL_BEHAVIOR_STREAM_BANK = 33,
  CELL_BEHAVIOR_HAZE_LEDGE_EDGE = 40,
  CELL_BEHAVIOR_WEEDS = 21,
  CELL_BEHAVIOR_WEEDS_OVER_HERB = 22,
  CELL_BEHAVIOR_WEEDS_OVER_MINT = 23,
  CELL_BEHAVIOR_WEEDS_OVER_BRAMBLE_SEED = 22
};
#include "types.h"
#include "macros.h"
#include "graphics/window_sprite_list.h"
#include "system/io_write.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/scene_tables.h"

/// The village by the Aqua Stone (gApojiiIslands_VillagePlacements).
enum ApojiiIslandsVillageActorId {
  /// "This is Apojii, the easternmost island in the Eastern Sea!"
  ACTOR_WELCOMING_YOUNG_MAN = ACTOR_FIRST_PLACED,
  /// "Whatever you do, don't sail out to the eastern edge of the sea..."
  ACTOR_WOMAN_WARNING_OF_THE_EDGE,
  /// "The huge waterfall at the edge of the world is known as Gaia Falls."
  ACTOR_MAN_ABOUT_GAIA_FALLS,
  /// "On Aqua Island, you can see the guardian moai."
  ACTOR_WOMAN_ABOUT_THE_MOAI,
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
  ACTOR_GIRL_ABOUT_GAIA_FALLS,
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
  ACTOR_WOMAN_ABOUT_THE_STARING_MAN = ACTOR_FIRST_PLACED,
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
  ACTOR_OLD_MAN_ABOUT_THE_AQUA_STONE,
  /// "He's too old to try now, but my husband used to dream of scaling Aqua
  /// Rock."
  ACTOR_OLD_WOMAN_ABOUT_HER_HUSBANDS_DREAM,
  /// "It'd be nice if we could import some items that our visitors would
  /// like."
  ACTOR_GENERAL_STOREKEEPER,
  /// "Wow! You look just like the guy who showed up in the fancy boat."
  ACTOR_INNKEEPER,
  /// "You guys aren't the warriors who came in that boat, are you?"
  ACTOR_OLD_WOMAN_AT_THE_INN,
  /// In the Sea of Time Islet's house: "The temperature of the ocean has been
  /// rising."
  ACTOR_OLD_MAN_ABOUT_THE_WARMING_SEA
};

/// Haze's islet (gApojiiIslands_HazePlacements).
enum ApojiiIslandsHazeActorId {
  /// The rope's posts: Lash at the one on the rock among the falls ties the
  /// rope up to the one on the cliff.
  ///@{
  ACTOR_FALLS_ROPE_POST = ACTOR_FIRST_PLACED,
  ACTOR_CLIFF_ROPE_POST,
  ///@}
  ACTOR_HAZE
};

#include "system/iwram_call.h"
#include "system/io_reg.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "text/msg_ids.h"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
void Sprite_StartAnimation(void *animation, s32 value);
u8 *Sprite_AddAnimation(void *animation, s32 id);
void Irq_SetHandler(s32 irq, s32 vcount, void *callback);
void Actor_StartScript(struct Actor *actor, const void *data);
s32 MapGrid_CheckCollision(struct Actor *actor, s32 *position);
void Field_TryJumpForward(void);
void Rope_Initialize(void);
void Rope_Place(s32 index, s32 flag, s32 firstId, s32 secondId);
void Shop_Run(s32 stockRow, s32 keeper);
void Inn_CheckIn(s32 innId, s32 keeper);

void ApojiiIslands_CycleFallsColors(void);
void ApojiiIslands_CycleSeaColors(void);
void ApojiiIslands_CycleAquaStoneColors(void);
void ApojiiIslands_StopFlashing(struct Actor *actor);
void Actor_FlashWhite(struct Actor *actor);
void ApojiiIslands_FatherSeesTheStone(void);
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Scroll_StartHBlank(void);
void ApojiiIslands_InitializeHaze(void);
void ApojiiIslands_EnterSeaOfTimeIsletHouse(void);
void Actor_SpawnWadingSplash(struct Actor *actor);
s32 Actor_EmitMist(struct Actor *actor);
void Leader_RiseFromWater(void);
void ApojiiIslands_DrawCellMarkers(void);
void ApojiiIslands_StartCellMarkers(void);
void ApojiiIslands_AddCellMarkers(u32 base, s32 level, s32 restLevel);
void GroundParticles_Draw(void);
void GroundParticles_Start(void);
void GroundParticles_SetAnchor(s32 id);

/// The markers the woken Aqua Stone lays over the spring: at most this many.
#define CELL_MARKERS 128
/// A marker's kinds, by its cell's collision code from the first: kind 2 shows
/// kind 3's picture mirrored.
#define CELL_MARKER_KINDS 4

/// A marker drawn over a cell of the spring: its place, the height it rests
/// at, its kind (the cell's collision code less CELL_BEHAVIOR_SPRING), its
/// state and its sprite's attributes.
struct CellMarker {
  s32 x;
  s32 y;
  s32 z;
  s32 rest;
  u16 kind;
  u16 state;
  u32 oam[3];
};

/// ApojiiIslands_DrawCellMarkers's table: the markers' VRAM cache entry and
/// the markers.
struct CellMarkers {
  u16 block;
  u16 count;
  struct CellMarker markers[CELL_MARKERS];
};

/// The falls' palette as loaded, the source of each cycle.
u16 gApojiiIslands_FallsColors[PALETTE_COLORS] BSS_DATA ALIGNED(4);
/// The falls' palette as this frame's cycle leaves it.
u16 gApojiiIslands_FallsCycled[PALETTE_COLORS] BSS_DATA;
/// The spray's palette as loaded.
u16 gApojiiIslands_SprayColors[PALETTE_COLORS] BSS_DATA;
/// The spray's palette as this frame's cycle leaves it.
u16 gApojiiIslands_SprayCycled[PALETTE_COLORS] BSS_DATA;
/// The sea's palette as loaded.
u16 gApojiiIslands_SeaColors[PALETTE_COLORS] BSS_DATA;
/// The sea's palette as this frame's cycle leaves it.
u16 gApojiiIslands_SeaCycled[PALETTE_COLORS] BSS_DATA;
/// The shore's palette as loaded.
u16 gApojiiIslands_ShoreColors[PALETTE_COLORS] BSS_DATA;
/// The shore's palette as this frame's cycle leaves it.
u16 gApojiiIslands_ShoreCycled[PALETTE_COLORS] BSS_DATA;
/// The woken Aqua Stone's colours as loaded.
u16 gApojiiIslands_StoneColors[PALETTE_COLORS] BSS_DATA;
/// The woken Aqua Stone's colours as this frame's cycle leaves them.
u16 gApojiiIslands_StoneCycled[PALETTE_COLORS] BSS_DATA;
/// The islet's screen line below which BG1 drifts and is blended
/// (Scroll_HBlank), and BG1's scroll above and below it.
s32 gApojiiIslands_SplitLine BSS_DATA;
u16 gApojiiIslands_UpperScrollX BSS_DATA;
u16 gApojiiIslands_LowerScrollX BSS_DATA;
/// The markers drawn over the spring once the Aqua Stone is woken.
struct CellMarkers gApojiiIslands_CellMarkers BSS_DATA;
extern u32 gWadingSplash_Script[];
extern u32 gRippleScript[];

#include "shared/tables.h"
#include "scene_ids.h"
#include "field/call.h"
#include "system/io_write_queue.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/ground_mask.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "scene/event_layer.h"
#include "system/task.h"
#include "field/map_grid.h"
#include "field/map.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "graphics/palette_buffers.h"
#include "version.h"

/// The cautious mother's collision as she paces her house: from patch 1 the
/// leader can no longer push her.
#if !TLA_PATCH_1
#define PACER_COLLISION (ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID)
#else
#define PACER_COLLISION (ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID)
#endif

/// The palettes the colour cycles read and write: background palettes but for
/// the last.
enum {
  VILLAGE_SEA_PALETTE = 3, ///< the village's sea
  SHORE_PALETTE = 13,      ///< the village's shore
  FALLS_PALETTE = 3,       ///< the islet's falls
  SPRAY_PALETTE = 4,       ///< the islet's spray
  /// The village's palette the woken Aqua Stone's colours are loaded from.
  STONE_PALETTE = 4,
  /// The sprite palette they turn in, which the spring's markers take.
  STONE_SPRITE_PALETTE = 14
};

/// The sea's, shore's, falls' and spray's cycles turn their last four colours,
/// a step every eighth frame.
enum { CYCLE_FIRST_COLOR = 12, CYCLE_COLORS = 4, CYCLE_STEP_FRAMES = 8 };

/// The woken Aqua Stone's cycle turns every colour but the transparent first.
#define STONE_CYCLE_COLORS (PALETTE_COLORS - 1)

/// The collision code of the spring round the Aqua Stone, the first of the
/// four kinds of its cells (ApojiiIslands_AddCellMarkers).
#define CELL_BEHAVIOR_SPRING 33

/// The treasures buried under the weeds, by their place in gSceneItems: the
/// village's Mint and Herb, and the islet's Bramble Seed.
enum {
  ACTOR_MINT = FIELD_TREASURE_ACTOR,
  ACTOR_HERB,
  ACTOR_BRAMBLE_SEED = FIELD_TREASURE_ACTOR
};

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
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
void ApojiiIslands_ScatterHazeWeeds(void);
void ApojiiIslands_ScatterWeedsOverBrambleSeed(void);
void ApojiiIslands_EnterDoor(void);
void ApojiiIslands_JumpDownLedge(void);
void ApojiiIslands_JumpForward(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gApojiiIslands_PaceScript[];

/// Haze, the Jupiter Djinni on the islet, joins the party.
void ApojiiIslands_HazeJoinsParty(void) {
  Djinn_JoinParty(ACTOR_HAZE, DJINNI_HAZE / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_HAZE));
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

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
  Actor_Get(gMainRoutineState.leaderActorId)->layer = MAPGRID_LAYER_OVERHEAD;
}

/// Sand ends: the leader is back on the ground layer.
void ApojiiIslands_RiseFromSand(void) {
  Actor_Get(gMainRoutineState.leaderActorId)->layer = MAPGRID_LAYER_GROUND;
}

extern s32 gApojiiIslands_FallsCycleFrame;

/// The islet's falls and their spray cycle their last colours, held while
/// Psynergy or Reveal runs.
void ApojiiIslands_CycleFallsColors(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 phase, k, offset;
  u16 *firstCopy, *secondCopy;

  if (state->psynergy != 0)
    return;
  if (state->revealActive != 0)
    return;
  if (gApojiiIslands_FallsCycleFrame == -1) {
    Iwram_CopyWords(gApojiiIslands_FallsColors,
                    (void *)&BG_PLTT_COLOR(FALLS_PALETTE, 0),
                    sizeof(gApojiiIslands_FallsColors));
    Iwram_CopyWordsEntry(gApojiiIslands_FallsCycled, gApojiiIslands_FallsColors,
                         sizeof(gApojiiIslands_FallsCycled));
    Iwram_CopyWordsEntry(gApojiiIslands_SprayColors,
                         (void *)&BG_PLTT_COLOR(SPRAY_PALETTE, 0),
                         sizeof(gApojiiIslands_SprayColors));
    Iwram_CopyWordsEntry(gApojiiIslands_SprayCycled, gApojiiIslands_SprayColors,
                         sizeof(gApojiiIslands_SprayCycled));
    gApojiiIslands_FallsCycleFrame = 0;
  }
  phase = gApojiiIslands_FallsCycleFrame / CYCLE_STEP_FRAMES % CYCLE_COLORS;
  k = 0;
  firstCopy = gApojiiIslands_FallsCycled;
  secondCopy = gApojiiIslands_SprayCycled;
  for (; k < CYCLE_COLORS; k++) {
    offset = ((phase + k) % CYCLE_COLORS + CYCLE_FIRST_COLOR) * 2;
    *(u16 *)((u8 *)firstCopy + offset) =
        gApojiiIslands_FallsColors[CYCLE_FIRST_COLOR + k];
    *(u16 *)((u8 *)secondCopy + offset) =
        gApojiiIslands_SprayColors[CYCLE_FIRST_COLOR + k];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(FALLS_PALETTE, 0), (u32)firstCopy,
                      DMA_COPY_WORDS(8));
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(SPRAY_PALETTE, 0), (u32)secondCopy,
                      DMA_COPY_WORDS(8));
  gApojiiIslands_FallsCycleFrame++;
}

extern s32 gApojiiIslands_SeaCycleFrame;

/// The village's sea and shore cycle their last colours, held while Psynergy
/// or Reveal runs.
void ApojiiIslands_CycleSeaColors(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 busy, phase, k, j;

  if (state->psynergy != 0)
    return;
  busy = state->revealActive;
  if (busy != 0)
    return;
  if (gApojiiIslands_SeaCycleFrame == -1) {
    Iwram_CopyWords(gApojiiIslands_SeaColors,
                    (void *)&BG_PLTT_COLOR(VILLAGE_SEA_PALETTE, 0),
                    sizeof(gApojiiIslands_SeaColors));
    Iwram_CopyWordsEntry(gApojiiIslands_SeaCycled, gApojiiIslands_SeaColors,
                         sizeof(gApojiiIslands_SeaCycled));
    Iwram_CopyWordsEntry(gApojiiIslands_ShoreColors,
                         (void *)&BG_PLTT_COLOR(SHORE_PALETTE, 0),
                         sizeof(gApojiiIslands_ShoreColors));
    Iwram_CopyWordsEntry(gApojiiIslands_ShoreCycled, gApojiiIslands_ShoreColors,
                         sizeof(gApojiiIslands_ShoreCycled));
    gApojiiIslands_SeaCycleFrame = busy;
  }
  phase = gApojiiIslands_SeaCycleFrame / CYCLE_STEP_FRAMES % CYCLE_COLORS;
  for (k = 0; k < CYCLE_COLORS; k++) {
    j = (phase + k) % CYCLE_COLORS;
    gApojiiIslands_SeaCycled[CYCLE_FIRST_COLOR + j] =
        gApojiiIslands_SeaColors[CYCLE_FIRST_COLOR + k];
    gApojiiIslands_ShoreCycled[CYCLE_FIRST_COLOR + j] =
        gApojiiIslands_ShoreColors[CYCLE_FIRST_COLOR + k];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(VILLAGE_SEA_PALETTE, 0),
                      (u32)gApojiiIslands_SeaCycled, DMA_COPY_WORDS(8));
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(SHORE_PALETTE, 0),
                      (u32)gApojiiIslands_ShoreCycled, DMA_COPY_WORDS(8));
  gApojiiIslands_SeaCycleFrame++;
}

/// Reveal holds the sea's colour cycle as it takes effect.
void ApojiiIslands_PauseSeaColors(void) {
  Task_Suspend(ApojiiIslands_CycleSeaColors);
  System_Wait(3);
}

/// The sea's colour cycle runs again as Reveal fades.
void ApojiiIslands_ResumeSeaColors(void) {
  Task_Resume(ApojiiIslands_CycleSeaColors);
  System_Wait(3);
}

/// The woken Aqua Stone's colours turn a step every other frame, in the sprite
/// palette the spring's markers take.
void ApojiiIslands_CycleAquaStoneColors(void) {
  s32 base;
  s32 i;

  if (gApojiiIslands_StoneCycleFrame[0] == -1) {
    Iwram_CopyWordsEntry(gApojiiIslands_StoneColors,
                         (void *)&BG_PLTT_COLOR(STONE_PALETTE, 0),
                         sizeof(gApojiiIslands_StoneColors));
    gApojiiIslands_StoneCycleFrame[0] = 0;
  }
  base = (gApojiiIslands_StoneCycleFrame[0] / 2) % STONE_CYCLE_COLORS;
  for (i = 0; i < STONE_CYCLE_COLORS; i++)
    gApojiiIslands_StoneCycled[(base + i) % STONE_CYCLE_COLORS + 1] =
        gApojiiIslands_StoneColors[i + 1];
  IO_WRITE_QUEUE_PUSH((u32)&OBJ_PLTT_COLOR(STONE_SPRITE_PALETTE, 0),
                      (u32)gApojiiIslands_StoneCycled, DMA_COPY_WORDS(8));
  gApojiiIslands_StoneCycleFrame[0]++;
}

/// A flashing actor's last update: its sprite's colour back to normal.
void ApojiiIslands_StopFlashing(struct Actor *actor) {
  Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
  actor->update = 0;
}

#include "shared/actor_flash_white.h"

/// Douse on the Aqua Stone: it flickers white as Aqua Rock's music starts,
/// sparkles rise round it and the screen flashes four times in random colours;
/// then the sea stops cycling, markers in the stone's turning colours blend in
/// over the spring, the father starts, and the camera pans southeast before
/// the party leaves for the World Map, where sparkles carry it across the sea
/// to Aqua Rock (WorldMap_CarryPartyToAquaRock).
void ApojiiIslands_DouseAquaStone(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_EndLastingPsynergy(0);
  actor->update = Actor_FlashWhite;
  Sound_Play(SOUND_FADE_OUT_BGM);
  wait(30);
  Sound_Play(MUS_AQUA_ROCK);
  GroundParticles_Start();
  GroundParticles_SetAnchor(ACTOR_AQUA_STONE);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  i = 3;
  do {
    PaletteFade_SetTarget(Random16() & 0x7fff, 0);
    PaletteFade_StartInterpolation(15);
    wait(10);
    Value2((s32 (*)())PaletteFade_SetTarget, PALETTE_TINT_NONE, 0);
    PaletteFade_StartInterpolation(10);
    i--;
    wait(10);
  } while (i >= 0);
  ActorTable_Snapshot();
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 |
                          BLDCNT_TGT2_BG3,
                      IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(0, BLEND_WEIGHT_MAX),
                      IO_WRITE_HALFWORD);
  wait(1);
  Task_Remove(ApojiiIslands_CycleSeaColors);
  Task_Add(ApojiiIslands_CycleAquaStoneColors, TASK_PASS_FRAME);
  ApojiiIslands_StartCellMarkers();
  ApojiiIslands_AddCellMarkers(CELL_BEHAVIOR_SPRING, 0, 0);
  for (i = 1; i <= 6; i++) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(i, BLEND_WEIGHT_MAX),
                        IO_WRITE_HALFWORD);
    wait(4);
  }
  GameFlag_Set(FLAG_APOJII_AQUA_STONE_DOUSED);
  GameFlag_Set(FLAG_FATHER_SEES_THE_STONE);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(360, CAMERA_KEEP, 328, 1);
  Actor_ShowEmoteById(ACTOR_FATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(440, CAMERA_KEEP, 376, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(472, CAMERA_KEEP, 408, 1);
  Import_EventSetExitRequest(APOJII_ISLANDS_TO_WORLD_MAP_AQUA_ROCK);
}

/// Back in the village after Douse, the father who wished on the Aqua Stone
/// looks about him and wonders whether it heard his wishes.
void ApojiiIslands_FatherSeesTheStone(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_Get(ACTOR_AQUA_STONE)->update =
      (ObjectUpdate)ApojiiIslands_StopFlashing;
  FieldEvent_SetMessageId((s32)&MsgWhatWhatJust);
  Actor_FaceActor(gMainRoutineState.leaderActorId, ACTOR_FATHER, 0);
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

/// Cyclone scatters the weeds it touches: their cells take those of the
/// scattered weeds, kept below the room in the map from row 38.
#define SCATTER_WEEDS(column, row, width, height)                              \
  FieldTileCopies_CopyTouchedCells(column, 38, width, height, column, row, 1)

/// Cyclone on the village's weeds scatters them.
void ApojiiIslands_ScatterWeeds(void) { SCATTER_WEEDS(68, 4, 39, 24); }

/// Defines Cyclone on the weeds over a buried treasure: they scatter, and
/// until the treasure is found it shows at (spotX, spotZ), the leader hopping
/// a cell east when he stands on it.
#define DEFINE_SCATTER_WEEDS_OVER_TREASURE(name, column, row, width, height,   \
                                           flag, treasure, spotX, spotZ)       \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);         \
                                                                               \
    SCATTER_WEEDS(column, row, width, height);                                 \
    if (Event_GetFlag(flag) == 0) {                                            \
      Actor_SetPosition(treasure, spotX, spotZ);                               \
      if (FIELD_CELL(leader->x.fixed) == FIELD_CELL(toFIXED(spotX)) &&         \
          FIELD_CELL(leader->z.fixed) == FIELD_CELL(toFIXED(spotZ))) {         \
        Event_Start();                                                         \
        FieldEvent_EndLastingPsynergy(0);                                      \
        Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,               \
                                ATTACHED_EFFECT_IN_FRONT |                     \
                                    ATTACHED_EFFECT_ONCE);                     \
        Actor_SetDestinationOffset(gMainRoutineState.leaderActorId, 16, 0);    \
        leader->velocityY = toFIXED(6);                                        \
        FieldActor_WaitForMoveById(gMainRoutineState.leaderActorId);           \
        Event_End();                                                           \
      }                                                                        \
    }                                                                          \
  }

/// Cyclone on the clump of weeds south of the Aqua Stone, at cell (23, 23),
/// blows it away and uncovers the Herb buried there.
DEFINE_SCATTER_WEEDS_OVER_TREASURE(ApojiiIslands_ScatterWeedsOverHerb, 68, 4,
                                   38, 21, FLAG_APOJII_ISLANDS_HERB_TAKEN,
                                   ACTOR_HERB, 376, 376)

/// Cyclone on the clump of weeds in the village's south-east corner, at cell
/// (37, 25), blows it away and uncovers the Mint buried there.
DEFINE_SCATTER_WEEDS_OVER_TREASURE(ApojiiIslands_ScatterWeedsOverMint, 68, 4,
                                   38, 21, FLAG_APOJII_ISLANDS_MINT_TAKEN,
                                   ACTOR_MINT, 600, 408)

/// Cyclone on Haze's islet's weeds scatters them.
void ApojiiIslands_ScatterHazeWeeds(void) { SCATTER_WEEDS(67, 8, 11, 6); }

/// Cyclone on the clump of weeds on Haze's islet at cell (12, 9) blows it away
/// and uncovers the Bramble Seed buried there.
DEFINE_SCATTER_WEEDS_OVER_TREASURE(ApojiiIslands_ScatterWeedsOverBrambleSeed,
                                   67, 8, 11, 6,
                                   FLAG_APOJII_ISLANDS_BRAMBLE_SEED_TAKEN,
                                   ACTOR_BRAMBLE_SEED, 200, 152)

/// Felix walks in at the door of one of the village's huts, and the screen
/// fades to its room: the house of the man who stares at the Aqua Stone (its
/// door at cell (29, 18)), the strict father's family's (8, 17), the old
/// couple's in the north-east (35, 7), the inn (7, 6) or the general store
/// (12, 5).
/// Each door's cell behaviour is the number of the exit it leaves by, which
/// the field keeps as `stepTrigger`.
void ApojiiIslands_EnterDoor(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 2);
  Actor_Get(gMainRoutineState.leaderActorId)->motionFlags = 0;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineState.leaderActorId, 2, -6);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(state->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// The staring man's son turns to the leader and asks why his father stares at
/// the rock, unless the leader comes at him from the west, facing east.
s32 ApojiiIslands_TalkToSon(void) {
  u16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_Get(gMainRoutineState.leaderActorId)->facing);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (facing != FACING_EAST) {
    Actor_FaceActor(ACTOR_SON, gMainRoutineState.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)&MsgWhyDoesMy);
    // "Why does my father keep staring at that rock every day?"
    Event_ShowMessage(ACTOR_SON, 0);
    Actor_FaceDirection(ACTOR_SON, FACING_SOUTHEAST, 0);
  }
  Event_End();
  return 0;
}

/// Mind Read on the staring man's son: his thought, unless the leader reads it
/// from the west, facing east.
s32 ApojiiIslands_MindReadSon(void) {
  u16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_Get(gMainRoutineState.leaderActorId)->facing);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (facing != FACING_EAST) {
    FieldEvent_SetMessageId((s32)&MsgICanTEver);
    // "I can't ever tell what kind of mood Mom and Dad are in. At least..."
    Event_ShowMessage(ACTOR_SON, 0);
  }
  Event_End();
  return 0;
}

/// Walking south off Haze's ledge jumps the leader down.
void ApojiiIslands_JumpDownLedge(void) {
  Actor_JumpDownLedge(gMainRoutineState.leaderActorId, ANIM_STAND, 5);
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_APOJII_ISLANDS)
    return gApojiiIslands_VillageTriggers;
  if (scene == SCENE_APOJII_ISLANDS_SHOPS)
    return gApojiiIslands_ShopsTriggers;
  if (scene == SCENE_APOJII_ISLANDS_HAZE)
    return gApojiiIslands_HazeTriggers;
  return gApojiiIslands_Triggers;
}

/// The Apojii Islands' general store, as Shop_Run numbers the shops.
enum { SHOP_APOJII = 23 };

/// The Apojii Islands' inn, as Inn_CheckIn numbers the inns.
enum { APOJII_INN = 8 };

/// The innkeeper: across the counter, the inn; from elsewhere, his words about
/// the fancy boat.
void ApojiiIslands_TalkToInnkeeper(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_Get(gMainRoutineState.leaderActorId)->facing) == FACING_NORTH) {
    Inn_CheckIn(APOJII_INN, actor);
    return;
  }
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)&MsgWowYouLook);
  // "Wow! You look just like the guy who showed up in the fancy boat. He..."
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// The inn worker asks whether the party are the warriors who came in the boat,
/// and answers the yes or the no.
void ApojiiIslands_AskAboutWarriors(s32 actor) {
  s32 msg;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  msg = (s32)&MsgYouGuysAren;
  FieldEvent_SetMessageId(msg);
  // "You guys aren't the warriors who came in that boat, are you?"
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    // Yes: "Hmm... A while ago, some others came in a boat much like yours..."
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    // No: "It's not nice to tease an old lady! If that's not your boat, then
    // how did you get here?"
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// The general storekeeper: across the counter, the store; from elsewhere, her
/// wish for imported goods.
void ApojiiIslands_TalkToGeneralStorekeeper(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_Get(gMainRoutineState.leaderActorId)->facing) == FACING_NORTH) {
    Shop_Run(SHOP_APOJII, actor);
    return;
  }
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)&MsgItDBe);
  // "It'd be nice if we could import some items that our visitors would like."
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// The islet's HBlank split: above gApojiiIslands_SplitLine BG1 scrolls with
/// the map and blending is off; below it, BG1 drifts and is blended.
void Scroll_HBlank(void) {
  u32 value;

  if (REG_VCOUNT < gApojiiIslands_SplitLine) {
    value = gApojiiIslands_UpperScrollX;
    REG_BG1HOFS = value;
    REG_BLDCNT = 0;
  } else {
    value = gApojiiIslands_LowerScrollX;
    Io_Write16(value, &REG_BG1HOFS);
    REG_BLDALPHA = BLEND_ALPHA(12, BLEND_WEIGHT_MAX);
    REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  }
}

/// Each frame, follows the overhead layer's scroll for the split, the water
/// below it drifting west a pixel every fourth frame.
void Layer_TrackScroll(void) {
  struct MapLayer *layer = &((struct MapState *)Ram_HeapSlots->mapGrid)
                                ->layers[MAPGRID_LAYER_OVERHEAD];
  s16 y = FIXEDtoINT(layer->y);

  gApojiiIslands_SplitLine = 1448 - y;
  gApojiiIslands_UpperScrollX = FIXEDtoINT(layer->x);
  gApojiiIslands_LowerScrollX = (s16)FIXEDtoINT(layer->x) - (gFrameCount >> 2);
}

#include "shared/scroll_start_hblank.h"

/// Sets the rooms' wading and their window transition, the houses fading from
/// the backdrop instead, then each room's own setup: the islet's, and in the
/// village the sea's cycle and the father's surprise once Douse has woken the
/// Aqua Stone.
s32 Scene_Initialize(void) {
  s16 scene;
  struct MainRoutineState *party;

  Map_SetUpperLevelWading(Ram_HeapSlots->mapGrid);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  party = &gMainRoutineState;
  scene = party->next;
  if (scene == SCENE_APOJII_ISLANDS_HAZE) {
    ApojiiIslands_InitializeHaze();
  } else if (scene == SCENE_APOJII_ISLANDS) {
    FieldTask_AddFrame(ApojiiIslands_CycleSeaColors, TASK_PASS_FRAME);
    if (Event_GetFlag(FLAG_FATHER_SEES_THE_STONE))
      ApojiiIslands_FatherSeesTheStone();
  } else if (scene == SCENE_APOJII_ISLANDS_SHOPS) {
    gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (party->entrance == APOJII_ISLANDS_SHOPS_FROM_SEA_OF_TIME_ISLET)
      ApojiiIslands_EnterSeaOfTimeIsletHouse();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Haze's islet on entry: the falls' colours cycle, the rope is placed, tied
/// or not, the water sounds, and the leader rises from the water when he comes
/// up from the cave.
void ApojiiIslands_InitializeHaze(void) {
  Task_AddFrame(ApojiiIslands_CycleFallsColors, TASK_PASS_FRAME);
  Rope_Initialize();
  Rope_Place(0, FLAG_HAZE_ROPE_TIED, ACTOR_FALLS_ROPE_POST,
             ACTOR_CLIFF_ROPE_POST);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Scroll_StartHBlank();
  if (gMainRoutineState.entrance ==
      APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_SOUTHWEST)
    Leader_RiseFromWater();
  if (gMainRoutineState.entrance ==
      APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_SOUTHEAST)
    Leader_RiseFromWater();
  if (gMainRoutineState.entrance ==
      APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_NORTH)
    Leader_RiseFromWater();
}

/// The Sea of Time Islet's house, drawn beside the houses, entered from that
/// islet: the window opens round the leader and the Apojii Islands' name is not
/// shown.
void ApojiiIslands_EnterSeaOfTimeIsletHouse(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
}

/// Felix jumps across the stream north of the hut by the Aqua Stone's pool when
/// he walks north into its bank (cells (29, 12) and (30, 12)), landing on the
/// grass beyond.
/// The field's `Field_TryJumpForward` makes the jump. The second cell is one of
/// the weeds' cells, and walking north into weeds anywhere in the village tries
/// the same jump; away from the stream it starts nothing: walking north into
/// the weeds by the hut, cell (26, 19), or at (11, 16), Felix only walks on,
/// with no jump.
void ApojiiIslands_JumpForward(void) { Field_TryJumpForward(); }

#include "shared/actor_spawn_wading_splash.h"

#include "shared/actor_emit_ripples.h"

/// Unused here: Aqua Rock's stream carries the leader south, splashing, and
/// off a ledge he drops with the water (AquaRock_FlowWithStream).
UNUSED void ApojiiIslands_FlowWithStream(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 owner = gMainRoutineState.leaderActorId;
  struct Actor *anchor = state->camera;
  struct Actor *leader = Actor_Get(owner);
  FIXED position[3];
  s32 result;

  if (gMainRoutineState.next != SCENE_AQUA_ROCK_MOAI ||
      leader->groundHeight != 0) {
    position[0] = leader->x.fixed;
    position[1] = leader->y.fixed;
    position[2] = leader->z.fixed + toFIXED(2);
    result = MapGrid_CheckCollision(leader, position);
    if ((gFrameCount & 4) == 0)
      Actor_SpawnWadingSplash(leader);
    if (result < 0) {
      Actor_SetAttachedEffect(owner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      FieldActor_SetMoveTargetFX(leader, leader->x.fixed, leader->y.fixed,
                                 leader->z.fixed + toFIXED(8));
      Actor_SelectAnimation(leader, ANIM_LEADER_SIT);
      FieldActor_WaitForMove(leader);
      do {
        System_Wait(1);
      } while (leader->y.fixed != leader->groundHeight);
      Actor_SpawnWadingSplash(leader);
      Actor_SelectAnimation(leader, ANIM_LEADER_SIT);
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

#include "shared/moai_add_water.h"

/// The Aqua Rock room's actor the copy below splashes at, as
/// aqua_rock_inside.c names it.
enum { ACTOR_VIAL_CHEST_PUDDLE_C = ACTOR_FIRST_PLACED + 7 };

#include "shared/leader_spin_away.h"

#include "shared/actor_emit_mist.h"

#include "shared/leader_sink_away.h"

#include "shared/leader_rise_from_water.h"

/// The markers' pictures, three of the map's animated ones: from tile 1408,
/// counted from background character block 1 as the map's tile animation
/// counts, each two tiles wide over two rows of 32 tiles.
#define MARKER_SOURCE_TILE 1408
/// The tiles of a 16 by 16 marker picture in sprite VRAM: its upper half's two,
/// then its lower half's.
#define MARKER_PICTURE_TILES 4

/// Copies two tiles of the map's pictures to the markers' tiles in sprite VRAM.
#define COPY_TILES(fromTile, toTile)                                           \
  Dma_Set((void *)(BG_CHAR_BLOCK(1) + (fromTile) * TILE_SIZE_4BPP),            \
          (void *)(offset + OBJ_VRAM0 + (toTile) * TILE_SIZE_4BPP),            \
          DMA_COPY_WORDS(2 * TILE_SIZE_4BPP / 4), REG_DMA3)
/// Copies the map's picture `from` to marker picture `to`, upper half first.
#define COPY_PICTURE(from, to)                                                 \
  (COPY_TILES(MARKER_SOURCE_TILE + (from) * 2, (to) * MARKER_PICTURE_TILES),   \
   COPY_TILES(MARKER_SOURCE_TILE + 32 + (from) * 2,                            \
              (to) * MARKER_PICTURE_TILES + 2))

/// Each frame: copies the markers' pictures, which the map's tile animation
/// redraws, to sprite VRAM, and draws a sprite over each live marker in view,
/// semi-transparent in the woken Aqua Stone's turning colours, at the lowest
/// priority while it rests.
void ApojiiIslands_DrawCellMarkers(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  /* FAKEMATCH(temporary): view, a pointer standing in for map->viewX's
   * address; as map->viewX and map->viewZ by name. */
  s32 *view = &map->viewX;
  s32 viewX = view[0] & FIELD_PIXEL_MASK;
  s32 viewZ = view[1] & FIELD_PIXEL_MASK;
  // DEAD: viewY, subtracted from both height and z, cancels in dy; the ROM
  // keeps its load and its store to the stack.
  s32 viewY = map->origin[1];
  u32 tile = gVramCacheEntries[gApojiiIslands_CellMarkers.block].offset >> 5;
  u32 offset = gVramCacheEntries[gApojiiIslands_CellMarkers.block].offset;
  struct CellMarker *record;
  s32 i;

  // Kinds 2 and 3 show one picture, kind 2 mirrored.
  COPY_PICTURE(0, 0);
  COPY_PICTURE(1, 1);
  COPY_PICTURE(2, 2);
  COPY_PICTURE(2, 3);
  record = gApojiiIslands_CellMarkers.markers;
  for (i = CELL_MARKERS - 1; i >= 0; i--, record++) {
    s32 dx, dy, height, z, palette, flip, kind;
    s32 x, y, left, top, picture;
    u32 *attr;

    if (record->state == 0)
      continue;
    dx = record->x - viewX;
    height = record->y - viewY;
    z = record->z - viewZ - viewY;
    dy = z - height;
    if (record->y == record->rest && record->state != 2)
      palette = OBJ_PALETTE(STONE_SPRITE_PALETTE) | OBJ_PRIORITY(3);
    else
      palette = OBJ_PALETTE(STONE_SPRITE_PALETTE) | OBJ_PRIORITY(2);
    y = FIXEDtoINT(dy);
    /* FAKEMATCH(dead): a once-only block placing link, the queue word's empty
     * link, after the palette's choice. */
    {
      u32 link = 0;

      x = FIXEDtoINT(dx);
      left = x - 8;
      top = y - 8;
      if ((u32)(x + 7) > 334)
        continue;
      if ((u32)(y + 39) > 286)
        continue;
      left &= OBJ_X_MASK;
      /* FAKEMATCH(dead): a self-update of Ram_HeapSlots->mapGrid after left's
       * mask. */
      Ram_HeapSlots->mapGrid += 0;
      top &= OBJ_Y_MASK;
      attr = record->oam;
      kind = record->kind;
      picture = kind * MARKER_PICTURE_TILES;
      flip = 0;
      if (kind == 2)
        flip = OBJ_HFLIP;
      *attr++ = link;
      *attr++ = top | OBJ_X(left) | flip | OBJ_SIZE_16X16 |
                OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
      *attr = palette | (tile + picture);
      Oam_AddSpriteAtDepth(record->oam, 0);
    }
  }
}

/// Clears the markers, takes a free VRAM cache entry for their four pictures,
/// left empty, and starts drawing them each frame.
void ApojiiIslands_StartCellMarkers(void) {
  Iwram_ClearWordsEntry(&gApojiiIslands_CellMarkers,
                        sizeof(struct CellMarkers));
  gApojiiIslands_CellMarkers.block = VramCache_FindFreeEntry();
  Vram_Load(gApojiiIslands_CellMarkers.block,
            CELL_MARKER_KINDS * MARKER_PICTURE_TILES * TILE_SIZE_4BPP, 0);
  FieldTask_AddFrame(ApojiiIslands_DrawCellMarkers, TASK_PASS_FRAME);
}

/// Adds a marker over each cell of layer 1 whose collision code is one of the
/// CELL_MARKER_KINDS from `base`, at `level` and resting at `restLevel`, each
/// in steps of 8 pixels, and gives those cells MAP_SHAPE_MARKED, its entry set
/// to `level`.
void ApojiiIslands_AddCellMarkers(u32 base, s32 level, s32 restLevel) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  /* FAKEMATCH(dead): layer 1's cells read through its taken address; as
   * map->layers[1].cells. */
  struct MapCell *cells = (&map->layers[1])->cells;
  /* FAKEMATCH(dead): layer 0's offsets read through its taken address; as
   * map->layers[0].offsetX and offsetY. */
  s32 left = (&map->layers[0])->offsetX + map->minX;
  s32 top = (&map->layers[0])->offsetY + map->minZ;
  /* FAKEMATCH(temporary): markers, a pointer standing in for
   * gApojiiIslands_CellMarkers's address; as the table by name. */
  struct CellMarkers *markers = &gApojiiIslands_CellMarkers;
  s32 width = FIELD_CELL(map->maxX - map->minX);
  s32 height = FIELD_CELL(map->maxZ - map->minZ);
  struct CellMarker *entry = &markers->markers[markers->count];
  /* FAKEMATCH(temporary): slot, a pointer standing in for the marked shape's
   * entry; as the entry written through its cast address. */
  s32 *slot = (s32 *)(gMapGridShapes + MAP_SHAPE_MARKED * 4);
  s32 origin = (FIELD_CELL(map->minZ) << 7) + FIELD_CELL(map->minX);
  struct MapCell *cell;
  s16 x, z;
  u32 value;

  *slot = level << 8;
  cells += origin;
  z = 0;
  if (z < height) {
    do {
      cell = cells + ((u16)z << 7);
      x = 0;
      if (x < width) {
        do {
          value = cell->collisionCode;
          if (value != 0 && value >= base && value < base + CELL_MARKER_KINDS) {
            entry->x = FIELD_CELL_ORIGIN((u16)x) + left + toFIXED(8);
            entry->z = FIELD_CELL_ORIGIN((u16)z) + top + toFIXED(8);
            entry->rest = restLevel * toFIXED(8);
            entry->y = level * toFIXED(8);
            entry->kind = value - base;
            entry->state = 1;
            entry++;
            markers->count++;
            {
              /* FAKEMATCH(dead): layer 0's terrain from the room's corner in a
               * local of its own, never read. */
              u8 *terrain = map->layers[0].terrainIds + origin;

              (map->layers[0].terrainIds + origin)[(u16)x + ((u16)z << 7)] =
                  MAP_SHAPE_MARKED;
            }
          }
          x = NEXT_HALFWORD(x);
          cell++;
        } while ((u16)x < width);
      }
      z = NEXT_HALFWORD(z);
    } while ((u16)z < height);
  }
}

extern s16 gGroundParticleFrames[];

#include "shared/ground_particles_draw.h"

#include "shared/ground_particles_start.h"

#include "shared/ground_particles_set_anchor.h"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// A splash's script: it shows for 20 frames, then is released.
u32 gWadingSplash_Script[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

#include "shared/ripple_script.c"

#include "shared/ground_particle_frames.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The cautious mother paces her house: a step east and up to the north wall,
/// a pause, then back to her place, a pause.
s32 gApojiiIslands_PaceScript[] = {
    SC_SET(SW_COLLISION, PACER_COLLISION),
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

#include "shared/scene_entrances_default.c"

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
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_MIKASALLA_ROOMS_SANCTUM, MikasallaRoomsSanctum, MIKASALLA_ROOMS_SANCTUM_FROM_APOJII_ISLANDS, EXIT_FLAG_NONE),
    SCENE_EXIT(APOJII_ISLANDS_TO_WORLD_MAP_AQUA_ROCK, WorldMap, WORLD_MAP_CARRIED_TO_AQUA_ROCK),
    SCENE_EXITS(ApojiiIslandsShops),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHEAST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHWEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_EAST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_WEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_NORTHWEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_SEA_OF_TIME_ISLET, SeaOfTimeIslet, SEA_OF_TIME_ISLET_FROM_APOJII_ISLANDS_SHOPS, EXIT_FLAG_NONE),
    SCENE_EXITS(ApojiiIslandsHaze),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_NORTH, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_HAZE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_SOUTH, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_HAZE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHWEST, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHEAST, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_NORTH, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The scene's other rooms' actors: none.
struct ActorPlacement gApojiiIslands_Placements[] = {
    {SCENE_TABLE_END},
};

/// The village's actors, in ApojiiIslandsVillageActorId's order.
struct ActorPlacement gApojiiIslands_VillagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 168, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 88, FACING_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 504, 0, 200, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 264, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 152, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 216, 0, 376, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 104, 0, 344, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 280, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The houses' actors, in ApojiiIslandsHousesActorId's order.
struct ActorPlacement gApojiiIslands_ShopsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 104, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, (s32)gApojiiIslands_PaceScript, 280, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 344, 0, 120, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 592, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 128, 0, 620, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// Haze's islet's actors, in ApojiiIslandsHazeActorId's order.
struct ActorPlacement gApojiiIslands_HazePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_HAZE), MOVEMENT_TYPE_STAND, 376, 0, 200, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The scene's other rooms' triggers: none.
struct Trigger gApojiiIslands_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// Frames the falls' cycle has run; -1 until it loads the palettes.
s32 gApojiiIslands_FallsCycleFrame = -1;

/// Frames the sea's cycle has run; -1 until it loads the palettes.
s32 gApojiiIslands_SeaCycleFrame = -1;

/// Frames the Aqua Stone's cycle has run; -1 until it loads the palette.
s32 gApojiiIslands_StoneCycleFrame[] = {
    -1,
};

/// The village's exits, doors, talks and Psynergy.
struct Trigger gApojiiIslands_VillageTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, APOJII_ISLANDS_TO_WORLD_MAP_A},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, APOJII_ISLANDS_TO_APOJII_ISLANDS_HAZE_NORTH},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, APOJII_ISLANDS_TO_APOJII_ISLANDS_HAZE_SOUTH},
    {TRIGGER_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_SOUTHEAST, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_SOUTHWEST, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_EAST, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_WEST, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_NORTHWEST, CONDITION_ALWAYS, EVENT(ApojiiIslands_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, APOJII_ISLANDS_TO_MIKASALLA_ROOMS_SANCTUM},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_STREAM_BANK, CONDITION_ALWAYS, EVENT(ApojiiIslands_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_WEEDS, CONDITION_ALWAYS, EVENT(ApojiiIslands_JumpForward)},
    {TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgThisIsApojii)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARNING_OF_THE_EDGE, CONDITION_ALWAYS, MSGNO(MsgWhateverYouDo)},
    {TRIGGER_TALK, ACTOR_MAN_ABOUT_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgTheHugeWaterfall)},
    {TRIGGER_TALK, ACTOR_WOMAN_ABOUT_THE_MOAI, CONDITION_ALWAYS, MSGNO(MsgOnAquaIsland)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_AT_THE_EDGE_OF_THE_WORLD, CONDITION_ALWAYS, MSGNO(MsgHaveYouAny)},
    {TRIGGER_TALK, ACTOR_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhat)},
    {TRIGGER_TALK, ACTOR_BOY_COMING_OF_AGE, CONDITION_ALWAYS, MSGNO(MsgWhenTheyCome)},
    {TRIGGER_TALK, ACTOR_GIRL_ABOUT_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgDoYouThink)},
    {TRIGGER_TALK, ACTOR_FATHER, CONDITION_ALWAYS, MSGNO(MsgIfThereS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgLookingOverGaia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARNING_OF_THE_EDGE, CONDITION_ALWAYS, MSGNO(MsgTheCurrentsNear)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ABOUT_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgAllOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ABOUT_THE_MOAI, CONDITION_ALWAYS, MSGNO(MsgWhyWouldAnyone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_AT_THE_EDGE_OF_THE_WORLD, CONDITION_ALWAYS, MSGNO(MsgWellAtLeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgApojiiHasSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_COMING_OF_AGE, CONDITION_ALWAYS, MSGNO(MsgAquaIslandIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ABOUT_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgIWonderWhere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER, CONDITION_ALWAYS, MSGNO(MsgAquaRockMust)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_AQUA_STONE, FLAG_APOJII_AQUA_STONE_DOUSED, EVENT(ApojiiIslands_DouseAquaStone)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(ApojiiIslands_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(ApojiiIslands_RiseFromSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(ApojiiIslands_PauseSeaColors)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(ApojiiIslands_ResumeSeaColors)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS, CONDITION_ALWAYS, EVENT(ApojiiIslands_ScatterWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_OVER_HERB, CONDITION_ALWAYS, EVENT(ApojiiIslands_ScatterWeedsOverHerb)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_OVER_MINT, CONDITION_ALWAYS, EVENT(ApojiiIslands_ScatterWeedsOverMint)},
    {TRIGGER_TABLE_END},
};

/// The houses' exits, doors, talks and Psynergy.
struct Trigger gApojiiIslands_ShopsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHEAST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHWEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_EAST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_WEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_NORTHWEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_SEA_OF_TIME_ISLET},
    {TRIGGER_TALK, ACTOR_WOMAN_ABOUT_THE_STARING_MAN, CONDITION_ALWAYS, MSGNO(MsgThereSThis)},
    {TRIGGER_TALK, ACTOR_SON, CONDITION_ALWAYS, EVENT(ApojiiIslands_TalkToSon)},
    {TRIGGER_TALK, ACTOR_STRICT_FATHER, CONDITION_ALWAYS, MSGNO(MsgIAbsolutelyRefuse)},
    {TRIGGER_TALK, ACTOR_CAUTIOUS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTheCurrentsAround)},
    {TRIGGER_TALK, ACTOR_BOY_KEPT_FROM_THE_OCEAN, CONDITION_ALWAYS, MSGNO(MsgIMNotAllowed)},
    {TRIGGER_TALK, ACTOR_GIRL_CALLING_FELIX_A_HERO, CONDITION_ALWAYS, MSGNO(MsgHeyMisterDid)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ABOUT_THE_AQUA_STONE, CONDITION_ALWAYS, MSGNO(MsgThatOldAqua)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ABOUT_HER_HUSBANDS_DREAM, CONDITION_ALWAYS, MSGNO(MsgHeSToo)},
    {TRIGGER_TALK, ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS, EVENT(ApojiiIslands_TalkToGeneralStorekeeper)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(ApojiiIslands_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_AT_THE_INN, CONDITION_ALWAYS, EVENT(ApojiiIslands_AskAboutWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ABOUT_THE_STARING_MAN, CONDITION_ALWAYS, MSGNO(MsgIJustWish)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SON, CONDITION_ALWAYS, EVENT(ApojiiIslands_MindReadSon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRICT_FATHER, CONDITION_ALWAYS, MSGNO(MsgISinkLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAUTIOUS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgPoorBoyHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_KEPT_FROM_THE_OCEAN, CONDITION_ALWAYS, MSGNO(MsgItSStupid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_CALLING_FELIX_A_HERO, CONDITION_ALWAYS, MSGNO(MsgWowHeSwam)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ABOUT_THE_AQUA_STONE, CONDITION_ALWAYS, MSGNO(MsgYouUnlockThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ABOUT_HER_HUSBANDS_DREAM, CONDITION_ALWAYS, MSGNO(MsgHeAlwaysWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS, MSGNO(MsgNoOneShips)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgIMSure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgThoseGuysOn)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ABOUT_THE_WARMING_SEA, CONDITION_ALWAYS, MSGNO(MsgTheTemperatureOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ABOUT_THE_WARMING_SEA, CONDITION_ALWAYS, MSGNO(MsgLongAgoPoseidon)},
    {TRIGGER_TABLE_END},
};

/// Haze's islet's ways into the water, its ledge, Haze, Lash on the rope and
/// Cyclone on the weeds. Each way down into the water to the cave is the cell
/// behaviour of the exit it leaves by, which Leader_SinkAway takes.
struct Trigger gApojiiIslands_HazeTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_NORTH},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_SOUTH},
    {TRIGGER_STEP, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHWEST, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {TRIGGER_STEP, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHEAST, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {TRIGGER_STEP, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_NORTH, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_HAZE_LEDGE_EDGE, CONDITION_ALWAYS, EVENT(ApojiiIslands_JumpDownLedge)},
    {TRIGGER_TALK, ACTOR_HAZE, CONDITION_ALWAYS, EVENT(ApojiiIslands_HazeJoinsParty)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FALLS_ROPE_POST, FLAG_HAZE_ROPE_TIED, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS, CONDITION_ALWAYS, EVENT(ApojiiIslands_ScatterHazeWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_OVER_BRAMBLE_SEED, CONDITION_ALWAYS, EVENT(ApojiiIslands_ScatterWeedsOverBrambleSeed)},
    {TRIGGER_TABLE_END},
};
#undef COPY_PICTURE
#undef COPY_TILES
#undef PACER_COLLISION
#undef DEFINE_SCATTER_WEEDS_OVER_TREASURE
#undef SCATTER_WEEDS
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
