#include "TYPES.H"
#include "SCENE_IDS.H"
#include "CALL.H"

void Engine_GameFlagClear();
void Engine_GameFlagSet();
void Engine_MapCopyCells();
void Engine_MapCopyCellAttributes();
void Engine_ActorSetPosition();
s32 Engine_GameFlagIsSet();
void Engine_MapObjectSetPosition();
s32 BattleFx_EmitRandomParticle();
s32 DisplayScroll_DisarmHBlankDma();

extern s16 gGameState[][1];

/* Lamakan Desert: open the map cells and place the actors for the area
 * variant stored at +0x1c0 of the game state, then reset the map objects. */
s32 RamakanSabaku_ConfigureAreaLayout(void)
{
    s32 i;
    s32 record;

    Engine_GameFlagClear(0x200);
    Engine_GameFlagSet(0x201);
    if (gGameState[224][0] == (s32)&SceneId_RamakanSabaku1) {
        Engine_MapCopyCells(64, 126, 4, 2, 22, 7);
        Call6(Engine_MapCopyCells, 68, 126, 4, 2, 8, 10);
        Engine_MapCopyCells(72, 126, 4, 2, 23, 21);
        Engine_MapCopyCellAttributes(72, 126, 4, 2, 23, 22);
        Call6(Engine_MapCopyCells, 76, 126, 4, 2, 16, 42);
        Call6(Engine_MapCopyCells, 80, 126, 4, 2, 36, 44);
        Call6(Engine_MapCopyCells, 84, 126, 4, 2, 14, 55);
        Call3(Engine_ActorSetPosition, 9, 0x1900000, 0x16c0000);
    } else {
        if (gGameState[224][0] != (s32)&SceneId_RamakanSabaku2) {
            goto third_area;
        }
        Call6(Engine_MapCopyCells, 64, 126, 4, 2, 42, 5);
        Engine_MapCopyCells(68, 126, 4, 2, 20, 11);
        Call6(Engine_MapCopyCellAttributes, 68, 126, 4, 2, 20, 12);
        Engine_MapCopyCells(72, 126, 4, 2, 14, 12);
        Engine_MapCopyCells(76, 126, 4, 2, 56, 18);
        Engine_MapCopyCells(80, 126, 4, 2, 7, 22);
        Call6(Engine_MapCopyCellAttributes, 80, 126, 4, 2, 7, 23);
        Engine_MapCopyCells(84, 126, 4, 2, 44, 23);
        Engine_MapCopyCellAttributes(84, 126, 4, 2, 44, 24);
        Engine_MapCopyCells(88, 126, 4, 2, 38, 24);
        Call6(Engine_MapCopyCells, 92, 126, 4, 2, 26, 28);
        Call6(Engine_MapCopyCells, 96, 126, 4, 2, 17, 35);
        Call6(Engine_MapCopyCells, 100, 126, 4, 2, 50, 36);
        Engine_MapCopyCells(104, 126, 4, 2, 34, 43);
        Engine_MapCopyCellAttributes(104, 126, 4, 2, 34, 44);
        Call6(Engine_MapCopyCells, 108, 126, 4, 2, 6, 46);
        Call6(Engine_MapCopyCells, 112, 126, 4, 2, 27, 55);
        Engine_MapCopyCells(116, 126, 4, 2, 43, 56);
        Call3(Engine_ActorSetPosition, 9, 0x1600000, 0xcc0000);
        Call3(Engine_ActorSetPosition, 10, 0x2e00000, 0x18c0000);
        Call3(Engine_ActorSetPosition, 11, 0x900000, 0x17c0000);
        Call3(Engine_ActorSetPosition, 12, 0x2400000, 0x2cc0000);
        Call3(Engine_ActorSetPosition, 13, 0x2880000, 0x1980000);
    }
    goto reset_objects;
    third_area:;
    if (gGameState[224][0] == (s32)&SceneId_RamakanSabaku3) {
        Engine_MapCopyCells(64, 124, 4, 4, 8, 14);
        Engine_MapCopyCells(68, 124, 4, 4, 6, 18);
        Engine_MapCopyCellAttributes(68, 124, 4, 1, 6, 20);
        Call6(Engine_MapCopyCells, 72, 124, 4, 4, 10, 21);
        Call6(Engine_MapCopyCells, 10, 121, 5, 7, 8, 32);
        Engine_MapCopyCells(5, 121, 5, 7, 43, 32);
        Call6(Engine_MapCopyCells, 0, 120, 3, 1, 9, 5);
        Engine_MapCopyCells(3, 120, 3, 1, 44, 5);
        Call3(Engine_ActorSetPosition, 8, 0xa80000, 0x5c0000);
        Call3(Engine_ActorSetPosition, 9, 0x800000, 0x13c0000);
        Engine_MapCopyCellAttributes(6, 0, 3, 3, 9, 6);
        if (Engine_GameFlagIsSet(0x90a) == 0) {
            Engine_MapCopyCells(0, 119, 3, 1, 9, 5);
        }
    }
    reset_objects:;
    for (i = 100; i <= 107; i++) {
        Call3(Engine_MapObjectSetPosition, i, -1, -1);
    }
    record = BattleFx_EmitRandomParticle();
    if (gGameState[224][0] != (s32)&SceneId_RamakanSabaku4) {
        record = DisplayScroll_DisarmHBlankDma();
        return record;
    }
    return record;
}
