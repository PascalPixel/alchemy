#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"

extern u8 RomBytes_08029a10[];
extern u8 RomBytes_08029e00[];
extern u8 RomBytes_0802de88[];
extern u8 RomBytes_0802e108[];

/* ui/render/drain_pending.c */
extern void UiWork_Finalize(struct Work *work, s32 release);
void WaitFrames(s32);

extern u8 *gIw;

struct PendingWork {
    u8 padding00[0x16];
    u16 flag;
    s32 value;
};

struct WorkSlot {
    struct PendingWork *work;
    u8 padding04[0x24];
};

struct DirectWork {
    u8 padding00[0x16];
    u16 flag;
    s32 value;
    u8 padding1c[8];
};

void UiWork_DrainPending(void)
{
    u8 *state;
    struct WorkSlot *slot;
    struct DirectWork *direct;
    u32 done;
    struct PendingWork *work;
    struct PendingWork *poll_work;
    s32 index;
    u16 flag;

    state = gIw;
    slot = (struct WorkSlot *)(state + 0x620);
    direct = (struct DirectWork *)(state + 0x500);
    index = 0;
    do {
        work = slot->work;
        if (work != 0 && work->flag != 0)
            UiWork_Finalize(work, 0);
        index++;
        slot++;
    } while (index != 3);

poll:
    done = 1;
    slot = (struct WorkSlot *)(state + 0x620);
    index = 0;
    do {
        poll_work = slot->work;
        if (poll_work != 0) {
            if (poll_work->value == 0) {
                flag = poll_work->flag;
                if (flag == 0)
                    slot->work = (struct PendingWork *)(u32)flag;
                else
                    done = 0;
            } else {
                done = 0;
            }
        }
        index++;
        slot++;
    } while (index != 3);
    index = 0;
    if (!done) {
        WaitFrames(1);
        goto poll;
    }
    goto directTest;
directLoop:
    if (direct->flag != 0)
        UiWork_Finalize(direct, 0);
    direct++;
    index++;
directTest:
    if (index != 8)
        goto directLoop;
}

/* ui/runtime/misc/clear_value_name_tables.c */
/* ui/runtime/misc/clear_value_name_tables.c */
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

/* ui/runtime/misc/push_value_slot.c */
struct State_08019908 {
    u8 filler0[RENDER_VALUE_TBL_OFS];
    u32 values[8];
    u16 flags[8];
};


/* 上限値は式のまま保持し、探索中の評価順を変えない。 */
#define SLOT_COUNT(seed) (((seed) | ~(seed)) + 9)

void UiWork_PushValueSlot(u32 value, u32 flag)
{
    struct State_08019908 *work = (struct State_08019908 *)gIw;
    u32 no = 0;
    u32 limit = SLOT_COUNT(value);

    do {
        if (work->flags[no] == 0) {
            work->values[no] = value;
            work->flags[no] = flag;
            break;
        }
        no++;
    } while (no != limit);
}

/* ui/render/lookup_named_value.c */
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

/* ui/runtime/misc/check_cancel_by_input.c */
s32 UiWork_CheckCancelByInput(void *obj)
{
  int zero;
  s32 flag;
  flag = 0;
  if (((*((u8 *)(((u8 *)(*((void **)ADDR_03001E8C))) + RENDER_BUSY_OFS))) != 0) && (Ui_CheckCancelByInput() == 0))
  {
    flag = 1;
    if (1)
    {
    }
  }
  zero = 0;
;
  if ((*((s32 *)ADDR_03001AE8)) & 0x303)
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

/* ui/runtime/misc/check_cancel_by_mode_input.c */
s32 UiWork_CheckCancelByModeInput(void *obj)
{
  void *p;
  s32 tmp;
  unsigned char zero;
  s32 key;
  s32 flag;
  void *work;
  int unused;
  p = *((void **)ADDR_03001E8C);
  work = p;
  flag = 0;
  if (((*((u8 *)(((u8 *)work) + RENDER_BUSY_OFS))) != 0) && (Ui_CheckByModeInput() == 0))
  {
    flag = 1;
  }
  key = (tmp = *((s32 *)ADDR_03001C94));
  zero = 0;
  if ((*((u8 *)(work + RENDER_MODE_OFS))) != zero)
  {
    key = *((s32 *)ADDR_03001AF8);
  }
  if (0x303 & key)
  {
    flag = 1;
  }
  if (flag != 0)
  {
;
    *((s16 *)(((u8 *)obj) + 0x14)) = zero;
    return 1;
  }
  return 0;
}

/* ui/render/finalize_pending.c */
struct Work {
    u8 padding00[0x14];
    u16 state;
    u16 flags;
    s32 pending_value;
};

struct FinalizeWorkSlot {
    struct Work *work;
    u8 padding04[0x24];
};


void UiWork_FinalizePendingCore(void)
{
    s32 slot_index;
    struct FinalizeWorkSlot *slot;
    struct Work *work;

    slot = (struct FinalizeWorkSlot *)(*(u8 **)ADDR_03001E8C + 0x620);
    slot_index = 0;
    do {
        work = slot->work;
        if (work != NULL && work->pending_value == 0
            && work->flags != 0
            && work->state != 0) {
            UiWork_Finalize(work,
                (s32)(u16)(2 & work->flags));
        }
        slot_index++;
        slot++;
    } while (slot_index != 3);
    WaitFrames(10);
}

/* ui/text_build_render_entries_mode1.c */
