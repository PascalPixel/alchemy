#include "TYPES.H"
#include "FIELD_EVENT.H"

struct DoorCells {
    const u16 *steps;
    u16 x;
    u16 y;
};

extern struct DoorCells KareiMachi_DoorCells[];

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Kalay door exit: freeze the area's actors, open the touched door with its cell animation, walk the leader out and leave through that exit. */
void KareiMachi_RunExitDoor(void)
{
    struct EventWork *event;
    struct FieldActor *actor;
    s16 *trigger;
    u32 i;
    s32 door;

    event = gEventWork;
    Engine_EventBegin();
    for (i = 8; i <= 65; i++) {
        actor = Engine_ActorGet(i);
        if (actor != NULL) {
            actor->motion_flags = 0;
        }
    }
    trigger = &event->touched_trigger;
    door = *trigger - 1;
    Engine_AudioPlayCue(158);
    {
        s32 x = KareiMachi_DoorCells[door].x;
        s32 y = KareiMachi_DoorCells[door].y;

        Engine_MapAnimateCells(KareiMachi_DoorCells[door].steps, x, y);
    }
    Call3(Engine_ActorSetSpeed, 0, 0x8000, 0x4000);
    Engine_ActorGet(0)->motion_flags = 0;
    Engine_ActorSetAnimation(0, 2);
    Call3(Engine_ActorCenterAndWalk, 0, 2, -8);
    Engine_EventWait(10);
    Engine_EventRequestExit(*trigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}
