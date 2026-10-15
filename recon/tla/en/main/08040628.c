/* Near miss, differ 6 of 161 (three reordered pairs), 2026-10-15: The
 * Broken Seal's Menu_CreateWorkspaceWindows; this game has three rows (one
 * with flag 0x17e), three more in debug mode, no cursor object (the fixed
 * block B output at 0x5a4) and its icons at 0x610. The three messages
 * (0x1154, 0x1156 and 0x1158 in English) are pool symbols the catalogs must
 * name before adoption (MsgTestA to MsgTestC here). Remaining: when only the
 * settings show the native stores first (2) to its stack slot before it
 * builds rows (1), and it reloads first before it starts the icon offset;
 * written first = 2 before rows = 1, with a barrier between, the order is
 * right but the two constants trade r2 and r3.
 */
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "render_input.h"

struct WorkspaceScreen {
    u8 unk000[0x5a4];
    struct RenderOutput *cursor;
    u8 unk5a8[0x68];
    struct RenderOutput *icons[6];
};

extern u8 MsgTestA[];
extern u8 MsgTestB[];
extern u8 MsgTestC[];
extern const s8 Data_0805ea8f[];

s32 GameFlag_Test(s32 flag);
struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_DrawDividerLine(struct RenderInput *window, s32 x0, s32 y0, s32 x1, s32 y1);
void UiText_DrawResource(s32 message, struct RenderInput *window, s32 x, s32 y);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
struct RenderOutput *RenderResource_CreateFrame(s32 frame, s32 flag, struct RenderInput *window, s32 x, s32 y);

struct RenderInput *Func_08040628(void)
{
    struct WorkspaceScreen *work;
    s32 rows;
    s32 first;
    s32 settings_only;
    s32 top;
    s32 height;
    struct RenderInput *win;
    s32 y;
    s32 msg;
    s32 i;

    rows = 3;
    work = Ram_HeapSlots->unkd0[0];
    settings_only = GameFlag_Test(0x17e);
    first = 0;
    if (settings_only != 0) {
        rows = 1;
        first = 2;
    }
    if (gDebugMode != 0)
        rows += 3;
    top = 8 - rows;
    height = rows * 3 + 1;
    if (top + height > 19) {
        top = 1;
        height = 19;
    }
    win = UiWindow_Create(5, top, 20, height, 2);
    for (i = 1; i < rows; i++)
        UiWindow_DrawDividerLine(win, 0, i * 3, 19, i * 3);
    y = 4;
    if (settings_only == 0) {
        msg = (s32)MsgTestA;
        UiText_DrawResource(msg, win, 48, 4);
        msg++;
        UiText_DrawResource(msg, win, 48, 28);
        y = 52;
    }
    UiText_DrawResource((s32)MsgTestB, win, 48, y);
    y += 24;
    if (gDebugMode != 0) {
        msg = (s32)MsgTestC;
        UiText_DrawResource(msg, win, 48, y);
        y += 24;
        UiText_DrawResource(msg + 1, win, 48, y);
        y += 24;
        msg += 2;
        UiText_DrawResource(msg, win, 48, y);
    }
    work->cursor = RenderOutput_Create(Resource_LoadFixedBlockBIntoFreeSlot(), 0x40000000, win, 0, 0);
    y = -4;
    for (i = 0; i < rows; i++) {
        work->icons[i] = RenderResource_CreateFrame(Data_0805ea8f[first + i], 0, win, 12, y);
        y += 24;
    }
    return win;
}
