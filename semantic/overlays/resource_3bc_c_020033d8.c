typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3bc owner at 0x020033d8, 144 bytes: seed a second per-instance
 * task's state and pick which of five mode-table addresses it dispatches
 * through.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020033d8 through `pop {r5, r6} /
 * pop {r0} / bx r0` at 0x02003460-0x02003464, followed by the 15-word literal
 * pool 0x02003444-0x02003467 that closes the 144-byte span; the next owner's
 * prologue is at 0x02003468.  Two arguments (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed): the one call site,
 * 0x020033ee, resolves with the `+2` rule to veneer 0x02004848 ->
 * Func_080000d0, the same task installer as the sibling
 * resource_3bc_c_02003abc.c.  The installed callback, 0x0200aee9 under this
 * overlay's proven 0x02008000 link base, is this overlay's own
 * 0x02002ee8 -- outside this mandate's called-function list (never reached by
 * `bl`, only installed) and not drafted here.
 *
 * The branch chain 0x020033f4-0x0200340a picks one of five addresses by
 * `mode` (and, only when `mode == 3`, by whether `param` is zero) and stores
 * the selected one as a 32-bit value into Data_0200dbe0 -- a mode-table /
 * handler-select pattern.  Whether the five addresses are code (dispatch
 * targets) or data (per-mode records) is not resolved here; they are typed as
 * plain addresses.  The four sibling stores that follow
 * (Data_0200dbdc/dc38/dbac cleared as halfwords, Data_0200dbb0 cleared as a
 * word) reset the rest of the task's state block alongside the handler
 * selection.
 */

extern u16 Data_0200dbd0;
extern u16 Data_0200dba4;
extern u16 Data_0200dbdc;
extern s32 Data_0200dbe0;
extern u16 Data_0200dc38;
extern u16 Data_0200dbac;
extern s32 Data_0200dbb0;

extern s32 Data_0200d9a6;   /* default handler (mode not 2/3/4) */
extern s32 Data_0200ccba;   /* handler for mode == 2 */
extern s32 Data_0200d9d2;   /* handler for mode == 4 */
extern s32 Data_0200cce2;   /* handler for mode == 3, param != 0 */
extern s32 Data_0200da50;   /* handler for mode == 3, param == 0 */

void Func_080000d0(s32 taskAddress, s32 frameBudget);

void Func_020033d8(u32 mode, u32 param)
{
    s32 handler;

    Data_0200dbd0 = (u16)mode;
    Data_0200dba4 = (u16)(param << 4);

    Func_080000d0(0x0200aee9, 0xc80);

    if (mode == 2) {
        handler = (s32)&Data_0200ccba;
    } else if (mode == 4) {
        handler = (s32)&Data_0200d9d2;
    } else if (mode == 3) {
        handler = (param != 0) ? (s32)&Data_0200cce2 : (s32)&Data_0200da50;
    } else {
        handler = (s32)&Data_0200d9a6;
    }

    Data_0200dbdc = 0;
    Data_0200dbe0 = handler;
    Data_0200dc38 = 0;
    Data_0200dbac = 0;
    Data_0200dbb0 = 0;
}
