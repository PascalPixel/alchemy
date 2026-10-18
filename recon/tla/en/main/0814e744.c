// Near miss of BattleFx_RunBreak, effect kind 43: differ 36 of 337 (draft 337).

// Twin: The Broken Seal's BattleFx_RunBreak (effect.c), call for
// call. The Lost Age reads the effect from its argument (it never stores
// work->effect), takes its heap blocks as Ram_HeapSlots fields, loads the
// blitter through BattleFx_LoadBlitter (BLIT_MAX is 1 where The Broken Seal's
// COMPOSITE_MAX is 2) and orders the callback 0xc80.

// Left: about a dozen rows where the native issues a load one slot earlier
// than agscc (canvas before the 1 for BeginCanvasLayer, the blitter word
// before the loop's zero, object->z before its store, the cell offset table
// before the size), and the member loop's three invariants (the actor
// offset 36, &screen, &point) set up in the reverse order.

// Tried:
// - member declared before sheet: 56 -> 36 (sheet and member swap their
// frame slots sp+16 and sp+20 into the native's).
// - greedy declaration-order sweep from there, two passes: no gain (36).

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void); // BattleFx_ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleFx_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_BeginPhaseFar
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 Battle_GetObjectTableValueFar(s32 actor);

// The Lost Age's blitter modes, BattleFx_LoadBlitter's high nibble: 3 minus The Broken
// Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))

#define SHARDS_PER_MEMBER 128
#define SHARDS ((struct EffectStep *)Ram_MapCellBuffer)
#define HI(v) (((s16 *)&(v))[1]) // the pixel half of a 16.16 value

extern char ResourceId_Data134;       // the particle sheet
extern char ResourceId_IceBlockSheet; // its palette
extern u16 gData_08197410[];          // gBattleFx_MoteCellOffsets
extern s32 gData_081982e4[];          // gBreak_Gravities

// Ice shards burst up from each target in turn, 20 frames apart.

// Setup: 128 shards per target are scattered around the middle of its body
// and thrown sideways and up. Frame loop: from its cue a target flashes, and
// its shards fall under one of four gravities until they drop below y = 112.
// The effect runs 20 frames per target plus 56.
void BattleFx_RunBreak(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  s32 *camera;
  s32 point[3];
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
  BattleFx_LoadBlitter(104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (i = 0; i != 8 * SHARDS_PER_MEMBER; i++)
    SHARDS[i].variant = -1;

  Render_ResetTransformState();
  Transform_SetLookAt(camera, camera + 3);

  // Scatter each target's shards around the middle of its body.
  for (member = 0; member != effect->count; member++) {
    struct MotionObject *object;
    s32 offset;
    s32 height;
    struct EffectStep *shard;

    offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
    object = GetBattleObjectSlotFar(effect->actors[member])->object;
    height = Battle_GetObjectTableValueFar(effect->actors[member]) / 2;
    point[0] = object->x;
    point[1] = height;
    point[2] = object->z;
    EffectPosition_ApplyBaseAndYOffset(point, &screen);
    screen.x >>= 1;
    for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
         i != SHARDS_PER_MEMBER; i++) {
      s32 radius;
      s32 angle;
      s32 lift;

      radius = Random16() & 0xff;
      angle = Random16() & 0xffff;
      shard->x = ((Trig_Sin(angle) * radius) >> 7) + (screen.x << 16);
      shard->y = ((Trig_Cos(angle) * radius) >> 3) + (screen.y << 16);
      shard->velocityX = (128 - (Random16() & 0xff)) << 9;
      lift = Random16() & 0xff;
      shard->variant = 0;
      shard->velocityY = (-lift - 128) << 10;
      shard++;
    }
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != effect->count * 20 + 56; frame++) {
    if (frame == 32)
      Func_081180e8(0);
    for (member = 0; member != effect->count; member++) {
      s32 offset;

      offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
      // The target's cue: a chime and a flash.
      if (frame == member * 20) {
        Audio_PlayCue(143);
        Func_0814cd48(effect->actors[member], 7, -1, member, 20);
      }
      // Its shards fly, and go out once they fall past y = 112.
      if (frame > member * 20) {
        struct EffectStep *shard;

        for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             i != SHARDS_PER_MEMBER; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            draw(canvas, sheet + gData_08197410[size - 1],
                 HI(shard->x) - size / 2, HI(shard->y) - size, size, size * 2);
            BattleFxKernels_IntegrateVector2(shard, 62, gData_081982e4[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 && HI(shard->y) > 112)
              shard->variant = -1;
          }
          shard++;
        }
      }
    }
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleFx_EndCanvasLayer();
}
