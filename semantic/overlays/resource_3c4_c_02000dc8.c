/*
 * resource_3c4 @ 0x02000dc8 (42 bytes).
 *
 * Builds a three-word 12.20 position descriptor on the stack from the record
 * the service returns, lifting the third component by 0x00200000 (0x80 << 14),
 * and hands its address to Func_02001aba.
 *
 * Func_02001aba is a genuine entry: 0x02001aba holds an alignment `nop` and
 * falls straight into the `push {r5, r6, r7, lr}` prologue at 0x02001abc, so
 * the call is balanced and no live state crosses the boundary.
 *
 * `add sp,#12 ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

typedef struct Record_02000dc8 {
    s32 pad0;
    s32 pad4;
    s32 x;                 /* +8  */
    s32 y;                 /* +12 */
    s32 z;                 /* +16 */
} Record_02000dc8;

Record_02000dc8 *Func_02003edc(s32 index);
void Func_02001aba(const s32 *position);

void Func_02000dc8(void)
{
    Record_02000dc8 *record = Func_02003edc(0);
    s32 position[3];

    position[0] = record->x;
    position[1] = record->y;
    position[2] = record->z + 0x00200000;
    Func_02001aba(position);
}
