#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"

void Engine_MapCopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void KorimaMagari_PlaceObjects();
s32 State_StampRecordCells(u16 *records, s32 value);
void Scene_RepaintBoardRecords(void);
void Effect_AdjustPaletteColors(s32 amount);

extern u16 *gKorimaMagariRecords;
extern u16 *gKorimaMagariReturned;
extern u16 *gKorimaMagariLayout;
extern u16 KorimaMagari_DefaultRecords[];

/* Set up the board: point the three state cells into the scene state, copy
 * the map cells, reset the board records unless flag 0x109 is set, place the
 * objects and actors, and fade the palette unless flag 0x845 is set. */
s32 KorimaMagari_SetupBoard(void)
{
    struct FieldActor *actor;

    /* FAKEMATCH: the one-pass loop keeps the third cell store ahead of the
     * shared zero. */
    do {
        gKorimaMagariReturned = (u16 *)gSceneState;
        gKorimaMagariLayout = (u16 *)gSceneState + 1;
        gKorimaMagariRecords = (u16 *)gSceneState + 2;
    } while (0);
    Engine_MapCopyCells(32, 0, 64, 32, 0, 64);
    Map_CopyCellAttributeRect(0, 0, 32, 32, 0, 64);
    Map_CopyCellAttributeRect(32, 0, 32, 32, 0, 32);
    if (Engine_GameFlagIsSet(0x109) == 0) {
        Dma_Set(KorimaMagari_DefaultRecords, gKorimaMagariRecords, 0x84000012, (volatile u32 *)0x040000d4);
        *gKorimaMagariReturned = 0;
        *gKorimaMagariLayout = 1;
    }
    KorimaMagari_PlaceObjects(gKorimaMagariRecords);
    State_StampRecordCells(KorimaMagari_DefaultRecords, 255);
    Scene_RepaintBoardRecords();
    Engine_ActorSetAnimation(9, 0);
    Object_GetById(9)->motion_flags = 0;
    actor = Object_GetById(10);
    actor->radius = 8;
    actor->scale_x = 0xc000;
    actor->scale_y = 0xc000;
    gEventWork->start_transition = 0x204;
    if (Engine_GameFlagIsSet(0x845) == 0)
        Effect_AdjustPaletteColors(4);
    return 0;
}
