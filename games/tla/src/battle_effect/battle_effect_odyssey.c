/// @file
/// Odyssey, a cloud swirl nothing shows, Iris's screen wipe and teardown, and
/// the shade ramps Iris's textures are filled with.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define ODYSSEY_FRAMES 114

/// The effect's pictures: GiantSwordSheet's sword at their start, then
/// GiantSwordBladeSheet's top half of the colossal sword, then DustBlobSheet.
#define ODYSSEY_SWORD_WIDTH 40
#define ODYSSEY_SWORD_HEIGHT 80
#define ODYSSEY_COLOSSAL_SWORD PICTURE_BYTES(ODYSSEY_SWORD)
#define ODYSSEY_COLOSSAL_SWORD_WIDTH 158
#define ODYSSEY_COLOSSAL_SWORD_HEIGHT 56
#define ODYSSEY_DUST_BLOB                                                      \
  (ODYSSEY_COLOSSAL_SWORD + PICTURE_BYTES(ODYSSEY_COLOSSAL_SWORD))

/// Where FlameGradientTexture is decoded in the map cell buffer, after
/// FireRingTexture's 128 by 128 texels.
#define ODYSSEY_FLAME_GRADIENT (128 * 128)

/// The shards the hit scatters, in the map cell buffer.
#define ODYSSEY_SHARDS ((struct Particle *)(Ram_MapCellBuffer + 24576))
#define ODYSSEY_SHARD_COUNT 128
/// The shards' canvas levels, eight from this one.
#define ODYSSEY_SHARD_LEVEL 32

/// The two swords plunge onto the target ODYSSEY_SWORD_INTERVAL frames apart
/// from ODYSSEY_SWORD_FRAME, the first from above and the second from below;
/// each strikes ODYSSEY_SWORD_PLUNGE_FRAMES later and stays until the
/// colossal sword's blow.
#define ODYSSEY_SWORDS 2
#define ODYSSEY_SWORD_FRAME 24
#define ODYSSEY_SWORD_INTERVAL 16
#define ODYSSEY_SWORD_PLUNGE_FRAMES 8
#define ODYSSEY_SWORD_HIT_FRAME                                                \
  (ODYSSEY_SWORD_FRAME + ODYSSEY_SWORD_PLUNGE_FRAMES)
/// The sparks each sword's hit scatters, in its half of the particles.
#define ODYSSEY_SWORD_SPARKS 16

/// The rings of light, FireRingTexture flashing out from the caster as the
/// effect starts and from the target as each sword strikes. Each is drawn for
/// ODYSSEY_RING_FRAMES: it shines for ODYSSEY_RING_SHINE_FRAMES, then fades 8
/// levels a frame.
#define ODYSSEY_RING_FRAMES 16
#define ODYSSEY_RING_SHINE_FRAMES 8

/// From ODYSSEY_GATHER_FRAME, two rings of light close in on the caster, each
/// for ODYSSEY_GATHER_RING_FRAMES, and the caster flashes white with each.
#define ODYSSEY_GATHER_FRAME 64
#define ODYSSEY_GATHER_RING_FRAMES 11

/// "Pierce a foe with a colossal sword." The red haze, turned blue, scrolls
/// behind the caster and the target alone; two rings of light flash out from
/// the caster, and two swords plunge onto the target, one from above and one
/// from below, each with a ring and a burst of sparks. Rings close in on the
/// caster, who charges (frame 84); the colossal sword sweeps across the screen
/// (86) and pierces the target (90), scattering 128 tumbling shards, and from
/// frame 100 everything fades to white before the battle's own backdrop
/// returns.
///
/// Abilities: Odyssey.
void BattleEffect_RunOdyssey(struct BattleEffectParams *args) {
  struct ScreenPoint to;
  struct ScreenPoint from;
  s16 list[UNIT_MAX];
  DrawRectangle draw[2];
  struct ModelTexture texture;
  struct BattleEffectState *state;
  void *canvas;
  u8 *graphics;
  struct MotionObject *caster;
  struct BattleBackgroundState *transition;
  s32 savedScrollX;
  struct RenderModel *model;
  void *projected;
  struct Particle *step;
  s32 frame;
  s32 i;
  s32 k;
  s32 fillMode;
  s32 flatKind;
  u8 *pixels;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GIANT_SWORD_SHEET, state->sheet, 0, 0);
  // DEAD: DustBlobSheet, which Megiddo draws, is decoded and never drawn; the
  // ROM keeps the load.
  Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET,
                             state->sheet + ODYSSEY_DUST_BLOB, 1, 0);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  // The blitter faces the caster's side.
  if (args->side == EFFECT_SIDE_ENEMIES)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  caster = BattleObject_GetFar(args->user)->object;
  transition = Ram_HeapSlots->battleBackground;
  savedScrollX = gBgScroll[1].x;

  for (frame = 0; frame != ODYSSEY_FRAMES; frame++) {
    EffectPosition_ProjectScaledBattler(args->user, &from);
    EffectPosition_ProjectScaledBattler(args->targets[0], &to);

    if (frame == 0) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, gMapGridCellBuffer,
                                 0, 0);
      // DEAD: FlameGradientTexture, which Megiddo draws, is decoded and never
      // drawn; the ROM keeps the load.
      Resource_LoadAndDecompress(RESOURCE_FLAME_GRADIENT_TEXTURE,
                                 Ram_MapCellBuffer + ODYSSEY_FLAME_GRADIENT, 0,
                                 0);
      Graphics_LoadGreyRamp();
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      EFFECT_FREE_PARTICLES(i, state->particles, ARRAY_COUNT(state->particles));
    }

    // Frame 2: only the caster and the target, over the red haze turned blue:
    // each colour's red becomes its blue and half its green, and its blue its
    // red.
    if (frame == 2) {
      u16 *palette;

      list[0] = args->user;
      list[1] = args->targets[0];
      list[2] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForListFar(list, 0);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_RED_HAZE_BACKDROP, 0);
      BattleEffect_BuildRepeatingTilemap();
      palette = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
        s32 entry = *palette;
        s32 red = RGB_LEVEL_MAX;
        u16 green;
        s32 blue;

        red &= entry;
        /* LEFTOVER(template): green, taken and clamped as
         * BattleEffect_RunCoatlicue and BattleEffect_RunJudgment take all three
         * channels in their palette loops, and left out of the colour here; as
         * no green at all. */
        green = RGB_GREEN((u16)entry);
        blue = RGB_BLUE((u16)entry);
        red = red > RGB_LEVEL_MAX ? RGB_LEVEL_MAX : red;
        green = green > RGB_LEVEL_MAX ? RGB_LEVEL_MAX : green;
        blue = blue > RGB_LEVEL_MAX ? RGB_LEVEL_MAX : blue;
        *palette++ = RGB(blue, (u16)red >> 1, red);
      }
      transition->busy = 1;
    }
    if (frame > 1)
      gBgScroll[1].x += 8;

    if (frame == 8) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      step = state->particles;
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        step->x = Random16() % 216 - 96;
        step->y = Random16() % 216;
        step++;
      }
    }
    if (frame == 86)
      Resource_LoadAndDecompress(RESOURCE_GIANT_SWORD_BLADE_SHEET,
                                 state->sheet + ODYSSEY_COLOSSAL_SWORD, 1, 1);

    // Frame 84: the caster charges at the target.
    if (frame == 84) {
      Sound_PlayFar(SE_POWER_UP);
      Actor_SelectAnimationFar(caster, POSE_LEAP);
      Actor_SetAnimationSpeedFar(caster, ANIM_STEP_NORMAL * 3);
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 7, 80);
    }

    // Frame 90: the colossal sword's blow, and the shards scatter away from
    // the caster.
    if (frame == 90) {
      struct Particle *shard;

      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      Sound_PlayFar(SE_CLATTER);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      state->shakeFrames = 8;
      shard = ODYSSEY_SHARDS;
      for (i = 0; i != ODYSSEY_SHARD_COUNT; i++) {
        s32 speed = (Random16() & 0x1ff) + 32;
        s32 angle = Random16() & 0x7fff;

        if (args->side == EFFECT_SIDE_PARTY)
          angle += DEGtoANG(180);
        shard->x = INTtoFIXED(to.x / 2);
        shard->y = INTtoFIXED(to.y);
        shard->velocityX = (Trig_Sin(angle) * speed) >> 6;
        shard->velocityY = -(Trig_Cos(angle) * speed) >> 5;
        shard->z = Random16();
        shard->velocityZ = (Random16() & 0x3ff) + toFIXED(1) / 64;
        shard->variant = (Random16() & 0x3fff) + FIXED_ONE / 8;
        shard++;
      }
    }

    // From frame 86: the colossal sword sweeps across the screen, drawn in two
    // halves, the lower one mirrored.
    if (frame > 85) {
      s32 x;

      if (args->side == EFFECT_SIDE_ENEMIES) {
        x = 2872 - frame * 32;
        if (x < -38)
          x = -38;
      } else {
        x = frame * 32 - 2910;
        if (x > 0)
          x = 0;
      }
      draw[0](canvas, state->sheet + ODYSSEY_COLOSSAL_SWORD, x, 8,
              ODYSSEY_COLOSSAL_SWORD_WIDTH, ODYSSEY_COLOSSAL_SWORD_HEIGHT);
      if (args->side == EFFECT_SIDE_ENEMIES)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
      draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
      draw[1](canvas, state->sheet + ODYSSEY_COLOSSAL_SWORD, x, 64,
              ODYSSEY_COLOSSAL_SWORD_WIDTH, ODYSSEY_COLOSSAL_SWORD_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }

    // Until frame 89: the sparks of the swords' hits, shrinking.
    if (frame <= 89) {
      for (i = 0, step = state->particles; i != ARRAY_COUNT(state->particles);
           i++) {
        if (step->variant >= 0) {
          s32 size = (step->variant >> 2) + 1;

          draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(step->x) - size / 2,
                  FIXED_INTEGER_PART(step->y) - size, size, size * 2);
          Particle_AdvanceWithGravity2D(step, 60, 0);
          step->variant--;
        }
        step++;
      }
    }

    // The two swords, the second flipped, each scattering its sparks as it
    // hits.
    for (i = 0; i != ODYSSEY_SWORDS; i++) {
      s32 start = i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_FRAME;

      if (frame == i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_FRAME +
                       ODYSSEY_SWORD_PLUNGE_FRAMES / 2)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_HIT_FRAME) {
        for (k = 0; k != ODYSSEY_SWORD_SPARKS; k++) {
          struct Particle *spark;
          s32 speed = (Random16() & 255) + 128;
          s32 angle = Random16() & 0x3fff;

          if (args->side == EFFECT_SIDE_ENEMIES) {
            if (i != 0)
              angle += DEGtoANG(90);
          } else if (i == 0) {
            angle -= DEGtoANG(90);
          } else {
            angle += DEGtoANG(180);
          }
          spark = &state->particles[i * (ARRAY_COUNT(state->particles) /
                                         ODYSSEY_SWORDS) +
                                    k];
          spark->x = INTtoFIXED(to.x / 2);
          spark->y = INTtoFIXED(to.y);
          spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          spark->velocityY = -(Trig_Cos(angle) * speed) >> 5;
          spark->variant = (Random16() & 7) + 17;
        }
        Sound_PlayFar(SE_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 64);
        state->shakeFrames = 4;
      }
      // Each sword plunges 16 rows a frame, the first down to row 8 and the
      // second up to row 40, and stays there.
      if (frame >= start && frame <= 89) {
        s32 age = frame - start;
        s32 x;
        s32 y;

        if (age <= ODYSSEY_SWORD_PLUNGE_FRAMES) {
          if (args->side == EFFECT_SIDE_ENEMIES)
            x = to.x / 2 - age * 10 + 80;
          else
            x = to.x / 2 + age * 10 - 80;
          if (i == 0)
            y = age * 16 - 120;
          else
            y = 168 - age * 16;
        } else {
          // DEAD: both arms alike; the ROM keeps the test of effect->side.
          if (args->side == EFFECT_SIDE_ENEMIES)
            x = to.x / 2;
          else
            x = to.x / 2;
          y = i == 0 ? 8 : 40;
        }
        if (i == 1) {
          if (args->side == EFFECT_SIDE_ENEMIES)
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                                BLITTER_MAX);
          else
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                BLITTER_MAX);
          draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
        }
        draw[i](canvas, state->sheet, x - ODYSSEY_SWORD_WIDTH / 2, y,
                ODYSSEY_SWORD_WIDTH, ODYSSEY_SWORD_HEIGHT);
        if (i == 1)
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
    }

    projected = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    pixels = gMapGridCellBuffer;
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = pixels;
    model->mesh = gBattleEffect_LargeSquareMesh;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->texture = &texture;
    model->vertices = projected;
    for (i = 0; i != 2; i++) {
      s32 start;

      // Two rings of light flash out from the caster (frames 2 and 8).
      start = i * 6 + 2;
      if (frame == start)
        Sound_PlayFar(SE_POWER_UP);
      if (frame >= start && frame < i * 6 + 2 + ODYSSEY_RING_FRAMES) {
        s32 age = frame - start;
        s32 shade = ODYSSEY_RING_SHINE_FRAMES * 8 - age * 8;

        if (shade > 0)
          shade = 0;
        if (shade > MODEL_SHADE_INVISIBLE) {
          s32 scale = age * (FIXED_ONE / 4) + FIXED_ONE / 4;

          model->shade = shade;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(from.x / 2 - 64), 0, 0);
          Matrix_Scale(scale, scale * 2, scale);
          Matrix_RotateX(gOdyssey_FlashPitches[i]);
          Matrix_RotateY(gOdyssey_FlashYaws[i]);
          Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Rings of light close in on the caster, who flashes white.
      start = i * 8 + ODYSSEY_GATHER_FRAME;
      if (frame == i * 8 + 68)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == i * 8 + 72) {
        /* FAKEMATCH(temporary): none, 0 held in a local; as 0 at its uses. */
        s32 none = 0;

        BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP, 0,
                                 none);
      } else if (frame == i * 8 + 76)
        BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, 0,
                                 0);
      if (frame >= start && frame < start + ODYSSEY_GATHER_RING_FRAMES) {
        s32 scale = FIXED_ONE * 3 - (frame - start) * (FIXED_ONE / 4);

        model->shade = 0;
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(from.x / 2 - 64), 0, 0);
        Matrix_Scale(scale, scale * 2, scale);
        Matrix_RotateX(gOdyssey_FlashPitches[i]);
        Matrix_RotateY(gOdyssey_FlashYaws[i]);
        Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
        Render_DrawModelList(model);
      }

      // A ring of light at the target with each sword's hit.
      start = i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_HIT_FRAME;
      if (frame >= start && frame < i * ODYSSEY_SWORD_INTERVAL +
                                        ODYSSEY_SWORD_HIT_FRAME +
                                        ODYSSEY_RING_FRAMES) {
        s32 age = frame - start;
        s32 shade = ODYSSEY_RING_SHINE_FRAMES * 8 - age * 8;

        if (shade > 0)
          shade = 0;
        if (shade > MODEL_SHADE_INVISIBLE) {
          FIXED scale = age * (FIXED_ONE / 4) + FIXED_ONE / 4;

          model->shade = shade;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(to.x / 2 - 64), 0, 0);
          Matrix_Scale(scale, scale * 2, scale);
          Matrix_RotateX(gOdyssey_FlashPitches[i]);
          Matrix_RotateY(gOdyssey_FlashYaws[i]);
          Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }
    }

    // From frame 90: the shards, flat triangles in eight shades; from 100 the
    // screen fades to white.
    fillMode = CANVAS_COPY_AND_CLEAR;
    flatKind = MODEL_SOLID_ADDITIVE;
    if (frame > 89) {
      u8 color[1];
      FIXED center;
      s32 x;
      s32 y;

      model->mesh = gBattleEffect_TriangleMesh;
      model->kind = flatKind;
      if (frame > 99) {
        EFFECT_SET_TRANSFER(state, fillMode, CANVAS_FILL_WORD(2));
        Palette_BrightenBgEntries(2, 2, 2);
      }
      for (i = 0, step = ODYSSEY_SHARDS; i != ODYSSEY_SHARD_COUNT; i++) {
        color[0] = (i & 7) + ODYSSEY_SHARD_LEVEL;
        model->shade = (long)color;
        Matrix_Initialize();
        x = step->x;
        y = step->y;
        center = -toFIXED(64);
        Matrix_Translate(x + center, y + center, 0);
        Matrix_Scale(step->variant * 3, step->variant * 6, step->variant);
        Matrix_RotateX(step->z);
        Matrix_RotateY(step->z);
        Render_TransformVertices(gBattleEffect_TriangleCorners, projected, 3);
        Render_DrawModelList(model);
        Particle_AdvanceWithGravity3D(step, 62, 0);
        step++;
      }
    }
    Heap_Release(model);
    Heap_Release(projected);

    Camera_ApplyVerticalShake(8);
    EFFECT_END_FRAME(state);
  }

  // The battle's own scene again.
  BattleObject_PutFar();
  transition->busy = 0;
  gBgScroll[1].x = savedScrollX;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           BATTLE_BACKGROUND_ID(Ram_HeapSlots->battleState),
                           BACKGROUND_DARKEN_FRAMES);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// The swirl runs until A is pressed, at most this many frames.
#define SWIRL_MAX_FRAMES 312000

/// The swirl's colours are BG palettes 6 on, the battle background's.
#define SWIRL_FIRST_COLOR (6 * PALETTE_COLORS)

/// BG2 turns the swirl about this point of the screen, where the canvas's
/// centre stays.
#define SWIRL_CENTER_X (DISPLAY_WIDTH / 2)
#define SWIRL_CENTER_Y 72

/// BG2 shows the swirl at about twice its size: the steps of its matrix are
/// the 16.16 sine and cosine of its turn divided by this.
#define SWIRL_STEP_DIVISOR 532

/// Unused: reached only from BattleEffect_RunBackgroundFadeIn, which nothing
/// refers to. A turning cloud swirl on BG2: the cloud sprite sheet is drawn
/// once into the canvas in the battle background's colours and copied twice
/// across the map buffer; then BG2 turns about the screen while BG1 scrolls,
/// the palette flashes white at the start, steps toward BlueFlameSheet's
/// colours every fourth frame and from frame 120 dims. B starts it over and A
/// ends it.
void BattleEffect_RunCloudSwirl(struct BattleEffectParams *args) {
  BlitterPair sheetDraw;
  s32 savedScrollX;
  u8 *canvas;
  struct BattleEffectState *state;
  struct BattleBackgroundState *transition;
  DrawRectangle draw;
  const u8 *palette;
  s32 frame;
  s32 enabled;
  FIXED sine;
  FIXED cosine;
  s32 i;
  u8 *cells = Ram_MapCellBuffer;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  savedScrollX = gBgScroll[1].x;
  transition = Ram_HeapSlots->battleBackground;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(3));
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  REG_BG1CNT &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT |= BGCNT_PRIORITY(0);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // The cloud sprite sheet, moved to the battle background's palettes (which
  // take EmberStreakSheet's colours), drawn into the canvas.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  sheetDraw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  palette = Resource_GetTableEntry(RESOURCE_CLOUD_SWIRL_SHEET);
  Iwram_CopyWords((void *)&BG_PLTT_COLOR(6, 0), palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet);
  palette = Resource_GetTableEntry(RESOURCE_EMBER_STREAK_SHEET);
  Iwram_CopyWords((void *)&BG_PLTT_COLOR(6, 0), palette, SHEET_PALETTE_BYTES);
  for (i = 0; i != CANVAS_BYTES; i++)
    state->sheet[i] += SWIRL_FIRST_COLOR;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  sheetDraw[0](canvas, state->sheet, 0, 0, CANVAS_SIZE, CANVAS_SIZE);
  EFFECT_PRESENT_CANVAS(state);
  Task_Remove(BattleEffect_TransferCanvas);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);

  // The swirl itself in the canvas (colour 0 made 1, so none is clear),
  // copied twice side by side into the map buffer.
  Resource_LoadAndDecompress(RESOURCE_CLOUD_SWIRL_SHEET, canvas, 1, 1);
  for (i = 0; i != CANVAS_BYTES; i++) {
    if (canvas[i] == 0)
      canvas[i] = 1;
  }
  {
    // One value is both the clip flags, EFFECT_CLIP_BOUNDS, and the mode,
    // BLITTER_COPY.
    s32 clipAndCopy = BLITTER_COPY;

    enabled = 1;
    BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7, clipAndCopy,
                              clipAndCopy);
  }
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_TransferFullScreenImage, TASK_PASS_VBLANK);
  transition->busy = enabled;
  draw(cells, canvas, 0, 0, CANVAS_SIZE, CANVAS_SIZE);
  draw(cells, canvas, CANVAS_SIZE, 0, CANVAS_SIZE, CANVAS_SIZE);
  state->transferPending = enabled;
  wait(1);

  for (frame = 0; frame != SWIRL_MAX_FRAMES; frame++) {
    if (gInput.repeat & KEY_B)
      frame = 0;
    if (gInput.repeat & KEY_A)
      break;

    if (frame == 0)
      Iwram_FillWordsEntry((void *)BG_PLTT, SHEET_PALETTE_BYTES,
                           COLOR_PAIR(RGB_WHITE));
    if (frame > 119)
      Palette_BrightenBgEntries(-1, -1, -1);
    else if ((frame & 3) == 1)
      BattleEffect_StepPaletteToResource(RESOURCE_BLUE_FLAME_SHEET);
    gBgScroll[1].x = frame / 2;

    // BG2 turns a 1024th of a turn a frame. Its steps are 8.8, so the 16.16
    // centre is scaled by Q_8_8(1) for the products to come out 16.16.
    sine = Trig_Sin(frame * (TRIG_TURN / 1024)) / SWIRL_STEP_DIVISOR;
    cosine = Trig_Cos(frame * (TRIG_TURN / 1024)) / SWIRL_STEP_DIVISOR;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, cosine, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PB, sine, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PC, -sine, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PD, cosine, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH(
        (u32)&REG_BG2X,
        (Iwram_MulFX(toFIXED(-SWIRL_CENTER_X) * Q_8_8(1), cosine) +
         Iwram_MulFX(toFIXED(-SWIRL_CENTER_Y) * Q_8_8(1), sine) +
         toFIXED(CANVAS_SIZE / 2)) >>
            8,
        IO_WRITE_WORD);
    IO_WRITE_QUEUE_PUSH(
        (u32)&REG_BG2Y,
        (Iwram_MulFX(toFIXED(-SWIRL_CENTER_X) * Q_8_8(1), -sine) +
         Iwram_MulFX(toFIXED(-SWIRL_CENTER_Y) * Q_8_8(1), cosine) +
         toFIXED(CANVAS_SIZE / 2)) >>
            8,
        IO_WRITE_WORD);
    wait(1);
  }

  gBgScroll[1].x = savedScrollX;
  transition->busy = 0;
  Task_Remove(BattleEffect_TransferFullScreenImage);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}

/// The wipe's band slides this far along BG2, from just off one edge of the
/// screen until it has passed the other.
#define WIPE_DISTANCE 192
/// The band brightens over this many columns at its left and dims over as
/// many at its right.
#define WIPE_EDGE_WIDTH 32

/// Iris's screen wipe: a soft-edged white band, drawn once into the canvas and
/// stretched across BG2, slides over the screen from the right (side 0, a
/// positive speed) or from the left (side 1, a negative speed), speed of its
/// pixels a frame. Halfway through, while the band covers the screen, it calls
/// midpoint to change the scene behind it.
void Iris_WipeScreen(u8 side, u8 speed, void (*midpoint)(void)) {
  s32 scrollX;
  u8 *canvas;
  struct BattleEffectState *state;
  u16 ramp[CANVAS_LEVEL_BRIGHTEST];
  s32 half;
  s32 y;
  s32 i;

  scrollX = (side & 1) * WIPE_DISTANCE - 64;
  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  // CANVAS_COPY does not read the share.
  EFFECT_SET_TRANSFER(state, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
  REG_BG2PA = Q_8_8(0.28125); // about 3.6 times as wide

  // The band, column i: rising to the brightest level over the left edge,
  // the brightest through the middle and falling again over the right.
  for (i = 0; i != CANVAS_SIZE; i++) {
    for (y = 0; y != CANVAS_SIZE; y++) {
      s32 tileX = i & 7;
      s32 color;
      s32 columnOffset;

      if (i < WIPE_EDGE_WIDTH)
        color = i * 2;
      else {
        color = CANVAS_LEVEL_BRIGHTEST -
                ((i - (CANVAS_SIZE - WIPE_EDGE_WIDTH)) << 1);
        if (i < CANVAS_SIZE - WIPE_EDGE_WIDTH)
          color = CANVAS_LEVEL_BRIGHTEST;
      }
      columnOffset = tileX + (i >> 3) * CANVAS_TILE_BYTES;
      canvas[columnOffset + ((y & 7) << 3) + ((y >> 3) << 10)] = color;
    }
  }

  // Colours 1 to 63: a grey ramp from black to white.
  for (i = 0; i != CANVAS_LEVEL_BRIGHTEST; i++) {
    s32 level = i >> 1;

    ramp[i] = RGB(level, level, level);
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(0, 1), (u32)ramp,
                      DMA_COPY_HALFWORDS(CANVAS_LEVEL_BRIGHTEST));

  REG_BG2X = scrollX << 8;
  // y, done with the band, now counts the slide's frames.
  y = WIPE_DISTANCE / ((s8)speed < 0 ? -(s8)speed : (s8)speed) + 1;
  half = (y - 1) >> 1;
  for (i = 0; i != y; i++) {
    scrollX += (s8)speed;
    REG_BG2X = scrollX << 8;
    EFFECT_PRESENT_CANVAS(state);
    if (i == half && midpoint != NULL)
      midpoint();
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
}

/// Unused: sets the colour mode of each running animation layer of a sprite
/// and marks it for redrawing, as Actor_SetSpriteColor does for an actor's.
UNUSED void Sprite_SetColorMode(struct Sprite *sprite, s32 mode) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct SpriteAnimation *part = sprite->entries[i];

    if (part != NULL && part->script != NULL)
      part->colorMode = mode;
  }
  sprite->dirty = 1;
}

/// Unused: nothing calls it. Fills count bytes of table with a rising shade
/// ramp (0 to CANVAS_LEVEL_BRIGHTEST, by a step scaled by scale), then repeats
/// it seven more times.
UNUSED void BattleEffect_BuildShadeRamp(u8 *table, u32 count, s32 scale) {
  u32 step = count * 2;
  s32 total = 0;
  u32 i;
  s32 j;

  if (step > CANVAS_LEVEL_BRIGHTEST)
    step = CANVAS_LEVEL_BRIGHTEST;
  step = CANVAS_LEVEL_BRIGHTEST / step;
  if (step < 6)
    step = 6;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = FIXEDtoINT(scale * total);
    if (value > CANVAS_LEVEL_BRIGHTEST)
      value = CANVAS_LEVEL_BRIGHTEST;
    table[i] = value;
  }
  for (j = 1; j != 8; j++) {
    for (i = 0; i != count; i++)
      table[j * count + i] = table[i];
  }
}

/// Unused: nothing calls it. Fills table with a shade ramp that rises over
/// count bytes and falls back, repeated repeats times.
UNUSED void BattleEffect_BuildShadeRampWave(u8 *table, u32 count, u32 repeats,
                                            s32 scale) {
  s32 total = 0;
  u32 step = count * 2;
  u32 i;
  u32 j;

  if (step > CANVAS_LEVEL_BRIGHTEST)
    step = CANVAS_LEVEL_BRIGHTEST;
  step = CANVAS_LEVEL_BRIGHTEST / step;
  if (step < 6)
    step = 6;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = FIXEDtoINT(scale * total);
    if (value > CANVAS_LEVEL_BRIGHTEST)
      value = CANVAS_LEVEL_BRIGHTEST;
    table[i] = value;
  }
  for (i = 0; i != count; i++)
    table[count + i] = table[count - 1 - i];
  for (j = 1; j != repeats; j++) {
    for (i = 0; i != count * 2; i++)
      table[j * count * 2 + i] = table[i];
  }
}

/// As BattleEffect_BuildShadeRampWave, at twice the step.
void BattleEffect_BuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats,
                                          s32 scale) {
  s32 total = 0;
  u32 step = count * 2;
  u32 i;
  u32 j;

  if (step > CANVAS_LEVEL_BRIGHTEST)
    step = CANVAS_LEVEL_BRIGHTEST;
  step = CANVAS_LEVEL_BRIGHTEST / step;
  if (step < 6)
    step = 6;
  step *= 2;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = FIXEDtoINT(scale * total);
    if (value > CANVAS_LEVEL_BRIGHTEST)
      value = CANVAS_LEVEL_BRIGHTEST;
    table[i] = value;
  }
  for (i = 0; i != count; i++)
    table[count + i] = table[count - 1 - i];
  for (j = 1; j != repeats; j++) {
    for (i = 0; i != count * 2; i++)
      table[j * count * 2 + i] = table[i];
  }
}

/// Iris's teardown: sets each target back on the ground at the battle origin
/// with the usual bounce, frees the battle background, centres the projection
/// again, reloads the sprites' basic colours and brings back the battle's own
/// backdrop.
void Iris_RestoreBattle(void) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  struct BattleBackgroundState *transition = Ram_HeapSlots->battleBackground;
  struct BattleState *battle = Ram_HeapSlots->battleState;
  s32 i;

  for (i = 0; i != state->args->targetCount; i++) {
    struct MotionObject *object =
        BattleObject_GetFar(state->args->targets[i])->object;

    SET_POSITION(object, 0, toFIXED(120), 0);
    object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
    BattleMotion_ApplyKnockbackFar(state->args->targets[i], KNOCKBACK_HOP);
  }
  transition->busy = 0;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Palette_LoadBasicSpriteColors();
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background,
                           BACKGROUND_DARKEN_FRAMES);
}
#undef ODYSSEY_FRAMES
#undef ODYSSEY_SWORD_WIDTH
#undef ODYSSEY_SWORD_HEIGHT
#undef ODYSSEY_COLOSSAL_SWORD
#undef ODYSSEY_COLOSSAL_SWORD_WIDTH
#undef ODYSSEY_COLOSSAL_SWORD_HEIGHT
#undef ODYSSEY_DUST_BLOB
#undef ODYSSEY_FLAME_GRADIENT
#undef ODYSSEY_SHARDS
#undef ODYSSEY_SHARD_COUNT
#undef ODYSSEY_SHARD_LEVEL
#undef ODYSSEY_SWORDS
#undef ODYSSEY_SWORD_FRAME
#undef ODYSSEY_SWORD_INTERVAL
#undef ODYSSEY_SWORD_PLUNGE_FRAMES
#undef ODYSSEY_SWORD_HIT_FRAME
#undef ODYSSEY_SWORD_SPARKS
#undef ODYSSEY_RING_FRAMES
#undef ODYSSEY_RING_SHINE_FRAMES
#undef ODYSSEY_GATHER_FRAME
#undef ODYSSEY_GATHER_RING_FRAMES
#undef SWIRL_MAX_FRAMES
#undef SWIRL_FIRST_COLOR
#undef SWIRL_CENTER_X
#undef SWIRL_CENTER_Y
#undef SWIRL_STEP_DIVISOR
#undef WIPE_DISTANCE
#undef WIPE_EDGE_WIDTH
