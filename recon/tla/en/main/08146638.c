// Near miss, differ 18 of 480.
//
// Measured 2026-10-25 under -mthumb-slow-loads: differ 18. Most rows left are
// loads from memory or the stack that alchemy-gcc issues one slot earlier than
// the native, which issues only its pool loads early. The first is the
// argument's spill, which the native stores before it starts the 0x03000000
// base; an empty asm before the first statement closes that row (differ 16).

// Twin: The Broken Seal's BattleEffect_RunAstralBlast (effect kind 68 there and
// here), ported call for call. The Lost Age's changes: the effect argument is
// read directly (the dispatcher stores work->effect), the rune palette and
// resource 0x134 as in BattleEffect_RunShiningStar, Object_SetMode for the caster where The
// Broken Seal dispatches an argument to its children, one blitter from
// BattleEffect_LoadBlitter(104, MAX), the callback at 0xc80, and the target thrown with
// Object_ResetMotion and Object_SetPosition. The tables are not const (see
// BattleEffect_RunShiningStar).

// BattleEffect_LoadBlitter is declared to return s32, as The Lost Age's LoadWork
// returns its store-flag and The Broken Seal's returns s32: void it is 36.

// Left: order only. Same frame, registers and instructions; at about fifteen
// rows the native issues a memory access (the argument's spill, a work slot,
// a spill reload or a field read) one instruction ahead of the constant or
// move beside it.

#include "battle.h"
#include "graphics.h"
#include "lib.h"
#include "resource.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "object_runtime.h"
#include "ram_buffer.h"

// Heap blocks are named by their byte offset in Ram_HeapSlots.
#define HEAP_OFFSET(field) ((s32)&((struct HeapSlots *)0)->field)
#define HEAP_BLITTER HEAP_OFFSET(unk64[1])
#define HEAP_BLITTER_ALTERNATE HEAP_OFFSET(unkb8[1])

// The blitter generator's modes, The Broken Seal's in reverse order.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);         // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void);         // ..ProcessPendingGraphicsTransfer
void ObjectGroup_CountDownMemberTimers(void);         // ObjectGroup_CountDownMemberTimers
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void Func_081180e8(s32 phase);    // BattleEventRuntime_StartPhaseFar
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames); // ObjectGroup_UpdateMembers
void Func_080150e4(s32 angle);  // SceneTransform_ApplyRoll
void Func_080151ac(s32 *scale); // SceneTransform_ApplyScale
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPosition(void *position);
void Transform_Pop(void);

// The camera's heading, as the camera work holds it.
struct CameraView {
  u8 unk00[0x36];
  u16 yaw;
};

extern char ResourceId_RuneSheet;
extern char ResourceId_Data134;
extern s32 gData_08197924[]; // the two ring points, alternating
extern u16 gData_08197410[]; // dot sizes 1 to 10: cell offsets in the sheet
extern u8 gMapCellBuffer[];

#define BOLT_COUNT 3
#define RING_POINTS 10
#define RING_STEPS 16 // dots drawn between two ring points
#define FAR_DEPTH 399999

#define ACTOR(id) ((struct ObjectRuntime *)GetBattleObjectSlotFar(id)->object)

// Three rings of light fly one after another from the caster to the target,
// each growing as it spins, while the camera swings round; each ring that
// lands throws the target back, and the last begins the results.
void BattleEffect_RunAstralBlast(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition base;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct ObjectRuntime *caster;
  struct ObjectRuntime *source;
  struct ObjectRuntime *target;
  struct EffectStep *point;
  struct EffectStep *bolt;
  s32 start;
  s32 nearest;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  caster = ACTOR(effect->actor);

  // Set up: the rune palette, the dot sheet, the caster's casting pose, one
  // max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Object_SetMode(caster, 2);
  ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)caster, 48);
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 2;
  work->transferValue = 75;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The rings leave from above the caster and reach the target in twelve
  // frames; the second and third aim past it.
  source = ACTOR(effect->actor);
  target = ACTOR(effect->actors[0]);
  for (i = 0, point = work->particles; i != BOLT_COUNT; i++) {
    point->x = source->x;
    point->y = source->y + 0x280000;
    point->z = source->z;
    if (i == 0)
      point->velocityX = (target->x - point->x) / 12;
    else
      point->velocityX = (target->x * 2 - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x280000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = 0;
    point++;
  }

  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 60; frame++) {
    // The camera turns toward the caster's side, slowing from frame 40.
    if (frame <= 47) {
      struct CameraView *view = (struct CameraView *)camera;
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (effect->side == 0)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }

    // A new ring every twelve frames: it spins and grows from a dot to
    // full size, and is drawn while it is near enough to the camera.
    for (i = 0; i != BOLT_COUNT; i++) {
      start = i * 12;
      if (frame >= start) {
        bolt = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > 10)
          size = 10;
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        SceneTransform_ApplyPosition(bolt);
        nearest = 0;
        for (j = 0; j != RING_POINTS; j++) {
          s32 depth;

          Transform_Push();
          Func_080150e4((frame - start) << 10);
          SceneTransform_ApplyYaw(0x4000);
          scale[0] = ((frame - start) << 12) + 0x1000;
          if (scale[0] > 0x10000)
            scale[0] = 0x10000;
          scale[1] = scale[0];
          scale[2] = scale[0];
          Func_080151ac(scale);
          Func_080150e4(j * 0x199a);
          depth = EffectPosition_ProjectPoint(
              &gData_08197924[(j & 1) * 3], &pos);
          if (nearest < depth)
            nearest = depth;
          pos.x >>= 1;
          // The first two stores read base, which nothing sets, and are
          // overwritten at once; both games' code keeps them.
          ring[i * 10 + j].velocityX = pos.x + base.x;
          ring[i * 10 + j].velocityY = pos.y + base.y;
          ring[i * 10 + j].velocityX = pos.x;
          ring[i * 10 + j].velocityY = pos.y;
          Transform_Pop();
        }
        if (nearest <= FAR_DEPTH) {
          for (j = 0; j != RING_POINTS; j++) {
            struct EffectStep *from =
                &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
            struct EffectStep *to =
                &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

            for (k = 0; k != RING_STEPS; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              draw[0](canvas, graphics + gData_08197410[size - 1], x - size / 2,
                      y - size, size, size * 2);
            }
          }
        }
        bolt->x += bolt->velocityX;
        bolt->y += bolt->velocityY;
        bolt->z += bolt->velocityZ;

        // Ten frames after it starts (plus one per ring) the ring lands: the
        // target is thrown back 40 pixels and up, and flashes; the third ring
        // begins the results instead.
        if (frame == start + i + 10) {
          target->acceleration = 0x20000;
          target->speedLimit = 0x80000;
          target->velocityY = 0x50000;
          target->gravity = 0xab85;
          target->actionFlags = 0;
          Object_ResetMotion(target);
          if (target->x < 0)
            Object_SetPosition(target, target->x - 0x280000, 0, target->z);
          else
            Object_SetPosition(target, target->x + 0x280000, 0, target->z);
          if (i == 2) {
            Func_081180e8(134);
          } else {
            Audio_PlayCue(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    Func_08158ce0(8, 8);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
