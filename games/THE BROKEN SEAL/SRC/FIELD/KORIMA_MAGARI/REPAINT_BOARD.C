#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The board's records and its layout in the scene state, where the board
   setup points; the layout is 0 or 1. */
extern s16 *gKorimaMagariRecords;
extern s16 *gKorimaMagariLayout;

void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                               s32 dest_y);
void State_StampRecordCells(s16 *records, s32 value);
void State_ApplyRectByLayoutSelector(void);

/* Repaint the board for its layout: its cells and attributes, then each
   record's piece, raised with its marker drawn in layout 1 and lowered in
   the other, and the record cells stamped.
   FAKEMATCH: forced temporaries; each copy's destination is set in a block
   of its own so it is built after the other arguments, and the zero the
   pieces take is held from before the loop. */
void Scene_RepaintBoardRecords(void)
{
    s32 zero;
    s16 *record = gKorimaMagariRecords;

    if (*gKorimaMagariLayout != 0) {
        { s32 f1 = 79; s32 g1 = 29; Engine_MapCopyCells(65, 53, 2, 1, f1, g1); }
        { s32 f2 = 15; s32 g2 = 28; Engine_MapCopyCells(65, 40, 2, 4, f2, g2); }
    } else {
        { s32 f3 = 79; s32 g3 = 25; Engine_MapCopyCells(65, 50, 2, 5, f3, g3); }
    }

    if (*gKorimaMagariLayout != 0) {
        { s32 f4 = 32; s32 g4 = 0; Engine_MapCopyCells(0, 32, 32, 32, f4, g4); }
        { s32 f5 = 64; s32 g5 = 0; Engine_MapCopyCells(32, 32, 32, 32, f5, g5); }
        { s32 f6 = 0; s32 g6 = 0; Map_CopyCellAttributeRect(0, 32, 32, 32, f6, g6); }
    } else {
        { s32 f7 = 32; s32 g7 = 0; Engine_MapCopyCells(0, 64, 32, 32, f7, g7); }
        { s32 f8 = 64; s32 g8 = 0; Engine_MapCopyCells(32, 64, 32, 32, f8, g8); }
        { s32 f9 = 0; s32 g9 = 0; Map_CopyCellAttributeRect(0, 64, 32, 32, f9, g9); }
    }

    if (record[0] != -1) {
        zero = 0;
        do {
            u8 *piece = *(u8 **)(record + 4);

            if (*gKorimaMagariLayout == 1) {
                Engine_ObjectSetAnimation((struct FieldActor *)piece, 4);
                piece[35] = 3;
                piece[85] = zero;
                *(s32 *)(piece + 12) = 0x1a0000;

                if (record[3] != 0) {
                    s32 col = record[1];
                    s32 row = record[2];
                    Engine_MapCopyCells(68, 40, 1, 4, col + 32, row);
                } else {
                    s32 col = record[1];
                    s32 row = record[2];
                    Engine_MapCopyCells(70, 40, 4, 1, col + 32, row);
                }
            } else {
                Engine_ObjectSetAnimation((struct FieldActor *)piece, 1);
                piece[35] = 1;
                piece[85] = 2;
                *(s32 *)(piece + 12) = zero;
            }
            record += 6;
        } while (record[0] != -1);
    }

    { s32 f10 = 10; s32 g10 = 50; Engine_MapCopyCells(70, 42, 1, 1, f10, g10); }

    if (*gKorimaMagariLayout == 1) {
        { s32 f11 = 0; s32 g11 = 0; Map_CopyCellAttributeRect(0, 32, 32, 32, f11, g11); }
        State_StampRecordCells(gKorimaMagariRecords, 254);
    } else {
        { s32 f12 = 0; s32 g12 = 0; Map_CopyCellAttributeRect(0, 64, 32, 32, f12, g12); }
        State_StampRecordCells(gKorimaMagariRecords, 255);
    }

    State_ApplyRectByLayoutSelector();
}
