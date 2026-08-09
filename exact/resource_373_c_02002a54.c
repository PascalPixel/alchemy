#include "types.h"

/*
 * resource_373 entry driver at 0x02002a54, 604 bytes: code through the sole
 * `bx r1` return at 0x02002c76, followed by its fourteen-word literal pool at
 * 0x02002c78-0x02002caf.  The next recorded owner begins at 0x02002cb0.
 *
 * This is the overlay's scene dispatcher.  It reads the current scene number
 * from Data_02000240+450, selects and initialises actors 20/21/22, publishes
 * two behaviour callbacks, and gates the local scene routines on persistent
 * story flags.  Scene 16 has an early path which copies the two sub-selectors
 * at +0x205/+0x206 to Func_08015360 and then runs Func_020034c8.
 *
 * Call accounting: all 45 BL sites were resolved with
 * `overlay_call_targets.ts resource_373 2a54 2cb0`; the body below contains
 * the same calls in the same control-flow branches.  There is no call_via
 * dispatch and m2c_guard finds no truncating call-through-register idiom.
 *
 * Link-base witnesses elsewhere in this overlay establish 0x02008000.  Thus
 * 0x0200d72d is Func_0200572c + Thumb bit; 0x0200da95 and 0x0200da41 are
 * likewise published callbacks at file offsets 0x5a94 and 0x5a40.  Their
 * signatures are not established here, so the encoded odd addresses are kept.
 * 0x0200e248 is in-image data (file offset 0x6248), passed to the actor-22
 * setup service in the same argument register as in the reference.
 */

extern u8 Data_02000240[];

void Func_020039cc();
void Func_02005f42();
void Func_02006bca();
void Func_02008464();
void Func_020085e0();
void Func_020085e8();
void Func_020089c2();
void Func_020089e8();
void Func_020089f8();
void Func_02008a0a();
s32 Func_02008a20();
s32 Func_02008a2a();
void Func_02008a2c();
s32 Func_02008a66();
s32 Func_02008a84();
void Func_02008a8e();
s32 Func_02008a92();
u8 *Func_02008abc();
void Func_02008aa2();
s32 Func_02008ad0();
s32 Func_02008ada();
s32 Func_02008afe();
s32 Func_02008b26();
u8 *Func_02008b2c();
s32 Func_02008b32();
u8 *Func_02008b34();
s32 Func_02008b3c();
void Func_02008b7a();
void Func_02008b82();
s32 Func_02008b88();
u8 *Func_02008b9c();
s32 Func_02008b9e();
void Func_02008bac();
u8 *Func_02008bb0();
void Func_02008bb6();
void Func_02008bb8();
void Func_02008bc0();
s32 Func_02008bc4();
void Func_02008bc6();
s32 Func_02008bd0();
void Func_02008bee();
s32 Func_02008bf0();
void Func_02008d9e();
void Func_02008af8();

s32 Func_02002a54(void)
{
    u8 *actor;

    {
        u8 *scene = Data_02000240;

        if (*(s16 *)(scene + 450) == 16) {
            Func_02008a0a(scene[0x205], scene[0x206]);
            Func_02005f42();
            return 0;
        }
    }

    if (Func_02008a20(0xfd0) == 0) {
        if (Func_02008a2a(0x87a) == 0)
            Func_020085e0(26);
        else
            Func_020085e8(20);
    }

    Func_020089e8(2, 0x66, 0x54, 0x29, 2, 1);
    Func_020089f8(1, 0x66, 0x53, 0x29, 1, 1);

    actor = Func_02008abc(Func_02008a66(0x87a) != 0 ? 21 : 20);
    Func_02008a2c(actor, 0);

    if (Func_02008a84(0x314) != 0)
        *(s32 *)(actor + 8) = 0x016a0000; /* 181 << 17 */
    else if (Func_02008a92(0x316) != 0)
        *(s32 *)(actor + 8) = 0x018a0000; /* 197 << 17 */
    else
        *(s32 *)(actor + 8) = 0x017a0000; /* 189 << 17 */

    *(s32 *)(actor + 16) = 0x02480000;    /* 146 << 18 */
    *(s32 *)(actor + 12) = 0x00c00000;    /* 192 << 16 */
    Func_02008464();
    {
        u8 *field22;
        u8 *field55;
        s32 zero;

        zero = 0;
        field22 = actor + 0x22;
        *field22 = 3;
        field55 = actor + 0x55;
        *field55 = zero;
    }

    {
        s32 duration = 200;

        Func_020089c2((void *)0x0200da95, duration << 4);
    }

    if (Func_02008ad0(0x87a) != 0)
        goto middle_done;

    if (Func_02008ada(0x815) != 0) {
        actor = Func_02008b2c(21);
        /* The reference performs a second accessor call for the service. */
        Func_02008aa2(Func_02008b34(21), 0);
        *(s32 *)(actor + 24) = 0x28f;
        *(s32 *)(actor + 28) = 0x28f;
    }

    if (Func_02008afe(0x808) != 0) {
        Func_02008bac(15, 0, 0);
        Func_02008bb6(16, 0, 0);
        Func_02008bc0(17, 0, 0);
    }

    if (Func_02008b26(0x815) == 0) {

    /* Branches are written in image order: the flag-clear arm falls
     * through first, while the flag-set arm is the forward target. */
    if (Func_02008b32(0x109) == 0) {
        if (Func_02008b3c(0x823) != 0) {
            Func_02008bee(22, 0x01000000, 0x01c80000);
            *(void **)(Func_02008b9c(22) + 0x6c) = (void *)0x0200d72d;
            Func_02008bb8(22, (void *)0x0200e248);
        }
    } else {
        Func_02008bb0(22)[0x5b] = 0;
        Func_02008b82(0x241);
    }

    {
        u8 *scene = Data_02000240;

        if (*(s16 *)(scene + 450) != 16 &&
            Func_02008b88(0x87a) == 0)
            {
                s32 duration = 200;

                Func_02008a8e((void *)0x0200da41, duration << 4);
            }
    }
    }

    if (Func_02008b9e(0x308) == 0) {
        u8 *scene = Data_02000240;

        if (*(s16 *)(scene + 450) == 17) {
            Func_02006bca();
            Func_02008bc6(0x308);
        }
    }

middle_done:
    if (Func_02008bc4(0x109) != 0) {
        if (Func_02008bd0(0x204) != 0) {
            s32 arg4 = 20;
            s32 arg5 = 50;

            Func_02008b88(49, 53, 8, 4, arg4, arg5);
        }
        if (Func_02008bf0(0x210) != 0)
            Func_020039cc();
    }

    Func_02008d9e(170);
    Func_02008b7a();
    Func_02008af8(1);
    return 0;
}
