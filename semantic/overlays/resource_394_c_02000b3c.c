typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 394 board-cell stamp at 0x02000b3c.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000b3c and the matching
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000b82.  80-byte row: 76 bytes
 * of code plus the single pool word 0x02010000 at 0x02000b88.  Control-flow
 * walk: the back-edges target 0x02000b5e and 0x02000b50 and the forward
 * branches 0x02000b70, 0x02000b72 and 0x02000b82, all inside the body; the
 * next owner's prologue is at 0x02000b8c.  The return address is popped into
 * r0, so the owner is `void`.
 *
 * Called three times: twice from 0x02000194 (with 254 and 255) and once from
 * the root 0x02000980 (with 255).
 *
 * The record list is the same twelve-byte-stride, -1-terminated array that
 * 0x02000a90 walks: an s16 id at +0, column at +2, row at +4, an orientation
 * flag at +6.  Each record stamps four consecutive cells of the 0x02010000
 * board — along the column when the orientation flag is zero and along the row
 * otherwise — writing `value` into byte 2 of the four-byte cell.  0x02010000
 * is below the 0x02008000 link band, so it is a plain EWRAM address rather
 * than an in-image offset.
 *
 * The inner counter runs 3, 2, 1, 0 and exits on `bge` failing, so it is four
 * iterations, not three.
 *
 * No call sites, matching the row's advertised count of zero.
 */

void Func_02000b3c(s16 *records, s32 value)
{
    s16 *record = records;

    if (record[0] == -1) return;

    do {
        s32 column = record[1];
        s32 row = record[2];
        s32 alongRow = record[3];
        s32 i;

        for (i = 3; i >= 0; i--) {
            *(u8 *)(0x02010000 + (((column + (row << 7)) << 2) + 2)) = (u8)value;
            if (alongRow == 0) {
                column++;
            } else {
                row++;
            }
        }
        record += 6;
    } while (record[0] != -1);
}
