#include "types.h"

struct ResourceRecord_0808e4b4 {
    u32 flags;
    s16 packed;
    s16 parameter;
    u32 unknown_08;
};

struct Runtime_0808e4b4 {
    u8 unknown_00[0x10];
    struct ResourceRecord_0808e4b4 *records;
};

struct Selection_0808e4b4 {
    u8 unknown_00[6];
    u16 coordinate;
};

struct ActionInfo_0808e4b4 {
    u8 unknown_00[12];
    u8 type;
};

struct SharedState_0808e4b4 {
    u8 unknown_000[0x1F4];
    u32 selected_object;
};

extern struct Runtime_0808e4b4 *Data_03001ebc;
extern struct SharedState_0808e4b4 Data_02000240;

struct ActionInfo_0808e4b4 *Func_08077080(s32);
struct Selection_0808e4b4 *Func_0808ba1c(u32);
u32 Func_0808bd24(void);
s32 Func_0808d428(s32);
u32 Func_0808df1c(u32, s32);

/*
 * Select a class-5 resource record for an action type and object policy.
 * Requested kind 0x70000005 accepts any matching descriptor family.
 */
struct ResourceRecord_0808e4b4 *Func_0808e4b4(
    u32 requested_kind,
    u16 requested_type,
    s32 *selected_object_out)
{
    struct ResourceRecord_0808e4b4 *record =
        Data_03001ebc->records;
    u32 coordinate =
        Func_0808ba1c(Data_02000240.selected_object)->coordinate;
    u32 selected_object;
    u32 active_object;
    s32 wildcard = requested_kind == 0x70000005;

    selected_object =
        Func_0808df1c(Data_02000240.selected_object, requested_type);
    *selected_object_out = (s32)selected_object;
    active_object = Func_0808bd24();

    while (record->flags != (u32)-1) {
        u16 packed = (u16)record->packed;
        s32 upper = record->packed & 0xF000;
        u32 object = packed & 0xFF;

        if ((record->flags & 0xF) == 5 &&
            Func_0808d428(record->parameter) != 0 &&
            (((packed & 0x0800) == 0) ||
             (u16)(upper - coordinate + 0x17FF) <= 0x2FFE) &&
            Func_08077080((record->flags >> 8) & 0xFF)->type ==
                requested_type &&
            (wildcard ||
             (record->flags & 0x7000000F) == requested_kind)) {
            if ((record->flags & 0x80) != 0) {
                return record;
            }
            if ((record->flags & 0x10) != 0) {
                if (object == selected_object) {
                    return record;
                }
            } else if (object == active_object) {
                return record;
            }
        }

        record++;
    }

    return 0;
}
