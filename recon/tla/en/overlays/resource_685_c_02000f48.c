/* Draft of overlay 685's Func_02000f48, differ 211 of 205: the trident talk, which takes the first
 * of items 439 to 441 the party holds and runs the next of three flagged scenes with it, or says
 * one of four lines. The decoded behaviour is complete; the layout is not. The native places the
 * shared "remove, then go on" block and the item 441 test ahead of the item 439 test (both reached
 * by backward branches: bne to Remove(item), beq to the 441 test), and compares each later owner
 * with the previous -1 kept in r10 and r8; this draft's if/else chain emits them in source order
 * with one -1. The layout looks like gotos or a rotated loop; not tried further. */
#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

struct EventFlags685 {
    u8 unk000[0x15a];
    u16 field15a;
};

s32 PartyInventory_FindOwner(s32 item);
void PartyInventory_Remove(s32 item);
void Func_0200343c(s32 x, s32 z, s32 a, s32 b);
void Func_02000804(s32 item);
void Func_02000978(s32 item);
void Func_02000a34(s32 item);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);

void Func_02000f48(s32 actor)
{
    s32 item;

    if ((Object_GetById(gPartyState.currentOwner)->x >> 20) > 41) {
        ((struct EventFlags685 *)Ram_HeapSlots->eventWork)->field15a = 1;
        return;
    }
    item = 439;
    if (PartyInventory_FindOwner(item) != -1) {
        PartyInventory_Remove(item);
    } else {
        item++;
        if (PartyInventory_FindOwner(item) != -1) {
            PartyInventory_Remove(item);
        } else {
            item = 441;
            if (PartyInventory_FindOwner(item) != -1)
                PartyInventory_Remove(item);
            else
                item = -1;
        }
    }
    if (item != -1) {
        if (!GameFlag_Test(0x975)) {
            GameFlag_SetBit(0x975);
            Func_02000804(item);
        } else if (!GameFlag_Test(0x976)) {
            GameFlag_SetBit(0x976);
            Func_02000978(item);
        } else if (!GameFlag_Test(0x977)) {
            GameFlag_SetBit(0x977);
            Func_0200343c(164 << 18, 208 << 16, 0, -4);
            Func_0200343c(164 << 18, 224 << 16, 0, -4);
            Func_0200343c(164 << 18, 240 << 16, 0, -4);
            Func_0200343c(164 << 18, 128 << 17, 0, -4);
            Func_02000a34(item);
            Func_0200343c(164 << 18, 208 << 16, 0, 0);
            Func_0200343c(164 << 18, 224 << 16, 0, 0);
            Func_0200343c(164 << 18, 240 << 16, 0, 0);
            Func_0200343c(164 << 18, 128 << 17, 0, 0);
        }
    } else {
        ObjectMotion_SetAngleToward(actor, gPartyState.currentOwner, 5);
        if (!GameFlag_Test(0x975))
            Func_0200350c((s32)MsgIMeanIt);
        else if (!GameFlag_Test(0x976))
            Func_0200350c((s32)MsgTheLegendSaysThat);
        else if (!GameFlag_Test(0x977))
            Func_0200350c((s32)MsgYesThereS);
        else
            Func_0200350c((s32)MsgTheTridentSeems);
        Func_02003524(actor, 0);
        ObjectMotion_ArmCallback(actor, 0x5000, 0);
    }
}
