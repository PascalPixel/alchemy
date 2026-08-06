#include "types.h"

struct SpawnDescriptor {
    s32 unused0, unused4;
    s32 positionX, positionY;
    s32 targetX, targetY;
    s16 kind, unused26;
    s32 unused28, unused32, callback;
};















/* Turn the actor, emit a radial burst, then launch its final effect. */
extern u8 * Func_02001f30(s32 slot);
extern void Func_02001e8a(s32 frames);
extern s32 Func_02001eac(s32 angle);
extern void Func_02001f8a(s32 slot, s32 x, s32 z);
extern void Func_02001f9c(s32 slot, s32 x, s32 z);
extern void Func_02001fd2(s32 slot);
extern void Func_02001c20(u8 *object, s32 height);
extern void Func_0200209a(s32 sound);
extern void Func_02001f50(s32 x, s32 y, s32 z);
extern void Func_020020ae(s32 sound);
extern void Func_02001f64(s32 x, s32 y, s32 z);
extern s32 Func_02001f40(s32 angle);
extern s32 Func_02001f46(s32 angle);
extern void Func_02001022(s32 x, s32 y, s32 z, s32 field44,
                         s32 field48, s32 field4c, u32 flags,
                         struct SpawnDescriptor *extra);
extern void Func_02002054(s32 slot, s32 x, s32 z);
extern void Func_0200207a(s32 slot);
extern void Func_02001cc8(u8 *object, s32 height);
extern void Func_0200108c(s32 x, s32 y, s32 z, s32 field44,
                         s32 field48, s32 field4c, u32 flags,
                         struct SpawnDescriptor *extra);
extern void Func_02002182(s32 sound);
extern void Func_020020e2(s32 slot, s32 mode);
extern void Func_0200203e(void);
void Func_02000dd8(s32 slot)
{
    u8 *object = Func_02001f30(slot);
    u8 *state = *(u8 **)(object + 80);
    struct SpawnDescriptor finalEffect;
    u32 angle;

    for (angle = 0; angle < 18; angle++) {
        Func_02001e8a(1);
        *(u16 *)(state + 30) += (s32)-0x100;
        *(s32 *)(object + 8) -= Func_02001eac(*(u16 *)(state + 30)) / 2;
        *(u32 *)(object + 56) = 0x80000000u;
    }
    object[85] = 0;

    Func_02001f8a(slot, 192 << 10, 192 << 9);
    object[34] = 0;
    *(s32 *)(object + 72) = 0xcccc;
    object[85] = 3;
    *(u32 *)(object + 108) = 0x02008da9;
    Func_02001f9c(slot, 160, 192);
    Func_02001fd2(slot);
    Func_02001c20(object, 128 << 14);

    Func_0200209a(188);
    Func_02001f50(160 << 11, 160 << 11, 128 << 9);
    Func_020020ae(141);
    Func_02001f64(-1, -1, 0xe666);

    for (angle = 0; angle <= 16; angle++) {
        s32 radialX = Func_02001f40(angle << 12);
        s32 radialZ = Func_02001f46(angle << 12);
        radialX -= radialX / 4;
        radialZ -= radialZ / 2;
        Func_02001022(*(s32 *)(object + 8), *(s32 *)(object + 12),
                      *(s32 *)(object + 16), radialX, 0, radialZ, 0, 0);
    }

    *(s32 *)(object + 40) = 160 << 11;
    Func_02002054(slot, 139, 196);
    Func_0200207a(slot);
    Func_02001cc8(object, 128 << 14);

    finalEffect.kind = 214;
    finalEffect.positionX = 128 << 8;
    *(u32 *)(object + 108) = 0;
    finalEffect.positionY = 0xcccc;
    *(u16 *)(state + 30) = 128 << 5;
    finalEffect.targetX = 192 << 9;
    finalEffect.targetY = 0x13333;
    Func_0200108c(*(s32 *)(object + 8), *(s32 *)(object + 12),
                  *(s32 *)(object + 16), 0, 0, 0, 224 << 13,
                  &finalEffect);
    Func_02002182(154);
    Func_020020e2(slot, 3);
    Func_0200203e();
}
