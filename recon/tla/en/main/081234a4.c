/* Draft: BattleEventRuntime_Reset, 76-byte EN extent.
 * 2026-10-04: differ 2 (alchemy diff). ☀️'s reset with ⚓️'s halfword cue
 * and the halfword after it. The native loads the battle work pointer before
 * starting the 0x82c offset; this build starts the offset constant first.
 * Tried: the session pointer as its own local (2), a zero local (2), "+r"
 * and volatile input barriers on the pointer (46-67: they change every
 * address computation). In sched2 the offset's lsls carries two forward
 * dependence links (true and output on the add) against the load's one, so
 * the tie never reaches the original-order rule; the same gap blocks
 * Resource_FindFreeEntry and Item_AdjustCounter.
 */
#include "BATTLE_WORK.H"

u32 BattleEventRuntime_Reset(void)
{
    struct BattleEventState *events;

    events = &Ram_HeapSlots->battle_work->events;
    events->phase = BATTLE_PLAYBACK_IDLE;
    events->queue.count = 0;
    events->event_index = 0;
    events->timer = 0;
    events->queue.target_index = 0;
    events->pending_cue = 0x86;
    events->unknown_16a = 0;
    events->flags = 0;
}
