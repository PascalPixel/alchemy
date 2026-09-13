#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void UiWork_ClearValueNameTables(void)
{
    s32 no;
    u32 *wtbl;
    u16 *htbl;
    volatile u32 *wp;
    volatile u16 *hp;

    wtbl = (u32 *)((u8 *)*(u32 *)ADDR_03001E8C + RENDER_VALUE_TBL_OFS);
    htbl = (u16 *)((u8 *)*(u32 *)ADDR_03001E8C + RENDER_NAME_TBL_OFS);
    no = 0;

    /* 対応する値と識別子は同じ順序で消去する。 */
    do {
        wp = wtbl + no;
        hp = htbl + no;
        *wp = 0;
        *hp = 0;
        no++;
    } while (no != 8);
}

struct UiNamedValueWork {
    u8 filler0[RENDER_VALUE_TBL_OFS];
    u32 values[8];
    u16 flags[8];
};

extern u8 *Data_03001e8c;

void UiWork_PushValueSlot(u32 value, u32 flag)
{
    struct UiNamedValueWork *work = (struct UiNamedValueWork *)Data_03001e8c;
    u32 no = 0;
    u32 limit = 8;

    do {
        if (work->flags[no] == 0) {
            work->values[no] = value;
            work->flags[no] = flag;
            break;
        }
        no++;
    } while (no != limit);
}

u32 UiRender_LookupNamedValue(u32 value, u32 clear)
{
    u32 index;
    u32 name_offset;
    u32 value_offset;
    u8 *base;
    u16 name;
    u32 result;
    u32 zero;

    base = *(u8 **)ADDR_03001E8C;
    result = 0;
    index = 0;
    zero = index;
    value_offset = RENDER_VALUE_TBL_OFS;
    name_offset = RENDER_NAME_TBL_OFS;
    name = *(u16 *)(name_offset + (u32)base);
    if (name == value) {
        result = *(u32 *)(value_offset + (u32)base);
        if (clear != 0) {
            *(u32 *)(value_offset + (u32)base) = zero;
            *(u16 *)(name_offset + (u32)base) = zero;
        }
    } else {
loop:
        index++;
        value_offset += 4;
        name_offset += 2;
        if (index <= 7) {
            if (*(u16 *)(name_offset + (u32)base) == value) {
                result = *(u32 *)(value_offset + (u32)base);
                if (clear != 0) {
                    *(u32 *)(value_offset + (u32)base) = zero;
                    *(u16 *)(name_offset + (u32)base) = zero;
                }
            } else {
                goto loop;
            }
        }
    }
    return result;
}
