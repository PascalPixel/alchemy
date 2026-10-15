/* Draft, differ 18 of 260: the second door scene, the twin of Func_02000af4 with a helper (actor 9)
 * who walks ahead when flag 0x932 is clear. Left: register allocation of two constants in the
 * helper's branch. The native shares the 2 of the two state stores in r8 and builds each 1
 * afresh (ldrb r2 first, then movs r3, #1 for the bit set); this draft shares the 1 of the bit
 * set and the last state store in r8 and builds each 2. Measured: the state pointer taken after
 * the flag test (68 to 18), a local helper for Func_02002c48 so its 0x10000 is not shared with
 * field48 (in the same step), a "+l" barrier on the bit set or a one-pass block after it (18), a
 * variable for the 2 (59). */
#include "../../../../games/tla/src/scene/lemurian_ship/lemurian_ship.h"

void Object_LinkObjectAndSetCallback(s32 actor, s32 target);

static inline void LemurianShip_SetWork(s32 a, s32 b, s32 c)
{
    Func_02002c48(a, b, c);
}

void Func_02000870(void)
{
    struct LemurianShipObject *leader;
    struct LemurianShipObject *helper;
    s32 opened;
    u8 *state;

    if (!GameFlag_Test(0x983)) {
        leader = Object_GetById(gPartyState.currentOwner);
        Func_02002c68();
        Func_02002da8(0);
        LemurianShip_ObjSpeed(gPartyState.currentOwner, 0xcccc, 0x6666);
        LemurianShip_ObjWalk(gPartyState.currentOwner, 840, 72);
        opened = GameFlag_Test(0x932);
        state = &leader->field22;
        if (opened) {
            Func_02002de0(243);
            LemurianShip_ObjEmoteWait(gPartyState.currentOwner, 0x102);
            LemurianShip_SetWork(0, 0x20000, 0x10000);
            Battle_WaitMode0(20);
            LemurianShip_SetWork(0, 0, 0);
            LemurianShip_MapCopy(68, 68, 50, 64, 3, 3);
            *state = 2;
            Battle_WaitMode0(20);
            ObjectMotion_Launch(gPartyState.currentOwner, 4, 0);
            LemurianShip_ObjSpeed(gPartyState.currentOwner, 0x19999, 0xcccc);
            LemurianShip_ObjCommit(gPartyState.currentOwner, 840, 88);
            Battle_WaitMode0(20);
            Func_02002de0(232);
            LemurianShip_MapCopy(72, 68, 50, 64, 3, 3);
            Battle_WaitMode0(20);
            *state = 1;
        } else {
            helper = Object_GetById(9);
            ObjectMotion_SetActionVariant(9, 2);
            LemurianShip_ObjSpeed(9, 0x26666, 0x13333);
            Object_GetById(9)->unk5a[0] &= ~1;
            LemurianShip_ObjWalk(9, 824, 88);
            Battle_WaitMode0(1);
            Object_GetById(9)->unk5a[0] |= 1;
            Func_02002de0(243);
            LemurianShip_ObjEmoteWait(gPartyState.currentOwner, 0x102);
            LemurianShip_SetWork(0, 0x20000, 0x10000);
            Battle_WaitMode0(20);
            LemurianShip_SetWork(0, 0, 0);
            LemurianShip_MapCopy(68, 68, 50, 64, 3, 3);
            *state = 2;
            Battle_WaitMode0(20);
            LemurianShip_ObjArm(gPartyState.currentOwner, 0x6000, 20);
            ObjectMotion_Launch(gPartyState.currentOwner, 4, 0);
            LemurianShip_ObjSpeed(gPartyState.currentOwner, 0x19999, 0xcccc);
            LemurianShip_ObjCommit(gPartyState.currentOwner, 840, 88);
            LemurianShip_ObjTurn(gPartyState.currentOwner, 0x8000);
            Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 9);
            helper->field48 = 0x10000;
            helper->field22 = 2;
            ObjectMotion_Launch(9, 4, 0);
            LemurianShip_ObjCommit(9, 832, 74);
            ObjectMotion_Launch(9, 4, 0);
            LemurianShip_ObjCommit(9, 856, 88);
            LemurianShip_ObjEmoteWait(gPartyState.currentOwner, 0x102);
            Battle_WaitMode0(20);
            Func_02002de0(232);
            LemurianShip_MapCopy(72, 68, 50, 64, 3, 3);
            Battle_WaitMode0(20);
            ObjectMotion_EnableActionAndResetMotion(gPartyState.currentOwner);
            *state = 1;
            helper->field22 = 0;
            WaitFrames(1);
            GameFlag_ClearBit(0x201);
        }
        Func_02002c70();
    }
}
