typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
extern u8 Data_02010000[];

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
            u8 *cell;
            cell = Data_02010000 + ((column + (row << 7)) << 2);
            cell[2] = (u8)value;
            if (alongRow == 0) column++;
            else row++;
        }
        record += 6;
    } while (record[0] != -1);
}
