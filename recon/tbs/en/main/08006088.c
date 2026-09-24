/* Draft, not exact (2026-09-24): 94 of 96 bytes, one instruction short.
   SIOCNT read through a multiplayer bitfield gives the reference's
   lsls 26 / lsrs 30 id test. Residual: the reference computes the packed
   masks in r2 and copies them into r0 (adds r0, r2, #0) before the
   is_parent test; here packed and result coalesce into r0. Types, a
   do-while and a (u16) cast on the copy did not keep the copy. */
#include "SERIAL_RUNTIME.H"

struct SioMultiControl {
    u32 baud_rate:2;
    u32 si:1;
    u32 sd:1;
    u32 id:2;
    u32 error:1;
    u32 start:1;
    u32 unused:6;
    u32 irq:1;
    u32 pad:17;
};

void Func_080060e8(void *payload);
u8 Func_0800615c(void *payload);

s32 SerialRuntime_ExchangePayloads(void *send, void *receive)
{
    struct SioMultiControl control = *(struct SioMultiControl *)0x04000128;
    struct SerialRuntime *state = SERIAL_RUNTIME;
    u32 packed;
    s32 result;

    if (state->phase == 1) {
        Func_0800615c(receive);
        Func_080060e8(send);
        state->sequence++;
    }
    packed = state->current_mask | (state->received_mask << 8);
    if (state->mode == 8)
        packed |= 0x80;
    result = packed;
    if (state->is_parent)
        result |= 0x1000;
    if ((u32)control.id > 1)
        result |= 0x2000;
    return result;
}
