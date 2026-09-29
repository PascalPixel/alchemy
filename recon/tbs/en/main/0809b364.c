/*
 * main:0809b364 BattleFx_UpdateDescendingParticlePositiveArc and
 * BattleFx_UpdateDescendingParticleNegativeArc - drafts; the range links as
 * disassembly (recon/tbs/raw/0809b364.s).
 *
 * Remaining: the reference loads the 1 it compares the battle mode with from
 * the literal pool and compares two registers, with the base threshold
 * computed after that load. Written as == 1 below, GCC compares with an
 * immediate and schedules the threshold first, which moves every register in
 * the head of both functions. The unit matched only while the 1 was a
 * link-time symbol named after its own value.
 */
#include "TYPES.H"

extern s16 gGameState[];

void Object_Destroy(void);

void BattleFx_UpdateDescendingParticlePositiveArc(void *arg0)
{
    u8 *object;
    u8 *source;
    s32 position;
    s32 threshold;
    s32 speed;

    object = arg0;
    threshold = *(s32 *)(object + 0x14) + 0xA0000;
    source = *(u8 **)(object + 0x68);
    if (gGameState[237] == 1)
        threshold = *(s32 *)(object + 0x14) + 0x40000;

    position = *(s32 *)(object + 0x0C);
    if (position <= threshold) {
        Object_Destroy();
        return;
    }

    speed = *(s32 *)(object + 0x18) + 0xC00;
    if (speed > 0x10000)
        speed = 0x10000;
    *(s32 *)(object + 0x18) = speed;
    *(s32 *)(object + 0x1C) = speed;
    *(s32 *)(object + 8) = *(s32 *)(source + 8);
    *(s32 *)(object + 0x0C) = position + (s32)0xFFFE0000;
    *(s32 *)(object + 0x10) =
        *(s32 *)(source + 0x10) + ((0x10000 - speed) * 5) + 0x90000;
}

void Object_Destroy(void);

void BattleFx_UpdateDescendingParticleNegativeArc(void *arg0)
{
    u8 *object;
    u8 *source;
    s32 position;
    s32 threshold;
    s32 speed;

    object = arg0;
    threshold = *(s32 *)(object + 0x14) + 0xA0000;
    source = *(u8 **)(object + 0x68);
    if (gGameState[237] == 1)
        threshold = *(s32 *)(object + 0x14) + 0x40000;

    position = *(s32 *)(object + 0x0C);
    if (position <= threshold) {
        Object_Destroy();
        return;
    }

    speed = *(s32 *)(object + 0x18) + 0xC00;
    if (speed > 0x10000)
        speed = 0x10000;
    *(s32 *)(object + 0x18) = speed;
    *(s32 *)(object + 0x1C) = -speed;
    *(s32 *)(object + 8) = *(s32 *)(source + 8);
    *(s32 *)(object + 0x0C) = position + (s32)0xFFFE0000;
    *(s32 *)(object + 0x10) =
        *(s32 *)(source + 0x10) - ((0x10000 - speed) * 5) + 0x100000;
}
