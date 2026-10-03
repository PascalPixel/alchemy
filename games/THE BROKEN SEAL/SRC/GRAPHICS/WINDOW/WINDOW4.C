#include "TYPES.H"
#include "WINDOW.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "SYSTEM.H"

void WaitFrames(s32);
extern u8 RomBytes_08029a10[];
extern u8 RomBytes_08029e00[];
extern u8 RomBytes_0802de88[];
extern u8 RomBytes_0802e108[];

/* ui/render/drain_pending.c */


struct WorkSlot {
    struct UiWindow *work;
    u8 padding04[0x24];
};



struct RenderChannel {
    struct UiWindow *work;
    u16 field_04;
    u16 field_06;
    u16 values[4];
    u16 field_10;
    u16 field_12;
    u16 countdown;
};

void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);

extern u8 Data_03001e8c[];

struct UiNamedValueWork {
    u8 filler0[RENDER_VALUE_TBL_OFS];
    u32 values[8];
    u16 flags[8];
};

extern u8 Data_03001ae8[];
extern u8 Data_03001c94[];
extern u8 gKeysPressedLatch[];
s32 AudioCommand_GetStateByteFar();


struct FinalizeWorkSlot {
    struct UiWindow *work;
    u8 padding04[0x24];
};

void UiWork_DrainPending(void)
{
    u8 *state;
    struct WorkSlot *slot;
    struct UiWindow *direct;
    u32 done;
    struct UiWindow *work;
    struct UiWindow *poll_work;
    s32 index;
    u16 flag;

    state = gWindowWork[0];
    slot = (struct WorkSlot *)(state + RENDER_CHANNEL_OFS);
    direct = (struct UiWindow *)(state + 0x500);
    index = 0;
    do {
        work = slot->work;
        if (work != 0 && work->flags != 0)
            UiWork_Finalize(work, 0);
        index++;
        slot++;
    } while (index != 3);

poll:
    done = 1;
    slot = (struct WorkSlot *)(state + RENDER_CHANNEL_OFS);
    index = 0;
    do {
        poll_work = slot->work;
        if (poll_work != 0) {
            if ((*(s32 *)&poll_work->frame) == 0) {
                flag = poll_work->flags;
                if (flag == 0)
                    slot->work = (struct UiWindow *)(u32)flag;
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
    if (direct->flags != 0)
        UiWork_Finalize(direct, 0);
    direct++;
    index++;
directTest:
    if (index != WINDOW_COUNT)
        goto directLoop;
}

/* Runs transition 4 of a render channel's work: each call draws a border one
 * tile outside the window and counts down; when the count reaches zero it
 * ends the transition, erases the outer border and redraws the window's own
 * border. */
void UiWork_AdvanceChannelTransition(struct RenderChannel *channel)
{
    struct UiWindow *work = channel->work;
    s32 transition = work->unknown_12;
    s32 x = work->x;
    s32 y = work->y;
    s32 width = work->width;
    s32 height = work->height;

    if (transition != 4)
        return;

    UiWindow_DrawFrame(x - 1, y - 1, width + 2, height + 2);
    channel->countdown--;
    if (channel->countdown != 0)
        return;

    channel->work->unknown_12 = 0;
    UiWindow_EraseBorderRect(x - 1, y - 1, width + 2, height + 2);
    UiWindow_DrawFrame(x, y, width, height);
}

void UiWork_ClearValueNameTables(void)
{
    s32 no;
    struct UiNamedValueWork *work;

    work = (struct UiNamedValueWork *)gWindowWork[0];
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
    struct UiNamedValueWork *work = (struct UiNamedValueWork *)gWindowWork[0];
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

    base = gWindowWork[0];
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

void UiWork_FinalizePendingCore(void)
{
    s32 slot_index;
    struct FinalizeWorkSlot *slot;
    struct UiWindow *work;

    slot = (struct FinalizeWorkSlot *)(gWindowWork[0] + RENDER_CHANNEL_OFS);
    slot_index = 0;
    do {
        work = slot->work;
        if (work != NULL && (*(s32 *)&work->frame) == 0
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
