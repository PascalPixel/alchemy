/// @file
/// Odyssey, the turning cloud swirl, and the screen wipe and teardown of summon
/// 25.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define KIND125_FRAMES 114

// A texture for the ARM model renderer: its size as powers of two and the
// address of its pixels.

// A model for the ARM renderer (Render_DrawModelList). A textured model (kind
// 7) takes a shade; a flat one the address of its colour.
/* VIEW(f190b74): tag struct RenderModel as struct RenderModel__f190b74;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect_odyssey.c:364: structure has no
 * member named `look'. */
struct RenderModel__f190b74 {
  s32 kind;
  s32 flags;
  const u16 *mesh;
  void *vertices;
  struct ModelTexture *texture;
  union {
    s32 shade;
    u8 *color;
  } look;
};

/// Where DustBlobSheet is decoded in the effect's pictures.
#define ODYSSEY_DUST_BLOB_PICTURES 0x2f10

/// Where FlameGradientTexture is decoded in the map cell buffer.
#define ODYSSEY_FLAME_GRADIENT 0x4000

/// Where GiantSwordBladeSheet is decoded in the effect's pictures.
#define ODYSSEY_SWORD_BLADE 0xc80

/// 114 frames. The screen turns to a scrolling red haze
/// with only the caster and the target on it (frame 2); two textured flashes
/// rise from the caster, two claws strike the target from above and below
/// (frames 24-89, sparks at each hit); at 84 the caster charges across, at 86
/// a great slash crosses the screen and at 90 the target is struck and 128
/// flat shards scatter, tumbling; from frame 100 everything fades to white,
/// and at the end the battle backdrop returns.
///
/// Abilities: Odyssey.
void BattleEffect_RunOdyssey(struct BattleEffectParams *args) {
  struct ScreenPoint to;
  struct ScreenPoint from;
  s16 list[14];
  DrawRectangle draw[2];
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  struct MotionObject *caster;
  struct BattlePresentationTransition *transition;
  s32 savedScrollX;
  struct RenderModel__f190b74 *model;
  void *projected;
  struct Particle *step;
  s32 frame;
  s32 i;
  s32 k;
  s32 fillMode;
  s32 flatKind;
  u8 *pixels;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GiantSwordSheet, work->sheet, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet,
                             work->sheet + ODYSSEY_DUST_BLOB_PICTURES, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
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

  for (frame = 0; frame != KIND125_FRAMES; frame++) {
    EffectPosition_ProjectScaledBattler(args->user, &from);
    EffectPosition_ProjectScaledBattler(args->targets[0], &to);

    if (frame == 0) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gMapGridCellBuffer,
                                 0, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0E9,
                                 Ram_MapCellBuffer + ODYSSEY_FLAME_GRADIENT, 0,
                                 0);
      Graphics_LoadGreyRamp();
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      EFFECT_FREE_PARTICLES(i, work->particles, 64);
    }

    // Frame 2: only the caster and the target over the red haze, whose
    // colours (entries 96 on) lose their green and take red into blue.
    if (frame == 2) {
      u16 *palette;

      list[0] = args->user;
      list[1] = args->targets[0];
      list[2] = 0xff;
      BattleActor_SpawnObjectsForListFar(list, 0);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               (s32)&ResourceId_RedHazeBackdrop, 0);
      BattleEffect_BuildRepeatingTilemap();
      palette = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != 128; i++) {
        s32 entry = *palette;
        s32 red = RGB_LEVEL_MAX;
        u16 green;
        s32 blue;

        red &= entry;
        /* FAKEMATCH(dead): green is taken and clamped, then never used. */
        green = RGB_GREEN((u16)entry);
        blue = RGB_BLUE((u16)entry);

        if (red > RGB_LEVEL_MAX)
          red = RGB_LEVEL_MAX;
        if (blue > RGB_LEVEL_MAX)
          blue = RGB_LEVEL_MAX;
        green = green > RGB_LEVEL_MAX ? RGB_LEVEL_MAX : green;
        *palette++ = red << 10 | ((u16)red >> 1) << 5 | blue;
      }
      transition->active = 1;
    }
    if (frame > 1)
      gBgScroll[1].x += 8;

    if (frame == 8) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      step = work->particles;
      for (i = 0; i != 64; i++) {
        step->x = Random16() % 216 - 96;
        step->y = Random16() % 216;
        step++;
      }
    }
    if (frame == 86)
      Resource_LoadAndDecompress((s32)&ResourceId_GiantSwordBladeSheet,
                                 work->sheet + ODYSSEY_SWORD_BLADE, 1, 1);

    // Frame 84: the caster charges at the target.
    if (frame == 84) {
      Sound_PlayFar(SE_POWER_UP);
      Actor_SelectAnimationFar(caster, 2);
      Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL * 3);
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 7, 80);
    }

    // Frame 90: the hit, and 128 shards scatter away from the caster.
    if (frame == 90) {
      struct Particle *shard;

      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      Sound_PlayFar(SE_CLATTER);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      work->shakeFrames = 8;
      shard = Ram_MapCellBuffer + 0x6000;
      for (i = 0; i != 128; i++) {
        s32 speed = (Random16() & 0x1ff) + 32;
        s32 angle = Random16() & 0x7fff;

        if (args->side == EFFECT_SIDE_PARTY)
          angle += TRIG_TURN / 2;
        shard->x = px(to.x / 2);
        shard->y = px(to.y);
        shard->velocityX = (Trig_Sin(angle) * speed) >> 6;
        shard->velocityY = -(Trig_Cos(angle) * speed) >> 5;
        shard->z = Random16();
        shard->velocityZ = (Random16() & 0x3ff) + 0x400;
        shard->variant = (Random16() & 0x3fff) + 0x2000;
        shard++;
      }
    }

    // From frame 86: the slash crosses the screen in two mirrored halves.
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
      draw[0](canvas, work->sheet + ODYSSEY_SWORD_BLADE, x, 8, 158, 56);
      if (args->side == EFFECT_SIDE_ENEMIES)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
      draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
      draw[1](canvas, work->sheet + ODYSSEY_SWORD_BLADE, x, 64, 158, 56);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }

    // Until frame 89: the sparks of the claws' hits, shrinking.
    if (frame <= 89) {
      for (i = 0, step = work->particles; i != 64; i++) {
        if (step->variant >= 0) {
          s32 size = (step->variant >> 2) + 1;

          draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                  Q_16_16_UPPER_HALF(step->x) - size / 2,
                  Q_16_16_UPPER_HALF(step->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(step, 60, 0);
          step->variant--;
        }
        step++;
      }
    }

    // Two claws, from frames 24 and 40: one sweeps down onto the target,
    // the other (flipped) up from below; sixteen sparks at each hit.
    for (i = 0; i != 2; i++) {
      s32 start = i * 16 + 24;

      if (frame == i * 16 + 28)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == i * 16 + 32) {
        for (k = 0; k != 16; k++) {
          struct Particle *spark;
          s32 speed = (Random16() & 255) + 128;
          s32 angle = Random16() & 0x3fff;

          if (args->side == EFFECT_SIDE_ENEMIES) {
            if (i != 0)
              angle += TRIG_TURN / 4;
          } else if (i == 0) {
            angle -= TRIG_TURN / 4;
          } else {
            angle += TRIG_TURN / 2;
          }
          spark = &work->particles[i * 32 + k];
          spark->x = px(to.x / 2);
          spark->y = px(to.y);
          spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          spark->velocityY = -(Trig_Cos(angle) * speed) >> 5;
          spark->variant = (Random16() & 7) + 17;
        }
        Sound_PlayFar(SE_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 64);
        work->shakeFrames = 4;
      }
      if (frame >= start && frame <= 89) {
        s32 age = frame - start;
        s32 x;
        s32 y;

        if (age <= 8) {
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
        draw[i](canvas, work->sheet, x - 20, y, 40, 80);
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

      // Two flashes rise from the caster (frames 2 and 8).
      start = i * 6 + 2;
      if (frame == start)
        Sound_PlayFar(SE_POWER_UP);
      if (frame >= start && frame < i * 6 + 18) {
        s32 age = frame - start;
        s32 shade = 64 - age * 8;

        if (shade > 0)
          shade = 0;
        if (shade > MODEL_SHADE_INVISIBLE) {
          s32 scale = (age << 14) + Q16_ONE / 4;

          model->look.shade = shade;
          Transform_Reset();
          Transform_ApplyTranslation(px(from.x / 2 - 64), 0, 0);
          Transform_ApplyAxisScale(scale, scale * 2, scale);
          Transform_ApplyPitch(gOdyssey_FlashPitches[i]);
          Transform_ApplyYaw(gOdyssey_FlashYaws[i]);
          Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Frames 64-82: the caster gathers itself, flashing.
      start = i * 8 + 64;
      if (frame == i * 8 + 68)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == i * 8 + 72) {
        s32 none = 0;

        BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP, 0,
                                 none);
      } else if (frame == i * 8 + 76)
        BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, 0,
                                 0);
      if (frame >= start && frame < start + 11) {
        s32 scale = Q16_ONE * 3 - ((frame - start) << 14);

        model->look.shade = 0;
        Transform_Reset();
        Transform_ApplyTranslation(px(from.x / 2 - 64), 0, 0);
        Transform_ApplyAxisScale(scale, scale * 2, scale);
        Transform_ApplyPitch(gOdyssey_FlashPitches[i]);
        Transform_ApplyYaw(gOdyssey_FlashYaws[i]);
        Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
        Render_DrawModelList(model);
      }

      // A flash at the target with each claw's hit.
      start = i * 16 + 32;
      if (frame >= start && frame < i * 16 + 48) {
        s32 age = frame - start;
        s32 shade = 64 - age * 8;

        if (shade > 0)
          shade = 0;
        if (shade > MODEL_SHADE_INVISIBLE) {
          s32 scale = (age << 14) + Q16_ONE / 4;

          model->look.shade = shade;
          Transform_Reset();
          Transform_ApplyTranslation(px(to.x / 2 - 64), 0, 0);
          Transform_ApplyAxisScale(scale, scale * 2, scale);
          Transform_ApplyPitch(gOdyssey_FlashPitches[i]);
          Transform_ApplyYaw(gOdyssey_FlashYaws[i]);
          Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }
    }

    // From frame 90: the shards, flat triangles in eight colours; from 100
    // the screen fades to white.
    fillMode = 1;
    flatKind = 3;
    if (frame > 89) {
      u8 color[1];
      s32 center;
      s32 x;
      s32 y;

      model->mesh = gBattleEffect_TriangleMesh;
      model->kind = flatKind;
      if (frame > 99) {
        EFFECT_SET_TRANSFER(work, fillMode, CANVAS_FILL_WORD(2));
        Palette_BrightenBgEntries(2, 2, 2);
      }
      for (i = 0, step = Ram_MapCellBuffer + 0x6000; i != 128; i++) {
        color[0] = (i & 7) + 32;
        model->look.color = color;
        Transform_Reset();
        x = step->x;
        y = step->y;
        center = -px(64);
        Transform_ApplyTranslation(x + center, y + center, 0);
        Transform_ApplyAxisScale(step->variant * 3, step->variant * 6,
                                 step->variant);
        Transform_ApplyPitch(step->z);
        Transform_ApplyYaw(step->z);
        Render_TransformVertices(gBattleEffect_TriangleCorners, projected, 3);
        Render_DrawModelList(model);
        BattleEffectKernels_IntegrateVector3(step, 62, 0);
        step++;
      }
    }
    Heap_Release(model);
    Heap_Release(projected);

    Camera_ApplyVerticalShake(8);
    EFFECT_END_FRAME(work);
  }

  // The battle's own scene again.
  BattleObject_PutFar();
  transition->active = 0;
  gBgScroll[1].x = savedScrollX;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           BATTLE_BACKGROUND_ID(Ram_HeapSlots->battleWork), 24);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// The swirl runs until A is pressed, at most this many frames.
#define SWIRL_MAX_FRAMES 312000

/// Copies the swirl's colours into background palette entries 96 on.
static inline s32 Swirl_CopyPalette(const void *source) {
  return Iwram_CopyWordsEntry((void *)&BG_PLTT_COLOR(6, 0), source, 128);
}

/// Draws a size by size square of the swirl at x along the top of the buffer.
static inline s32 Swirl_Draw(DrawRectangle draw, void *buffer,
                             const void *canvas, s32 x, s32 size) {
  return draw(buffer, canvas, x, 0, size, size);
}

/// A turning cloud swirl on BG2, shown from BattleEffect_RunBackgroundFadeIn.
/// The cloud sheet is drawn once into the canvas (its colours moved to palette
/// entries 96 on) and copied twice across the map buffer; then BG2 turns about
/// the screen's centre while BG1 scrolls, the palette flashes white at the
/// start, steps toward the swirl's own colours every fourth frame and from
/// frame 120 dims. B starts it over and A ends it.
void BattleEffect_RunCloudSwirl(struct BattleEffectParams *args) {
  BlitterPair sheetDraw;
  s32 savedScrollX;
  u8 *canvas;
  struct BattleEffectWork *work;
  struct BattlePresentationTransition *transition;
  DrawRectangle draw;
  const u8 *palette;
  s32 frame;
  s32 enabled;
  s32 sine;
  s32 cosine;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  savedScrollX = gBgScroll[1].x;
  transition = Ram_HeapSlots->battleBackground;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(3));
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // The cloud sheet, recoloured to palette entries 96 on, drawn into the
  // canvas through the copy blitter.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  sheetDraw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  palette = Resource_GetTableEntry((s32)&ResourceId_CloudSwirlSheet);
  /* FAKEMATCH(wrapper): the copy through Swirl_CopyPalette. */
  Swirl_CopyPalette(palette);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
  Iwram_CopyWordsEntry((void *)&BG_PLTT_COLOR(6, 0), palette, 128);
  for (i = 0; i != 0x4000; i++)
    work->sheet[i] += 96;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  sheetDraw[0](canvas, work->sheet, 0, 0, 128, 128);
  EFFECT_PRESENT_CANVAS(work);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);

  // The swirl itself in the canvas (colour 0 made 1, so none is clear),
  // copied twice side by side into the map buffer.
  Resource_LoadAndDecompress((s32)&ResourceId_CloudSwirlSheet, canvas, 1, 1);
  for (i = 0; i != 0x4000; i++) {
    if (canvas[i] == 0)
      canvas[i] = 1;
  }
  {
    s32 mode = 3;

    enabled = 1;
    BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7, mode, mode);
  }
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, TASK_PASS_FRAME);
  transition->active = enabled;
  /* FAKEMATCH(wrapper): the blit through Swirl_Draw with the constant buffer.
   */
  Swirl_Draw(draw, Ram_MapCellBuffer, canvas, 0, 128);
  draw(Ram_MapCellBuffer, canvas, 128, 0, 128, 128);
  work->transferPending = enabled;
  wait(1);

  for (frame = 0; frame != SWIRL_MAX_FRAMES; frame++) {
    if (gInput.repeat & KEY_B)
      frame = 0;
    if (gInput.repeat & KEY_A)
      break;

    if (frame == 0)
      Iwram_FillWordsEntry((void *)BG_PLTT, 128, 0x7fff7fff);
    if (frame > 119)
      Palette_BrightenBgEntries(-1, -1, -1);
    else if ((frame & 3) == 1)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_BlueFlameSheet);
    gBgScroll[1].x = frame / 2;

    // BG2 turns a 1024th of a circle a frame, at 1/532 of the sine's scale
    // (about half size), about the screen's centre (120, 72).
    sine = Trig_Sin(frame << 6) / 532;
    cosine = Trig_Cos(frame << 6) / 532;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, cosine, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PB, sine, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PC, -sine, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PD, cosine, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X,
                        (Iwram_MulQ16(-120 << 24, cosine) +
                         Iwram_MulQ16(-72 << 24, sine) + 0x400000) >>
                            8,
                        IO_WRITE_WORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2Y,
                        (Iwram_MulQ16(-120 << 24, -sine) +
                         Iwram_MulQ16(-72 << 24, cosine) + 0x400000) >>
                            8,
                        IO_WRITE_WORD);
    wait(1);
  }

  gBgScroll[1].x = savedScrollX;
  transition->active = 0;
  Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}

/// Iris's screen wipe: a soft-edged grey band, drawn once into the
/// canvas, slides across the screen on BG2 at speed pixels per frame, from
/// the left or (when the low bit of side is set) from the right. Halfway
/// through, while the band covers the screen, it calls midpoint to change
/// the scene behind it.
void Iris_WipeScreen(u8 side, u8 speed, void (*midpoint)(void)) {
  s32 scrollX;
  u8 *canvas;
  struct BattleEffectWork *work;
  u16 ramp[63];
  s32 half;
  s32 x;
  s32 i;

  scrollX = (side & 1) * 192 - 64;
  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  /* FAKEMATCH(dead): a never-taken five-argument call reserves the stack
   * frame's outgoing argument word at sp+0. */
  if (0)
    BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE, 7, 7, 3, 3);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY, 75);
  REG_BG2PA = 0x48;

  // The band: colours 0 to 62 rising over the first 32 rows, 63 through the
  // middle 64 and falling again over the last 32, in 8x8 256-colour tiles.
  for (i = 0; i != 128; i++) {
    for (x = 0; x != 128; x++) {
      s32 line = i & 7;
      s32 color;
      s32 row;

      if (i <= 31)
        color = i * 2;
      else {
        color = 63 - ((i - 96) << 1);
        if (i <= 95)
          color = 63;
      }
      row = line + (i >> 3) * 64;
      canvas[row + ((x & 7) << 3) + ((x >> 3) << 10)] = color;
    }
  }

  // Colours 1 to 63: a grey ramp from black to half white.
  for (i = 0; i != 63; i++) {
    s32 level = i >> 1;

    ramp[i] = RGB(level, level, level);
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(0, 1), (u32)ramp,
                      DMA_COPY_HALFWORDS(63));

  REG_BG2X = scrollX << 8;
  // x, done with the band, now counts the slide's frames.
  x = 192 / ((s8)speed < 0 ? -(s8)speed : (s8)speed) + 1;
  half = (x - 1) >> 1;
  for (i = 0; i != x; i++) {
    scrollX += (s8)speed;
    REG_BG2X = scrollX << 8;
    EFFECT_PRESENT_CANVAS(work);
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

/// Fills count bytes of table with a rising shade ramp (0 to 63, by a step
/// scaled by scale), then repeats it seven more times.
void BattleEffect_BuildShadeRamp(u8 *table, u32 count, s32 scale) {
  u32 step = count * 2;
  s32 total = 0;
  u32 i;
  s32 j;

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = Q_16_16_TO_INT(scale * total);
    if (value > 63)
      value = 63;
    table[i] = value;
  }
  for (j = 1; j != 8; j++) {
    for (i = 0; i != count; i++)
      table[j * count + i] = table[i];
  }
}

/// Fills table with a shade ramp that rises over count bytes and falls back,
/// repeated repeats times.
void BattleEffect_BuildShadeRampWave(u8 *table, u32 count, u32 repeats,
                                     s32 scale) {
  s32 total = 0;
  u32 step = count * 2;
  u32 i;
  u32 j;

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = Q_16_16_TO_INT(scale * total);
    if (value > 63)
      value = 63;
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

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  step *= 2;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = Q_16_16_TO_INT(scale * total);
    if (value > 63)
      value = 63;
    table[i] = value;
  }
  for (i = 0; i != count; i++)
    table[count + i] = table[count - 1 - i];
  for (j = 1; j != repeats; j++) {
    for (i = 0; i != count * 2; i++)
      table[j * count * 2 + i] = table[i];
  }
}

/// Iris's teardown: sets each target back on the ground at the battle
/// origin with the usual bounce
/// and restarts its idle motion, ends the presentation transition, centres the
/// camera at (120, 120), reloads the battle palette and redraws the battle's
/// own backdrop over the summon's.
void Iris_RestoreBattle(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->battleBackground;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  for (i = 0; i != work->args->targetCount; i++) {
    struct MotionObject *object =
        BattleObject_GetFar(work->args->targets[i])->object;

    SET_POSITION(object, 0, px(120), 0);
    object->verticalMotionStrength = 0xab85;
    BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_HOP);
  }
  transition->active = 0;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Palette_LoadBasicSpriteColors();
  // The battle's background.
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
}
#undef KIND125_FRAMES
#undef SWIRL_MAX_FRAMES
