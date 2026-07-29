typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct DebugState {
    u8 unknown_00[4];
    u16 active_04;
};

extern struct DebugState *Data_03001e68;

void Func_080030f8(u32);
s32 Func_080022fc(s32, s32);
void Func_08016418(void *, s32);
void *Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e940(const void *, void *, s32, s32);
void Func_0801ea08(s32, s32, void *, s32, s32);
void Func_08019fcc(s32, s32, void **, void *, s32);
void Func_0801a404(s32, s32, void **, void *, s32);
void *Func_0801eadc(void *, u32, void *, s32, s32);
void *Func_08004080(void);
void Func_0801a2ec(s32, s32, void *);

s32 Func_08029554(void)
{
    s32 column = 0;
    s32 page = 0;
    s32 redraw = 1;
    void *window = 0;

    Data_03001e68->active_04 = 1;
    Func_080030f8(1);

    for (;;) {
        if (*(volatile u32 *)0x03001b04 & 0x20) {
            column--;
            redraw = 1;
        }
        if (*(volatile u32 *)0x03001b04 & 0x10) {
            column++;
            redraw = 1;
        }
        if (*(volatile u32 *)0x03001b04 & 0x200) {
            page--;
            redraw = 1;
        }
        if (*(volatile u32 *)0x03001b04 & 0x100) {
            page++;
            redraw = 1;
        }

        if ((*(volatile u32 *)0x03001b04 & 3) != 0)
            break;

        if (redraw) {
            const void *title;
            s32 first = column * 32;
            s32 i;

            redraw = 0;
            column = (column + 8) & 7;
            page = Func_080022fc(page + 3, 3);

            Func_08016418(window, 2);
            window = Func_080162d4(0x0a, 0, 0x12, 0x0c, 2);
            if (page == 0) {
                title = (const void *)0x08037440;
                Func_0801e940(title, window, 0, 0);
            } else if (page == 1) {
                title = (const void *)0x08037448;
                Func_0801e940(title, window, 0, 0);
            } else {
                Func_0801e940(
                    (const void *)0x08037450, window, 0, 0);
            }
            Func_0801e940((const void *)0x08037458, window, 0, 8);
            Func_0801ea08(column, 0, window, 0x28, 8);
            Func_0801ea08(first, 3, window, 0x40, 8);
            Func_0801e940((const void *)0x08037460, window, 0x58, 8);
            Func_0801ea08(first + 31, 3, window, 0x60, 8);

            for (i = 0; i < 32; i++) {
                void *resource = (void *)-1;
                u32 metadata[2];
                s32 x = (i & 7) * 0x10;
                s32 y = (i >> 3) * 0x10 + 0x10;

                if (page == 0) {
                    Func_08019fcc(
                        first + i, 1, &resource, metadata, page);
                    Func_0801eadc(
                        resource, 0x40000000, window, x, y);
                } else if (page == 1) {
                    Func_0801a404(
                        first + i, 1, &resource, metadata, 0);
                    Func_0801eadc(
                        resource, 0x40000000, window, x, y);
                } else {
                    resource = Func_08004080();
                    Func_0801a2ec(i, 0, resource);
                    Func_0801eadc(
                        resource, 0x40000000, window, x, y);
                }
            }
        }
        Func_080030f8(1);
    }

    Func_08016418(window, 2);
    Data_03001e68->active_04 = 0;
    return 0;
}
