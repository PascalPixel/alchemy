#include "types.h"

/*
 * resource_3bc state-machine slot 5, complete 416-byte owner and nine-word
 * pool.  Its case-zero arm stages the central actor, runs two cue phases,
 * performs the bounded leftward glide, and publishes the scene byte at +498.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including repeated calls to the "same"
 * helper -- Func_0808a010, Func_0808a180, Func_0808a1f0, Func_020038dc,
 * Func_0808a1e8, Func_0808a090 and Func_020022c4 each recur with a
 * different veneer address per call site) -- declared/named as the literal
 * per-site targets, not the shared ultimate-destination symbol.
 */

extern s16 Data_02000240[];

extern void Func_02004bd6(void);                    /* Func_0200288c veneer */
extern void Func_02006d50(void);                    /* Func_0808a018 veneer */
extern s32 Func_02004dec(s32 actor, s32 slot);       /* Func_02002a94 veneer */
extern void Func_02006e16(s32 message);              /* Func_0808a170 veneer #1 */
extern void Func_02006e6a(s32 x, s32 z);             /* Func_0808a208 veneer */
extern void Func_02006e84(s32 x, s32 y, s32 z, s32 mode); /* Func_0808a210 veneer */
extern void Func_02006e90(void);                     /* Func_0808a218 veneer */
extern void Func_02006d86(s32 frames);               /* Func_0808a010 veneer #1 */
extern void Func_02006e56(s32 actor, s32 mode);      /* Func_0808a180 veneer #1 */
extern void Func_02006e5e(s32 actor, s32 mode);      /* Func_0808a180 veneer #2 */
extern void Func_020059ea(s32 actor, s32 x, s32 z);  /* Func_02003640 veneer */
extern void Func_02006de0(s32 actor, s32 x, s32 z);  /* Func_0808a090 veneer #1 */
extern void Func_020046b8(s32 actor, s32 x, s32 z);  /* Func_020022f4 veneer */
extern void Func_02006df6(s32 actor, s32 x, s32 z);  /* Func_0808a090 veneer #2 */
extern void Func_0200469e(s32 actor, s32 x, s32 z);  /* Func_020022c4 veneer #1 */
extern void Func_02006dd8(s32 frames);               /* Func_0808a010 veneer #2 */
extern void Func_02006ed0(s32 actor, s32 cue);       /* Func_0808a1f0 veneer #1 */
extern void Func_02006de6(s32 frames);               /* Func_0808a010 veneer #3 */
extern void Func_02005cd0(s32 actor);                /* Func_020038dc veneer #1 */
extern void Func_02006e6c(s32 actor, s32 mode);      /* Func_0808a100 veneer */
extern void Func_02006eee(s32 actor, s32 cue);       /* Func_0808a1f0 veneer #2 */
extern void Func_02006ef0(s32 actor, s32 cue, s32 frames); /* Func_0808a1e8 veneer #1 */
extern u8 *Func_02006e2e(s32 actor);                 /* Func_0808a080 veneer */
extern void Func_02006c72(s32 frames);               /* Func_080000c0 veneer (loop body) */
extern void Func_02006f20(s32 actor, s32 cue, s32 frames); /* Func_0808a1e8 veneer #2 */
extern void Func_02004710(s32 actor, s32 x, s32 z);  /* Func_020022c4 veneer #2 */
extern void Func_02006f14(s32 actor, s32 mode);      /* Func_0808a180 veneer #3 */
extern void Func_02005d36(s32 actor);                /* Func_020038dc veneer #2 */
extern void Func_02005c88(s32 actor);                /* Func_0200381c veneer */
extern void Func_02006f64(s32 actor, s32 mode);      /* Func_0808a200 veneer */
extern void Func_02004fcc(s32 actor, s32 slot);      /* Func_02002b50 veneer */
extern void Func_02006f3a(s32 message);              /* Func_0808a170 veneer #2 */
extern void Func_02006f52(s32 actor, s32 mode);      /* Func_0808a180 veneer #4 */
extern void Func_02005048(s32 state, s32 actor, s32 slot); /* Func_02002bac veneer */
extern void Func_02006ea8(void);                     /* Func_0808a020 veneer */

void Func_02002330(s32 actor)
{
    s32 state;
    s32 frames;
    u8 *record;

    if (Data_02000240[225] == 2) {
        Func_02004bd6();
        return;
    }

    Func_02006d50();
    state = Func_02004dec(actor, 5);

    if (state == 0) {
        Func_02006e16(0x20c3);
        Func_02006e6a(0x30000, 0x6000);
        Func_02006e84(0x4380000, -1, 0xa80000, 1);
        Func_02006e90();
        Func_02006d86(30);
        Func_02006e56(actor, 0);
        Func_02006e5e(actor, 0);
        Func_020059ea(0, 0x3d8, 0xb8);
        Func_02006de0(0, 0x18000, 0xc000);
        Func_020046b8(0, 0x3e0, 0xb8);
        Func_02006df6(0, 0x4ccc, 0x2666);
        Func_0200469e(0, 0x460, 0xb8);
        Func_02006dd8(120);
        Func_02006ed0(0, 0x101);
        Func_02006de6(120);
        Func_02005cd0(0);
        Func_02006e6c(0, 1);
        Func_02006eee(0, 0x100);
        Func_02006ef0(0, 0x105, 0);

        record = Func_02006e2e(0);
        for (frames = 119; frames >= 0; frames--) {
            if (*(s32 *)(record + 8) > 0x3e00000)
                *(s32 *)(record + 8) -= 0x13333;
            Func_02006c72(1);
        }

        Func_02006f20(0, 0x103, 60);
        Func_02004710(0, 0x460, 0xb8);
        Func_02006f14(actor, 0);
        Func_02005d36(0);
        ((u8 *)Data_02000240)[498] = 1;
        Func_02005c88(0);
        Func_02006f64(0, 0);
        Func_02004fcc(actor, 5);
    } else if (state == 1) {
        Func_02006f3a(0x20c2);
        Func_02006f52(actor, 0);
    }

    Func_02005048(state, actor, 5);
    Func_02006ea8();
}
