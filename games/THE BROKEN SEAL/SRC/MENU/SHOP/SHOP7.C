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

extern u8 Data_03001f2c[];

/* shop/effect/reset.c */
void EffectSlot_UpdateFar(s32);
void ObjectGroup_SetChildValueUnlessFifteenFar(s32, u32);

struct Position { s32 x, y, z; };

union PositionWord { s32 w; s16 h[2]; };

struct Effect_080b2f4c { u8 filler[0x48]; };

struct ShopBurstRuntime {
    u8 unknown_000[0x134];
    s16 member_x[8];
    s16 member_z[8];
};

u32 Random16(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);
void Shop_RestoreSceneTiles(s32 address);
void Func_0808a528(struct Effect_080b2f4c *effect, s32 mode, s32 x, s32 z);
void Func_0808a520(
    struct Effect_080b2f4c *effect,
    void (*callback)(struct Effect_080b2f4c *));
void Func_0808a518(struct Effect_080b2f4c *effect, s32 value);
void Func_08009248(s32 object, u32 frame_offset);
void AudioCommand_WaitForStateByteClear(void);
void Func_0808a530(struct Effect_080b2f4c *effect);
void Func_08009280(s32 object, s32 arg);
void Shop_InitEffect(void);
void Shop_ResetEffects(void);
void BattleFx_UpdateRadialMotion(struct Effect_080b2f4c *effect);
extern s8 Data_080b4ab2[];

struct FieldEffectState {
    u8 padding0[0x1C0];
    s32 effect;
    u8 padding1C4[4];
    s32 delay;
};

struct FieldObject {
    u8 padding0[0x34];
    u16 saved_x;
    u16 saved_y;
    s16 x;
    u16 y;
};

extern struct FieldEffectState *gEventWork;
s32 Party_ListActiveOwnersFar(s16 *);
void Party_AdjustSixDigitCounterAFar(s32);
struct FieldObject *Owner_GetStateFar(s32);
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

struct InnGlobalState {
    u8 padding_00[0x10];
    u32 limit;
};

struct InnObjectComponent {
    u8 padding_00[0x28];
    u16 *resource_id;
};

struct InnObject {
    u8 padding_00[0x50];
    struct InnObjectComponent *component;
};

extern struct InnGlobalState gGameState;
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
struct InnObject *Object_GetByIdFar(s32);

void Shop_ResetEffects(void)
{
    s32 p;
    s32 cnt;
    s32 work;
    s8 no;
    s32 offset;

    work = *(s32 *)((u32)&Data_03001f2c);
    p = work + 0x3B0;
    cnt = 0x17;
    do {
        cnt -= 1;
        EffectSlot_UpdateFar(p);
        p += 0x48;
    } while (cnt >= 0);
    no = *(s8 *)(work + 0x3AB);
    if (no != -1) {
        ObjectGroup_SetChildValueUnlessFifteenFar(*(s32 *)(work + (offset = (no * 4) + 0x114)), (Random16() * 7) >> 16);
    }
}

void Shop_RunPartyMemberIconBurst(s32 member)
{
    struct ShopRuntime *shop;
    struct ShopBurstRuntime *burst;
    s8 saved_kind;
    struct Position position;
    struct Effect_080b2f4c *effect;
    s32 callback_flags;
    s32 i;

    shop = ((struct ShopRuntime *)gMenuWork);
    burst = (struct ShopBurstRuntime *)shop;
    saved_kind = shop->cursor.anchor->kind;
    *(u8 *)((u8 *)shop + 0x3ab) = 0xff;
    shop->cursor.anchor->kind = 13;
    Audio_PlayCue(Data_080b4ab2[shop->party_action]);
    Shop_RestoreSceneTiles(0x00202108);
    Func_08009280((s32)shop->party_member_icons[member], 0);
    WaitFrames(20);
    callback_flags = 0xc80;
    Scheduler_AddOrUpdateCallback((s32)(Shop_ResetEffects), callback_flags);

    /* FAKEMATCH: the x store goes through a union with a halfword view so it
       may alias the member_z load, which keeps the reference schedule. */
    ((union PositionWord *)&position.x)->w = (s32)burst->member_x[member] << 16;
    position.z = ((s32)burst->member_z[member] << 16) + (s32)0xfff40000;

    i = 0;
    effect = (struct Effect_080b2f4c *)((u8 *)shop + 0x3b0);
    do {
        Func_0808a528(effect, 0x11c, position.x, position.z);
        Func_0808a520(effect, BattleFx_UpdateRadialMotion);
        Func_0808a518(effect, 7);
        Func_08009248(
            *(s32 *)((u8 *)effect + 0),
            (Random16() * 7) >> 16);
        *(s32 *)((u8 *)effect + 44) = 0xb333;
        *(s32 *)((u8 *)effect + 40) = 0xb333;
        WaitFrames(3);
        if (i == 5) {
            *(u8 *)((u8 *)shop + 0x3ab) = (u8)member;
        }
        i++;
        effect = (struct Effect_080b2f4c *)((u8 *)effect + 0x48);
    } while (i <= 17);

    AudioCommand_WaitForStateByteClear();
    {
        u8 active_mode = 2;
        u8 *entry = (u8 *)shop + 0x3f0;
        for (i = 23; i >= 0; i--) {
            if (*(s8 *)(entry + 5) != 0) {
                *(u8 *)(entry + 0) = active_mode;
            }
            entry += 0x48;
        }
    }

    WaitFrames(20);
    Audio_PlayCue(126);
    {
        s32 icon_offset = 0xff;

        *(u8 *)((u8 *)shop + 0x3ab) = icon_offset;
        icon_offset += 21;
        icon_offset += member * 4;
        Func_08009248(*(s32 *)((u8 *)shop + icon_offset), 0);
    }
    WaitFrames(20);

    {
        u8 *flag_entry = (u8 *)shop + 0x3f5;
        struct Effect_080b2f4c *entry2 =
            (struct Effect_080b2f4c *)((u8 *)shop + 0x3b0);
        for (i = 23; i >= 0; i--) {
            s32 flag = *flag_entry;

            flag <<= 24;

            flag_entry += 0x48;
            if (flag != 0) {
                Func_0808a530(entry2);
            }
            entry2 = (struct Effect_080b2f4c *)((u8 *)entry2 + 0x48);
        }
    }

    Scheduler_RemoveCallback((u32)(Shop_ResetEffects));
    Func_08009280((s32)shop->party_member_icons[member], 16);
    Shop_InitEffect();
    WaitFrames(30);
    shop->cursor.anchor->kind = saved_kind;
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
            if (*(s16 *)((u8 *)Owner_GetStateFar(
                    *(s16 *)(base + offset)) + 56) != 0)
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
    struct InnObject *object;
    s32 win;
    s32 amount;
    s32 message_base;

    Shop_InitializeCursorWork();
    state = gMenuWork;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Object_GetByIdFar(object_id);
    state->resource_id = *object->component->resource_id;
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
    } else if ((u32)amount > gGameState.limit) {
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
        state->resource_id = *object->component->resource_id;
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
    struct FieldObject *object;
    struct FieldEffectState *state;

    count = Party_ListActiveOwnersFar(objects);
    Party_AdjustSixDigitCounterAFar(-room_price);

    for (index = 0; index < count; index++) {
        object = Owner_GetStateFar(objects[index]);
        if (object->x != 0) {
            object->x = object->saved_x;
            object->y = object->saved_y;
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
