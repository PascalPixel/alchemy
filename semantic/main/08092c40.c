#include "types.h"

struct PlacementRoot_08092c40 {
    u8 unknown_000[0xea4];
    u8 extended_layout;
};

struct PlacementRuntime_08092c40 {
    u8 unknown_000[0x19e];
    s16 mode;
    u8 unknown_1a0[0x2c];
    s32 busy;
    u8 unknown_1d0[8];
    s16 selector;
    u8 unknown_1da[0x1a];
    u32 selected_object;
    s32 dialog_handle;
    s32 secondary_handle;
};

struct Position_08092c40 {
    s32 x;
    s32 y;
    s32 z;
};

extern struct PlacementRoot_08092c40 *Data_03001e8c;
extern struct PlacementRuntime_08092c40 *Data_03001ebc;

s32 Func_08092ba8(s32 key);
void *Func_0808ba1c(u32 id);
void Func_08005268(void *source, struct Position_08092c40 *position);
s32 Func_08094154(u32 id, struct Position_08092c40 *position);
void Func_08015110(
    s32 selector,
    s32 *box_x,
    s32 *box_y,
    s32 *extent_x,
    s32 *extent_y);
s32 Func_080151e0(u32 value);
void Func_08015108(
    s32 selector,
    s32 *box_x,
    s32 *box_y,
    s32 *extent_x,
    s32 *extent_y);
void Func_080030f8(u32 frames);
s32 Func_080915ac(u32 value);
s32 Func_08015038(s32 selector, s32 x, s32 y, s32 style);
s32 Func_080150f8(s32 value, s32 mode, s32 x, s32 y);
s32 Func_08015048(void);

/*
 * Place and create the paired object dialog and resource window associated
 * with a packed field key.  The high nibble selects placement policy and the
 * low twelve bits select the field object.
 */
s32 Func_08092c40(s32 key)
{
    struct PlacementRoot_08092c40 *root = Data_03001e8c;
    struct PlacementRuntime_08092c40 *runtime = Data_03001ebc;
    s32 dialog = 0;
    s32 secondary = 0;
    s32 value = Func_08092ba8(key);
    u32 flags = key & 0xf000;
    u32 object_id = key & 0x0fff;
    s32 selector = runtime->selector;
    void *object;

    runtime->selected_object = object_id;
    object = Func_0808ba1c(object_id);

    if (runtime->busy == 0) {
        struct Position_08092c40 position;
        s32 anchor_x = 0;
        s32 anchor_y = 0;
        s32 position_valid = 0;
        s32 box_x = 0;
        s32 box_y = 0;
        s32 extent_x = 0;
        s32 extent_y = 0;
        s32 width = 4;
        s32 window_x;
        s32 draw_y = 0;
        s32 adjustment = 0;
        s32 render_selector = selector;
        s32 mapped;

        if (object != 0) {
            if (runtime->mode == 3) {
                Func_08005268(
                    (u8 *)object + 8,
                    &position);
                anchor_x = position.x >> 3;
                anchor_y = (position.y >> 3) - 2;
                position_valid = 1;
            } else {
                position_valid =
                    Func_08094154(object_id, &position) != -1;
                anchor_x = position.x >> 3;
                anchor_y = position.y >> 3;
            }
        } else if ((s32)object_id <= 7) {
            u32 fallback_id = *(u32 *)0x02000434;

            value = object_id;
            object = Func_0808ba1c(fallback_id);
            if (runtime->mode == 3) {
                Func_08005268(
                    (u8 *)object + 8,
                    &position);
                anchor_x = position.x >> 3;
                anchor_y = position.y >> 3;
                position_valid = 1;
            } else {
                position_valid =
                    Func_08094154(fallback_id, &position) != -1;
                anchor_x = position.x >> 3;
                anchor_y = position.y >> 3;
            }
        }

        if (position_valid == 0) {
            box_x = 15;
            box_y = 10;
        } else {
            s32 half_extent;

            Func_08015110(
                selector,
                &box_x,
                &box_y,
                &extent_x,
                &extent_y);
            half_extent =
                (s32)((u32)extent_x +
                      ((u32)extent_x >> 31)) >>
                1;
            box_x = anchor_x - half_extent;
            if ((flags & 0x4000) != 0 ||
                ((flags & 0x8000) == 0 && anchor_y <= 8))
                box_y = anchor_y - extent_y - 1;
            else
                box_y = anchor_y + 4;
        }

        if (root->extended_layout != 0)
            width = 5;

        if ((flags & 0x1000) != 0) {
            window_x = anchor_x - width - 2;
            if (window_x < 0)
                window_x = 0;
        } else if ((flags & 0x2000) != 0) {
            window_x = anchor_x + 2;
            if (window_x + width > 29)
                window_x = 29 - width;
        } else if (anchor_x <= 15) {
            window_x = anchor_x - width - 2;
            if (window_x < 0)
                window_x = anchor_x + 2;
        } else {
            window_x = anchor_x + 2;
            if (window_x + width > 29)
                window_x = anchor_x - width - 2;
        }

        mapped = Func_080151e0(value);
        if (mapped != -1) {
            s32 previous_extent_y;

            Func_08015110(
                selector,
                &box_x,
                &box_y,
                &extent_x,
                &extent_y);
            render_selector = -1;
            draw_y = box_y - 5;
            if (box_y <= anchor_y)
                draw_y = box_y + extent_y;
            if (draw_y < 0)
                draw_y = box_y + extent_y;
            else if (draw_y + 5 > 19)
                draw_y = box_y - 5;

            if (box_y < draw_y) {
                previous_extent_y = extent_y;
                Func_08015108(
                    -1,
                    &box_x,
                    &box_y,
                    &extent_x,
                    &extent_y);
                adjustment =
                    previous_extent_y - extent_y + 1;
            }
        } else if (box_y < anchor_y) {
            s32 previous_extent_y = extent_y;

            Func_08015108(
                selector,
                &box_x,
                &box_y,
                &extent_x,
                &extent_y);
            adjustment = previous_extent_y - extent_y + 1;
            render_selector = -1;
        }

        if (window_x < 0)
            window_x = 0;
        else if (window_x + width > 29)
            window_x = 29 - width;

        if (root->extended_layout != 0) {
            Func_080030f8(8);
            dialog = Func_08015038(
                render_selector,
                box_x,
                adjustment != 0
                    ? box_y + adjustment - 1
                    : box_y,
                adjustment != 0 ? 18 : 2);
        } else {
            s32 theme = Func_080915ac(value);
            s32 style =
                ((u32)theme << 16) |
                (adjustment != 0 ? 17 : 1);

            dialog = Func_08015038(
                render_selector,
                box_x,
                adjustment != 0
                    ? box_y + adjustment - 1
                    : box_y,
                style);
        }

        secondary =
            Func_080150f8(value, 0, window_x, draw_y);
        while (Func_08015048() == 0)
            Func_080030f8(1);
    }

    runtime->dialog_handle = dialog;
    runtime->secondary_handle = secondary;
    runtime->selector = (u16)runtime->selector + 1;
    return dialog;
}
