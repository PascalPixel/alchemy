// Measured 2026-11-05 (tla-en): differ 183 of 7666, slots normalized 103, high
// registers blind too 101, every register blind 63. The frame is the listing's
// 264 and i is r11 (226 of 229 rows).
//
// What settled i in r11: the three-round loops at kinds 14, 68 and 84 release
// their blitters in a do { } while (0), which ends CSE's block, so the two table
// loads stay short-lived, the loop no longer hoists them, and with m first and
// m, x and y local to the loop, r8, r10 and r9 fill before i is allocated.
// A pair of block locals freed most of the rest: life and its half at kinds 5 and
// 86, m, x and y at kind 15. Loops whose p is stepped with i read p = &SPARKS[i]:
// loop reduction keeps the step in its own register with p a copy, exactly as
// the listing's kind 66. Where i is set before its pointer (kind 44 and the
// first particles), i = 0 comes first. Kind 41's offsets 0x1cd2 and 0x3781 were
// wrong by the work's 0x700.
//
// Two facts to read this function by. sched2 moves an instruction across a call
// when it only touches a callee-saved register, so an r5 set before a call and
// read after is often a reload, not a local. And reload rotates through r0, r1,
// particle, r3, r5: one reload more or less shifts the low registers of every later
// row, so differences are read from the top, and one fix can settle hundreds.
//
// Four more facts, each of which settled many rows here. A local mentioned once in
// the loop is folded back by loop.c; set in the loop's block and mentioned twice it
// hoists, and a hoisted constant-valued local with no register is rematerialized
// by reload, which costs one rotation step. PRE's reaching register survives copy
// propagation only when the first register is set again in the same block: vec is
// origin, then base. A draw call returns a value (the load of r0 otherwise ranks
// first by dependants). A loop pointer p = &SPARKS[i] keeps a separate stepping
// register only when p is used outside the loop, so p is the shared one there.
//
// Reload's rotation is r0, r1, r2, r3, r5; a form that pressures a low-register loop
// until reload takes r4 adds r4 to the rotation for the whole function, and the
// figure jumps to about 4400. Reading such a jump as a wrong form is a mistake.
//
// Three more facts that settled large arms. A loop written with labels and goto
// has no loop notes, so nothing hoists out of it and its steps are explicit: kinds
// 80 and 85. A local reused for several values is global and ranks low: kind 71's
// fade, angle and size are one local, which is what keeps r5 free there. Passing a
// value through a long-lived local of the function gives it a high register, as
// kind 85's limit does for its row base.
//
// Left, top down (slots normalized 103, every register blind 72):
// - Kind 80 (28): r3, r2, r1 for three, mirror and source; here r2, r1, r2. The
//   destination's symbol loads into r1 in the listing. The buffer here is the symbol
//   gMapCellBuffer, the constant everywhere else; Data_02010208 needs a home.
// - Kind 85 (34): the base's long-lived local takes r8 and h sl, swapped; reload is
//   one step behind from the arm's first load (r3 where the listing has r5), and
//   no reload shows between. threshold's asm cut stands for a copy GCSE otherwise
//   propagates away; the source's form of that copy is not known.
// - The projection's fresh add takes the asm's second mention of o; the source's
//   second mention is not known.
// - Kind 79 (15): the copy of the step is r8 and the step r7, here reversed by
//   rank; first uses read the step in the listing, so p is replaced there up to the
//   first label, and p is not the shared one. record_giv makes a giv non-replaceable
//   only when it is mentioned outside the loop or the biv steps inside its life.
// - Kind 70's step starts from i at run time, 4 rows: i = 0 is above the if, so
//   the biv's start is not known; the listing has no multiply.
// - Ties: kind 90's texture store order (8), kind 84's pixels (4), kind 16's store (2),
//   the first center store (2), kind 14's mov r2, r8 (2).
//
// Before adoption: RELEASE_BLITTERS and RELOAD_BLITTER are exempt only if every
// use matches through them, CLEAR and FILL need S2's wrapper tags unless they are
// the game's own layer, and both asms (the "X" at the first projection, the cut on
// kind 85's threshold) and kind 80 and 85's closing do { } while (0) are barriers.

// Draft, differ 4660 of 7666, skeleton gap 417 (tla-en, 2026-11-01)
// Most of the differ is two offsets: the frame is 256 against 264, so every
// slot above the low spills sits 8 low (action 120 against 128), and i takes r10
// where the listing has r11. Fix those first; most of the gap is sched2 order
// that follows from allocation.
// Proved here: the buffer is one set-once local at the top, unallocated, so its
// pool load returns at each use. n, m, x, y, p and the like belong to their
// kind's arm, not to one shared set. i * 24 at kind 80 is (i * 2 + i) * 8; the
// second particle halving is p->vx; center is written through a local pointer.
// Kinds 63, 70 and 85 set y in both arms: the tail is cross-jumped, and that is
// what keeps y + 24 and y - 32 apart. vz is in both arms at the first particles;
// life / 4 comes before the projection at the first spark arm; the frame index
// is m = ...; m += ...; in two statements.
// Target's slot: PRE's reaching register survives copy propagation only with a
// second set, which comes from the second target read at kinds 96 to 98 staying
// in its block. RELOAD_BLITTER's do { } while (0) ends CSE's block there and
// gives slot 12 with 37 uses and the six direct reads at sp+240. It stands for
// every adjacent release and load, so S2's macro exemption applies only once
// every use matches through it. It may instead be CSE's flush after 1,000 insns
// in the listing; nothing here tells the two apart.
// Left to explain:
// - Two missing slots, 28 and 24, at kind 60: m * 0xa00 and the third source
//   have no register in the listing, with r9 and r11 free there, while r9 takes
//   them here. Named locals for both change nothing. This is the 8 bytes.
// - Origin's slot 16 has one store and one direct use at the first projection,
//   so the two-set reason above does not fit it; a local pointer does not help.
// - i in r11 needs r8, r10 and r9 used by pseudos that outrank it and conflict
//   with it. Here i (1.38) outranks count and n in the first frame loop, and the
//   listing's i has about the same live length. Splitting i at the main loop
//   gave 5277, one shared i 4882.
// Tried: an inlined scroll helper gives no direct reads (and is a wrapper).
// Kind 83 before the loop gave 1566 and inside it with a break 1599 on the old
// draft. target[0] / 2 before step lost 25. The 34-row loop is i in r11, a
// separate threshold in lr, and 15 and 0x700 unhoisted; that form lost 19 here,
// since k stays in r8 and the constants hoist. A greedy pass over block locals
// has stopped gaining.
#include "heap_release.h"
#include "types.h"

#include "dma.h"

struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 vz;
  s32 life;
};

struct SummonWork {
  struct Particle particle[64];
  u8 frames[0x5100];
  u8 frames2[0x1880];
  s32 wave[160];
  u8 unk7300[0x480];
  s32 unk7780;
  s32 unk7784;
  u8 unk_7788[0x20];
  s32 unk77a8;
  u8 unk77ac[8];
  s32 unk77b4;
  s32 unk77b8;
  u8 unk77bc[0x12c];
  s32 unk78e8;
};

struct SummonAction {
  s32 id;
  s32 side;
  s32 target;
  u8 unk0c[0xc];
  s32 unk18;
  s32 unk1c;
  u8 unk20[4];
  s16 actor;
};

struct BattleObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x10];
  s32 unk24;
  s32 unk28;
  s32 unk2c;
  u8 unk30[4];
  s32 unk34;
  u8 unk38[0x10];
  s32 unk48;
};

typedef s32 (*DrawFunc)(void *, const void *, s32, s32, s32, s32);

struct DrawFuncs {
  DrawFunc draw[2];
};

struct Texture {
  u8 width;
  u8 height;
  u16 unk2;
  void *pixels;
};

struct Model {
  s32 kind;
  s32 unk4;
  const void *mesh;
  void *matrix;
  struct Texture *texture;
  long fade;
  u8 unk18;
  u8 frame;
};

struct IwramPointers {
  u8 unk00[0x30];
  void *bg;
  u8 unk34[0x28];
  struct SummonWork *summon;
  void *screen;
  void *tiles;
  DrawFunc slot68;
  u8 unk6c[0x50];
  DrawFunc slotbc;
};

#define IWRAM ((struct IwramPointers *)0x03000000)

extern u8 Value_000000b4;
extern u8 Value_000000b9;
extern u8 Value_000000c1;
extern u8 Value_000000c2;
extern u8 Value_000000d9;
extern u8 Value_000000da;
extern u8 Value_000000dd;
extern u8 Value_000000e6;
extern u8 Value_000000f0;
extern u8 Value_000000f1;
extern u8 Value_00000105;
extern u8 Value_00000115;
extern u8 Value_00000129;
extern u8 Value_00000130;
extern u8 Value_00000134;
extern u8 Value_0000013a;
extern u8 Value_0000013e;
extern u8 Value_00000147;
extern u8 Value_00000148;
extern u8 Value_0000014d;
extern u8 Value_00000150;
extern u8 Value_00000151;
extern u8 Value_00000153;
extern u8 Value_00000155;
extern u8 Value_00000157;
extern u8 Value_00000159;
extern u8 Value_0000015c;
extern u8 Value_00000161;
extern u8 Value_00000163;
extern u8 Value_00000166;
extern u8 Value_00000167;
extern u8 Value_00000169;
extern u8 Value_0000016d;
extern u8 Value_0000016f;
extern u8 Value_00000170;
extern u8 Value_00000171;
extern u8 Value_00000172;
extern u8 Value_00000178;
extern u8 Value_0000017c;
extern u8 Value_0000017d;
extern u8 Value_0000017f;
extern u8 Value_00000182;
extern u8 Value_00000184;
extern u8 Value_00000187;
extern u8 Value_00000188;
extern u8 Value_0000018a;
extern u8 Value_00000192;
extern u8 Value_00000193;

extern const u16 gData_08197410[];
extern const u8 gData_08197467[];
extern const u8 gData_0819746d[];
extern const u8 gData_08197473[];
extern const u16 gData_0819747a[];
extern const u16 gData_08197486[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern const u8 gData_0819749e[];
extern u8 gData_081974ad[];
extern u16 gData_081974bc[];
extern u8 gData_0819750c[];
extern u8 gData_0819751a[];
extern const u8 gData_081977f8[];
extern const u8 gData_0819781a[];
extern u8 gData_08197826[];
extern const u32 gData_08197834[];
extern const u8 gData_0819887e[];
extern const u8 gData_08198882[];
extern const u8 gData_08198886[];
extern const u16 gData_0819888a[];
extern const u16 gData_0819889e[];
extern const u16 gData_081988b2[];
extern u16 gData_081988c6[];
extern u16 gData_081988da[];
extern const u16 gData_081988ee[];
extern const u16 gData_081988fc[];
extern const u16 gData_0819890a[];
extern const u16 gData_08198918[];
extern u16 gData_08198926[];
extern const u16 gData_08198934[];
extern const u8 gData_0819893a[];
extern const u8 gData_0819893d[];
extern u8 gData_08198940[][2];
extern const u16 gData_08198946[];
extern const u8 gData_0819894e[];
extern const u8 gData_08198952[];
extern const u8 gData_08198956[];
extern const s8 gData_0819895a[];
extern const u8 gData_0819895e[];
extern const u8 gData_08198962[];
extern const u8 gData_08198966[];
extern const u8 gData_08198c6c[];
extern const u8 gData_08198d2c[];
extern const u8 gData_08199090[];
extern const u8 gData_081990d0[];
extern const u8 gData_081991b0[];
extern const u8 gData_08199210[];
extern const u8 gData_08199268[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

s32 __divsi3(s32, s32);
s32 __modsi3(s32, s32);
s32 Trig_Cos(s32);
s32 Trig_Sin(s32);
void HeapSlot_Release(s32);
void Heap_Release(void *);
void *Resource_GetTableEntry(s32);
void wait(s32);
s32 Task_Add(void *, s32);
s32 Task_Remove(void *);
u32 Random16(void);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPitch(s32);
void SceneTransform_ApplyYaw(s32);
void Func_080150e4(s32);
void Func_08015160(s32, s32, s32);
void Func_080151e4(s32, s32, s32);
void Func_0801521c(s32);
void Transform_SetLookAt(void *, void *);
void Actor_SetMode(struct BattleObject *, s32);
void Actor_SetAnimationSpeedFar(struct BattleObject *, s32);
s32 BattleUnit_GetEffectHeightFar(s32);
void Func_08118088(s32, s32);
struct BattleObject **GetBattleObjectSlotFar(s32);
void Func_081180e8(s32);
void BattleEffectKernels_IntegrateVector3(struct Particle *, s32, s32);
void BattleEffectKernels_IntegrateVector2(struct Particle *, s32, s32);
void Func_08143001(void);
void Func_08143489(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_StartCanvasLayer(s32);
void BattleEffect_StartTiledCanvas(s32);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_FetchRectangleBlitters(s32, struct DrawFuncs *);
s32 BattleEffect_RunSparkGroups(struct SummonAction *, s32);
void BattleEffect_RunMysticFlame(struct SummonAction *);
void Func_0814cd48(s32, s32, s32, s32, s32);
void SummonEffect_RunCanvasMode3(struct SummonAction *);
void SummonEffect_RunCanvasMode1(struct SummonAction *);
void SummonEffect_RunCanvasMode2(struct SummonAction *);
void Func_08152475(void);
void BattleEffect_RunProjectileVolley(struct SummonAction *, s32);
void Resource_LoadAndDecompress(s32, void *, s32, s32);
void Func_08158ce0(s32, s32);
void Func_0815b434(void *, s32, s32);
s32 EffectPosition_ProjectPoint(void *, s32 *);
void EffectPosition_ProjectScaledBattler(s32, s32 *);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0815f0a0(s32);
void Graphics_PackTileRows(void *, void *, s32, s32);
void Func_0818caa8(void *, s32, s32, s32);
s32 BattleEffect_LoadBlitter(s32, s32);
s32 Func_08196404(s32, s32, s32, s32, s32);
void Render_TransformVertices(const void *, void *, s32);
struct Model *Render_AllocateModelList(s32);
void Render_DrawModelList(struct Model *);
void Sound_PlayFar(s32);

#define REG_BG2PA (*(volatile u16 *)0x04000020)
#define REG_BG2X (*(volatile s32 *)0x04000028)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
#define DMA3 ((volatile u32 *)0x040000d4)
#define PLTT ((void *)0x05000000)
#define PLTT_U16 ((volatile u16 *)0x05000000)
#define BG_VRAM ((void *)0x06004000)
#define SPARKS sparks
#define RINGS ((struct Particle *)0x02016e00)
#define IWRAM_SLOT_68 (IWRAM->slot68)
#define IWRAM_SLOT_BC (IWRAM->slotbc)
static __inline__ void CLEAR(void *dst, s32 size) { ((s32 (*)(void *, s32))0x03000258)(dst, size); }
static __inline__ void FILL(void *dst, s32 size, s32 value) { ((s32 (*)(void *, s32, s32))0x03000260)(dst, size, value); }
#define RELOAD_BLITTER(slot, n) do { HeapSlot_Release(slot); BattleEffect_LoadBlitter(slot, n); } while (0)
#define RELEASE_BLITTERS() HeapSlot_Release(188); do { HeapSlot_Release(104); } while (0)

extern u8 gMapCellBuffer[];
extern u8 Data_02010208[];
void Func_0815f16c(struct SummonAction *action, s32 kind) {
  u8 *buffer;
  struct Particle *sparks;
  struct SummonWork *work;
  void *screen;
  s32 frame;
  void *bg;
  void *tiles;
  s32 scrollA;
  s32 scrollB;
  s32 duration;
  s32 base[3];
  s32 target[3];
  s32 origin[3];
  s32 center[3];
  s32 pos[3];
  s32 point[3];
  s32 point2[3];
  struct DrawFuncs drawFuncs;
  struct Texture texture164;
  struct Texture texture156;
  struct Texture texture148;
  struct Texture texture140;
  struct Texture texture132;
  struct Texture *tex;
  s32 step;
  s32 i;
  u8 *src;
  u8 *dst;
  s32 v;
  s32 *vec;

  buffer = (u8 *)0x02010000;
  sparks = (struct Particle *)0x02016000;
  work = IWRAM->summon;
  screen = IWRAM->screen;
  bg = IWRAM->bg;
  tiles = IWRAM->tiles;
  scrollA = 0;
  scrollB = 0;

  if (kind == 41 || kind == 38 || kind == 62 || kind == 85 || kind == 86 ||
      kind == 87 || kind == 88)
    BattleEffect_StartTiledCanvas(0x2000);
  else
    BattleEffect_StartCanvasLayer(0);

  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&Value_00000134, tiles, 0, 0);
  Resource_LoadAndDecompress((s32)&Value_00000159, work->frames, 1, 0);
  Resource_LoadAndDecompress((s32)&Value_0000015c, buffer, 1, 0);
  Graphics_PackTileRows(buffer, work->frames + 0x5100, 40, 0x120);

  if (kind == 5 || kind == 53 || kind == 78) {
    Resource_LoadAndDecompress((s32)&Value_0000013e, buffer, 1, 0);
  } else if (kind == 42 || kind == 15) {
    Resource_LoadAndDecompress((s32)&Value_0000016d, buffer, 1, 0);
  } else if (kind == 16) {

    s32 j;
    s32 v;
    u8 *src;

    Resource_LoadAndDecompress((s32)&Value_000000d9, buffer, 1, 0);
    for (i = 1; i != 8; i++) {
      s32 fade; j = 0; fade = i * 8; src = buffer;
      for (; j != 0x400; j++) {
        v = *src++;
        if (v != 0) {
          v -= fade;
          if (v <= 0)
            v = 1;
        }
        buffer[j + i * 0x400] = v;
      }
    }
  } else if (kind == 10) {
    Resource_LoadAndDecompress((s32)&Value_00000105, buffer, 1, 0);
  } else if (kind == 6 || kind == 57 || kind == 14 || kind == 17) {
    Resource_LoadAndDecompress((s32)&Value_00000192, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_00000188, buffer + 0xc56, 1, 0);
  } else if (kind == 68) {
    Resource_LoadAndDecompress((s32)&Value_00000192, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_00000161, buffer + 0x1809, 1, 0);
  } else if (kind == 61) {
    Resource_LoadAndDecompress((s32)&Value_0000013a, buffer, 1, 1);
  } else if (kind == 38) {
    Resource_LoadAndDecompress((s32)&Value_00000187, buffer, 1, 1);
  } else if (kind == 44) {
    Resource_LoadAndDecompress((s32)&Value_00000130, buffer, 1, 0);
  } else if (kind == 60) {
    Resource_LoadAndDecompress((s32)&Value_00000192, buffer, 1, 0);
  } else if (kind == 46) {
    Resource_LoadAndDecompress((s32)&Value_0000017c, buffer, 1, 0);
  } else if (kind == 50) {
    Resource_LoadAndDecompress((s32)&Value_00000178, buffer, 1, 0);
  } else if (kind == 90) {
    Resource_LoadAndDecompress((s32)&Value_000000da, buffer, 0, 0);
  } else if (kind == 91) {
    Resource_LoadAndDecompress((s32)&Value_000000c1, buffer + 0x4000, 0, 0);
  } else if (kind == 84) {
    s32 j;
    s32 k;

    Resource_LoadAndDecompress((s32)&Value_00000192, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_00000188, work->frames, 1, 1);
CLEAR(buffer + 0xc56, 0x4000);
    for (i = 0; i != 4; i++) {
      for (j = 0; j != 120; j++) {
        for (k = 0; k != 24; k++)
          buffer[0xc5a + i * 0x1000 + j * 32 + k] =
              work->frames[i * 2880 + j * 24 + k];
      }
    }
    Resource_LoadAndDecompress((s32)&Value_00000159, work->frames, 1, 0);
  } else if (kind == 63 || kind == 64) {
    Resource_LoadAndDecompress((s32)&Value_00000115, buffer, 1, 0);
  } else if (kind == 71 || kind == 82) {
    Resource_LoadAndDecompress((s32)&Value_000000b4, buffer, 0, 0);
  } else if (kind == 96 || kind == 97 || kind == 98) {
    Resource_LoadAndDecompress((s32)&Value_0000018a, buffer, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_0000013e, buffer + 0xd80, 1, 0);
  } else if (kind == 80) {
    s32 k; s32 i;

    Resource_LoadAndDecompress((s32)&Value_0000013a, buffer + 0x1000, 1, 0);
CLEAR(buffer, 0x1000);
    for (i = 0; i != 48; i++) {
      for (k = 0; k != 24; k++) {
        buffer[0x208 + i * 64 + k] = buffer[0x1000 + i * 24 + k];
        buffer[0x237 + i * 64 - k] = buffer[0x1000 + i * 24 + k];
      }
    }
  } else if (kind == 79) {
    s32 i;
    s32 k;

    Func_0815b434(buffer, 48, 0x10000);
    for (i = 0; i != 48; i++) {
      for (k = 0; k != 24; k++) {
        buffer[0x800 + i * 48 + k] = buffer[i * 24 + k];
        buffer[0x800 + i * 48 + 47 - k] = buffer[i * 24 + k];
        buffer[0x800 + (95 - i) * 48 + k] = buffer[i * 24 + k];
        buffer[0x800 + (95 - i) * 48 + 47 - k] = buffer[i * 24 + k];
      }
    }
  } else if (kind == 94) {
    Resource_LoadAndDecompress((s32)&Value_0000014d, buffer, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_000000e6, buffer + 0x2000, 1, 1);
  } else if (kind == 73) {
    REG_BLDALPHA = 0xc10;
  } else if (kind == 70) {
    Resource_LoadAndDecompress((s32)&Value_00000193, buffer, 1, 0);
  } else if (kind == 66) {
    Resource_LoadAndDecompress((s32)&Value_00000129, buffer, 1, 1);
  } else if (kind != 41 && kind != 62) {
    Resource_LoadAndDecompress((s32)&Value_00000161, buffer, 1, 0);
  }

  switch (kind) {
  case 0:
  case 4:
  case 7:
  case 8:
  case 12:
  case 38:
  case 39:
  case 40:
  case 41:
  case 42:
  case 43:
  case 63:
  case 72:
  case 73:
  case 79:
  case 85:
  case 94:
  case 97:
  case 98:
    Dma_Set(Resource_GetTableEntry((s32)&Value_00000184), PLTT, 0x84000020,
            DMA3);
    if (action->unk1c == 1)
      Resource_LoadAndDecompress((s32)&Value_00000157, buffer + 0x3c56, 1,
                                 0);
    break;
  case 2:
  case 9:
  case 10:
  case 11:
  case 13:
  case 44:
  case 45:
  case 46:
  case 47:
  case 48:
  case 49:
  case 68:
  case 69:
  case 70:
  case 71:
  case 80:
  case 82:
  case 99:
    if (action->unk1c == 1)
      Resource_LoadAndDecompress((s32)&Value_00000155, buffer + 0x3c56, 1,
                                 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&Value_00000155), PLTT, 0x84000020,
              DMA3);
    break;
  case 3:
  case 5:
  case 14:
  case 15:
  case 50:
  case 51:
  case 52:
  case 53:
  case 54:
  case 55:
  case 64:
  case 65:
  case 66:
  case 67:
  case 74:
  case 78:
  case 81:
  case 90:
  case 91:
    if (action->unk1c == 1)
      Resource_LoadAndDecompress((s32)&Value_00000151, buffer + 0x3c56, 1,
                                 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&Value_00000151), PLTT, 0x84000020,
              DMA3);
    break;
  case 1:
  case 6:
  case 16:
  case 17:
  case 56:
  case 57:
  case 58:
  case 59:
  case 60:
  case 61:
  case 62:
  case 75:
  case 76:
  case 77:
  case 83:
  case 84:
  case 86:
  case 88:
  case 89:
  case 92:
    if (action->unk1c == 1)
      Resource_LoadAndDecompress((s32)&Value_00000153, buffer + 0x3c56, 1,
                                 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&Value_00000153), PLTT, 0x84000020,
              DMA3);
    break;
  case 93:
  case 95:
  case 96:
    Dma_Set(Resource_GetTableEntry((s32)&Value_00000182), PLTT, 0x84000020,
            DMA3);
    break;
  case 100:
    Resource_LoadAndDecompress((s32)&Value_00000155, buffer + 0x3c56, 1, 1);
    break;
  }

  if (kind == 81 || kind == 83 || kind == 91 || kind == 92 || kind == 93)
    Resource_LoadAndDecompress((s32)&Value_000000c2, buffer, 0, 0);
  work->unk7780 = 2;
  if (kind == 42)
    work->unk7784 = 75;
  else
    work->unk7784 = 50;
  Task_Add(Func_08143001, 3200);
  EffectPosition_ProjectScaledBattler(action->actor, target);
  EffectPosition_ProjectScaledBattler(action->target, base);
  BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
  {
  struct BattleObject *object;
  s32 save24;
  s32 save28;
  s32 save2c;
  s32 save34;
  s32 save48;
  struct BattleObject *object2;
  s32 n;
  s32 m;
  s32 t;
  s32 r;
  s32 x;
  s32 y;
  s32 h;
  s32 count;
  s32 start;
  s32 half;
  s32 height;
  s32 angle;
  s32 top;
  s32 scroll;
  s32 life;
  s32 row;
  s32 limit;
  s32 size;
  s32 fade;
  s32 scale;
  void *matrix;
  struct Model *model;
  const u8 *table;
  s32 k;
  struct Particle *p;

  work->unk77b4 = 24;
  work->unk77b8 = 0;
  object = *GetBattleObjectSlotFar(action->target);
  for (i = 0; i != 64; i++) {
    work->particle[i].x = (Random16() & 63) + 32;
    work->particle[i].y = 0;
    work->particle[i].z = 0;
    work->particle[i].vx = Random16() & 0xffff;
    work->particle[i].vy = Random16() & 0xffff;
    work->particle[i].vz = Random16() & 0xffff;
  }
  Actor_SetAnimationSpeedFar(object, 0);
  vec = origin;
  vec[0] = object->x;
  vec[1] = object->y + 0x500000;
  vec[2] = object->z;
  save24 = object->unk24;
  save28 = object->unk28;
  save2c = object->unk2c;
  save34 = object->unk34;
  save48 = object->unk48;
  object->unk24 = 0;
  object->unk28 = 0;
  object->unk2c = 0;
  object->unk34 = 0;
  object->unk48 = 0;
  vec = base;
  EffectPosition_ProjectScaledBattler(action->target, vec);
  vec[0] = vec[0] / 2;
  Sound_PlayFar(212);

  for (frame = 0; frame != 32; frame++) {
    struct Particle *p;
    s32 n;
    count = 0;
    p = work->particle;
    for (i = 0; i != 64; i++, p++) {
      if (p->x >= 0) {
        if (frame >= i / 4) {
          n = 5;
          Render_ResetTransformState();
          Func_080150e4(p->vz);
          SceneTransform_ApplyPitch(p->vx);
          SceneTransform_ApplyYaw(p->vy);
          EffectPosition_ProjectPoint(p, pos);
          pos[0] = pos[0] / 2 + base[0];
          if (kind <= 37)
            pos[1] = pos[1] + base[1] - 8;
          else if (kind == 65)
            pos[1] = pos[1] + base[1] + 44;
          else
            pos[1] = pos[1] + base[1] + 12;
          if (pos[2] < -60)
            pos[2] = -60;
          if (pos[2] > 60)
            pos[2] = 60;
          pos[2] += 60;
          drawFuncs.draw[1](screen, (u8 *)tiles + gData_08197410[n - 1],
                            pos[0] - n / 2, pos[1] - n, n, n * 2);
          p->x -= 4;
        }
        count++;
      }
    }
    if (kind <= 37 && count < 64) {
      s32 *o = origin;
      Render_ResetTransformState();
      Transform_SetLookAt(bg, (u8 *)bg + 12);
      EffectPosition_ProjectPoint(o, pos);
      pos[0] = pos[0] / 2;
      drawFuncs.draw[0](screen, buffer + 0x3c56, pos[0] - 10, pos[1] - 4, 20,
                        40);
      __asm__("" : : "X"(o));
    }
    work->unk78e8 = 1;
    wait(1);
  }

  if (kind == 41) {
    REG_BG2PA = 0x100;
    if (action->side == 0)
      REG_BG2X = (frame - target[0]) << 8;
    else
      REG_BG2X = (96 - target[0]) << 8;
  } else if (kind == 85) {
    REG_BG2PA = 0x100;
    if (action->side == 0)
      REG_BG2X = (64 - target[0]) << 8;
    else
      REG_BG2X = (64 - target[0]) << 8;
  } else if (kind == 88) {
    REG_BG2PA = 0x100;
    if (action->side == 0) {
      scrollA = 0xffc00000;
      scrollB = 0x20000;
    } else {
      scrollA = 0xffc80000;
      scrollB = 0xfffe0000;
    }
    REG_BG2X = (scrollA >> 16) << 8;
  } else if (kind == 62 || kind == 86) {
    REG_BG2PA = 0x100;
    if (action->side == 0) {
      scrollA = 0xff800000;
      scrollB = 0xc0000;
    } else {
      scrollA = 0;
      scrollB = 0xfff40000;
    }
    REG_BG2X = (scrollA >> 16) << 8;
  }
  if (kind == 38) {
    REG_BG2PA = 0x100;
    REG_BG2X = (64 - target[0]) << 8;
    work->unk7780 = 1;
    work->unk7784 = 0;
    CLEAR(BG_VRAM, 0x3c00);
    CLEAR(screen, 0x3c00);
    REG_BLDCNT = 0;
  }
  if (kind == 61) {
    REG_BG2PA = 0x100;
    if (action->side == 0)
      REG_BG2X = (32 - target[0]) << 8;
    else
      REG_BG2X = (96 - target[0]) << 8;
  }
  if (kind == 45 || kind == 47 || kind == 54 || kind == 56) {
    CLEAR(BG_VRAM, 0x3c00);
    CLEAR(screen, 0x3c00);
    action->unk1c = 0;
    Task_Remove(Func_08143489);
    Task_Remove(Func_08143001);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    Actor_SetMode(object, 3);
    if (kind == 45)
      BattleEffect_RunProjectileVolley(action, 9);
    if (kind == 54)
      BattleEffect_RunMysticFlame(action);
    if (kind == 56)
      BattleEffect_RunProjectileVolley(action, 8);
    return;
  }
  Actor_SetAnimationSpeedFar(object, 16);
  object->unk24 = save24;
  object->unk28 = save28;
  object->unk2c = save2c;
  object->unk34 = save34;
  object->unk48 = save48;
  if (kind == 65) {
    CLEAR(BG_VRAM, 0x3c00);
    CLEAR(screen, 0x3c00);
    action->unk1c = 0;
    Task_Remove(Func_08143489);
    Task_Remove(Func_08143001);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    action->unk18 = 3;
    BattleEffect_RunSparkGroups(action, 2);
    return;
  }
  CLEAR(screen, 0x3c00);
  CLEAR(BG_VRAM, 0x3c00);
  if (kind == 79) {
    for (i = 1; i != 64; i++) {
      v = 31 - ((Trig_Cos(i << 8) << 5) >> 16);
      PLTT_U16[i] = (v << 10) | (v << 5) | v;
    }
  }
  object2 = *GetBattleObjectSlotFar(action->actor);
  {
    s32 *c = center;

    c[0] = __divsi3(object2->x - origin[0], 6);
    c[1] = __divsi3(object2->y - origin[1] + 0x1e0000, 6);
    c[2] = __divsi3(object2->z - origin[2], 6);
  }
  for (i = 0; i != 64; i++)
    work->particle[i].life = 0;
  if (kind != 44) {
    half = BattleUnit_GetEffectHeightFar(action->actor) / 2;
    i = 0;
    p = work->particle;
    for (; i != 32; i++, p++) {
      p->x = object2->x;
      p->y = half;
      p->z = object2->z;
      if (kind == 61) {
        p->vx = ((Random16() & 255) - 127) << 12;
        p->vy = ((Random16() & 255) - 64) << 10;
        p->vz = ((Random16() & 255) - 127) << 12;
      } else {
        p->vx = ((Random16() & 255) - 127) << 12;
        p->vy = ((Random16() & 255) - 64) << 12;
        p->vz = ((Random16() & 255) - 127) << 12;
      }
      p->vx = p->vx / 2;
      p->vy = p->vy / 2;
      p->vz = p->vz / 2;
      p->life = i / 2 + 32;
    }
  }
  if (kind == 41) {
    Resource_LoadAndDecompress((s32)&Value_0000016f, work->frames, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000170, buffer, 1, 0);
    REG_BLDALPHA = 0xe10;
  }
  if (kind == 86) {
    Resource_LoadAndDecompress((s32)&Value_000000f1, buffer, 1, 1);
    REG_BLDALPHA = 0x810;
  }
  if (kind == 88) {
    Resource_LoadAndDecompress((s32)&Value_000000dd, buffer, 1, 1);
    REG_BLDALPHA = 0x810;
  }
  if (kind == 62) {
    Resource_LoadAndDecompress((s32)&Value_00000171, work->frames, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000172, buffer, 1, 0);
    REG_BLDALPHA = 0xe10;
  }
  if (kind == 85) {
    Resource_LoadAndDecompress((s32)&Value_000000f0, buffer, 1, 1);
    REG_BLDALPHA = 0xe10;
  }

  if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
      kind == 78 || kind == 41 || kind == 62 || kind == 49 || kind == 12 ||
      kind == 83 || kind == 84 || kind == 85 || kind == 86 || kind == 88) {
  } else if (kind == 66) {
    p = SPARKS;
    for (i = 0; i != 64; i++) {
      struct Particle *q;
      q = &SPARKS[i];
      q->x = object2->x;
      q->y = 0x140000;
      q->z = object2->z;
      if (object2->x < 0)
        q->vx = -((Random16() & 255) + 128) << 11;
      else
        q->vx = ((Random16() & 255) + 128) << 11;
      q->vy = ((Random16() & 255) - 127) << 10;
      q->vz = 0;
      q->life = 0;
    }
  } else {
    struct Particle *p;
    if (kind == 42 || (height = 0x140000, kind == 15))
      height = 0;
    p = SPARKS;
    for (i = 0; i != 64; i++, p++) {
      p->x = object2->x;
      p->y = height;
      p->z = object2->z;
      if (kind == 5 || kind == 53) {
        p->vx = ((Random16() & 255) - 127) << 11;
        p->vy = (Random16() & 255) << 11;
        p->vz = ((Random16() & 255) - 127) << 11;
      } else if (kind == 68 || kind == 69 || kind == 74 || kind == 75 ||
                 kind == 16 || kind == 97 || kind == 10) {
        p->vx = ((Random16() & 255) - 127) << 13;
        p->vy = ((Random16() & 255) - 127) << 12;
        p->vz = ((Random16() & 255) - 127) << 13;
      } else if (kind == 98) {
        p->vx = ((Random16() & 255) - 127) << 14;
        p->vy = ((Random16() & 255) - 127) << 13;
        p->vz = ((Random16() & 255) - 127) << 14;
      } else if (kind == 55) {
        p->vx = ((Random16() & 255) - 127) << 11;
        p->vy = (Random16() & 127) << 10;
        p->vz = ((Random16() & 255) - 127) << 11;
      } else {
        p->vx = ((Random16() & 255) - 127) << 10;
        p->vy = (Random16() & 127) << 10;
        p->vz = ((Random16() & 255) - 127) << 10;
      }
      p->vx = p->vx / 4;
      p->vy = p->vy / 4;
      p->vz = p->vz / 4;
      p->life = 0;
    }
  }

  if (kind == 2 || kind == 3 || kind == 42 || kind == 15 || kind == 52 ||
      kind == 72 || kind == 59 || kind == 58 || kind == 69 || kind == 89)
    Task_Add(Func_08152475, 3200);

  if (kind == 4 || kind == 5 || kind == 6 || kind == 17 || kind == 53 ||
      kind == 60 || kind == 57 || kind == 14 || kind == 63 || kind == 64 ||
      kind == 100 || kind == 83 || kind == 90 || kind == 91 || kind == 76 ||
      kind == 66 || kind == 92 || kind == 93 || kind == 96 || kind == 97)
    duration = 32;
  else if (kind == 0 || kind == 1 || kind == 2 || kind == 3 || kind == 13 ||
           kind == 38 || kind == 39 || kind == 74 || kind == 75 || kind == 40 ||
           kind == 52 || kind == 72 || kind == 69 || kind == 55 || kind == 59 ||
           kind == 61 || kind == 44 || kind == 70 || kind == 86 || kind == 88 ||
           kind == 89 || kind == 16 || kind == 10)
    duration = 48;
  else if (kind == 51 || kind == 67 || kind == 9 || kind == 11 || kind == 95 ||
           kind == 99)
    duration = 20;
  else if (kind == 41 || kind == 62 || kind == 50 || kind == 81 || kind == 84 ||
           kind == 91 || kind == 79 || kind == 98)
    duration = 40;
  else if (kind == 85)
    duration = 72;
  else if (kind == 58 || kind == 42 || kind == 15 || kind == 68)
    duration = 64;
  else if (kind == 8 || kind == 71 || kind == 80 || kind == 94)
    duration = 54;
  else
    duration = 80;

  for (frame = 0; frame != duration; frame++) {
    struct Particle *p;
    struct Particle *particle;
    if (kind != 41 && kind != 62 && kind != 83 && kind != 85 && kind != 86 &&
        kind != 88) {
      s32 *w;
      w = work->wave;
      for (i = 0; i != 160; i++)
        *w++ =
            (0x40000 - (Trig_Sin((frame << 12) + (i << 11)) << 2)) >> 10;
    }
    switch (kind) {
    case 10:
      Func_0815f0a0((s32)&Value_00000105);
      break;
    case 66:
      Func_0815f0a0((s32)&Value_00000129);
      break;
    case 69:
      Func_0815f0a0((s32)&Value_000000b9);
      break;
    case 63:
      Func_0815f0a0((s32)&Value_00000115);
      break;
    case 44:
    case 68:
      Func_0815f0a0((s32)&Value_00000130);
      break;
    case 61:
      Func_0815f0a0((s32)&Value_0000013a);
      break;
    case 38:
      Func_0815f0a0((s32)&Value_00000187);
      break;
    case 0:
    case 40:
    case 74:
      Func_0815f0a0((s32)&Value_00000150);
      break;
    case 42:
    case 43:
    case 55:
      Func_0815f0a0((s32)&Value_0000017f);
      break;
    case 48:
    case 77:
      Func_0815f0a0((s32)&Value_0000017d);
      break;
    case 49:
      Func_0815f0a0((s32)&Value_00000184);
      break;
    case 2:
    case 59:
    case 75:
    case 82:
    case 91:
      Func_0815f0a0((s32)&Value_00000167);
      break;
    case 1:
    case 13:
    case 58:
    case 89:
      Func_0815f0a0((s32)&Value_00000166);
      break;
    case 3:
    case 50:
    case 52:
    case 78:
      Func_0815f0a0((s32)&Value_00000178);
      break;
    case 39:
    case 83:
      Func_0815f0a0((s32)&Value_00000163);
      break;
    case 5:
    case 53:
      Func_0815f0a0((s32)&Value_0000013e);
      break;
    case 81:
    case 90:
    case 98:
      Func_0815f0a0((s32)&Value_00000148);
      break;
    case 73:
      Func_0815f0a0((s32)&Value_00000147);
      break;
    case 84:
      Func_0815f0a0((s32)&Value_00000169);
      break;
    }
    if (frame < 6) {
      EffectPosition_ProjectScaledBattler(action->target, base);
      base[0] = base[0] / 2;
      base[1] += 8;
    }
    if (kind != 41 && kind != 38 && kind != 62 && kind != 63 && kind != 64 &&
        kind != 79 && kind != 70 && kind != 82 && kind != 85 && kind != 86 &&
        kind != 88 && kind != 89 && kind != 94 && kind != 96 && frame < 12) {
      if (action->side == 0)
        drawFuncs.draw[0](screen, work->frames + (frame / 2) * 0xd80,
                          base[0] - 28, base[1] - 40, 48, 72);
      else
        drawFuncs.draw[0](screen, work->frames + (frame / 2) * 0xd80,
                          base[0] - 20, base[1] - 40, 48, 72);
    }
    if (kind != 41 && kind != 38 && kind != 62 && kind != 83 && kind != 85 &&
        kind != 86 && kind != 88 && kind != 94) {
      if (frame >= 4 && frame < 16)
        drawFuncs.draw[1](screen,
                          work->frames + 0x5100 + ((frame - 4) / 2) * 0x3c0,
                          target[0] / 2 - 8, base[1] - 24, 20, 48);
      Render_ResetTransformState();
      Transform_SetLookAt(bg, (u8 *)bg + 12);
      if (frame > 3) {
        if (kind == 79) {
          struct Particle *r = work->particle;
          for (i = 0; i != 64; i++) {
            r = &work->particle[i];
            if (r->life > 0) {
                s32 n;
              n = (r->life >> 2) + 1;
              if (n > 10)
                n = 10;
              EffectPosition_ProjectPoint(r, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[0](screen, (u8 *)tiles + gData_08197410[n - 1],
                                point[0] - n / 2, point[1] - n, n, n * 2);
              { s32 gravity;
              if (kind == 89) gravity = 0; else gravity = -0x800;
              BattleEffectKernels_IntegrateVector3(r, 62, gravity); }
              r->life -= 2;
            }
          }
        } else {
          for (i = 0; i != 64; i++) {
            p = &work->particle[i / 2];
            if (p->life > 0) {
                s32 n;
              n = __divsi3(p->life, 20) + 2;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[(i / 2) & 1](
                  screen, (u8 *)tiles + gData_08197410[n - 1], point[0] - n / 2,
                  point[1] - n, n, n * 2);
              BattleEffectKernels_IntegrateVector3(p, 62, -0x800);
              p->life--;
            }
          }
        }
      }
    }

    if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
        kind == 78 || kind == 49 || kind == 12 || kind == 82) {
      s32 start;
      struct BattleObject *object3;
      s32 half;
      s32 dx;
      s32 dy;
      s32 dz;
      s32 slot;

      start = 12;
      if (frame == 12)
        Sound_PlayFar(142);
      if (kind == 78)
        start = 0;
      if (frame == 50)
        Func_0814cd48(action->target, 7, -1, -1, 0);
      if (frame == 79)
        Func_0814cd48(action->target, 0, -1, -1, 0);
      if (frame == start) {
        struct Particle *p;
        p = SPARKS;
        for (i = 0; i != 64; i++, p++) {
          p->x = object2->x;
          p->y = 0x140000;
          p->z = object2->z;
          p->vx = ((Random16() & 255) - 128) << 10;
          p->vy = ((Random16() & 255) - 128) << 10;
          p->vz = ((Random16() & 255) - 128) << 10;
          if (kind == 78) {
            p->vx <<= 1;
            p->vy <<= 1;
            p->vz <<= 1;
          }
          p->life = 0;
        }
      }
      if (frame >= start) {
        p = SPARKS;
        object3 = *GetBattleObjectSlotFar(action->target);
        half = BattleUnit_GetEffectHeightFar(action->target) / 2;
        slot = 1 - (kind != 78);
        for (i = 0; i != 32; i++, p++) {
          if (p->life >= 0) {
            n = (i & 1) + 6;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] >> 1;
            drawFuncs.draw[slot](
                screen, (u8 *)tiles + gData_08197410[n - 1], point[0] - n / 2,
                point[1] - n, n, n * 2);
            BattleEffectKernels_IntegrateVector3(p, 62, 0);
            if (frame > i + 22) {
              dx = (object3->x - p->x) >> 8;
              dy = (object3->y + half - p->y) >> 8;
              dz = (object3->z - p->z) >> 8;
              p->vx += dx;
              p->vy += dy;
              p->vz += dz;
              if ((dx > -0x1000 && dx < 0x1000) && (dz > -0x1000 && dz < 0x1000))
                p->life = -1;
            }
          }
        }
      }
    }

    if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
        kind == 49 || kind == 12) {
    } else if (kind == 66) {
      struct Particle *p;
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        if (frame >= i + 4 && p->life <= 23) {
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[0](screen, buffer, point[0] - 20, point[1] - 32, 40,
                            64);
          BattleEffectKernels_IntegrateVector3(p, 64, 0);
          p->life++;
        }
      }
    } else if (kind == 51 || kind == 67 || kind == 9 || kind == 95 ||
               kind == 99) {
    } else if (kind == 6 || kind == 57 || kind == 14 || kind == 17) {
      s32 y;
      if (kind == 17) {
        if (frame == 4)
          work->unk77a8 = 12;
        if (frame == 8)
          Sound_PlayFar(221);
      }
      if (kind == 6 || kind == 57) {
        if (frame >= 6 && frame < 20) {
          for (i = 0; i != 2; i++)
            drawFuncs.draw[0](
                screen, buffer + 0xc56 + (((frame + i * 3) / 2) & 3) * 2880,
                target[0] / 2 - 8, 0, 24, 104);
        }
      } else if (kind == 17 && (frame >= 6 && frame < 20)) {
        s32 x;
        for (i = 0; i != 4; i++) {
          s32 m;
          angle = frame * 1848 + i * 0x4000;
          m = ((frame + i * 3) / 2) & 3;
          x = target[0] / 2 + ((Trig_Sin(angle) << 5) >> 16);
          Trig_Cos(angle);
          drawFuncs.draw[0](screen, buffer + 0xc56 + m * 2880, x - 8, 0, 24,
                            104);
        }
      }
      if (frame >= 8 && frame < 24) {
        s32 angle;
        s32 off;
        s32 amp;

        if (kind == 14) {
          amp = 12;
          off = 24;
        } else {
          off = 0;
          amp = 32;
        }
        for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
          m = i & 3;
          angle = Random16() & 0xffff;
          x = ((Trig_Sin(angle) << 3) >> 16) + target[0] / 2 -
              gData_08197492[m] / 2;
          y = ((Trig_Cos(angle) * amp) >> 16) + off - gData_08197498[m] / 2;
          RELEASE_BLITTERS();
          Func_08196404(188, 7, 7, gData_0819887e[Random16() & 3] | 3, 0);
          IWRAM_SLOT_BC(screen, buffer + gData_08197486[m], x, y + 56,
                        gData_08197492[m], gData_08197498[m]);
          HeapSlot_Release(188);
          BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
        }
      }
    } else if (kind == 68) {
      s32 angle;
      if (frame >= 8 && frame < 24) {
        s32 y;
        for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
          m = i & 3;
          angle = Random16() & 0xffff;
          x = ((Trig_Sin(angle) << 4) >> 16) + target[0] / 2 -
              gData_08197492[m] / 2;
          y = ((Trig_Cos(angle) << 4) >> 16) - gData_08197498[m] / 2;
          RELEASE_BLITTERS();
          Func_08196404(188, 7, 7, gData_08198882[Random16() & 3] | 3, 1);
          IWRAM_SLOT_BC(screen, buffer + gData_08197486[m], x, y + 80,
                        gData_08197492[m], gData_08197498[m]);
          HeapSlot_Release(188);
          BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
        }
      }
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        if (frame >= i + 4) {
          s32 life;
          life = p->life;
          if (life <= 23) {
            s32 m;
            m = life / 4;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[1](screen, buffer + 0x1809 + m * 0x480,
                              point[0] - 12, point[1] - 24, 24, 48);
            BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
            p->life++;
          }
        }
      }
      if (frame == 6)
        Sound_PlayFar(221);
    } else if (kind == 44) {
      s32 count;
      HeapSlot_Release(188);
      HeapSlot_Release(104);
      if (frame >= 0 && frame < 24) {
        s32 x;
        x = target[0] / 2;
        top = frame * 32 - 232;
        scroll = frame * 16 - 48;
        if (top > 0)
          top = 0;
        while (scroll > 104)
          scroll -= 104;
        BattleEffect_LoadBlitter(188, 19);
        IWRAM_SLOT_BC(screen, buffer, x - 8, top + scroll - 104, 17, 104);
        IWRAM_SLOT_BC(screen, buffer, x - 8, top + scroll, 17, 104 - scroll);
        IWRAM_SLOT_BC(screen, buffer + 0x6e8, x - 17, top + 47, 34, 65);
        HeapSlot_Release(188);
        if (frame == 8)
          work->unk77a8 = 8;
        if (frame > 1) {
            struct Particle *p;
          i = 0;
          p = work->particle;
          count = 0;
          for (; i != 64; i++, p++) {
            if (p->life == 0) {
              count++;
              p->x = object2->x;
              p->y = 0x140000;
              p->z = object2->z;
              p->vx = ((Random16() & 255) - 127) << 12;
              p->vy = ((Random16() & 255) - 64) << 10;
              p->vz = ((Random16() & 255) - 127) << 12;
              p->life = i / 2 + 32;
              if (count == 4)
                break;
            }
          }
        }
      }
      BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 61) {
      s32 x;
      HeapSlot_Release(188);
      HeapSlot_Release(104);
      if (frame >= 4 && frame < 24) {
        s32 h;
        x = target[0] / 2;
        h = 48;
        BattleEffect_LoadBlitter(188, 19);
        IWRAM_SLOT_BC(screen, buffer, x - 24, h, 24, h);
        HeapSlot_Release(188); BattleEffect_LoadBlitter(188, 23);
        IWRAM_SLOT_BC(screen, buffer, x, h, 24, h);
        HeapSlot_Release(188);
      }
      BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 60) {
      s32 m;
      if (frame > 15)
        REG_BLDALPHA = (32 - frame) | 0x1000;
      if (frame > 5) {
          s32 x;
        s32 o1;
        x = target[0] / 2;
        m = __modsi3(frame / 2, 3);
        x -= 20;
        o1 = m * 0xa00;
        drawFuncs.draw[0](screen, buffer + 0xc56 + o1, x, 16, 40,
                          32);
        drawFuncs.draw[0](screen, buffer + 0x2a56 + m * 0x500, x, 48,
                          40, 32);
        {
        u8 *o0;
        o0 = buffer + 0x1156 + o1;
        drawFuncs.draw[0](screen, o0, x, 80,
                          40, 32);
        }
      }
    } else if (kind == 5 || kind == 53 || kind == 78) {
      if (frame < 24) {
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i / 2 + 4) {
            s32 life;
            life = p->life;
            if (life <= 11) {
              s32 half;
              half = life / 2;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[0](screen, buffer + half * 0x800,
                                point[0] - 16, point[1] - 32, 32, 64);
              BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
              p->life++;
            }
          }
        }
      }
    } else if (kind == 4) {
    } else if (kind == 8) {
    } else if (kind == 41) {
      s16 *t = (s16 *)target;
      y = t[3] + ((Trig_Cos(frame << 9) << 2) >> 16) + 16;
      if (frame < 4) {
        drawFuncs.draw[0](screen, work->frames,
                          gData_0819750c[action->side * 7],
                          y + gData_0819751a[0], 57, 98);
      } else {
        if (frame < 8)
          drawFuncs.draw[0](screen, work->frames,
                            gData_0819750c[action->side * 7],
                            y + gData_0819751a[0], 57, 98);
        drawFuncs.draw[0](screen, work->frames + 0x15d2,
                          gData_0819750c[action->side * 7 + 1],
                          y + gData_0819751a[1], 99, 69);
        if (frame >= 4 && frame < 6)
          FILL(screen, 0x4000, 0x3f3f3f3f);
        if (frame >= 6 && frame < 8)
          drawFuncs.draw[0](screen, work->frames + 0x3081,
                            gData_0819750c[action->side * 7 + 2],
                            y + gData_0819751a[2], 128, 91);
        if (frame >= 8 && frame < 10)
          drawFuncs.draw[0](screen, buffer,
                            gData_0819750c[action->side * 7 + 3],
                            y + gData_0819751a[3], 128, 91);
        if (frame >= 10 && frame < 12)
          drawFuncs.draw[0](screen, buffer + 0x2d80,
                            gData_0819750c[action->side * 7 + 4],
                            y + gData_0819751a[4], 128, 59);
        if (frame >= 12 && frame < 14)
          drawFuncs.draw[0](screen, buffer + 0x4b00,
                            gData_0819750c[action->side * 7 + 5],
                            y + gData_0819751a[5], 122, 29);
        if (frame >= 14 && frame < 16)
          drawFuncs.draw[0](screen, buffer + 0x58d2,
                            gData_0819750c[action->side * 7 + 6],
                            y + gData_0819751a[6], 76, 25);
      }
    } else if (kind == 85) {
      s32 h;
      s32 y;
      REG_BLDCNT = 0;
      work->unk7780 = 1;
      work->unk7784 = 0;
      HeapSlot_Release(188);
      HeapSlot_Release(104);
      BattleEffect_LoadBlitter(104, 35);
      drawFuncs.draw[0] = IWRAM_SLOT_68;
      if (frame == 0) {
        for (i = 0; i != 1104; i++)
          work->frames[i] = Random16() & 31;
      }
      y = frame * 28 - 96;
      if (y > 16)
        y = 16;
      {
        register s32 hold __asm__("r2");
        __asm__("" : "=r"(hold));
        h = 96;
        __asm__("" : "=r"(h) : "0"(h), "r"(hold));
      }
      if (frame > 7) {
        s32 t;
        i = 0;
        t = 0;
        row = 112 - frame * 2;
        for (; i != 34; i++, t++, row++) {
          if (row >= 0 && row < 96) {
            s32 k;
            s32 threshold;
            k = 0;
            threshold = t;
            __asm__("" : "=r"(threshold) : "0"(threshold));
            for (; k != 69; k++) {
              if (work->frames[(row & 15) * 69 + k] < threshold)
                buffer[row * 69 + k] = 0;
            }
          }
        }
      }
      drawFuncs.draw[0](screen, buffer, 29, y, 69, h);
      if (frame > 5) {
        switch ((frame - 6) / 4) {
        case 0:
          drawFuncs.draw[0](screen, buffer + 0x19e0, 32, 92, 59, 24);
          break;
        case 1:
          drawFuncs.draw[0](screen, buffer + 0x1f68, 30, 87, 70, 27);
          break;
        case 2:
          drawFuncs.draw[0](screen, buffer + 0x26ca, 23, 57, 84, 55);
          break;
        case 3:
          drawFuncs.draw[0](screen, buffer + 0x38d6, 27, 55, 75, 57);
          break;
        case 4:
          drawFuncs.draw[0](screen, buffer + 0x4989, 28, 19, 67, 86);
          break;
        case 5:
          drawFuncs.draw[0](screen, buffer + 0x600b, 33, 22, 50, 65);
          break;
        case 6:
          drawFuncs.draw[0](screen, buffer + 0x6cbd, 41, 21, 48, 29);
          break;
        }
      }
      HeapSlot_Release(104);
      BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 96 || kind == 97 || kind == 98) {
      if (frame < 20) {
          s32 m;
        if (frame < 8)
          m = gData_08198886[frame / 2];
        else
          m = 3;
        HeapSlot_Release(188);
        HeapSlot_Release(104); BattleEffect_LoadBlitter(104, 3);
        IWRAM_SLOT_68(screen, buffer + m * 0x360, target[0] / 2 - 18, 56, 18,
                      48);
        RELOAD_BLITTER(104, 7);
        IWRAM_SLOT_68(screen, buffer + m * 0x360, target[0] / 2, 56, 18, 48);
        HeapSlot_Release(104);
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      }
      if ((kind == 97 || kind == 98) && frame < 24) {
        if (kind == 97)
          limit = 16;
        else
          limit = 24;
        for (i = 0; i != limit; i++) {
          struct Particle *p;
          p = &SPARKS[i];
          if (frame >= i / 2 + 4) {
            s32 life;
            life = p->life;
            if (life <= 9) {
              s32 half;
              half = life / 2;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[0](screen, buffer + 0xd80 + half * 0x800,
                                point[0] - 16, point[1] - 32, 32, 64);
              BattleEffectKernels_IntegrateVector3(p, 64, 0x1000);
              p->life++;
            }
          }
        }
      }
    } else if (kind == 86) {
      s32 m;
      work->unk7780 = 1;
      work->unk7784 = 0;
      HeapSlot_Release(188);
      HeapSlot_Release(104);
      if (action->side == 0)
        BattleEffect_LoadBlitter(104, 35);
      else
        BattleEffect_LoadBlitter(104, 39);
      drawFuncs.draw[0] = IWRAM_SLOT_68;
      scrollA += scrollB;
      if (frame > 6)
        scrollB = scrollB * 48 / 64;
      REG_BG2X = (scrollA >> 16) << 8;
      t = frame - 28;
      if (t >= 0 && t < 16)
        REG_BLDALPHA = ((t / 2 + 8) << 8) | (16 - t);
      if (frame >= 4 && frame < 8)
        FILL(screen, 0x4000, 0x01010101);
      if (frame > 3) {
        if (action->side == 0)
          drawFuncs.draw[0](screen, buffer, 32, 21, 96, 88);
        else
          drawFuncs.draw[0](screen, buffer, 0, 21, 96, 88);
      }
      m = frame / 2;
      if (m < 10) {
        if (action->side == 0)
          drawFuncs.draw[0](screen, buffer + gData_0819888a[m],
                            gData_081988c6[m], gData_081988da[m],
                            gData_0819889e[m], gData_081988b2[m]);
        else
          drawFuncs.draw[0](screen, buffer + gData_0819888a[m],
                            128 - gData_081988c6[m] - gData_0819889e[m],
                            gData_081988da[m], gData_0819889e[m],
                            gData_081988b2[m]);
      }
      HeapSlot_Release(104);
      BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 88) {
      s32 m;
      work->unk7780 = 1;
      work->unk7784 = 0;
      if (frame == 7) {
        work->unk77a8 = 16;
        Sound_PlayFar(134);
      }
      if (frame == 11)
        Sound_PlayFar(134);
      if (frame == 15)
        Sound_PlayFar(134);
      HeapSlot_Release(188);
      HeapSlot_Release(104);
      if (action->side == 0)
        BattleEffect_LoadBlitter(104, 35);
      else
        BattleEffect_LoadBlitter(104, 39);
      drawFuncs.draw[0] = IWRAM_SLOT_68;
      scrollA += scrollB;
      if (frame == 5)
        scrollA += 0x200000;
      if (frame > 6)
        scrollB = scrollB * 48 / 64;
      REG_BG2X = (scrollA >> 16) << 8;
      t = frame - 29;
      if (t >= 0 && t < 16)
        REG_BLDALPHA = ((t / 2 + 8) << 8) | (16 - t);
      if (frame > 4)
        drawFuncs.draw[0](screen, buffer, 35, 17, 60, 95);
      if (frame == 5)
        FILL(screen, 0x4000, 0x3f3f3f3f);
      if (frame >= 0 && (m = __divsi3(frame, 3)) < 7) {
        if (action->side == 0)
          drawFuncs.draw[0](screen, buffer + gData_081988ee[m],
                            gData_08198918[m], gData_08198926[m] - 16,
                            gData_081988fc[m], gData_0819890a[m]);
        else
          drawFuncs.draw[0](screen, buffer + gData_081988ee[m],
                            128 - gData_08198918[m] - gData_081988fc[m],
                            gData_08198926[m] - 16, gData_081988fc[m],
                            gData_0819890a[m]);
      }
      for (i = 0; i != 3; i++) {
        s32 m;
        t = frame - 8 - i * 2;
        if (t >= 0 && t <= 8) {
          m = __divsi3(t, 3);
          if (action->side == 0)
            drawFuncs.draw[0](screen, buffer + gData_08198934[m],
                              gData_08198940[i][0] - gData_0819893a[m] / 2,
                              gData_08198940[i][1] - gData_0819893d[m] / 2 - 16,
                              gData_0819893a[m], gData_0819893d[m]);
          else
            drawFuncs.draw[0](screen, buffer + gData_08198934[m],
                              128 - gData_08198940[i][0] -
                                  gData_0819893a[m] / 2,
                              gData_08198940[i][1] - gData_0819893d[m] / 2 - 16,
                              gData_0819893a[m], gData_0819893d[m]);
        }
      }
      HeapSlot_Release(104);
      BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 62) {
      s32 t;
      scrollA += scrollB;
      if (frame > 6)
        scrollB = scrollB * 48 / 64;
      REG_BG2X = (scrollA >> 16) << 8;
      if (frame == 0)
        REG_BLDALPHA = 0x810;
      t = frame - 24;
      if (t >= 0 && t < 16)
        REG_BLDALPHA = (16 - t) | 0x1000;
      if (frame >= 4 && frame < 6)
        FILL(screen, 0x4000, 0x3f3f3f3f);
      if (frame < 4) {
        if (action->side == 1)
          drawFuncs.draw[0](screen, work->frames, 0, 24, 80, 104);
        else
          drawFuncs.draw[0](screen, work->frames, 48, 24, 80, 104);
      } else {
        if (frame < 8) {
          if (action->side == 1)
            drawFuncs.draw[0](screen, work->frames, 0, 24, 80, 104);
          else
            drawFuncs.draw[0](screen, work->frames, 48, 24, 80, 104);
        }
        if (action->side == 1)
          drawFuncs.draw[0](screen, work->frames + 0x1e00, 16, 16, 80, 104);
        else
          drawFuncs.draw[0](screen, work->frames + 0x1e00, 32, 16, 80, 104);
        if (frame >= 6 && frame < 8)
          drawFuncs.draw[0](screen, work->frames + 0x3e80, 0, 16, 128, 91);
        if (frame >= 8 && frame < 10)
          drawFuncs.draw[0](screen, buffer, 0, 16, 128, 91);
        if (frame >= 10 && frame < 12)
          drawFuncs.draw[0](screen, buffer + 0x2d80, 0, 16, 128, 59);
        if (frame >= 12 && frame < 14)
          drawFuncs.draw[0](screen, buffer + 0x4b00, 0, 16, 128, 29);
        if (frame >= 14 && frame < 16)
          drawFuncs.draw[0](screen, buffer + 0x5980, 0, 16, 128, 26);
      }
    } else if (kind == 50) {
      s32 m;
      for (i = 0; i != 12; i++) {
        if (frame >= i + 6 && frame < i + 18) {
          s32 x;
          m = (frame - i - 6) / 2;
          x = target[0] / 2 - gData_08197467[m] / 2;
          if (i & 1)
            x += (i + 1) / 2 * 3;
          else
            x -= (i + 1) / 2 * 3;
          {
            s32 which;
            which = 1;
            if (i != 0)
              which = ((i - 1) & 3) > 1;
            drawFuncs.draw[which](
              screen, buffer + gData_0819747a[m], x, gData_08197473[m] + 48,
              gData_08197467[m], gData_0819746d[m]);
          }
        }
      }
    } else if (kind == 46) {
      if (frame == 0) {
        struct Particle *p;
        p = SPARKS;
        for (i = 0; i != 64; i++, p++) {
          p->x = (Random16() & 127) + 32;
          p->y = 0;
          p->z = 0;
          p->vx = Random16() & 0xffff;
          p->vy = Random16() & 0xffff;
          p->vz = Random16() & 0xffff;
        }
        SPARKS[63].y = 159;
      }
      p = SPARKS;
      for (i = 0; i != 64; i++, p++) {
        if (p->x >= 0 && frame >= i / 2) {
            s32 m;
          m = i & 3;
          Render_ResetTransformState();
          SceneTransform_ApplyPitch(p->vx);
          SceneTransform_ApplyYaw(p->vy);
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2 + target[0] / 2;
          point[1] = point[1] + target[1] + 32;
          drawFuncs.draw[1](screen, buffer + gData_08198946[m], point[0] - 4,
                            point[1] - 4, 8, 8);
          p->x -= 6;
          if (p->x < 0 && ((i & 7) == 0 || i == 63)) {
            Sound_PlayFar(133);
            Func_0814cd48(action->actor, 7, 5, 0, 4);
          }
        }
      }
    } else if (kind == 38) {
      if (frame >= 5 && frame < 50) {
        if (frame > 25)
          h = 196 - frame * 4;
        else
          h = frame * 16 - 64;
        if (h > 96)
          h = 96;
        drawFuncs.draw[0](screen, buffer, 48, 104 - h, 32, h);
      }
    } else if (kind == 79) {
      struct Particle *p;
      p = SPARKS;
      if (frame == 0) {
        p->x = (target[0] / 2) << 16;
        p->y = (target[1] + 48) << 16;
        if (action->side == 0)
          p->vx = -0x30000;
        else
          SPARKS->vx = 0x30000;
        p->vy = 0x80000;
        p->x += -p->vx * 12;
        p->y += -p->vy * 12;
      }
      if (frame <= 150) {
        REG_BLDCNT = 0;
        work->unk7780 = 1;
        work->unk7784 = 0;
      } else {
        work->unk7780 = 2;
        work->unk7784 = 50;
        REG_BLDCNT = 0x3f44;
      }
      if (frame < 32) {
        drawFuncs.draw[1](screen, buffer + 0x800, (p->x >> 16) - 24,
                          (p->y >> 16) - 96, 48, 96);
        if (frame < 12)
          BattleEffectKernels_IntegrateVector2(SPARKS, 64, 0);
        else if (frame == 12)
          p->vy = __divsi3(-p->vy, 3);
        else if (frame < 13 || frame > 18)
          BattleEffectKernels_IntegrateVector2(SPARKS, 64, 0x4000);
      }
      if (frame == 11) {
        Sound_PlayFar(144);
        work->unk77a8 = 16;
      }
    } else if (kind == 63 || kind == 64) {
      s32 x;
      s32 y;
      if (frame < 6) {
        if (action->side == 0) {
          x = target[0] / 2 + (6 - frame) * 3 * 2;
          y = target[1] - (6 - frame) * 3 * 4 + 24;
        } else {
          x = target[0] / 2 - (6 - frame) * 3 * 2;
          y = target[1] - (6 - frame) * 3 * 4 + 24;
        }
        drawFuncs.draw[1](screen, buffer, x - 16, y - 32, 32, 64);
      }
    } else if (kind == 70) {
      s32 x;
      s32 y;
      if (frame > 31)
        REG_BLDALPHA = (48 - frame) | 0x1000;
      if (frame < 14) {
        struct Particle *p;
        if (action->side == 0) {
          x = target[0] / 2 + (14 - frame) * 3 * 2;
          y = target[1] - (14 - frame) * 3 * 4 + 24;
        } else {
          x = target[0] / 2 - (14 - frame) * 3 * 2;
          y = target[1] - (14 - frame) * 3 * 4 + 24;
        }
        drawFuncs.draw[1](screen, buffer, x - 16, y - 32, 32, 64);
        if (frame == 13) {
          Sound_PlayFar(134);
          work->unk77a8 = 8;
          BattleEffect_StartActorMotion(action->actor, 1, 0x20000, 0x80000, 0x30000,
                                    150);
          p = SPARKS;
          table = gData_081977f8;
          for (i = 0; i != 16; i++, p++) {
            p->x = (*table++ + x - 40) << 16;
            p->y = *table++ << 16;
            p->vx = ((Random16() & 127) - 64) << 11;
            p->vy = -(Random16() & 127) << 11;
            p->z = 32;
            p->life = 0;
          }
        }
      } else {
        for (i = 0; i != 16; i++) {
            s32 m;
          particle = &SPARKS[i];
          { s32 a; a = __modsi3(i, 5) * 3;
          m = a + __modsi3(__divsi3(particle->life, 96), 3); }
          drawFuncs.draw[i > 2 ? 0 : 1](screen, buffer + 0x800 + gData_08197834[m],
                                 (particle->x >> 16) - gData_0819781a[m] / 2,
                                 (particle->y >> 16) - gData_08197826[m] / 2,
                                 gData_0819781a[m], gData_08197826[m]);
          BattleEffectKernels_IntegrateVector2(particle, 64, 0x2000);
          particle->life += particle->z;
          if (particle->z > 1 && (frame & 1))
            particle->z--;
        }
      }
    } else if (kind == 94) {
      s32 x;
      s32 y;
      if (frame == 0) {
        struct Particle *p;
        Resource_LoadAndDecompress((s32)&Value_000000e6, buffer + 0x2000, 1,
                                   1);
        REG_BLDALPHA = 0x410;
        work->unk7780 = 1;
        work->unk7784 = 0;
        REG_BG2PA = 0x100;
        p = work->particle;
        for (i = 0; i != 32; i++, p++) {
          p->x = ((Random16() & 31) + 48) << 16;
          p->y = ((Random16() & 31) + 64) << 16;
          p->z = Random16();
          p->vx = ((Random16() & 63) - 32) << 12;
          p->vy = -((Random16() & 127) + 16) << 12;
          p->vz = (Random16() & 15) + 16;
          p->life = 0;
        }
      }
      if (frame == 14) {
        BattleEffect_StartActorMotion(action->actor, 1, 0x20000, 0x80000, 0x30000,
                                  150);
        Sound_PlayFar(134);
        work->unk77a8 = 8;
      }
      i = 0;
      particle = work->particle;
      if (frame < 14) {
        if (action->side == 0) {
          x = target[0] + (14 - frame) * 3 * 2;
          y = target[1] - (14 - frame) * 3 * 4 + 24;
        } else {
          x = target[0] - (14 - frame) * 3 * 2;
          y = target[1] - (14 - frame) * 3 * 4 + 24;
        }
        drawFuncs.draw[0](screen, buffer + 0x2000, 32, y - 32, 64, 64);
        REG_BG2X = (64 - x) << 8;
      } else {
        for (; i != 32; i++, particle++) {
            s32 m;
          s32 w;
          s32 h;
          s32 left;
          s32 top;
          m = __modsi3(i, 5) * 3;
          m += __modsi3(__divsi3(particle->z, 96), 3);
          w = gData_0819749e[m];
          left = (particle->x >> 16) - w / 2;
          top = particle->y >> 16;
          h = gData_081974ad[m];
          top -= h / 2;
          drawFuncs.draw[0](screen, buffer + 0x83c + gData_081974bc[m],
                            left, top, w, h);
          BattleEffectKernels_IntegrateVector2(particle, 63, 0x8000);
          particle->z += particle->vz;
          if ((frame & 1) && particle->vz > 0)
            particle->vz--;
        }
      }
    } else if (kind == 84) {
      s32 fade;
      s32 angle;
      if (frame == 0) {
        struct Particle *p;
        p = RINGS;
        for (i = 0; i != 32; i++, p++) {
          p->z = Random16();
          p->vz = (Random16() & 255) + 128;
          if (Random16() & 1)
            p->vz = -p->vz;
          p->life = Random16() & 3;
        }
      }
      if (frame == 4)
        work->unk77a8 = 8;
      if (frame == 6)
        Sound_PlayFar(221);
      if (frame == 24)
        Sound_PlayFar(390);
      if (frame >= 8 && frame < 24) {
        s32 y;
        for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
          m = i & 3;
          angle = Random16() & 0xffff;
          x = ((Trig_Sin(angle) << 4) >> 16) + target[0] / 2 -
              gData_08197492[m] / 2;
          y = ((Trig_Cos(angle) << 4) >> 16) - gData_08197498[m] / 2;
          RELEASE_BLITTERS();
          Func_08196404(188, 7, 7, gData_0819894e[Random16() & 3] | 3, 1);
          IWRAM_SLOT_BC(screen, buffer + gData_08197486[m], x, y + 80,
                        gData_08197492[m], gData_08197498[m]);
          HeapSlot_Release(188);
          BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
        }
      }
      if (frame > 3) {
        struct Particle *p;
        struct Model *model;
        struct Texture *tex;
        matrix = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        texture164.width = 5;
        texture164.height = 7;
        model->kind = 6;
        tex = &texture164;
        model->mesh = gData_08199268;
        model->matrix = matrix;
        model->texture = tex;
        for (i = 0; i != 20; i++) {
          struct Particle *p;
          p = &RINGS[i];
          if (frame >= i && frame < i + 8) {
            fade = 0;
            if (frame == i)
              fade -= 16;
            else if (frame >= i + 4)
              fade = -((frame - i - 4) * 16);
            model->fade = fade;
            {
              s32 life = p->life;
              texture164.pixels = buffer + 0xc56 + (life << 12);
              p->life = (life + 1) & 3;
            }
            Render_ResetTransformState();
            Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                          0);
            Func_080150e4(p->z);
            p->z += p->vz;
            Func_080151e4(0x40000, 0x10000, 0x10000);
            Render_TransformVertices(gData_081991b0, matrix, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
      }
    } else if (kind == 71 || kind == 80 || kind == 82) {
      void *matrix;
      struct Model *model;
      s32 fade;
      matrix = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      fade = 0;
      if (frame > 39)
        fade = (40 - frame) * 4;
      else
        work->unk77a8 = 1;
      texture156.width = 6;
      texture156.height = 6;
      texture156.pixels = buffer;
      model->kind = 7;
      model->texture = &texture156;
      model->mesh = gData_08199340;
      model->matrix = matrix;
      model->fade = fade;
      Render_ResetTransformState();
      Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16, 0);
      Func_080151e4(0x8000, 0x10000, 0x8000);
      if (frame == 8)
        Sound_PlayFar(104);
      if (fade >= -63) {
        fade = frame << 12;
        if (kind == 82) {
          SceneTransform_ApplyPitch(-0x800);
          SceneTransform_ApplyYaw(-frame << 11);
          SceneTransform_ApplyPitch(-frame << 4);
        } else {
          SceneTransform_ApplyPitch(-0x1000);
          SceneTransform_ApplyYaw(frame << 11);
        }
        Func_0801521c(fade);
        Render_TransformVertices(gData_08199210, matrix, 4);
        Render_DrawModelList(model);
        if (kind != 82) {
          SceneTransform_ApplyYaw(-frame << 12);
          Func_0801521c(0x8000);
          Render_TransformVertices(gData_08199210, matrix, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(matrix);
      if (kind == 82) {
          s32 m;
          s32 two;
        m = frame / 4;
        two = m * 2;
        fade = 0x10000;
        if (frame > 39)
          fade = 0x38000 - (frame << 12);
        if (two > 1 && fade > 0) {
          Func_0815b434(work->frames, m * 4, fade);
          HeapSlot_Release(188);
          Func_0818caa8(work->frames, target[0] / 2, target[1], m * 4);
          BattleEffect_LoadBlitter(188, 15);
        }
      }
    } else if (kind == 81 || kind == 83 || kind == 92 || kind == 93) {
      void *matrix;
      s32 t;
      matrix = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      texture148.width = 7;
      texture148.height = 7;
      texture148.pixels = buffer;
      model->texture = &texture148;
      model->kind = 7;
      model->mesh = gData_08199364;
      model->matrix = matrix;
      if (kind == 92 || kind == 93) {
        s32 fade;
        for (i = 0; i != 3; i++) {
          t = frame - i * 2;
          if (t >= 0) {
            angle = t * 3 << 12;
            fade = 0;
            if (t > 7)
              fade = (8 - t) * 8;
            model->fade = fade;
            if (fade > -64) {
              Render_ResetTransformState();
              Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                            0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              Func_080150e4((i << 14) + 0x2000);
              SceneTransform_ApplyPitch(-0x1000);
              Func_0801521c(angle >> 1);
              Render_TransformVertices(gData_08199210, matrix, 4);
              Render_DrawModelList(model);
            }
          }
        }
      } else if (kind == 83) {
        s32 fade;
        s32 angle;
        t = frame - 4;
        if (t >= 0) {
          angle = t << 13;
          fade = 0;
          if (t > 7)
            fade = (8 - t) * 8;
          model->fade = fade;
          if (fade >= -63) {
            Render_ResetTransformState();
            Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                          0);
            Func_080151e4(0x8000, 0x10000, 0x8000);
            SceneTransform_ApplyPitch(0x4000);
            Func_0801521c(angle);
            Render_TransformVertices(gData_08199210, matrix, 4);
            Render_DrawModelList(model);
          }
        }
      } else {
        for (i = 0; i != 3; i++) {
          t = frame - i * 4;
          if (t >= 0) {
              s32 fade;
            fade = 0;
            if (t > 7)
              fade = (8 - t) * 8;
            model->fade = fade;
            if (fade > -64) {
              Render_ResetTransformState();
              Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                            0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              Func_080150e4((i << 14) + 0x2000);
              SceneTransform_ApplyPitch(-0x1000);
              Func_0801521c(t * 3 << 12);
              Render_TransformVertices(gData_08199210, matrix, 4);
              Render_DrawModelList(model);
            }
          }
        }
      }
      Heap_Release(model);
      Heap_Release(matrix);
      if (kind != 92 && kind != 93) {
        s32 x;
        if (frame == 4) {
          struct Particle *p;
          for (i = 0; i != 64; i++) {
            struct Particle *p;
            p = &SPARKS[i];
            p->x = Random16();
            p->y = 0;
            p->vy = ((Random16() & 3) + 3) << 17;
          }
        }
        if (frame > 4) {
          for (i = 0; i != 64; i++) {
            struct Particle *p;
            s32 k;
            p = &SPARKS[i];
            for (k = 0; k != 12; k++) {
              r = (p->y >> 16) - k * 2;
              if (r >= 0) {
                  s32 n;
                x = target[0] / 2 + ((Trig_Sin(p->x) * r) >> 16);
                y = target[1] + ((Trig_Cos(p->x) * r) >> 16);
                n = 2 - k / 8;
                drawFuncs.draw[0](screen, (u8 *)tiles + gData_08197410[n - 1],
                                  x - n / 2, y - n, n, n * 2);
              }
            }
            p->y += p->vy;
            p->vy = p->vy * 62 / 64;
          }
        }
      }
    } else if (kind == 91) {
      struct Texture *tex;
      void *matrix;
      s32 t;
      matrix = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      texture140.width = 7;
      texture140.height = 7;
      tex = &texture140;
      tex->pixels = buffer;
      model->kind = 7;
      model->texture = tex;
      model->mesh = gData_08199364;
      model->matrix = matrix;
      t = frame - 4;
      if (t >= 0) {
          s32 fade;
        angle = t << 14;
        fade = 0;
        if (t > 7)
          fade = (8 - t) * 8;
        model->fade = fade;
        if (fade >= -63) {
          Render_ResetTransformState();
          Func_08015160((target[0] / 2 - 64) << 16, 0x280000, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          SceneTransform_ApplyPitch(1000);
          Func_0801521c(angle);
          Render_TransformVertices(gData_08199210, matrix, 4);
          Render_DrawModelList(model);
        }
      }
      texture140.width = 7;
      texture140.height = 4;
      texture140.pixels = buffer + 0x4000;
      model->mesh = gData_08198d2c;
      for (i = 0; i != 2; i++) {
        s32 start;
        start = gData_08198952[i] + 4;
        if (frame > start) {
          fade = (start - frame) * 8;
          if (fade > -16)
            fade = -16;
          if (fade > -64) {
            scale = (frame - start) * gData_08198956[i] * 1500 + 0x4180;
            model->fade = fade;
            Render_ResetTransformState();
            Func_08015160((target[0] / 2 - 60) << 16, gData_0819895a[i] << 16,
                          0);
            Func_080151e4(scale * 2, scale * 4, scale / 4);
            SceneTransform_ApplyPitch(1000);
            if (i & 1)
              SceneTransform_ApplyYaw((i * 2 + frame) << 12);
            else
              SceneTransform_ApplyYaw((i * 2 - frame) << 12);
            Render_TransformVertices(gData_08198c6c, matrix, 16);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(matrix);
      if (frame == 4)
        work->unk77a8 = 8;
    } else if (kind == 90) {
      struct Texture *tex;
      s32 scale;
      matrix = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      tex = &texture132;
      tex->width = 6;
      tex->height = 6;
      tex->pixels = buffer;
      model->kind = 7;
      model->matrix = matrix;
      model->texture = tex;
      model->mesh = gData_081990d0;
      for (i = 0; i != 4; i++) {
        start = gData_0819895e[i] + 4;
        model->frame = (frame * 8 + i * 32) & 127;
        if (frame == start + 4)
          Sound_PlayFar(104);
        if (frame > start) {
          fade = (start - frame) * 8 + 56;
          if (fade > -16)
            fade = -16;
          if (fade > -64) {
            scale = (frame - start) * gData_08198962[i] * 2000 + 0x4000;
            model->fade = fade;
            Render_ResetTransformState();
            Func_08015160((target[0] / 2 - 60) << 16, 0x280000, 0);
            Func_080151e4(scale, gData_08198966[i] * scale * 2, scale / 4);
            SceneTransform_ApplyPitch(1000);
            if (i & 1)
              SceneTransform_ApplyYaw((i * 8 + frame) << 10);
            else
              SceneTransform_ApplyYaw((i * 8 - frame) << 10);
            Render_TransformVertices(gData_08199090, matrix, 32);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(matrix);
      if (frame == 4)
        work->unk77a8 = 20;
    } else if (kind == 15) {
      if (frame == 0) {
        struct Particle *p;
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          p->life = -i * 2;
          p->x = Random16();
          p->vx = (Random16() & 511) + 512;
          p->z = i * 2 + 80;
        }
      }
      for (i = 0; i != 0x400; i++) {
        if (buffer[0x400 + i] >> 1 == frame / 2)
          buffer[0x800 + i] = buffer[i];
      }
      EffectPosition_ProjectScaledBattler(action->actor, point2);
      point2[0] = point2[0] / 2;
      for (i = 0; i != 16; i++) {
        struct Particle *p;
        p = &SPARKS[i];
        if (p->life >= 0 && p->z > 0) {
          s32 m;
          s32 x;
          s32 y;
          m = __modsi3(i, 3);
          x = point2[0] + ((Trig_Sin(p->x) * p->z) >> 16);
          y = point2[1] + ((Trig_Cos(p->x) * p->z) >> 16);
          p->z -= 4;
          p->x += p->vx;
          if (kind == 16)
            drawFuncs.draw[0](screen, buffer, x - 16, y - 16, 32, 32);
          else
            drawFuncs.draw[0](screen, buffer + m * 0x240, x - 12, y - 12, 24,
                              24);
        }
        p->life++;
      }
    } else if (kind == 16) {
      p = SPARKS;
      for (i = 0; i != 32; i++, p++) {
        if (frame >= i / 2 + 4 && p->life <= 23) {
            s32 m;
          m = 0;
          if (p->life > 11)
            m = (p->life - 12) / 2;
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[1](screen, buffer + m * 0x400, point[0] - 16,
                            point[1] - 16, 32, 32);
          BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
          BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
          p->life++;
        }
      }
      if (frame == 4)
        work->unk77a8 = 12;
      if (frame == 8)
        Sound_PlayFar(103);
    } else if (kind == 10) {
      if (frame == 0) {
        work->unk7780 = 1;
        work->unk7784 = 0;
        REG_BLDALPHA = 0x610;
      }
      p = SPARKS;
      for (i = 0; i != 24; i++, p++) {
        if (frame >= i / 2 + 4 && p->life <= 23) {
            s32 m;
          m = 0;
          if (p->life > 11)
            m = __divsi3(p->life - 12, 3);
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[0](screen, buffer + m * 0x800, point[0] - 16,
                            point[1] - 32, 32, 64);
          BattleEffectKernels_IntegrateVector3(p, 58, 0x1000);
          p->life++;
        }
      }
      if (frame == 4)
        work->unk77a8 = 12;
      if (frame == 8)
        Sound_PlayFar(103);
    } else if (kind == 42) {
      s32 m;
      if (frame > 47)
        REG_BLDALPHA = (64 - frame) | 0x1000;
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        s32 three;
        three = 3;
        m = __modsi3(i, three);
        EffectPosition_ProjectPoint(p, point);
        point[0] = point[0] / 2;
        drawFuncs.draw[i & 1](screen, buffer + m * 0x240, point[0] - 12,
                              point[1] - 12, 24, 24);
        BattleEffectKernels_IntegrateVector3(p, 60, 1 << ((i & three) + 11));
        p->life++;
      }
    } else if (kind == 76) {
      if (frame == 18)
        Sound_PlayFar(390);
      for (i = 0; i != 3; i++) {
        if (frame == i * 3 + 3) {
          Sound_PlayFar(212);
          FILL(screen, 0x4000, 0x2f2f2f2f);
        }
      }
    } else if (kind == 74) {
      if (frame == 8)
        Sound_PlayFar(103);
      p = SPARKS;
      for (i = 0; i != 32; i++, p++) {
        if (frame >= i / 2 + 4) {
            s32 life;
          life = p->life;
          if (life <= 23) {
              s32 m;
            m = life / 4;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[1](screen, buffer + m * 0x480,
                              point[0] - 12, point[1] - 24, 24, 48);
            BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
            BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
            p->life++;
          }
        }
      }
    } else if (kind != 100) {
      s32 m;
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        if (frame >= i + 4) {
          s32 life;
          life = p->life;
          if (life <= 23) {
            m = life / 4;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            if (kind == 69 || kind == 74)
              drawFuncs.draw[1](screen, buffer + m * 0x480,
                                point[0] - 12, point[1] - 24, 24, 48);
            else
              drawFuncs.draw[i & 1](screen, buffer + m * 0x480,
                                    point[0] - 12, point[1] - 24, 24, 48);
            if (kind == 55)
              BattleEffectKernels_IntegrateVector3(p, 60, 0x400);
            else
              BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
            p->life++;
          }
        }
      }
    }

    if (kind == 8 && frame < 44) {
      s32 half;
      EffectPosition_ProjectPoint(origin, point);
      point[0] = point[0] / 2;
      drawFuncs.draw[1](screen, buffer + 0x3c56, point[0] - 10, point[1] - 4,
                        20, 40);
      if (frame == 23)
        (*GetBattleObjectSlotFar(action->target))->unk28 = 0x30000;
      if (frame > 19)
        origin[1] += 0x40000;
      if (frame == 23) {
        struct Particle *p;
        half = BattleUnit_GetEffectHeightFar(action->actor) / 2 + 0x1e0000;
        p = work->particle;
        for (i = 0; i != 64; i++, p++) {
          p->x = object2->x;
          p->y = half;
          p->z = object2->z;
          p->vx = ((Random16() & 255) - 127) << 11;
          p->vy = ((Random16() & 255) - 64) << 11;
          p->vz = ((Random16() & 255) - 127) << 11;
          p->life = i / 2 + 32;
        }
      }
      if (frame == 24) {
        BattleEffect_StartActorMotion(action->actor, 1, 0, 0, 0x90000, 100);
        Func_0814cd48(action->actor, 7, 5, 0, 4);
        Func_081180e8(134);
        work->unk77a8 = 8;
      }
      if (frame >= 24 && frame < 36)
        drawFuncs.draw[1](screen,
                          work->frames + 0x5100 + ((frame - 24) / 2) * 0x3c0,
                          target[0] / 2 - 8, 28, 20, 48);
    }
    if (kind == 66) {
      for (i = 0; i != 4; i++) {
        if (frame == i * 4 + 8) {
          Func_0814cd48(action->actor, 7, 5, 0, 2);
          work->unk77a8 = 2;
          Sound_PlayFar(133);
        }
      }
    }
    if (kind <= 37 && frame <= 5) {
      EffectPosition_ProjectPoint(origin, point);
      point[0] = point[0] / 2;
      drawFuncs.draw[1](screen, buffer + 0x3c56, point[0] - 10, point[1] - 4,
                        20, 40);
      origin[0] += center[0];
      origin[1] += center[1];
      origin[2] += center[2];
    }
    if (kind != 8 && frame == 3)
      Func_081180e8(-1);
    if (frame == 4) {
      if (kind == 91)
        Sound_PlayFar(145);
      else if (kind == 78 || kind == 81 || kind == 98)
        Sound_PlayFar(144);
      else
        Sound_PlayFar(134);
    }
    if (frame == 6) {
      if (kind == 4 || kind == 5 || kind == 7 || kind == 43 || kind == 48 ||
          kind == 73 || kind == 77 || kind == 78 || kind == 49 || kind == 12 ||
          kind == 53 || kind == 64 || kind == 100 || kind == 66 || kind == 81 ||
          kind == 83 || kind == 76 || kind == 98) {
        Func_08118088(action->actor, 4);
        work->unk77a8 = 12;
      } else if (kind == 8 || kind == 79 || kind == 85) {
        work->unk77a8 = 8;
      } else if (kind == 50 || kind == 44 || kind == 63) {
        Func_08118088(action->actor, 1);
        work->unk77a8 = 2;
      } else if (kind == 60 || kind == 38) {
        Func_08118088(action->actor, 3);
        work->unk77a8 = 16;
      } else if (kind == 95 || kind == 99 || kind == 96 || kind == 97 ||
                 kind == 92 || kind == 93 || kind == 86 || kind == 74) {
        Func_08118088(action->actor, 0);
        work->unk77a8 = 12;
      }
    }
    if (frame == 6)
      Func_0814cd48(action->actor, 7, 5, 0, 4);
    if (frame == 14)
      Func_0814cd48(action->actor, 7, 5, 0, 4);
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->unk78e8 = 1;
    wait(1);
  }

  if (kind == 51 || kind == 67 || kind == 9 || kind == 10 || kind == 11 ||
      kind == 70 || kind == 79 || kind == 90) {
    CLEAR(BG_VRAM, 0x3c00);
    CLEAR(screen, 0x3c00);
    action->unk1c = 0;
    Task_Remove(Func_08143489);
    Task_Remove(Func_08143001);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    if (kind == 51 || kind == 10 || kind == 70 || kind == 79 || kind == 90)
      SummonEffect_RunCanvasMode1(action);
    else if (kind == 9 || kind == 67)
      SummonEffect_RunCanvasMode2(action);
    else if (kind == 11)
      SummonEffect_RunCanvasMode3(action);
    return;
  }
  if (kind == 2 || kind == 3 || kind == 42 || kind == 15 || kind == 52 ||
      kind == 72 || kind == 58 || kind == 59 || kind == 69 || kind == 89)
    Task_Remove(Func_08152475);
  Task_Remove(Func_08143001);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
  }
}
