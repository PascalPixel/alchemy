// Part of effect.c, the battle effect object, compiled only through it.
// Bolt.

// Bolt's sparks fall under gravity and bounce off the ground at half speed.

// Lightning bolts: a bolt strikes each target, with sparks and glints.
// PARTICLES holds the sparks, 128 for each target.
// The second blitter, made anew for each glint.
#define BOLT_GLINT_BLITTER                                                     \
  ((DrawRectangle *)                                                           \
       gBattleFxWork)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT]

// The columns of a gBolt_Counts row.
enum {
  BOLT_SPARKS,       // sparks thrown from each bolt's foot
  BOLT_GLINTS,       // glints a frame around each target
  BOLT_GLINT_SPREAD, // how high and wide the glints spread
  BOLT_STRIKES       // bolts drawn over each other
};

// The bolt pictures in the sheet, after the tornado.
#define BOLT_CELLS 0xc56

// Abilities: Bolt, Flash Bolt, Blue Bolt (kind 16).
//
// A bolt of lightning strikes each target in turn, 8 frames apart.
//
// Target i's turn starts on frame 8i with a flash of the canvas. The bolt
// drops from the top, 64 pixels a frame down to y 104, and the screen shakes
// the frame after; on frame 8i + 2 sparks fly up from its foot and glints
// flicker there for 22 frames; on frame 8i + 4 the target recoils. The
// battle moves on to the results on frame 24.
//
// effect: the targets; variant picks the row of gBolt_Counts
//         and the glints' blend.
void BattleFx_RunBolt(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  /* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 60 bytes for 64 (differ 66, frame offsets
     only). */
  BlitterPair draw;
  u8 *sprites;
  struct EffectPosition screen;
  s32 j;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + BOLT_CELLS, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  for (j = 0; j != 1024; j++)
    PARTICLES[j].variant = 0;
  EFFECT_FREE_PARTICLES(j, work->particles, 64);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  AudioCommand_PlayFar(SE_ITEM_BREAK);
  for (frame = 0; frame != work->effect->count * 8 + 40; frame++) {
    if (frame == 24)
      BattleEventRuntime_BeginPhaseFar(SE_133);
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 8)
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
    }
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      s32 start = i * 8;

      EffectPosition_ProjectBattler(work->effect->actors[i], &screen);
      screen.x /= 2;
      if (frame == start + 1)
        work->shakeFrames = 4;
      if (frame == start + 4) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, i, 6);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
      }

      // The bolt (24 wide) grows 64 pixels a frame and stops at 104.
      if (frame >= start && frame < start + 16) {
        s32 height = (frame - start) << 6;

        if (height > 104)
          height = 104;
        for (j = 0; j != gBolt_Counts[work->effect->variant][BOLT_STRIKES];
             j++) {
          draw[0](canvas,
                  work->sheet +
                      (((i + frame + j) / 2) & 3) * LIGHTNING_BOLT_CELL_BYTES +
                      BOLT_CELLS,
                  screen.x - 12, 0, 24, height);
        }

        // Throw sparks upwards, within 90 degrees, from the foot (y = 104).
        if (frame == start + 2) {
          for (j = 0; j != gBolt_Counts[work->effect->variant][BOLT_SPARKS];
               j++) {
            struct EffectStep *spark = &PARTICLES[i * 128 + j];
            s32 speed = (Random16() & 0x1ff) + 64;
            s32 angle = (Random16() & 0x7fff) - 0x4000;

            spark->x = screen.x << 16;
            spark->y = 104 << 16;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
            spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
            spark->variant = (Random16() & 7) + 32;
          }
        }
      }

      // Glints: at random spots above the foot, flipped at random.
      if (frame >= start + 2 && frame < start + 24) {
        for (j = 0; j != gBolt_Counts[work->effect->variant][BOLT_GLINTS];
             j++) {
          s32 kind = j & 3;
          s32 drop = Random16() %
                     gBolt_Counts[work->effect->variant][BOLT_GLINT_SPREAD];
          s32 spread =
              gBolt_Counts[work->effect->variant][BOLT_GLINT_SPREAD] - drop + 1;
          s32 y = screen.y - drop - (gBattleFx_GlintCellHeights[kind] >> 1) + 8;
          u32 random = Random16();
          s32 x = screen.x;

          x += random % spread;
          x -= spread / 2;
          x -= (gBattleFx_GlintCellWidths[kind] >> 1);

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              gBolt_GlintFlips[Random16() & 3] |
                                  EFFECT_CLIP_BOUNDS,
                              gBolt_GlintModes[work->effect->variant]);
          BOLT_GLINT_BLITTER(canvas, work->sheet + gBattleFx_GlintCells[kind],
                             x, y, gBattleFx_GlintCellWidths[kind],
                             gBattleFx_GlintCellHeights[kind]);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        }
      }
    }

    // Sparks bounce off the ground at half speed and shrink as they die out.
    for (j = 0; j != 1024; j++) {
      struct EffectStep *spark = &PARTICLES[j];

      if (spark->variant > 0) {
        spark->variant--;
        EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
        if (spark->y > 104 << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x < 127 << 16) && spark->y >= 0) {
          s32 size = spark->variant / 16 + 1;
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;

          EFFECT_DRAW_MOTE(draw[0], canvas, sprites, x, y, size);
        }
      }
    }
    Camera_ApplyShake(2, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef BOLT_GLINT_BLITTER
#undef BOLT_CELLS
