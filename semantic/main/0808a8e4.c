#include "types.h"

struct MapDescriptor_0808a8e4 {
    s16 resource;
    s8 flag_group;
    s8 map_mode;
    u16 display_resource;
    u16 unknown_06;
};

struct DmaRegisters_0808a8e4 {
    const void *source;
    void *destination;
    u32 control;
};

extern const struct MapDescriptor_0808a8e4 Data_0809f1a8[];

void *Func_08004938(s32);
void Func_08002df0(void *);
void Func_0800307c(s32, s32, s32);
void Func_0800403c(void);
void Func_080040e8(void);
void Func_08004760(void);
void Func_08004858(void);
void Func_08015360(u8, u8);
void Func_08077098(void);
s32 Func_080770c0(s32);
void Func_080770d0(s32);
void Func_0808a5f8(s32);
void Func_0808a6e4(s16);
void Func_0808ab48(void);
void Func_0808ab74(s16, s32);
void Func_0808acc4(void);
void Func_0808b090(void);
s32 Func_0808c4f8(void);
s16 Func_080b50a0(s16);
s16 Func_080f4000(void);
s16 Func_080f6000(void);
void Func_080f9010(s32);

/*
 * Enter the selected map and keep dispatching map transitions. Special map
 * identifiers invoke the palette-backed selection screens before returning
 * their result to the ordinary transition path.
 */
void Func_0808a8e4(s32 entry_mode)
{
    volatile u8 *const startup_flag = (volatile u8 *)0x03001f54;
    volatile s16 *const map_id = (volatile s16 *)0x02000400;
    volatile s16 *const map_variant = (volatile s16 *)0x02000402;
    volatile u8 *const entry_a = (volatile u8 *)0x02000445;
    volatile u8 *const entry_b = (volatile u8 *)0x02000446;
    volatile s16 *const selected_entry = (volatile s16 *)0x0200045e;
    volatile u16 *const display_resource = (volatile u16 *)0x0200041a;
    volatile u16 *const dma0_control = (volatile u16 *)0x040000ba;
    volatile struct DmaRegisters_0808a8e4 *const dma3 =
        (volatile struct DmaRegisters_0808a8e4 *)0x040000d4;
    void *const palette = (void *)0x050001c0;

    if (*startup_flag != 0 && entry_mode == 1) {
        *map_id = 5;
        *map_variant = 1;
    } else if (*startup_flag != 0 && entry_mode == 2) {
        *map_id = 1;
        *map_variant = 1;
    } else {
        Func_08077098();
        *map_id = 0;
        *map_variant = 2;
    }

    Func_08015360(*entry_a, *entry_b);
    Func_0800403c();
    Func_080040e8();
    Func_080040e8();

    for (;;) {
        const struct MapDescriptor_0808a8e4 *descriptor;
        void *saved_palette;
        s16 selection;

        if (Func_080770c0(0x101) != 0)
            Func_080770d0(0x101);
        else
            Func_080f9010(0x120);

        descriptor = &Data_0809f1a8[*map_id];
        selection = *map_variant;

        *dma0_control &= 0xc5ff;
        *dma0_control &= 0x7fff;
        Func_080040e8();
        Func_0800307c(1, 0, 0);
        Func_0800307c(2, 0, 0);
        Func_08004858();
        Func_08004760();
        Func_0800403c();

        switch (*map_id) {
        case 0x1fb:
            selection = 0;
            break;

        case 0x1fc: {
            saved_palette = Func_08004938(0x40);

            dma3->source = palette;
            dma3->destination = saved_palette;
            dma3->control = 0x84000010;
            selection = Func_080f6000();
            goto restore_palette;
        }

        case 0x1fd: {
            saved_palette = Func_08004938(0x40);

            dma3->source = palette;
            dma3->destination = saved_palette;
            dma3->control = 0x84000010;
            selection = Func_080f4000();

restore_palette:
            dma3->source = saved_palette;
            dma3->destination = palette;
            dma3->control = 0x84000010;
            Func_08002df0(saved_palette);
            break;
        }

        case 0x1fe:
            selection = Func_080b50a0(selection);
            break;

        default:
            Func_0808ab74(*map_id, Func_080770c0(0x109));
            Func_0808b090();

            if (Func_080770c0(0x109) == 0) {
                if (Func_080770c0(0x11a) == 0 &&
                    Func_080770c0(0x11b) == 0) {
                    Func_0808acc4();
                } else {
                    Func_080770d0(0x11a);
                }
            } else if (*selected_entry != -1) {
                Func_080f9010(*selected_entry);
            } else {
                Func_0808acc4();
            }

            *display_resource = descriptor->display_resource;
            Func_0808ab48();
            Func_0808a5f8(Func_0808c4f8());
            continue;
        }

        Func_0808a6e4(selection);
    }
}
