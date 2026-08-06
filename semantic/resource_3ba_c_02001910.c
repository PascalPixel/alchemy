#include "types.h"

/*
 * resource_3ba owner at 0x02001910, 208 bytes: reset the five party-build
 * slots, build the caller's id as this overlay's active party member, park
 * its id in the shared record, deselect it, then re-derive its motion
 * fields' clamped step ratios and refresh it.
 *
 * Complete owner: `push {r5, lr}` at 0x02001910 through the matching
 * interworking return at 0x020019da-0x020019de.  The branch at 0x02001982
 * skips the three-word literal pool at 0x02001984-0x0200198f, so the pool
 * carries no live state.  One argument (r0), void.
 *
 * This is the same 208-byte operation and call sequence as the independently
 * reconstructed resource_3bc owner at 0x02002640.  Eleven reachable calls
 * resolve to six established targets: Func_08077168 x5 (slots 0,1,2,3,5),
 * Func_08077150, Func_0808a200, Func_08077008, Func_03000380 x2, and
 * Func_0808a548.  resource_3ba's scene entry calls this owner with ids 0 and
 * 1, corroborating the single s32 input.
 *
 * The record fields at +52/+54 are copied to +56/+58.  Each copied value is
 * then used to derive a Q14 ratio, clamped to [0, 0x4000], with the explicit
 * nonzero-field epsilon guard preserved from the machine code.  Byte +0x131
 * is cleared unconditionally; its role is otherwise not established.
 */

extern u8 Data_02000240[];

          /* established, reset slot */
          /* established, reset a member by handle */
          /* established shape (id, flag) */
           /* item/party record by id, established */
           /* relocated IWRAM helper, established */
          /* established no-arg call */

extern void Func_020055a0();
extern void Func_020055a6();
extern void Func_020055ac();
extern void Func_020055b2();
extern void Func_020055b8();
extern void Func_020055b6();
extern void Func_020056b0();
extern u8 * Func_0200558e();
extern s32 Func_0200546e();
extern s32 Func_020054b0();
extern void Func_020057b8();
void Func_02001910(s32 id)
{
    u8 *record;
    s32 computed;
    s32 ratio;

    Func_020055a0(0);
    Func_020055a6(1);
    Func_020055ac(2);
    Func_020055b2(3);
    Func_020055b8(5);
    Func_020055b6(id);

    *(s32 *)&Data_02000240[500] = id;

    Func_020056b0(id, 0);

    record = Func_0200558e(id);

    *(s16 *)(record + 56) = *(s16 *)(record + 52);
    record[0x131] = 0;
    *(s16 *)(record + 58) = *(s16 *)(record + 54);

    computed = Func_0200546e(*(s16 *)(record + 56) << 14, *(s16 *)(record + 52));
    if (computed > 0x4000) {
        ratio = 0x4000;
    } else if (computed < 0) {
        ratio = 0;
    } else {
        ratio = computed;
    }
    *(s16 *)(record + 20) = ratio;
    if (ratio == 0 && *(s16 *)(record + 56) != 0) {
        *(s16 *)(record + 20) = 1;
    }

    computed = Func_020054b0(*(s16 *)(record + 58) << 14, *(s16 *)(record + 54));
    if (computed > 0x4000) {
        ratio = 0x4000;
    } else if (computed < 0) {
        ratio = 0;
    } else {
        ratio = computed;
    }
    *(s16 *)(record + 22) = ratio;
    if (ratio == 0 && *(s16 *)(record + 58) != 0) {
        *(s16 *)(record + 22) = 1;
    }

    Func_020057b8();
}
