/// @file
/// The staff credits, played over the world map after the farewell at Prox:
/// each entrance is one stretch of the credits. Between them the camera flies
/// to each of the four lighthouses and shows its beacon's light
/// (StaffCredit_ShowBeacon), and last the four beams meet over Mt. Aleph and a
/// light falls from where they meet; then the scene goes on to the summit.
///
/// The overlay links the world map's flight, gauge and placed actors as well;
/// what of them the credits never call is marked UNUSED.
#include "fixed_math.h"
#include "flags.h"
#include "macros.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/actor_view.h"
#include "field/call.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/perspective.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "menu/menu.h"
#include "scene/gabomba_statue.h"
#include "scene/scene.h"
#include "scene/staff_credit.h"
#include "sound/sound.h"
#include "system/bg_scroll.h"
#include "system/frame.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "window/window.h"

void Party_DrainPp(s32 count);
FIXED Party_GetPpRatio(void);

#include "shared/tables.h"

/// The beacon's work buffer: the glow's or the beams' tiles, then from
/// BEACON_MAPS their maps.
#define BEACON_BUFFER_SIZE 24576
#define BEACON_MAPS 8192
/// The glow is drawn as a 64-pixel square, one byte a pixel, in levels of
/// light 1 to 15, each a colour of the picture palette.
#define GLOW_GRID_SIZE 64
#define GLOW_LEVEL_MAX (PALETTE_COLORS - 1)
/// Two pixels of colour 1, the glow's darkest: the tiles' blank rows.
#define GLOW_BLANK_PAIR ((u8)PIXELS_4BPP(1))
/// The beams' brighter colours, in the palette after the picture's.
#define BEAM_LIGHT_PALETTE (PICTURE_PALETTE + 1)
/// The meeting beams' tiles, loaded from tile 0 of character block 0.
#define BEAM_TILES 256
/// The picture layer's tiles the glow clears before it is drawn.
#define GLOW_CLEARED_TILES 256
/// Where the four beams meet, in pixels from where the last flight ends: the
/// light there stands west and north of it.
#define BEAM_MEET_OFFSET_X (-50)
#define BEAM_MEET_OFFSET_Z (-30)
/// The camera's turn back each frame of the flight home, and each frame of the
/// flight out until the turn ends.
#define FLIGHT_TURN_STEP (TRIG_TURN / 2048)

/// The beams' palette when they light up.
extern s32 gStaffCredit_BeamsLightPalette[];

u16 gStaffCredit_BlendLevel BSS_DATA;
u16 gStaffCredit_BlendSteady BSS_DATA;
/// The actors the World Map has placed (world_map_objects.c).
struct Actor *gWorldMap_PlacedActors[128] BSS_DATA;
u16 gStaffCredit_TransitionBlend BSS_DATA;
s16 gGauge_Shown BSS_DATA ALIGNED(4);
s16 gGauge_Vram BSS_DATA;
s16 gGauge_Countdown BSS_DATA;
/// The Psynergy gauge's sprite, its tiles as drawn, and the pictures they are
/// drawn from (world_map_gauge.c).
u32 gGauge_Sprite[3] BSS_DATA;
u8 gGauge_Tiles[8][32] BSS_DATA;
u8 gGauge_SourceTiles[12][32] BSS_DATA;
/// The beacon scene's pictures: the beacons' palettes, the beams' palette,
/// their maps and their tiles.
extern u8 gStaffCredit_BeaconPalettes[];
extern u8 gStaffCredit_BeamsPalette[];
extern u8 gStaffCredit_BeamsMaps[];
extern u8 gStaffCredit_BeamsTiles[];
/// Scrolls the credits from the page CREDITS_FROM_PAGE gives (Credits_Run,
/// through the overlay's own import row).
void StaffCredit_RunCredits(s32 value);
s32 WorldMap_GetBlockCellBehavior(s32 *position);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets the layer blend, switching its alpha every two frames unless
/// gStaffCredit_BlendSteady is set.
void StaffCredit_BlendBeacon(void) {
  Scene_QueueIoWrite16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if ((gFrameCount & 2) && gStaffCredit_BlendSteady == 0) {
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA,
                         gStaffCredit_BlendLevel | (BLEND_WEIGHT_MAX - 1));
  } else {
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA,
                         gStaffCredit_BlendLevel | BLEND_WEIGHT_MAX);
  }
}

/// Adds a red, green and blue step to each colour of a palette, clamped to 0 to
/// 31.
void StaffCredit_StepPaletteColors(u16 *colors, s32 count, s32 red, s32 green,
                                   s32 blue) {
  s32 i;

  for (i = 0; i != count; i++) {
    s32 r = RGB_RED(*colors) + red;
    s32 g = RGB_GREEN(*colors) + green;
    s32 b = RGB_BLUE(*colors) + blue;

    if (r > RGB_LEVEL_MAX)
      r = RGB_LEVEL_MAX;
    if (r < 0)
      r = 0;
    if (g > RGB_LEVEL_MAX)
      g = RGB_LEVEL_MAX;
    if (g < 0)
      g = 0;
    if (b > RGB_LEVEL_MAX)
      b = RGB_LEVEL_MAX;
    if (b < 0)
      b = 0;
    *colors++ = RGB(r, g, b);
  }
}

/// Draws the beacon's glow as rings of light about a corner, brightest at the
/// middle.
void StaffCredit_DrawGlowRings(u8 *grid, s32 count, s32 scale) {
  s32 i = 0;
  // The corner the rings are centred on.
  s32 cornerColumn = GLOW_GRID_SIZE - 1;
  s32 cornerRow = GLOW_GRID_SIZE - 1;

  Iwram_FillWords(grid, GLOW_GRID_SIZE * GLOW_GRID_SIZE, PIXELS_8BPP(1));
  if (count == 1)
    return;
  do {
    s32 y = 0;
    s32 last = count - 1;
    s32 level;
    s32 x = i;
    s32 error = i;

    level = FIXEDtoINT(Trig_Cos(i * (TRIG_TURN / 4) / last) * GLOW_LEVEL_MAX);
    level = level * scale / FIXED_ONE;
    if (level > GLOW_LEVEL_MAX)
      level = GLOW_LEVEL_MAX;
    if (level <= 0)
      level = 1;
    for (; x >= y; y++) {
      s32 lastColumn = GLOW_GRID_SIZE - 1;
      s32 column = cornerColumn - x;
      s32 row = cornerRow - y;
      u8 *cell;
      cell = &grid[(row << 6) + column];
      cell[0] = level;
      if (column < lastColumn)
        cell[1] = level;
      // The same point mirrored across the diagonal.
      column = row;
      row = cornerRow - x;
      cell = &grid[(row << 6) + column];
      cell[0] = level;
      if (column < lastColumn)
        cell[1] = level;
      error = error - y * 2 - 1;
      if (error < 0) {
        error = error + x * 2 - 2;
        x--;
      }
    }
    i++;
  } while (i != count - 1);
}

extern s16 gStaffCredit_FlightStarts[][2];
extern s16 gStaffCredit_FlightEnds[][2];
extern s16 gStaffCredit_FlightReturns[][2];
extern s32 gStaffCredit_StartZooms[];
extern s32 gStaffCredit_EndZooms[];
extern u8 gStaffCredit_FlightFrames[];
extern u8 gStaffCredit_FlightTurns[];
extern u16 gStaffCredit_GlowFadeIn[];
/// The glow layer's scroll as each stage starts, which sets it over its
/// lighthouse.
extern s8 gStaffCredit_GlowPositions[][2];

/// The lighthouses whose beacons the credits fly to, in the order they are
/// shown, and last the four beams meeting over Mt. Aleph.
enum BeaconStage {
  BEACON_MARS,
  BEACON_MERCURY,
  BEACON_VENUS,
  BEACON_JUPITER,
  BEACON_BEAMS_MEET
};

/// The staff credits' one placed actor, a beacon's glow (SPRITE_MARS_BEACON)
/// placed atop Mars Lighthouse; StaffCredit_ShowBeacon puts it out of sight,
/// and in the last stage grows it where the four beams meet.
enum { ACTOR_BEACON = ACTOR_FIRST_PLACED };

/// Flies the camera to a lit lighthouse and shows its beacon's glow, then flies
/// back; BEACON_BEAMS_MEET is the four beams meeting over Mt. Aleph.
void StaffCredit_ShowBeacon(s32 stage) {
  struct WorldMapView *map = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  s32 palette;
  u8 *buffer;
  u8 *grid;
  s32 x;
  s32 y;
  s32 i;
  s32 frame;

  Actor_SetPosition(ACTOR_BEACON, 0, 0);
  gStaffCredit_BlendSteady = 0;
  wait(1);
  Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
  gFieldWork->transition = 0;
  palette = stage;
  buffer = Heap_AllocEwram(BEACON_BUFFER_SIZE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEACON), 0);
  if (stage == BEACON_BEAMS_MEET)
    palette = 0;
  gBgScroll[0].y = 0;
  gBgScroll[0].x = 0;
  FieldCamera_MoveToFX(toFIXED(gStaffCredit_FlightStarts[stage][0]),
                       CAMERA_KEEP,
                       toFIXED(gStaffCredit_FlightStarts[stage][1]), 0);
  WorldMap_UpdateCurrentTileBlock();
  FieldEffect_ScheduleRatioTransition(gStaffCredit_StartZooms[stage], 1);
  map->yaw = TRIG_TURN / 16;
  map->pitch = TRIG_TURN * 13 / 16;
  Iwram_ClearWordsEntry((void *)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                        PALETTE_COLORS * 2);
  wait(1);
  Actor_GetById(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transitionFrames = 64;
  Event_StartScreenTransition();
  FieldCamera_MoveToFX(toFIXED(gStaffCredit_FlightEnds[stage][0]), CAMERA_KEEP,
                       toFIXED(gStaffCredit_FlightEnds[stage][1]), 1);
  for (frame = 0; frame != gStaffCredit_FlightFrames[stage]; frame++) {
    {
      s32 length = gStaffCredit_FlightFrames[stage];

      map->yaw = (length - frame) * FLIGHT_TURN_STEP;
      if (frame == length - 59)
        FieldEffect_ScheduleRatioTransition(gStaffCredit_EndZooms[stage], 60);
    }
    if (frame >= gStaffCredit_FlightFrames[stage] - 64) {
      u16 *turn = &map->pitch;
      u8 *step = gStaffCredit_FlightTurns + 1;

      *turn += step[stage];
    }
    wait(1);
  }
  WorldMap_Freeze();
  if (stage == BEACON_BEAMS_MEET) {
    u8 *frames = buffer + BEACON_MAPS;

    Decompress_TaggedLz(gStaffCredit_BeamsTiles, buffer);
    Decompress_TaggedLz(gStaffCredit_BeamsMaps, frames);
    IO_WRITE_QUEUE_PUSH(VRAM, (u32)buffer,
                        DMA_COPY_WORDS(BEAM_TILES * TILE_SIZE_4BPP / 4));
    IO_WRITE_QUEUE_PUSH(PICTURE_MAP, (u32)frames,
                        DMA_COPY_WORDS(PICTURE_FRAME_SIZE / 4));
    grid = frames;
    wait(1);
  } else {
    u16 *tiles = (u16 *)buffer;

    IO_WRITE_QUEUE_PUSH(
        (u32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
        (u32)(gStaffCredit_BeaconPalettes + palette * PALETTE_COLORS * 2),
        DMA_COPY_HALFWORDS(PALETTE_COLORS));
    Iwram_ClearWords((void *)PICTURE_TILES,
                     GLOW_CLEARED_TILES * TILE_SIZE_4BPP);
    wait(1);
    // The glow's map: its quarter drawn four times, mirrored across and down
    // into a whole disc, and blank rows below.
    for (y = 0; y != 8; y++) {
      for (x = 0; x != 16; x++) {
        s32 tile = y * 16 + x + PICTURE_FIRST_TILE;

        tiles[y * 32 + x] = tile | (s16)BG_TILE_PALETTE(PICTURE_PALETTE);
        tiles[y * 32 - x + 31] =
            tile | (s16)(BG_TILE_PALETTE(PICTURE_PALETTE) | TILE_HFLIP);
        tiles[(15 - y) * 32 + x] =
            tile | (s16)(BG_TILE_PALETTE(PICTURE_PALETTE) | TILE_VFLIP);
        tiles[(15 - y) * 32 - x + 31] =
            tile |
            (s16)(BG_TILE_PALETTE(PICTURE_PALETTE) | TILE_HFLIP | TILE_VFLIP);
      }
    }
    for (y = 0; y != 16; y++) {
      for (x = 0; x != 32; x++)
        tiles[y * 32 + x + 16 * BG_SCREEN_WIDTH] =
            BG_TILE_PALETTE(PICTURE_PALETTE) | PICTURE_FIRST_TILE;
    }
    IO_WRITE_QUEUE_PUSH(PICTURE_MAP, (u32)buffer,
                        DMA_COPY_HALFWORDS(BG_SCREEN_WIDTH * BG_SCREEN_WIDTH));
    wait(1);
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Task_Add(StaffCredit_BlendBeacon, TASK_PASS_FRAME);
  Event_PlaySound(SE_GLITTER);
  for (frame = 0; frame != 172; frame++) {
    if (frame == 0) {
      if (stage == BEACON_BEAMS_MEET) {
        gBgScroll[0].x = 8;
        gBgScroll[0].y = frame;
        IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                            (u32)gStaffCredit_BeamsPalette,
                            DMA_COPY_HALFWORDS(PALETTE_COLORS));
        IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(BEAM_LIGHT_PALETTE, 0),
                            (u32)gStaffCredit_BeamsLightPalette,
                            DMA_COPY_HALFWORDS(PALETTE_COLORS));
      } else {
        gBgScroll[0].x = gStaffCredit_GlowPositions[stage][0];
        gBgScroll[0].y = gStaffCredit_GlowPositions[stage][1];
        IO_WRITE_QUEUE_PUSH(
            (u32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
            (u32)(gStaffCredit_BeaconPalettes + palette * PALETTE_COLORS * 2),
            DMA_COPY_HALFWORDS(PALETTE_COLORS));
      }
    }
    if (stage == BEACON_BEAMS_MEET) {

      {
        s16 *flightEnds = (s16 *)gStaffCredit_FlightEnds;
        s32 endX;
        s32 endZ;
        FIXED offsetZ;
        FIXED offsetX;
        endX = flightEnds[BEACON_BEAMS_MEET * 2];
        endZ = flightEnds[BEACON_BEAMS_MEET * 2 + 1];
        offsetZ = toFIXED(BEAM_MEET_OFFSET_Z);
        offsetX = toFIXED(BEAM_MEET_OFFSET_X);

        Actor_SetPositionFX(ACTOR_BEACON, toFIXED(endX) + offsetX,
                            toFIXED(endZ) + offsetZ);
      }
      Actor_SetBlendMode(Actor_GetById(ACTOR_BEACON), 1);
      Actor_SetAnimation(ACTOR_BEACON, 0);
      Actor_GetById(ACTOR_BEACON)->scaleX = 0;
      Actor_GetById(ACTOR_BEACON)->scaleY = 0;
      if (frame > 22) {
        s32 scale = (frame - 21) * (FIXED_ONE / 64);

        if (scale > FIXED_ONE)
          scale = FIXED_ONE;
        Actor_GetById(ACTOR_BEACON)->scaleX = scale;
        Actor_GetById(ACTOR_BEACON)->scaleY = scale;
      }
      if (frame <= 31) {
        s32 step = frame / 4;

        if (step <= 6)
          IO_WRITE_QUEUE_PUSH(
              PICTURE_MAP,
              (u32)(buffer + step * PICTURE_FRAME_SIZE + BEACON_MAPS),
              DMA_COPY_WORDS(PICTURE_FRAME_SIZE / 4));
        gStaffCredit_BlendLevel = BLEND_ALPHA(0, 10);
      }
      if (frame == 32) {
        gStaffCredit_BlendSteady = 1;
        gStaffCredit_BlendLevel = BLEND_ALPHA(0, 10);
      }
      if (frame == 36)
        gStaffCredit_BlendLevel = BLEND_ALPHA(0, 11);
      if (frame == 40)
        gStaffCredit_BlendLevel = BLEND_ALPHA(0, 12);
      if (frame == 44)
        gStaffCredit_BlendLevel = BLEND_ALPHA(0, 13);
      if (frame == 48)
        gStaffCredit_BlendLevel = BLEND_ALPHA(0, 14);
      if (frame == 52)
        gStaffCredit_BlendLevel = BLEND_ALPHA(0, 15);
      if (frame == 56)
        gStaffCredit_BlendLevel = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
      if (frame == 70)
        IO_WRITE_QUEUE_PUSH(
            PICTURE_MAP, (u32)(buffer + BEACON_MAPS + 7 * PICTURE_FRAME_SIZE),
            DMA_COPY_WORDS(PICTURE_FRAME_SIZE / 4));
      if (frame == 72)
        IO_WRITE_QUEUE_PUSH(
            PICTURE_MAP, (u32)(buffer + BEACON_MAPS + 8 * PICTURE_FRAME_SIZE),
            DMA_COPY_WORDS(PICTURE_FRAME_SIZE / 4));
      if (frame == 74)
        IO_WRITE_QUEUE_PUSH(
            PICTURE_MAP, (u32)(buffer + BEACON_MAPS + 9 * PICTURE_FRAME_SIZE),
            DMA_COPY_WORDS(PICTURE_FRAME_SIZE / 4));
      if (frame == 76)
        IO_WRITE_QUEUE_PUSH(
            PICTURE_MAP, (u32)(buffer + BEACON_MAPS + 10 * PICTURE_FRAME_SIZE),
            DMA_COPY_WORDS(PICTURE_FRAME_SIZE / 4));
      if (frame > 153) {
        gStaffCredit_BlendLevel = 0;
        StaffCredit_StepPaletteColors((u16 *)&BG_PLTT_COLOR(0, 0), 256, -2, -2,
                                      -2);
        StaffCredit_StepPaletteColors((u16 *)&OBJ_PLTT_COLOR(0, 0), 256, -2, -2,
                                      -2);
      } else if (frame > 79 && (frame & 1)) {
        StaffCredit_StepPaletteColors((u16 *)&BG_PLTT_COLOR(0, 0), 256, 1, 1,
                                      1);
        StaffCredit_StepPaletteColors((u16 *)&OBJ_PLTT_COLOR(0, 0), 256, 1, 1,
                                      1);
      }
    } else if (frame >= 0) {
      grid = buffer + BEACON_MAPS;

      if (frame + 8 <= 63) {
        s32 offset = 0;
        s32 by, bx, row;
        s32 rowBase;
        s32 base;
        u8 *out;
        u8 *fill;

        StaffCredit_DrawGlowRings(grid, frame + 8, FIXED_ONE);
        by = 0;
        rowBase = 0;
        goto blockRowTest;
      blockRow:
        bx = 0;
        fill = (u8 *)offset + (s32)buffer;
        do {
          *fill = GLOW_BLANK_PAIR;
          bx++;
          offset++;
          fill++;
        } while (bx != 8 * TILE_SIZE_4BPP);
        bx = 0;
        base = rowBase * 8;
        goto blockTest;
      block:
        row = 0;
        i = 0;
        do {
          x = 0;
          out = (u8 *)offset + (s32)buffer;
          do {
            u8 *pair = grid + (x * 2 + (base + i));

            x++;
            *out = pair[1] << 4;
            *out |= pair[0];
            offset++;
            out++;
          } while (x != 4);
          row++;
          i += GLOW_GRID_SIZE;
        } while (row != 8);
        base += 8;
        bx++;
      blockTest:
        if (bx != 8)
          goto block;
        rowBase += 64;
        by++;
      blockRowTest:
        if (by != 8)
          goto blockRow;
        IO_WRITE_QUEUE_PUSH(
            PICTURE_TILES, (u32)buffer,
            DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4));
      }
    }
    if (stage != BEACON_BEAMS_MEET) {
      u16 *alpha = &gStaffCredit_BlendLevel;
      u32 scrollFrames = frame - 32;

      // DEAD: the fade-in's level, at once overwritten with full strength;
      // the ROM keeps the table read and its store.
      *alpha = frame <= 6 ? gStaffCredit_GlowFadeIn[frame % 6]
                          : BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
      gStaffCredit_BlendLevel = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
      if (frame == 138)
        gStaffCredit_BlendSteady = 1;
      if (frame > 64 && (frame & 1))
        StaffCredit_StepPaletteColors((u16 *)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                                      16, 1, 1, 1);
      if (frame > 140)
        StaffCredit_StepPaletteColors((u16 *)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                                      16, -2, -2, -2);
      if (scrollFrames <= 95)
        gBgScroll[0].y++;
    }
    wait(1);
  }
  FieldTask_Remove(StaffCredit_BlendBeacon);
  if (stage != BEACON_BEAMS_MEET) {
    FieldCamera_SetSpeed(6);
    WorldMap_ReloadPendingAreaGraphics();
    FieldCamera_MoveToFX(toFIXED(gStaffCredit_FlightReturns[stage][0]),
                         CAMERA_KEEP,
                         toFIXED(gStaffCredit_FlightReturns[stage][1]), 1);
    FieldEffect_ScheduleRatioTransition(FIXED_ONE * 7 / 10, 60);
    for (frame = 0; frame != 124;) {
      map->yaw -= FLIGHT_TURN_STEP;
      if (frame == 60) {
        gFieldWork->transitionFrames = 64;
        Event_FinishScreenTransition();
      }
      frame++;
      wait(1);
    }
    Event_WaitTransitionFrames();
  } else {
    gBgScroll[0].x = 0;
    gBgScroll[0].y = 0;
  }
  Heap_Release(buffer);
  DebugText_Clear();
  Scene_QueueIoWrite16((u32)&REG_BLDALPHA, gStaffCredit_BlendLevel);
  Scene_QueueIoWrite16((u32)&REG_BLDCNT, 0);
  Event_SetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

/// The staff credits' entrances, each a stretch of the credits: the summit
/// enters the first two, and each beacon's stretch enters the next.
enum StaffCreditEntrance {
  STAFF_CREDIT_OPENING,    ///< then back to the summit
  STAFF_CREDIT_TO_BEACONS, ///< then on to the beacons
  STAFF_CREDIT_MARS_BEACON,
  STAFF_CREDIT_MERCURY_BEACON,
  STAFF_CREDIT_VENUS_BEACON,
  STAFF_CREDIT_JUPITER_BEACON,
  STAFF_CREDIT_BEAMS_MEET ///< the four beams over Mt. Aleph, then the summit
};

/// Credits_Run's mode for the scrolling credits from the given page (mode 1 is
/// the title's slide show).
#define CREDITS_FROM_PAGE(page) ((page) + 2)

/// The summit's entrances the credits return to (mt_aleph_summit.c): Alex sees
/// the Golden Sun begin to form, and Alex takes the Golden Sun's power.
enum { SUMMIT_GOLDEN_SUN_FORMS = 2, SUMMIT_ALEX_TAKES_THE_POWER = 3 };

/// Chooses where the credits go next by the entrance they were entered from:
/// each beacon's entrance first shows its lighthouse's beacon, the last the
/// beams meeting, and each but the last runs the next stretch of credits.
s32 Scene_Initialize(void) {
  struct FieldWork *work;

  Event_SetFlag(FLAG_RETREAT_BLOCKED);
  work = Ram_HeapSlots->fieldWork;
  work->transition = 0;
  work->transitionFrames = 16;
  System_Wait(1);
  FieldCamera_SetSpeed(8);
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  WorldMap_StartPlacedActors();
  switch (gMainRoutineWork.entrance) {
  case STAFF_CREDIT_OPENING:
    Resource_ReservedNoOp(RESOURCE_CREDITS_FAR_CALLS);
    StaffCredit_RunCredits(CREDITS_FROM_PAGE(0));
    Event_SetDestination(SCENE_MT_ALEPH_SUMMIT, SUMMIT_GOLDEN_SUN_FORMS);
    break;
  case STAFF_CREDIT_TO_BEACONS:
    Resource_ReservedNoOp(RESOURCE_CREDITS_FAR_CALLS);
    StaffCredit_RunCredits(CREDITS_FROM_PAGE(1));
    Event_SetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_MARS_BEACON);
    break;
  case STAFF_CREDIT_MARS_BEACON:
    StaffCredit_ShowBeacon(BEACON_MARS);
    Resource_ReservedNoOp(RESOURCE_CREDITS_FAR_CALLS);
    StaffCredit_RunCredits(CREDITS_FROM_PAGE(2));
    Event_SetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_MERCURY_BEACON);
    break;
  case STAFF_CREDIT_MERCURY_BEACON:
    StaffCredit_ShowBeacon(BEACON_MERCURY);
    Resource_ReservedNoOp(RESOURCE_CREDITS_FAR_CALLS);
    StaffCredit_RunCredits(CREDITS_FROM_PAGE(3));
    Event_SetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_VENUS_BEACON);
    break;
  case STAFF_CREDIT_VENUS_BEACON:
    StaffCredit_ShowBeacon(BEACON_VENUS);
    Resource_ReservedNoOp(RESOURCE_CREDITS_FAR_CALLS);
    StaffCredit_RunCredits(CREDITS_FROM_PAGE(4));
    Event_SetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_JUPITER_BEACON);
    break;
  case STAFF_CREDIT_JUPITER_BEACON:
    StaffCredit_ShowBeacon(BEACON_JUPITER);
    Resource_ReservedNoOp(RESOURCE_CREDITS_FAR_CALLS);
    StaffCredit_RunCredits(CREDITS_FROM_PAGE(5));
    Event_SetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_BEAMS_MEET);
    break;
  case STAFF_CREDIT_BEAMS_MEET:
    StaffCredit_ShowBeacon(BEACON_BEAMS_MEET);
    Event_SetDestination(SCENE_MT_ALEPH_SUMMIT, SUMMIT_ALEX_TAKES_THE_POWER);
    break;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "shared/tile_sprite_reset.h"

#include "shared/world_map_objects.c"

/// The ship's flicker and its sparks while it powers up (flight mode 1).
void StaffCredit_FlashPoweringUp(struct Actor *actor) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_SpawnSparkPair(actor);
  }
}

/// The step through gStaffCredit_GlowColorModes, every other frame.
extern s32 gStaffCredit_GlowColorStep;

/// The ship's glow and its trail in flight (flight mode 2).
void StaffCredit_GlowInFlight(struct Actor *actor) {
  if (gFrameCount & 1) {
    Actor_SetSpriteColorMode(
        actor, gStaffCredit_GlowColorModes[gStaffCredit_GlowColorStep]);
    gStaffCredit_GlowColorStep = (gStaffCredit_GlowColorStep + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    WorldMap_SpawnParticle(actor);
  }
}

/// Sets the ship's look by flight mode: 0 plain, 1 powering up, 2 in flight.
void Actor_SetFlightMode(struct Actor *actor, s32 mode) {
  if (mode == 0) {
    actor->update = 0;
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  } else if (mode == 1) {
    actor->update = (ObjectUpdate)StaffCredit_FlashPoweringUp;
  } else {
    actor->update = (ObjectUpdate)StaffCredit_GlowInFlight;
    Event_PlaySound(SE_TAKE_FLIGHT);
  }
}

#include "shared/leader_search.h"

/// Unused: the leader's ship lands, plain again.
UNUSED void StaffCredit_EndFlight(void) {
  Actor_SetFlightMode(Actor_Get(FieldParty_GetLeaderActor()), 0);
}

/// The copy of the world map's WorldMap_UpdateTransitionBlend, which
/// StaffCredit_PlayLighthouseTransition runs each frame: every other two
/// frames the blend alternates between two strengths.
void StaffCredit_UpdateTransitionBlend(void) {
  Scene_QueueIoWrite16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2) {
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA,
                         gStaffCredit_TransitionBlend | (BLEND_WEIGHT_MAX - 4));
  } else {
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA,
                         gStaffCredit_TransitionBlend | BLEND_WEIGHT_MAX);
  }
}

static inline void Camera_Move(FIXED x, FIXED y, FIXED z, s32 mode) {
  FieldCamera_MoveToFX(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
#include "shared/camera_jitter.h"

/// Unused: the copy of the world map's WorldMap_PlayLighthouseTransition, the
/// view of a lighthouse lit, which the credits never call.
UNUSED void StaffCredit_PlayLighthouseTransition(s32 mode) {
  u8 *buffer;

  System_Wait(1);
  Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
  gFieldWork->transition = 0;
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transitionFrames = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    MapGrid_SetShake(2, 3, 1);
    Event_PlaySound(SE_RUMBLE);
  }
  WorldMap_Freeze();
  buffer = Heap_AllocEwram(16384);
  Decompress_TaggedLz(gWorldMapPackedTiles, buffer);
  Decompress_TaggedLz(gWorldMapPackedPictures,
                      buffer + PICTURE_FIRST_TILE * TILE_SIZE_4BPP);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                      (u32)(gWorldMapPalettes + mode * PALETTE_COLORS * 2),
                      DMA_COPY_HALFWORDS(PALETTE_COLORS));
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count < (s32)ARRAY_COUNT(q->entries)) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)buffer;
      q->count = count + 1;
      *entry++ = PICTURE_TILES;
      *entry = DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4);
    }
    IRQ_UNLOCK(saved);
  }
  Task_AddFrame(StaffCredit_UpdateTransitionBlend, TASK_PASS_FRAME);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_GLITTER);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 14);
  PUSH_PICTURE(6);
  FieldEvent_Wait(2);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 13);
  PUSH_PICTURE(5);
  FieldEvent_Wait(2);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 12);
  PUSH_PICTURE(4);
  FieldEvent_Wait(2);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 11);
  PUSH_PICTURE(3);
  FieldEvent_Wait(2);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 10);
  PUSH_PICTURE(2);
  FieldEvent_Wait(2);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 9);
  PUSH_PICTURE(1);
  FieldEvent_Wait(2);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 8);
  PUSH_PICTURE(0);
  FieldEvent_Wait(140);
  PUSH_PICTURE(1);
  FieldEvent_Wait(4);
  PUSH_PICTURE(2);
  FieldEvent_Wait(4);
  PUSH_PICTURE(3);
  FieldEvent_Wait(4);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 9);
  PUSH_PICTURE(4);
  FieldEvent_Wait(4);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 10);
  PUSH_PICTURE(5);
  FieldEvent_Wait(4);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 11);
  PUSH_PICTURE(6);
  FieldEvent_Wait(4);
  gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 12);
  PUSH_PICTURE(7);
  if (mode == 1) {
    MapGrid_SetShake(2, 2, 1);
    Event_PlaySound(SE_RUMBLE);
    gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 13);
    FieldEvent_Wait(4);
    gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 14);
    FieldEvent_Wait(4);
    gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 15);
    FieldEvent_Wait(4);
    gStaffCredit_TransitionBlend = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    FieldEvent_Wait(45);
    FieldTask_Remove(StaffCredit_UpdateTransitionBlend);
    DebugText_Clear();
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA, gStaffCredit_TransitionBlend);
    Scene_QueueIoWrite16((u32)&REG_BLDCNT, 0);
  } else {
    gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 13);
    FieldEvent_Wait(4);
    gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 14);
    FieldEvent_Wait(4);
    gStaffCredit_TransitionBlend = BLEND_ALPHA(0, 15);
    FieldEvent_Wait(4);
    gStaffCredit_TransitionBlend = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    FieldEvent_Wait(45);
    FieldTask_Remove(StaffCredit_UpdateTransitionBlend);
    DebugText_Clear();
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA, gStaffCredit_TransitionBlend);
    Scene_QueueIoWrite16((u32)&REG_BLDCNT, 0);
  }
  Heap_Release(buffer);
  Event_SetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

/// Unused: the copy of the world map's WorldMap_FlashWithSparks: the actor
/// flashes white, with a pair of sparks every sixteenth frame.
UNUSED void StaffCredit_FlashWithSparks(struct Actor *actor) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    StaffCredit_SpawnGrowingPair(actor);
  }
}

/// Cycles an actor's colour mode, sending out a growing pair of sparks every
/// sixteenth frame.
void Actor_AnimateAndEmit(struct Actor *actor) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Actor_SetSpriteColorMode(actor, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    StaffCredit_SpawnGrowingPair(actor);
}

#include "shared/actor_animate.h"

#include "shared/actor_grow_up.h"

#include "shared/actor_grow_down.h"

/// Adds a pair of sparks under an actor, one growing and one shrinking.
void StaffCredit_SpawnGrowingPair(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct Sprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  s32 i;

  for (i = 0; i < 2; i++) {
    child = (FieldActor_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed,
                               parent->y.fixed, parent->z.fixed));
    pair[i] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->watchedActor = parent;
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = work->vramBlock;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            gVramCacheEntries[sprite->slot].offset / TILE_SIZE_4BPP &
            OBJ_TILE_MASK;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->picture = 0;
      }
    }
  }
  {
    struct Actor *p = pair[0];
    struct FieldSprite *sp = p->sprite;

    p->update = (void *)Actor_GrowDown;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->sprite;

    sp->priority = 1;
    p->update = (void *)Actor_GrowUp;
    p->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
}

#include "shared/world_map_gauge.c"

s32 gWorldMap_SparkScript[] = {
    SC_CALL((s32)WorldMap_RiseFromAnchor),
    SC_RELEASE,
};

// clang-format off
#include "shared/world_map_particle_script.c"

/// The colour modes the ship's glow steps through, one every other frame.
u32 gStaffCredit_GlowColorModes[] = {
    SPRITE_COLOR_NORMAL, SPRITE_COLOR_BLUE_TINT, SPRITE_COLOR_NORMAL, SPRITE_COLOR_GREY,
};
// clang-format on

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 9850, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 10150, 0, 8180, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 10144, 0, 8330, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 10144, 0, 8240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 10137, 0, 8602, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 10423, 0, 8354, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 10190, 0, 8883, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 10578, 0, 8952, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 10635, 0, 9040, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 10271, 0, 9420, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 10380, 0, 9370, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 10460, 0, 9400, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 11125, 0, 10695, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {15, CONDITION_ALWAYS, 12070, 0, 10470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {16, CONDITION_ALWAYS, 11880, 0, 10090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {17, CONDITION_ALWAYS, 10976, 0, 9960, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {18, CONDITION_ALWAYS, 11736, 0, 9530, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {19, CONDITION_ALWAYS, 11910, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {20, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {21, CONDITION_ALWAYS, 9240, 0, 9532, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {22, CONDITION_ALWAYS, 9344, 0, 9530, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {23, CONDITION_ALWAYS, 9148, 0, 9048, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {24, CONDITION_ALWAYS, 9148, 0, 8988, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {25, CONDITION_ALWAYS, 9200, 0, 8440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {26, CONDITION_ALWAYS, 9060, 0, 8290, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {27, CONDITION_ALWAYS, 9256, 0, 7950, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {28, CONDITION_ALWAYS, 9546, 0, 8316, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {29, CONDITION_ALWAYS, 12770, 0, 11570, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {30, CONDITION_ALWAYS, 11724, 0, 8064, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {31, CONDITION_ALWAYS, 11680, 0, 6678, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {32, CONDITION_ALWAYS, 13486, 0, 7482, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {33, CONDITION_ALWAYS, 12921, 0, 9639, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {34, CONDITION_ALWAYS, 12921, 0, 9752, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {35, CONDITION_ALWAYS, 12770, 0, 10220, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {36, CONDITION_ALWAYS, 14478, 0, 8410, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {37, CONDITION_ALWAYS, 14250, 0, 8728, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {38, CONDITION_ALWAYS, 13370, 0, 6070, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {39, CONDITION_ALWAYS, 13452, 0, 6204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {40, CONDITION_ALWAYS, 10898, 0, 6300, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {41, CONDITION_ALWAYS, 11198, 0, 6132, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {42, CONDITION_ALWAYS, 12472, 0, 7592, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {43, CONDITION_ALWAYS, 12090, 0, 7165, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {44, CONDITION_ALWAYS, 12972, 0, 1240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {45, CONDITION_ALWAYS, 13088, 0, 1090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {46, CONDITION_ALWAYS, 9490, 0, 10960, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {47, CONDITION_ALWAYS, 5836, 0, 10120, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {48, CONDITION_ALWAYS, 8582, 0, 7420, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {49, CONDITION_ALWAYS, 6314, 0, 6223, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {50, CONDITION_ALWAYS, 8848, 0, 4650, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {51, CONDITION_ALWAYS, 8970, 0, 7500, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {52, CONDITION_ALWAYS, 7256, 0, 8325, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {53, CONDITION_ALWAYS, 7280, 0, 8180, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {54, CONDITION_ALWAYS, 7714, 0, 7760, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {55, CONDITION_ALWAYS, 7275, 0, 7365, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {56, CONDITION_ALWAYS, 7383, 0, 5753, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {57, CONDITION_ALWAYS, 7504, 0, 5650, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {58, CONDITION_ALWAYS, 7662, 0, 5558, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {59, CONDITION_ALWAYS, 8960, 0, 5856, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {60, CONDITION_ALWAYS, 8933, 0, 3800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {61, CONDITION_ALWAYS, 8916, 0, 1640, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {62, CONDITION_ALWAYS, 9000, 0, 1307, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {63, CONDITION_ALWAYS, 9040, 0, 1245, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {64, CONDITION_ALWAYS, 1992, 0, 7272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {65, CONDITION_ALWAYS, 2032, 0, 7600, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {67, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {68, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {69, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {71, CONDITION_ALWAYS, 10720, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {72, CONDITION_ALWAYS, 7279, 0, 7307, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {80, CONDITION_ALWAYS, 11754, 0, 8030, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {81, CONDITION_ALWAYS, 11700, 0, 8034, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {82, CONDITION_ALWAYS, 11596, 0, 10204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {83, CONDITION_ALWAYS, 10240, 0, 9142, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {84, CONDITION_ALWAYS, 9020, 0, 882, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {85, CONDITION_ALWAYS, 9288, 0, 9434, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {86, CONDITION_ALWAYS, 9288, 0, 9640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {87, CONDITION_ALWAYS, 12640, 0, 4390, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {88, CONDITION_ALWAYS, 7868, 0, 9120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {89, CONDITION_ALWAYS, 9479, 0, 5060, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {98, CONDITION_ALWAYS, 8916, 0, 1640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {100, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {101, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {102, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {103, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_BEACON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    9020, 0, 882, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};

/// Where each beacon flight's camera starts, in pixels
/// (StaffCredit_ShowBeacon).
// clang-format off
LATE_DATA s16 gStaffCredit_FlightStarts[][2] = {
    {9276, 1782}, {10754, 3137}, {10160, 6268}, {8243, 7334}, {8926, 5100},
};

/// Where each flight ends, over its lighthouse, in pixels.
LATE_DATA s16 gStaffCredit_FlightEnds[][2] = {
    {9020, 782}, {10101, 4284}, {10179, 7270}, {7255, 7305}, {9801, 5100},
};

/// Where the camera flies back to from each beacon, in pixels.
LATE_DATA s16 gStaffCredit_FlightReturns[][2] = {
    {9570, 2573}, {12179, 4639}, {8727, 7317}, {9751, 6829}, {9751, 5220},
};

/// Each flight's zoom as it starts, as a 16.16 ratio.
LATE_DATA s32 gStaffCredit_StartZooms[] = {
    FIXED_ONE / 2, FIXED_ONE * 4 / 5, FIXED_ONE * 3 / 2,
    FIXED_ONE * 3 / 5, FIXED_ONE * 9 / 10,
};

/// Each flight's zoom over its beacon, as a 16.16 ratio.
LATE_DATA s32 gStaffCredit_EndZooms[] = {
    FIXED_ONE, FIXED_ONE * 4 / 5, FIXED_ONE * 7 / 10,
    FIXED_ONE * 13 / 10, FIXED_ONE * 3 / 5,
};

/// Each flight's length, in frames.
LATE_DATA u8 gStaffCredit_FlightFrames[] = {
    130, 170, 120, 126,
};

/// How far the camera's pitch turns on each of a flight's last 64 frames, read
/// from the second entry on by stage.
LATE_DATA u8 gStaffCredit_FlightTurns[] = {
    107, 108, 64, 108, 108, 108,
};

/// The beacon glow's blend weights as it fades in.
LATE_DATA u16 gStaffCredit_GlowFadeIn[] = {
    BLEND_ALPHA(0, 14), BLEND_ALPHA(0, 13), BLEND_ALPHA(0, 12), BLEND_ALPHA(0, 11),
    BLEND_ALPHA(0, 10), BLEND_ALPHA(0, 9),  BLEND_ALPHA(0, 8),
};
// clang-format on

LATE_DATA s8 gStaffCredit_GlowPositions[][2] = {
    {22, -4}, {8, -8}, {2, -28}, {14, 12}, {8, 8}, {0, 0},
};

LATE_DATA s32 gWorldMap_PlacedActorScript[] = {
    SC_CALL(TileSprite_Reset),
    SC_END,
    0,
};

LATE_DATA s32 gStaffCredit_GlowColorStep = 0;
