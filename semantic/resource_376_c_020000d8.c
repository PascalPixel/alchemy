typedef signed int s32;
extern s32 Func_080770c0(s32 flagId);

/* Complete 36-byte flag-selected table getter through its three-word pool. */
void *Func_020000d8(void)
{
    if (Func_080770c0(0x834) != 0)
        return (void *)0x02009590;
    return (void *)0x02009568;
}
