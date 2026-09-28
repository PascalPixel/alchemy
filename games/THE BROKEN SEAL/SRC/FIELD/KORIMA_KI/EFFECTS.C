#include "KORIMAKI.H"

s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame)
{
    frame->progress += 0x1EB8;
    if (frame->limit == 0x80000000) {
        if (frame->second_limit == frame->limit) {
            if (frame->third_limit == frame->second_limit) {
                Engine_ObjectDispatchRelease(frame);
            }
        }
    }
    return 1;
}

void PaletteScene_SpawnEffect(void)
{

    struct PaletteEffect *effect;
    struct EffectSprite *sprite;
    s32 phase;
    s32 effect_flags;
    s32 sprite_flags;
    s32 spawn_x = 0x01460000;
    s32 spawn_y = 0x00200000;
    s32 spawn_z = 0x00c00000;
    s32 target_x = 0x01460000;
    s32 target_z = 0x00f00000;

    phase = gFrameCount & 3;
    if (phase != 0) return;
    if (gKorimaKiSparkSound != 0) Audio_PlayCue(200);
    effect = (struct PaletteEffect *)Engine_ObjectCreate(26, spawn_x, spawn_y, spawn_z);
    if (effect == 0) return;
    sprite = effect->sprite;
    sprite->state = phase;
    effect_flags = 0xfe;
    effect_flags &= effect->flags;
    effect->flags = effect_flags;
    sprite_flags = ~12;
    sprite_flags &= sprite->flags;
    sprite_flags |= 4;
    sprite->flags = sprite_flags;
    effect->progress = 0x1999;
    effect->rate_x = 0x40000;
    effect->rate_y = 0x40000;
    effect->mode = phase;
    Object_SetAnimation(effect, 2);
    Engine_ObjectSetPosition((struct FieldActor *)effect, target_x, 0, target_z);
    Object_SetScript(effect, gKorimaKiEffectScript);
}

/* Steps the shared transition counter, firing at 0 and at 20 and wrapping at
 * 30. */
void PaletteScene_AdvanceTransition(void)
{
    s32 step = gKorimaKiTransitionStep;

    if (step == 0) {
        KorimaPalette_Restore(0);
        ColorBuffer_Interpolate(20);
    } else if (step == 20) {
        KorimaPalette_Restore(1);
        ColorBuffer_Interpolate(8);
    }
    step = gKorimaKiTransitionStep + 1;
    gKorimaKiTransitionStep = step;
    if (step == 30) {
        gKorimaKiTransitionStep = 0;
    }
}
