typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3bf state predicate at 0x02005bd8.
 *
 * Loads the first word of the object in r0 and reports whether it is one of
 * the two lowest state codes.  The compare is `cmp r3,#1` / `bhi`, i.e. an
 * UNSIGNED comparison, so the field is read as u32 here.
 */
s32 Func_02005bd8(const u32 *object)
{
    return object[0] <= 1u;
}
