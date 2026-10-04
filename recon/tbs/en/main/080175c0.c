/* 2026-10-04 sun100 main-7: still score 2 (frame 16 vs 8); return-shape,
 * zero-reuse and declaration-order variants all give frame 8. Lead to test:
 * ARM frames grow downward and reload's setup_save_areas leaves a 4-byte
 * caller-save slot per call-clobbered register a call-crossing pseudo held
 * in any reload pass, even when that pseudo is later evicted; the native's
 * eight extra bytes fit two such slots. Needs the source that makes global
 * alloc choose caller-saves (refs > 4 x calls crossed, no callee-saved reg). */
/* 2026-10-04 frame audit: UiText_OpenEntryMessage, 96 bytes, pool included.
 * Uncredited under S1/S2/C1; the owned English ROM supplies the raw listing.
 * Entries JA/EN/DE/ES/FR/IT: 0801750c, 080175c0, 08016260, 08016590, 08016570, 08017570.
 * Native frame: 16 bytes. Only outgoing arguments at sp+0 and sp+4 are
 * used by UiText_QueueRenderEntries; no address of sp+8..15 escapes.
 * The previous unused[8] supplied only frame size, outside S2. Removing
 * it gives score 2: only the 16-to-8-byte prologue/epilogue adjustment.
 * The current caller consumes the returned slot, and the six-argument
 * queue routine has no additional caller-frame input. Keep the ordinary
 * eight-byte frame until a truthful source owner for the rest is known.
 * Independent native-stack/consumer review agrees the explicit reserve
 * has no consumed owner. Adjacent functions remain in their source module.
 */
#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"

s32 UiText_BuildRenderEntries(s32, s32);
struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, const u16 *colours, s32 flags);

s32 UiText_OpenEntryMessage(s32 window, s32 argument)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 entry;
    s32 result = 0;

    work->result[0] = 0;
    work->result[1] = 0;
    entry = UiText_BuildRenderEntries(argument, 1);
    if (work->entries[entry] == 0)
        return 0;
    if (window == 0)
        return 0;
    result = (s32)UiText_QueueRenderEntries((struct UiWindow *)window, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
