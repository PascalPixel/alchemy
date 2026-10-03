#include "TYPES.H"
#include "BATTLE_RUNTIME.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "SHOP.H"
#include "FIXED_POINT_POSITION.H"


s32 Shop_CanServe(s32, s32);
s32 Shop_ServicePrice(s32 selection, s32 variant);
s32 Shop_MsgByMode(s32 msg);

/* shop/unit/hilite.c */
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
extern struct ShopRuntime *gMenuWork;

/* shop/draw/sel_msg.c */
extern u8 MsgReviveCost;
extern u8 MsgNoHealingNeeded;
void UiWindow_Clear(s32 target);
void UiText_DrawMessageAt(s32 message, s32 target, s32 arg2, s32 arg3);
void UiWork_PushValueSlotFar(s32 message, s32 style);

void EffectSlot_SetPositionFar(struct EffectSlot *, s32, s32);
s32 BattleFx_HasReachedTargetFar(struct EffectSlot *);
void BattleFx_ClearOwnedSlotFar(struct EffectSlot *);

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32, s32, s32 *);

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
                if (Item_Get(unit->inventory[i])->flags & 1) {
                    unit->inventory[i] ^= 0x200;
                    Owner_RecalculateStatsFar(id);
                }
            }
        }
    }
}

void Shop_HiliteUnit(s32 enabled, s32 selected)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 index;
    s32 variant = shop->party_action;

    if (enabled != 0) {
        for (index = 0; index < shop->party_member_count; index++) {
            if (index == selected)
                AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 30);
            else
                AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 1);
            shop->party_member_scale[index] = 0x10000;
            if (!Shop_CanServe(shop->party_member_ids[index], variant))
                shop->party_member_scale[index] = 0xb333;
        }
    }
}

void Shop_DrawSelMsg(s32 target, s32 selection)
{
    s32 variant;
    s32 message;

    variant = gMenuWork->party_action;
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

void BattleFx_UpdateRadialMotion(struct EffectSlot *effect)
{
    struct FixedPointPosition position;
    s8 *state_pointer;
    s32 state;
    s32 result;

    state_pointer = &effect->state;
    state = *state_pointer;
    if (state == 0) {
        position.x = effect->origin_x;
        position.z = effect->origin_z;
        Vector_AddPolarOffset(0x280000, Random16(), (s32 *)&position);
        EffectSlot_SetPositionFar(effect, position.x, position.z);
        position.x = effect->origin_x;
        position.z = effect->origin_z;
        Vector_AddPolarOffset(0x40000, Random16(), (s32 *)&position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->max_speed = 0x20000;
        effect->acceleration = 0x6666;
        effect->flag42 = state;
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
