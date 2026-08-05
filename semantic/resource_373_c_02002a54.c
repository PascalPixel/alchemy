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

void Func_02000d70();
void Func_020034c8();
void Func_02003fb0();
void Func_02005950();
void Func_02005b48();
void Func_080000c0();
void Func_080000d0();
void Func_08009128();
void Func_08009180();
void Func_080091c0();
void Func_080091e0();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
u8 *Func_0808a080();
void Func_0808a098();
void Func_0808a0f0();
void Func_0808a5e0();
void Func_08015360();

s32 Func_02002a54(void)
{
    s16 scene = *(s16 *)(Data_02000240 + 450);
    u8 *actor;
    s32 flag815;

    if (scene == 16) {
        Func_08015360(Data_02000240[0x205], Data_02000240[0x206]);
        Func_020034c8();
        return 0;
    }

    if (Func_080770c0(0xfd0) == 0) {
        if (Func_080770c0(0x87a) == 0)
            Func_02005b48(26);
        else
            Func_02005b48(20);
    }

    Func_08009180(2, 0x66, 0x54, 0x29, 2, 1);
    Func_08009180(1, 0x66, 0x53, 0x29, 1, 1);

    actor = Func_0808a080(Func_080770c0(0x87a) != 0 ? 21 : 20);
    Func_080091e0(actor, 0);

    if (Func_080770c0(0x314) != 0)
        *(s32 *)(actor + 8) = 0x016a0000; /* 181 << 17 */
    else if (Func_080770c0(0x316) != 0)
        *(s32 *)(actor + 8) = 0x018a0000; /* 197 << 17 */
    else
        *(s32 *)(actor + 8) = 0x017a0000; /* 189 << 17 */

    *(s32 *)(actor + 16) = 0x02480000;    /* 146 << 18 */
    *(s32 *)(actor + 12) = 0x00c00000;    /* 192 << 16 */
    Func_02005950();
    actor[0x55] = 0;
    actor[0x22] = 3;

    Func_080000d0((void *)0x0200da95, 2000);

    if (Func_080770c0(0x87a) == 0 && Func_080770c0(0x815) != 0) {
        actor = Func_0808a080(21);
        /* The reference performs a second accessor call for the service. */
        Func_080091e0(Func_0808a080(21), 0);
        *(s32 *)(actor + 24) = 0x28f;
        *(s32 *)(actor + 28) = 0x28f;
    }

    if (Func_080770c0(0x808) != 0) {
        Func_0808a0f0(15, 0, 0);
        Func_0808a0f0(16, 0, 0);
        Func_0808a0f0(17, 0, 0);
    }

    flag815 = Func_080770c0(0x815);
    if (flag815 == 0) {
        /* Branches are written in image order: the flag-clear arm falls
         * through first, while the flag-set arm is the forward target. */
        if (Func_080770c0(0x109) == 0) {
            if (Func_080770c0(0x823) != 0) {
                Func_0808a0f0(22, 0x01000000, 0x01c80000);
                actor = Func_0808a080(22);
                *(void **)(actor + 0x6c) = (void *)0x0200d72d;
                Func_0808a098(22, (void *)0x0200e248);
            }
        } else {
            actor = Func_0808a080(22);
            Func_080770d0(0x241);
            actor[0x5b] = 0;
        }
    }

    if (scene != 16 && Func_080770c0(0x87a) == 0)
        Func_080000d0((void *)0x0200da41, 2000);

    if (Func_080770c0(0x308) == 0 && scene == 17) {
        Func_02003fb0();
        Func_080770c8(0x308);
    }

    if (Func_080770c0(0x109) != 0) {
        if (Func_080770c0(0x204) != 0)
            Func_080091c0(49, 53, 8, 4, 20, 50);
        if (Func_080770c0(0x210) != 0)
            Func_02000d70();
    }

    Func_0808a5e0(170);
    Func_08009128();
    return 0;
    Func_080000c0(1);
}
