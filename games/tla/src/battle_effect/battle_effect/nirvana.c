/// @file
/// Nirvana, Stone Justice, Soul Shatter and Serac.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

// Nirvana's ring of sparks.

extern char ResourceId_Data134;
extern char ResourceId_PinkBurstSheet;
// The Broken Seal's gBattleEffect_MoteCells: spark cells of the
// Data134 sheet, by size - 1.
extern u16 gBattleEffect_MoteCells[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 blitter); // BattleEffect_BuildBlitter, packed
void BattlePlayback_StartFar(s32 phase);
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 group, s32 timerSlot,
                              s32 frames);
void Transform_Pop(void);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void Transform_ApplyPitch(s32);
void Transform_ApplyRoll(s32 angle);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffect_CountDownBattlerTints(void);

/// A mote leaves the first target and is drawn as a ring of 24 sparks
/// turning about it while it is pulled back to the user.
void BattleEffect_RunNirvana(struct BattleEffectArgument *effect) {
  struct BattleFxWork *work;
  void *canvas;
  DrawRectangle blit;
  struct BattleCamera *view;
  u8 *sheet;
  struct MotionObject *object;
  struct EffectStep *orb;
  s32 half;
  s32 frame;
  s32 i;

  work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  view = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet),
                       128);
  object = BattleObject_GetFar(effect->actors[0])->object;
  half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
  orb = work->particles;
  orb->x = object->x;
  orb->y = object->y + half;
  orb->z = object->z;
  orb->velocityX = 0;
  orb->velocityY = 0;
  orb->velocityZ = 0;
  orb->variant = 0;
  BattleEffect_LoadBlitter(104, 19);
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != 80; frame++) {
    struct MotionObject *user = BattleObject_GetFar(effect->actor)->object;
    s32 userHalf = BattleUnit_GetEffectHeightFar(effect->actor) / 2;

    if (frame == 64)
      BattlePlayback_StartFar(134);
    Transform_Reset();
    Transform_SetLookAt(view->eye, view->pos);
    if (frame == 72)
      BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
    if (frame == 79)
      BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);
    if (frame == 0)
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 42);
    if (orb->variant == 0) {
      s32 size = 6;

      for (i = 0; i != 24; i++) {
        s32 point[3];
        struct EffectPosition out;

        Transform_Push();
        SceneTransform_ApplyPosition3(orb->x, orb->y, orb->z);
        if (i <= 11)
          Transform_ApplyPitch(frame << 9);
        else
          Transform_ApplyPitch((frame << 9) + TRIG_TURN / 4);
        Transform_ApplyRoll(i * 5461 + (frame << 10));
        SceneTransform_ApplyPosition3(0, Trig_Sin(frame * 409) << 6, 0);
        point[0] = 0;
        point[1] = 0;
        point[2] = 0;
        EffectPosition_ProjectPoint(point, &out);
        out.x >>= 1;
        Transform_Pop();
        blit(canvas, sheet + gBattleEffect_MoteCells[size - 1],
             out.x - size / 2, out.y - size, size, size * 2);
      }
      BattleEffectKernels_IntegrateVector3(orb, 62, 0);
      if (frame > 0) {
        s32 dx = (user->x - orb->x) >> 10;
        s32 dy = (user->y + userHalf - orb->y) >> 10;
        s32 dz = (user->z - orb->z) >> 10;
        s32 ox;
        s32 ux;

        orb->velocityX += dx;
        orb->velocityY += dy;
        orb->velocityZ += dz;
        ux = user->x;
        if (ux < 0)
          ux = -ux;
        ox = orb->x;
        if (ox < 0)
          ox = -ox;
        if (ux < ox)
          orb->variant = -1;
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

// The pixel half of a 16.16 fixed-point value.
static inline s16 *Fixed_Halves(s32 *value) {
  /* FAKEMATCH(wrapper): the halves read through this inline. */
  return (s16 *)value;
}
#define HI(v) (Fixed_Halves(&(v))[1])

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define ROCKS 21

extern char ResourceId_BoulderSheet;

// The Broken Seal's table of this name, ShardHeights and ShardCells.
extern u8 gSpire_ShardWidths[];
extern u8 gData_081974ad[];
extern u16 gSpire_ShardCells[];

void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_StartActorMotion(s32 actor, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// Stone Justice: 21 boulders rise in a column beside the user and tumble
// across the screen into the target, while the battle background slides
// with them.
//
// Setup: canvas layer 1 at full width, the boulder sheet, one transparent
// blitter, and the user's sprite group set apart (48) for the length of
// the effect.
// Frame loop (107 frames): the boulders are thrown at frame 0 and spin as
// they fly; from frame 64 the background scrolls after them and each
// boulder turns toward the target; every boulder that reaches the target
// shakes the screen and knocks the target back.
void BattleEffect_RunStoneJustice(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 scroll;
  s32 scrollSpeed;
  s32 cueTimer;
  struct MotionObject *user;
  s32 scrollX;
  struct EffectPosition userPosition;
  struct EffectPosition targetPosition;
  struct EffectStep *rock;
  s32 frame;
  s32 hit;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(
      104, BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  scroll = 0;
  scrollSpeed = 0;
  cueTimer = 0;
  user = BattleObject_GetFar(effect->actor)->object;
  Actor_SetAnimationSpeedFar(user, 48);

  for (frame = 0; frame != 107; frame++) {
    EffectPosition_ProjectScaledBattler(effect->actor, &userPosition);
    EffectPosition_ProjectScaledBattler(effect->actors[0], &targetPosition);
    if (frame == 0)
      Sound_PlayFar(SE_GATE_MOVE);
    if (frame == 64)
      Sound_PlayFar(SE_104);

    // Frame 0: the boulders stand in a column 2 pixels apart, on the
    // user's side of the canvas, and are thrown upward at random speeds.
    if (frame == 0) {
      for (i = 0; i != ROCKS; i++) {
        rock = &work->particles[i];
        if (effect->side == 0)
          rock->x = (Random16() % 40 + 8) << 16;
        else
          rock->x = (120 - Random16() % 40) << 16;
        Random16();
        rock->y = (i * 2 + 96) << 16;
        rock->velocityX = 0;
        rock->velocityY = -((Random16() & 127) + 32) << 11;
        rock->z = (Random16() & 15) + 8;
        rock->velocityZ = 0;
        rock->variant = 0;
      }
    }
    if (frame == 52)
      Actor_SelectAnimationFar(user, 2);

    // From frame 64 the background follows the boulders, ever faster.
    if (frame > 64) {
      if (effect->side == 0)
        scroll += scrollSpeed;
      else
        scroll -= scrollSpeed;
      scrollSpeed += 0x8000;
      scrollSpeed = scrollSpeed * 60 / 64;
    }
    scrollX = scroll >> 16;
    REG_BG2X = (64 - (userPosition.x - scrollX)) << 8;

    // Each boulder spins through three pictures of its shard (z is its
    // spin rate, velocityZ the angle) and hits the target once it passes
    // it.
    hit = 0;
    for (i = 0; i != ROCKS; i++) {
      s32 cell;
      u32 width;

      rock = &work->particles[i];
      cell = (i % 5) * 3 + rock->velocityZ / 96 % 3;
      width = gSpire_ShardWidths[cell];

      blit(canvas, work->sheet + gSpire_ShardCells[cell] + 0x83c,
           HI(rock->x) - width / 2 + scrollX,
           HI(rock->y) - gData_081974ad[cell] / 2, width, gData_081974ad[cell]);
      if (rock->variant == 0) {
        if (effect->side == 0) {
          if (targetPosition.x > HI(rock->x) + userPosition.x - 64) {
            rock->variant = 1;
            hit = 1;
          }
        } else {
          if (targetPosition.x < HI(rock->x) + userPosition.x - 64) {
            rock->variant = 1;
            hit = 1;
          }
        }
      }
      BattleEffectKernels_IntegrateVector2(rock, 60, 0);
      rock->velocityZ += rock->z;
      if (frame > i / 2 + 64) {
        if (effect->side == 0)
          rock->velocityX -= px(1) / 2;
        else
          rock->velocityX += px(1) / 2;
        if (rock->z > 1)
          rock->z--;
      }
    }

    // A hit flashes and pushes the target; the impact sound plays at most
    // every eight frames.
    if (cueTimer > 0)
      cueTimer--;
    if (hit == 1) {
      if (cueTimer == 0) {
        cueTimer = 8;
        Sound_PlayFar(SE_HEAVY_IMPACT);
      }
      BattleEffect_StartActorMotion(effect->actors[0], 1, 2 << 16, 4 << 16,
                                    2 << 16, 150);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
      work->shakeFrames = 2;
    }

    BattleCamera_Shake(2, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Actor_SetAnimationSpeedFar(user, 16);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#undef HI

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

#define SPARK_OFFSET 0x460
#define SPARKS 16
#define MOTES 32

extern char ResourceId_LimeColumnSheet;
extern char ResourceId_BlastSheet;

extern char ResourceId_OrangePaletteB;

// The Broken Seal's gEruption_PuffCells and gCybele_EmberSizes:
// the spark pictures by stage.
extern u16 Data_08198b5e[];
extern u16 Data_08198b6c[];

s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags,
                              u32 mode); // BattleEffect_BuildBlitter
void BattleEffect_FetchRectangleBlitters(
    s32 flip, void **out); // ..FetchRectangleBlitters

void EffectPosition_ProjectBattler(s32 actor, struct EffectPosition *position);

// Soul Shatter: a column bursts up under the target in a spray of sparks
// and throws it into the air; the user glows, and from frame 32 motes of
// the target's soul fly out and are drawn back into the user.
//
// Setup: canvas layer 1 at a light blend, centred on the target; the
// column, the spark pictures after it, the mote sheet and the orange
// palette.
// Frame loop (106 frames): the column rises from frame 8, 12 pixels a
// frame, stands at 80 pixels (drawn with max blitters) and sinks from
// frame 80; the target is thrown up at frame 10 and comes down at frame
// 80.
/// Soul Shatter, The Broken Seal's BattleEffect_RunUndeadSword as The Lost Age
/// reworked it: the same two mirrored halves of a column and sixteen sparks,
/// over a longer effect in which the target is thrown up and 32 motes of its
/// soul drift back into the user.
void BattleEffect_RunSoulShatter(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  struct MotionObject *target;
  DrawRectangle draw[2];
  struct EffectPosition position;
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x0c10;
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + SPARK_OFFSET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       128);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  target = BattleObject_GetFar(effect->actors[0])->object;
  EffectPosition_ProjectBattler(effect->actors[0], &position);
  position.x /= 2;
  work->shakeFrames = 8;

  for (frame = 0; frame != 106; frame++) {
    // Frame 0: sparks leave the column's foot upward, and the motes start
    // at the target, 52 pixels up, in every direction.
    if (frame == 0) {
      target->y = 0;
      {
        struct EffectStep *spark;

        for (i = 0, spark = work->particles; i != SPARKS; i++, spark++) {
          s32 speed = (Random16() & 0xff) + 256;
          s32 angle = (Random16() & 0x7fff) + TRIG_TURN / 4;

          spark->x = position.x << 16;
          spark->y = (96 - (Random16() & 31)) << 16;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
          spark->variant = Random16() & 7;
        }
      }
      {
        struct EffectStep *mote;

        for (i = 0, mote = &work->particles[SPARKS * 2]; i != MOTES;
             i++, mote++) {
          s32 speed = (Random16() & 127) + 128;
          s32 angle = Random16() & 0x7fff;

          mote->x = target->x;
          mote->y = 52 << 16;
          mote->z = target->z;
          mote->velocityY = ((Random16() & 0xff) - 128) << 9;
          mote->velocityX = (Trig_Sin(angle) * speed) >> 7;
          mote->velocityZ = (Trig_Cos(angle) * speed) >> 7;
          mote->variant = 0;
        }
      }
    }

    // The cues: the burst, the target thrown up 32 pixels, the user's
    // glow, the drain and the target's fall.
    if (frame == 4)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 10) {
      work->shakeFrames = 8;
      Sound_PlayFar(SE_CLATTER);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 80);
      target->verticalMotionStrength = 0;
      target->y = 32 << 16;
    }
    if (frame == 80)
      target->verticalMotionStrength = 0xab85;
    if (frame == 24)
      BattleEffect_TintBattler(effect->actor, -1, 3, -1, 0);
    if (frame == 40)
      Sound_PlayFar(SE_SWIRL);
    if (frame == 32)
      BattlePlayback_StartFar(134);
    if (frame == 64)
      BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
    if (frame == 88)
      BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);

    // The column: two 14-pixel halves, the right one mirrored.
    if (frame > 7) {
      s32 centre = position.x;
      s32 height;

      if (frame <= 79)
        height = frame * 12 - 96;
      else
        height = 560 - frame * 6;
      if (height > 0) {
        u32 mode;
        s32 x;

        if (height > 80) {
          height = 80;
          mode = BLIT_MAX;
        } else {
          mode = BLIT_ADDITIVE;
        }
        for (i = 0, x = centre - 14; i != 2; i++, x += 14) {
          if (i == 0)
            BattleEffect_BuildBlitter(
                104, 7, 7, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
          else
            BattleEffect_BuildBlitter(
                104, 7, 7,
                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, mode);
          draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
          draw[0](canvas, work->sheet, x, 104 - height, 14, height);
          HeapSlot_Release(104);
        }
      }
    }

    // The sparks: from frame 8, two more each frame, through seven
    // pictures three frames apiece; one in four is drawn mirrored.
    BattleEffect_FetchRectangleBlitters(effect->side, draw);
    {
      struct EffectStep *spark;

      for (i = 0, spark = work->particles; i != SPARKS; i++, spark++) {
        if (frame >= i / 2 + 8 && spark->variant <= 28) {
          s32 cell = spark->variant / 3;
          s32 x = HI(spark->x);
          s32 y = HI(spark->y);

          if (cell > 6)
            cell = 6;
          draw[(i & 3) ? 0 : 1](
              canvas, work->sheet + Data_08198b5e[cell] + SPARK_OFFSET,
              x - Data_08198b6c[cell] / 2, y - Data_08198b6c[cell] / 2,
              Data_08198b6c[cell], Data_08198b6c[cell]);
          spark->variant++;
          BattleEffectKernels_IntegrateVector2(spark, 62, -0x2000);
        }
      }
    }

    // From frame 32 the motes fly; mote i turns toward the user's middle
    // from frame 32 + i and vanishes once it passes the user.
    if (frame > 31) {
      struct MotionObject *user;
      s32 userHalfHeight;
      struct BattleCamera *camera;

      user = BattleObject_GetFar(effect->actor)->object;
      userHalfHeight = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
      camera = Ram_HeapSlots->cameraWork;
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      for (i = 0; i != MOTES; i++) {
        struct EffectStep *mote = &work->particles[SPARKS * 2 + i];
        if (mote->variant >= 0) {
          s32 size = (i & 1) + 9;
          s32 userX;
          s32 sameSide;
          s32 moteX;

          EffectPosition_ProjectPoint(&mote->x, &screen);
          draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  (screen.x >> 1) - size / 2, screen.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(mote, 62, 0);
          if (frame > i + 32) {
            s32 dx = (user->x - mote->x) >> 7;
            s32 dy = (user->y + userHalfHeight - mote->y) >> 7;
            s32 dz = (user->z - mote->z) >> 7;

            mote->velocityX += dx;
            mote->velocityY += dy;
            mote->velocityZ += dz;
            moteX = mote->x;
            if (moteX < 0)
              moteX = -moteX;
            // A once-only block around the read of user->x.
            do {
              userX = user->x;
            } while (0);
            if (userX < 0)
              userX = -userX;
            // Gone once it has passed the user on the user's side of x = 0.
            /* FAKEMATCH(dead): each arm stores its test in sameSide, never
             * read. */
            if (moteX >= userX && (user->x < 0 ? (sameSide = mote->x < 0)
                                               : (sameSide = mote->x >= 0)))
              mote->variant = -1;
          }
        }
      }
    }

    HeapSlot_Release(188);
    HeapSlot_Release(104);
    if (frame <= 7)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

struct FxPartyList;
struct Model;
struct ModelVertex;
struct ProjectedVertex;
struct StripQuad;

/* VIEW(f16d130): tag struct ModelTexture as struct ModelTexture__f16d130;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, BattleEffect_RunSerac moves: 1 of 1094 instructions in
 * tla-ja. */
struct ModelTexture__f16d130 {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const u8 *pixels;
};

/* VIEW(f16d130): tag struct RenderModel as struct RenderModel__f16d130;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, BattleEffect_RunSerac moves: 1 of 1094 instructions in
 * tla-ja. */
struct RenderModel__f16d130 {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture__f16d130 *texture;
  const void *shade;
  u8 scrollU;
  u8 scrollV;
};

void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList__ability_effects(struct Model *model);

void SceneTransform_ApplyUniformScale(s32 scale);
void Transform_ApplyYaw(s32 angle);

void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets

void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);

void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

struct ProjectedPoint {
  s32 x, y, depth;
};

void Render_ProjectVertices__ability_effects(
    struct ModelVertex *vertices, struct ProjectedVertex *screen,
    s32 count) __asm__("Render_ProjectVertices");
#define Render_ProjectVertices Render_ProjectVertices__ability_effects

extern char ResourceId_Data0C1;
extern char ResourceId_DustBlobSheet;
extern char ResourceId_Data0CF;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_BlueFlameSheet;

// One model vertex: signed byte coordinates, padded to a word.

// The cone's 64 vertices, stored at the buffer's constant address.
extern u8 gMapCellBuffer[];
#define VERTICES ((struct ModelVertex *)Ram_MapCellBuffer)
extern u8 gData_02014000[];
extern u8 Data_02012000[];
#define MESH_BUFFER (Ram_MapCellBuffer + 0x2000)
extern u8 Data_02012540[];       // the mesh's end, after 112 faces
extern const s8 Data_08198b7a[]; // the cone's ring heights
extern const s8 Data_08198b82[]; // and radii
extern const u8 Data_08198b8a[]; // each ripple's delay
extern const u8 Data_08198b8e[]; // its growth
extern const s8 Data_08198b92[]; // its height
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
extern const u8 Data_0819919c[];
extern const u8 Data_081991a4[];

// Abilities: Serac.
//
// The Mercury Djinni drops onto the target, dust bursting where it lands;
// a spire of ice (a cone of 8 rings of 8 vertices, its mesh built on the
// first frame) grows up around the target in rings of mist while the
// camera circles, the caster closes in at frame 56, and at 64 the spire
// shatters into 64 tumbling shards.
void BattleEffect_RunSerac(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle blitters[2];
  s32 screenB[3];
  s32 screenA[3];
  u8 *mistSheet;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_ConfigureEffectDisplay();
  if (effect->side == 0) {
    BattleEffect_StartTiledCanvasFilled(0x2000);
  } else {
    BattleEffect_StartTiledCanvas(0x2000);
  }
  BattleEffect_RunDjinniEntrance(effect, (effect->side << 4) | 0x101, screenA,
                                 screenB);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), 0x3c00);
  Iwram_ClearWordsEntry(canvas, 0x3c00);
  REG_BLDALPHA = 0x1010;
  REG_BG2PA = 0x100;
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  mistSheet = gData_02014000;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, mistSheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CF, work->sheet + 0x400, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                             work->sheet + 0x4000, 1, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                       128);
  // A block of its own: camera, savedYaw and target take their stack slots
  // after the blitter pair's address.
  {
    struct BattleCamera *camera;
    s16 savedYaw;
    struct MotionObject *target;
    struct ProjectedPoint screen;
    s32 world[3];
    struct ProjectedPoint point;
    struct ProjectedPoint dropScreen;
    struct ModelTexture__f16d130 texture;
    struct RenderModel__f16d130 *model;
    void *vertices;
    u8 color[1];
    s32 savedScrollX;
    s32 savedScrollY;
    s32 row;
    s32 column;
    s32 i;

    camera = (struct BattleCamera *)Ram_HeapSlots->cameraWork;
    savedYaw = camera->yaw;
    target = BattleObject_GetFar(effect->actors[0])->object;
    REG_BG2PA = 0x100;
    REG_BG2X = 0;

    for (frame = 0; frame != 88; frame++) {
      if (frame == 0) {
        camera->yaw = savedYaw;
        for (row = 0; row != 8; row++) {
          for (column = 0; column != 8; column++) {
            s32 height;
            s32 radius;
            s32 angle;

            if (row == 0) {
              height = Data_08198b7a[row];
              radius = Data_08198b82[row];
            } else {
              height = Data_08198b7a[row] + (Random16() & 3);
              radius = Data_08198b82[row] + (Random16() & 3);
            }
            angle = (column << 13) + TRIG_TURN / 16;
            VERTICES[row * 8 + column].x = (Trig_Sin(angle) * radius) >> 16;
            VERTICES[row * 8 + column].y = -height;
            VERTICES[row * 8 + column].z = (Trig_Cos(angle) * radius) >> 16;
            if (row <= 6) {
              u8 *quad = MESH_BUFFER + (row * 8 + column) * 24;
              u8 *quad2;
              s32 bottom = row * 8 + 7;
              s32 left = column * 16;
              s32 right = left + 15;
              s32 next;
              s32 here;

              quad[4] = right;
              quad[5] = row * 8;
              quad[6] = right;
              quad[9] = bottom;
              quad[7] = bottom;
              quad[8] = left;
              quad2 = MESH_BUFFER + 12 + (row * 8 + column) * 24;
              quad2[4] = left;
              quad2[8] = left;
              quad2[5] = row * 8;
              quad2[9] = bottom;
              quad2[6] = right;
              quad2[7] = row * 8;
              next = (column + 1) % 8 + row * 8;
              quad[0] = next;
              here = column % 8 + row * 8;
              quad[1] = here + 8;
              quad[2] = next + 8;
              quad2[0] = here;
              quad2[1] = here + 8;
              quad2[2] = next;
            }
          }
        }
        Data_02012540[0] = 0;
        Data_02012540[1] = 0;
        Data_02012540[2] = 0;
      }

      // The camera circles the spire.
      if ((u32)(frame - 4) <= 79) {
        s32 turn;

        if (frame <= 63) {
          turn = 320;
        } else {
          turn = 1600 - frame * 20;
        }
        if (effect->side == 0) {
          camera->yaw -= turn;
        } else {
          camera->yaw += turn;
        }
        if (effect->side == 0) {
          if ((s16)camera->yaw < -14000) {
            camera->yaw = -14000;
          }
        } else if ((s16)camera->yaw > 14000) {
          camera->yaw = 14000;
        }
      }

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      savedScrollX = gCameraSceneParameters[3];
      savedScrollY = gCameraSceneParameters[4];
      gCameraSceneParameters[4] = savedScrollY - 16;
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      world[0] = target->x;
      world[1] = 0;
      world[2] = target->z;
      Render_ProjectPoint(world, &screen);
      if (screen.x <= 63) {
        screen.x = 64;
      }
      if (screen.x > 176) {
        screen.x = 176;
      }
      gCameraSceneParameters[3] = 184 - screen.x;
      IO_WRITE_QUEUE_PUSH(0x04000028, (64 - screen.x) << 8, 0x30000);

      // Dust where the Djinni lands.
      if ((u32)frame <= 63) {
        if (frame == 0) {
          for (i = 0; i != 32; i++) {
            struct EffectStep *p = &work->particles[i];
            s32 speed = (Random16() & 1023) + 1024;
            s32 angle = Random16() & 0xffff;

            p->x = target->x;
            p->y = 0;
            p->z = target->z;
            p->velocityX = (Trig_Sin(angle) * speed) >> 9;
            p->velocityY = 0;
            p->velocityZ = (Trig_Cos(angle) * speed) >> 9;
            p->variant = -i;
          }
        }
        for (i = 0; i != 32; i++) {
          struct EffectStep *p = &work->particles[i];
          s32 picture;
          s32 x;
          s32 y;

          if ((u32)++p->variant > 23) {
            continue;
          }
          picture = p->variant / 6;
          Render_ProjectPoint(p, &point);
          BattleEffectKernels_IntegrateVector3(p, 60, 0x2000);
          x = point.x - 16;
          y = point.y - 16;
          blitters[0](canvas, work->sheet + 0x400 + (picture << 10), x, y, 32,
                      32);
        }
      }
      // The Djinni falls.
      if (frame <= 29) {
        point.x = target->x;
        point.y = 0x3c0000 - (frame << 17);
        point.depth = target->z + (Trig_Sin(frame << 9) << 5);
        Render_ProjectPoint(&point, &dropScreen);
        blitters[1](canvas, work->sheet + 0x4000, dropScreen.x - 20,
                    dropScreen.y - 30, 40, 40);
      }
      if (frame == 4) {
        Sound_PlayFar(SE_SWITCH_MECHANISM);
      }
      if (frame == 30) {
        Sound_PlayFar(SE_139);
      }
      if (frame == 60) {
        Sound_PlayFar(SE_POWER_UP);
      }
      if (frame == 64) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        BattlePlayback_StartFar(-1);
      }
      if (frame == 30) {
        BattleEffect_TintBattler(effect->actors[0], 9, 5, -1, 0);
      }
      if (frame == 26) {
        work->shakeFrames = 8;
      }
      if (frame == 64) {
        work->shakeFrames = 4;
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
      }
      if (frame == 70) {
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
      }
      if (frame == 56) {
        // The caster closes four fifths of the way.
        struct Actor *caster =
            (struct Actor *)BattleObject_GetFar(effect->actor)->object;
        struct Actor *to =
            (struct Actor *)BattleObject_GetFar(effect->actors[0])->object;
        s32 x = caster->x;
        s32 z;
        s32 dx = (to->x - x) * 80 / 100;
        s32 dz;
        s32 speed;

        z = caster->z;
        dz = (to->z - z) * 80 / 100;
        x += dx;
        z += dz;
        dx >>= 8;
        dz >>= 8;
        speed = (Iwram_Sqrt(dx * dx + dz * dz) << 8) / 10;
        caster->acceleration = speed;
        caster->speedLimit = speed;
        caster->snapToTarget = 1;
        caster->velocityY = px(6);
        caster->gravity = 0x13333;
        caster->bounce = 0;
        caster->actionFlags = 1;
        Actor_StartIdleFar(caster);
        Actor_SetMoveTargetFar(caster, x, 0, z);
        Actor_SelectAnimationFar(caster, 2);
      }

      if (frame > 63) {
        if (frame == 64) {
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
          for (i = 0; i != 64; i++) {
            struct EffectStep *p = &work->particles[i];
            s32 angle = (i & 7) << 13;
            s32 ring;

            p->x = target->x + ((Trig_Sin(angle) * (ring = i / 8)) << 2);
            p->y = ((Random16() & 0xffff) << 2) - ((ring * 5) << 17) + 0x400000;
            p->z = target->z + ((Trig_Cos(angle) * ring) << 2);
            p->velocityX = ((Random16() & 0xffff) << 1) + 0x10000;
            p->velocityY = ((Random16() & 0xffff) - 12768) * 3;
            p->velocityZ = ((Random16() & 0xffff) - 0x8000) << 1;
            if (p->x < 0) {
              p->velocityX = -p->velocityX;
            }
            p->variant = (Random16() & 31) + 32;
          }
        } else {
          model->kind = 2;
          model->unk04 = 0;
          model->mesh = Data_0819919c;
          model->vertices = vertices;
          for (i = 0; i != 64; i++) {
            struct EffectStep *p = &work->particles[i];
            s32 angle = (frame + i) << 10;

            if (p->variant < 0) {
              continue;
            }
            color[0] = p->variant;
            p->variant -= 2;
            model->shade = color;
            BattleEffectKernels_IntegrateVector3(p, 62, 0x400);
            Transform_Push();
            SceneTransform_ApplyPosition3(p->x, p->y, p->z);
            Transform_ApplyPitch(angle);
            Transform_ApplyRoll(angle);
            SceneTransform_ApplyUniformScale(Q16_ONE);
            Render_ProjectVertices(Data_081991a4, vertices, 3);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
      }
      if (frame < 64) {
        if (frame > 25) {
          s32 scale;
          s32 start;
          // Set before the branch below and stored after it; the second value
          // stays apart from the 7 beside it.
          s32 spireShade;
          const void *mesh = Data_02012000;

          // The spire, then the mist inside it.
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = (const u8 *)work->sheet;
          model->kind = 6;
          model->unk04 = 3;
          model->vertices = vertices;
          model->scrollU = 0;
          model->texture = &texture;
          model->mesh = mesh;
          model->scrollV = -((frame / 2) & 31);
          Transform_Push();
          spireShade = 0;
          if (frame <= 31) {
            scale = frame * 10000 - 260000;
          } else {
            scale = 60000;
          }
          model->shade = (const void *)spireShade;
          SceneTransform_ApplyPosition3(target->x, -0x40000, target->z);
          SceneTransform_ApplyScale3(scale, scale, scale);
          Render_ProjectVertices((const u8 *)gMapCellBuffer, vertices, 64);
          Render_DrawModelList(model);
          spireShade = -32;
          model->shade = (const void *)spireShade;
          model->kind = 7;
          model->scrollU = 0;
          model->scrollV = (frame / 2) & 31;
          model->unk04 = 2;
          Render_DrawModelList(model);
          Transform_Pop();

          // Four rings of mist spread from its foot.
          texture.logWidth = 7;
          texture.logHeight = 7;
          texture.pixels = mistSheet;
          model->kind = 7;
          model->texture = &texture;
          model->mesh = Data_08198ec4;
          model->unk04 = 0;
          model->vertices = vertices;
          model->scrollU = 0;
          model->scrollV = 0;
          for (i = 0; i != 4; i++) {
            s32 size;
            s32 shade;

            start = Data_08198b8a[i] + 22;
            if (frame <= start) {
              continue;
            }
            size = (frame - start) * Data_08198b8e[i] * 1500 + 0x4180;
            shade = (start - frame) * 8 + 56;
            if (shade > 0) {
              shade = 0;
            }
            if (shade <= -64) {
              continue;
            }
            Transform_Push();
            model->shade = (const void *)shade;
            SceneTransform_ApplyPosition3(target->x, Data_08198b92[i] << 16,
                                          target->z);
            SceneTransform_ApplyScale3(size, size, size);
            model->scrollU = ((frame + i * 5) & 7) << 4;
            Render_ProjectVertices(Data_08198cac, vertices, 32);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
      gCameraSceneParameters[3] = savedScrollX;
      gCameraSceneParameters[4] = savedScrollY;
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

void BattleEffect_RunHeavyBlow(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Heavy Divide (kind 101).
void BattleEffect_RunHeavyDivide(s32 a0) { BattleEffect_RunHeavyBlow(a0, 0); }

/// Abilities: Strong Hit (kind 469).
void BattleEffect_RunStrongHit(s32 a0) { BattleEffect_RunHeavyBlow(a0, 1); }
#undef HI
#undef BLITTER
#undef ROCKS
#undef SPARK_OFFSET
#undef SPARKS
#undef MOTES
#undef SLOT_BLITTER
#undef VERTICES
#undef MESH_BUFFER
