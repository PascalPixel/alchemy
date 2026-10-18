// Part of effect.c, the battle effect object, compiled only through it.
// Gaia.

// The standing wall's top (48x96, stretched from 48x32) and foot (48x21)
// in the rebuilt sheet.
#define WALL_TOP_CELLS 0x13c0
#define WALL_FOOT_CELLS 0x25c0
#define GAIA_ROCKS 11 // rocks that tumble up the wall

// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche
// (kind 3).
//
// Rising wall: the ground swells, a wall of rock rises from it, stands
// while rocks tumble up its face and the targets are battered, and sinks
// again. The wall is symmetric: its left half is drawn, then mirrored for
// the right. Its centre is at canvas x 64 + wallX. The rocks are 11
// animation objects, moved by particles[10] to particles[20].
//
// effect: the targets; variant 0 to 2 widens the wall.
void BattleFx_RunGaia(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sprites;
  s32 wallX;
  s32 rockX;
  struct SpriteScale scale;
  struct SpritePlacement position;
  u8 *src;
  u16 *color;
  void *palette = (void *)BG_PLTT;
  s32 sheetIndex;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // The higher the variant, the wider the wall.
  if (work->effect->variant == 0)
    REG_BG2PA = Q_8_8(0.8); // 1.25 times the width
  else if (work->effect->variant == 1)
    REG_BG2PA = Q_8_8(1.0 / 1.5); // 1.5 times the width
  if (work->effect->side == 1) {
    wallX = 8;
    if (work->effect->variant == 0)
      rockX = 40;
    else if (work->effect->variant == 1)
      rockX = 36;
    else
      rockX = 40;
  } else {
    wallX = -16;
    rockX = -12;
  }
  REG_BG2X = gGaia_Bg2Shifts[work->effect->side][work->effect->variant] << 8;

  // Decode the EarthWall sheet into Ram_MapCellBuffer, and load the orange
  // palette with background colours 1 to 63 each 8 steps darker a channel.
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer,
                             1, 0);
  Iwram_CopyWords(palette,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
  color = (u16 *)&BG_PLTT[1];
  for (i = 0; i != 63; i++) {
    s32 entry = *color;
    s32 blue = ((u16)entry >> 10) & 31;
    s32 green = ((u16)entry >> 5) & 31;
    s32 red = 31;

    red &= entry;
    blue -= 8;
    green -= 8;
    red -= 8;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = RGB(red, green, blue);
  }

  // Rebuild the pictures in work->sheet:
  //   the growing wall: nine one-line rows, each stretched to 32 lines;
  //   the top of the wall (WALL_TOP_CELLS): 48x32 stretched to 48x96;
  //   the foot of the wall (WALL_FOOT_CELLS): 48x21;
  //   the swelling ground: three frames.
  src = Ram_MapCellBuffer;
  sheetIndex = 0;
  for (j = 0; j != 9; j++) {
    for (i = 0; i != 32; i++) {
      for (k = 0; k != gGaia_RowWidths[j]; k++, sheetIndex++)
        work->sheet[sheetIndex] = src[k];
    }
    src += gGaia_RowWidths[j];
  }
  for (j = 0; j != 32; j++) {
    for (i = 0; i != 3; i++) {
      for (k = 0; k != 48; k++, sheetIndex++)
        work->sheet[sheetIndex] = src[k];
    }
    src += 48;
  }
  for (i = 0; i != 1008; i++)
    work->sheet[sheetIndex++] = *src++;
  for (i = 0; i != 3; i++) {
    for (k = 0; k != gGaia_MoundWidths[i] * gGaia_MoundHeights[i]; k++)
      work->sheet[sheetIndex++] = *src++;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // The rock objects.
  for (i = 0; i != GAIA_ROCKS; i++) {
    struct AnimationObject *rock = ResourceObject_CreateFar(390);

    work->objects[i] = rock;
    if (rock != 0) {
      rock->flags = 0;
      AnimationObjects_SelectAnimationFar(rock, i / 4);
      work->objects[i]->image.priority = 1;
    }
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_MAX);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        75); // trails, fading to 3/4

  // Rocks: x is the column, y counts down from 128 by z, velocityX cycles
  // through 1 to 12, and variant is the wait before the rock appears.
  for (i = 0; i != GAIA_ROCKS; i++) {
    struct EffectStep *rock = &work->particles[10 + i];

    SET_POSITION(rock, (Random16() & 15) + 88, 128, (Random16() & 15) + 2);
    SET_VELOCITY(rock, i, 1, 0x8000);
    rock->variant = 44 - i * 4;
  }
  // Then particles[16] to particles[21] take the fixed columns of
  // gGaia_LateRockColumns.
  for (i = 0; i != 6; i++)
    work->particles[16 + i].x = gGaia_LateRockColumns[i];

  // Dust rises from the foot of the wall.
  for (i = 0; i != 256; i++) {
    struct EffectStep *dust = &PARTICLES[i];

    dust->x = ((Random16() & 63) + wallX + 32) << 16;
    dust->y = ((Random16() & 7) + 96) << 16;
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  work->shakeFrames = 250;
  for (frame = 0; frame != 192; frame++) {
    if (frame == 0)
      AudioCommand_PlayFar(SE_212);
    if (frame == 40)
      AudioCommand_PlayFar(SE_141);
    if (frame == 96)
      AudioCommand_PlayFar(SE_145);
    if (frame == 120)
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);

    // The swelling ground (frames 0 to 81).
    if (frame <= 81) {
      s32 cell = frame / 4;

      if (cell > 2)
        cell = (cell & 1) + 1;
      draw[0](canvas, work->sheet + gGaia_MoundCells[cell],
              wallX - gGaia_MoundWidths[cell] + 64,
              116 - gGaia_MoundHeights[cell], gGaia_MoundWidths[cell],
              gGaia_MoundHeights[cell]);
      draw[1](canvas, work->sheet + gGaia_MoundCells[cell], wallX + 64,
              116 - gGaia_MoundHeights[cell], gGaia_MoundWidths[cell],
              gGaia_MoundHeights[cell]);
    }

    // The rising wall (frames 12 to 87): its narrowest row until frame 66,
    // then a row wider every 3 frames, the widest from frame 85.
    if (frame >= 12 && frame <= 87) {
      s32 row = (frame - 64) / 3;

      CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowCells[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowCells[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The sinking wall (frames 160 to 183).
    if (frame >= 160 && frame <= 183) {
      s32 row = 7 - (frame - 160) / 3;

      CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowCells[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowCells[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The standing wall (frames 88 to 159).
    if (frame >= 88 && frame <= 159) {
      s32 width = 48;

      draw[0](canvas, &work->sheet[WALL_TOP_CELLS], wallX + 16, 0, width, 96);
      draw[1](canvas, &work->sheet[WALL_TOP_CELLS], wallX + 64, 0, width, 96);
      draw[0](canvas, &work->sheet[WALL_FOOT_CELLS], wallX + 16, 96, width, 21);
      draw[1](canvas, &work->sheet[WALL_FOOT_CELLS], wallX + 64, 96, width, 21);
    }

    // Dust (from frame 88): until frame 159, dust that leaves the top
    // returns to the foot.
    if (frame > 87) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *dust = &PARTICLES[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          EFFECT_DRAW_MOTE(draw[0], canvas, sprites, HI(dust->x), HI(dust->y),
                           size);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame <= 159)
            dust->y = 0x600000;
        } else {
          dust->variant--;
        }
      }
    }

    // Rocks (from frame 5): six at half size, then from frame 72 all 11, each
    // larger than the last and larger with the variant. The objects take
    // turns showing the rocks, moving on every 2 frames; four objects play
    // each of animations 0 and 1, three animation 2.
    if (frame > 4) {
      s32 rocks = 6;

      if (frame > 71)
        rocks = GAIA_ROCKS;
      for (k = 0; k != rocks; k++) {
        struct EffectStep *rock = &work->particles[10 + k];
        s32 slot;

        if (rock->variant == 0) {
          scale = unitScale;
          if (frame > 71) {
            scale.x = (k << 12) + 0x8000;
            scale.x += work->effect->variant << 14;
          } else {
            scale.x = 0x8000;
          }
          scale.y = scale.x;
          position.groundHeight = 0;
          position.x = (rock->x + rockX * 2) << 16;
          position.height = 0x2000000 - (rock->y << 16);
          position.y = 0x2000000;
          slot = (frame / 2 + k) % GAIA_ROCKS;
          if (slot != -1)
            Object_ApplyProjectedPlacementFar(work->objects[slot], &position,
                                              &scale, 0);
          rock->y -= rock->z;
          rock->velocityX += rock->velocityY;
          if (rock->velocityX > 12)
            rock->velocityX -= 12;
          if (rock->y < 0) {
            // Below 0 the rock starts again at 128 (from frame 88 in a new
            // column, at a new speed).
            if (frame > 159) {
              rock->variant = -1;
            } else {
              if (frame > 87) {
                rock->z = (Random16() & 7) + 8;
                if (work->effect->variant == 0)
                  rock->x = Random16() % 96 + 42;
                else if (work->effect->variant == 1)
                  rock->x = Random16() % 112 + 34;
                else
                  rock->x = Random16() % 160 + 10;
              }
              rock->y = 128;
              rock->variant = 8;
            }
          }
        } else {
          rock->variant--;
        }
      }
    }

    // The targets (frames 86 to 158): a flash every 12 frames and a recoil
    // every 4.
    if (frame <= 158) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame > 85) {
          if (frame % 12 == 0)
            ObjectGroup_UpdateMembers(work->effect->actors[i],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 6);
          if ((frame & 3) == 0)
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 5);
        }
      }
    }
    if (frame < 90 || frame > 160)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  for (i = 0; i != GAIA_ROCKS; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  BattleFx_EndCanvasLayer();
}
