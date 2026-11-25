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
#define LOBBED_SHELLS_SMOKE_PICTURES                                           \
  16000 ///< where the smoke starts in the sheet
/// GoldShellSheet's shell as it tumbles, upright and then turned on its side.
#define LOBBED_SHELL_WIDTH 24
#define LOBBED_SHELL_HEIGHT 32
/// The shell's tumbling pictures, and the shards' pictures after them; it turns
/// on its side from picture LOBBED_SHELL_UPRIGHT_PICTURES, a picture every
/// three frames.
#define LOBBED_SHELL_PICTURES 10
#define LOBBED_SHELL_UPRIGHT_PICTURES 5
#define LOBBED_SHELLS_SHARD_PICTURES                                           \
  (LOBBED_SHELL_PICTURES * PICTURE_BYTES(LOBBED_SHELL))
/// The pieces a shell bursts into: puffs of smoke, or shards.
#define LOBBED_SHELLS_SMOKE_PUFFS 4
#define LOBBED_SHELLS_SHARDS 16
/// The frames a burst's piece is drawn for.
#define LOBBED_SHELLS_BURST_FRAMES 45
/// Each target's shell is lobbed this many frames after the last one's, and
/// lands LOBBED_SHELL_FLIGHT_FRAMES later, as its target flashes.
#define LOBBED_SHELLS_TARGET_FRAMES 8
#define LOBBED_SHELL_FLIGHT_FRAMES 17
/// The frames the effect runs on after the last shell is lobbed: smoke drifts
/// longer than shards fall.
#define LOBBED_SHELLS_SMOKE_END_FRAMES 72
#define LOBBED_SHELLS_SHARDS_END_FRAMES 56

/// Nonzero when the style's shells burst into shards instead of smoke.
#define LOBBED_SHELLS_SHATTER(mode)                                            \
  gLobbedShells_Styles[(mode)][LOBBED_SHELLS_STYLE_SHATTER]

/// Lobs a shell from the user at each target in turn; when it lands the target
/// flashes and the shell bursts into smoke that drifts up or shards that fly up
/// and fall.
void BattleEffect_RunLobbedShells(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint position;
  BlitterPair draw;
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  s32 k;
  s32 total;
  struct Actor *source;
  struct BattleCamera *camera;
  u8 *resource;
  s32 i;
  FIXED height;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  resource = Resource_GetTableEntry(RESOURCE_GOLD_SHELL_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, state->sheet);
  resource = Resource_GetTableEntry(RESOURCE_SMOKE_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, state->sheet + LOBBED_SHELLS_SMOKE_PICTURES);
  if (mode == LOBBED_SHELLS_BONE_CHILLER)
    resource = Resource_GetTableEntry(RESOURCE_BLUE_PALETTE);
  else if (mode == LOBBED_SHELLS_STICKY_GOO)
    resource = Resource_GetTableEntry(RESOURCE_SPIDER_WEB_SHEET);
  else if (mode == LOBBED_SHELLS_STICKY_POISON)
    resource = Resource_GetTableEntry(RESOURCE_LIME_PALETTE);
  else if (mode == LOBBED_SHELLS_ROTTEN_BLOOD)
    resource = Resource_GetTableEntry(RESOURCE_GREEN_PALETTE);
  else if (mode == LOBBED_SHELLS_MAD_SPATTER)
    resource = Resource_GetTableEntry(RESOURCE_GOLD_SHELL_SHEET);
  else if (mode == LOBBED_SHELLS_POISON_INK)
    resource = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
  else
    resource = Resource_GetTableEntry(RESOURCE_GREEN_PALETTE);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);

  // Each target's shell starts at the user.
  EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
  source = BattleObject_GetFar(state->args->user)->actor;
  height = toFIXED(4);
  EFFECT_FOR_EACH_TARGET(k, state->args) {
    struct Actor *target = BattleObject_GetFar(state->args->targets[k])->actor;

    for (i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
      struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];

      SET_POSITION(shot, source->x.fixed, height, source->z.fixed);
      SET_VELOCITY(shot, (target->x.fixed - source->x.fixed) >> 4, toFIXED(4),
                   (target->z.fixed - source->z.fixed) >> 4);
      shot->variant = 0;
    }
  }
  EFFECT_FREE_PARTICLES(i, LOBBED_SHELLS_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    if (LOBBED_SHELLS_SHATTER(mode) == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
    }
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    if (LOBBED_SHELLS_SHATTER(mode) == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
    }
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  if (LOBBED_SHELLS_SHATTER(mode) == 0)
    total = state->args->targetCount * LOBBED_SHELLS_TARGET_FRAMES +
            LOBBED_SHELLS_SMOKE_END_FRAMES;
  else
    total = state->args->targetCount * LOBBED_SHELLS_TARGET_FRAMES +
            LOBBED_SHELLS_SHARDS_END_FRAMES;
  Sound_PlayFar(SE_CURSE);

  for (frame = 0; frame != total; frame++) {
    struct Particle *burst;

    EFFECT_VIEW_FROM_CAMERA(camera);
    k = 0;
    if (state->args->targetCount != 0) {
      do {
        if (frame >= k * LOBBED_SHELLS_TARGET_FRAMES) {
          struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS];

          if (frame ==
              k * LOBBED_SHELLS_TARGET_FRAMES + LOBBED_SHELL_FLIGHT_FRAMES) {
            BattleEffect_TintBattler(state->args->targets[k],
                                     SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 16);
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
          }
          if (shot->variant >= 0) {
            s32 step;

            /* FAKEMATCH(dead): an empty once-only loop. */
            do {
            } while (0);
            // The shell tumbles as it flies.
            step = (frame - k * LOBBED_SHELLS_TARGET_FRAMES) / 3;
            if (step > LOBBED_SHELL_PICTURES - 1)
              step = LOBBED_SHELL_PICTURES - 1;
            Projection_Point(shot, &position);
            position.x >>= 1;
            if (step >= LOBBED_SHELL_UPRIGHT_PICTURES)
              draw[0](canvas, state->sheet + step * PICTURE_BYTES(LOBBED_SHELL),
                      position.x - LOBBED_SHELL_HEIGHT / 2,
                      position.y - LOBBED_SHELL_WIDTH / 2, LOBBED_SHELL_HEIGHT,
                      LOBBED_SHELL_WIDTH);
            else
              draw[0](canvas, state->sheet + step * PICTURE_BYTES(LOBBED_SHELL),
                      position.x - LOBBED_SHELL_WIDTH / 2,
                      position.y - LOBBED_SHELL_HEIGHT / 2, LOBBED_SHELL_WIDTH,
                      LOBBED_SHELL_HEIGHT);
            if (shot->variant == 0)
              Particle_AdvanceWithGravity3D(shot, 63, -toFIXED(1) / 2);
            // On the ground the shell stops and bursts.
            if (shot->y < 0) {
              s32 count;
              /* FAKEMATCH(temporary): records, a pointer to
               * gLobbedShells_Styles for the one read; as
               * LOBBED_SHELLS_SHATTER(mode). */
              s8 *records = gLobbedShells_Styles[0];
              s32 groupSlots = 8;

              shot->y = 0;
              shot->variant = 1;
              if (records[mode << 1] == 0)
                count = LOBBED_SHELLS_SMOKE_PUFFS;
              else
                count = LOBBED_SHELLS_SHARDS;
              // Each target's pieces take four groups of eight slots, room
              // for four shells as The Lost Age's Mud lobs; a shell's 16
              // shards fill two.
              for (i = 0; i != count; i++) {
                struct Particle *burst;
                burst = &LOBBED_SHELLS_BURSTS[(k << 2) * groupSlots + i];

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
      } while (k != state->args->targetCount);
    }
    // The bursts.
    for (i = 0; i != LOBBED_SHELLS_BURST_SLOTS; i++) {
      burst = &LOBBED_SHELLS_BURSTS[i];
      if (burst->variant >= 0 && burst->variant < LOBBED_SHELLS_BURST_FRAMES &&
          burst->y >= 0) {
        Projection_Point(burst, &position);
        position.x >>= 1;
        if (LOBBED_SHELLS_SHATTER(mode) == 0) {
          draw[1](canvas,
                  state->sheet +
                      burst->variant / 8 * PICTURE_BYTES(SMOKE_PUFF) +
                      LOBBED_SHELLS_SMOKE_PICTURES,
                  position.x - SMOKE_PUFF_WIDTH / 2,
                  position.y - SMOKE_PUFF_HEIGHT / 2, SMOKE_PUFF_WIDTH,
                  SMOKE_PUFF_HEIGHT);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          // Every second shard tumbles through the other nine pictures.
          if (i & 1)
            picture += 9;
          flip = args->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas,
              state->sheet + gLobbedShells_ShardPictures[picture] +
                  LOBBED_SHELLS_SHARD_PICTURES,
              position.x - gLobbedShells_ShardWidths[picture] / 2,
              position.y - gLobbedShells_ShardHeights[picture] / 2,
              gLobbedShells_ShardWidths[picture],
              gLobbedShells_ShardHeights[picture]);
        }
        // Smoke rises; shards fall.
        if (LOBBED_SHELLS_SHATTER(mode) == 0)
          Particle_AdvanceWithGravity3D(burst, 62, toFIXED(1) / 32);
        else
          Particle_AdvanceWithGravity3D(burst, 62, -toFIXED(1) / 2);
        burst->variant++;
      }
    }
    BattleCamera_Shake(2, 2);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

#undef LOBBED_SHELLS_BURSTS
#undef LOBBED_SHELLS_SLOTS
#undef LOBBED_SHELLS_BURST_SLOTS
#undef LOBBED_SHELLS_SMOKE_PICTURES
#undef LOBBED_SHELLS_SHATTER
#undef LOBBED_SHELL_WIDTH
#undef LOBBED_SHELL_HEIGHT
#undef LOBBED_SHELL_PICTURES
#undef LOBBED_SHELLS_SHARD_PICTURES
#undef LOBBED_SHELLS_SMOKE_PUFFS
#undef LOBBED_SHELLS_SHARDS
#undef LOBBED_SHELLS_BURST_FRAMES
#undef LOBBED_SHELL_UPRIGHT_PICTURES
#undef LOBBED_SHELLS_TARGET_FRAMES
#undef LOBBED_SHELL_FLIGHT_FRAMES
#undef LOBBED_SHELLS_SMOKE_END_FRAMES
#undef LOBBED_SHELLS_SHARDS_END_FRAMES
