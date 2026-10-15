#include "garoh_interiors.h"

void GameFlag_ClearBit(s32 flag);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_EnableActionAndSetCallback(s32 id, s32 action);
void Func_02000710(void);
void Func_020007dc(s32 value);
void Func_0200081c(s32 id, s32 x, s32 z);
void Func_02000824(s32 id, s32 x, s32 z, s32 angle);
void Func_0200087c(s32 value);
void Func_02000894(s32 id, s32 angle);

s32 Func_02000580(void)
{
    s32 turn;

    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
    Func_0200087c(0);
    if (gPartyState.scene == (s32)&SceneId_Ov660Room1)
    {
        if (GameFlag_Test(0x91b) || gPartyState.entrance == 99)
        {
            Func_0200081c(19, 0, 0);
            Func_0200081c(20, 0, 0);
            ObjectMotion_EnableActionAndSetCallback(18, 3);
        }
        else
        {
            {
                s32 x;
                s32 z;
                s32 id;

                turn = 0x2000;
                x = 153;
                id = 17;
                /* FAKEMATCH: and this forced id keeps its load right after that half; unforced it loads last. */
                asm volatile("" : "+l"(id));
                x <<= 17;
                z = 335 << 16;
                Func_02000824(id, x, z, turn);
            }
            {
                s32 angle = 0x6000;
                s32 z = 336 << 16;

                Func_02000824(18, 345 << 16, z, angle);
            }
            ObjectDispatch_SetSingleChildFlags(Object_GetById(19), 0);
            ObjectDispatch_SetSingleChildFlags(Object_GetById(20), 0);
            Object_GetById(17)->motionFlags = 0;
            Object_GetById(17)->y = 0;
            Object_GetById(18)->motionFlags = 0;
            Object_GetById(18)->y = 0;
            Object_GetById(19)->motionFlags = 0;
            Object_GetById(19)->y = 0;
            Object_GetById(20)->motionFlags = 0;
            Object_GetById(20)->y = 0;
            Func_02000894(17, 0xe000);
            ObjectDispatch_SetSingleChildFlags(Object_GetById(17), 0);
            Func_02000894(18, turn);
            ObjectDispatch_SetSingleChildFlags(Object_GetById(18), 0);
        }
        if (gPartyState.entrance == 99 && !GameFlag_Test(0x91b))
            Func_02000710();
    }
    if (GameFlag_Test(0x91b))
        GameFlag_ClearBit(0x310);
    if (GameFlag_Test(0x310))
        Func_020007dc(0xa00);
    return 0;
}
