#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"

void Main_080091b8(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void KorimaMagari_PlaceObjects();
s32 State_StampRecordCells(u16 *records, s32 value);
void Scene_RepaintBoardRecords(void);
void Effect_AdjustPaletteColors(s32 amount);

extern u16 *Data_020092c0;
extern u16 *Data_020092c4;
extern u16 *Data_020092c8;
extern u16 Data_0200911c[];
extern u16 Data_02001000[];

/* Set up the board: point the three state cells into the buffer at 0x02001000,
 * copy the map cells, reset the board records unless flag 0x109 is set, place
 * the objects and actors, and fade the palette unless flag 0x845 is set. */
s32 KorimaMagari_Func02000980(void)
{
    struct FieldActor *actor;

    /* FAKEMATCH: the one-pass loop keeps the third cell store ahead of the
     * shared zero. */
    do {
        Data_020092c4 = Data_02001000;
        Data_020092c8 = Data_02001000 + 1;
        Data_020092c0 = Data_02001000 + 2;
    } while (0);
    Main_080091b8(32, 0, 64, 32, 0, 64);
    Engine_MapCopyCellAttributes(0, 0, 32, 32, 0, 64);
    Engine_MapCopyCellAttributes(32, 0, 32, 32, 0, 32);
    if (Engine_GameFlagIsSet(0x109) == 0) {
        Dma_Set(Data_0200911c, Data_020092c0, 0x84000012, (volatile u32 *)0x040000d4);
        *Data_020092c4 = 0;
        *Data_020092c8 = 1;
    }
    KorimaMagari_PlaceObjects(Data_020092c0);
    State_StampRecordCells(Data_0200911c, 255);
    Scene_RepaintBoardRecords();
    Engine_ActorSetAnimation(9, 0);
    Engine_ActorGet(9)->motion_flags = 0;
    actor = Engine_ActorGet(10);
    actor->radius = 8;
    actor->scale_x = 0xc000;
    actor->scale_y = 0xc000;
    gEventWork->start_transition = 0x204;
    if (Engine_GameFlagIsSet(0x845) == 0)
        Effect_AdjustPaletteColors(4);
    return 0;
}
