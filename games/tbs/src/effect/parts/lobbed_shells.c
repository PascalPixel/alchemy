/// @file
/// Lobbed shells (Mad Spatter, Decompose, Bone Chiller ...).
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// The shells take 16 slots of PARTICLES per target, of which the first
// flies; the shell bursts reuse the map buffer as records of their own.

// Lobbed shells: a shell arcs from the user onto each target and bursts
// into smoke or shards, in seven colourings.

/// Gold shells that burst into shards.
///
/// Abilities: Mad Spatter (kind 347).
void BattleFx_RunMadSpatter(struct BattleEffectArgument *effect) {
  BattleFx_RunLobbedShells(effect, 4);
}
/// Green shells that burst into smoke.
///
/// Abilities: Decompose (kind 350).
void BattleFx_RunDecompose(struct BattleEffectArgument *effect) {
  BattleFx_RunLobbedShells(effect, 5);
}
/// Blue shells that burst into smoke.
///
/// Abilities: Bone Chiller (kind 309).
void BattleFx_RunBoneChiller(struct BattleEffectArgument *effect) {
  BattleFx_RunLobbedShells(effect, 0);
}
/// Shells in the spider web's colours that burst into smoke.
///
/// Abilities: Sticky Goo (kind 315).
void BattleFx_RunStickyGoo(struct BattleEffectArgument *effect) {
  BattleFx_RunLobbedShells(effect, 1);
}
/// Shells in Mars Djinn colours that burst into smoke.
///
/// Abilities: Poison Ink (kind 340).
void BattleFx_RunPoisonInk(struct BattleEffectArgument *effect) {
  BattleFx_RunLobbedShells(effect, 6);
}
/// Lime shells that burst into smoke.
///
/// Abilities: Sticky Poison (kind 322).
void BattleFx_RunStickyPoison(struct BattleEffectArgument *effect) {
  BattleFx_RunLobbedShells(effect, 2);
}
/// Green shells that burst into shards.
///
/// Abilities: Rotten Blood (kind 330).
void BattleFx_RunRottenBlood(struct BattleEffectArgument *effect) {
  BattleFx_RunLobbedShells(effect, 3);
}
/// The bursts live after the shells in PARTICLES: 32 slots a target, of
/// which 4 smoke puffs or 16 shards are used.
#define LOBBED_SHELLS_BURSTS ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))
#define LOBBED_SHELLS_SLOTS 16           ///< particles reserved for each target
#define LOBBED_SHELLS_BURST_SLOTS 256    ///< burst particles after the shells
#define LOBBED_SHELLS_SMOKE_CELLS 0x3e80 ///< the smoke pictures in the sheet

/// Nonzero when the style's shells burst into shards instead of smoke: the
/// first of the style's two entries in gLobbedShells_Records; the second is
/// never read.
#define LOBBED_SHELLS_SHATTER(mode) gLobbedShells_Records[(mode) * 2]

/// Lobs a shell from the user at each target in turn, 8 frames apart. Each
/// tumbles through ten images as it flies and, when it lands, the target
/// flashes and the battle shows the damage; the shell bursts into 4 smoke
/// puffs that drift up or 16 shards that fly up and fall.
///
/// Called by the seven handlers above, from BattleFx_RunMadSpatter to
/// BattleFx_RunRottenBlood. Loads three palettes over the first 64
/// background colours in turn, the style's last; uses PARTICLES 0 to 1023
/// for the shells and LOBBED_SHELLS_BURSTS for the bursts.
///
/// mode : the style, 0 to 6: the colours and the row of gLobbedShells_Records.
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
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // Setup. The shell images, the smoke at LOBBED_SHELLS_SMOKE_CELLS and the
  // style's palette; mode 5 shares mode 3's green.
  palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
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
  // flight to the target and an upward throw of 4 pixels a frame; only the
  // first of each target's 16 is drawn.
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
  EFFECT_FREE_PARTICLES(i, LOBBED_SHELLS_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

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
  // Trails keep 3/4 a frame. Smoke lingers 16 frames longer than shards.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  if (LOBBED_SHELLS_SHATTER(mode) == 0)
    total = work->effect->count * 8 + 72;
  else
    total = work->effect->count * 8 + 56;
  AudioCommand_PlayFar(SE_103);

  for (frame = 0; frame != total; frame++) {
    struct EffectStep *burst;

    EFFECT_VIEW_FROM_CAMERA(camera);
    // Frame loop. The shells: target k's shell flies from frame 8k, and the
    // target is hit on frame 8k + 17, about when its shell lands; a shell's
    // variant turns 1 when it does.
    k = 0;
    if (work->effect->count != 0) {
      do {
        if (frame >= k * 8) {
          struct EffectStep *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS];

          if (frame == k * 8 + 17) {
            ObjectGroup_UpdateMembers(work->effect->actors[k],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 16);
            EFFECT_SHOW_DAMAGE(SE_133);
          }
          /* FAKEMATCH(one-pass): an empty one-pass block ends the CSE block
           * after the hit, so k * 8 is recomputed; without it, 200 of 670
           * instructions differ in tbs-en. */
          do {
          } while (0);
          if (shot->variant >= 0) {
            // The shell tumbles through ten images, three frames each; the
            // last five lie on their side.
            s32 step = (frame - k * 8) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ProjectPoint(shot, &position);
            position.x >>= 1;
            if (step > 4)
              draw[0](canvas, work->sheet + step * (32 * 24), position.x - 16,
                      position.y - 12, 32, 24);
            else
              draw[0](canvas, work->sheet + step * (24 * 32), position.x - 12,
                      position.y - 16, 24, 32);
            // Half a pixel of gravity a frame while it flies.
            if (shot->variant == 0)
              EffectStep_AdvanceWithGravity3D(shot, 63, -0x8000);
            // On the ground: the shell stops, stays drawn where it lies, and
            // bursts into target k's run of LOBBED_SHELLS_BURSTS.
            if (shot->y < 0) {
              s32 count;
              s8 *records = gLobbedShells_Records;
              s32 row;

              shot->y = 0;
              shot->variant = 1;
              /* FAKEMATCH(temporary): the table's address held in records
               * loads before the row; as LOBBED_SHELLS_SHATTER(mode) in the
               * test, 60 of 670 instructions differ in tbs-en. */
              row = mode * 2;
              if (records[row] == 0)
                count = 4;
              else
                count = 16;
              for (i = 0; i != count; i++) {
                struct EffectStep *burst;
                s32 group = k * 4;

                /* FAKEMATCH(one-pass): an empty one-pass block ends the CSE
                 * block between group's set and group * 8; without it, 48 of
                 * 670 instructions differ in tbs-en. */
                do {
                } while (0);
                burst = &LOBBED_SHELLS_BURSTS[group * 8 + i];

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
    // The bursts, for 45 frames: smoke through six 24x48 images, eight
    // frames each, drifting up; or shards that tumble through nine images,
    // five frames each, the odd shards from a second set, and fall. A burst
    // that sinks below the ground is no longer drawn or moved.
    for (i = 0, burst = LOBBED_SHELLS_BURSTS; i != LOBBED_SHELLS_BURST_SLOTS;
         burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ProjectPoint(burst, &position);
        position.x >>= 1;
        if (LOBBED_SHELLS_SHATTER(mode) == 0) {
          draw[1](canvas,
                  work->sheet + burst->variant / 8 * (24 * 48) +
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

#undef LOBBED_SHELLS_BURSTS
#undef LOBBED_SHELLS_SLOTS
#undef LOBBED_SHELLS_BURST_SLOTS
#undef LOBBED_SHELLS_SMOKE_CELLS
#undef LOBBED_SHELLS_SHATTER
