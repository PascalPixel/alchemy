#include "MAPCOPY.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE.H"

struct TileRun {
    s16 id;
    s16 x;
    s16 y;
    s16 vertical;
    s16 unused08;
    s16 unused0a;
};

struct Cell {
    u8 unk0;
    u8 unk1;
    u8 kind;
    u8 type;
};

extern s16 *gOv;
extern s16 *gOv2;
extern u16 *gOv3;
extern u8 gUnk[];
s32 Math_DivideSigned();

/* The board's records in the scene state, where the board setup points. */
extern u8 gKorimaMagariEntrances[];
extern u8 gKorimaMagariExits[];
extern u8 gKorimaMagariPlacements[];
extern u8 gKorimaMagariEvents[];
extern s16 *gKorimaMagariRecords;
void Scene_PushBlockAlongRun(s16 *records);
void State_StampRecordCells(s16 *records, s32 value);
void State_ApplyRectByLayoutSelector(void);

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

u8 *KorimaMagari_GetEntrances(void)
{
    return gKorimaMagariEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *KorimaMagari_GetExits(void)
{
    return gKorimaMagariExits;
}

u8 *KorimaMagari_GetPlacements(void)
{
    return gKorimaMagariPlacements;
}

u8 *KorimaMagari_GetEvents(void)
{
    return gKorimaMagariEvents;
}

/* Redraw the board from its records: move the block along its run, restore
   the cell attributes, stamp the record cells and apply the layout's
   rectangle. */
void KorimaMagari_RedrawBoard(void)
{
    Scene_PushBlockAlongRun(gKorimaMagariRecords);
    Map_CopyCellAttributeRect(0, 0x40, 0x20, 0x20, 0, 0);
    ((s32 (*)())State_StampRecordCells)((s32)gKorimaMagariRecords, 0xff);
    State_ApplyRectByLayoutSelector();
}

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

void Scene_CallHelper118c(void)
{
    Field_TryJumpForward();
}
