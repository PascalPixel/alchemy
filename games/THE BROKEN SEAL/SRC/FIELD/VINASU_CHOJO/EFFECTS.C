/* Frame-driven effects. */
#include "CHOJO.H"

void SceneEffect_UpdateObjectByFrameParity(s32 a)
{
    if (*(s32 *)&gFrameCount & 2) {
        Object_SetPartPalettes(a, 7);
    } else {
        Object_SetPartPalettes(a, 0);
    }
    if (IwramUnsignedRemainder(*(s32 *)&gFrameCount, 15) == 0) {
        VinasuChojo_SpawnLinkedPairEffects(a);
    }
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)&gFrameCount;

    if (*p & 1) {
        Object_SetPartPalettes(a, IwramUnsignedRemainder(*p >> 1, 6));
    }
    if (IwramUnsignedRemainder(*p, 15) == 0) {
        VinasuChojo_SpawnLinkedPairEffects(a);
    }
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}
