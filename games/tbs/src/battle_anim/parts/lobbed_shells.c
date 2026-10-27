/// @file
/// Lobbed shells (Mad Spatter, Decompose, Bone Chiller ...).
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// The shells take 16 slots of PARTICLES per target, of which the first
// flies; LOBBED_SHELLS_BURSTS gives the bursts' slots.

// Lobbed shells: a shell arcs from the user onto each target and bursts
// into smoke or shards, in seven colourings.

/// The lobbed shells' styles (BattleAnim_RunLobbedShells), each named for the
/// animation that plays it, and its row of gLobbedShells_Styles.
enum LobbedShellsMode {
  LOBBED_SHELLS_BONE_CHILLER,  ///< blue, bursting into smoke
  LOBBED_SHELLS_STICKY_GOO,    ///< the spider web's colours, smoke
  LOBBED_SHELLS_STICKY_POISON, ///< lime, smoke
  LOBBED_SHELLS_ROTTEN_BLOOD,  ///< green, shards
  LOBBED_SHELLS_MAD_SPATTER,   ///< gold, shards
  LOBBED_SHELLS_DECOMPOSE,     ///< green, smoke
  LOBBED_SHELLS_POISON_INK     ///< Mars Djinn colours, smoke
};

/// Gold shells arc from the user onto each target in turn and burst into
/// shards.
///
/// Abilities: Mad Spatter (BATTLE_ANIM_MAD_SPATTER).
void BattleAnim_RunMadSpatter(struct BattleAnimArgs *args) {
  BattleAnim_RunLobbedShells(args, LOBBED_SHELLS_MAD_SPATTER);
}
/// Green shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Decompose (BATTLE_ANIM_DECOMPOSE).
void BattleAnim_RunDecompose(struct BattleAnimArgs *args) {
  BattleAnim_RunLobbedShells(args, LOBBED_SHELLS_DECOMPOSE);
}
/// Blue shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Bone Chiller (BATTLE_ANIM_BONE_CHILLER).
void BattleAnim_RunBoneChiller(struct BattleAnimArgs *args) {
  BattleAnim_RunLobbedShells(args, LOBBED_SHELLS_BONE_CHILLER);
}
/// Shells in the spider web's colours arc from the user onto each target in
/// turn and burst into smoke.
///
/// Abilities: Sticky Goo (BATTLE_ANIM_STICKY_GOO).
void BattleAnim_RunStickyGoo(struct BattleAnimArgs *args) {
  BattleAnim_RunLobbedShells(args, LOBBED_SHELLS_STICKY_GOO);
}
/// Shells in Mars Djinn colours arc from the user onto each target in turn
/// and burst into smoke.
///
/// Abilities: Poison Ink (BATTLE_ANIM_POISON_INK).
void BattleAnim_RunPoisonInk(struct BattleAnimArgs *args) {
  BattleAnim_RunLobbedShells(args, LOBBED_SHELLS_POISON_INK);
}
/// Lime shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Sticky Poison (BATTLE_ANIM_STICKY_POISON).
void BattleAnim_RunStickyPoison(struct BattleAnimArgs *args) {
  BattleAnim_RunLobbedShells(args, LOBBED_SHELLS_STICKY_POISON);
}
/// Green shells arc from the user onto each target in turn and burst into
/// shards.
///
/// Abilities: Rotten Blood (BATTLE_ANIM_ROTTEN_BLOOD).
void BattleAnim_RunRottenBlood(struct BattleAnimArgs *args) {
  BattleAnim_RunLobbedShells(args, LOBBED_SHELLS_ROTTEN_BLOOD);
}
/// The bursts live after the shells in PARTICLES, from PARTICLES[256] (0x1c00
/// bytes is 256 particles of 28): 32 slots a target, of which 4 smoke puffs
/// or 16 shards are used.
#define LOBBED_SHELLS_BURSTS                                                   \
  ((struct Particle *)(Ram_MapGridCellBuffer + 0x1c00))
#define LOBBED_SHELLS_SLOTS 16        ///< particles reserved for each target
#define LOBBED_SHELLS_BURST_SLOTS 256 ///< burst particles after the shells
#define LOBBED_SHELLS_SMOKE_OFFSETS 0x3e80 ///< the smoke pictures in the sheet

/// Nonzero when the style's shells burst into shards instead of smoke: the
/// first of the style's two entries in gLobbedShells_Styles; the second is
/// never read.
#define LOBBED_SHELLS_SHATTER(mode) gLobbedShells_Styles[(mode) * 2]

/// Lobs a shell from the user at each target in turn, 8 frames apart. Each
/// tumbles through ten images as it flies and, when it lands, the target
/// flashes and the battle shows the damage; the shell bursts into 4 smoke
/// puffs that drift up or 16 shards that fly up and fall.
///
/// Called by the seven handlers above, from BattleAnim_RunMadSpatter to
/// BattleAnim_RunRottenBlood. Loads three palettes over the first 64
/// background colours in turn, the style's last; frees PARTICLES 0 to 1023,
/// then gives the shells 16 a target from 0 and the bursts 32 a target from
/// LOBBED_SHELLS_BURSTS. Runs 8 frames a target plus 72 for smoke or 56 for
/// shards, and before it returns removes its transfer callback, frees its
/// blitters and closes the canvas layer.
///
/// mode: the style (enum LobbedShellsMode): its colours and its row of
///       gLobbedShells_Styles, which makes Rotten Blood's and Mad Spatter's
///       shells burst into shards.
void BattleAnim_RunLobbedShells(struct BattleAnimArgs *args, s32 mode) {
  struct ScreenPoint position;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 total;
  struct Actor *source;
  struct BattleCamera *camera;
  u8 *palette;
  s32 i;
  s32 height;

  slots = gHeapSlots + HEAP_SLOT_BATTLE_ANIM;
  cursor = slots;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  camera = ANIM_SLOT(slots, HEAP_SLOT_CAMERA);
  work->args = args;
  BattleAnim_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // Setup. The shell images, the smoke at LOBBED_SHELLS_SMOKE_OFFSETS and the
  // style's palette; Decompose shares Rotten Blood's green.
  palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_SmokeSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet + LOBBED_SHELLS_SMOKE_OFFSETS);
  if (mode == LOBBED_SHELLS_BONE_CHILLER)
    palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
  else if (mode == LOBBED_SHELLS_STICKY_GOO)
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
  else if (mode == LOBBED_SHELLS_STICKY_POISON)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == LOBBED_SHELLS_ROTTEN_BLOOD)
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  else if (mode == LOBBED_SHELLS_MAD_SPATTER)
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  else if (mode == LOBBED_SHELLS_POISON_INK)
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // Each target's shells start 4 pixels up at the user, with a 16-frame
  // flight to the target and an upward throw of 4 pixels a frame; only the
  // first of each target's 16 is drawn.
  ANIM_FREE_PARTICLES(i, PARTICLES, 1024);
  source = GetBattleObjectSlotFar(work->args->user)->actor;
  height = 0x40000;
  ANIM_FOR_EACH_TARGET(k, work->args) {
    struct Actor *target =
        GetBattleObjectSlotFar(work->args->targets[k])->actor;

    for (i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
      struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];

      SET_POSITION(shot, source->x.fixed, height, source->z.fixed);
      SET_VELOCITY(shot, (target->x.fixed - source->x.fixed) >> 4, 0x40000,
                   (target->z.fixed - source->z.fixed) >> 4);
      shot->variant = 0;
    }
  }
  ANIM_FREE_PARTICLES(i, LOBBED_SHELLS_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

  // draw[0] draws the shells facing their flight; draw[1] draws the smoke
  // additively, or the shards mirrored the other way from draw[0].
  if (args->side == ANIM_SIDE_ENEMIES) {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
    draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
    if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER) {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
      draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
    } else {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
      draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
    }
  } else {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_LIGHTEN);
    draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
    if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER) {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
      draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
    } else {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
      draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
    }
  }
  // Trails keep 3/4 a frame. Smoke lingers 16 frames longer than shards.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER)
    total = work->args->targetCount * 8 + 72;
  else
    total = work->args->targetCount * 8 + 56;
  Sound_PlaySongFar(SE_CURSE);

  for (frame = 0; frame != total; frame++) {
    struct Particle *burst;

    ANIM_VIEW_FROM_CAMERA(camera);
    // Frame loop. The shells: target k's shell flies from frame 8k, and the
    // target is hit on frame 8k + 17, about when its shell lands; a shell's
    // variant turns 1 when it does.
    k = 0;
    if (work->args->targetCount != 0) {
      do {
        if (frame >= k * 8) {
          struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS];

          if (frame == k * 8 + 17) {
            BattleAnim_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, k, 16);
            ANIM_SHOW_DAMAGE(SE_IMPACT);
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
            Projection_Point(shot, &position);
            position.x >>= 1;
            if (step > 4)
              draw[0](canvas, work->sheet + step * (32 * 24), position.x - 16,
                      position.y - 12, 32, 24);
            else
              draw[0](canvas, work->sheet + step * (24 * 32), position.x - 12,
                      position.y - 16, 24, 32);
            // Half a pixel of gravity a frame while it flies.
            if (shot->variant == 0)
              Particle_AdvanceWithGravity3D(shot, 63, -0x8000);
            // On the ground: the shell stops, stays drawn where it lies, and
            // bursts into target k's run of LOBBED_SHELLS_BURSTS.
            if (shot->y < 0) {
              s32 count;
              s8 *records = gLobbedShells_Styles;
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
                struct Particle *burst;
                s32 group = k * 4;

                /* FAKEMATCH(one-pass): an empty one-pass block ends the CSE
                 * block between group's set and group * 8; without it, 48 of
                 * 670 instructions differ in tbs-en. */
                do {
                } while (0);
                burst = &LOBBED_SHELLS_BURSTS[group * 8 + i];

                SET_POSITION(burst, shot->x, shot->y, shot->z);
                // Smoke drifts up to a pixel a frame across the ground;
                // shards fly up to 4 across and 2 to 4 up.
                if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER) {
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
      } while (k != work->args->targetCount);
    }
    // The bursts, for 45 frames: smoke through six 24x48 images, eight
    // frames each, drifting up; or shards that tumble through nine images,
    // five frames each, the odd shards from a second set, and fall. A burst
    // that sinks below the ground is no longer drawn or moved. The unsigned
    // test also skips the free slots, whose variant is -1.
    for (i = 0, burst = LOBBED_SHELLS_BURSTS; i != LOBBED_SHELLS_BURST_SLOTS;
         burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        Projection_Point(burst, &position);
        position.x >>= 1;
        if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER) {
          draw[1](canvas,
                  work->sheet + burst->variant / 8 * (24 * 48) +
                      LOBBED_SHELLS_SMOKE_OFFSETS,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          // Each shard faces the way it flies. The shard pictures follow the
          // ten 768-byte shell images (0x1e00 bytes) in the sheet.
          flip = args->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas,
              work->sheet + gLobbedShells_ShardOffsets[picture] + 0x1e00,
              position.x - gLobbedShells_ShardWidths[picture] / 2,
              position.y - gLobbedShells_ShardHeights[picture] / 2,
              gLobbedShells_ShardWidths[picture],
              gLobbedShells_ShardHeights[picture]);
        }
        // Smoke rises, gaining 1/32 pixel a frame each frame; shards fall,
        // gaining half a pixel.
        if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER)
          Particle_AdvanceWithGravity3D(burst, 62, 0x800);
        else
          Particle_AdvanceWithGravity3D(burst, 62, -0x8000);
        burst->variant++;
      }
    }
    BattleCamera_Shake(2, 2);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}

#undef LOBBED_SHELLS_BURSTS
#undef LOBBED_SHELLS_SLOTS
#undef LOBBED_SHELLS_BURST_SLOTS
#undef LOBBED_SHELLS_SMOKE_OFFSETS
#undef LOBBED_SHELLS_SHATTER
