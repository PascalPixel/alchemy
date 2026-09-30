#include "TYPES.H"
#include "BATTLE_RUNTIME.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"

u8 *Item_Get(u16);

s32 Shop_CanServe(s32, s32);
s32 Shop_ServicePrice(s32 selection, s32 variant);
s32 Shop_MsgByMode(s32 msg);

/* shop/unit/hilite.c */
void AnimationObjects_SelectAnimationFar(void *, s32);
extern u8 *gMenuWork;

/* shop/draw/sel_msg.c */
extern u8 MsgReviveCost;
extern u8 MsgNoHealingNeeded;
void UiWindow_Clear(s32 target);
void UiText_DrawMessageAt(s32 message, s32 target, s32 arg2, s32 arg3);
void UiWork_PushValueSlotFar(s32 message, s32 style);

void EffectSlot_SetPositionFar(struct Effect_080b2f4c *, s32, s32);
s32 BattleFx_HasReachedTargetFar(struct Effect_080b2f4c *);
void BattleFx_ClearOwnedSlotFar(struct Effect_080b2f4c *);

struct Position {
    s32 x;
    s32 y;
    s32 z;
};

struct Effect_080b2f4c {
    u8 filler_00[0xC];
    s32 x;
    s32 z;
    s32 source_x;
    s32 source_z;
    s32 filler_1C;
    s32 velocity;
    s32 acceleration;
    u8 filler_28[0x18];
    s8 state;
    u8 filler_41;
    s8 flag;
};

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32, s32, struct Position *);

void BattleUnit_ResetStateByMode(s32 id, s32 mode)
{
    struct BattleUnit *unit;

    unit = Owner_GetStateFar(id);
    if (mode == 0) {
        unit->hp = unit->max_hp;
        Owner_RecalculateRatiosFar(id);
    } else if (mode == 1) {
        unit->poison = 0;
    } else if (mode == 2) {
        unit->evil_spirit = 0;
    } else if (mode == 3) {
        s32 i;

        for (i = 0; i < 15; i++) {
            if (unit->inventory[i] & 0x200) {
                if (Item_Get(unit->inventory[i])[3] & 1) {
                    unit->inventory[i] ^= 0x200;
                    Owner_RecalculateStatsFar(id);
                }
            }
        }
    }
}

void Shop_HiliteUnit(s32 enabled, s32 selected)
{
    u8 *state;
    u8 *half_base;
    s32 *item;
    s32 index;
    s32 offset;
    s32 variant;
    s16 id;

    state = gMenuWork;
    variant = *(s8 *)(state + 0x3aa);
    if (enabled != 0) {
        index = 0;
        if (index < *(s8 *)(state + 0x3a7)) {
            half_base = state + 2;
            offset = 0x36c;
            item = (s32 *)(state + 0x114);
            do {
                if (index == selected)
                    AnimationObjects_SelectAnimationFar((void *)*item, 30);
                else
                    AnimationObjects_SelectAnimationFar((void *)*item, 1);
                item[16] = 0x10000;
                id = *(s16 *)(half_base + offset);
                if (Shop_CanServe(id, variant) == 0)
                    item[16] = 0xb333;
                index++;
                offset += 2;
                item++;
            } while (index < *(s8 *)(state + 0x3a7));
        }
    }
}

void Shop_DrawSelMsg(s32 target, s32 selection)
{
    s32 variant;
    s32 message;

    variant = (s8)gMenuWork[0x3AA];
    message = Shop_ServicePrice(selection, variant);
    if (target != 0) {
        UiWindow_Clear(target);
        if (Shop_CanServe(selection, variant) != 0) {
            variant = (s32)&MsgReviveCost;
        } else {
            variant = (s32)&MsgNoHealingNeeded;
        }
        variant = Shop_MsgByMode(variant);
        UiWork_PushValueSlotFar(message, 5);
        UiText_DrawMessageAt(variant, target, 0, 0);
    }
}

void BattleFx_UpdateRadialMotion(struct Effect_080b2f4c *effect)
{
    struct Position position;
    s8 *state_pointer;
    s32 state;
    s32 result;

    state_pointer = &effect->state;
    state = *state_pointer;
    if (state == 0) {
        position.x = effect->source_x;
        position.z = effect->source_z;
        Vector_AddPolarOffset(0x280000, Random16(), &position);
        EffectSlot_SetPositionFar(effect, position.x, position.z);
        position.x = effect->source_x;
        position.z = effect->source_z;
        Vector_AddPolarOffset(0x40000, Random16(), &position);
        effect->x = position.x;
        effect->z = position.z;
        effect->velocity = 0x20000;
        effect->acceleration = 0x6666;
        effect->flag = state;
        *state_pointer = (u8)*state_pointer + 1;
    } else if (state == 1) {
        result = BattleFx_HasReachedTargetFar(effect);
        if (result == 0)
            *state_pointer = result;
    } else if (state == 2) {
        if (BattleFx_HasReachedTargetFar(effect) == 0)
            BattleFx_ClearOwnedSlotFar(effect);
    }
}
