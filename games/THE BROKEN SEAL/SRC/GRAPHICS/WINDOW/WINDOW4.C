#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern u8 Data_03001e8c[];

struct UiNamedValueWork {
    u8 filler0[RENDER_VALUE_TBL_OFS];
    u32 values[8];
    u16 flags[8];
};

extern u8 *gWindowWork;

extern u8 Data_03001ae8[];
extern u8 Data_03001c94[];
extern u8 gKeysPressedLatch[];
s32 AudioCommand_GetStateByteFar();

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

    base = *(u8 **)((u32)&Data_03001e8c);
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

s32 UiWork_CheckCancelByInput(void *obj)
{
  int zero;
  s32 flag;
  flag = 0;
  if (((*((u8 *)(((u8 *)(*((void **)((u32)&Data_03001e8c)))) + RENDER_BUSY_OFS))) != 0) && (AudioCommand_GetStateByteFar() == 0))
  {
    flag = 1;
  }
  zero = 0;
  if ((*((s32 *)((u32)&Data_03001ae8))) & 0x303)
  {
    flag = 1;
  }
  if (flag != zero)
  {
    *((s16 *)(((u8 *)obj) + 0x14)) = zero;
    return 1;
  }
  return zero;
}

s32 UiWork_CheckCancelByModeInput(void *obj)
{
  void *p;
  s32 tmp;
  unsigned char zero;
  s32 key;
  s32 flag;
  void *work;
  p = *((void **)((u32)&Data_03001e8c));
  work = p;
  flag = 0;
  if (((*((u8 *)(((u8 *)work) + RENDER_BUSY_OFS))) != 0) && (AudioCommand_GetStateByteFar() == 0))
  {
    flag = 1;
  }
  key = (tmp = *((s32 *)((u32)&Data_03001c94)));
  zero = 0;
  if ((*((u8 *)(work + RENDER_MODE_OFS))) != zero)
  {
    key = *((s32 *)((u32)&gKeysPressedLatch));
  }
  if (0x303 & key)
  {
    flag = 1;
  }
  if (flag != 0)
  {
    *((s16 *)(((u8 *)obj) + 0x14)) = zero;
    return 1;
  }
  return 0;
}
