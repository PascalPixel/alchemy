// Near miss of BattleEffect_RunFire, effect kind 12.

// Twin: The Broken Seal's BattleEffect_RunFire (effect.c), call for
// call. The Lost Age reads the effect from its argument, takes its heap
// blocks as Ram_HeapSlots fields, loads its blitters through BattleEffect_LoadBlitter
// (BLIT_ADDITIVE 0 and BLIT_MAX 1 where The Broken Seal has 3 and 2),
// orders both callbacks 0xc80, and plays cue 134 for a landing ember in
// variant 2 where The Broken Seal always plays 136.

// Near miss: differ 46 of 513 (draft 513), all order residue: about
// twenty pairs where the native issues a pool or stack load, or one call
// argument, one slot away from agscc's place (the camera before the
// effect's copy, r1 before r0 for both BattleEffect_LoadBlitter calls, the particle
// buffer before its counter, the cell offset table before the size).

// Under -mthumb-slow-loads (2026-10-26): differ 14 of 513. Declared
// returning s32, as nibble_pair.c defines it, BattleEffect_LoadBlitter
// gets r1 before r0 at both calls (18 to 14). Left: five rows, each a
// stack or field load agscc issues one slot earlier than the native (the
// camera word before the effect copy, a reload before an argument store,
// the trail's coordinates). An empty one-pass block or asm before the heap
// reads: 22.

// Tried (2026-10-15):
// - greedy declaration-order sweep, two passes: no gain.
// - a separate counter for the frame loop's embers: no change.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "mem.h"
#include "motobj.h"
#include "actor.h"
#include "ram_buffer.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void Camera_ApplyPhasedDelta(void); // Camera_ApplyPhasedDelta
void BattleEffect_CountDownBattlerTints(void); // BattleEffect_CountDownBattlerTints
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // Actor_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_StartPhaseFar
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
s32 BattleUnit_GetEffectHeightFar(s32 actor);

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus The
// Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))

#define EMBERS ((struct EffectStep *)Ram_MapCellBuffer)
#define IN_FLIGHT -1 // an ember's variant; 0 and up count its flare's frames

extern char ResourceId_EmberStreakSheet;
extern char ResourceId_Data134; // the particle sheet
extern u8 Data_08198300[];      // gFire_Counts
extern u16 gData_08197410[];    // gBattleEffect_MoteCells
extern u16 Data_08198316[];     // gFire_FlareCells
extern u8 Data_08198303[];      // gFire_FlareWidths
extern u8 Data_0819830f[];      // gFire_FlareBiasY
extern u8 Data_08198309[];      // gFire_FlareHeights

// Embers spray out of the user, two a frame, then home in on the targets.

// Setup: up to 64 embers start at the top of the user's body with a random
// heading and speed. Frame loop: an ember flies for 48 frames (drawn MAX
// while young, then ADDITIVE), then accelerates toward its target, held to
// 60/64 of its speed until frame 85 of its life. Below the ground it lands:
// it leaves a six-picture flare, its target flashes and the screen shakes
// for two frames. The camera sways between frames 16 and 80. The variant
// chooses the number of embers.
void BattleEffect_RunFire(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 top;
  struct EffectPosition screen;
  DrawRectangle draw[2];
  struct MotionObject *user;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  if (effect->side == 1)
    BattleEffect_StartCanvasLayer(1);
  else
    BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  BattleEffect_LoadBlitter(104,
                BLIT_MODE(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  REG_BLDALPHA = 0x1010;

  // The embers leave the top of the user's body: a random heading and
  // speed across, -16 to 111 sixty-fourths of a pixel up.
  user = GetBattleObjectSlotFar(effect->actor)->object;
  top = user->y + BattleUnit_GetEffectHeightFar(effect->actor);
  for (i = 0; i != 64; i++) {
    struct EffectStep *ember = &EMBERS[i];
    s32 angle;
    s32 speed;

    angle = Random16();
    speed = (Random16() & 127) + 127;
    ember->velocityX = (Trig_Sin(angle) * speed) >> 6;
    ember->velocityY = (s32)(((Random16() & 127) - 16) << 16) >> 6;
    ember->velocityZ = (Trig_Cos(angle) * speed) >> 6;
    ember->x = user->x;
    ember->y = top;
    ember->z = user->z;
    ember->variant = IN_FLIGHT;
  }

  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Task_Add(Camera_ApplyPhasedDelta, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75; // each transfer fades the canvas to 3/4: a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != (Data_08198300[effect->variant] >> 1) + 132;
       frame++) {
    if (frame > 16 && frame < 80)
      work->cameraDelta = 0x100;
    else
      work->cameraDelta = 0;
    if (frame == (Data_08198300[effect->variant] >> 1) + 108)
      Func_081180e8(133);
    BattleCamera_SetViewFar(0, 0, 0, 100);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    // The embers in flight, two more each frame.
    for (i = 0; i != Data_08198300[effect->variant]; i++) {
      struct EffectStep *ember = &EMBERS[i];
      s32 start;

      start = i / 2;
      if (frame > start && ember->variant == IN_FLIGHT) {
        s32 size;

        // Depth 160-799 gives sizes 10 down to 1.
        EffectPosition_ProjectPoint((s32 *)ember, &screen);
        screen.x >>= 1;
        if (screen.depth < 160)
          screen.depth = 160;
        if (screen.depth > 799)
          screen.depth = 799;
        size = 10 - (screen.depth - 160) / 64;
        draw[frame < start + 48 ? 1 : 0](
            canvas, sheet + gData_08197410[size - 1], screen.x - size / 2,
            screen.y - size, size, size * 2);
        ember->x += ember->velocityX;
        ember->y += ember->velocityY;
        ember->z += ember->velocityZ;
      }

      // After 48 frames it steers toward its target by 1/512 of the
      // distance a frame.
      if (frame > start + 48 && ember->variant == IN_FLIGHT) {
        struct MotionObject *target;
        s32 vx;
        s32 vy;
        s32 vz;

        target =
            GetBattleObjectSlotFar(effect->actors[i % effect->count])->object;
        vx = ember->velocityX += (target->x - ember->x) >> 9;
        vy = ember->velocityY += (target->y - ember->y) >> 9;
        vz = ember->velocityZ += (target->z - ember->z) >> 9;
        if (frame < start + 85) {
          ember->velocityX = vx * 60 / 64;
          ember->velocityY = vy * 60 / 64;
          ember->velocityZ = vz * 60 / 64;
        }

        // Below the ground it lands: it keeps its screen position for the
        // flare, its target flashes in palette 10 and the screen shakes.
        if (ember->y < 0) {
          ember->variant = 0;
          ember->x = screen.x;
          ember->y = screen.y;
          if (effect->variant == 2)
            Sound_PlayFar(134);
          else
            Sound_PlayFar(136);
          Func_0814cd48(effect->actors[i % effect->count], 10, 5,
                        i % effect->count, 4);
          work->shakeFrames = 2;
        }
      }
    }

    // The landing flares: a picture every two frames, six in all.
    for (i = 0; i != Data_08198300[effect->variant]; i++) {
      struct EffectStep *ember = &EMBERS[i];

      if (ember->variant >= 0 && ember->variant < 12) {
        s32 cel;
        u32 width;

        cel = ember->variant / 2;
        draw[1](canvas, work->sheet + Data_08198316[cel],
                ember->x - ((width = Data_08198303[cel]) >> 1),
                ember->y + Data_0819830f[cel] - 56, width, Data_08198309[cel]);
        ember->variant++;
      }
    }

    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(Camera_ApplyPhasedDelta);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
