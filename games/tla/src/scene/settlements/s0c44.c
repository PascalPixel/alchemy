#include "local.h"

extern char SceneId_Ov690Room4;

void Object_SetModeById(u32 id, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void CenterOn(s32 id, s32 column, s32 offset)
{
    /* FAKEMATCH: forwarding through the inline sets the arguments up in their own order; the direct call negates the offset after the column (differ 2). */
    ObjectMotion_CenterOnColumn(id, column, offset);
}

void Func_02000c44(void)
{
    u32 i;

    Func_02002068();
    Func_02002150(0);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes rebuild the zero on every pass, and a for loop hoists it (differ 26 with the forwarders below as direct calls). */
    i = 8;
loop:
    {
        struct ObjectRuntime *object = Object_GetById(i);

        if (object != 0)
            object->motionFlags = 0;
    }
    i++;
    if (i <= 63)
        goto loop;
    if (gPartyState.scene == (s32)&SceneId_Ov690Room4)
        CopyCells(33, 30, 23, 14, 1, 2);
    else
        CopyCells(27, 29, 40, 16, 1, 2);
    Func_020021b0(158);
    Object_SetModeById(gPartyState.currentOwner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
    CenterOn(gPartyState.currentOwner, 2, -4);
    Battle_WaitMode0(6);
    Func_02002110(2);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02002070();
}
