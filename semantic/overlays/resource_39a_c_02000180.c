typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 39a overlay pair placement at 0x02000180.
 *
 * Complete owner: the two-stage `push {r5, r6, r7, lr}` / high-register
 * `push` prologue at 0x02000180 with `sub sp, #8`, and the matching
 * `add sp, #8 / pop {r3, r5, r6, r7} / ... / pop {r0} / bx r0` at
 * 0x02000240.  The popped word is the return address, so nothing is returned.
 * Two pool words follow the return and are data.  All ten distinct branch
 * targets in the row are placed.
 *
 * The eight bytes of frame hold the second and third arguments, which are
 * needed again after the calls have clobbered r1 and r2.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address is an import identity, not a place to disassemble.
 * That is the convention the byte-exact sources in this overlay already use
 * (`assets/code/resource_39a_c_02000030.c` declares `Func_02002442`), so
 * imports are named by the address their call site computes and their
 * interfaces are left open.  Declarations are old-style because one name is
 * reached with different argument counts.
 */

/* 0x02000240 is below the 0x02008000 link base, so it is a resident table.
 * The word at byte offset 500 spans halfword entries 250 and 251 and is read
 * as a whole 32-bit value here. */
extern s16 Data_02000240[];

/* Imports.  The two lookups are typed for their return value. */
void Func_08009080();
void Func_08009150();
void Func_08009158();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a080();
void Func_02002492();
void Func_020024b2();
void Func_020024ca();
void Func_020024d4();
void Func_020024de();
void Func_020024e8();
u8 *Func_020024f2();
u8 *Func_020024fa();
void Func_0200250a();
void Func_0200257e();

/* Both placements share this shape: a fixed-point coordinate is biased by the
 * caller's offset, truncated to its top twelve bits and re-biased by 0x80000. */
#define PLACE_02000180(base, delta) \
    ((((base) + (delta)) & (s32)0xfff00000) + 0x80000)

void Func_02000180(s32 subject, s32 x, s32 z)
{
    u8 *anchor;
    u8 *mover;
    s32 dx;
    s32 dz;

    anchor = Func_0808a080(*(s32 *)((u8 *)Data_02000240 + 500));
    mover = Func_0808a080(subject);
    /* No argument register is written before this branch. */
    Func_0808a018();

    dx = x << 16;
    dz = z << 16;

    *(s32 *)(anchor + 48) = 0x10000;
    *(s32 *)(anchor + 52) = 0x8000;
    Func_08009150(anchor,
                  PLACE_02000180(*(s32 *)(anchor + 8), dx),
                  *(s32 *)(anchor + 12),
                  PLACE_02000180(*(s32 *)(anchor + 16), dz));
    Func_08009080(anchor, 27);

    *(s32 *)(mover + 48) = 0x10000;
    *(s32 *)(mover + 52) = 0x8000;
    Func_08009150(mover,
                  PLACE_02000180(*(s32 *)(mover + 8), dx),
                  *(s32 *)(mover + 12),
                  PLACE_02000180(*(s32 *)(mover + 16), dz));

    /* The signs tested here are the original arguments reloaded from the
     * frame, not the shifted copies. */
    if (x < 0 || z < 0) {
        Func_08009080(mover, 4);
    } else {
        Func_08009080(mover, 3);
    }

    Func_08009158(anchor);
    Func_0808a020();
}
