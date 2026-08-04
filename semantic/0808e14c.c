#include "types.h"

typedef struct {
    u32 flags;
    s16 packed;
    s16 parameter;
    u32 unknown08;
} Record0808e14c;

typedef struct {
    u8 unknown00[0x10];
    Record0808e14c *records;
} Runtime0808e14c;

typedef struct {
    u8 unknown00[6];
    u16 coordinate;
} Selection0808e14c;

extern Runtime0808e14c *Data_03001ebc;
extern u32 Data_02000434;

Selection0808e14c *Func_0808ba1c(u32 selection);
s32 Func_0808bd24(void);
s32 Func_0808d428(s32 parameter);
s32 Func_0808ddec(u32 selection);

Record0808e14c *Func_0808e14c(u16 requestedObject)
{
    Record0808e14c *record = Data_03001ebc->records;
    u32 selection = Data_02000434;
    u32 coordinate = Func_0808ba1c(selection)->coordinate;
    u32 requested = requestedObject & 0x1ff;
    s32 selectedObject = Func_0808ddec(selection);
    s32 activeObject = Func_0808bd24();

    while (record->flags != (u32)-1) {
        u16 packed = record->packed;
        s32 upper = record->packed & 0xf000;
        u32 object = packed & 0xff;

        if ((record->flags & 0xf) == 4 &&
            Func_0808d428(record->parameter) != 0 &&
            (((s16)(packed & 0x800) == 0) ||
             (u16)(upper - coordinate + 0x17ff) <= 0x2ffe) &&
            (requested == 0 || record->flags >> 8 == requested)) {
            if ((record->flags & 0x10) != 0) {
                if (object == (u32)selectedObject)
                    return record;
            } else if (object == (u32)activeObject) {
                return record;
            }
        }

        record++;
    }

    return NULL;
}
