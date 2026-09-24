#include "TYPES.H"
#include "FIELD_EVENT.H"


extern const u16 *ImiruMura_ExitCellSteps[];
extern s16 ImiruMura_ExitCellPoints[][2];

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Imil door exit: freeze the area's actors, open the door touched (trigger 50 + exit) with its cell animation, walk the leader out and request that exit. */
void ImiruMura_RunExitDoor(void)
{
    struct EventWork *event;
    struct FieldActor *actor;
    u32 i;
    s32 exit;

    event = gEventWork;
    Engine_EventBegin();
    for (i = 8; i <= 65; i++) {
        actor = Engine_ActorGet(i);
        if (actor != NULL) {
            actor->motion_flags = 0;
        }
    }
    exit = (s16)(event->touched_trigger - 50);
    if (exit == 6) {
        Engine_AudioPlayCue(188);
    } else {
        Engine_AudioPlayCue(158);
    }
    {
        s32 x = ImiruMura_ExitCellPoints[exit - 1][0];
        s32 y = ImiruMura_ExitCellPoints[exit - 1][1];

        Engine_MapAnimateCells(ImiruMura_ExitCellSteps[exit - 1], x, y);
    }
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x8000, 0x4000);
    gEventWork->start_transition = 0x100;
    if (exit == 6) {
        Call3((void (*)())Engine_ActorSetSpeed, 0, 0x3333, 0x1999);
        Engine_ActorSetAnimation(0, 2);
        Engine_ActorSetSpritePriority(0, 3);
        Call3((void (*)())Engine_ActorSetDestinationOffset, 0, 0, -8);
    } else {
        Engine_ActorGet(0)->motion_flags = 0;
        Call3((void (*)())Engine_ActorCenterAndWalk, 0, 3, -16);
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(exit);
    Engine_EventEnd();
}
