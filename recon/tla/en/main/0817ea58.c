// Draft, differ 8 of 3921 (tla-en, 2026-11-09); 8 in every edition (tla-ja
// at 0817ea5c).
// Thirteenth round, 9 November, still 8, about 35 attempts.
// - The map loop's k as j * step, step a local holding 16 (plain C): loop.c
//   reduces k and its zero then follows the hoisted 6 and 32, as listed, but
//   k * 2 inherits k's benefit (simplify_giv_expr adds a source giv's benefit
//   unconditionally, 18 + 2) and is reduced in k's place (16; step at function
//   level 1770; k given a second use by a dead test 83 to 2168). Dead tests
//   before and after k's set stop k deriving k * 2 (computed from k, and with
//   step at function level the increment is the listing's adds #16), but their
//   labels set maybe_never, so the 6 and the 32 stay in the outer loop (116 to
//   141). map[k * 2 + i] in place of mp 22 (57 on the biv form); w = 32 before
//   the loops 16 (the 6 stays a movable after k).
// - z as i * step (step = -0x100), with and without the two dead tests: 48 to
//   89 (z is reduced, but step takes r9 and the zero sl).
// - The copy's empty asm before the 42 + i loop: 12 without it (the loop's
//   hoisted constants rise above the call; the tag's 28 was stale), a memory
//   clobber in its place 8, at the priority store 22, after the sprite store
//   16, an empty once-only loop after or around the copy 12 and 14; that
//   loop's copy alone linked 14 (a second pool word), the 0x100 copy alone 38,
//   both 117 (the pool's order moves).
// Twelfth round, 9 November, 16 to 8. Read from a scratch cc1 that prints each
// sched2 ready list with priority, class and dependants (analysis only):
// - The spawn call (6) and the orange palette copy: both reach Iwram_CopyWords
//   through IwramIrqMain (IWRAM_COPY_WORDS_LINKED, the trade's form). Its address
//   is then a pool MEM, which depends on the call before it (the call flushes
//   memory), so the spawn call's priority becomes 3 and it leads, as listed; the
//   palette copy loads it into a local after its source, so it is set after the
//   arguments and wins the tie with lsls r0. Both in that pool, one word.
// - pos.x's read after the tile store (2): with -fstrict-aliasing a bitfield
//   store carries Sprite's alias set, which holds int through the 32-bit scale,
//   so the s32 read waits for it. Through SpriteAttributes (no int) it does not
//   and is scheduled first, as listed. Sprite without scale makes every such
//   pair independent (22), a nested attribute struct for every access 68.
// Left (8): the map loop's k = 0 (2) and the third phase's z = 0 and mov sl
// (4): natively both come after the loop's hoisted constants in RTL order
// (ties broken by LUID), as a strength-reduced giv's init would; and mov r1, fp
// one row early (2): the empty asm before the 42 + i loop is an ASM_INPUT, so
// every earlier read of fp gains it as a dependant and the reload of fp outranks
// adds r3, #120 on dependants (3 to 2). That loop's copy through
// IWRAM_COPY_WORDS_LINKED does the asm's work and settles mov r1, fp, but its
// pool word is then a second word where the listing has one (14); linking any
// one of the eight copies before it, 24 to 120. Not better: the plain IwramCopyWords
// symbol for every copy (415), every copy linked (36), the map loop's k as j * 16
// in three spellings (1696: not reduced, "not worth while"), k's init first (57).
// Eleventh round, 9 November, 24 to 16: the third phase's registers are the
// listing's. Two tagged dead branches stand in for the five: the first puts a
// label between i = 0 and z = 0, so find_equiv_reg stops there and z's zero
// is its own reload (r0); the second's test reloads work (fp) once after it,
// the one pick before z's read; ay * 40 is then plain C and shifts as the
// listing does. Left: z's movs r0, #0 and mov sl, r0 are scheduled two rows
// early (4: in the listing's RTL they follow the loop's hoisted zero and the
// sprites pointer, as a giv's init would; z as a giv of i, 75 to 85), the
// spawn call (6) and the three pairs before it (6). Learned on the way:
// asm("") and asm volatile("") are ASM_INPUT, which find_equiv_reg's
// volatile_insn_p passes (28 between the inits); a volatile asm with operand
// syntax or an r4 clobber stops it, but z's zero then copies the hoisted zero's
// r7 (30) or CSE forgets i == 0 and the loop gains an entry test (71). The
// inits and the two branches in other orders: 47 to 69.
// Tenth round, 9 November, unchanged at 24. Traced with a scratch cc1 that
// prints each reload pick (analysis only): the spill registers are r0 to r4
// and r7, picked round robin. Third phase, listing: i's zero r4, z's zero r0,
// then one pick of r1 before z's read r2 and the -0x800 r3; the -0x100 r4, 1
// r0, compare r1 and next i r2 after it already match. Here z's zero takes
// i's r4 through find_equiv_reg (i's reload movs r4, #0 sits just before it,
// no label between), and the five's movs r1, #5 is the one r1 pick. A zero
// local set just before the frame loop gives z the listing's fresh r0, but
// CSE then stores the timer from z and the ay store reloads its own 0 (79,
// 85); ay or timer from that local (79 to 445), z from it (58), i and z
// initialised in either order or apart (24). 14 minutes of permuter on the
// frame == 0 block (seed 9917, PERM_RANDOMIZE): nothing below 24.
// Ninth round, 9 November (devices pass), 58 to 24, about 35 attempts: the third
// phase computes ay * 40 as (ay * five) << 3, five a function-level local
// holding 5 (plain C; the permuter, seed 7172, found the first step, a local
// holding 40, which gave 47 as a muls by 40). Reload then spends r1 on the 5,
// which is the one reload register the listing spends before z's read: z's
// read, the -0x800, the loop's tail and the next loop's i and -1 now take
// the listing's registers, and the instruction count is the listing's. Left
// (24): ay * 5 is movs r1, #5; adds; muls where the listing shifts and adds
// (6 rows), and z's zero inherits i's r4 where the listing has its own movs
// r0, #0 (3): together they are the listing's fresh zero in r0 and some use
// of r1 not yet found; the spawn call (6), and three single pairs before it
// (6), and a branch distance (2).
// Also measured: z's 0 from a pin in r0 set before i = 0 gives that fresh
// zero and the listing's tail without forty (54), but z then takes r6 and the
// hoisted zero sl (any of r0 to r3 the same); with forty, 60. Not better: the
// pin after i = 0 (68 to 85), pins on i's zero (71) or both (153), z through
// an asm (85), z = (s16)0 (47), the shifts kept with forty folded in (58),
// forty set just before the spawn call (47, the same); at the spawn call,
// i = 0 through an asm (104 to 121), the copy routine through an inline
// returning it, a direct call or an inline taking it, identity inlines on 663
// or sprites[0] (58 each). On the five form: the pin (93), the shifts spelled
// out (73), five in a block (75), an eight local (64), (ay * five) * 8 (24).
// The permuter's other finds on the 58 form: a work copy for the third phase
// (46 there, a temporary; not kept). All six editions 24 (each at its own
// address).
// Before this round all six editions measured 58 (tla-ja at its own address,
// 0817ea5c). Both empty asms tagged.
// Eighth round, 8 November, unchanged at 58, about 27 attempts. Read from the greg and
// sched2 dumps (--rtl), so the next round need not repeat them:
// - Third phase, first loop. Listing's reload registers: i's zero r4, z's zero r0 (a
//   fresh reload), z's read r2, -0x800 r3, -0x100 r4, 1 r0, the compare r1, the next
//   loop's i r2 (so its -1 stays movs/negs in r1). Here z's zero inherits r4 through
//   find_equiv_reg, z's read is r1 and everything after runs one behind. Two things
//   are missing, not one: a fresh reload for z's zero AND one more reload register
//   spent (or r1 busy) before z's read; a fresh z alone would still read into r1.
//   The step order is now z -= 0x100 before i++ (58 either way): one register further
//   on, it gives the listing's -0x100 r4, 1 r0, compare r1.
// - Not the cause: postreload (a 0..255 constant costs 0 in a SET, so it never turns
//   movs into mov); asm volatile("") between the inits (find_equiv_reg ignores it,
//   58); an asm with operands or a split init (the entry test stays, 89); an unused
//   label between them (deleted before reload, 58). s16 z gives z's zero a fresh
//   reload (find_equiv_reg rejects the wrong-mode zero) but costs the sign extension
//   (85); u16 115; u32 58; j in place of z 58 (j is sl too); direct
//   work->particles[i] 78. Giv forms (75) get the next loop's registers right, but
//   loop.c emits the giv's init after the hoisted zero: 124 live insns against 126,
//   11 refs each, so the giv ranks first and takes r7.
// - Spawn call: sched2 gives it priority 2 (its only dependants are i's zero, r2 then
//   r8) against 3 for movs r5, #238 and the IwramCopyWords pool load, so both go above
//   it. The listing's order (bl, movs r5, ldr r6, movs r2, lsls r5, mov r8, adds r5) is
//   exactly what haifa gives if the r6 load depends on the call: the call's priority
//   becomes 3 and its two dependants win the tie with r5. That needs the address as a
//   memory read after the call; as a constant set (the FIXED_ADDRESS cast, a direct
//   Iwram_CopyWords call, a CopyFunc cast, all 58) it does not depend. A once-only
//   block ending at the call, opened at Task_Remove, Palette_LoadBasicObjColors, the
//   0x1010 store, the actors' 0xff store or the list spawn: 62 to 64 (the arguments
//   lose their order, as before).
// Seventh round, 8 November, unchanged: a once-only block in place of either asm (62);
// the 0x800 copy with the asm dropped (60); the last sprite's store after its priority
// (68); the 512-particle reset through a none = -1 local (62), a block pointer (60),
// ~0 or a once-only block before it (58). Its -1 comes from the cleared 0 (subs #1)
// where the native builds movs 1, negs: the same shared-zero root as the third phase.
// Sixth round, 8 November, unchanged: the third phase's z = (z - 0x800) - ay * 40 in five
// spellings, z declared with its zero, the step list reversed, Random16 and the stores
// reordered (58 to 60); a once-only block or empty asm after BattleEffect_SpawnObjects(1,
// 663, 1), which the native calls before its loop constants (60 to 62); a once-only
// block in place of the second empty asm at every statement near it (62 or worse).
// Fifth round, 8 November: the two asms re-measured (i's cut 60 without, 60 as
// &DECODE_BUFFER[i * 0x400], 0x400 * i, (u32)i or a cast address; the empty asm 62
// without and 62 as a once-only block), so both stay, tagged. Third phase: z and i
// in either order, z from frame or i, a fresh counter, z -= 0x100 in the body, a
// do-while, a once-only block between the inits all 58 (the second zero always
// inherits the first reload's register: the listing's second zero has its own
// reload, so one reload upstream differs); the giv forms 75 to 129. 20 minutes of
// permuter on the third phase's loop (seed 9131, PERM_RANDOMIZE): nothing below 58.
// Fourth round, 7 November: the map loop matches but for k = 0's place (2 rows)
// with the row loop a do-while that sets w = 32 after the load (tagged dead): w is
// then hoisted after table and 6, so its live length is shortest and the ranking
// gives w r4, 6 r5, table r6 as listed. w = 32 at the top of the body hoists
// first (66); after the if, cse folds the test (1708); k as j * 16 1722; k = 0
// before j 105. Item 2 below is settled. Third phase: z = 0 before the if (66),
// z -= 0x100 in the body (58, no change), ay stored through work->particles[i] (107).
// Third round, 7 November: 25 minutes of permuter over the third phase's first
// loop (seed 5811) and about 60 reshapings, none below 66. Map loop: w, 6 and
// table rank by live length (50, 44, 42 insns, 6 refs each); w = 32 inside either
// for still hoists first, and a table local, n[table] or *(table + n) cost 1706
// (one reload more upstream). Third phase: the giv form (83) and p->ay = p->timer
// (64, an extra timer read) both swap z and the stored zero (z r7, zero sl), and an
// empty asm between the two inits does not stop find_equiv_reg (70).
// Devices, 7 November: register s32 z asm("sl") 74 (no help: z is already sl; the
// zero is still shared), on the giv form 136; a zero local pinned to r7 133; a
// pinned r0..r3 zero copied into z 64 when z is set first (cse propagates it: z r6,
// zero sl, the swap again), 95 the other way; asm("" : "+r") on z, i or the zero
// 80 to 101. Map loop: w pinned to r4..r6 117 to 121 (w loses its REG_EQUIV and
// the loop gains an entry test), w through asm("+r") 129. None kept.
// Second round, 6 November: 40 minutes of permuter (seed 7102), nothing better.
// Third phase: no z, p->z from i * -0x100 (83: z's giv and the stores' zero
// share a register); z = 0 through an empty asm with an output before the loop
// (95).
// Near miss: the reload rotation in the third phase, the map loop's registers, the
// spawn call's order and one pos.x load (Left).
// 2026-11-05: draft 3919; frame and every spill slot equal the listing's; with
// registers, slots and offsets masked, 20 lines apart.
// The method that moved it: reload registers rotate through the spill list one per
// reload (allocate_reload_reg), so a wrong scratch register downstream means a reload
// too many or too few upstream, in RTL order, which sched2 hides. Read each from the
// reload dump's order; the raw figure jumps by hundreds on one such reload.
// What is settled, each from the listing:
// - Spill slots run in pseudo order, so the second phase's locals are a block after
//   the first phase, and the wave height and pointer belong to the first phase.
// - Both wave loops: the first arm reads j * 9, the second sets row = j * 9 and steps
//   k from it, so the row's giv combines late and its stepper comes before the
//   pointer's. The call reads sprites[j * 9 + i], not row + i, so PRE leaves it alone
//   and it is an inner giv reduced in the second loop pass.
// - The else arm's index is set in two steps, which leaves it unreduced.
// - First wave loop: the loop optimizer runs twice, and the listing's height is
//   hoisted and reduced in the second run only (slot 12, whole height): its base h
//   has a life of 2 because the stored x is computed before it (k, reused, so x is
//   no giv). Each arm reads the timer, so the index chain is local to the arm and the
//   modulo's temporaries take r3, r2; the one tail takes the explicit mask, built by
//   movs because it moves and rematerializes, and -1024 stays in the loop in HImode,
//   whose short pool range forces the mid-loop pool.
// - Second wave loop: each arm stores the tile itself and their tails cross-jump, with
//   a word mask (no mid-loop pool). The camera follow is two statements the same way.
// - The sprite struct has an s32 (FieldSprite's scale): its members then conflict with
//   s32 stores, so a load after such a store costs its full latency, which orders
//   every sprite pointer load after q's and pos2's stores. No device needed.
// - sprites[i + 18] with i known 21 after its loop gives [fp, r3]; sprites[39] does not.
// - A red local that is not set from the constant first keeps r before g.
// - Hoists set the reload order: the turret's x and ringX are assigned in their loops.
// - pixels and off are long-lived constant locals with no register, rematerialized at
//   each use: the beam's r0 reload and the map loop's movs r7, #224.
// - The explosion blit's x, n, y locals in that order; the shake readout's bg local;
//   the map row counter of 16 (mp set in the for). Task_Add returns s32.
// Forms kept for now, each needing S2's tag at adoption: k reused for the wave x
// (temporary), __asm__ cutting i before DECODE_BUFFER + i * 0x400 (barrier; i known
// 21 makes cse2 swap the add's operands), the empty asm before the missile sprites'
// loop (barrier; the call stays before the hoisted constants), p->ay's volatile view.
// Left, about 20 hunks:
// 1. Third phase: the listing loads zero three times (r4 for i, r0 for z, r7 for the
//    stores); here find_equiv_reg gives z r4 and the two reloads after it run one
//    behind: z r1 here, r2 there, and the -1 of the next loop is subs r1, #1 here, movs
//    and negs there. z as a giv of i (z = i * -0x100 at the top of the body) makes the
//    two zeros but ranks above the stored zero (11 refs, 124 against 126; native below).
//    Keeping i or z from the reload through an asm breaks the hoists' order.
//    Also 66 (2026-11-06): z = 0 before the for, both inits in either order, z declared
//    for the whole third phase, one zero local for the stores. p->ay = p->timer gives 64
//    but reads timer, which the listing does not.
// 2. Map loop's w, n, table in r4, r5, r6 here r6, r5, r4. The native order is
//    table, 6, 32 by live length, so 32 is set after the inner loop's hoists; g reused
//    for the width (for-init) gives table r6, 6 r5 but ranks above k (r0 there).
// 3. The spawn call before the second-phase sprites' hoists: an empty asm or
//    do { } while (0) settles the call but moves its arguments (the call's priority falls).
// 4. pos.x loads before the 0xffffff reload in the listing; here the load waits on the
//    tile store (cost 3). Only with the struct's s32 present.
// Earlier measures that still stand: the second phase's frame loop is a label-and-goto
// loop (as a for, the frame-address copies are hoisted and work loses r11).
#include "resource.h"
#include "heap_release.h"
#include "system/iwram_call.h"
#include "system/irq.h"
#include "types.h"
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile s32 vs32;

typedef struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 ay;
  s32 timer;
} Particle;

typedef struct Vec3 {
  s32 x;
  s32 y;
  s32 z;
  s32 w;
} Vec3;

typedef struct Vec2 {
  s32 x;
  s32 y;
} Vec2;

typedef struct Sprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 scale;
  u8 vram;
  u8 unk11;
  u16 rotation;
  u8 unk14[2];
  u8 unk16;
  u8 unk17;
} Sprite;

/// A sprite's first ten bytes, its attribute 2 fields after eight others.
struct SpriteAttributes {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

typedef struct VramSlot {
  u16 unk0;
  u16 offset;
} VramSlot;

typedef struct Bitmap {
  u32 width : 8;
  u32 height : 8;
  u32 unk2 : 16;
  void *data;
} Bitmap;

struct SummonWork {
  Particle particles[48];
  u8 unk0540[0x1c0];
  u8 frames[0x7080];
  s32 transferMode;
  s32 transferValue;
  u8 unk_7788[8];
  s32 unk7790;
  s32 unk7794;
  s32 unk7798;
  s32 unk779c;
  u8 unk77a0[8];
  s32 shake;
  u8 unk77ac[0x30];
  Sprite *sprites[57];
  u8 unk78c0[0x28];
  s32 transferPending;
};

struct SummonArgument {
  s32 kind;
  s32 side;
  s32 actor;
  s32 unk0c;
  s32 unk10;
  s32 count;
  s32 variant;
  s32 unk1c;
  s32 unk20;
  s16 actors[8];
};

struct InputState {
  u32 held;
  u32 unk4;
  u32 unk8;
  u32 repeat;
};


typedef void (*CopyFunc)(void *, const void *, s32);
typedef void (*ClearFunc)(void *, s32);
typedef void (*BlitFunc)(void *, const void *, s32, s32, s32, s32);

struct SystemWork {
  u8 unk00[0x24];
  s32 *bank;
  u8 unk28[0x34];
  struct SummonWork *summon;
  u8 *canvas;
  u8 *map;
  void *blit;
  u8 unk6c[0x44];
  s32 *control;
};

#define SYSTEM ((struct SystemWork *)0x03000000)
static inline void Iwram_Copy(void *d, const void *s, s32 n) { Iwram_CopyWords(d, s, n); }
/// Iwram_CopyWords reached from IwramIrqMain rather than at its fixed address,
/// as the trade's snapshot copies reach it: its address is then a pool load,
/// made after the arguments and after the call before it.
#define IWRAM_COPY_WORDS_LINKED ((CopyFunc)(IwramIrqMain + Iwram_CopyWordsOffset))
#define Iwram_Clear Iwram_ClearWords
extern VramSlot gResourceTableEntries[];
#define VRAM_SLOTS gResourceTableEntries
#define DECODE_BUFFER ((u8 *)0x02010000)
#define PARTICLES ((Particle *)0x02014000)
extern struct InputState gInput;
extern s32 gCameraSceneParameters[];
struct BgScroll {
  u16 x, y;
};
extern struct BgScroll gBgScroll[];
extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_ForestCanopyTiles;
extern char ResourceId_BronzeAutomatonTiles;
extern char ResourceId_MissileTiles;
extern char ResourceId_Data0BA;
extern char ResourceId_Data0BB;
extern char ResourceId_Data0C2;
extern char ResourceId_Data134;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_SmokeSheet;

extern u16 gData_08197410[];
extern Vec2 Data_08196ed0;
extern Vec2 Data_08196ed8;
extern Vec2 Data_08196ee0;
extern s8 Data_081994f0[];
extern u8 Data_08199508[];
extern u8 Data_08199514[][2];
extern s32 Data_08199528[][2];
extern u8 Data_08199578[];
extern u16 Data_08199582[];
extern u16 Data_0819958c[];
extern s8 Data_08199590[];
extern u8 Data_08199592[][2];
extern u8 Data_0819959c[][6][2];
extern u16 Data_081995cc[];
extern u8 gData_08199210[];
extern u8 gData_08199364[];

void BattleEffect_StartCanvasLayer(s32);
void Func_0813ba50(void);
void Func_08179e6c(void);
s32 BattleEffect_LoadBlitter(s32, s32);
s32 Task_Add(void *, s32);
void BattleActor_SpawnObjectsForListFar(void *, s32);
void BattleBackground_LoadFar(s32, s32, s32);
void Resource_LoadAndDecompress(s32, void *, s32, s32);
Sprite *Func_0815b290(s32, s32, u32, s32);
Sprite *Func_0815b3b0(s32, s32, u32, s32);
void Render_ApplyProjectedPlacementFar(Sprite *, Vec3 *, Vec2 *, s32);
void wait(s32);
void HeapSlot_Release(s32);
void BattleEffect_FetchRectangleBlitters(s32, void **);
u32 Random16(void);
s32 Trig_Cos(s32);
s32 Trig_Sin(s32);
void Sound_PlayFar(s32);
void Func_0815f0a0(s32);
void BattleEffectKernels_IntegrateVector2(Particle *, s32, s32);
void BattleEffectKernels_IntegrateVector3(Particle *, s32, s32);
s32 *Render_AllocateModelList(s32);
void Render_ResetTransformState(void);
void Func_08015160(s32, s32, s32);
void Func_080150e4(s32);
void SceneTransform_ApplyPitch(s32);
void Func_0801521c(s32);
void Render_TransformVertices(const void *, void *, s32);
void Render_DrawModelList(s32 *);
void Heap_Release(void *);
void BattleEffect_CountDownBattlerTints(void);
void Task_Remove(void *);
void Func_0814cca8(void);
s32 VramCache_AllocEntry(s32);
void Palette_LoadBasicObjColors(void);
void BattleEffect_SpawnObjects(s32, s32, s32);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0814cd48(s32, s32, s32, s32, s32);
void Func_08158ce0(s32, s32);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Func_0813baec(void);

#define ALCHEMY_TYPES_H
#include "dma.h"

void BattleEffect_RunDaedalus(struct SummonArgument *arg) {
  s16 targets[14];
  Vec3 pos;
  Vec3 pos2;
  Vec3 pos3;
  Vec2 scale;
  void *blit[2];
  Vec2 scale2;
  Bitmap bitmap;
  Vec2 scale3;
  u8 *dest;
  s32 frame;
  u8 *map;
  s32 *ctl;
  s32 *bank;
  s32 camera;
  s32 prevCamera;
  s32 waveX;
  s32 waveScroll;
  s32 shipY;
  s32 wavePhase;
  s32 savedY;
  s32 bgX;
  struct SummonWork *work;
  s32 k;
  s32 i;
  s32 j;

  dest = SYSTEM->canvas;
  work = SYSTEM->summon;
  map = SYSTEM->map;
  ctl = SYSTEM->control;
  bank = SYSTEM->bank;
  BattleEffect_StartCanvasLayer(0);
  Func_0813ba50();
  Func_08179e6c();
  BattleEffect_LoadBlitter(104, 3);
  blit[0] = SYSTEM->blit;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  camera = 0;
  waveScroll = 0;
  shipY = -0x1d0000;
  wavePhase = 0;
  savedY = gBgScroll[1].x;
  bgX = -80;

  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u16 *col;
      s32 n;
      s32 w;
      s32 off;
      u8 *mp;
      s32 k;
      s32 r, g, b;
      s32 v;
      const u8 *src;
      Sprite *obj;

      Resource_ResetEntry(bank[21]);
      gCameraSceneParameters[4] = 240;
      targets[0] = 255;
      BattleActor_SpawnObjectsForListFar(targets, 0);
      BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
      for (col = (u16 *)0x050000c0, i = 0; i != 128; i++, col++) {
        s32 mid;
        s32 s;

        s = *col & 0x1f;
        mid = (*col >> 5) & 0x1f;
        b = ((*col >> 10) & 0x1f) - 4;
        g = mid - 4;
        r = s - 4;
        if (g < 0)
          g = 0;
        if (r < 0)
          r = 0;
        if (b < 0)
          b = 0;
        if (g > 31)
          g = 31;
        if (r > 31)
          r = 31;
        if (b > 31)
          b = 31;
        *col = (b << 10) | (r << 5) | g;
      }
      work->unk7790 = 0;
      work->unk7794 = 5;
      work->unk7798 = -1;
      work->unk779c = 0;
      Task_Add(Func_0813baec, 0x480);
      ctl[4] = 1;
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->frames, 0, 0);
      off = 0x700;
      n = 3;
      for (j = 0, k = 0; j != 10; j++, k += 16) {
        i = 0;
        mp = (u8 *)(k * 2 + (s32)map);
        do {
          v = ((u8 *)work)[gData_08197410[n] + i + off];
          /* FAKEMATCH(dead): w set to 32 inside the row loop, after the load; as w = 32 at its declaration before the loops, 78 of 3921 instructions differ in tla-en. */
          w = 32;
          if (v != 0) {
            v = v - j * 4 - 40;
            if (v <= 0)
              v = 1;
          }
          *mp++ = v;
        } while (++i != w);
      }
      src = (const u8 *)Resource_GetTableEntry((s32)&ResourceId_ForestCanopyTiles);
      Iwram_Copy((void *)0x050003e0, src, 32);
      src += 32;
      Resource_DecodeType01(src, work->frames);
      for (j = 0; j != 2; j++) {
        for (i = 0; i != 8; i++) {
          obj = Func_0815b290(32, 32, 0x80000000, 0xf000);
          work->sprites[j * 9 + i] = obj;
          obj->priority = 1;
          Iwram_Copy((void *)(VRAM_SLOTS[obj->vram].offset + 0x06010000),
                     &work->frames[(j << 12) + i * 0x200], 0x200);
        }
        obj = Func_0815b3b0(32, 32, 0x80000000, 0xf000);
        work->sprites[j * 9 + 8] = obj;
        Iwram_Copy(obj, work->sprites[j * 9], 24);
        for (i = 0; i != 8; i++)
          work->particles[j * 9 + i].timer = work->sprites[j * 9 + i]->tile;
      }
      src = (const u8 *)Resource_GetTableEntry((s32)&ResourceId_BronzeAutomatonTiles);
      Iwram_Copy((void *)0x05000200, src, 0x1c0);
      src += 0x1c0;
      Resource_DecodeType01(src, DECODE_BUFFER);
      for (i = 0; i != 21; i++) {
        obj = Func_0815b290(32, 32, 0x80002000, 0);
        obj->priority = 3;
        work->sprites[i + 18] = obj;
        Iwram_Copy((void *)(VRAM_SLOTS[obj->vram].offset + 0x06010000),
                   DECODE_BUFFER + i * 0x400, 0x400);
      }
      obj = Func_0815b290(32, 64, 0xc000a000, 0);
      work->sprites[i + 18] = obj;
      obj->priority = 3;
      /* FAKEMATCH(barrier): an asm operand cuts i, known 21, from the copy's source; as
       * DECODE_BUFFER + i * 0x400 with no asm, 26 of 3921 instructions differ in tla-en. */
      __asm__("" : "=r"(i) : "0"(i));
      Iwram_Copy((void *)(VRAM_SLOTS[obj->vram].offset + 0x06010000),
                 DECODE_BUFFER + i * 0x400, 0x800);
      src = (const u8 *)Resource_GetTableEntry((s32)&ResourceId_MissileTiles);
      Iwram_Copy((void *)0x050003c0, src, 32);
      src += 32;
      Resource_DecodeType01(src, DECODE_BUFFER);
      obj = Func_0815b290(16, 8, 0x4000, 0xe000);
      obj->priority = 1;
      work->sprites[41] = obj;
      Iwram_Copy((void *)(VRAM_SLOTS[obj->vram].offset + 0x06010000),
                 DECODE_BUFFER, 0x100);
      /* FAKEMATCH(barrier): an empty asm keeps the call before the loop's hoisted
       * constants; as no asm, 12 of 3921 instructions differ in tla-en. */
      __asm__("");
      for (i = 0; i != 15; i++) {
        work->sprites[42 + i] = Func_0815b3b0(16, 8, 0x4000, 0xf000);
        Iwram_Copy(work->sprites[42 + i], work->sprites[41], 24);
        work->sprites[42 + i]->tile = work->sprites[41]->tile;
        if (i > 6)
          work->sprites[42 + i]->priority = 2;
      }
    }
    if (frame > 27) {
      Vec3 *posp;
      s32 row;
      Sprite **slot;

      scale = Data_08196ed0;
      pos.w = 0;
      pos.y = 0xff0000;
      waveScroll += 0x8000;
      waveX = waveScroll + camera;
      if (waveX > 0x1fffff) {
        wavePhase = (wavePhase + 7) % 8;
        waveX -= 0x200000;
        waveScroll -= 0x200000;
      }
      for (j = 0; j != 2; j++) {
        for (i = 0; i != 9; i++) {
          Sprite *obj;
          s32 v;

          posp = &pos;
          {
            s32 h;

            k = (i << 21) + waveX;
            h = (j << 5) + 103;
            posp->x = k;
            posp->z = h << 16;
          }
          if (i == 8) {
            obj = work->sprites[j * 9 + 8];
            v = work->particles[j * 9 + (wavePhase + 71) % 8].timer;
          } else {
            row = j * 9;
            k = row; k += i;
            obj = work->sprites[k];
            v = work->particles[row + (wavePhase + 64 + i) % 8].timer;
          }
          // The tile through the attributes alone: as obj->tile, the store would
          // also stand for the sprite's 32-bit scale and pos.x's read would wait
          // for it (2 of 3921 instructions differ in tla-en).
          ((struct SpriteAttributes *)obj)->tile = v & 0x3ff;
          if (pos.x > 0xffffff)
            pos.x -= 0x1200000;
          Render_ApplyProjectedPlacementFar(work->sprites[j * 9 + i], &pos, &scale, 0);
        }
      }
    }
    bgX += 4;
    *(s32 *)0x04000028 = bgX << 8;
    work->transferPending = 1;
    wait(1);
  }

  {
  void **blits;
  u16 *pal;
  s32 swayX;
  s32 swayY;
  s32 bobX;
  s32 bobY;
  s32 *ringX;
  void *pixels;

  HeapSlot_Release(104);
  Iwram_Clear(dest, 0x3c00);
  Iwram_Clear((void *)0x06004000, 0x4000);
  *(vu16 *)0x0400000c = 0x786;
  *(vu16 *)0x04000020 = 0x80;
  *(s32 *)0x04000028 = 0;
  blits = blit;
  BattleEffect_FetchRectangleBlitters(0, blits);
  work->unk7790 = 0;
  work->unk7794 = 5;
  work->unk7798 = -1;
  work->unk779c = 0;
  ctl[4] = 1;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0BA, work->frames, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->frames + 0x6c0, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0BB, work->frames + 0x36c0, 0, 0);
  pixels = DECODE_BUFFER;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, pixels, 0, 0);
  Iwram_Copy((void *)0x05000000, (const void *)Resource_GetTableEntry((s32)&ResourceId_SmokeSheet),
             128);
  work->transferMode = 2;
  work->transferValue = 50;
  *(vu16 *)0x04000052 = 0x1010;
  *(vu16 *)0x04000050 = 0x3f44;
  for (i = 0; i != 224; i++) {
    pal = (u16 *)0x05000200 + i;
    if (((*pal >> 5) & 0x1f) == 31)
      break;
  }
  for (i = 0; i != 512; i++)
    PARTICLES[i].timer = -1;

  frame = 0;
  if (!(gInput.held & 2) && !(gInput.repeat & 1)) {
  phase2:
    if (frame == 0) {
      *(vu16 *)0x04000020 = 0x100;
      *pal = 0x190b;
      shipY = -0x1d0000;
      camera = 0;
      work->sprites[39]->rotation = 0xc000;
      work->sprites[38]->rotation = 0;
      work->sprites[31]->rotation = 0;
      work->particles[20].x = 0;
      work->particles[20].y = 0;
      work->particles[20].vx = -0x60000;
      work->particles[20].vy = 0;
      work->particles[20].timer = 0;
      work->particles[23].x = 0;
      work->particles[23].y = 0;
      work->particles[23].vx = -0xc0000;
      work->particles[23].vy = 0;
      work->particles[23].timer = 0;
      work->particles[21].x = 0;
      work->particles[21].y = 0;
      work->particles[21].vx = -0x10000;
      work->particles[21].vy = 0;
      work->particles[21].timer = 0;
      work->particles[22].x = 0xa00000;
      work->particles[22].y = 0;
      work->particles[22].vx = 0;
      work->particles[22].vy = 0;
      work->particles[22].timer = 0;
      for (i = 0; i != 8; i++) {
        Particle *p;

        p = &work->particles[32 + i];
        p->x = ((i / 3) << 18) + 0xb00000;
        p->y = ((i % 3) * 3 << 17) + 0x4a0000;
        p->vx = 0;
        p->vy = 0;
        p->timer = 0;
      }
      for (i = 0; i != 8; i++) {
        Particle *p;

        p = &work->particles[40 + i];
        p->x = ((i / 3) << 18) + 0x700000;
        p->y = ((i % 3) * 3 << 17) + 0x520000;
        p->vx = 0;
        p->vy = 0;
        p->timer = 0;
      }
    }
    prevCamera = camera;
    if (frame >= 542 && frame < 558) {
      s32 r, g, b;

      r = *pal & 0x1f;
      g = (*pal >> 5) & 0x1f;
      b = (*pal >> 10) & 0x1f;
      if (r < 31)
        r++;
      if (g < 20)
        g++;
      if (b < 6)
        b++;
      *pal = (b << 10) | (g << 5) | r;
    }
    if (frame > 557) {
      s32 r, g, b;

      r = *pal & 0x1f;
      g = (*pal >> 5) & 0x1f;
      b = (*pal >> 10) & 0x1f;
      if (r > 20)
        r--;
      if (g > 0)
        g--;
      if (b > 0)
        b--;
      *pal = (b << 10) | (g << 5) | r;
    }
    if (frame == 280) {
      Iwram_Copy((void *)0x05000000,
                 (const void *)Resource_GetTableEntry((s32)&ResourceId_SmokeSheet), 128);
      work->particles[20].timer = 1;
    }
    if (frame == 288)
      work->particles[23].timer = 1;
    if (frame == 279) {
      *(vu16 *)0x04000020 = 0x80;
      *(vu16 *)0x0400000c = 0x784;
      *(vu16 *)0x04000020 = 0x80;
    }
    if (frame == 506)
      work->particles[21].timer = 1;
    if (frame == 489) {
      *(vu16 *)0x04000020 = 0x80;
      *(vu16 *)0x0400000c = 0x784;
      work->unk7798 = 0;
    }
    if (frame == 528) {
      *(vu16 *)0x04000020 = 0x80;
      *(vu16 *)0x0400000c = 0x785;
    }
    if (frame == 0) {
      work->transferMode = 1;
      work->transferValue = 0;
      work->transferMode = 2;
      work->transferValue = 50;
      for (i = 0; i != 10; i++) {
        Particle *p;

        p = &PARTICLES[502 + i];
        p->x = Data_08199514[i][0] << 16;
        p->y = Data_08199514[i][1] << 16;
        p->vx = Data_08199528[i][0];
        p->vy = Data_08199528[i][1];
        p->timer = Random16() & 127;
      }
    }
    for (i = 0; i != 10; i++) {
      Particle *p;

      p = &PARTICLES[502 + i];
      if (frame >= Data_08199578[i]) {
        ((BlitFunc)blit[0])(dest,
                            &work->frames[(Data_08199508[(p->timer / 4) % 12] << 8) + 0x36c0],
                            p->x >> 16, p->y >> 16, 16, 16);
        p->x += p->vx;
        p->y += p->vy;
        p->timer++;
      }
    }
    if (frame <= 473)
      waveScroll += 0x8000;
    waveX = waveScroll + camera;
    if (waveX >= 0x200000) {
      wavePhase = (wavePhase + 7) % 8;
      waveX -= 0x200000;
      waveScroll -= 0x200000;
    }
    if (shipY < 0)
      shipY += 0x2000;
    scale2 = Data_08196ed8;
    bobX = 0;
    bobY = 0;
    if (frame > 537) {
      swayX = Trig_Cos(0x21a00) << 2;
      swayY = Trig_Sin(0x21a00) << 2;
      if (frame >= 542 && frame < 558) {
        s32 v;

        v = (frame << 11) - 0x10f000;
        bobX = Trig_Sin(v) * 3;
        bobY = Trig_Sin(v) * 2;
      }
      if (frame >= 560 && frame < 576) {
        s32 v;

        v = (frame << 11) - 0x118000;
        bobX = Trig_Sin(v) * 2;
        bobY = Trig_Sin(v) * 3;
      }
    } else {
      swayX = Trig_Cos(frame << 8) << 2;
      swayY = Trig_Sin(frame << 8) << 2;
    }
    if (swayY > 0)
      swayY = -swayY;
    swayY -= shipY * 2;
    pos2.w = 0;
    pos2.y = 0xff0000;
    for (i = 0; i != 5; i++) {
      if (frame == Data_08199582[i]) {
        work->shake = 23;
        Sound_PlayFar(144);
      }
    }
    if (frame == 510)
      Sound_PlayFar(186);
    if (frame == 568) {
      work->shake = 10;
      Sound_PlayFar(145);
    }
    if (frame == 578)
      Sound_PlayFar(163);
    for (i = 0; i != 16; i++) {
      Particle *p;

      p = &work->particles[32 + i];
      if (p->timer != 0) {
        Particle *q;

        p->timer++;
        if (p->x > -0x100000) {
          p->x += p->vx;
          p->y += p->vy;
          if (p->vy > -0x10000 && p->vy < 0x10000)
            p->vy += p->ay;
        }
        if (p->vx > -0x80000)
          p->vx -= 0x8000;
        q = &PARTICLES[i * 32 + (p->timer & 31)];
        q->timer = 0;
        q->x = p->x / 2;
        q->y = p->y - 0x140000;
        q->vx = 0;
        q->vy = 0;
      }
      if (work->particles[20].timer == (i & 7) * 6 + 10) {
        Sound_PlayFar(138);
        p->timer = 1;
        p->x += swayX;
        p->y += swayY - (Data_081994f0[work->shake] << 16);
        p->vy = (Random16() & 0xffff) - 0x8000;
        p->ay = (Random16() & 0x3fff) - 0x2000;
      }
    }
    if (frame <= 489) {
      for (i = 8; i != 16; i++) {
        Particle *p;

        p = &work->particles[32 + i];
        pos2.x = p->x + camera;
        pos2.z = p->y;
        if (p->timer != 0)
          Render_ApplyProjectedPlacementFar(work->sprites[40 + i], &pos2,
                                            &scale2, 0);
      }
    }
    if (frame == 578) {
      work->unk7794 = 1;
      work->unk7790 = 0;
    }
    if (frame >= 578 && frame < 594)
      work->unk7798 = ~((frame - 578) / 8);
    {
      for (i = 0; i != 2; i++) {
        pos2.x = camera - swayX + bobX + 0x780000;
        pos2.z = (((i << 5) - Data_081994f0[work->shake]) << 16) + swayY + bobY +
                 0x640000;
        Render_ApplyProjectedPlacementFar(work->sprites[32 + i], &pos2, &scale2,
                                          0);
      }
    }
    if (frame <= 316) {
      Particle *p;

      p = &work->particles[23];
      if (p->timer != 0) {
        p->timer++;
        p->x += p->vx;
        p->y += p->vy;
        p->vy += 0x2000;
        p->vx = p->vx * 48 / 64;
        p->vy = p->vy * 63 / 64;
        work->sprites[38]->rotation += 0x200;
      }
      pos2.x = p->x - swayX + camera + 0x780000;
      pos2.z = ((99 - Data_081994f0[work->shake]) << 16) + swayY + p->y;
      Render_ApplyProjectedPlacementFar(work->sprites[38], &pos2, &scale2, 0);
    }
    {
    Particle *p;
    s32 v;

    v = camera + bobX;
    pos2.x = v + 0x8e0000;
    pos2.z = ((72 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    Render_ApplyProjectedPlacementFar(work->sprites[18], &pos2, &scale2, 0);
    pos2.x = v + 0xae0000;
    pos2.z = ((72 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    Render_ApplyProjectedPlacementFar(work->sprites[19], &pos2, &scale2, 0);
    v += 0x800000;
    pos2.x = v;
    pos2.z = ((104 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    Render_ApplyProjectedPlacementFar(work->sprites[20], &pos2, &scale2, 0);
    p = &work->particles[22];
    if (frame > 602)
      Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
    if (frame == 570) {
      p->x = 0x980000;
      p->y = ((92 - Data_081994f0[work->shake]) << 16) + swayY;
      p->timer = 1;
      p->vx = -0x40000;
      p->vy = -0x5000;
      for (i = 0; i != 16; i++) {
        Particle *q;

        q = &PARTICLES[312 + i];
        q->x = ((Random16() & 15) + 56) << 16;
        q->y = ((Random16() & 31) + 64) << 16;
        q->vx = -((Random16() & 31) + 8) << 12;
        q->vy = -((Random16() & 31) + 8) << 12;
        q->timer = 0;
      }
      for (i = 0; i != 32; i++)
        PARTICLES[400 + i].timer = -1;
      for (i = 0; i != 32; i++)
        PARTICLES[432 + i].timer = -1;
    }
    if (p->timer != 0) {
      if (p->timer > 9) {
        Particle *q;

        q = &PARTICLES[400 + (p->timer & 31)];
        q->timer = 0;
        q->vx = -(Trig_Sin(work->sprites[39]->rotation) * 2);
        q->vy = Trig_Cos(work->sprites[39]->rotation) * 2;
        q->x = (p->x + camera) / 2 -
               ((Trig_Sin(work->sprites[39]->rotation) * 27) >> 1);
        q->y = p->y + Trig_Cos(work->sprites[39]->rotation) * 27 - 0xc0000;
      }
      if (((p->timer >= 50 && p->timer < 60) ||
           (p->timer >= 113 && p->timer < 200)) &&
          (p->timer & 3) == 0) {
        Particle *q;

        q = &PARTICLES[432 + (p->timer & 31)];
        q->timer = 0;
        q->vx = ((Random16() & 31) + 16) << 12;
        q->vy = (-Random16() | ~15) << 14;
        q->x = 0x580000;
        q->y = 0x800000;
      }
      prevCamera = camera;
      {
        Particle *f;

        f = &work->particles[22];
        if (f->timer > 229)
          camera += (0x980000 - (f->x + ((f->timer - 230) << 17)) - camera) / 4;
        else if (f->timer > 0)
          camera += (0x980000 - f->x - camera) / 4;
      }
      if (p->timer > 1) {
        p->x += p->vx;
        p->y += p->vy;
      }
      if (p->timer > 7) {
        p->vx = p->vx * 63 / 64;
        p->vy = p->vy * 63 / 64 + 0x8c8;
      }
      if (p->timer < 80)
        p->vy += 0x200;
      if (p->timer > 29)
        p->vx -= 0x4000;
      if (p->timer >= 40 && p->timer < 68)
        p->vy -= 0x1000;
      if (p->timer >= 50 && p->timer < 60) {
        work->sprites[39]->rotation = (p->timer << 7) - 0x5900;
        p->vy -= 0x1000;
      }
      if (p->timer >= 108 && p->timer < 140) {
        if (p->timer >= 120 && p->timer < 123)
          work->sprites[39]->rotation -= 0x180;
        else
          work->sprites[39]->rotation += 0x100;
        p->vy -= 0x1000;
      }
      if (p->timer >= 140 && p->timer < 160)
        p->vy -= 0x800;
      if (p->timer > 159)
        p->vy -= 0x2000;
      for (i = 0; i != 32; i++) {
        Particle *q;

        q = &PARTICLES[400 + i];
        if ((u32)q->timer <= 5) {
          ((BlitFunc)blits[1])(dest, &work->frames[q->timer * 288],
                              (q->x >> 16) - 2, (q->y >> 16) - 12, 12, 24);
          BattleEffectKernels_IntegrateVector2(q, 64, 0);
          q->timer++;
        }
      }
      for (i = 0; i != 32; i++) {
        Particle *q;

        q = &PARTICLES[432 + i];
        if ((u32)q->timer <= 23) {
          ((BlitFunc)blits[1])(dest, &work->frames[(q->timer / 4 << 11) + 0x6c0],
                              (q->x >> 16) - 16, (q->y >> 16) - 32, 32, 64);
          q->vx += 0x1000;
          BattleEffectKernels_IntegrateVector2(q, 64, 0);
          q->timer++;
        }
      }
      p->timer++;
    }
    scale2.x = 0x103ff;
    scale2.y = 0x103ff;
    if (p->timer == 0) {
      pos2.x = camera + bobX + 0x980000;
      pos2.z = ((92 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    } else {
      pos2.x = p->x + camera;
      pos2.z = p->y;
    }
    Render_ApplyProjectedPlacementFar(work->sprites[39], &pos2, &scale2, 0);
    }
    work->sprites[39]->unk16 = 0;
    work->sprites[39]->unk17 = 32;
    if (frame >= 568) {
      s32 *beam;
      void *buffer;
      s32 v;
      s32 z;

      buffer = Heap_AllocEwram(512);
      beam = Render_AllocateModelList(1);
      bitmap.width = 7;
      bitmap.height = 7;
      bitmap.data = pixels;
      beam[4] = (s32)&bitmap;
      beam[1] = 0;
      beam[2] = (s32)gData_08199364;
      beam[0] = 7;
      beam[3] = (s32)buffer;
      v = frame - 568;
      z = 96 - (v << 4);
      if (z > 0)
        z = 0;
      if (z >= -63) {
        beam[5] = z;
        Render_ResetTransformState();
        Func_08015160(camera + 0x20000, 0x80000, 0);
        Func_080150e4(0x4300);
        SceneTransform_ApplyPitch(-0x800);
        Func_0801521c((v << 14) + 0x4180);
        Render_TransformVertices(gData_08199210, buffer, 4);
        Render_DrawModelList(beam);
      }
      Heap_Release(beam);
      Heap_Release(buffer);
    }
    if (frame == 568) {
      work->particles[30].x = 0;
      work->particles[30].vx = 0x80000;
    }
    for (j = 0; j != 1; j++) {
      ringX = &work->particles[30].x;
      if (frame >= Data_0819958c[j] &&
          frame < Data_0819958c[j] + 26) {
        s32 radius;
        s32 n;

        radius = *ringX >> 16;
        n = (frame - Data_0819958c[j]) / 2;
        *ringX += work->particles[30].vx;
        work->particles[30].vx = work->particles[30].vx * 48 / 64;
        if (n <= 5) {
          for (i = 0; i != 32; i++) {
            s32 v;

            v = (Trig_Sin(i << 11) * radius / 2 >> 16) +
                Data_08199590[j];
            ((BlitFunc)blit[0])(dest, &work->frames[n * 288], v - 6,
                                (Trig_Cos(i << 11) * radius * 2 >> 16) + 60, 12,
                                24);
          }
        }
      }
    }
    scale2.x = 0x10000;
    scale2.y = 0x10000;
    work->sprites[21]->priority = 2;
    work->sprites[22]->priority = 2;
    work->sprites[23]->priority = 2;
    work->sprites[24]->priority = 2;
    {
    s32 v;

    v = prevCamera + bobX;
    pos2.x = v + 0xa00000;
    pos2.z = ((104 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    Render_ApplyProjectedPlacementFar(work->sprites[21], &pos2, &scale2, 0);
    pos2.x = v + 0x800000;
    pos2.z = ((136 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    Render_ApplyProjectedPlacementFar(work->sprites[22], &pos2, &scale2, 0);
    pos2.x = v + 0xa00000;
    pos2.z = ((136 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    Render_ApplyProjectedPlacementFar(work->sprites[23], &pos2, &scale2, 0);
    pos2.x = v + 0xc00000;
    pos2.z = ((136 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    Render_ApplyProjectedPlacementFar(work->sprites[24], &pos2, &scale2, 0);
    work->sprites[25]->priority = 2;
    pos2.x = v + 0x800000;
    pos2.z = ((104 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
    Render_ApplyProjectedPlacementFar(work->sprites[25], &pos2, &scale2, 0);
    }
    for (i = 0; i != 5; i++) {
      pos2.x = (Data_08199592[i][0] << 16) + swayX + prevCamera + bobX +
               0xb00000;
      pos2.z =
          ((Data_08199592[i][1] - Data_081994f0[work->shake]) << 16) +
          swayY + bobY + 0x540000;
      work->sprites[26 + i]->priority = 2;
      Render_ApplyProjectedPlacementFar(work->sprites[26 + i], &pos2, &scale2,
                                        0);
    }
    if (frame <= 489) {
      for (i = 0; i != 8; i++) {
        Particle *p;

        p = &work->particles[32 + i];
        pos2.x = p->x + camera;
        pos2.z = p->y;
        if (p->timer != 0)
          Render_ApplyProjectedPlacementFar(work->sprites[41 + i], &pos2,
                                            &scale2, 0);
      }
    }
    if (frame < 540) {
      Particle *p;

      p = &work->particles[21];
      if (p->timer == 1) {
        p->x += p->vx;
        p->y += p->vy;
        p->vy += 0x2000;
        p->vx = p->vx * 60 / 64;
        p->vy = p->vy * 63 / 64;
      }
      for (i = 0; i != 4; i++) {
        pos2.x = ((i & 1) << 21) + p->x + camera + 0x800000;
        pos2.z = ((((i / 2) << 5) - Data_081994f0[work->shake]) << 16) +
                 swayY + p->y + 0x680000;
        work->sprites[34 + i]->priority = 1;
        Render_ApplyProjectedPlacementFar(work->sprites[34 + i], &pos2, &scale2,
                                          0);
      }
    }
    if (frame <= 316) {
      Particle *p;

      p = &work->particles[20];
      if (p->timer != 0) {
        p->timer++;
        p->x += p->vx;
        p->y += p->vy;
        p->vy += 0x2000;
        p->vx = p->vx * 48 / 64;
        p->vy = p->vy * 63 / 64;
        work->sprites[31]->rotation += 0x100;
      }
      pos2.x = swayX + p->x + camera + 0xb80000;
      pos2.z = ((92 - Data_081994f0[work->shake]) << 16) + swayY + p->y;
      work->sprites[31]->priority = 1;
      Render_ApplyProjectedPlacementFar(work->sprites[31], &pos2, &scale2, 0);
    }
    {
    Vec3 *wave;
    s32 row;
    Sprite **slot;

    for (j = 0; j != 2; j++) {
      for (i = 0; i != 9; i++) {
        Sprite *obj;
        s32 v;

        wave = &pos2;
        wave->x = (i << 21) + waveX;
        wave->z = (((j << 5) - Data_081994f0[work->shake]) << 16) + shipY +
                  0x840000;
        if (i == 8) {
          work->sprites[j * 9 + i]->tile = work->particles[j * 9 + (wavePhase + 71) % 8].timer & 0x3ff;
        } else {
          row = j * 9;
          k = row; k += i;
          work->sprites[k]->tile = work->particles[row + (wavePhase + 64 + i) % 8].timer & 0x3ff;
        }
        if (pos2.x > 0xffffff)
          pos2.x -= 0x1200000;
        Render_ApplyProjectedPlacementFar(work->sprites[j * 9 + i], &pos2, &scale2, 0);
      }
    }
    }
    for (j = 0; j != 4; j++) {
      for (i = 0; i != 6; i++) {
        if (frame >= i * 2 + 490 && frame < i * 2 + 502) {
          s32 x, y, n;

          x = Data_0819959c[j][i][0];
          n = (frame - (i * 2 + 490)) / 2;
          y = Data_0819959c[j][i][1] + (swayY >> 16) - Data_081994f0[work->shake];
          ((BlitFunc)blit[0])(dest, &work->frames[n * 288], x - 6, y - 12, 12, 24);
        }
      }
    }
    for (i = 0; i != 16; i++) {
      for (j = 0; j != 32; j++) {
        Particle *q;

        q = &PARTICLES[i * 32 + j];
        if ((u32)q->timer <= 5) {
          ((BlitFunc)blit[0])(dest, &work->frames[q->timer * 288],
                              (q->x >> 16) - 2, (q->y >> 16) - 12, 12, 24);
          q->x += q->vx;
          q->y += q->vy;
          q->vy -= 0x4000;
          q->timer++;
        }
      }
    }
    {
      s32 *shake;
      s32 v;
      struct BgScroll *bg;
      s32 *params;

      shake = &work->shake;
      bg = gBgScroll;
      v = Data_081994f0[*shake];
      bg[1].y = v + 32;
      gCameraSceneParameters[4] = 120 - v;
      if (*shake > 0)
        *shake -= 1;
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
    if (frame != 800 && !(gInput.held & 2) && !(gInput.repeat & 1))
      goto phase2;
  }

  Task_Remove(Func_0813baec);
  gBgScroll[1].x = savedY;
  ctl[4] = 0;
  Func_0814cca8();
  for (i = 0; i != 57; i++)
    ResourceObject_ReleaseFar(work->sprites[i]);
  bank[21] = VramCache_AllocEntry(128);
  Palette_LoadBasicObjColors();
  Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010,
          (vu32 *)0x040000d4);
  *(u16 *)0x050000bc = *(u16 *)0x050001e8;
  work->transferMode = 2;
  work->transferValue = 50;
  *(s32 *)0x04000028 = 0;
  *(vu16 *)0x04000050 = 0x3f44;
  *(vu16 *)0x04000020 = 0x80;
  *(vu16 *)0x0400000c = 0x784;
  *(vu16 *)0x04000052 = 0x1010;
  arg->actors[arg->count] = 0xff;
  BattleActor_SpawnObjectsForListFar(arg->actors, 0);
  BattleEffect_SpawnObjects(1, 663, 1);
  for (i = 0; i != 15; i++) {
    work->sprites[1 + i] = Func_0815b3b0(32, 16, 0x80006000, 0);
    IWRAM_COPY_WORDS_LINKED(work->sprites[1 + i], work->sprites[0], 24);
    work->sprites[1 + i]->unk16 = 32;
    work->sprites[1 + i]->unk17 = 8;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->frames, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0BA, work->frames + 0x3000, 0, 0);
  {
    const void *colors =
        (const void *)Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    CopyFunc copy = IWRAM_COPY_WORDS_LINKED;

    copy((void *)0x05000000, colors, 128);
  }

  for (frame = 0; frame != 109; frame++) {
    if (frame == 0) {
      s32 z;
      Particle *p;

      i = 0;
      /* FAKEMATCH(dead): a store the next line overwrites; its branch puts
       * a label between i's zero and z's, so z's zero is a reload of its
       * own; without it, 58 of 3921 instructions differ in tla-en. */
      if (work == NULL)
        z = 1;
      z = 0;
      /* FAKEMATCH(dead): a store the loop's first line overwrites; its test
       * reloads work once between z's zero and z's first read; without it,
       * 69 of 3921 instructions differ in tla-en. */
      if (work == NULL)
        p = NULL;
      for (; i != 16; z -= 0x100, i++) {
        p = &work->particles[i];
        p->timer = 0;
        Random16();
        p->ay = 0;
        p->z = z - (*(vs32 *)&p->ay * 40) - 0x800;
        p->vx = -Trig_Cos(p->z) * 8;
        p->vy = -Trig_Sin(p->z) * 8;
        p->x = ((Random16() % 80 + 80) << 16) - p->vx * 24;
        p->y = (((Random16() & 7) + 96) << 16) - p->vy * 24;
        work->sprites[i]->rotation = p->z;
        work->sprites[i]->unk16 = 0;
        work->sprites[i]->unk17 = 8;
      }
      for (i = 0; i != 512; i++)
        PARTICLES[i].timer = -1;
    }
    scale3 = Data_08196ee0;
    pos3.w = 0;
    pos3.y = 0xff0000;
    for (i = 0; i != 16; i++) {
      Particle *p;

      p = &work->particles[i];
      if (frame >= Data_081995cc[i] && p->timer == 0) {
        pos3.x = p->x;
        pos3.z = p->y;
        if (pos3.z > 0 && pos3.z < 0x800000)
          Render_ApplyProjectedPlacementFar(work->sprites[i], &pos3, &scale3,
                                            0);
        if (p->y > 0x6fffff) {
          p->timer = 1;
          for (j = 0; j != 2; j++) {
            s32 speed;
            s32 angle;
            Particle *q;

            q = &PARTICLES[i * 8 + j];
            speed = (Random16() & 31) + 32;
            angle = (Random16() & 0x7fff) + 0x4000;
            q->x = p->x / 2;
            q->y = p->y;
            q->vx = Trig_Sin(angle) * speed >> 4;
            q->vy = Trig_Cos(angle) * speed >> 4;
            q->timer = 0;
          }
          Sound_PlayFar(144);
          for (j = 0; j != arg->count; j++) {
            BattleEffect_StartActorMotion(arg->actors[j], 1, 0, 0, 0x20000, 100);
            Func_0814cd48(arg->actors[j], 7, 5, j, 7);
            work->shake = 4;
          }
        } else {
          Particle *q;

          { s32 m = (frame - Data_081995cc[i]) % 16; q = &PARTICLES[(i << 4) + m + 256]; }
          q->x = p->x / 2;
          q->y = p->y - 0x100000;
          q->vx = 0;
          q->vy = 0;
          q->timer = 0;
        }
        work->sprites[i]->rotation = p->z;
        BattleEffectKernels_IntegrateVector3(p, 64, 0);
        p->vx = -Trig_Cos(work->sprites[i]->rotation) * 8;
        p->vy = -Trig_Sin(work->sprites[i]->rotation) * 8;
        work->sprites[i]->tile = work->sprites[0]->tile;
      }
    }
    for (i = 0; i != 256; i++) {
      Particle *q;

      q = &PARTICLES[256 + i];
      if ((u32)q->timer <= 7) {
        s32 v;

        v = q->timer - 2;
        if (v >= 0)
          ((BlitFunc)blits[1])(dest, &work->frames[v * 288 + 0x3000],
                              (q->x >> 16) - 6, (q->y >> 16) - 12, 12, 24);
        q->timer++;
      }
    }
    for (i = 0; i != 128; i++) {
      Particle *q;

      q = &PARTICLES[i];
      if ((u32)q->timer <= 17) {
        s32 v;

        v = q->timer / 3;
        ((BlitFunc)blits[i & 1])(dest, &work->frames[v << 11],
                                (q->x >> 16) - 16, (q->y >> 16) - 48, 32, 64);
        BattleEffectKernels_IntegrateVector2(q, 56, -0x2000);
        q->timer++;
      }
    }
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  for (i = 0; i != 16; i++)
    ResourceObject_ReleaseFar(work->sprites[i]);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
  }
}
