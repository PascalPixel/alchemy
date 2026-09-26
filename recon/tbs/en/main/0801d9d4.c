/* 2026-09-26: complete 412-byte owner; 57 differing halfwords, 33 aligned
   edits (previous draft: 420 bytes, 137 halfwords, 87 edits). Corrected the
   RenderOutput_Create call to five arguments and CreateFrame to five, reused
   WorkspaceWork/RenderInput, and kept pooled message ids in an incremented
   local. The work, flag, rows and table-offset spill now match fp/r9/sl/sp+8.
   Residual: early reload-register choices and scheduling, cursor x/y loads,
   and signed frame-table load (ldrsb versus ldrb plus two shifts). The greg
   dump chooses r1 for the initial rows reload, not the reference's r0.
   Narrowing CreateFrame's first argument to s8 and using a packed signed-byte
   bitfield table both reproduce this same binary; do not repeat those axes.
   An unpacked one-byte struct has a four-byte stride and is not equivalent. */
#include "TYPES.H"
#include "WORKSPACE_OPTIONS.H"

struct WorkspaceMenu {
    struct WorkspaceWork options;
    struct RenderOutput *entries[6];
};

extern u8 Data_03001f54;
extern const s8 Data_080367dc[];
extern u8 Value_00000c23, Value_00000c25, Value_00000c27;

s32 GameFlag_TestFar(s32 flag);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);
void UiText_DrawResource(s32, struct RenderInput *, s32, s32);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32, s32, const void *);
void Func_080b0038(void *, s32, s32);
void *RenderOutput_CreateFrame(s32, s32, struct RenderInput *, s32, s32);

struct RenderInput *Menu_CreateWorkspaceWindows(void)
{
    struct WorkspaceMenu *work;
    s32 rows;
    s32 first;
    s32 has_flag;
    s32 width;
    s32 height;
    struct RenderInput *window;
    s32 y;
    s32 slot;
    s32 msg;

    work = (struct WorkspaceMenu *)Data_03001ea0;
    rows = 3;
    has_flag = GameFlag_TestFar(382);
    first = 0;
    if (has_flag != 0) {
        first = 2;
        rows = 1;
    }
    if (Data_03001f54 != 0)
        rows += 3;
    width = 8 - rows;
    height = rows * 3 + 1;
    if (width + height > 19) {
        width = 1;
        height = 19;
    }
    window = UiWindow_Create(5, width, 20, height, 2);
    if (rows > 1) {
        s32 line = 3;
        s32 n = rows - 1;
        do {
            UiWindow_DrawDividerLine(window, 0, line, 19, line);
            line += 3;
        } while (--n != 0);
    }
    y = 4;
    if (has_flag == 0) {
        msg = (s32)&Value_00000c23;
        UiText_DrawResource(msg, window, 48, 4);
        msg++;
        UiText_DrawResource(msg, window, 48, 28);
        y = 52;
    }
    UiText_DrawResource((s32)&Value_00000c25, window, 48, y);
    y += 24;
    if (Data_03001f54 != 0) {
        msg = (s32)&Value_00000c27;
        UiText_DrawResource(msg, window, 48, y);
        y += 24;
        UiText_DrawResource(msg + 1, window, 48, y);
        y += 24;
        msg += 2;
        UiText_DrawResource(msg, window, 48, y);
    }
    slot = Resource_FindFreeEntry();
    if (slot <= 95) {
        VramBlock_LoadCached(slot, 128, (void *)0x080310a4);
        work->options.cursor.output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        y = window->y * 8 + 16;
        Func_080b0038(&work->options.cursor, window->x * 8, y);
    }
    y = -4;
    if (rows > 0) {
        struct RenderOutput **out = work->entries;
        const s8 *entry = Data_080367dc + first;
        s32 n = rows;
        do {
            *out++ = RenderOutput_CreateFrame(*entry++, 0, window, 12, y);
            y += 24;
        } while (--n != 0);
    }
    return window;
}
