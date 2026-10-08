#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "GLOBAL_CELLS.H"
#include "SHOP.H"
#include "TBS_EDITION.H"
#include "INN_RUNTIME.H"
#include "UI.H"
#include "BTLSYS.H"
#include "PARTY_STATE.H"
#include "OBJECT_RUNTIME.H"
#include "EVENT_RUNTIME.H"
#include "FIXED_POINT_POSITION.H"
#include "text/MSG_IDS.H"

#define UiWindow_CreateWithSideObjectFar ((s32 (*)(u16,s32,s32,s32))UiWindow_CreateWithSideObjectFar)

union PositionWord { s32 w; s16 h[2]; };

void AudioCommand_PlayFar(s32 sound_id);
void EffectSlot_SetCallbackFar(
    struct EffectSlot *effect,
    void (*callback)(struct EffectSlot *));

extern struct EventRuntime *gEventWork;
void Event_WaitTransitionFramesFar(void);
void AudioCommand_PlayFar(s32);
void Event_StartScreenTransitionFar(void);

enum InnMessageId {
    INN_MESSAGE_WELCOME = 0xd1c,
    INN_MESSAGE_STAY_COMPLETE,
    INN_MESSAGE_NOT_ENOUGH_COINS,
    INN_MESSAGE_GOODBYE,
    INN_MESSAGE_REST_COMPLETE,
};

void UiWork_FinalizeFar(s32, s32);
void UiWork_PushValueSlotFar(s32, s32);
struct ObjectRuntime *Object_GetByIdFar(s32);

void Shop_ResetEffects(void)
{
    struct ShopRuntime *shop = (struct ShopRuntime *)gMenuWork;
    struct EffectSlot *effect = shop->effects;
    s32 count = 23;
    s8 member;

    do {
        count--;
        EffectSlot_UpdateFar((s32)effect);
        effect++;
    } while (count >= 0);
    member = shop->burst_member;
    if (member != -1)
        ObjectGroup_SetChildValueUnlessFifteenFar(
            shop->party_member_icons[member], (Random16() * 7) >> 16);
}

void Shop_RunPartyMemberIconBurst(s32 member)
{

    struct ShopRuntime *shop;
    s8 saved_kind;
    struct FixedPointPosition position;
    struct EffectSlot *effect;
    s32 callback_flags;
    s32 i;

    shop = ((struct ShopRuntime *)gMenuWork);
    saved_kind = shop->cursor.anchor->active;
    shop->burst_member = 0xff;
    shop->cursor.anchor->active = 13;
    AudioCommand_PlayFar(Shop_PartyActionCues[shop->party_action]);
    Shop_RestoreSceneTiles(0x00202108);
    AnimationObjects_SetActiveTimerStepsFar(shop->party_member_icons[member], 0);
    WaitFrames(20);
    callback_flags = 0xc80;
    Scheduler_AddOrUpdateCallback(Shop_ResetEffects, callback_flags);

    /* FAKEMATCH: the x store goes through a union with a halfword view so it may alias the member_z load, which keeps the reference schedule. */
    ((union PositionWord *)&position.x)->w = shop->party_member_x[member] << 16;
    position.z = (shop->party_member_y[member] << 16) + 0xfff40000;

    i = 0;
    effect = shop->effects;
    do {
        EffectSlot_InitializeFar(effect, 0x11c, position.x, position.z);
        EffectSlot_SetCallbackFar(effect, BattleFx_UpdateRadialMotion);
        EffectSlot_SetObjectModeFar(effect, 7);
        ObjectGroup_SetChildValueUnlessFifteenFar(
            effect->object,
            (Random16() * 7) >> 16);
        effect->scale_y = 0xb333;
        effect->scale_x = 0xb333;
        WaitFrames(3);
        if (i == 5) {
            shop->burst_member = member;
        }
        i++;
        effect++;
    } while (i <= 17);

    AudioCommand_WaitForStateByteClear();
    {
        u8 active_mode = 2;
        u8 *entry = (u8 *)&shop->effects[0].state;
        for (i = 23; i >= 0; i--) {
            if (*(s8 *)(entry + 5) != 0) {
                *entry = active_mode;
            }
            entry += sizeof(struct EffectSlot);
        }
    }

    WaitFrames(20);
    AudioCommand_PlayFar(126);
    shop->burst_member = -1;
    ObjectGroup_SetChildValueUnlessFifteenFar(shop->party_member_icons[member], 0);
    WaitFrames(20);

    {
        u8 *flag_entry = (u8 *)&shop->effects[0].active;
        struct EffectSlot *entry2 =
            shop->effects;
        for (i = 23; i >= 0; i--) {
            s32 flag = *flag_entry;

            flag <<= 24;

            flag_entry += 0x48;
            if (flag != 0) {
                BattleFx_ClearOwnedSlotFar(entry2);
            }
            entry2++;
        }
    }

    Scheduler_RemoveCallback(Shop_ResetEffects);
    AnimationObjects_SetActiveTimerStepsFar(shop->party_member_icons[member], 16);
    Shop_InitEffect();
    WaitFrames(30);
    shop->cursor.anchor->active = saved_kind;
}

#if EDITION_INTERNATIONAL
#define MESSAGE_WINDOW_ROWS 12
#else
#define MESSAGE_WINDOW_ROWS 11
#endif

s32 Inn_RoomPrice(s32 mode)
{
    struct ShopRuntime *shop = (struct ShopRuntime *)gMenuWork;
    s32 active = 0;
    s32 factor = Inn_PriceMultipliers[mode];
    s32 index;

    for (index = 0; index < shop->party_member_count; index++) {
        if (UnitGetFar(shop->party_member_ids[index])->hp != 0)
            active++;
    }
    return factor * active;
}

s32 Inn_CheckIn(s32 mode, s32 object_id)
{
    struct InnRuntimeState *state;
    struct ObjectRuntime *object;
    s32 win;
    s32 amount;
    s32 message_base;

    Shop_InitializeCursorWork();
    state = gMenuWork;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Object_GetByIdFar(object_id);
    state->resource_id = ((struct AnimationObject *)object->animation)->entries[0]->anim_id;
    win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);

    amount = Inn_RoomPrice(mode);
    UiWork_PushValueSlotFar(amount, 5);
    message_base = (s32)MsgInnWelcome;
    UiMessage_ShowAndWait(message_base);
    state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
    Shop_DrawMoney();

    if (UiMessage_ShowChoice(0) != 0) {
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(state->window, 2);
    } else if (amount > (u32)gGameState.coins) {
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(state->window, 2);
    } else {
        UiWork_FinalizeFar(state->window, 2);
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(win, 2);
        Inn_PlaySleep(amount);

        object = Object_GetByIdFar(object_id);
        state->resource_id = ((struct AnimationObject *)object->animation)->entries[0]->anim_id;
        win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
    }

    UiWork_FinalizeFar(win, 2);
    Inn_Cleanup();
    return 0;
}

void Inn_PlaySleep(s32 room_price)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct BtlUnit *object;
    struct EventRuntime *state;

    count = Party_ListActiveOwnersFar(objects);
    Party_AdjustCoinsFar(-room_price);

    for (index = 0; index < count; index++) {
        object = UnitGetFar(objects[index]);
        if (object->hp != 0) {
            object->hp = object->max_hp;
            object->ep = object->max_ep;
            UnitRatioFar(objects[index]);
        }
    }

    state = gEventWork;
    state->value_1c0 = 0x209;
    state->value_1c8 = 60;
    WaitFrames(20);
    Event_FinishScreenTransitionFar();
    Event_WaitTransitionFramesFar();
    AudioCommand_PlayFar(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
    Event_StartScreenTransitionFar();
    Event_WaitTransitionFramesFar();
    WaitFrames(30);
    gEventWork->value_1c8 = 16;
}
