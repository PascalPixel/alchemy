// Measured 2026-11-05 (tla-en): differ 2371 of 5364, 915 with slots and pool
// offsets normalized, 241/244 with registers masked too (draft 5367
// instructions, frame 416 where the listing has 420). Calls agree in order, 258
// of 258. Compare with registers masked first: most of what is left is reload
// and allocation following one early choice, so a right form can raise the raw
// figure while the masked one falls.
// What the listing settled this round: fades read one colour as an s32 and mask
// red with `r = 31; r &= c` (first at the whitening, 401 and 446 fades, last at
// the opening and 816 fades); a sprite's tile is stored before its frame; the
// crystal's tile buffer is the fixed address, so its pool entry stays apart from
// the decode call's; the splash computes its cell in the loop (PRE then hoists
// only the inner sum, as the listing's slot 48 shows), has one counter for its
// rings (r7), a ring pointer per arm (y then takes r9), and angle and position
// per loop; the second span table is copied before the free-slot search; the
// light bars' fixed-point time is `* 0x10000` (folded to frame << 16 plus a
// constant) with their x a constant local before the shade test; the rings
// double y at the calls.
// Still open, each measured:
// 1. i (r10) has priority 1.92 and one pseudo above it holds r7: i * 8 of the
//    crystal's drop frames (1.93), which the listing has in r6. One more loop on
//    i passes it, i takes r7 and the draft falls to about 5273 instructions, so
//    the crystal band and the 446 clamp, r10 over r11 in the listing, keep their
//    own counters (row, x with bandColumn): 5 and 6 instructions short there. The
//    446 frames take `w * 2 * w` for this reason; the listing's form is
//    `(w * 2) * w`, as the crystal's frames have it.
// 2. In the splash r takes r7 and the rings' counter r8 (priority 1.53 over
//    1.28); the listing has them the other way. Sharing the counter with the
//    column loop as well, which the listing also has in r7, sends it to r4 as a
//    caller save.
// 3. A streak head's base stays hoisted: the head's constant and the points' are
//    one value, so loop.c matches them (`move-insn matches`); the listing reloads
//    the head's each pass and the points' from its own pool entry. Spelling the
//    points from the scene's start folds back to the same constant, and a symbol
//    either way rewrites it from the scene's base.
// 4. The bright band's (i - top) * 2 still reduces to a giv where the listing
//    keeps it; top's scope, the shift spelling and i set first do not move it.
// 5. The opening and 446 fades hoist a second 31 and copy it for red; `c & 31`
//    shares the green and blue mask instead. All types of c, r, g, b measured.
// Measured 2026-11-05 (tla-en): differ 2314 of 5364, 1772 with slots and pool
// offsets normalized. Colours read into s16 locals took it from 2613: c, r,
// b. Counters follow the listing's loops: r10's loops are i, r11's are j, and
// a loop the listing counts in r8 or a low register has a variable of its own
//; the splash's five loops share one, r7 in
// the listing. A loop more or less on i can move it to r7 and cost 1700.
// Shape differs by small hunks only.
// Measured 2026-11-05 (tla-en): differ 3039 of 5364. Measure with all five
// addresses (the figure before this one, 3800 of 4851, was the parent alone:
// 3287 of 5364 with all five). A tagged one-pass block settles the frame at
// 420. Left: allocation throughout, led by the streak base the native reloads
// each pass and the spilled &work slot.
// Draft, differ 3287 of 5364, skeleton gap 373 + 380 (tla-en, 2026-11-01)
// Measure with the four nested functions' listings as well:
// diff ...08192c9c.c 0819284c,08192894,081929fc,08192a4c,08192c9c. Measured
// against 08192c9c alone they all count as added. All four nested functions
// are at differ 0; what is left is
// BattleEffect_RunIris itself.
// Same calls and control flow as the listing, and the prologue's shape now
// matches. What is left is allocation, in this order of cause:
// 1. The native's &work is one pseudo set twice: at entry, and again by
//    `add r4, sp; str r4, [sp, #64]` in the frame == 446 block, right after
//    Iwram_FillWords(work->sheet + 0x1810, ...). That is a second occurrence
//    of the address in one basic block, so PRE's reaching register gets two
//    sets. A do { } while (0) around that one Iwram_FillWords reproduces it:
//    differ 3150, frame 420 as in the native, every slot's use count within a
//    few of the listing's except the swaps in 2. It is a one-pass block, so it
//    stays out. A static inline, a label, a goto to the next line, an empty if,
//    if (0), an empty for and while (0) do not split it. Next step: the C that
//    really ends CSE there; S2's macro exception would need the same form at
//    every zero fill, which is Pascal's call.
// 2. With that block, slots 176 and 184 are swapped (&canvas and &sparks) and
//    &work sits at 40, not 64. The slots follow pseudo numbers from PRE, whose
//    order comes from its hash table, sized by the function's insn count: 20
//    extra insns move &work, so this may snap only as the count closes.
// 3. Counters. The listing's registers read plainly: r10 is the outer counter
//    nearly everywhere, r11 the inner one (the splash loop's outer is r11 with
//    r10 inside, the crystal grid is r11 over r8). Writing exactly that gives
//    4587: i wins r7 and every high-register mov goes. i must fall below the
//    locals that fill r4 to r8 in the dense loops, so it is the right sharing
//    with its priority still wrong (refs over live length). The names here are
//    the older search's, and a per-loop rename search finds nothing better.
// Measured: the opening runs canvas, work, objects, sprites, sparks, battle,
// savedScroll, camera; Iris_DrawSplash's locals likewise (work and sprites
// before draw). gData_08197410, Data_08199f40 and Data_0819a040 are not const:
// the native rereads them in loops, which also supplies the register pressure
// of the streak loop. Data_02012ef4 is the fixed address, not a symbol: CSE
// then cannot rewrite the streak base as an earlier base + 20, and the pool
// keeps its own entries as in the listing. The head of a streak is the 2-D
// [i] (multiplied), its points flat [i * 10 + j] (synthesized). The drop
// copies walk src with their own counter: `src[m]` lets loop.c drop m for an
// end pointer. The bright band's test is compound, entry copy included, so v
// is set in it. The second fade's red is 31 masked afterwards; the same form
// at the 446 and 816 fades measures worse, though the listing's hoisted 31
// suggests it there too.
// Still open: the head of a streak hoists its base where the native reloads it
// each pass (spelling does not move it, so look at loop.c's threshold); the
// bright band reduces (i - top) * 2 to a giv where the native keeps it, likely
// the same threshold.
// Do not trust a small search gain here. Allocation flips on any change:
// moving y into the else arm at frame 509, which is wrong against the listing,
// gained 182 with the counters' registers unchanged. Swaps gaining 8 to 22 in the
// palette, opening and spans2 orders also contradicted the listing and are out.
// ResourceId_Data076, 077, 09A and 0A5 are placeholders for ids
// resource_directory.s does not name yet.

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
#define HALF(p, n) (((s16 *)(p))[n])

#include "bg_scroll.h"
#include "dma.h"
#include "input.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

struct Scale {
  s32 x;
  s32 y;
};

// The spark objects' work, in the map cell buffer at 0x1f40.
struct IrisSparks {
  s16 life[36]; // high byte: frames left; 0 frees the slot
  u16 tiles[15];
  u16 frames[36];
  s32 x[36];
  s32 y[36];
  s32 vx[36];
  s32 vy[36];
};

// A point of a streak: the head moves, the other nine trail it.
struct IrisPoint {
  s32 x;
  s32 y;
  s32 angle;
  s32 speed;
  s32 unk10;
};

// The opening's circle of light and its fifteen streaks, at 0x2ee0.
struct IrisScene {
  union {
    s32 x;
    struct {
      u16 xFraction;
      s16 xPixels;
    };
  };
  union {
    s32 y;
    struct {
      u16 yFraction;
      s16 yPixels;
    };
  };
  s32 unk08;
  s32 unk0c;
  union {
    s32 radius;
    struct {
      u16 radiusFraction;
      s16 radiusPixels;
    };
  };
  struct IrisPoint streaks[15][10];
};

struct IrisSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 unk08_10 : 6;
  u8 unk0a[6];
  u8 resource;
};

struct IrisSpriteFlags {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
};

struct Widths5 {
  s32 w[5];
};

struct Widths7 {
  s32 w[7];
};

struct Spans4 {
  struct WordPair p[4];
};

extern char ResourceId_Data134, ResourceId_Data076, ResourceId_Data077;
extern char ResourceId_Data0A5, ResourceId_Data09A, ResourceId_VioletSkyBackdrop;
extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 Data_02010578[];
extern struct IrisSparks Data_02011f40;
extern struct IrisScene Data_02012ee0;
#define Data_02012ef4 ((struct IrisPoint (*)[10])(Ram_MapCellBuffer + 0x2ef4))
extern s32 gCameraSceneParameters[];
extern u16 gData_08197410[];
extern const struct Scale Data_08196f48;
extern const struct Widths5 Data_08196f50;
extern const struct Widths7 Data_08196f64;
extern const struct Widths7 Data_08196f80;
extern const struct Spans4 Data_08196f9c;
extern const struct Spans4 Data_08196fbc;
extern s32 Data_08199f40[];
extern const u8 Data_08199f48[];
extern const u8 Data_08199f88[];
extern const u8 Data_08199f98[];
extern const u8 Data_08199fbc[];
extern const u16 Data_08199fc8[];
extern const u16 Data_08199fd2[];
extern const s8 Data_08199fde[];
extern const u8 Data_08199fed[];
extern const u16 Data_08199ffc[];
extern const s16 Data_0819a01a[];
extern const u8 Data_0819a038[];
extern s32 Data_0819a040[];
extern const u16 Data_0819a048[];
extern const u16 Data_0819a054[];
extern const u8 Data_0819a060[];
extern const u8 Data_0819a066[];
extern const u8 Data_0819a06c[];
extern const u8 Data_0819a072[];
extern const u8 Data_0819a078[];
extern const u8 Data_0819a07e[];
extern const u8 Data_0819a084[];
extern const u8 Data_0819a0ac[];
extern const u8 Data_0819a0d4[];
extern const u8 Data_0819a140[];
extern const u8 Data_0819a160[];

struct IrisSprite *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
struct IrisSprite *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815b510(u8 *map, s32 size, s32 scale);
void Func_08164b2c(s32 red, s32 green, s32 blue);
void Func_08191958(u8 side, u8 speed, void (*midpoint)(void));
void Func_0819253c(void);
void Func_08191cc4(void);
void BattleEffect_BuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats, s32 scale);
void Func_08192648(u32 frame, s32 x, s32 y, s32 small);
void Func_0819273c(s32 halfWidth, s32 x, s32 height);
void BattleEffect_DrawDiagonalCanvasLine(s32 x, s32 y, s32 width, u8 color);
void SceneTransform_ResetMatrix(void);
void SceneTransform_ApplyPosition(s32 *position);
void Func_080151ac(s32 *scale); // SceneTransform_ApplyScale
void Heap_Release(void *buffer);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[((u8 *)(obj))[16]].tile)
#define SCENE (&Data_02012ee0)
#define PLTT 0x05000000
#define STREAK(i, j) (((struct IrisPoint *)Data_02012ef4)[(i) * 10 + (j)])
// The 128 background colours the scene fades (entries 96 to 223).
#define FADE_PLTT ((u16 *)(PLTT + 0xc0))

// A splash where a drop lands: rings spread from (x, y) for twice the
// splash's duration, then a smaller pair of rings and a column of spray
// rises while the screen shakes; the drop itself is drawn last.
static inline void Iris_DrawSplash(s32 t, s32 x, s32 y, s32 small) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  DrawRectangle drawAlt;
  DrawRectangle draw;
  u8 *sprites;
  s32 r;
  s32 cel;
  s32 sheetOffset;
  s32 point;
  s32 segment;
  s32 column;

  sprites = Ram_HeapSlots->unk64[0];
  draw = Ram_HeapSlots->unk64[1];
  drawAlt = Ram_HeapSlots->unkb8[1];
  if (t < Data_08199f40[small] * 2) {
    r = Data_08199f40[small] * 2 - t;
    cel = r / 6;
    sheetOffset = cel * 0x302;
    cel *= 8;
    for (point = 0; point != 64; point++) {
        s32 px; s32 a; s32 py;
      a = point << 10;
      px = (r * Trig_Sin(a) / 2 >> 16) + x;
      py = (r * Trig_Cos(a) >> 16) + y;
      draw(canvas, sprites + cel + 444, px, py - 2, 2, 4);
    }
    if (small == 0) {
      s32 offset = 0;
      u8 *ring;
      DrawRectangle blit;
      s32 spoke;

      if (t <= 11)
        offset = 196;
      else if (t <= 23)
        offset = 98;
      blit = draw;
      ring = sprites + offset;
      for (spoke = 0; spoke != 8; spoke++) {
        s32 px; s32 a; s32 py;
        a = (spoke << 13) + (t * 5 << 9);
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        blit(canvas, ring, px - 3, py - 7, 7, 14);
        a += a >> 1;
        px = (Iwram_MulQ16(0x28000, r * Trig_Sin(a) / 2) >> 16) + x;
        py = (Iwram_MulQ16(0x50000, r * Trig_Cos(a) / 2) >> 16) + y;
        blit(canvas, ring, px - 3, py - 7, 7, 14);
      }
      draw(canvas, work->sheet + (gData_08197410[8] + sheetOffset), x - 4, y - 9,
           9, 18);
    } else {
      s32 offset = 294;
      u8 *ring;
      DrawRectangle blit;
      s32 spoke;

      if (t <= 11)
        offset = 394;
      else if (t <= 23)
        offset = 344;
      blit = draw;
      ring = sprites + offset;
      for (spoke = 0; spoke != 8; spoke++) {
        s32 px; s32 a; s32 py;
        a = (spoke << 13) + (t * 5 << 9);
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        blit(canvas, ring, px - 2, py - 5, 5, 10);
        a += a >> 1;
        px = (Iwram_MulQ16(0x28000, r * Trig_Sin(a) / 2) >> 16) + x;
        py = (Iwram_MulQ16(0x50000, r * Trig_Cos(a) / 2) >> 16) + y;
        blit(canvas, ring, px - 2, py - 5, 5, 10);
      }
      draw(canvas, work->sheet + (gData_08197410[6] + sheetOffset), x - 3, y - 7,
           7, 14);
    }
  } else {
    if (t < Data_08199f40[small] * 2 + Data_08199f40[small]) {
      s32 ringCel;

      r = t - Data_08199f40[small] * 2;
      ringCel = r / 3 * 8;
      if (ringCel == 64)
        ringCel = 56;
      ringCel += 444;
      for (point = 0; point != 64; point++) {
        s32 px; s32 a; s32 py;
        a = point << 10;
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        draw(canvas, sprites + ringCel, px - 1, py - 2, 2, 4);
      }
      for (point = 0; point != 64; point++) {
        s32 px; s32 a; s32 py;
        a = point << 10;
        px = (r * Trig_Sin(a) * 2 >> 16) + x;
        py = (r * Trig_Cos(a) >> 16) + y + Data_08199f48[small] * 2;
        draw(canvas, sprites + ringCel, px - 1, py - 2, 2, 4);
      }
    }
    for (segment = 0, column = y; segment < t - Data_08199f40[small] * 2 && column <= 112;
         segment++, column += 32)
      draw(canvas, sprites + 508, x - 4, column, 8, 32);
    if (t < Data_08199f40[small] * 2 + 8) {
      s32 shake = (Data_08199f40[small] * 2 - t + 8) >> 1;

      gBgScroll[1].y = 32;
      if (t & 1)
        gBgScroll[1].y = shake + 32;
      else
        gBgScroll[1].y = 32 - shake;
    }
    if (small == 0)
      drawAlt(canvas, work->sheet + 0x1b12, x - 10, y - 20, 20, 40);
    else
      drawAlt(canvas, work->sheet + 0x4c12, x - 6, y - 12, 12, 24);
  }
}

// Summon 25 (Iris): a circle of light opens over a dark field while fifteen
// streaks swirl up out of it; the goddess's crystal rises in a 4 x 4 grid
// of sprites and bursts into sparks; drops fall and splash in rings; pillars
// of light climb, the scene fills with rising motes, rays turn about the
// centre, and the screen whitens as she heals the party.
void BattleEffect_RunIris(struct BattleEffectArgument *effect) {
  struct IrisSparks *sparks;
  struct IrisSprite **objects;
  s32 frame;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  u8 *canvas;

  // The first spark slot with no life left, or -1.
  s32 FindFreeSpark(void) {
    s32 i;
    s32 slot = 36;

    for (i = 0; i != 36; i++) {
      if (sparks->life[i] == 0) {
        slot = i;
        break;
      }
    }
    if (slot == 36)
      slot = -1;
    return slot;
  }

  // Each live spark: placed at its size, moved, dropped when it leaves the
  // screen, otherwise animated and aged.
  void UpdateSparks(void) {
    struct Scale scale;
    s32 place[4];
    s32 i;

    for (i = 0; i != 36; i++) {
      if (sparks->life[i] != 0) {
        scale.x = scale.y = sparks->life[i] * 2;
        place[3] = 0;
        place[1] = 0xff0000;
        place[0] = sparks->x[i];
        place[2] = sparks->y[i];
        Render_ApplyProjectedPlacementFar(objects[i], place, (s32 *)&scale, 0);
        sparks->x[i] += sparks->vx[i];
        sparks->y[i] += sparks->vy[i];
        if (sparks->x[i] < -0x100000 || sparks->x[i] > 0xf00000 ||
            sparks->y[i] < 0 || sparks->y[i] > 0xa00000) {
          sparks->life[i] = 0;
          continue;
        }
        if (!(frame & 3))
          sparks->frames[i]++;
        if (sparks->frames[i] > 14)
          sparks->frames[i] = 0;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
        sparks->life[i] -= 0x100;
      }
    }
  }

  // The first free mote of 32, or -1.
  s32 FindFreeParticle(void) {
    s32 i;
    s32 slot = 32;

    for (i = 0; i != 32; i++) {
      if (work->particles[i].variant == 0) {
        slot = i;
        break;
      }
    }
    if (slot == 32)
      slot = -1;
    return slot;
  }

  // The motes: a sheet cell, fading after 32 frames with bit 4, or a
  // textured quad spinning up with bit 8.
  void DrawParticles(void) {
    struct ModelTexture texture;
    s32 place[3];
    s32 i;

    for (i = 0; i != 32; i++) {
      if (work->particles[i].variant != 0) {
        s32 n = work->particles[i].variant & 15;
        s32 px = work->particles[i].x >> 16;
        s32 py = work->particles[i].y >> 16;
        s32 offset = 0;

        if (work->particles[i].x < 0 || work->particles[i].x > 0x7f0000 ||
            work->particles[i].y < 0 || work->particles[i].y > 0x7f0000) {
          work->particles[i].variant = 0;
          continue;
        }
        if (work->particles[i].variant & 16) {
          s32 age = frame - work->particles[i].z;

          if (age > 32)
            offset = (age - 32) * 0x302;
          if (offset == 0x1810) {
            work->particles[i].variant = 0;
            continue;
          }
        }
        if (work->particles[i].variant & 256) {
          void *verts = Heap_AllocEwram(32);
          struct RenderModel *model = Render_AllocateModelList(1);
          s32 age;

          offset = 0;
          age = frame - work->particles[i].z;
          if (age > 24)
            offset = (age - 24) * 0x800;
          texture.logWidth = 5;
          texture.logHeight = 6;
          texture.pixels = work->sheet + 0x1810 + offset;
          SceneTransform_ResetMatrix();
          place[0] = work->particles[i].x - 0x400000;
          place[1] = work->particles[i].y - 0x400000;
          place[2] = 0;
          SceneTransform_ApplyPosition(place);
          place[1] = place[0] = work->particles[i].velocityZ;
          place[2] = 0;
          Func_080151ac(place);
          work->particles[i].velocityZ += 0x2000;
          model->kind = 4;
          model->texture = &texture;
          model->mesh = Data_08199f98;
          model->vertices = verts;
          Render_TransformVertices(Data_08199f88, verts, 4);
          Render_DrawModelList(model);
          Heap_Release(model);
          Heap_Release(verts);
          if (offset == 0x3800)
            work->particles[i].variant = 0;
        } else {
          draw[0](canvas, work->sheet + (gData_08197410[n - 1] + offset),
                  px - n / 2, py - n, n, n * 2);
        }
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
      }
    }
  }

  u8 *sprites;
  s32 pos[4];
  u8 *battle;
  s32 savedScroll;
  s16 *camera;
  s32 savedZoom;
  struct Scale scale;
  struct Scale shrink;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  s32 lift;
  s32 i;
  s32 j;
  s32 byteIndex;
  s32 width;

  s32 k;
  s32 column;
  s32 row;
  s32 bandColumn;
  s32 m;
  s32 x;
  s32 y;
  s32 off;
  s32 top;
  u8 *dst;
  u8 *tiles;
  u8 v;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  objects = (struct IrisSprite **)gMapCellBuffer;
  sprites = Ram_HeapSlots->unk64[0];
  sparks = &Data_02011f40;
  battle = (u8 *)Ram_HeapSlots->battleWork;
  savedScroll = gBgScroll[1].x;
  camera = Ram_HeapSlots->cameraWork;
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0x2000);
  Func_0813ba50();
  *(u16 *)PLTT = 0;
  *(u16 *)(PLTT + 2) = 0;
  work->transferMode = 0;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  REG_DISPCNT = 0x7741;
  REG_BG2CNT = 0x784;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  Func_08191958(0, 4, Func_0819253c);
  Iwram_FillWords(canvas, 0x4000, 0);
  while (((volatile struct DmaRegisters *)REG_DMA3)->control & 0x80000000) {
  }
  work->transferPending = 1;
  wait(1);
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  work->transferMode = 2;
  work->transferValue = 75;
  savedZoom = camera[27];
  if (effect->side == 0) {
    s32 yaw = 0x4000;

    camera[27] = yaw;
  } else {
    s32 yaw = 0xc000;

    camera[27] = yaw;
  }
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  BattleEffect_LoadBlitter(SLOT_ALT, 3);
  draw[0] = Ram_HeapSlots->unk64[1];
  draw[1] = Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, Data_02010578, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  for (j = 0; j != 0x302; j++)
    sprites[j] >>= 1;
  Func_0815b434(work->sheet, 32, 0x10000);
  Graphics_LoadGreyRamp();
  frame = 0;
  HeapSlot_Release(SLOT_ALT);
  Render_ResetTransformState();

  for (;;) {
    // A or B: skips the opening, or the rest of the scene.
    if (gInput.repeat & 3) {
      if (frame <= 175) {
        frame = 176;
      } else if ((u32)(frame - 177) <= 268) {
        REG_BLDALPHA = 0x1010;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        work->transferPending = 1;
        wait(1);
        if (frame <= 386) {
          for (i = 0; i != 16; i++)
            ResourceObject_ReleaseFar(work->objects[i]);
          BattleEffect_SelectLivingTargets(work->effect);
          Palette_LoadBasicObjColors();
          for (i = 0; i != work->effect->count; i++)
            Func_0814cd48(work->effect->actors[i], -1, 5, -1, 0);
        }
        frame = 446;
      } else {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        work->transferPending = 1;
        wait(1);
        break;
      }
    }
    if (frame == 850)
      break;

    if (frame == 0)
      Sound_PlayFar(206);
    for (i = 0; i != 11; i++)
      if (frame == Data_08199fbc[i])
        Sound_PlayFar(130);
    if (frame == 175)
      Sound_PlayFar(212);
    for (i = 0; i != 5; i++)
      if (frame == Data_08199fc8[i])
        Sound_PlayFar(209);
    for (i = 0; i != 6; i++)
      if (frame == Data_08199fd2[i])
        Sound_PlayFar(212);
    if (frame == 369)
      Sound_PlayFar(206);
    for (i = 0; i != 6; i++)
      if (frame == 644 + i * 12)
        Sound_PlayFar(104);
    if (frame == 755)
      Sound_PlayFar(212);
    for (i = 0; i != 6; i++)
      if (frame == 759 + i * 9)
        Sound_PlayFar(145);
    if (frame == 820)
      Sound_PlayFar(208);

    // The opening: the circle and the streaks.
    if (frame == 0) {
      SCENE->x = 0x3c0000;
      SCENE->y = 0x800000;
      SCENE->radius = 0xc0000;
      for (i = 0; i != 15; i++) {
        struct IrisPoint *s = Data_02012ef4[i];

        s->x = Data_08199fde[i] << 16;
        s->y = Data_08199fed[i] << 16;
        s->angle = Data_08199ffc[i];
        s->speed = Data_0819a01a[i];
        s->unk10 = 0;
        for (j = 1; j != 10; j++) {
          STREAK(i, j).x = 0x960000;
          STREAK(i, j).y = 0x960000;
        }
      }
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame <= 175) {
      typeof(SCENE) scene = SCENE;
      BattleEffect_LoadBlitter(SLOT_ALT, 3);
      draw[1] = Ram_HeapSlots->unkb8[1];
      for (i = 0; i != 15; i++) {
        if (!(frame & 1)) {
          for (j = 9; j != 0; j--) {
            STREAK(i, j).x = STREAK(i, j - 1).x;
            STREAK(i, j).y = STREAK(i, j - 1).y;
          }
        }
        if (frame >= i * 8 + 24) {
          struct IrisPoint *s = Data_02012ef4[i];
          if (HALF(s, 3) > 59) {
            s32 size;

            s->x += Trig_Sin(s->angle) * 3 / 2;
            s->y -= Trig_Cos(s->angle) * 3 / 2;
            s->angle += s->speed * 2 / 3;
            size = (i & 3) + 5;
            for (j = 0; j != 10; j++) {
              struct IrisPoint *p = &STREAK(i, j);

              draw[1](canvas, sprites + gData_08197410[size - 1],
                      HALF(p, 1) - (size >> 1), HALF(p, 3) - size, size,
                      size * 2);
            }
          }
        }
      }
      if (frame <= 71) {
        scene->y -= 0x10000;
        scene->radius -= 0x1000;
        if (!(frame & 15)) {
          for (i = 0; i != 128; i++) {
            s32 c = FADE_PLTT[i];
            s32 r = 31;
            u16 g = (u16)c >> 5 & 31;
            s32 b = (u16)c >> 10 & 31;

            r &= c;

            FADE_PLTT[i] = (b + 1) << 10 | (g + 1) << 5 | (r + 1);
          }
        }
      } else {
        scene->radius += 0x2000;
        if (!(frame & 7)) {
          for (y = 0; y != 128; y++) {
            s32 c = FADE_PLTT[y];
            s32 r = 31;
            s32 g = (u16)c >> 5 & 31;
            s32 b = (u16)c >> 10 & 31;
            r &= c;

            if (r < 23)
              r++;
            if (g > 1)
              g--;
            if (b < 15)
              b++;
            FADE_PLTT[y] = b << 10 | g << 5 | r;
          }
        }
      }
      HeapSlot_Release(SLOT_ALT);
      {
        s32 size = scene->radiusPixels * 2;

        Func_0815b434(work->sheet, size, 0x10000);
        Func_0818caa8(work->sheet, scene->xPixels, scene->yPixels, size);
      }
      if (frame == 175) {
        REG_BG2CNT = 0x785;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        Iwram_FillWords(FADE_PLTT, 0x100, 0x7fff7fff);
      }
    }

    // The crystal: sixteen sprites, the sparks and the first motes.
    if (frame == 176) {
      u8 *colors;

      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      work->transferPending = 1;
      wait(1);
      battle[0x867] = 1;
      BattleBackground_LoadFar(1, (s32)&ResourceId_Data076, 0);
      colors = Resource_GetTableEntry((s32)&ResourceId_Data0A5);
      Dma_Set(colors, (void *)(PLTT + 0x200), 0x84000080, REG_DMA3);
      colors += 0x200;
      Resource_DecodeType01(colors, Data_02010578);
      tiles = (u8 *)(Ram_MapCellBuffer + 0x578);
      for (i = 0; i != 16; i++) {
        struct IrisSprite *obj = Func_0815b290(32, 32, 0x80002000, 0);

        work->objects[i] = obj;
        ((struct IrisSpriteFlags *)obj)->priority = 1;
        Dma_Set(tiles, (void *)OBJ_TILES(obj), 0x84000100, REG_DMA3);
        tiles += 0x400;
      }
      colors = Resource_GetTableEntry((s32)&ResourceId_Data09A);
      Iwram_FillWords((void *)(PLTT + 0x3e0), 32, -1);
      colors += 32;
      Resource_DecodeType01(colors, work->sheet);
      for (i = 0; i != 15; i++) {
        struct IrisSprite *obj = Func_0815b290(16, 16, 0x40000000, 0xf000);

        objects[i] = obj;
        ((struct IrisSpriteFlags *)obj)->priority = 1;
        sparks->tiles[i] = obj->tile;
        sparks->frames[i] = i;
        Dma_Set(work->sheet + i * 128, (void *)OBJ_TILES(obj), 0x84000020,
                REG_DMA3);
      }
      for (; i != 36; i++) {
        struct IrisSprite *obj = Func_0815b3b0(16, 16, 0x40000000, 0xf000);

        objects[i] = obj;
        Dma_Set(objects[0], obj, 0x84000006, REG_DMA3);
        sparks->frames[i] = Random16() % 15;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
      }
      for (i = 0; i != 18; i++) {
        sparks->x[i] = (Random16() % 15 << 16) + 0x390000;
        sparks->y[i] = (Random16() % 15 << 16) + 0x4f0000;
        sparks->vx[i] = ((Random16() & 63) - 32) << 12;
        sparks->vy[i] = ((Random16() & 63) - 32) << 12;
      }
      for (; i != 36; i++) {
        sparks->x[i] = (Random16() % 15 << 16) + 0xa90000;
        sparks->y[i] = (Random16() % 15 << 16) + 0x4f0000;
        sparks->vx[i] = ((Random16() & 63) - 32) << 12;
        sparks->vy[i] = ((Random16() & 63) - 32) << 12;
      }
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->sheet, 0, 0);
      for (i = 0; i != 6; i++) {
        work->particles[i].x = (Random16() % 15 << 16) + 0x210000;
        work->particles[i].y = (Random16() % 15 << 16) + 0x390000;
        work->particles[i].velocityX = -(Random16() & 31) << 12;
      }
      for (; i != 12; i++) {
        work->particles[i].x = (Random16() % 15 << 16) + 0x510000;
        work->particles[i].y = (Random16() % 15 << 16) + 0x390000;
        work->particles[i].velocityX = (Random16() & 31) << 12;
      }
      work->transferMode = 2;
      work->transferValue = 70;
    }
    if ((u32)(frame - 177) <= 216) {
      scale = Data_08196f48;
      pos[3] = 0;
      pos[1] = 0xff0000;
      lift = 0;
      if (frame > 350) {
        u16 *pal = (u16 *)(PLTT + 0x200);

        for (i = 0; i != 255; i++) {
          s32 c = pal[i];
          s32 r = 31;
          s32 g;
          s32 b;

          r &= c;
          g = (u16)c >> 5 & 31;
          b = (u16)c >> 10 & 31;

          if (r != 31)
            r++;
          if (g != 31)
            g++;
          if (b != 31)
            b++;
          pal[i] = b << 10 | g << 5 | r;
        }
      }
      if (frame <= 385) {
        if ((u32)(frame - 351) <= 14)
          lift = Trig_Sin((frame - 350) << 10) << 6;
        else if (frame > 365)
          lift = ((366 - frame) * 5 << 17) + 0x400000;
        if (frame <= 229)
          lift = (Trig_Sin((frame << 6) - 0x2c00) << 6) - 0x147200;
        for (j = 0; j != 4; j++) {
          for (column = 0; column != 4; column++) {
            pos[0] = 0x480000 + column * 0x200000;
            pos[2] = lift + 0x380000 + j * 0x200000;
            Render_ApplyProjectedPlacementFar(work->objects[j * 4 + column], pos,
                                              (s32 *)&scale, 0);
          }
        }
      }
      if (frame == 386) {
        for (i = 0; i != 16; i++)
          ResourceObject_ReleaseFar(work->objects[i]);
        BattleEffect_SelectLivingTargets(work->effect);
        Palette_LoadBasicObjColors();
        for (i = 0; i != work->effect->count; i++)
          Func_0814cd48(work->effect->actors[i], -1, 5, -1, 0);
      }
      if (frame <= 191) {
        for (i = 0; i != 3; i++) {
          u8 v = frame + (i & 1) * 47 + 80;
          s32 y;
          s32 off;
          s32 x;
          s32 m;

          y = i + 62;
          off = ((y & 7) << 3) + ((y >> 3) << 10);
          for (x = 0; x != 128; x += 8) {
            for (m = x; m != x + 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if ((u32)(frame - 177) <= 46) {
      for (i = 0; i != 36; i++) {
        shrink.x = shrink.y = 0x10000 - ((frame - 177) >> 1 << 11);
        pos[3] = 0;
        pos[1] = 0xff0000;
        pos[0] = sparks->x[i];
        pos[2] = sparks->y[i];
        Render_ApplyProjectedPlacementFar(objects[i], pos, (s32 *)&shrink, 0);
        sparks->x[i] += sparks->vx[i];
        sparks->y[i] += sparks->vy[i];
        if (!(frame & 3))
          sparks->frames[i]++;
        if (sparks->frames[i] > 14)
          sparks->frames[i] = 0;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
      }
    }
    if ((u32)(frame - 179) <= 38) {
      s32 radius = frame - 170;

      for (i = 0; i != 256; i++) {
        s32 a = i << 8;
        s32 px = radius * Trig_Sin(a);
        s32 py = radius * Trig_Cos(a);

        draw[0](canvas, work->sheet + gData_08197410[3], (px >> 16) + 58,
                (py * 2 >> 16) + 48, 4, 8);
      }
      for (i = 0; i != 12; i++) {
        draw[0](canvas, work->sheet + gData_08197410[1],
                (work->particles[i].x >> 16) - 2,
                (work->particles[i].y >> 16) - 2, 2, 4);
        work->particles[i].x += work->particles[i].velocityX;
      }
    }
    if ((u32)(frame - 179) <= 31)
      REG_BLDALPHA = (210 - frame) >> 1 | 0x1000;

    // The drops: their frames, the sparks' cells and the spray.
    if (frame == 230) {
      REG_BLDALPHA = 0x1010;
      REG_BG2CNT = 0x784;
      for (i = 1; i != 8; i++) {
        for (k = 0; k != 10; k = bandColumn) {
          u8 *src = work->sheet + gData_08197410[k];
          u8 *out = work->sheet + gData_08197410[k] + i * 0x302;
          s32 n;

          bandColumn = k + 1;
          n = bandColumn * 2 * bandColumn;

          for (off = 0; off != n; off++) {
            u8 v = src[off] - i * 8;

            if (v > 63)
              v = 0;
            out[off] = v;
          }
        }
      }
      for (i = 0; i != 8; i++) {
        Func_0815b510(work->sheet + 0x1b12 + i * 800, 20, 0x10000 - i * 0x2000);
        Func_0815b510(work->sheet + 0x4c12 + i * 288, 12, 0x10000 - i * 0x2000);
      }
      for (i = 0; i != 36; i++)
        sparks->life[i] = 0;
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
      for (i = 0; i != 32; i++)
        for (j = 0; j != 8; j++)
          sprites[508 + i * 8 + j] = Data_0819a038[j];
      dst = sprites;
      for (i = 0; i != 2; i++) {
        for (j = 0; j != 3; j++) {
          u8 *src = work->sheet + gData_08197410[Data_0819a040[i] - 1];

          for (m = 0; m != *(Data_0819a040 + i) * *(Data_0819a040 + i) * 2; m++) {
            s32 v = *src++ - j * 16;

            if (v < 0)
              v = 0;
            *dst++ = v;
          }
        }
      }
      for (j = 0; j != 8; j++) {
        u8 *src = work->sheet + gData_08197410[1];

        for (m = 0; m != 8; m++) {
          s32 v = *src++ - j * 8;

          if (v < 0)
            v = 0;
          *dst++ = v;
        }
      }
    }
    BattleEffect_LoadBlitter(SLOT_ALT, 3);
    if ((u32)(frame - 240) <= 127 && !(frame & 15)) {
      s32 v = (240 - frame) / 16;

      Func_08164b2c(v, v, v);
    }
    for (j = 0; j != 6; j++) {
      if (frame > Data_0819a048[j]) {
        if (frame < Data_0819a054[j]) {
          Iris_DrawSplash(frame - Data_0819a048[j], Data_0819a060[j],
                          Data_0819a066[j], Data_0819a06c[j]);
          if (Data_0819a048[j] - Data_0819a06c[j] * 16 + 48 == frame) {
            for (i = 0; i != 6; i++) {
              s32 s = FindFreeSpark();

              sparks->life[s] = 0x7900;
              sparks->x[s] = Data_0819a060[j] << 17;
              sparks->y[s] = (Data_0819a066[j] << 16) + 0x200000;
              sparks->vx[s] = ((Random16() & 31) - 16) << 12;
              sparks->vy[s] = -(Random16() & 31) << 12;
            }
          }
        } else if (frame < Data_0819a054[j] + Data_0819a078[j]) {
          s32 t = frame - Data_0819a054[j];

          Func_08192648(t / Data_0819a07e[j], Data_0819a060[j],
                        Data_0819a066[j] - Data_0819a072[j] * t / 2,
                        Data_0819a06c[j]);
        }
      }
    }
    if ((u32)(frame - 279) <= 132 || frame > 446)
      UpdateSparks();
    HeapSlot_Release(SLOT_ALT);

    // The pillars of light.
    if (frame == 370) {
      Func_0815b434(work->sheet + 0x3e8, 16, 0x10000);
      for (i = 0; i != 16; i++) {
        Dma_Set(work->sheet + 0x3e8 + i * 8, work->sheet + 0x302 + i * 16,
                0x80000008, REG_DMA3);
        for (j = 0; j != 8; j++)
          work->sheet[0x30a + i * 16 + j] = work->sheet[0x3ef + i * 8 - j];
      }
      REG_BLDALPHA = 0x1008;
    }
    if ((u32)(frame - 414) <= 7)
      REG_BLDALPHA = (frame - 406) | 0x1000;
    if (frame > 385)
      gCameraSceneParameters[4] = 2556 - frame * 6;
    if (frame <= 422) {
      if (frame > 370) {
        s32 h = (frame - 370) * 8;

        if (h > 127)
          h = 127;
        Func_0819273c(3, 34, h);
      }
      if (frame > 375) {
        struct Widths5 widths = Data_08196f50;
        s32 h = (frame - 375) * 8;

        if (h > 127)
          h = 127;
        Func_0819273c(widths.w[h >> 5], (h >> 5) * 2 + 94, h);
      }
      if (frame > 380) {
        struct Widths7 widths = Data_08196f64;
        s32 h = (frame - 380) * 4;
        s32 q;

        if (h > 127)
          h = 127;
        q = h / 19;
        Func_0819273c(widths.w[q], q + 12, h);
      }
      if (frame > 382) {
        struct Widths7 widths = Data_08196f80;
        s32 h = (frame - 382) * 8;
        s32 q;

        if (h > 127)
          h = 127;
        q = h / 21;
        Func_0819273c(widths.w[q], q * 2 + 70, h);
      }
      if (frame > 385) {
        s32 h = (frame - 385) * 4;

        if (h > 127)
          h = 127;
        Func_0819273c(14, (h >> 3) + 48, h);
      }
      if (frame == 422) {
        work->transferMode = 0;
        work->transferValue = 0;
        REG_BLDALPHA = 0x1010;
      }
    }
    if ((u32)(frame - 423) <= 19) {
      s32 step = frame - 422;

      top = 120 - step * 8;
      for (i = top; i != top + 40 && (v = (i - top) * 2, i <= 127); i++) {
        if (i >= 0) {
          if (v > 63)
            v = 63;
          off = ((i & 7) << 3) + ((i >> 3) << 10);
          for (k = 0; k != 128; k += 8) {
            for (m = 0; m != 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if ((u32)(frame - 401) <= 30 && (frame & 1)) {
      Resource_GetTableEntry((s32)&ResourceId_VioletSkyBackdrop);
      for (i = 0; i != 128; i++) {
        s32 c = FADE_PLTT[i];
        s32 r = 31;
        s32 g;
        s32 b;

        r &= c;
        g = (u16)c >> 5 & 31;
        b = (u16)c >> 10 & 31;

        if (r > 3)
          r -= 2;
        if (g > 3)
          g -= 2;
        if (b > 3)
          b -= 2;
        FADE_PLTT[i] = b << 10 | g << 5 | r;
      }
    }

    // The fountain: the second backdrop, the motes and the light shafts.
    if (frame == 446) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      Render_ResetTransformState();
      BattleBackground_LoadFar(1, (s32)&ResourceId_Data077, 0);
      off = 0x302;
      for (i = 0; i != 128; i++) {
        s32 c = FADE_PLTT[i];
        s32 r = 31;
        s32 g;
        s32 b;

        r &= c;
        g = (u16)c >> 5 & 31;
        b = (u16)c >> 10 & 31;

        FADE_PLTT[i] = (b + 1) << 10 | (g + 1) << 5 | (r + 1);
      }
      Graphics_LoadGreyRamp();
      for (i = 0; i != 6; i += 2) {
        Func_0815b434(work->sheet + off, i + 14, 0x10000);
        off += (i + 14) * 2 * (i + 14);
      }
      /* FAKEMATCH(one-pass): the block ends CSE, so &work is one pseudo set twice
       * as the native has it, and the frame is 420; as a plain call, 3287 of 5364
       * instructions differ in tla-en. */
      do {
        Iwram_FillWords(work->sheet + 0x1810, 0x1000, 0);
      } while (0);
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x2010, 16, 64, 0x10000);
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x2810, 16, 64, 0x10000);
      dst = work->sheet + 0x2010;
      for (x = 0; x != 64; x++) {
        for (bandColumn = 0; bandColumn != 32; bandColumn++) {
          if (*dst > x)
            *dst = x;
          dst++;
        }
      }
      for (i = 0; i != 36; i++)
        sparks->life[i] = 0;
      for (i = 0; i != 32; i++)
        work->particles[i].variant = 0;
      for (i = 1; i != 8; i++) {
        for (j = 0; j != 10; j = width) {
          u8 *src = work->sheet + gData_08197410[j];
          u8 *out = work->sheet + gData_08197410[j] + i * 0x302;
          s32 n;

          width = j + 1;
          n = width * 2;
          n *= width;

          for (byteIndex = 0; byteIndex != n; byteIndex++) {
            u8 v = src[byteIndex] - i * 8;

            if (v > 63)
              v = 0;
            out[byteIndex] = v;
          }
        }
      }
      work->transferMode = 2;
      work->transferValue = 70;
    }
    if ((u32)(frame - 447) <= 15) {
      s32 step = frame - 446;

      top = 136 - step * 8;
      for (i = top; i != 0; i--) {
        u8 v = (top - i) * 2;

        if (i <= 127) {
          if (v > 63)
            v = 63;
          if (top - 8 < i)
            v = 0;
          off = ((i & 7) << 3) + ((i >> 3) << 10);
          for (x = 0; x != 128; x += 8) {
            for (m = 0; m != 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if (frame > 446)
      DrawParticles();
    if ((u32)(frame - 447) <= 194) {
      void *verts = Heap_AllocEwram(80);
      struct RenderModel *model = Render_AllocateModelList(1);
      s32 scroll;
      struct Spans4 spans;
      struct Spans4 spans2;

      texture.logWidth = 5;
      texture.logHeight = 6;
      SceneTransform_ResetMatrix();
      model->kind = 4;
      model->mesh = Data_0819a0d4;
      model->vertices = verts;
      if (frame <= 563) {
        scroll = 0x1000;
        if (frame > 500) {
          scroll = (s16)(scroll - (frame - 500) * 64);
          if (scroll < 0)
            scroll = 0;
        }
        texture.pixels = work->sheet + 0x1810 + scroll;
        if (!(frame & 7)) {
          s32 s;

          spans = Data_08196f9c;
          s = FindFreeSpark();

          if (s != -1) {
            s32 r = Random16() & 127;

            sparks->life[s] = -0x7c00;
            sparks->x[s] =
                (spans.p[(r + 1) >> 5].a + Random16() % spans.p[(r + 1) >> 5].b)
                << 16;
            sparks->y[s] = (r << 16) + 0x100000;
            sparks->vx[s] = (Random16() & 15) * 3 << 11;
            sparks->vy[s] = -0x8000 - ((Random16() & 31) * 3 << 11);
          }
        }
        model->texture = &texture;
        Render_TransformVertices(Data_0819a084, verts, 10);
        Render_DrawModelList(model);
      }
      if (frame <= 563) {
        scroll = 0x1000;
        if (frame > 470) {
          scroll = (s16)(scroll - (frame - 470) * 64);
          if (scroll < 0)
            scroll = 0;
        }
        texture.pixels = work->sheet + 0x1810 + scroll;
        if (!(frame & 7) && frame <= 501) {
          s32 s;

          spans2 = Data_08196fbc;
          s = FindFreeSpark();

          if (s != -1) {
            s32 r = Random16() & 127;

            sparks->life[s] = -0x7c00;
            sparks->x[s] =
                (spans2.p[(r + 1) >> 5].a + Random16() % spans2.p[(r + 1) >> 5].b)
                << 16;
            sparks->y[s] = (r << 16) + 0x100000;
            sparks->vx[s] = (Random16() & 15) * 3 << 11;
            sparks->vy[s] = -0x8000 - ((Random16() & 31) * 3 << 11);
          }
        }
        if (!(frame & 3)) {
          s32 n = FindFreeParticle();

          if (n != -1) {
            work->particles[n].x = 0x200000;
            if (frame <= 509)
              work->particles[n].x += (Random16() & 63) << 16;
            else
              work->particles[n].x += (Random16() & 31) << 16;
            work->particles[n].y = 0x7f0000;
            work->particles[n].velocityX = (Random16() & 15) << 12;
            work->particles[n].velocityY =
                -0x8000 - ((Random16() & 31) * 7 << 11);
            work->particles[n].variant = (Random16() & 3) + 1;
          }
        }
        model->texture = &texture;
        Render_TransformVertices(Data_0819a0ac, verts, 10);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    // The five light bars sweeping in from the edges.
    if ((u32)(frame - 513) <= 209) {
      s16 shade = 723 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(0, 127, frame - 512, shade);
    }
    if ((u32)(frame - 522) <= 209) {
      s16 shade = 732 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(0,
                    (Iwram_MulQ16((frame - 521) * 0x10000, 0x400) + 0x7b0000) >> 16,
                    frame - 521, shade);
    }
    if ((u32)(frame - 531) <= 209) {
      s32 x = 0x40000;
      s16 shade = 741 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      x -= Iwram_MulQ16((frame - 530) * 0x10000, 0x400);
      BattleEffect_DrawDiagonalCanvasLine(x >> 16, 127, frame - 530, shade);
    }
    if ((u32)(frame - 535) <= 209) {
      s16 shade = 745 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(0,
                    (Iwram_MulQ16((frame - 534) * 0x10000, 0x600) + 0x780000) >> 16,
                    frame - 534, shade);
    }
    if ((u32)(frame - 554) <= 209) {
      s32 x = 0x60000;
      s16 shade = 764 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      x -= Iwram_MulQ16((frame - 553) * 0x10000, 0x600);
      BattleEffect_DrawDiagonalCanvasLine(x >> 16, 127, frame - 553, shade);
    }

    // The healing: rings turning about the centre, then the great glow.
    if ((u32)(frame - 639) <= 110) {
      s32 sinT = Trig_Sin(0x7800);
      s32 cosT = Trig_Cos(0x7800);

      for (j = 0; j != 80; j += 8) {
        if (638 + j < frame && frame < 670 + j) {
          s32 t = frame - j - 638;
          s32 ry = t >> 1;
          s32 tile = gData_08197410[0] + t / 4 * 0x302;
          s32 shift = j >> 1;

          for (i = 0; i != 128; i++) {
            s32 a = i << 9;
            s32 sx = (t >> 2) * Trig_Sin(a);
            s32 sy = ry * Trig_Cos(a);
            s32 px = Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy * 2, sinT);
            s32 py = Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy * 2, cosT);

            px = (px >> 16) + shift;
            draw[0](canvas, work->sheet + tile, px + 64, (py >> 16) - shift + 63, 1, 2);
          }
        }
      }
    }
    if ((u32)(frame - 755) <= 2 || frame == 781)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    if (frame == 754) {
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x5810, 16, 32, 0x10000);
      Func_0815b434(work->sheet + 0x1c10, 32, 0x10000);
      for (i = 0; i != 32; i++) {
        Dma_Set(work->sheet + 0x1c10 + i * 16, work->sheet + 0x1810 + i * 32,
                0x84000008, REG_DMA3);
        for (j = 0; j != 16; j++)
          work->sheet[0x1820 + i * 32 + j] = work->sheet[0x181f + i * 32 - j];
      }
      for (i = 0; i != 32; i++)
        Dma_Set(work->sheet + 0x1bf0 - i * 32, work->sheet + 0x1c10 + i * 32,
                0x84000010, REG_DMA3);
      for (i = 1; i != 8; i++) {
        u8 *src = work->sheet + 0x1810;
        u8 *out = work->sheet + 0x1810 + i * 0x800;
        u8 v = 64 - i * 8;

        for (j = 0; j != 0x800; j++) {
          if (v < src[j])
            out[j] = v;
          else
            out[j] = src[j];
        }
      }
    }
    if ((u32)(frame - 759) <= 30) {
      s32 sinT = Trig_Sin(0x5800);
      s32 cosT = Trig_Cos(0x5800);
      s32 t = frame - 758;
      s32 tile = gData_08197410[5] + t / 4 * 0x302;

      for (i = 0; i != 96; i++) {
        s32 a = i * 384;
        s32 sx = Trig_Sin(a) * (t << 1);
        s32 sy = t * Trig_Cos(a);
        s32 px = Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy, sinT);
        s32 py = Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy, cosT);

        draw[0](canvas, work->sheet + tile, (px >> 16) + 101, (py >> 16) + 26,
                6, 12);
      }
    }
    if ((u32)(frame - 783) <= 46) {
      s32 sinT = Trig_Sin(0xe800);
      s32 cosT = Trig_Cos(0xe000);
      s32 t = frame - 782;
      s32 tile = gData_08197410[5] + t / 6 * 0x302;

      for (i = 0; i != 197; i++) {
        s32 a = i * 384;
        s32 sx = Trig_Sin(a) * (t >> 1);
        s32 sy = (t << 1) * Trig_Cos(a);
        s32 px = Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy, sinT);
        s32 py = Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy, cosT);

        draw[0](canvas, work->sheet + tile, (px >> 16) + 93, (py >> 16) + 32, 6,
                12);
      }
    }
    if (frame > 758) {
      void *verts = Heap_AllocEwram(64);
      struct RenderModel *model = Render_AllocateModelList(1);
      s32 place[3];
      s32 grow[3];
      s32 s;

      texture2.logWidth = 5;
      texture2.logHeight = 5;
      texture2.pixels = work->sheet + 0x5810;
      SceneTransform_ResetMatrix();
      place[0] = 0x200000;
      place[1] = -0x180000;
      place[2] = 0;
      SceneTransform_ApplyPosition(place);
      Func_080150e4(0x9800);
      grow[0] = 0x10000;
      grow[1] = (frame - 758) << 10;
      if (grow[1] > 0x10000)
        grow[1] = 0x10000;
      grow[2] = 0;
      Func_080151ac(grow);
      model->kind = 4;
      model->mesh = Data_0819a160;
      model->texture = &texture2;
      model->vertices = verts;
      Render_TransformVertices(Data_0819a140, verts, 8);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
      s = FindFreeSpark();
      if (s != -1) {
        sparks->life[s] = (Random16() & 15) << 15;
        sparks->x[s] = 0xef0000;
        sparks->y[s] = (Random16() & 255) << 16;
        sparks->vx[s] = -(Random16() & 31) << 12;
        sparks->vy[s] = ((Random16() & 31) - 16) << 12;
      }
      if (!(frame & 3)) {
        s32 n = FindFreeParticle();

        if (n != -1) {
          s32 spin = Random16() & 3;

          work->particles[n].x = 0x600000;
          work->particles[n].y = ((Random16() & 7) << 16) + 0x240000;
          work->particles[n].z = frame;
          work->particles[n].velocityX =
              Iwram_MulQ16((spin + 2) << 15, Trig_Sin(0x9800));
          work->particles[n].velocityY =
              -Iwram_MulQ16((spin + 2) << 15, Trig_Cos(0x9800));
          work->particles[n].variant = (Random16() & 3) | 16;
        }
        n = FindFreeParticle();
        if (n != -1) {
          work->particles[n].x = ((Random16() & 31) << 16) + 0x600000;
          work->particles[n].y = (Random16() & 31) << 16;
          work->particles[n].z = frame;
          work->particles[n].velocityX = -((Random16() & 15) << 13);
          work->particles[n].velocityY = (Random16() & 15) << 14;
          work->particles[n].velocityZ = 0x8000;
          work->particles[n].variant = ((Random16() & 3) + 1) | 0x110;
        }
      }
    }
    if (frame > 782) {
      if (frame & 2)
        gBgScroll[1].y = gBgScroll[1].y + (frame & 1) + 2;
      else
        gBgScroll[1].y = gBgScroll[1].y - (frame & 1) - 2;
    }
    if (frame > 816) {
      for (i = 0; i != 128; i++) {
        s32 c = FADE_PLTT[i];
        s32 r = 31;
        s32 g = (u16)c >> 5 & 31;
        s32 b = (u16)c >> 10 & 31;

        r &= c;

        if (r <= 30)
          r++;
        if (g <= 30)
          g++;
        if (b <= 30)
          b++;
        FADE_PLTT[i] = b << 10 | g << 5 | r;
      }
    }
    frame++;
    work->transferPending = 1;
    wait(1);
  }

  camera[27] = savedZoom;
  HeapSlot_Release(SLOT_BLITTER);
  Func_08191cc4();
  Iwram_FillWords(canvas, 0x4000, 0);
  while (((volatile struct DmaRegisters *)REG_DMA3)->control & 0x80000000) {
  }
  work->transferPending = 1;
  wait(1);
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  gBgScroll[1].x = savedScroll;
  for (i = 0; i != 36; i++)
    ResourceObject_ReleaseFar(objects[i]);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
