typedef signed int s32;

extern void Func_02001bfc(s32, s32, s32 *);
extern void Func_02001c38(s32 *, s32, s32, s32);

void Func_02000b54(s32 *record, s32 first, s32 second)
{
    s32 position[3];

    if (record == 0) {
        return;
    }
    position[0] = record[2];
    position[1] = record[3];
    position[2] = record[4];
    Func_02001bfc(first, second, position);
    Func_02001c38(record, position[0], position[1], position[2]);
}
