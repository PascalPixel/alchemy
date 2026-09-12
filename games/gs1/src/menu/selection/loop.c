#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "sound_ids.h"
#include "resource.h"

/* menu/selection/loop.c */
/* menu/selection/selection_loop.c */
/* menu/selection/loop.c */
extern u8 *gIw;
extern u32 gIw2;
extern volatile u32 gIw3;

void WaitFrames(s32);

s32 Menu_SelectionLoop(s32 mode)
{
    u8 *state = gIw;

    Menu_Apply(state, 0);
    for (;;) {
        WaitFrames(1);
        if (*(u16 *)(state + 0x3a0) != 0) {
            continue;
        }
        if (mode != 0x3e7) {
            if (gIw3 & 0x10) {
                Menu_StepRight(state);
            } else if (gIw3 & 0x20) {
                Menu_StepLeft(state);
            } else if (gIw2 & 1) {
                return Menu_ConfirmSelection(state);
            }
        }
        if (mode != 0 && (gIw2 & 2)) {
            return -1;
        }
    }
}

/* menu/selection/wait_for_input.c */
extern void Audio_PlayCue(u32);

u32 Menu_WaitForSelectionInput(u32 value)
{
    u8 *state = *(u8 **)ADDR_03001E98;
    volatile u32 *input;
    u32 result;

again:
    WaitFrames(1);
    if (*(u16 *)(state + 0x3a0) != 0)
        goto again;

    if (value != 999) {
        input = (u32 *)ADDR_03001B04;
        if (*input & 0x10) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            Menu_StepRight(state);
        } else if (*input & 0x20) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            Menu_StepLeft(state);
        }

        if (*(u32 *)ADDR_03001C94 & 1) {
            result = *(u16 *)(state + 0x39c)
                   + *(u16 *)(state + 0x39e);
            if (*(u16 *)(*(u8 **)(state + 0x348) + 10) == 6) {
                if (result == 0)
                    Audio_PlayCue(SOUND_MENU_CONFIRM);
                else
                    Audio_PlayCue(SOUND_MENU_CANCEL);
            } else {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
            }
            return result;
        }
    }

    if (value != 0 && (*(u32 *)ADDR_03001C94 & 2)) {
        Audio_PlayCue(SOUND_MENU_CANCEL);
        return -1;
    }
    goto again;
}

/* menu/selection/move_forward.c */
struct State_0801b9a8;
struct State_0801b9ec;


void Menu_MoveSelectionForward(u8 *state)
{
    u16 *selection = (u16 *)(state + 0x39c);
    u16 *index = (u16 *)(state + 0x39e);
    u16 *count;
    u32 end = *selection + *index + 1;

    count = (u16 *)(state + 0x394);
    if (end != *count) {
        Menu_Apply(state, *index);
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 33;

            *status = value;
        }
        WaitFrames(1);
        *index += 1;
        if (*index == 4 && end + 1 < *count) {
            *index += 0xffff;
            {
                u32 value = 8;

                *(u16 *)(state + 60) = value;
            }
            *selection += 1;
            Menu_Apply2(state, 1);
            if (*selection + *index + 2 == *count) {
                u32 value = 0;

                *(u16 *)(state + 62) = value;
            }
            {
                u32 value = 1;

                *(u16 *)(state + 10) = value;
            }
        }
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 1;

            *status = value;
            Menu_Apply3(state, *(u16 *)(state + 0x39e));
        }
        WaitFrames(1);
        Menu_Apply4(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
        WaitFrames(1);
    }
}

/* menu/selection/move_backward.c */

void Menu_MoveSelectionBackward(u8 *state)
{
    u32 *selection = (u32 *)(state + 0x39c);

    if (*selection != 0) {
        u32 no;

        Menu_Apply(state, *(u16 *)(state + 0x39e));
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 33;

            *status = value;
        }
        WaitFrames(1);
        no = *(u16 *)(state + 0x39e);
        if (no == 1 && *(u16 *)selection != 0) {
            {
                u32 value = 8;

                *(u16 *)(state + 8) = value;
            }
            *(u16 *)selection += 0xffff;
            Menu_Apply2(state, 0);
            if (*(u16 *)selection == 0) {
                *(u16 *)(state + 10) = 0;
            }
            *(u16 *)(state + 62) = no;
        } else {
            *(u16 *)(state + 0x39e) += 0xffff;
        }
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 1;

            *status = value;
            Menu_Apply3(state, *(u16 *)(state + 0x39e));
        }
        WaitFrames(1);
        Menu_Apply4(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
        WaitFrames(1);
    }
}

/* menu/selection/reload_node_resource.c */
struct Node_0801b9a8 {
    u8 filler0[4];
    struct Node_0801b9a8 *next;
    u8 filler8[2];
    u16 type;
    u16 value;
    u8 filler14[18];
    u16 base;
};

struct State_0801b9a8 {
    u8 filler0[0x348];
    struct Node_0801b9a8 *head;
};

extern u8 gVal;

void Menu_ReloadNodeResource(struct State_0801b9a8 *state, u32 index)
{
    struct Node_0801b9a8 *node = state->head;
    u32 output;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 first = node->base - (u32)&gVal;

        value = node->value;
        Menu_SetRange(first, 0, &value, &output, 1);
    }
}

/* menu/selection/load_node_resource.c */
struct Node_0801b9ec {
    u8 filler0[4];
    struct Node_0801b9ec *next;
    u8 filler8[2];
    u16 type;
    u16 value;
    u8 filler14[18];
    u16 base;
};

struct State_0801b9ec {
    u8 filler0[0x348];
    struct Node_0801b9ec *head;
};


void Menu_LoadSelectedResource(void);

void Menu_LoadSelectionNodeResource(struct State_0801b9ec *state, u32 index)
{
    struct Node_0801b9ec *node = state->head;
    u32 res;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 id = node->base - (u32)&gVal;

        value = node->value;
        Menu_SetRange(id, 0, &value, &res, 1);
        Menu_LoadSelectedResource();
    }
}

/* menu/selection/send_node_count_list.c */
void Menu_SendNodeCountList(u8 *arg0)
{
    u16 data[6];
    u8 *node = *(u8 **)(arg0 + 0x348);
    s32 count = 0;

    while (node != 0) {
        node = *(u8 **)(node + 4);
        count++;
    }
    data[count] = 0xff;
    Menu_Apply(data, 0);
}

/* resource/load_by_mode.c */
/* resource/load_by_mode.c */
extern s32 Resource_FindFreeSlot(void);

s32 Resource_LoadByMode(s32 mode, s32 value)
{
    s32 output;
    s32 result = -1;

    switch (mode) {
    case 1:
    case 6:
        Sys_SetRange(value, 0, &result, &output, 0);
        break;
    case 2:
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return -1;
        Sys_Place(value, 0x1a, result);
        break;
    case 9:
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return -1;
        Ui_BuildPatternToSlot(value, 0, result);
        break;
    case 4:
        Ability_LoadGlyph(value, 1, &result, &output, 0);
        break;
    }
    return result;
}

/* resource/load_by_mode_into_slot.c */

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option)
{
    s32 output;
    s32 original = result;

    if (result == -1) {
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return original;
    }

    switch (mode) {
    case 1:
    case 6:
        Sys_SetRange(value, option, &result, &output, 1);
        break;
    case 2:
        Sys_Place(value, 58, result);
        break;
    case 7:
        Sys_Place(value, 42, result);
        break;
    case 4:
        Ability_LoadGlyph(value, option, (s32)&result, (s32)&output, 1);
        break;
    case 8:
        Ui_BuildPatternToSlot(value, 0, result);
        break;
    case 9:
        load_table_entry(value, 0, (void *)result);
        break;
    }

    return result;
}

/* menu/selection/setup_entry.c */
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

s32 Resource_CopyData(s32 handle, s32 size, void *buffer);
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

extern u8 Value_000000f1;

struct SelectionNode_0801c188 *Menu_RunLoadSelected(void *state);
struct ResourceBuffer_0801c188 *Runtime_AllocateHeapBlock(s32 owner, s32 size);


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
    Menu_ApplySetupEntry(resource, buffer);

    if (transfer->active == 0)
        transfer->handle = Resource_FindFreeSlot();
    transfer->transfer_id =
        Resource_CopyData(transfer->handle, 0x400, buffer);
    transfer->active = 1;
    transfer->no = no;
    transfer->x = 40;
    transfer->y = 40;
    transfer->width = 240;
    Menu_DoSetupEntry(17);
}
