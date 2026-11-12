#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "actor.h"
#include "task.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer task runs at this priority, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// A texture for the model kernels: its size as powers of two, then its
// pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

// One entry of a model list, as Render_AllocateModelList allocates them and
// Render_DrawModelList draws them.
struct Model {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 alpha;
  s32 tail;
};

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C2;
extern char ResourceId_EffectPalette02;

// Triple Chomp's jaw quad: its mesh and its four corners.
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];

struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
s32 BattleUnit_GetEffectHeightFar(s32 objectId);
void BattleEffect_StartCanvasLayer(s32 bgControl);
void *Resource_GetTableEntry(s32 id);
s32 BattleEffect_LoadBlitter(s32 slot, s32 packed);

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void Render_ResetTransformState(void);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void *Heap_AllocEwram(s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_080150e4(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Heap_Release(void *block);
void Func_081180e8(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void wait(s32 frames);
void HeapSlot_Release(s32 slot);
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
  struct ModelTexture texture;
  struct EffectStep *spark;
  struct Model *model;
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
    Iwram_CopyWords((void *)0x05000000,
                    Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                    128);
    work->transferValue = 50;
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_MAX));
    REG_BLDALPHA = 0x1010;
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
  first = GetBattleObjectSlotFar(effect->actor);
  second = GetBattleObjectSlotFar(effect->actors[0]);
  user = first->object;
  target = second->object;
  percent = 90;
  x = user->x + percent * (target->x - user->x) / 100;
  z = user->z + percent * (target->z - user->z) / 100;
  userHeight = BattleUnit_GetEffectHeightFar(effect->actor);
  targetHeight = BattleUnit_GetEffectHeightFar(effect->actors[0]);
  EffectPosition_ProjectBattler(effect->actors[0], &targetScreen);
  Actor_ResetMotion(user);
  Actor_SetTargetPosition(user, x, 0, z);
  Actor_SetMode(user, 2);
  user->snapToTarget = 1;
  user->actionFlags = 1;
  user->acceleration = 2 << 16;
  user->speedLimit = 8 << 16;
  wait(20);

  for (frame = 0; frame != 96; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Frame 0: both leap. Frame 11: both turn upside down.
    if (frame == 0) {
      target->velocityY = 15 << 16;
      target->gravity = 0x91eb;
      user->velocityY = 15 << 16;
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
      Func_0814cd48(effect->actors[0], 7, 5, 0, 10);
      target->velocityY = 8 << 16;
      target->gravity = 0xab85;
      user->velocityY = 5 << 16;
      user->gravity = 0x7851;
      user->acceleration = 1 << 16;
      user->speedLimit = 2 << 16;
      user->actionFlags = 0;
      Actor_ResetMotion(user);
      Actor_SetTargetPosition(user, 0, 0, user->z);
    }

    if (effect->variant == 0) {
      // The flash, and the ring of flashes, at the user.
      Render_ResetTransformState();
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
        s32 scale = ((frame - 56) << 13) + 0x4000;
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
          for (i = 0, angle = 0x2000; i != 2; angle += 0x4000, i++) {
            Render_ResetTransformState();
            Func_08015160((targetScreen.x / 2 - 64) << 16, 32 << 16, 0);
            Func_080151e4(scale, scale, scale);
            Func_080150e4(angle);
            SceneTransform_ApplyPitch(-0x1000);
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
      Actor_SetMode(user, 0);
    }
    if (frame == 54) {
      if (effect->variant == 0)
        Func_081180e8(134);
      else
        Func_081180e8(145);
    }
    if (frame == 0) {
      Sound_PlayFar(136);
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

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.

#define HEAP_SLOT_BLITTER_ALTERNATE 188

// The transfer task runs at this priority, where ☀️'s runs at 0x480.

extern char ResourceId_FireSwirlSheet;
extern char ResourceId_PinkStarSheet;
extern char ResourceId_SkullSheet;
extern char ResourceId_BlueFlameSheet;

// The swirl's seven growth frames, cut from one sheet. Not const: the native
// reloads them after every call.
extern u16 gData_08199d40[];
extern s8 gData_08199d4e[];
extern s8 gData_08199d55[];
extern u8 gData_08199d31[];
extern u8 gData_08199d38[];

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);

void Func_08118088(s32 actor, s32 motion);

void BattleEffect_CountDownBattlerTints(void);

// Effect kind 51: the caster dashes at the target while a swirl of flame grows
// over seven frames, then bursts into sparks that fall and bounce on the
// ground. Variant 0 is ☀️'s: the swirl rises at the caster and throws pink
// stars. Variant 1 is ⚓️'s own: the swirl, recoloured into the blue flame's
// palette, rises at the target and throws 32 skulls.
void BattleEffect_RunHelmSplitter(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  DrawRectangle draw[2];
  struct EffectPosition position;
  struct EffectStep *sparks;
  struct EffectStep *spark;
  s32 *life;
  s32 frame;
  s32 i;

  // Set up the canvas and load the swirl and the spark sheets.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  if (effect->variant == 0) {
    REG_BG2PA = 0x100;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work->sheet, 1,
                             1);
  if (effect->variant == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                               work->sheet + 0x3e80, 1, 0);
    work->transferValue = 50;
  } else {
    // Lift every drawn pixel of the swirl into the upper half of the palette,
    // where the blue flame's colours go.
    u8 *pixels = work->sheet;

    for (i = 0; i != 16000; i++) {
      if (pixels[i] != 0) {
        pixels[i] = pixels[i] * 2 + 32;
        if (pixels[i] > 63)
          pixels[i] = 63;
      }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet,
                               work->sheet + 0x3e80, 1, 1);
    Iwram_CopyWords((void *)0x05000000,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
    work->transferValue = 75;
  }
  work->transferMode = 2;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);

  // No spark is alive yet.
  for (i = 0, life = &work->particles[0].variant; i != 64; i++) {
    *life = 0;
    life += 7;
  }
  sparks = work->particles;

  // The caster dashes at the target.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0x70000);

  for (frame = 0; frame != 70; frame++) {
    // Frames 8-23: the swirl grows, one cell every second frame.
    if (effect->variant == 0) {
      EffectPosition_ProjectBattler(effect->actor, &position);
      if (effect->side == 0)
        REG_BG2X = (80 - position.x) << 8;
      else
        REG_BG2X = (32 - position.x) << 8;
      if (frame >= 8 && frame <= 23) {
        s32 cell = (frame - 8) / 2;

        if (cell > 6)
          cell = 6;
        if (effect->side == 0) {
          draw[0](canvas, work->sheet + gData_08199d40[cell],
                  gData_08199d4e[cell] + 30,
                  gData_08199d55[cell] + position.y - 60, gData_08199d31[cell],
                  gData_08199d38[cell]);
        } else {
          draw[0](canvas, work->sheet + gData_08199d40[cell],
                  -gData_08199d4e[cell] - gData_08199d31[cell] + 98,
                  gData_08199d55[cell] + position.y - 60, gData_08199d31[cell],
                  gData_08199d38[cell]);
        }
      }
    } else {
      EffectPosition_ProjectBattler(effect->actors[0], &position);
      if (frame >= 8 && frame <= 23) {
        s32 cell = (frame - 8) / 2;

        if (cell > 6)
          cell = 6;
        if (effect->side == 0) {
          draw[0](canvas, work->sheet + gData_08199d40[cell],
                  position.x / 2 + gData_08199d4e[cell] - 16,
                  gData_08199d55[cell] + position.y - 92, gData_08199d31[cell],
                  gData_08199d38[cell]);
        } else {
          draw[0](canvas, work->sheet + gData_08199d40[cell],
                  position.x / 2 - gData_08199d4e[cell] - gData_08199d31[cell] +
                      16,
                  gData_08199d55[cell] + position.y - 92, gData_08199d31[cell],
                  gData_08199d38[cell]);
        }
      }
    }

    // Frame 18: the hit. The target flashes and reels, the screen shakes and
    // the swirl bursts into sparks.
    if (frame == 18) {
      EffectPosition_ProjectBattler(effect->actors[0], &position);
      Func_081180e8(134);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Func_08118088(effect->actors[0], 6);
      work->shakeFrames = 4;
      for (i = 0; i != effect->variant * 16 + 16; i++) {
        s32 force = (Random16() & 255) + 256;
        s32 angle = Random16() & 0xffff;

        spark = &sparks[i];
        if (effect->variant == 0)
          spark->x = 0x400000;
        else
          spark->x = (position.x / 2) << 16;
        spark->y = 0x500000;
        spark->velocityX = (Trig_Sin(angle) * force) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * force) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
    }

    // The sparks fall, bounce off the ground at y = 104 and fade out.
    for (i = 0; i != 64; i++) {
      spark = &sparks[i];
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0);
        if (spark->y > 0x680000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0) {
          s32 sx = spark->x >> 16;
          s32 sy = spark->y >> 16;

          if (effect->variant == 0)
            draw[0](canvas, work->sheet + 0x3e80 + (((frame + i) / 4 % 6) << 8),
                    sx - 8, sy - 8, 16, 16);
          else
            draw[0](canvas, work->sheet + 0x3e80 + i % 3 * 576, sx - 12,
                    sy - 12, 24, 24);
        }
      }
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// The sparks live in the map cell buffer's second 16 KB: 128 at the target,
// 128 thrown up at the start and 128 shed by the head.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

// The pillar sheet's cell is 24 x 39; the faded copies follow it.
#define PILLAR_CELL_SIZE 936

struct Point2D {
  s32 x;
  s32 y;
};

extern char ResourceId_FirePillarSheetB;
extern char ResourceId_Data134;
extern char ResourceId_EmberStreakSheet;

extern char ResourceId_OrangePaletteB;
extern char ResourceId_DualStreamPath;

// The puff arc's six cells, cut from the ember streak sheet.
extern u8 gData_08197467[];
extern u8 gData_0819746d[];
extern u8 gData_08197473[];
extern u16 gData_0819747a[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gData_08197410[];

// How many of the 56 embers are sparks, by variant (48 and 40); the rest
// are flashes.
extern u8 Data_08199d5c[];

void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);

void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// Returns the fixed IWRAM word copy entry, so the call loads its address before
// its arguments.
static inline s32 (*Iwram_GetCopyWords(void))(void *destination,
                                              const void *source, s32 size) {
  return Iwram_CopyWords;
}

// Dragon Cloud (effect kind 49): a puff arc rises over the user, then a
// dragon's head flies the path from frame 18, trailing fading copies of
// itself, while the camera turns; from frame 78 it dives onto the target.
// The puffs burst at frame 50 and the target flashes; at frame 83 the head
// strikes, the target reels, and 56 embers fly from it: sparks, then flashes.
// Variant 1, ⚓️'s own, loads orange colours, throws 128 sparks up from the
// user's side at the start, sheds four sparks a frame from the head and
// throws 128 more from the target with the embers.
void BattleEffect_RunDragonCloud(struct BattleEffectArgument *effect) {
  struct Point2D trail[20];
  struct EffectPosition target;
  struct EffectPosition position;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 drift;
  s8 *path;
  s32 x;
  s32 y;
  u8 *graphics;
  struct Actor *object;
  struct EffectStep *step;
  s32 i;
  s32 j;
  s32 angle;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  drift = 0;
  x = 0;
  y = 0;
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;

  // The pillar sheet, and nine copies of its cell, each darker than the last,
  // for the trail.
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work->sheet, 1,
                             1);
  for (i = 1; i != 20; i++) {
    for (j = 0; j != PILLAR_CELL_SIZE; j++) {
      s32 shade = work->sheet[0x1680 + j];

      if (i > 10) {
        shade = shade - i * 4 + 40;
        if (shade < 0)
          shade = 0;
        work->sheet[0x1680 + (i - 10) * PILLAR_CELL_SIZE + j] = shade;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             &work->sheet[0x3c00], 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             Ram_MapCellBuffer, 1, 0);
  /* FAKEMATCH(wrapper): the copy routine fetched through Iwram_GetCopyWords,
   * so the call loads its address before its arguments; as Iwram_CopyWords
   * called directly, 2 of 1123 instructions differ in tla-en. */
  if (effect->variant == 1)
    Iwram_GetCopyWords()(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The 56 embers wait at the target: sparks first, then flashes, which rise
  // more slowly.
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != 56; i++) {
    step = &work->particles[8 + i];
    step->x = object->x;
    step->y = 0;
    step->z = object->z;
    if (i >= Data_08199d5c[effect->variant]) {
      step->velocityX = ((Random16() & 63) - 32) << 14;
      step->velocityY = (Random16() & 63) << 12;
    } else {
      step->velocityX = ((Random16() & 63) - 32) << 14;
      step->velocityY = (Random16() & 63) << 13;
    }
    step->velocityZ = ((Random16() & 63) - 32) << 14;
    /* FAKEMATCH(dead): an empty once-only block that keeps the variant's 1 and
     * i's step after velocityZ's store; without it, 4 of 1123 instructions
     * differ in tla-en. */
    do {
    } while (0);
    step->variant = 1;
  }

  // The sparks: all asleep but the first 128, at the target (only variant 1
  // draws them).
  for (i = 0; i != SPARK_COUNT; i++)
    SPARKS[i].variant = -1;
  for (i = 0; i != 128; i++) {
    step = &SPARKS[i];
    step->x = object->x;
    step->y = 0;
    step->z = object->z;
    step->velocityX = ((Random16() & 63) - 32) << 14;
    step->velocityY = (Random16() & 63) << 12;
    step->velocityZ = ((Random16() & 63) - 32) << 14;
    step->variant = (Random16() & 15) + 24;
  }

  // The eight puffs, on an arc over the user, each two frames after the last.
  for (i = 0, angle = -0x4000; i != 8; angle += 0x1000, i++) {
    step = &work->particles[i];
    if (effect->side == 1)
      step->x = (Trig_Sin(angle) * 24 >> 16) + 88;
    else
      step->x = (-(Trig_Sin(angle) * 24) >> 16) + 16;
    step->y = (Trig_Cos(angle) * 16 >> 16) + 40;
    /* FAKEMATCH(dead): an empty once-only block that keeps the y store ahead of
     * the variant's; without it, 2 of 1123 instructions differ in tla-en. */
    do {
    } while (0);
    step->variant = -(i * 2);
  }
  path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

  for (frame = 0; frame != 150; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 83) {
      if (effect->variant == 0)
        Func_081180e8(134);
      else
        Func_081180e8(145);
    }
    if (frame == 0)
      Sound_PlayFar(136);
    if (frame == 50)
      Sound_PlayFar(136);

    // For 64 frames the camera turns away from the user's side.
    if (effect->side == 0) {
      if (frame <= 63)
        camera->yaw -= 0x100;
    } else {
      if (frame <= 63)
        camera->yaw += 0x100;
    }
    BattleCamera_SetViewFar(0, 0, 0, 100);

    // Frames 0-17: the puff arc rises, one cell every three frames.
    if (frame <= 17) {
      s32 cell = frame / 3;
      s32 width;
      s32 offset;

      offset = gData_0819747a[cell];
      /* FAKEMATCH(dead): width, read from the puff table and never used; as no
       * width, 523 of 1123 instructions differ in tla-en. */
      width = gData_08197467[cell];
      draw[0](canvas, &work->sheet[offset + 0x3c00],
              56 - gData_08197467[cell] / 2, gData_08197473[cell] + 60,
              gData_08197467[cell], gData_0819746d[cell]);
      draw[1](canvas, &work->sheet[gData_0819747a[cell] + 0x3c00],
              64 - gData_08197467[cell] / 2, gData_08197473[cell] + 60,
              gData_08197467[cell], gData_0819746d[cell]);
    }

    // Variant 1: 128 sparks thrown up from the user's side at the start.
    if (effect->variant == 1) {
      if (frame == 0) {
        for (i = 0; i != 128; i++) {
          s32 angle;
          s32 force;

          step = &SPARKS[128 + i];
          step->x = 0x3c0000;
          step->y = 0x6c0000;
          angle = (Random16() & 0x7fff) + 0x4000;
          force = (Random16() & 255) + 127;
          step->velocityX = (Trig_Sin(angle) * force) >> 7;
          step->velocityY = (Trig_Cos(angle) * force) >> 5;
          step->variant = (Random16() & 15) + 24;
        }
      }
      for (i = 0; i != 64; i++) {
        step = &SPARKS[128 + i];
        if (step->variant >= 0 && frame >= i / 8) {
          s32 size = (step->variant >> 4) + 1;

          draw[i & 1](canvas, graphics + gData_08197410[size - 1],
                      HI(step->x) - size / 2, HI(step->y) - size, size,
                      size * 2);
          BattleEffectKernels_IntegrateVector2(step, 62, 0x4000);
          if (step->y > ((i & 3) + 108) << 16)
            step->velocityY = -step->velocityY / 2;
          step->variant--;
        }
      }
    }

    // Frames 18-58: the head flies the path, a 16-bit start and then
    // steps; from frame 78 it dives from above the target.
    if (frame >= 18 && frame <= 58) {
      if (frame == 18) {
        x = (path[0] << 8) + (u8)path[1];
        y = (path[2] << 8) + (u8)path[3] + 16;
        path += 4;
      } else {
        x += path[0];
        y += path[1];
        path += 2;
      }
    }
    if (frame >= 78 && frame <= 118) {
      EffectPosition_ProjectBattler(effect->actors[0], &target);
      if (frame == 78) {
        x = target.x / 2;
        y = 48;
      } else {
        y -= 16;
      }
    }

    // The trail: each copy takes the place of the one before, the older
    // ones darker.
    for (j = 19; j != 0; j--) {
      if (frame > j + 18 && frame <= j + 83) {
        trail[j].x = trail[j - 1].x;
        trail[j].y = trail[j - 1].y;
        if (j > 10)
          draw[0](canvas, &work->sheet[0x1680 + (j - 10) * PILLAR_CELL_SIZE],
                  trail[j].x - 12, trail[j].y, 24, 39);
        else
          draw[0](canvas, &work->sheet[0x1680], trail[j].x - 12, trail[j].y, 24,
                  39);
      }
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    // Frames 18-83: the head, tilted by how fast it climbs or falls.
    if (frame >= 18 && frame <= 83) {
      s32 lift;

      if (frame >= 78 && frame <= 118)
        position.x = x;
      else if (effect->side == 1)
        position.x = 64 - x / 2;
      else
        position.x = x / 2 + 64;
      position.y = 60 - y;
      lift = (position.y - trail[0].y - 24) / 2;
      if (lift > 2)
        lift = 2;
      if (lift < -2)
        lift = -2;
      drift += lift;
      if (drift > 8)
        drift = 8;
      if (drift < -8)
        drift = -8;
      lift = drift / 4 + 2;
      trail[0].x = position.x;
      trail[0].y = position.y - 20;
      draw[0](canvas, &work->sheet[lift * 0x480], trail[0].x - 12,
              trail[0].y - 2, 24, 48);

      // The head sheds four sparks a frame (only variant 1 draws them).
      for (i = 0; i != 4; i++) {
        s32 angle;
        s32 force;
        s32 index;

        index = ((frame * 4 + i) & 127) + 256;
        step = &SPARKS[index];
        step->x = position.x << 16;
        step->y = position.y << 16;
        angle = Random16();
        force = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * force) >> 7;
        step->velocityY = (Trig_Cos(angle) * force) >> 5;
        step->variant = (Random16() & 15) + 24;
      }
    }

    // After the strike: the embers fly from the target, sized by depth;
    // the flashes play the flash sheet.
    if (frame > 82) {
      for (i = 0; i != 56; i++) {
        step = &work->particles[8 + i];
        if (step->y >= 0) {
          s32 size;

          EffectPosition_ProjectPoint(&step->x, &position);
          position.x >>= 1;
          if (position.depth <= 159)
            position.depth = 160;
          if (position.depth > 799)
            position.depth = 799;
          size = 9 - (position.depth - 160) / 64;
          if (i >= Data_08199d5c[effect->variant]) {
            s32 cell;

            if (effect->variant == 0)
              cell = step->variant / 2;
            else
              cell = step->variant / 4;
            if (cell <= 5) {
              draw[effect->variant](canvas, Ram_MapCellBuffer + (cell << 11),
                                    position.x - 16, position.y - 40, 32, 64);
              step->variant++;
              BattleEffectKernels_IntegrateVector3(step, 62, 0x2000);
            }
          } else {
            draw[0](canvas, graphics + gData_08197410[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            BattleEffectKernels_IntegrateVector3(step, 64, -0x2000);
          }
        }
      }

      // Variant 1's 128 sparks at the target.
      if (effect->variant == 1) {
        for (i = 0; i != 128; i++) {
          s32 size;

          step = &SPARKS[i];
          size = step->variant;
          if (size >= 0) {
            EffectPosition_ProjectPoint(&step->x, &position);
            size >>= 3;
            size += 1;
            position.x >>= 1;
            draw[i & 1](canvas, graphics + gData_08197410[size - 1],
                        position.x - size / 2, position.y - size, size,
                        size * 2);
            BattleEffectKernels_IntegrateVector3(step, 62, 0x2000);
            step->variant--;
          }
        }
      }
    }

    // Variant 1: the sparks the head shed.
    if (effect->variant == 1) {
      struct EffectStep *spark = &SPARKS[256];

      for (i = 0; i != 128; i++, spark++) {
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 4) + 1;

          draw[i & 1](canvas, graphics + gData_08197410[size - 1],
                      HI(spark->x) - size / 2, HI(spark->y) - size, size,
                      size * 2);
          BattleEffectKernels_IntegrateVector2(spark, 60, -0x4000);
          spark->variant--;
        }
      }
    }

    // Frame 83: the strike. Frame 50: the puffs burst.
    if (frame == 83) {
      work->shakeFrames = 8;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Func_08118088(effect->actors[0], 1);
    }
    if (frame == 50) {
      work->shakeFrames = 12;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
    }
    if (frame > 49) {
      for (i = 0; i != 8; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 11) {
          s32 cell = step->variant / 2;

          draw[1](canvas, &work->sheet[gData_0819747a[cell] + 0x3c00],
                  step->x - gData_08197467[cell] / 2,
                  step->y + gData_08197473[cell], gData_08197467[cell],
                  gData_0819746d[cell]);
        }
        step->variant++;
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
