// Differ 90 of 902 (tla-en, 2026-11-08, -mtune=gba). Re-measured 8 November,
// later: unchanged; the pin is a frame question (without it the frame is 120
// bytes, not 132, 941), and a 12-byte unused local in five places does not
// restore it (933 to 937). The grid's column step
// is listed c++, b++, d++, column++, x += 8 (94 to 90; all 120 step orders and
// all 1,260 orders of the row-top statements measured, nothing under 90; 8
// minutes of permuter on the grid, seed 4417, 90). The pin is still needed
// (941 without). The palette copy is a plain Iwram_CopyWords call; the pin on
// q is tagged.
// 2026-11-07, twelfth pass, 140 to 94, no new device. The vertex from
// (row * 16 + column) * 4 (a vertex from c or q is not reduced, q being a
// hard register; row * 16 is hoisted, merged with the pinned copy, and the
// vertex starts from Q << 2 as in the native), which also puts the row
// slots in the native's order (row * 15 at 32, the + 15 at 28); c set
// before b gives b r5 and c r6 (140 to 114). Statement order alone in the
// frame loop: row = 1 before angle = frame << 11, the waves' column = 0
// before their vertex start, i = 0 before height /= 2 (114 to 100); in the
// grid, column = 0 and x = -60 before d, c, b (100 to 94, the best of all
// 720 orders of the row-top statements).
// Left, about 70 of the 94: d takes ip and the + 15 copy slot 8 (the
// native has them the other way round), and the two extra instructions
// shift every literal-pool offset. In greg d ranks 11 refs over 50 insns
// against the copy's 11 over 45. The copy is a moved insn, so loop puts it
// right before the column loop, after any row-level init of d: no order
// of the row statements can make d's life as short as the copy's (at best
// 46 against 45). What does give the native slot 8 for d and ip for the
// + 15 is a row-level bottom = v + 15 made non-replaceable by a dead
// bottom = 0 before the row loop (FAKEMATCH(dead)): but loop then charges
// the user variable a copy cost (benefit 5 - 4 - 2), leaves the + 15
// unreduced (sl + 15 each row, no slot 28) and the frame is 4 bytes short
// (435). A + 15 from row * 17 - row + 15 is reduced but on other slots
// (220). Not gained: d's init in a do { } while (0) (depth comes from real
// loops), d as a giv of the column (from row * 16, i, v, a, b or c: 356 to
// 944), u8 or other types for d, extra uses of d that combine might fold
// (cse or the front end folds them first), an empty asm using v + 15 (104),
// the permuter on the grid (15 minutes, 94). Also left: in the waves the
// + 64 and the store are scheduled before column++ (6).
// 2026-11-07, eleventh pass, 150 to 140, with a device: the native-like grid
// (b, c and d as row counters from Q, a = i + column left unreduced, the quad
// from (row * 15 + column) * 24 in the if block, so the row slots and quad
// start match) plus Q pinned to r4 (register asm, a pin: needs
// FAKEMATCH(pin) on adoption, or a milder device), the vertex from
// (c - 16) * 4, the + 15 as v + 15, and quad[20] and quad[21] moved (the
// permuter, 10 minutes). Without the pin this form is 401. The frame loop
// now nearly matches; what is left is in the grid: the vertex starts from
// (c << 2) - 64 rather than Q << 2 (a vertex from q is not reduced: q is a
// hard register), the + 15 takes slot 8 and d ip (the native has them the
// other way round), b and c are in r6 and r5, and the + 15 slot is 32
// rather than 28. Barriers (asm "+r") on v, Q or the vertex start:
// 766 to 963.
// Before this pass: 150 of 902, with b read through a dead store to frames
// (now gone) and the quad stepped from mesh + row * 15 * 24.
// 2026-11-07, ninth pass, 158 to 150: the quad as a pointer stepped by 24 in
// the column loop from mesh + row * 15 * 24, and d, vertex and quad[5] read
// from v rather than i (an enumeration over i, v and row << 4 at each use).
// The + 15 now takes ip as in the native; Q is still r0, vertex r4, d lr
// beside a stepped slot 8, and the quad start row * 360. Not gained:
// i replaced by height, phase, frame or frames (frame -8: 862 to 874), the
// vertex walked as a pointer (964 to 968), quad[17], [19] or [7] from v (303,
// 183), a from v (1001), the permuter, 8 minutes (154 by a constant local).
// 2026-11-07, eighth pass, unchanged at 158. Q (the row base copy) takes r0
// because greg copies i's preferences to it through Q = i (i dies there),
// and i prefers r0 from the splash's (i << 14) shift into the call argument;
// u then skips r0 as someone else's preference. With the splash at a
// constant (diagnostic only) u takes r0. quad[20] = u after quad[19] (u 22
// insns) gives vertex r1, but Q still r0 and u r4 (618). Not gained: Q or i
// pinned to r4 (879, 931), a row-local base for i (852 to 869), i = v at the
// row end or in the column (943), the shift's spellings or i copied through
// an empty asm (618 to 965, 197), the splash on its own counter (frame -8,
// 947), the permuter on the grid, 8 minutes (best 160).
// 2026-11-07, seventh pass, devices allowed, 162 to 158: the permuter on the
// grid (14 minutes) found only a dead store to frames in b. Not gained: a
// barrier on the quad row start (mesh + row * 15 * 24 computed per row as in
// the native, but row * 15 is then found first and the three row slots rotate:
// 166 to 252), on firstQuad (426) or on quad (847); asm uses of d or the + 15
// (460, 728); d at function scope or set from v or row (162); volatile d (280).
// 2026-11-07, sixth pass, 178 to 162: the water mesh read through one
// function-level pointer (mesh, set once and also used for model->mesh) fixes
// the frame loop's reloads after the grid; the grid itself is unchanged (d in
// ip, the + 15 in slot 8, Q copied to r0, vertex r4). No device paid: pins of
// u r0, vertex r1, d ip (206 to 957), barriers on d or i (704 to 949),
// volatile d (280), a Q local (210), the + 15 as a local (477), a, b or c from
// d (238 to 815), the permuter (best 162 before this, and on it).
// 2026-11-07, fifth pass, 180 to 178: quad[1]'s corner index d as its own
// counter (set from i + 17 at the row top, stepped with the column) puts the
// spilled slot 8 where the native has it, but d still takes ip and the
// + 15 the slot: in greg the column loop's + 15 copy and d tie (11 refs
// across 45 each) and the lower pseudo number, the + 15, wins the lo
// register that reload then spills; the native needs d first. Not gained:
// a pin of the + 15 to ip (479 to 968), quad[20] = u moved later (662 to
// 962), a or vertex from v (758, 326), the counter on the quadColumns form
// (260), c from d (815), the permuter, 9 minutes on the grid (best 176, by
// temporaries only, not kept).
// 2026-11-06, second pass: the vertex start fold is gone. Its source was GCSE,
// not cse: the if block's row << 4 (quad[5] and row * 15's (row << 4) - row)
// is redundant with v, so GCSE turns it into a copy of v with a REG_EQUAL
// (row << 4) note, loop hoists and reduces it as 138, and cse2 folds the
// vertex start (624 << 2) through that note into row << 6. firstQuad = row * 15
// computed once at the top of the row makes cse merge v into that row << 4,
// the note never appears, and vertex starts from i << 2 as in the native.
// quad[1] from i + column + 17 rather than a local d then gives 180 (the
// permuter found it; with d, 277). The frame loop now nearly matches; what is
// left is in the grid: firstQuad sits before the hoisted givs, so the outer
// loop finds row * 15 first, reduces the quad start as row * 360 (stepped by
// 360) instead of row * 15 with * 24 per row, and its spill slots rotate
// (row * 15, + 15, - 84 should be sp+32, 28, 24); d is ip, (row << 4) + 15 the
// spilled slot 8, and Q is still copied (adds r0, r4, #0) from i. Not gained:
// v as its own row counter stepped by 16 (950 to 962: gcse copy-propagates
// i = v, the frame shrinks), i = row << 4 with no v (960), quad[5] or the
// + 15 from i or v (538 to 626), a - row or i - row for the quad (856), the
// row's - 84 and + 15 also computed at the top (467), firstQuad in the column
// loop (614), row * 3 * 5 (folded back to row * 15).
// 2026-11-07, third pass, differ unchanged at 180. Where the note comes
// from: expand_mult puts REG_EQUAL (mult row 16) on row * 15's (row << 4)
// temporary; GCSE replaces that temporary with a copy of v and keeps the
// note, which becomes (row << 4). A plain row << 4 (quad[5]) gets no note.
// So any row * 15 synthesized inside the column loop folds the vertex start,
// and firstQuad at the row top avoids that only by making loop reduce the
// quad's start in its first pass (hence the rerun's row * 360). The form
// closest to the native grid: no firstQuad, and the quad indexed by
// row * quadColumns with s32 quadColumns = WATER_COLUMNS - 1 at the row
// top. The multiplier is a register, so nothing is synthesized and no note
// is made; row * 15 is reduced alone (sp+32, + 15 at 28, - 84 at 24, as in
// the native), the quad start is slot 32 * 3 << 3 each row, and vertex starts
// from Q << 2. That gives 262 (with d) or 273, because the column loop's
// ranks are still u 1.89 > Q 1.551 > vertex 1.545, where the native needs
// vertex > u > Q: Q is copied to r0, vertex goes to r4 and the frame loop's
// reload rotation stays off. Not gained on that form: vertex after u and a,
// or from a (272); u declared in the if block (815); pins for vertex r1 and
// u r0 (390 to 478); quad[5] or the + 15 from i or v (550 to 962); the
// permuter, 12 minutes (186). Also not gained: (row << 4) - row or v - row
// written out (loop no longer sees the quad as a giv: 868, 439); a copy of
// firstQuad or a barrier on it in the column loop (180, 268, 442).
// 2026-11-07, fourth pass, on the quadColumns form, differ unchanged at 180.
// Weighted refs over live length in its lreg dump: u 12 over 19 (1.89),
// Q 19 over 49 (1.551), vertex 17 over 44 (1.545), so vertex must reach
// 43 insns or fewer and u 24 or more for vertex > u > Q. Q collects the
// inits' refs because regmove rewrites them from i to Q after the copy
// Q = i. Not gained: u declared first (u 20 insns, 272), a first or vertex
// last (272, 273), vertex, b, c and d from (row << 4) (832 to 962: the
// frame shrinks), a vertex pointer stepped from the row top (957), and an
// enumerated permuter over those declaration orders and spellings (best 272).
// Earlier, at 266 of 902:
// 2026-11-06, devices allowed: none gains. Pins (u r0, vertex r1, Q r4, i r4)
// 493 to 1009; an empty asm on i 956 (the frame shrinks); i, (row << 4) or a
// stepped i at each grid use 832 to 986; quad[5] from i or v 626 (still row << 6).
// The frame loop's rest follows reload's spill rotation out of the grid (offset 3
// of 6 at its first reload), so only the native grid allocation fixes both.
// Permuter, 20,000 candidates over the whole file: 266.
// 2026-11-06: allocation only: the grid's (Q, u, vertex) and, after it, 140 of
// the 266 in the frame loop's reloads. Permuter (5 minutes): best 264, by moving
// quad[12] = a, store-order noise as below, not kept.
// The grid's allocation (Q,
// u, vertex) and, after it, 140 of the 266 in the frame loop's reloads. The
// cse2 dump shows why vertex starts from row << 6: loop copies i into 624 for
// vertex's start (624 << 2), and cse2 folds that through 138's REG_EQUAL
// (row << 4, kept when the outer loop reduced 138 to 632). The native starts
// vertex from Q (lsls r3, r4, #2); the draft's row << 6 needs one more reload
// (mov r7, fp), and the frame loop's reload registers follow from it. Shown by
// column = 0 before the row loop and again after the column loop, with no
// init in the column for: the starts become (Q + column) << 2, the frame loop
// matches (rest 140 to 65) and differ is 202 of 908, but the six extra
// instructions add column to every start, so it is not the source and is not
// kept. No gain: u8 or u32 for u, a, b, c, d, x, v, i, row, column (266 to
// 960); row * 16, (u32)row << 4 for v; i, v or casts for quad[5], [17], [19]
// (626); vertex spelled from i * 4 + column * 4 or &cells[..] (266); i stepped
// by 16 in the row for (950 to 957); the waves loop on its own row or column
// (314 to 849); column = 0 at the top of the row (266).
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
  u8 *mesh; // The water's mesh, read through one pointer.
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(1);
  mesh = gData_02014000;
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
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
  }

  // The water's grid: 16 by 8 vertices, and a quad (two triangles) for
  // each cell but the last row and column.
  for (row = 0; row != WATER_ROWS; row++) {
    s32 b, c, d;
    s32 v = row << 4;
    s32 x;
    s8 *cells = (s8 *)gMapCellBuffer;

    /* FAKEMATCH(pin): q held in r4; as a plain local, 941 of 902 instructions
     * differ in tla-en. */
    register s32 q asm("r4");
    q = v;
    i = v;
    column = 0;
    x = -60;
    d = q + 17;
    c = q + 16;
    b = q + 1;

    for (; column != WATER_COLUMNS;
         c++, b++, d++, column++, x += 8) {
      s8 *vertex = cells + (row * 16 + column) * 4;
      s32 a = i + column;
      s32 u = column * 8;

      vertex[0] = column * 11 - 82;
      vertex[1] = 60;
      vertex[2] = row * 24 - 84;
      if (row <= 6 && column <= 14) {
        u8 *quad = mesh + (row * 15 + column) * 24;

        quad[4] = x + 67;
        quad[6] = x + 67;
        quad[18] = x + 67;
        quad[7] = v + 15;
        quad[8] = u;
        quad[9] = v + 15;
        quad[16] = u;
        quad[21] = v + 15;
        quad[5] = q;
        quad[17] = q;
        quad[19] = q;
        quad[0] = b;
        quad[20] = u;
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
    row = 1;
    angle = frame << 11;
    for (offset = 64; row != 7; row++, offset += 64) {
      s8 *vertex;

      for (column = 0, vertex = (s8 *)gMapCellBuffer + offset + 1;
           column != WATER_COLUMNS; column++) {
        s32 amplitude = (16 - column) * height / 32;

        *vertex = ((Trig_Sin(angle + column * 5000 + Trig_Sin(row * 3553)) * amplitude) >> 16) - amplitude + 64;
        vertex += 4;
      }
    }
    i = 0;
    height /= 2;
    for (bubble = work->particles; i != 20; i++, bubble++) {
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
    model->mesh = mesh;
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
