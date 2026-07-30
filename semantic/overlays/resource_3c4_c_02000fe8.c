/*
 * resource_3c4 @ 0x02000fe8 (80 bytes: 72 code + two pool words).
 *
 * Copies a follow position into the caller's record when the reference record
 * is past -0x00300000 (pool word 0xffd00000) on the +12 axis and slot 8 sits
 * at row 10 of the 12.20 grid; otherwise the record is zeroed.  The `ble` and
 * `asrs #20` make both tests signed.
 *
 * The three slot-8 fetches are separate service calls in the reference and are
 * kept separate here: they are not proven to return the same record.
 *
 * The epilogue is `pop {r5} ; pop {r1} ; bx r1`: r0 survives, so the constant
 * zero is the observable result.
 */
typedef signed int s32;

typedef struct Record_02000fe8 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8  */
    s32 w12;               /* +12 */
    s32 w16;               /* +16 */
} Record_02000fe8;

Record_02000fe8 *Func_020040fc(s32 index);
Record_02000fe8 *Func_0200410a(s32 slot);
Record_02000fe8 *Func_02004118(s32 slot);
Record_02000fe8 *Func_02004126(s32 slot);

s32 Func_02000fe8(Record_02000fe8 *record)
{
    Record_02000fe8 *reference = Func_020040fc(0);
    s32 w16 = 0;

    if (reference->w12 > (s32)0xffd00000
        && (Func_0200410a(8)->w16 >> 20) == 10) {
        record->w8 = Func_02004118(8)->w8;
        record->w12 = (s32)0xffe00000;
        w16 = Func_02004126(8)->w16;
    } else {
        record->w8 = 0;
        record->w12 = 0;
    }
    record->w16 = w16;
    return 0;
}
