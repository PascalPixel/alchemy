#include "types.h"

#define U8_AT(base, offset)  (*(u8 *)((u8 *)(base) + (offset)))
#define S8_AT(base, offset)  (*(s8 *)((u8 *)(base) + (offset)))
#define U16_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define U32_AT(base, offset) (*(u32 *)((u8 *)(base) + (offset)))

void *Func_080048b0(s32, s32);
u32 Func_08077300(void);
void Func_0801fd84(void);
void *Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e74c(s32, void *, s32, s32);
void Func_0801e858(void *, void *, s32, s32);
void Func_0801e41c(void *, s32, s32, s32, s32);
void *Func_08021620(s32, void *, s32, s32);
void Func_0801ccc0(s32, s32);
void Func_08020088(void);
void Func_0801ff14(void);
void Func_08016418(void *, s32);
void Func_08020198(void *, void *);
void Func_080030f8(s32);
void Func_0801fe2c(void *, s32, s32, void *);
void Func_08020150(void *, void *);
void Func_0801ffd8(void *, s32, s32);
void Func_08016498(void *);
void Func_0801fda8(void *, s32, s32, s32, s32);
void Func_080216b4(void *);
s32 Func_080022fc(s32, s32);
void Func_080f9010(s32);
void Func_0801fd98(void);
void Func_08002dd8(s32);

static s32 IsSelectable_08020244(u8 *base, s32 selection, s32 mode)
{
    u8 *entry = base + 0x105c + selection * 0x40;

    if (entry[0] == 0)
        return 0;
    if (mode == 1 && S8_AT(entry, 0x15) != 0)
        return 0;
    if (mode == 4 && S8_AT(entry, 0x16) == 0)
        return 0;
    if (mode == 5 && S8_AT(entry, 0x15) == 0)
        return 0;
    return 1;
}

/*
 * Three-slot record selector used by the party-management screens.
 *
 * mode selects the eligibility rule.  The modal view keeps the summary and
 * detail panes alive only while the selected slot has data for them.
 */
s32 Func_08020244(s16 initial_selection, s32 mode)
{
    u8 *base;
    u8 *ui_globals;
    void *allocation;
    void *list_window;
    void *description_window = 0;
    void *stats_window = 0;
    void *extra_window = 0;
    void *motion;
    u32 threshold;
    s32 selection;
    s32 refresh = 1;
    s32 result;
    s32 attempts;
    s32 row;
    s32 dma_source = 0;

    allocation = Func_080048b0(55, 0xa70);
    base = *(u8 **)0x03001f1c;
    ui_globals = *(u8 **)0x03001e8c;
    threshold = Func_08077300();
    selection = initial_selection < 0 ? 0 : initial_selection;

    if (mode != 0) {
        attempts = 0;
        while (!IsSelectable_08020244(base, selection, mode)) {
            selection++;
            if (selection == 3)
                selection = 0;
            attempts++;
            if (attempts > 2)
                return -2;
        }
    }

    *(void **)0x040000d4 = &dma_source;
    *(void **)0x040000d8 = allocation;
    *(u32 *)0x040000dc = 0x8500029c;
    Func_0801fd84();

    list_window = Func_080162d4(1, 2, 28, 7, 2);
    for (row = 0; row < 3; row++) {
        u8 *entry = base + 0x105c + row * 0x40;
        void *record_text = base + 0x1040 + row * 0x40;
        s32 unavailable = 0;

        if (entry[0] == 0)
            unavailable = 0;
        else if (U16_AT(entry, 0x1a) < threshold)
            unavailable = 1;
        else if (U32_AT(entry, 4) != U32_AT(entry, 0x1c))
            unavailable = 3;
        else if (mode == 5 && S8_AT(entry, 0x15) == 0)
            unavailable = 2;
        else {
            Func_0801e858(record_text + 0x10, list_window, 12, row * 16);
            Func_0801e74c(U16_AT(entry, 2) + 0x99b, list_window,
                          62, row * 16);
            ui_globals[0xea3] = 1;
            continue;
        }
        Func_0801e74c(unavailable, list_window, 10, row * 16);
    }
    Func_0801e41c(list_window, 0, 2, 27, 2);
    Func_0801e41c(list_window, 0, 4, 27, 4);
    motion = Func_08021620(mode, list_window, 72, -24);

    for (;;) {
        if (refresh) {
            u8 *entry = base + 0x105c + selection * 0x40;
            void *record = base + 0x1040 + selection * 0x40;
            refresh = 0;

            if (entry[0] != 0) {
                Func_0801ccc0(entry[0x18], entry[0x19]);
                if (extra_window == 0)
                    extra_window = Func_080162d4(1, 10, 14, 9, 2);
                Func_08020198(extra_window, record);
                Func_080030f8(1);

                if (stats_window == 0)
                    stats_window = Func_080162d4(16, 10, 13, 3, 2);
                Func_0801ff14();
                Func_0801fe2c(stats_window, 0, 0, record);
                Func_080030f8(1);

                if (S8_AT(entry, 0x0c) + S8_AT(entry, 0x0d) +
                        S8_AT(entry, 0x0e) + S8_AT(entry, 0x0f) !=
                    0) {
                    if (description_window == 0)
                        description_window =
                            Func_080162d4(16, 14, 13, 5, 2);
                    Func_08020150(description_window, record);
                    Func_08020088();
                    Func_0801ffd8(description_window, 0, 0);
                } else {
                    Func_08020088();
                    Func_08016418(description_window, 2);
                    description_window = 0;
                }
            } else {
                Func_0801ccc0(*(u8 *)0x02000445, *(u8 *)0x02000446);
                Func_08020088();
                Func_0801ff14();
                Func_08016418(description_window, 2);
                Func_08016418(stats_window, 2);
                Func_08016418(extra_window, 2);
                description_window = 0;
                stats_window = 0;
                extra_window = 0;
            }

            Func_08016498(list_window);
            Func_0801e41c(list_window, 0, 2, 27, 2);
            Func_0801e41c(list_window, 0, 4, 27, 4);
            Func_0801fda8(list_window, 0, selection * 2, 26, 1);
        }

        Func_080216b4(motion);
        Func_080030f8(1);

        if (*(u32 *)0x03001b04 & 0x40) {
            Func_080f9010(111);
            refresh = 1;
            do {
                selection = Func_080022fc(selection + 2, 3);
            } while (mode != 0 &&
                     !IsSelectable_08020244(base, selection, mode));
            continue;
        }
        if (*(u32 *)0x03001b04 & 0x80) {
            Func_080f9010(111);
            refresh = 1;
            do {
                selection = Func_080022fc(selection + 4, 3);
            } while (mode != 0 &&
                     !IsSelectable_08020244(base, selection, mode));
            continue;
        }

        if (*(u32 *)0x03001c94 & 2) {
            Func_080f9010(113);
            result = -1;
            break;
        }
        if ((*(u32 *)0x03001c94 & 1) == 0)
            continue;
        Func_080f9010(112);
        result = selection;
        break;
    }

    Func_08020088();
    Func_0801ff14();
    Func_08016418(description_window, 2);
    Func_08016418(stats_window, 2);
    Func_08016418(extra_window, 2);
    Func_08016418(list_window, 2);
    Func_0801fd98();
    Func_08002dd8(55);
    Func_0801ccc0(*(u8 *)0x02000445, *(u8 *)0x02000446);
    Func_080030f8(1);
    return result;
}
