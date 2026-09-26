/* NONMATCHING: the reference has three spill slots before the records and
 * selection arrays (44-byte frame); this draft has two (40-byte frame).
 * A volatile local viewport gives 44 bytes but six extra instructions and
 * the wrong spill order, so it was rejected. A typed view of the slot's
 * +0x2a halfword store leaves the pooled constant unchanged. The count in
 * the phase-11 record scan stays in r6; the reference spills r2 across
 * GetMotionRecord. A condition-call loop gives 1858 bytes/325 aligned
 * edits, but still 40 stack bytes; a post-increment scan gives 1860/326.
 * Sharing the scan counter across phases stays at 1858/331 and keeps
 * the buffer base in sl. None recovers the missing count spill, so the
 * complete baseline body is retained. Fix that lifetime before registers. */
#include "../../../../games/THE BROKEN SEAL/SRC/BATTLE/PRESENTATION/EVENT_PLAYBACK.C"
