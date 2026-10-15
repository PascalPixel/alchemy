#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_work.h"
#include "types.h"
#include "system.h"
#include "object_runtime.h"
#include "party_state.h"

s32  GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Motion_CamBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ObjectEffect_RunPendingFlagEvent(void)
{
    s32 result = 0;
    s32 flag = 0x120;

    if (GameFlag_Test(flag) != 0)
    {
        ObjectEffect_EndContextEffect(24);
        GameFlag_ClearBit(flag);
        result = 1;
    }
    else
    {
        flag = 0x121;
        if (GameFlag_Test(flag) != 0)
        {
            ObjectEffect_EndContextEffect(23);
            GameFlag_ClearBit(flag);
            result = 2;
        }
        else
        {
            flag = 0x122;
            if (GameFlag_Test(flag) != 0)
            {
                s32                   id;
                struct ObjectRuntime *object;

                GameFlag_ClearBit(flag);
                id = gPartyState.currentOwner;
                object = ObjectTable_Get(id);
                object->y += 0x00a00000;
                Motion_CamBounds(-1, -1, -1, 0);
                while (object->y + object->velocityY > object->terrainHeight)
                {
                    WaitFrames(1);
                }
                Audio_PlayCue(159);
                object->y = object->terrainHeight;
                Object_SetMode(object, 22);
                EventRuntime_Wait(15);
                Object_AttachWorkTargetToObject(id, 1);
                result = 3;
            }
        }
    }
    return result;
}

u8 *ResourceMetadata_RegisterFar(void *resource, s32 id);

void Func_080d5fd4(void)
{
    struct ObjectRuntime *object = ObjectTable_Get(gPartyState.currentOwner);
    u8 *metadata = ResourceMetadata_RegisterFar(*(void **)((u8 *)object + 0x50), 268);

    metadata[5] = 15;
}
