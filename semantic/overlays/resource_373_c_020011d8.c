typedef signed int s32;

/*
 * Resource 373: guarded cutscene beat.
 *
 * Complete owner: `push {r5, lr}` at 0x020011d8 through the shared
 * `pop {r5} ; pop {r0} ; bx r0` at 0x02001230..0x02001234, followed by its
 * three-word literal pool at 0x02001238.  Both the early exit and the normal
 * exit reach that one epilogue, and nothing stays live past it.
 *
 * All ten calls are placed below.  None is an interworking `call_via rN`
 * site: r3 is never loaded with a helper address in this routine.
 *
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image; some of them fall numerically inside this overlay's own
 * span because overlay-local code and resident services share the 0x02000000
 * namespace and the module's `bl` displacements are fixed up on load.  They
 * are kept as calls, as in the resource 394 conversion.
 *
 * UNCERTAINTY 2: r0 is not deliberately set before the return.  On the early
 * path it still holds the guard result and on the normal path it holds
 * whatever Func_020071f2 left; that is the signature of a `void` routine
 * whose callers ignore r0, which is how it is spelled here.
 */

s32 Func_0200717a(s32 flag);
void Func_02007196(s32 id, s32 mode);
void Func_020071a2(void);
void Func_020071d4(s32 count);
void Func_020071e0(s32 index, s32 x, s32 y);
void Func_020071f2(void);
void Func_0200724e(s32 index, s32 id, s32 script);
void Func_02007280(s32 id);
void Func_020072a2(s32 channel, s32 slot, s32 value);
void Func_020072ae(s32 channel, s32 slot, s32 value);

void Func_020011d8(void)
{
    s32 id;

    /* The guard flag word is materialised from the pool as 0x00000808. */
    if (Func_0200717a(0x00000808) != 0) {
        return;
    }

    Func_020071a2();

    /* 0x80 << 9 and 0x80 << 8. */
    Func_020071e0(0, 0x10000, 0x8000);

    id = 0x00000f4d;
    Func_02007280(id);
    Func_020072a2(15, 0, 2);

    id += 2;                      /* 0x00000f4f */
    Func_020072ae(16, 0, 2);
    Func_02007196(id, 1);

    Func_020071d4(6);
    Func_0200724e(0, 69, 0x00000366);
    Func_020071f2();
}
