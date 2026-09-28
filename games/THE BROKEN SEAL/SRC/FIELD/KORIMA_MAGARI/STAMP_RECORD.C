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


s32 IwramSignedDivide();   /* 0x02000efa */

s32 IwramSignedDivide();   /* 0x02000f08 */

void State_StampRecordCells(s16 *records, s32 value)
{
    extern u8 gMapCellBuffer[];

    s16 *record = records;
    if (record[0] == -1) return;
    do {
        s32 column = record[1];
        s32 row = record[2];
        s32 along = record[3];
        s32 i;
        for (i = 3; i >= 0; i--) {
            u8 *cell;
            cell = gMapCellBuffer + ((column + (row << 7)) << 2);
            cell[2] = (u8)value;
            if (along == 0) column++;
            else row++;
        }
        record += 6;
    } while (record[0] != -1);
}

const struct TileRun *SceneData_FindTileRunAt(
    const struct TileRun *run,
    s32 x,
    s32 y)
{
    while (run->id != -1) {
        s32 x0 = run->x;
        s32 x1 = x0;
        s32 y0 = run->y;
        s32 y1 = y0;

        if (run->vertical == 0)
            x1 += 3;
        else
            y1 += 3;

        if (x >= x0 && x <= x1 &&
            y >= y0 && y <= y1)
            return run;
        run++;
    }
    return 0;
}
