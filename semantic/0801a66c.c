#include "layout_guard.h"
#include "types.h"

enum {
    SELECTION_ROW_COUNT_0801A66C = 5,
    SELECTION_ROW_STRIDE_0801A66C = 0x34,
};

struct SelectionState_0801a66c {
    u8 padding000[0x0a];
    s16 left_marker_active;
    u8 padding00c[0x06];
    s16 left_marker_visible;
    u8 padding014[0x2a];
    s16 right_marker_active;
    u8 padding040[0x06];
    s16 right_marker_visible;
    u8 padding048[0x2a];
    /* Byte rows keep the original two-byte alignment under old agbcc. */
    u8 left_rows[SELECTION_ROW_COUNT_0801A66C][SELECTION_ROW_STRIDE_0801A66C];
    s16 left_tail_state;
    u8 padding178[0x32];
    s16 right_tail_state;
    u8 padding1ac[0x32];
    u8 right_rows[SELECTION_ROW_COUNT_0801A66C][SELECTION_ROW_STRIDE_0801A66C];
    s16 cursor_upload_pending;
    u16 cursor_tile_handle;
    s16 cursor_tile_upload;
    u8 padding2e8[0x12];
    s16 cursor_animation;
    u8 padding2fc[0x09];
    u8 cursor_attribute0_high;
    u8 padding306;
    u8 cursor_attribute1_high;
    u8 padding308;
    u8 cursor_attribute2_high;
    u8 padding30a[0x0c];
    s16 cursor_frame;
    u8 padding318[0x30];
    void *first_node;
    void *last_node;
    void *active_node;
    u8 padding354[0x40];
    s16 entry_count;
    u8 padding396[0x04];
    s16 scroll_position;
    s16 page_offset;
    s16 list_origin_x;
    s16 special_selection;
    u8 padding3a2[0x16];
    s16 special_anchor;
    u8 padding3ba[0x2a];
};

LAYOUT_OFFSET_GUARD(
    SelectionState0801a66c_LeftRows,
    struct SelectionState_0801a66c,
    left_rows,
    0x72);
LAYOUT_OFFSET_GUARD(
    SelectionState0801a66c_RightRows,
    struct SelectionState_0801a66c,
    right_rows,
    0x1de);
LAYOUT_OFFSET_GUARD(
    SelectionState0801a66c_CursorUploadPending,
    struct SelectionState_0801a66c,
    cursor_upload_pending,
    0x2e2);
LAYOUT_OFFSET_GUARD(
    SelectionState0801a66c_CursorAttribute0,
    struct SelectionState_0801a66c,
    cursor_attribute0_high,
    0x305);
LAYOUT_OFFSET_GUARD(
    SelectionState0801a66c_FirstNode,
    struct SelectionState_0801a66c,
    first_node,
    0x348);
LAYOUT_OFFSET_GUARD(
    SelectionState0801a66c_EntryCount,
    struct SelectionState_0801a66c,
    entry_count,
    0x394);
LAYOUT_OFFSET_GUARD(
    SelectionState0801a66c_SpecialAnchor,
    struct SelectionState_0801a66c,
    special_anchor,
    0x3b8);
LAYOUT_SIZE_GUARD(
    SelectionState0801a66c_Size,
    struct SelectionState_0801a66c,
    0x3e4);

struct SelectionState_0801a66c *Func_080048f4(s32, s32);
u16 Func_08004080(void);
s16 Func_08003fa4(s32, s32, const void *);

/*
 * Allocate the shared selector state and put both five-row lists, their
 * markers, linked-list anchors, and cursor sprite into a known idle state.
 * The cursor's tiles are allocated once here; later page builders only have
 * to position and reveal it.
 */
s32 Func_0801a66c(void)
{
    struct SelectionState_0801a66c *state = Func_080048f4(0x12, 0x3e4);
    s32 row;

    state->first_node = 0;
    state->last_node = 0;
    state->active_node = 0;

    state->scroll_position = 0;
    state->page_offset = 0;
    state->list_origin_x = 0x80;
    state->special_selection = 0x20;
    state->entry_count = 0;
    state->special_anchor = 0x3e7;

    for (row = 0; row < SELECTION_ROW_COUNT_0801A66C; row++) {
        *(s16 *)state->left_rows[row] = 0;
        *(s16 *)state->right_rows[row] = 0;
    }

    state->left_tail_state = 0;
    state->right_tail_state = 0;
    state->left_marker_active = 0;
    state->right_marker_active = 0;
    state->left_marker_visible = 0;
    state->right_marker_visible = 0;

    state->cursor_tile_handle = Func_08004080();
    state->cursor_tile_upload = Func_08003fa4(
        state->cursor_tile_handle,
        0x100,
        (const void *)0x080346f8);
    state->cursor_upload_pending = 0;
    state->cursor_animation = 0;
    state->cursor_frame = 0;

    state->cursor_attribute1_high =
        (state->cursor_attribute1_high & ~0x3e) | 0x40;
    state->cursor_attribute0_high =
        ((state->cursor_attribute0_high & ~0x0c) & ~0x10) | 0x20;
    state->cursor_attribute0_high &= ~0x03;
    state->cursor_attribute0_high &= 0x3f;
    state->cursor_attribute2_high &= ~0x0c;

    return state->cursor_attribute2_high;
}
