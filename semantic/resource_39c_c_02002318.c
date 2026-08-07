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

extern s32 Func_02000758(s32 *result);
/* 0x020008ec starts by reserving outgoing stack space before its push. */
extern void Func_020008ec(s32, s32, s32, s32, s32, s32);
extern void *Func_02000048(s32, s32, s32, s32);
extern void Func_02001db4(s32 variant);
extern void Func_02002030(s32 mode);
extern s32 Func_020021cc(void);
extern s32 Func_02002304(void);

extern void Func_080770c8(s32 flag);
extern void Func_080770d0(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 index);
extern void Func_0808a0a0(s32 index);
extern void Func_0808a0b8(s32 index, s32 x, s32 z);
extern void Func_0808a0e8(s32 index);
extern void Func_0808a248(s32 value);
extern void Func_0808a338(s32 value, s32 mode);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 soundId);

void Func_02002318(void)
{
    s32 placement[6];
    s32 priorX;
    s32 x;

    Func_0808a018();
    if (Func_02000758(placement) == 0) {
        goto finish;
    }

    if (placement[1] == 10) {
        Func_020008ec(placement[0], 10, placement[2], placement[3],
                      placement[4], placement[5]);
        if ((placement[4] >> 20) == 38) {
            Func_080770c8(0x318);
        } else {
            Func_080770d0(0x318);
        }
        goto finish;
    }

    if (placement[1] != 11) {
        goto finish;
    }

    priorX = *(s32 *)(Func_0808a080(11) + 8) >> 20;
    Func_020008ec(placement[0], placement[1], placement[2], placement[3],
                  placement[4], placement[5]);
    x = placement[2] >> 20;

    if (x == 47) {
        Func_080770c8(0x319);
        Func_080770d0(0x31a);
        Func_080770d0(0x31b);
        Func_020021cc();
        if (priorX == 54) {
            Func_02002030(0);
        } else if (priorX == 48) {
            Func_02002030(1);
        }
        Func_02001db4(2);
        goto wait_60;
    } else if (x == 48) {
        Func_080770c8(0x31a);
        Func_080770d0(0x31b);
        Func_080770d0(0x319);
        if (Func_020021cc() != 0) {
            s32 objectX = 210 << 18;

            Func_02002030(2);
            Func_02002304();
            Func_02001db4(1);
            Func_0808a0a0(9);
            Func_02000048(objectX, 0, 0x03120000, 223);
            Func_02000048(objectX, 0, 0x03320000, 223);
            Func_0808a0b8(9, 210 << 2, 186 << 2);
            Func_0808a010(5);
            Func_080f9010(0xbd);
            Func_0808a0e8(9);
            Func_0808a010(40);
            Func_080770c8(0x877);
            Func_0808a338(0x10000, 0);
            *(s32 *)(Data_03001ebc + 0x1c0) = 32;
            Func_0808a368();
            Func_0808a370();
            Func_0808a248(15);
            goto finish;
        }

        Func_02002030(2);
        Func_02002304();
        Func_02001db4(1);
        goto wait_60;
    } else if (x == 53) {
        Func_080770c8(0x31b);
        Func_080770d0(0x319);
        Func_080770d0(0x31a);
        Func_020021cc();
        Func_02002030(0);
        goto wait_60;
    }

    goto default_scene;

wait_60:
    Func_0808a010(60);
    goto finish;

default_scene:
    Func_080770d0(0x319);
    Func_080770d0(0x31a);
    Func_080770d0(0x31b);
    Func_020021cc();
    if (priorX == 47) {
        Func_02002030(2);
    } else if (priorX == 48) {
        Func_02002030(1);
    }
    Func_02001db4(0);
    Func_0808a010(60);

finish:
    Func_0808a020();
}
