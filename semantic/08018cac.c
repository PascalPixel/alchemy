/*
 * The call through the heap workspace is an audited GCC call-via-r6 veneer.
 * The copied 0x318-byte ARM routine takes five real arguments; it is modeled
 * as an ordinary typed function pointer and contains no C asm embedding.
 */
#include "layout_guard.h"
#include "types.h"

typedef struct WindowFrame_08018cac {
    u8 padding00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u8 padding10[6];
    u16 flags;
} WindowFrame_08018cac;

typedef struct OamCommand_08018cac {
    struct OamCommand_08018cac *next;
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 affine;
} OamCommand_08018cac;

typedef struct SpriteNode_08018cac {
    struct SpriteNode_08018cac *next;
    u8 active;
    u8 kind;
    s16 x;
    s16 y;
    u8 padding0a[2];
    s16 affine;
    u8 oam_slot;
    u8 terminator;
    OamCommand_08018cac oam;
} SpriteNode_08018cac;

typedef struct DisplayState_08018cac {
    u8 padding000[0x58];
    void **special_objects;
    u8 padding05c[0x63c];
    SpriteNode_08018cac sprites[64];
    u8 paddingd98[0x110];
    u16 vertical_offset;
    u8 paddingeaa[2];
    u16 display_mode;
    u8 paddingeae[0x402];
    u16 horizontal_offset;
    u8 padding12b2[4];
    u16 marker_resource;
    u16 tile_base;
} DisplayState_08018cac;

typedef struct DmaTransfer_08018cac {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer_08018cac;

typedef s32 (*HeapKernel_08018cac)(
    WindowFrame_08018cac *object,
    s32 character,
    s32 x,
    s32 y,
    s32 alternate);

LAYOUT_OFFSET_GUARD(
    WindowFrame08018cac_Position,
    WindowFrame_08018cac,
    x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    WindowFrame08018cac_Flags,
    WindowFrame_08018cac,
    flags,
    0x16);
LAYOUT_SIZE_GUARD(
    OamCommand08018cac_Size,
    OamCommand_08018cac,
    0x0c);
LAYOUT_OFFSET_GUARD(
    SpriteNode08018cac_OamSlot,
    SpriteNode_08018cac,
    oam_slot,
    0x0e);
LAYOUT_OFFSET_GUARD(
    SpriteNode08018cac_Oam,
    SpriteNode_08018cac,
    oam,
    0x10);
LAYOUT_SIZE_GUARD(
    SpriteNode08018cac_Size,
    SpriteNode_08018cac,
    0x1c);
LAYOUT_OFFSET_GUARD(
    DisplayState08018cac_SpecialObjects,
    DisplayState_08018cac,
    special_objects,
    0x58);
LAYOUT_OFFSET_GUARD(
    DisplayState08018cac_Sprites,
    DisplayState_08018cac,
    sprites,
    0x698);
LAYOUT_OFFSET_GUARD(
    DisplayState08018cac_VerticalOffset,
    DisplayState_08018cac,
    vertical_offset,
    0x0ea8);
LAYOUT_OFFSET_GUARD(
    DisplayState08018cac_DisplayMode,
    DisplayState_08018cac,
    display_mode,
    0x0eac);
LAYOUT_OFFSET_GUARD(
    DisplayState08018cac_HorizontalOffset,
    DisplayState_08018cac,
    horizontal_offset,
    0x12b0);
LAYOUT_OFFSET_GUARD(
    DisplayState08018cac_MarkerResource,
    DisplayState_08018cac,
    marker_resource,
    0x12b6);
LAYOUT_OFFSET_GUARD(
    DisplayState08018cac_TileBase,
    DisplayState_08018cac,
    tile_base,
    0x12b8);

extern DisplayState_08018cac *Data_03001e8c;

void Func_08002df0(void *);
s32 Func_08002f40(s32);
void *Func_08004938(s32);
SpriteNode_08018cac *Func_08015e8c(void);
void Func_08016584(WindowFrame_08018cac *, SpriteNode_08018cac *);
s32 Func_080178b0(s32, void *);
s32 Func_08004080(void);

/* Build one display record for a field object's encoded character. */
s32 Func_08018cac(
    WindowFrame_08018cac *object,
    s32 character,
    s32 x,
    s32 y,
    s32 forced)
{
    DisplayState_08018cac *state = Data_03001e8c;
    u8 tile_data[128];
    SpriteNode_08018cac *record;
    s32 result;
    u16 record_tile_offset;

    if (forced != 1 && (object->flags & 8) != 0) {
        s32 alternate;

        if (*state->special_objects == object) {
            Func_08002f40(0x14);
            Func_08002f40(0x13);
            if (character == 0x20)
                return 3;
        }

        alternate = Func_08002f40(0x13);
        if (character == 0x20)
            return 4;

        {
            void *workspace = Func_08004938(0x318);
            volatile DmaTransfer_08018cac *dma =
                (volatile DmaTransfer_08018cac *)0x040000d4;

            dma->source = (const void *)0x080155d0;
            dma->destination = workspace;
            dma->control = 0x84000000 | (0x318 >> 2);
            result = ((HeapKernel_08018cac)workspace)(
                object, character, x, y, alternate);
            Func_08002df0(workspace);
        }
        return result;
    }

    if (character == 0x20)
        return 5;

    record = Func_08015e8c();
    if (record == 0)
        return 0;

    record_tile_offset = (u16)((record - state->sprites) * 4);
    record->kind = 1;
    record->active = 0;

    if (forced == 1) {
        result = 1;
        record->kind = 2;
    } else {
        switch (state->display_mode) {
        case 2:
            record->kind = 4;
            record->affine = 0;
            break;
        case 3:
            record->kind = 5;
            break;
        case 4:
            record->kind = 6;
            record->affine = 8;
            break;
        case 5:
            record->kind = 7;
            record->affine = 0;
            break;
        }

        result = Func_080178b0(character, tile_data);
        if (result == 0)
            result = 1;
    }

    if (record->kind == 2) {
        if (state->marker_resource == 0x63)
            state->marker_resource = (u16)Func_08004080();

        record->oam.attr1 =
            (record->oam.attr1 & 0xfe00) |
            (((object->x + object->width - 2) * 8 + 4) & 0x01ff);
        ((u8 *)&record->oam.attr0)[0] =
            (u8)(((u8)object->y + (u8)object->height - 2) * 8 - 1);
    } else {
        volatile DmaTransfer_08018cac *dma =
            (volatile DmaTransfer_08018cac *)0x040000d4;
        s32 tile = state->tile_base + record_tile_offset;

        dma->source = tile_data;
        dma->destination = (void *)(0x06010000 + (tile << 5));
        dma->control = 0x84000020;
        record->oam.attr0 =
            y + (state->vertical_offset >> 1) + (object->y << 3) - 2;
        record->oam.attr1 =
            (x + (state->horizontal_offset >> 1) +
             (object->x << 3) + 2) | 0x4000;
        record->oam.attr2 = (u16)tile;
    }

    record->terminator = 0xfe;
    record->x = record->oam.attr1 & 0x01ff;
    record->y = ((u8 *)&record->oam.attr0)[0];
    record->oam_slot = (u8)record_tile_offset;
    record->next = 0;
    Func_08016584(object, record);
    return result;
}
