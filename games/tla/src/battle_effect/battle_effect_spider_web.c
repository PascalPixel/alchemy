/// @file
/// Drawing helpers several effects share (a sprite taken from the pool, the
/// grey ramp, shaded discs, the mirrored palette), then the Ulysses summon,
/// the swirl of Poison Beat and Spinning Beat, and Spider Web.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Takes the first free sprite of the actor system's pool with a new animation
/// entry. Its callers also pass the sprite's OAM shape and mode, which it
/// leaves for the caller to set.
void *EffectSprite_Acquire(s32 width, s32 height, u32 shape, s32 mode) {
  struct Sprite *sprite = Ram_HeapSlots->sprites;
  struct Sprite *found = NULL;
  s32 i = 0;

  while (i < ANIM_OBJECT_COUNT) {
    if (sprite->width == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  sprite->entries[0] = AnimationObject_AllocateEmpty();
  sprite->width = width;
  sprite->height = height;
  sprite->scale = FIXED_ONE;
  sprite->retainCacheEntry = 0;
  sprite->offsetY = 0;
  sprite->offsetX = 0;
  sprite->count = 1;
  sprite->flags = 0;
  sprite->rotation = 0;
  Sprite_StartAnimationFar(sprite, 0);
  return found;
}

/// Fills BG palette colours 1 to 63, the canvas's levels, with a grey ramp,
/// each level i / 2.
void Graphics_LoadGreyRamp(void) {
  s32 i = 0;
  u16 *color = (u16 *)&BG_PLTT_COLOR(0, 1);

  do {
    {
      s32 level = i / 2;

      i++;
      *color = RGB(level, level, level);
    }
    color++;
  } while (i != CANVAS_LEVEL_BRIGHTEST);
}

/// Draws the upper left quarter of a shaded disc into a map `size` / 2 bytes
/// wide and `size` rows tall, centred on its bottom right corner and squeezed
/// to half width: each ring is darker than the one inside it by a cosine
/// falloff, scaled by `scale` (FIXED_ONE at full strength) and clamped at
/// CANVAS_LEVEL_BRIGHTEST.
void BattleEffect_DrawShadedCircles(u8 *map, s32 size, FIXED scale) {
  s32 half = size / 2;
  s32 center = half - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWordsEntry(map, half * size);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;

    shade = FIXEDtoINT(Trig_Cos(radius * DEGtoANG(90) / last) *
                       CANVAS_LEVEL_BRIGHTEST);
    shade = shade * scale / (FIXED_ONE);

    if (shade > CANVAS_LEVEL_BRIGHTEST)
      shade = CANVAS_LEVEL_BRIGHTEST;
    for (; x >= y; y++) {
      col = center - x / 2;
      map[half * (last - y) + col] = shade;
      col = center - y / 2;
      map[half * (last - x) + col] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
    }
  }
}

/// Draws the whole shaded disc BattleEffect_DrawShadedCircles draws a quarter
/// of, mirrored into the four quarters of a map `size` bytes wide and twice
/// `size` rows tall.
void BattleEffect_DrawShadedCirclesMirrored(u8 *map, s32 size, s32 scale) {
  s32 center = size / 2 - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWordsEntry(map, size * size * 2);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;
    s32 row;

    shade = FIXEDtoINT(Trig_Cos(radius * DEGtoANG(90) / last) *
                       CANVAS_LEVEL_BRIGHTEST);
    shade = shade * scale / (FIXED_ONE);
    if (shade > CANVAS_LEVEL_BRIGHTEST)
      shade = CANVAS_LEVEL_BRIGHTEST;
    for (; x >= y; y++) {
      col = center - x / 2;
      row = last - y;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      col = center - y / 2;
      row = last - x;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
    }
  }
}

/// Mirrors the first 64 background colours, keeping the backdrop colour.
void Palette_MirrorBgColors(void) {
  u16 saved[SHEET_PALETTE_COLORS];
  u16 flipped[SHEET_PALETTE_COLORS];
  s32 i;

  Iwram_CopyWords(saved, (void *)BG_PLTT, SHEET_PALETTE_BYTES);
  for (i = 1; i != SHEET_PALETTE_COLORS; i++)
    flipped[i] = saved[SHEET_PALETTE_COLORS - i];
  flipped[0] = saved[0];
  Iwram_CopyWords((void *)BG_PLTT, flipped, SHEET_PALETTE_BYTES);
  REG_BLDALPHA = BLEND_ALPHA(16, 8);
}

/// The mage is effect sprites 0 to 6, on gUlysses_ObjectX and _ObjectZ, and
/// sprite 7 is a white flake: its animations from ULYSSES_FLAKE_ANIMATION are
/// the flake turning.
#define ULYSSES_MAGE_SPRITES 7
#define ULYSSES_FLAKE_SPRITE ULYSSES_MAGE_SPRITES
#define ULYSSES_FLAKE_ANIMATION 7

/// The particle the last flake flies by: the state's last.
#define ULYSSES_FLAKE (state->particles[ARRAY_COUNT(state->particles) - 1])

/// A frame task of Ulysses's end (BattleEffect_RunUlysses adds and removes
/// it): the white flake flutters away from where the mage stood, its speed in
/// depth rising for 20 frames and then falling, drifting sideways from frame
/// 12, and turning through its eight pictures, seven frames each.
void Ulysses_FlutterFlake(void) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  struct SpriteScale scale = gUlysses_ParticleScale;
  struct SpritePlacement place;

  place.groundHeight = 0;

  place.height = 0;

  place.x = ULYSSES_FLAKE.x;
  place.y = ULYSSES_FLAKE.y;
  Render_ApplyProjectedPlacementFar(state->sprites[ULYSSES_FLAKE_SPRITE],
                                    &place.x, &scale.x, 0);
  ULYSSES_FLAKE.x += ULYSSES_FLAKE.velocityX;
  ULYSSES_FLAKE.y += ULYSSES_FLAKE.velocityY;
  if (state->frame <= 19)
    ULYSSES_FLAKE.velocityY += toFIXED(1) / 16;
  else
    ULYSSES_FLAKE.velocityY -= toFIXED(1) / 16;
  if (state->frame > 11)
    ULYSSES_FLAKE.velocityX += toFIXED(1) / 16;
  Sprite_StartAnimationFar(state->sprites[ULYSSES_FLAKE_SPRITE],
                           state->frame / 7 % 8 + ULYSSES_FLAKE_ANIMATION);
  state->frame++;
}

/// The mage's sprite scale, one to one.
extern const struct Scale gUlysses_MageScale;

/// Where UlyssesSilhouette, then FlutteringFlakeSheet's cels, are decoded in
/// the effect's pictures, and the silhouette's size.
#define ULYSSES_PICTURES 6017
#define ULYSSES_SILHOUETTE_WIDTH 80
#define ULYSSES_SILHOUETTE_HEIGHT 112
/// FlutteringFlakeSheet's eighteen pictures of the white flake turning.
#define ULYSSES_FLAKE_CELS 18
/// Where RuneCircleSheet and YellowOrbSheet are decoded in the effect's
/// pictures.
#define ULYSSES_RUNE_CIRCLE_PICTURES 10113
#define ULYSSES_YELLOW_ORB_PICTURES 7831
/// FadingCrescentSheet, in the map cell buffer: a crescent of light closing to
/// a disc and shrinking, seven pictures.
#define ULYSSES_CRESCENT_WIDTH 64
#define ULYSSES_CRESCENT_HEIGHT 64
/// A crescent's scale `t` frames into its sweep: a sine that turns a
/// sixty-fourth of a turn a frame, at 48 sixty-fourths, so it swells to three
/// quarters of its size and shrinks a little.
#define ULYSSES_CRESCENT_SCALE(t) ((Trig_Sin((t) << 10) * 48) / 64)

#define ULYSSES_FRAMES 326 ///< frames of the runes, the summoning and the fire
/// The flame bursts and the rune circles open, the second
/// ULYSSES_RUNE_CIRCLE_INTERVAL frames after the first; the mage's silhouette
/// glows in them, and the mage appears.
#define ULYSSES_RUNE_CIRCLES_FRAME 76
#define ULYSSES_RUNE_CIRCLE_INTERVAL 16
#define ULYSSES_SILHOUETTE_FRAME 92
#define ULYSSES_APPEAR_FRAME 108
/// The rune circles' colours fade, then the strikes begin.
#define ULYSSES_RUNE_CIRCLES_FADE_FRAME 124
#define ULYSSES_STRIKES_FRAME 132
/// The fire flares over the mage, and the flake flutters away.
#define ULYSSES_FLARE_FRAME 266
#define ULYSSES_FLAKE_FRAME 288

/// The flake drifts down, swaying 8 pixels either way, in 1/65536 turns a
/// frame.
#define ULYSSES_FLAKE_SWAY 860
/// The silhouette swells and shrinks back as it zooms in, in 1/65536 turns a
/// frame.
#define ULYSSES_SILHOUETTE_SWELL 800
/// The silhouette is drawn in the two BG palettes from ULYSSES_GLOW_PALETTE,
/// which glow violet: brighter over its first ULYSSES_GLOW_FRAMES, and gone
/// ULYSSES_GLOW_END frames after it shows.
#define ULYSSES_GLOW_PALETTE 8
#define ULYSSES_GLOW_COLORS 32
#define ULYSSES_GLOW_FRAMES 16
#define ULYSSES_GLOW_END 32
/// Colour `i` of the glow's at `level` (0 to 16): violet, brighter up the
/// ramp, its red three quarters of its blue.
#define ULYSSES_GLOW_RED(i, level) (((i) * (((level) * 9 * 4) / 16)) / 64)
#define ULYSSES_GLOW_BLUE(i, level) (((i) * (level) * 3) / 64)

/// The rune circles: the first grows to three quarters of its size, holds and
/// spreads out from ULYSSES_RUNE_CIRCLE_HOLD_FRAMES; the second rises from
/// lying flat over ULYSSES_RUNE_CIRCLE_TILT_FRAMES, then shrinks.
#define ULYSSES_RUNE_CIRCLE_HOLD_FRAMES 40
#define ULYSSES_RUNE_CIRCLE_TILT_FRAMES 24
#define ULYSSES_RUNE_CIRCLE_TILT_X(t)                                          \
  (ULYSSES_RUNE_CIRCLE_TILT_FRAMES * 682 - (t) * 682)
#define ULYSSES_RUNE_CIRCLE_TILT_Z(t)                                          \
  ((t) * 169 + TRIG_TURN - ULYSSES_RUNE_CIRCLE_TILT_FRAMES * 169)

/// A flame of the opening burst and of the flare: six pictures, three or four
/// frames each.
#define ULYSSES_BURST_FLAME_FRAMES 18
#define ULYSSES_FLARE_FLAME_FRAMES 24
#define ULYSSES_FLARE_FLAMES 32 ///< flames of the flare, in a column

/// The mage's six strikes, each ULYSSES_STRIKE_INTERVAL frames after the last,
/// by the strike's own frame: violet flames at the mage's hand, then a flake
/// flies, a crescent of light sweeps round the targets and closes, and the
/// strike lands, with a burst of light and sparks.
#define ULYSSES_STRIKES 6
#define ULYSSES_STRIKE_INTERVAL 8
#define ULYSSES_STRIKE_FLAME_FRAMES 36
#define ULYSSES_STRIKE_FLAKE_FRAME 12
#define ULYSSES_STRIKE_FLAKE_FRAMES 32
#define ULYSSES_STRIKE_CRESCENT_FRAME 44
#define ULYSSES_STRIKE_CRESCENT_FRAMES 22
#define ULYSSES_STRIKE_CLOSE_FRAME 59
#define ULYSSES_STRIKE_POWER_FRAME 60
#define ULYSSES_STRIKE_HIT_FRAME 64
#define ULYSSES_STRIKE_BURST_FRAMES 12
/// The sparks each strike throws, in the effect's pictures.
#define ULYSSES_SPARKS 19352
#define ULYSSES_STRIKE_SPARKS 16
#define ULYSSES_SPARK_COUNT (ULYSSES_STRIKES * ULYSSES_STRIKE_SPARKS)

/// A white flake drifts down, a flame bursts and a ring of red runes opens
/// with the mage's silhouette glowing violet in it; the mage appears, and six
/// times violet flames gather at the mage's hand and a crescent of light
/// sweeps round the foes and strikes; then fire flares over the mage, who
/// fades, and a white flake flutters away.
///
/// Abilities: Ulysses.
void BattleEffect_RunUlysses(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleBackgroundState *background;
  struct BattleEffectState *state;
  s32 frame;
  u8 *sprites;
  s32 flakeAway;
  s32 skip;
  DrawRectangle *blit;
  struct {
    s32 x;
    s32 z;
  } mage;
  s32 loaded;
  s32 draw;
  s32 i;
  u8 *cells;
  u8 *picture;
  struct SpritePlacement place;
  BlitterPair blits;
  struct ModelTexture texture;
  struct ModelTexture quadTexture;
  struct Scale mageScale;
  struct BattleState *battle;
  s32 mask = RGB_LEVEL_MAX;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  background = Ram_HeapSlots->battleBackground;
  sprites = Ram_HeapSlots->battleSheet;
  flakeAway = 0;
  skip = 0;
  BattleEffect_ShowLivingTargets(state->args);
  // The battle background darkens to black, its green first.
  for (frame = 0; frame != 32; frame++) {
    volatile u16 *c = (volatile u16 *)&BG_PLTT_COLOR(6, 0);

    for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++) {
      s32 v = *c;
      s32 r = mask & v;
      s32 g;
      s32 b;

      g = (u16)RGB_GREEN((u16)v);
      b = (u16)RGB_BLUE((u16)v);
      if (g == 0) {
        if (b > 0) {
          b--;
        }
        if (r > 0) {
          r--;
        }
      }
      if (g > 0) {
        g--;
      }
      *c = RGB(r, g, b);
    }
    wait(1);
  }
  BattleEffect_StartCanvasLayer(0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  wait(1);
  // BG1 shows a canvas-sized picture from BG character block 2, as
  // BattleEffect_BuildCanvasTilemap writes it: the rune circles.
  {
    u32 *map = (u32 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK);
    u32 v;
    s32 y;
    s32 x;

    Iwram_FillWords(map, 4 * MAP_ROW_BYTES, -1);
    map += 4 * MAP_ROW_WORDS;
    Iwram_FillWords(map, 2 * MAP_ROW_BYTES,
                    MAP_ENTRY_PAIR(BG1_CLEAR_TILE, BG1_CLEAR_TILE));
    v = MAP_ENTRY_PAIR(BG1_BLOCK2_FIRST_TILE, BG1_BLOCK2_FIRST_TILE + 1);
    map += 2 * MAP_ROW_WORDS;
    for (y = 0; y != CANVAS_TILES; y++) {
      for (x = 0; x != 8; x++) {
        *map++ = v;
        v += MAP_ENTRY_PAIR(2, 2);
      }
      for (x = 0; x != 8; x++) {
        *map++ = MAP_ENTRY_PAIR(BG1_CLEAR_TILE, BG1_CLEAR_TILE);
      }
    }
  }
  BattleEffect_CreateSprites(ULYSSES_MAGE_SPRITES + 1, SPRITE_ULYSSES, 3);
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK);
  wait(1);
  blit = blits;
  BattleEffect_BuildCanvasBlitters(0, blit);
  // The silhouette, in the glow's palettes, goes to BG2's tiles and stays
  // there while the rune circles go to BG1's.
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  cells = state->sheet + ULYSSES_PICTURES;
  Resource_LoadAndDecompress(RESOURCE_ULYSSES_SILHOUETTE, cells, 0, 0);
  for (i = 0; i != PICTURE_BYTES(ULYSSES_SILHOUETTE); i++, cells++) {
    if (*cells) {
      *cells += ULYSSES_GLOW_PALETTE * PALETTE_COLORS;
    }
  }
  picture = state->sheet + ULYSSES_PICTURES;
  blits[0](canvas, picture, 24, 8, ULYSSES_SILHOUETTE_WIDTH,
           ULYSSES_SILHOUETTE_HEIGHT);
  EFFECT_PRESENT_CANVAS(state);
  Task_Remove(BattleEffect_TransferCanvas);
  Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLUTTERING_FLAKE_SHEET, picture, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sprites, 0, 0);
  loaded = 0;
  mage.x = toFIXED(200);
  mage.z = toFIXED(84);
  background->busy = 1;
  Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  wait(1);
  for (frame = 0; frame != ULYSSES_FRAMES; frame++) {
    struct Particle *particle;
    // Once A or B is pressed, odd frames are not drawn or waited for: the
    // rest plays twice as fast.
    draw = 1;
    if (gInput.repeat & (KEY_A | KEY_B)) {
      skip = 1;
    }
    if ((frame & draw) && skip == 1) {
      draw = 0;
    }
    if (frame == 0) {
      struct Particle *p;

      if (!loaded) {
        Task_Add(BattleEffect_ProcessPendingBlock2Transfer,
                 TASK_PASS_VBLANK + 15);
        Task_Add(Display_ApplyBg2Affine, TASK_PASS_VBLANK + 127);
        Resource_LoadAndDecompress(RESOURCE_RUNE_CIRCLE_SHEET,
                                   state->sheet + ULYSSES_RUNE_CIRCLE_PICTURES,
                                   1, 0);
        loaded = 1;
      }
      gBgScroll[1].x = -112;
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      REG_DISPCNT = EFFECT_DISPCNT;
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      REG_BG1CNT =
          BGCNT_256_COLORS | BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK);
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET),
                           SHEET_PALETTE_BYTES);
      for (i = 0; i != 9; i++) {
        p = &state->particles[i];
        p->variant = -2 * i;
        p->x = (Random16() & 15) + 60;
        p->y = (Random16() & 15) + 44;
      }
    }
    // The white flake drifts down.
    if (frame < ULYSSES_RUNE_CIRCLES_FRAME) {
      s32 k = (frame / 4) % ULYSSES_FLAKE_CELS;
      s32 x = 60 - (FIXEDtoINT(Trig_Sin(frame * ULYSSES_FLAKE_SWAY) << 3));
      s32 y = frame - 8;
      u8 *source = state->sheet + ULYSSES_PICTURES + gUlysses_Pictures[k];
      u8 w = gUlysses_PictureWidths[k];
      s32 left = x - (w >> 1);
      u8 h = gUlysses_PictureHeights[k];

      blit[1](canvas, source, left, y - (h >> 1), w, h);
    }
    // A flame bursts where it lands.
    if (frame >= ULYSSES_RUNE_CIRCLES_FRAME && frame < ULYSSES_APPEAR_FRAME) {
      struct Particle *p;

      for (i = 0, p = state->particles; i != 4; i++, p++) {
        if ((u32)p->variant < ULYSSES_BURST_FLAME_FRAMES) {
          s32 k = p->variant / 3;
          s32 side = i & 1;
          u8 *source = state->sheet + gBattleEffect_EmberStreakPictures[k];
          u8 w = gBattleEffect_EmberStreakPictureWidths[k];

          blit[side](canvas, source, p->x - (w >> 1),
                     p->y + gBattleEffect_EmberStreakPictureBiasY[k] - 32, w,
                     gBattleEffect_EmberStreakPictureHeights[k]);
        }
        p->variant++;
      }
    }
    if (frame == ULYSSES_APPEAR_FRAME) {
      REG_BLDALPHA = BLEND_ALPHA(16, 0);
    }
    if (frame >= ULYSSES_APPEAR_FRAME && frame <= 116) {
      REG_BLDALPHA = BLEND_ALPHA(16, (frame - ULYSSES_APPEAR_FRAME) * 2);
    }
    // The silhouette zooms in, swelling and shrinking back, and glows.
    if (frame >= ULYSSES_SILHOUETTE_FRAME && frame < ULYSSES_STRIKES_FRAME) {
      s32 t = frame - ULYSSES_SILHOUETTE_FRAME;
      FIXED distance = Trig_Cos(t * ULYSSES_SILHOUETTE_SWELL) / 3 + FIXED_ONE;
      s32 zoom;
      s32 level = -1;

      if (distance < toFIXED(33) / 32) {
        distance = toFIXED(33) / 32;
      }
      zoom = toFIXED(256) / distance;
      state->bg2pa = zoom;
      state->bg2pd = zoom;
      state->bg2x = (64 - toFIXED(184) / distance) << 8;
      state->bg2y = (64 - toFIXED(76) / distance) << 8;
      if (t < ULYSSES_GLOW_FRAMES) {
        level = t;
      } else if (frame >= 116 && frame <= ULYSSES_RUNE_CIRCLES_FADE_FRAME) {
        level = ULYSSES_GLOW_END * 2 - t * 2;
      }
      if (level != -1) {
        for (i = 0; i != ULYSSES_GLOW_COLORS; i++) {
          s32 red = ULYSSES_GLOW_RED(i, level);
          s32 blue = ULYSSES_GLOW_BLUE(i, level);

          ((u16 *)&BG_PLTT_COLOR(ULYSSES_GLOW_PALETTE, 0))[i] =
              RGB(red, 0, blue);
        }
      }
      if (frame == 116) {
        REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                      DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                      DISPCNT_WIN1_ON;
      }
    }
    // The rune circles' colours fade by 3 a frame.
    if (frame >= ULYSSES_RUNE_CIRCLES_FADE_FRAME &&
        frame < ULYSSES_STRIKES_FRAME) {
      volatile u16 *c = (volatile u16 *)BG_PLTT;

      for (i = 0; i != SHEET_PALETTE_COLORS; i++, c++) {
        s32 v = *c;
        s32 low = RGB_LEVEL_MAX;
        s32 r;
        s32 g;
        s32 b;

        low &= v;
        r = low - 3;
        g = ((u16)RGB_GREEN((u16)v)) - 3;
        b = ((u16)RGB_BLUE((u16)v)) - 3;

        if (r < 0)
          r = 0;
        if (g < 0)
          g = 0;
        if (b < 0)
          b = 0;
        *c = RGB(r, g, b);
      }
    }
    if (frame == ULYSSES_STRIKES_FRAME) {
      if (loaded == 1) {
        Task_Remove(BattleEffect_ProcessPendingBlock2Transfer);
        Task_Remove(Display_ApplyBg2Affine);
        Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK + 15);
        Resource_LoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET,
                                   state->sheet + ULYSSES_YELLOW_ORB_PICTURES,
                                   1, 0);
        loaded = 0;
      }
      REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                    DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
      REG_BG2X = -(8 << 8);
      REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
      REG_BG2PA = Q_8_8(0.8);
      REG_BG2PD = Q_8_8(1);
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_RUNE_SHEET),
                           SHEET_PALETTE_BYTES);
    }
    if (frame == ULYSSES_STRIKES_FRAME + 1) {
      struct Particle *p;
      struct Particle *spark;

      Iwram_FillWords(canvas, CANVAS_VIEW_BYTES, 0);
      Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
      REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                    DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                    DISPCNT_WIN1_ON;
      spark = (struct Particle *)(state->sheet + ULYSSES_SPARKS);
      for (i = 0; i != ULYSSES_STRIKES; i++) {
        p = &state->particles[i];
        p->x = INTtoFIXED(gUlysses_ParticleStarts[i][0]);
        p->variant = -ULYSSES_STRIKE_INTERVAL * i;
        p->y = INTtoFIXED(gUlysses_ParticleStarts[i][1]);
        p->z = gUlysses_ParticleStarts[i][2] << 6;
        p->velocityX = gUlysses_ParticleStarts[i][3] * (toFIXED(1) / 2);
        p->velocityY = gUlysses_ParticleStarts[i][4] * (toFIXED(1) / 2);
        p->velocityZ = 0;
      }
      for (i = 0; i != ULYSSES_SPARK_COUNT; i++) {
        spark[i].variant = -1;
      }
      Resource_LoadAndDecompress(RESOURCE_FADING_CRESCENT_SHEET,
                                 gMapGridCellBuffer, 0, 0);
    }
    if (frame == ULYSSES_RUNE_CIRCLES_FRAME) {
      Sound_PlayFar(SE_ITEM_BREAK);
    }
    if (frame == ULYSSES_FLARE_FRAME) {
      Sound_PlayFar(SE_ITEM_BREAK);
    }

    if (frame > ULYSSES_STRIKES_FRAME && frame < ULYSSES_FLARE_FRAME) {
      void *verts;
      struct RenderModel *model;
      struct Particle *sparks;
      u8 *crescents = gMapGridCellBuffer;
      struct Particle *spark;
      s32 shade;
      s32 j;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      shade = 0;
      sparks = (struct Particle *)(state->sheet + ULYSSES_SPARKS);
      if (frame > 196) {
        BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
      }
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->shade = shade;
      model->texture = &texture;
      model->mesh = gBattleEffect_SquareMesh;
      model->vertices = verts;
      for (i = 0, particle = state->particles; i != ULYSSES_STRIKES;
           particle++, i++) {
        s32 delay = i * ULYSSES_STRIKE_INTERVAL;
        struct Particle *anchor = particle + 8;
        s32 t;

        if (++particle->variant == 0) {
          Sound_PlayFar(SE_ITEM_BREAK);
        }
        t = particle->variant;
        if (t == ULYSSES_STRIKE_POWER_FRAME) {
          Sound_PlayFar(SE_POWER_UP);
          t = particle->variant;
        }
        if (t == ULYSSES_STRIKE_HIT_FRAME) {
          Sound_PlayFar(SE_HEAVY_IMPACT);
          t = particle->variant;
        }
        // Violet flames at the mage's hand.
        if ((u32)t < ULYSSES_STRIKE_FLAME_FRAMES) {
          s32 k = (t / 6) % 6;
          u8 *source = state->sheet + gBattleEffect_EmberStreakPictures[k];
          u8 w = gBattleEffect_EmberStreakPictureWidths[k];

          blits[0](canvas, source, FIXEDtoINT(particle->x) - (w >> 1),
                   FIXEDtoINT(particle->y) +
                       gBattleEffect_EmberStreakPictureBiasY[k] - 36,
                   w, gBattleEffect_EmberStreakPictureHeights[k]);
          t = particle->variant;
        }
        // A flake flies out.
        if ((u32)(t - ULYSSES_STRIKE_FLAKE_FRAME) <
            ULYSSES_STRIKE_FLAKE_FRAMES) {
          s32 k = ((t - ULYSSES_STRIKE_FLAKE_FRAME) / 4) % ULYSSES_FLAKE_CELS;
          u8 *source = state->sheet + ULYSSES_PICTURES + gUlysses_Pictures[k];
          s32 x = FIXEDtoINT(particle->x);
          u8 w = gUlysses_PictureWidths[k];
          s32 left = x - (w >> 1);
          s32 y = FIXEDtoINT(particle->y);
          u8 h = gUlysses_PictureHeights[k];

          blit[1](canvas, source, left, y - (h >> 1), w, h);
          t = particle->variant;
        }
        // The crescent of light sweeps round the targets, wobbling, and
        // closes from ULYSSES_STRIKE_CLOSE_FRAME.
        if ((u32)(t - ULYSSES_STRIKE_CRESCENT_FRAME) <
            ULYSSES_STRIKE_CRESCENT_FRAMES) {
          FIXED scale =
              ULYSSES_CRESCENT_SCALE(t - ULYSSES_STRIKE_CRESCENT_FRAME);
          s32 vx;
          s32 x = particle->x;
          FIXED mx = x - toFIXED(64);
          FIXED my = particle->y - toFIXED(64);
          s32 closing = t - ULYSSES_STRIKE_CLOSE_FRAME;

          if (closing < 0) {
            closing = 0;
          }
          vx = particle->velocityX;
          particle->y += particle->velocityY;
          x += vx;
          particle->x = x;
          if (x < toFIXED(64)) {
            particle->velocityX = vx + toFIXED(1) / 2;
          }
          if (particle->y < toFIXED(60)) {
            particle->velocityY += toFIXED(1) / 2;
          } else {
            particle->velocityY -= toFIXED(1) / 2;
          }
          if (particle->z >= 0) {
            particle->z += FIXEDtoINT(Trig_Sin(particle->velocityZ) * 3 << 8);
          } else {
            particle->z -= FIXEDtoINT(Trig_Sin(particle->velocityZ) * 3 << 8);
          }
          particle->velocityZ += TRIG_TURN / 64;
          if (draw) {
            // As an integer sum, the offset first.
            texture.pixels = (u8 *)(closing * PICTURE_BYTES(ULYSSES_CRESCENT) +
                                    (s32)crescents);
            Matrix_Initialize();
            Matrix_Translate(mx, my, 0);
            Matrix_RotateX(TRIG_TURN * 3 / 4);
            Matrix_RotateY(particle->z);
            Matrix_ScaleUniform(scale);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     4);
            Render_DrawModelList(model);
          }
          t = particle->variant;
        }
        // The strike lands: sparks fly, and every target is jolted and
        // flashes.
        if (t == ULYSSES_STRIKE_HIT_FRAME) {
          anchor->x = FIXEDtoINT(particle->x);
          anchor->y = FIXEDtoINT(particle->y);
          j = 0;
          {
            s32 first = i * ULYSSES_STRIKE_SPARKS;

            // As an integer sum, the offset first.
            spark =
                (struct Particle *)(first * (s32)sizeof(*spark) + (s32)sparks);
          }
          for (; j != ULYSSES_STRIKE_SPARKS; j++, spark++) {
            s32 angle = Random16() & TRIG_ANGLE_MASK;
            s32 speed = (Random16() & 15) + 8;

            spark->variant = (Random16() & 7) + 8;
            spark->x = particle->x;
            spark->y = particle->y;
            spark->velocityX = (Trig_Sin(angle) * speed) / 4;
            spark->velocityY = (Trig_Cos(angle) * speed) / 4;
          }
          for (j = 0; j != args->targetCount; j++) {
            s32 lift = (Random16() << 2) + toFIXED(1);

            BattleEffect_StartActorMotion(args->targets[j], 1, toFIXED(2),
                                          toFIXED(8), lift,
                                          delay + (Random16() & 31) + 52);
            BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                     TINT_KEEP, j, 4);
          }
        }
        // A burst of light where it landed.
        if (draw && (u32)(particle->variant - ULYSSES_STRIKE_HIT_FRAME) <
                        ULYSSES_STRIKE_BURST_FRAMES) {
          s32 k = ((particle->variant - ULYSSES_STRIKE_HIT_FRAME) / 2) % 6;

          blit[1](canvas,
                  state->sheet + ULYSSES_YELLOW_ORB_PICTURES +
                      k * PICTURE_BYTES(YELLOW_ORB),
                  anchor->x - YELLOW_ORB_WIDTH / 2,
                  anchor->y - YELLOW_ORB_HEIGHT / 2, YELLOW_ORB_WIDTH,
                  YELLOW_ORB_HEIGHT);
        }
      }
      for (i = 0; i != ULYSSES_SPARK_COUNT; i++) {
        spark = &sparks[i];
        if (spark->variant >= 0) {
          s32 n = spark->variant / 4 + 2;

          if (draw) {
            blit[1](canvas, sprites + gBattleEffect_FlarePictures[n - 1],
                    FIXEDtoINT(spark->x) - n, FIXEDtoINT(spark->y) - n, n * 2,
                    n * 2);
          }
          Particle_AdvanceWithGravity2D(spark, 60, toFIXED(1) / 8);
          spark->variant--;
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    if (draw && frame < ULYSSES_STRIKES_FRAME) {
      void *verts;
      struct RenderModel *model;
      s32 j;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      /* FAKEMATCH(dead): j takes frame * 3 + 1 here and 0 below before any
       * read. */
      j = frame * 3 + 1;
      model->shade = 0;
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      quadTexture.logWidth = 7;
      quadTexture.logHeight = 7;
      model->texture = &quadTexture;
      quadTexture.pixels = state->sheet + ULYSSES_RUNE_CIRCLE_PICTURES;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = verts;
      j = 0;
      for (; j != 2; j++) {
        s32 t = frame - ULYSSES_RUNE_CIRCLES_FRAME -
                j * ULYSSES_RUNE_CIRCLE_INTERVAL;

        if (t >= 0) {
          s32 scale;

          Matrix_Initialize();
          Matrix_Translate(toFIXED(8), 0, 0);
          Matrix_RotateX(DEGtoANG(90));
          if (j == 0) {
            scale = t * (FIXED_ONE * 3 / 64) - FIXED_ONE * 9 / 8;
            if (t < ULYSSES_RUNE_CIRCLE_HOLD_FRAMES) {
              scale = t * (FIXED_ONE / 32);
              if (scale > FIXED_ONE * 3 / 4) {
                scale = FIXED_ONE * 3 / 4;
              }
            }
            Matrix_ScaleUniform(scale);
            Matrix_RotateY(-t * (TRIG_TURN / 128));
          } else {
            if (t >= ULYSSES_RUNE_CIRCLE_TILT_FRAMES) {
              scale = FIXED_ONE * 7 / 4 - t * (FIXED_ONE / 32);
              Matrix_ScaleUniform(scale);
            } else {
              Matrix_RotateX(ULYSSES_RUNE_CIRCLE_TILT_X(t));
              Matrix_RotateZ(ULYSSES_RUNE_CIRCLE_TILT_Z(t));
            }
            Matrix_RotateY(t * (TRIG_TURN / 128));
          }
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    // The mage stands from ULYSSES_APPEAR_FRAME, swaying once the strikes
    // begin.
    if (frame >= ULYSSES_APPEAR_FRAME && frame < ULYSSES_FLAKE_FRAME) {
      mageScale = gUlysses_MageScale;
      if (frame > ULYSSES_STRIKES_FRAME - 1) {
        s32 u = frame - ULYSSES_STRIKES_FRAME;

        mage.x = (Trig_Sin(u * (TRIG_TURN / 32)) << 1) + toFIXED(200);
        mage.z = (Trig_Sin(u * (TRIG_TURN / 64)) << 1) + toFIXED(84);
      }
      if (draw) {
        place.height = place.groundHeight = 0;
        for (i = 0; i != ULYSSES_MAGE_SPRITES; i++) {
          place.x = INTtoFIXED(gUlysses_ObjectX[i]) + mage.x - toFIXED(32);
          place.y = INTtoFIXED(gUlysses_ObjectZ[i]) + mage.z - toFIXED(32);
          Render_ApplyProjectedPlacementFar(state->sprites[i], &place.x,
                                            (s32 *)&mageScale, 0);
        }
      }
    }
    // Fire flares over the mage in a column of flames.
    if (frame >= ULYSSES_FLARE_FRAME) {
      if (frame == ULYSSES_FLARE_FRAME) {
        struct Particle *p;

        REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
        REG_BG2PA = Q_8_8(1);
        REG_BG2PD = Q_8_8(1);
        for (i = 0, p = state->particles; i != ULYSSES_FLARE_FLAMES; i++, p++) {
          p->x = Random16() % 48 + 40;
          p->y = Random16() % 96 + 16;
          p->variant = -i / 2;
        }
      }
      for (i = 0, particle = state->particles; i != ULYSSES_FLARE_FLAMES;
           i++, particle++) {
        if ((u32)particle->variant < ULYSSES_FLARE_FLAME_FRAMES && draw) {
          s32 k = (particle->variant / 4) % 6;
          u8 *source = state->sheet + gBattleEffect_EmberStreakPictures[k];
          u8 w = gBattleEffect_EmberStreakPictureWidths[k];

          blits[0](canvas, source, particle->x - (w >> 1),
                   particle->y + gBattleEffect_EmberStreakPictureBiasY[k] - 48,
                   w, gBattleEffect_EmberStreakPictureHeights[k]);
        }
        particle->variant++;
      }
    }
    if (frame == ULYSSES_FLAKE_FRAME) {
      Task_Add(Ulysses_FlutterFlake, TASK_PASS_FRAME);
      ULYSSES_FLAKE.x = toFIXED(188);
      ULYSSES_FLAKE.y = toFIXED(96);
      ULYSSES_FLAKE.velocityX = 0;
      ULYSSES_FLAKE.velocityY = 0;
      state->frame = 0;
      flakeAway = 1;
    }
    if (draw) {
      EFFECT_END_FRAME(state);
    }
  }

  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK);
  wait(1);
  /* FAKEMATCH(dead): a once-only block. */
  {
    void *destination = Ram_HeapSlots->battleCanvas;

    IWRAM_FILL_WORDS(destination, CANVAS_BYTES, 0);
  }
  IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES, 0);
  Iwram_FillWords((void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES, 0);
  wait(1);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  background->busy = 0;
  battle = Ram_HeapSlots->battleState;
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  gBgScroll[1].x = state->savedBg1X;
  gBgScroll[1].y = state->savedBg1Y;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Iwram_ClearWords((void *)&BG_PLTT_COLOR(6, 0), 256);
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_ZOOMED, battle->background, -1);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
  Task_Remove(BattleEffect_DarkenBackgroundStep);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  REG_BLDCNT = 0;
  wait(1);
  // The battle background's colours come back, red and blue first.
  for (frame = 0; frame != 40; frame++) {
    u16 *src = battle->palette;
    volatile u16 *dst = (volatile u16 *)&BG_PLTT_COLOR(6, 0);

    for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, src++, dst++) {
      s32 v = *src;
      s32 r = RGB_LEVEL_MAX;
      s32 g;
      s32 b;
      s32 nr;
      s32 ng;
      s32 nb;

      r &= v;
      g = (u16)RGB_GREEN((u16)v);
      b = (u16)RGB_BLUE((u16)v);
      nr = r + frame - 30;
      ng = g + frame - 40;
      nb = b + frame - 30;
      if (nr < 0) {
        nr = 0;
      } else if (nr > r) {
        nr = r;
      }
      if (ng < 0) {
        ng = 0;
      } else if (ng > g) {
        ng = g;
      }
      if (nb < 0) {
        nb = 0;
      } else if (nb > b) {
        nb = b;
      }
      *dst = RGB(nr, ng, nb);
    }
    wait(1);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  if (flakeAway == 1) {
    Task_Remove(Ulysses_FlutterFlake);
  }
  for (i = 0; i != ULYSSES_MAGE_SPRITES + 1; i++) {
    Sprite_DestroyFar(state->sprites[i]);
  }
}

#undef ULYSSES_MAGE_SPRITES
#undef ULYSSES_FLAKE_SPRITE
#undef ULYSSES_FLAKE_ANIMATION
#undef ULYSSES_FLAKE
#undef ULYSSES_PICTURES
#undef ULYSSES_SILHOUETTE_WIDTH
#undef ULYSSES_SILHOUETTE_HEIGHT
#undef ULYSSES_FLAKE_CELS
#undef ULYSSES_RUNE_CIRCLE_PICTURES
#undef ULYSSES_YELLOW_ORB_PICTURES
#undef ULYSSES_CRESCENT_WIDTH
#undef ULYSSES_CRESCENT_HEIGHT
#undef ULYSSES_CRESCENT_SCALE
#undef ULYSSES_FRAMES
#undef ULYSSES_RUNE_CIRCLES_FRAME
#undef ULYSSES_RUNE_CIRCLE_INTERVAL
#undef ULYSSES_SILHOUETTE_FRAME
#undef ULYSSES_APPEAR_FRAME
#undef ULYSSES_RUNE_CIRCLES_FADE_FRAME
#undef ULYSSES_STRIKES_FRAME
#undef ULYSSES_FLARE_FRAME
#undef ULYSSES_FLAKE_FRAME
#undef ULYSSES_FLAKE_SWAY
#undef ULYSSES_SILHOUETTE_SWELL
#undef ULYSSES_GLOW_PALETTE
#undef ULYSSES_GLOW_COLORS
#undef ULYSSES_GLOW_FRAMES
#undef ULYSSES_GLOW_END
#undef ULYSSES_GLOW_RED
#undef ULYSSES_GLOW_BLUE
#undef ULYSSES_RUNE_CIRCLE_HOLD_FRAMES
#undef ULYSSES_RUNE_CIRCLE_TILT_FRAMES
#undef ULYSSES_RUNE_CIRCLE_TILT_X
#undef ULYSSES_RUNE_CIRCLE_TILT_Z
#undef ULYSSES_BURST_FLAME_FRAMES
#undef ULYSSES_FLARE_FLAME_FRAMES
#undef ULYSSES_FLARE_FLAMES
#undef ULYSSES_STRIKES
#undef ULYSSES_STRIKE_INTERVAL
#undef ULYSSES_STRIKE_FLAME_FRAMES
#undef ULYSSES_STRIKE_FLAKE_FRAME
#undef ULYSSES_STRIKE_FLAKE_FRAMES
#undef ULYSSES_STRIKE_CRESCENT_FRAME
#undef ULYSSES_STRIKE_CRESCENT_FRAMES
#undef ULYSSES_STRIKE_CLOSE_FRAME
#undef ULYSSES_STRIKE_POWER_FRAME
#undef ULYSSES_STRIKE_HIT_FRAME
#undef ULYSSES_STRIKE_BURST_FRAMES
#undef ULYSSES_SPARKS
#undef ULYSSES_STRIKE_SPARKS
#undef ULYSSES_SPARK_COUNT

/// The swirl swipe's modes (BattleEffect_RunSwirlSwipe), each named for the
/// effect that plays it.
enum SwirlSwipeMode {
  SWIRL_SWIPE_POISON_BEAT,  ///< lime; the target recoils at once, 48 frames
  SWIRL_SWIPE_SPINNING_BEAT ///< magenta; the target is thrown up, 74 frames
};

/// A lime swirl sweeps across the first target and knocks it back.
///
/// Abilities: Poison Beat.
void BattleEffect_RunPoisonBeat(struct BattleEffectParams *args) {
  BattleEffect_RunSwirlSwipe(args, SWIRL_SWIPE_POISON_BEAT);
}

/// A magenta swirl sweeps across the first target and throws it into the air.
///
/// Abilities: Spinning Beat.
void BattleEffect_RunSpinningBeat(struct BattleEffectParams *args) {
  BattleEffect_RunSwirlSwipe(args, SWIRL_SWIPE_SPINNING_BEAT);
}

/// A swirl sweeps across the first target from the user's side.
void BattleEffect_RunSwirlSwipe(struct BattleEffectParams *args, s32 mode) {
  struct HeapSlots *heap;
  struct BattleEffectState *state;
  void *canvas;
  struct MotionObject *target;
  struct ScreenPoint position;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 picture;

  // The canvas at its own width, and a transparent blitter, mirrored when the
  // targets are the party.
  heap = Ram_HeapSlots;
  state = (struct BattleEffectState *)heap->battleEffect;
  canvas = heap->battleCanvas;
  EFFECT_START_NARROW_CANVAS(0);
  if (args->side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_TRANSPARENT);
    draw[0] = heap->ramCode;
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_TRANSPARENT);
    draw[0] = heap->ramCode;
  }
  Resource_LoadAndDecompress(RESOURCE_MAGENTA_SWIRL_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_MAGENTA_TAIL_SHEET, Ram_MapCellBuffer, 1,
                             0);
  if (mode == SWIRL_SWIPE_POISON_BEAT) {
    LOAD_BG_PALETTE(RESOURCE_LIME_PALETTE);
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  // The canvas is placed on the first target.
  target = BattleObject_GetFar(args->targets[0])->object;
  EffectPosition_ProjectScaledBattler(args->targets[0], &position);
  if (args->side == EFFECT_SIDE_ENEMIES)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != SWIRL_SWIPE_SPINNING_BEAT)
    frames = 48;

  for (frame = 0; frame != frames; frame++) {
    // The swirl: six pictures, four frames each, the last two from the tail.
    picture = frame / 4;
    if (picture <= 5) {
      if (picture <= 3)
        draw[0](canvas, state->sheet + gSwirlSwipe_Pictures[picture],
                gSwirlSwipe_PictureX[picture + args->side * 6],
                gSwirlSwipe_PictureBiasY[picture] + 32,
                gSwirlSwipe_PictureWidths[picture],
                gSwirlSwipe_PictureHeights[picture]);
      else
        draw[0](canvas, Ram_MapCellBuffer + gSwirlSwipe_Pictures[picture],
                gSwirlSwipe_PictureX[picture + args->side * 6],
                gSwirlSwipe_PictureBiasY[picture] + 32,
                gSwirlSwipe_PictureWidths[picture],
                gSwirlSwipe_PictureHeights[picture]);
    }
    if (frame == 8) {
      if (mode == SWIRL_SWIPE_POISON_BEAT) {
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HOP);
      } else {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      }
      state->shakeFrames = 8;
    }
    // Spinning Beat throws the target up at frame 13; it lands at frame 65.
    if (mode == SWIRL_SWIPE_SPINNING_BEAT) {
      if (frame == 13) {
        target->velocityY = toFIXED(12);
        target->verticalMotionStrength = toFIXED(47) / 100;
        target->verticalMotionPhase = FIXED_ONE / 4;
      }
      if (frame == 65) {
        state->shakeFrames = 4;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPIDER_WEB_FRAMES 63
/// SpiderWebSheet's quarter of the web, a square drawn four times mirrored.
#define SPIDER_WEB_QUARTER_SIZE 32

/// Builds the transparent blitter for one quarter.
#define LOAD_QUARTER(flags)                                                    \
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, flags, BLITTER_TRANSPARENT)

/// Draws one quarter of the web with the blitter just built.
#define DRAW_QUARTER(x, y)                                                     \
  (Ram_HeapSlots->ramCode)(canvas, state->sheet, x, y, size, size)

/// A spider web spreads over the targets in four mirrored quarters, fades,
/// and Impair's effect follows.
///
/// Abilities: Spider Web.
void BattleEffect_RunSpiderWeb(struct BattleEffectParams *args) {
  struct ScreenPoint middle;
  void *canvas;
  struct BattleEffectState *state;
  volatile u16 *blend;
  s32 size;
  s32 flags;
  s32 frame;
  s32 i;

  // The canvas, unseen at first, on the targets' middle.
  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(2));
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
  BattleEffect_GetTargetsMidpoint(&middle);
  REG_BG2X = (64 - middle.x) << 8;
  Resource_LoadAndDecompress(RESOURCE_SPIDER_WEB_SHEET, state->sheet, 1, 1);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  Sound_PlayFar(SE_CRACK);
  blend = &REG_BLDALPHA;

  frame = 0;
  size = SPIDER_WEB_QUARTER_SIZE;
  for (; frame != SPIDER_WEB_FRAMES; frame++) {
    // It fades in over its first nine frames and out over its last nine.
    if (frame <= 8)
      *blend = EFFECT_BLEND_CANVAS(frame << 1);
    if (frame > 53)
      *blend = EFFECT_BLEND_CANVAS(124 - (frame << 1));

    // The four quarters: the one picture as it is, mirrored, flipped, and
    // both. Each blitter is built, drawn with and released before the next
    // is built.
    flags = EFFECT_CLIP_BOUNDS;
    LOAD_QUARTER(flags);
    DRAW_QUARTER(33, 41);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
    LOAD_QUARTER(flags);
    DRAW_QUARTER(64, 41);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y;
    LOAD_QUARTER(flags);
    DRAW_QUARTER(33, 72);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP;
    LOAD_QUARTER(flags);
    DRAW_QUARTER(64, 72);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);

    // Every target flashes at frame 10, and the damage shows at 32 with a
    // crack.
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_CRACK);
    for (i = 0; i != args->targetCount; i++) {
      if (frame == 10)
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
    }
    EFFECT_END_FRAME(state);
  }

  // Both canvases are cleared, and Impair's effect follows: the targets'
  // Defense drops.
  Task_Remove(BattleEffect_TransferCanvas);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
  Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
  args->prelude = 0;
  Task_Remove(BattleEffect_DarkenBackgroundStep);
  BattleEffect_RunImpair(args);
}

#undef LOAD_QUARTER
#undef DRAW_QUARTER
#undef SPIDER_WEB_FRAMES
#undef SPIDER_WEB_QUARTER_SIZE
