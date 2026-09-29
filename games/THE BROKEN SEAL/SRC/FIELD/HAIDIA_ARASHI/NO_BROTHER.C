#include "GROUP_DEPARTURE.H"

extern u8 MsgHaidiaNoBrother[];

void HaidiaArashi_RunRiverSearch(void);

/* The storm night at the river, until flag 0x83a is set: the villagers are
 * placed along the bank, actor 26 cries out for her brother, the leader and
 * actor 22 come down to look, actor 23 is swept away, and the river search
 * follows. The calls go through the header's tagged wrappers. */
void FieldScene_RunFlagGatedActorSequence(void)
{
    u8 *tbl;

    if (Value1(Engine_GameFlagIsSet, 0x83a) != 0) {
        return;
    }
    Engine_EventBegin();
    Call3(Engine_ActorSetPosition, 10, 0xC00000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 10, 0x2000, 0);
    Call2(Engine_ActorSetAnimation, 10, 5);
    {
        u8 *o;
        s32 v;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 10);
        v = Value2(IwramUnsignedRemainder, Engine_RandomNext(), 0x5A) + 60;
        tbl = HaidiaArashi_ActorEightScript;
        *(u16 *)(o + 0x64) = v;
        Call2(Engine_ActorEnableActionCallback, 10, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 9, 0xC00000, 0x4A50000);
    Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
    Call3(Engine_ActorSetPosition, 24, 0xE30000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 24, 0x4000, 0);
    Call2(Engine_ActorSetAnimation, 24, 6);
    {
        u8 *o;
        s32 v;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 24);
        v = Value2(IwramUnsignedRemainder, Engine_RandomNext(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Call2(Engine_ActorEnableActionCallback, 24, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 25, 0xFA0000, 0x4BE0000);
    Call3(Engine_ActorFaceDirection, 25, 0x4000, 0);
    Call2(Engine_ActorSetAnimation, 25, 6);
    {
        u8 *o;
        s32 v;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 25);
        v = Value2(IwramUnsignedRemainder, Engine_RandomNext(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Call2(Engine_ActorEnableActionCallback, 25, (s32)tbl);
    }
    Call3(Engine_ActorSetPosition, 26, 0xE30000, 0x4A50000);
    Call3(Engine_ActorFaceDirection, 26, 0x3000, 0);
    Call3(Engine_ActorSetPosition, 23, 0xF30000, 0x4FD0000);
    Call3(Engine_ActorFaceDirection, 23, 0xC000, 0);
    Call2(Engine_ActorSetSpriteFlags, Value1((s32 (*)())Engine_ActorGet, 23), 0);
    Call1(Engine_TaskWait, 3);
    Call1(Engine_EventSetMessage, (s32)MsgHaidiaNoBrother);
    Call2(Engine_EventShowMessage, 0x201a, 0);
    Call3(Engine_ActorShowEmote, ACTOR_PARTY_LEADER, 0x100, 20);
    Call3(Engine_ActorWalkToAndWait, ACTOR_PARTY_LEADER, 150, 0x446);
    {
        u8 *p;
        p = (u8 *)Value1((s32 (*)())Engine_ActorGet, ACTOR_PARTY_LEADER);
        if (p != 0) {
            Call3(Engine_ActorSetPosition, 22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Call3(Engine_ActorWalkToAndWait, 22, 132, 0x446);
    Call3(Engine_ActorFaceEachOther, ACTOR_PARTY_LEADER, 22, 0);
    Call1(Engine_EventWait, 40);
    Call3(Engine_ActorFaceDirection, ACTOR_PARTY_LEADER, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 22, 0x4000, 20);
    Call2(Engine_CameraSetSpeed, 0x40000, 0x8000);
    Call4(Engine_CameraMoveTo, 0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Call1(Engine_EventWait, 40);
    Call2(Engine_ActorRunRepeatedMotion, 10, 2);
    Call3(Engine_EventShowMessageAndWait, 10, 0, 10);
    Call2(Engine_ActorRunRepeatedMotion, 23, 3);
    Call3(Engine_ActorFaceDirection, 9, 0, 10);
    Call2(Engine_ActorSetAnimationAndWait, 9, 3);
    Call3(Engine_EventShowMessageAndWait, 9, 0, 10);
    Call3(Engine_ActorFaceDirection, 9, 0x3000, 10);
    Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
    Call4(Engine_CameraMoveTo, 0xE80000, -1, 0x4E50000, 1);
    Engine_CameraWaitForMove();
    Call1(Engine_EventWait, 20);
    Call1(Engine_AudioPlayCue, 134);
    Call3(Engine_ActorJump, 23, 4, 0);
    Call2(Engine_ActorSetAnimation, 23, 6);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorSetPosition, 23, 0, 0);
    Call1(Engine_EventWait, 60);
    BattleFx_PlayQueuedSound();
    Call2(Engine_ActorSetAnimation, 10, 1);
    {
        u8 *o;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 10);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Call2(Engine_ActorSetAnimation, 24, 1);
    {
        u8 *o;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 24);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Call2(Engine_ActorSetAnimation, 25, 1);
    {
        u8 *o;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 25);
        *(s32 *)(o + 0x18) = 0x10000;
        *(s32 *)(o + 0x1C) = 0x10000;
    }
    Call2(Engine_ActorStartRepeatedMotion, 10, 2);
    Call2(Engine_ActorStartRepeatedMotion, 9, 2);
    Call2(Engine_ActorStartRepeatedMotion, 24, 2);
    Call2(Engine_ActorStartRepeatedMotion, 25, 2);
    Call2(Engine_ActorRunRepeatedMotion, 26, 2);
    Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Engine_CameraMoveTo, 0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Call2(Engine_ActorSetAttachedEffect, 26, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 9, 0x102);
    Call1(Engine_EventWait, 60);
    Call2(Engine_ActorRunRepeatedMotion, 26, 2);
    Call2(Engine_ActorStartRepeatedMotion, 26, 3);
    Call2(Engine_EventShowMessage, 26, 0);
    Call3(Engine_ActorJump, 25, 2, 0);
    Call3(Engine_ActorSetDestination, 25, 234, 0x4B5);
    Call3(Engine_ActorJump, 26, 2, 0);
    Call3(Engine_ActorSetDestination, 26, 227, 0x4B1);
    Call1(Engine_EventWait, 90);
    Call4(Engine_CameraMoveTo, 0xE80000, -1, 0x4E50000, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_ActorSetPosition, 23, 0xF30000, 0x4FD0000);
    Call1(Engine_TaskWait, 1);
    Call1(Engine_AudioPlayCue, 106);
    {
        u8 *o;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 23);
        *(s32 *)(o + 0x28) = 0x20000;
    }
    Call1(Engine_EventWait, 6);
    Call2(Engine_ActorSetAnimation, 23, 7);
    Call1(Engine_EventWait, 20);
    BattleFx_PlayQueuedSound();
    Call1(Engine_EventWait, 20);
    Call2(Engine_CameraSetSpeed, 0x19999, 0x3333);
    Call4(Engine_CameraMoveTo, 0xD80000, -1, 0x4D00000, 1);
    Engine_CameraWaitForMove();
    Call2(Engine_ActorRunRepeatedMotion, 24, 2);
    Call1(Engine_EventWait, 20);
    Call3(Engine_ActorShowEmote, 24, 0x105, 40);
    Call3(Engine_ActorFaceEachOther, 24, 10, 0);
    Call1(Engine_EventWait, 10);
    Call2(Engine_ActorRunRepeatedMotion, 10, 2);
    Call2(Engine_EventShowMessage, 0x800A, 0);
    {
        u8 *o;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 25);
        o[0x5A] &= 0xFE;
    }
    {
        u8 *o;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 26);
        o[0x5A] &= 0xFE;
    }
    Call3(Engine_ActorSetSpeed, 25, 0x9999, 0x4CCC);
    Call3(Engine_ActorSetSpeed, 26, 0x9999, 0x4CCC);
    Call3(Engine_ActorSetDestination, 25, 247, 0x4BA);
    Call3(Engine_ActorMoveToAndWait, 26, 227, 0x4A5);
    {
        u8 *o;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 25);
        o[0x5A] |= 1;
    }
    {
        u8 *o;
        o = (u8 *)Value1((s32 (*)())Engine_ActorGet, 26);
        o += 0x5A;
        /* FAKEMATCH: the set bit first, so the or writes into the register
           that holds it, as the ROM's does. */
        {
            u8 set = 1 | *o;
            *o = set;
        }
    }
    Call3(Engine_ActorFaceDirection, 26, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 10);
    Call2(Engine_ActorSetAnimation, 24, 4);
    Call3(Engine_EventShowMessageAndWait, 0x8018, 0, 10);
    Call3(Engine_ActorFaceDirection, 10, 0xC000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0, 10);
    Call2(Engine_ActorSetAnimation, 10, 4);
    Call3(Engine_EventShowMessageAndWait, 0x800A, 0, 10);
    Call3(Engine_ActorShowEmote, 24, 0x105, 0);
    Call3(Engine_ActorShowEmote, 10, 0x105, 60);
    Call3(Engine_ActorShowEmote, 9, 0x106, 20);
    Call3(Engine_ActorFaceDirection, 9, 0x8000, 40);
    Call3(Engine_ActorFaceDirection, 9, 0xC000, 20);
    Call3(Engine_ActorFaceDirection, 9, 0, 30);
    Call3(Engine_ActorFaceDirection, 9, 0x4000, 10);
    Call3(Engine_EventShowMessageAndWait, 9, 0, 10);
    Call3(Engine_ActorFaceDirection, 10, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x9000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 26, 0x8000, 10);
    Call2(Engine_ActorRunRepeatedMotion, 10, 1);
    Call3(Engine_EventShowMessageAndWait, 0x800A, 0, 10);
    Call2(Engine_ActorSetAnimation, 9, 4);
    Call3(Engine_EventShowMessageAndWait, 9, 0, 10);
    Call3(Engine_ActorShowEmote, 10, 0x105, 0);
    Call3(Engine_ActorShowEmote, 24, 0x105, 0);
    Call3(Engine_ActorShowEmote, 25, 0x105, 0);
    Call3(Engine_ActorShowEmote, 26, 0x105, 40);
    Call3(Engine_ActorFaceDirection, 9, 0, 10);
    Call3(Engine_ActorFaceEachOther, 24, 25, 0);
    Call1(Engine_EventWait, 20);
    Call3(Engine_ActorFaceDirection, 9, 0, 0);
    Call3(Engine_ActorFaceDirection, 10, 0, 10);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 10);
    Call2(Engine_ActorSetAnimation, 24, 3);
    Call2(Engine_ActorSetAnimationAndWait, 25, 3);
    Call3(Engine_ActorFaceEachOther, 10, 9, 0);
    Call1(Engine_EventWait, 20);
    Call2(Engine_ActorRunRepeatedMotion, 10, 1);
    Call3(Engine_EventShowMessageAndWait, 0x800A, 0, 10);
    Call2(Engine_ActorSetAnimationAndWait, 9, 3);
    Call3(Engine_ActorFaceDirection, 24, 0xD000, 10);
    Call2(Engine_ActorStartRepeatedMotion, 24, 1);
    Call3(Engine_EventShowMessageAndWait, 24, 0, 10);
    Call3(Engine_ActorFaceDirection, 10, 0, 0);
    Call3(Engine_ActorFaceDirection, 9, 0, 0);
    Call2(Engine_ActorRunRepeatedMotion, 26, 1);
    Call3(Engine_ActorFaceDirection, 26, 0x2000, 20);
    Call3(Engine_ActorFaceDirection, 25, 0xA000, 20);
    Call2(Engine_ActorSetAnimationAndWait, 25, 3);
    Call3(Engine_EventShowMessageAndWait, 25, 0, 10);
    Call2(Engine_ActorSetAnimationAndWait, 26, 3);
    Call1(Engine_EventWait, 20);
    Call2(Engine_ActorRunRepeatedMotion, 9, 2);
    Call2(Engine_ActorSetAnimationAndWait, 9, 3);
    Call3(Engine_EventShowMessageAndWait, 9, 0, 10);
    Call3(Engine_ActorFaceDirection, 26, 0x8000, 10);
    Call2(Engine_ActorSetAnimationAndWait, 26, 3);
    Call1(Engine_EventWait, 20);
    Call2(Engine_ActorRunRepeatedMotion, 9, 1);
    Call3(Engine_EventShowMessageAndWait, 9, 0, 10);
    HaidiaArashi_RunRiverSearch();
    Call1(Engine_GameFlagSet, 0x83A);
    Engine_EventEnd();
}
