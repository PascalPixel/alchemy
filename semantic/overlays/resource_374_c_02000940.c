typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

/*
 * Resource 374 scene dispatcher at 0x02000940 (636 bytes, 53 call sites).
 *
 * Complete owner: prologue `push {r5, lr} ; sub sp, #8` at 0x02000940 and the
 * interworking return `movs r0, #0 ; add sp, #8 ; pop {r5} ; pop {r1} ;
 * bx r1` at 0x02000b80.  The popped return register is r1, not r0, so by the
 * epilogue rule r0 survives and IS the result: the owner returns the constant
 * 0.  Bytes 0x02000b8a-0x02000bbb are the literal pool (alignment,
 * 0x0000090b, 0x0000090c, 0x0000090d, 0x02000240, 0x0000087a, 0x00000834,
 * 0x0200ac00, 0x00000815, 0x00004ccc, 0x00009999, 0x014b0000, 0x00000842),
 * reached only by `ldr rN, [pc, #imm]`.  The next prologue at 0x02000bbc is a
 * separate owner.
 *
 * The eight-byte stack frame is not a local variable area: it carries the
 * fifth and sixth arguments of the two six-argument imports Func_08009180 and
 * Func_080091c0 (`str r3, [sp, #0]` / `str r2, [sp, #4]` immediately before
 * each call), which is why they are spelled with six arguments here.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`, never from the
 * disassembler's `bl` annotations: 49 import veneers and 4 prologues in this
 * overlay.  The four prologues are the scene bodies this dispatcher selects —
 * 0x020017c8, 0x020015b4, 0x02001084 (all in this reconstruction) and the byte-exact
 * 0x020025b0 — and they are the reason the printed targets (0x02002284,
 * 0x02002100, 0x02001bea, 0x02002f62) look like unrelated addresses.
 *
 * Link base 0x02008000: the pool word 0x0200ac00 is file offset 0x2c00, an
 * even in-image address, so it is data (a table passed to Func_0808a168) and
 * not a Thumb entry point.  0x02000240 is below the base and is the same
 * EWRAM progress table `semantic/overlays/resource_371_c_0200008c.c` reads;
 * this owner reads its signed halfword element 225 (byte offset 450) three
 * separate times, reloading it after each block that can change it.
 *
 * Control flow note: the `case 97` arm of the first dispatch is a direct
 * `b.n 0x02000b4a` into the middle of the later `state == 19` arm, so both
 * reach Func_020015b4 and then the common return.  That join is kept as a
 * `goto` rather than duplicated, because the two paths are literally the same
 * instructions.
 *
 * Uncertainties: Func_080770c0 is the story-flag query used the same way by
 * the byte-exact siblings and by 0x02000248; Func_0808a0f0 is reached nine
 * times with three arguments and Func_0808a080 seven times with one, so
 * old-style declarations are used for every import.
 */

/* EWRAM progress table (signed halfword elements). */
extern s16 Data_02000240[];

/* In-image data under the 0x02008000 link base. */
extern u8 Data_02002c00[];

/* Scene bodies selected by this dispatcher. */
void Func_02001084(void);
void Func_020015b4(void);
void Func_020017c8(void);
void Func_020025b0(void);

/* Main-image imports reached through this overlay's veneer table. */
void Func_080000c0();          /* frame wait */
void Func_08009128();
void Func_08009180();          /* six arguments, last two stack-carried */
void Func_080091c0();          /* six arguments, last two stack-carried */
void Func_080091e0();
s32 Func_080770c0();           /* story-flag query */
void Func_080770c8();
u8 *Func_0808a080();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a168();
void Func_0808a248();
void Func_0808a2c0();
void Func_0808a2c8();
void Func_0808a2e0();
void Func_0808a360();
void Func_0808a370();

s32 Func_02000940(void)
{
    s16 state;
    u8 *record;

    if (Func_080770c0(0x90b) != 0) Func_0808a0f0(8, 0, 0);
    if (Func_080770c0(0x90c) != 0) Func_0808a0f0(9, 0, 0);
    if (Func_080770c0(0x90d) != 0) Func_0808a0f0(10, 0, 0);

    state = Data_02000240[225];
    if (state == 98) {
        Func_080770c8(32);
        Func_0808a248(50);
        return 0;
    }
    if (state == 99) {
        Func_020025b0();
        return 0;
    }
    if (state == 97) goto scene_15b4;

    record = Func_0808a080(8);
    *(s32 *)(record + 28) = 0x18000;
    record = Func_0808a080(9);
    *(s32 *)(record + 28) = 0x18000;
    record = Func_0808a080(10);
    *(s32 *)(record + 28) = 0x18000;

    if (Func_080770c0(0x87a) != 0) {
        Func_08009180(97, 2, 80, 5, 2, 2);
        Func_08009180(42, 53, 42, 54, 3, 1);
        Func_08009128();
        Func_080000c0(1);
        return 0;
    }

    if (Func_080770c0(0x834) != 0) {
        Func_0808a2c0();
        Func_0808a2c8();
        Func_080091c0(21, 38, 1, 1, 18, 41);
        if (Func_080770c0(0x840) != 0) {
            Func_0808a0f0(17, 0, 0);
            Func_0808a0f0(18, 0, 0);
            Func_0808a168(19, 0x10000, Data_02002c00);
        }
    } else if (Func_080770c0(0x815) != 0) {
        Func_0808a0f0(16, 0x00b40000, 0x02380000);
        Func_08009180(92, 2, 80, 5, 2, 2);
        Func_08009180(42, 53, 42, 54, 3, 1);
        Func_08009128();
        Func_080000c0(1);
    }

    state = Data_02000240[225];
    if (state == 12) {
        Func_020017c8();
        return 0;
    }

    if (Func_080770c0(0x834) != 0) {
        record = Func_0808a080(20);
        *(s32 *)(record + 24) = 0x4ccc;
        *(s32 *)(record + 28) = 0x4ccc;
        Func_080091e0(Func_0808a080(20), 0);
        record = Func_0808a080(21);
        *(s32 *)(record + 24) = 0x9999;
        *(s32 *)(record + 28) = 0x9999;
        Func_0808a100(13, 5);
    } else if (Func_080770c0(0x815) != 0) {
        Func_0808a0f0(21, 0x014b0000, 0x00f90000);
        Func_080091e0(Func_0808a080(21), 0);
    }

    if (Func_080770c0(0x840) != 0) {
        Func_0808a0f0(26, 0, 0);
        Func_0808a0f0(22, 0, 0);
    }

    state = Data_02000240[225];
    if (state == 19) {
scene_15b4:
        Func_020015b4();
        return 0;
    }

    if (Func_080770c0(0x834) != 0 && Func_080770c0(0x842) != 0) {
        Func_02001084();
        return 0;
    }

    if (Func_080770c0(0x834) != 0) {
        Func_0808a360();
        Func_0808a370();
        Func_0808a2e0();
    }

    return 0;
}
