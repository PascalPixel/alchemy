#include "types.h"

/*
 * resource_39c owner at 0x02004c68, 732 bytes through its ten-word tail
 * pool.  This is the two-way actor/camera movement beat: actor 0 and actor
 * 13 are staged on opposite sides of the scene, the camera-space workspace
 * field is eased down to 0x04000000 and back up to 0x0488ffff, and the final
 * presentation calls close the beat.
 *
 * The saved-register prologue begins at 0x02004c68.  The sole interworking
 * return is pop {r0}/bx r0 at 0x02004f16-0x02004f18.  One alignment halfword
 * follows at 0x02004f1a; the ten directly referenced pool words occupy
 * 0x02004f1c-0x02004f43, and the next owner's prologue is 0x02004f44.
 * m2c_guard measures 690 code bytes through the return; the alignment and
 * pool make the admitted whole-owner span 0x2dc (732) bytes.
 *
 * The first loop's cadence gate is after its actor updates.  The second loop
 * is entered by a forward branch at 0x02004e68, so its first update happens
 * before the cadence gate at 0x02004e6a; the `first_pass` flag below preserves
 * that control-flow shape while keeping the static call order identical.
 * All 61 resolved call sites across 17 targets are represented in ordinary C.
 */

extern u8 *Data_03001e70;
extern volatile s32 Data_03001e40;

extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_08009128(void);
extern void Func_080000c0(s32 frames);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_080091a0(void);
extern void Func_080f9010(s32 cue);
extern void Func_0808a0d0(s32 actor, s32 x, s32 z);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_080091e0(void *actor, s32 arg1);
extern void Func_0808a0b8(s32 actor, s32 x, s32 z);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);

void Func_02004c68(void)
{
    s32 permuted_46;
    u8 *workspace = Data_03001e70;
    u8 *camera = workspace + 356;
    u8 *actor0;
    u8 *actor13;
    s32 step = 0x00009c28;
    s32 first_pass;

    *(s32 *)(camera + 12) = 0x04890000;
    *(s32 *)(camera + 28) = 0;

    actor0 = Func_0808a080(0);
    actor0[85] = 0;

    actor0 = Func_0808a080(0);
    *(s32 *)(actor0 + 16) += (s32)0xff770000;
    actor0 = Func_0808a080(0);
    *(s32 *)(actor0 + 64) = *(s32 *)(Func_0808a080(0) + 16);

    actor13 = Func_0808a080(13);
    actor13[85] = 0;
    Func_0808a0f0(13, 0x02a80000, 0x006e0000);
    actor13 = Func_0808a080(13);
    *(s32 *)(actor13 + 16) += (s32)0xff770000;
    actor13 = Func_0808a080(13);
    *(s32 *)(actor13 + 64) = *(s32 *)(Func_0808a080(13) + 16);

    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(60);
    Func_080091a0();
    Func_080f9010(223);

    /* Ease the world field down while both actors move by the same step. */
    for (;;) {
        *(s32 *)(camera + 12) -= step;

        actor0 = Func_0808a080(0);
        *(s32 *)(actor0 + 16) += step;
        actor0 = Func_0808a080(0);
        *(s32 *)(actor0 + 64) = *(s32 *)(Func_0808a080(0) + 16);

        actor13 = Func_0808a080(13);
        *(s32 *)(actor13 + 16) += step;
        actor13 = Func_0808a080(13);
        *(s32 *)(actor13 + 64) = *(s32 *)(Func_0808a080(13) + 16);

        if (*(s32 *)(camera + 12) <= 0x04000000) {
            break;
        }
        if ((Data_03001e40 & 15) == 0 && step > 0x00000ccb) {
            step -= 0x560;
        }
        Func_080000c0(1);
    }

    Func_08009128();
    *(s32 *)(camera + 12) = 0x04000000;
    Func_080000c0(2);

    actor0 = Func_0808a080(0);
    actor0[85] = 3;
    actor13 = Func_0808a080(13);
    *(s32 *)(actor13 + 16) = 0x006e0000;
    actor13 = Func_0808a080(13);
    permuted_46 = *(s32 *)(Func_0808a080(13) + 16);
    Func_0808a010(30);
    *(s32 *)(actor13 + 64) = permuted_46;

    Func_0808a0d0(0, 176 << 2, 146 << 2);
    actor0 = Func_0808a080(0);
    Func_0808a100(0, 6);
    Func_0808a010(6);
    *(s32 *)(actor0 + 68) = 0;
    Func_0808a100(0, 7);

    actor0 = Func_0808a080(0);
    *(s32 *)(actor0 + 48) = 192 << 10;
    actor0 = Func_0808a080(0);
    Func_080f9010(152);
    *(s32 *)(actor0 + 52) = 128 << 10;
    actor0 = Func_0808a080(0);
    *(s32 *)(actor0 + 40) = 128 << 11;
    Func_080091e0(Func_0808a080(0), 0);
    Func_0808a0b8(0, 176 << 2, 146 << 2);
    Func_0808a0e8(0);
    Func_080091e0(Func_0808a080(0), 1);
    actor0 = Func_0808a080(0);
    *(s32 *)(actor0 + 68) = 128 << 7;
    Func_0808a100(0, 6);
    Func_0808a010(6);
    Func_0808a1b8(0, 128 << 8, 30);
    Func_080f9010(223);

    /* The forward branch enters the update body before the cadence gate. */
    first_pass = 1;
    for (;;) {
        if (!first_pass) {
            if ((Data_03001e40 & 7) == 0 && step <= 0x0000cccc) {
                step += 0x1999;
            }
            Func_080000c0(1);
        }
        first_pass = 0;

        *(s32 *)(camera + 12) += step;
        actor13 = Func_0808a080(13);
        *(s32 *)(actor13 + 16) -= step;
        actor13 = Func_0808a080(13);
        *(s32 *)(actor13 + 64) = *(s32 *)(Func_0808a080(13) + 16);

        if (*(s32 *)(camera + 12) > (s32)0x0488ffff) {
            break;
        }
    }

    Func_08009180(45, 91, 40, 91, 5, 4);
    Func_080091c0(104, 34, 5, 4, 40, 34);
    *(s32 *)(camera + 12) = 0x04000000;
    Func_08009128();
    Func_080000c0(2);
    Func_0808a0f0(13, 0, 0);
    Func_0808a010(30);

    actor0 = Func_0808a080(0);
    actor0[35] = (u8)(actor0[35] | 1);
}
