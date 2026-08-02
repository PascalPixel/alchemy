#include "layout_guard.h"
#include "types.h"

enum {
    MODAL_RESOURCE_080A5B94 = 55,
    MODAL_WORKSPACE_SIZE_080A5B94 = 0xa70,
    BUSY_FLAG_OFFSET_080A5B94 = 0xea6,
};

struct UiControl_080a5b94 {
    u8 padding00[4];
    u16 modal_active;
};

struct ModalWorkspace_080a5b94 {
    u8 padding000[0x24];
    void *object_list;
    u8 padding028[0xe4];
    s32 window;
    u8 padding110[0x68];
    u16 selected_kind;
    u8 padding17a[0x8e];
    u8 member_ids[0x11];
    u8 member_count;
    u8 padding21a[0x856];
};

struct BusyOwner_080a5b94 {
    u8 padding000[BUSY_FLAG_OFFSET_080A5B94];
    u8 busy;
};

struct DisplayRecord_080a5b94 {
    u8 padding000[0x17e];
    s16 encoded_selection;
};

struct GlobalTable_080a5b94 {
    struct UiControl_080a5b94 *ui;
    u8 padding04[0x20];
    struct BusyOwner_080a5b94 *busy_owner;
    u8 padding28[0x2c];
    struct DisplayRecord_080a5b94 *display_record;
};

LAYOUT_OFFSET_GUARD(
    ModalWorkspace080a5b94_ObjectList,
    struct ModalWorkspace_080a5b94,
    object_list,
    0x24);
LAYOUT_OFFSET_GUARD(
    ModalWorkspace080a5b94_Window,
    struct ModalWorkspace_080a5b94,
    window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    ModalWorkspace080a5b94_SelectedKind,
    struct ModalWorkspace_080a5b94,
    selected_kind,
    0x178);
LAYOUT_OFFSET_GUARD(
    ModalWorkspace080a5b94_MemberIds,
    struct ModalWorkspace_080a5b94,
    member_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    ModalWorkspace080a5b94_MemberCount,
    struct ModalWorkspace_080a5b94,
    member_count,
    0x219);
LAYOUT_SIZE_GUARD(
    ModalWorkspace080a5b94_Size,
    struct ModalWorkspace_080a5b94,
    MODAL_WORKSPACE_SIZE_080A5B94);
LAYOUT_OFFSET_GUARD(
    BusyOwner080a5b94_Busy,
    struct BusyOwner_080a5b94,
    busy,
    BUSY_FLAG_OFFSET_080A5B94);
LAYOUT_OFFSET_GUARD(
    DisplayRecord080a5b94_EncodedSelection,
    struct DisplayRecord_080a5b94,
    encoded_selection,
    0x17e);
LAYOUT_OFFSET_GUARD(
    GlobalTable080a5b94_BusyOwner,
    struct GlobalTable_080a5b94,
    busy_owner,
    0x24);
LAYOUT_OFFSET_GUARD(
    GlobalTable080a5b94_DisplayRecord,
    struct GlobalTable_080a5b94,
    display_record,
    0x54);

struct ModalWorkspace_080a5b94 *Func_080048b0(s32, s32);
void Func_08015408(s32, s32, s32, s32);
void Func_080030f8(u32);
void Func_080a1090(s32);
s8 Func_08077158(void *);
void Func_080a3354(s32, s32, s32, s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_080a2144(s32);
void Func_08015418(s32);
void Func_080a2474(void);
s32 Func_080a5cc0(s32 *, s32 *, s32 *);
void Func_080a2490(void);
void *Func_08077080(s32);
void Func_08015278(void *);
void Func_080a34c0(void);
void Func_08002dd8(s32);
s32 Func_08015410(s32, s32, s32, s32);
void Func_0808a548(s32);

/*
 * Run the item/effect selector as a modal scene.  Confirmation publishes the
 * selected member and kind into the live display record; cancellation still
 * follows the same teardown path and returns the selector's result code.
 */
s32 Func_080a5b94(void)
{
    struct GlobalTable_080a5b94 *globals =
        (struct GlobalTable_080a5b94 *)0x03001e68;
    struct ModalWorkspace_080a5b94 *workspace =
        Func_080048b0(MODAL_RESOURCE_080A5B94, MODAL_WORKSPACE_SIZE_080A5B94);
    s32 selected_member;
    s32 unused_output;
    s32 selected_kind;
    s32 result;
    s32 transition;

    globals->ui->modal_active = 1;
    Func_08015408(0, 0, 30, 20);
    Func_080030f8(1);
    Func_080a1090(0);

    workspace->member_count = Func_08077158(workspace->member_ids);
    Func_080a3354(0, 3, 0, 7);
    workspace->window = Func_08015010(13, 0, 17, 3, 2);
    Func_080a2144(14);
    Func_08015418(0x06002500);
    Func_080a2474();

    result = Func_080a5cc0(
        &selected_member,
        &unused_output,
        &selected_kind);
    Func_080a2490();

    if (result == 1) {
        Func_08077080(workspace->selected_kind & 0x3fff);
        globals->display_record->encoded_selection =
            (s16)(selected_kind | (selected_member << 10));
    }

    Func_08015278(workspace->object_list);
    globals->busy_owner->busy = 1;
    Func_080a34c0();
    Func_08015408(0, 0, 30, 20);
    Func_08002dd8(MODAL_RESOURCE_080A5B94);

    globals->ui->modal_active = 0;
    Func_080030f8(1);
    transition = Func_08015410(0, 0, 30, 20);
    globals->busy_owner->busy = 0;
    Func_0808a548(transition);

    return result;
}
