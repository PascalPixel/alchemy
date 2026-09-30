#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Once actor 10 has been pushed to column 42, drops it into place and opens
   the way below. */
void SceneState_RunActor13AtColumn42Setup(void)
{
    struct FieldActor *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Engine_ActorGet(10);
    Engine_EventBegin();
    Engine_EventPrepareSpeakers(0);
    if (obj->x >> 20 == 42) {
        Engine_EventWait(30);
        Engine_AudioPlayCue(188);
        obj->motion_flags = 0;
        val = 0xfffe0000;
        obj->unknown_14 = val;
        obj->y = val;
        Engine_GameFlagSet(0x200);
        a = 3;
        b = 5;
        Engine_MapCopyCellsTo(44, 117, 41, 117, a, b);
    }
    Engine_EventEnd();
}

