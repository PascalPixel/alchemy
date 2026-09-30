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

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

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
