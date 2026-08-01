#include "types.h"

/*
 * Core-drive row 0x080fa16c, 64 bytes (kind nonstandard_thumb_branch_module,
 * admitted by the 2026-07-31 classification rule -- body read: an ordinary
 * `push {r4, r5}` / `pop {r4, r5}; bx lr` leaf with no register trick, the
 * label is a classification artifact of its branch density, not a
 * cannot-express shape).
 *
 * Reads a value byte from the stream cursor at track+64; a byte >= 128 is
 * treated as a "repeat last value" marker (the cursor is not advanced and
 * the value comes from track+5 instead, wherever a prior call to this same
 * owner left it), otherwise the byte is the value and is cached to track+5
 * for a future repeat. Walks a singly linked list headed at track+32 (each
 * node's "next" pointer at +52), looking for the first node whose flags
 * byte at +0 has bit 0x83 set and bit 0x40 clear, and whose byte at +17
 * equals the value just read; on the first match it sets bit 0x40 in that
 * node's flags (marking it, role not established beyond that) and stops
 * the walk.
 *
 * Complete owner: asm/080fa16c.s in full, `push {r4, r5}` through
 * `pop {r4, r5}; bx lr`; no literal pool. Two arguments (r0 dead, matching
 * the established player/track family convention; r1 is reassigned
 * mid-function to walk the list, so it is not the same variable as the
 * track parameter throughout).
 */

struct ListNode_080fa16c {
    u8 flags;
    u8 unknown_01[16];
    u8 marker;
    u8 unknown_12[34];
    struct ListNode_080fa16c *next;
};

struct Track_080fa16c {
    u8 unknown_00[5];
    u8 cachedValue;
    u8 unknown_06[26];
    struct ListNode_080fa16c *list;
};

void Func_080fa16c(void *player, struct Track_080fa16c *track)
{
    u8 *cursor = *(u8 **)((u8 *)track + 64);
    u8 value;
    struct ListNode_080fa16c *node;

    value = cursor[0];
    if (value >= 128) {
        value = track->cachedValue;
    } else {
        track->cachedValue = value;
        *(u8 **)((u8 *)track + 64) = cursor + 1;
    }

    node = track->list;
    while (node != 0) {
        if ((node->flags & 0x83) != 0 && (node->flags & 0x40) == 0 &&
            node->marker == value) {
            node->flags |= 0x40;
            break;
        }
        node = node->next;
    }
}
