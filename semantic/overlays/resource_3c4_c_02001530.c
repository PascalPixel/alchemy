/*
 * resource_3c4 @ 0x02001530 (32 bytes).
 *
 * Same shape as 0x02001510 with a different service, different expected
 * coordinates, and a different follow-up.
 *
 * The follow-up target 0x0200233e is the shared tail of Func_02002330 rather
 * than a separate prologue.  It is a legitimate secondary entry: its epilogue
 * is `pop {r0} ; bx r0`, which balances exactly the `push {lr}` frame this
 * owner holds at the call.  It is therefore declared, not defined, here.
 */
typedef signed int s32;

typedef struct Record_02001530 {
    s32 pad0;
    s32 pad4;
    s32 x;                 /* +8,  12.20 fixed point */
    s32 pad12;
    s32 y;                 /* +16, 12.20 fixed point */
} Record_02001530;

Record_02001530 *Func_02004642(s32 index);
void Func_0200233e(void);

void Func_02001530(void)
{
    Record_02001530 *record = Func_02004642(0);

    if ((record->x >> 20) != 30 || (record->y >> 20) != 52) {
        Func_0200233e();
    }
}
