/* 2026-10-03 ordinary fixed-slot loop trials: bounded ten-point loop 3745; derived signed countdown 475; derived unsigned countdown 695. */
/* 2026-10-03 ordinary idle-tail trials: natural continuation 695; natural remaining count 800; plain countdown 875. */
/* Current near miss (2026-10-03): score 475.
 * Removed the empty countdown constraint: it restored instruction presence
 * rather than only register choice or order. The ordinary trailing loop
 * remains uncredited; the former score475 was not adoption-ready.
 * Uses the shared AnimationObject/AnimationEntry model and the actual
 * Vector_AddPolarOffset(s32, s32, s32 *) interface with explicit u16 calls.
 * Only empty register/scheduling constraints remain. No code is credited.
 * Earlier attempts are recorded below; they are measurements, not inputs. */
/* 2026-10-03 measured variants: r source-x binding 1840; h source-x binding 1840. Kept lowest score. */
/* 2026-10-03 measured variants: asm inputs 635; asm volatile inputs 635; asm angle input 635; asm input angle reverse 635; asm x output 635; asm angle zero output 635; asm zero output 1475; asm x zero output 1655; asm tied x 635; asm angle with early x 635; asm zero volatile 635; asm zero memory 635. Kept lowest score. */
/* 2026-10-03: a two-instruction x-copy/shift device worsened register allocation to 2115 and was removed; current draft retains only empty constraints. */
/* 2026-10-03 measured variants: baseline 475; plain x input 965; plain x low input 965; input x with clobber1 1175; input x with clobber2 1175; input x with clobber3 1175; input x case8 560; input x before zero 635. Kept lowest score. */
/* 2026-10-03 measured variants: baseline 505; remove step bindings 475; step output r5 475; x staging input 590; x stage assignment after zero 505; remove x fixed 505; remove zero fixed 1580; reorder barrier inputs 505. Kept lowest score. */
/* 2026-10-03 measured variants: baseline 565; split step 555; step output r5 535; remove step bindings 535; paired tail reorder 505; pair zero r3 1795; angle constraint memory 565; px output constraint 650. Kept lowest score. */
/* 2026-10-03 measured variants: baseline 650; split step 640; paired tail reorder 590; only zero constraint 565; pair barrier r1 650; pair barrier both 650; pair memory only 650. Kept lowest score. */
/* 2026-10-03 measured variants: baseline 745; split step 735; unsigned step 745; paired store barrier 650; paired tail reorder 685; paired tail split 8995; only px constraint 745; only zero constraint 660; swap store inputs 745. Kept lowest score. */
/* 2026-10-03 allocation trials: explicit zero-extension plus bounded constraints reached 745; later fixed-angle lifetime scored 930 and was removed. */
/* 2026-10-03: shared AnimationObject ring baseline 7745; equivalent-statement search reached 2765 (45 register,14 operand,18 reordered,3 inserted,8 deleted). */
/* alchemy permute: Map_BuildProbeRing against recon/tbs/raw/08012b2c.s: score 2765 (45 register-only, 14 operand, 18 reordered, 3 inserted, 8 deleted).
   Job 1, iteration 3960; rewrites: 8x reorder independent statements, 5x swap commutative operands, 4x add a same-width cast, 4x pointer arithmetic or indexing, 3x change loop form, 2x introduce a temporary, 2x split or join a compound assignment, 2x test truth or compare with zero, 1x reorder local declarations, 1x drop a same-width cast. */
/* 2026-10-03 trials: shared ordinary baseline 7745; fixed s16 angle/counter 11220, s32 angle 6205, angle/out/z 7140. Fixed counter was invalid across calls and is removed. */
/* 2026-10-03: replaced invented map-state overlay with the actual AnimationObject / AnimationEntry kind; removed permuter-only temporary chains for a fresh ordinary-source baseline. Previous score 4240. */
/* 2026-09-29 alchemy permute: score 8050 to 4240 on the permuter's scorer
   (0 is exact); remaining 86 register-only, 15 operand, 22 reordered, 8
   inserted, 13 deleted. Kept rewrites: 13x reorder independent statements,
   5x introduce a temporary, 4x reorder local declarations, 4x pointer
   arithmetic or indexing, 2x swap commutative operands, 2x remove a
   temporary, 2x change loop form, 1x add a same-width cast, 1x split or
   join a compound assignment, 1x test truth or compare with zero.
   FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
/* Draft, not exact (2026-09-25): 492 of 500 bytes, 229 differing halfwords.
   Written from the listing; the switch, every loop shape (including the
   empty tails that run the counter up to ten) and the constants line up.
   Remaining: the ROM gives the angle r5, z r6, out r7 and x r8 and keeps
   the counter in r4, saving it around each call in a 4-byte frame; here
   the counter takes r7 and the angle r8 (declaration order, u16/s32 angle
   types and indexed case-20 stores moved nothing). */
#include "TYPES.H"
#include "IO_REG.H"
#include "ANIMSPR.H"

struct ProbePoint {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
};

extern struct AnimationObject *gSpriteObjects;
extern volatile u32 gKeysHeld;
extern s32 gFrameTick;

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *point);

/* Lays out icon-monitor points around a position. The first sprite layer
   selects the ring shape; holding B turns it with the frame counter. */
void Map_BuildProbeRing(s32 x, s32 z, struct ProbePoint *out)
{
    /* FAKEMATCH: prior ordinary and permuted shapes kept the angle in r6; constrain the measured angle register while preserving all truncations. */
    s32 angle;
    s32 i;
    u8 kind;

    angle = 0;
    kind = gSpriteObjects->entries[0]->kind;
    if ((gKeysHeld & KEY_B) != 0)
        angle = (s16)(gFrameTick << 8);
    {
        register s32 a asm("r5") = angle; /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
        register struct ProbePoint *p asm("r7") = out; /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
        asm("" : "+r"(a), "+r"(p)); /* FAKEMATCH: native register lifetime and shift scheduling. */
        angle = a;
        out = p;
    }
    switch ((u32)kind) {
        u16 tmp2;
    case 3:
        for (i = 0; i < 6; i++) {
            {
            register s32 zero asm("r3"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            register s32 px asm("r2"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            angle <<= 16;
            asm volatile("" : "+r"(angle) : : "r2", "r3"); /* FAKEMATCH: keeps the angle shift before low-register staging. */
            px = x << 16;
            zero = 0;
            asm("" : "+r"(angle) : "r"(zero)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (u32)angle >> 16;
            out->x = px;
            out->y = zero;
            out->z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2aaa);
        }
        for (; i < 10; i++);
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        for (i = 0; i < 8; i++) {
            {
            register s32 zero asm("r3"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            register s32 px asm("r2"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            angle <<= 16;
            asm volatile("" : "+r"(angle) : : "r2", "r3"); /* FAKEMATCH: keeps the angle shift before low-register staging. */
            px = x << 16;
            zero = 0;
            asm("" : "+r"(angle) : "r"(zero)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (u32)angle >> 16;
            out->x = px;
            out->y = zero;
            out->z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2000);
        }
        for (; i < 10; i++);
        break;
    case 4:
    case 6:
        for (i = 0; i < 10; i++) {
            {
            register s32 zero asm("r3"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            register s32 px asm("r2"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            angle <<= 16;
            asm volatile("" : "+r"(angle) : : "r2", "r3"); /* FAKEMATCH: keeps the angle shift before low-register staging. */
            px = x << 16;
            zero = 0;
            asm("" : "+r"(angle) : "r"(zero)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (u32)angle >> 16;
            out[0].x = px;
            out->y = zero;
            out[0].z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x1999);
        }
        break;
    case 20: {
        struct ProbePoint *pair;
        tmp2 = (u16)angle;
        angle = (s16)(tmp2 + 0x4000);
        i = 0;
        pair = out;
        while (i < 2) {
            pair[0].x = x << 16;
            pair[0].y = 0;
            pair[0].z = z << 16;
            angle <<= 16;
            asm("" : "+r"(angle)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (u32)angle >> 16;
            Vector_AddPolarOffset(0x280000, (u16)angle, &out[0].x);
            pair[1].x = x << 16;
            asm volatile("" : : : "r2", "memory"); /* FAKEMATCH: orders the paired x store before the remaining coordinates. */
            pair[1].y = 0;
            pair[1].z = z << 16;
            Vector_AddPolarOffset(0x280000, (u16)angle, &out[1].x);
            pair += 2;
            out += 2;
            angle = (s16)(angle + (u32)0x8000);
            i++;
        }
        break;
    }
    default:
        i = 0;
        {
            s32 a = (u32)angle << 16;
            asm("" : "+r"(a)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (s32)((u32)a + 0x20000000) >> 16;
        }
        while (i < 4) {
            angle <<= 16;
            asm("" : "+r"(angle)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (u32)angle >> 16;
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            ++out;
            (u16)(angle = (s16)(angle + 0x4000));
            i++;
        }
        for (i = 9 - i; i >= 0; i--);
        break;
    }
}
