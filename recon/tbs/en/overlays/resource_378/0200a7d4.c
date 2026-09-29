/* NONMATCHING: resource_378 at 0x0200a7d4 (312 bytes with its five pool
 * words), ShindenHeya_ChooseRestartOption, between SPAWN_EFFECT.C and
 * RUN_PAIRED.C in FIELD/COMMON/SHINDEN_HEYA, stays listing.
 *
 * Remaining differences, measured 2026-09-29 with the stock agscc: 324
 * bytes against 312, code 296 against 292 and seven pool words against
 * five.
 *
 * 1. Message numbers. The reference loads message 0x116e (the restart
 *    question) once from its pool into r5 and draws the two answers as
 *    r5 + 1 and r5 + 2, as a link-time message symbol does. The plain
 *    number below folds 0x116e, 0x116f and 0x1170 into three pool words.
 *    The main image has no name for these messages yet.
 *
 * 2. The cursor address. The reference rematerializes the cursor record's
 *    address (sp + 8) at each of its four calls and keeps window r7,
 *    choice r5, the constant 1 in r6 and the bob table in r8. Here CSE
 *    merges the two calls before the loop into one pseudo and GCSE copies
 *    it into the loop, so the address lives in r6 and pushes the window
 *    to r8 and the table to r10: one more high-register save.
 *    Rematerialization needs that pseudo to carry a REG_EQUIV note (loop
 *    hoisting would give it one) and lose its register; no spelling tried
 *    so far (typed 12-byte record, frame aggregate, scoped lifetimes, a
 *    cursor pointer variable, named table) does that. Everything else
 *    already matches instruction for instruction.
 *
 * To link it, IMPORT.S also needs labels on its unnamed veneers for
 * UiWindow_Create, UiWork_Finalize, UiText_DrawResource,
 * UiTextResource_Initialize, UiTextResource_SetPosition,
 * UiTextResource_Release and SaveState_CountRecordsExcludingFlagged, and
 * the sixteen-word cursor bob table at 0x0200c11c a label in the listing.
 * The input words are the linker-placed IWRAM names.
 */
#include "TYPES.H"

/* A text cursor record the text-resource routines fill and move. */
struct TextObject {
    u8 storage[12];
};

extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern volatile u32 gFrameTick;
/* How far the cursor bobs right over sixteen two-frame steps. */
extern const s32 ShindenHeya_CursorBob[16];

void Engine_ActorSetPosition(s32 actor, s32 x, s32 y);
void Engine_ColorBufferApplyTarget(s32 target, s32 mode);
void Engine_ColorBufferInterpolate(s32 frames);
void Engine_EventWait(s32 frames);
s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawResource(s32 message, s32 window, s32 x, s32 y);
s32 SaveState_CountRecordsExcludingFlagged(s32 flag);
void UiTextResource_Initialize(struct TextObject *object, s32 *slot);
void UiTextResource_SetPosition(struct TextObject *object, s32 x, s32 y);
void UiTextResource_Release(s32 slot);
void UiWork_Finalize(s32 window, s32 mode);

/* Shrine room: hides the party, fades the screen and asks whether to
 * restart, offering the file screen when no save records exist. Returns
 * the choice, 0 or 1, once A is pressed. */
s32 ShindenHeya_ChooseRestartOption(void)
{
    s32 handle;
    struct TextObject cursor;
    s32 window;
    s32 text;
    s32 choice;

    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(0, 0, 0);
    Engine_ColorBufferApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    window = UiWindow_Create(2, 7, 25, 5, 1);
    text = 0x116e;
    UiText_DrawResource(text, window, 16, 0);
    if (SaveState_CountRecordsExcludingFlagged(1) == 0)
        UiText_DrawResource(text + 2, window, 16, 16);
    else
        UiText_DrawResource(text + 1, window, 16, 16);
    UiTextResource_Initialize(&cursor, &handle);
    UiTextResource_SetPosition(&cursor, 72, 60);
    choice = 0;
    while ((gKeyState & 1) == 0) {
        if (gKeysRepeat & 0xc0)
            choice ^= 1;
        UiTextResource_SetPosition(&cursor, ShindenHeya_CursorBob[(gFrameTick >> 1) & 15] + 24, (choice << 4) + 60);
        Engine_EventWait(1);
    }
    UiTextResource_Release(handle);
    UiWork_Finalize(window, 1);
    return choice;
}
