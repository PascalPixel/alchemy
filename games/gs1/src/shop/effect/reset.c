#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "inn.h"
#include "shop.h"
#include "battle_calc.h"
#include "battle_msg.h"
#include "battle_party.h"
#include "battle_intro.h"
#include "inventory.h"

/* shop/effect/reset.c */
/* shop/effect/reset.c */
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16

void Shop_ResetEffects(void)
{
    s32 p;
    s32 cnt;
    s32 work;
    s8 no;
    s32 offset;

    work = *(s32 *)ADDR_03001F2C;
    p = work + 0x3B0;
    cnt = 0x17;
    do {
        cnt -= 1;
        Effect_Do(p);
        p += 0x48;
    } while (cnt >= 0);
    no = *(s8 *)(work + 0x3AB);
    if (no != -1) {
        Effect_Apply(*(s32 *)(work + (offset = (no * 4) + 0x114)), (Rand() * 7) >> 16);
    }
}

/* inn/room_price.c */
/* inn/room_price.c */
extern s8 gRom[];

struct FieldObject *Runtime_GetObject(s32);

s32 Inn_RoomPrice(s32 mode)
{
    u8 *global = (u8 *)gIw;
    u8 *base;
    s32 active = 0;
    s32 factor = gRom[mode];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(global + 0x3A7)) {
        base = global + 2;
        offset = 0x36C;
        do {
            if (*(s16 *)((u8 *)Runtime_GetObject(
                    *(s16 *)(base + offset)) + 56) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(global + 0x3A7));
    }

    return factor *active;
}

/* inn/check_in.c */
#if defined(GS1_EDITION_JA)
#define MESSAGE_WINDOW_ROWS 11
#else
#define MESSAGE_WINDOW_ROWS 12
#endif

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

extern struct InnGlobalState gCell;
extern char Value_00000d1c;

void Inn_Cleanup(void);

s32 Inn_RoomPrice(s32);
void Inn_PlaySleep(s32);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_Close(s32 window, s32 style);

void UiText_DrawQuantity(s32, s32);
struct InnObject *Scene_GetRecord(s32);

s32 Inn_CheckIn(s32 mode, s32 object_id)
{
    struct InnState *state;
    struct InnObject *object;
    s32 win;
    s32 amount;
    s32 message_base;

    FunctionHead_080b010c();
    state = gIw;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Scene_GetRecord(object_id);
    state->resource_id = *object->component->resource_id;
    win = Sys_SetMode(state->resource_id, 0, 0, 0);

    amount = Inn_RoomPrice(mode);
    UiText_DrawQuantity(amount, 5);
    message_base = (s32)&Value_00000d1c;
    FunctionHead_080b04dc(message_base);
    state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
    Shop_DrawMoney();

    if (FunctionHead_080b0634(0) != 0) {
        FunctionHead_080b04dc(message_base
            + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
        UiWindow_Close(state->window, 2);
    } else if ((u32)amount > gCell.limit) {
        FunctionHead_080b04dc(message_base
            + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
        UiWindow_Close(state->window, 2);
    } else {
        UiWindow_Close(state->window, 2);
        FunctionHead_080b04dc(message_base
            + (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
        UiWindow_Close(win, 2);
        Inn_PlaySleep(amount);

        object = Scene_GetRecord(object_id);
        state->resource_id = *object->component->resource_id;
        win = Sys_SetMode(state->resource_id, 0, 0, 0);
        FunctionHead_080b04dc(message_base
            + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
    }

    UiWindow_Close(win, 2);
    Inn_Cleanup();
    return 0;
}

/* inn/sleep.c */
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

extern struct FieldEffectState *gWork;


void WaitFrames(s32);

void Audio_PlayCue(s32 cue);
void AudioCommand_WaitForStateByteClear(void);

void Inn_PlaySleep(s32 room_price)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct FieldObject *object;
    struct FieldEffectState *state;

    count = FunctionHead_08077158(objects);
    FunctionHead_08077230(-room_price);

    for (index = 0; index < count; index++) {
        object = Runtime_GetObject(objects[index]);
        if (object->x != 0) {
            object->x = object->saved_x;
            object->y = object->saved_y;
            Owner_RecalculateRatios(objects[index]);
        }
    }

    state = gWork;
    state->effect = 0x209;
    state->delay = 60;
    WaitFrames(20);
    FunctionHead_0808a368();
    Event_WaitValue1c8Frames();
    Audio_PlayCue(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
    Event_SetStatus1c6();
    Event_WaitValue1c8Frames();
    WaitFrames(30);
    gWork->delay = 16;
}

/* shop/pick_unit_item.c */
/* shop/sel/pick_item.c */
#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

s32 Modulo(s32 value, s32 divisor);
struct ShopCursorAnchor *FunctionHead_080150c8(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);


/* Select a party member and then an item owned by that member. */
s32 Shop_PickUnitItem(s32 *selected_unit, s32 *selected_item)
{
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;
    s32 list_window;
    s32 selected_index = 0;
    s32 redraw = 1;
    s32 unit_id = 0;
    s32 item_slot;
    s32 result = 0;

    FunctionHead_0808a368();
    shop = SHOP_RUNTIME;
    shop->item_window = UiWindow_CreateFar(16, 12, 14, 8, 2);
    list_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
    cursor_anchor = FunctionHead_080150c8(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        result);
    cursor_anchor->kind = 4;
    cursor_anchor->unknown_00[4] = result;
    Sys_Place(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    shop->mode = 12;
    Sys_SetRange(list_window, 2, 0, 8, result);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selected_index = Modulo(
                selected_index + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selected_index];
            Shop_PlaceCursor((void *)list_window, selected_index * 24 - 12, 0);
            shop->mode = 3;
            Shop_DrawParty(list_window, selected_index, 0);
            Shop_DrawUnitGrid(shop->item_window, unit_id);
        }

        WaitFrames(1);
        if ((INPUT_NEW_KEYS & 1) != 0) {
            if (Ability_GetAvailability(unit_id) == 0) {
                Audio_PlayCue(0x71);
                continue;
            }

            Audio_PlayCue(0x70);
            item_slot = Shop_SelUse(unit_id);
            if (item_slot == -1) {
                shop->cursor.anchor->kind = 4;
                shop->mode = 12;
                redraw = 1;
                continue;
            }
            *selected_unit = unit_id;
            *selected_item = item_slot;
            result = 0;
            goto done;
        }

        if ((INPUT_NEW_KEYS & 2) != 0) {
            Audio_PlayCue(0x71);
            *selected_unit = -1;
            *selected_item = -1;
            result = -1;
            goto done;
        }

        if ((INPUT_REPEAT_KEYS & 0x20) != 0) {
            Audio_PlayCue(0x6f);
            selected_index--;
            redraw = 1;
        }
        if ((INPUT_REPEAT_KEYS & 0x10) != 0) {
            Audio_PlayCue(0x6f);
            selected_index++;
            redraw = 1;
        }
    }

done:
    Menu_ReleaseEntryObjects();
    UiWindow_Close(list_window, 2);
    UiWindow_Close(shop->item_window, 2);
    WaitFrames(1);
    Inn_Cleanup();
    return result;
}

/* shop/sel/use.c */
s32 Ability_GetAvailability(s32);

s32 UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizePending(void);

extern u8 Value_00000075;

s32 Shop_SelUse(s32 actor)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    /* win2 declared ahead of win1 (and both ahead of object) to match the
     * reference's sp+8/sp+12/sp+16 spill-slot order for these three
     * call-result locals; declaring them in call order instead misassigns
     * object and win2 to each other's slots under this compiler. */
    s32 win2;
    s32 win1;
    u8 *object;
    s32 selection;
    s32 redraw;
    s32 count;
    s32 result;
    s32 status;
    s32 flags;
    s32 off;
    void *window;
    s32 x;
    s32 y;

    /* count/result/redraw assigned as statements here (not declaration
     * initializers) so the reference's constant-materialization order is
     * reproduced: a shared movs r1,#1 is committed to count's and redraw's
     * home registers (sl, r9) right before r1 is needed again for win1's
     * call args, in this exact count-then-redraw order. */
    object = Runtime_GetObject(actor);
    count = 1;
    result = 0;
    redraw = 1;
    win1 = UiWindow_CreateFar(14, 8, 16, 4, 2);
    win2 = UiWindow_CreateFar(0, 5, 30, 3, 2);
    shop->cursor.anchor->kind = 18;
    shop->mode = 12;
    selection = 0;

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            count = Ability_GetAvailability(actor);
            if (selection > count - 1)
                selection = count - 1;

            /* Split into a plain-int offset statement followed by a separate
             * pointer dereference: writing this as one expression (any
             * algebraic grouping of object+216+selection*2) always makes the
             * compiler fold object into the offset register before the load
             * (adds+adds+ldrh[reg,#0]); computing the byte offset in `off`
             * first keeps object untouched in its own register so the load
             * folds to a single reg+reg ldrh, matching the reference. */
            off = selection * 2 + 216;
            flags = *(u16 *)(object + off) & 0x1ff;
            window = (void *)shop->item_window;
            x = Modulo(selection, 5) << 4;
            y = (Math_Div(selection, 5) << 4) + 8;
            Shop_PlaceCursor(window, x, y);
            shop->mode = 3;
            Shop_DrawUseItem(win1, actor, selection);
            Shop_DrawMsg(win2, flags + (s32)&Value_00000075);
        }

        WaitFrames(1);

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            status = Sys_Apply(actor, selection);
            if (status == 0) {
                Audio_PlayCue(112);
                result = selection;
                goto exit_loop;
            } else if (status == -4) {
                UiWork_Create(0xc96, 8, 1, 2);
            } else if (result == -3) {
                UiWork_Create(0xc97, 8, 1, 2);
            }
            Audio_PlayCue(113);
            while (UiWork_IsCompleteFar() == 0)
                WaitFrames(1);
            UiWork_FinalizePending();
            continue;
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(113);
            result = -1;
            goto exit_loop;
        }

        /* selection -= 1 / += 1 as its own statement before the Modulo
         * call (matching sibling main:080b211c's idiom) so the compiler
         * commits the +-1 directly into selection's home register (r7)
         * ahead of the add; folding it into one `selection +- 1 + count`
         * expression instead subtracts/adds 1 from the sum register after
         * the add, which is a different (non-matching) instruction order. */
        if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
            Audio_PlayCue(111);
            selection -= 1;
            selection = Modulo(selection + count, count);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
            Audio_PlayCue(111);
            selection += 1;
            selection = Modulo(selection + count, count);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x40) != 0) {
            selection -= 5;
            if (selection < 0)
                selection += 15;
            while (selection >= count)
                selection -= 5;
            Audio_PlayCue(111);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x80) != 0) {
            selection += 5;
            if (selection >= count)
                selection -= 15;
            while (selection < 0)
                selection += 5;
            Audio_PlayCue(111);
            redraw = 1;
        }
    }

exit_loop:
    UiWindow_Close(win2, 2);
    UiWindow_Close(win1, 2);
    WaitFrames(1);
    return result;
}

/* shop/draw/use_item.c */
extern u8 Value_00000182;
extern u8 Value_00000c94;
extern u8 Value_00000c95;
extern u8 Value_00000c8d;

void Shop_DrawUseItem(s32 window, s32 unit_id, s32 item_id)
{
    u8 *unit = (u8 *)Runtime_GetObject(unit_id);
    s32 slot_offset = item_id * 2 + 216;
    s32 masked = *(volatile u16 *)(unit + slot_offset) & 0x1ff;
    s32 mult = (*(volatile u16 *)(unit + slot_offset) >> 11) + 1;

    if (window != 0) {
        s32 result;

        UiWindow_Commit(window);
        UiText_DrawAt(masked + (s32)&Value_00000182, window, 0, 0);

        result = Sys_Apply(unit_id, item_id);
        if (result == -4) {
            UiText_DrawAt((s32)&Value_00000c94, window, 0, 8);
        } else if (result == -3) {
            UiText_DrawAt((s32)&Value_00000c95, window, 0, 8);
        } else {
            s32 qty;
            s32 total;

            qty = Shop_SalePrice(*(u16 *)(unit + slot_offset));
            total = mult *qty;

            UiText_DrawAt((s32)&Value_00000c8d, window, 8, 8);
            UiNumber_DrawAt(total, 5, window, 40, 8);
            UiText_DrawAt((s32)&Value_00000c8d - 5, window, 80, 8);
        }
    }
}

/* battle/presentation_update_camera.c */
/* battle/presentation/cam/update.c */
struct SceneCameraState {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 *field1c;
    s32 field20;
    u8 filler24[16];
    u16 field34;
    u16 field36;
};

struct SceneCameraTransfer {
    s32 first;
    s32 second;
    s32 third;
};

struct BattlePresentationTransition {
    s32 battle_value;
    s32 timer;
    u8 reserved08[12];
    s32 flag;
};

struct LinkWork {
    u8 pad0[0x44];
    u8 enabled;
    u8 pad1[0x0b];
    u8 side;
    u8 miss;
    u8 paused;
};

#define LINK_STAT (*(u16 *)0x03001f64)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)

void Battle_Run(void)
{
    void **slot = (void **)ADDR_03001E80;
    struct SceneCameraState *state = slot[0];
    struct BattlePresentationTransition *transition = slot[32];
    struct LinkWork *work = slot[-3];
    struct SceneCameraTransfer local;
    s32 *pos;
    s16 delta;
    u32 id;

    if (work->enabled != 0) {
        if ((LINK_STAT & 3) != 3) {
            work->miss++;
            if (work->miss > 24) {
                work->paused = 1;
            }
        } else {
            id = (REG_SIOCNT << 0x1A) >> 0x1E;
            if (work->side != id) {
                work->paused = 1;
            }
            work->miss = 0;
        }
    }

    if (transition->timer != 0) {
        delta = transition->battle_value - state->field36;
        delta /= 16;
        state->field36 += delta;
        transition->timer--;
    }

    pos = &state->field0c;
    if (state->field1c != 0) {
        pos = state->field1c;
    }

    FunctionHead_080049ac();
    Battle_Do(pos);
    SceneTransform_ApplyYaw((s16)state->field36);
    SceneTransform_ApplyPitch((s16)state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
    ((void (*)(struct SceneCameraTransfer *, struct SceneCameraState *))0x03000250)(&local, state);

    if (transition->flag == 0) {
        Battle_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

/* battle/intro/announce_encounter.c */
extern u8 *gBattleWork;

void UiText_ShowMessageAndWait(s32);

void BattleIntro_AnnounceEncounter(s32 enemy_count)
{
    s16 enemies[8];
    u8 *battle_state;
    s16 *enemy;
    s32 announced;

    battle_state = gBattleWork;
    FunctionHead_08015118();
    BattleParty_ListPresentEnemies(enemies);

    announced = 0;
    if (enemy_count != 0) {
        enemy = enemies;
        do {
            UiText_DrawQuantity((u16)*enemy++, 1);
            if (announced == enemy_count - 1)
                UiText_ShowMessageAndWait((s32)&Value_00000811);
            else
                UiText_ShowMessageAndWait((s32)&Value_00000810);
            announced++;
            FunctionHead_080bb65c();
        } while (announced != enemy_count);
    }

    UiWork_FinalizeSharedSlot();
    if (battle_state[69] == BATTLE_ENCOUNTER_PARTY_FIRST) {
        FunctionHead_08015118();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000812);
        FunctionHead_080bb65c();
    } else if (battle_state[69] == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        FunctionHead_08015118();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000813);
        FunctionHead_080bb65c();
    }
}

/* battle/party/collect_unit_list.c */
extern s8 gRomPresentationUpdateCamera[];

void BattleParty_CollectUnitList(void)
{
    u16 buf[14];
    u8 *state;
    s32 count;
    s32 i;
    s32 offset;
    s32 index;
    s32 last;
    s32 kind;
    u16 *out;

    state = gBattleWork;
    count = Battle_Check(buf);
    for (i = 0; i < count; i++) {
        *(u16 *)(state + 88 + i * 2) = buf[i];
    }
    offset = count * 2 + 88;
    *(u16 *)(state + offset) = 0xFF;

    count = BattleParty_ListPresentEnemies(buf);
    kind = state[66];
    if (kind >= 0) {
        if (kind <= 1) {
            for (i = 0; i < count; i++) {
                out = (u16 *)(state + 2);
                out[50 + i] = buf[i];
            }
            goto done;
        }
    }
    for (i = 0; i < count; i++) {
        index = (gRomPresentationUpdateCamera[i] + count / 2) * 2 + 100;
        out = (u16 *)(state + 2);
        *(u16 *)((u8 *)out + index) = buf[i];
    }
done:
    out = (u16 *)(state + 2);
    last = count * 2 + 100;
    *(u16 *)((u8 *)out + last) = 0xFF;
}
