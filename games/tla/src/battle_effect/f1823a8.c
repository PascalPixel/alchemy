#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_action.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "types.h"

#define HEAP_SLOT_BLITTER 104 // Ram_HeapSlots->unk64[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3.
#define BLITTER_ADDITIVE 0
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define STARS_PER_TARGET 64 // each target's slice of the star buffer
#define STAR_TOTAL 384

extern char ResourceId_Data134;
extern char ResourceId_BlastSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_JupiterDjinnSheet;

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern const u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_LoadBlitter(s32 slot, s32 blitter);
void *Resource_GetTableEntry(s32 resourceId);
void Task_Add(s32 func, s32 priority);
s32 Task_Remove(s32 func);
s32 __divsi3(s32 dividend, s32 divisor);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_0815f0a0(s32 resourceId);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
void Render_ResetTransformState(void);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void SceneTransform_ApplyPosition(const s32 *position);
void Transform_Push(void);
void Transform_Pop(void);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Func_080150e4(s32 angle);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);
void HeapSlot_Release(s32 slot);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);

// Swarms of stars over the targets. Each target gets a swarm, eight frames
// after the one before: its stars burst from a point that rises out of it,
// each turning about the point on one of four axes and drawn smaller the
// deeper it is, and from frame 24 they are pulled back into the point,
// each one gone once it has arrived. The palette steps through the
// Mercury, Mars and Jupiter Djinn's. Mode 0: 16 stars a target, the
// target flashes as its swarm starts, and the effect lasts eight frames
// a target more than 38. Mode 1: 48 stars, a flash at the start and 55
// frames later, 72 frames in all.
void Func_081823a8(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  // Only draw[0] is used; the array takes an 8-byte spill slot in the frame.
  DrawRectangle draw[2];
  s32 member;
  u8 *sparkCels = Ram_HeapSlots->unk64[0];
  s32 frames;
  s32 *camera;
  s32 frame;
  s32 i;
  struct EffectPosition screen;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_ADDITIVE, 7));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (mode == 0)
    frames = effect->count * 8 + 38;
  else
    frames = 72;
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != frames; frame++) {
    camera = Ram_HeapSlots->cameraWork;

    // The first frame sets each target's point on it, rising, and every
    // star at its point with a random push across the floor.
    if (frame == 0) {
      for (i = 0; i != effect->count; i++) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        work->particles[i].x = object->x;
        work->particles[i].y = 0;
        work->particles[i].z = object->z;
        if (mode == 1) {
          work->particles[i].velocityX = 0;
          work->particles[i].velocityY = 0x8000;
        } else {
          work->particles[i].velocityX = -object->x / 40;
          work->particles[i].velocityY = 3 << 16;
        }
        work->particles[i].velocityZ = 0;
      }
      for (i = 0; i != STAR_TOTAL; i++) {
        struct EffectStep *star = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        s32 angle = Random16() & 0xffff;
        s32 speed = 255; // the mask starts in speed's own register

        speed &= Random16();

        star->x = 0;
        star->y = ((Random16() & 31) + 20) << 16;
        star->z = 0;
        speed += 128;
        star->velocityX = (Trig_Sin(angle) * speed) >> 5;
        star->velocityY = 0;
        star->velocityZ = (Trig_Cos(angle) * speed) >> 5;
        star->variant = 0;
      }
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_BlastSheet), 128);
    }

    // Two palette steps a frame from frame 8.
    for (i = 0; i != 2; i++) {
      if (frame >= 8) {
        if (frame < 20)
          Func_0815f0a0((s32)&ResourceId_MercuryDjinnSheet);
        else if (frame < 32)
          Func_0815f0a0((s32)&ResourceId_MarsDjinnSheet);
        else
          Func_0815f0a0((s32)&ResourceId_JupiterDjinnSheet);
      }
    }

    for (member = 0; member != effect->count; member++) {
      struct EffectStep *point = &work->particles[member];
      s32 start = member;
      s32 starCount;

      // Set in two steps: as one giv the loop pass would turn it into a walker.
      start *= 8;
      starCount = 16;
      if (mode != 0)
        starCount = 48;
      if (frame == start) {
        if (mode == 0)
          Func_0814cd48(effect->actors[member], 7, 5, member, 16);
        else
          Func_0814cd48(effect->actors[member], 7, -1, member, 16);
      }
      if (frame == start + 55 && mode != 0)
        Func_0814cd48(effect->actors[member], 7, -1, member, 16);

      // The swarm turns about its point, which rises from frame 4.
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      if (frame >= start + 4 && start < 44)
        BattleEffectKernels_IntegrateVector3(point, 64, 0);
      SceneTransform_ApplyPosition(&point->x);

      for (i = 0; i != starCount; i++) {
        struct EffectStep *star =
            &((struct EffectStep *)
                  Ram_MapCellBuffer)[member * STARS_PER_TARGET + i];

        if (frame >= i / 8 + start && star->variant == 0) {
          s32 size;

          Transform_Push();
          switch ((member + i) & 3) {
          case 0:
            SceneTransform_ApplyYaw(frame * (i + 8) * 32);
            break;
          case 1:
            SceneTransform_ApplyPitch(-frame * (i + 8) * 32);
            break;
          case 2:
            Func_080150e4(-frame * (i + 8) * 32);
            break;
          case 3:
            SceneTransform_ApplyPitch(-frame * (i + 8) * 32);
            Func_080150e4(-frame * (i + 8) * 32);
            break;
          }
          EffectPosition_ProjectPoint(&star->x, &screen);
          screen.x >>= 1;
          Transform_Pop();
          if (screen.depth < 250)
            screen.depth = 250;
          if (screen.depth > 0x27a)
            screen.depth = 0x27a;
          size = 9 - (screen.depth - 250) / 64;
          draw[0](canvas, sparkCels + gData_08197410[size - 1],
                  screen.x - size / 2, screen.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(star, 60, 0);

          // The pull back into the point; a star within 2047 of it across
          // the floor is gone.
          if (frame >= i / 8 + 24) {
            s32 dx = -star->x >> 7;
            s32 dy = -star->y >> 7;
            s32 dz = -star->z >> 7;

            star->velocityX += dx;
            star->velocityY += dy;
            star->velocityZ += dz;
            star->velocityX = star->velocityX * 62 / 64;
            star->velocityY = star->velocityY * 62 / 64;
            star->velocityZ = star->velocityZ * 62 / 64;
            if ((dx >= -2047 && dx <= 2047) && (dz >= -2047 && dz <= 2047))
              star->variant = -1;
          }
        }
      }
    }

    BattleCamera_Shake(4, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
