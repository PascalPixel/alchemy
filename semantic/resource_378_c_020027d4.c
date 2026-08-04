typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_0808a330(s32 amount, s32 mode);
extern void Func_0808a348(s32 frames);
extern void Func_0808a010(s32 frames);
extern s32 Func_08015010();
extern void Func_08015078();
extern s32 Func_08015400(s32 index);
extern void Func_08015228(s32 *point, s32 *handle);
extern void Func_08015230(s32 *point, s32 x, s32 y);
extern void Func_08015238(s32 handle);
extern void Func_08015018(s32 panel, s32 mode);

/*
 * Present the two-choice scene panel and track the input cursor until the
 * runtime completion bit becomes set.  Horizontal input toggles the returned
 * choice; the cursor frame comes from the 16-entry table at 0x0200c11c.
 * Complete owner and five-word pool: 0x020027d4-0x0200290b.
 */
s32 Func_020027d4(void)
{
    volatile u32 *complete = (volatile u32 *)0x03001c94;
    volatile u32 *keys = (volatile u32 *)0x03001b04;
    volatile u32 *clock = (volatile u32 *)0x03001800;
    const s32 *cursorFrames = (const s32 *)0x0200c11c;
    s32 point[3];
    s32 handle;
    s32 panel;
    s32 choice = 0;

    Func_0808a0f0(8, 0, 0);
    Func_0808a0f0(9, 0, 0);
    Func_0808a0f0(10, 0, 0);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(11, 0, 0);
    Func_0808a0f0(12, 0, 0);
    Func_0808a0f0(0, 0, 0);
    Func_0808a330(0x10000, 2);
    Func_0808a348(1);
    Func_0808a010(1);

    panel = Func_08015010(2, 7, 25, 5, 1);
    Func_08015078(0x116e, panel, 16, 0);
    if (Func_08015400(1) == 0)
        Func_08015078(0x1170, panel, 16, 16);
    else
        Func_08015078(0x116f, panel, 16, 16);

    Func_08015228(point, &handle);
    Func_08015230(point, 72, 60);
    if ((*complete & 1) == 0) {
        do {
            if ((*keys & 0xc0) != 0)
                choice ^= 1;
            Func_08015230(point,
                          cursorFrames[(*clock >> 1) & 15] + 24,
                          choice * 16 + 60);
            Func_0808a010(1);
        } while ((*complete & 1) == 0);
    }
    Func_08015238(handle);
    Func_08015018(panel, 1);
    return choice;
}
