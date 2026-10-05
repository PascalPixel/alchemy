#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
#include "MENU_LIST.H"
#include "WINDOW.H"
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
/* The caller's r9 points just past the captured context and padding fields.
 * The callback result is ignored. */
s32 UiText_DrawWaitingForOpponent(void)
{
    /* FAKEMATCH: Retain the native incoming-r9 chain spill before message argument setup; ordinary captured-frame shapes change its store/register order. */
    struct MessageFrame *volatile saved;
    u16 data[64];
    struct MessageFrame *frame;
    struct MessageFrame *captured;
    u16 *arg_buf;
    /* FAKEMATCH: Retain native r2 size setup before adjusting the captured-frame pointer. */
    register s32 arg_size asm("r2");
    u16 *buf;
    /* FAKEMATCH: Ordinary buffer operands introduce a second SP copy; capture the native r6 buffer and hand it to buf. */
    register u16 *held_buf asm("r6");
    /* FAKEMATCH: An unconstrained chain capture uses r5 directly; retain the native r2-to-r5 handoff. */
    register struct MessageFrame *held asm("r2");
    /* FAKEMATCH: Capture the caller-provided nested-function chain in its native r9 register before either call. */
    register struct MessageFrame *chain asm("r9");

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
    frame = captured - 1;
    /* FAKEMATCH: The international message load follows the frame adjustment; the prior shape swapped that pair. */
    __asm__("" : "+r"(frame) : : "r0");
    UiText_CopyMessageString((s32)MsgWaitingForOpponent, arg_buf, arg_size);
#else
    {
        s32 message = (s32)MsgWaitingForOpponent;

        arg_size = 32;
        frame = captured - 1;
        UiText_DecodeMessage(message, arg_buf, arg_size);
    }
#endif
    UiText_RenderWideStringAtOffset(buf, frame->context->window, 0, 4);
}
