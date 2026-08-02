typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_02000630(s32 *directionOut, s32 *slotOut, s32 *classOut);
extern s32 Func_080091d8(u8 *actor, s32 *position);

static s32 Abs_02000758(s32 value)
{
    return value < 0 ? -value : value;
}

/*
 * Slide the selected footprint through a tile grid until terrain refuses the
 * move.  A refusal restores the selected object's activity byte; otherwise
 * the record receives the last reachable fixed-point position.  Complete
 * owner: 0x02000758-0x020008eb, including alignment and its three-word pool.
 */
s32 Func_02000758(s32 *record)
{
    const s32 (*bounds)[4] = (const s32 (*)[4])0x0200a758;
    const s32 *steps = (const s32 *)0x0200a700;
    s32 direction;
    s32 row;
    s32 column;
    s32 rows;
    s32 columns;
    s32 basePosition[3];
    s32 wholeGridSteps = 0;
    u8 *object;

    record[5] = 0;
    object = Func_02000630(&direction, record + 1, record);
    if (object == 0)
        return 0;

    object[34] = 2;
    rows = (Abs_02000758(bounds[record[0]][1]) +
            Abs_02000758(bounds[record[0]][3])) >> 4;
    columns = (Abs_02000758(bounds[record[0]][0]) +
               Abs_02000758(bounds[record[0]][2])) >> 4;

    basePosition[0] = *(s32 *)(object + 8) + (steps[direction] & (s32)0xffff0000);
    basePosition[1] = *(s32 *)(object + 12);
    basePosition[2] = *(s32 *)(object + 16) + (s32)((unsigned int)steps[direction] << 16);
    record[3] = basePosition[1];

    for (;;) {
        record[4] = basePosition[2] + (bounds[record[0]][1] << 16);
        for (row = 0; row < rows; row++) {
            record[2] = basePosition[0] + (bounds[record[0]][0] << 16);
            for (column = 0; column < columns; column++) {
                if (Func_080091d8(object, &record[2]) == 2)
                    goto found;
                record[2] += 0x00100000;
            }
            record[4] += 0x00100000;
        }
        wholeGridSteps++;
        basePosition[0] += steps[direction] & (s32)0xffff0000;
        basePosition[2] += (s32)((unsigned int)steps[direction] << 16);
    }

found:
    object[34] = 0;
    if (wholeGridSteps == 0)
        return 0;
    record[2] = *(s32 *)(object + 8) +
        (steps[direction] & (s32)0xffff0000) * wholeGridSteps;
    record[3] = *(s32 *)(object + 12);
    record[4] = *(s32 *)(object + 16) +
        (s32)((unsigned int)steps[direction] << 16) * wholeGridSteps;
    return 1;
}
