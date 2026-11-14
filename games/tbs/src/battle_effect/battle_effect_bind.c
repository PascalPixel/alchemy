/// @file
/// Bind and Triangle Strike.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// Bind and Luff spin a shrinking triangle onto the first target and stand a
// rune there. The triangle's corners are projected onto the canvas and each
// edge is stamped with flares.

/// Turns the screen into shades of blue, top down, with a ragged edge; nothing
/// calls it.
UNUSED void BattleEffect_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *pixel;
  u16 *palette;
  palette = (u16 *)&BG_PLTT_COLOR(2, 0);
  for (i = 0; i != 32; i++) {
    s32 half;
    half = i / 2;
    *palette++ = RGB(half, half, i);
  }
  delays = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  end = 0;
  speed = 16;
  Iwram_CopyWords(BLUE_RAMP_PIXELS, (void *)BG_CHAR_BLOCK(2),
                  BATTLE_BACKGROUND_BYTES);
  for (x = 0; x != 256; x++) {
    delays[x] = Random16() & 63;
  }
  row = 0;
  for (i = 0; i != 27; i++) {
    end += speed / 4;
    speed++;
    while (row != end) {
      for (x = 0; x != 256; x++) {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = BLUE_RAMP_PIXELS + ((x & 7) + ((x / 8) << 6) +
                                        ((y & 7) << 3) + ((y / 8) << 11));
            color = ((s16 *)BG_PLTT)[*pixel];
            red = color & 31;
            green = (color >> 5) & 31;
            blue = (color >> 10) & 31;
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = 63 - red;
          }
        }
      }
      row++;
    }
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS,
                    BATTLE_BACKGROUND_BYTES);
    wait(1);
    if (end > 248)
      break;
  }
  // Then onto a second blue ramp.
  palette = (u16 *)&BG_PLTT_COLOR(6, 0);
  for (i = 0; i != 32; i++) {
    s32 half;
    half = i / 2;
    *palette++ = RGB(half, half, i);
  }
  for (i = 0; i != BATTLE_BACKGROUND_BYTES; i++) {
    BLUE_RAMP_PIXELS[i] += 64;
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS,
                  BATTLE_BACKGROUND_BYTES);
  wait(1);
}

/// A triangle of light spins and shrinks onto the first target, changing colour
/// and leaving a fading trail, then a rune stands on the target.
///
/// Abilities: Bind; Luff.
void BattleEffect_RunBind(struct BattleEffectParams *args) {
  struct ScreenPoint anchor;
  // The target's position, then each corner projected around it.
  struct ScreenPoint corner;
  struct ScreenPoint screen;
  struct Vector3 scale;
  s32 originX;
  s32 originY;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  u8 *palette;
  struct Actor *actor;
  s32 memberFrame;
  struct Particle *point;
  s32 size;
  s32 j;
  s32 k;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (work->args->prelude == 1)
    BattleEffect_RunDjinniEntrance(args, ELEMENT_JUPITER, work->args->side,
                                   DJINNI_ANCHOR_OVER_TARGET, &originX,
                                   &originY);
  REG_BG2PA = Q_8_8(1);

  palette = Resource_GetTableEntry(RESOURCE_RUNE_SHEET);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      graphics);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_D),
                      work->sheet + 4096);
  palette = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet + 8192);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  Projection_ScaledUnit(work->args->targets[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * 20 + 72; frame++) {
    if (frame == 64)
      EFFECT_SHOW_DAMAGE(0);
    // The colours cycle through the hues.
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - TRIG_TURN / 3,
                                    TRIG_TURN / 3, 0);

    // The Djinni sways, then rises away.
    if (work->args->prelude == 1) {
      s32 x;
      s32 y;

      x = ((Trig_Sin(frame << 11) * 20) >> 16) + originX + shift - 20;
      y = (((Trig_Cos(frame << 11) * 4)) >> 16) + originY - 24;
      BattleEffect_FetchRectangleBlitters(work->args->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + 8192, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + 8192, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += CANVAS_FILL_WORD(1);

    for (member = 0; member != 1; member++) {
      memberFrame = frame - member * 8;
      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      if (memberFrame >= 0 && memberFrame < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        SET_VECTOR(ScreenPoint_Coordinates(&corner), actor->x.fixed,
                   actor->y.fixed, actor->z.fixed);
        Projection_Point(&corner, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;
        // The triangle.
        if (memberFrame <= 67) {
          for (j = 0; j != 3; j++) {
            point = &work->particles[member * 32 + j];
            Transform_Reset();
            if (memberFrame <= 63) {
              scale.x = Q16_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale.y = Q16_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale.z = Q16_ONE * 21 / 8 - (memberFrame * 3 << 9);
              Transform_ApplyScale(Vector3_Coordinates(&scale));
              Transform_ApplyRoll((64 - memberFrame) << 9);
              Transform_ApplyYaw((64 - memberFrame) << 9);
            }
            Transform_ApplyRoll(j * (TRIG_TURN / 3));
            Projection_Point(gBind_Vertex, &corner);
            point->velocityX = corner.x + screen.x;
            point->velocityY = corner.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct Particle *from = &work->particles[j + member * 32];
            struct Particle *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != 24; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              EFFECT_DRAW_FLARE(draw[0], canvas, work->sheet + 4096, x, y,
                                size);
            }
          }
        }
        // Then the rune, in two mirrored halves.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. Its frame loop is BattleEffect_RunBind's,
/// without the Djinni prelude, and on frame 46 it starts the user's approach to
/// the first target.
void BattleEffect_RunUnusedTriangleStrike(struct BattleEffectParams *args) {
  struct ScreenPoint anchor;
  // The target's position, then each corner projected around it.
  struct ScreenPoint corner;
  struct ScreenPoint screen;
  struct Vector3 scale;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  u8 *palette;
  struct Actor *actor;
  s32 memberFrame;
  struct Particle *point;
  s32 size;
  s32 j;
  s32 k;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);

  palette = Resource_GetTableEntry(RESOURCE_RUNE_SHEET);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      graphics);
  palette = Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_D);
  Decompress_TaggedLz(palette, work->sheet + 4096);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  Projection_ScaledUnit(work->args->targets[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * 20 + 72; frame++) {
    if (frame == 64)
      EFFECT_SHOW_DAMAGE(0);
    // The user sets off towards the target.
    if (frame == 46)
      BattleMotion_ApproachTargetFar(work->args->user, work->args->targets[0],
                                     16, 0);
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - TRIG_TURN / 3,
                                    TRIG_TURN / 3, 0);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += CANVAS_FILL_WORD(1);

    for (member = 0; member != 1; member++) {
      memberFrame = frame - member * 8;
      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      if (memberFrame >= 0 && memberFrame < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        SET_VECTOR(ScreenPoint_Coordinates(&corner), actor->x.fixed,
                   actor->y.fixed, actor->z.fixed);
        Projection_Point(&corner, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;
        if (memberFrame <= 67) {
          for (j = 0; j != 3; j++) {
            point = &work->particles[member * 32 + j];
            Transform_Reset();
            if (memberFrame <= 63) {
              scale.x = Q16_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale.y = Q16_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale.z = Q16_ONE * 21 / 8 - (memberFrame * 3 << 9);
              Transform_ApplyScale(Vector3_Coordinates(&scale));
              Transform_ApplyRoll((64 - memberFrame) << 9);
              Transform_ApplyYaw((64 - memberFrame) << 9);
            }
            Transform_ApplyRoll(j * (TRIG_TURN / 3));
            Projection_Point(gBind_TriangleStrikeVertex, &corner);
            point->velocityX = corner.x + screen.x;
            point->velocityY = corner.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct Particle *from = &work->particles[j + member * 32];
            struct Particle *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != 24; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              EFFECT_DRAW_FLARE(draw[0], canvas, work->sheet + 4096, x, y,
                                size);
            }
          }
        }
        // Then the rune, in two mirrored halves.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
