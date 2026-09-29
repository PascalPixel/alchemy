#include "TYPES.H"
#include "MAKYURI.H"
#include "FIELD_EFFECT.H"
#include "SCENE_IDS.H"
extern u8 MsgMakyuriWhoHonorsHeart[];

void Makyuri_SpawnLightObjects(s32 count, s32 base);
void Makyuri_ClearPalette(void);
void Makyuri_CyclePalette(void);
void BattleFx_StartFadeOverlay(s32 value);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
void UiText_ShowCenteredMessage(s32 message, s32 a1, s32 a2);
void MakyuriIriguchi_ArriveWithSparks(void);
void MakyuriIriguchi_CrossDoorway(void);
void MakyuriIriguchi_RunDoorScene(void);

/* An actor's lift: how fast it rises and what pulls it back. */
struct ActorLift {
    s32 gravity;
    s32 rise;
};

#define ACTOR_LIFT(actor) ((struct ActorLift *)(actor)->unknown_44)

/* Mercury Lighthouse entrance: set the blend, lights and palette cycle and
 * record the retreat point, then prepare the scene for the entrance the
 * party came through; through the sixth entrance the first time, a burst
 * of light lifts the leader in. */
s32 MakyuriIriguchi_ApplyEntryState(void)
{
    struct EffectOptions options;
    s32 velocity[3];
    struct FieldActor *actor;
    u32 i;
    s32 angle;
    s32 flag;

    /* FAKEMATCH: the do/while and the held values keep the blend constants
     * in registers ahead of their address loads, as in MAKYURI_HEYA. */
    do {
        s32 blend = 0x3f40;
        *(volatile u16 *)0x04000050 = blend;
    } while (0);
    {
        s32 alpha = 0x1010;
        *(volatile u16 *)0x04000052 = alpha;
    }
    Makyuri_SpawnLightObjects(21, (s32)gSceneState);
    GameFlag_Set(0x111);
    gGameState.retreat_entrance = 11;
    gGameState.retreat_scene = (s32)&SceneId_MakyuriHeya4;
    BattleFx_StartFadeOverlay(0);
    if (GameFlag_IsSet(0x875))
        Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
    else
        Makyuri_ClearPalette();
    gEventWork->start_transition = 0x204;
    switch (gGameState.entrance) {
    case 1:
        if (!GameFlag_IsSet(0x872))
            Event_RequestExit(20);
    case 2:
        Actor_Get(12)->scale_x = -0x10000;
        Actor_Get(13)->scale_x = -0x10000;
        Actor_Get(14)->scale_x = -0x10000;
        Task_Wait(1);
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        if (GameFlag_IsSet(0x875)) {
            Map_CopyCellAttributes(84, 5, 10, 7, 20, 5);
            Map_CopyCellAttributes(101, 5, 12, 7, 37, 5);
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
        Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
        if (GameFlag_IsSet(0x875)) {
            Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
            Map_Redraw();
            Task_Wait(1);
            Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
        }
        if (gGameState.entrance != 6)
            break;
        flag = GameFlag_IsSet(0x251);
        if (flag != 0)
            break;
        GameFlag_Set(0x251);
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        Map_Redraw();
        Task_Wait(1);
        Actor_Get(0)->y.fixed = 0x820000;
        ACTOR_LIFT(Actor_Get(0))->rise = 0x8000;
        ACTOR_LIFT(Actor_Get(0))->gravity = flag;
        Actor_Get(0)->motion_flags = flag;
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(30);
        Actor_Get(0)->motion_flags = 3;
        Audio_PlayCue(204);
        Event_Wait(24);
        actor = Actor_Get(0);
        options.palette = 7;
        for (i = 0; i <= 16; i++) {
            angle = i << 12;
            velocity[0] = Math_Cos(angle);
            velocity[1] = 0;
            velocity[2] = Math_Sin(angle);
            velocity[0] -= velocity[0] / 4;
            velocity[2] -= velocity[2] / 2;
            Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0], velocity[1],
                         velocity[2], EFFECT_USE_PALETTE | 1, &options);
        }
        Audio_PlayCue(188);
        Actor_SetAttachedEffect(0, 0x101);
        Object_SetModeById(0, 22);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_MapRenderWaitForValues();
        Actor_SetAttachedEffect(0, 0x100);
        ACTOR_LIFT(Actor_Get(0))->rise = 0x10000;
        ACTOR_LIFT(Actor_Get(0))->gravity = 0x4000;
        if (!GameFlag_IsSet(0x875)) {
            ColorBuffer_ApplySource(0x10000, 0);
            ColorBuffer_ApplyTarget(0x10003, 1);
            ColorBuffer_Interpolate(30);
            Event_WaitForScreen();
            Object_SetModeById(0, 1);
            Event_Wait(30);
            UiText_ShowCenteredMessage((s32)MsgMakyuriWhoHonorsHeart, 0, 0);
            ColorBuffer_ApplyTarget(0x10000, 0);
            ColorBuffer_Interpolate(30);
        }
        Event_End();
        break;
    case 18:
    case 19:
    case 20:
        MakyuriIriguchi_ArriveWithSparks();
    case 17:
        BattleFx_SetQueuedSoundAndPlay(170);
        break;
    case 25:
        Actor_SetChildValue(0, 15);
        Actor_SetSpriteFlags(Actor_Get(0), 0);
        Event_Begin();
        Map_Redraw();
        Task_Wait(1);
        gEventWork->start_transition = 0x100;
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(120);
        Event_RequestExit(50);
        Event_End();
        break;
    case 30:
        if (!GameFlag_IsSet(0x109))
            MakyuriIriguchi_CrossDoorway();
        else
            Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
        break;
    case 31:
        if (!GameFlag_IsSet(0x109))
            MakyuriIriguchi_RunDoorScene();
        break;
    }
    return 0;
}
