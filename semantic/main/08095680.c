#include "types.h"

struct Global_08095680 {
    u8 pad_000[0x1F4];
    u32 selected_object_id;
    u8 pad_1f8[0x3C];
    u16 packed_selection;
    s16 active;
};

struct Placement_08095680 {
    u8 pad_00[2];
    s16 selection;
    u8 pad_04[4];
    s32 x;
    s32 y;
    s32 z;
};

struct Object_08095680 {
    u8 pad_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 field_14;
    u8 pad_18[0x3D];
    u8 mode;
};

extern struct Global_08095680 Data_02000240;

s32 Func_080770c0(s32);
struct Placement_08095680 *Func_0808d394(s32);
struct Object_08095680 *Func_0808ba1c(s32);
void Func_080090f0(struct Object_08095680 *, s32, s32, s32);
void Func_08009080(struct Object_08095680 *, s32);

void Func_08095680(void)
{
    struct Global_08095680 *global;
    u32 *selected_object_id;
    u32 packed;
    s32 category;
    s32 selection;
    s32 index;

    global = &Data_02000240;
    packed = global->packed_selection;
    category = (s16)packed & 0xF000;
    selection = 0xFFF;
    selection &= packed;
    if (Func_080770c0(0x109) != 0 && category == 0) {
        category = selection & 0x800;
        selection &= 0x7FF;
        if ((u32)(selection - 300) <= 80 && global->active > 0) {
            selected_object_id = &global->selected_object_id;
            for (index = 8; index <= 65; index++) {
                struct Placement_08095680 *placement;
                struct Object_08095680 *object;

                placement = Func_0808d394(index);
                if (placement == NULL
                    || placement->selection - 48 != (s32)selection - 300) {
                    continue;
                }

                object = Func_0808ba1c(index);
                if (object == NULL)
                    continue;

                if (category == 0) {
                    object->field_14 = 0;
                    object->mode = 3;
                    Func_080090f0(
                        object, placement->x, placement->y, placement->z);
                } else {
                    struct Object_08095680 *selected;

                    selected = Func_0808ba1c(*selected_object_id);
                    Func_080090f0(
                        object,
                        selected->x,
                        selected->y,
                        selected->z - 0x200000);
                }
                Func_08009080(object, 1);
            }
        }
    }
}
