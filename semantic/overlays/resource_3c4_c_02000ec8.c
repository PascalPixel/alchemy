/*
 * resource_3c4 @ 0x02000ec8 (70 bytes).
 *
 * Raises bit 1 of the byte at +35 of the caller's record, then lowers it again
 * when the reference record fetched from the service is behind the caller's
 * record on the +16 axis and within 0x00040000 (0x80 << 11) plus the caller's
 * +12 offset on the +12 axis.  Both comparisons are signed (`bge`, `bgt`).
 *
 * The reference keeps the byte in a register across the test and stores it
 * twice, so the intermediate `|= 2` really is observable at +35 before the
 * conditional clear; both stores are written out here.
 *
 * The epilogue is `pop {r5} ; pop {r1} ; bx r1`: r0 survives, so the constant
 * zero is the observable result.
 */
typedef signed int s32;
typedef unsigned char u8;

typedef struct Record_02000ec8 {
    s32 pad0;
    s32 pad4;
    s32 pad8;
    s32 w12;               /* +12 */
    s32 w16;               /* +16 */
    s32 pad20;
    s32 pad24;
    s32 pad28;
    u8 pad32[3];
    u8 flags;              /* +35 */
} Record_02000ec8;

Record_02000ec8 *Func_0808a080();

s32 Func_02000ec8(Record_02000ec8 *record)
{
    Record_02000ec8 *other = Func_0808a080(0);
    u8 raised = (u8)(record->flags | 2);

    record->flags = raised;
    if (other->w16 < record->w16) {
        s32 limit = record->w12 + (record->w16 - other->w16) + 0x00040000;

        if (other->w12 <= limit) {
            record->flags = (u8)(raised & 0xfd);
        }
    }
    return 0;
}
