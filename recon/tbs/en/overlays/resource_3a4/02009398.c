/* Draft of resource_3a4 0x02009398 (FieldScene_RunScene3a4_02001398), built with
 * games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.H.
 * Remaining difference: the ROM holds message 0x1953 in r5 across the calls and adds one to it, as a link-time value would; the constant 0x1953 is folded into two pool words instead.
 * The listing keeps these rows. */
#include "YAMA.H"

extern u8 Data_00001953[];

void FieldScene_RunScene3a4_02001398(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    u8 *record;
    s32 base5_1953;

    Event_Begin();
    base5_1953 = (s32)Data_00001953;
    Value2(Engine_MessageShowCentered, base5_1953, 1);
    if (GameFlag_IsSet(0x908) != 0) {
    } else {
        if (GameFlag_IsSet(0xf14) != 0) {
        } else {
            GameFlag_Set(0x205);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x316, 140);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x30c, 140);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
            record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            if ((s32)record != 0) {
                Actor_SetPosition(ACTOR_GERALD, *(volatile s32 *)((s32)record + 8), *(volatile s32 *)((s32)record + 16));
            }
            Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
            Actor_WalkToAndWait(ACTOR_GERALD, 0x320, 140);
            Actor_FaceDirection(ACTOR_GERALD, 0xc000, 20);
            Event_SetMessage((base5_1953 + 1));
            Actor_SetAnimation(ACTOR_GERALD, 4);
            Event_Wait(20);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
            Actor_Jump(ACTOR_GERALD, 6, 0);
            Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
            *(u8 *)((u8 *)Engine_ActorGet(1) + 90) &= 254;
            Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 110);
            Event_Wait(1);
            *(u8 *)((u8 *)Engine_ActorGet(1) + 90) |= 1;
            Audio_PlayCue(161);
            Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
            *(u8 *)((u8 *)Engine_ActorGet(1) + 90) &= 254;
            Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 120);
            Event_Wait(1);
            {
                u8 *record = Actor_Get(ACTOR_GERALD);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(80);
            Audio_PlayCue(141);
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Event_Wait(40);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
            Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
            Actor_FaceDirection(ACTOR_GERALD, 0, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
            Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
            Actor_FaceDirection(ACTOR_GERALD, 0, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
            Actor_FaceDirection(ACTOR_GERALD, 0xc000, 40);
            Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
            Actor_FaceDirection(ACTOR_GERALD, 0x4000, 20);
            Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
            Actor_SetSpeed(ACTOR_GERALD, 0x28000, 0x14000);
            Actor_SetAnimation(ACTOR_GERALD, 5);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x31c, 138);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x324, 140);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x324, 166);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x2fc, 166);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x2fc, 198);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x312, 198);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x312, 246);
            Actor_SetAnimation(ACTOR_GERALD, 1);
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Event_Wait(40);
            FieldScene_RunSharedSetPiece(10);
        }
    }
    Event_End();
}
