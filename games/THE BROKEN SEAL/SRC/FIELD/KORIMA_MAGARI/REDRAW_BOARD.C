#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The board's records in the scene state, where the board setup points. */
extern s16 *gKorimaMagariRecords;

void Scene_PushBlockAlongRun(s16 *records);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                               s32 dest_y);
void State_StampRecordCells(s16 *records, s32 value);
void State_ApplyRectByLayoutSelector(void);

/* FAKEMATCH: a call spelled through this wrapper sets r0 last of its
   arguments, after the constant; a direct call loads the records first. */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Redraw the board from its records: move the block along its run, restore
   the cell attributes, stamp the record cells and apply the layout's
   rectangle. */
void KorimaMagari_RedrawBoard(void)
{
    Scene_PushBlockAlongRun(gKorimaMagariRecords);
    Map_CopyCellAttributeRect(0, 0x40, 0x20, 0x20, 0, 0);
    Value2((s32 (*)())State_StampRecordCells, (s32)gKorimaMagariRecords, 0xff);
    State_ApplyRectByLayoutSelector();
}
