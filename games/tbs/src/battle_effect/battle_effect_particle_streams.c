/// @file
/// Meteor and Outer Space.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define PARTICLE_STREAMS_FALL_FRAMES 192 ///< frames of the fall
#define PARTICLE_STREAMS_BURST_FRAMES 54 ///< frames of the burst

/// The particle streams' modes (BattleEffect_RunParticleStreams).
enum ParticleStreamsMode {
  PARTICLE_STREAMS_METEOR,     ///< one spawned sprite falls
  PARTICLE_STREAMS_OUTER_SPACE ///< the user leaps, and two animated sprites
                               ///< fall in the lightning bolt's colours
};

/// The pictures in the flame sheet (ResourceId_FlameSheetB), each starting
/// where the one before ends: six flames, then three large and three small
/// glows, each glow brighter at its centre than the one before. The fall draws
/// only the six and the brightest glow of each size.
#define PARTICLE_STREAMS_FLAME_SIZE 32
#define PARTICLE_STREAMS_LARGE_GLOW_SIZE 24
#define PARTICLE_STREAMS_SMALL_GLOW_SIZE 8
/// Where each picture begins in the particle streams' sheet, in bytes.
enum ParticleStreamsSheetPicture {
  /// The six 32x32 flames, in a row
  PARTICLE_STREAMS_FLAMES = 0,
  /// The brightest 24x24 glow
  PARTICLE_STREAMS_LARGE_GLOW = PARTICLE_STREAMS_FLAMES +
      6 * PARTICLE_STREAMS_FLAME_SIZE * PARTICLE_STREAMS_FLAME_SIZE +
      2 * PARTICLE_STREAMS_LARGE_GLOW_SIZE * PARTICLE_STREAMS_LARGE_GLOW_SIZE,
  /// The brightest 8x8 glow
  PARTICLE_STREAMS_SMALL_GLOW = PARTICLE_STREAMS_LARGE_GLOW +
      PARTICLE_STREAMS_LARGE_GLOW_SIZE * PARTICLE_STREAMS_LARGE_GLOW_SIZE +
      2 * PARTICLE_STREAMS_SMALL_GLOW_SIZE * PARTICLE_STREAMS_SMALL_GLOW_SIZE,
  /// Past the sheet's end: the backdrop's 160 colours, one a line, that the
  /// fall writes there.
  PARTICLE_STREAMS_BACKDROP_LINES = PARTICLE_STREAMS_SMALL_GLOW +
      PARTICLE_STREAMS_SMALL_GLOW_SIZE * PARTICLE_STREAMS_SMALL_GLOW_SIZE
};

/// Brightens background colours 1 to 63 by a step in each channel.
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep) {
  u16 *color = (u16 *)&BG_PLTT[1];
  s32 i;

  for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
    s32 blue = RGB_BLUE(*color);
    s32 green = RGB_GREEN(*color);
    s32 red = RGB_RED(*color);

    blue += blueStep;
    green += greenStep;
    red += redStep;
    if (blue > RGB_LEVEL_MAX)
      blue = RGB_LEVEL_MAX;
    if (green > RGB_LEVEL_MAX)
      green = RGB_LEVEL_MAX;
    if (red > RGB_LEVEL_MAX)
      red = RGB_LEVEL_MAX;
    *color++ = RGB(red, green, blue);
  }
}

/// Streams the fall's backdrop colours into colour 0, one each line.
void BattleEffect_ArmPaletteHBlankDma(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(work->sheet + PARTICLE_STREAMS_BACKDROP_LINES, BG_PLTT,
          DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
}

/// The battle dissolves into a starfield and a meteor falls through it in
/// flames among falling fireballs, then bursts in embers and flashes.
///
/// Abilities: Meteor.
void BattleEffect_RunMeteor(struct BattleEffectParams *args) {
  BattleEffect_RunParticleStreams(args, PARTICLE_STREAMS_METEOR);
}

/// The Fusion Dragon leaps out of sight and falls back through a starfield in
/// flames, then bursts in embers and flashes, all in the lightning bolt's
/// colours.
///
/// Abilities: Outer Space, the Fusion Dragon's.
void BattleEffect_RunOuterSpace(struct BattleEffectParams *args) {
  BattleEffect_RunParticleStreams(args, PARTICLE_STREAMS_OUTER_SPACE);
}

/// The burst's flashes in PARTICLES, then its embers, and how many of each it
/// draws.
#define PARTICLE_STREAMS_FLASHES 128
#define PARTICLE_STREAMS_EMBER_COUNT 512
#define PARTICLE_STREAMS_EMBERS (&PARTICLES[PARTICLE_STREAMS_FLASHES])
#define PARTICLE_STREAMS_FLASHES_DRAWN 64
#define PARTICLE_STREAMS_EMBERS_DRAWN 64
/// The fall's flames, from work->particles[0], and its landings after them.
#define PARTICLE_STREAMS_FLAME_COUNT 32
#define PARTICLE_STREAMS_LANDINGS 16
/// The burst's rising motes, one at each of the 64 work->particles: the
/// flames', which it sets out, the landings', drawn where they were left, and
/// 16 this effect never places, which rise from wherever an earlier effect
/// left them.
#define PARTICLE_STREAMS_MOTES_DRAWN 64
/// The starfield's stars.
#define PARTICLE_STREAMS_STARS 256
/// How far down each column's ragged edge starts: the dissolve sweeps the
/// picture's rows and this many more.
#define PARTICLE_STREAMS_RAGGED_EDGE 64
/// The backdrop's glow turns blue, and later green, once its level passes
/// these.
#define PARTICLE_STREAMS_GLOW_BLUE_LEVEL 32
#define PARTICLE_STREAMS_GLOW_GREEN_LEVEL 80
/// The burst's flashes rise from this frame on, three more a frame, and the
/// background brightens from PARTICLE_STREAMS_BRIGHTEN_DELAY frames after it.
#define PARTICLE_STREAMS_FLASH_FRAME 16
#define PARTICLE_STREAMS_BRIGHTEN_DELAY 20
/// The blast sheet's flash pictures, each shown two frames.
#define PARTICLE_STREAMS_FLASH_PICTURES 7
/// An ember is drawn at size PARTICLE_STREAMS_EMBER_SIZE_MAX at
/// MOTE_DEPTH_NEAR or nearer, one size smaller every MOTE_DEPTH_STEP farther.
#define PARTICLE_STREAMS_EMBER_SIZE_MAX 9

/// The mote pictures in the blast sheet, after its seven blast pictures
/// (BLAST_PICTURE_SIZES): two sets, MOTE_SET_BYTES apart.
#define PARTICLE_STREAMS_MOTE_PICTURES (16 * 16 + 32 * 32 + 5 * 48 * 48)

/// Meteor and Outer Space: the fall through the starfield, then the burst.
void BattleEffect_RunParticleStreams(struct BattleEffectParams *args,
                                     s32 mode) {
  volatile u32 fill;
  u8 offsets[128];
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleEffectWork *work;
  void *colors = (void *)BG_PLTT;

  /// Starts a landing where a fireball reaches the ground; it is never drawn.
  void ParticleStreams_SpawnLanding(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != PARTICLE_STREAMS_LANDINGS; i++) {
      struct Particle *landing =
          &work->particles[PARTICLE_STREAMS_FLAME_COUNT + i];

      if (landing->variant == -1) {
        landing->variant = 0;
        landing->x = x;
        landing->y = y;
        landing->velocityX = velocityX;
        break;
      }
    }
  }

  /// Starts a flame; the fall starts its own inline.
  UNUSED void ParticleStreams_SpawnFlame(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != PARTICLE_STREAMS_FLAME_COUNT; i++) {
      struct Particle *flame = &work->particles[i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        break;
      }
    }
  }

  struct SpritePlacement position;
  s32 vector[3];
  struct ScreenPoint spot;
  struct ScreenPoint screen;
  struct SpriteScale scale;
  s32 frame;
  s32 i;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

  // In Outer Space the Fusion Dragon leaps out of sight.
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  direction = mode;
  if (direction == PARTICLE_STREAMS_OUTER_SPACE) {
    struct Actor *actor = GetBattleObjectSlotFar(work->args->user)->actor;

    actor->velocityY = toFIXED(10);
    actor->gravity = toFIXED(57) / 100;
    BattleEffect_TintBattler(work->args->user, -1, POSE_LEAP, -1, 0);
    Sound_PlayFar(SE_CRASH);
    direction = work->args->side == EFFECT_SIDE_PARTY ? 1 : -1;
  } else {
    direction = -1;
  }
  BattleEffect_ConfigureEffectDisplay();
  // The backdrop and colour 1 go black for the wipe.
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
    for (i = 0; i != 2; i++) {
      struct Sprite *sprite = Sprite_CreateFar(SPRITE_BATTLE_FUSION_DRAGON +
                                               i * SPRITE_SECOND_PART);

      work->sprites[i] = sprite;
      if (sprite != 0) {
        sprite->flags = 0;
        Sprite_StartAnimationFar(sprite, 2);
        work->sprites[i]->image.priority = 3;
      }
    }
  } else {
    BattleEffect_SpawnObjects(1, SPRITE_METEOR, 3);
  }
  // The starfield.
  Resource_LoadAndDecompress(RESOURCE_FLAME_SHEET_B, work->sheet, 1, 1);
  if (mode == PARTICLE_STREAMS_OUTER_SPACE)
    Iwram_CopyWords(colors,
                    Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
                    SHEET_PALETTE_BYTES);
  Dma_Fill32Cell(fill, CANVAS_FILL_WORD(1), Ram_MapGridCellBuffer, 256 * 128);
  Iwram_CopyWords(BG_CHAR_BLOCK(2), Ram_MapGridCellBuffer,
                  BATTLE_BACKGROUND_BYTES);
  wait(1);
  REG_BLDCNT = 0;
  REG_BG2PA = Q_8_8(1);
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
    s32 red = (Random16() & 15) + 16;
    s32 green = (Random16() & 15) + 16;
    s32 blue = (Random16() & 15) + 16;

    BG_PLTT_COLOR(8, i) = RGB(red, green, blue);
  }
  Dma_Fill32Cell(fill, 0, canvas, CANVAS_BYTES);
  for (i = 0; i != PARTICLE_STREAMS_STARS; i++) {
    s32 x = Random16() & (CANVAS_SIZE - 1);
    s32 y = Random16() & (CANVAS_SIZE - 1);
    s32 shade = (Random16() & 63) + 64;

    ((u8 *)canvas)[CANVAS_PIXEL_OFFSET(x, y)] = shade;
  }
  Iwram_CopyWords(BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
  // The dissolve: the battle background is erased from the top in ragged
  // columns, uncovering the starfield behind it.
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_SelectLivingTargets(work->args);
  work->bg2x = 0;
  work->bg2y = 0;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = direction * Q_8_8(0.5);
  work->scrollStepY = Q_8_8(1);
  Task_Add(BattleEffect_AdvanceBg2OnInterval, TASK_PASS_VBLANK + 127);
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, TASK_PASS_VBLANK);
  for (i = 0; i != ARRAY_COUNT(offsets); i++)
    offsets[i] = Random16() & (PARTICLE_STREAMS_RAGGED_EDGE - 1);
  {
    s32 limit = 0;
    s32 step = 1;
    s32 row = 0;

    do {
      limit += step / 2;
      step += 4;
      for (; row != limit; row++) {
        s32 x;

        for (x = 0; x != BATTLE_BACKGROUND_WIDTH; x++) {
          s32 y = row - offsets[x & 127];

          // Background pixel (x, y), laid out in 8x8 tiles.
          if (y >= 0)
            if (y <= 127)
              Ram_MapGridCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                    (x & 7)] = 0;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    } while (limit < CANVAS_SIZE + PARTICLE_STREAMS_RAGGED_EDGE);
  }
  // The fall, drawn on the battle background's picture (BG1, over the
  // starfield), which darkens a little each frame to leave trails.
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    struct BattleBackgroundWork *view =
        HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);

    gBgScroll[1].x = 0;
    gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    // A blitter for the background's 256-pixel rows.
    BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_LIGHTEN);
    draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
    Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_VBLANK + 126);
    EFFECT_FREE_PARTICLES(i, work->particles, ARRAY_COUNT(work->particles));
    view->busy = 1;
    work->frame = 0;
    frame = 0;
    do {
      s32 growth = frame << 8;
      s32 count = work->frame;
      s32 dividend;
      s32 glow;
      u16 *line;
      s32 size;
      s32 x;
      s32 y;
      s32 hold;

      /* FAKEMATCH(temporary): the division written out on a copy of count. */
      dividend = count;
      /* FAKEMATCH(dead): updates of count that cancel out. */
      if (work) {
        count++;
        count--;
      }
      if (dividend < 0)
        dividend += 3;
      glow = dividend >> 2;
      /* FAKEMATCH(barrier): hold set by an empty asm that reads count and
       * clobbers r1, r3 and r6. */
      __asm__("" : "=r"(hold) : "r"(count) : "r1", "r3", "r6");
      line = (u16 *)(work->sheet + PARTICLE_STREAMS_BACKDROP_LINES);
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 16)
          break;
      } else {
        if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 4)
          break;
      }
      if (frame == 0)
        Sound_PlayFar(SE_RUMBLE);
      /* FAKEMATCH(dead): updates of hold that cancel out. */
      if (hold) {
        hold++;
        hold--;
      }
      // The backdrop: a blue glow that climbs and pales as the fall goes on,
      // over the battle view. Entry i colours line i + 1: the HBlank DMA writes
      // it after line i.
      for (i = 0; i != BATTLE_VIEW_TOP - 1; i++)
        *line++ = 0;
      for (; i != BATTLE_VIEW_BOTTOM - 1; i++) {
        s32 green = i - BATTLE_VIEW_TOP;
        s32 quarter = green / 4;
        s32 level = quarter + glow;
        s32 blue;

        /* FAKEMATCH(dead): updates of quarter that cancel out, under a test of
         * quarter, keep it live past level's sum. */
        if (quarter) {
          quarter++;
          quarter--;
        }
        blue = level - PARTICLE_STREAMS_GLOW_BLUE_LEVEL;
        green = level - PARTICLE_STREAMS_GLOW_GREEN_LEVEL;

        CLAMP(blue, 0, RGB_LEVEL_MAX);
        CLAMP(green, 0, RGB_LEVEL_MAX);
        *line++ = RGB(green >> 1, green, blue);
      }
      for (; i != DISPLAY_HEIGHT; i++)
        *line++ = 0;
      if (direction == 1)
        x = frame / 4;
      else
        x = 64 - frame / 4;
      y = 96 - frame;
      // The falling sprite drifts with the stream and grows.
      position.groundHeight = 0;
      position.height = toFIXED(255);
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        size = growth + FIXED_ONE * 5 / 8;
        scale.x = size;
        scale.y = size;
        position.x = INTtoFIXED(x) + toFIXED(80);
        position.y = INTtoFIXED(64 - y);
        Actor_ApplyProjectedPlacementFar(work->sprites[0], &position, &scale,
                                         0);
        Actor_ApplyProjectedPlacementFar(work->sprites[1], &position, &scale,
                                         0);
      } else {
        size = growth + FIXED_ONE;
        scale.x = size;
        scale.y = size;
        position.x = INTtoFIXED(x) + toFIXED(96);
        position.y = INTtoFIXED(96 - y);
        Actor_ApplyProjectedPlacementFar(work->sprites[0], &position, &scale,
                                         0);
      }
      // A new flame below the falling sprite.
      for (i = 0, y = 32 - y; i != PARTICLE_STREAMS_FLAME_COUNT; i++) {
        struct Particle *flame = &work->particles[i];

        if (flame->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + DEGtoANG(90);

          flame->variant = 0;
          flame->x =
              INTtoFIXED(x + 96) + Trig_Sin(angle) * 30 / FIXED_ONE * size;
          flame->y = INTtoFIXED(y) - Trig_Cos(angle) * 30 / FIXED_ONE * size;
          break;
        }
      }
      // Seven sparks orbit the falling sprite.
      screen.x = 0;
      screen.y = 0;
      screen.depth = toFIXED(512);
      Matrix_Initialize();
      Matrix_TranslateVector(ScreenPoint_Coordinates(&screen));
      Matrix_RotateZ(TRIG_TURN / 32);
      Matrix_RotateY(frame << 8);
      for (i = 0; i != 7; i++) {
        SET_VECTOR(vector, INTtoFIXED(gParticleStreams_OrbitPoints[i][0] / 2),
                   INTtoFIXED(gParticleStreams_OrbitPoints[i][1] + frame),
                   INTtoFIXED(gParticleStreams_OrbitPoints[i][2] / 2));
        Projection_Point(vector, &spot);
        spot.x = FIXEDtoINT(spot.x) + 128;
        spot.y = FIXEDtoINT(spot.y) + 60;
        draw(Ram_MapGridCellBuffer, work->sheet + PARTICLE_STREAMS_SMALL_GLOW,
             spot.x - PARTICLE_STREAMS_SMALL_GLOW_SIZE / 2,
             spot.y - PARTICLE_STREAMS_SMALL_GLOW_SIZE / 2,
             PARTICLE_STREAMS_SMALL_GLOW_SIZE,
             PARTICLE_STREAMS_SMALL_GLOW_SIZE);
      }
      if (direction == 1)
        x = frame / 4 - 16;
      else
        x = 16 - frame / 4;
      // Seven fireballs fall; one that reaches the ground becomes a landing.
      y = frame - 96;
      for (i = 0; i != 7; i++) {
        s32 fireballY = gParticleStreams_FireballPoints[i][1] + y;

        if (fireballY <= 93)
          draw(Ram_MapGridCellBuffer, work->sheet + PARTICLE_STREAMS_LARGE_GLOW,
               gParticleStreams_FireballPoints[i][0] + x -
                   PARTICLE_STREAMS_LARGE_GLOW_SIZE / 2,
               fireballY - PARTICLE_STREAMS_LARGE_GLOW_SIZE / 2,
               PARTICLE_STREAMS_LARGE_GLOW_SIZE,
               PARTICLE_STREAMS_LARGE_GLOW_SIZE);
        else if (fireballY <= 95)
          ParticleStreams_SpawnLanding(
              INTtoFIXED(gParticleStreams_FireballPoints[i][0] + x),
              INTtoFIXED(fireballY), 1);
      }
      for (i = 0; i != PARTICLE_STREAMS_FLAME_COUNT; i++) {
        struct Particle *flame = &work->particles[i];

        // A flame rises against the stream through its six pictures.
        if (flame->variant >= 0) {
          draw(Ram_MapGridCellBuffer,
               work->sheet + PARTICLE_STREAMS_FLAMES +
                   flame->variant * (PARTICLE_STREAMS_FLAME_SIZE *
                                     PARTICLE_STREAMS_FLAME_SIZE),
               FIXEDtoINT(flame->x) - PARTICLE_STREAMS_FLAME_SIZE / 2,
               FIXEDtoINT(flame->y) - PARTICLE_STREAMS_FLAME_SIZE / 2,
               PARTICLE_STREAMS_FLAME_SIZE, PARTICLE_STREAMS_FLAME_SIZE);
          flame->x -= direction * (toFIXED(5) / 4);
          flame->y -= toFIXED(5);
          flame->variant++;
          if (flame->variant == 6)
            flame->variant = -1;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
      frame++;
      work->frame++;
    } while (frame != PARTICLE_STREAMS_FALL_FRAMES);
    wait(1);
    view->busy = 0;
    Task_Remove(BattleEffect_AdvanceBg2OnInterval);
    Task_Remove(BattleEffect_ArmPaletteHBlankDma);
    Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
    gBgScroll[1].x = savedX;
    gBgScroll[1].y = savedY;
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  // The burst: embers, rising motes and growing flashes from the impact.
  BattleEffect_SetupBlendedDisplay();
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET, work->sheet, 1, 0);
  for (i = 0; i != PARTICLE_STREAMS_FLAME_COUNT; i++) {
    struct Particle *mote = &work->particles[i];

    mote->x = Random16() & 127;
    mote->y = (Random16() & 127) + 127;
  }
  for (i = 0; i != PARTICLE_STREAMS_FLASHES; i++) {
    struct Particle *flash = &PARTICLES[i];

    SET_POSITION(flash, 0, 0, 0);
    SET_VELOCITY(flash, ((Random16() & 255) - 127) << 12,
                 (Random16() & 255) << 11, ((Random16() & 255) - 127) << 12);
    flash->variant = 0;
  }
  for (i = 0; i != PARTICLE_STREAMS_EMBER_COUNT; i++) {
    struct Particle *ember = &PARTICLE_STREAMS_EMBERS[i];

    SET_POSITION(ember, 0, 0, 0);
    SET_VELOCITY(ember, ((Random16() & 255) - 128) << 13,
                 (Random16() & 255) << 11, ((Random16() & 255) - 128) << 13);
    ember->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));

  for (frame = 0; frame != PARTICLE_STREAMS_BURST_FRAMES; frame++) {
    FIXED dragonScale = (frame + 464) << 8;
    struct BattleCamera *camera = gBattleCamera;
    s32 elapsed = frame - PARTICLE_STREAMS_FLASH_FRAME;
    struct SpriteScale scale;
    s32 x;
    s32 y;
    FIXED meteorScale;

    if (elapsed >= PARTICLE_STREAMS_BRIGHTEN_DELAY)
      Palette_BrightenBgEntries(2, 2, 2);
    if (frame == 0)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == 40)
      Sound_PlayFar(SE_CRASH);
    if (frame == 48) {
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        Sprite_DestroyFar(work->sprites[0]);
        Sprite_DestroyFar(work->sprites[1]);
        BattleObject_PutFar();
      }
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != PARTICLE_STREAMS_EMBERS_DRAWN; i++) {
      // Embers fall and bounce back up from the ground.
      struct Particle *ember = &PARTICLE_STREAMS_EMBERS[i];

      if (ember->y >= 0) {
        s32 size;

        Projection_Point(&ember->x, &screen);
        screen.x >>= 1;
        if (screen.depth < MOTE_DEPTH_NEAR)
          screen.depth = MOTE_DEPTH_NEAR;
        if (screen.depth > MOTE_DEPTH_FAR)
          screen.depth = MOTE_DEPTH_FAR;
        // UB: from depth 736 the size is 0, a picture 0 pixels wide, and its
        // picture is read from gBattleEffect_MotePictures[-1], the last
        // halfword of the blitter template, which ends at the table
        // (BattleEffect_BuildBlitter). No UBFIX form yet.
        size = PARTICLE_STREAMS_EMBER_SIZE_MAX -
               (screen.depth - MOTE_DEPTH_NEAR) / MOTE_DEPTH_STEP;
        draw(canvas,
             work->sheet +
                 (gBattleEffect_MotePictures[size - 1] +
                  (i & 1) * MOTE_SET_BYTES) +
                 PARTICLE_STREAMS_MOTE_PICTURES,
             screen.x - size / 2, screen.y - size, size, size * 2);
        Particle_AdvanceWithGravity3D(ember, PARTICLE_DAMPING_NONE,
                                      -toFIXED(1) / 8);
        if (ember->y <= toFIXED(20)) {
          ember->x = 0;
          ember->z = 0;
          ember->y = toFIXED(20);
          SET_VELOCITY(ember, ((Random16() & 63) - 32) << 15,
                       (Random16() & 63) << 13, ((Random16() & 63) - 32) << 15);
        }
      }
    }
    for (i = 0; i != PARTICLE_STREAMS_MOTES_DRAWN; i++) {
      struct Particle *mote = &work->particles[i];
      // Motes rise up the screen and wrap round.
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
    for (i = 0; i != PARTICLE_STREAMS_FLASHES_DRAWN; i++) {
      // Flashes grow and rise, three more each frame.
      struct Particle *flash = &PARTICLES[i];

      if (i / 3 < elapsed && flash->y >= 0) {
        Projection_Point(&flash->x, &screen);
        screen.x >>= 1;
        if (flash->variant >= 0 &&
            flash->variant < 2 * PARTICLE_STREAMS_FLASH_PICTURES) {
          s32 picture = flash->variant / 2;

          draw(canvas, work->sheet + gParticleStreams_FlashPictures[picture],
               screen.x - gParticleStreams_FlashSizes[picture] / 2,
               screen.y - gParticleStreams_FlashSizes[picture] / 2,
               gParticleStreams_FlashSizes[picture],
               gParticleStreams_FlashSizes[picture]);
        }
        flash->variant++;
        if (flash->variant == 2 * PARTICLE_STREAMS_FLASH_PICTURES) {
          flash->y = toFIXED(20);
          flash->x = 0;
          flash->z = INTtoFIXED((Random16() & 255) - 127);
          SET_VELOCITY(flash, 0, (Random16() & 255) << 11, 0);
          flash->variant = 0;
        } else {
          Particle_AdvanceWithGravity3D(flash, PARTICLE_DAMPING_NONE, 1);
        }
      }
    }
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    meteorScale = (frame << 8) + toFIXED(2);
    position.groundHeight = 0;
    position.height = toFIXED(255);
    if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
      scale.x = dragonScale;
      scale.y = dragonScale;
      position.x = INTtoFIXED(x) + toFIXED(96);
      position.y = INTtoFIXED(96 - y);
      Actor_ApplyProjectedPlacementFar(work->sprites[0], &position, &scale, 0);
      Actor_ApplyProjectedPlacementFar(work->sprites[1], &position, &scale, 0);
    } else {
      scale.x = meteorScale;
      scale.y = meteorScale;
      position.x = INTtoFIXED(x) + toFIXED(96);
      position.y = INTtoFIXED(96 - y);
      Actor_ApplyProjectedPlacementFar(work->sprites[0], &position, &scale, 0);
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

#undef PARTICLE_STREAMS_EMBERS
#undef PARTICLE_STREAMS_FALL_FRAMES
#undef PARTICLE_STREAMS_FLASHES_DRAWN
#undef PARTICLE_STREAMS_EMBERS_DRAWN
#undef PARTICLE_STREAMS_FLAME_COUNT
#undef PARTICLE_STREAMS_LANDINGS
#undef PARTICLE_STREAMS_MOTES_DRAWN
#undef PARTICLE_STREAMS_STARS
#undef PARTICLE_STREAMS_RAGGED_EDGE
#undef PARTICLE_STREAMS_GLOW_BLUE_LEVEL
#undef PARTICLE_STREAMS_GLOW_GREEN_LEVEL
#undef PARTICLE_STREAMS_FLASH_FRAME
#undef PARTICLE_STREAMS_BRIGHTEN_DELAY
#undef PARTICLE_STREAMS_FLASH_PICTURES
#undef PARTICLE_STREAMS_MOTE_PICTURES
#undef PARTICLE_STREAMS_EMBER_SIZE_MAX
