/* Contiguous unnamed state-owner run for resource_3c3. */

#include "types.h"

/*
 * Resource 3c3, dialogue bracket at 0x02000730 (124 bytes, 11 call sites).
 *
 * Complete owner: `push {lr}` at 0x02000730, `pop {r0} / bx r0` at 0x02000798.
 * The popped branch register IS r0, so it holds the return address and the
 * owner is `void`.  Bytes 0x0200079c-0x020007ab are the four-word literal
 * pool: 0x0000089f (the story flag 0x020003c4 sets on its way out),
 * 0x00002668 and 0x0000264e (two dialogue line ids), and 0x03001ebc (the
 * workspace pointer cell).
 *
 * SHARED TAIL, spelled with a label rather than per-arm copies.  The
 * `Func_0808a180(9, 0)` at 0x02000772 is reached from two paths — the
 * already-seen arm branches to it over the whole body (`b.n 0x02000772` at
 * 0x02000746), and the unseen arm falls into it after Func_0808a110.  There
 * are exactly THREE Func_0808a180 sites in the assembly; writing the tail once
 * per arm would inflate the multiset to four.  The third site sits in the
 * skip-beat arm at 0x02000790 and is genuinely distinct.
 *
 * The skip-beat counter is the documented idiom: `movs r3,#236 / lsls r3,#1`
 * (byte offset 472) off the 0x03001ebc workspace pointer, bumped by 2 here
 * against the sibling 0x020003c4's 1.  `Data_03001ebc` is a pointer CELL, so
 * `ldr r3,[pc] / ldr r2,[r3]` is one dereference.
 *
 * The Func_0808a070 guard is tested for `!= 0` at this site (`bne`) — the
 * prompt idiom appears with both polarities, so the comparison is read per
 * call site rather than assumed.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`; all 11 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_0808a180 x3, Func_0808a170 x2, and one each of GameFlag_IsSet,
 *   Func_0808a018, Func_0808a020, Func_0808a070, Func_0808a110,
 *   Func_0808a178.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_02000fd0();
s32 Func_02000fb6();
void Func_02001060();
void Func_02001068();
s32 Func_02001078();
s32 Func_02001008();
void Func_02001094();
void Func_0200106c();
void Func_020010a4();
void Func_020010be();
void Func_0200103a();
                        /* test a story flag (used in a condition) */
                        /* open a scripted scene */
                        /* close a scripted scene */
                        /* dialogue prompt; result selects the branch */
                        /* scene-presentation request */
                        /* show a dialogue line by id */
                        /* dialogue-line variant with a mode word */
                        /* wait for the slot's action to finish */

extern u8 *Data_03001ebc;

void Func_02000730(void)
{
    u8 *workspace;

    Func_02000fd0();

    if (Func_02000fb6(0x89f) != 0) {
        Func_02001060(0x2668);
        goto close;
    }

    Func_02001068(0x264e);
    {
        s32 arg1 = 0;
        s32 arg0 = 9;

        Func_02001078(arg0, arg1);
    }

    if (Func_02001008(0, 0) != 0) {
        goto skip;
    }

    Func_02001094(9, 0);
    Func_0200106c(9, 4);

close:
    Func_020010a4(9, 0);
    goto done;

skip:
    /* Skip-beat counter, two beats' worth. */
    workspace = Data_03001ebc;
    *(u16 *)(workspace + 472) += 2;
    Func_020010be(9, 0);

done:
    Func_0200103a();
}

#include "types.h"

extern void Func_0200104c(void);
extern void Func_020010d2(s32);
extern void Func_020010f4(s32, s32, s32);
extern void Func_020010de(s32, s32);
extern void Func_0200104a(u8 *, s32);
extern u8 *Func_02001084(s32);
extern void Func_02001038(s32);
extern void Func_020010f8(s32, s32);
extern void Func_02001064(u8 *, s32);
extern u8 *Func_0200109e(s32);
extern void Func_02001054(s32);
extern void Func_0200111a(s32, s32);
extern void Func_02001086(u8 *, s32);
extern u8 *Func_020010c0(s32);
extern void Func_02001074(s32);
extern void Func_02001134(s32, s32);
extern void Func_020010a0(u8 *, s32);
extern u8 *Func_020010da(s32);
extern void Func_02001090(s32);
extern void Func_0200112e(s32, s32, s32);
extern void Func_020010cc(s32);
extern void Func_020010f0(void);
void Func_020007ac(void)
{
    unsigned int beat;

    Func_0200104c();

    Func_020010d2(0x266d);
    Func_020010f4(10, 0, 10);

    beat = 0;
    do {
        Func_020010de(10, 0);
        Func_0200104a(Func_02001084(10), 1);
        Func_02001038(4);

        Func_020010f8(10, 15);
        Func_02001064(Func_0200109e(10), 0);
        beat++;
        Func_02001054(4);
    } while (beat <= 5);

    beat = 0;
    do {
        Func_0200111a(10, 0);
        Func_02001086(Func_020010c0(10), 1);
        Func_02001074(2);

        Func_02001134(10, 15);
        Func_020010a0(Func_020010da(10), 0);
        beat++;
        Func_02001090(2);
    } while (beat <= 11);

    Func_0200112e(10, 0, 0);

    Func_020010cc(0x897);

    Func_020010f0();
}

