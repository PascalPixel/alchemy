// Differ 2 of 549 (tla-en, 2026-11-08, -mtune=gba).
// 2026-11-08: the pair is a sched2 LUID tie. Loop puts the i * 3 giv's init
// after quad = work (insn order i, quad, giv), so the reload of work wins over
// the giv's spill store. Without gain: quad set earlier or in the for list
// first, a delay counter at every declaration position (30 to 72), quad from
// &work->particles[i], once-only blocks, non-const tables, 3 * i, and 10
// permuter minutes. The inline CopyWords_ is still needed (4 without it).
// Measured 2026-11-05 (tla-en): differ 2 of 549, no device. Left: one pair, the
// loop's opening: ldr r6, [sp, #32] (quad) comes before str r2, [sp, #12] (i * 3's
// start) here and after it in the listing. A delay variable set before quad gives
// that order but swaps the slots at sp+12 and sp+16 (24 rows).
// Settled today: draw is the first of a two-element array declared after canvas.
// sp+24 is its unread second element, and as an array each use reads the stack
// again, so r7 is free to reload it across both Djinn calls; that gives the
// listing's caller-save of the sheet pointer in r4 and x in r8, with no barrier.
// The quads' delay is i * 3 (the slot at sp+12 is a giv), and the step of quad
// belongs in the for update, after i++. The older notes below predate all this.

// Draft, differ 183 of 549, skeleton gap 31 (tla-en, 2026-11-01)
// Func_0814b9e8, the body of three effect kinds through BattleEffect_RunBalm
// (mode 0), BattleEffect_RunTinder (mode 1) and BattleEffect_RunPhoenix
// (mode 2) in games/tla/src/battle_effect/render_mode_entry.c.

// New in The Lost Age: no ☀️ function has this body (shape twin
// BattleEffect_RunPlanetDiver 0.471, structure twin
// BattleEffect_RunDragonCloud 0.494). Written from the listing on
// ☀️'s canvas skeleton: the canvas preparer Func_0815585c for modes 0 and
// 1, a Djinn circling over the canvas when the effect has a prelude, and
// sixteen textured 64x64 quads drawn through ⚓️'s render jobs
// (Render_AllocateModelList, Render_TransformVertices, Render_DrawModelList), whose job and texture
// layouts follow project_vertices.c.

// What is left: the frame is 80 of 88. The Djinn's sheet pointer lives in r7
// here; the listing keeps it in r4, saved around the first draw at sp+8 (a
// caller-save slot), and reloads draw into r7. Global allocation only does
// that when r7 is taken at that point, and no pseudo in the listing explains
// it yet; a real answer settles that 4 bytes and the spill rotation behind
// most of the remaining sched2 order. The other 4 bytes are sp+24, a slot
// the listing allocates between canvas (sp+28) and draw (sp+20) and never
// reads, so a pseudo there lost all its references to reload. The rest of
// the gap is load order in sched2, and the stack offsets.

// What moved it: lineX and lineY take their +64 and -16 at the assignment
// (reload splits x = t + 64 into a copy and an add, and sched2 moves the add
// across the call); the two blitter calls take flags 7 and 3; the quads'
// height is a running variable, because the shifted forms never strength
// reduce; fade is set in two steps; each use has its own x/y variable; delay
// is a plain counter, declared last for its slot at sp+12.

// Tried without gain: a local copy of draw for the two Djinn calls (CSE
// removes it), a named sheet pointer, i = 0 after the loop's pointer, x and y
// in the block, about 450 random declaration orders.

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "task.h"
#include "heap_release.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "fixed_math.h"
#include "resource.h"

static __inline__ s32 CopyWords_(void *destination, const void *source, s32 size) {
  return Iwram_CopyWords(destination, source, size);
}


#define HEAP_BLITTER 104 // ☀️ HEAP_SLOT_BLITTER

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

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

// A render job (the first of Render_AllocateModelList's entries).
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

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);               // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase);          // ☀️ BattleEvent_StartPhaseFar
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags); // ☀️ BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ BattleEffect_TintBattler
void Func_0815585c(struct BattleEffectArgument *effect, s32 flags, s32 *anchor,
                   s32 *size); // the canvas preparer
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *pos);
struct RenderJob *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const s8 *vertices, void *projected, s32 count);
void Render_DrawModelList(struct RenderJob *job);
void Func_08015160(s32 x, s32 y, s32 z); // translate
void Func_080151e4(s32 x, s32 y, s32 z); // scale
void Func_080150e4(s32 angle);           // roll
void SceneTransform_ApplyPitch(s32 angle);
void Render_ResetTransformState(void);
void Sound_PlayFar(s32 cue);

// Sixteen textured quads tumble down over the targets (or, from the first
// target, rise), three frames apart, for 64 frames; with a prelude a Djinn
// circles over the canvas and a fan of light lines spreads from it. At
// frame 56 every target flashes.
void Func_0814b9e8(struct BattleEffectArgument *effect, s32 mode) {
  s32 anchor[3];
  s32 size[3];
  struct EffectPosition pos;
  struct Texture texture;
  s32 y;
  struct BattleEffectWork *work;
  struct EffectStep *quad;
  s32 x;
  s32 frame;
  struct RenderJob *job;
  void *vertices;
  void *canvas;
  DrawRectangle draws[2];
  s32 i;
  s32 height;
  s32 fade;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);

  // The mode's canvas, sheets and palette.
  if (mode == QUADS_MERCURY) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 33, anchor, size);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D0,
                               work->sheet + SHEET_TEXTURE, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SHEET_DJINN, 1, 0);
    CopyWords_((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
  } else if (mode == QUADS_MARS) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 2, anchor, size);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SHEET_DJINN, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FeatherSheet,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  } else {
    EffectPosition_ProjectBattler(effect->actors[0], &pos);
    anchor[0] = pos.x;
    Resource_LoadAndDecompress((s32)&ResourceId_FeatherSheet,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Centre the canvas on the anchor; the blitter mirrors for side 0.
  REG_BG2PA = 0x100;
  REG_BG2X = (64 - anchor[0]) << 8;
  if (effect->side == 0)
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  else
    BattleEffect_LoadBlitter(HEAP_BLITTER,
                  BLITTER_ADDITIVE << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draws[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (frame = 0; frame != QUAD_FRAMES; frame++) {
    if (frame == 8)
      Sound_PlayFar(246);

    // The quads start above the screen (or at the target for mode 2), each
    // with its own heading, spin and size.
    if (frame == 0) {
      height = -0x200000;
      for (i = 0; i != 32; i++) {
        quad = &work->particles[i];
        quad->x = Random16() & 0xffff;
        Random16();
        quad->z = 1 << 16;
        if (mode == QUADS_RISING)
          quad->y = 32 << 16;
        else
          quad->y = height;
        quad->velocityY = ((Random16() & 0xffff) << 1) + 0x10000;
        quad->velocityX = Random16() & 0xffff;
        quad->velocityZ = Random16() & 0xffff;
        quad->variant = (Random16() & 0x3fff) + 0x2000;
        height += -0x40000;
      }
    }

    // Frame 56: the next phase, and every target flashes.
    if (frame == 56) {
      Sound_PlayFar(212);
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
      draws[0](canvas, work->sheet + SHEET_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draws[0](canvas, work->sheet + SHEET_DJINN, x, y, 40, 40);
      if (frame <= 79) {
        s32 angle;
        s32 lineX;
        s32 lineY;

        Iwram_FillWords(work->sheet, 128, 0x28282828 - frame / 2 * 0x01010101);
        i = 0;
        angle = frame << 9;
        for (; i != 16; i++) {
          lineX = ((Trig_Sin(angle) * frame) >> 16) + 64;
          lineY = ((Trig_Cos(angle) * frame) / 8 >> 16) - 16;
          draws[0](canvas, work->sheet, lineX, lineY, 1, 128);
          angle += 0x1000;
        }
      }
    }

    // The quads: each a render job of one textured face, placed by its own
    // translation, scale, pitch and roll.
    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;
    if (frame <= 63) {
      vertices = Heap_AllocEwram(32);
      job = Render_AllocateModelList(1);
      fade = 0;
      if (frame > 55)
        fade = (56 - frame) * 8;
      job->fade = fade;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + SHEET_TEXTURE;
      job->kind = 6;
      job->texture = &texture;
      job->faces = gData_08199340;
      job->vertices = vertices;

      for (i = 0, quad = work->particles; i != QUAD_COUNT; i++, quad++) {
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
          Render_TransformVertices(gData_08199210, vertices, 4);
          Render_DrawModelList(job);
        }
      }
      Heap_Release(job);
      Heap_Release(vertices);
    }
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
