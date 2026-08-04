#include "types.h"

struct MenuRecord_08012518 {
    s16 resource;
    u16 value;
    u8 enabled;
    s8 option;
    s8 palette;
    u8 padding_07;
};

struct RenderState_08012518 {
    u8 bytes[0x27];
};

struct Metadata_08012518 {
    u8 unknown_00[4];
    u8 kind;
};

struct DmaTransfer_08012518 {
    const void *source;
    void *destination;
    u32 control;
};

void Func_08002f0c(
    s32 mode,
    struct Metadata_08012518 *(*lookup)(s32));
void Func_080030f8(s32 frames);
void Func_08003bb4(s32 mode);
void Func_0800403c(void);
void Func_080040e8(void);
void Func_080041d8(const void *callback, s32 size);
void Func_08004858(void);
void *Func_080048f4(s32 kind, s32 size);
void Func_0800b6b8(
    u32 layer,
    void *destination,
    s32 resource,
    s32 character_map);
s32 Func_0800b8ac(struct RenderState_08012518 *state, s32 resource);
struct RenderState_08012518 *Func_0800bc70(s32 resource);
void Func_0800c004(s32 mode);
void Func_08012b2c(s32 x, s32 y, void *workspace);
s32 Func_08012af8(s32 start, s32 step);
void Func_08012d20(s32 slot, s32 value);
void Func_08012d48(s32 slot, s32 value);
void Func_08012d70(s32 slot, s32 value);
void Func_08012de8(s32 slot, s32 resource);
void Func_08012e28(void);
struct Metadata_08012518 *Func_08185000(s32 resource);

static void SyncRecord_08012518(
    s32 slot,
    const struct MenuRecord_08012518 *record)
{
    Func_08012de8(slot, record->resource);
    Func_08012d48(slot, record->option);
    Func_08012d20(slot, record->palette);
    Func_08012d70(slot, record->value);
}

void Func_08012518(void)
{
    struct MenuRecord_08012518 records[4];
    struct DmaTransfer_08012518 *dma =
        (struct DmaTransfer_08012518 *)0x040000d4;
    void *workspace;
    s32 x = 144;
    s32 y = 96;
    s32 edit_field = 1;
    s32 visible = 1;
    s32 selected = 0;
    s32 zero = 0;
    s32 index;

    workspace = Func_080048f4(9, 160);
    *(volatile u8 *)0x03001c90 = 3;
    dma->source = &zero;
    dma->destination = workspace;
    dma->control = 0x85000001;
    zero = 0;
    dma->source = &zero;
    dma->destination = records;
    dma->control = 0x85000008;

    records[0].resource = (s16)Func_08012af8(-1, 1);
    for (index = 0; index < 4; index++) {
        records[index].value = 1;
        records[index].option = 1;
        records[index].resource = records[0].resource;
    }
    records[0].enabled = 1;

    *(volatile u8 *)0x03001f54 = 2;
    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x05000000 = 0x01e0;
    *(volatile u16 *)0x04000000 = 0x1140;
    Func_08003bb4(1);

restart:
    Func_08004858();
    Func_080040e8();
    workspace = Func_080048f4(9, 160);
    Func_0800403c();
    Func_0800c004(2);
    Func_0800b6b8(0, (void *)0x02010000, records[0].resource, 0);

    if (Func_08185000(records[0].resource)->kind == 20) {
        Func_0800b6b8(
            1,
            (void *)0x02018000,
            records[0].resource + 1,
            0);
    }

    for (index = 0; index < 10; index++) {
        struct RenderState_08012518 *state;
        s32 alternate =
            Func_08185000(records[0].resource)->kind == 20 &&
            (index & 1) != 0;

        state = Func_0800bc70(
            records[0].resource + alternate + (alternate << 12));
        Func_0800b8ac(state, records[1].resource);
        Func_0800b8ac(state, records[2].resource);
        Func_0800b8ac(state, records[3].resource);
        state->bytes[0x26] = visible;
    }

    for (index = 0; index < 4; index++) {
        Func_08012d48(
            index,
            records[index].enabled ? records[index].option : 8);
        Func_08012d20(index, records[index].palette);
        Func_08012d70(index, records[index].value);
    }

    Func_08012b2c(x, y, workspace);
    Func_080041d8((const void *)Func_08012e28, 3200);

    for (;;) {
        volatile u32 *held = (volatile u32 *)0x03001ae8;
        volatile u32 *pressed = (volatile u32 *)0x03001b04;
        u32 changed = 0;

        Func_080030f8(1);

        if ((*held & 8) != 0) {
            if ((*held & 0x20) != 0)
                x--;
            if ((*held & 0x10) != 0)
                x++;
            if ((*held & 0x40) != 0)
                y--;
            if ((*held & 0x80) != 0)
                y++;
            Func_08012b2c(x, y, workspace);
        } else {
            if ((*pressed & 0x40) != 0)
                selected = (selected - 1) & 3;
            if ((*pressed & 0x80) != 0)
                selected = (selected + 1) & 3;
            if ((*pressed & 0x20) != 0)
                edit_field = (edit_field - 1) & 3;
            if ((*pressed & 0x10) != 0)
                edit_field = (edit_field + 1) & 3;
        }

        if ((*(volatile u32 *)0x03001c94 & 8) != 0) {
            u8 *entry = *(u8 **)0x03001e60 + 0x26;

            visible ^= 1;
            for (index = 0; index < 10; index++) {
                *entry = visible;
                entry += 0x38;
            }
        }

        switch (edit_field) {
        case 0:
            if (selected != 1) {
                if ((*pressed & 0x200) != 0) {
                    if (records[selected].value != 0)
                        records[selected].value--;
                    if (selected == 0)
                        records[1].value = records[0].value;
                    continue;
                }
                if ((*pressed & 0x100) != 0) {
                    if (records[selected].value < 99)
                        records[selected].value++;
                    if (selected == 0)
                        records[1].value = records[0].value;
                    continue;
                }
            }
            break;

        case 1: {
            u32 repeat = (*held & 8) != 0 ? 10 : 1;
            u32 count;

            if ((*pressed & 0x200) != 0) {
                for (count = 0; count < repeat; count++) {
                    records[selected].resource =
                        (s16)Func_08012af8(records[selected].resource, -1);
                }
                changed = 1;
            }
            if ((*pressed & 0x100) != 0) {
                for (count = 0; count < repeat; count++) {
                    records[selected].resource =
                        (s16)Func_08012af8(records[selected].resource, 1);
                }
                changed = 1;
            }

            if (changed != 0) {
                if (selected == 0)
                    goto restart;
                if (records[selected].enabled)
                    SyncRecord_08012518(selected, &records[selected]);
                continue;
            }
            break;
        }

        case 2:
            if ((*pressed & 0x200) != 0) {
                records[selected].option--;
                if (records[selected].option < 0)
                    records[selected].option = 3;
                changed = 1;
            }
            if ((*pressed & 0x100) != 0) {
                records[selected].option++;
                if (records[selected].option > 3)
                    records[selected].option = 0;
                changed = 1;
            }
            if (changed != 0) {
                if (records[selected].enabled)
                    Func_08012d48(selected, records[selected].option);
                continue;
            }
            break;

        case 3:
            if ((*pressed & 0x200) != 0) {
                records[selected].palette--;
                if (records[selected].palette < 0)
                    records[selected].palette = 15;
                changed = 1;
            }
            if ((*pressed & 0x100) != 0) {
                records[selected].palette++;
                if (records[selected].palette > 15)
                    records[selected].palette = 0;
                changed = 1;
            }
            if (changed != 0) {
                if (records[selected].enabled)
                    Func_08012d20(selected, records[selected].palette);
                continue;
            }
            break;
        }

        if ((*pressed & 1) != 0) {
            if (edit_field == 0) {
                if (selected != 1 && records[selected].enabled) {
                    Func_08012d70(selected, records[selected].value);
                    if (selected == 0)
                        Func_08012d70(1, records[1].value);
                    continue;
                }
            } else if (selected != 0) {
                records[selected].enabled ^= 1;
                Func_08012d48(
                    selected,
                    records[selected].enabled
                        ? records[selected].option
                        : 8);
                continue;
            }
        }

        if ((*pressed & 4) != 0) {
            Func_080040e8();
            Func_08002f0c(
                (*held & 2) != 0 ? 17 : 18,
                Func_08185000);
            goto restart;
        }
    }
}
