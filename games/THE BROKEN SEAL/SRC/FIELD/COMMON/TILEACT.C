#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "GAME_STATE.H"
#include "EVENT_RUNTIME.H"
#include "FIELD_SCENE.H"
#include "text/MSG_IDS.H"
s32 GameFlag_TestFar(s32 flag);

#define EffectRuntime_GetCurrentObject ((void * (*)(s32))EffectRuntime_GetCurrentObject)
#define EffectRuntime_PrepareRisingObject ((void (*)(void*))EffectRuntime_PrepareRisingObject)

struct EffectDescriptor;
extern struct EventRuntime *gEventWork;


void WaitFrames(s32 frames);
void ObjectDispatch_ApplyArgumentToChildrenFar(void *object, s32 mode);
void ObjectDispatch_ReleaseFar(void *object);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void UiWork_PushValueSlotFar(s32 quantity, s32 slot);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void Party_AdjustCoinsFar(s32 coins);
s32 BattleFx_GetWeightedResult(s32 weight, s32 group);
void BattleFx_SelectBattleCue(s32 weight, s32 group);
void BattleParty_ApplyDrain(s32 amount, s32 mode);
void *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item);
void Battle_WaitMode0(s32 frames);
void Battle_Reset(void);
void BattleFx_FinishAction(void);
void BattleFx_PlayQueuedSound(void);
void AudioCommand_PlayFar(s32 cue);

s32 Field_RunTileAction(s32 tile)
{
    const struct SceneEvent *descriptor;
    s32 leader;
    u32 n;
    s16 condition;
    u32 action;
    s32 type;
    s32 allowed;

    leader = gGameState.selected_actor;
    n = tile - 242;
    if (n <= 5) {
        Battle_InitializeRenderObject();
        tile = Field_TileMessageOffsets[n];
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileChecked + tile, 1);
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + tile, 1);
    } else {
        descriptor = (const struct SceneEvent *)BattleFx_FindDescriptor(3, tile);
        if (descriptor != NULL) {
            s32 flags = (s32)descriptor->control;

            condition = descriptor->condition;
            n = (flags >> 4) & 31;
            if (((u16)descriptor->trigger & 0x400) == 0 && n != 0) {
                Battle_InitializeRenderObject();
                UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileChecked + n, 1);
                GameFlag_SetBitFar(0x142);
            } else {
                GameFlag_ClearBitFar(0x142);
            }
            action = descriptor->value;
            if ((action & 0x0f000000) != 0 || (action & 0xfff00000) == 0x400000) {
                if ((action & 0x0f000000) != 0) {
                    if (GameFlag_IsConditionActive(condition) != 0)
                        ((void (*)(s32))descriptor->value)(gGameState.selected_actor);
                    if (GameFlag_TestFar(0x142) != 0)
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + n, 1);
                } else {
                    if (GameFlag_IsConditionActive(condition) != 0)
                        UiText_ShowPositionedMessageAndWaitFar((*(const u16 *)&descriptor->value), 1);
                    else
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgButFoundNothing, 1);
                }
            } else {
                Battle_Reset();
                if (GameFlag_IsConditionActive(condition) != 0) {
                    action = descriptor->value;
                    allowed = 1;
                    if ((action & 0xf0000) == 0x10000 && leader <= 7)
                        allowed = 0;
                    if (allowed != 0) {
                        if (((s32)descriptor->control & 0x1ff) == 19)
                            EffectRuntime_SetMode4AndPlayCue(tile);
                        type = descriptor->value & 0xfff00000;
                        if (type == 0x300000) {
                            void *object;

                            if (((s32)descriptor->control & 0x1ff) == 19)
                                EffectRuntime_SetMode2(tile);
                            object = EffectRuntime_GetCurrentObject(tile);
                            EffectRuntime_PrepareRisingObject(object);
                            AudioCommand_PlayFar(83);
                            UiWork_PushValueSlotFar((*(const u16 *)&descriptor->value), 5);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone, 3);
                            BattleParty_ApplyDrain(999, 0);
                            UiWindow_CreateWithLayoutBoundsFar(1);
                            AudioCommand_PlayFar(126);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone + 1, 1);
                            UiWork_FinalizeAndReleaseBlock16Far();
                            ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
                            AudioCommand_PlayFar(246);
                            Battle_WaitMode0(30);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone + 2, 1);
                            EffectRuntime_ClearCurrentFlags(tile);
                            if (condition != -1)
                                GameFlag_SetBitFar(condition);
                        } else if (type == 0x500000) {
                            struct EventRuntime *work = gEventWork;

                            if (((s32)descriptor->control & 0x1ff) == 19)
                                EffectRuntime_SetMode7AndLaunch(tile);
                            if (condition != -1) {
                                condition = gGameState.pending_djinn_event = condition | 0x1000;
                            }
                            work->value_17c = BattleFx_GetWeightedResult(99, (*(const u16 *)&descriptor->value));
                            /* FAKEMATCH: needed to match: the one-pass loop keeps this store before the next call's value load. */
                            do {
                                gGameState.battle_start = 2;
                            } while (0);
                            BattleFx_SelectBattleCue(99, (*(const u16 *)&descriptor->value));
                            AudioCommand_PlayFar(gGameState.scene_cue);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgChestWasMimic, 1);
                        } else if (type == 0x200000) {
                            void *object;

                            object = BattleFx_StartRandomParticleEmitter(gGameState.selected_actor, 0);
                            WaitFrames(30);
                            if (((s32)descriptor->control & 0x1ff) == 19)
                                EffectRuntime_SetMode2(tile);
                            EffectRuntime_PrepareRisingObject(object);
                            AudioCommand_PlayFar(83);
                            UiWork_PushValueSlotFar((*(const u16 *)&descriptor->value), 5);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotCoins, 3);
                            Party_AdjustCoinsFar((*(const u16 *)&descriptor->value));
                            if (condition != -1)
                                GameFlag_SetBitFar(condition);
                            ObjectDispatch_ReleaseFar(object);
                        } else {
                            void *object;
                            s32 owner;

                            object = BattleFx_StartRandomParticleEmitter(gGameState.selected_actor, descriptor->value & 0xfff);
                            WaitFrames(30);
                            owner = PartyInventory_AddFar((*(const u16 *)&descriptor->value));
                            n = 0xffff;
                            if (owner == -1) {
                                UiWork_PushValueSlotFar(descriptor->value & 0xfff, 2);
                                UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundItem, 1);
                                UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundItem + 4, 1);
                                Object_DestroyIfPresent(object);
                                if (((s32)descriptor->control & 0x1ff) == 19)
                                    EffectRuntime_SetMode5AndPlayCue(tile);
                            } else {
                                if (((s32)descriptor->control & 0x1ff) == 19)
                                    EffectRuntime_SetMode2(tile);
                                EffectRuntime_PrepareRisingObject(object);
                                AudioCommand_PlayFar(83);
                                UiWork_PushValueSlotFar(descriptor->value & n, 2);
                                if (owner == gGameState.selected_actor) {
                                    UiText_ShowPositionedMessageAndWaitFar((s32)MsgKorosseoRobinGotItem, 3);
                                } else {
                                    UiWork_PushValueSlotFar(owner, 1);
                                    UiText_ShowPositionedMessageAndWaitFar((s32)MsgGaveItemToMember, 3);
                                }
                                if (condition != -1)
                                    GameFlag_SetBitFar(condition);
                                ObjectDispatch_ReleaseFar(object);
                            }
                        }
                    } else {
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgCantOpenIt, 1);
                    }
                } else {
                    UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + n, 1);
                }
                BattleFx_FinishAction();
                BattleFx_PlayQueuedSound();
            }
        } else {
            UiText_ShowPositionedMessageAndWaitFar((s32)MsgCheckedGround, 1);
            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGroundDidntFindAnything, 1);
        }
        GameFlag_ClearBitFar(0x142);
    }
    return 0;
}
