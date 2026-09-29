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

s32 SceneData_ReturnZero(void)
{
    return 0;
}

extern u8 KareiHeya_Exits[];

/* The exit table, the third entry the main image calls. */
u8 *KareiHeya_GetExits(void)
{
    return KareiHeya_Exits;
}
