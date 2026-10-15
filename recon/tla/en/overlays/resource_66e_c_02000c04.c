/* Draft, differ 140 of 167 (the switch and both loops have the native's shape; the count is
 * inflated by pool offsets). Left: loop.c strength-reduces the limits[group] store into a
 * walking pointer and GCSE's PRE and loop.c hoist the array's stack address into a callee-saved
 * register; the native rebuilds both (mov r2, sp; str r0, [r2, r3]) on every pass, which shifts
 * every register after it. As a volatile array only the hoisted sp copy and the lowest group's
 * register (r9 for r10) are left (differ 117). Tried: the first loop as a goto loop. */
#include "types.h"
#include "text/msg_ids.h"

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
u32 Random16Far(void);
s32 Func_02000b60(s32 group);
s32 Func_02000ba0(s32 limit, s32 group, s32 *out);

u8 *Func_02000c04(void)
{
    s32 *hints = Runtime_AllocateBlock(252, 320);
    s32 limits[4];
    s32 count = 0;
    s32 lowest = 9999;
    s32 lowestGroup = 0;
    u32 group;
    s32 index;
    u8 *message;

    for (group = 0; group <= 3; group++) {
        s32 limit = Func_02000b60(group);

        limits[group] = limit;
        if (lowest > limit) {
            lowest = limit;
            lowestGroup = group;
        }
        if (limit != -1)
            count += Func_02000ba0(limit, group, hints + count);
    }
    if (count == 0) {
        for (group = 0; group <= 3; group++)
            count += Func_02000ba0(18, group, hints + count);
        if (count == 0) {
            Runtime_ReleaseHeapBlock(252);
            return MsgInTheItem;
        }
        Func_02000ba0(18, lowestGroup, hints);
        index = 0;
    } else {
        index = Random16Far() % count;
    }
    switch (hints[index]) {
    case 56: message = MsgToTheEast; break;
    case 57: message = MsgToTheNorth; break;
    case 58: message = MsgToTheNorthBeneath; break;
    case 59: message = MsgHeadEastBy; break;
    case 60: message = MsgHeadEastByNortheast; break;
    case 61: message = MsgHeadNorthBy; break;
    case 62: message = MsgToTheNorthwest; break;
    case 63: message = MsgHeadWestBy; break;
    case 64: message = MsgToTheNorthIn; break;
    case 65: message = MsgHeadEastBySoutheast; break;
    case 75: message = MsgHeadEastByNortheastIn; break;
    case 76: message = MsgToTheSoutheast; break;
    case 79: message = MsgToTheSouth; break;
    case 80: message = MsgToTheEastBeside; break;
    case 81: message = MsgHeadEastByNortheastInThe; break;
    case 82: message = MsgToTheNorthSomewhere; break;
    case 83: message = MsgHeadNorthByNorthwest; break;
    case 84: message = MsgToTheNorthInRuins; break;
    case 85: message = MsgHeadEastByNortheastInA; break;
    case 95: message = MsgToTheEastIn; break;
    case 96: message = MsgToTheSoutheastIn; break;
    case 97: message = MsgHeadSouthBy; break;
    case 98: message = MsgHeadEastBySoutheastIn; break;
    case 99: message = MsgHeadEastByNortheastOn; break;
    case 100: message = MsgHeadSouthBySoutheast; break;
    case 101: message = MsgToTheNorthwestIn; break;
    case 102: message = MsgToTheNorthwestAlong; break;
    case 103: message = MsgToTheNorthwestBeside; break;
    case 104: message = MsgHeadNorthByNorthwestAt; break;
    case 105: message = MsgToTheNorthInThe; break;
    case 115: message = MsgHeadEastByNortheastInACave; break;
    case 116: message = MsgHeadEastBySoutheastInThe; break;
    case 117: message = MsgHeadEastBySoutheastInA; break;
    case 118: message = MsgToTheNorthInTheMountain; break;
    case 119: message = MsgToTheEastSomewhere; break;
    case 120: message = MsgToTheEastInThe; break;
    case 121: message = MsgHeadNorthByNorthwestIn; break;
    case 122: message = MsgInTheNorthwest; break;
    case 123: message = MsgHeadNorthByNorthwestNear; break;
    case 124: message = MsgToTheNorthInTheRuins; break;
    case 125: message = MsgHeadNorthByNortheast; break;
    default: message = MsgInTheItem; break;
    }
    Runtime_ReleaseHeapBlock(252);
    return message;
}
