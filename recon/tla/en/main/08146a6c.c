// Near miss, differ 30 of 410.

// Twin: The Broken Seal's BattleEffect_RunShiningStar (effect kind 62 there and
// here), ported call for call. The Lost Age's changes: the effect argument is
// read directly (the dispatcher stores work->effect), resource 0x134 is
// unpacked into the sheet slot where The Broken Seal decodes its particle
// sprites, the one blitter comes from BattleEffect_LoadBlitter(104, MAX) before the
// transfer settings, the callback runs at 0xc80, and the radii are bytes.
// The two tables are not const: as const arrays loop.c hoists the cell offset
// out of the draw loops and spills it (differ 182).

// BattleEffect_LoadBlitter is declared to return s32, as The Lost Age's LoadWork
// returns its store-flag and The Broken Seal's returns s32: void it is 32
// (alchemy permute, 12 minutes, found this and nothing else).

// Left: order only. Same frame, registers and instructions; at ten rows
// the native issues a load (the argument's spill, a work slot, a spill reload,
// a sine operand) one instruction before the constant or store beside it,
// starting with the prologue's str r0, [sp, #52], which agscc places after
// the 0x03000000 base.

#include "battle.h"
#include "graphics.h"
#include "resource.h"
#include "lib.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "io_reg.h"
#include "effect_step.h"
#include "iwram_call.h"
#include "motobj.h"
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
void ObjectGroup_TickMemberTimers(void);         // ObjectGroup_TickMemberTimers
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void Func_081180e8(s32 phase);    // BattleEventRuntime_StartPhaseFar
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames); // ObjectGroup_UpdateMembers
void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase,
                   s32 bluePhase); // Graphics_UpdatePhasePalette
void Audio_PlayCue(s32 cue);

extern char ResourceId_RuneSheet;
extern char ResourceId_Data134;
extern u8 gData_0819793c[];  // ring radii, alternating: 32, 16
extern u16 gData_08197410[]; // dot sizes 1 to 10: cell offsets in the sheet
extern u8 gMapCellBuffer[];

#define STAR_COUNT 8
#define RING_POINTS 10
#define RING_STEPS 12 // dots drawn between two ring points
#define DOT_SIZE 2
#define GROUND_Y 0x1dffff

// Eight rings of stars fly from the caster to the target, one every eight
// frames and spinning as they go; each bounces where it meets the ground,
// shaking the screen and flashing the targets.
void BattleEffect_RunShiningStar(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *star;
  struct EffectPosition pos;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the rune palette, the dot sheet, one max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each star leaves from above the caster on its own course to the target,
  // the later ones after a longer delay (held in variant).
  source = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0, point = work->particles; i != STAR_COUNT; i++) {
    s32 delay = i * 8;

    point->x = source->x / 2;
    point->y = source->y + 0x780000;
    point->z = source->z;
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x140000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = DOT_SIZE;
  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 128; frame++) {
    Func_081496c8(frame, 0xaaab, 0x5555, 0);
    if (frame == 96)
      Func_081180e8(134);
    for (i = 0; i != STAR_COUNT; i++) {
      star = &work->particles[i];
      if (frame >= star->variant) {
        // Project the star and, while it is on screen, lay out its ring of
        // ten points (the velocity fields of the map buffer's steps), then
        // join each point to the next with a row of dots.
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        EffectPosition_ProjectPoint((s32 *)star, &pos);
        pos.x >>= 1;
        if (pos.x >= -8 && pos.x <= 127) {
          if (pos.y <= 127) {
            if (pos.y >= -8) {
              for (j = 0; j != RING_POINTS; j++) {
                ring[i * 10 + j].velocityX =
                    pos.x +
                    ((Trig_Sin(j * 0x199a - ((frame - star->variant) << 11)) *
                      gData_0819793c[j & 1]) /
                         2 >>
                     16);
                ring[i * 10 + j].velocityY =
                    pos.y -
                    (Trig_Cos(j * 0x199a - ((frame - star->variant) << 11)) *
                         gData_0819793c[j & 1] >>
                     16);
              }
              for (j = 0; j != RING_POINTS; j++) {
                struct EffectStep *from = &ring[j + i * 10];
                struct EffectStep *to = &ring[i * 10 + (j + 1) % 10];

                for (k = 0; k != RING_STEPS; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  draw[0](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
                }
              }
            }
          }
        }

        // At the ground the star bounces up at half its sideways speed.
        if (star->y <= GROUND_Y) {
          star->velocityY = -star->velocityY;
          star->velocityX /= 2;
          star->velocityZ /= 2;
          work->shakeFrames = 4;
          Audio_PlayCue(134);
          for (j = 0; j != effect->count; j++)
            Func_0814cd48(effect->actors[j], 7, 5, j, 8);
        }
        star->x += star->velocityX;
        star->y += star->velocityY;
        star->z += star->velocityZ;
      }
    }
    Func_08158ce0(4, 4);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
