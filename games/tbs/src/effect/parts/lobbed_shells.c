// Part of effect.c, the battle effect object, compiled only through it.
// Lobbed shells (Mad Spatter, Decompose, Bone Chiller ...).

// Lobbed shells: a shell arcs from the user onto each target and bursts
// into smoke or shards, in seven colourings.

// Abilities: Mad Spatter (kind 347).
//
// Battle effect 347: gold shells that burst into shards.
DEFINE_EFFECT_KIND(BattleFx_RunMadSpatter, BattleFx_RunLobbedShells, 4)

// Abilities: Decompose (kind 350).
//
// Battle effect 350: green shells that burst into smoke.
DEFINE_EFFECT_KIND(BattleFx_RunDecompose, BattleFx_RunLobbedShells, 5)

// Abilities: Bone Chiller (kind 309).
//
// Battle effect 309: blue shells that burst into smoke.
DEFINE_EFFECT_KIND(BattleFx_RunBoneChiller, BattleFx_RunLobbedShells, 0)

// Abilities: Sticky Goo (kind 315).
//
// Battle effect 315: shells in the spider web's colours that burst into
// smoke.
DEFINE_EFFECT_KIND(BattleFx_RunStickyGoo, BattleFx_RunLobbedShells, 1)

// Abilities: Poison Ink (kind 340).
//
// Battle effect 340: shells in Mars Djinn colours that burst into smoke.
DEFINE_EFFECT_KIND(BattleFx_RunPoisonInk, BattleFx_RunLobbedShells, 6)

// Abilities: Sticky Poison (kind 322).
//
// Battle effect 322: lime shells that burst into smoke.
DEFINE_EFFECT_KIND(BattleFx_RunStickyPoison, BattleFx_RunLobbedShells, 2)

// Abilities: Rotten Blood (kind 330).
//
// Battle effect 330: green shells that burst into shards.
DEFINE_EFFECT_KIND(BattleFx_RunRottenBlood, BattleFx_RunLobbedShells, 3)

// gLobbedShells_Records has two entries per style; the first is nonzero when
// the shells burst into shards instead of smoke.

// The bursts live after the shells in gMapCellBuffer.
#define SHELL_BURSTS ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))
#define LOBBED_SHELLS_SLOTS 16           // particles reserved for each target
#define LOBBED_SHELLS_BURST_SLOTS 256    // burst particles after the shells
#define LOBBED_SHELLS_SMOKE_CELLS 0x3e80 // the smoke pictures in the sheet

// Nonzero when the style's shells burst into shards instead of smoke.
#define LOBBED_SHELLS_SHATTER(mode) gLobbedShells_Records[(mode) * 2]

// Lobs a shell from the user at each target in turn, 8 frames apart. Each
// tumbles through ten images as it flies and, when it lands, the target
// flashes and the battle shows the damage; the shell bursts into 4 smoke
// puffs that drift up or 16 shards that fly up and fall.
// mode : the style, 0 to 6: the colours and the row of gLobbedShells_Records.
void BattleFx_RunLobbedShells(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 total;
  struct MotionObject *source;
  struct BattleCamera *camera;
  u8 *palette;
  s32 i;
  s32 height;

  slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix

  // The shell images, the smoke at 0x3e80 and the style's palette.
  palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_SmokeSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + LOBBED_SHELLS_SMOKE_CELLS);
  if (mode == 0)
    palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
  else if (mode == 1)
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
  else if (mode == 2)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 3)
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  else if (mode == 4)
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  else if (mode == 6)
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // Each target's shells start 4 pixels up at the user, with a 16-frame
  // flight to the target and an upward throw.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  height = 0x40000;
  EFFECT_FOR_EACH_TARGET(k, work->effect) {
    struct MotionObject *target =
        GetBattleObjectSlotFar(work->effect->actors[k])->object;

    for (i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
      struct EffectStep *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];

      SET_POSITION(shot, source->x, height, source->z);
      SET_VELOCITY(shot, (target->x - source->x) >> 4, 0x40000,
                   (target->z - source->z) >> 4);
      shot->variant = 0;
    }
  }
  EFFECT_FREE_PARTICLES(i, SHELL_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

  // draw[0] draws the shells facing their flight; draw[1] draws the smoke
  // additively, or the shards mirrored the other way from draw[0].
  if (effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    if (LOBBED_SHELLS_SHATTER(mode) == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    }
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    if (LOBBED_SHELLS_SHATTER(mode) == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_MAX);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    }
  }
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  if (LOBBED_SHELLS_SHATTER(mode) == 0)
    total = work->effect->count * 8 + 72;
  else
    total = work->effect->count * 8 + 56;
  AudioCommand_PlayFar(SE_103);

  for (frame = 0; frame != total; frame++) {
    struct EffectStep *burst;

    EFFECT_VIEW_FROM_CAMERA(camera);
    // The shells: target k's shell flies from frame 8k, and the target is
    // hit on frame 8k + 17.
    k = 0;
    if (work->effect->count != 0) {
      do {
        if (frame >= k * 8) {
          struct EffectStep *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS];

          if (frame == k * 8 + 17) {
            ObjectGroup_UpdateMembers(work->effect->actors[k],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 16);
            BattleEventRuntime_BeginPhaseFar(SE_133);
          }
          /* FAKEMATCH: restores the CSE block boundary after
           * the hit block (k << 3 is recomputed). Without the block: differ
           * 449; without both of this function's empty blocks: differ 337. */
          do {
          } while (0);
          if (shot->variant >= 0) {
            // The shell tumbles through ten images, three frames each; the
            // last five lie on their side.
            s32 step = (frame - k * 8) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ApplyBaseAndYOffset(shot, &position);
            position.x >>= 1;
            if (step > 4)
              draw[0](canvas, work->sheet + step * 768, position.x - 16,
                      position.y - 12, 32, 24);
            else
              draw[0](canvas, work->sheet + step * 768, position.x - 12,
                      position.y - 16, 24, 32);
            if (shot->variant == 0)
              EffectStep_AdvanceWithGravity3D(shot, 63, -0x8000);
            // On the ground: the shell stops and bursts.
            if (shot->y < 0) {
              s32 count;
              s8 *records = gLobbedShells_Records;
              s32 row;

              shot->y = 0;
              shot->variant = 1;
              /* FAKEMATCH: native loads the table address
               * before the row (r0, r1) 2026-10-11: gLobbedShells_Records[mode
               * * 2] in the test differs 135, as a conditional expression too.
               */
              row = mode * 2;
              if (records[row] == 0)
                count = 4;
              else
                count = 16;
              for (i = 0; i != count; i++) {
                struct EffectStep *burst;
                s32 group = k * 4;

                /* FAKEMATCH: restores the CSE block boundary
                 * between group's set and group * 8. Without the block: differ
                 * 171; as SHELL_BURSTS + group * 8 + i without it: differ 177.
                 */
                do {
                } while (0);
                burst = &SHELL_BURSTS[group * 8 + i];

                SET_POSITION(burst, shot->x, shot->y, shot->z);
                if (LOBBED_SHELLS_SHATTER(mode) == 0) {
                  SET_VELOCITY(burst, ((Random16() & 63) - 32) << 11, 0,
                               ((Random16() & 63) - 32) << 11);
                } else {
                  SET_VELOCITY(burst, ((Random16() & 63) - 32) << 13,
                               ((Random16() & 31) + 32) << 12,
                               ((Random16() & 63) - 32) << 13);
                }
                burst->variant = 0;
              }
            }
          }
        }
        k++;
      } while (k != work->effect->count);
    }
    // The bursts, for 45 frames: smoke through six images, or shards that
    // tumble through nine images, half of them from a second set.
    for (i = 0, burst = SHELL_BURSTS; i != LOBBED_SHELLS_BURST_SLOTS;
         burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ApplyBaseAndYOffset(burst, &position);
        position.x >>= 1;
        if (LOBBED_SHELLS_SHATTER(mode) == 0) {
          draw[1](canvas,
                  work->sheet + burst->variant / 8 * 1152 +
                      LOBBED_SHELLS_SMOKE_CELLS,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          // Each shard faces the way it flies.
          flip = effect->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas,
              work->sheet + gLobbedShells_ShardOffsets[picture] + 0x1e00,
              position.x - gLobbedShells_ShardWidths[picture] / 2,
              position.y - gLobbedShells_ShardHeights[picture] / 2,
              gLobbedShells_ShardWidths[picture],
              gLobbedShells_ShardHeights[picture]);
        }
        if (LOBBED_SHELLS_SHATTER(mode) == 0)
          EffectStep_AdvanceWithGravity3D(burst, 62, 0x800);
        else
          EffectStep_AdvanceWithGravity3D(burst, 62, -0x8000);
        burst->variant++;
      }
    }
    Camera_ApplyShake(2, 2);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
