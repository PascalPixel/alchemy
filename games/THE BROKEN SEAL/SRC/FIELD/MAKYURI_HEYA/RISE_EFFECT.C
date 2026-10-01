#include "PROBE.H"

extern const s32 MakyuriHeya_RiseScript[];

void OverlayObject_SpawnKind24AtObject(u8 *src)
{
    u8 *obj;
    u8 *rec;

    obj = Object_Create(24, *(s32 *)(src + 8), *(s32 *)(src + 12), *(s32 *)(src + 16));
    if (obj == 0) {
        return;
    }

    rec = *(u8 **)(obj + 0x50);
    Object_SetScript(obj, MakyuriHeya_RiseScript);

    obj[0x55] = 0;
    obj[0x22] = 1;
    obj[0x23] = 2;

    if (rec != 0) {
        AnimationObjects_SelectAnimation(rec, 2);
        rec[0x26] = 0;
        rec[9] |= 0x0C;
    }
}

s32 SceneEffect_AdvanceAnchoredRiseFrame(struct AnchoredEffect *work)
{
    struct EffectAnchor *src = work->f104;
    s16 step = ++work->f100;
    s32 seed;

    if (step > 31) {
        return 0;
    }

    seed = Engine_MathSin(step << 10);
    work->f24 = seed;
    work->f28 = seed;
    work->f8 = src->f8;
    work->f12 += 0x10000;
    work->f16 = src->f16;
    return 1;
}

s32 SceneEffect_AdvanceScaleOverSixteenFrames(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n * 3) << 10;
    *(s32 *)(o + 28) = (n * 3) << 10;
    return 1;
}

s32 OverlayObject_StepScaleUpSixteenFrames(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n << 11) + 0x10000;
    *(s32 *)(o + 28) = (n << 11) + 0x10000;
    return 1;
}
