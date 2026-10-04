/* 2026-10-04 canonical contract correction: remove all three empty counter
 * tails. Fresh score 120 described a 500-byte shape that still required
 * unconsumed counter instructions; it was not adoption-ready. Without those
 * tails the complete source extent is 476 bytes versus native 500, score 2000
 * (8 operand, 4 reordered, 2 inserted, 14 deleted) in all six TBS plans.
 * No new steering is added; raw/scaffold ownership and credit are unchanged.
 *
 * Native tails after the six/eight/default rings alter only r4 and flags,
 * writing no remaining point or ground field. Each owned TBS ROM has exactly
 * two direct BL callers, both in Ui_RunIconMonitor; a whole-ROM aligned-word
 * scan and all 96 active overlays per edition find no pointer references.
 * Both callers discard r4: its next local use assigns a fresh constant.
 * Battle_PlaceActorsByFormationKind consumes only the actual 6/8/10/4 points
 * selected by the same kind and cannot observe these counter-only tails.
 * The scans do not prove the absence of an arbitrary computed target, but
 * no current caller/data contract supports these instructions as behavior.
 * Do not restore them as timing, count results or point initialization
 * without new evidence. The older measured attempts below are retained as
 * rejected evidence, not instructions for reaching an exact match.
 */
/* 2026-10-04 paired zero-stage narrowing: paired zero after clobber1245; paired zero plus memory order1170; paired zero input only1245. */
/* 2026-10-04 paired store-order narrowing: paired volatile x store 120; paired consumed x memory and angle 190; paired volatile x memory and angle 190; paired x stage before shift 120. */
/* 2026-10-04 paired remaining-order trials: paired cursor binding 120; paired first-store split barriers 225; paired first-store memory input 180. */
/* 2026-10-04 second-call binding trials: second pointer binding without barrier 350; second both argument bindings 350; second split pointer binding 350; second point before y store 120. */
/* 2026-10-04 first-call angle alone without barrier295; both bindings
 * without barrier235 retained. Native500B extent and all behavior unchanged. */
/* 2026-10-04 first-call simplification trials: first arguments ordinary locals 9365; first angle binding with then-existing barrier235; first point binding only 8690; first arguments input constraint only 4430; first arguments bindings without barrier 235. */
/* 2026-10-04 paired first-store trials: paired x-angle coupled lifetimes 615; paired x-angle clobber r3 615; paired x-angle then memory store 615; paired x-angle then value dependency 615. */
/* 2026-10-04 paired live-register trials: simpler first call clobber 355; shared high-register y 285; shared y staged in r3 455; first store low-register exclusion 485; first store named x staging 235. */
/* 2026-10-04 paired register/scheduling trials: paired second y register 1525; paired second pointer scheduling 410; paired first argument scheduling 235; paired first store memory ordering 355. */
/* 2026-10-04 ordinary shared-point trials: paired explicit xyz origin 3397; paired first second pointers 6860; paired array point call 295; paired cursor point call 3775; paired split pointer increment 875; paired consumed zero temporary 545. */
/* 2026-10-04 shared point contract: AnimationPosition in ANIMSPR.H is the
 * actual 16-byte monitor record. The rendering consumer uses word four as
 * ground height; this routine writes only x/y/z. Score remains 295. */
/* 2026-10-03 paired coordinate/truncation trials: paired x store between angle shifts 295; paired shared angle/store ordering constraint 365. */
/* 2026-10-03 measured paired-store scheduling: paired x-store r3 ordering barrier 355; paired x-store r3 clobber 355. */
/* 2026-10-03 ordinary paired-origin trials: paired in-place fixed-point origin 2130; paired named fixed-point coordinates 305; paired explicit origin and radius 2485. */
/* 2026-10-03 ordinary paired-point trials: paired coordinates after truncation 295; paired cursor initialized before angle bias 295; consumed second-point pointer 410; paired for-loop lifetimes 355; typed two-point cursor 295. */
/* 2026-10-03 ordinary coordinate-store trials: x store before truncation 295; three coordinate stores before truncation 295; x store before existing angle barrier 295. */
/* 2026-10-03 ordinary fixed-slot loop trials: bounded ten-point loop 3745; derived signed countdown 475; derived unsigned countdown 695. */
/* 2026-10-03 ordinary idle-tail trials: natural continuation 695; natural remaining count 800; plain countdown 875. */
/* Earlier near miss (2026-10-04), rejected: 500 bytes, score 120 in all six TBS plans.
 * Ordinary x stores before angle truncation remove the three regular-ring
 * scheduling differences. Only paired-point register/order differences remain
 * (2 reordered only); no instruction is inserted or deleted.
 * Further ordinary pointer/origin forms and narrow scheduling trials above
 * did not improve the paired case. Two consumed first-call argument
 * bindings remove one further reorder. Staging the consumed second point
 * before its y/z stores fixes its scratch register and call ordering. No new
 * assembly barriers remain. Only pair-cursor setup and first x-store order
 * still differ; the original ground word is never touched.
 * Removed the empty countdown constraint: it restored instruction presence
 * rather than only register choice or order. The ordinary trailing loop
 * was uncredited; the former score475 was not adoption-ready.
 * Uses the shared AnimationObject/AnimationEntry model and the actual
 * Vector_AddPolarOffset(s32, s32, s32 *) interface with explicit u16 calls.
 * Three empty counter tails also remained; the audit above removes them.
 * No code was credited.
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

extern struct AnimationObject *gSpriteObjects;
extern volatile u32 gKeysHeld;
extern s32 gFrameTick;

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *point);

/* Lays out icon-monitor points around a position. The first sprite layer
   selects the ring shape; holding B turns it with the frame counter. */
void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *out)
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
        register struct AnimationPosition *p asm("r7") = out; /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
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
            out->x = px;
            angle = (u32)angle >> 16;
            out->y = zero;
            out->z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2aaa);
        }
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
            out->x = px;
            angle = (u32)angle >> 16;
            out->y = zero;
            out->z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2000);
        }
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
            out[0].x = px;
            angle = (u32)angle >> 16;
            out->y = zero;
            out[0].z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x1999);
        }
        break;
    case 20: {
        struct AnimationPosition *pair;
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
            {
                register s32 a asm("r1") = (u16)angle; /* FAKEMATCH: consumed first-call angle, ordinary295 ->235 with its point argument. */
                register s32 *point asm("r2") = &out[0].x; /* FAKEMATCH: consumed first-call point, ordinary295 ->235 with its angle. */
                Vector_AddPolarOffset(0x280000, a, point);
            }
            pair[1].x = x << 16;
            asm volatile("" : : : "r2", "memory"); /* FAKEMATCH: orders the paired x store before the remaining coordinates. */
            {
                register s32 *point asm("r2") = &out[1].x; /* FAKEMATCH: stage the consumed second point before y/z stores; register/order-only235 ->120. */
                pair[1].y = 0;
                pair[1].z = z << 16;
                Vector_AddPolarOffset(0x280000, (u16)angle, point);
            }
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
        break;
    }
}
