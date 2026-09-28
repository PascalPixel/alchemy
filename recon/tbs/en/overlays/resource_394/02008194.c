/* Draft of Scene_RepaintBoardRecords, resource_394 at 0x02008194 (split from FIELD/KORIMA_MAGARI/PRESET_LAYOUT.C).
 * Remaining difference: its C does not compile to the game's instructions (it differs beyond relocations), so the overlay keeps its listing rows. */
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

u32 Random16Far(void);

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned table address 0x02009170.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x020091d0.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x020091e0.
 */

/*
 * The eight-byte owner at 0x0200004c includes its one pool word, which holds
 * the returned table address 0x02009240.
 */

/*
 * Repaint the board records.  The owner at 0x02000194 includes its two pool
 * words; 0x020092c0 and 0x020092c8 are pointer cells, declared extern rather
 * than as literal addresses so that neither pool word derives the other.  The
 * layout selector is re-read at every test and must not be folded into one
 * local; the zero stored into piece[85] and piece + 12 is a function-scope
 * local; the record pointer advances only in the loop's common tail.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo shared with later uses in the block.  A value-returning call also
 * sets r0 last of its arguments.
 */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

/*
 * Apply the resource's asymmetric RGB555 color adjustment.  The owner spans
 * 0x02000ecc-0x02000f34; control jumps over the mask literal at 0x02000f14 and
 * rejoins at 0x02000f18 before the common return.
 */

s32 SceneData_Run();   /* 0x02000eec */

s32 IwramSignedDivide();   /* 0x02000efa */

s32 IwramSignedDivide();   /* 0x02000f08 */
void State_ApplyRectByLayoutSelector(void);

void Scene_RepaintBoardRecords(void)
{
    s32 zero;
    s16 *record = gOv;

    if (*gOv2 != 0) {
        { s32 f1 = 79; s32 g1 = 29; Map_CopyMetatileCellsRect(65, 53, 2, 1,  f1, g1); }
        { s32 f2 = 15; s32 g2 = 28; Map_CopyMetatileCellsRect(65, 40, 2, 4,  f2, g2); }
    } else {
        { s32 f3 = 79; s32 g3 = 25; Map_CopyMetatileCellsRect(65, 50, 2, 5,  f3, g3); }
    }

    if (*gOv2 != 0) {
        { s32 f4 = 32; s32 g4 = 0; Map_CopyMetatileCellsRect(0, 32, 32, 32,  f4, g4); }
        { s32 f5 = 64; s32 g5 = 0; Map_CopyMetatileCellsRect(32, 32, 32, 32,  f5, g5); }
        { s32 f6 = 0; s32 g6 = 0; Map_CopyCellAttributeRect(0, 32, 32, 32,  f6, g6); }
    } else {
        { s32 f7 = 32; s32 g7 = 0; Map_CopyMetatileCellsRect(0, 64, 32, 32,  f7, g7); }
        { s32 f8 = 64; s32 g8 = 0; Map_CopyMetatileCellsRect(32, 64, 32, 32,  f8, g8); }
        { s32 f9 = 0; s32 g9 = 0; Map_CopyCellAttributeRect(0, 64, 32, 32,  f9, g9); }
    }

    if (record[0] != -1) {
        zero = 0;
        do {
            u8 *piece = *(u8 **)(record + 4);

            if (*gOv2 == 1) {
                ObjectDispatch_ApplyArgumentToChildren(piece, 4);
                piece[35] = 3;
                piece[85] = zero;
                *(s32 *)(piece + 12) = 0x1a0000;

                if (record[3] != 0) {
                    s32 col = record[1];
                    s32 row = record[2];
                    Map_CopyMetatileCellsRect(68, 40, 1, 4, col + 32, row);
                } else {
                    s32 col = record[1];
                    s32 row = record[2];
                    Map_CopyMetatileCellsRect(70, 40, 4, 1, col + 32, row);
                }
            } else {
                ObjectDispatch_ApplyArgumentToChildren(piece, 1);
                piece[35] = 1;
                piece[85] = 2;
                *(s32 *)(piece + 12) = zero;
            }
            record += 6;
        } while (record[0] != -1);
    }

    { s32 f10 = 10; s32 g10 = 50; Map_CopyMetatileCellsRect(70, 42, 1, 1,  f10, g10); }

    if (*gOv2 == 1) {
        { s32 f11 = 0; s32 g11 = 0; Map_CopyCellAttributeRect(0, 32, 32, 32,  f11, g11); }
        SceneData_Apply2(gOv, 254);
    } else {
        { s32 f12 = 0; s32 g12 = 0; Map_CopyCellAttributeRect(0, 64, 32, 32,  f12, g12); }
        SceneData_Apply3(gOv, 255);
    }

    State_ApplyRectByLayoutSelector();
}
