#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "RESOURCE_3A9.H"

enum ArrivalMessage {
    MSG_CAN_LIVE_IN_PEACE_IN = 0x1a8f,
    MSG_GOING_TOLBI_ALSO = 0x1ad7,
    MSG_PLEASE_FINISH_EATING_IF_TAKING = 0x1add,
    MSG_DO_KNOW_ABOUT_CONTINENT_SOUTH = 0x1ae3,
    MSG_OUR_INN_FEELS_EMPTY_NOW = 0x1afb
};

/* Table selection, dialogue and arrival scripts for resource_3a9. */
typedef struct Placement {
    u32 destination;
    u16 x;
    u16 y;
} Placement;

u8 *Object_GetById(s32);

/*
 * Clears the set of scene slots this sub-state leaves behind. Sub-state 16
 * takes the last arm even though it lies inside 9..17, so the test is not
 * written as a range. 0x911 is read as an event-flag id from its argument
 * position, and the six-argument call's argument meanings are not
 * established.
 */
void SceneState_ClearSlotsBySubState(void)
{
    s16 sub = gGameState.entrance;

    switch (sub) {
    case 3:
    {
        /* The last two arguments travel on the stack. */
        s32 fifth = 4;
        s32 sixth = 2;
        Map_CopyCellsTo(30, 14, 30, 16, fifth, sixth);
        return;
    }
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
        break;
    default:
        goto other;
    }

    /* sub is 9..15 or 17. */
    if (GameFlag_IsSet(0x911) != 0) {
        /* Nine distinct call sites, not a loop; the trailing 15 is out of
         * order and is kept that way. */
        Actor_Destroy(10);
        Actor_Destroy(11);
        Actor_Destroy(12);
        Actor_Destroy(13);
        Actor_Destroy(14);
        Actor_Destroy(17);
        Actor_Destroy(18);
        Actor_Destroy(19);
        Actor_Destroy(15);
    } else {
        Actor_SetChildValue(13, 2);
    }
    return;

other:
    if (GameFlag_IsSet(0x911) != 0) {
        Actor_Destroy(16);
        Actor_Destroy(17);
    }
}
