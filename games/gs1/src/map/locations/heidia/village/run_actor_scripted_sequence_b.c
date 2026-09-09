#include "types.h"

extern u8 Value_0000241e;

/*
 * Actor script -- resource_3bf.  It walks a run of consecutive resource
 * identifiers starting at 0x241e, re-supplying the actor handle to every
 * service that takes one, and returns nothing.
 */

/*
 * Symbol names use the offsets as printed while absolute data addresses keep
 * their raw pool values; the module's own pool constants sit 0x8000 above
 * the printed offsets.  Func_02007376 is reached with two different argument
 * shapes -- once as (handle, 0) and once as (identifier) -- so these services
 * keep old-style declarations rather than an invented common prototype.
 */
void Func_0200731c();
void Func_02007338();
void Func_02007344();
void Func_0200735c();
void Func_0200735e();
void Func_02007376_a();
void Func_02007376_b();
void Func_02007388();
void Func_0200738e();

/*
 * Keeping this call in its own helper preserves the interleaving of the
 * handle move with the two-instruction constant.
 */
static __inline__ void Call_02007388(s32 handle)
{
    Func_02007388(handle, 258, 60);
}

/*
 * The resource run is taken as the address of Value_0000241e rather than as
 * an integer constant, which preserves its pointer identity and materialises
 * it after the first call.
 */
void RunActorScriptedSequenceB(s32 handle)
{
    u8 *id;

    Func_0200731c(handle, 1);
    id = &Value_0000241e;
    Func_02007344((s32)id);
    Func_0200735c(handle, 0);
    Call_02007388(handle);
    Func_0200735e((s32)(id + 1));
    Func_02007376_a(handle, 0);
    id += 2;
    Func_02007338(handle, 4);
    Func_02007376_b((s32)id);
    Func_0200738e(handle, 0);
}
