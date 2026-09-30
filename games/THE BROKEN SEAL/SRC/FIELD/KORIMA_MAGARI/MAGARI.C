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
s32 IwramSignedDivide();

/* The board's records in the scene state, where the board setup points. */
extern s16 *gKorimaMagariRecords;
void Scene_PushBlockAlongRun(s16 *records);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                               s32 dest_y);
void State_StampRecordCells(s16 *records, s32 value);
void State_ApplyRectByLayoutSelector(void);

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

/* 0x02000efa */

/* 0x02000f08 */
u8 *SceneData_GetTable9170(void)
{
    return (u8 *)0x02009170;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable91d0(void)
{
    return (u8 *)0x020091d0;
}

u8 *SceneData_GetTable91e0(void)
{
    return (u8 *)0x020091e0;
}

u8 *SceneData_GetTable9240(void)
{
    return (u8 *)0x02009240;
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

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

/* 0x02000efa */

/* 0x02000f08 */
void Scene_CallHelper118c(void)
{
    Field_TryJumpForward();
}
