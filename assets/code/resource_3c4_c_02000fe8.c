typedef signed int s32;

typedef struct Record {
    s32 pad0;
    s32 pad4;
    s32 w8;
    s32 w12;
    s32 w16;
} Record;

Record *Func_020040fc(s32);
Record *Func_0200410a(s32);
Record *Func_02004118(s32);
Record *Func_02004126(s32);

s32 Func_02000fe8(Record *record)
{
    Record *reference = Func_020040fc(0);

    if (reference->w12 > (s32)0xffd00000
        && (Func_0200410a(8)->w16 >> 20) == 10) {
        record->w8 = Func_02004118(8)->w8;
        record->w12 = (s32)0xffe00000;
        record->w16 = Func_02004126(8)->w16;
    } else {
        record->w8 = 0;
        record->w12 = 0;
        record->w16 = 0;
    }
    return 0;
}
