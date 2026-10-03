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
#include "BATTLE_RUNTIME.H"
#include "PARTY_STATE.H"
#include "OBJECT_RUNTIME.H"
#include "EVENT_RUNTIME.H"
#include "FIXED_POINT_POSITION.H"


/* shop/effect/reset.c */
void EffectSlot_UpdateFar(s32);
void ObjectGroup_SetChildValueUnlessFifteenFar(void *, u32);

union PositionWord { s32 w; s16 h[2]; };

u32 Random16(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);
void Shop_RestoreSceneTiles(s32 address);
void EffectSlot_InitializeFar(struct EffectSlot *effect, s32 mode, s32 x, s32 z);
void EffectSlot_SetCallbackFar(
    struct EffectSlot *effect,
    void (*callback)(struct EffectSlot *));
void EffectSlot_SetObjectModeFar(struct EffectSlot *effect, s32 value);
void AudioCommand_WaitForStateByteClear(void);
void BattleFx_ClearOwnedSlotFar(struct EffectSlot *effect);
void Func_08009280(struct AnimationObject *object, s32 arg);
void Shop_InitEffect(void);
void Shop_ResetEffects(void);
void BattleFx_UpdateRadialMotion(struct EffectSlot *effect);
extern s8 Data_080b4ab2[];

extern struct EventRuntime *gEventWork;
s32 Party_ListActiveOwnersFar(s16 *);
void Party_AdjustSixDigitCounterAFar(s32);
void Owner_RecalculateRatiosFar(s32);
void Event_ClearStatus1c6Far(void);
void Event_WaitValue1c8FramesFar(void);
void Audio_PlayCue(s32);
void Event_SetStatus1c6Far(void);
extern s8 Inn_PriceMultipliers[];

enum InnMessageId {
    INN_MESSAGE_WELCOME = 0xd1c,
    INN_MESSAGE_STAY_COMPLETE,
    INN_MESSAGE_NOT_ENOUGH_COINS,
    INN_MESSAGE_GOODBYE,
    INN_MESSAGE_REST_COMPLETE,
};

extern char MsgInnWelcome;
void Shop_InitializeCursorWork(void);
void Inn_Cleanup(void);
void UiMessage_ShowAndWait(s32 message_id);
s32 UiMessage_ShowChoice(s32);
s32 Inn_RoomPrice(s32);
void Inn_PlaySleep(s32);
void UiWork_FinalizeFar(s32, s32);
s32 UiWindow_CreateWithSideObjectFar(u16, s32, s32, s32);
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
    Audio_PlayCue(Data_080b4ab2[shop->party_action]);
    Shop_RestoreSceneTiles(0x00202108);
    Func_08009280(shop->party_member_icons[member], 0);
    WaitFrames(20);
    callback_flags = 0xc80;
    Scheduler_AddOrUpdateCallback((s32)(Shop_ResetEffects), callback_flags);

    /* FAKEMATCH: the x store goes through a union with a halfword view so it
       may alias the member_z load, which keeps the reference schedule. */
    ((union PositionWord *)&position.x)->w = (s32)shop->party_member_x[member] << 16;
    position.z = ((s32)shop->party_member_y[member] << 16) + (s32)0xfff40000;

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
            shop->burst_member = (u8)member;
        }
        i++;
        effect++;
    } while (i <= 17);

    AudioCommand_WaitForStateByteClear();
    {
        u8 active_mode = 2;
        struct EffectSlot *entry = shop->effects;
        for (i = 23; i >= 0; i--, entry++) {
            if (entry->active != 0)
                entry->state = active_mode;
        }
    }

    WaitFrames(20);
    Audio_PlayCue(126);
    shop->burst_member = -1;
    ObjectGroup_SetChildValueUnlessFifteenFar(shop->party_member_icons[member], 0);
    WaitFrames(20);

    {
        struct EffectSlot *entry = shop->effects;
        for (i = 23; i >= 0; i--, entry++) {
            if (entry->active != 0)
                BattleFx_ClearOwnedSlotFar(entry);
        }
    }

    Scheduler_RemoveCallback((u32)(Shop_ResetEffects));
    Func_08009280(shop->party_member_icons[member], 16);
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
        if (Owner_GetStateFar(shop->party_member_ids[index])->hp != 0)
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
    state->resource_id = (u16)((struct AnimationObject *)object->animation)->entries[0]->anim_id;
    win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);

    amount = Inn_RoomPrice(mode);
    UiWork_PushValueSlotFar(amount, 5);
    message_base = (s32)&MsgInnWelcome;
    UiMessage_ShowAndWait(message_base);
    state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
    Shop_DrawMoney();

    if (UiMessage_ShowChoice(0) != 0) {
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(state->window, 2);
    } else if ((u32)amount > (u32)gGameState.coins) {
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
        state->resource_id = (u16)((struct AnimationObject *)object->animation)->entries[0]->anim_id;
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
    struct BattleUnit *object;
    struct EventRuntime *state;

    count = Party_ListActiveOwnersFar(objects);
    Party_AdjustSixDigitCounterAFar(-room_price);

    for (index = 0; index < count; index++) {
        object = Owner_GetStateFar(objects[index]);
        if (object->hp != 0) {
            object->hp = object->max_hp;
            object->pp = object->max_pp;
            Owner_RecalculateRatiosFar(objects[index]);
        }
    }

    state = gEventWork;
    state->value_1c0 = 0x209;
    state->value_1c8 = 60;
    WaitFrames(20);
    Event_ClearStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    Audio_PlayCue(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
    Event_SetStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    WaitFrames(30);
    gEventWork->value_1c8 = 16;
}
