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


/* shop/effect/reset.c */
void EffectSlot_UpdateFar(s32);
void ObjectGroup_SetChildValueUnlessFifteenFar(s32, u32);

struct Position { s32 x, y, z; };

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
void Func_08009248(s32 object, u32 frame_offset);
void AudioCommand_WaitForStateByteClear(void);
void BattleFx_ClearOwnedSlotFar(struct EffectSlot *effect);
void Func_08009280(s32 object, s32 arg);
void Shop_InitEffect(void);
void Shop_ResetEffects(void);
void BattleFx_UpdateRadialMotion(struct EffectSlot *effect);
extern s8 Data_080b4ab2[];

struct FieldEffectState {
    u8 padding0[0x1C0];
    s32 effect;
    u8 padding1C4[4];
    s32 delay;
};

extern struct FieldEffectState *gEventWork;
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
            (s32)shop->party_member_icons[member], (Random16() * 7) >> 16);
}

void Shop_RunPartyMemberIconBurst(s32 member)
{
    struct ShopRuntime *shop;
    s8 saved_kind;
    struct Position position;
    struct EffectSlot *effect;
    s32 callback_flags;
    s32 i;

    shop = ((struct ShopRuntime *)gMenuWork);
    saved_kind = shop->cursor.anchor->active;
    shop->burst_member = 0xff;
    shop->cursor.anchor->active = 13;
    Audio_PlayCue(Data_080b4ab2[shop->party_action]);
    Shop_RestoreSceneTiles(0x00202108);
    Func_08009280((s32)shop->party_member_icons[member], 0);
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
        Func_08009248(
            (s32)effect->object,
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
        u8 *entry = (u8 *)&shop->effects[0].state;
        for (i = 23; i >= 0; i--) {
            if (*(s8 *)(entry + 5) != 0) {
                *entry = active_mode;
            }
            entry += sizeof(struct EffectSlot);
        }
    }

    WaitFrames(20);
    Audio_PlayCue(126);
    {
        s32 icon_offset = 0xff;

        shop->burst_member = icon_offset;
        icon_offset += 21;
        icon_offset += member * 4;
        Func_08009248(*(s32 *)((u8 *)shop + icon_offset), 0);
    }
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

    Scheduler_RemoveCallback((u32)(Shop_ResetEffects));
    Func_08009280((s32)shop->party_member_icons[member], 16);
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
    u8 *global = (u8 *)gMenuWork;
    u8 *base;
    s32 active = 0;
    s32 factor = Inn_PriceMultipliers[mode];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(global + 0x3A7)) {
        base = global + 2;
        offset = 0x36C;
        do {
            if (Owner_GetStateFar(*(s16 *)(base + offset))->hp != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(global + 0x3A7));
    }

    return factor *active;
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
    state->resource_id = *((struct ShopKeeperAnimation *)object->animation)->resource;
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
        state->resource_id = *((struct ShopKeeperAnimation *)object->animation)->resource;
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
    struct FieldEffectState *state;

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
    state->effect = 0x209;
    state->delay = 60;
    WaitFrames(20);
    Event_ClearStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    Audio_PlayCue(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
    Event_SetStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    WaitFrames(30);
    gEventWork->delay = 16;
}
