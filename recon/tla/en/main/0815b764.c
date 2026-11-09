// Draft, differ 36 of 1932 (tla-en, 2026-11-09); 36 in every edition
// 2026-11-09 (round 13), unchanged at 36: dead tests (FAKEMATCH dead) do not
// reach what is left. A reload trace of the listing's order (shade = 0 right
// after the model's allocation, j = 0 after model->vertices: 49) shows the
// asm's frame reload picking r7 (round-robin index 5), then the vertices
// reload r0 and j's zero r1 where the listing has r1 and r2: one pick short.
// A dead test there (if ((s32)model == 300) j = 1; and the like) gives that
// pick, but its label ends reload inheritance, so the giv inits reload frame
// instead of reading r7 (148 to 151). The frame * 3 rows need t * 3072's init
// before -t * 512's: -t * 512's init takes r7 for its 38912, and frame * 3
// reads r7 in the listing, so only the giv order can do it; dead tests on
// t * 3072 or -t * 512 before or after the yaw (161 to 1176) and the asm
// moved to each later slot of the setup (49 to 1230) do not. The else arm's
// 0x1c000 is a local pseudo tied to its result (loop.c: savings 1, life 1,
// not desirable to move); in the listing it is rebuilt by reload in r3.
// 2026-11-09 (round 12), 40 to 36, no new device: a single-move search over
// the ship-burst setup's statements (asm included) puts model->shade = 0
// after model->vertices, so the shade's zero store no longer precedes the
// giv inits (rows 1335 to 1338 now match). Left: the zero (shade and j share
// r0 here; the listing builds shade's zero in r3 early and j's in r2 late:
// j = 0 last, shade first gives exactly those two stores but 49 with one more
// instruction, because the vertices' reload then rotates to r0 and t's init
// reloads frame), the giv inits' frame * 3 (about 24), the 0x1c000 (4: a
// function-level constant local for it reads 1259 to 1277) and the palette
// step pair (2).
// 2026-11-09 (round 11), unchanged at 40, a few attempts. The loop dump
// lists the ship-burst givs in reverse discovery order, which is their init
// order: t << 9, t << 11, -t * 512, -t, t * 3072, t * 3, t * 2, t; t * 2 and
// t * 3, synth_mult's steps of t * 3072, are reduced as givs of their own,
// so their inits come after t * 3072's. The listing's frame * 3 before
// -t * 512's init needs a giv or a loop-moved insn met after -t * 512 in the
// body. Not better: frame * 3 spelled into sc (558); a yaw local at the j == 0
// arm's top (1158) and 0x1c000 through a local at the loop body's top
// (1256): each new block local renumbers the pseudos after it, and the
// frame changes.
// 2026-11-09 (last stretch), unchanged at 40. The else arm's 0x1c000 and the
// ship-burst's frame reloads fit the pattern the other drafts show: in the
// listing a constant or spilled value is rebuilt by reload at its use, and
// each such reload turns reload's register rotation; a pin makes it an
// ordinary insn and moves the rotation (the 265 of the pinned r3). Thumb's
// subsi3 takes only registers, so loop.c's single-use replacement cannot fold
// 0x1c000 into the subtraction. 12 minutes of permuter (seed 9191): nothing
// below 40 (best 42).
// 2026-11-09 (devices pass), unchanged at 40. The else arm's 0x1c000 as a
// pinned r3 (265), through an asm (310) or a local (40), its shift through an
// asm (419); identity inlines on t, t * 3072, sc, the yaws or t << 11 (40,
// or 331 to 1209 where they reach t's giv); the palette copies or the fills
// through an inline that returns the routine (40). 12 minutes of permuter
// (seed 6161): only j = model->shade = 0 (44, weighted better).
// Every edition measures the same 40 (tla-ja, tla-de, tla-es, tla-fr, tla-it):
// the function's code is one form in all six, so what is left is the C, not an
// edition difference.
// 2026-11-08, third pass, from 42: the first closing fill reads its destination
// into a local before its once-only block (40). Its loop note is a scheduling
// barrier after the destination's ldr, so the size, the zero and the call
// address follow it as in the listing. Left (about 34 rows, all in two places):
// - The ship-burst block (draw && frame <= 131), about 30 rows. The listing
//   computes frame * 3 from r7 (the frame reload) before the -t * 512 init uses
//   r7 as its temporary, and only then sets the t * 3072 giv (r7) from it; so
//   t's own init reloads frame into r5 instead of copying it. The giv steps run
//   r9, r10, r8, r7 on both sides, so the inits go out in the same giv order
//   (t << 9, t << 11, -t * 512, t * 3072, t); one init cannot interleave with
//   another, so something before -t * 512's init must already compute frame * 3
//   (a hoisted invariant, an earlier giv such as t * 3 placed after the yaw in
//   the body, or a statement before the loop) for CSE to reuse. Not found:
//   frame * 3 or frame * 3072 spelled into sc, with or without t inlined (the
//   outer frame loop then reduces it and the frame grows: 419 to 558); an asm
//   reading frame * 3 at or after the model (54 to 388); a t * 3 giv forced
//   into the else arm (46). The two zeros: the shade store shares j's zero
//   here (r0); j = model->shade = 0 gives the listing's r3 for shade but 44; j
//   = 0 in every slot of the setup crossed with the asm at four slots (40 at
//   best, as now, up to 1268). A focused permute of that block (8 minutes,
//   about 10,000 candidates): nothing below 40.
// - The palette loop's step pair (2 rows): the listing steps pal before the
//   other two givs. Computing the value before the store (a local c, or g |
//   ... ) gives that order through the store's address giv, but its later init
//   then lengthens pal's life, so pal takes r4 and the givs r0 and r1 (56); a
//   non-volatile pal, &pal[i] first, operands swapped, * 1024: 40 to 121.
// - The else arm's 0x1c000 - (t << 11), 4 rows: the constant takes r0 here
//   (local-alloc ties it to the result) and r3 in the listing; every spelling
//   measured 40; probably follows the ship-burst block.
// 2026-11-08, second pass, unchanged at 42: the devices re-measured (without the
// particle loop's asm 44, without the ship-burst asm 317; that asm after the
// model's allocation 42, reading frame - 76 390); t, sc and the yaw in every
// equivalent spelling (42), the j arms swapped (365); the particle's variant
// stored first, last or in a once-only block (46 to 48); the palette's value
// operands swapped (42), its red as a local (129), a stepped pal (124); the
// first closing fill doubly blocked or the wait blocked (42), canvas for its
// destination (72).
// 2026-11-08, from 50: once-only blocks around the first two closing fills
// (after the wait(1) that follows REG_BG1CNT) put each destination's load ahead
// of its size, as the listing has them (44, then 42; around the third, the
// wait or REG_BG1CNT no gain or worse). Re-measured the devices: the particle
// loop's asm is still worth 2 (a once-only block in its place, or around one
// of its stores, 52 to 1301); the ship-burst block's asm still holds 283 (325
// without; once-only blocks around each of that block's statements instead,
// 325 or 327; the asm after the model's allocation also 42, with frame's ldr
// then two rows early). j = 0 after model->vertices gives the listing's late
// zero store but 53 (the giv inits then reload frame). The palette loop's
// pointer step (pal + i, &pal[i] at the top, a stepped pal): 42 or worse.
// Left: the ship-burst giv order (about 30 rows), the first closing fill's
// size still set before its destination's load (a local for the destination:
// no gain), the palette loop's step pair.
// 2026-11-07, from 52: an empty asm (FAKEMATCH barrier) after variant in the
// frame 133 particle loop settles that step pair (50). Left as below.
// 2026-11-07, from 62: gData_0819746d and gData_08197473 declared without
// const (54, then 52; found by permute as a volatile global): their loads then
// stay behind the stack-argument stores, so both blits' str of w falls where the
// listing has it. No new device. Tried and worse: a register-asm local for sc or
// yaw (kills the giv: 81 to 1213), frame pinned to r7 (430), an empty or memory
// asm after the closing wait(1) (88; sched2 still hoists the 0x03000000 base).
// Permute on 62 and 52 bases (10 and 8 minutes): nothing below 52. Left: the
// ship-burst giv order (about 36 rows), the closing fills (about 10), the
// palette and frame 133 step pairs (2 each).
// 2026-11-07, from 68: three loops take their pointer as &array[i] at the top of
// the body (frame 0 and frame 133 particles, the spark loop), so the hoisted
// constants and step inits go out first, as the listing schedules them (66, 64,
// 62). Left: the ship-burst giv order (a top-of-loop yaw local gives the
// listing order, sc init before -t * 512, but swaps r7 and r8: 150 to 185),
// the closing fills (0x03000000 hoisted above wait here) and single str/step
// pairs in the palette, frame 133 and blit-argument rows.
// 2026-11-07, from 76: the ship position is one struct (x, z) on the stack, not
// two locals (68; an s32[2] gives the same); its slots then sit where the
// listing keeps them and the setup and placement rows agree. Not worked further:
// the ship-burst giv order and single scheduling pairs, as below.
// 2026-11-07, from 82: the listing adds the offset before the base in q and
// texture.pixels; integer sums of the pointer give that (78, then 76), and the
// fade loop steps src and dst at its end (80). Two zeros at the ship-burst
// block (j = 0 after model->vertices, 91) and store-order tries stay worse.
// 2026-11-07, from 91: j = 0 as its own statement before model->shade (the loop
// then starts from j) gives the listing's count; j = 0 elsewhere in the setup
// is 86 to 412. Left: the shade store now shares j's zero, and frame * 3 still
// reloads frame. Loop dump: giv inits go out in reverse insn order, so the
// listing's t << 9, t * 3072, t << 11, -t * 512 needs -t * 512 earliest; a yaw
// local plus a once-set t << 11 gets that order but costs 160 to 170.
// 2026-11-06, from 205: sc = t * 3072 - 0x12000, the listing's form (a giv of
// t * 3072 in r7, -0x12000 added at the use), with an empty asm reading frame
// between the two allocations of the ship-burst block. The listing reloads
// frame into r7 there and its step inits for t << 9, t * 3072 and t << 11 take
// it from r7. The sc form alone is 322; the asm alone 197 to 200; the asm
// before model->shade 93, before model->vertices 100, after it 377, before
// verts 322. No local copy of frame does it (copy-propagated: 322 or 436).
// Left in that block: the step inits run t << 9, t << 11, the -t * 512 init
// (which reuses r7), then t * 3072, which therefore reloads frame (r0); the
// listing's order is t << 9, t * 3072, t << 11, -t * 512, so its frame * 3
// still reads r7. j's zero store also lands two rows late. Elsewhere: single
// scheduling pairs (frame 133's blit, the closing fills, the fade loop).
// 2026-11-06, from 261: REG_BLDALPHA = ((frame - 108) * 2) << 8 | 16 (EVB in the
// high byte) gives the listing's value r2, address r3, 16 in r1 (243); place[1]
// is stored before place[3] (259 from 261); the ship placement loop uses the
// function's i, which the listing keeps in r9 there and in the frame == 266 loops
// (alone 322: the loop then matches but for reload registers); with
// sc = (t - 24) * 3 << 10 as well, 205 of 1932, draft 1929. That sc form is
// not the listing's: the listing reduces t * 3 * 1024 as a giv (r7) and adds
// -0x12000 at the use, where this form reduces t * 3 - 72 and shifts at the
// use (3 instructions fewer); its gain is the reload rotation it gives the
// later blocks, which are now nearly all common. The reload registers rotate
// round-robin (frame r7, verts r1, j's zero r2 in the listing), so one block's
// reload count shifts every block after it. Order permutations of loaded,
// shipX, shipZ and transition[4] on this base: all worse (211 to 233).
// 2026-11-05, differ 261 of 1932 unchanged. Diagnosis of the ship-burst block by a
// consumed reload: __asm__("" : : "r"(frame)) before model->vertices = verts gives
// the listing's count (1932) and 226; adding a second consumed reload gives verts r1
// and j's zero r2 as in the listing (219). So reload order is r4 (the cmp's frame), r7
// (frame, inherited through the giv inits), r0 busy or used, r1 verts, r2 zero, r3 and
// r4 the giv constants, which reads as frame * 3 (r0 here) being computed before the
// verts store. Giv inits are emitted before the j loop's start, after every statement
// of the block, so that order is not explained. A frame * 3 local before the store
// with sc = (f3 - 228 - j * 48) * 1024 - 0x12000 loses sc's giv (324, 1928). These two
// asm forms were diagnostics only and are not in this draft. After this block the
// place loop (frame - 108) also differs; it was not worked.
// Measured 2026-11-05 (tla-en): differ 261: the model's mesh and vertices are typed pointers, not void.
// Measured 2026-11-05 (tla-en): with function-level i at the ship placement (no inner s32 i)
// the counts agree, 1932 and 1932, at differ 342; the inner i kept here gives 263 and 1931.
// Decl scans (all blocks, moves and 4000 random orders): no effect. The ship-burst block:
// in the listing frame reloads into r7 above the block's stores, two ticks ahead of here;
// locals taking frame or frame - 76 before verts, and every order of the model's statements, do not move it.
// Measured 2026-11-05 (tla-en): differ 263 of 1932, 79 normalized: Task_Add returns s32.
// 2026-11-05, unchanged at 271 of 1932: at the ship-burst block the reload order
// draw r3, frame r4, then verts is the draft's next register, r7; the listing's
// verts is r1, so r7 and r0 were taken or busy there. Tried a frame * 3 local
// before the verts store (326), a start local (563), swapped arms (342): worse.
// Measured 2026-11-05 (tla-en): differ 271 of 1932 (scoping the
// ship-placement loop's i to its block took it from 350). Left: at the
// ship-burst model block the listing reads verts into r1 and j's zero into
// r2, then frame into r7, which the next giv's temporary overwrites, so t
// reads frame again; here verts takes r7, the zero r0 and frame r1. The same
// rows are two reload registers ahead in the listing, with nothing visible
// before that says why. The giv order itself is right: the steps at the
// loop's end run r9, r10, r8, r7 on both sides. Swapping the if (j == 0) arms
// makes the rows after the block agree (257), which shows they follow from
// this one block.

// Draft, differ 780 of 1932, skeleton gap 102 (tla-en, 2026-11-01)
// BattleEffect_RunUlysses: same calls and control flow as the listing.
// The largest step left is a counting question for Pascal, so it is not in this
// draft: forward every Iwram_FillWords and Iwram_ClearWords through
// static __inline__ s32 FillWords_(...) and ClearWords_(...), as f143d80.c does
// untagged and The Broken Seal's iwram_call.h does for every call; CopyWords
// needs none. Measured on a scratch copy of this file, that alone gives differ
// 354, gap 54 and the listing's frame of 152. Without it 0x3c00 and 0x06004000
// are one pseudo across each block (calls.c forces a costly constant argument
// into a pseudo; an inlined parameter stays a constant), local-alloc gives them
// r8 and r10, battle loses r10 to a stack slot at sp+56, the frame is 156 and
// every slot from sp+56 up reads 4 too high. S2 lists wrapper as a device, so
// it stays out until that is decided; if adopted it needs its tag or a ruling.
// Left, measured with the forwards in place:
// - The ship-burst model block (draw && frame <= 131): the listing reloads verts
//   in r1 and frame in r7, here verts in r7 and frame in r1, and t starts from a
//   second ldr of frame, not a copy. The reload order after that block stays one
//   register behind down to the frame loop's end. Not closed by t as the stepped
//   variable (loses the givs and 22 instructions), a start local (one more
//   spill), or moving the terms of t. Find what uses a reload before verts.
// - alpha: settled 2026-11-06 by ((frame - 108) * 2) << 8 | 16.
// - bg2pa and bg2pd: the discarded ldrh before each strh (2 missing instructions,
//   as in 0816c6f8). A volatile form would be a device.
// - q and pixels: settled 2026-11-07 by integer sums, offset first.
// - loaded = 0 and transition[4] = 1: the listing stores transition before
//   shipX with base r4. That store first gives base r2 and rotates the reloads.
// - A few scheduling pairs in frame 133's particle loop, the first model's blit
//   and the frame == 266 loop's p->y.
// Settled, keep these: work is read before transition, so the store of
// transition no longer blocks work's load; mask is set after frame, once, outside
// both loops (S2's one-constant local), so it hoists to r6 and stays apart from
// the pooled halfword 31. step is one
// pointer shared by the particle loop and the last loop over the particles, which
// is what gives it r6 over a (floor_log2 priority), and the frame == 266 loop keeps
// its own p in r5; the loop steps step before i; cellBuffer is a block-level
// constant local with no register, reloaded from the pool, which sets the reload
// order for the next two blocks; j = 0 comes before q is set; x, y and left are
// locals so left is taken before y is read; >> 16 replaces a cast to s16, retired.
// t = frame - 92 is declared after its own range test, so GCSE leaves two givs
// that combine and reduce (sp+12, sp+16); anchor = step + 8 and the spark row as a
// variable keep i * 8 and i * 448 unreduced; req is copied as one 8-byte struct;
// the sheet pointer after the loop is its own variable; pal is indexed; t reloads
// inside each sound test.
// Spill slots from the top, with the forwards: effect, canvas, transition,
// frame, sprites, dmaArmed, skip, blit, shipZ, shipX, loaded, draw, frame - 108,
// verts, model, sparks, &texture, delay, block two's verts and j, anchor,
// frame * 2 - 184, frame - 92, and &req as a caller save.

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
  const u8 *mesh;
  s16 *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
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
#define HI(v) ((v) >> 16)

#include "bg_scroll.h"
#include "input.h"
#include "io_write_queue.h"

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value) {
  return Iwram_FillWords(destination, size, value);
}

extern char ResourceId_Data0B1;
extern char ResourceId_Data0B3;
extern char ResourceId_Data0C7;
extern char ResourceId_Data0C8;
extern char ResourceId_Data137;
extern char ResourceId_Data13A;
extern char ResourceId_Data150;
extern char ResourceId_Data15C;
extern char ResourceId_Data178;
extern u8 gMapCellBuffer[];
extern u8 Data_02010200[];
extern const u16 Data_08197424[];
extern const u16 gData_0819747a[];
extern const u8 gData_08197467[];
extern u8 gData_08197473[];
extern u8 gData_0819746d[];
extern const u16 Data_08198686[];
extern const u8 Data_08198662[];
extern const u8 Data_08198674[];
extern const s8 Data_081986aa[];
extern const u8 Data_08198654[];
extern const u8 Data_0819865b[];
struct PlacementSource {
  s32 unk00;
  s32 unk04;
};

extern const struct PlacementSource Data_08196e54;
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];
extern s32 gCameraSceneParameters[];

void BattleEffect_ProcessPendingBlock2Transfer(void);
void Display_ApplyBg2Affine(void);
void Palette_StepFadeTransfer(void);
void Func_0815b68c(void);
void Func_0815f0a0(s32 palette);
void ResourceObject_ReleaseFar(void *object);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);

// Ulysses: the hero's ship sails out of a whirlpool, its crew of six
// warriors leap at the foe and the ship's broadside lands as a storm of
// explosions.
void BattleEffect_RunUlysses(struct BattleEffectArgument *effect) {
  u8 *canvas;
  s32 *transition;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  s32 dmaArmed;
  s32 skip;
  DrawRectangle *blit;
  struct {
    s32 x;
    s32 z;
  } ship;
  s32 loaded;
  s32 draw;
  s32 i;
  u8 *cells;
  u8 *picture;
  s32 place[4];
  DrawRectangle blits[2];
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct PlacementSource req;
  u8 *battle;
  s32 mask;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  transition = Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  dmaArmed = 0;
  skip = 0;
  BattleEffect_SelectLivingTargets(work->effect);
  for (frame = 0, mask = 31; frame != 32; frame++) {
    volatile u16 *c = (volatile u16 *)(BG_PLTT + 96);

    for (i = 0; i != 128; i++, c++) {
      s32 v = *c;
      s32 r = mask & v;
      s32 g;
      s32 b;

      g = (u16)(((u16)v >> 5) & 31);
      b = (u16)(((u16)v >> 10) & 31);
      if (g == 0) {
        if (b > 0) {
          b--;
        }
        if (r > 0) {
          r--;
        }
      }
      if (g > 0) {
        g--;
      }
      *c = b << 10 | g << 5 | r;
    }
    wait(1);
  }
  BattleEffect_StartCanvasLayer(0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1141, 0x20000);
  wait(1);
  {
    u32 *map = (u32 *)0x0600f800;
    u32 v;
    s32 y;
    s32 x;

    FillWords_(map, 256, -1);
    map += 64;
    FillWords_(map, 128, 0x03ff03ff);
    v = (u32)Data_02010200;
    map += 32;
    for (y = 0; y != 16; y++) {
      for (x = 0; x != 8; x++) {
        *map++ = v;
        v += 0x20002;
      }
      for (x = 0; x != 8; x++) {
        *map++ = 0x03ff03ff;
      }
    }
  }
  BattleEffect_SpawnObjects(8, 0x28f, 3);
  REG_BG2PA = 0x80;
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f46;
  REG_BG1CNT = 0x1f83;
  wait(1);
  blit = blits;
  BattleEffect_FetchRectangleBlitters(0, (void **)blit);
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  cells = work->sheet + 0x1781;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B1, cells, 0, 0);
  for (i = 0; i != 0x2300; i++, cells++) {
    if (*cells) {
      *cells += 128;
    }
  }
  picture = work->sheet + 0x1781;
  blits[0](canvas, picture, 24, 8, 80, 112);
  work->transferPending = 1;
  wait(1);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Resource_LoadAndDecompress((s32)&ResourceId_Data178, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B3, picture, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data137, sprites, 0, 0);
  loaded = 0;
  ship.x = 0xc80000;
  ship.z = 0x540000;
  transition[4] = 1;
  ClearWords_(canvas, 0x3c00);
  REG_DISPCNT = 0x7141;
  wait(1);
  for (frame = 0; frame != 326; frame++) {
    struct EffectStep *step;
    draw = 1;
    if (gInput.repeat & 3) {
      skip = 1;
    }
    // The listing masks the frame with draw (still 1) here, not a literal 1.
    if ((frame & draw) && skip == 1) {
      draw = 0;
    }
    if (frame == 0) {
      struct EffectStep *p;

      if (!loaded) {
        Task_Add(BattleEffect_ProcessPendingBlock2Transfer, 0xc8f);
        Task_Add(Display_ApplyBg2Affine, 0xcff);
        Resource_LoadAndDecompress((s32)&ResourceId_Data0C7, work->sheet + 0x2781, 1, 0);
        loaded = 1;
      }
      gBgScroll[1].x = 0xff90;
      REG_BLDALPHA = 0x1010;
      REG_DISPCNT = 0x7741;
      REG_BG2CNT = 0x784;
      REG_BG1CNT = 0x1f80;
      work->transferMode = 2;
      work->transferValue = 75;
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_Data150), 128);
      for (i = 0; i != 9; i++) {
        p = &work->particles[i];
        p->variant = -2 * i;
        p->x = (Random16() & 15) + 60;
        p->y = (Random16() & 15) + 44;
      }
    }
    if (frame <= 75) {
      s32 k = (frame / 4) % 18;
      s32 x = 60 - ((Trig_Sin(frame * 860) << 3) >> 16);
      s32 y = frame - 8;
      u8 *source = work->sheet + 0x1781 + Data_08198686[k];
      u8 w = Data_08198662[k];
      s32 left = x - (w >> 1);
      u8 h = Data_08198674[k];

      blit[1](canvas, source, left, y - (h >> 1), w, h);
    }
    if ((u32)(frame - 76) <= 31) {
      struct EffectStep *p;

      for (i = 0, p = work->particles; i != 4; i++, p++) {
        if ((u32)p->variant <= 17) {
          s32 k = p->variant / 3;
          s32 side = i & 1;
          u8 *source = work->sheet + gData_0819747a[k];
          u8 w = gData_08197467[k];

          blit[side](canvas, source, p->x - (w >> 1),
                     p->y + gData_08197473[k] - 32, w, gData_0819746d[k]);
        }
        p->variant++;
      }
    }
    if (frame == 108) {
      REG_BLDALPHA = 16;
    }
    if ((u32)(frame - 108) <= 8) {
      REG_BLDALPHA = ((frame - 108) * 2) << 8 | 16;
    }
    if ((u32)(frame - 92) <= 39) {
      s32 t = frame - 92;
      s32 z = Trig_Cos(t * 800) / 3 + 0x10000;
      s32 q16;
      s32 level = -1;

      if (z <= 0x107ff) {
        z = 0x10800;
      }
      q16 = 0x1000000 / z;
*(volatile u16 *)&work->bg2pa; *(volatile u16 *)&work->bg2pa = q16;
*(volatile u16 *)&work->bg2pd; *(volatile u16 *)&work->bg2pd = q16;
      work->bg2x = (64 - 0xb80000 / z) << 8;
      work->bg2y = (64 - 0x4c0000 / z) << 8;
      if (t <= 15) {
        level = t;
      } else if ((u32)(frame - 116) <= 8) {
        level = 64 - t * 2;
      }
      if (level != -1) {
        volatile u16 *pal = (volatile u16 *)(BG_PLTT + 128);

        for (i = 0; i != 32; i++) {
          s32 g = (i * ((level * 9 * 4) / 16)) / 64;

          pal[i] = ((i * level * 3) / 64) << 10 | g;
        }
      }
      if (frame == 116) {
        REG_DISPCNT = 0x7341;
      }
    }
    if ((u32)(frame - 124) <= 7) {
      volatile u16 *c = (volatile u16 *)BG_PLTT;

      for (i = 0; i != 64; i++, c++) {
        s32 v = *c;
        s32 low = 31;
        s32 r;
        s32 g;
        s32 b;

        low &= v;
        r = low - 3;
        g = ((u16)(((u16)v >> 5) & 31)) - 3;
        b = ((u16)(((u16)v >> 10) & 31)) - 3;

        if (r < 0)
          r = 0;
        if (g < 0)
          g = 0;
        if (b < 0)
          b = 0;
        *c = b << 10 | g << 5 | r;
      }
    }
    if (frame == 132) {
      if (loaded == 1) {
        Task_Remove(BattleEffect_ProcessPendingBlock2Transfer);
        Task_Remove(Display_ApplyBg2Affine);
        Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc8f);
        Resource_LoadAndDecompress((s32)&ResourceId_Data15C, work->sheet + 0x1e97, 1, 0);
        loaded = 0;
      }
      REG_DISPCNT = 0x7141;
      REG_BG2X = -0x800;
      REG_BG2Y = -0x1000;
      REG_BG2PA = 0xcc;
      REG_BG2PD = 0x100;
      REG_BG2CNT = 0x784;
      REG_BLDALPHA = 0x1010;
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_Data13A), 128);
    }
    if (frame == 133) {
      const s8 *src;
      struct EffectStep *p;
      struct EffectStep *q;

      FillWords_(canvas, 0x3c00, 0);
      ClearWords_((void *)0x06004000, 0x3c00);
      REG_DISPCNT = 0x7541;
      src = Data_081986aa;
      q = (struct EffectStep *)(work->sheet + 0x4b98);
      for (i = 0; i != 6; i++, src += 5) {
        p = &work->particles[i];
        p->x = src[0] << 16;
        p->variant = -8 * i;
        /* FAKEMATCH(barrier): an empty asm parts the particle's first stores from
         * the rest; as no asm, 44 of 1932 instructions differ in tla-en. */
        __asm__("");
        p->y = src[1] << 16;
        p->z = src[2] << 6;
        p->velocityX = src[3] << 15;
        p->velocityY = src[4] << 15;
        p->velocityZ = 0;
      }
      for (i = 0; i != 96; i++) {
        q[i].variant = -1;
      }
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C8, gMapCellBuffer, 0, 0);
    }
    if (frame == 76) {
      Sound_PlayFar(138);
    }
    if (frame == 266) {
      Sound_PlayFar(138);
    }

    if ((u32)(frame - 133) <= 132) {
      void *verts;
      struct RenderModel *model;
      struct EffectStep *sparks;
      u8 *portraits = gMapCellBuffer;
      struct EffectStep *q;
      s32 shade;
      s32 j;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      shade = 0;
      sparks = (struct EffectStep *)(work->sheet + 0x4b98);
      if (frame > 196) {
        Func_0815f0a0((s32)&ResourceId_Data150);
      }
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->kind = 7;
      model->shade = shade;
      model->texture = &texture;
      model->mesh = gData_08199340;
      model->vertices = verts;
      for (i = 0, step = work->particles; i != 6; step++, i++) {
        s32 delay = i * 8;
        struct EffectStep *anchor = step + 8;
        s32 t;

        if (++step->variant == 0) {
          Sound_PlayFar(138);
        }
        t = step->variant;
        if (t == 60) {
          Sound_PlayFar(212);
          t = step->variant;
        }
        if (t == 64) {
          Sound_PlayFar(134);
          t = step->variant;
        }
        if ((u32)t <= 35) {
          s32 k = (t / 6) % 6;
          u8 *source = work->sheet + gData_0819747a[k];
          u8 w = gData_08197467[k];

          blits[0](canvas, source, HI(step->x) - (w >> 1),
                   HI(step->y) + gData_08197473[k] - 36, w, gData_0819746d[k]);
          t = step->variant;
        }
        if ((u32)(t - 12) <= 31) {
          s32 k = ((t - 12) / 4) % 18;
          u8 *source = work->sheet + 0x1781 + Data_08198686[k];
          s32 x = HI(step->x);
          u8 w = Data_08198662[k];
          s32 left = x - (w >> 1);
          s32 y = HI(step->y);
          u8 h = Data_08198674[k];

          blit[1](canvas, source, left, y - (h >> 1), w, h);
          t = step->variant;
        }
        if ((u32)(t - 44) <= 21) {
          s32 s = (Trig_Sin((t - 44) << 10) * 48) / 64;
          s32 vx;
          s32 x = step->x;
          s32 mx = x - 0x400000;
          s32 my = step->y - 0x400000;
          s32 u = t - 59;

          if (u < 0) {
            u = 0;
          }
          vx = step->velocityX;
          step->y += step->velocityY;
          x += vx;
          step->x = x;
          if (x <= 0x3fffff) {
            step->velocityX = vx + 0x8000;
          }
          if (step->y <= 0x3bffff) {
            step->velocityY += 0x8000;
          } else {
            step->velocityY -= 0x8000;
          }
          if (step->z >= 0) {
            step->z += (Trig_Sin(step->velocityZ) * 3 << 8) >> 16;
          } else {
            step->z -= (Trig_Sin(step->velocityZ) * 3 << 8) >> 16;
          }
          step->velocityZ += 1024;
          if (draw) {
            // Offset first, as the listing adds it.
            texture.pixels = (u8 *)((u << 12) + (s32)portraits);
            Render_ResetTransformState();
            Func_08015160(mx, my, 0);
            SceneTransform_ApplyPitch(0xc000);
            SceneTransform_ApplyYaw(step->z);
            Func_0801521c(s);
            Render_TransformVertices(gData_08199210, verts, 4);
            Render_DrawModelList(model);
          }
          t = step->variant;
        }
        if (t == 64) {
          anchor->x = HI(step->x);
          anchor->y = HI(step->y);
          j = 0;
          {
            s32 first = i * 16;

            // Offset first, as the listing adds it.
            q = (struct EffectStep *)(first * (s32)sizeof(*q) + (s32)sparks);
          }
          for (; j != 16; j++, q++) {
            s32 a = Random16() & 0xffff;
            s32 sp = (Random16() & 15) + 8;

            q->variant = (Random16() & 7) + 8;
            q->x = step->x;
            q->y = step->y;
            q->velocityX = (Trig_Sin(a) * sp) / 4;
            q->velocityY = (Trig_Cos(a) * sp) / 4;
          }
          for (j = 0; j != effect->count; j++) {
            s32 m = (Random16() << 2) + 0x10000;

            BattleEffect_StartActorMotion(effect->actors[j], 1, 0x20000, 0x80000, m,
                                      delay + (Random16() & 31) + 52);
            Func_0814cd48(effect->actors[j], 7, -1, j, 4);
          }
        }
        if (draw && (u32)(step->variant - 64) <= 11) {
          s32 k = ((step->variant - 64) / 2) % 6;

          blit[1](canvas, work->sheet + 0x1e97 + k * 0x780,
                  anchor->x - 20, anchor->y - 24, 40, 48);
        }
      }
      for (i = 0; i != 96; i++) {
        q = &sparks[i];
        if (q->variant >= 0) {
          s32 n = q->variant / 4 + 2;

          if (draw) {
            blit[1](canvas, sprites + Data_08197424[n - 1], HI(q->x) - n,
                     HI(q->y) - n, n * 2, n * 2);
          }
          BattleEffectKernels_IntegrateVector2(q, 60, 0x2000);
          q->variant--;
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    if (draw && frame <= 131) {
      void *verts;
      struct RenderModel *model;
      s32 j;

      verts = Heap_AllocEwram(32);
      /* FAKEMATCH(barrier): an empty asm reads frame into r7 here, where the
       * listing reloads it, and the loop's step inits take it from there; as
       * no asm, 317 of 1932 instructions differ in tla-en. */
      asm("" : : "r"(frame));
      model = Render_AllocateModelList(1);
      j = 0;
      work->transferMode = 2;
      work->transferValue = 50;
      texture2.logWidth = 7;
      texture2.logHeight = 7;
      model->texture = &texture2;
      texture2.pixels = work->sheet + 0x2781;
      model->mesh = gData_08199364;
      model->kind = 7;
      model->vertices = verts;
      model->shade = 0;
      for (; j != 2; j++) {
        s32 t = frame - 76 - j * 16;

        if (t >= 0) {
          Render_ResetTransformState();
          Func_08015160(0x80000, 0, 0);
          SceneTransform_ApplyPitch(0x4000);
          if (j == 0) {
            s32 sc = t * 3072 - 0x12000;

            if (t <= 39) {
              sc = t << 11;
              if (sc > 0xc000) {
                sc = 0xc000;
              }
            }
            Func_0801521c(sc);
            SceneTransform_ApplyYaw(-t * 512);
          } else {
            if (t > 23) {
              Func_0801521c(0x1c000 - (t << 11));
            } else {
              SceneTransform_ApplyPitch(0x3ff0 - t * 682);
              Func_080150e4(t * 169 + 0xf028);
            }
            SceneTransform_ApplyYaw(t << 9);
          }
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    if ((u32)(frame - 108) <= 179) {
      req = Data_08196e54;
      if (frame > 131) {
        s32 u = frame - 132;

        ship.x = (Trig_Sin(u << 11) << 1) + 0xc80000;
        ship.z = (Trig_Sin(u << 10) << 1) + 0x540000;
      }
      if (draw) {
        place[1] = place[3] = 0;
        for (i = 0; i != 7; i++) {
          place[0] = (Data_08198654[i] << 16) + ship.x - 0x200000;
          place[2] = (Data_0819865b[i] << 16) + ship.z - 0x200000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place, (s32 *)&req, 0);
        }
      }
    }
    if (frame > 265) {
      if (frame == 266) {
        struct EffectStep *p;

        REG_BG2X = -0x7000;
        REG_BG2PA = 0x100;
        REG_BG2PD = 0x100;
        for (i = 0, p = work->particles; i != 32; i++, p++) {
          p->x = Random16() % 48 + 40;
          p->y = Random16() % 96 + 16;
          p->variant = -i / 2;
        }
      }
      for (i = 0, step = work->particles; i != 32; i++, step++) {
        if ((u32)step->variant <= 23 && draw) {
          s32 k = (step->variant / 4) % 6;
          u8 *source = work->sheet + gData_0819747a[k];
          u8 w = gData_08197467[k];

          blits[0](canvas, source, step->x - (w >> 1),
                   step->y + gData_08197473[k] - 48, w, gData_0819746d[k]);
        }
        step->variant++;
      }
    }
    if (frame == 288) {
      Task_Add(Func_0815b68c, 1152);
      work->particles[63].x = 0xbc0000;
      work->particles[63].y = 0x600000;
      work->particles[63].velocityX = 0;
      work->particles[63].velocityY = 0;
      work->frame = 0;
      dmaArmed = 1;
    }
    if (draw) {
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  Func_081180e8(134);
  REG_BG1CNT = 0x1f83;
  wait(1);
  // Once-only blocks: the listing loads each fill's destination before its size;
  // the first destination is read before its block, whose loop note then keeps
  // the size and the call's address behind that load.
  {
    void *destination = Ram_HeapSlots->battleFxWork[1];

    do {
      FillWords_(destination, 0x4000, 0);
    } while (0);
  }
  do {
    FillWords_((void *)0x06004000, 0x3c00, 0);
  } while (0);
  FillWords_((void *)0x06008000, 0x7800, 0);
  wait(1);
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(SLOT_BLITTER);
  transition[4] = 0;
  battle = Ram_HeapSlots->battleWork;
  Sound_PlayFar(390);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  ClearWords_((void *)(BG_PLTT + 96), 256);
  wait(1);
  BattleBackground_LoadFar(2, *(u16 *)(battle + 0x670), -1);
  REG_BG2CNT = 0x787;
  ClearWords_((void *)0x06004000, 0x3c00);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  REG_BLDCNT = 0;
  wait(1);
  for (frame = 0; frame != 40; frame++) {
    u16 *src = (u16 *)(battle + 0x56c);
    volatile u16 *dst = (volatile u16 *)(BG_PLTT + 96);

    for (i = 0; i != 128; i++, src++, dst++) {
      s32 v = *src;
      s32 r = 31;
      s32 g;
      s32 b;
      s32 nr;
      s32 ng;
      s32 nb;

      r &= v;
      g = (u16)(((u16)v >> 5) & 31);
      b = (u16)(((u16)v >> 10) & 31);
      nr = r + frame - 30;
      ng = g + frame - 40;
      nb = b + frame - 30;
      if (nr < 0) {
        nr = 0;
      } else if (nr > r) {
        nr = r;
      }
      if (ng < 0) {
        ng = 0;
      } else if (ng > g) {
        ng = g;
      }
      if (nb < 0) {
        nb = 0;
      } else if (nb > b) {
        nb = b;
      }
      *dst = nb << 10 | ng << 5 | nr;
    }
    wait(1);
  }
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  if (dmaArmed == 1) {
    Task_Remove(Func_0815b68c);
  }
  for (i = 0; i != 8; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
}
