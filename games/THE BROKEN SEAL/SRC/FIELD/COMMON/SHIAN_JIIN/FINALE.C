#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"
extern u8 gEffectWork[];

void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1)
{
    u8 *globalCtx;

    GameFlag_Set(160 << 1);
    Psynergy_Begin(141, 1);
    globalCtx = *(u8 **)gEffectWork;
    Psynergy_SetTarget(arg0, arg1);
    globalCtx[0x23] = 0;
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Task_Wait(1);
}

void FieldScene_FinishSequence(void)
{
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Psynergy_PlayEffect(2);
    Psynergy_LowerHands();
}

void FieldScene_SpawnEightShots(void)
{
    struct Descriptor_02000484 descriptor;
    u8 *record;
    u32 i;

    record = Actor_Get(8);
    descriptor.field0 = 1;
    descriptor.field24 = 0x0119;
    descriptor.field28 = 0x0200d1d8;
    descriptor.field16 = 224 << 10;
    descriptor.field20 = 192 << 9;
    for (i = 0; i <= 7; i++) {
        Event_Wait(10);
        if (i & 1) {
            Audio_PlayCue(0x82);
        }
        Effect_Spawn(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + 0xffe80000, 0,
                      0x9999, 0, 0x00360001, (u8 *)&descriptor);
    }
    Event_Wait(60);
}

void FieldScene_SelectActorModeFromInputBit(s32 arg0)
{
    if ((*(u32 *)&gFrameCount >> 1) & 1) {
        Object_SetPalette(arg0, 10);
    } else {
        Object_SetPalette(arg0, 9);
    }
}

void FieldScene_RunParticleRain(void)
{
    struct Descriptor_020041ec descriptor;
    u8 *record;
    u32 i;
    s32 x;
    s32 y;
    s32 scale;

    Audio_PlayCue(0x83);
    *(u32 *)(((u8 *)Engine_ActorGet(8)) + 108) = (u32)FieldScene_SelectActorModeFromInputBit;
    Event_Wait(40);
    ColorBuffer_ApplySource(128 << 9, 0);
    ColorBuffer_ApplyTarget(0x205c54, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(40);
    Audio_PlayCue(0x83);
    *(u32 *)(((u8 *)Engine_ActorGet(2)) + 108) = (u32)FieldScene_SelectActorModeFromInputBit;
    Event_Wait(120);
    record = Actor_Get(8);
    descriptor.field0 = 1;
    descriptor.field4 = 2;
    descriptor.field24 = 0x011d;
    for (i = 0; i <= 63; i++) {
        if ((i & 3) == 0) {
            Audio_PlayCue(246);
        }
        x = *(s32 *)(record + 8)
            + ((((u32)(Random_Next() * 3) << 4) >> 16) << 16)
            + 0xfff40000;
        y = *(s32 *)(record + 12)
            + ((((u32)Random_Next() << 5) >> 16) << 16)
            + 0xfff00000;
        scale = (((u32)((u32)Random_Next() << 2) >> 16) << 15) + (128 << 8);
        Effect_Spawn(x, y, *(s32 *)(record + 16), 0,
                      scale, 0, 152 << 13, (u8 *)&descriptor);
        Task_Wait(2);
    }
    Audio_PlayCue(220);
    Event_Wait(30);
    ColorBuffer_ApplyTarget(128 << 9, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(40);
    *(u32 *)(((u8 *)Engine_ActorGet(8)) + 108) = 0;
    *(u32 *)(((u8 *)Engine_ActorGet(2)) + 108) = 0;
    Actor_SetChildValue(8, 0);
    Actor_SetChildValue(ACTOR_IVAN, 0);
}
