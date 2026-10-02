#include "TYPES.H"
#include "GAME_STATE.H"
#include "EVENT_RUNTIME.H"

struct TileDescriptor {
    s32 flags;                  /* bits 0-8 the kind, bits 4-8 the message */
    u16 value_flags;
    s16 condition;
    union {
        u32 word;               /* a callback, or a kind over a value */
        u16 value;              /* the value in the word's low half */
    } action;
};

/* The leader's cell, reached by its own address. */
#define STATE_LEADER (*(s32 *)&gGameState.selected_actor)

/* Six message offsets, for tiles 242 to 247. */
extern const u8 Field_TileMessageOffsets[];
extern struct EventRuntime *gEventWork;

/* Messages are numbered by the edition's catalog; a name's address is its
   number. The two families take a kind's offset. */
extern char MsgTileChecked;
extern char MsgCheckedGround;
extern char MsgTileResult;
extern char MsgGroundDidntFindAnything;
extern char MsgFoundItem;
extern char MsgGotCoins;
extern char MsgKorosseoRobinGotItem;
extern char MsgGaveItemToMember;
extern char MsgCantOpenIt;
extern char MsgGotPsynergyStone;
extern char MsgChestWasMimic;
extern char MsgButFoundNothing;

void WaitFrames(s32 frames);
void Object_SetMode(void *object, s32 mode);
void ObjectDispatch_ReleaseFar(void *object);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void UiText_DrawQuantity(s32 quantity, s32 slot);
void UiWindow_CreateWithLayoutBoundsFar(s32 mode);
void UiWork_FinalizeAndReleaseBlock16Far(void);
s32 PartyInventory_AddFar(s32 item);
s32 GameFlag_IsSet(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void Party_AdjustSixDigitCounterAFar(s32 coins);
s32 BattleFx_GetWeightedResult(s32 weight, s32 group);
void BattleFx_SelectBattleCue(s32 weight, s32 group);
void BattleParty_ApplyDrain(s32 amount, s32 mode);
s32 GameFlag_IsConditionActive(s32 condition);
struct TileDescriptor *BattleFx_FindDescriptor(s32 kind, s32 tile);
void EffectRuntime_SetMode5AndPlayCue(s32 tile);
void EffectRuntime_SetMode7AndLaunch(s32 tile);
void EffectRuntime_SetMode4AndPlayCue(s32 tile);
void EffectRuntime_SetMode2(s32 tile);
void *EffectRuntime_GetCurrentObject(s32 tile);
void EffectRuntime_ClearCurrentFlags(s32 tile);
void *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item);
void Object_DestroyIfPresent(void *object);
void EffectRuntime_PrepareRisingObject(void *object);
void Battle_WaitMode0(s32 frames);
void Battle_InitializeRenderObject(void);
void Battle_Reset(void);
void BattleFx_FinishAction(void);
void BattleFx_PlayQueuedSound(void);
void AudioCommand_PlayFar(s32 cue);

/* Serves the tile the leader stepped on or examined. Tiles 242 to 247 show
   a fixed pair of messages. The others look up their descriptor and run its
   action: a callback, a message, a Psynergy Stone, an ambush, coins or an
   item, each only while the descriptor's condition holds. */
s32 Field_RunTileAction(s32 tile)
{
    struct TileDescriptor *descriptor;
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
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgTileChecked + tile, 1);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgTileResult + tile, 1);
    } else {
        descriptor = BattleFx_FindDescriptor(3, tile);
        if (descriptor != NULL) {
            s32 flags = descriptor->flags;

            condition = descriptor->condition;
            n = (flags >> 4) & 31;
            if ((descriptor->value_flags & 0x400) == 0 && n != 0) {
                Battle_InitializeRenderObject();
                UiText_ShowPositionedMessageAndWaitFar((s32)&MsgTileChecked + n, 1);
                GameFlag_SetBitFar(0x142);
            } else {
                GameFlag_ClearBitFar(0x142);
            }
            action = descriptor->action.word;
            if ((action & 0x0f000000) != 0 || (action & 0xfff00000) == 0x400000) {
                if ((action & 0x0f000000) != 0) {
                    if (GameFlag_IsConditionActive(condition) != 0)
                        ((void (*)(s32))descriptor->action.word)(gGameState.selected_actor);
                    if (GameFlag_IsSet(0x142) != 0)
                        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgTileResult + n, 1);
                } else {
                    if (GameFlag_IsConditionActive(condition) != 0)
                        UiText_ShowPositionedMessageAndWaitFar(descriptor->action.value, 1);
                    else
                        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgButFoundNothing, 1);
                }
            } else {
                Battle_Reset();
                if (GameFlag_IsConditionActive(condition) != 0) {
                    action = descriptor->action.word;
                    allowed = 1;
                    if ((action & 0xf0000) == 0x10000 && leader <= 7)
                        allowed = 0;
                    if (allowed != 0) {
                        if ((descriptor->flags & 0x1ff) == 19)
                            EffectRuntime_SetMode4AndPlayCue(tile);
                        type = descriptor->action.word & 0xfff00000;
                        if (type == 0x300000) {
                            void *object;

                            if ((descriptor->flags & 0x1ff) == 19)
                                EffectRuntime_SetMode2(tile);
                            object = EffectRuntime_GetCurrentObject(tile);
                            EffectRuntime_PrepareRisingObject(object);
                            AudioCommand_PlayFar(83);
                            UiText_DrawQuantity(descriptor->action.value, 5);
                            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgGotPsynergyStone, 3);
                            BattleParty_ApplyDrain(999, 0);
                            UiWindow_CreateWithLayoutBoundsFar(1);
                            AudioCommand_PlayFar(126);
                            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgGotPsynergyStone + 1, 1);
                            UiWork_FinalizeAndReleaseBlock16Far();
                            Object_SetMode(object, 2);
                            AudioCommand_PlayFar(246);
                            Battle_WaitMode0(30);
                            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgGotPsynergyStone + 2, 1);
                            EffectRuntime_ClearCurrentFlags(tile);
                            if (condition != -1)
                                GameFlag_SetBitFar(condition);
                        } else if (type == 0x500000) {
                            struct EventRuntime *work = gEventWork;

                            if ((descriptor->flags & 0x1ff) == 19)
                                EffectRuntime_SetMode7AndLaunch(tile);
                            if (condition != -1) {
                                condition = gGameState.pending_djinn_event = condition | 0x1000;
                            }
                            work->value_17c = BattleFx_GetWeightedResult(99, descriptor->action.value);
                            /* FAKEMATCH: the one-pass loop keeps this store ahead of
                               the next call's value load. */
                            do {
                                gGameState.battle_start = 2;
                            } while (0);
                            BattleFx_SelectBattleCue(99, descriptor->action.value);
                            AudioCommand_PlayFar(gGameState.scene_cue);
                            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgChestWasMimic, 1);
                        } else if (type == 0x200000) {
                            void *object;

                            object = BattleFx_StartRandomParticleEmitter(gGameState.selected_actor, 0);
                            WaitFrames(30);
                            if ((descriptor->flags & 0x1ff) == 19)
                                EffectRuntime_SetMode2(tile);
                            EffectRuntime_PrepareRisingObject(object);
                            AudioCommand_PlayFar(83);
                            UiText_DrawQuantity(descriptor->action.value, 5);
                            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgGotCoins, 3);
                            Party_AdjustSixDigitCounterAFar(descriptor->action.value);
                            if (condition != -1)
                                GameFlag_SetBitFar(condition);
                            ObjectDispatch_ReleaseFar(object);
                        } else {
                            void *object;
                            s32 owner;

                            object = BattleFx_StartRandomParticleEmitter(STATE_LEADER, descriptor->action.word & 0xfff);
                            WaitFrames(30);
                            owner = PartyInventory_AddFar(descriptor->action.value);
                            n = 0xffff;
                            if (owner == -1) {
                                UiText_DrawQuantity(descriptor->action.word & 0xfff, 2);
                                UiText_ShowPositionedMessageAndWaitFar((s32)&MsgFoundItem, 1);
                                UiText_ShowPositionedMessageAndWaitFar((s32)&MsgFoundItem + 4, 1);
                                Object_DestroyIfPresent(object);
                                if ((descriptor->flags & 0x1ff) == 19)
                                    EffectRuntime_SetMode5AndPlayCue(tile);
                            } else {
                                if ((descriptor->flags & 0x1ff) == 19)
                                    EffectRuntime_SetMode2(tile);
                                EffectRuntime_PrepareRisingObject(object);
                                AudioCommand_PlayFar(83);
                                UiText_DrawQuantity(descriptor->action.word & n, 2);
                                if (owner == STATE_LEADER) {
                                    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgKorosseoRobinGotItem, 3);
                                } else {
                                    UiText_DrawQuantity(owner, 1);
                                    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgGaveItemToMember, 3);
                                }
                                if (condition != -1)
                                    GameFlag_SetBitFar(condition);
                                ObjectDispatch_ReleaseFar(object);
                            }
                        }
                    } else {
                        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgCantOpenIt, 1);
                    }
                } else {
                    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgTileResult + n, 1);
                }
                BattleFx_FinishAction();
                BattleFx_PlayQueuedSound();
            }
        } else {
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgCheckedGround, 1);
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgGroundDidntFindAnything, 1);
        }
        GameFlag_ClearBitFar(0x142);
    }
    return 0;
}
