/// @file
/// The healing motes of Ply, Wish, Cure, Cure Poison, Tonic and Psy Crystal.
// Part of effect.c.

#include "../effect_internal.h"

// Healing sparkles come in seven modes of one runner,
// BattleEffect_RunHealingMotes; the wrappers name the mode of each effect.
//
// PARTICLES holds the motes: 512 are scattered, and target n draws from the
// 64 at n * 64 (32 of them, or 128 running into the next target's, by the
// effect's variant). Each mote keeps its position relative to the target's
// centre, so swirling the motes is a turn of the whole frame of reference,
// and drawing one in is a step of 2 pixels a frame along the line to the
// centre.

#define HEALING_DJINNI_CELLS 0x1000 ///< the djinni picture in the sheet

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
/// Abilities: Ply, Ply Well, Pure Ply, Herb, Nut, Vial, Potion, Soothing Water
/// (BATTLE_FX_PLY); Fizz (BATTLE_FX_PLY_204).
void BattleEffect_RunPly(struct BattleEffectArgument *effect) {
  BattleEffect_RunHealingMotes(effect, HEALING_MODE_PLY);
}
/// Healing motes tumble in on each target from all around it.
///
/// Abilities: Wish, Wish Well, Pure Wish (BATTLE_FX_WISH); Regenerate
/// (BATTLE_FX_WISH_44); Reflect (BATTLE_FX_WISH_45); Spritz
/// (BATTLE_FX_WISH_206).
void BattleEffect_RunWish(struct BattleEffectArgument *effect) {
  BattleEffect_RunHealingMotes(effect, HEALING_MODE_WISH);
}
/// A ring of healing motes below each target closes in on it, in the blast
/// sheet's colours.
///
/// Abilities: Cure, Cure Well, Potent Cure, Corn (BATTLE_FX_CURE); Worms
/// (BATTLE_FX_CURE_352).
void BattleEffect_RunCure(struct BattleEffectArgument *effect) {
  BattleEffect_RunHealingMotes(effect, HEALING_MODE_CURE);
}
/// Sparkles twinkle around each target, four more a frame, and go out one by
/// one instead of closing in.
///
/// Abilities: Cure Poison, Restore, Antidote, Elixir (BATTLE_FX_CURE_POISON).
///
/// Cure Poison and Antidote play HEALING_MODE_CURE_POISON; Restore and Elixir,
/// the stronger variant, play HEALING_MODE_RESTORE, whose sparkles also turn
/// on a second axis.
void BattleEffect_RunCurePoison(struct BattleEffectArgument *effect) {
  if (effect->variant == EFFECT_VARIANT_BASIC) {
    BattleEffect_RunHealingMotes(effect, HEALING_MODE_CURE_POISON);
    return;
  }
  BattleEffect_RunHealingMotes(effect, HEALING_MODE_RESTORE);
}

/// Plays the healing motes as Restore and Elixir show them
/// (HEALING_MODE_RESTORE). No effect table entry reaches this one.
UNUSED void
BattleEffect_RunUnusedRestoreMotes(struct BattleEffectArgument *effect) {
  BattleEffect_RunHealingMotes(effect, HEALING_MODE_RESTORE);
}
/// Sparkles twinkle around each target, turning on two axes, and go out one by
/// one; the runner plays HEALING_MODE_TONIC exactly as HEALING_MODE_RESTORE.
///
/// Abilities: Tonic (BATTLE_FX_TONIC).
void BattleEffect_RunTonic(struct BattleEffectArgument *effect) {
  BattleEffect_RunHealingMotes(effect, HEALING_MODE_TONIC);
}
/// Healing motes gather around each target in the Mars Djinni palette; with a
/// prelude, a Mars Djinni appears first.
///
/// Abilities: Psy Crystal, Adept Ring, Ember (BATTLE_FX_PSY_CRYSTAL).
void BattleEffect_RunPsyCrystal(struct BattleEffectArgument *effect) {
  BattleEffect_RunHealingMotes(effect, HEALING_MODE_PSY_CRYSTAL);
}
#define HEALING_MOTES 512 ///< motes in PARTICLES
#define HEALING_SLOTS 64  ///< particles reserved for each target
#define HEALING_STAGGER 8 ///< frames between one target's start and the next
#define HEALING_SOUND_DELAY 20  ///< frames from a target's start to its sound
#define HEALING_TINT_DELAY 36   ///< frames from a target's start to its tint
#define HEALING_TINT_FRAMES 28  ///< frames a target stays tinted white
#define HEALING_DAMAGE_FRAME 40 ///< the frame the battle shows the recovery

/// Motes of light scattered around each target swirl in towards it, one target
/// every 8 frames, with the recovery sound on each target's 20th frame and a
/// tint on its 36th.
///
/// Called by the wrappers above, each with its enum HealingMotesMode.
///
/// Sets work->effect, overwrites PARTICLES 0 to 511 and the first 64 colours
/// of the background palette, decodes into the battle sheet slot and
/// work->sheet, and for Ply writes REG_BG2X and REG_BG2PA. After a djinni
/// prelude it clears effect->variant, so the mote count and resource fall
/// back to the basic variant's. The transfer callback it adds is
/// removed again at the end, with the blitters and the canvas layer.
///
/// effect: the targets. With prelude 1, a djinni (Mars for Psy Crystal,
///         Mercury otherwise) appears first, sways and rises. variant picks
///         the motes per target (32 for the basic variant, 128 for the
///         strongest, otherwise 64) and, in every mode but Cure and Psy
///         Crystal, the resource (the ice shard for the basic variant, the
///         ice block otherwise).
/// mode: how the motes scatter and turn. Only Ply shows the canvas at its
///       own width, centred on the first target. Cure Poison, Restore and
///       Tonic put the motes out one by one instead of drawing them in;
///       they and Ply draw sparkle dots. Psy Crystal takes the Mars
///       resource, Cure the blast resource.
void BattleEffect_RunHealingMotes(struct BattleEffectArgument *effect,
                                  u32 mode) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frameCount;
  s32 member;
  u8 *sprites;
  s32 motesPerTarget;
  s32 canvasShiftX;
  s32 doubleWidth;
  DrawRectangle *draw;
  s32 frame;
  s32 i;
  u8 *resource;
  struct EffectPosition base;
  struct EffectPosition screen;
  s32 point[3];
  s32 originX;
  s32 originY;
  BlitterPair blit;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  canvasShiftX = 0;
  work->effect = effect;
  BattleEffect_BeginCanvasLayer(0);

  // Bring out the djinni, Mars for Psy Crystal and Mercury otherwise; origin
  // receives where it ends. Ply and Psy Crystal place it over the first
  // target, the others at the first target's x at depth 0.
  if (work->effect->prelude == 1) {
    s32 element =
        mode == HEALING_MODE_PSY_CRYSTAL ? ELEMENT_MARS : ELEMENT_MERCURY;

    if (mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_PLY)
      BattleEffect_RunDjinniEntrance(effect, element, work->effect->side,
                                     DJINNI_ANCHOR_OVER_TARGET, &originX,
                                     &originY);
    else
      BattleEffect_RunDjinniEntrance(effect, element, work->effect->side,
                                     DJINNI_ANCHOR_TARGET_X, &originX,
                                     &originY);
    work->effect->variant = EFFECT_VARIANT_BASIC;
  }

  // Ply shows the canvas at its own width, shifted so the first target
  // is at its centre (64); the others keep it at double width.
  if (mode == HEALING_MODE_PLY) {
    EffectPosition_ProjectScaledBattler(work->effect->actors[0], &base);
    canvasShiftX = 64 - base.x;
    REG_BG2X = canvasShiftX << 8;
    REG_BG2PA = Q_8_8(1);
    doubleWidth = 0;
  } else {
    doubleWidth = 1;
  }

  // Pictures, and the resource whose first 128 bytes are the palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, work->sheet, 0, 0);
  if (mode <= HEALING_MODE_WISH || mode == HEALING_MODE_CURE_POISON ||
      mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC) {
    if (work->effect->variant == EFFECT_VARIANT_BASIC)
      resource = Resource_GetTableEntry((s32)&ResourceId_IceShardSheet);
    else
      resource = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
  } else if (mode == HEALING_MODE_PSY_CRYSTAL) {
    resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    resource = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  }
  Iwram_CopyWords((void *)BG_PLTT, resource, 128);

  // The djinni: 40x40 at the canvas's own width, the small 20x40 picture at
  // double width.
  if (doubleWidth == 0) {
    if (mode == HEALING_MODE_PSY_CRYSTAL)
      resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      resource = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
  } else {
    if (mode == HEALING_MODE_PSY_CRYSTAL)
      resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet);
    else
      resource =
          Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet);
  }
  resource += 128;
  Resource_DecodeType01(resource, work->sheet + HEALING_DJINNI_CELLS);
  BattleEffect_FetchRectangleBlitters(work->effect->side,
                                      draw = blit); // [0] maximum, [1] additive

  // Scatter the motes around the centre (16.16, relative to the point the
  // frame loop centres on). Cure Poison, Restore and Tonic end 16 frames sooner
  // than the others, as their motes go out rather than travel in.
  if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL) {
    // x and z within 64 pixels, y 32 to 95 pixels up.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0x7f) + 64) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * HEALING_STAGGER + 88;
  } else if (mode == HEALING_MODE_WISH) {
    // All three within 64 pixels.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * HEALING_STAGGER + 88;
  } else if (mode == HEALING_MODE_CURE) {
    // A ring of radius 32 to 95 pixels, 50 pixels down. velocityY is set,
    // but the frame loop moves the motes only along the line to the centre.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      s32 heading = Random16() & 0xffff;
      s32 radius = (Random16() & 63) + 32;

      SET_POSITION(spark, Trig_Sin(heading) * radius, -50 << 16,
                   Trig_Cos(heading) * radius);
      spark->velocityY = ((Random16() & 31) + 32) << 13;
      spark->variant = 0;
    }
    frameCount = work->effect->count * HEALING_STAGGER + 88;
  } else if (mode == HEALING_MODE_CURE_POISON) {
    // x and z within 64 pixels, y within 32.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 14,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * HEALING_STAGGER + 72;
  } else {
    // All three within 64 pixels.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * HEALING_STAGGER + 72;
  }

  // Motes for each target: 32 for the basic variant, 128 for the strongest,
  // else 64.
  motesPerTarget = 64;
  if (work->effect->variant == EFFECT_VARIANT_BASIC)
    motesPerTarget = 32;
  else if (work->effect->variant == EFFECT_VARIANT_STRONGEST)
    motesPerTarget = 128;
  // Each transfer keeps 75% of every canvas pixel, so the motes leave
  // trails.
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = gCameraWork;

    if (frame == HEALING_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(0);

    // The djinni sways from side to side and, after 32 frames, rises 2 pixels
    // a frame. For the first 4 frames it is also added on top.
    if (work->effect->prelude == 1) {
      if (doubleWidth == 0) {
        s32 x =
            ((Trig_Sin(frame << 11) * 20) >> 16) + originX + canvasShiftX - 20;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + HEALING_DJINNI_CELLS, x, y, 40, 40);
        if (frame <= 3)
          draw[1](canvas, work->sheet + HEALING_DJINNI_CELLS, x, y, 40, 40);
      } else {
        s32 x = ((Trig_Sin(frame << 11) * 10) >> 16) + originX / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + HEALING_DJINNI_CELLS, x, y, 20, 40);
        if (frame <= 3)
          draw[1](canvas, work->sheet + HEALING_DJINNI_CELLS, x, y, 20, 40);
      }
    }

    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->actors[member])->object;
      s32 start = member * HEALING_STAGGER;

      // Centre on the point 40 pixels above the target.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, object->x, 40 << 16, object->z);
      SceneTransform_ApplyPosition(point);
      if (frame == start + HEALING_SOUND_DELAY)
        AudioCommand_PlayFar(SE_RECOVERY);
      if (frame == start + HEALING_TINT_DELAY)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, member,
                                  HEALING_TINT_FRAMES);

      // From the target's start, its frame of reference turns 1/128 of a
      // circle a frame. Cure offsets the yaw by 40 frames a target where the
      // others offset it by their start, and Wish does not offset it.
      if (frame > start) {
        if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL) {
          SceneTransform_ApplyYaw((frame - member * HEALING_STAGGER) << 9);
        } else if (mode == HEALING_MODE_WISH) {
          SceneTransform_ApplyPitch(frame << 9);
          SceneTransform_ApplyRoll(frame << 9);
        } else if (mode == HEALING_MODE_CURE) {
          SceneTransform_ApplyYaw((frame - member * 40) << 9);
        } else if (mode == HEALING_MODE_CURE_POISON) {
          SceneTransform_ApplyYaw((frame - member * HEALING_STAGGER) << 9);
        } else {
          SceneTransform_ApplyYaw((frame - member * HEALING_STAGGER) << 9);
          SceneTransform_ApplyPitch((frame - member * HEALING_STAGGER) << 9);
        }

        // Four motes appear each frame; for Cure Poison, Restore and Tonic mote
        // i goes out at frame start + i / 2 + 32.
        for (i = 0; i != motesPerTarget; i++) {
          struct EffectStep *spark = &PARTICLES[member * HEALING_SLOTS + i];
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
            // The distance to the centre in steps of 2 pixels: dividing each
            // axis by it moves the mote 2 pixels along the line.
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 size;

              EffectPosition_ProjectPoint(spark, &screen);
              if (mode == HEALING_MODE_PLY)
                screen.x += canvasShiftX;
              else
                screen.x >>= 1;
              screen.y += 16;

              // Size 3 to 1 over depths 314 to 634 (Ply cycles through 9
              // sizes).
              CLAMP(screen.depth, 314, 634);
              size = 3 - (screen.depth - 314) / 128;
              if (mode == HEALING_MODE_PLY)
                size = (i * 4 + frame) % 9;
              if (mode == HEALING_MODE_PLY ||
                  mode == HEALING_MODE_CURE_POISON ||
                  mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC) {
                u8 *cell = work->sheet + gBattleEffect_PuffCells[size];
                u32 edge = gBattleEffect_PuffSizes[size];
                u32 half = edge >> 1;

                draw[1](canvas, cell, screen.x - half, screen.y - half, edge,
                        edge);
              } else {
                EFFECT_DRAW_MOTE(draw[1], canvas, sprites, screen.x, screen.y,
                                 size);
              }

              // Ply, Wish, Cure and Psy Crystal draw the mote in; the others
              // leave it where it is until it goes out.
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

#undef HEALING_DJINNI_CELLS
#undef HEALING_MOTES
#undef HEALING_SLOTS
#undef HEALING_STAGGER
#undef HEALING_SOUND_DELAY
#undef HEALING_TINT_DELAY
#undef HEALING_TINT_FRAMES
#undef HEALING_DAMAGE_FRAME
