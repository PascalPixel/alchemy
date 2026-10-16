// First draft, differ 472 of 549: Func_0814b9e8, the body of three effect
// kinds through Func_0814b9c4 (mode 0), Func_0814b9d0 (mode 1) and
// Func_0814b9dc (mode 2) in games/tla/src/battle/effect/render_mode_entry.c.

// New in The Lost Age: no ☀️ function has this body (shape twin
// BattleFx_RunLeapingStrike 0.471, structure twin
// BattleEffect_RunDualParticleStream 0.494). Written from the listing on
// ☀️'s canvas skeleton: the canvas preparer Func_0815585c for modes 0 and
// 1, a Djinn circling over the canvas when the effect has a prelude, and
// sixteen textured 64x64 quads drawn through ⚓️'s render jobs
// (Func_081969f8, Func_08196958, Func_08196a7c), whose job and texture
// layouts follow project_vertices.c.

// The calls, constants and control flow follow the listing; the frame is
// 4 bytes short (84 of 88: one spill slot fewer), which shifts every stack
// offset, and the quads' starting heights are a giv the native
// strength-reduces into a register (0xffe00000 stepped by 0xfffc0000)
// where agscc shifts each time (555 of 549 instructions).

// Tried (differ):
// first draft from the listing                                      486
// the rising quads' start fixed to 32 << 16 (0x200000)              484
// greedy declaration-order scan: canvas, work, draw after the
// arrays                                                          472
// the quads' heights written as -0x200000 - i * 0x40000 (481 before
// the scan), -(i << 18) - 0x200000 (480), (-32 - i * 4) << 16
// (483), or as a running y stepped by -0x40000 (504)

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "fixed_math.h"
#include "resource.h"

#define HEAP_BLITTER 104 // ☀️ HEAP_SLOT_BLITTER

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define CALLBACK_ORDER 0xc80
#define QUAD_FRAMES 64
#define QUAD_COUNT 16

// Where the quads' texture and the Djinn sit in the sheet.
#define SHEET_TEXTURE 0x2710
#define SHEET_DJINN 0x65c0

// The kinds this body draws, chosen by the effect's entry.
enum {
  QUADS_MERCURY, // over a prepared canvas, with the Mercury Djinn
  QUADS_MARS,    // over a prepared canvas, with the Mars Djinn
  QUADS_RISING   // rising from the first target
};

// A texture as the render jobs take it: 1 << logWidth by 1 << logHeight.
struct Texture {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  void *pixels;
};

// A render job (the first of Func_081969f8's entries).
struct RenderJob {
  s32 kind;
  s32 next;
  const void *faces;
  void *vertices;
  struct Texture *texture;
  s32 fade;
  s32 tail;
};

extern char ResourceId_Data0D0;           // directory row 0xd0
extern char ResourceId_FeatherSheet;      // directory row 0xd1
extern char ResourceId_MercuryDjinnSheet; // directory row 0x154
extern char ResourceId_BlueFlameSheet;    // directory row 0x130
extern char ResourceId_MarsDjinnSheet;    // directory row 0x150

extern s32 gCameraSceneParameters[]; // unsized: sized, word 4 folds
extern const s8 gData_08199210[];    // a quad's four corners, x y z for each
extern const u8 gData_08199340[];    // the quad's face

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void);               // ☀️ BattleFx_EndCanvasLayer
void Func_08143000(void);               // ☀️ ProcessPendingGraphicsTransfer
void Func_081434f8(void);               // ☀️ ObjectGroup_TickMemberTimers
void Func_081180e8(s32 phase);          // ☀️ BattleEventRuntime_BeginPhaseFar
s32 Func_081963ec(s32 slot, s32 flags); // ☀️ BattleEffect_LoadWork
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void Func_0815585c(struct BattleEffectArgument *effect, s32 flags, s32 *anchor,
                   s32 *size); // the canvas preparer
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id,
                                                 struct EffectPosition *pos);
struct RenderJob *Func_081969f8(s32 count);
void Func_08196958(const s8 *vertices, void *projected, s32 count);
void Func_08196a7c(struct RenderJob *job);
void Func_08015160(s32 x, s32 y, s32 z); // translate
void Func_080151e4(s32 x, s32 y, s32 z); // scale
void Func_080150e4(s32 angle);           // roll
void SceneTransform_ApplyPitch(s32 angle);
void Render_ResetTransformState(void);
void Audio_PlayCue(s32 cue);

// Sixteen textured quads tumble down over the targets (or, from the first
// target, rise), three frames apart, for 64 frames; with a prelude a Djinn
// circles over the canvas and a fan of light lines spreads from it. At
// frame 56 every target flashes.
void Func_0814b9e8(struct BattleEffectArgument *effect, s32 mode) {
  s32 anchor[3];
  s32 size[3];
  struct EffectPosition pos;
  struct Texture texture;
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw;
  struct RenderJob *job;
  void *vertices;
  struct EffectStep *quad;
  s32 frame;
  s32 i;
  s32 x;
  s32 y;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_BeginCanvasLayer(0);

  // The mode's canvas, sheets and palette.
  if (mode == QUADS_MERCURY) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 33, anchor, size);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D0,
                               work->sheet + SHEET_TEXTURE, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SHEET_DJINN, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
  } else if (mode == QUADS_MARS) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 2, anchor, size);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SHEET_DJINN, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FeatherSheet,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  } else {
    EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &pos);
    anchor[0] = pos.x;
    Resource_LoadAndDecompress((s32)&ResourceId_FeatherSheet,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(Func_08143000, CALLBACK_ORDER);

  // Centre the canvas on the anchor; a copy blitter, or an additive one for
  // side 1.
  REG_BG2PA = 0x100;
  REG_BG2X = (64 - anchor[0]) << 8;
  if (effect->side == 0)
    Func_081963ec(HEAP_BLITTER, BLITTER_COPY << 4 | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  else
    Func_081963ec(HEAP_BLITTER,
                  BLITTER_ADDITIVE << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (frame = 0; frame != QUAD_FRAMES; frame++) {
    if (frame == 8)
      Audio_PlayCue(246);

    // The quads start above the screen (or at the target for mode 2), each
    // with its own heading, spin and size.
    if (frame == 0) {
      for (i = 0; i != 32; i++) {
        quad = &work->particles[i];
        quad->x = Random16() & 0xffff;
        Random16();
        quad->z = 1 << 16;
        if (mode == QUADS_RISING)
          quad->y = 32 << 16;
        else
          quad->y = (-8 - i) << 18;
        quad->velocityY = ((Random16() & 0xffff) << 1) + 0x10000;
        quad->velocityX = Random16() & 0xffff;
        quad->velocityZ = Random16() & 0xffff;
        quad->variant = (Random16() & 0x3fff) + 0x2000;
      }
    }

    // Frame 56: the next phase, and every target flashes.
    if (frame == 56) {
      Audio_PlayCue(212);
      Func_081180e8(-1);
      for (i = 0; i != effect->count; i++)
        Func_0814cd48(effect->actors[i], 7, -1, i, 8);
    }

    // With a prelude the Djinn circles over the anchor and climbs away
    // after frame 32, and a fan of light lines spreads from the canvas's
    // middle while the canvas fades.
    if (effect->prelude == 1) {
      x = ((-Trig_Sin(frame << 11) * 20) >> 16) + 44;
      y = ((Trig_Cos(frame << 11) * 4) >> 16) + anchor[1] - 24;
      if (frame > 32)
        y = y - frame * 4 + 128;
      draw(canvas, work->sheet + SHEET_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draw(canvas, work->sheet + SHEET_DJINN, x, y, 40, 40);
      if (frame <= 79) {
        s32 angle;

        Iwram_FillWords(work->sheet, 128, 0x28282828 - frame / 2 * 0x01010101);
        angle = frame << 9;
        for (i = 0; i != 16; i++) {
          x = (Trig_Sin(angle) * frame) >> 16;
          y = (Trig_Cos(angle) * frame) / 8 >> 16;
          draw(canvas, work->sheet, x + 64, y - 16, 1, 128);
          angle += 0x1000;
        }
      }
    }

    // The quads: each a render job of one textured face, placed by its own
    // translation, scale, pitch and roll.
    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;
    if (frame <= 63) {
      vertices = Runtime_BumpAllocateAlternatePool(32);
      job = Func_081969f8(1);
      job->fade = frame > 55 ? (56 - frame) * 8 : 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + SHEET_TEXTURE;
      job->kind = 6;
      job->texture = &texture;
      job->faces = gData_08199340;
      job->vertices = vertices;

      quad = work->particles;
      for (i = 0; i != QUAD_COUNT; i++, quad++) {
        s32 scale = quad->variant;

        if (frame >= i * 3) {
          s32 across;
          s32 deep;

          Render_ResetTransformState();
          SceneTransform_ApplyPitch(3000);
          if (mode == QUADS_RISING)
            quad->y -= quad->velocityY;
          else
            quad->y += quad->velocityY;
          if (quad->velocityY < -0x20000)
            quad->z += 0x10000;
          else
            quad->z += 0xc000;
          across = Trig_Sin(quad->x) * ((s16 *)&quad->z)[1];
          deep = Trig_Cos(quad->x) * ((s16 *)&quad->z)[1];
          quad->x += 0x200;
          Func_08015160(across, quad->y, deep);
          Func_080151e4(scale, scale, scale);
          SceneTransform_ApplyPitch(quad->velocityX);
          Func_080150e4(quad->velocityZ);
          quad->velocityX += 0x400;
          quad->velocityZ += 0x400;
          Func_08196958(gData_08199210, vertices, 4);
          Func_08196a7c(job);
        }
      }
      MemFree(job);
      MemFree(vertices);
    }
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down.
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Scheduler_RemoveCallback(Func_08143000);
  Func_08143bb8();
}
