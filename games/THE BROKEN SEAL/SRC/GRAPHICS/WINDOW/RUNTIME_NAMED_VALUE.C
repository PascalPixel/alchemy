#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

struct UiNamedValueWork {
    u8 filler0[RENDER_VALUE_TBL_OFS];
    u32 values[8];
    u16 flags[8];
};

extern u8 *gWindowWork;

void UiWork_ClearValueNameTables(void)
{
    s32 no;
    struct UiNamedValueWork *work;

    work = (struct UiNamedValueWork *)gWindowWork;
    no = 0;

    /* 対応する値と識別子は同じ順序で消去する。 */
    do {
        work->values[no] = 0;
        work->flags[no] = 0;
        no++;
    } while (no != 8);
}

void UiWork_PushValueSlot(u32 value, u32 flag)
{
    struct UiNamedValueWork *work = (struct UiNamedValueWork *)gWindowWork;
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
