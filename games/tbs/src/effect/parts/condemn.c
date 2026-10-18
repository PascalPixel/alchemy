// Part of effect.c, the battle effect object, compiled only through it.
// Condemn.

#define CONDEMN_FRAMES 132
#define CONDEMN_BREAK_FRAME 88   // the panel breaks in a white flash
#define CONDEMN_BOUNCE_FRAME 120 // the falling target bounces

// gCondemn_PanelX and _PanelY have seven stages for each column and row.
#define CONDEMN_PANEL_X(column, stage) gCondemn_PanelX[(column)][(stage)]
#define CONDEMN_PANEL_Y(row, stage) gCondemn_PanelY[(row)][(stage)]

// Abilities: Condemn (kind 40).
//
// A tall panel (57x98) swings across the screen three times, each pass fading
// out through a ripple, the last over the first target; on frame 88 it shatters
// in a white flash into a burst of larger pieces and the target is knocked
// back, bouncing on frame 120. The effect layer ripples throughout. 132 frames.
void BattleFx_RunCondemn(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  void *cells;
  BlitterPair draw;
  struct MotionObject *target;
  /* FAKEMATCH: the second position is never used: it gives the
   * frame its native 76 bytes 2026-10-11: a single position shrinks only the
   * frame (differ 4). */
  struct EffectPosition pos[2];
  s32 swing[2];
  s32 drawPanel;
  s32 panelColumn;
  s32 panelRow;
  s32 zero;
  s32 frame;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  /* FAKEMATCH: the zero is held in sl through the setup calls.
   * Storing the literal 0 without the local: differ 66. */
  zero = 0;
  work->effect = effect;
  BattleFx_BeginTiledCanvas(0);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // The panel in work, the pieces of the burst in PARTICLES.
  sheet = Resource_GetTableEntry((s32)&ResourceId_CounterRevealSheet);
  Iwram_CopyWords((void *)BG_PLTT, sheet, 128);
  sheet = sheet + 128;
  Resource_DecodeType01(sheet, work);
  cells = Ram_MapCellBuffer;
  sheet = Resource_GetTableEntry((s32)&ResourceId_RedCrescentSheetA);
  sheet = sheet + 128;
  Resource_DecodeType01(sheet, cells);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma,
                                EFFECT_CALLBACK_ORDER);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                        zero); // redraw from blank each frame
  // curve is where the panel swings about, in 16.16 pixels: the first pass
  // high up on the near side.
  drawPanel = 1;
  if (work->effect->side == 1) {
    swing[0] = -0x500000;
  } else {
    swing[0] = 0x700000;
  }
  swing[1] = -0x200000;
  for (frame = 0; frame != CONDEMN_FRAMES; frame++) {
    s32 spin;
    s32 screenX;
    s32 screenY;
    s32 ripple;
    s32 lineOffset;
    s32 angle;
    s32 i;
    s32 *scanline;

    spin = frame << 9;
    screenX = (swing[0] >> 16) + (((Trig_Sin(spin) * 16)) >> 16) + 48;
    screenY = (swing[1] >> 16) + (((Trig_Cos(spin) * 4)) >> 16) + 16;
    if (frame == CONDEMN_BREAK_FRAME) {
      AudioCommand_PlayFar(SE_HEAVY_IMPACT);
    }
    // Frame 32: the second pass, lower and further across; the panel skips
    // a frame while it moves.
    if (frame == 32) {
      if (work->effect->side == 1) {
        swing[0] = -0x200000;
      } else {
        swing[0] = 0x480000;
      }
      swing[1] = 0x180000;
      drawPanel = 0;
    }
    if (frame == 33) {
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      drawPanel = 1;
    }
    // Frame 64: the last pass, on the first target.
    if (frame == 64) {
      EffectPosition_ProjectScaledBattler(work->effect->actors[0], pos);
      if (work->effect->side == 1) {
        swing[0] = (pos[0].x - 128) << 16;
      } else {
        swing[0] = (pos[0].x - 64) << 16;
      }
      swing[1] = 0;
      drawPanel = 0;
    }
    if (frame == 65) {
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      drawPanel = 1;
    }
    // The last 16 frames of each of the first two passes ripple harder and
    // harder as the panel fades out.
    scanline = work->bg2xLines;
    ripple = 0;
    if (frame <= 31) {
      if (frame > 15) {
        ripple = (frame * 2) - 32;
        REG_BLDALPHA = (31 - frame) | 0x1000;
      }
    } else if (frame <= 63) {
      if (frame > 47) {
        ripple = (frame * 2) - 96;
        REG_BLDALPHA = (63 - frame) | 0x1000;
      }
    }
    if (ripple < 0) {
      ripple = 0;
    }

    i = 0;
    lineOffset = (6 - screenX) << 8;
    angle = frame << 11;
    for (; i != 160; i++) {
      *scanline++ = lineOffset - ((Trig_Sin(angle) * ripple) >> 10);
      angle += 0x800;
    }
    if (drawPanel != 0) {
      if (work->effect->side == 0) {
        panelColumn = 0;
        panelRow = 0;
      } else {
        panelColumn = 1;
        /* FAKEMATCH: i is 160 here, so this is 0: a literal 0
         * drops a stack slot and shrinks the frame (differ 452). */
        panelRow = i >> 31;
      }
      // The panel until frame 91; from frame 88 the burst: a white flash
      // over the whole canvas, then larger pieces two frames each.
      if (frame <= 87) {
        draw[work->effect->side](canvas, work, CONDEMN_PANEL_X(panelColumn, 0),
                                 CONDEMN_PANEL_Y(panelRow, 0) + screenY, 57,
                                 98);
      } else {
        if (frame <= 91) {
          draw[work->effect->side](
              canvas, work, CONDEMN_PANEL_X(panelColumn, 0),
              CONDEMN_PANEL_Y(panelRow, 0) + screenY, 57, 98);
        }
        draw[work->effect->side](
            canvas, work->sheet + 0x15d2, CONDEMN_PANEL_X(panelColumn, 1),
            CONDEMN_PANEL_Y(panelRow, 1) + screenY, 99, 69);
        if ((frame - 88) <= 1U) {
          Iwram_FillWords(canvas, 0x4000, 0x3F3F3F3F);
        }
        if ((frame - 90) <= 1U) {
          draw[work->effect->side](
              canvas, work->sheet + 0x3081, CONDEMN_PANEL_X(panelColumn, 2),
              CONDEMN_PANEL_Y(panelRow, 2) + screenY, 128, 91);
        }
        if ((frame - 92) <= 1U) {
          draw[work->effect->side](
              canvas, Ram_MapCellBuffer, CONDEMN_PANEL_X(panelColumn, 3),
              CONDEMN_PANEL_Y(panelRow, 3) + screenY, 128, 91);
        }
        if ((frame - 94) <= 1U) {
          draw[work->effect->side](canvas, Ram_MapCellBuffer + 0x2d80,
                                   CONDEMN_PANEL_X(panelColumn, 4),
                                   CONDEMN_PANEL_Y(panelRow, 4) + screenY, 128,
                                   59);
        }
        if ((frame - 96) <= 1U) {
          draw[work->effect->side](canvas, Ram_MapCellBuffer + 0x4b00,
                                   CONDEMN_PANEL_X(panelColumn, 5),
                                   CONDEMN_PANEL_Y(panelRow, 5) + screenY, 122,
                                   29);
        }
        if ((frame - 98) <= 1U) {
          draw[work->effect->side](canvas, Ram_MapCellBuffer + 0x58d2,
                                   CONDEMN_PANEL_X(panelColumn, 6),
                                   CONDEMN_PANEL_Y(panelRow, 6) + screenY, 76,
                                   25);
        }
      }
    }
    // Frame 88: knock the target back, away from the middle of the field.
    if (frame == CONDEMN_BREAK_FRAME) {
      target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
      target->velocityY = 0x10000;
      target->acceleration = 0x20000;
      target->speedLimit = 0x20000;
      target->verticalMotionStrength = 0;
      target->autoFaceMotion = 0;
      target->snapToTarget = 0;
      Object_SetMoveTargetFar(target, target->x << 1, 0, target->z);
      ObjectGroup_UpdateMembers(work->effect->actors[0], -1, POSE_DAMAGE, -1,
                                0);
    }
    if (frame == CONDEMN_BOUNCE_FRAME) {
      GetBattleObjectSlotFar(work->effect->actors[0])
          ->object->verticalMotionStrength = 0xAB85;
    }
    EFFECT_PRESENT_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  BattleBackground_LoadFar(1, gBattleWork->background, 24);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

#undef CONDEMN_FRAMES
#undef CONDEMN_BREAK_FRAME
#undef CONDEMN_BOUNCE_FRAME
#undef CONDEMN_PANEL_X
#undef CONDEMN_PANEL_Y
