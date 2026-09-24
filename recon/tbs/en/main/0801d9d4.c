/* 2026-09-24: hand-written, 137 differing halfwords (420 of 412 bytes).
   Structure matches; the reference keeps the work pointer in fp, the flag
   in r9, spills the table offset to sp+8 and keeps y and the message id in
   callee-saved registers. */
#include "TYPES.H"

struct UiWindow {
    u8 unknown_00[12];
    u16 x;
    u16 y;
};

extern u8 Data_03001f54;
extern const s8 Data_080367dc[];

s32 GameFlag_TestFar(s32 flag);
struct UiWindow *UiWindow_Create(s32 kind, s32 x, s32 y, s32 width, s32 height);
void UiWindow_DrawDividerLine(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawResource(s32 message, struct UiWindow *window, s32 x, s32 y);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *src);
s32 Func_0801eadc(s32 slot, u32 attr, struct UiWindow *window, s32 flags);
void Func_080b0038(void *object, s32 x, s32 y);
s32 Func_08021750(s32 entry, s32 mode, struct UiWindow *window, s32 x, s32 y, s32 *out);

struct UiWindow *Menu_CreateWorkspaceWindows(void)
{
    u8 *work;
    s32 rows;
    s32 first;
    s32 has_flag;
    s32 width;
    s32 height;
    struct UiWindow *window;
    s32 y;
    s32 slot;

    work = *(u8 **)0x03001ea0;
    rows = 3;
    has_flag = GameFlag_TestFar(382);
    first = 0;
    if (has_flag != 0) {
        first = 2;
        rows = 1;
    }
    if (Data_03001f54 != 0)
        rows += 3;
    width = 8 - rows;
    height = rows * 3 + 1;
    if (width + height > 19) {
        width = 1;
        height = 19;
    }
    window = UiWindow_Create(5, width, 20, height, 2);
    if (rows > 1) {
        s32 line = 3;
        s32 n = rows - 1;

        do {
            UiWindow_DrawDividerLine(window, 0, line, 19, line);
            line += 3;
        } while (--n != 0);
    }
    y = 4;
    if (has_flag == 0) {
        UiText_DrawResource(0xc23, window, 48, 4);
        UiText_DrawResource(0xc24, window, 48, 28);
        y = 52;
    }
    UiText_DrawResource(0xc25, window, 48, y);
    y += 24;
    if (Data_03001f54 != 0) {
        UiText_DrawResource(0xc27, window, 48, y);
        y += 24;
        UiText_DrawResource(0xc28, window, 48, y);
        y += 24;
        UiText_DrawResource(0xc29, window, 48, y);
    }
    slot = Resource_FindFreeEntry();
    if (slot <= 95) {
        VramBlock_LoadCached(slot, 128, (void *)0x080310a4);
        *(s32 *)(work + 0x5a4) = Func_0801eadc(slot, 0x40000000, window, 0);
        Func_080b0038(work + 0x5a4, window->x * 8, window->y * 8 + 16);
    }
    y = -4;
    if (rows > 0) {
        s32 *out = (s32 *)(work + 0x610);
        const s8 *entry = Data_080367dc + first;
        s32 n = rows;

        do {
            *out++ = Func_08021750(*entry++, 0, window, 12, y, out);
            y += 24;
        } while (--n != 0);
    }
    return window;
}

