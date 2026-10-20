/// @file
/// Healing motes: Ply, Wish, Cure, Cure Poison.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// Healing sparkles.
//
// Healing sparkles come in seven modes. Motes of light scattered around each
// target swirl in towards it, one target every 8 frames, with the recovery
// sound on each target's 20th frame and a tint on its 36th. The modes differ
// in how the motes scatter and turn and in which resource they draw; the
// wrappers name the mode of each effect.
//
// PARTICLES holds the motes: 512 are scattered, and target n draws from the
// 64 at n * 64 (32 of them, or 128 running into the next target's, by the
// effect's variant). Each mote keeps its position relative to the target's
// centre, so swirling the motes is a turn of the whole frame of reference,
// and drawing one in is a step of 2 pixels a frame along the line to the
// centre.

#define HEALING_DJINNI_CELLS 0x1000 ///< the djinni picture in the sheet

/// Ply: sparkles of light swirl around each target and close in on it, in the
/// ice shard's or ice block's colours; the canvas is shown at its own width,
/// centred on the first target.
///
/// Abilities: Ply, Ply Well, Pure Ply, Herb, Nut, Vial, Potion, Soothing Water
/// (kind 22); Fizz (kind 204).
void BattleFx_RunPly(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 0);
}
/// Wish (scattering style 1): healing motes tumble in from all around each
/// target.
///
/// Abilities: Wish, Wish Well, Pure Wish (kind 23); Regenerate (kind 44);
/// Reflect (kind 45); Spritz (kind 206).
void BattleFx_RunWish(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 1);
}
/// Cure (mode 2): a ring of healing motes below each target closes in on it, in
/// the blast sheet's colours.
///
/// Abilities: Cure, Cure Well, Potent Cure, Corn (kind 21); Worms (kind 352).
void BattleFx_RunCure(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 2);
}
/// Cure Poison: sparkles twinkle around each target, four more a frame, and go
/// out one by one instead of closing in.
///
/// Abilities: Cure Poison, Restore, Antidote, Elixir (kind 24).
void BattleFx_RunCurePoison(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunHealingMotes(effect, 3);
    return;
  }
  BattleFx_RunHealingMotes(effect, 4);
}

/// Healing sparkles with scattering style 4, sent out from each target, as
/// Cure Poison plays them for variants other than 0. No effect table entry
/// reaches this one.
UNUSED void BattleFx_RunHealingMotesMode4(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 4);
}
/// Tonic (scattering style 5): sparkles twinkle around each target, turning on
/// two axes, and go out one by one; mode 5 plays as mode 4 does.
///
/// Abilities: Tonic (kind 208).
void BattleFx_RunTonic(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 5);
}
/// Psy Crystal: healing motes gather around each target in the Mars Djinni
/// palette; with a prelude, a Mars Djinni appears first.
///
/// Abilities: Psy Crystal, Adept Ring, Ember (kind 212).
void BattleFx_RunPsyCrystal(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 6);
}
#define HEALING_MOTES 512 ///< motes in PARTICLES
#define HEALING_SLOTS 64  ///< particles reserved for each target

/// Healing sparkles: motes of light scattered around each target swirl in
/// towards it, one target every 8 frames, with the recovery sound on each
/// target's 20th frame and a tint on its 36th.
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

  // Bring out the djinni; origin receives where it appears.
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

  // Pictures and resource.
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

  // Scatter the motes around the centre.
  if (mode == 0 || mode == 6) {
    // x and z within 64 pixels, y 32 to 95 pixels up.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0x7f) + 64) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 88;
  } else if (mode == 1) {
    // All three within 64 pixels.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 88;
  } else if (mode == 2) {
    // A ring of radius 32 to 95 pixels, 50 pixels down.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      s32 heading = Random16() & 0xffff;
      s32 radius = (Random16() & 63) + 32;

      SET_POSITION(spark, Trig_Sin(heading) * radius, -50 << 16,
                   Trig_Cos(heading) * radius);
      spark->velocityY = ((Random16() & 31) + 32) << 13;
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 88;
  } else if (mode == 3) {
    // x and z within 64 pixels, y within 32.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 14,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 72;
  } else {
    // All three within 64 pixels.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 72;
  }

  // Motes for each target: 32 for variant 0, 128 for variant 2, else 64.
  motesPerTarget = 64;
  if (work->effect->variant == 0)
    motesPerTarget = 32;
  else if (work->effect->variant == 2)
    motesPerTarget = 128;
  work->transferMode =
      CANVAS_COPY_AND_FADE; // each transfer fades the canvas to 3/4, for trails
  work->transferValue = 75; // percent
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = gCameraWork;

    if (frame == 40)
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
      s32 start = member * 8;

      // Centre on the point 40 pixels above the target.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, object->x, 40 << 16, object->z);
      SceneTransform_ApplyPosition(point);
      if (frame == start + 20)
        AudioCommand_PlayFar(SE_RECOVERY);
      if (frame == start + 36)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, member, 28);
      if (frame > start) {
        if (mode == 0 || mode == 6) {
          SceneTransform_ApplyYaw((frame - member * 8) << 9);
        } else if (mode == 1) {
          SceneTransform_ApplyPitch(frame << 9);
          SceneTransform_ApplyRoll(frame << 9);
        } else if (mode == 2) {
          SceneTransform_ApplyYaw((frame - member * 40) << 9);
        } else if (mode == 3) {
          SceneTransform_ApplyYaw((frame - member * 8) << 9);
        } else {
          SceneTransform_ApplyYaw((frame - member * 8) << 9);
          SceneTransform_ApplyPitch((frame - member * 8) << 9);
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

              // Move the mote 2 pixels a frame towards the centre.
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
