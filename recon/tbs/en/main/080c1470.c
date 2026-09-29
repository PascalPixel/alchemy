/* 2026-09-29: eight minutes of permutation: 3327 -> 2747; kept without its
 * register temporaries for the fill offset and the palette address, 2882
 * (11 register-only, 2 stack-only, 16 operand, 8 reordered, 7 inserted, 13
 * deleted): the spoke loop rotated into a tested do/while that clears the
 * state before stepping the angle, and a few operand orders. Resource
 * numbers 0xc8..0xcb are still Value_ symbols. */
/* Whole owner [080c1470, 080c16d0), 608 bytes including its pool.
   2026-09-26 H2: give the first loop its own angle/distance/speed locals.
   Candidate 596/608, 205 differing halfwords, 97 aligned edits. The complete
   first-loop body now has the reference r5 angle, r6 radius, r7 star cursor,
   r8 distance, r9 count and sl multiply target. Its preheader still orders
   the cursor/target initializers differently. Remaining: 8/36-byte frame,
   ring-radius rematerialization, work-slot access, resource cursor lifetime,
   affine accesses and result convention. The shared-angle model was wrong.
   2026-09-26 H1: typed BG2 affine register members replace scalar casts.
   Candidate 596/608, 226 differing halfwords, 118 aligned edits (baseline
   604/608, 228 halfwords, 173 edits). The 0x100 value is now constructed
   and all pools move to the end as required. This compiler emits an extra
   halfword read before each volatile aggregate-member store; the register
   cursor uses member offsets rather than the ROM's incremented addresses.
   The 8/36-byte frame and phase-wide register differences remain. No credit.
   Earlier draft (2026-09-24): candidate=604 reference=608, 228 differing
   halfwords. Hand-written from the assembly. Residual: the reference frame is
   36 bytes (28 bytes of locals this candidate does not have), the star loop
   swaps r9 and sl between the counter and the MulQ16 routine, the spoke loop
   holds the 0x1000 factor in r8, and the gWorkSlot[40] canvas pointer is
   reached as the slot base plus 160. */
/* Battle effect: set up a field of sixteen stars flying out from the centre
   and three rotating spokes, load the palette the mode selects and start the
   two update callbacks. */
#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "CALLBACK_SCHEDULER.H"

struct BgAffineRegisters {
    u16 pa;
    u16 pb;
    u16 pc;
    u16 pd;
    s32 x;
    s32 y;
};

struct Star {
    s32 x;
    s32 y;
    s32 z;
    s32 dx;
    s32 dy;
    s32 age;
    s32 speed;
};

struct Spoke {
    s32 x;
    s32 y;
    s32 inner_x;
    s32 inner_y;
    s32 state;
};

struct StarFieldWork {
    u8 padding0000[0x11c0];
    struct Star stars[16];
    struct Spoke spokes[3];
    s32 unknown_13bc;
    s32 unknown_13c0;
    u8 padding13c4[8];
    s32 unknown_13cc;
};

typedef s32 (*FillWordsFn)(void *destination, s32 size);
typedef s32 (*WordCopyFn)(void *destination, const void *source, s32 size);

extern u8 gWorkSlot[];
extern s32 *gTransitionWork;
extern u8 Value_000000c8[];
extern u8 Value_000000c9[];
extern u8 Value_000000ca[];
extern u8 Value_000000cb[];

void *Runtime_AllocateBlock(s32 slot, s32 size);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
u8 *Resource_GetTableEntry(s32 index);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Func_080c9000(s32, s32, s32, s32, s32);

void BattleFx_InitializeStarField(s32 mode)
{
    struct StarFieldWork *work;
    struct Star *star;
    struct Spoke *spoke;
    s32 i;
    s32 angle;
    s32 palette;
    u8 *data;
    volatile struct BgAffineRegisters *bg;


    gTransitionWork[2] = 1;
    work = Runtime_AllocateBlock(39, 0x13d0);
    Runtime_AllocateHeapBlock(40, 0x4000);
    star = work->stars;
    for (i = 15; i >= 0; i--) {
        s32 angle;
        u32 distance;
        s32 speed;
        angle = Random16();
        distance = Random16() + 0x10000;
        speed = distance >> 1;
        star->x = Iwram_MulQ16(Trig_Cos(angle), speed);
        star->y = Iwram_MulQ16(Trig_Sin(angle), speed);
        if (star->x & 1)
            star->x = -star->x;
        if ((star->y & 1) != 0)
            star->y = -star->y;
        star->z = (0x8000 + Random16()) >> 2;
        star->dx = (-star->x >> 7) + (star->y >> 8);
        star->dy = (-star->y >> 7) + (-star->x >> 8);
        star->age = 0;
        star->speed = 1 + (distance >> 13);
        star++;
    }
    angle = 0;
    spoke = work->spokes;
    i = 2;
    if (0 <= i) {
        do {
            spoke->x = Iwram_MulQ16(Trig_Cos(angle), 0x1000);
            spoke->y = Iwram_MulQ16(Trig_Sin(angle), 0x1000);
            spoke->inner_x = Iwram_MulQ16(Trig_Cos(angle), 0x200);
            spoke->inner_y = Iwram_MulQ16(Trig_Sin(angle), 0x200);
            spoke->state = 0;
            angle += 0x5555;
            spoke++;
            i--;
        } while (0 <= i);
    }
    work->unknown_13bc = 0;
    work->unknown_13c0 = 0;
    work->unknown_13cc = 0;
    ((FillWordsFn)0x03000164)(*(void **)(gWorkSlot + 40 * 4), 0x4000);
    data = Resource_GetTableEntry((s32)Value_000000c9);
    ((WordCopyFn)0x03001388)((void *)0x05000000, data, 128);
    Resource_DecodeType01(data + 128, work);
    switch (mode) {
    case 0:
        palette = (s32)Value_000000c8;
        break;
    case 1:
        palette = (s32)Value_000000c9;
        break;
    case 2:
        palette = (s32)Value_000000ca;
        break;
    default:
        palette = (s32)Value_000000cb;
        break;
    }
    data = Resource_GetTableEntry(palette);
    Dma_Set(data, (void *)0x05000000, 0x84000020, (volatile u32 *)0x040000d4);
    bg = (volatile struct BgAffineRegisters *)0x04000020;
    bg->x = 0;
    bg->y = 0;
    bg->pa = 0x100;
    bg->pb = 0;
    bg->pc = 0;
    bg->pd = 0x100;
    Func_080c9000(46, 7, 7, 3, 3);
    Func_080c9000(47, 7, 7, 3, 2);
    Scheduler_AddOrUpdateCallback(0x080c11ed, 0xc80);
    Scheduler_AddOrUpdateCallback(0x080c1439, 0xc80);
}
