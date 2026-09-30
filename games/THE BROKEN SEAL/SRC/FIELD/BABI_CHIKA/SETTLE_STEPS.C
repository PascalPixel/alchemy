#include "TYPES.H"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"
#include "CALL.H"

/* The floor height of each step, by the actor's step index. */
extern s32 Data_0200b350[];

/* Moves steps 10 and 11 to the floor heights their step indices name, then
 * marks the cell under each of actors 10 to 14 that has sunk below the
 * floor. */
void BabiChika_SettleSteps(s32 wait)
{
    u32 i;

    Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0x8000, 0x4000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 11, 0x8000, 0x4000);
    if (wait != 0) {
        Audio_PlayCue(180);
    }
    Object_SetPosition(Object_GetById(10), Object_GetById(10)->x.fixed,
                             Data_0200b350[(s16)Object_GetById(10)->unknown_64], Object_GetById(10)->z.fixed);
    Object_SetPosition(Object_GetById(11), Object_GetById(11)->x.fixed,
                             Data_0200b350[(s16)Object_GetById(11)->unknown_64], Object_GetById(11)->z.fixed);
    ObjectMotion_CommitCurrentPositionAndActivate(10);
    ObjectMotion_CommitCurrentPositionAndActivate(11);
    Object_GetById(10)->y.fixed = Data_0200b350[(s16)Object_GetById(10)->unknown_64];
    Object_GetById(11)->y.fixed = Data_0200b350[(s16)Object_GetById(11)->unknown_64];
    if (wait != 0) {
        Audio_PlayCue(0x121);
    }
    for (i = 0; i < 5; i++) {
        if (Object_GetById(i + 10)->y.fixed / 0x10000 < 0 && Object_GetById(i + 10)->y.fixed / 0x10000 > -30) {
            Map_CopyCellAttributeRect(4, 9, 1, 1, Object_GetById(i + 10)->x.fixed >> 20,
                                         Object_GetById(i + 10)->z.fixed >> 20);
        }
    }
    Battle_WaitMode0(wait);
}
