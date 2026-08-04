typedef signed int s32;

/*
 * Complete actor-12 two-flag dialogue scene through its three-word pool.
 *
 * Call symbols are per-site, not per-import: the raw region in
 * assets/code/resource_385_overlay.s (lines 766-783, still unadopted) spells
 * every call as `bl sub_02000xxx`, and the two calls that both ultimately
 * reach Func_080770c8 go through TWO DIFFERENT overlay-local veneers,
 * sub_02001608 and sub_0200160e -- not the same symbol twice.
 */
void Func_02001618(void);
void Func_0200168a(s32 actor, s32 mode, s32 value);
void Func_02001608(s32 flag);
void Func_0200160e(s32 flag);
void Func_020016b4(s32 dialogue);
void Func_020016cc(s32 actor, s32 mode);
void Func_02001648(void);

void Func_0200052c(void)
{
    Func_02001618();
    Func_0200168a(12, 0, 2);
    Func_02001608(0x306);
    Func_0200160e(0x868);
    Func_020016b4(0x1caf);
    Func_020016cc(12, 0);
    Func_02001648();
}
