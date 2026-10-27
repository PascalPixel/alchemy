// Draft (tla-en): listing 535 instructions, draft 537, common 388, differ 296.
// The body of effect kinds 465 and 466 (summon_modes_2.c's BattleEffect_RunDjinnStun
// and BattleEffect_RunMysticCall pass modes 0 and 1). The Lost Age's rework of ☀️'s
// BattleEffect_RunSwirlingStars (structure 0.70): the same four rotations by
// star and the same depth-sized cels, one swarm per target.
// Written 2026-10-15 (lane N2) from the listing and the twin.
// Left:
// - the native frame is 84 bytes, here 80: the native keeps the effect
// argument spilled at sp+68 and gives fp to the yaw angle walker of the
// star loop (frame * 256, stepped by frame * 32), with -frame spilled at
// sp+24 for the other walker; here the argument takes fp, and every
// spill slot after it moves (member, frames, camera, sparkCels).
// - the native walks the actor offset (sp+16), the point (sp+12) and the
// star offset (sp+8, by 0x700) per target and rebuilds start = member *
// 8 from member; here start is a walker too.
// - the star count, frames and flash calls follow from those slots.
// Measured: the first draft 338; the arrival test grouped as two range
// pairs, so both fold to one unsigned compare each as in the native: 296.
// Resource 0x184 (BattleEffect_BlastSheet) and the Djinn sheets 0x154, 0x150
// and 0x152 need names in system/resource/directory.s when this is
// adopted.
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
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
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
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
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
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
  u8 *sparkCels = Ram_HeapSlots->unk64[0];
  struct EffectPosition screen;
  DrawRectangle draw;
  s32 *camera;
  s32 frames;
  s32 frame;
  s32 member;
  s32 i;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_ADDITIVE, 7));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (mode == 0)
    frames = effect->count * 8 + 38;
  else
    frames = 72;
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != frames; frame++) {
    camera = Ram_HeapSlots->cameraWork;

    // The first frame sets each target's point on it, rising, and every
    // star at its point with a random push across the floor.
    if (frame == 0) {
      struct EffectStep *point = work->particles;
      struct EffectStep *star;

      for (i = 0; i != effect->count; i++, point++) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(effect->actors[i])->object;
        s32 rise;

        point->y = 0;
        point->x = object->x;
        point->z = object->z;
        if (mode == 1) {
          rise = 0x8000;
          point->velocityX = 0;
        } else {
          point->velocityX = -object->x / 40;
          rise = 3 << 16;
        }
        point->velocityY = rise;
        point->velocityZ = 0;
      }
      star = (struct EffectStep *)Ram_MapCellBuffer;
      for (i = 0; i != STAR_TOTAL; i++, star++) {
        s32 angle = Random16() & 0xffff;
        s32 speed = Random16() & 255;

        star->x = 0;
        star->z = 0;
        star->y = ((Random16() & 31) + 20) << 16;
        speed += 128;
        star->velocityX = (speed * Trig_Sin(angle)) >> 5;
        star->velocityY = 0;
        star->velocityZ = (speed * Trig_Cos(angle)) >> 5;
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
      struct EffectStep *star;
      s32 start = member * 8;
      s32 starCount;

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

      star =
          &((struct EffectStep *)Ram_MapCellBuffer)[member * STARS_PER_TARGET];
      for (i = 0; i != starCount; i++, star++) {
        if (frame >= start + i / 8 && star->variant == 0) {
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
          draw(canvas, sparkCels + gData_08197410[size - 1],
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

    Func_08158ce0(4, 16);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
