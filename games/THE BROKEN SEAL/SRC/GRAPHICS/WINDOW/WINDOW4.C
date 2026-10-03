#include "TYPES.H"
#include "IO_REG.H"
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

void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);

extern s32 gKeysHeld;
extern s32 gKeyState;
extern s32 gKeysPressedLatch;
s32 AudioCommand_GetStateByteFar();

void UiWork_DrainPending(void)
{
    struct UiRenderWork *state;
    struct UiChannelSlot *slot;
    struct UiWindow *direct;
    u32 done;
    struct UiWindow *work;
    struct UiWindow *poll_work;
    s32 index;
    u16 flag;

    state = (struct UiRenderWork *)gWindowWork[0];
    slot = state->channels;
    direct = state->windows;
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
    slot = state->channels;
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
void UiWork_AdvanceChannelTransition(struct UiChannelSlot *channel)
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
    struct UiRenderWork *work;

    work = (struct UiRenderWork *)gWindowWork[0];
    no = 0;

    /* 対応する値と識別子は同じ順序で消去する。 */
    do {
        work->values[no] = 0;
        work->names[no] = 0;
        no++;
    } while (no != 8);
}

void UiWork_PushValueSlot(u32 value, u32 flag)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 no = 0;
    u32 limit = 8;

    do {
        if (work->names[no] == 0) {
            work->values[no] = value;
            work->names[no] = flag;
            break;
        }
        no++;
    } while (no != limit);
}

u32 UiRender_LookupNamedValue(u32 name, u32 clear)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 index;
    u32 value = 0;

    for (index = 0; index < 8; index++) {
        if (work->names[index] == name) {
            value = work->values[index];
            if (clear != 0) {
                work->values[index] = 0;
                work->names[index] = 0;
            }
            break;
        }
    }
    return value;
}

s32 UiWork_CheckCancelByInput(struct UiChannelSlot *channel)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 cancel = 0;
    s32 zero;

    if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
        cancel = 1;
    zero = 0;
    if (gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
        cancel = 1;
    if (cancel != zero) {
        channel->countdown = zero;
        return 1;
    }
    return zero;
}

s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *channel)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 key;
    s32 cancel = 0;
    u8 zero;

    if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
        cancel = 1;
    key = gKeyState;
    zero = 0;
    if (work->mode != zero)
        key = gKeysPressedLatch;
    if (key & 0x303)
        cancel = 1;
    if (cancel != 0) {
        channel->countdown = zero;
        return 1;
    }
    return 0;
}

void UiWork_FinalizePendingCore(void)
{
    s32 slot_index;
    struct UiChannelSlot *slot;
    struct UiWindow *work;

    slot = ((struct UiRenderWork *)gWindowWork[0])->channels;
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
