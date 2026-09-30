#include "GOMA.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "CALL.H"

void Map_CopyCellAttributeRect();
void Object_SetModeById();

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

/* The waterway's scene start: open with the window transition; in the
   first area, entering by the fifth entrance clears flag 0x12f, and
   otherwise actor 8 is set up, placed where flag 0x864 records it. */
s32 FieldScene_PlaceActor8OnEntry(void)
{
    struct FieldActor *record;

    gEventWork->start_transition = 0x204;
    if (gGameState.scene == (s32)&SceneId_GomaSuiro1) {
        if (gGameState.entrance == 5) {
            Engine_GameFlagClear(0x12f);
        } else {
            SetFlagBits((u8 *)Object_GetById(8) + 89, 16);
            if (Engine_GameFlagIsSet(0x864) != 0) {
                Engine_ActorSetPosition(8, 0x15a0000, 0x1240000);
                record = Object_GetById(8);
                Engine_ActorSetSpriteFlags(record, 0);
                *((u8 *)Object_GetById(8) + 35) |= 2;
                Object_SetModeById(8, 2);
                Call6(Map_CopyCellAttributeRect, 19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}

