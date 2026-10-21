/// @file
/// The healing motes of Ply, Wish, Cure, Cure Poison, Tonic and Psy Crystal.
// Part of effect.c.

#include "../effect_internal.h"

// Healing sparkles come in seven modes of one runner,
// BattleFx_RunHealingMotes; the wrappers name the mode of each effect.
//
// PARTICLES holds the motes: 512 are scattered, and target n draws from the
// 64 at n * 64 (32 of them, or 128 running into the next target's, by the
// effect's variant). Each mote keeps its position relative to the target's
// centre, so swirling the motes is a turn of the whole frame of reference,
// and drawing one in is a step of 2 pixels a frame along the line to the
// centre.

#define HEALING_DJINNI_CELLS 0x1000 ///< the djinni picture in the sheet

/// Sparkles of light swirl around each target and close in on it, in the ice
/// shard's or ice block's colours, with the canvas shown at its own width and
/// centred on the first target.
///
/// Abilities: Ply, Ply Well, Pure Ply, Herb, Nut, Vial, Potion, Soothing Water
/// (BATTLE_FX_PLY); Fizz (BATTLE_FX_PLY_204).
void BattleFx_RunPly(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 0);
}
/// Healing motes tumble in on each target from all around it.
///
/// Abilities: Wish, Wish Well, Pure Wish (BATTLE_FX_WISH); Regenerate
/// (BATTLE_FX_WISH_44); Reflect (BATTLE_FX_WISH_45); Spritz
/// (BATTLE_FX_WISH_206).
void BattleFx_RunWish(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 1);
}
/// A ring of healing motes below each target closes in on it, in the blast
/// sheet's colours.
///
/// Abilities: Cure, Cure Well, Potent Cure, Corn (BATTLE_FX_CURE); Worms
/// (BATTLE_FX_CURE_352).
void BattleFx_RunCure(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 2);
}
/// Sparkles twinkle around each target, four more a frame, and go out one by
/// one instead of closing in.
///
/// Abilities: Cure Poison, Restore, Antidote, Elixir (BATTLE_FX_CURE_POISON).
///
/// Cure Poison and Antidote (variant 0) play mode 3; Restore and Elixir
/// (variant 1) play mode 4, whose sparkles also turn on a second axis.
void BattleFx_RunCurePoison(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunHealingMotes(effect, 3);
    return;
  }
  BattleFx_RunHealingMotes(effect, 4);
}

/// Plays the healing motes in mode 4, the sparkles Cure Poison shows for
/// variants other than 0 (Restore and Elixir). No effect table entry reaches
/// this one.
UNUSED void BattleFx_RunHealingMotesMode4(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 4);
}
/// Sparkles twinkle around each target, turning on two axes, and go out one by
/// one; the runner plays Tonic's mode 5 exactly as it plays mode 4.
///
/// Abilities: Tonic (BATTLE_FX_TONIC).
void BattleFx_RunTonic(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 5);
}
/// Healing motes gather around each target in the Mars Djinni palette; with a
/// prelude, a Mars Djinni appears first.
///
/// Abilities: Psy Crystal, Adept Ring, Ember (BATTLE_FX_PSY_CRYSTAL).
void BattleFx_RunPsyCrystal(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 6);
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
/// Called by the wrappers above: BattleFx_RunPly (mode 0), BattleFx_RunWish
/// (1), BattleFx_RunCure (2), BattleFx_RunCurePoison (3 or 4),
/// BattleFx_RunTonic (5) and BattleFx_RunPsyCrystal (6).
///
/// Sets work->effect, overwrites PARTICLES 0 to 511 and the first 64 colours
/// of the background palette, decodes into the battle sheet slot and
/// work->sheet, and in mode 0 writes REG_BG2X and REG_BG2PA. After a djinni
/// prelude it clears effect->variant, so the variant's mote count and
/// resource fall back to variant 0's. The transfer callback it adds is
/// removed again at the end, with the blitters and the canvas layer.
///
/// effect: the targets. With prelude 1, a djinni (Mars in mode 6, Mercury
///         otherwise) appears first, sways and rises. variant picks the
///         motes per target (0: 32, 2: 128, others: 64) and, in modes 0,
///         1, 3, 4 and 5, the resource (0: ice shard, others: ice block).
/// mode: how the motes scatter and turn. Only mode 0 shows the canvas at
///       its own width, centred on the first target. Modes 3, 4 and 5 put
///       the motes out one by one instead of drawing them in; modes 0, 3,
///       4 and 5 draw sparkle dots. Mode 6 takes the Mars resource, mode 2
///       the blast resource.
void BattleFx_RunHealingMotes(struct BattleEffectArgument *effect, u32 mode) {
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
  BattleFx_BeginCanvasLayer(0);

  // Bring out the djinni, Mars (entrance mode 2) for mode 6 and Mercury (1)
  // otherwise; origin receives where it ends. Modes 0 and 6 place it over
  // the first target (anchor 0), the others at the first target's x at
  // depth 0 (anchor 1).
  if (work->effect->prelude == 1) {
    s32 kind = mode == 6 ? 2 : 1;

    if (mode == 6 || mode == 0)
      BattleFx_RunDjinniEntrance(effect, kind, work->effect->side, 0, &originX,
                                 &originY);
    else
      BattleFx_RunDjinniEntrance(effect, kind, work->effect->side, 1, &originX,
                                 &originY);
    work->effect->variant = 0;
  }

  // Mode 0 shows the canvas at its own width, shifted so the first target
  // is at its centre (64); the others keep it at double width.
  if (mode == 0) {
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
  if (mode <= 1 || mode == 3 || mode == 4 || mode == 5) {
    if (work->effect->variant == 0)
      resource = Resource_GetTableEntry((s32)&ResourceId_IceShardSheet);
    else
      resource = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
  } else if (mode == 6) {
    resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    resource = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  }
  Iwram_CopyWords((void *)BG_PLTT, resource, 128);

  // The djinni: 40x40 at the canvas's own width, the small 20x40 picture at
  // double width.
  if (doubleWidth == 0) {
    if (mode == 6)
      resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      resource = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
  } else {
    if (mode == 6)
      resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet);
    else
      resource =
          Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet);
  }
  resource += 128;
  Resource_DecodeType01(resource, work->sheet + HEALING_DJINNI_CELLS);
  BattleFx_FetchRectangleBlitters(work->effect->side,
                                  draw = blit); // [0] maximum, [1] additive

  // Scatter the motes around the centre (16.16, relative to the point the
  // frame loop centres on). Modes 3 to 5 end 16 frames sooner than the
  // others, as their motes go out rather than travel in.
  if (mode == 0 || mode == 6) {
    // x and z within 64 pixels, y 32 to 95 pixels up.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0x7f) + 64) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * HEALING_STAGGER + 88;
  } else if (mode == 1) {
    // All three within 64 pixels.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * HEALING_STAGGER + 88;
  } else if (mode == 2) {
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
  } else if (mode == 3) {
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

  // Motes for each target: 32 for variant 0, 128 for variant 2, else 64.
  motesPerTarget = 64;
  if (work->effect->variant == 0)
    motesPerTarget = 32;
  else if (work->effect->variant == 2)
    motesPerTarget = 128;
  // Each transfer keeps 75% of every canvas pixel, so the motes leave
  // trails.
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
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
      // circle a frame. Mode 2 offsets the yaw by 40 frames a target where
      // the others offset it by their start, and mode 1 does not offset it.
      if (frame > start) {
        if (mode == 0 || mode == 6) {
          SceneTransform_ApplyYaw((frame - member * HEALING_STAGGER) << 9);
        } else if (mode == 1) {
          SceneTransform_ApplyPitch(frame << 9);
          SceneTransform_ApplyRoll(frame << 9);
        } else if (mode == 2) {
          SceneTransform_ApplyYaw((frame - member * 40) << 9);
        } else if (mode == 3) {
          SceneTransform_ApplyYaw((frame - member * HEALING_STAGGER) << 9);
        } else {
          SceneTransform_ApplyYaw((frame - member * HEALING_STAGGER) << 9);
          SceneTransform_ApplyPitch((frame - member * HEALING_STAGGER) << 9);
        }

        // Four motes appear each frame; in modes 3, 4 and 5 mote i goes out at
        // frame start + i / 2 + 32.
        for (i = 0; i != motesPerTarget; i++) {
          struct EffectStep *spark = &PARTICLES[member * HEALING_SLOTS + i];
          s32 endFrame;

          if (mode == 3 || mode == 4 || mode == 5)
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
              if (mode == 0)
                screen.x += canvasShiftX;
              else
                screen.x >>= 1;
              screen.y += 16;

              // Size 3 to 1 over depths 314 to 634 (mode 0 cycles through 9
              // sizes).
              CLAMP(screen.depth, 314, 634);
              size = 3 - (screen.depth - 314) / 128;
              if (mode == 0)
                size = (i * 4 + frame) % 9;
              if (mode == 0 || mode == 3 || mode == 4 || mode == 5) {
                u8 *cell = work->sheet + gBattleFx_PuffCells[size];
                u32 edge = gBattleFx_PuffSizes[size];
                u32 half = edge >> 1;

                draw[1](canvas, cell, screen.x - half, screen.y - half, edge,
                        edge);
              } else {
                EFFECT_DRAW_MOTE(draw[1], canvas, sprites, screen.x, screen.y,
                                 size);
              }

              // Modes 0 to 2 and 6 draw the mote in; 3 to 5 leave it where
              // it is until it goes out.
              if (mode <= 2 || mode == 6) {
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
