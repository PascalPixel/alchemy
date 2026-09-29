#include "STORY.H"

void SceneActor_UpdateObjectByCounterBits(u8 *obj)
{
    if ((gFrameCount & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((gFrameCount & 15) == 0) {
        WorldMap_CreateLinkedEffects(obj);
    }
}

/* Both frame-parity steps read the frame counter afresh at every use. */
void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    if ((*(volatile u32 *)&gFrameCount & 1) != 0) {
        Object_SetPartPalettes(obj, Engine_MathModulo((s32)(*(volatile u32 *)&gFrameCount >> 1), 6));
    }
    if ((*(volatile u32 *)&gFrameCount & 15) == 0) {
        WorldMap_CreateLinkedEffects(obj);
    }
}

void SceneState_RunSlotStepOnOddFrames(s32 arg0)
{
    if ((*(volatile s32 *)&gFrameCount & 1) != 0) {
        s32 slot = Engine_MathModulo((u32)*(volatile s32 *)&gFrameCount >> 1, 6);

        Object_SetPartPalettes(arg0, slot);
    }
}

void Effect_AnimateVerticalPositive(struct StoryVerticalEffectActor *effect)
{
    struct StoryVerticalEffectActor *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }

    vertical_amplitude = Math_Sin(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z + (0x10000 - vertical_amplitude) * 5 + 0x80000;
}

void Effect_AnimateVerticalNegative(struct StoryVerticalEffectActor_02004004 *effect)
{
    struct StoryVerticalEffectActor_02004004 *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }

    vertical_amplitude = Math_Sin(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = -vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z - (0x10000 - vertical_amplitude) * 5 + 0x100000;
}
