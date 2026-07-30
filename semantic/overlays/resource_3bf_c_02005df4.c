typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3bf state predicate at 0x02005df4.
 *
 * Reports whether the first word of the object in r0 equals the state code 2.
 */
s32 Func_02005df4(const u32 *object)
{
    return object[0] == 2u;
}
