/* Near miss: score 180. ☀️'s, with the remembered pair at gPartyState 0x1e8
   and the held buttons from gInput. ⚓️ loads gPartyState's address before
   building the field offset (ldr r2 before movs r1, #244), where agscc
   builds the offset first; the same order shows in 080ae16c and 0804d4f8.
   45 s of permuting found nothing. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "TLA_EDITION.H"
#include "PARTY_STATE.H"

/* The controller state the engine refreshes each frame. */
struct InputState {
    u32 held;
    u32 pressed;
};

extern volatile struct InputState gInput;

struct Work;

struct TextObject {
    u8 storage[12];
};

void Event_SetPairWork1c0Far(s16 primary, s16 secondary);
void UiTextResource_Release(s32 resource);
void UiTextResource_Initialize(struct TextObject *object, s32 *resource);
void UiTextResource_SetPosition(struct TextObject *object, s32 x, s32 y);
struct Work *UiWindow_Create(s32 kind, s32 x, s32 y, s32 width, s32 layer);
void UiWork_Finalize(struct Work *work, s32 release);
void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second);
s32 Menu_HandleSelectionRowInput(struct Work *work, s16 value, s16 *sub, s16 *mode);

/* ☀️'s debug selection, starting from the pair ⚓️ keeps in gPartyState. */
s16 Menu_RunSelection(void)
{
    s32 resource;
    s16 mode;
    s16 secondary;
    struct TextObject object;
    s16 primary;
    struct Work *work;
    s16 result;

    work = 0;
    mode = 0;
    primary = gPartyState.primary;
    secondary = gPartyState.secondary;
    work = UiWindow_Create(0, 7, 30, 5, 2);
    Menu_DrawSelectionRow(work, primary, &secondary);
    UiTextResource_Initialize(&object, &resource);

    while (gInput.held != 0)
        WaitFrames(1);

    for (;;) {
        result = (s16)Menu_HandleSelectionRowInput(work, primary, &secondary, &mode);
        if (result == -1) {
            UiTextResource_Release(resource);
            UiWork_Finalize(work, 2);
            Event_SetPairWork1c0Far(primary, secondary);
            return result;
        }
        if (result == -2) {
            UiTextResource_Release(resource);
            UiWork_Finalize(work, 2);
            return result;
        }

        UiTextResource_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
        primary = result;
        WaitFrames(1);
    }
}
