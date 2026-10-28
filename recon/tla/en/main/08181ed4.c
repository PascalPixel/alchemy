// 2026-10-26 (round 5): differ 112. spawned is declared in a block opened after
// BattleEffect_FetchRectangleBlitters, so its pseudo comes after the blitter
// pair's address and the two spill slots take the native's places (the pair's
// address at sp+16, spawned at sp+12): 120 to 112; the target's actor read
// straight from effect->actors before Func_081180e8 (without the actors
// pointer) keeps 112. Left: in the spawn loop the 1 and the ring counter swap
// r5 and r7, and scheduling rows (the prologue's heap loads, the palette copy's
// address shift, the frame-20 block).
// 2026-10-26: differ 120 of 513, with Scheduler_AddOrUpdateCallback returning
// s32 as defined.
// Draft (tla-en): listing 513 instructions, draft 513, common 442, differ 142.
// The body of effect kinds 114, 461 and 462 (summon_modes_1.c's
// BattleEffect_RunPurgatory, BattleEffect_RunDjinnBlast and
// BattleEffect_RunDjinnStorm pass modes 0, 2 and 1). Written from the listing
// on ☀️'s skeleton (2026-10-15, lane N2); its nearest ☀️ shape is
// BattleEffect_RunPlasma (0.50), which is no twin. The statements, frame
// (texture sp+40, the blitter pair sp+48, midpoint sp+56) and the instruction
// count match. Left:
// - the spill slots of the blitter pair's address and of "spawned" are
// swapped (native: the pair's address at sp+16, spawned at sp+12), and
// every access to either is one row off; declaring spawned last changes
// nothing (the address is a pseudo made after the declarations).
// - in the drop spawn loop the native keeps the speed in r7 and the angle
// in r5, here the other way round; &midpoint takes r5 there, r7 here.
// - REG_BLDALPHA's two values each load their pool word before the
// register address's shift; here after it.
// - small scheduling rows in the draw call's argument setup and the
// column's scale branches.
// Measured: the first draft 271; the mode-1 test nested as its own if, so
// frame 1..23 compiles as one unsigned compare: 264; Trig_Sin(angle) *
// speed (the native copies speed and multiplies by the result): 258;
// speed's +32 folded into its first assignment: 240; speed and angle
// declared the other way round: no change.
// 2026-10-19 (run 33): alchemy-devtools permute (5 minutes) found the target's
// actors pointer taken before Func_081180e8 at frame 20: 240 to 142. It
// holds r5 across the call, which moves the drop loop's speed to r7 and the
// angle to r5 as the native has them; the frame-20 block itself is now
// three rows off (the native reloads effect after the call). The swapped
// spill slots (the pair's address and spawned) remain. A pointer variable
// for the blitter pair instead: 296. A second run from 142 (8 minutes)
// reached 120 only by reading an uninitialized temporary: not taken.
// Resources 0xc0 (BattleEffect_CrossTextureSheet), 0xd2 (Resource_Data0D2) and
// the Djinn sheets 0x154, 0x150 and 0x152 need names in
// system/resource/directory.s when this is adopted.
// 2026-10-26: 127 under K2's -mthumb-slow-loads; the target's actor read
// into its own local before Func_081180e8 at frame 20: 122 (permute, 20
// minutes, found nothing better that was plain C).
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "types.h"

#define HEAP_SLOT_BLITTER 104           // Ram_HeapSlots->unk64[1]
#define HEAP_SLOT_BLITTER_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

#define GEYSER_FRAMES 54
#define GEYSER_DRAW_FRAMES 40
#define GEYSER_PEAK_FRAME 20 // the result phase starts
#define DROP_COUNT 512       // drops in the map cell buffer, used in a ring

// A texture: log2 of its width and height in the first word, then its
// pixels.
struct Texture {
  u32 size;
  void *pixels;
};

// One entry of the list Render_DrawModelList draws; kind 7 is a textured mesh
// whose texture scrolls by scrollU and scrollV.
struct RenderPrimitive {
  s32 kind;
  s32 unk04;
  const void *faces;
  void *vertices;
  struct Texture *texture;
  s32 brightness;
  u8 scrollU;
  u8 scrollV;
};

extern char ResourceId_Data134;
extern char ResourceId_CrossTextureSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0D2;
extern char ResourceId_BlastSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_JupiterDjinnSheet;

// The life of each drop in the map cell buffer: gMapCellBuffer's
// EffectSteps from their variant field on.
extern s32 Data_02010018[];

// Per mode: the drops' gravity, and their smallest cel.
extern const s32 Data_08199660[];
extern const s8 Data_0819966c[];

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern const u16 gData_08197410[];

// The geyser: a mesh of sixteen corners and its faces.
extern const u8 gData_08198c6c[];
extern const u8 Data_08198df8[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_FetchRectangleBlitters(s32 side, DrawRectangle *pair);
void *Resource_GetTableEntry(s32 resourceId);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void Func_0815e22c(struct EffectPosition *midpoint);
void Func_0815f0a0(s32 resourceId);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
void Func_08118088(s32 actor, s32 motion);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct RenderPrimitive *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void Func_080151e4(s32 x, s32 y, s32 z);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderPrimitive *list);
void MemFree(void *buffer);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);

// A geyser between the targets. A textured column, its texture scrolling
// upward, stands on the floor at the targets' midpoint, and drops burst
// from its foot, fall back under gravity and bounce once on the floor.
// Mode 0 (kind 114): the cross texture in orange; the column rises and
// falls on a sine and 200 drops burst at once. Mode 1 (kind 462): the
// column keeps widening and fades from frame 32, with 200 drops and 16 more
// each frame to frame 23. Mode 2 (kind 461): a thin column with 100 drops
// and 2 more each frame. In modes 1 and 2 the palette also steps through
// the Mercury, Mars and Jupiter Djinn's. At frame 20 the target is knocked
// back and the result phase starts.
void Func_08181ed4(struct BattleEffectArgument *effect, s32 mode) {
  struct Texture texture;
  DrawRectangle draw[2];
  struct EffectPosition midpoint;
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  s16 actor;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *sparkCels = Ram_HeapSlots->unk64[0];
  struct EffectStep *drop;
  struct RenderPrimitive *column;
  void *vertices;
  s32 frame;
  s32 i;
  s32 count;
  s32 scale;
  s32 brightness;

  BattleEffect_StartCanvasLayer(1);
  if (mode == 0)
    REG_BLDALPHA = 0x0c10;
  else
    REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    s32 spawned;
    work->transferMode = 2;
    work->transferValue = 75;
    Scheduler_AddOrUpdateCallback(
        (s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
    spawned = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
    if (mode == 0) {
      Resource_LoadAndDecompress((s32)&ResourceId_CrossTextureSheet,
                                 work->sheet, 1, 1);
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
    } else {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0D2, work->sheet, 0, 0);
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_BlastSheet), 128);
    }

    for (frame = 0; frame != GEYSER_FRAMES; frame++) {
      // The geyser follows the targets' midpoint while it rises.
      if (frame < GEYSER_PEAK_FRAME)
        Func_0815e22c(&midpoint);
      if (frame == 0) {
        Sound_PlayFar(144);
        work->shakeFrames = 16;
      }
      if (frame == GEYSER_PEAK_FRAME) {
        work->shakeFrames = 8;
        actor = effect->actors[0];
        Func_081180e8(134);
        Func_08118088(actor, 1);
      }

      // The first frame clears every drop.
      if (frame == 0) {
        s32 *life = Data_02010018;

        spawned = 0;
        for (i = 0; i != DROP_COUNT; i++) {
          *life = -1;
          life += 7;
        }
      }

      // Modes 1 and 2 step the palette three times a frame from frame 8:
      // toward the Mercury Djinn's, from frame 20 the Mars Djinn's and from
      // frame 32 the Jupiter Djinn's.
      if (mode == 1 || mode == 2) {
        for (i = 0; i != 3; i++) {
          if (frame >= 8) {
            if (frame < 20)
              Func_0815f0a0((s32)&ResourceId_MercuryDjinnSheet);
            else if (frame < 32)
              Func_0815f0a0((s32)&ResourceId_MarsDjinnSheet);
            else
              Func_0815f0a0((s32)&ResourceId_JupiterDjinnSheet);
          }
        }
      }

      // How many drops burst this frame.
      count = 0;
      if (mode == 2) {
        if (frame == 0)
          count = 100;
        if (frame >= 1 && frame < 24)
          count = 2;
      } else {
        if (frame == 0)
          count = 200;
        if (mode == 1 || mode == 2) {
          if (frame >= 1 && frame < 24)
            count = 16;
        }
      }
      for (i = 0; i != count; i++) {
        s32 speed;
        s32 angle;

        drop = &((struct EffectStep *)Ram_MapCellBuffer)[spawned & 511];
        speed = (Random16() & 1023) + 32;
        angle = (Random16() & 0x7fff) - 0x4000;
        drop->x = (midpoint.x / 2) << 16;
        drop->y = 108 << 16;
        if (mode == 2)
          drop->velocityX = (Trig_Sin(angle) * speed) >> 8;
        else
          drop->velocityX = (Trig_Sin(angle) * speed) >> 6;
        drop->velocityY = -(Trig_Cos(angle) * speed * 2) >> 7;
        drop->variant = (Random16() & 7) + 48;
        spawned++;
      }

      // Each drop falls, bounces off the floor at y 120, and is drawn while
      // it is on the canvas, its cel shrinking as it burns down.
      drop = (struct EffectStep *)Ram_MapCellBuffer;
      for (i = 0; i != DROP_COUNT; i++, drop++) {
        if (drop->variant > 0) {
          s32 x;
          s32 y;

          drop->variant--;
          BattleEffectKernels_IntegrateVector2(drop, 60, Data_08199660[mode]);
          y = drop->y >> 16;
          if (y > 120) {
            drop->velocityY = -drop->velocityY / 2;
          } else if (drop->x >= 0 && (x = drop->x >> 16) <= 126 &&
                     drop->y >= 0) {
            s32 size = (drop->variant - 16) / 8;

            if (size < Data_0819966c[mode])
              size = Data_0819966c[mode];
            draw[i & 1](canvas, sparkCels + gData_08197410[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }

      // The column, for the first 40 frames.
      if (frame < GEYSER_DRAW_FRAMES) {
        vertices = Runtime_BumpAllocateAlternatePool(512);
        column = Render_AllocateModelList(1);
        texture.size = (texture.size & ~0xff) | 5;
        texture.size = (texture.size & ~0xff00) | 5 << 8;
        texture.pixels = work->sheet;
        column->scrollU = (frame << 2) & 31;
        column->scrollV = (frame << 1) & 31;
        Render_ResetTransformState();
        Func_08015160((midpoint.x / 2 - 64) << 16, -16 << 16, 0);
        SceneTransform_ApplyPitch(2000);
        if (mode == 0) {
          // Up on a sine in 16 frames, held, and down again from frame 24.
          if (frame < 16)
            scale = Trig_Sin(frame << 10);
          else {
            scale = 0x10000;
            if (frame > 23)
              scale = Trig_Sin((frame << 10) - 0x2000);
          }
          brightness = -32;
        } else if (mode == 1) {
          // Wider every frame, fading from frame 32.
          scale = frame << 12;
          brightness = -32;
          if (frame > 31)
            brightness = 224 - (frame << 3);
        } else {
          scale = 0x4000;
          brightness = -32;
          if (frame > 31)
            brightness = 224 - (frame << 3);
        }
        column->kind = 7;
        column->brightness = brightness;
        column->faces = Data_08198df8;
        column->texture = &texture;
        column->vertices = vertices;
        Func_080151e4(scale * 2, 7 << 16, scale * 2);
        Render_TransformVertices(gData_08198c6c, vertices, 16);
        Render_DrawModelList(column);
        MemFree(column);
        MemFree(vertices);
      }

      Func_08158ce0(4, 16);
      ObjectGroup_CountDownMemberTimers();
      work->transferPending = 1;
      WaitFrames(1);
    }
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
