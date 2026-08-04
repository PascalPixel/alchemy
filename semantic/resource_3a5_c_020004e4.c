typedef signed short s16;
typedef signed int s32;

extern void Func_080770d0();
extern void Func_080091b8();
extern void Func_080091c0();
extern void Func_080091c8();
extern void Func_0808a0f0();
extern void Func_0808a3c0();
extern void Func_0808a378();

/*
 * Populate the foreground cast for scene variants 89, 90, and 91.  Each
 * placement is kept explicit because the coordinates describe the authored
 * composition, while the common tail resets actor slots 8-13 and 100-106.
 */
void Func_020004e4(void)
{
    s16 scene = *(s16 *)(0x02000240 + 448);
    s32 actor;

    Func_080770d0(0x201);

    if (scene == 89) {
        Func_080091b8(70, 68, 4, 2, 22, 7);
        Func_080091b8(70, 68, 4, 2, 8, 10);
        Func_080091b8(70, 68, 4, 2, 23, 21);
        Func_080091c0(70, 68, 4, 1, 23, 23);
        Func_080091b8(70, 68, 4, 2, 16, 42);
        Func_080091b8(70, 68, 4, 2, 36, 44);
        Func_080091b8(70, 68, 4, 2, 14, 55);
    } else if (scene == 90) {
        Func_080091b8(70, 68, 4, 2, 42, 5);
        Func_080091b8(70, 68, 4, 2, 20, 11);
        Func_080091c0(70, 68, 4, 1, 20, 13);
        Func_080091b8(70, 68, 4, 2, 14, 12);
        Func_080091b8(70, 68, 4, 2, 56, 18);
        Func_080091b8(70, 68, 4, 2, 7, 22);
        Func_080091c0(70, 68, 4, 1, 7, 24);
        Func_080091b8(70, 68, 4, 2, 44, 23);
        Func_080091c0(70, 68, 4, 1, 44, 25);
        Func_080091b8(70, 68, 4, 2, 38, 24);
        Func_080091b8(70, 68, 4, 2, 26, 28);
        Func_080091b8(70, 68, 4, 2, 17, 35);
        Func_080091b8(70, 68, 4, 2, 50, 36);
        Func_080091b8(70, 68, 4, 2, 34, 43);
        Func_080091c0(70, 68, 4, 1, 34, 45);
        Func_080091b8(70, 68, 4, 2, 6, 46);
        Func_080091b8(70, 68, 4, 2, 27, 55);
        Func_080091b8(70, 68, 4, 2, 43, 56);
    } else if (scene == 91) {
        Func_080091c8(69, 99, 4, 2, 8, 16);
        Func_080091c8(69, 99, 4, 2, 6, 20);
        Func_080091c8(69, 99, 4, 2, 10, 23);
        Func_080091c0(69, 99, 4, 2, 8, 14);
        Func_080091c0(69, 99, 4, 2, 6, 18);
        Func_080091c0(69, 99, 4, 1, 6, 20);
        Func_080091c0(69, 99, 4, 2, 10, 21);
        Func_080091b8(0, 121, 5, 7, 8, 32);
        Func_080091b8(0, 121, 5, 7, 43, 32);
        Func_080091b8(6, 120, 3, 1, 9, 5);
        Func_080091b8(9, 120, 3, 1, 44, 5);
        Func_080091c0(9, 0, 3, 3, 9, 6);
    }

    Func_0808a0f0(8, 0, 0);
    Func_0808a0f0(9, 0, 0);
    Func_0808a0f0(10, 0, 0);
    Func_0808a0f0(11, 0, 0);
    Func_0808a0f0(12, 0, 0);
    Func_0808a0f0(13, 0, 0);
    for (actor = 100; actor <= 106; actor++)
        Func_0808a3c0(actor, 0, 0);

    if (scene != 92)
        Func_0808a378(0, 0x4000000, 0x10000, 0x2000, 0x10000);
}
