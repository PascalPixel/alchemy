/// @file
/// Death Plunge.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer task runs at this priority, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// A texture for the model kernels: its size as powers of two, then its
// pixels.
/* VIEW(f188714): tag struct ModelTexture as struct ModelTexture__f188714;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunDeathPlunge moves: 1 of 632 instructions in
 * tla-ja. */
struct ModelTexture__f188714 {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

/* VIEW(f188714): tag struct Model as struct Model__f188714; ability_effects.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect/death_plunge.c:249: structure has
 * no member named `vertices'. */
struct Model__f188714 {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture__f188714 *texture;
  s32 alpha;
  s32 tail;
};

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C2;
extern char ResourceId_EffectPalette02;

// Triple Chomp's jaw quad: its mesh and its four corners.
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];

s32 BattleUnit_GetEffectHeightFar(s32 objectId);
void BattleEffect_StartCanvasLayer(s32 bgControl);
s32 BattleEffect_LoadBlitter(s32 slot, s32 packed);

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 group, s32 timerSlot,
                              s32 frames);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
struct PoolEntry *Render_AllocateModelList(s32 count);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void Transform_ApplyRoll(s32 angle);
void Transform_ApplyPitch(s32 angle);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void BattlePlayback_StartFar(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_EndCanvasLayer(void);

// Death Plunge (effect kind 53): the user runs nine tenths of the way to the
// target, and both leap; at frame 11 they turn upside down, lifted by their
// own heights. At frame 54 the target flashes and both are thrown back, the
// user to his place, and at frame 64 they land the right way up.
// Variant 0 is ☀️'s: a large flash at the user for two frames, then 16
// flashes that spread in a rising ring for twelve frames.
// Variant 1 is ⚓️'s own, in the colours of effect palette 2:
// 32 sparks burst from the target at frame 56 (of which the first 16 are
// drawn), and Triple Chomp's jaw closes on the target twice, at 45 and 135
// degrees, growing and fading from frame 56.
void BattleEffect_RunDeathPlunge(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  struct BattleObj *first;
  struct BattleObj *second;
  struct Actor *user;
  struct Actor *target;
  s32 percent;
  s32 x;
  s32 z;
  s32 userHeight;
  s32 targetHeight;
  struct EffectPosition targetScreen;
  s32 point[3];
  struct EffectPosition screen;
  struct ModelTexture__f188714 texture;
  struct EffectStep *spark;
  struct Model__f188714 *model;
  void *vertices;
  s32 i;
  u8 *pixels; // The jaw's texture, held for the whole effect.

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up the canvas, the flash sheet, the jaw's texture and a clipped MAX
  // blitter.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  pixels = Ram_MapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, pixels, 0, 0);
  if (effect->variant == 1) {
    Iwram_CopyWordsEntry(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_EffectPalette02), 128);
    work->transferValue = 50;
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_MAX));
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_MAX));
  }
  work->transferMode = 2;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The user runs nine tenths of the way to the target.
  first = BattleObject_GetFar(effect->actor);
  second = BattleObject_GetFar(effect->actors[0]);
  user = first->object;
  target = second->object;
  percent = 90;
  x = user->x + percent * (target->x - user->x) / 100;
  z = user->z + percent * (target->z - user->z) / 100;
  userHeight = BattleUnit_GetEffectHeightFar(effect->actor);
  targetHeight = BattleUnit_GetEffectHeightFar(effect->actors[0]);
  EffectPosition_ProjectBattler(effect->actors[0], &targetScreen);
  Actor_StartIdleFar(user);
  Actor_SetMoveTargetFar(user, x, 0, z);
  Actor_SelectAnimationFar(user, 2);
  user->snapToTarget = 1;
  user->actionFlags = 1;
  user->acceleration = px(2);
  user->speedLimit = px(8);
  wait(20);

  for (frame = 0; frame != 96; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Frame 0: both leap. Frame 11: both turn upside down.
    if (frame == 0) {
      target->velocityY = px(15);
      target->gravity = 0x91eb;
      user->velocityY = px(15);
      user->gravity = 0x91eb;
    }
    if (frame == 11) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y += userHeight;
      target->y += targetHeight;
    }

    // Frame 54: the hit. The target flashes and both are thrown back, the
    // user to where he started.
    if (frame == 54) {
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 10);
      target->velocityY = px(8);
      target->gravity = 0xab85;
      user->velocityY = px(5);
      user->gravity = 0x7851;
      user->acceleration = px(1);
      user->speedLimit = px(2);
      user->actionFlags = 0;
      Actor_StartIdleFar(user);
      Actor_SetMoveTargetFar(user, 0, 0, user->z);
    }

    if (effect->variant == 0) {
      // The flash, and the ring of flashes, at the user.
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = user->x;
      point[1] = user->y;
      point[2] = user->z;
      EffectPosition_ProjectPoint(point, &screen);
      screen.x >>= 1;
      if (frame == 54 || frame == 55)
        draw[0](canvas, work->sheet, screen.x - 16, screen.y - 16, 32, 64);
      if (frame >= 56 && frame <= 67) {
        for (i = 0; i != 16; i++) {
          s32 angle = i << 12;
          s32 rx = screen.x + ((Trig_Sin(angle) * (frame - 46) * 2) >> 16);
          s32 ry = ((Trig_Cos(angle) * (frame - 46) * 2) >> 16) - frame + 100;

          draw[0](canvas, work->sheet + (((frame - 56) / 2) << 11), rx - 16, ry,
                  32, 64);
        }
      }
    } else if (frame > 55) {
      // Frame 56: 32 sparks burst from the target, each a quarter frame
      // later than the one before.
      if (frame == 56) {
        spark = work->particles;
        for (i = 0; i != 32; i++) {
          s32 angle;
          s32 force;

          spark->x = (targetScreen.x / 2) << 16;
          spark->y = (targetScreen.y - 16) << 16;
          angle = Random16();
          force = (Random16() & 255) + 64;
          spark->velocityX = (Trig_Sin(angle) * force) >> 6;
          spark->velocityY = (Trig_Cos(angle) * force) >> 5;
          spark->variant = -i / 4;
          spark++;
        }
      }

      // Each spark plays the flash's 6 cells over 24 frames as it flies up.
      for (i = 0; i != 16; i++) {
        spark = &work->particles[i];
        if (spark->variant >= 0 && spark->variant <= 23) {
          draw[0](canvas, work->sheet + ((spark->variant / 4) << 11),
                  HI(spark->x) - 16, HI(spark->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(spark, 62, -0x2000);
        }
        spark->variant++;
      }
    }

    if (effect->variant == 1) {
      // The jaw, twice, a quarter turn apart: it grows from frame 56 and
      // fades out from frame 64.
      vertices = Heap_AllocEwram(512);
      model = Render_AllocateModelList(1);
      if (frame > 55) {
        s32 scale = ((frame - 56) << 13) + Q16_ONE / 4;
        s32 alpha = 0;

        if (frame > 63)
          alpha = (64 - frame) * 4;
        if (alpha > -64) {
          s32 angle;

          texture.logWidth = 7;
          texture.logHeight = 7;
          model->kind = 7;
          model->texture = &texture;
          model->mesh = gData_08199364;
          model->vertices = vertices;
          texture.pixels = pixels;
          model->alpha = alpha;
          for (i = 0, angle = TRIG_TURN / 8; i != 2;
               angle += TRIG_TURN / 4, i++) {
            Transform_Reset();
            SceneTransform_ApplyPosition3((targetScreen.x / 2 - 64) << 16,
                                          px(32), 0);
            SceneTransform_ApplyScale3(scale, scale, scale);
            Transform_ApplyRoll(angle);
            Transform_ApplyPitch(-TRIG_TURN / 16);
            Render_TransformVertices(gData_08199210, vertices, 4);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }

    // Frame 64: both land the right way up.
    if (frame == 64) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y -= userHeight;
      target->y -= targetHeight;
      Actor_SelectAnimationFar(user, 0);
    }
    if (frame == 54) {
      if (effect->variant == 0)
        BattlePlayback_StartFar(134);
      else
        BattlePlayback_StartFar(145);
    }
    if (frame == 0) {
      Sound_PlayFar(SE_WHOOSH);
      // A once-only block: it keeps the shake after the sound.
      do {
        work->shakeFrames = 6;
      } while (0);
    }
    if (frame == 53)
      work->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
