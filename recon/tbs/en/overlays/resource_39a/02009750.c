/* NONMATCHING: resource_39a at 0x02009750 (88 bytes with its pool),
 * ImiruFuchin_ApplyEntryHook, the first entry veneer's hook, between
 * FIELD/COMMON/IMIRU_FUCHIN/TRACKING.C and ARRIVAL.C, stays listing.
 *
 * Remaining difference: the reference loads the scene number 0x34 from its
 * literal pool and compares registers, as a link-time scene number does; a
 * plain constant compiles to cmp with an immediate. With 0x34 as a link-time
 * value this body compiles to the reference exactly.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

void FieldScene_RunScene39aSequenceA(void);
void ImiruFuchin_ApplyEntrySetup(void);

/* Open the screen through a window. Until flag 0x109 is set, entering scene
 * 0x34 raises flag 0x144 and plays the leader's arrival; every other entry
 * sets the room up. */
s32 ImiruFuchin_ApplyEntryHook(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (!GameFlag_IsSet(0x109) && gGameState.scene == 0x34) {
        GameFlag_Set(0x144);
        FieldScene_RunScene39aSequenceA();
    } else {
        ImiruFuchin_ApplyEntrySetup();
    }
    return 0;
}
