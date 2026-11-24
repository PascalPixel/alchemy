/// @file
/// Meteor and Outer Space; the summons' formations and the burst they end in;
/// brightening the background's colours; and an unused copy of Nereid.
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

/// Unused: nothing calls it. Knocks a unit back: unless it is the Fusion
/// Dragon or one of three of the Doom Dragon's forms, it leaps back to 130 to
/// 193 percent of its place's x, then takes its damage pose.
UNUSED void BattleEffect_StartActorMotionRandomDistance(s32 id) {
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
  Actor_SelectAnimationFar(object, POSE_DAMAGE);
}

/// Unused: nothing reaches it (no handler, summon case or call). A second copy
/// of BattleEffect_RunNereid, with its names: Nereid rides in on her turtle
/// over rising waves, columns of water and spray rise over the targets, and
/// she bursts away.
UNUSED void BattleEffect_RunNereidCopy(struct BattleEffectParams *args) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectState *state;
  u8 *cells;
  u8 *palette;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  cells = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);

  // The spray's colours and pictures, the column of water and the motes.
  palette = Resource_GetTableEntry(RESOURCE_WATER_SPRAY_SHEET);
  Dma_Set(palette, (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
          REG_DMA3);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet);
  palette = Resource_GetTableEntry(RESOURCE_FIRE_PILLAR_SHEET_A);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet + NEREID_COLUMN_PICTURE);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      cells);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct Particle *point;
    struct Particle *drop;
    struct Particle *drops;
    s32 *life;

    // Nereid rides in from the right.
    rise = 0;
    focus.x = toFIXED(256);
    focus.y = toFIXED(88);
    velocity.x = -toFIXED(16);
    velocity.y = -toFIXED(4);
    EFFECT_FREE_PARTICLES(k, state->particles, 64);
    // DEAD: particles NEREID_UNREAD_FIRST on get a place and delay nothing
    // reads; the ROM keeps the stores.
    for (k = 0; k != NEREID_UNREAD_COUNT; k++) {
      point = &state->particles[NEREID_UNREAD_FIRST + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }
    // Every drop in the map cell buffer starts free.
    life = &PARTICLES->variant;
    for (k = 0; k != NEREID_DROPS; k++) {
      *life = -1;
      life += sizeof(struct Particle) / sizeof(s32);
    }
    drops = PARTICLES;
    BattleEffect_SelectLivingTargets(state->args);
    wait(1);
    BattleEffect_SpawnObjects(NEREID_OBJECTS, SPRITE_NEREID, 2);

    for (frame = 0; frame != NEREID_FRAMES; frame++) {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > NEREID_COLUMNS_FRAME &&
          frame < 98)
        frame = 98;
      if (frame == 120)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      // The waves rise.
      if (frame <= 15)
        rise += 2;

      // She slows, and is pushed back right once she passes x = 120.
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

      // Drops burst up among the targets.
      if (frame == 28) {
        for (k = 0; k != NEREID_BURST_DROPS; k++) {
          drop = &drops[k];
          if (drop->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            drop->x = ((Trig_Sin(angle) * radius) >> 3) + toFIXED(32);
            drop->y = ((Trig_Cos(angle) * radius) >> 2) + toFIXED(96);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
          }
        }
      }
      if (frame >= NEREID_COLUMNS_FRAME && frame < NEREID_COLUMNS_END_FRAME) {
        s32 spawned = 0;

        for (k = 0; k != NEREID_DROPS; k++) {
          drop = &drops[k];
          if (drop->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            drop->x = ((Trig_Sin(angle) * radius) >> 3) + toFIXED(32);
            drop->y = ((Trig_Cos(angle) * radius) >> 2) + toFIXED(96);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
            spawned++;
            if (spawned == NEREID_DROPS_A_FRAME)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(SE_SCATTER);
      if (frame == NEREID_COLUMNS_FRAME)
        Sound_PlayFar(SE_CRASH);
      if (frame == NEREID_COLUMNS_END_FRAME)
        Sound_PlayFar(SE_CLATTER);

      // The columns of water scroll up.
      if (frame >= NEREID_COLUMNS_FRAME && frame < NEREID_COLUMNS_END_FRAME) {
        for (k = 0; k != NEREID_COLUMNS; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % FIRE_PILLAR_HEIGHT;

          draw[0](canvas, state->sheet + NEREID_COLUMN_PICTURE,
                  gNereidCopy_Columns[k * 2] - FIRE_PILLAR_WIDTH / 2,
                  gNereidCopy_Columns[k * 2 + 1] - span - FIRE_PILLAR_HEIGHT,
                  FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
          draw[0](canvas, state->sheet + NEREID_COLUMN_PICTURE,
                  gNereidCopy_Columns[k * 2] - FIRE_PILLAR_WIDTH / 2,
                  gNereidCopy_Columns[k * 2 + 1] - span, FIRE_PILLAR_WIDTH,
                  span);
        }
      }

      // The waves along the bottom.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, state->sheet,
                  k * WATER_WAVE_WIDTH + ((frame / 4) & 31) - WATER_WAVE_WIDTH,
                  120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
      }

      // Drops swell as they clear: they fall under gravity until frame 80, then
      // rise away, and a falling drop that sinks into the waves is freed.
      for (k = 0; k != NEREID_DROPS; k++) {
        drop = &drops[k];
        if (drop->variant >= 0) {
          s32 size = k % 3 + 2;

          if (drop->velocityY > 0)
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
            size = MOTE_PICTURES;
          EFFECT_DRAW_MOTE(draw[drop->velocityY > 0], canvas, cells,
                           FIXED_INTEGER_PART(drop->x),
                           FIXED_INTEGER_PART(drop->y), size);
          drop->x += drop->velocityX;
          drop->y += drop->velocityY;
          if (frame > NEREID_COLUMNS_END_FRAME)
            drop->velocityY += -toFIXED(1) / 2;
          else
            drop->velocityY += gNereidCopy_Gravity[k & 3];
          drop->velocityX = drop->velocityX * 62 / DAMPING_SCALE;
          drop->velocityY = drop->velocityY * 62 / DAMPING_SCALE;
          drop->variant++;
          if (drop->velocityY > 0 &&
              FIXED_INTEGER_PART(drop->y) > NEREID_WAVES_Y)
            drop->variant = -1;
        }
      }

      // The targets bounce and flash white every twelve frames from frame 30.
      if (frame < NEREID_COLUMNS_END_FRAME) {
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
      EFFECT_PRESENT_CANVAS(state);
    }

    // She bursts away.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_NEREID, focus.x, focus.y);
    for (k = 0; k != NEREID_OBJECTS; k++)
      Sprite_DestroyFar(state->sprites[k]);
  }
  BattleEffect_EndCanvasLayer();
}

extern const struct Scale gFormationObjects_Scale;
extern const struct Scale gFormationObjects_LargeScale;

/// The grid formations' sprites stand this many pixels apart.
#define FORMATION_SPACING 32

/// Arranges state->sprites in a formation (enum SummonFormation); x and z are
/// its origin in 16.16 pixels.
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x, s32 z) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  struct Scale normal = gFormationObjects_Scale;
  struct Scale big = gFormationObjects_LargeScale;
  FIXED pos[4];
  s32 i;
  void **objects;

  pos[3] = 0;
  pos[1] = toFIXED(255);
  switch (formation) {
  case SUMMON_FORMATION_THOR:
    objects = state->sprites;
    for (i = 0; i != 3 * 3; i++) {
      pos[0] = INTtoFIXED((i % 3) * FORMATION_SPACING) + x;
      pos[2] = INTtoFIXED((i / 3) * FORMATION_SPACING) + z;
      Render_ApplyProjectedPlacementFar(*objects++, pos, &normal.x, 0);
    }
    break;
  case SUMMON_FORMATION_MOLOCH:
    for (i = 0; i != 4 * 3; i++) {
      pos[0] = INTtoFIXED((i % 4) * FORMATION_SPACING) + x;
      pos[2] = INTtoFIXED((i / 4) * FORMATION_SPACING) + z;
      Render_ApplyProjectedPlacementFar(state->sprites[i], pos, &normal.x, 0);
    }
    break;
  case SUMMON_FORMATION_NEREID:
    for (i = 0; i != NEREID_OBJECTS; i++) {
      pos[0] = INTtoFIXED(gHelmSplitter_TwelveX[i]) + x - toFIXED(16);
      pos[2] = INTtoFIXED(gHelmSplitter_TwelveZ[i]) + z - toFIXED(32);
      Render_ApplyProjectedPlacementFar(state->sprites[i], pos, &normal.x, 0);
    }
    break;
  case SUMMON_FORMATION_CYBELE:
    for (i = 0; i != 8; i++) {
      pos[0] = INTtoFIXED(gHelmSplitter_RightEightX[i]) + x + toFIXED(16);
      pos[2] = INTtoFIXED(gHelmSplitter_RightEightZ[i]) + z;
      Render_ApplyProjectedPlacementFar(state->sprites[i], pos, &normal.x, 0);
    }
    break;
  case SUMMON_FORMATION_TIAMAT_ALTERNATE:
    for (i = 0; i != 8; i++) {
      pos[0] = INTtoFIXED(gFormationObjects_X[i]) + x;
      pos[2] = INTtoFIXED(gFormationObjects_Z[i]) + z;
      Render_ApplyProjectedPlacementFar(state->sprites[i], pos, &normal.x, 0);
    }
    break;
  case SUMMON_FORMATION_TIAMAT:
    for (i = 0; i != 8; i++) {
      pos[0] = INTtoFIXED(gHelmSplitter_SmallEightX[i]) + x;
      pos[2] = INTtoFIXED(gHelmSplitter_SmallEightZ[i]) + z;
      Render_ApplyProjectedPlacementFar(state->sprites[i], pos, &big.x, 0);
    }
    break;
  case SUMMON_FORMATION_MEGAERA:
    for (i = 0; i != MEGAERA_OBJECTS; i++) {
      pos[0] = INTtoFIXED(gFormationObjects_WideX[i]) + x;
      pos[2] = INTtoFIXED(gFormationObjects_WideZ[i]) + z;
      Render_ApplyProjectedPlacementFar(state->sprites[i], pos, &normal.x, 0);
    }
    break;
  }
}

/// The fragment pictures in the sprite sheet, four of 12x24, after the flash.
#define IMPACT_BURST_SHARD_PICTURES 23000
#define IMPACT_BURST_FRAGMENT_WIDTH 12 ///< each fragment picture's width
#define IMPACT_BURST_FRAGMENT_HEIGHT 24

#define IMPACT_BURST_FRAMES 72
#define IMPACT_BURST_FRAGMENTS 3     ///< fragments, state->particles 61 to 63
#define IMPACT_BURST_DRAWN_LARGE 30  ///< large sparks drawn
#define IMPACT_BURST_DRAWN_SMALL 60  ///< small sparks drawn
#define IMPACT_BURST_LARGE_SPARKS 64 ///< large sparks thrown
#define IMPACT_BURST_SMALL_SPARKS 64 ///< small sparks thrown
/// the first fragment in state->particles
#define IMPACT_BURST_FIRST_FRAGMENT 61
#define IMPACT_BURST_FRAGMENT_SPEED 32 ///< in the units of a spark's speed
/// A spark shrinks a size each time this many frames of its life run out.
#define IMPACT_BURST_SHRINK_FRAMES 16

/// Opens an impact flash and scatters sparks and fragments, keeping the
/// effect's sprites in formation for the first 36 frames.
void BattleEffect_RunImpactBurst(s32 formation, s32 x, FIXED y) {
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle additive;
  DrawRectangle maxBlend;
  u8 *sheet;
  s32 orgX;
  struct Particle *p;
  s32 frame;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
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
  Resource_LoadAndDecompress(RESOURCE_FLARE_SHEET, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLARE_IMAGE,
                             state->sheet + IMPACT_BURST_SHARD_PICTURES, 0, 0);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Large sparks fly.
  for (i = 0; i != IMPACT_BURST_LARGE_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 256;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    state->particles[i].x = x;
    state->particles[i].y = y;
    state->particles[i].velocityX = (Trig_Sin(angle) * speed) >> 7;
    state->particles[i].velocityY = -((Trig_Cos(angle) * speed) >> 6);
    state->particles[i].variant = (Random16() & 15) + 16;
  }

  // Three fragments fly apart.
  for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
    state->particles[IMPACT_BURST_FIRST_FRAGMENT + i].x = x;
    state->particles[IMPACT_BURST_FIRST_FRAGMENT + i].y = y;
    state->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityX =
        (Trig_Sin(i * (DEGtoANG(120))) * IMPACT_BURST_FRAGMENT_SPEED) >> 6;
    state->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityY =
        -((Trig_Cos(i * (DEGtoANG(120))) * IMPACT_BURST_FRAGMENT_SPEED) >> 5);
  }

  // Small sparks fly.
  for (i = 0; i != IMPACT_BURST_SMALL_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 32;
    s32 angle = Random16() & TRIG_ANGLE_MASK;
    struct Particle *cells = PARTICLES;

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
      s32 picture = (frame - 8) / 5;
      u32 width;
      u32 height;

      if (picture < 0)
        picture = 0;
      // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
      width = gHelmSplitter_ImpactBurstPictureWidths[picture];
      height = gHelmSplitter_ImpactBurstPictureHeights[picture];
      maxBlend(canvas,
               state->sheet + gHelmSplitter_ImpactBurstPictures[picture],
               FIXEDtoINT(x) - (width >> 1), FIXEDtoINT(y) - (height >> 1),
               width, height);
#else
      maxBlend(
          canvas, state->sheet + gHelmSplitter_ImpactBurstPictures[picture],
          (FIXEDtoINT(x)) -
              ((width = gHelmSplitter_ImpactBurstPictureWidths[picture]) >> 1),
          (FIXEDtoINT(y)) -
              ((height = gHelmSplitter_ImpactBurstPictureHeights[picture]) >>
               1),
          width, height);
#endif
    }

    for (i = 0, p = state->particles; i != IMPACT_BURST_DRAWN_LARGE; i++, p++) {
      if (frame > i / 2 && p->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        p->variant--;
        Particle_AdvanceWithGravity2D(p, 60, 0);
        size = p->variant / IMPACT_BURST_SHRINK_FRAMES + 3;
        px = FIXED_INTEGER_PART(p->x);
        py = FIXED_INTEGER_PART(p->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, px, py, size);
      }
    }

    for (i = 0, p = PARTICLES; i != IMPACT_BURST_DRAWN_SMALL; i++, p++) {
      if (frame > 35 && p->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        p->variant--;
        Particle_AdvanceWithGravity2D(p, 60, 0);
        size = p->variant / IMPACT_BURST_SHRINK_FRAMES + 1;
        px = FIXED_INTEGER_PART(p->x);
        py = FIXED_INTEGER_PART(p->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, px, py, size);
      }
    }

    for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
      struct Particle *shard =
          &state->particles[IMPACT_BURST_FIRST_FRAGMENT + i];

      if (frame >= 36 && frame <= 63) {
        s32 picture;
        s32 px;
        s32 py;

        Particle_AdvanceWithGravity2D(shard, PARTICLE_DAMPING_NONE, 0);
        picture = (frame - 36) / 7;
        px = FIXED_INTEGER_PART(shard->x);
        py = FIXED_INTEGER_PART(shard->y);
        maxBlend(canvas,
                 state->sheet + picture * PICTURE_BYTES(IMPACT_BURST_FRAGMENT) +
                     IMPACT_BURST_SHARD_PICTURES,
                 px - IMPACT_BURST_FRAGMENT_WIDTH / 2,
                 py - IMPACT_BURST_FRAGMENT_HEIGHT / 2,
                 IMPACT_BURST_FRAGMENT_WIDTH, IMPACT_BURST_FRAGMENT_HEIGHT);
      }
    }

    if (frame <= 35)
      BattleEffect_PlaceFormationObjects(formation, orgX, y);
    EFFECT_PRESENT_CANVAS(state);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  EFFECT_RELEASE_BLITTERS();
}

/// Brightens background colours 1 to 63 by a step in each channel, or darkens
/// them by a negative one.
void Palette_BrightenBgEntries(s32 blueDelta, s32 greenDelta, s32 redDelta) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(0, 1);
  s32 i;

  for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
    s32 blue = RGB_BLUE(*color);
    s32 green = RGB_GREEN(*color);
    s32 red = RGB_RED(*color);

    blue += blueDelta;
    green += greenDelta;
    red += redDelta;
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

/// Brightens the battle background's colours, BG palettes 6 to 13, by a step
/// in each channel, or darkens them by a negative one.
void Palette_BrightenBattleBg(s32 redDelta, s32 greenDelta, s32 blueDelta) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);
  s32 i;

  for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
    s32 blue = RGB_BLUE(*color);
    s32 green = RGB_GREEN(*color);
    s32 red = RGB_RED(*color);

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
  }
}

/// As Palette_BrightenBattleBg, from the battle background's own colours as the
/// battle keeps them.
void Palette_BrightenSavedBattleBg(s32 redDelta, s32 greenDelta,
                                   s32 blueDelta) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);
  u16 *source = Ram_HeapSlots->battleState->palette;
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

/// Streams the fall's backdrop colours into colour 0, one each line.
void BattleEffect_ArmPaletteHBlankDma(void) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  volatile u16 *channel = REG_DMA0;

  channel[DMA_CONTROL_HALF] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
  (void)channel[DMA_CONTROL_HALF];
  Dma_Set(state->backdropLines, (void *)BG_PLTT, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
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

/// The fall and the burst, in frames.
#define PARTICLE_STREAMS_FALL_FRAMES 192
#define PARTICLE_STREAMS_BURST_FRAMES 54
/// The burst's frame the damage is told from.
#define PARTICLE_STREAMS_DAMAGE_FRAME 48

/// The pictures in FlameSheetB (RESOURCE_FLAME_SHEET_B), each starting
/// where the one before ends: six flames, then three large and three small
/// glows, each glow brighter at its centre than the one before. The fall draws
/// only the six and the brightest glow of each size.
#define PARTICLE_STREAMS_FLAME_SIZE 32
#define PARTICLE_STREAMS_LARGE_GLOW_SIZE 24
#define PARTICLE_STREAMS_SMALL_GLOW_SIZE 8
/// Where each picture starts in FlameSheetB, in bytes.
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
      2 * PARTICLE_STREAMS_SMALL_GLOW_SIZE * PARTICLE_STREAMS_SMALL_GLOW_SIZE
};

/// The burst's flashes in PARTICLES, then its embers, and how many of each it
/// draws.
#define PARTICLE_STREAMS_FLASHES 128
#define PARTICLE_STREAMS_EMBER_COUNT 512
#define PARTICLE_STREAMS_EMBERS (&PARTICLES[PARTICLE_STREAMS_FLASHES])
#define PARTICLE_STREAMS_FLASHES_DRAWN 64
#define PARTICLE_STREAMS_EMBERS_DRAWN 64
/// The fall's flames, from state->particles[0], and its landings after them.
#define PARTICLE_STREAMS_FLAME_COUNT 32
#define PARTICLE_STREAMS_LANDINGS 16
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
/// The blast sprite sheet's flash pictures, each shown two frames.
#define PARTICLE_STREAMS_FLASH_PICTURES 7
/// An ember's depth is held between these: the nearest are drawn as mote 9,
/// each PARTICLE_STREAMS_EMBER_SIZE_DEPTH deeper a size smaller.
#define PARTICLE_STREAMS_EMBER_NEAR 160
#define PARTICLE_STREAMS_EMBER_FAR 799
#define PARTICLE_STREAMS_EMBER_SIZE_DEPTH 64

/// The mote pictures in the blast sprite sheet, after its seven blast pictures
/// (BLAST_PICTURE_SIZES): two sets, MOTE_SET_BYTES apart.
#define PARTICLE_STREAMS_MOTE_PICTURES (16 * 16 + 32 * 32 + 5 * 48 * 48)

/// Meteor and Outer Space: the fall through the starfield, then the burst.
void BattleEffect_RunParticleStreams(struct BattleEffectParams *args,
                                     s32 mode) {
  u8 offsets[128];
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleEffectState *state;

  // Starts a landing where a fireball reaches the ground; it is never drawn.
  void ParticleStreams_SpawnLanding(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != PARTICLE_STREAMS_LANDINGS; i++) {
      struct Particle *landing =
          &state->particles[PARTICLE_STREAMS_FLAME_COUNT + i];

      if (landing->variant == -1) {
        landing->variant = 0;
        landing->x = x;
        landing->y = y;
        landing->velocityX = velocityX;
        break;
      }
    }
  }

  // Unused: starts a flame; the fall starts its own inline.
  UNUSED void ParticleStreams_SpawnFlame(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != PARTICLE_STREAMS_FLAME_COUNT; i++) {
      struct Particle *flame = &state->particles[i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        break;
      }
    }
  }

  FIXED position[4];
  FIXED vector[3];
  struct ScreenPoint spot;
  struct ScreenPoint *pos;
  struct ScreenPoint screen;
  struct Scale scale;
  s32 frame;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);

  // In Outer Space the Fusion Dragon leaps out of sight. The stream then runs
  // rightward (direction 1) when the party casts, else leftward (-1).
  direction = mode;
  if (direction == PARTICLE_STREAMS_OUTER_SPACE) {
    struct MotionObject *user = BattleObject_GetFar(args->user)->object;

    user->velocityY = toFIXED(10);
    user->verticalMotionStrength = toFIXED(57) / 100;
    BattleEffect_TintBattler(args->user, TINT_KEEP, POSE_LEAP, TINT_KEEP, 0);
    Sound_PlayFar(SE_CRASH);
    direction = args->side == EFFECT_SIDE_PARTY ? mode : -1;
  } else {
    direction = -1;
  }

  // The backdrop and colour 1 go black for the wipe.
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  state->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);

  // The Fusion Dragon's two halves, or the meteor.
  if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
    for (i = 0; i != 2; i++) {
      struct Sprite *sprite = Sprite_CreateFar(SPRITE_BATTLE_FUSION_DRAGON +
                                               i * SPRITE_SECOND_PART);

      state->sprites[i] = sprite;
      if (sprite != 0) {
        sprite->flags = 0;
        Sprite_StartAnimationFar(sprite, 2);
        ((struct Sprite *)state->sprites[i])->image.priority = 3;
      }
    }
  } else {
    BattleEffect_SpawnObjects(1, SPRITE_METEOR, 3);
  }
  // The starfield.
  Resource_LoadAndDecompress(RESOURCE_FLAME_SHEET_B, state->sheet, 1, 1);
  if (mode == PARTICLE_STREAMS_OUTER_SPACE)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
                    SHEET_PALETTE_BYTES);

  // BG1's picture all colour 1, black, for the dissolve to erase.
  IWRAM_FILL_WORDS(Ram_MapCellBuffer, 256 * 128, CANVAS_FILL_WORD(1));
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapCellBuffer,
                  BATTLE_BACKGROUND_BYTES);
  wait(1);
  REG_BLDCNT = 0;
  REG_BG2PA = Q_8_8(1);
  REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;

  // The stars: pale colours scattered over the canvas.
  for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
    s32 red = (Random16() & 15) + 16;
    s32 green = (Random16() & 15) + 16;
    s32 blue = (Random16() & 15) + 16;

    BG_PLTT_COLOR(8, i) = RGB(red, green, blue);
  }
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);

  for (i = 0; i != PARTICLE_STREAMS_STARS; i++) {
    s32 x = Random16() & (CANVAS_SIZE - 1);
    s32 y = Random16() & (CANVAS_SIZE - 1);
    s32 shade = (Random16() & 63) + 64;

    ((u8 *)canvas)[CANVAS_PIXEL_OFFSET(x, y)] = shade;
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
  // The dissolve: the map is erased from the top in ragged columns.
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_SelectLivingTargets(state->args);

  state->bg2x = 0;
  state->bg2y = 0;
  state->scrollTimer = 0;
  state->scrollInterval = 2;
  // Half a pixel a step with the stream and a pixel down, in 20.8.
  state->scrollStepX = direction << 7;
  state->scrollStepY = 1 << 8;
  Task_Add(BattleEffect_AdvanceBg2OnInterval, TASK_PASS_VBLANK + 127);
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // Each column starts at its own random row, and the rows go faster and
  // faster.
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

        for (x = 0; x != 256; x++) {
          s32 y = row - offsets[x & 127];

          if (y >= 0)
            if (y <= 127)
              Ram_MapCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                (x & 7)] = 0;
        }
      }
      EFFECT_PRESENT_CANVAS(state);
    } while (limit < CANVAS_SIZE + PARTICLE_STREAMS_RAGGED_EDGE);
  }
  // The fall.
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    struct BattleBackgroundState *transition = Ram_HeapSlots->battleBackground;
    s32 transferMode = CANVAS_COPY_AND_DARKEN;

    gBgScroll[1].x = 0;
    gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7,
                              COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLITTER_MAX);
    draw = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_SET_TRANSFER(state, transferMode, CANVAS_FILL_WORD(2));
    Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_VBLANK + 126);
    EFFECT_FREE_PARTICLES(i, state->particles, 64);
    transition->busy = 1;
    state->frame = 0;
    frame = 0;
    do {
      s32 growth = frame << 8;
      s32 count = state->frame;
      s32 dividend;
      s32 glow;
      u16 *line;
      s32 spriteScale;
      s32 x;
      s32 y;
      s32 hold;

      /* FAKEMATCH(temporary): the division written out on a copy of count. */
      dividend = count;
      /* FAKEMATCH(dead): updates of count that cancel out. */
      if (state) {
        count++;
        count--;
      }
      if (dividend < 0)
        dividend += 3;
      glow = dividend >> 2;
      /* FAKEMATCH(barrier): hold set by an empty asm that reads count and
       * clobbers r1, r3 and r6. */
      asm("" : "=r"(hold) : "r"(count) : "r1", "r3", "r6");
      line = state->backdropLines;

      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 16)
          break;
      } else {
        if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 4)
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

        /* FAKEMATCH(dead): updates of quarter that cancel out. */
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

      // The falling sprite drifts with the stream and grows.
      if (direction == 1)
        x = frame / 4;
      else
        x = 64 - frame / 4;
      y = 96 - frame;
      position[3] = 0;
      position[1] = toFIXED(255);
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        spriteScale = growth + FIXED_ONE * 5 / 8;
        scale.x = spriteScale;
        scale.y = spriteScale;
        position[0] = INTtoFIXED(x) + toFIXED(80);
        position[2] = INTtoFIXED(64 - y);
        Render_ApplyProjectedPlacementFar(state->sprites[0], position, &scale,
                                          0);
        Render_ApplyProjectedPlacementFar(state->sprites[1], position, &scale,
                                          0);
      } else {
        spriteScale = growth + FIXED_ONE;
        scale.x = spriteScale;
        scale.y = spriteScale;
        position[0] = INTtoFIXED(x) + toFIXED(96);
        position[2] = INTtoFIXED(96 - y);
        Render_ApplyProjectedPlacementFar(state->sprites[0], position, &scale,
                                          0);
      }

      // A new flame below the falling sprite.
      for (i = 0, y = 32 - y; i != PARTICLE_STREAMS_FLAME_COUNT; i++) {
        struct Particle *flame = &state->particles[i];

        if (flame->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + DEGtoANG(90);

          flame->variant = 0;
          flame->x = INTtoFIXED(x + 96) +
                     Trig_Sin(angle) * 30 / FIXED_ONE * spriteScale;
          flame->y =
              INTtoFIXED(y) - Trig_Cos(angle) * 30 / FIXED_ONE * spriteScale;
          break;
        }
      }

      // Seven sparks orbit the falling sprite.
      screen.x = 0;
      screen.y = 0;
      screen.depth = toFIXED(512);
      Matrix_Initialize();
      Matrix_TranslateVector(&screen.x);
      Matrix_RotateZ(TRIG_TURN / 32);
      Matrix_RotateY(frame << 8);
      for (i = 0; i != 7; i++) {
        vector[0] = INTtoFIXED(gParticleStreams_OrbitPoints[i][0] / 2);
        vector[1] = INTtoFIXED(gParticleStreams_OrbitPoints[i][1] + frame);
        vector[2] = INTtoFIXED(gParticleStreams_OrbitPoints[i][2] / 2);
        EffectPosition_ProjectPoint(vector, pos = &spot);
        pos->x = FIXED_INTEGER_PART(pos->x) + 128;
        pos->y = FIXED_INTEGER_PART(pos->y) + 60;
        draw(Ram_MapCellBuffer, state->sheet + PARTICLE_STREAMS_SMALL_GLOW,
             pos->x - PARTICLE_STREAMS_SMALL_GLOW_SIZE / 2,
             pos->y - PARTICLE_STREAMS_SMALL_GLOW_SIZE / 2,
             PARTICLE_STREAMS_SMALL_GLOW_SIZE,
             PARTICLE_STREAMS_SMALL_GLOW_SIZE);
      }

      // Seven fireballs fall; one that reaches the ground becomes a landing.
      if (direction == 1)
        x = frame / 4 - 16;
      else
        x = 16 - frame / 4;
      y = frame - 96;
      for (i = 0; i != 7; i++) {
        s32 fireballY = gParticleStreams_FireballPoints[i][1] + y;

        if (fireballY <= 93)
          draw(Ram_MapCellBuffer, state->sheet + PARTICLE_STREAMS_LARGE_GLOW,
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
        struct Particle *flame = &state->particles[i];

        // A flame rises against the stream through its six pictures.
        if (flame->variant >= 0) {
          draw(Ram_MapCellBuffer,
               state->sheet + PARTICLE_STREAMS_FLAMES +
                   flame->variant * (PARTICLE_STREAMS_FLAME_SIZE *
                                     PARTICLE_STREAMS_FLAME_SIZE),
               FIXED_INTEGER_PART(flame->x) - PARTICLE_STREAMS_FLAME_SIZE / 2,
               FIXED_INTEGER_PART(flame->y) - PARTICLE_STREAMS_FLAME_SIZE / 2,
               PARTICLE_STREAMS_FLAME_SIZE, PARTICLE_STREAMS_FLAME_SIZE);
          flame->x -= direction * (toFIXED(5) / 4);
          flame->y -= toFIXED(5);
          flame->variant++;
          if (flame->variant == 6)
            flame->variant = -1;
        }
      }
      EFFECT_PRESENT_CANVAS(state);
      frame++;
      state->frame++;
    } while (frame != PARTICLE_STREAMS_FALL_FRAMES);
    wait(1);
    transition->busy = 0;
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
  REG_BG2CNT =
      BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET, state->sheet, 1, 0);
  for (i = 0; i != PARTICLE_STREAMS_FLAME_COUNT; i++) {
    struct Particle *mote = &state->particles[i];

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
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));

  for (frame = 0; frame != PARTICLE_STREAMS_BURST_FRAMES; frame++) {
    s32 dragonScale = (frame + 464) << 8;
    struct BattleCamera *camera = Ram_HeapSlots->cameraState;
    s32 elapsed = frame - PARTICLE_STREAMS_FLASH_FRAME;
    struct Scale scale;
    s32 x;
    s32 y;
    s32 grow;

    if (elapsed >= PARTICLE_STREAMS_BRIGHTEN_DELAY)
      Palette_BrightenBgEntries(2, 2, 2);
    if (frame == 0)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == 40)
      Sound_PlayFar(SE_CRASH);
    if (frame == PARTICLE_STREAMS_DAMAGE_FRAME) {
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        Sprite_DestroyFar(state->sprites[0]);
        Sprite_DestroyFar(state->sprites[1]);
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

        EffectPosition_ProjectPoint(&ember->x, &screen);
        screen.x >>= 1;
        if (screen.depth < PARTICLE_STREAMS_EMBER_NEAR)
          screen.depth = PARTICLE_STREAMS_EMBER_NEAR;
        if (screen.depth > PARTICLE_STREAMS_EMBER_FAR)
          screen.depth = PARTICLE_STREAMS_EMBER_FAR;
        size = 9 - (screen.depth - PARTICLE_STREAMS_EMBER_NEAR) /
                       PARTICLE_STREAMS_EMBER_SIZE_DEPTH;
        // UB: from depth 736 the size is 0, a picture 0 pixels wide, and its
        // picture is read from gBattleEffect_MotePictures[-1]; UBFIX draws
        // the farthest embers at the smallest size.
#ifdef UBFIX
        if (size < 1)
          size = 1;
#endif
        draw(canvas,
             state->sheet +
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

    for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
      struct Particle *mote = &state->particles[i];
      // Motes rise up the screen and wrap round.
      u32 rise = i & 7;
      s32 size = rise + 3;

      draw(canvas,
           state->sheet +
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
        EffectPosition_ProjectPoint(&flash->x, &screen);
        screen.x >>= 1;
        if ((u32)flash->variant < 2 * PARTICLE_STREAMS_FLASH_PICTURES) {
          s32 picture = flash->variant / 2;

          draw(canvas, state->sheet + gParticleStreams_FlashPictures[picture],
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

    // The falling sprite comes down onto the targets, growing.
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    grow = (frame << 8) + FIXED_ONE * 2;
    position[3] = 0;
    position[1] = toFIXED(255);
    if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
      scale.x = dragonScale;
      scale.y = dragonScale;
      position[0] = INTtoFIXED(x) + toFIXED(96);
      position[2] = INTtoFIXED(96 - y);
      Render_ApplyProjectedPlacementFar(state->sprites[0], position, &scale, 0);
      Render_ApplyProjectedPlacementFar(state->sprites[1], position, &scale, 0);
    } else {
      scale.x = grow;
      scale.y = grow;
      position[0] = INTtoFIXED(x) + toFIXED(96);
      position[2] = INTtoFIXED(96 - y);
      Render_ApplyProjectedPlacementFar(state->sprites[0], position, &scale, 0);
    }
    state->shakeFrames = 1;
    BattleCamera_Shake(8, 8);
    EFFECT_PRESENT_CANVAS(state);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  if (mode == PARTICLE_STREAMS_METEOR)
    Sprite_DestroyFar(state->sprites[0]);
  BattleEffect_EndCanvasLayer();
}

#undef FORMATION_SPACING
#undef PARTICLE_STREAMS_FALL_FRAMES
#undef PARTICLE_STREAMS_BURST_FRAMES
#undef PARTICLE_STREAMS_DAMAGE_FRAME
#undef PARTICLE_STREAMS_FLAME_SIZE
#undef PARTICLE_STREAMS_LARGE_GLOW_SIZE
#undef PARTICLE_STREAMS_SMALL_GLOW_SIZE
#undef PARTICLE_STREAMS_FLASHES
#undef PARTICLE_STREAMS_EMBER_COUNT
#undef PARTICLE_STREAMS_EMBERS
#undef PARTICLE_STREAMS_FLASHES_DRAWN
#undef PARTICLE_STREAMS_EMBERS_DRAWN
#undef PARTICLE_STREAMS_FLAME_COUNT
#undef PARTICLE_STREAMS_LANDINGS
#undef PARTICLE_STREAMS_STARS
#undef PARTICLE_STREAMS_RAGGED_EDGE
#undef PARTICLE_STREAMS_GLOW_BLUE_LEVEL
#undef PARTICLE_STREAMS_GLOW_GREEN_LEVEL
#undef PARTICLE_STREAMS_FLASH_FRAME
#undef PARTICLE_STREAMS_BRIGHTEN_DELAY
#undef PARTICLE_STREAMS_FLASH_PICTURES
#undef PARTICLE_STREAMS_EMBER_NEAR
#undef PARTICLE_STREAMS_EMBER_FAR
#undef PARTICLE_STREAMS_EMBER_SIZE_DEPTH
#undef PARTICLE_STREAMS_MOTE_PICTURES
