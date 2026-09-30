#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

void WaitFrames(s32);
extern u8 RomBytes_08029a10[];
extern u8 RomBytes_08029e00[];
extern u8 RomBytes_0802de88[];
extern u8 RomBytes_0802e108[];

/* ui/render/drain_pending.c */
extern void UiWork_Finalize(struct Work *work, s32 release);
extern u8 *gWindowWork;

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

struct ChannelWork {
    u8 reserved_00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 reserved_10;
    u16 transition;
};

struct RenderChannel {
    struct ChannelWork *work;
    u16 field_04;
    u16 field_06;
    u16 values[4];
    u16 field_10;
    u16 field_12;
    u16 countdown;
};

void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);

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

    state = gWindowWork;
    slot = (struct WorkSlot *)(state + RENDER_CHANNEL_OFS);
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
    slot = (struct WorkSlot *)(state + RENDER_CHANNEL_OFS);
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
    if (index != WINDOW_COUNT)
        goto directLoop;
}

/* Runs transition 4 of a render channel's work: each call draws a border one
 * tile outside the window and counts down; when the count reaches zero it
 * ends the transition, erases the outer border and redraws the window's own
 * border. */
void UiWork_AdvanceChannelTransition(struct RenderChannel *channel)
{
    struct ChannelWork *work = channel->work;
    s32 transition = work->transition;
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

    channel->work->transition = 0;
    UiWindow_EraseBorderRect(x - 1, y - 1, width + 2, height + 2);
    UiWindow_DrawFrame(x, y, width, height);
}
