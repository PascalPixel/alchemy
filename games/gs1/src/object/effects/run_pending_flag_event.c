#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/object/effects/run_pending_flag_event.h"

s32 GameFlag_IsSet(s32);
void GameFlag_Clear(s32);

void Motion_CamBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void WaitFrames(s32);
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);
void Battle_WaitMode0(s32 arg0);

extern s32 gCell[];

s32 ObjectEffect_RunPendingFlagEvent(void)
{
    s32 result = 0;
    s32 flag = 0x120;

    if (GameFlag_IsSet(flag)!= 0) {
        Obj_Do(24);
        GameFlag_Clear(flag);
        result = 1;
    } else {
        flag = 0x121;
        if (GameFlag_IsSet(flag)!= 0) {
            Obj_Do(23);
            GameFlag_Clear(flag);
            result = 2;
        } else {
            flag = 0x122;
            if (GameFlag_IsSet(flag)!= 0) {
                s32 id;
                void *obj;

                GameFlag_Clear(flag);
                id = gCell[125];
                obj = ObjectTable_Get(id);
                *(s32 *)((u8 *)obj + 12) += 0x00a00000;
                Motion_CamBounds(-1, -1, -1, 0);
                while (*(s32 *)((u8 *)obj + 12) + *(s32 *)((u8 *)obj + 40) >
                       *(s32 *)((u8 *)obj + 20)) {
                    WaitFrames(1);
                }
                Audio_PlayCue(159);
                *(s32 *)((u8 *)obj + 12) = *(s32 *)((u8 *)obj + 20);
                Object_SetMode(obj, 22);
                Battle_WaitMode0(15);
                Obj_Apply(id, 1);
                result = 3;
            }
        }
    }
    return result;
}
