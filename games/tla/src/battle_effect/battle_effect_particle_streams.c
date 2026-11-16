/// @file
/// The particle streams of Meteor, the formation objects summons place,
/// palette brightening, and an unused second copy of the Nereid summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "enemy_ids.h"
#include "sounds.h"

/// The particle streams' modes (BattleEffect_RunParticleStreams).
enum ParticleStreamsMode {
  PARTICLE_STREAMS_METEOR,     ///< one spawned sprite falls
  PARTICLE_STREAMS_OUTER_SPACE ///< the user leaps, and two animated sprites
                               ///< fall in the lightning bolt's colours
};

/// BattleEffect_StartActorMotion with fixed speeds and a random distance:
/// unless the unit is the Fusion Dragon or one of three of the Doom Dragon's
/// forms, the actor leaps back to 130-193% of its anchor's x, then takes
/// mode 5.
void BattleEffect_StartActorMotionRandomDistance(s32 id) {
  struct BattleObject *slot;
  struct MotionObject *object;
  u32 enemy;

  slot = BattleObject_GetFar(id);
  object = slot->object;
  enemy = Unit_Get(id)->enemyId;
  if (enemy != ENEMY_FUSION_DRAGON && enemy != ENEMY_DOOM_DRAGON_A &&
      enemy != ENEMY_DOOM_DRAGON_E && enemy != ENEMY_DOOM_DRAGON_H) {
    s32 percent = (Random16() & 63) + 130;

    object->acceleration = toFIXED(8);
    object->speedLimit = toFIXED(10);
    object->velocityY = toFIXED(3) / 2;
    Actor_StartIdleFar(object);
    Actor_SetMoveTargetFar(object, slot->anchorX * percent / 100, 0,
                           slot->anchorZ);
  }
  object->verticalMotionStrength = toFIXED(3) / 5;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SelectAnimationFar(object, 5);
}

// Unused: a second copy of Nereid's summon effect.

/// The map cell buffer holds the embers.
#define EMBERS ((struct Particle *)Ram_MapCellBuffer)
#define EMBER_COUNT 1024
#define PILLAR_SHEET 1764 ///< the fire pillar after the spray in the sheet

#define FORMATION_SIZE 12

#define SCENE_FRAMES 124
#define SKIP_FROM_FRAME 32 ///< A or B jumps from here...
#define SKIP_TO_FRAME 98   ///< ...to here
#define FIRST_SPOUT_FRAME 28
#define SPOUT_FRAME 32 ///< the embers spout and the pillars rise until frame 79
#define SPOUT_END_FRAME 80
#define RESULTS_FRAME 120

/// Unused: nothing reaches it (no handler, summon case or call). A second
/// copy of BattleEffect_RunNereid. As written:
/// a formation of twelve sweeps in from the right; three pillars of fire
/// stream upward and a fountain of embers spouts from the ground, falls, and
/// at the end rises away, while the targets jolt every twelve frames.
///
/// Setup: open the canvas, load the spray and pillar pictures and the ember
/// cells, empty the particle pools, and spawn the formation. Each frame (124,
/// A or B skipping from 33 to 98): the formation glides in and slows; 32
/// embers spout at frame 28 and 16 more a frame from 32 to 79; the spray rolls
/// along the ground until frame 95; the pillars stream up from 32 to 79; each
/// ember grows as it falls (the brighter blitter once it falls) and grows
/// again toward the end; the targets flash and jump every 12 frames from 30 to
/// 79. At the end the formation bursts and its objects are released.
UNUSED void BattleEffect_RunNereidCopy(struct BattleEffectParams *args) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *cells;
  u8 *palette;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  cells = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);

  // The spray's palette and picture, the pillar's picture, the ember cells.
  palette = Resource_GetTableEntry(RESOURCE_WATER_SPRAY_SHEET);
  Dma_Set(palette, (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
          REG_DMA3);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry(RESOURCE_FIRE_PILLAR_SHEET_A);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet + PILLAR_SHEET);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      cells);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct Particle *point;
    struct Particle *ember;
    struct Particle *embers;
    s32 *life;

    // The formation starts off the right edge, gliding left.
    rise = 0;
    focus.x = toFIXED(256);
    focus.y = toFIXED(88);
    velocity.x = -toFIXED(16);
    velocity.y = -toFIXED(4);
    EFFECT_FREE_PARTICLES(k, work->particles, 64);
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }
    life = &EMBERS->variant;
    for (k = 0; k != EMBER_COUNT; k++) {
      *life = -1;
      life += sizeof(struct Particle) / sizeof(s32);
    }
    embers = EMBERS;
    BattleEffect_SelectLivingTargets(work->args);
    wait(1);
    BattleEffect_SpawnObjects(FORMATION_SIZE, SPRITE_NEREID, 2);

    for (frame = 0; frame != SCENE_FRAMES; frame++) {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > SKIP_FROM_FRAME &&
          frame < SKIP_TO_FRAME)
        frame = SKIP_TO_FRAME;
      if (frame == RESULTS_FRAME)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      if (frame <= 15)
        rise += 2;

      // The formation glides in, slowing, and drifts back right as it
      // passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / DAMPING_SCALE;
        velocity.y = velocity.y * 56 / DAMPING_SCALE;
        if (focus.x < toFIXED(120))
          velocity.x += toFIXED(1) / 2;
      }
      BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_NEREID, focus.x,
                                         focus.y);

      // The first spout fills the first 256 slots; then 16 embers a frame
      // from the ground at (32, 96).
      if (frame == FIRST_SPOUT_FRAME) {
        for (k = 0; k != 256; k++) {
          ember = &embers[k];
          if (ember->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            ember->x = ((Trig_Sin(angle) * radius) >> 3) + toFIXED(32);
            ember->y = ((Trig_Cos(angle) * radius) >> 2) + toFIXED(96);
            ember->velocityX = ((Random16() & 63) - 32) << 14;
            ember->velocityY = (-(Random16() & 63) - 8) << 13;
            ember->variant = 0;
          }
        }
      }
      if (frame >= SPOUT_FRAME && frame < SPOUT_END_FRAME) {
        s32 spawned = 0;

        for (k = 0; k != EMBER_COUNT; k++) {
          ember = &embers[k];
          if (ember->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            ember->x = ((Trig_Sin(angle) * radius) >> 3) + toFIXED(32);
            ember->y = ((Trig_Cos(angle) * radius) >> 2) + toFIXED(96);
            ember->velocityX = ((Random16() & 63) - 32) << 14;
            ember->velocityY = (-(Random16() & 63) - 8) << 13;
            ember->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(SE_SCATTER);
      if (frame == SPOUT_FRAME)
        Sound_PlayFar(SE_CRASH);
      if (frame == SPOUT_END_FRAME)
        Sound_PlayFar(SE_CLATTER);

      // The pillars stream upward, each a 104-pixel picture drawn twice to
      // wrap.
      if (frame >= SPOUT_FRAME && frame < SPOUT_END_FRAME) {
        for (k = 0; k != NEREID_COLUMNS; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % 104;

          draw[0](canvas, work->sheet + PILLAR_SHEET,
                  gNereidCopy_Columns[k * 2] - 17,
                  gNereidCopy_Columns[k * 2 + 1] - span - 104,
                  FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
          draw[0](canvas, work->sheet + PILLAR_SHEET,
                  gNereidCopy_Columns[k * 2] - 17,
                  gNereidCopy_Columns[k * 2 + 1] - span, FIRE_PILLAR_WIDTH,
                  span);
        }
      }

      // The spray rolls along the ground, rising for the first 16 frames.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
      }

      // The embers: bigger when falling and toward the end, pulled down by
      // one of four gravities, then up after frame 80; gone below y = 104.
      for (k = 0; k != EMBER_COUNT; k++) {
        ember = &embers[k];
        if (ember->variant >= 0) {
          s32 size = k % 3 + 2;

          if (ember->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[ember->velocityY > 0](
              canvas, cells + gBattleEffect_MotePictures[size - 1],
              FIXED_INTEGER_PART(ember->x) - size / 2,
              FIXED_INTEGER_PART(ember->y) - size, size, size * 2);
          ember->x += ember->velocityX;
          ember->y += ember->velocityY;
          if (frame > SPOUT_END_FRAME)
            ember->velocityY += -toFIXED(1) / 2;
          else
            ember->velocityY += gNereidCopy_Gravity[k & 3];
          ember->velocityX = ember->velocityX * 62 / DAMPING_SCALE;
          ember->velocityY = ember->velocityY * 62 / DAMPING_SCALE;
          ember->variant++;
          if (ember->velocityY > 0 && FIXED_INTEGER_PART(ember->y) > 104)
            ember->variant = -1;
        }
      }

      // The targets flash and jump every 12 frames while the embers spout.
      if (frame < SPOUT_END_FRAME) {
        for (k = 0; k != args->targetCount; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  BattleObject_GetFar(args->targets[k])->object;

              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, TINT_KEEP, 0);
              target->velocityY = toFIXED(9) / 2;
              target->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
            }
            if (phase == 6)
              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_NORMAL,
                                       POSE_DAMAGE, TINT_KEEP, 0);
          }
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    }

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_NEREID, focus.x, focus.y);
    for (k = 0; k != FORMATION_SIZE; k++)
      Sprite_DestroyFar(work->sprites[k]);
  }
  BattleEffect_EndCanvasLayer();
}

extern const struct Scale gFormationObjects_Scale;
extern const struct Scale gFormationObjects_LargeScale;

/// Places the summoned objects of one formation around (x, z): seven
/// formations, the last three (4: eleven objects from signed columns, 5: twelve
/// in a 4x3 grid, 6: eight more), in the order 0, 5, 1, 2, 6, 3, 4.
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x, s32 z) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct Scale normal = gFormationObjects_Scale;
  struct Scale big = gFormationObjects_LargeScale;
  FIXED pos[4];
  s32 i;
  void **objects;

  pos[3] = 0;
  pos[1] = toFIXED(255);
  switch (formation) {
  case 0:
    objects = work->sprites;
    for (i = 0; i != 9; i++) {
      pos[0] = INTtoFIXED((i % 3) * 32) + x;
      pos[2] = INTtoFIXED((i / 3) * 32) + z;
      Render_ApplyProjectedPlacementFar(*objects++, pos, &normal.x, 0);
    }
    break;
  case 5:
    for (i = 0; i != 12; i++) {
      pos[0] = INTtoFIXED((i % 4) * 32) + x;
      pos[2] = INTtoFIXED((i / 4) * 32) + z;
      Render_ApplyProjectedPlacementFar(work->sprites[i], pos, &normal.x, 0);
    }
    break;
  case 1:
    for (i = 0; i != 12; i++) {
      pos[0] = INTtoFIXED(gHelmSplitter_TwelveX[i]) + x - toFIXED(16);
      pos[2] = INTtoFIXED(gHelmSplitter_TwelveZ[i]) + z - toFIXED(32);
      Render_ApplyProjectedPlacementFar(work->sprites[i], pos, &normal.x, 0);
    }
    break;
  case 2:
    for (i = 0; i != 8; i++) {
      pos[0] = INTtoFIXED(gHelmSplitter_RightEightX[i]) + x + toFIXED(16);
      pos[2] = INTtoFIXED(gHelmSplitter_RightEightZ[i]) + z;
      Render_ApplyProjectedPlacementFar(work->sprites[i], pos, &normal.x, 0);
    }
    break;
  case 6:
    for (i = 0; i != 8; i++) {
      pos[0] = INTtoFIXED(gFormationObjects_X[i]) + x;
      pos[2] = INTtoFIXED(gFormationObjects_Z[i]) + z;
      Render_ApplyProjectedPlacementFar(work->sprites[i], pos, &normal.x, 0);
    }
    break;
  case 3:
    for (i = 0; i != 8; i++) {
      pos[0] = INTtoFIXED(gHelmSplitter_SmallEightX[i]) + x;
      pos[2] = INTtoFIXED(gHelmSplitter_SmallEightZ[i]) + z;
      Render_ApplyProjectedPlacementFar(work->sprites[i], pos, &big.x, 0);
    }
    break;
  case 4:
    for (i = 0; i != 11; i++) {
      pos[0] = INTtoFIXED(gFormationObjects_WideX[i]) + x;
      pos[2] = INTtoFIXED(gFormationObjects_WideZ[i]) + z;
      Render_ApplyProjectedPlacementFar(work->sprites[i], pos, &normal.x, 0);
    }
    break;
  }
}

#define SHARD_E 23000

#define IMPACT_BURST_FRAMES 72

#define IMPACT_BURST_FRAGMENT_WIDTH 12 ///< each fragment picture's width
#define IMPACT_BURST_FRAGMENT_HEIGHT 24
#define IMPACT_BURST_FRAGMENTS 3 ///< fragments, work->particles 61 to 63
/// the first fragment in work->particles
#define IMPACT_BURST_FIRST_FRAGMENT 61

#define IMPACT_BURST_DRAWN_LARGE 30  ///< large sparks drawn
#define IMPACT_BURST_DRAWN_SMALL 60  ///< small sparks drawn
#define IMPACT_BURST_LARGE_SPARKS 64 ///< large sparks thrown
#define IMPACT_BURST_SMALL_SPARKS 64 ///< small sparks thrown

/// Opens an impact flash and scatters sparks and fragments, keeping the
/// effect's sprites in formation for the first 36 frames.
void BattleEffect_RunImpactBurst(s32 formation, s32 x, FIXED y) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle additive;
  DrawRectangle maxBlend;
  u8 *sheet;
  s32 orgX;
  struct Particle *p;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  orgX = x;
  x += toFIXED(40);
  x /= 2;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  maxBlend = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_ADDITIVE);
  additive = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FLARE_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLARE_IMAGE, work->sheet + SHARD_E, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (i = 0; i != IMPACT_BURST_LARGE_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 256;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    work->particles[i].x = x;
    work->particles[i].y = y;
    work->particles[i].velocityX = (Trig_Sin(angle) * speed) >> 7;
    work->particles[i].velocityY = -((Trig_Cos(angle) * speed) >> 6);
    work->particles[i].variant = (Random16() & 15) + 16;
  }

  for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].x = x;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].y = y;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityX =
        (Trig_Sin(i * (DEGtoANG(120))) * 32) >> 6;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityY =
        -((Trig_Cos(i * (DEGtoANG(120))) * 32) >> 5);
  }

  for (i = 0; i != IMPACT_BURST_SMALL_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 32;
    s32 angle = Random16() & TRIG_ANGLE_MASK;
    struct Particle *cells = (struct Particle *)Ram_MapCellBuffer;

    cells[i].x = x;
    cells[i].y = y;
    cells[i].velocityX = (Trig_Sin(angle) * speed) >> 6;
    cells[i].velocityY = -((Trig_Cos(angle) * speed) >> 5);
    cells[i].variant = (Random16() & 15) + 20;
  }

  for (frame = 0; frame != IMPACT_BURST_FRAMES; frame++) {
    if (frame == 4)
      Sound_PlayFar(SE_CAST);
    if (frame == 32)
      Sound_PlayFar(SE_POWER_UP);

    if (frame <= 47) {
      s32 cell = (frame - 8) / 5;
      u32 width;
      u32 height;

      if (cell < 0)
        cell = 0;
      maxBlend(
          canvas, work->sheet + gHelmSplitter_ImpactBurstPictures[cell],
          (FIXEDtoINT(x)) -
              ((width = gHelmSplitter_ImpactBurstPictureWidths[cell]) >> 1),
          (FIXEDtoINT(y)) -
              ((height = gHelmSplitter_ImpactBurstPictureHeights[cell]) >> 1),
          width, height);
    }

    for (i = 0, p = work->particles; i != IMPACT_BURST_DRAWN_LARGE; i++, p++) {
      if (frame > i / 2 && p->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        p->variant--;
        Particle_AdvanceWithGravity2D(p, 60, 0);
        size = p->variant / 16 + 3;
        px = FIXED_INTEGER_PART(p->x);
        py = FIXED_INTEGER_PART(p->y);
        additive(canvas, sheet + gBattleEffect_MotePictures[size - 1],
                 px - size / 2, py - size, size, size * 2);
      }
    }

    for (i = 0, p = (struct Particle *)Ram_MapCellBuffer;
         i != IMPACT_BURST_DRAWN_SMALL; i++, p++) {
      if (frame > 35 && p->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        p->variant--;
        Particle_AdvanceWithGravity2D(p, 60, 0);
        size = p->variant / 16 + 1;
        px = FIXED_INTEGER_PART(p->x);
        py = FIXED_INTEGER_PART(p->y);
        additive(canvas, sheet + gBattleEffect_MotePictures[size - 1],
                 px - size / 2, py - size, size, size * 2);
      }
    }

    for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
      struct Particle *shard =
          &work->particles[IMPACT_BURST_FIRST_FRAGMENT + i];

      if (frame >= 36 && frame <= 63) {
        s32 cell;
        s32 px;
        s32 py;

        Particle_AdvanceWithGravity2D(shard, PARTICLE_DAMPING_NONE, 0);
        cell = (frame - 36) / 7;
        px = FIXED_INTEGER_PART(shard->x);
        py = FIXED_INTEGER_PART(shard->y);
        maxBlend(canvas,
                 work->sheet + cell * PICTURE_BYTES(IMPACT_BURST_FRAGMENT) +
                     SHARD_E,
                 px - IMPACT_BURST_FRAGMENT_WIDTH / 2,
                 py - IMPACT_BURST_FRAGMENT_HEIGHT / 2,
                 IMPACT_BURST_FRAGMENT_WIDTH, IMPACT_BURST_FRAGMENT_HEIGHT);
      }
    }

    if (frame <= 35)
      BattleEffect_PlaceFormationObjects(formation, orgX, y);
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  EFFECT_RELEASE_BLITTERS();
}

/// Adds the deltas to BG palette entries 1 to 63, clamped
/// at 0.
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(0, 1);
  s32 i;

  for (i = 0; i != 63; i++) {
    s32 blue = RGB_BLUE(*color);
    s32 green = RGB_GREEN(*color);
    s32 red = RGB_RED(*color);

    blue += blue_delta;
    green += green_delta;
    red += red_delta;
    if (blue > RGB_LEVEL_MAX)
      blue = RGB_LEVEL_MAX;
    if (green > RGB_LEVEL_MAX)
      green = RGB_LEVEL_MAX;
    if (red > RGB_LEVEL_MAX)
      red = RGB_LEVEL_MAX;
    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = RGB(red, green, blue);
  }
}

/// As Palette_BrightenBgEntries over BG palette entries 96 to 223, the deltas
/// red first.
void Palette_BrightenBattleBg(s32 red_delta, s32 green_delta, s32 blue_delta) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);
  s32 i;

  for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
    s32 blue = RGB_BLUE(*color);
    s32 green = RGB_GREEN(*color);
    s32 red = RGB_RED(*color);

    red += red_delta;
    green += green_delta;
    blue += blue_delta;
    if (red > RGB_LEVEL_MAX)
      red = RGB_LEVEL_MAX;
    if (green > RGB_LEVEL_MAX)
      green = RGB_LEVEL_MAX;
    if (blue > RGB_LEVEL_MAX)
      blue = RGB_LEVEL_MAX;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *color++ = RGB(red, green, blue);
  }
}

/// As Palette_BrightenBattleBg, from the battle work's saved palette into BG
/// palette entries 96 to 223.
void Palette_BrightenSavedBattleBg(s32 redDelta, s32 greenDelta,
                                   s32 blueDelta) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);
  u16 *source = Ram_HeapSlots->battleWork->palette;
  s32 i;

  for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
    s32 blue = RGB_BLUE(*source);
    s32 green = RGB_GREEN(*source);
    s32 red = RGB_RED(*source);

    red += redDelta;
    green += greenDelta;
    blue += blueDelta;
    if (red > RGB_LEVEL_MAX)
      red = RGB_LEVEL_MAX;
    if (green > RGB_LEVEL_MAX)
      green = RGB_LEVEL_MAX;
    if (blue > RGB_LEVEL_MAX)
      blue = RGB_LEVEL_MAX;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *color++ = RGB(red, green, blue);
    source++;
  }
}

/// Stops DMA 0, then arms it to copy one palette line from the battle effect
/// work into the BG palette at each horizontal blank.
void BattleEffect_ArmPaletteHBlankDma(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  volatile u16 *channel = REG_DMA0;
  u32 palette;

  channel[DMA_CONTROL_HALF] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
  palette = 0xa0;
  (void)channel[DMA_CONTROL_HALF];
  Dma_Set(work->backdropLines, (void *)(palette << 19),
          DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
}

/// The particle streams in mode 0.
void BattleEffect_RunMeteor(struct BattleEffectParams *args) {
  BattleEffect_RunParticleStreams(args, PARTICLE_STREAMS_METEOR);
}

/// The particle streams in mode 1.
void BattleEffect_RunOuterSpace(struct BattleEffectParams *args) {
  BattleEffect_RunParticleStreams(args, PARTICLE_STREAMS_OUTER_SPACE);
}

#undef EMBERS

// BattleEffect_RunParticleStreams, which Meteor (SUMMON_EFFECT_METEOR and
// SUMMON_EFFECT_NONE) and the outer space scene run, with its nested helpers
// AddFlame and AddMote (unused).

/// The map cell buffer: first a 256 x 128 picture the streams draw into (BG1's
/// tiles), then, in the second act, the flashes and the embers.
#define FLASHES ((struct Particle *)Ram_MapCellBuffer)
#define EMBERS (&FLASHES[128])

/// BattleEffect_LoadBlitter's argument: the blitter mode in the high nibble,
/// the COMPOSITE_ flags in the low one.

#define STREAM_FRAMES 192
#define BLAST_FRAMES 54
#define BLAST_FLASH_FRAME 16 ///< the flashes rise from here
#define BLAST_HIT_FRAME 48

#define PARTICLE_STREAMS_LARGE_GLOW_SIZE 24
#define PARTICLE_STREAMS_SMALL_GLOW_SIZE 8
/// Where the two glows are in the sheet.
#define PARTICLE_STREAMS_LARGE_GLOW 7296
#define PARTICLE_STREAMS_SMALL_GLOW 8000
/// The burst's flashes in PARTICLES, then its embers.
#define PARTICLE_STREAMS_FLASHES 128
#define PARTICLE_STREAMS_EMBER_COUNT 512
/// The mote pictures in the blast sheet, after its seven blast pictures
/// (BLAST_PICTURE_SIZES): two sets, MOTE_SET_BYTES apart.
#define PARTICLE_STREAMS_MOTE_PICTURES (16 * 16 + 32 * 32 + 5 * 48 * 48)

/// Meteor's scene: the ground dissolves, a sun climbs over a ring of orbs while
/// burning drops rain, then embers fountain up and the sun swells over the
/// enemies.
void BattleEffect_RunParticleStreams(struct BattleEffectParams *args,
                                     s32 mode) {
  u8 offsets[128];
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleEffectWork *work;

  // A flame on the ground where a drop lands, in the first free of 16
  // slots after the motes.
  void AddFlame(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != 16; i++) {
      struct Particle *flame = &work->particles[32 + i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        flame->velocityX = velocityX;
        break;
      }
    }
  }

  // A mote in the first free of the 32 slots (unused).
  void AddMote(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != 32; i++) {
      struct Particle *mote = &work->particles[i];

      if (mote->variant == -1) {
        mote->variant = 0;
        mote->x = x;
        mote->y = y;
        break;
      }
    }
  }

  FIXED position[4];
  FIXED vector[3];
  struct ScreenPoint spot;
  struct ScreenPoint *pos;
  struct ScreenPoint point;
  struct Scale scale;
  s32 frame;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);

  // Mode 1: the user leaps and flashes; the streams lean the user's way.
  direction = mode;
  if (direction == PARTICLE_STREAMS_OUTER_SPACE) {
    struct MotionObject *user = BattleObject_GetFar(args->user)->object;

    user->velocityY = toFIXED(10);
    user->verticalMotionStrength = toFIXED(57) / 100;
    BattleEffect_TintBattler(args->user, TINT_KEEP, 2, TINT_KEEP, 0);
    Sound_PlayFar(SE_CRASH);
    direction = args->side == EFFECT_SIDE_PARTY ? mode : -1;
  } else {
    direction = -1;
  }

  // A dark backdrop and an empty canvas.
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);

  // The Fusion Dragon's two halves (Outer Space), or the Meteor.
  if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
    for (i = 0; i != 2; i++) {
      struct Sprite *sprite = Sprite_CreateFar(SPRITE_BATTLE_FUSION_DRAGON +
                                               i * SPRITE_SECOND_PART);

      work->sprites[i] = sprite;
      if (sprite != 0) {
        sprite->flags = 0;
        Sprite_StartAnimationFar(sprite, 2);
        ((struct Sprite *)work->sprites[i])->image.priority = 3;
      }
    }
  } else {
    BattleEffect_SpawnObjects(1, SPRITE_METEOR, 3);
  }
  Resource_LoadAndDecompress(RESOURCE_FLAME_SHEET_B, work->sheet, 1, 1);
  if (mode == PARTICLE_STREAMS_OUTER_SPACE)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
                    SHEET_PALETTE_BYTES);

  // BG1 shows the ground: the map cell buffer filled with colour 1.
  IWRAM_FILL_WORDS(Ram_MapCellBuffer, 256 * 128, CANVAS_FILL_WORD(1));
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapCellBuffer,
                  BATTLE_BACKGROUND_BYTES);
  wait(1);
  REG_BLDCNT = 0;
  REG_BG2PA = Q_8_8(1);
  REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;

  // 63 pale colours, and 256 of them scattered over the canvas.
  for (i = 0; i != 63; i++) {
    s32 red = (Random16() & 15) + 16;
    s32 green = (Random16() & 15) + 16;
    s32 blue = (Random16() & 15) + 16;

    BG_PLTT_COLOR(8, i) = RGB(red, green, blue);
  }
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);

  for (i = 0; i != 256; i++) {
    s32 x = Random16() & 127;
    s32 y = Random16() & 127;
    s32 shade = (Random16() & 63) + 64;

    ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_SelectLivingTargets(work->args);

  // BG2 scrolls a pixel every other frame, down and toward the user's side.
  work->bg2x = 0;
  work->bg2y = 0;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = direction << 7;
  work->scrollStepY = 256;
  Task_Add(BattleEffect_AdvanceBg2OnInterval, TASK_PASS_FRAME + 127);
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The ground dissolves: each column starts at its own random row, and the
  // rows go faster and faster.
  for (i = 0; i != 128; i++)
    offsets[i] = Random16() & 63;
  {
    s32 limit = 0;
    s32 step = 1;
    s32 row = 0;

    do {
      limit += step / 2;
      step += 4;
      for (; row != limit; row++) {
        s32 x;

        for (x = 0; x != 256; x++) {
          s32 y = row - offsets[x & 127];

          if (y >= 0)
            if (y <= 127)
              Ram_MapCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                (x & 7)] = 0;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    } while (limit <= 191);
  }
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    struct BattleBackgroundWork *transition = Ram_HeapSlots->battleBackground;
    s32 transferMode = CANVAS_COPY_AND_DARKEN;

    gBgScroll[1].x = 0;
    gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7,
                              COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLITTER_MAX);
    draw = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_SET_TRANSFER(work, transferMode, CANVAS_FILL_WORD(2));
    Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_FRAME + 126);
    EFFECT_FREE_PARTICLES(i, work->particles, 64);
    transition->busy = 1;
    work->frame = 0;
    frame = 0;
    do {
      s32 angle = frame << 8;
      s32 count = work->frame;
      u8 *base = (u8 *)work;
      s32 dividend;
      s32 heat;
      u16 *line;
      s32 size;
      s32 x;
      s32 y;

      /* FAKEMATCH(temporary): the division written out on a copy of count. */
      dividend = count;
      /* FAKEMATCH(dead): updates that cancel out end CSE's knowledge of count.
       */
      if (base) {
        count++;
        count--;
      }
      if (dividend < 0)
        dividend += 3;
      heat = dividend >> 2;
      /* FAKEMATCH(barrier): count and base through an empty asm clobbering r1
       * and r3. */
      asm("" : "=r"(base) : "0"(base), "r"(count) : "r1", "r3");
      line = ((struct BattleEffectWork *)base)->backdropLines;

      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 16)
          break;
      } else {
        if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 4)
          break;
      }
      {
        s32 hold;

        /* FAKEMATCH(barrier): hold set by an empty asm takes r7 across the
         * call. */
        asm("" : "=r"(hold));
        /* FAKEMATCH(barrier): an empty asm that clobbers r6 before the call. */
        asm("" : : : "r6");
        if (frame == 0)
          Sound_PlayFar(SE_RUMBLE);
        /* FAKEMATCH(dead): updates of hold that cancel out. */
        if (hold) {
          hold++;
          hold--;
        }
      }

      // The heat palette: black at the top and bottom, rising through red
      // to yellow as the frames pass.
      for (i = 0; i != 15; i++)
        *line++ = 0;
      for (; i != 135; i++) {
        s32 green = i - 16;
        s32 quarter = green / 4;
        s32 level = quarter + heat;
        s32 red;

        /* FAKEMATCH(dead): updates of quarter that cancel out. */
        if (quarter) {
          quarter++;
          quarter--;
        }
        red = level - 32;
        green = level - 80;

        if (red < 0)
          red = 0;
        if (red > RGB_LEVEL_MAX)
          red = RGB_LEVEL_MAX;
        if (green < 0)
          green = 0;
        if (green > RGB_LEVEL_MAX)
          green = RGB_LEVEL_MAX;
        *line++ = (red << 10) | (green << 5) | (green >> 1);
      }
      for (; i != DISPLAY_HEIGHT; i++)
        *line++ = 0;

      // The sun (or the flames) climbs, growing.
      if (direction == 1)
        x = frame / 4;
      else
        x = 64 - frame / 4;
      y = 96 - frame;
      position[3] = 0;
      position[1] = toFIXED(255);
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        size = angle + FIXED_ONE * 5 / 8;
        scale.x = size;
        scale.y = size;
        position[0] = INTtoFIXED(x) + toFIXED(80);
        position[2] = INTtoFIXED(64 - y);
        Render_ApplyProjectedPlacementFar(work->sprites[0], position, &scale,
                                          0);
        Render_ApplyProjectedPlacementFar(work->sprites[1], position, &scale,
                                          0);
      } else {
        size = angle + FIXED_ONE;
        scale.x = size;
        scale.y = size;
        position[0] = INTtoFIXED(x) + toFIXED(96);
        position[2] = INTtoFIXED(96 - y);
        Render_ApplyProjectedPlacementFar(work->sprites[0], position, &scale,
                                          0);
      }

      // A new mote on the sun's rim.
      for (i = 0, y = 32 - y; i != 32; i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + DEGtoANG(90);

          mote->variant = 0;
          mote->x =
              INTtoFIXED(x + 96) + Trig_Sin(angle) * 30 / FIXED_ONE * size;
          mote->y = INTtoFIXED(y) - Trig_Cos(angle) * 30 / FIXED_ONE * size;
          break;
        }
      }

      // The ring of seven orbs, turning.
      point.x = 0;
      point.y = 0;
      point.depth = toFIXED(512);
      Matrix_Initialize();
      Matrix_TranslateVector(&point.x);
      Matrix_RotateZ(TRIG_TURN / 32);
      Matrix_RotateY(frame << 8);
      for (i = 0; i != 7; i++) {
        vector[0] = INTtoFIXED(gParticleStreams_OrbitPoints[i][0] / 2);
        vector[1] = INTtoFIXED(gParticleStreams_OrbitPoints[i][1] + frame);
        vector[2] = INTtoFIXED(gParticleStreams_OrbitPoints[i][2] / 2);
        EffectPosition_ProjectPoint(vector, pos = &spot);
        pos->x = FIXED_INTEGER_PART(pos->x) + 128;
        pos->y = FIXED_INTEGER_PART(pos->y) + 60;
        draw(Ram_MapCellBuffer, work->sheet + PARTICLE_STREAMS_SMALL_GLOW,
             pos->x - PARTICLE_STREAMS_SMALL_GLOW_SIZE / 2,
             pos->y - PARTICLE_STREAMS_SMALL_GLOW_SIZE / 2,
             PARTICLE_STREAMS_SMALL_GLOW_SIZE,
             PARTICLE_STREAMS_SMALL_GLOW_SIZE);
      }

      // The drops fall, and each becomes a flame where it lands.
      if (direction == 1)
        x = frame / 4 - 16;
      else
        x = 16 - frame / 4;
      y = frame - 96;
      for (i = 0; i != 7; i++) {
        s32 drop = gParticleStreams_FireballPoints[i][1] + y;

        if (drop <= 93)
          draw(Ram_MapCellBuffer, work->sheet + PARTICLE_STREAMS_LARGE_GLOW,
               gParticleStreams_FireballPoints[i][0] + x -
                   PARTICLE_STREAMS_LARGE_GLOW_SIZE / 2,
               drop - PARTICLE_STREAMS_LARGE_GLOW_SIZE / 2,
               PARTICLE_STREAMS_LARGE_GLOW_SIZE,
               PARTICLE_STREAMS_LARGE_GLOW_SIZE);
        else if (drop <= 95)
          AddFlame(INTtoFIXED(gParticleStreams_FireballPoints[i][0] + x),
                   INTtoFIXED(drop), 1);
      }

      // The motes drift up and away, six pictures each.
      for (i = 0; i != 32; i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->variant >= 0) {
          draw(Ram_MapCellBuffer, work->sheet + (mote->variant << 10),
               FIXED_INTEGER_PART(mote->x) - 16,
               FIXED_INTEGER_PART(mote->y) - 16, WATER_WAVE_WIDTH,
               WATER_WAVE_HEIGHT);
          mote->x -= direction * (toFIXED(5) / 4);
          mote->y -= toFIXED(5);
          mote->variant++;
          if (mote->variant == 6)
            mote->variant = -1;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
      frame++;
      work->frame++;
    } while (frame != STREAM_FRAMES);
    wait(1);
    transition->busy = 0;
    Task_Remove(BattleEffect_AdvanceBg2OnInterval);
    Task_Remove(BattleEffect_ArmPaletteHBlankDma);
    Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
    gBgScroll[1].x = savedX;
    gBgScroll[1].y = savedY;
  }

  // Act two: the blast.
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_SetupBlendedDisplay();
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2CNT =
      BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET, work->sheet, 1, 0);
  for (i = 0; i != 32; i++) {
    struct Particle *mote = &work->particles[i];

    mote->x = Random16() & 127;
    mote->y = (Random16() & 127) + 127;
  }
  for (i = 0; i != PARTICLE_STREAMS_FLASHES; i++) {
    struct Particle *flash = &FLASHES[i];

    SET_POSITION(flash, 0, 0, 0);
    flash->velocityX = ((Random16() & 255) - 127) << 12;
    flash->velocityY = (Random16() & 255) << 11;
    flash->velocityZ = ((Random16() & 255) - 127) << 12;
    flash->variant = 0;
  }
  for (i = 0; i != PARTICLE_STREAMS_EMBER_COUNT; i++) {
    struct Particle *ember = &EMBERS[i];

    SET_POSITION(ember, 0, 0, 0);
    ember->velocityX = ((Random16() & 255) - 128) << 13;
    ember->velocityY = (Random16() & 255) << 11;
    ember->velocityZ = ((Random16() & 255) - 128) << 13;
    ember->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));

  for (frame = 0; frame != BLAST_FRAMES; frame++) {
    s32 size = (frame + 464) << 8;
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 elapsed = frame - BLAST_FLASH_FRAME;
    struct Scale scale;
    s32 x;
    s32 y;
    s32 grow;

    if (elapsed > 19)
      Palette_BrightenBgEntries(2, 2, 2);
    if (frame == 0)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == 40)
      Sound_PlayFar(SE_CRASH);
    if (frame == BLAST_HIT_FRAME) {
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        Sprite_DestroyFar(work->sprites[0]);
        Sprite_DestroyFar(work->sprites[1]);
        BattleObject_PutFar();
      }
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    // The embers fountain up from the ground in front of the camera.
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    for (i = 0; i != 64; i++) {
      struct Particle *ember = &EMBERS[i];

      if (ember->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint(&ember->x, &point);
        point.x >>= 1;
        if (point.depth <= 159)
          point.depth = 160;
        if (point.depth > 799)
          point.depth = 799;
        size = 9 - (point.depth - 160) / 64;
        draw(canvas,
             work->sheet +
                 (gBattleEffect_MotePictures[size - 1] +
                  (i & 1) * MOTE_SET_BYTES) +
                 PARTICLE_STREAMS_MOTE_PICTURES,
             point.x - size / 2, point.y - size, size, size * 2);
        Particle_AdvanceWithGravity3D(ember, PARTICLE_DAMPING_NONE,
                                      -toFIXED(1) / 8);
        if (ember->y <= toFIXED(20)) {
          ember->x = 0;
          ember->z = 0;
          ember->y = toFIXED(20);
          ember->velocityX = ((Random16() & 63) - 32) << 15;
          ember->velocityY = (Random16() & 63) << 13;
          ember->velocityZ = ((Random16() & 63) - 32) << 15;
        }
      }
    }

    // The motes rise.
    for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
      struct Particle *mote = &work->particles[i];
      u32 rise = i & 7;
      s32 size = rise + 3;

      draw(canvas,
           work->sheet +
               (gBattleEffect_MotePictures[size - 1] +
                (i & 1) * MOTE_SET_BYTES) +
               PARTICLE_STREAMS_MOTE_PICTURES,
           mote->x - size / 2, mote->y - size, size, size * 2);
      mote->y = mote->y - rise - 8;
      if (mote->y < -10)
        mote->y = 128;
    }

    // The flashes rise from the target, three more every frame.
    for (i = 0; i != 64; i++) {
      struct Particle *flash = &FLASHES[i];

      if (i / 3 < elapsed && flash->y >= 0) {
        EffectPosition_ProjectPoint(&flash->x, &point);
        point.x >>= 1;
        if ((u32)flash->variant <= 13) {
          s32 cell = flash->variant / 2;

          draw(canvas, work->sheet + gParticleStreams_FlashPictures[cell],
               point.x - gParticleStreams_FlashSizes[cell] / 2,
               point.y - gParticleStreams_FlashSizes[cell] / 2,
               gParticleStreams_FlashSizes[cell],
               gParticleStreams_FlashSizes[cell]);
        }
        flash->variant++;
        if (flash->variant == 14) {
          flash->y = toFIXED(20);
          flash->x = 0;
          flash->z = INTtoFIXED((Random16() & 255) - 127);
          flash->velocityX = 0;
          flash->velocityY = (Random16() & 255) << 11;
          flash->velocityZ = 0;
          flash->variant = 0;
        } else {
          Particle_AdvanceWithGravity3D(flash, PARTICLE_DAMPING_NONE, 1);
        }
      }
    }

    // The sun swells over the enemies.
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    grow = (frame << 8) + FIXED_ONE * 2;
    position[3] = 0;
    position[1] = toFIXED(255);
    if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
      scale.x = size;
      scale.y = size;
      position[0] = INTtoFIXED(x) + toFIXED(96);
      position[2] = INTtoFIXED(96 - y);
      Render_ApplyProjectedPlacementFar(work->sprites[0], position, &scale, 0);
      Render_ApplyProjectedPlacementFar(work->sprites[1], position, &scale, 0);
    } else {
      scale.x = grow;
      scale.y = grow;
      position[0] = INTtoFIXED(x) + toFIXED(96);
      position[2] = INTtoFIXED(96 - y);
      Render_ApplyProjectedPlacementFar(work->sprites[0], position, &scale, 0);
    }
    work->shakeFrames = 1;
    BattleCamera_Shake(8, 8);
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  if (mode == PARTICLE_STREAMS_METEOR)
    Sprite_DestroyFar(work->sprites[0]);
  BattleEffect_EndCanvasLayer();
}
