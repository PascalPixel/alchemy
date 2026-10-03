#include "EVENTWRK.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void ObjectEffect_EndContextEffect(s32 effect);
void Motion_CamBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Object_AttachWorkTargetToObject(s32 object_id, s32 mode);

/* Answers the first raised context-effect flag: 0x120 ends effect 24,
   0x121 ends effect 23, and 0x122 drops the current party member from
   above onto the ground. Returns which flag it handled, or 0. */
s32 ObjectEffect_RunPendingFlagEvent(void)
{
    s32 result = 0;
    s32 flag = 0x120;

    if (GameFlag_Test(flag)!= 0) {
        ObjectEffect_EndContextEffect(24);
        GameFlag_ClearBit(flag);
        result = 1;
    } else {
        flag = 0x121;
        if (GameFlag_Test(flag)!= 0) {
            ObjectEffect_EndContextEffect(23);
            GameFlag_ClearBit(flag);
            result = 2;
        } else {
            flag = 0x122;
            if (GameFlag_Test(flag)!= 0) {
                s32 id;
                struct ObjectRuntime *object;

                GameFlag_ClearBit(flag);
                id = gPartyState.current_owner;
                object = ObjectTable_Get(id);
                object->y += 0x00a00000;
                Motion_CamBounds(-1, -1, -1, 0);
                while (object->y + object->velocity_y > object->terrain_height) {
                    WaitFrames(1);
                }
                Audio_PlayCue(159);
                object->y = object->terrain_height;
                Object_SetMode(object, 22);
                EventRuntime_Wait(15);
                Object_AttachWorkTargetToObject(id, 1);
                result = 3;
            }
        }
    }
    return result;
}
