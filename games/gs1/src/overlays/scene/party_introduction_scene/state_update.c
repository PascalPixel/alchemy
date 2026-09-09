#include "types.h"

#define SceneState_PumpUntilSlotsFree Func_02004328

/* One symbol per call site, named at that site's call word rather than at a
 * runtime address. A callee reached from several sites therefore appears
 * several times here under different names. */
extern void Func_02008e52(s32 arg0);                        /* 0x0200433c -> 080f9010 */
extern void Func_02008e4a(s32 arg0, s32 arg1);              /* 0x02004344 -> 0808a398 */
extern void Func_02008c6a(s32 arg0, s32 arg1);              /* 0x0200434c -> 08015040 */
extern s32 Func_02008cd0(s32 arg0);                         /* 0x02004352 -> 08077248 */
extern s32 Func_02008cda(s32 arg0);                         /* 0x0200435c -> 08077248 */
extern void Func_02008c88(s32 arg0, s32 arg1);              /* 0x0200436a -> 08015040 */
extern s32 Func_02008e80(s32 *out_first, s32 *out_second);  /* 0x02004372 -> 080b0058 */
extern void Func_02008d08(s32 arg0, s32 arg1);              /* 0x02004382 -> 080772b0 */
extern void Func_02008cd8(s32 arg0);                        /* 0x0200438a -> 08077030 */
extern void Func_02008cde(s32 arg0);                        /* 0x02004390 -> 08077030 */
extern void Func_02008ce4(s32 arg0);                        /* 0x02004396 -> 08077030 */
extern void Func_02008cea(s32 arg0);                        /* 0x0200439c -> 08077030 */

/*
 * Drain until room: save the s16 counter at scene workspace + 472, prime two
 * channels, then loop while fewer than four of thirty slots are free,
 * requesting more and passing on any event pair that is not -1. On exit it
 * flushes four times with id 224 and restores the saved counter. The 148-byte
 * owner includes its three-word literal pool. Callee roles are not established.
 */
void SceneState_PumpUntilSlotsFree(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s16 saved = *(s16 *)(work + 472);
    s32 first;
    s32 second;
    s32 cnt;

    Func_02008e52(0x53);
    Func_02008e4a(224, 3);
    Func_02008c6a(0x111b, 1);
    do {
        cnt = 30 - Func_02008cd0(0);
        cnt -= Func_02008cda(1);

        if (cnt <= 3) {
            Func_02008c88(0x111c, 1);
            if (Func_02008e80(&second, &first) != -1)
                Func_02008d08(second, first);
        }
    } while (cnt <= 3);
    Func_02008cd8(224);
    Func_02008cde(224);
    Func_02008ce4(224);
    Func_02008cea(224);
    *(s16 *)(work + 472) = saved;
}
