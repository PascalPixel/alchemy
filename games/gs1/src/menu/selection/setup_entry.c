#include "types.h"
#include "scene.h"
#include "resource.h"

/* menu/selection/setup_entry.c */
/* Sprite record setup for the menu selection list. */

struct Object_0801c0dc {
    u8 filler0[5];
    u8 field_50 : 2;
    u8 field_52 : 2;
    u8 field_54 : 1;
    u8 field_55 : 1;
    u8 field_56 : 2;
    u8 filler6;
    u8 field_70 : 1;
    u8 field_71 : 5;
    u8 field_76 : 2;
    u16 field_80 : 10;
    u16 field_8a : 2;
    u16 field_8c : 4;
};

struct ListNode {
    u8 unk_00[8];
    s16 base;
    s16 kind;
    u16 src;
    s16 tile;
    u8 unk_10[16];
    s16 end;
    s16 field_22;
    u8 unk_24[2];
    s16 field_26;
    struct Object_0801c0dc obj;
};

extern u8 Value_0000001f;
extern u8 Value_00000182;
extern u8 Value_00000333;

/*
 * Load the graphic for kind through one of three tile loaders, record the
 * tile base, source slot and returned tile index in the node, then reset the
 * node's render object.  The object is reached through a pointer so that
 * every byte access stays within Thumb's 5-bit offset range from one base
 * register.  The per-kind tile counts are link-time constants; an integer
 * literal cannot produce 0x1f here.
 */
void Menu_Run(u32 kind, s32 base, struct ListNode *node, s32 reuse)
{
    s32 src;
    s32 tile;
    struct Object_0801c0dc *obj;

    switch (kind) {
    case 1:
    case 6:
        if (reuse != 0)
            src = node->src;
        Menu_SetRange(base, 0, &src, &tile, reuse);
        node->end = base + (s32)&Value_0000001f;
        break;
    case 2:
        if (reuse != 0)
            src = node->src;
        UiIcon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
        node->end = base + (s32)&Value_00000182;
        break;
    case 4:
        if (reuse != 0)
            src = node->src;
        Ability_LoadGlyph(base, 1, (s32)&src, (s32)&tile, reuse);
        node->end = base + (s32)&Value_00000333;
        break;
    }

    node->base = base;
    node->src = src;
    node->tile = tile;
    node->kind = kind;
    node->field_22 = 0x100;
    node->field_26 = 0x100;

    obj = &node->obj;
    obj->field_52 = 0;
    obj->field_55 = 0;
    obj->field_54 = 0;
    /*
     * field_76 = 1 stays before field_56 = 0: both mask with 0x3f, and the
     * mask is copied before the byte-7 store because the byte-5 store still
     * needs it.
     */
    obj->field_76 = 1;
    obj->field_56 = 0;
    obj->field_8c = 0;
    obj->field_80 = tile;
    obj->field_8a = 0;
}

/* ui/text/resource/text_resource_no_op_callback.c */
/* ui/text/resource/no_op_callback.c */
void UiTextResource_NoOpCallback(void)
{
}

/* ui/text/resource/no_op_callback_bank.c */
/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback0(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback1(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback2(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback3(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback4(void)
{
}

/* ui/text/resource/initialize.c */
struct Object_0801c0dc {
    u8 filler0[5];
    u8 field_50 : 2;
    u8 field_52 : 2;
    u8 field_54 : 1;
    u8 field_55 : 1;
    u8 field_56 : 2;
    u8 filler6;
    u8 field_70 : 1;
    u8 field_71 : 5;
    u8 field_76 : 2;
    u16 field_80 : 10;
    u16 field_8a : 2;
    u16 field_8c : 4;
};

s32 Resource_FindFreeSlot(void);
s32 Resource_CopyData(s32, s32, const void *);
extern const u8 gRom[];

void TextResource_Initialize(struct Object_0801c0dc *object, s32 *slot)
{
    const void *data = gRom;
    s32 value = Resource_FindFreeSlot();

    /* ビットフィールドは生成時の設定順を保持する。 */
    *slot = value;
    object->field_80 = Resource_CopyData(value, 0x80, data);
    object->field_52 = 0;
    object->field_54 = 0;
    object->field_55 = 1;
    object->field_50 = 0;
    object->field_71 = 0;
    object->field_76 = 0;
    object->field_56 = 2;
    object->field_8a = 0;
}

/* ui/text/resource/set_position.c */
struct Obj {
    u8 padding[4];
    s8 kind;
    u8 padding2;
    u16 index : 9;
    u16 rest : 7;
};
void Runtime_PushSlotEntry(s32 *arg0, s32 arg1);
void TextResource_SetPosition(struct Obj *obj, s32 arg1, s32 arg2)
{
    obj->index = arg1;
    obj->kind = arg2;
    Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}

/* ui/text/resource/release.c */
s32 Resource_ResetEntry(s32);

/* 受け取った値を呼出し先へ渡す。 */
void TextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}

/* menu/resource_transfer/load_selected.c */
struct SelectionNode_0801c188 {
    u8 padding_00[8];
    u16 no;
    u16 type;
};

struct TransferState_0801c188 {
    u8 padding_00[8];
    u16 no;
    u16 active;
    u16 handle;
    u16 transfer_id;
    u8 padding_10[18];
    s16 x;
    s16 y;
    s16 width;
};

struct ResourceBuffer_0801c188 {
    u8 payload[0x604];
    void *resource;
};

extern u8 *gIw;
extern u8 Value_000000f1;

struct SelectionNode_0801c188 *Menu_RunLoadSelected(void *state);
struct ResourceBuffer_0801c188 *Runtime_AllocateHeapBlock(s32 owner, s32 size);

u16 Resource_FindFreeSlot(void);
u16 Resource_CopyData(s32 handle, s32 size, void *buffer);

void Menu_LoadSelectedResource(void)
{
    u8 *state = gIw;
    struct SelectionNode_0801c188 *selection = Menu_RunLoadSelected(state);
    struct TransferState_0801c188 *transfer;
    struct ResourceBuffer_0801c188 *buffer;
    u8 *tbl;
    void *resource;
    s32 no;

    if (selection->type != 1 && selection->type != 6)
        return;

    buffer = Runtime_AllocateHeapBlock(17, 0x608);
    transfer = (struct TransferState_0801c188 *)(state + 0x30C);
    no = selection->no;
    tbl = GetResource((s32)&Value_000000f1);
    {
        void **destination = &buffer->resource;
        resource = tbl
            + *(u16 *)(tbl + selection->no * 2);
        *destination = resource;
    }
    Menu_Apply(resource, buffer);

    if (transfer->active == 0)
        transfer->handle = Resource_FindFreeSlot();
    transfer->transfer_id =
        Resource_CopyData(transfer->handle, 0x400, buffer);
    transfer->active = 1;
    transfer->no = no;
    transfer->x = 40;
    transfer->y = 40;
    transfer->width = 240;
    Menu_Do(17);
}
