#include "TYPES.H"
#include "SCENE_IDS.H"
#include "CALL.H"

void Engine_GameFlagClear();
void Engine_MapCopyCells();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsLayered();
void Engine_ActorSetPosition();
s32 Engine_MapObjectSetPosition();
s32 Engine_DisplayScrollStartHBlankDma();

extern s16 gGameState[][1];

/* Lamakan Desert entry: clear flag 0x201, lay out the map cells for the
 * entrance taken, park the scene actors and, outside the fourth area
 * (SceneId_RamakanSabaku4), start the heat-shimmer scroll. */
s32 RamakanSabaku_ApplyEntryState(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Engine_GameFlagClear(0x201);
    if (gGameState[224][0] == (s32)&SceneId_RamakanSabaku1) {
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 22, 7);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 8, 10);
        Engine_MapCopyCells(70, 68, 4, 2, 23, 21);
        Engine_MapCopyCellAttributes(70, 68, 4, 1, 23, 23);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 16, 42);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 36, 44);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 14, 55);
    } else {
        if (gGameState[224][0] != (s32)&SceneId_RamakanSabaku2) {
        } else {
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 42, 5);
            Engine_MapCopyCells(70, 68, 4, 2, 20, 11);
            Engine_MapCopyCellAttributes(70, 68, 4, 1, 20, 13);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 14, 12);
            Engine_MapCopyCells(70, 68, 4, 2, 56, 18);
            Engine_MapCopyCells(70, 68, 4, 2, 7, 22);
            Engine_MapCopyCellAttributes(70, 68, 4, 1, 7, 24);
            Engine_MapCopyCells(70, 68, 4, 2, 44, 23);
            Engine_MapCopyCellAttributes(70, 68, 4, 1, 44, 25);
            Engine_MapCopyCells(70, 68, 4, 2, 38, 24);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 26, 28);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 17, 35);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 50, 36);
            Engine_MapCopyCells(70, 68, 4, 2, 34, 43);
            Engine_MapCopyCellAttributes(70, 68, 4, 1, 34, 45);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 6, 46);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 27, 55);
            Engine_MapCopyCells(70, 68, 4, 2, 43, 56);
            goto clear_actors;
        }
        if (gGameState[224][0] == (s32)&SceneId_RamakanSabaku3) {
            Engine_MapCopyCellsLayered(69, 99, 4, 2, 8, 16);
            Engine_MapCopyCellsLayered(69, 99, 4, 2, 6, 20);
            Engine_MapCopyCellsLayered(69, 99, 4, 2, 10, 23);
            Engine_MapCopyCellAttributes(69, 99, 4, 2, 8, 14);
            Call6(Engine_MapCopyCellAttributes, 69, 99, 4, 2, 6, 18);
            Engine_MapCopyCellAttributes(69, 99, 4, 1, 6, 20);
            Engine_MapCopyCellAttributes(69, 99, 4, 2, 10, 21);
            Call6(Engine_MapCopyCells, 0, 121, 5, 7, 8, 32);
            Engine_MapCopyCells(0, 121, 5, 7, 43, 32);
            Engine_MapCopyCells(6, 120, 3, 1, 9, 5);
            Engine_MapCopyCells(9, 120, 3, 1, 44, 5);
            Engine_MapCopyCellAttributes(9, 0, 3, 3, 9, 6);
        }
    }
    clear_actors:;
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(13, 0, 0);
    v5 = 100;
    do {
        record = Engine_MapObjectSetPosition(v5, 0, 0);
        v5 = (v5 + 1);
    } while (v5 <= 107);
    if (gGameState[224][0] != (s32)&SceneId_RamakanSabaku4) {
        record = Value7(Engine_DisplayScrollStartHBlankDma, 0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
        return record;
    }
    return record;
}
