#include "object_lookup.h"
#include "types.h"

#define ObjectEffect_RunPendingFlagEvent Func_08094428

s32 GameFlag_IsSet(s32);
void GameFlag_Clear(s32);
void Func_08094380(s32 arg0);
void ObjectMotion_PlaceWithinCameraBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void WaitFrames(s32);
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);
void BattleRuntime_WaitIfModeZero(s32 arg0);
void Func_0809335c(s32 arg0, s32 arg1);
extern s32 Data_02000240[];

s32 ObjectEffect_RunPendingFlagEvent(void)
{
    s32 result = 0;
    s32 flag = 0x120;

    if (GameFlag_IsSet(flag) != 0) {
        Func_08094380(24);
        GameFlag_Clear(flag);
        result = 1;
    } else {
        flag = 0x121;
        if (GameFlag_IsSet(flag) != 0) {
            Func_08094380(23);
            GameFlag_Clear(flag);
            result = 2;
        } else {
            flag = 0x122;
            if (GameFlag_IsSet(flag) != 0) {
                s32 id;
                void *obj;

                GameFlag_Clear(flag);
                id = Data_02000240[125];
                obj = ObjectTable_Get(id);
                *(s32 *)((u8 *)obj + 12) += 0x00a00000;
                ObjectMotion_PlaceWithinCameraBounds(-1, -1, -1, 0);
                while (*(s32 *)((u8 *)obj + 12) + *(s32 *)((u8 *)obj + 40) >
                       *(s32 *)((u8 *)obj + 20)) {
                    WaitFrames(1);
                }
                Audio_PlayCue(159);
                *(s32 *)((u8 *)obj + 12) = *(s32 *)((u8 *)obj + 20);
                Object_SetMode(obj, 22);
                BattleRuntime_WaitIfModeZero(15);
                Func_0809335c(id, 1);
                result = 3;
            }
        }
    }
    return result;
}
