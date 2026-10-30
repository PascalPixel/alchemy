/// @file
/// Bind and Triangle Strike.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// The triangle effects (Bind, Luff and BattleEffect_RunUnusedTriangleStrike)
// spin a shrinking triangle onto the first target for 64 frames and then stand
// a rune there. The triangle is drawn as the ring and star effects draw
// their outlines: its corners are projected onto the canvas and stored in the
// velocity fields of Particles, then each edge is sampled at evenly
// spaced points and a flare is stamped at each. Both runners write the same
// frame loop; only the prelude, the step towards the target, the canvas's
// priority and the vertex table differ. The blue-ramp helper here, which
// nothing calls, converts a full-screen tiled picture in place.

/// Turns the screen into shades of blue, top down, with a ragged edge. Works
/// on a copy of the screen in Ram_MapGridCellBuffer. Each column starts up to
/// 63 lines late, and the edge speeds up a little every frame; after 27 frames
/// the picture moves onto a second blue ramp at colours 96 to 127. Takes 256
/// random numbers, one a column.
///
/// Nothing calls it or takes its address. Writes background colours 32 to 63
/// and then 96 to 127, the BATTLE_BACKGROUND_BYTES of BG character block 2 (the
/// battle background's picture) and Ram_MapGridCellBuffer, keeps its delays in
/// the sheet slot, and waits a frame for each step of the edge.
UNUSED void BattleEffect_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;
  // A 32-colour blue ramp at colours 32 to 63, red and green at half the blue.
  palette = (u16 *)&BG_PLTT_COLOR(2, 0);
  i = 0;
  do {
    s32 half;
    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  delays = *(u8 **)&gHeapSlots[HEAP_SLOT_BATTLE_SHEET];
  // The edge moves speed / 4 lines a frame, speed rising by one a frame
  // from 16.
  end = 0;
  speed = 16;
  Iwram_CopyWords(BLUE_RAMP_PIXELS, (void *)BG_CHAR_BLOCK(2),
                  BATTLE_BACKGROUND_BYTES);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + 256);
  row = 0;
  i = 0;
  do {
    end += speed / 4;
    speed++;
    while (row != end) {
      x = 0;
      do {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = BLUE_RAMP_PIXELS + ((x & 7) + ((x / 8) << 6) +
                                        ((y & 7) << 3) + ((y / 8) << 11));
            // Column x reaches line row - delays[x]; the pixel there becomes
            // the brightest of its colour's channels, inverted, as an index
            // into the ramp.
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
        x++;
      } while (x != 256);
      row++;
    }
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS,
                    BATTLE_BACKGROUND_BYTES);
    wait(1);
    // DEAD: never true, as the edge reaches only line 186 by the 27th frame;
    // the ROM keeps the test. Every column (120 lines, at most 63 late) is
    // done once the edge passes line 182.
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  // Then move every pixel up 64 colours, onto a second ramp at 96 to 127.
  palette = (u16 *)&BG_PLTT_COLOR(6, 0);
  i = 0;
  do {
    s32 half;
    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  i = 0;
  do {
    BLUE_RAMP_PIXELS[i] += 64;
    i++;
  } while (i != BATTLE_BACKGROUND_BYTES);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS,
                  BATTLE_BACKGROUND_BYTES);
  wait(1);
}

/// A triangle of light spins and shrinks onto the first target for 64 frames,
/// then a rune stands on the target; the battle shows the result on frame 64.
///
/// Abilities: Bind (BATTLE_EFFECT_BIND); Luff (BATTLE_EFFECT_LUFF).
///
/// When args->prelude is 1 a Jupiter Djinni first appears over the first
/// target, sways and rises away. The canvas darkens a little faster every 16
/// frames.
///
/// Runs BattleEffect_RunDjinniEntrance first when args->prelude is 1. Moves
/// BG2X so the canvas's x 64 is on the first target, cycles the palette
/// each frame through BattleEffect_UpdatePhasePalette, raises
/// work->transferValue every 16 frames, keeps the triangle's corners in
/// work->particles 0 to 2, and builds and frees its blitters every frame.
void BattleEffect_RunBind(struct BattleEffectParams *args) {
  struct ScreenPoint anchor;
  // World input and projected vertices reuse one coordinate workspace.
  union {
    struct Vector3 world;
    struct ScreenPoint projected;
  } position;
  struct ScreenPoint screen;
  struct Vector3 scale;
  s32 originX;
  s32 originY;
  BlitterPair draw;
  void **slots;
  void **cursor;
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

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  graphics = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (work->args->prelude == 1)
    BattleEffect_RunDjinniEntrance(args, ELEMENT_JUPITER, work->args->side,
                                   DJINNI_ANCHOR_OVER_TARGET, &originX,
                                   &originY);
  REG_BG2PA = Q_8_8(1);

  // Setup. The rune, the flares at 4096 and the Djinni at 8192, in the
  // rune's colours; the Djinni sheet's palette is skipped.
  palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
                      graphics);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD),
                      work->sheet + 4096);
  palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet + 8192);
  // 4 off every pixel each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  Projection_ScaledUnit(work->args->targets[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * 20 + 72; frame++) {
    // Frame loop: 20 frames a target and 72 more, though only the first
    // target is drawn.
    if (frame == 64)
      EFFECT_SHOW_DAMAGE(0);
    // Red, green and blue sway a third of a turn apart.
    BattleEffect_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);

    // The Djinni sways 20 pixels either way, once every 32 frames, and from
    // frame 33 rises 2 pixels a frame. For its first four frames it is drawn
    // a second time, additively, so it flashes in.
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
    // The blitters, plain and mirrored, keeping the brighter pixel; from
    // frame 32 the canvas darkens one level faster every 16 frames.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[1] = (DrawRectangle)HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_2);
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += CANVAS_FILL_WORD(1);

    // Only the first target is drawn.
    for (member = 0; member != 1; member++) {
      memberFrame = frame - member * 8;
      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      if (memberFrame >= 0 && memberFrame < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        position.world.x = actor->x.fixed;
        position.world.y = actor->y.fixed;
        position.world.z = actor->z.fixed;
        Projection_Point(&position.world, &screen);
        // Always canvas x 64, where BG2X puts the target; only the
        // projected y is kept.
        screen.x = anchor.x + shift;
        screen.y -= 24;
        // The triangle: its three corners, turned a third of a circle apart and
        // shrinking as they spin, are projected into the particles' velocity
        // fields; each side is 24 flares. Over 64 frames the scale falls from
        // 2.625 by 3/128 a frame while its roll and yaw turn steadily back
        // from half a turn to 0; frames 64 to 67 hold it still.
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
            Projection_Point(gBind_Vertex, &position.projected);
            point->velocityX = position.projected.x + screen.x;
            point->velocityY = position.projected.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct Particle *from = &work->particles[j + member * 32];
            struct Particle *to = &work->particles[(j + 1) % 3 + member * 32];

            // Flares shrink from size 5 to 1, a size every 16 frames.
            size = 5 - memberFrame / 16;
            for (k = 0; k != 24; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + gBattleEffect_FlareCells[size - 1] + 4096,
                      x - size, y - size, size * 2, size * 2);
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

  // Finish: the blitters were freed each frame.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. It would show the spinning triangle and rune of
/// BattleEffect_RunBind without the Djinni, with the user stepping up to the
/// first target on frame 46.
///
/// Abilities: none (BATTLE_EFFECT_UNUSED_TRIANGLE_STRIKE).
///
/// It reads its own copy of the vertex, gBind_TriangleStrikeVertex, which
/// holds the same point as Bind's (gBind_Vertex). Changes the same state as
/// BattleEffect_RunBind, on the narrow canvas at priority 0, and starts the
/// user's approach through BattleMotion_ApproachTargetFar.
void BattleEffect_RunUnusedTriangleStrike(struct BattleEffectParams *args) {
  struct ScreenPoint anchor;
  // World input and projected vertices reuse one coordinate workspace.
  union {
    struct Vector3 world;
    struct ScreenPoint projected;
  } position;
  struct ScreenPoint screen;
  struct Vector3 scale;
  void **slots;
  void **cursor;
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

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  graphics = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);

  // Setup, as for Bind: the rune, and the flares at 4096.
  palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
                      graphics);
  palette = Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD);
  Decompress_TaggedLz(palette, work->sheet + 4096);
  // 4 off every pixel each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  Projection_ScaledUnit(work->args->targets[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * 20 + 72; frame++) {
    // Frame loop, as in BattleEffect_RunBind.
    if (frame == 64)
      EFFECT_SHOW_DAMAGE(0);
    // The user sets off three quarters of the way to the target, arriving
    // in 16 steps.
    if (frame == 46)
      BattleMotion_ApproachTargetFar(work->args->user, work->args->targets[0],
                                     16, 0);
    BattleEffect_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[1] = (DrawRectangle)HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_2);
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += CANVAS_FILL_WORD(1);

    // Only the first target is drawn.
    for (member = 0; member != 1; member++) {
      memberFrame = frame - member * 8;
      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      if (memberFrame >= 0 && memberFrame < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        position.world.x = actor->x.fixed;
        position.world.y = actor->y.fixed;
        position.world.z = actor->z.fixed;
        Projection_Point(&position.world, &screen);
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
            Projection_Point(gBind_TriangleStrikeVertex, &position.projected);
            point->velocityX = position.projected.x + screen.x;
            point->velocityY = position.projected.y + screen.y + 16;
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

              draw[0](canvas,
                      work->sheet + gBattleEffect_FlareCells[size - 1] + 4096,
                      x - size, y - size, size * 2, size * 2);
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
