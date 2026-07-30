/*
 * resource_3c4 @ 0x02000cc0 (14 bytes).
 *
 * A two-instruction forwarder.  Nothing in the body initialises r0, so r0 is
 * an incoming argument that is passed straight through; r1 is forced to zero.
 * The epilogue is `pop {r1} ; bx r1`, which preserves r0, so the constant zero
 * materialised after the call is the observable return value.
 */
typedef signed int s32;

/* Resident service outside the overlay image; arity taken from the call site. */
void Func_02003d72(s32 selector, s32 flag);

s32 Func_02000cc0(s32 selector)
{
    Func_02003d72(selector, 0);
    return 0;
}
