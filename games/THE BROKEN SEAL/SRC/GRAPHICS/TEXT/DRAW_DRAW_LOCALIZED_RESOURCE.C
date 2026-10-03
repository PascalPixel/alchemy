/* Complete current-source and own-ROM audit, 2026-10-02.
 * Ordinary TBS flags; all six complete 60-byte functions, message pools and
 * call identities are exact, with one real public definition. The renderer
 * has its actual void signature; the caller ignores this callback's result.
 * EN frame trials, with the required r9 chain capture: chain-before differed
 * at 44 bytes; chain-after 45; captured emitted 56 bytes with 47 differences;
 * r2-captured 40; slot-input 12; slot-address 13; data-order emitted 64 bytes
 * with 49 differences; data-handoff emitted 64 with 42; r6-buffer 4;
 * call-order 3; frame-order and void-render were complete EN matches.
 * Sharing the international order left six Japanese instruction differences;
 * the immediate message/size/frame handoff matches the Japanese order.
 * Named context/window fields preserve the actual caller's captured layout.
 * Argument-view refinement, 2026-10-02: both message owners use u16
 * code-unit buffers; Copy takes u32 capacity and Decode takes s32.
 * The ordinary u16 array/pointer views and explicit unsigned Copy
 * capacity cast preserve complete 60-byte output, pools and calls in
 * all six editions on the first form; no failed reshape or new device.
 * All private trial assemblies were reproduced from their current sources.
 */
#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
s32 UiText_CopyMessageString(s32,u16*,u32);
void UiText_DecodeMessage(s32,u16*,s32);
struct UiWindow;
void UiText_RenderWideStringAtOffset(u16 *, struct UiWindow *, s32, s32);
extern u8 MsgWaitingForOpponent[];
struct MessageContext {
    u8 unknown_00[0x44];
    struct UiWindow *window;
};

struct MessageFrame {
    struct MessageContext *context;
    u8 *padding;
};
/* The caller sets r9 to the end of its two captured pointer fields.
   Ordinary frame shapes select different registers; measured handoffs keep
   the native chain save, frame store and two call-argument orders. */
s32 UiText_DrawLocalizedResource80d(void)
{
    /* FAKEMATCH: Retain the native incoming-r9 chain spill before message argument setup; ordinary captured-frame shapes change its store/register order. */
    struct MessageFrame *volatile saved;
    u16 data[64];
    struct MessageFrame *frame;
    struct MessageFrame *captured;
    /* FAKEMATCH: The ordinary argument shape swaps size setup and frame adjustment; keep the immediate buffer argument in r1. */
    register u16 *arg_buf asm("r1");
    /* FAKEMATCH: Retain native r2 size setup before adjusting the captured-frame pointer. */
    register s32 arg_size asm("r2");
    u16 *buf;
    /* FAKEMATCH: Ordinary buffer operands introduce a second SP copy; capture the native r6 buffer and hand it to buf. */
    register u16 *held_buf asm("r6");
    /* FAKEMATCH: An unconstrained chain capture uses r5 directly; retain the native r2-to-r5 handoff. */
    register struct MessageFrame *held asm("r2");
    /* FAKEMATCH: Capture the caller-provided nested-function chain in its native r9 register before either call. */
    register struct MessageFrame *chain asm("r9");
    /* FAKEMATCH: Read the incoming chain without an instruction so GCC retains its native r9 save. */
    __asm__("" : "=r"(chain));
    held = chain;
    held_buf = data;
    /* FAKEMATCH: Retain the native chain/buffer register choices until both are handed to ordinary locals. */
    __asm__("" : "+r"(held), "+r"(held_buf));
    buf = held_buf;
    captured = held;
    saved = held;
    /* FAKEMATCH: Ordinary stores use SP-relative addressing or move the buffer copy; retain the native explicit frame-slot address and store order. */
    __asm__("" : : "r"(captured), "r"(held), "m"(saved), "r"(buf));
    arg_buf = buf;
#if EDITION_INTERNATIONAL
    arg_size = 0x34;
    /* FAKEMATCH: International code prepares the buffer and size before adjusting the captured frame. */
    __asm__("" : "+r"(arg_buf), "+r"(arg_size) : "r"(captured));
    frame = captured - 1;
    /* FAKEMATCH: The international message load follows the frame adjustment; the prior shape swapped that pair. */
    __asm__("" : "+r"(frame) : : "r0");
    UiText_CopyMessageString((s32)MsgWaitingForOpponent, arg_buf, (u32)arg_size);
#else
    {
        /* FAKEMATCH: Japanese code loads its message before setting size 32; retain the immediate r0 message handoff. */
        register s32 arg_msg asm("r0") = (s32)MsgWaitingForOpponent;
        /* FAKEMATCH: The shared international order differed by six Japanese instruction bytes; hold its message before size setup. */
        __asm__("" : "+r"(arg_buf), "+r"(arg_msg) : "r"(captured));
        arg_size = 32;
        /* FAKEMATCH: Retain native Japanese message/size-before-frame order with the immediate call arguments. */
        __asm__("" : "+r"(arg_buf), "+r"(arg_size), "+r"(arg_msg) : "r"(captured));
        frame = captured - 1;
        /* FAKEMATCH: Adjust the captured frame before the immediate decode call while preserving its prepared argument registers. */
        __asm__("" : "+r"(frame) : "r"(arg_buf), "r"(arg_size), "r"(arg_msg));
        UiText_DecodeMessage(arg_msg, arg_buf, arg_size);
    }
#endif
    UiText_RenderWideStringAtOffset((u16 *)buf, frame->context->window, 0, 4);
    {
        /* FAKEMATCH: The renderer returns void; preserve the native post-call r0 through the callback epilogue instead of using that void call as a value. */
        register s32 result asm("r0");
        /* FAKEMATCH: Capture r0 without an instruction to retain the native r1 return-address restore. */
        __asm__("" : "=r"(result));
        return result;
    }
}
