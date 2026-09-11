#ifndef ALCHEMY_ABI_OVERLAYS_SHARED_COPY_MODE_TO_OWNER
#define ALCHEMY_ABI_OVERLAYS_SHARED_COPY_MODE_TO_OWNER

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020009dc(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);


#endif
