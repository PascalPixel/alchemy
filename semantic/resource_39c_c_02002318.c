#include "types.h"

/*
 * resource_39c owner at 0x02002318, 492 bytes through its literal pool:
 * process one placement result, keep three mutually-exclusive scene flags in
 * step with its X tile, and run the matching scene transition.
 *
 * The six-word placement record is produced by Func_02000758.  Its first two
 * words select the placement shape and object, words 2-4 are fixed-point X/Y/Z
 * coordinates, and word 5 is the optional callback passed to the placement
 * routine.  Selector 10 only updates flag 792 from the resulting Z tile.
 * Selector 11 uses the X tile to choose one of three scene variants, with a
 * longer object-and-camera sequence when the middle variant's dialogue check
 * succeeds.
 *
 * The complete owner starts with push {r5, r6, lr} at 0x02002318 and reaches
 * its sole interworking return at 0x020024da-0x020024e4.  Alignment at
 * 0x020024e6 and the seven referenced pool words at 0x020024e8-0x02002503
 * extend the owner to the next code at 0x02002504.  The bounded call sweep
 * resolves all 52 direct calls across 21 targets, with no call-through-
 * register site.
 */

extern u8 *Data_03001ebc;


/* 0x020008ec starts by reserving outgoing stack space before its push. */






















extern void Func_02007dd2(void);
extern s32 Func_02002a7e(s32 *result);
extern void Func_02002c38(s32, s32, s32, s32, s32, s32);
extern void Func_02007de8(s32 flag);
extern void Func_02007dfa(s32 flag);
extern u8 * Func_02007e42(s32 index);
extern void Func_02002c72(s32, s32, s32, s32, s32, s32);
extern void Func_02007e20(s32 flag);
extern void Func_02007e2e(s32 flag);
extern void Func_02007e34(s32 flag);
extern s32 Func_02004570(void);
extern void Func_020043de(s32 mode);
extern void Func_020043ea(s32 mode);
extern void Func_02004174(s32 variant);
extern void Func_02007e58(s32 flag);
extern void Func_02007e66(s32 flag);
extern void Func_02007e6c(s32 flag);
extern s32 Func_020045a8(void);
extern void Func_02004416(s32 mode);
extern s32 Func_020046f0(void);
extern void Func_020041a6(s32 variant);
extern void Func_02007ee6(s32 index);
extern void * Func_0200244e(s32, s32, s32, s32);
extern void * Func_0200245a(s32, s32, s32, s32);
extern void Func_02007f14(s32 index, s32 x, s32 z);
extern void Func_02007ed2(s32 frames);
extern void Func_02008090(s32 soundId);
extern void Func_02007f4e(s32 index);
extern void Func_02007ee4(s32 frames);
extern void Func_02007eca(s32 flag);
extern void Func_0200802c(s32 value, s32 mode);
extern void Func_02008056(void);
extern void Func_02008062(void);
extern void Func_02008030(s32 value);
extern void Func_0200449c(s32 mode);
extern s32 Func_02004774(void);
extern void Func_0200422a(s32 variant);
extern void Func_02007f0e(s32 flag);
extern void Func_02007f1c(s32 flag);
extern void Func_02007f22(s32 flag);
extern s32 Func_0200465e(void);
extern void Func_020044c8(s32 mode);
extern void Func_02007f4a(s32 frames);
extern void Func_02007f3a(s32 flag);
extern void Func_02007f40(s32 flag);
extern void Func_02007f46(s32 flag);
extern s32 Func_02004682(void);
extern void Func_020044f0(s32 mode);
extern void Func_020044fc(s32 mode);
extern void Func_02004286(s32 variant);
extern void Func_02007f84(s32 frames);
extern void Func_02007f98(void);
void Func_02002318(void)
{
    s32 placement[6];
    s32 priorX;
    s32 x;

    Func_02007dd2();
    if (Func_02002a7e(placement) == 0) {
        goto finish;
    }

    if (placement[1] == 10) {
        Func_02002c38(placement[0], 10, placement[2], placement[3],
                      placement[4], placement[5]);
        if ((placement[4] >> 20) == 38) {
            Func_02007de8(0x318);
        } else {
            Func_02007dfa(0x318);
        }
        goto finish;
    }

    if (placement[1] != 11) {
        goto finish;
    }

    priorX = *(s32 *)(Func_02007e42(11) + 8) >> 20;
    Func_02002c72(placement[0], placement[1], placement[2], placement[3],
                  placement[4], placement[5]);
    x = placement[2] >> 20;

    if (x == 47) {
        Func_02007e20(0x319);
        Func_02007e2e(0x31a);
        Func_02007e34(0x31b);
        Func_02004570();
        if (priorX == 54) {
            Func_020043de(0);
        } else if (priorX == 48) {
            Func_020043ea(1);
        }
        Func_02004174(2);
        goto wait_60;
    } else if (x == 48) {
        Func_02007e58(0x31a);
        Func_02007e66(0x31b);
        Func_02007e6c(0x319);
        if (Func_020045a8() != 0) {
            s32 objectX = 210 << 18;

            Func_02004416(2);
            Func_020046f0();
            Func_020041a6(1);
            Func_02007ee6(9);
            Func_0200244e(objectX, 0, 0x03120000, 223);
            Func_0200245a(objectX, 0, 0x03320000, 223);
            Func_02007f14(9, 210 << 2, 186 << 2);
            Func_02007ed2(5);
            Func_02008090(0xbd);
            Func_02007f4e(9);
            Func_02007ee4(40);
            Func_02007eca(0x877);
            Func_0200802c(0x10000, 0);
            *(s32 *)(Data_03001ebc + 0x1c0) = 32;
            Func_02008056();
            Func_02008062();
            Func_02008030(15);
            goto finish;
        }

        Func_0200449c(2);
        Func_02004774();
        Func_0200422a(1);
        goto wait_60;
    } else if (x == 53) {
        Func_02007f0e(0x31b);
        Func_02007f1c(0x319);
        Func_02007f22(0x31a);
        Func_0200465e();
        Func_020044c8(0);
        goto wait_60;
    }

    goto default_scene;

wait_60:
    Func_02007f4a(60);
    goto finish;

default_scene:
    Func_02007f3a(0x319);
    Func_02007f40(0x31a);
    Func_02007f46(0x31b);
    Func_02004682();
    if (priorX == 47) {
        Func_020044f0(2);
    } else if (priorX == 48) {
        Func_020044fc(1);
    }
    Func_02004286(0);
    Func_02007f84(60);

finish:
    Func_02007f98();
}
