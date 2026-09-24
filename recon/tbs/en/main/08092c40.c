/* DRAFT: 832 of 836 bytes, same control flow. Remaining: register
 * allocation. The ROM gives the message window handle sl, the side window
 * r9, the flags fp, the message r8 and the object id r6, and keeps the
 * screen column x in call-clobbered r4 saved at sp+4 around calls; the
 * on-screen test is mvns/negs/orrs, and 0x1cc and 0x19e are reached from
 * 0x1f4 by subs. This compile puts x in r7 and the message in sl. */
#include "TYPES.H"

/* ui/text/open_message_at_object.c */
struct MessageWork {
    u8 padding000[0x19e];
    s16 view_mode;
    u8 padding1a0[0x1cc - 0x1a0];
    s32 busy;
    u8 padding1d0[8];
    s16 message;
    u8 padding1da[0x1f4 - 0x1da];
    s32 speaker;
    s32 message_window;
    s32 side_window;
};

struct WorkPointers {
    u8 *window;
    u8 padding04[0x2c];
    struct MessageWork *message;
};

struct PlayerTable {
    u8 padding000[0x1f4];
    s32 leader;
};

extern struct WorkPointers gWindowWork;
extern struct PlayerTable Data_02000240;

s32 ObjectTable_ReadActiveValue(s32 key);
u8 *ObjectTable_Get(s32 id);
s32 Render_ProjectPoint(const s32 *point, s32 *screen);
s32 Object_GetScreenPosition(s32 id, s32 *screen);
void Func_08015110(s32 message, s32 *x, s32 *y, s32 *width, s32 *height);
void Func_08015108(s32 message, s32 *x, s32 *y, s32 *width, s32 *height);
s32 Func_080151e0(s32 speaker);
void WaitFrames(s32 frames);
s32 BattleFx_GetResourceId(s32 speaker);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 style);
s32 UiWindow_CreateWithSideObjectFar(s32 speaker, s32 mode, s32 x, s32 y);
s32 UiWork_IsCompleteFar(void);

s32 UiText_OpenMessageAtObject(u32 arg)
{
    u8 *win;
    struct MessageWork *work;
    s32 speaker;
    s32 extra;
    s32 tail;
    s32 margin;
    s32 height;
    s32 width;
    s32 top;
    s32 left;
    s32 pos[3];
    s32 handle;
    s32 side;
    s32 flags;
    s32 id;
    s32 message;
    u8 *object;
    s32 x;
    s32 y;
    s32 visible;
    s32 column;
    s32 face;
    s32 lines;

    win = gWindowWork.window;
    work = gWindowWork.message;
    handle = 0;
    side = 0;
    speaker = ObjectTable_ReadActiveValue(arg);
    flags = arg & 0xf000;
    extra = 0;
    tail = 0;
    margin = 4;
    message = work->message;
    id = arg & 0xfff;
    x = 0;
    object = ObjectTable_Get(id);
    work->speaker = id;
    y = 0;
    visible = 0;
    if (work->busy == 0) {
        if (object != 0) {
            if (work->view_mode == 3) {
                Render_ProjectPoint((s32 *)(object + 8), pos);
                x = pos[0] >> 3;
                y = (pos[1] >> 3) - 2;
                visible = 1;
            } else {
                visible = Object_GetScreenPosition(id, pos) != -1;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        } else if (id <= 7) {
            speaker = id;
            object = ObjectTable_Get(Data_02000240.leader);
            if (work->view_mode == 3) {
                Render_ProjectPoint((s32 *)(object + 8), pos);
                x = pos[0] >> 3;
                y = pos[1] >> 3;
                visible = 1;
            } else {
                visible = Object_GetScreenPosition(Data_02000240.leader, pos) != -1;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        }
        if (!visible) {
            left = 15;
            top = 10;
        } else {
            left = 0;
            top = 0;
            Func_08015110(message, &left, &top, &width, &height);
            left = x - width / 2;
            if (flags & 0x4000)
                top = y - height - 1;
            else if (!(flags & 0x8000) && y <= 8)
                top = y - height - 1;
            else
                top = y + 4;
        }
        if (win[0xea4] != 0)
            margin = 5;
        column = x;
        if (flags & 0x1000) {
            column = x - margin - 2;
            if (column < 0)
                column = 0;
        } else if (flags & 0x2000) {
            column += 2;
            if (column + margin > 29)
                column = 29 - margin;
        } else if (column <= 15) {
            column = column - margin - 2;
            if (column < 0)
                column = x + 2;
        } else {
            column += 2;
            if (column + margin > 29)
                column = x - margin - 2;
        }
        face = Func_080151e0(speaker);
        handle = -1;
        if (face != -1) {
            Func_08015110(message, &left, &top, &width, &height);
            tail = top - 5;
            message = handle;
            if (top <= y)
                tail = top + height;
            if (tail < 0)
                tail = top + height;
            else if (tail + 5 > 19)
                tail = top - 5;
            if (top < tail) {
                lines = height;
                Func_08015108(-1, &left, &top, &width, &height);
                extra = lines - height + 1;
                message = -1;
            }
        } else if (top < y) {
            lines = height;
            Func_08015108(message, &left, &top, &width, &height);
            extra = lines - height + 1;
            message = face;
        }
        if (column < 0)
            column = 0;
        else if (column + margin > 29)
            column = 29 - margin;
        if (win[0xea4] != 0) {
            WaitFrames(8);
            if (extra != 0)
                handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
            else
                handle = UiText_OpenMessageWindowFar(message, left, top, 2);
        } else {
            face = BattleFx_GetResourceId(speaker);
            if (extra != 0)
                handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1, (face << 16) | 17);
            else
                handle = UiText_OpenMessageWindowFar(message, left, top, (face << 16) | 1);
        }
        side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
        while (!UiWork_IsCompleteFar())
            WaitFrames(1);
    }
    work->message_window = handle;
    work->side_window = side;
    work->message++;
    return handle;
}
