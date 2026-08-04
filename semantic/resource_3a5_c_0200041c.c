typedef signed short s16;

extern void Func_080770c8();

/* Select the descriptor table belonging to the current scene variant. */
void *Func_0200041c(void)
{
    s16 *scene = (s16 *)0x02000240;

    if (scene[224] == 91 && scene[225] == 5)
        Func_080770c8(0x90a);

    if (scene[224] == 89)
        return (void *)0x0200a3c8;
    if (scene[224] == 90)
        return (void *)0x0200a410;
    if (scene[224] == 91)
        return (void *)0x0200a4b8;
    return (void *)0x0200a3b0;
}
