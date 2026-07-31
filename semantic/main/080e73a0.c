#include "types.h"

struct Request_080e73a0 {
    void *owner;
    void *payload;
    u8 unknown_08[16];
    s32 status;
};

struct Context_080e73a0 {
    struct Request_080e73a0 *table;
};

/*
 * Claim the first free slot of the 32-entry request table and fill it in.
 * A slot is free when its status word reads -1; claiming one clears the status
 * and stores the owner and payload.  When every slot is in use the request is
 * dropped silently.
 *
 * The table base is the hidden context the caller leaves in r9, read once as
 * *(r9 - 136) and never written; the owner saves and restores r9 around its
 * body, so the register is balanced here.  Caller unknown -- no direct call,
 * branch or pool word in the image reaches this address -- so the context has
 * no in-tree spelling and is left as an uninitialised local.
 */
void Func_080e73a0(void *owner, void *payload)
{
    struct Context_080e73a0 *context;
    struct Request_080e73a0 *slot;
    s32 index = 0;

    slot = (struct Request_080e73a0 *)((u8 *)context->table + 0x7080);
    if (slot->status == -1) {
        slot->status = 0;
        goto fill;
    }

    do {
        index += 1;
        if (index == 32)
            return;
        slot = (struct Request_080e73a0 *)
            ((u8 *)context->table + index * 28 + 0x7080);
    } while (slot->status != -1);
    slot->status = 0;

fill:
    slot->owner = owner;
    slot->payload = payload;
}
