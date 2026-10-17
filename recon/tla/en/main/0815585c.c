// Draft of Func_0815585c, The Broken Seal's BattleFx_PrepareCanvasEffect
// (games/tbs/src/effect/effect.c): differ 165 of 818 (tla-en,
// 2026-10-15). The Lost Age reworked it: the kind, the side and the anchor
// come packed in one word (kind | side << 4 | anchor << 5), eleven anchors
// by jump table where The Broken Seal has five, anchor 6 keeps the effect
// at 40 frames with no palette fade and brightens the backdrop from frame
// 24, anchor 8 runs 54 frames, the sparks live in the work's particles,
// the small Djinn decodes into the work's sheet, the anchor's world point
// as well as its screen point goes out, and there is no canvas layer of its
// own. Kind 0 (Venus) takes its palette from resource 0x184 and its picture
// from 0x157.

// What moved it: the three heap slots read before the packed word is
// unpacked (562 to 546), packed as unsigned (lsrs), and the fade step's
// zero through a variable of its own in the long branch (546 to 169, found
// by alchemy permute; tagged below), and Func_081963ec
// returning its loader's s32, as nibble_pair.c now does (169 to 165).

// Left: allocation through the frame loop, from the prologue's first
// spills (the native keeps anchor at sp+20 and reloads it for the test of
// 8; here it stays in r1) and order rows of the usual kind. alchemy permute
// 10 minutes: 155 with the clear size held in a variable from the prologue
// (not kept, it only moves the allocation), 159 with a pointer copy of step.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "transform.h"

// The packed preparer word: kind | side << 4 | anchor << 5.
#define PREPARE_KIND(packed) ((packed) & 7)
#define PREPARE_SIDE(packed) (((packed) >> 4) & 1)
#define PREPARE_ANCHOR(packed) (((packed) >> 5) & 15)

// The anchor that keeps the effect short (40 frames, no palette fade) and
// brightens the backdrop from frame 24.
#define ANCHOR_BRIGHTEN 6
// The anchor whose effect runs 54 frames.
#define ANCHOR_SHORT 8

struct MotionObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct BattleObj {
  struct MotionObject *object;
};

struct CameraWork {
  s32 eye[3];
  s32 pos[3];
};

extern u8 ResourceId_BlastSheet[];
extern u8 ResourceId_MercuryDjinnSmallSheet[];
extern u8 ResourceId_MarsDjinnSmallSheet[];
extern u8 ResourceId_JupiterDjinnSmallSheet[];
extern u8 ResourceId_VenusDjinnSmallSheet[];
extern u8 ResourceId_Data134[];

// The Broken Seal's gBattleFx_MoteCellOffsets: each spark size's cel.
extern const u16 gData_08197410[];

u8 *Resource_GetTableEntry(s32 id);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Func_081963ec(s32 slot, s32 modeAndFlags);
struct BattleObj *GetBattleObjectSlotFar(s32 id);
s32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void Func_08143000(void);
void Func_08164abc(s32 red, s32 green, s32 blue);
void Audio_PlayCue(s32 cue);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Func_080150e4(s32 angle);
void Graphics_RestoreTransferWork(void);
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer, s32 frames);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);

// The opening every Djinn effect shares: sparks gather from around the
// caster, spin about a point travelling from the caster to the anchor, and
// fall into it; a small Djinn (the kind) appears there at frame 54 and
// bursts into motes at frame 64. The anchor's screen point goes to screen
// and its world point to world.
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world) {
  s32 base[3];
  s32 goal[3];
  s32 step[3];
  s32 zero;
  s32 point[3];
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  u8 *sheet;
  s32 total;
  struct MotionObject *target;
  struct MotionObject *actor;
  u8 *palette;
  s32 resource;
  s32 frame;
  s32 i;
  s32 kind;
  s32 side;
  s32 anchor;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  kind = PREPARE_KIND(packed);
  side = PREPARE_SIDE(packed);
  anchor = PREPARE_ANCHOR(packed);
  total = 40;
  if (anchor != ANCHOR_BRIGHTEN) {
    if (anchor == ANCHOR_SHORT) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
      total = 54;
    } else {
      /* FAKEMATCH: the zero through a variable of its own; written as 0,
       * the stores of the fade and the frame count allocate differently
       * all through the function (differ 546 against 169; alchemy
       * permute). */
      zero = 0;
      work->fadeFrames = 24;
      work->fadeStep = zero;
      if (kind > 3) {
        kind -= 4;
        total = 84;
      } else {
        total = 55;
      }
    }
  }

  // The Djinn's palette, then its small picture.
  switch (kind) {
  case 0:
    resource = (s32)ResourceId_BlastSheet;
    break;
  case 1:
    resource = (s32)ResourceId_MercuryDjinnSmallSheet;
    break;
  case 2:
    resource = (s32)ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    resource = (s32)ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  palette = Resource_GetTableEntry(resource);
  Iwram_CopyWords((void *)0x05000000, palette, 128);
  switch (kind) {
  case 0:
    resource = (s32)ResourceId_VenusDjinnSmallSheet;
    break;
  case 1:
    resource = (s32)ResourceId_MercuryDjinnSmallSheet;
    break;
  case 2:
    resource = (s32)ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    resource = (s32)ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  Resource_DecodeType01(Resource_GetTableEntry(resource) + 128, work->sheet);
  Resource_LoadAndDecompress((s32)ResourceId_Data134, sheet, 0, 0);

  // Additive and max blitters, clipped (flipped for side 1).
  if (side == 1) {
    Func_081963ec(104, 0x07);
    Func_081963ec(188, 0x17);
  } else {
    Func_081963ec(104, 0x03);
    Func_081963ec(188, 0x13);
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  actor = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;

  // 64 sparks start on the ground around the caster, flying outward.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &work->particles[i];
    s32 angle = Random16() & 0xffff;
    s32 speed = (Random16() & 255) + 128;

    spark->x = 0;
    spark->y = ((Random16() & 31) + 20) << 16;
    spark->z = 0;
    spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(angle) * speed) >> 5;
    spark->variant = 0;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  // The spin's centre moves from the caster to the anchor over frames 6-45.
  base[0] = actor->x;
  base[1] = 0;
  base[2] = actor->z;
  switch (anchor) {
  case 0:
    goal[0] = target->x;
    goal[1] = 0x3c0000;
    goal[2] = target->z;
    break;
  case 8:
    goal[0] = target->x;
    goal[1] = 0x3c0000;
    goal[2] = target->z;
    break;
  case 1:
    goal[0] = target->x;
    goal[1] = 0x3c0000;
    goal[2] = 0;
    break;
  case 9:
    goal[0] = target->x;
    goal[1] = 0x780000;
    goal[2] = 0;
    break;
  case 2:
    goal[0] = actor->x;
    goal[1] = 0x3c0000;
    goal[2] = actor->z;
    break;
  case 3:
    goal[0] = actor->x;
    goal[1] = 0x3c0000;
    goal[2] = 0;
    break;
  case 4:
  case 6:
    goal[0] = 0;
    goal[1] = 0x3c0000;
    goal[2] = 0;
    break;
  case 7:
    goal[0] = 0;
    goal[1] = 0x140000;
    goal[2] = 0;
    break;
  case 5:
    goal[0] = actor->x;
    goal[1] = 0x500000;
    goal[2] = 0;
    break;
  case 10:
    goal[0] = actor->x;
    goal[1] = 0x1e0000;
    goal[2] = 0;
    break;
  }
  step[0] = (goal[0] - base[0]) / 40;
  step[1] = (goal[1] - base[1]) / 40;
  step[2] = (goal[2] - base[2]) / 40;

  for (frame = 0; frame != total; frame++) {
    struct CameraWork *camera = Ram_HeapSlots->cameraWork;

    if (frame > 75)
      REG_BLDALPHA = (168 - frame * 2) | 0x1000;
    if (anchor == ANCHOR_BRIGHTEN && frame > 23)
      Func_08164abc(2, 2, 2);
    if (frame == 8)
      Audio_PlayCue(212);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
    if (frame >= 6 && frame <= 45) {
      base[0] += step[0];
      base[1] += step[1];
      base[2] += step[2];
    }
    SceneTransform_ApplyPosition(base);
    if (frame == 0)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == 24)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);

    // The 32 sparks spin about the centre, each on its own axis, and from
    // their 24th frame fall into it; a spark that has arrived stops.
    if (frame <= 63) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *spark = &work->particles[i];

        if (frame >= i / 8 && spark->variant == 0) {
          s32 size;

          Graphics_SaveTransferWorkOnce();
          switch (i & 3) {
          case 0:
            SceneTransform_ApplyYaw(frame * (i * 32 + 256));
            break;
          case 1:
            SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
            break;
          case 2:
            Func_080150e4(-frame * (i * 32 + 256));
            break;
          case 3:
            SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
            Func_080150e4(-frame * (i * 32 + 256));
            break;
          }
          EffectPosition_ApplyBaseAndYOffset(&spark->x, &position);
          position.x >>= 1;
          Graphics_RestoreTransferWork();
          if (position.depth < 250)
            position.depth = 250;
          if (position.depth > 634)
            position.depth = 634;
          size = 8 - (position.depth - 250) / 64;
          draw[1](canvas, sheet + gData_08197410[size - 1],
                  position.x - size / 2, position.y - size, size, size * 2);
          BattleFxKernels_IntegrateVector3(spark, 60, 0);
          if (frame >= i / 8 + 24) {
            s32 pullX = -spark->x >> 7;
            s32 pullY = -spark->y >> 7;
            s32 pullZ = -spark->z >> 7;

            spark->velocityX += pullX;
            spark->velocityY += pullY;
            spark->velocityZ += pullZ;
            spark->velocityX = spark->velocityX * 62 / 64;
            spark->velocityY = spark->velocityY * 62 / 64;
            spark->velocityZ = spark->velocityZ * 62 / 64;
            if ((pullX > -2048 && pullX < 2048) &&
                (pullZ > -2048 && pullZ < 2048))
              spark->variant = -1;
          }
        }
      }
    }

    // The small Djinn sways at the anchor for frames 54 to 69.
    if (frame >= 54 && frame <= 69) {
      point[0] = Trig_Sin(frame << 10) << 2;
      point[1] = 0;
      point[2] = 0;
      world[0] = goal[0] + (Trig_Sin(frame << 10) << 2);
      world[1] = goal[1];
      world[2] = goal[2];
      EffectPosition_ApplyBaseAndYOffset(point, &position);
      screen[0] = position.x;
      screen[1] = position.y;
      position.x >>= 1;
      draw[0](canvas, work->sheet, position.x - 10, position.y - 20, 20, 40);
    }

    // At frame 64 it bursts into 64 motes that fall away.
    if (frame == 64) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];
        s32 angle = Random16() & 0xffff;
        s32 speed = (Random16() & 255) + 128;

        mote->x = screen[0] << 15;
        mote->y = screen[1] << 16;
        mote->velocityX = (Trig_Sin(angle) * speed) >> 6;
        mote->velocityY = (Trig_Cos(angle) * speed) >> 5;
        mote->variant = (Random16() & 15) + 8;
      }
    }
    if (frame > 63) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          draw[0](canvas, sheet + gData_08197410[size - 1],
                  (s16)(mote->x >> 16) - size / 2, (s16)(mote->y >> 16) - size,
                  size, size * 2);
          BattleFxKernels_IntegrateVector2(mote, 60, 0);
          mote->variant--;
        }
      }
    }
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  Iwram_ClearWords(canvas, 0x4000);
  REG_BLDALPHA = 0x1010;
}
