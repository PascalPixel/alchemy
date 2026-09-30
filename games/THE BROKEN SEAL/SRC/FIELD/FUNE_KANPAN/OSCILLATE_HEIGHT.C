#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

s32 SceneActor_OscillateHeightBetweenLimits(struct FieldActor *actor)
{
    s16 *flag = (s16 *)&actor->unknown_66;
    s32 val;
    s32 tmp;

    if (*flag != 0) {
        val = actor->y.fixed - (((u32)(Random_Next() << 15)) >> 16) - 0x8000;
        actor->y.fixed = val;
        if (val >= 0x40000)
            goto done;
        tmp = 0;
    } else {
        val = actor->y.fixed + (((u32)(Random_Next() << 15)) >> 16) + 0x8000;
        actor->y.fixed = val;
        if (val <= 0xC0000)
            goto done;
        tmp = 1;
    }
    *flag = tmp;
done:
    return 1;
}

s32 SceneActor_SetFacingFromSample(struct FieldActor *actor)
{
    u32 v = ((u32)(Random_Next() << 5)) >> 16;

    if (v == 6) {
        s32 t = 0xD0;
        actor->facing = t << 8;
    } else if (v == 9) {
        s32 t = 0xB0;
        actor->facing = t << 8;
    }
    return 1;
}
