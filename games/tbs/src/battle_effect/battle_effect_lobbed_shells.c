/// @file
/// Lobbed shells (Mad Spatter, Decompose, Bone Chiller ...).
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// Lobbed shells: a shell arcs from the user onto each target and bursts
// into smoke or shards, in seven colourings.

/// The lobbed shells' styles (BattleEffect_RunLobbedShells), each named for the
/// effect that plays it, and its row of gLobbedShells_Styles.
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
/// Abilities: Mad Spatter.
void BattleEffect_RunMadSpatter(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_MAD_SPATTER);
}
/// Green shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Decompose.
void BattleEffect_RunDecompose(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_DECOMPOSE);
}
/// Blue shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Bone Chiller.
void BattleEffect_RunBoneChiller(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_BONE_CHILLER);
}
/// Shells in the spider web's colours arc from the user onto each target in
/// turn and burst into smoke.
///
/// Abilities: Sticky Goo.
void BattleEffect_RunStickyGoo(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_STICKY_GOO);
}
/// Shells in Mars Djinn colours arc from the user onto each target in turn
/// and burst into smoke.
///
/// Abilities: Poison Ink.
void BattleEffect_RunPoisonInk(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_POISON_INK);
}
/// Lime shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Sticky Poison.
void BattleEffect_RunStickyPoison(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_STICKY_POISON);
}
/// Green shells arc from the user onto each target in turn and burst into
/// shards.
///
/// Abilities: Rotten Blood.
void BattleEffect_RunRottenBlood(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_ROTTEN_BLOOD);
}
/// The bursts, after the shells in PARTICLES.
#define LOBBED_SHELLS_BURSTS (PARTICLES + 256)
#define LOBBED_SHELLS_SLOTS 16        ///< particles reserved for each target
#define LOBBED_SHELLS_BURST_SLOTS 256 ///< burst particles after the shells
#define LOBBED_SHELLS_SMOKE_PICTURES 16000 ///< the smoke pictures in the sheet

/// Nonzero when the style's shells burst into shards instead of smoke.
#define LOBBED_SHELLS_SHATTER(mode) gLobbedShells_Styles[(mode) * 2]

/// Lobs a shell from the user at each target in turn; when it lands the target
/// flashes and the shell bursts into smoke that drifts up or shards that fly up
/// and fall.
void BattleEffect_RunLobbedShells(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint position;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 total;
  struct Actor *source;
  struct BattleCamera *camera;
  u8 *palette;
  s32 i;
  s32 height;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  palette = Resource_GetTableEntry(RESOURCE_GOLD_SHELL_SHEET);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry(RESOURCE_SMOKE_SHEET);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet + LOBBED_SHELLS_SMOKE_PICTURES);
  if (mode == LOBBED_SHELLS_BONE_CHILLER)
    palette = Resource_GetTableEntry(RESOURCE_BLUE_PALETTE);
  else if (mode == LOBBED_SHELLS_STICKY_GOO)
    palette = Resource_GetTableEntry(RESOURCE_SPIDER_WEB_SHEET);
  else if (mode == LOBBED_SHELLS_STICKY_POISON)
    palette = Resource_GetTableEntry(RESOURCE_LIME_PALETTE);
  else if (mode == LOBBED_SHELLS_ROTTEN_BLOOD)
    palette = Resource_GetTableEntry(RESOURCE_GREEN_PALETTE);
  else if (mode == LOBBED_SHELLS_MAD_SPATTER)
    palette = Resource_GetTableEntry(RESOURCE_GOLD_SHELL_SHEET);
  else if (mode == LOBBED_SHELLS_POISON_INK)
    palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
  else
    palette = Resource_GetTableEntry(RESOURCE_GREEN_PALETTE);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // Each target's shell starts at the user.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  source = GetBattleObjectSlotFar(work->args->user)->actor;
  height = px(4);
  EFFECT_FOR_EACH_TARGET(k, work->args) {
    struct Actor *target =
        GetBattleObjectSlotFar(work->args->targets[k])->actor;

    for (i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
      struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];

      SET_POSITION(shot, source->x.fixed, height, source->z.fixed);
      SET_VELOCITY(shot, (target->x.fixed - source->x.fixed) >> 4, px(4),
                   (target->z.fixed - source->z.fixed) >> 4);
      shot->variant = 0;
    }
  }
  EFFECT_FREE_PARTICLES(i, LOBBED_SHELLS_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
    }
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
    }
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER)
    total = work->args->targetCount * 8 + 72;
  else
    total = work->args->targetCount * 8 + 56;
  Sound_PlayFar(SE_CURSE);

  for (frame = 0; frame != total; frame++) {
    struct Particle *burst;

    EFFECT_VIEW_FROM_CAMERA(camera);
    k = 0;
    if (work->args->targetCount != 0) {
      do {
        if (frame >= k * 8) {
          struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS];

          if (frame == k * 8 + 17) {
            BattleEffect_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, k, 16);
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
          }
          if (shot->variant >= 0) {
            s32 step;

            /* FAKEMATCH(dead): an empty once-only loop ends CSE's block;
             * without it (the call's argument spelled out or as shot), 449 of
             * 670 instructions differ in tbs-en. */
            do {
            } while (0);
            // The shell tumbles as it flies.
            step = (frame - k * 8) / 3;
            if (step > 9)
              step = 9;
            Projection_Point(&PARTICLES[k * LOBBED_SHELLS_SLOTS], &position);
            position.x >>= 1;
            if (step > 4)
              draw[0](canvas, work->sheet + step * (32 * 24), position.x - 16,
                      position.y - 12, 32, 24);
            else
              draw[0](canvas, work->sheet + step * (24 * 32), position.x - 12,
                      position.y - 16, 24, 32);
            if (shot->variant == 0)
              Particle_AdvanceWithGravity3D(shot, 63, -px(1) / 2);
            // On the ground the shell stops and bursts.
            if (shot->y < 0) {
              s32 count;
              s8 *records = gLobbedShells_Styles;
              s32 row;
              s32 groupSlots = 8;

              shot->y = 0;
              shot->variant = 1;
              row = mode * 2;
              if (records[row] == 0)
                count = 4;
              else
                count = 16;
              for (i = 0; i != count; i++) {
                struct Particle *burst;
                s32 group = k * 4;
                burst = &LOBBED_SHELLS_BURSTS[group * groupSlots + i];

                SET_POSITION(burst, shot->x, shot->y, shot->z);
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
    // The bursts.
    for (i = 0, burst = LOBBED_SHELLS_BURSTS; i != LOBBED_SHELLS_BURST_SLOTS;
         burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        Projection_Point(burst, &position);
        position.x >>= 1;
        if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER) {
          draw[1](canvas,
                  work->sheet + burst->variant / 8 * (24 * 48) +
                      LOBBED_SHELLS_SMOKE_PICTURES,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = args->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas,
              work->sheet + gLobbedShells_ShardPictures[picture] + 10 * 768,
              position.x - gLobbedShells_ShardWidths[picture] / 2,
              position.y - gLobbedShells_ShardHeights[picture] / 2,
              gLobbedShells_ShardWidths[picture],
              gLobbedShells_ShardHeights[picture]);
        }
        // Smoke rises; shards fall.
        if (LOBBED_SHELLS_SHATTER(mode) == LOBBED_SHELLS_BONE_CHILLER)
          Particle_AdvanceWithGravity3D(burst, 62, px(1) / 32);
        else
          Particle_AdvanceWithGravity3D(burst, 62, -px(1) / 2);
        burst->variant++;
      }
    }
    BattleCamera_Shake(2, 2);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef LOBBED_SHELLS_BURSTS
#undef LOBBED_SHELLS_SLOTS
#undef LOBBED_SHELLS_BURST_SLOTS
#undef LOBBED_SHELLS_SMOKE_PICTURES
#undef LOBBED_SHELLS_SHATTER
