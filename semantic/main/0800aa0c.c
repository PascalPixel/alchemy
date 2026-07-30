#include "types.h"

typedef void (*Transfer2_0800aa0c)(void *, u32);
typedef s32 (*TransferResult2_0800aa0c)(const void *, void *);
typedef void (*Render_0800aa0c)(const void *, void *, u32);
typedef void (*Upload_0800aa0c)(void *, u32, u32, void *);

struct Dma_0800aa0c {
    const void *source;
    void *destination;
    u32 control;
};

struct Glyph_0800aa0c {
    u8 _00[2];
    s16 advance;
    u8 command;
    u8 transfer_mode;
    u8 priority;
    u8 source_kind;
    const u32 *frames;
    u8 _0c[4];
    const u8 *text;
    u8 text_position;
    u8 baseline;
    u8 frame;
    u8 previous_frame;
};

struct TextOwner_0800aa0c {
    u8 _00[8];
    u16 tile_base;
    u8 _0a[18];
    u8 allocation_slot;
    u8 _1d;
    u8 _1e[2];
    u8 width;
    u8 height;
    s8 interior_colour;
    s8 border_colour;
    u8 active_command;
    u8 dirty;
    u8 flags;
    u8 glyph_count;
    struct Glyph_0800aa0c *glyphs[1];
};

void *Func_080048b0(s32 kind, u32 size);
void *Func_08004938(u32 size);
void Func_08002df0(void *allocation);
void Func_08002dd8(s32 kind);
void Func_08005340(const void *source, void *destination);
void Func_0800a97c(const void *source, void *destination);
void Func_0800b9f4(struct Glyph_0800aa0c *glyph, u8 parameter);
s32 Func_08003fa4(u32 slot, u32 size, s32 source);

static u8 CommandOffset_0800aa0c(u8 command, s16 style)
{
    const u8 *table;
    u32 index;

    switch (command) {
    case 1:
        table = (const u8 *)0x0801307c;
        index = (u16)style >> 13;
        break;
    case 2:
    case 20:
        table = (const u8 *)0x08013094;
        index = (u16)style >> 13;
        break;
    case 3:
        table = (const u8 *)0x0801309c;
        index = (u16)style >> 12;
        break;
    case 4:
        table = (const u8 *)0x080130cc;
        index = (u16)style >> 10;
        break;
    case 5:
        table = (const u8 *)0x080130ac;
        index = (u16)style >> 12;
        break;
    case 6:
        table = (const u8 *)0x0801310c;
        index = (u16)style >> 10;
        break;
    case 8:
        table = (const u8 *)0x080130bc;
        index = ((u32)(style << 16) + 0x10000000) >> 29;
        break;
    case 22:
        table = (const u8 *)0x0801308c;
        index = (u16)style >> 13;
        break;
    case 88:
        table = (const u8 *)0x080130c4;
        index = ((u32)(style << 16) + 0x10000000) >> 29;
        break;
    default:
        return 0;
    }
    return table[index];
}

s32 Func_0800aa0c(struct TextOwner_0800aa0c *owner, s16 style)
{
    struct Glyph_0800aa0c *glyph;
    u8 *runtime = *(u8 **)0x03001e68;
    Render_0800aa0c render = *(Render_0800aa0c *)0x03001f20;
    u32 render_was_present = 1;
    u32 changed = 0;
    u32 i;

    if (render == 0) {
        volatile struct Dma_0800aa0c *dma =
            (volatile struct Dma_0800aa0c *)0x040000d4;
        Func_080048b0(52, 0x2c4);
        render = *(Render_0800aa0c *)0x03001f20;
        dma->source = (const void *)0x08009bb8;
        dma->destination = render;
        dma->control = 0x84000000 | ((0x08009d9c - 0x08009bb8) >> 2);
        render_was_present = 0;
    }

    for (i = 0; i < owner->glyph_count; i++) {
        u8 frame;
        u8 command_offset;
        glyph = owner->glyphs[i];
        if (glyph == 0 || glyph->text == 0)
            continue;

        if (glyph->advance <= 0) {
            u8 command = glyph->text[glyph->text_position++];
            u8 parameter = glyph->text[glyph->text_position++];

            switch (command) {
            case 0xef:
                glyph->previous_frame = 0xff;
                glyph->text = 0;
                owner->glyph_count--;
                frame = 0xff;
                break;
            case 0xf0:
                glyph->previous_frame = 0xff;
                glyph->advance += (s16)(parameter << 4);
                frame = 0xff;
                break;
            case 0xf1:
                glyph->command = parameter;
                continue;
            case 0xf2:
            case 0xf3:
            case 0xf4:
            case 0xf6:
            case 0xf7:
            case 0xf8:
            case 0xf9:
            case 0xfa:
            case 0xfc:
                continue;
            case 0xf5:
                glyph->advance += (s16)(parameter << 4);
                continue;
            case 0xfd:
                glyph->text_position = parameter;
                continue;
            case 0xfe:
                Func_0800b9f4(glyph, parameter);
                owner->active_command = parameter;
                continue;
            case 0xff:
                glyph->text_position -= 2;
                frame = glyph->previous_frame;
                break;
            default:
                glyph->advance += (s16)(parameter << 4);
                glyph->previous_frame = command;
                frame = command;
                break;
            }
        } else {
            glyph->advance -= glyph->baseline;
            frame = glyph->previous_frame;
        }

        command_offset = CommandOffset_0800aa0c(glyph->command, style);
        frame += command_offset & 7;
        if (i == 0 && (command_offset >> 7) != 0)
            changed = 1;
        if (glyph->frame != frame) {
            glyph->frame = frame;
            owner->dirty = 1;
        }
    }

    if (owner->dirty != 0) {
        u32 area = owner->width * owner->height;
        u8 *canvas = Func_08004938(area);
        u16 order[4];
        s32 order_last = -1;
        s32 position;

        ((Transfer2_0800aa0c)0x030005c0)(canvas, area);

        for (position = owner->glyph_count - 1; position >= 0; position--) {
            glyph = owner->glyphs[position];
            if (glyph != 0 && glyph->frames != 0 &&
                glyph->frame != 0xff && glyph->priority <= 3) {
                u16 key = (glyph->priority << 8) | position;
                s32 insert = order_last;
                while (insert >= 0 && order[insert] > key) {
                    order[insert + 1] = order[insert];
                    insert--;
                }
                order[insert + 1] = key;
                order_last++;
            }
        }

        for (position = 0; position <= order_last; position++) {
            glyph = owner->glyphs[(u8)order[position]];
            if (glyph->source_kind == 1) {
                Func_08005340((const void *)glyph->frames[glyph->frame],
                              canvas);
            } else if (glyph->source_kind == 3) {
                if (glyph->transfer_mode != 0) {
                    void *temporary = Func_08004938(0x400);
                    Func_0800a97c(
                        (const void *)glyph->frames[glyph->frame], temporary);
                    render(temporary, canvas, glyph->transfer_mode);
                    Func_08002df0(temporary);
                } else {
                    const void *source =
                        (const void *)glyph->frames[glyph->frame];
                    if (((TransferResult2_0800aa0c)0x03000164)(
                            source, canvas) != 0)
                        render(source, canvas, 0);
                }
            } else {
                render((const void *)glyph->frames[glyph->frame], canvas,
                       glyph->transfer_mode);
            }
        }

        if ((owner->flags & 2) != 0) {
            u8 *mask = Func_08004938(area);
            u32 x;
            u32 y;

            ((Transfer2_0800aa0c)0x03000164)(mask, area);
            for (y = 1; y < owner->height - 1; y++) {
                for (x = 1; x < owner->width - 1; x++) {
                    u32 at = y * owner->width + x;
                    if (canvas[at - 1] != 0 && canvas[at + 1] != 0 &&
                        canvas[at - owner->width] != 0 &&
                        canvas[at + owner->width] != 0)
                        mask[at] = 1;
                }
            }
            for (i = 0; i < area; i++) {
                if (mask[i] != 0)
                    canvas[i] = owner->border_colour;
                else if (canvas[i] != 0)
                    canvas[i] = owner->interior_colour;
            }
            Func_08002df0(mask);
        }

        {
            u32 tile = Func_08003fa4(owner->allocation_slot, area, 0);
            Upload_0800aa0c upload = *(Upload_0800aa0c *)0x03001f24;
            void *destination =
                (void *)((u8 *)0x06010000 + tile * 32);

            upload(canvas, owner->width, owner->height, destination);
            owner->tile_base =
                (owner->tile_base & 0xfc00) | (tile & 0x03ff);
            owner->dirty = 0;
            *(u16 *)runtime += area;
        }
        Func_08002df0(canvas);
    }

    if (!render_was_present)
        Func_08002dd8(52);
    return changed;
}
