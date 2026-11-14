/// @file
/// The healing motes of Ply, Wish, Cure, Cure Poison, Tonic and Psy Crystal.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// Healing sparkles come in seven modes of one runner, the wrappers naming the
// mode of each effect. Each mote keeps its place relative to its target, so the
// swirl is one turn of the whole group.

#define HEALING_DJINNI_PICTURE 4096 ///< the Djinni picture in the sheet

/// The healing motes' modes (BattleEffect_RunHealingMotes), each named for the
/// first ability that plays it.
enum HealingMotesMode {
  HEALING_MODE_PLY,         ///< swirling sparkles, the canvas at its own width
  HEALING_MODE_WISH,        ///< motes tumbling in from all around
  HEALING_MODE_CURE,        ///< a ring of motes below each target
  HEALING_MODE_CURE_POISON, ///< sparkles that go out one by one
  HEALING_MODE_RESTORE,     ///< the same, turning on a second axis
  HEALING_MODE_TONIC,       ///< played exactly as HEALING_MODE_RESTORE
  HEALING_MODE_PSY_CRYSTAL  ///< motes in the Mars Djinni palette
};

/// Sparkles of light swirl around each target and close in on it, in the ice
/// shard's or ice block's colours, with the canvas shown at its own width and
/// centred on the first target.
///
/// Abilities: Ply, Ply Well, Pure Ply, Herb, Nut, Vial, Potion, Soothing Water;
/// Fizz.
void BattleEffect_RunPly(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_PLY);
}
/// Healing motes tumble in on each target from all around it.
///
/// Abilities: Wish, Wish Well, Pure Wish; Regenerate; Reflect; Spritz.
void BattleEffect_RunWish(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_WISH);
}
/// A ring of healing motes below each target closes in on it, in the blast
/// sheet's colours.
///
/// Abilities: Cure, Cure Well, Potent Cure, Corn; Worms.
void BattleEffect_RunCure(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_CURE);
}
/// Sparkles twinkle around each target, four more a frame, and go out one by
/// one instead of closing in.
///
/// Abilities: Cure Poison, Restore, Antidote, Elixir.
void BattleEffect_RunCurePoison(struct BattleEffectParams *args) {
  if (args->variant == EFFECT_VARIANT_BASIC) {
    BattleEffect_RunHealingMotes(args, HEALING_MODE_CURE_POISON);
    return;
  }
  BattleEffect_RunHealingMotes(args, HEALING_MODE_RESTORE);
}

/// Plays the healing motes as Restore shows them; no effect table entry reaches
/// this one.
UNUSED void
BattleEffect_RunUnusedRestoreMotes(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_RESTORE);
}
/// Sparkles twinkle around each target, turning on two axes, and go out one by
/// one; the runner plays HEALING_MODE_TONIC exactly as HEALING_MODE_RESTORE.
///
/// Abilities: Tonic.
void BattleEffect_RunTonic(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_TONIC);
}
/// Healing motes gather around each target in the Mars Djinni palette; with a
/// prelude, a Mars Djinni appears first.
///
/// Abilities: Psy Crystal, Adept Ring, Ember.
void BattleEffect_RunPsyCrystal(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_PSY_CRYSTAL);
}

/// Motes of light scattered around each target swirl in towards it, one target
/// after another.
void BattleEffect_RunHealingMotes(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frameCount;
  s32 member;
  u8 *sprites;
  s32 motesPerTarget;
  s32 canvasShiftX;
  s32 doubleWidth;
  s32 frame;
  s32 i;
  u8 *resource;
  struct ScreenPoint base;
  struct ScreenPoint screen;
  s32 point[3];
  s32 originX;
  s32 originY;
  BlitterPair blit;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sprites = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  canvasShiftX = 0;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);

  if (work->args->prelude == 1) {
    s32 element =
        mode == HEALING_MODE_PSY_CRYSTAL ? ELEMENT_MARS : ELEMENT_MERCURY;

    if (mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_PLY)
      BattleEffect_RunDjinniEntrance(args, element, work->args->side,
                                     DJINNI_ANCHOR_OVER_TARGET, &originX,
                                     &originY);
    else
      BattleEffect_RunDjinniEntrance(args, element, work->args->side,
                                     DJINNI_ANCHOR_TARGET_X, &originX,
                                     &originY);
    work->args->variant = EFFECT_VARIANT_BASIC;
  }

  if (mode == HEALING_MODE_PLY) {
    Projection_ScaledUnit(work->args->targets[0], &base);
    canvasShiftX = 64 - base.x;
    REG_BG2X = canvasShiftX << 8;
    REG_BG2PA = Q_8_8(1);
    doubleWidth = 0;
  } else {
    doubleWidth = 1;
  }

  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, work->sheet, 0, 0);
  if (mode <= HEALING_MODE_WISH || mode == HEALING_MODE_CURE_POISON ||
      mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC) {
    if (work->args->variant == EFFECT_VARIANT_BASIC)
      resource = Resource_GetTableEntry(RESOURCE_ICE_SHARD_SHEET);
    else
      resource = Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
  } else if (mode == HEALING_MODE_PSY_CRYSTAL) {
    resource = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
  } else {
    resource = Resource_GetTableEntry(RESOURCE_BLAST_SHEET);
  }
  Iwram_CopyWords((void *)BG_PLTT, resource, 128);

  if (doubleWidth == 0) {
    if (mode == HEALING_MODE_PSY_CRYSTAL)
      resource = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
    else
      resource = Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SHEET);
  } else {
    if (mode == HEALING_MODE_PSY_CRYSTAL)
      resource = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET);
    else
      resource = Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
  }
  resource += 128;
  Decompress_TaggedLz(resource, work->sheet + HEALING_DJINNI_PICTURE);
  BattleEffect_FetchRectangleBlitters(work->args->side, blit);

  // Scatter the motes around each target's centre.
  if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL) {
    for (i = 0; i != 512; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0x7f) + 64) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->args->targetCount * 8 + 88;
  } else if (mode == HEALING_MODE_WISH) {
    for (i = 0; i != 512; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->args->targetCount * 8 + 88;
  } else if (mode == HEALING_MODE_CURE) {
    for (i = 0; i != 512; i++) {
      struct Particle *spark = &PARTICLES[i];
      s32 heading = Random16() & TRIG_ANGLE_MASK;
      s32 radius = (Random16() & 63) + 32;

      SET_POSITION(spark, Trig_Sin(heading) * radius, -px(50),
                   Trig_Cos(heading) * radius);
      spark->velocityY = ((Random16() & 31) + 32) << 13;
      spark->variant = 0;
    }
    frameCount = work->args->targetCount * 8 + 88;
  } else if (mode == HEALING_MODE_CURE_POISON) {
    for (i = 0; i != 512; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 14,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->args->targetCount * 8 + 72;
  } else {
    for (i = 0; i != 512; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->args->targetCount * 8 + 72;
  }

  motesPerTarget = 64;
  if (work->args->variant == EFFECT_VARIANT_BASIC)
    motesPerTarget = 32;
  else if (work->args->variant == EFFECT_VARIANT_STRONGEST)
    motesPerTarget = 128;
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    if (frame == 40)
      EFFECT_SHOW_DAMAGE(0);

    // The Djinni sways, then rises away.
    if (work->args->prelude == 1) {
      if (doubleWidth == 0) {
        s32 x =
            ((Trig_Sin(frame << 11) * 20) >> 16) + originX + canvasShiftX - 20;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + HEALING_DJINNI_PICTURE, x, y, 40, 40);
        if (frame <= 3)
          blit[1](canvas, work->sheet + HEALING_DJINNI_PICTURE, x, y, 40, 40);
      } else {
        s32 x = ((Trig_Sin(frame << 11) * 10) >> 16) + originX / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + HEALING_DJINNI_PICTURE, x, y, 20, 40);
        if (frame <= 3)
          blit[1](canvas, work->sheet + HEALING_DJINNI_PICTURE, x, y, 20, 40);
      }
    }

    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *actor =
          GetBattleObjectSlotFar(work->args->targets[member])->actor;
      s32 start = member * 8;

      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, actor->x.fixed, px(40), actor->z.fixed);
      Transform_ApplyPosition(point);
      if (frame == start + 20)
        Sound_PlayFar(SE_RECOVERY);
      if (frame == start + 36)
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, TINT_KEEP, member, 28);

      // Each target's motes swirl round it.
      if (frame > start) {
        if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL) {
          Transform_ApplyYaw((frame - start) << 9);
        } else if (mode == HEALING_MODE_WISH) {
          Transform_ApplyPitch(frame << 9);
          Transform_ApplyRoll(frame << 9);
        } else if (mode == HEALING_MODE_CURE) {
          Transform_ApplyYaw((frame - member * 40) << 9);
        } else if (mode == HEALING_MODE_CURE_POISON) {
          Transform_ApplyYaw((frame - start) << 9);
        } else {
          Transform_ApplyYaw((frame - start) << 9);
          Transform_ApplyPitch((frame - start) << 9);
        }

        // Four motes appear each frame.
        for (i = 0; i != motesPerTarget; i++) {
          struct Particle *spark = &PARTICLES[member * 64 + i];
          s32 endFrame;

          if (mode == HEALING_MODE_CURE_POISON ||
              mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC)
            endFrame = start + i / 2 + 32;
          else
            endFrame = 0x10000;
          if (frame > start + i / 4 && frame < endFrame) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 size;

              Projection_Point(spark, &screen);
              if (mode == HEALING_MODE_PLY)
                screen.x += canvasShiftX;
              else
                screen.x >>= 1;
              screen.y += 16;

              CLAMP(screen.depth, 314, 634);
              size = 3 - (screen.depth - 314) / 128;
              if (mode == HEALING_MODE_PLY)
                size = (i * 4 + frame) % 9;
              if (mode == HEALING_MODE_PLY ||
                  mode == HEALING_MODE_CURE_POISON ||
                  mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC) {
                u8 *picture = work->sheet + gBattleEffect_SparklePictures[size];
                u32 edge = gBattleEffect_SparkleSizes[size];
                u32 half = edge >> 1;

                blit[1](canvas, picture, screen.x - half, screen.y - half, edge,
                        edge);
              } else {
                EFFECT_DRAW_MOTE(blit[1], canvas, sprites, screen.x, screen.y,
                                 size);
              }

              // Ply, Wish, Cure and Psy Crystal draw the motes in; the others
              // leave them where they are until they go out.
              if (mode <= HEALING_MODE_CURE ||
                  mode == HEALING_MODE_PSY_CRYSTAL) {
                spark->x -= spark->x / distance;
                spark->y -= spark->y / distance;
                spark->z -= spark->z / distance;
              }
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef HEALING_DJINNI_PICTURE
