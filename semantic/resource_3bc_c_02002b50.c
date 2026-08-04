#include "types.h"

/*
 * resource_3bc owner at 0x02002b50, 92 bytes: release the caller's handle
 * through Func_08015120, then display one of the same three story-state
 * messages as sibling owner Func_02002a94 (semantic/overlays/
 * resource_3bc_c_02002a94.c) -- offset by one id -- and close.
 *
 * Complete owner: `push {r5, lr}` at 0x02002b50 through the matching
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x02002b8c-0x02002b90,
 * followed by the seven-word literal pool 0x02002b92-0x02002bab that closes
 * the 92-byte span; the next owner's prologue is at 0x02002bac (queue
 * neighbour Func_02002bac).  Two arguments (r0, r1), void (the popped word
 * on the bx path is the return address, not a result).
 *
 * Not found by the structural inventory walk (unindexed); six call sites,
 * all resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay_show.ts resource_3bc 2b50 |
 * bun tools/overlay_call_targets.ts resource_3bc 2b50 2bac --annotate`):
 *   0x02002b58 -> veneer 0x02004978 -> Func_08015120(arg1, 5)   established (resource_3bc_c_02002a94.c)
 *   0x02002b80 -> veneer 0x02004ab0 -> Func_0808a170(msg)       display message by id (established)
 *   0x02002b88 -> veneer 0x02004ac0 -> Func_0808a180(arg0, 0)   established shape
 *
 * The message-state read at Data_02000240[0xe0] (byte offset 0x1c0) is the
 * identical three-way selector as Func_02002a94's -- same field, same two
 * compared constants (0x8f, 0x90) -- but every result here is one higher
 * (0x2077/0x2079/0x207b instead of 0x2076/0x2078/0x207a), so this owner
 * displays the following message in each of the three sequences rather than
 * the state's own message.
 */

extern s16 Data_02000240[];

void Func_08015120();          /* release/select by handle, established (arg, 5) */
void Func_0808a170();          /* display message by id */
void Func_0808a180();

void Func_02002b50(s32 arg0, s32 arg1)
{
    s16 state;
    s32 messageId;

    Func_08015120(arg1, 5);

    state = Data_02000240[0xe0];
    if (state == 0x8f) {
        messageId = 0x2076;
    } else if (state == 0x90) {
        messageId = 0x2078;
    } else {
        messageId = 0x207a;
    }
    Func_0808a170(messageId + 1);

    Func_0808a180(arg0, 0);
}
