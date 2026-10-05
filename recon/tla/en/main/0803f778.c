/*
 * Draft: Menu_RunTopSelection, ☀️'s MENU13.C body over ⚓️'s event work
 * (trigger tile at 0x162) with the 0x080f8000 block's far-call entries 0 and
 * 2 for cases 3 and 4. Exact (differ 0) in English, and with
 * UiWindow_OpenMode1AndWaitFrame (0803f800, ☀️'s source unchanged) after it
 * it also builds the Japanese, German and French editions byte for byte.
 * Spanish and Italian differ by one call: case 0 there branches to the
 * far-call stub at 0x080d12b0 (Spanish) / the same slot (Italian), the
 * scaffold entry just before TABLE_26.o, whose target has the bytes of
 * English Func_080cc54c; TABLE_26.S names the stub eight bytes later
 * Object_GetTriggerTileAheadOfCurrentFar in those editions, and the Spanish
 * scaffold labels 0x080d691c (English Func_080cd91c's bytes) Func_080cc54c.
 * The two localized tables look shifted by two entries; once TABLE_26 is
 * relabelled for them this and 0803f800 join MENU13.C after
 * Resource_ResetPendingTransfer.
 * Links as recon/tla/raw/0803f778.s.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"

void UiWindow_OpenMode1AndWaitFrame(void);
s32 Menu_SelectTopEntry(s32);
s32 UiWork_CloseAndRelease(void);
s32 ItemMenu_Open(void);
s32 Object_GetTriggerTileAheadOfCurrentFar(void);
s32 Menu_OpenConfirmPromptFar(void);
/* Far-call entries 0 and 2 of the 0x080f8000 code block. */
s32 Func_080f8000(void);
s32 Func_080f8010(void);

s32 Menu_RunTopSelection(void)
{
    s32 ret;
    s32 sel;
    u8 *state;

    state = Ram_HeapSlots->event_work;
    sel = 0;

loop:
    UiWindow_OpenMode1AndWaitFrame();
    sel = Menu_SelectTopEntry(sel);
    ret = UiWork_CloseAndRelease();

    switch (sel) {
    case 0:
        ret = Object_GetTriggerTileAheadOfCurrentFar();
        if (ret == 0)
            ret = 0xff;
        *(u16 *)(state + 0x162) = ret;
        break;
    case 1:
        ret = Menu_OpenConfirmPromptFar();
        if (ret == -1)
            goto loop;
        break;
    case 2:
        ret = ItemMenu_Open();
        if (ret != 0)
            goto loop;
        break;
    case 3:
        ret = Func_080f8000();
        if (ret == -1)
            goto loop;
        break;
    case 4:
        ret = Func_080f8010();
        if (ret == -1)
            goto loop;
        break;
    default:
        break;
    }

    return ret;
}
