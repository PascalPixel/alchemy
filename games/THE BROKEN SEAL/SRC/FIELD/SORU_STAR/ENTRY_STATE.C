#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

void SoruStar_SetupElementalRings();
void Func_0200227c();
void Scene_EnterStarRoom();

/* Elemental Star Room entry: set flag 0x144, show actors 15..24 above the
 * floor and start their ring, then restore the actors and the opened cells
 * each story flag records. */
s32 SoruStar_ApplyEntryState(void)
{
    u32 i;
    s32 record;
    s32 base5_f;

    Engine_ColorBufferApplySource(0x10000, 0);
    Engine_GameFlagSet(0x144);
    base5_f = 15;
    do {
        *(u8 *)((u8 *)Object_GetById(base5_f) + 89) = 0;
        Engine_ActorSetSpritePriority(base5_f++, 1);
    } while ((u32)base5_f <= 24);
    SoruStar_SetupElementalRings(15, 16);
    if (Value1(Engine_GameFlagIsSet, 0x83b) != 0) {
        Call3(Engine_ActorSetPosition, 9, 0x1c80000, 0x1680000);
        Call3(Engine_ActorSetPosition, 5, 0x1b80000, 0x15a0000);
    }
    if (Engine_GameFlagIsSet(0x83c) != 0) {
        Engine_MapCopyCellsTo(0, 40, 43, 66, 3, 3);
        Engine_MapCopyCellsTo(83, 40, 96, 29, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 29);
        Call6(Engine_MapCopyCellsTo, 87, 42, 41, 31, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 74, 29, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 19, 29);
        Engine_MapCopyCellsTo(87, 42, 19, 31, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 96, 10, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 41, 10);
        Engine_MapCopyCellsTo(87, 42, 41, 12, 1, 2);
    }
    if (Engine_GameFlagIsSet(0x83d) != 0) {
        Engine_MapCopyCellsTo(0, 40, 43, 46, 3, 3);
        Engine_MapCopyCellsTo(83, 40, 84, 4, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 29, 4);
        Call6(Engine_MapCopyCellsTo, 87, 42, 29, 6, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 76, 21, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 21, 21);
        Engine_MapCopyCellsTo(87, 42, 21, 23, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 76, 29, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 21, 29);
        Engine_MapCopyCellsTo(87, 42, 21, 31, 1, 2);
    }
    if (Engine_GameFlagIsSet(0x83e) != 0) {
        Engine_MapCopyCellsTo(0, 40, 13, 66, 3, 3);
        Engine_MapCopyCellsTo(83, 40, 65, 31, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 10, 31);
        Call6(Engine_MapCopyCellsTo, 87, 42, 10, 33, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 79, 9, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 24, 9);
        Engine_MapCopyCellsTo(87, 42, 24, 11, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 91, 10, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 36, 10);
        Engine_MapCopyCellsTo(87, 42, 36, 12, 1, 2);
        Func_0200227c();
    }
    if (Engine_GameFlagIsSet(0x83b) == 0) {
        if (gGameState.entrance == 10) {
            Scene_EnterStarRoom();
        }
    }
    return 0;
}
