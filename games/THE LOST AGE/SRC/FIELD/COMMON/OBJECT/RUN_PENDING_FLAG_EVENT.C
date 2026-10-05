#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EVENTWRK.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Motion_CamBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

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
