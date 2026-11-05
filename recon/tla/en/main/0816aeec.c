// 2026-11-05, differ 266 of 902 unchanged, measured from the lreg and greg dumps. Lo
// registers go r3 (the column's locals), then by rank: quad r2, then the native's
// vertex r1, u r0, Q r4, b r5, c r6, d r7 (kicked to its slot by reload). In the draft
// i = v dies into Q (r138), and Q r4 is the two as one hard register, so the native's
// one mov r4, r9 needs only the ranks. Ranks here: u 12 refs across 19 (1.89), vertex
// 17 across 44 (1.55), column 46 across 156 (1.47), Q 17 across 49 (1.39), i 97 across
// 452 (1.29). Moving quad[20] = u after quad[1] puts u at 24 or more and gives vertex
// r1 and u r0 (u at exactly 24 is 628, past it 950), but then the column takes r4, i
// goes to r8 and the frame falls to 124 or 120. The column cannot be kept out of r4
// without ranking below Q: Q's length is the column loop's and no store order changes
// it, and 18 refs on Q would need 46 insns. Other r4 candidates do not hold: i live
// across the column loop ranks 1.15 and takes r8; cprop replaces every use of i = v
// in the column loop; i set at the top of the column loop (hoisted as a set-once user
// variable, then a giv copy) gives 955 to 964; splitting i's other loops into their
// own locals (all 256 subsets) gives nothing below 266; a separate wave column ranks
// the grid's column higher still (849).
// 2026-11-05: differ 266 of 902 (tla-en), 76 masked. pixel = ... in the call's
// argument settles the reload rotation up to the grid. Not gained: the frame loop as
// a label and a goto (642: this one keeps its notes), i = row << 4 with a from the
// expression (947, masked 225: i takes r8 and vertex a row << 6 giv at slot 16, 102
// vs 92 after expand_mult's copy), a base stepped by 16 (825, masked 145), moving
// u's last store (961: u takes r5). The grid needs vertex above u: 1.55 against 1.89.
// 2026-11-05: differ 274 of 902 (tla-en), 78 with registers, slots and offsets
// masked. No gain from a stepped i (945), q = row << 4 with a from row (843),
// i = row << 4 set after the row locals (947), u spelled out (904), or a row
// level cell pointer from i (468, masked 108): it is reduced in the outer loop.
// 2026-11-05: differ 274 of 902 (tla-en). Declaration moves and wrappers
// beyond the one in place gain nothing; the Q allocation below stands.
// 2026-11-05: differ 274 of 902 (tla-en). A wrapper on one Iwram call gains 2;
// the allocation below still stands.
// Draft, differ 276 of 902, skeleton gap 80 (tla-en, 2026-11-01)
// The instruction count, the frame and every spill slot now match; the base,
// a, x, row and column are in the native's r9, lr, r10, r11 and r8. What is
// left is one allocation, and the rest of the gap is its reload rotation (the
// model stores, the camera, scratch registers after the grid). In the column
// loop the native has Q (the row base copy: quad[5], [17], [19], and b, c, d)
// in r4, u in r0 and vertex in r1; here Q is r0, u is r1 and vertex is r4. The
// native also has one copy, mov r4, r9; here it is mov r4, r9 then adds r0, r4,
// i (r48) dying into the hoisted row << 4 (r134). And the native's vertex start
// is r4 << 2, where cse folds this to row << 6 through i's REG_EQUAL.
// Next, from the lreg and greg dumps: the priorities are u 1.89, vertex 1.55,
// column 1.47, r134 1.39, i 1.29, and strb makes these LO_REGS first. So the
// native wants vertex above u, and Q above column or Q and i as one pseudo. The
// direct reading is that Q is i itself, which crosses calls and so can only be
// r4, but gcse's copy propagation replaces i with the base in the column loop
// whenever i = v is a plain copy, and i = row << 4 alone is not reduced in the
// outer loop (no r9 at all, column takes r4). Find the source that keeps i live
// through the column loop with a still reading the base.
// i = v is the weak point here: i dies at once and only steers allocation, and
// without it differ goes to 845. It predates this round, so it stays as a known
// problem, not as a form to copy.
// Found by reading the loop, gcse and cse2 dumps, each step measured:
// - The native's slots come from the order the outer loop finds its givs:
//   row * 24, then row << 4 + 15, then row * 15. So quad stays in the if block;
//   at the top of the column loop the three slots rotate.
// - mesh and cells are row-level constants: they settle the pool loads and put
//   vertex's start beside quad's, so r3 is free for both and quad takes r2.
// - a = i + column reads the base (lr, r9 + r8), and row << 4 in the if block
//   gives the hoisted copy. Sweeping i, v and row << 4 at each use found this.
// - (16 - column) * height, Trig_Sin(..) * amplitude and Trig_Sin(..) * height
//   give the native's multiply operands; position = &screen after the picture
//   test gives r5 and r6.
// - b, d, c in that order keep +1, +16 and +17 in their own registers.
// - blit[2] gives the 8-byte slot at sp+76; Task_Add and
//   BattleEffect_LoadBlitter return s32.
// - In the bubble loop, step = &work->particles[i] with the calls on
//   &work->particles[i] gives r7 and its copy in r8.
// No effect or worse (about 25,000 forms swept): every mix of i, v and row << 4
// at each use, row * 16, a hand-stepped base (983), i set in the column loop
// (458) or the if block, quad at the top of the loop or from a - row, a local
// for the +15, the vertex pointer's spellings, declaration orders, separate
// grid counters (845, 511). Reordering the stores, in data order or at random,
// moves u's live length but only as noise (best 264) and has no meaning, so
// it was not kept. No device was added.
// Decompiled from the listing.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
s32 Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_CrackedTextureSheet;
extern char ResourceId_Data0B4;
extern char ResourceId_SmokeSheet;
extern char ResourceId_CrystalSheet;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];       // the water's mesh, built here
extern const u8 gData_08199340[]; // the splash's mesh
extern const u8 gData_08199210[]; // its corners

#define WATER_COLUMNS 16
#define WATER_ROWS 8

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_CopyWords, 276 of 902 instructions differ in tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

// The body of Dreamtide (mode 0) and Watery Grave (mode 1): a sheet of
// water (a 16 by 8 grid of vertices, its mesh built here) rolls in under
// the targets in waves while the camera turns; Dreamtide strikes the first
// target with two splashes at frame 17, Watery Grave strikes every target
// at frames 17 and 46 while bubbles rise from the deep.
void Func_0816aeec(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle blit[2];
  void *canvas;
  s16 *camera;
  s16 savedYaw;
  s32 frames;
  struct EffectPosition screen;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *bubble;
  s32 height;
  s32 phase;
  s32 row;
  s32 column;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(1);
  if (mode == 0) {
    REG_BLDALPHA = 0xc10;
  } else {
    REG_BLDALPHA = 0x1010;
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  camera = (s16 *)Ram_HeapSlots->cameraWork;
  savedYaw = camera[27];
  Resource_LoadAndDecompress((s32)&ResourceId_CrackedTextureSheet, work->sheet,
                             1, 1);
  Func_0816ae40(work->sheet, work->sheet + 0x400, 32, 32);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet + 0x2000, 0,
                             0);
  if (mode == 1) {
    u8 *pixel;

    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, pixel = work->sheet + 0x3000, 1, 0);
    for (i = 0; i != 0x1b00; i++, pixel++) {
      if (*pixel != 0) {
        *pixel += 8;
        if (*pixel > 63) {
          *pixel = 63;
        }
      }
    }
    CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
  }

  // The water's grid: 16 by 8 vertices, and a quad (two triangles) for
  // each cell but the last row and column.
  for (row = 0; row != WATER_ROWS; row++) {
    s32 v = row << 4;
    s32 x = -60;
    u8 *mesh = gData_02014000;
    s8 *cells = (s8 *)gMapCellBuffer;

    i = v;

    for (column = 0; column != WATER_COLUMNS; column++, x += 8) {
      s8 *vertex = cells + (i + column) * 4;
      s32 u = column * 8;
      s32 a = i + column;

      vertex[0] = column * 11 - 82;
      vertex[1] = 60;
      vertex[2] = row * 24 - 84;
      if (row <= 6 && column <= 14) {
        s32 b = i + column + 1;
        s32 d = i + column + 17;
        s32 c = i + column + 16;
        u8 *quad = mesh + (row * 15 + column) * 24;

        quad[4] = x + 67;
        quad[6] = x + 67;
        quad[18] = x + 67;
        quad[7] = (row << 4) + 15;
        quad[8] = u;
        quad[9] = (row << 4) + 15;
        quad[16] = u;
        quad[20] = u;
        quad[5] = (row << 4);
        quad[17] = (row << 4);
        quad[19] = (row << 4);
        quad[21] = (row << 4) + 15;
        quad[0] = b;
        quad[1] = d;
        quad[2] = c;
        quad[12] = a;
        quad[13] = b;
        quad[14] = c;
      }
    }
  }
  {
    u8 *end = gData_02014000 + 105 * 24;

    end[0] = 0;
    end[1] = 0;
    end[2] = 0;
  }
  frames = 48;
  if (mode != 0) {
    frames = 81;
  }

  for (frame = 0; frame != frames; frame++) {
    void *vertices;
    s32 shade;
    s32 angle;
    s32 offset;

    if (frame == 0) {
      camera[27] = savedYaw;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }
    if (effect->side == 0) {
      if (camera[27] <= 11999) {
        camera[27] += 128;
      }
    } else if (camera[27] >= -11999) {
      camera[27] -= 128;
    }

    if (mode == 0) {
      if (frame == 17) {
        Func_081180e8(144);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0x80000, 100);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
        work->shakeFrames = 8;
      }
    } else {
      if (frame == 17) {
        Sound_PlayFar(145);
        for (i = 0; i != effect->count; i++) {
          Func_08118088(effect->actors[i], 0);
          Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 8;
      }
      if (frame == 46) {
        Func_081180e8(134);
        for (i = 0; i != effect->count; i++) {
          BattleEffect_StartActorMotion(effect->actors[i], 1, 0, 0, 0x80000, 100);
          Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 8;
      }
    }
    if (mode == 0) {
      if ((u32)(frame - 33) <= 14) {
        REG_BLDALPHA = (48 - frame) | 0x1000;
      }
    } else if ((u32)(frame - 65) <= 14) {
      REG_BLDALPHA = (80 - frame) | 0x1000;
    }

    vertices = Heap_AllocEwram(0x400);
    model = Render_AllocateModelList(1);
    shade = 0;

    // The waves rise, then ebb.
    if (mode == 0) {
      if (frame <= 31) {
        height = frame * 4;
        if (height > 64) {
          height = 64;
        }
      } else {
        height = 320 - frame * 8;
        if (height < 0) {
          height = 0;
        }
      }
    } else if (frame <= 63) {
      height = frame * 2;
      if (height > 64) {
        height = 64;
      }
    } else {
      height = 576 - frame * 8;
      if (height < 0) {
        height = 0;
      }
    }
    phase = frame * 2;
    angle = frame << 11;
    for (row = 1, offset = 64; row != 7; row++, offset += 64) {
      s8 *vertex = (s8 *)gMapCellBuffer + offset + 1;

      for (column = 0; column != WATER_COLUMNS; column++) {
        s32 amplitude = (16 - column) * height / 32;

        *vertex = ((Trig_Sin(angle + column * 5000 + Trig_Sin(row * 3553)) * amplitude) >> 16) - amplitude + 64;
        vertex += 4;
      }
    }
    height /= 2;
    for (i = 0, bubble = work->particles; i != 20; i++, bubble++) {
      if (bubble->variant == 0) {
        bubble->y =
            (((Trig_Sin(angle + Trig_Sin(3552 + i * 888)) * height) >> 16) - height + 64) << 16;
      }
    }

    texture.logWidth = 6;
    texture.logHeight = 6;
    texture.pixels = work->sheet + 0x400;
    Render_ResetTransformState();
    if (mode != 0) {
      SceneTransform_ApplyPitch(4000);
    }
    if (effect->side == 0) {
      SceneTransform_ApplyYaw(camera[27]);
    } else {
      SceneTransform_ApplyYaw(camera[27] + 0x8000);
    }
    {
      s32 shift = 128 - frame * 8;

      if (shift < -64) {
        shift = -64;
      }
      Func_08015160(shift << 16, 0, 0);
    }
    model->kind = 7;
    model->texture = &texture;
    model->mesh = gData_02014000;
    model->vertices = vertices;
    model->shade = shade;
    model->scrollU = (frame + phase) & 127;
    model->scrollV = 127 & ~frame;
    Render_TransformVertices(gMapCellBuffer, vertices, 128);
    Render_DrawModelList(model);

    if (frame == 0) {
      s32 bubbleZ = -0x3c0000;

      for (i = 0, bubble = work->particles; i != 20; i++, bubble++) {
        bubble->variant = Random16() % 6;
        bubble->z = bubbleZ;
        bubble->x = -0x3c0000;
        bubble->velocityX = 0;
        bubble->velocityY = 0;
        bubble->velocityZ = 0;
        bubbleZ += 0x60000;
      }
    }
    if (mode == 1) {
      for (i = 0; i != 20; i++) {
        struct EffectStep *step = &work->particles[i];
        s32 picture = step->variant;
        struct EffectPosition *position;

        if (picture < 0) {
          continue;
        }
        position = &screen;
        if (EffectPosition_ProjectPoint(&work->particles[i], position) == 0 &&
            HI(position->depth) > 10) {
          s32 x = HI(position->x);
          s32 y;

          position->x = x + 64;
          y = HI(position->y);
          position->y = y + 64;
          blit[0](canvas, work->sheet + 0x3000 + picture * 0x480, x + 52,
                  y + 40, 24, 48);
          BattleEffectKernels_IntegrateVector3(&work->particles[i], 62,
                                               -0x2000);
        }
        if (++step->variant == 6) {
          step->variant = 0;
        }
      }
    } else {
      s32 t;

      EffectPosition_ProjectBattler(effect->actors[0], &target);
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gData_08199340;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;
      for (i = 0; i != 2; i++) {
        t = frame - 16 - i * 4;
        if (t < 0) {
          continue;
        }
        texture.pixels = work->sheet + 0x2000;
        if (t > 7) {
          shade = (8 - t) * 4;
        }
        model->shade = shade;
        if (shade <= -64) {
          continue;
        }
        Render_ResetTransformState();
        Func_08015160((target.x / 2 - 64) << 16, (target.y - 64) << 16, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_080150e4((i << 14) + 0x2000);
        SceneTransform_ApplyPitch(0x1000);
        SceneTransform_ApplyYaw(angle);
        Func_0801521c(t * 0x3000);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
