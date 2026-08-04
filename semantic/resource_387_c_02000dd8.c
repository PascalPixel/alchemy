#include "types.h"

struct SpawnDescriptor {
    s32 unused0, unused4;
    s32 positionX, positionY;
    s32 targetX, targetY;
    s16 kind, unused26;
    s32 unused28, unused32, callback;
};

extern u8 *Func_0808a080(s32 slot);
extern void Func_080000c0(s32 frames);
extern s32 Func_08000120(s32 angle);
extern s32 Func_08000118(s32 angle);
extern void Func_0808a090(s32 slot, s32 x, s32 z);
extern void Func_0808a0b8(s32 slot, s32 x, s32 z);
extern void Func_0808a0e8(s32 slot);
extern void Func_02000db8(u8 *object, s32 height);
extern void Func_080f9010(s32 sound);
extern void Func_080091f0(s32 x, s32 y, s32 z);
extern void Func_0200013c(s32 x, s32 y, s32 z, s32 field44,
                         s32 field48, s32 field4c, u32 flags,
                         struct SpawnDescriptor *extra);
extern void Func_0808a100(s32 slot, s32 mode);
extern void Func_080091f8(void);

/* Turn the actor, emit a radial burst, then launch its final effect. */
void Func_02000dd8(s32 slot)
{
    u8 *object = Func_0808a080(slot);
    u8 *state = *(u8 **)(object + 80);
    struct SpawnDescriptor finalEffect;
    s32 angle;

    object[85] = 0;
    for (angle = 0; angle < 18; angle++) {
        Func_080000c0(1);
        *(u16 *)(state + 30) += (s32)-0x100;
        *(s32 *)(object + 8) -= Func_08000120(*(u16 *)(state + 30)) / 2;
        *(u32 *)(object + 56) = 0x80000000u;
    }

    *(u32 *)(object + 108) = 0x02008da9;
    Func_0808a090(slot, 192 << 10, 192 << 9);
    Func_0808a0b8(slot, 160, 192);
    *(s32 *)(object + 72) = 0xcccc;
    object[85] = 3;
    object[34] = 0;
    Func_0808a0e8(slot);
    Func_02000db8(object, 128 << 14);

    Func_080f9010(188);
    Func_080091f0(160 << 11, 160 << 11, 128 << 9);
    Func_080f9010(141);
    Func_080091f0(-1, -1, 0xe666);

    for (angle = 0; angle <= 16; angle++) {
        s32 radialX = Func_08000120(angle << 12);
        s32 radialZ = Func_08000118(angle << 12);
        radialX -= radialX / 4;
        radialZ -= radialZ / 2;
        Func_0200013c(*(s32 *)(object + 8), *(s32 *)(object + 12),
                      *(s32 *)(object + 16), radialX, 0, radialZ, 0, 0);
    }

    *(s32 *)(object + 40) = 160 << 11;
    Func_0808a0b8(slot, 139, 196);
    Func_0808a0e8(slot);
    Func_02000db8(object, 128 << 14);

    *(u32 *)(object + 108) = 0;
    *(u16 *)(state + 30) = 128 << 5;
    finalEffect.kind = 214;
    finalEffect.positionX = 128 << 8;
    finalEffect.positionY = 0xcccc;
    finalEffect.targetX = 192 << 9;
    finalEffect.targetY = 0x13333;
    Func_0200013c(*(s32 *)(object + 8), *(s32 *)(object + 12),
                  *(s32 *)(object + 16), 0, 0, 0, 224 << 13,
                  &finalEffect);
    Func_080f9010(154);
    Func_0808a100(slot, 3);
    Func_080091f8();
}
