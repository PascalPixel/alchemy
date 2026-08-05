#include "types.h"

/*
 * resource_391 owner at 0x02000d3c, 6700 bytes advertised.  The complete owner
 * is one prologue at 0x02000d3c and one epilogue at 0x0200273e-0x02002748; a
 * control-flow walk from the prologue reaches 6,382 bytes of code and leaves
 * eight literal pools totalling 318 bytes:
 *
 *   0x010d2-0x0111f (78)   0x01366-0x0138f (42)   0x0173e-0x01777 (58)
 *   0x01a78-0x01a83 (12)   0x01bfa-0x01bff  (6)   0x02002-0x02037 (54)
 *   0x02532-0x02557 (38)   0x0274a-0x02767 (30)
 *
 * 6,382 + 318 = 6,700, so the row is fully accounted for.  The pool map is
 * derived from the owner's own control flow (every one of those ranges is
 * jumped over by a `b.n`), NOT from overlay_show's "pool words referenced"
 * listing - that listing contains a spurious 0x2001424 = 0x78023023, which is
 * really the instruction pair `adds r0,#35 / ldrb r2,[r0,#0]` in live code.
 * Excluding it as pool would have dropped two real instructions.  Register
 * state is carried ACROSS every pool: several of them sit mid-call-setup.
 *
 * This is the scene script for the whole cutscene: it configures the camera and
 * the actor set, installs three per-frame effect tasks, and then plays out a
 * long fixed sequence of actor moves, waits, camera pushes and sound cues.
 *
 * CALLS.  Resolved with 'bun tools/overlay_call_targets.ts resource_391 0d3c'.
 * An overlay bl stores target_offset - 2, so overlay_show's annotations are
 * wrong at every site and were not used.  672 sites collapse to 43 distinct
 * targets: 41 import veneers resolving to main-image Func_08xxxxxx addresses,
 * plus the two in-overlay helpers Func_02002768 (63 sites) and Func_02002780
 * (68 sites), whose two- and three-argument signatures come from the byte-exact
 * assets/code/resource_391_c_02002768.c and _02002780.c.
 * The inventory says calls=616; that field is a lower bound, so completeness is
 * stated as placed(672) >= 616 and proved by the multiset comparison of bl
 * targets in the reconstruction assembly against the Func_ calls below.
 *
 * LINK BASE 0x02008000, confirmed here by three independent witnesses of the
 * cheapest kind - odd pool words that are a known function start plus the Thumb
 * bit, each handed to the task installers Func_080000d0/Func_080000d8:
 *   0x0200a7c9 = Func_020027c8 + 1   (semantic/overlays/resource_391_c_020027c8.c)
 *   0x0200a975 = Func_02002974 + 1   (semantic/overlays/resource_391_c_02002974.c)
 *   0x0200aad9 = Func_02002ad8 + 1   (assets/code/resource_391_c_02002ad8.c)
 * Under that base the even pool words 0x0200ae20/ae54/ae88/aebc/af48/af6c/afc8
 * and 0x0200b024 are in-image data at file offsets 0x2e20..0x3024, and the six
 * state words 0x0200b384..0x0200b398 are at 0x3384..0x3398, at and just past the
 * end of the 0x3384-byte assembled reconstruction.  The overlay image is
 * writable EWRAM, so those are live variables, not constants.
 *
 * This owner is the WRITER of the state the two installed tasks read, which
 * settles the open question in resource_391_c_020027c8.c and matches
 * resource_391_c_02002974.c exactly:
 *   Data_0200b384  height   (16.16), reset to 0x00800000 before each ramp
 *   Data_0200b388  spread,  reset to 0
 *   Data_0200b38c  phase    1 -> 2 -> 3; the task clears it to 0 when the ramp
 *                           finishes, and this owner spins on that
 *   Data_0200b390  cleared before the third task is installed
 *   Data_0200b394  a scene variant flag - Func_080770c0(3)'s result, gating the
 *                  fourth actor (slot 3) and actor 15 throughout
 *   Data_0200b398  the gate Func_020027c8 tests: set to 1 while that task runs,
 *                  cleared before it is stopped
 *
 * Epilogue 'pop {r3,r5} / mov r8,r3 / mov sl,r5 / pop {r5,r6,r7} / pop {r0} /
 * bx r0': r0 holds the popped return address, so the owner returns nothing, and
 * it reads no incoming argument register, so it takes none.
 *
 * The recurring five-instruction idiom
 *   ldr r3,[pc] ; ldr r2,[r3] ; movs r3,#236 ; lsls r3,#1 ; adds r2,r2,r3
 *   ldrh r3,[r2] ; adds r3,#1 ; strh r3,[r2]
 * bumps a u16 counter at workspace + 472.  It appears once on the "absent"
 * side of nearly every Data_0200b394 test, i.e. whenever a beat that would have
 * animated slot 3 is skipped - so it is a progress/step counter that must
 * advance the same number of times in both scene variants.  Spelled here as the
 * SKIP_BEAT macro; 0x03001ebc is the workspace pointer the rest of the overlay
 * loads directly.
 *
 * UNCERTAINTIES:
 *  - the numeric first arguments of the imports are actor/slot ids (0..3 are the
 *    four principals, 11..16 are secondary entities) and the second arguments are
 *    16.16 positions, durations in frames, or opaque animation ids depending on
 *    the import; no attempt is made to name them beyond what the call sites prove.
 *  - Func_0808a010 and Func_080000c0 both take a frame count and both appear as
 *    waits; they are kept distinct because the assembly does.
 *  - Func_0808a070(slot, 0) is tested three ways here - against 0, against
 *    non-zero, and (once, at 0x020015b2) against exactly 1 - so it returns a
 *    small code, not a boolean.  It is declared s32.
 *  - Data_0200ae20 etc. are opaque in-image blocks handed to Func_0808a098/
 *    Func_0808a0b0; their shape is not reconstructed.
 *  - r4 is never saved and never used; r8 and sl are saved and used only as
 *    long-lived constant holders, and both are reassigned mid-owner, so every
 *    constant below is taken per-use rather than as one variable.
 */

/* In-image data under the 0x02008000 link base. */
extern u8 Data_0200ae20[];
extern u8 Data_0200ae54[];
extern u8 Data_0200ae88[];
extern u8 Data_0200aebc[];
extern u8 Data_0200af48[];
extern u8 Data_0200af6c[];
extern u8 Data_0200afc8[];
extern u8 Data_0200b024[];
extern s32 Data_0200b384;
extern s32 Data_0200b388;
extern s32 Data_0200b38c;
extern s32 Data_0200b390;
extern s32 Data_0200b394;
extern s32 Data_0200b398;

/* IWRAM workspace pointer. */
#define WORKSPACE (*(u8 *volatile *)0x03001ebc)
#define SKIP_BEAT() do { \
    u8 *skip_workspace = WORKSPACE; \
    u16 *skip_counter = (u16 *)(skip_workspace + 472); \
    *skip_counter += 1; \
} while (0)

struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x23 - 0x14];
    u8 f23;
    u8 pad24[4];
    s32 f28;
};

/* Old-style declarations are mandatory in overlay sources. */
void Func_02002768();
void Func_02002780();
void Func_020027c8(void);
void Func_02002974(void);
void Func_02002ad8(void);
void Func_080000c0();
void Func_080000d0();
void Func_080000d8();
void Func_08009128();
void Func_080091e0();
s32 Func_080770c0();
void Func_080770c8();
void Func_080f9010();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
struct Obj *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0a8();
void Func_0808a0b0();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e0();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a330();
void Func_0808a348();
void Func_0808a360();
void Func_0808a370();
void Func_0808a4f0();

void Func_02000d3c(void)
{
    struct Obj *p;
    s32 movement_2000;
    s32 movement_4000;
    s32 movement_6000;
    s32 movement_a000;
    s32 movement_c000;
    s32 actor1_2000;
    s32 actor1_6000;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a210(246 << 16, -1, 151 << 18, 0);

    /* The scene-variant flag: with it clear, slot 3 and actor 15 are absent. */
    Data_0200b394 = Func_080770c0(3);

    p = Func_0808a080(13);
    Func_080091e0(p, 0);
    p = Func_0808a080(14);
    Func_080091e0(p, 0);
    p = Func_0808a080(15);
    Func_080091e0(p, 0);
    p = Func_0808a080(16);
    Func_080091e0(p, 0);
    p = Func_0808a080(17);
    Func_080091e0(p, 0);
    p = Func_0808a080(18);
    Func_080091e0(p, 0);
    p = Func_0808a080(19);
    Func_080091e0(p, 0);
    p = Func_0808a080(20);
    Func_080091e0(p, 0);
    p = Func_0808a080(21);
    Func_080091e0(p, 0);

    Func_0808a098(17, Data_0200b024);
    Func_0808a098(18, Data_0200b024);
    Func_0808a098(19, Data_0200b024);
    Func_0808a098(20, Data_0200b024);
    Func_0808a098(21, Data_0200b024);

    Func_0808a0f0(0, 232 << 15, 0x025a0000);
    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();

    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0d0(0, 254, 0x251);
    Func_0808a090(1, 0x9999, 0x4ccc);
    Func_0808a090(2, 0x9999, 0x4ccc);

    p = Func_0808a080(0);
    if (p != 0) {
        Func_0808a0f0(1, p->f08, p->f10);
    }
    p = Func_0808a080(0);
    if (p != 0) {
        Func_0808a0f0(2, p->f08, p->f10);
    }
    Func_0808a098(1, Data_0200ae20);
    Func_0808a098(2, Data_0200ae54);

    if (Data_0200b394 != 0) {
        Func_0808a090(3, 0x9999, 0x4ccc);
        p = Func_0808a080(0);
        if (p != 0) {
            Func_0808a0f0(3, p->f08, p->f10);
        }
        Func_0808a098(3, Data_0200ae88);
    }

    movement_2000 = 0x2000;
    Func_0808a0a0(2);
    movement_4000 = 0x4000;
    Func_02002780(2, movement_2000, 40);
    Func_02002780(2, 0x8000, 20);
    Func_02002780(2, movement_4000, 40);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a010(60);
    Func_0808a1b8(1, movement_4000, 0);
    movement_6000 = 0x6000;
    Func_02002780(0, movement_6000, 60);
    Func_02002780(3, movement_2000, 10);
    Func_0808a1b8(1, movement_2000, 0);
    movement_a000 = 0xa000;
    Func_02002780(0, movement_a000, 10);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a010(40);
    Func_0808a1b8(1, movement_4000, 0);
    Func_02002780(0, movement_6000, 10);
    Func_0808a138(1, 2);
    Func_0808a170(0x1474);
    Func_02002768(1, 10);
    Func_0808a170(0x147c);
    movement_c000 = 0xc000;
    Func_02002780(2, movement_c000, 20);
    Func_0808a110(2, 3);
    Func_02002768(2, 20);
    Func_02002780(1, 0, 20);
    Func_02002780(0, movement_a000, 40);
    Func_02002780(1, movement_4000, 20);
    Func_02002780(0, movement_6000, 30);
    Func_02002780(1, movement_6000, 20);
    Func_02002780(0, 0xe000, 30);
    Func_0808a138(2, 2);
    Func_0808a1e8(2, 256, 0);
    Func_0808a010(40);
    Func_0808a1b8(1, movement_4000, 0);
    Func_02002780(0, movement_6000, 20);
    Func_02002780(2, movement_c000, 10);
    Func_080f9010(17);
    Func_080f9010(206);
    Func_0808a330(0x7fff, 0);
    Func_0808a348(1);
    Func_080000c0(1);

    /* First effect task: the 0x27c8 spawner, gated by Data_0200b398. */
    Data_0200b398 = 1;
    Func_080000d0((void *)((u32)Func_020027c8 + 1), 3200);

    Func_080000c0(20);
    Func_0808a330(0x00405210, 1);
    Func_0808a330(0x10000, 2);
    Func_0808a348(120);
    Func_080000c0(60);
    Func_0808a098(0, Data_0200aebc);
    Func_0808a098(1, Data_0200aebc);
    Func_0808a098(2, Data_0200aebc);
    Func_0808a098(3, Data_0200aebc);
    Func_0808a010(100);
    Func_02002768(1, 20);
    Func_02002768(2, 40);

    if (Data_0200b394 != 0) {
        Func_0808a010(40);
        Func_0808a1e8(3, 258, 0);
        Func_0808a010(40);
        Func_02002768(3, 40);
    } else {
        SKIP_BEAT();
    }

    Func_0808a010(20);

    {
        u8 *actor_data;
        s32 placement_20000;

        if (Data_0200b394 != 0) {
            s32 conditional_placement;

            p = Func_0808a080(3);
            conditional_placement = 0x20000;
            p->f28 = conditional_placement;
            Func_0808a010(10);
            Func_0808a090(3, conditional_placement, conditional_placement);
            Func_0808a0e0(3, -2, 0);
            Func_0808a098(3, Data_0200af48);
            p = Func_0808a080(3);
            Func_080091e0(p, 0);
            Func_0808a100(3, 19);
            Func_0808a010(10);
        }

        p = Func_0808a080(0);
        placement_20000 = 0x20000;
        p->f28 = placement_20000;
        Func_0808a010(10);
        Func_0808a090(0, placement_20000, placement_20000);
        actor_data = Data_0200af48;
        Func_0808a098(0, actor_data);
        p = Func_0808a080(0);
        Func_080091e0(p, 0);
        Func_0808a100(0, 19);
        Func_0808a010(20);

        p = Func_0808a080(1);
        p->f28 = placement_20000;
        Func_0808a010(10);
        Func_0808a090(1, placement_20000, placement_20000);
        Func_0808a098(1, actor_data);
        p = Func_0808a080(1);
        Func_080091e0(p, 0);
        Func_0808a100(1, 19);
        Func_0808a010(40);

        p = Func_0808a080(2);
        p->f28 = placement_20000;
        Func_0808a010(10);
        /* Slot 2 gets no Func_0808a090 - that asymmetry is in the original. */
        Func_0808a098(2, actor_data);
        p = Func_0808a080(2);
        Func_080091e0(p, 0);
        Func_0808a100(2, 19);
    }

    /* Stop the 0x27c8 spawner, then uninstall it. */
    Data_0200b398 = 0;
    Func_0808a010(160);
    Func_080000d8((void *)((u32)Func_020027c8 + 1));

    Func_0808a010(120);
    Func_0808a330(0x00406218, 1);
    Func_0808a348(60);
    Func_080000c0(60);

    /* Second effect task: the 0x2974 three-phase emitter. */
    Data_0200b388 = 0;
    Data_0200b384 = 0x00800000;
    Data_0200b38c = 1;
    Func_080000d0((void *)((u32)Func_02002974 + 1), 3200);

    Func_0808a010(180);
    Func_080f9010(21);
    Func_02002768(1, 80);
    Func_02002768(2, 40);
    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(1, 258, 0);
    Func_0808a1e8(2, 258, 0);
    Func_0808a1e8(3, 258, 0);
    Func_0808a010(60);
    Func_02002768(2, 20);

    Func_0808a130(2, 2);
    Func_0808a010(20);
    Func_0808a130(1, 1);
    Func_0808a010(40);
    Func_0808a130(0, 2);
    Func_0808a130(3, 1);
    Func_0808a010(20);
    Func_0808a130(2, 3);
    Func_0808a010(40);
    Func_0808a130(0, 1);
    Func_0808a010(20);
    Func_0808a130(1, 2);
    Func_0808a010(20);
    Func_0808a130(3, 2);
    Data_0200b38c = 2;
    Func_0808a1e8(1, 258, 0);
    Func_02002768(1, 20);

    if (Data_0200b394 != 0) {
        Func_0808a1e8(3, 258, 0);
        Func_02002768(3, 10);
    } else {
        SKIP_BEAT();
    }

    Data_0200b38c = 3;

    Func_0808a080(0)->f23 &= 0xfe;
    Func_0808a080(1)->f23 &= 0xfe;
    Func_0808a080(2)->f23 &= 0xfe;
    Func_0808a080(3)->f23 &= 0xfe;
    Func_0808a1e0(0, 3);
    Func_0808a1e0(1, 3);
    Func_0808a1e0(2, 3);
    Func_0808a1e0(3, 3);

    /* Third effect task: 0x2ad8. */
    Data_0200b390 = 0;
    Func_080000d0((void *)((u32)Func_02002ad8 + 1), 3200);

    Func_080f9010(220);

    Func_0808a080(13)->f23 &= 0xfe;
    Func_0808a1e0(13, 2);
    Func_0808a0f0(13, 253 << 16, 0x025b0000);
    Func_0808a098(13, Data_0200af6c);

    Func_0808a080(14)->f23 &= 0xfe;
    Func_0808a1e0(14, 2);
    Func_0808a0f0(14, 233 << 16, 0x02750000);
    Func_0808a098(14, Data_0200af6c);

    if (Data_0200b394 != 0) {
        Func_0808a080(15)->f23 &= 0xfe;
        Func_0808a1e0(15, 2);
        Func_0808a0f0(15, 207 << 16, 0x02610000);
        Func_0808a098(15, Data_0200af6c);
    }

    Func_0808a080(16)->f23 &= 0xfe;
    Func_0808a1e0(16, 2);
    Func_0808a0f0(16, 227 << 16, 145 << 18);
    Func_0808a098(16, Data_0200af6c);

    /* Wait for the 0x2974 emitter to finish its ramp and clear the phase. */
    if (Data_0200b38c != 0) {
        do {
            Func_080000c0(1);
        } while (Data_0200b38c != 0);
    }

    Func_0808a010(300);
    Func_080000d8((void *)((u32)Func_02002974 + 1));
    Func_0808a010(120);
    Func_080f9010(17);
    Func_0808a330(0x10000, 1);
    Func_0808a348(60);
    Func_080000c0(60);

    Func_0808a0a8(13);
    Func_0808a0a8(14);
    if (Data_0200b394 != 0) {
        Func_0808a0a8(15);
    }
    Func_0808a0a8(16);
    Func_080000c0(1);

    Func_0808a098(13, Data_0200afc8);
    Func_0808a098(14, Data_0200afc8);
    if (Data_0200b394 != 0) {
        Func_0808a098(15, Data_0200afc8);
    }
    Func_0808a0b0(16, Data_0200afc8);

    Func_0808a010(80);
    Func_0808a138(1, 2);
    Func_0808a010(40);
    Func_0808a178(1, 0);
    Func_0808a0f0(11, 220 << 16, 247 << 17);
    Func_0808a0f0(12, 220 << 16, 247 << 17);
    Func_080000c0(1);

    if (Func_0808a070(11, 0) == 1) {
        SKIP_BEAT();
    }

    Func_0808a138(0, 1);
    Func_0808a010(20);
    Func_0808a138(2, 2);
    Func_02002768(2, 20);

    if (Data_0200b394 != 0) {
        Func_0808a138(3, 2);
        Func_0808a010(10);
        Func_0808a170(0x1488);
        Func_02002768(3, 40);
    }

    Func_0808a130(1, 1);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a010(80);
    Func_0808a138(2, 2);
    Func_0808a170(0x1489);
    Func_02002768(2, 40);
    Func_0808a138(1, 3);
    Func_0808a010(40);
    Func_0808a1e0(1, 2);

    Func_0808a080(1)->f23 |= 1;
    p = Func_0808a080(1);
    Func_080091e0(p, 1);
    Func_0808a128(1, 6, 0);
    Func_0808a0e0(1, -3, 0);
    Func_0808a100(1, 1);
    Func_02002780(1, 0x4000, 60);
    Func_02002768(1, 20);
    Func_0808a130(1, 2);
    actor1_2000 = 0x2000;
    Func_02002768(1, 10);
    Func_0808a138(0, 3);
    Func_02002780(1, actor1_2000, 20);
    actor1_6000 = 0x6000;
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a010(40);
    Func_02002780(1, actor1_6000, 40);
    Func_02002780(1, actor1_2000, 20);
    Func_02002780(1, actor1_6000, 20);
    Func_02002780(1, actor1_2000, 10);
    Func_0808a128(1, 2, 0);
    Func_0808a010(40);
    Func_0808a128(1, 2, 0);
    Func_0808a010(10);
    Func_0808a128(1, 4, 0);
    Func_0808a010(20);
    Func_02002768(1, 20);

    if (Data_0200b394 != 0) {
        Func_0808a1e8(3, 256, 0);
        Func_0808a010(60);
        Func_0808a138(3, 2);
        Func_0808a010(80);
        Func_0808a1e0(3, 2);
        Func_0808a080(3)->f23 |= 1;
        p = Func_0808a080(3);
        Func_080091e0(p, 1);
        Func_0808a128(3, 4, 0);
        Func_0808a0e0(3, -2, 0);
        Func_0808a100(3, 1);
        Func_02002780(3, 0xe000, 60);
        Func_0808a138(3, 2);
        Func_0808a010(20);
        Func_02002768(3, 20);
    } else {
        SKIP_BEAT();
    }

    Func_0808a128(1, 2, 0);
    Func_02002780(1, 0x4000, 20);
    Func_0808a110(1, 3);
    Func_02002780(1, 0x2000, 10);
    Func_02002768(1, 20);
    Func_0808a110(1, 3);
    Func_0808a010(10);
    Func_0808a138(2, 1);
    Func_0808a010(40);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a1e0(2, 2);
    Func_0808a080(2)->f23 |= 1;
    p = Func_0808a080(2);
    Func_080091e0(p, 1);
    Func_0808a128(2, 4, 0);
    Func_0808a100(2, 1);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a138(0, 2);
    Func_0808a010(10);
    Func_0808a1e0(0, 2);
    Func_0808a080(0)->f23 |= 1;
    p = Func_0808a080(0);
    Func_080091e0(p, 1);
    Func_0808a128(0, 4, 0);
    Func_0808a100(0, 1);
    Func_02002780(0, 0x6000, 60);
    Func_0808a1e8(0, 0x105, 0);
    Func_0808a1e8(2, 0x105, 0);
    Func_0808a010(60);
    Func_02002780(0, 0xa000, 20);
    Func_0808a110(1, 3);
    Func_0808a110(0, 3);
    Func_02002780(0, 0x6000, 10);
    Func_02002780(1, 0x4000, 10);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a110(2, 3);
    Func_02002768(2, 20);
    Func_0808a100(0, 3);
    Func_0808a100(3, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_0808a178(2, 0);
    Func_0808a100(2, 3);
    Func_0808a1b8(2, 0xe000, 0);
    Func_0808a1b8(1, 0x2000, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a100(2, 3);
        Func_0808a110(1, 3);
        SKIP_BEAT();
    } else {
        Func_0808a138(1, 2);
        Func_02002780(1, 0x2000, 10);
        Func_0808a130(1, 2);
        Func_0808a180(1, 0);
    }

    Func_02002780(1, 0x4000, 10);
    Func_0808a110(1, 4);
    Func_02002768(1, 20);
    Func_02002780(2, 0xc000, 10);
    Func_0808a110(2, 3);
    Func_02002768(2, 10);

    if (Data_0200b394 != 0) {
        Func_0808a138(3, 2);
        Func_02002780(3, 0, 20);
        Func_02002780(3, 0x2000, 10);
        Func_0808a100(3, 4);
        Func_02002768(3, 10);
    } else {
        SKIP_BEAT();
    }

    Func_0808a1b8(1, 0x2000, 0);
    Func_02002780(0, 0xa000, 10);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x6000, 0);
    Func_02002780(1, 0x4000, 10);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(1, 258, 0);
    Func_0808a010(80);
    Func_02002780(2, 0xe000, 10);
    Func_0808a130(2, 2);
    Func_02002768(2, 20);
    Func_0808a1b8(1, 0x2000, 0);
    Func_02002780(0, 0xa000, 40);
    Func_0808a1b8(1, 0x4000, 0);
    Func_02002780(0, 0x6000, 10);
    Func_02002780(2, 0xc000, 10);
    Func_0808a110(2, 3);
    Func_02002768(2, 10);
    Func_0808a1f0(1, 258);
    Func_0808a010(40);
    Func_02002768(1, 20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a1e8(1, 258, 0);
    Func_0808a010(40);
    Func_02002768(1, 20);
    Func_0808a100(2, 3);
    Func_02002768(2, 10);
    Func_0808a138(1, 2);
    Func_02002780(1, 0x2000, 10);
    Func_0808a178(1, 0);
    Func_0808a1b8(0, 0xa000, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(1, 3);
    } else {
        Func_0808a010(20);
        Func_0808a138(1, 2);
        Func_0808a010(40);
        SKIP_BEAT();
    }

    Func_0808a180(1, 0);
    Func_080f9010(21);
    Func_0808a330(0x00406218, 1);
    Func_0808a348(60);
    Func_080000c0(60);

    /* Re-arm and reinstall the 0x2974 emitter for the second ramp. */
    Data_0200b388 = 0;
    Data_0200b384 = 0x00800000;
    Data_0200b38c = 1;
    Func_080000d0((void *)((u32)Func_02002974 + 1), 3200);

    Func_0808a010(80);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(3, 2);
    Func_0808a138(2, 2);
    Func_0808a010(60);
    Func_02002780(2, 0xc000, 10);
    Func_0808a170(0x149d);
    Func_02002768(2, 10);
    Func_02002780(1, 0xc000, 10);
    Func_02002780(0, 0xc000, 10);
    if (Data_0200b394 != 0) {
        Func_02002780(3, 0xc000, 10);
    }

    Func_0808a080(0)->f23 &= 0xfe;
    Func_0808a080(1)->f23 &= 0xfe;
    Func_0808a080(2)->f23 &= 0xfe;
    Func_0808a080(3)->f23 &= 0xfe;
    Func_0808a1e0(0, 3);
    Func_0808a1e0(1, 3);
    Func_0808a1e0(2, 3);
    Func_0808a1e0(3, 3);

    Data_0200b38c = 2;
    Func_080f9010(220);
    Func_0808a0f0(13, 253 << 16, 0x025b0000);
    Func_0808a098(13, Data_0200af6c);
    Func_0808a0f0(14, 233 << 16, 0x02750000);
    Func_0808a098(14, Data_0200af6c);
    if (Data_0200b394 != 0) {
        Func_0808a0f0(15, 207 << 16, 0x02610000);
        Func_0808a098(15, Data_0200af6c);
    }
    Func_0808a0f0(16, 227 << 16, 145 << 18);
    Func_0808a098(16, Data_0200af6c);
    Func_0808a010(120);

    Data_0200b38c = 3;
    do {
        Func_080000c0(1);
    } while (Data_0200b38c != 0);

    Func_02002768(11, 80);
    Func_02002768(12, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a1e8(3, 0x101, 0);
    Func_0808a010(60);
    Func_02002768(12, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_02002768(12, 10);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_02002768(12, 10);
    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(1, 256, 0);
    Func_0808a1e8(2, 256, 0);
    Func_0808a1e8(3, 256, 0);
    Func_0808a010(40);
    Func_02002768(11, 10);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(3, 0, 0);
    Func_02002780(2, 0xc000, 40);
    Func_02002768(12, 10);
    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(1, 258, 0);
    Func_0808a1e8(2, 258, 0);
    Func_0808a1e8(3, 258, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 80);
    Func_02002768(12, 10);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_02002780(3, 0, 40);
    Func_02002768(11, 10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 10);
    Func_0808a100(0, 4);
    Func_0808a100(1, 4);
    Func_0808a100(3, 4);
    Func_0808a110(2, 4);
    Func_0808a010(60);
    Func_02002768(12, 10);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_02002768(12, 20);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_02002780(3, 0, 20);
    Func_02002768(12, 10);

    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(3, 2);
    Func_0808a138(2, 2);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_02002768(12, 20);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_02002780(3, 0, 20);
    Func_02002768(11, 20);
    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(1, 258, 0);
    Func_0808a1e8(3, 258, 0);
    Func_0808a1e8(2, 258, 0);
    Func_0808a010(40);
    Func_02002768(12, 10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 10);
    Func_02002768(12, 10);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_0808a010(60);
    Func_0808a180(12, 0);
    Func_0808a180(11, 0);

    Func_080000d8((void *)((u32)Func_02002974 + 1));
    Func_0808a010(80);
    Func_0808a330(0x10000, 1);
    Func_0808a348(60);
    Func_080000c0(80);

    Func_0808a0a8(13);
    Func_0808a0a8(14);
    /* Unlike the first pass, slot 15 is faded unconditionally here. */
    Func_0808a0a8(15);
    Func_0808a0a8(16);
    Func_080000c0(1);
    Func_0808a098(13, Data_0200afc8);
    Func_0808a098(14, Data_0200afc8);
    if (Data_0200b394 != 0) {
        Func_0808a098(15, Data_0200afc8);
    }
    Func_0808a0b0(16, Data_0200afc8);

    Func_0808a010(20);
    Func_0808a1e0(0, 2);
    Func_0808a1e0(1, 2);
    Func_0808a1e0(2, 2);
    Func_0808a1e0(3, 2);
    Func_0808a080(0)->f23 |= 1;
    Func_0808a080(1)->f23 |= 1;
    Func_0808a080(2)->f23 |= 1;
    Func_0808a080(3)->f23 |= 1;

    Func_0808a138(2, 2);
    Func_02002780(2, 0xe000, 10);
    Func_0808a178(2, 0);
    Func_0808a1b8(1, 0x2000, 0);
    Func_0808a1b8(3, 0, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a138(1, 2);
        Func_0808a010(10);
        Func_0808a178(1, 0);

        if (Func_0808a070(0, 0) == 0) {
            Func_02002780(3, 0, 20);
            Func_0808a1e8(1, 0x101, 0);
            Func_0808a1e8(2, 0x101, 0);
            Func_0808a1e8(3, 0x101, 0);
            Func_0808a010(40);
            Func_02002780(1, 0x4000, 20);
            Func_02002768(1, 10);
            Func_02002780(2, 0xc000, 20);
            Func_02002780(2, 0xe000, 20);
            Func_0808a110(2, 3);
            Func_02002768(2, 20);
            Func_02002780(1, 0x2000, 20);
        } else {
            Func_02002780(3, 0, 20);
            Func_0808a1e8(1, 258, 0);
            Func_0808a1e8(2, 258, 0);
            Func_0808a1e8(3, 258, 0);
            Func_0808a010(40);
            Func_02002780(1, 0x4000, 20);
            Func_0808a170(0x14b4);
            Func_02002768(1, 20);
            Func_0808a110(2, 3);
            Func_02002768(2, 20);
        }

        Func_0808a100(3, 3);
        Func_0808a110(1, 3);
    } else {
        Func_0808a010(20);
        Func_0808a110(1, 3);
        Func_0808a010(10);
        Func_0808a170(0x14b6);
        Func_02002768(1, 10);
        Func_0808a1b8(1, 0x4000, 0);
        Func_02002780(0, 0x6000, 20);
        Func_0808a100(1, 3);
        Func_0808a110(0, 3);
        Func_0808a010(10);
        Func_0808a110(2, 4);
        Func_0808a178(2, 0);

        if (Func_0808a070(0, 0) == 0) {
            Func_0808a010(20);
            Func_0808a1e8(2, 0x103, 0);
            Func_0808a010(40);
            Func_02002780(2, 0xe000, 10);
            Func_02002768(2, 10);
            if (Data_0200b394 != 0) {
                Func_02002780(3, 0, 10);
                Func_0808a130(3, 3);
                Func_02002768(3, 20);
            } else {
                SKIP_BEAT();
            }
            Func_0808a1e8(0, 258, 0);
            Func_0808a1e8(1, 258, 0);
            Func_0808a010(40);
            Func_0808a138(1, 2);
            Func_02002768(1, 20);
            Func_0808a1e8(1, 0x105, 0);
            Func_0808a010(120);
            Func_02002768(2, 40);
            if (Data_0200b394 != 0) {
                Func_02002780(3, 0x2000, 10);
                Func_0808a110(3, 4);
                Func_02002768(3, 10);
            } else {
                SKIP_BEAT();
            }
            Func_0808a010(60);
            Func_0808a138(2, 2);
            if (Data_0200b394 != 0) {
                Func_02002780(2, 0xa000, 40);
                Func_02002780(2, 0xe000, 20);
            }
            Func_02002768(2, 10);
            Func_0808a130(0, 2);
            Func_0808a138(1, 2);
            Func_0808a010(40);
            Func_02002768(2, 20);
            Func_0808a110(0, 3);
            Func_0808a110(1, 3);
            Func_0808a010(20);
            Func_0808a100(3, 3);
            goto shared_scene_tail;
        }

        goto alternate_scene_tail;

shared_scene_tail:
        Func_0808a110(2, 3);
        goto scene_close;

alternate_scene_tail:
        {
            Func_0808a1e8(2, 0x105, 0);
            Func_0808a010(40);
            Func_0808a110(2, 3);
            Func_0808a170(0x14bf);
            Func_02002768(2, 20);
            if (Data_0200b394 != 0) {
                Func_02002780(3, 0, 10);
                Func_0808a130(3, 1);
                Func_02002768(3, 20);
            } else {
                SKIP_BEAT();
            }
            Func_0808a1e8(1, 258, 0);
            Func_0808a1e8(0, 258, 0);
            Func_0808a010(40);
            Func_0808a138(1, 2);
            Func_02002768(1, 20);
            Func_0808a1e8(2, 0x105, 0);
            Func_0808a010(80);
            Func_02002768(2, 40);
            if (Data_0200b394 != 0) {
                Func_02002780(3, 0x2000, 20);
                Func_0808a100(3, 4);
                Func_02002768(3, 40);
            } else {
                SKIP_BEAT();
            }
            Func_0808a138(2, 2);
            Func_0808a010(20);
            Func_02002768(2, 20);
            Func_0808a130(1, 2);
            Func_0808a138(0, 2);
            Func_0808a010(40);
            Func_02002768(2, 20);
            goto scene_close;
        }
    }

scene_close:
    Func_080f9010(17);
    Func_0808a090(1, 0x00013333, 0x9999);
    Func_0808a090(2, 0x00013333, 0x9999);
    Func_0808a090(3, 0x00013333, 0x9999);

    Func_0808a100(1, 2);
    p = Func_0808a080(0);
    if (p != 0) {
        Func_0808a0b8(1, *(s16 *)((u8 *)p + 10), *(s16 *)((u8 *)p + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a100(2, 2);
    p = Func_0808a080(0);
    if (p != 0) {
        Func_0808a0b8(2, *(s16 *)((u8 *)p + 10), *(s16 *)((u8 *)p + 18));
    }
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);

    if (Data_0200b394 != 0) {
        Func_0808a100(3, 2);
        p = Func_0808a080(0);
        if (p != 0) {
            Func_0808a0b8(3, *(s16 *)((u8 *)p + 10), *(s16 *)((u8 *)p + 18));
        }
        Func_0808a0e8(3);
        Func_0808a0f0(3, 0, 0);
    }

    Func_080770c8(0x843);
    Func_0808a0f0(11, 0, 0);
    Func_0808a0f0(12, 0, 0);
    Func_0808a4f0();
    Func_0808a020();
}
