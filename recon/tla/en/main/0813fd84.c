// Draft, differ 12 of 2579 (tla-en, 2026-11-09); 12 in every edition (de, es
// Pools read 10 November: every literal pool word this draft compiles equals
// the ROM's (diff compares instructions only; Catastrophe hid a wrong one).
// and fr by --symbol BattleEffect_RunCruelRuin)
// 10 November, by hand, still 12 (27 attempts on the object loop, none on the
// blasts). From the loop dump: the givs are found row (mult 9, insn 4676)
// then last (mult 36 plus work, 4689), and reduced givs are set up and
// stepped latest found first, so last comes before row; the listing has row
// first, so there the mult 9 giv that survives is found after last.
// - last written before row, with j * 9 inline, as work->objects + j * 9 + 8,
//   as &work->objects[8] + j * 9, or as a byte offset of j * 36 in four
//   spellings: 12, the same rows. Row's own insn then does come after last
//   (4693), but with lifetime 0: the product by 36 computes j * 9 on the way
//   (4676, a temporary), cse makes that temporary row's holder, and it stays
//   the parent.
// - last's offset without a j * 9 inside it, j * 32 + j * 4 or j * 8 + j on
//   the pointer, in six spellings: 300 (two instructions longer and one
//   stack slot moved, so every sp offset after it differs).
// - row after last in a form cse folds back to j * 9, (j << 3) + j, j * 8 +
//   j, j + j * 8, j * 3 * 3, j * 5 + j * 4: 12. In a form it cannot fold, j *
//   10 - j or (j << 4) - j * 7: 652 and 659 (row is no longer a plain
//   reduced giv).
// 2026-11-09 (round 14), 16 to 12, no device: the mask's byte is read into an
// s32 local first, s32 b = ((u8 *)obj)[9]; u8 x = b & -13; the and's -13
// operand is then a QImode register and the byte a SUBREG of an SImode one,
// so combine's canonical swap (a SUBREG of a register goes after a plain
// register) leaves the byte as operand 1 and the output ties to it, as in the
// listing. With both operands QImode SUBREGs (the old form) combine swaps them
// on every simplification. Left: the blasts (8) and the object loop (4).
// Object loop: last as its own pointer, for (j = 0, last = &objects[8]; ...;
// j++, last += 9), steps row before last as the listing does, but sets last
// up before the movable's ldr r5, [sp, #40] where the listing sets it up
// after row's init (12, the same: 4 moved from the steps to the init); its
// init after the scroll update, or for (last = ..., j = 0) also 12; before
// req 210; row as its own induction variable 582; j * 9 in the else arm 195,
// in the particles read 609. The listing's inits (row, then last) say both
// are reduced givs with row's parent found after *last's address giv.
// Second blit: a local DrawRectangle f = ALT_BLITTER puts movs r1, #192 first
// but the stores fall behind the load through f (14); the barrier moved
// before the second BattleEffect_LoadBlitter call moves movs r0, #188 instead
// (12); the second LOAD_BLITTER without its barrier 444.
// 2026-11-09 (round 8), unchanged at 16, about 10 attempts. The object loop
// from the loop dump: row's giv (insn 4674) absorbs the Apply call's j * 9
// (4889), which is used once and so never a parent; reduced increments are
// emitted in giv-list order, latest found first, so *last's address giv (4744)
// steps before row. A second use of the Apply's j * 9 (a dead if (i == 200)
// u = j * 9; after the call, or through a local) makes 4889 the parent and row
// then steps first, but the i loop's Apply pointer ([sp, #20]) goes and every
// slot moves: 569. A dead read of last at the j or i level 16 to 206; last
// before row 16; Apply indexed by row 1066.
// 2026-11-09 (round 13), 32 to 16: a tagged dead test (if (i == 300) u = 0;)
// after the place stores makes the one extra reload pick, so the else arm's
// reloads take r1, r2, r0 as in the listing. Left: the blasts (8, sched2), the
// object loop's row and last steps (4), the mask copy tie (4: combine leaves
// the and as (and -13copy byte) and the output ties to the copy). Not better:
// the same test on tile, phase or scrollX (16), on phase, row or j in the
// else arm (16 to 54), before the place stores (32, 70); dead tests in the
// creation loop around the mask (16 to 643), on last or row in the object
// loop for the giv order (16 to 275), on r or y in the blasts (266 to 401).
// 2026-11-09 (round 12), unchanged at 32. A reload trace of the object loop:
// i's, scrollX's and the place pointer's reloads pick r0, r1, r2 at the
// place stores, the i == 8 arm reloads only into its outputs, so the else
// arm starts from last_spill_reg 2 and picks r0, r1, r2 (r3 to r6 busy). The
// listing's r1, r2, r0 need last_spill_reg 0 there: one more pick after the
// place stores, landing on r0 and leaving no code (a redundant reload of i
// that reload_cse deletes would do it). Not better: 8 == i (32), i >= 8 (36),
// place[2] before place[0] (48); single-move searches over the object loop's
// statements and the frame's statements before it (32); over the blasts'
// statements no valid order beats 32 (y -= r after the first blit reads 30
// but changes the program).
// 2026-11-09 (last stretch), 34 to 32, no device: the priority mask as one
// byte expression, u8 x = ((s8 *)obj)[9] & -13, read through the signed byte
// (the creation block's s8 mask is gone). The and is then QImode: its -13 is a
// QImode temporary that reload builds in SImode (movs r4, #13; negs) and moves
// (adds rN, r4, #0), the listing's three rows; an s32 x keeps a sign extension
// (42), an s8 x 64, the and written in place 65. Left at the mask (2): the and
// ties its output to the -13 (operand 1, as the Thumb andsi3 expander's bic
// path orders it), so the byte takes r2 and the copy r3, where the listing has
// them the other way round; -13 & byte, ~12, 0xf3, (u8) or (s8) casts and a
// local holding -13 (s32: 89, u8: 50) do not reorder it; a pinned r2 mask is
// worse (74 to 89). 12 minutes of permuter (seed 8484) on this form: nothing
// below 32.
// 2026-11-09 (devices pass), unchanged at 34. Mask: the -13 through a copy
// held by an empty asm (493 to 566: the copy and the mask merge and spill),
// the copy tied to the mask by an asm operand (533), a u8 copy through an asm
// (44), a pinned -13 in r4 with a copy (87: CSE folds the constant into the
// copy and the pin's set dies), a dead m = 0 before the creation loop to
// keep its copy from loop.c's single-use replacement (552 to 611), inline
// helpers for the and-or or the priority bit-field (65 to 87: -13 straight
// into r2, the copy gone; the bit-field one narrows the skeleton gap to 15),
// an identity inline on the mask (65, gap 13, the same). Blasts: the
// 0x10000, y, half, r, canvas or sheet through asms or identity inlines (34
// to 2111). 12 minutes of permuter (seed 5152): nothing below 34.
// The same 34 in ja, de, es, fr and it (-mtune=gba); every edition's bytes agree with en's here.
// 2026-11-08 (last), from 36: the closing once-only block now holds the backdrop call as well as
// gBgScroll[1].y = 32, so the store, the ldrh it may alias and the call share one schedule block and
// movs r0, #1 fills the store's delay before the ldrh, as in the listing (34). What is left, read from
// the --rtl dumps, so the next round need not rediscover it:
// - Mask (6): adds r2, r4, #0 is thumb_movqi's register move, so the listing keeps a QImode copy of a
//   -13 that reload loads in SImode. A signed-char byte expression, ((s8 *)obj)[9] & -13, does give a
//   QImode -13 that loop.c hoists, but its (subreg:SI) copy falls to the single-use replacement and
//   reload builds -13 straight into r2 (65). Char locals are promoted to SImode, zero-extended, so an
//   s8 local holds 243 (hence movs r4, #243 now); (u8) of an s32 mask is folded back by CSE (89);
//   u8 or s8 masks with byte expressions, at either block, 50 to 74.
// - Object loop order (4): loop.c emits reduced givs' starts and steps in the biv's list, latest found
//   first. The pointer that survives is the address giv of *last, found in the i loop after row, so its
//   step comes first. Row would step first if last's own giv absorbed that address giv (last must be
//   used twice: a register giv used once never combines) or if row's j * 9 survived as the Apply call's
//   (that pseudo used twice). Tried: last declared before row (34, the same), row inside the i loop
//   (622), particles[j * 9 + u] (622: the stepped objects pointer and a frame slot go).
// - Else arm (16): reload's round-robin (last_spill_reg). The listing's i, 0x77dc and fp reloads take
//   r1, r2, r0, so its last allocation before the arm landed on r0; here it is the place pointer's r2
//   in the same block, and the i == 8 arm reloads only into its outputs. Unmoved by five more spellings
//   of the arm's read, the place stores swapped (50), i >= 8 (38) or the i == 8 arm's modulo as plain %.
// - Blasts (8): at the shaded circle, lsls r2 and add r0, fp tie on priority, class and dependants, so
//   RTL order (r0 loaded first) decides; at the first blit, ldr r0 canvas is anti-dependent on ldr r4,
//   [r0], so it ranks below add r1 and adds r3; at the second, LOAD_BLITTER's loop notes make the first
//   insn after them (the store of half) a barrier, where the listing has movs r1, #192 first. Dropping
//   either barrier (466 and worse) or loading the blitter into a local first (554) is worse.
// 2026-11-08 (latest), none better: an s32 mask builds -13 with movs, negs but drops the copy (67, also with a copied local, x = mask & byte, or (x & mask) | 4); the and-or as one byte expression (76), u8 x with -13 (74), s8 x (71), an s8 byte read (61), a (u8) mask (36), the s8 mask at the outer block, set late or copied (36).
// 2026-11-08 (later), from 38: the closing shade ramp sets its column pointer in the j loop's
// initializer, so c is built after j = 0 as in the listing (36). Re-tried: the mask as an s32 or
// s8 inside the block, an s8 read of the byte, the and-or as one expression; the else arm's index
// through a local objects pointer or as *(objects + m); the object loop's row and last stepped
// in its for; the closing background id read into a local or a second once-only block: no better.
// 2026-11-08, from 44: a once-only block around the closing gBgScroll[1].y = 32 puts the
// store after the backdrop address (38); 0x670 is still built a step late. No other
// once-only block, at or before any statement, goes under 38. Re-tried for the mask
// without gain: an s32 copy from an outer s32 or s8 mask, a mask set late, (u8) or
// (s8) casts (cprop folds the constant to 243), the bit-field (69), goto for the
// creation loop (642 and worse); the counter decrement as its own statement, an
// offset or y local and a byte local at the end, the else arm's index spelled four
// ways, and 2 x 20 minutes of permuter: nothing better.
// Before, 44. Tried 2026-11-08: an s32 or s16
// mask, the priority bit-field (75 each), the byte through a pointer (76).
// Near miss: the priority mask's hoisted -13 and its copy, then scheduling and reload order only. The frame, every
// slot and every pool agree.
// BattleEffect_RunCruelRuin. What this round settled, each read from the listing and each a real form:
// - Loop givs: grey, blue and green are givs of frame (blue first, as the slots order them); grey reduces because
//   the palette test and its colour spell frame - 80 again, and that second giv combines with the first.
// - level is frame * fadeStep through a constant local: the mult giv reduces where a plain frame * 2 has benefit 0,
//   and only a pseudo the loop makes takes slot 24, below blue and green and above Apply.
// - The creation loop has its own counter. Sharing j put j's rank under row and Apply's.
// - The object loop: row = j * 9 and last in the j loop, the Apply read spelled objects[j * 9 + i] so it is a giv only
//   on loop's second run, the else arm's row + i in a local reused for m so it is no giv at all.
// - The mask is a local int: as a bare ~0x3ff it is an HImode literal, whose short range split the pool twice.
// - shade is one local of the frame loop, always in r5; i = shade in the first setup is the listing's mov sl, r5.
// - Reload picks its registers round-robin, so constants hoisted out of a loop and left without a hard register
//   (pixels, strip, six merged with the setup's 6) are what make the register names line up downstream.
// - i = 0 before the pointers it sits beside; particle pointers are body locals (givs), not stepped by hand.
// - The false device `__asm__("" : "=r"(x) : "0"(x))` is gone.
// Later forms, each read from the listing: the cell loop's height and half are set before their
// loop; the creation loops own their object local (it conflicted with the particle pointer); the tile
// store reads through int locals so its and ties to the halfword; tile is taken at the loop's top,
// where its giv is replaceable, and the modulo keeps a quotient local so the shift is in place; the
// else arm's index is a block local set twice, which loop.c leaves alone; logWidth's store through
// a second struct view does not alias logHeight's, so movs r5, #160 and add r5, sp stay adjacent and
// the peephole folds them to add r5, sp, #160.
// Left, in order of reach:
// - The priority mask: the listing has movs r4, #13; negs r4, r4; adds r2, r4, #0, a hoisted QImode -13 and
//   its copy. loop.c's single-use replacement in a loop with calls deletes that copy from the bit-field's
//   expansion (insn 3013), so reload rebuilds -13 in r2. An s8 mask of the creation block, as now, is hoisted
//   and reloaded in r4 as the listing's rotation needs, but as movs r4, #243; lsls; asrs: three instructions.
//   Measured no better (2026-11-06): an s32, u8 or s16 mask at the creation block or the loop; x as u8, s8 or u32;
//   the bit-field through a byte or a word container; one local for -13 and 4. Remeasured 2026-11-06:
//   obj->attr.bits.priority = 1 builds an SImode -13 straight into r2 without the copy (75); a u8 x (60);
//   the and and or as separate byte stores (84). In the bit-field form the copy
//   (687 = subreg 685) is replaced in loop.c's first pass, where -13 is then "life 1, not desirable".
// - The six blasts: lsls r2 before add r0 at the shaded circle, adds r3 before add r1 at the first blit, movs r1
//   before str r5 at the second. Order only; the listing's order needs 0x10000 set before the call's arguments.
//   Constant locals, routine locals, sheet locals and inline forwards do not move them.
// - The else arm's reload order, r0, r1, r2 here and r1, r2, r0 in the listing (one rotation); the object
//   loop's row before last step; add r6 after movs r4.
// Adoption: the resource directory has no names yet for the ids written here as ResourceId_Data0B4, Data0C2, Data0C9,
// Data0CA, Data104, CornerRuneASheet, ForestCanopyTiles and SunbeamCloudsBackdrop. Barrier-free so far; the
// do { } while (0) of LOAD_BLITTER and WAIT_TRANSFER's volatile reads need their S2 reading then.

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

struct ModelTextureB {
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
void Task_Add(void (*func)(void), s32 order);
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

#include "bg_scroll.h"
#include "dma.h"
#include "input.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct RuinObject {
  u8 unk0[8];
  union {
    struct {
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
    } bits;
    u16 all;
  } attr;
  u8 unk0a[4];
  u8 resource;
};

struct WordPair {
  s32 a;
  s32 b;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern char ResourceId_Data0B4, ResourceId_Data0C2, ResourceId_Data0C9;
extern char ResourceId_Data0CA, ResourceId_Data104, ResourceId_Data134;
extern char ResourceId_CornerRuneASheet, ResourceId_ForestCanopyTiles;
extern char ResourceId_SunbeamCloudsBackdrop, ResourceId_VioletSkyBackdrop;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02012000[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196dd8;
extern const u16 Data_08197438[];
extern const u8 Data_081976e0[];
extern const u16 Data_081976f0[];
extern const u16 Data_081976f8[];
extern const u8 Data_08197700[];
extern const u8 Data_08197702[];
extern const u8 Data_08197704[];
extern const u8 Data_0819770a[];
extern const u8 Data_08197710[];
extern const u8 Data_08197716[];
extern const u8 Data_0819771c[];
extern const u8 Data_08197720[];
extern const u8 Data_08197724[];
extern const u8 Data_08197728[];
extern const u8 Data_0819772c[];
extern const u8 Data_08197730[];
extern const s32 Data_08197734[];
extern const u8 Data_081991c0[];
extern const u8 Data_081991e0[];
extern const u8 Data_08199220[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

struct RuinObject *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
struct RuinObject *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
s32 Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815b510(void *cells, s32 size, s32 scale);
void Func_08164a4c(s32 r, s32 g, s32 b);
void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 r, s32 g, s32 b);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void Heap_Release(void *buffer);

#define SLOT_ALT 188 // unkb8[1]
// One draw through a blitter built for it alone.
#define LOAD_BLITTER(slot, flags)                                              \
  BattleEffect_LoadBlitter(slot, flags); do { } while (0)
#define ALT_BLITTER ((DrawRectangle)Ram_HeapSlots->unkb8[1])
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)
// The listing reads the transfer count twice per pass of its wait.
#define WAIT_TRANSFER(work)                                                    \
  while (*(volatile u32 *)&(work)->vblanksSinceTransfer,                        \
         *(volatile u32 *)&(work)->vblanksSinceTransfer <= 1)

// Forwarded so that a constant size reaches the call's register at each call.
static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  Iwram_FillWords(destination, size, value);
}
static __inline__ void CopyWords_(void *destination, const void *source, s32 size) {
  Iwram_CopyWords(destination, source, size);
}

// Cruel Ruin: a pillar of dark light swallows the caster, a shadowed sky
// rains ruin in six blasts while the ground splits, then a black sphere
// bursts over the foes.
void BattleEffect_RunCruelRuin(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  s32 place[4];
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct WordPair req;
  struct ModelTexture texture3;
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  s32 *transition;
  s32 savedScroll;
  s32 scrollX;
  s32 phase;
  s32 *actor;
  s32 i;
  s32 j;
  s32 k;
  s32 fr;
  s32 fg;
  s32 fb;
  s32 dr;
  s32 dg;
  s32 db;
  s32 steps;
  s32 u;
  struct RuinObject *obj;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  work->transferMode = 2;
  work->transferValue = 50;
  draw[0] = Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  savedScroll = gBgScroll[1].x;
  scrollX = 0;
  phase = 0;
  actor = *GetBattleObjectSlotFar(effect->actor);
  actor[18] = phase;
  Graphics_LoadGreyRamp();
  Sound_PlayFar(140);
  for (frame = 0; frame != 36; frame++) {
    s32 size;

    if (frame == 24) {
      work->transferMode = 4;
      work->transferValue = 0x06060606;
    }
    EffectPosition_ProjectBattler(effect->actor, &pos);
    size = frame * 6 + 2;
    if (size > 96) {
      size = 96;
    }
    Func_0815b434(work->sheet, size, 0x10000);
    Func_0818caa8(work->sheet, pos.x / 2, pos.y - 16, size);
    if (frame > 15) {
      actor[3] += 0x100000;
    }
    work->transferPending = 1;
    wait(1);
  }
  actor[18] = 0xab85;
  actor[3] = 0;
  FillWords_((void *)0x06004000, 0x4000, 0x3f3f3f3f);
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  BattleBackground_LoadFar(1, (s32)&ResourceId_SunbeamCloudsBackdrop, 0);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  draw[0] = Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gMapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneASheet, work->sheet, 1, 0);
  Func_0816ae40(work->sheet, gData_02014000, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet + 0x2648, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x3648, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3688, 0, 0);
  for (i = 0; i != 10; i++) {
    s32 size = i * 2 + 2;

    Func_0815b510(work->sheet + 0x3a48 + Data_08197438[i], size, 0x10000);
  }
  Graphics_LoadGreyRamp();
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BLDALPHA = 0x1010;
  fr = 0;
  fg = 0;
  fb = 0;
  dr = 0;
  dg = 0;
  db = 0;
  steps = 0;
  for (i = 0; i != 24; i++) {
    struct EffectStep *p = &work->particles[i];
    p->z = i * 0xaaa + (Random16() & 0xfff);
    p->variant = -(Random16() & 15);
  }
  for (i = 0; i != 31; i++) {
    struct EffectStep *p = &work->particles[i + 32];
    p->x = Random16() & 127;
    p->y = (Random16() & 31) + 88;
    p->z = (Random16() & 3) + 5;
    p->variant = i * -2;
  }
  {
  {u8 *pixels = gMapCellBuffer;
  for (frame = 0; frame != 180; frame++) {
    struct RenderModel *model;
    void *verts;
    s32 shade;

    if ((gInput.repeat & 3) && frame <= 168) {
      frame = 169;
    }
    if (frame == 4) {
      Sound_PlayFar(220);
    }
    if (frame == 40) {
      Sound_PlayFar(154);
    }
    if (frame == 64) {
      Sound_PlayFar(139);
    }
    if (frame == 74) {
      Sound_PlayFar(139);
    }
    if (frame == 84) {
      Sound_PlayFar(139);
    }
    if (frame == 94) {
      Sound_PlayFar(139);
    }
    if (frame == 124) {
      Sound_PlayFar(144);
    }
    for (i = 0; i != 6; i++) {
      if (frame == 140 + i * 4) {
        Sound_PlayFar(104);
      }
    }
    if (frame == 169) {
      Sound_PlayFar(145);
      work->shakeFrames = 16;
    }
    if (frame == 0) {
      Func_08164b2c(0, 0, 0);
    }
    if (frame == 32) {
      fr = 0;
      fg = 0;
      fb = 0;
      dr = -0x2492;
      dg = -0x6db6;
      db = -0x2492;
      steps = 28;
    }
    if (frame == 124) {
      fr = 0x1f0000;
      fg = 0x80000;
      fb = -0x40000;
      dr = 0;
      dg = 0;
      db = 0;
      steps = 1;
    }
    if (frame == 128) {
      dr = (-0x10000 - fr) / 16;
      dg = (0x60000 - fg) / 16;
      db = -fb / 16;
      steps = 16;
    }
    if (frame == 144) {
      dr = (-0x90000 - fr) / 8;
      dg = (0xd0000 - fg) / 8;
      db = (-0x50000 - fb) / 8;
      steps = 8;
    }
    if (frame == 156) {
      dr = (0x1f0000 - fr) / 16;
      dg = (0x80000 - fg) / 16;
      db = (-0x40000 - fb) / 16;
      steps = 16;
    }
    if (steps > 0) {
      steps--;
      fr += dr;
      fg += dg;
      fb += db;
      Func_08164b2c(fr >> 16, fg >> 16, fb >> 16);
    }
    if (frame == 124) {
      FillWords_(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame <= 123) {
      s32 size = 140;
      s32 y = 58;
      s32 scale = 0x20000;

      if (frame <= 29) {
        size -= frame * 4;
      } else {
        size = 114 - frame * 3;
      }
      if (frame > 7) {
        scale = 0x24000 - (frame << 11);
      }
      if (scale <= 0x7fff) {
        scale = 0x8000;
      }
      if (frame > 35) {
        y = 20;
      } else if (frame > 19) {
        y = (Trig_Cos((frame << 10) + -0x5000) * 44 >> 16) + 14;
      }
      if (size <= 7) {
        size = 8;
      }
      if (frame > 67) {
        size += (frame - 68) / 6;
      }
      Func_0815b434(work->sheet, size, scale);
      Func_0818caa8(work->sheet, 60, y, size);
    }
    if (frame > 127) {
      for (i = 0; i != 31; i++) {
        struct EffectStep *p = &work->particles[i + 32];

        if ((u32)p->variant <= 15) {
          s32 s = Trig_Sin(p->variant << 10) * p->z >> 16;

          draw[0](canvas, work->sheet + 0x3a48 + Data_08197438[s - 1], p->x - s,
               p->y - s * 2, s * 2, s * 4);
        }
        p->variant++;
      }
    }
    verts = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    {
      s32 s;
      s32 t;
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = gData_08199364;
      model->vertices = verts;
      texture.pixels = pixels;
      shade = 0;
      model->shade = shade;
      i = shade;

      for (s = (frame << 15) + -0x200000, t = frame - 64; i != 4;
           s += -0x50000, i++, t -= 10) {
        if ((u32)t <= 15) {
          s32 sc = 0x80000 - s;

          Render_ResetTransformState();
          Func_08015160(-0x40000, -0x2c0000, 0);
          Func_080151e4(sc / 2, sc, sc);
          SceneTransform_ApplyYaw(Data_081976f8[i]);
          SceneTransform_ApplyPitch(Data_081976f0[i]);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    u = frame - 124;
    if ((u32)u <= 15) {
      s32 sc = u << 15;

      shade = 64 - u * 8;
      if (shade > 0) {
        shade = 0;
      }
      model->shade = shade;
      Render_ResetTransformState();
      Func_08015160(-0x40000, -0x2c0000, 0);
      Func_080151e4(sc >> 1, sc, sc);
      Render_TransformVertices(Data_081991e0, verts, 4);
      Render_DrawModelList(model);
    }
    {
      s32 t = frame - 24;

      if ((u32)t <= 99) {
        s32 sc = 0x30000 - (t << 11);

        if (sc <= 0xffff) {
          sc = 0x10000;
        }
        shade = -32;
        if (t <= 7) {
          shade = t * 4 - 64;
        }
        if (frame > 63) {
          shade = 224 - frame * 4;
        }
        model->shade = shade;
        texture.pixels = gData_02014000;
        t <<= 9;
        Render_ResetTransformState();
        Func_08015160(-0x40000, -0x2c0000, 0);
        Func_080151e4(sc, sc / 3, sc >> 1);
        Func_080150e4(t);
        if (shade > -64) {
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
        texture.pixels = work->sheet + 0x2648;
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gData_08199340;
        if (frame > 63) {
          shade = frame * 4 + -320;
          if (shade > 0) {
            shade = 0;
          }
          model->shade = shade;
          Render_ResetTransformState();
          Func_08015160(-0x40000, -0x2c0000, 0);
          Func_080151e4(sc, sc >> 1, sc >> 1);
          Func_080150e4(t);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    if (frame > 123) {
      ((struct ModelTextureB *)&texture)->logWidth = 5;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x3648;
      model->mesh = Data_08199220;
      model->kind = 6;
      shade = -16;
      model->shade = shade;
      for (i = 0; i != 24; i++) {
        struct EffectStep *p = &work->particles[i];

        if (p->variant >= 0) {
          s32 sy = p->variant << 13;

          if (sy > 80000) {
            sy = 80000;
          }
          Render_ResetTransformState();
          Func_08015160(-0x40000, -0x2c0000, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_080150e4(p->z);
          Func_080151e4(30384, sy, 0x8000);
          Render_TransformVertices(Data_081991c0, verts, 4);
          Render_DrawModelList(model);
        }
        p->variant++;
      }
      if (frame > 163) {
        work->transferMode = 1;
        work->transferValue = 0x01010101;
        Func_08164a4c(2, 2, 2);
      }
      if (frame > 159) {
        struct EffectStep *q = &work->particles[63];
        s32 r = frame * 2 + -288;

        if (r > 64) {
          r = 64;
        }
        if (frame == 160) {
          q->x = 0x800000;
          q->y = -0x200000;
        }
        if (frame <= 171) {
          q->x += -0x80000;
          q->y += 0x100000;
          Func_0815b434(work->sheet, r, 0x10000);
          Func_0818caa8(work->sheet, HI(q->x), HI(q->y), r);
        }
        model->kind = 6;
        shade = 0;
        model->shade = shade;
        texture.pixels = work->sheet + 0x3688;
        Render_ResetTransformState();
        Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_080150e4(0xa000);
        Func_080151e4(r * 9400, 0x20000, 0x8000);
        Render_TransformVertices(Data_081976e0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    if (work->shakeFrames > 0) {
      {s32 y_ = Random16() % 8 + 28; gBgScroll[1].y = y_;}
      work->shakeFrames--;
    } else {
      gBgScroll[1].y = work->savedBg1Y;
    }
    BattleEffect_CountDownBattlerTints();
    WAIT_TRANSFER(work);
    work->transferPending = 1;
    wait(1);
  }
  }}
  {
  s32 count = -1;
  s32 fadeStep = 2;
  u8 *strip = Data_02012000;
  void *verts;
  Render_BuildQuadStrip(strip, 8, 16, 32);
  for (k = 0; k != 2; k++) {
    s32 c;

    for (c = 0; c != 33; c++) {
      s32 w = Data_08197702[k];
      s8 *v;
      s32 acc;
      s32 height;
      s32 half;

      if (c > 15) {
        w = w + c * 2 - 32;
      }
      acc = 0;
      {
        s8 *cells = (s8 *)gMapCellBuffer;
        s32 cell = k * 32 + k + c;

        v = cells + cell * 8;
      }
      height = 126 - c * 7;
      half = w / 2;
      for (i = 0; i != 2; i++, acc += w, v += 4) {
        v[0] = height;
        v[1] = (Trig_Sin(c * 0x92c) * Data_08197700[k] >> 16) + acc - half;
        v[2] = 0;
      }
    }
  }
  work->transferMode = 2;
  work->transferValue = 50;
  FillWords_(canvas, 0x4000, 0x3f3f3f3f);
  work->transferPending = 1;
  wait(1);
  FillWords_(canvas, 0x4000, 0);
  REG_BG2CNT = 0x786;
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  Func_08164abc(31, 31, 31);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3648, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data104, work->sheet + 0x36c8, 0, 0);
  Graphics_LoadGreyRamp();
  {
    u8 *e = Resource_GetTableEntry((s32)&ResourceId_ForestCanopyTiles);

    FillWords_((void *)&OBJ_PLTT_COLOR(15, 0), 32, 0x7fff7fff);
    Resource_DecodeType01(e + 32, work->sheet);
  }
  {s32 q;
  for (q = 0; q != 2; q++) {
    u8 *src;
    struct RuinObject *obj;

    i = 0;
    src = work->sheet + q * 0x1000;
    for (; i != 8; i++) {
      obj = Func_0815b290(32, 32, 0x80000000, 0xf000);
      work->objects[q * 9 + i] = obj;
      {s32 b = ((u8 *)obj)[9]; u8 x = b & -13; x |= 4; ((u8 *)obj)[9] = x;}
      CopyWords_((void *)OBJ_TILES(obj), src, 512);
      src += 512;
    }
    obj = Func_0815b3b0(32, 32, 0x80000000, 0xf000);
    work->objects[q * 9 + 8] = obj;
    CopyWords_(obj, work->objects[q * 9], 24);
    for (i = 0; i != 8; i++) {
      work->particles[q * 9 + i].variant =
          ((struct RuinObject *)work->objects[q * 9 + i])->attr.bits.tile;
    }
  }
  }
  wait(1);
  frame = 0;
  while (1) {
    struct RenderModel *model;

    if ((gInput.repeat & 3) && frame <= 84 && count == -1) {
      work->transferMode = 4;
      work->transferValue = 0x04040404;
      count = 17;
      REG_BG2CNT = 0x784;
    }
    if (frame <= 17) {
      s32 g = 31 - frame * fadeStep;
      s32 r = g;
      s32 b = 31 - (frame * fadeStep + frame);

      if (g < 0) {
        r = 0;
      }
      if (g <= 5) {
        g = 6;
      }
      if (b < -18) {
        b = -18;
      }
      Func_08164b2c(r, g, b);
      if (frame <= 16) {
        u16 *e = Resource_GetTableEntry((s32)&ResourceId_ForestCanopyTiles);
        u16 *dst = (u16 *)&OBJ_PLTT_COLOR(15, 0);
        s32 add;

        i = 0;
        add = 32 - frame * fadeStep;
        for (; i != 16; i++, e++, dst++) {
          s32 cr = (*e & 31) + add;
          s32 cg = (u16)((*e >> 5) & 31) + add;
          s32 cb = (u16)((*e >> 10) & 31) + add;

          if (cr > 31) {
            cr = 31;
          }
          if (cg > 31) {
            cg = 31;
          }
          if (cb > 31) {
            cb = 31;
          }
          *dst = cb << 10 | cg << 5 | cr;
        }
      }
    }
    if (frame == 0) {
      REG_BG2CNT = 0x786;
    }
    if (frame == 75) {
      REG_BG2CNT = 0x784;
    }
    if (frame == 85) {
      work->transferMode = 4;
      work->transferValue = 0x04040404;
    }
    verts = Heap_AllocEwram(528);
    model = Render_AllocateModelList(1);
    {s32 zero = 0;
    texture2.logWidth = 5;
    texture2.logHeight = 2;
    texture2.pixels = work->sheet + 0x3648;
    model->shade = zero;
    model->mesh = Data_08199220;
    model->texture = &texture2;
    model->vertices = verts;
    model->kind = 6;
    for (i = zero; i != 6; i++) {
      s32 d = frame - Data_08197704[i];

      if (d == 0) {
        if (i == 5) {
          Sound_PlayFar(145);
        } else {
          Sound_PlayFar(144);
        }
        work->shakeFrames = 8;
        FillWords_(canvas, 0x4000, 0x3f3f3f3f);
      }
      if ((u32)d <= 15) {
        s32 r = (Trig_Sin(d << 11) * Data_0819770a[i] >> 16) + 2;

        if (r > 1) {
          s32 x = Data_08197710[i] + d * 8;
          s32 y = Data_08197716[i];
          s32 half;

          Func_0815b434(work->sheet, r, 0x10000);
          LOAD_BLITTER(SLOT_ALT, 19);
          half = r / 2;
          y -= r;
          y += 8;
          ALT_BLITTER(canvas, work->sheet, x - half, y, half, r);
          HeapSlot_Release(SLOT_ALT);
          LOAD_BLITTER(SLOT_ALT, 23);
          ALT_BLITTER(canvas, work->sheet, x, y, half, r);
          HeapSlot_Release(SLOT_ALT);
        }
      }
    }}
    for (i = 0; i != 4; i++) {
      struct EffectStep *p = &work->particles[i + 18];
      s32 start = Data_0819771c[i] >> 1;

      if (frame == 0) {
        p->y = frame;
        p->variant = frame;
        p->x = Data_08197724[i] << 16;
      }
      if (frame >= start && frame < start + 32) {
        s32 size = 0;
        s32 px = 0;
        s32 py = 0;
        s32 angle = 0;
        s32 scale = 0;
        size = Data_08197720[i];

        if (p->y < Data_08197730[i] << 16) {
          p->x -= Data_08197728[i] << 16;
          p->y += Data_0819772c[i] << 16;
        } else {
          p->variant++;
        }
        p->x += 0x80000;
        Render_ResetTransformState();
        px = p->x + -0x400000;
        py = p->y + -0x400000;
        Func_08015160(px, py, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        angle = Data_08197734[i];
        Func_080150e4(angle);
        scale = size * 9400;
        Func_080151e4(scale, 0x20000, 0x8000);
        Render_TransformVertices(Data_081976e0, verts, 4);
        Func_0815b434(work->sheet, size, 0x10000);
        Func_0818caa8(work->sheet, HI(p->x) - 1, HI(p->y), size);
        Render_DrawModelList(model);
      }
    }
    {s32 six = 6;
    texture2.logWidth = six;
    texture2.logHeight = 5;
    texture2.pixels = work->sheet + 0x36c8;
    Render_BuildQuadStrip(strip, 2, 32, 32);
    model->mesh = strip;
    model->kind = six;
    model->scrollU = 64;}
    {
      u8 *cells = gMapCellBuffer;
      s32 mask;

      for (i = 0, mask = 127; i != 2; cells += 264, i++) {
        s32 t = frame - i * 46 - 12;

        if ((u32)t <= 45) {
          s32 x;
          u8 *m;
          s32 z;
          s32 v;

          if (i == 0) {
            v = t * 2 + 63;
            v = mask & ~v;
            model->scrollU = v;
            if (t <= 32) {
              u8 *m = (u8 *)(t * 24 + (s32)strip);
              m[0] = i;
              m[1] = i;
              m[2] = i;
            }
          } else {
            v = t * 4 + 63;
            v = mask & ~v;
            model->scrollU = v;
            {
            s32 z = 0;
            if (t <= 15) {
              u8 *m = (u8 *)(t * 48 + (s32)strip);
              m[0] = z;
              m[1] = z;
              m[2] = z;
            }}
          }
          Render_ResetTransformState();
          x = -0x180000;
          if (t >= 0) {
            x = (t << 16) + -0x180000;
          }
          if (i == 0) {
            if (t > 31) {
              x += (t * 7 - 224) << 16;
            }
          } else if (t > 15) {
            x += (t * 7 - 112) << 16;
          }
          Func_08015160(x, -0x100000, 0);
          Func_080151e4(45536, 0x10000, 0x8000);
          if (i == 0) {
            Func_080150e4(-0xbe8);
          } else {
            Func_080150e4(-0x400);
          }
          Render_TransformVertices(cells, verts, 66);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    req = Data_08196dd8;
    place[3] = 0;
    place[1] = 0xff0000;
    scrollX += 0x180000;
    if (scrollX > 0x1fffff) {
      phase = (phase + 7) % 8;
      scrollX += -0x200000;
    }
    gBgScroll[1].x += 0xfff8;
    for (j = 0; j != 2; j++) {
      s32 row = j * 9;
      void **last = &work->objects[row + 8];

      for (i = 0; i != 9; i++) {
        s32 mask = ~0x3ff;
        s32 k_ = 64;
        s32 tile = i + k_ + phase;
        place[0] = (i << 21) + scrollX;
        place[2] = 0x790000 + (j << 21);
        /* FAKEMATCH(dead): a store both arms below overwrite; its test
         * builds 300 in a reload register, the one pick after the place
         * stores that leaves reload's round-robin on r0, so the else arm's
         * reloads take r1, r2 and r0; without it, 32 of 2579 instructions
         * differ in tla-en. */
        if (i == 300)
          u = 0;
        if (i == 8) {
          obj = *last;
          {s32 t_ = phase + 71; s32 q_ = t_; if (t_ < 0) q_ = phase + 78; q_ >>= 3; q_ <<= 3; u = t_ - q_;}
        } else {
          {s32 m = row; m += i; obj = work->objects[m];}
          {s32 x_ = phase + i; s32 q_ = tile; if (tile < 0) q_ = x_ + 71; q_ >>= 3; q_ <<= 3; u = tile - q_;}
        }
{s32 v = work->particles[row + u].variant & 0x3ff; s32 x = obj->attr.all & mask; obj->attr.all = x | v;}
        if (place[0] > 0xffffff) {
          place[0] += -0x1200000;
        }
        Render_ApplyProjectedPlacementFar(work->objects[j * 9 + i], place,
                                          (s32 *)&req, 0);
      }
    }
    BattleEffect_CountDownBattlerTints();
    WAIT_TRANSFER(work);
    if (work->shakeFrames > 0) {
      {s32 y_ = Random16() % 8 + 28; gBgScroll[1].y = y_;}
      work->shakeFrames--;
    } else {
      gBgScroll[1].y = work->savedBg1Y;
    }
    work->transferPending = 1;
    wait(1);
    frame++;
    if (frame == 102) {
      break;
    }
    if (count > 0 && --count == 0) {
      break;
    }
  }
  }
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  {
    u16 *background = (u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670);

    // A once-only block: the listing stores 32 after the backdrop address and
    // sets up the call while the store settles.
    do {
      gBgScroll[1].y = 32;
      BattleBackground_LoadFar(1, *background, 24);
    } while (0);
  }
  wait(1);
  gCameraSceneParameters[4] = 120;
  Sound_PlayFar(390);
  for (i = 0; i != 18; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  FillWords_(canvas, 0x4000, 0x3f3f3f3f);
  REG_BG2CNT = 0x784;
  effect->actors[effect->count] = 0xff;
  BattleActor_SpawnObjectsForListFar(effect->actors, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0, 0);
  REG_BLDALPHA = 0x1010;
  for (i = 0; i != 120; i++) {
    s32 v = i / 2;
    u8 *c;

    if (v > 63) {
      v = 63;
    }
    for (j = 0, c = (u8 *)(i + (s32)work + 1792); j != 120; j++) {
      *c = v;
      c += 120;
    }
  }
  {
    for (frame = 0; frame != 140; frame++) {
      s32 grey = frame - 80;
      s32 blue = grey * 1024;
      s32 green = grey * 32;

      if (frame == 0) {
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 0, -1, i, 16);
        }
      }
      if (frame == 127) {
        Func_081180e8(134);
      }
      if (frame == 107) {
        FillWords_(canvas, 0x4000, 0x3f3f3f3f);
      }
      if (frame > 107) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Func_08164b2c(0, 0, 0);
      }
      if (frame == 32) {
        Sound_PlayFar(140);
      }
      if (frame == 48) {
        work->shakeFrames = 90;
        Sound_PlayFar(145);
      }
      if (frame == 72) {
        Sound_PlayFar(208);
      }
      if (frame <= 109) {
        struct RenderModel *model;
        void *verts;

        if (frame > 31) {
          s32 a = (frame - 32) / 4;
          s32 b = 0;

          if (frame > 47) {
            b = (frame - 48) / 2;
          }
          Func_08164abc(a, a / 2, b);
        }
        if ((u32)(frame - 80) <= 31) {
          u32 c;
          u16 *dst = (u16 *)&OBJ_PLTT[1];

          i = 0;
          c = blue | green | (frame - 80);
          for (; i != 223; i++) {
            *dst++ = c;
          }
        } else if (frame > 47) {
          ColorBuffer_ScaleFar((void *)OBJ_PLTT, (void *)OBJ_PLTT, 0xf000, 224);
        }
        if (frame > 47) {
          draw[0](canvas, work->sheet, 216 - frame * 2, 0, 120, 120);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        {s32 zero = 0;
        texture3.logWidth = 5;
        texture3.logHeight = 2;
        texture3.pixels = work->sheet + 0x3840;
        model->shade = zero;
        model->mesh = Data_08199220;
        model->texture = &texture3;
        model->vertices = verts;
        model->kind = 6;}
        if (frame > 31) {
          struct EffectStep *q = &work->particles[63];
          s32 r = frame * 2 - 32;

          if (r > 64) {
            r = 64;
          }
          if (frame == 32) {
            q->x = 0x800000;
            q->y = -0x200000;
          }
          if (frame <= 43) {
            q->x += -0x80000;
            q->y += 0x100000;
            Func_0815b434(work->sheet + 0x38c0, r, 0x10000);
            Func_0818caa8(work->sheet + 0x38c0, HI(q->x), HI(q->y), r);
          }
          Render_ResetTransformState();
          Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_080150e4(0xa000);
          Func_080151e4(r * 9400, 0x20000, 0x8000);
          Render_TransformVertices(Data_081976e0, verts, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      {
        s32 r = 8;

        if (frame > 130) {
          r = 2;
        } else if (frame > 120) {
          r = 4;
        } else if (frame > 110) {
          r = 6;
        }
        Func_08158ce0(r, r);
      }
      work->transferPending = 1;
      wait(1);
    }
  }
  Palette_LoadBasicObjColors();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
