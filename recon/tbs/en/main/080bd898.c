/* NONMATCHING: the reference has three spill slots before the records and
 * selection arrays (44-byte frame); this draft has two (40-byte frame).
 * A volatile local viewport gives 44 bytes but six extra instructions and
 * the wrong spill order, so it was rejected. A typed view of the slot's
 * +0x2a halfword store leaves the pooled constant unchanged. The count in
 * the phase-11 record scan stays in r6; the reference spills r2 across
 * GetMotionRecord. Fix that lifetime before tuning register names. */
#include "../../../../games/THE BROKEN SEAL/SRC/BATTLE/PRESENTATION/EVENT_PLAYBACK.C"
