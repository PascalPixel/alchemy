#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "RESOURCE_3A9.H"

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
