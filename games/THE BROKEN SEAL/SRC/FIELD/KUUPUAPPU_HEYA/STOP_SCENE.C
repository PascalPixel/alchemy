#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"


struct Half {
    u16 v;
};
void FieldScene_ConfigurePairedActors();
void Map_SetViewBounds();

void KuupuappuHeya_StartActorStops(void)
{
    u32 i;
    s32 record;
    /* FAKEMATCH: a one-halfword struct keeps the zero a halfword pool constant. */
    struct Half zero;

    Engine_EventBegin();
    Camera_MoveTo(0xa80000, -1, 0x2900000, 1);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 248, 0x2b8);
    Actor_SetPosition(1, 0xf80000, 0x2b80000);
    Actor_SetPosition(2, 0xf80000, 0x2b80000);
    Actor_WalkTo(0, 200, 0x2b8);
    Actor_WalkTo(1, 248, 0x2c8);
    Actor_WalkToAndWait(2, 232, 0x2b8);
    Engine_ActorWaitForMove(1);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0x8000, 0);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetAnimation(1, 12);
    FieldScene_ConfigurePairedActors();
    Call4(Map_SetViewBounds, 0x300000, 0x2400000, 0x1200000, 0x2e00000);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0xc000, 0x6000);
    Actor_SetSpeed(24, 0x10000, 0x13333);
    Actor_SetSpeed(25, 0x18000, 0x18000);
    {
        u16 *timer = &KuupuappuHeya_StopTimer;

        zero.v = 0;
        *timer = zero.v;
    }
    FieldScheduler_AddOrUpdateCallback((s32)KuupuappuHeya_UpdateActorStops, 0xc94);
    GameFlag_Clear(0x1ff);
    Engine_EventEnd();
    Engine_AudioPlayCue(9);
}
