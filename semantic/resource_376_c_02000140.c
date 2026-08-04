typedef signed int s32;
extern s32 Func_080770c0(s32 flagId);

/* Complete 80-byte three-flag table selector through its seven-word pool. */
void *Func_02000140(void)
{
    if (Func_080770c0(0x834) != 0)
        return (void *)0x02009ac8;
    if (Func_080770c0(0x87a) != 0)
        return (void *)0x02009ffc;
    if (Func_080770c0(0x815) != 0)
        return (void *)0x02009da4;
    return (void *)0x02009c00;
}
