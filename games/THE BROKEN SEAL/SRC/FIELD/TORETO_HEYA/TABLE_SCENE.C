/* The party sits down at the table: the host hums, the four take their
 * places and gesture along, and the scene returns to the room afterwards. */
#include "HEYA.H"
#include "CALL.H"
extern u8 MsgToretoHmHrooom[];

/* The action tables the four party members take at the table. */
extern const u8 ToretoHeya_TableActions0[];
extern const u8 ToretoHeya_TableActions1[];
extern const u8 ToretoHeya_TableActions2[];
extern const u8 ToretoHeya_TableActions3[];

void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 mode);

void ToretoHeya_RunTableScene(void)
{
    u32 i;
    s32 rec8;
    s32 record;
    s32 base3_2000240;

    rec8 = Engine_GameFlagIsSet(3);
    Engine_EventBegin();
    Engine_AudioPlayCue(17);
    Engine_EventSetMessage((s32)MsgToretoHmHrooom);
    Engine_EventShowMessageAndWait(0x8009, 0, 20);
    Engine_AudioPlayCue(29);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Engine_ActorSetSpeed(3, 0x10000, 0x8000);
    *((u8 *)Engine_ActorGet(3) + 35) &= 254;
    Engine_ActorSetSpritePriority(3, 2);
    *((u8 *)Engine_ActorGet(0) + 35) &= 254;
    Engine_ActorSetSpritePriority(0, 2);
    record = ((s32 (*)())Engine_ActorGet)(0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = ((s32 (*)())Engine_ActorGet)(0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    if (rec8 != 0) {
        record = ((s32 (*)())Engine_ActorGet)(0);
        if (record != 0) {
            ((void (*)())Engine_ActorSetPosition)(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Engine_ActorEnableActionCallback(3, ToretoHeya_TableActions3);
    }
    ((s32 (*)())Engine_ActorEnableActionCallback)(0, (s32)ToretoHeya_TableActions0);
    ((s32 (*)())Engine_ActorEnableActionCallback)(1, (s32)ToretoHeya_TableActions1);
    Object_SetActionCallbackAndRefreshById(2, (s32)ToretoHeya_TableActions2);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 40);
    Engine_ActorSetAnimation(8, 11);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 8);
    Engine_EventWait(20);
    ToretoHeya_PlayGesture(8);
    Call2(Engine_EventShowMessage, 0x8008, 0);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(3, 2);
    Engine_ActorStartRepeatedMotion(2, 2);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0);
    Engine_ActorShowEmote(2, 0x100, 60);
    ToretoHeya_PlayGesture(11);
    Call3(Engine_EventShowMessageAndWait, 0x8008, 0, 10);
    Engine_ActorStartRepeatedMotion(0, 1);
    Engine_ActorStartRepeatedMotion(1, 1);
    Engine_ActorStartRepeatedMotion(3, 1);
    Engine_ActorRunRepeatedMotion(2, 1);
    Call2(Engine_EventShowMessage, 0x8008, 0);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    Engine_ActorSetAttachedEffect(2, 0x102);
    Engine_EventWait(40);
    ToretoHeya_PlayGesture(11);
    Engine_EventShowMessage(0x8008, 0);
    {
        u8 *work = *(u8 **)&gEventWork;

        *(s32 *)(work + 0x1c0) = 0x200;
        *(s32 *)(work + 0x1c8) = 64;
    }
    base3_2000240 = (s32)&gGameState;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_ToretoHeya, 19);
    BattleFx_SetWeightedResult(36, 0);
    Engine_EventEnd();
}
