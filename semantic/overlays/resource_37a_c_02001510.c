typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 37a scene entry at 0x02001510 (640 bytes, 0x02001510-0x0200178f).
 *
 * Complete owner: `push {r5, lr}` / `sub sp, #8` prologue at 0x02001510 and the
 * matching interworking return `add sp, #8 / pop {r5} / pop {r0} / bx r0` at
 * 0x02001758.  r0 holds the popped return address, so nothing is returned: the
 * owner is `void`.
 *
 * Pool map, derived by walking the owner's control flow from the prologue:
 *  - 0x0200155a-0x02001573 is a literal pool reached only by falling into it,
 *    which never happens: the bare `b.n 0x02001574` at 0x02001558 hops it.
 *    That branch is mid-call-setup — r1 = 1 is set at 0x02001556, before the
 *    hop, and is the second argument of the Func_0808a210 site at 0x0200157e.
 *    (This is the "carry register state ACROSS a pool" case; treating the hop
 *    as a fragment boundary would silently drop that argument.)
 *  - 0x02001760-0x0200178f is the trailing pool, past the epilogue.
 * Everything else is reached as an instruction.  Pool words:
 *   0x00000000, 0x0200ade4, 0x0200addc, 0x0200ade0, 0x0200adec, 0x00001001,
 *   0x023e0000, 0x00008010, 0x0200ade4, 0x0200addc, 0x0200ade0, 0x0200adec,
 *   0x0200ade8, 0x02008d9d, 0x02008ef9, 0x0200901d, 0x02009141, 0x03001ebc.
 *
 * LINK BASE.  resource_37a is linked at 0x02008000, and this owner is the
 * cheapest witness in the overlay: the four odd pool words 0x02008d9d,
 * 0x02008ef9, 0x0200901d and 0x02009141 are exactly Func_02000d9c + 1,
 * Func_02000ef8 + 1, Func_0200101c + 1 and Func_02001140 + 1 — four functions
 * whose byte-exact sources are already tracked as
 * assets/code/resource_37a_c_02000d9c.c, _02000ef8.c, _0200101c.c and
 * _02001140.c.  They are installed here through Func_080000d0 and removed
 * again through Func_080000d8, which is precisely the per-frame task ABI.
 * The even pool words 0x0200addc/0x0200ade0/0x0200ade4/0x0200ade8/0x0200adec
 * are therefore in-image DATA at file offsets 0x2ddc/0x2de0/0x2de4/0x2de8/
 * 0x2dec (parity test: odd = Thumb entry, even = data).  The overlay image is
 * writable EWRAM, so these cells are written, not const.
 *
 * All 61 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are not the callees.
 * sites=61, distinct_targets=22 (47 veneer, 14 overlay-local prologue), against
 * the inventory's calls=57 (that field counts distinct targets and is a known
 * low lower bound).  The overlay-local callees are Func_02001264 (once, the
 * scene setup converted alongside this file), Func_02000238 and Func_02000360
 * (five times each, the two halves of the five animation loops; both are
 * byte-exact in assets/code) and Func_020025fc (three times).
 *
 * Shape: this is the same scene-entry family as the byte-exact
 * assets/code/resource_37a_c_02001380.c — barrier, scene setup, camera and
 * layer configuration, five `do` loops that alternate Func_02000238 and
 * Func_02000360 with a shrinking frame delay (12, 5, 4, 3, 2) and a growing
 * iteration count (4, 6, 8, 10, 12), then the renderer call, the four task
 * installs/removals, the shared runtime defaults, and the close.  The loops are
 * `do`/`while` in assembly (`adds r5,#1` then `cmp` / `bne` back to the head).
 *
 * The four halfword cells at 0x0200ade4/0x0200addc/0x0200ade0/0x0200adec are
 * seeded to 0 at the top and then, after the first loop, reseeded from the
 * image's random source Func_080000f8 with the established scaling idiom
 * `((u32)(rand * 60) >> 16) + 20` (built in assembly as 16*x - x, then << 2,
 * then an unsigned >> 16).  The word at 0x0200ade8 is cleared with `str`.
 *
 * The store block at 0x02001734 is the shared runtime-defaults write also seen
 * in assets/code/resource_37a_c_02001380.c: the pointer table at 0x03001ebc
 * yields the runtime record, whose word at +0x1c0 becomes 0x100 and whose word
 * at +0x1c8 becomes 32.  In assembly r3 is a value (448 - 192 = 256) and then a
 * displacement (256 + 200 = 456 = 0x1c8); the two uses are unrelated despite
 * sharing the register.
 *
 * Uncertainties: Func_080000f8 is treated as the image's random source, which
 * is inference from the scaling idiom rather than proof; 0x00008010 and
 * 0x00001001 are opaque ids (neither is an address under the 0x02008000 base);
 * Func_0808a018, Func_0808a218, Func_0808a368 and Func_0808a370 are reached
 * with no argument register set by this owner.  Arities are taken from the
 * registers written before each site.
 */

/* In-image data cells, addressed here through their linked (base 0x02008000)
   addresses because that is exactly how the owner's pool spells them. */
#define SCENE_H0    (*(u16 *)0x0200addc)    /* file offset 0x2ddc */
#define SCENE_H1    (*(u16 *)0x0200ade0)    /* file offset 0x2de0 */
#define SCENE_H2    (*(u16 *)0x0200ade4)    /* file offset 0x2de4 */
#define SCENE_W     (*(u32 *)0x0200ade8)    /* file offset 0x2de8 */
#define SCENE_H3    (*(u16 *)0x0200adec)    /* file offset 0x2dec */

/* Overlay-local callees; the four task bodies are byte-exact in assets/code. */
void Func_02000238();
void Func_02000360();
void Func_02001264();
void Func_020025fc();
void Func_02000d9c(void);
void Func_02000ef8(void);
void Func_0200101c(void);
void Func_02001140(void);

/* Old-style declarations: arities vary per call site across this overlay. */
void Func_080000d0();
void Func_080000d8();
void Func_08009180();
void Func_0808a010();
void Func_0808a018();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a128();
void Func_0808a170();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a210();
void Func_0808a218();
void Func_0808a248();
void Func_0808a368();
void Func_0808a370();
void Func_080f9010();

/* Used for its return value. */
u32 Func_080000f8();

static u16 scene_timer(void)
{
    return (u16)(((u32)(Func_080000f8() * 60)) >> 16) + 20;
}

void Func_02001510(void)
{
    u8 **runtime_globals = (u8 **)0x03001ebc;
    u8 *runtime;
    s32 i;

    Func_0808a018();
    Func_02001264();

    SCENE_H2 = 0;
    SCENE_H0 = 0;
    SCENE_H1 = 0;
    SCENE_H3 = 0;

    Func_0808a170(0x1001);
    Func_0808a1b8(16, 0x4000, 20);              /* 128 << 7 */
    Func_0808a1e8(16, 256, 0);                  /* 128 << 1 */
    Func_0808a128(16, 6, 30);
    Func_0808a210(0x023e0000, -1, 0xae0000, 1); /* 174 << 16; r1 = 1 set
                                                   before the pool hop */
    Func_0808a218();
    Func_0808a010(30);
    Func_020025fc(0x8010, 20);

    i = 0;
    do {
        Func_080f9010(0xf6);
        Func_02000238();
        Func_0808a010(12);
        Func_080f9010(0xf6);
        i += 1;
        Func_02000360();
        Func_0808a010(12);
    } while (i != 4);

    Func_020025fc(0x8010, 6);

    SCENE_H2 = scene_timer();
    SCENE_H0 = scene_timer();
    SCENE_H1 = scene_timer();
    SCENE_H3 = scene_timer();
    SCENE_W = 0;

    Func_080000d0((void (*)(void))((s32)&Func_02000d9c | 1), 3200);
    Func_080000d0((void (*)(void))((s32)&Func_02000ef8 | 1), 3200);
    Func_080000d0((void (*)(void))((s32)&Func_0200101c | 1), 3200);
    Func_080000d0((void (*)(void))((s32)&Func_02001140 | 1), 3200);

    i = 0;
    do {
        Func_080f9010(0xf6);
        Func_02000238();
        Func_0808a010(5);
        Func_080f9010(0xf6);
        i += 1;
        Func_02000360();
        Func_0808a010(5);
    } while (i != 6);

    i = 0;
    do {
        Func_080f9010(0xf6);
        Func_02000238();
        Func_0808a010(4);
        Func_080f9010(0xf6);
        i += 1;
        Func_02000360();
        Func_0808a010(4);
    } while (i != 8);

    i = 0;
    do {
        Func_080f9010(0xf6);
        Func_02000238();
        Func_0808a010(3);
        Func_080f9010(0xf6);
        i += 1;
        Func_02000360();
        Func_0808a010(3);
    } while (i != 10);

    i = 0;
    do {
        Func_080f9010(0xf6);
        Func_02000238();
        Func_0808a010(2);
        Func_080f9010(0xf6);
        i += 1;
        Func_02000360();
        Func_0808a010(2);
    } while (i != 12);

    Func_08009180(45, 30, 34, 10, 4, 2);
    Func_0808a128(16, 6, 40);
    Func_020025fc(0x8010, 6);
    Func_0808a090(16, 0x20000, 0x10000);        /* 128 << 10, 128 << 9 */
    Func_0808a0d0(16, 576, 280);                /* 144 << 2, 140 << 1 */

    Func_080000d8((void (*)(void))((s32)&Func_02000d9c | 1));
    Func_080000d8((void (*)(void))((s32)&Func_02000ef8 | 1));
    Func_080000d8((void (*)(void))((s32)&Func_0200101c | 1));
    Func_080000d8((void (*)(void))((s32)&Func_02001140 | 1));

    runtime = runtime_globals[0];
    *(s32 *)(runtime + 0x1c0) = 0x100;
    *(s32 *)(runtime + 0x1c8) = 32;

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(4);
}
