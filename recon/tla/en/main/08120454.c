/* Draft, not exact (2026-09-29, Mercury): ⚓️'s Battle_ResolveTargetAction,
 * the twin of ☀️'s 080bbb0c (games/THE BROKEN SEAL/SRC/BATTLE/
 * ACTION_RESOLVE_TARGET_ACTION.C), at 08120454 (recon/tla/raw/08120454.s;
 * ja 08120450, de 0812046c; `psynergy editions` names it as the nearest
 * start). The body is ☀️'s pre-cleanup body with the ⚓️ differences
 * recovered from the retired cross-edition hook set (e24225696^) written in
 * as code: a second actor for the combined attacks 0x138 and 0x13c, a
 * guard level 4 that blocks damage, guard 2 at two fifths, a battle rule
 * that keeps foes at 1 HP, 88 effects with the extra ⚓️ cases, five-turn
 * refrain and the per-state immunity checks after the copy.
 * Remaining before it can compile for ⚓️: ⚓️ headers for the battle types
 * (BattlePlan with actor_id2, BattleUnit, BattleAction), and names at
 * their own places for every callee the listing still calls by address
 * (BattleEv_Push 08120360, GameFlag_TestFar 080ad008, BattleRandom
 * 080ad148, Func_081203a8 and the rest), and for the message ids. The
 * helper macros and hook bodies are expanded inline here; fold them back
 * into shared code once both games compile from one source. */
#include "../../../../games/THE LOST AGE/INCLUDE/TYPES.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_ACTOR.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_CALC.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_EFX.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_EVENT.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_MSG.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_RUNTIME.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_SUMMON.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_TYPES.H"
#include "../../../../games/THE LOST AGE/INCLUDE/BATTLE_WORK.H"
#include "../../../../games/THE LOST AGE/INCLUDE/PARTY_STATE.H"
#include "../../../../games/THE LOST AGE/INCLUDE/OWNER_STATE.H"
#include "../../../../games/THE LOST AGE/INCLUDE/RAM_BUFFER.H"
#include "../../../../games/THE LOST AGE/INCLUDE/RUNTIME_MEM.H"

s32 BattleUnit_KeepsOneHp(s32 unit);
void Battle_ApplyActionExtras(s32 unit, s32 a, s32 b);
void BattleParty_InsertUnitCentered(s16 *units, s32 unit);
s32 BattleFormation_SelectRandomAvailableMember(s32 record_id);
void *GetBattleObjectSlot(s32 unit);
void BattlePresentation_SpawnActorObject(void *object, s32 unit, s32 x, s32 y);
void BattleActor_CommitPlacement(void);
s32 BattleParty_ListPresentEnemies(s16 *entries);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
struct AffinityPair {
    s16 low;
    s16 high;
};
s32 Battle_ResolveTargetAction(struct BattlePlan *plan, s32 slot)
{
    struct BattleAction *action;
    s32 action_id;
    struct BattleUnit *actor;
    s32 actor_id;
    s32 range;
    s32 bonus;
    struct BattleSession *work;
    s32 offset;
    s32 half;
    s32 adjust;
    s32 dealt;
    s32 crush;
    s32 hit;
    s32 modifier;
    s32 skip;
    s32 nibble;
    s32 affinity;
    struct BattleUnit *copy;
    s32 g1;
    s32 power;
    s16 hp0;
    s32 kind;
    s32 scale;
    s32 pass;
    s16 *cmd;
    u8 target_id;
    s8 *rm;
    struct BattleUnit *target;
    s32 n;
    s32 value;
    s32 affi;
    s32 dmg;
    s8 *am;
    s32 guard;
    s16 saved[8];
    s32 count;
    s32 tmp;
    s32 size;
    bonus = 0;
    work = Ram_HeapSlots->battle_work;
    half = 0;
    dealt = 0;
    crush = 0;
    skip = 0;
    affinity = 0;
    size = sizeof(struct BattleUnit);
    copy = (struct BattleUnit *)Runtime_BumpAllocate(size);
    { actor_id = (((action_id) == 0x138 || (action_id) == 0x13c) ? (plan)->actor_id2 : (plan)->actor_id); target_id = ((plan)->target_ids[(slot)]); action_id = plan->action_id; range = plan->range_index; adjust = ((plan)->target_adjustments[(slot)]); modifier = ((plan)->target_modifiers[(slot)]); };
    action = BattleAction_Get(action_id);
    actor = OwnerState_Get(actor_id);
    target = OwnerState_Get(target_id);
    ((void (*)(void *, const void *, s32))0x03000730)( (copy), (target), (size));
    { s32 state = *(u16 *)((u8 *)target + 0x14a); switch (state) { case 0xdd: if (((plan)->command) == 1 && ((u32)(actor_id ^ target_id) >> 7) != 0) { BattleEv_Push(11, target_id); BattleEv_Push(0, target_id); BattleEv_Push(4, 0xcb5); goto done; } break; case 0x65: case 0x66: case 0x67: if (action_id != 0x23d && ((u8 *)work)[0x868] != 0 && ((u32)(actor_id ^ target_id) >> 7) != 0) { BattleEv_Push(11, target_id); BattleEv_Push(0, target_id); BattleEv_Push(4, 0xcb4); goto done; } break; } if (target->guard_level == 4 && ((u32)(actor_id ^ target_id) >> 7) != 0) { BattleEv_Push(11, target_id); BattleEv_Push(0, target_id); BattleEv_Push(4, 0xcab); goto done; } };
    if (action->range != 255) {
        offset = ((plan)->target_offsets[(slot)]);
        if (offset < 0)
            offset = -offset;
    } else {
        offset = 0;
    }
    if (range != 4) {
        s16 *tbl;
        s32 i;
        value = (*(s16 *)((u8 *)(target) + 38 + (range) * 2 * 2));
        tbl = (s16 *)((u8 *)target + 36);
        i = 0;
        if (value >= tbl[1]) {
            s16 *p;
            p = tbl;
            do {
                i++;
                p += 2;
                if (i > 3)
                    break;
            } while (value >= p[1]);
        }
        if (i == 4)
            affinity = -1;
        i = 0;
        {
            s32 off;
            off = 2;
            if (value <= *(s16 *)(((u8 *)(tbl)) + off)) {
                do {
                    i++;
                    if (i > 3)
                        break;
                } while (value <= ((struct AffinityPair *)((u8 *)target + 36))[i].high);
            }
        }
        if (i == 4)
            affinity = 1;
    }
    if ((u32)plan->range_index <= 3) {
;
        if (((plan)->command) != 2) {
            s32 off;
            off = plan->range_index * 4 + 72;
            power = *(s16 *)((u8 *)actor + off);
            goto after_power;
        }
    } else
;
    power = 100;
after_power:
    if (((plan)->command) == 5 && (u32)plan->range_index <= 3 && affinity > 0) {
        s32 chance;
        {
            s32 off;
            off = plan->range_index * 4 + 72;
            chance = power - ((s16 *)((u8 *)target + off))[1];
        }
        chance += 30;
        chance *= 0x28f;
        if (chance > (BattleRandom16Far() & 0xffff))
            BattleEv_Push(BATTLE_EVENT_SCRIPT_UPDATE, 5);
    }
    { if (action_id == 0x2ae || action_id == 0x165) Battle_ApplyActionExtras(target_id, 0, 0); if ((u32)(action_id - 0x2d0) <= 1) Battle_ApplyActionExtras(target_id, 1, 0); if (action_id == 0x2d8) Battle_ApplyActionExtras(target_id, 1, 1); };
    nibble = action->target_flags & 15;
    {
        s32 first;
        first = ((plan)->target_results[(slot)]);
        if (first == -1)
            hit = Battle_HitCheck(
                actor_id, target_id, range, action->effect,
                HitFalloff[offset]);
        else
            hit = first;
    }
    { if ((u8)(action->effect + 206) <= 1 || action->effect == 0x56 || action->effect == 0x57) { s32 st; s32 rec; s32 state_save; s32 cursor; s32 msg; u8 efx; state_save = (s32)((u8 *)actor + 0x14a); st = *(u16 *)state_save; affi = -1; rec = Summon_FindSlot(); efx = action->effect; if (efx == EFX_STANDBY_WORK) { st = BattleFormation_SelectRandomAvailableMember(work->enemy_group); } else if (efx == 0x56) { if (*(u16 *)state_save == 0xa4) st = (BattleRandom16Far() & 3) + 0x17a; else st = 0x51; } else if (efx == 0x57) { if (((s8 *)work)[0x56b] != 0) { s32 qi; s32 state_offset; s32 charge_offset; qi = ((s8 *)work)[0x56a]; state_offset = 0x564 + qi * 2; st = *(u16 *)((u8 *)work + state_offset); charge_offset = 0x568 + qi; affi = ((u8 *)work)[charge_offset]; } else hit = 0; } if (hit != 0 && Summon_ClassValid(st) != 0 && rec >= 0) { if (affi == -1) { affi = Summon_TakeCharge(st, 1); if (affi & 0x8000) Summon_ResetCharge(st); } BattleUnit_AssignFar(rec, st, affi & 0x7fff); if (action->effect == 0x57) { s32 qi; ((s8 *)work)[0x56b]--; qi = ((s8 *)work)[0x56a] + 1; ((s8 *)work)[0x56a] = qi - (((s32)(qi + ((u32)qi >> 31)) >> 1) * 2); } if (*(u16 *)((u8 *)actor + 0x14a) == 0xa4) { BattleParty_InsertUnitCentered(work->enemy_units, rec); } else { s16 *slots; slots = (s16 *)((u8 *)work + 2); { s32 off; s32 i; s32 j; off = 100; i = 0; state_save = 0; if (*(s16 *)((u8 *)slots + off) == 254) { *(s16 *)((u8 *)slots + off) = rec; } else { s32 woff; s32 next; j = 0; woff = 100; for (;;) { affi = (s32)slots; cursor = j + 100; n = *(s16 *)(cursor + affi); if (n == 255) { *(s16 *)(affi + cursor) = rec; next = state_save + 102; *(s16 *)((u8 *)affi + next) = n; break; } i++; next = j + 2; j = next; if (i > 5) break; state_save = next; woff = next + 100; if (*(s16 *)(woff + affi) == 254) { *(s16 *)(woff + affi) = rec; break; } } } } } Summon_Refresh(); { s32 x; s32 y; cursor = (s32)GetBattleObjectSlot(rec); x = *(s32 *)(cursor + 12); if (x < 0) x += 0xffff; y = *(s32 *)(cursor + 16); x >>= 16; if (y < 0) y += 0xffff; y >>= 16; BattlePresentation_SpawnActorObject((void *)cursor, rec, x, y); } BattleActor_CommitPlacement(); { s32 listed; listed = BattleParty_ListPresentEnemies(saved); if (listed > 0) { u16 *q; q = (u16 *)saved; count = listed; do { Actor_ResetMotionAtAnchor(*q++); count--; } while (count != 0); } } BattleEv_Push(BATTLE_EVENT_UNIT, rec); if (action->effect == 0x57) { msg = 0xd64; goto tla_summon_msg; } else if (action_id != 0x1f7) { BattleEv_Push(BATTLE_EVENT_TEXT, 0xd56); } else { BattleEv_Push(BATTLE_EVENT_TEXT, 0xd54); } } else if (action_id == 0x1f7) { msg = 0xd55; tla_summon_msg: BattleEv_Push(BATTLE_EVENT_TEXT, msg); } else { BattleEv_Push(BATTLE_EVENT_TEXT, 0xd57); } } };
    if (hit != 0) {
        s32 efx;
;
        efx = action->effect;
        if (action->effect == EFX_IMMOBILIZE || action->effect == 0x53) { s32 hidx; hit = 0; hidx = 748; if (*(s16 *)(((u8 *)(work)) + hidx) == target_id) { hit = 1; } else { n = 0; tla_scan_next: n++; if ((u32)n <= 19) { s32 idx; idx = ((n << 1) << 3) + 748; if (*(s16 *)(((u8 *)(work)) + idx) == target_id) hit = 1; else goto tla_scan_next; } } } else if (action->effect == EFX_HALF_DEF) { half = 1; } else if (action->effect == EFX_LETHAL) { crush = 1; } else if (action->effect == EFX_INSTANT_DOWN) { skip = 1; } else if (action->effect == EFX_ACTOR_FLASH) { if (actor->hp != 0) BattleEv_Push(BATTLE_EVENT_ACTOR_EFFECT, actor_id); } else if (action->effect == EFX_DRAIN_PP) { if (target->pp != 0) nibble = 10; else hit = 0; } else if (action->effect == 0x5a) { half = 2; } else if (action->effect == 0x5b) { half = 2; };
    }
    if ((action->effect != 0x4b || slot == 0) && skip == 0
        && (target->hp != 0 || BattleFx_IsReviveFar(action->effect) != 0)) {
        s32 pp;
        s32 cur;
        s32 apwr;
        switch (nibble) {
        case BATTLE_DAMAGE_WEAPON:
        case BATTLE_DAMAGE_WEAPON_SCALED:
        {
            s32 def;
            if ((target->guard_level == 4))
                break;
            hp0 = target->hp; def = target->defense; scale = def;
            if (half != 0) { scale = def / 2; if (half == 2) scale = 0; };
            pass = 1;
            do {
                { if (range != 4) { s32 off; off = range * 4 + 72; bonus = power - ((s16 *)((u8 *)target + off))[1]; } };
                if (pass == 0)
                    bonus = 0;
                apwr = action->power;
                if (nibble == 4)
                    dmg = Math_Div(
                        Battle_CalcAttack((actor->attack), scale, 0,
                                          bonus)
                            * apwr,
                        10);
                else {
                    { kind = actor->attack; if (action_id == 0x138 || action_id == 0x13c) { kind = ((struct BattleUnit *)OwnerState_Get(((u8 *)plan)[0]))->attack; if (action_id == 0x138) kind += ((struct BattleUnit *)OwnerState_Get(((u8 *)plan)[2]))->attack; } };
                    dmg = Battle_CalcAttack(kind, scale, apwr,
                                            bonus);
                }
                dmg *= adjust;
                if (modifier != 0) {
                    if (modifier == 1)
                        dmg = dmg * 5 / 4;
                    else
                        dmg = dmg * 3 / 2;
                    dmg += (u8)Math_DivU(((u8 *)target)[15], 5) + 6;
                    if (pass == 0) {
                        BattleEv_Push(BATTLE_EVENT_MARK, 0);
                        {
                            s32 text;
                            text = (s32)&MsgCritical;
                            if ((u32)target_id <= 7)
                                text += 1;
                            BattleEv_Push(BATTLE_EVENT_TEXT_CONTINUE, text);
                        }
                    }
                }
                dmg += BattleRandom16Far() & 3;
                { guard = ((target)->guard_level); if (guard != 0) { if (guard == 1) dmg /= 2; else if (guard == 2) dmg = Math_Div(dmg * 2, 5); else dmg = Math_Div(dmg, 10); }; };
                if (dmg <= 0)
                    dmg = 1;
                if (crush != 0) {
                    if (dmg < hp0 - 1) {
                        dmg = hp0 - 1;
                        if (dmg <= 0)
                            dmg = 1;
                    }
                }
                if (GameFlag_TestFar(366) != 0 && ((plan)->command) == 5
                    && hp0 <= dmg) {
                    dmg = hp0 - 1;
                }
                pass++;
            } while (pass <= 1);
            { hp0 -= dmg; BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id); BattleEv_Push(BATTLE_EVENT_UNIT, target_id); BattleEv_Push(BATTLE_EVENT_VALUE, dmg); };
            {
                s32 text;
                if ((u32)target_id <= 7)
                    text = (s32)&MsgDmgEmphP + affinity;
                else
                    text = (s32)&MsgDmgEmphE + affinity;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
            }
            if (hp0 <= 0 && (BattleUnit_KeepsOneHp(target_id) != 0))
                hp0 = 1;
            if (hp0 <= 0) {
                { u16 state; cur = 0; BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id); state = *(u16 *)((u8 *)target + 0x14a); if (state != 0x171 && state != 0x175) { BattleEv_Push(BATTLE_EVENT_UNIT, target_id); if ((u32)target_id <= 7) BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgGoesDown); else BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgFelled); } };
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - hp0;
            target->hp = (s16)hp0;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }
        case BATTLE_DAMAGE_PP_DRAIN:
        {
            if ((target->guard_level == 4))
                break;
            if (action->power == 0)
                break;
            pp = target->pp;
            if (range != 4) {
                s32 off;
                off = range * 4;
                off = off + 72;
                bonus = power - ((s16 *)((u8 *)target + off))[1];
            }
            apwr = action->power;
            dmg = Battle_CalcPower(apwr, bonus, 256);
            dmg = Math_Div(dmg * PpLossFalloff[offset], 100);
            dmg *= adjust;
            { guard = ((target)->guard_level); if (guard != 0) { if (guard == 1) dmg /= 2; else if (guard == 2) dmg = Math_Div(dmg * 2, 5); else dmg = Math_Div(dmg, 10); }; };
            if (action->effect == EFX_DRAIN_PP && dmg > pp)
                dmg = pp;
            if ((dmg) > (pp)) (dmg) = (pp);
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;
                if ((u32)target_id <= 7)
                    text = (s32)&MsgPpLossP;
                else
                    text = (s32)&MsgPpLossE;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->pp - pp;
            target->pp = pp;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }
        case BATTLE_DAMAGE_HP_HEAL:
        {
            if (action->power == 0)
                break;
            cur = target->hp;
            if ((cur) == 0) break;
            apwr = action->power;
            dmg = Battle_CalcRestore(apwr, range == 4 ? 100 : power, 256);
            dmg = Math_Div(dmg * HpHealFalloff[offset], 100);
            dmg *= adjust;
            dmg += BattleRandom16Far() & 3;
            cur += dmg;
            if (cur > target->max_hp) {
                cur = target->max_hp;
                dmg = cur - target->hp;
            }
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            if (cur == target->max_hp)
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpRecover);
            }
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }
        case BATTLE_DAMAGE_PP_DIRECT:
        {
            if ((target->guard_level == 4))
                break;
            if (action->power == 0)
                break;
            pp = target->pp;
            { if (range != 4) { s32 off; off = range * 4 + 72; bonus = power - ((s16 *)((u8 *)target + off))[1]; } };
            apwr = action->power;
            dmg = Battle_CalcPower(apwr, bonus, 256);
            dmg = Math_Div(dmg * PpDmgFalloff[offset], 100);
            dmg *= adjust;
            { guard = ((target)->guard_level); if (guard != 0) { if (guard == 1) dmg /= 2; else if (guard == 2) dmg = Math_Div(dmg * 2, 5); else dmg = Math_Div(dmg, 10); }; };
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;
                if ((u32)target_id <= 7)
                    text = (s32)&MsgDmgP;
                else
                    text = (s32)&MsgDmgE;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            goto pp_store;
        }
        case BATTLE_DAMAGE_HP_ADDITIVE_6:
        case BATTLE_DAMAGE_HP_ADDITIVE_7:
        case BATTLE_DAMAGE_HP_ADDITIVE_9:
        {
            s32 round;
            if ((target->guard_level == 4))
                break;
            if (action->power == 0)
                break;
            cur = target->hp;
            round = 1;
            do {
                { if (range != 4) { s32 off; off = range * 4 + 72; bonus = power - ((s16 *)((u8 *)target + off))[1]; } };
                if (round == 0)
                    bonus = 0;
                apwr = action->power;
                if (action_id == 0x2ab || action_id == 0x2a1 || action_id == 0x2d4 || ((plan)->command) == 6 || ((plan)->command) == 10) { s32 rate; rate = 0; switch (action_id) { default: case 0x189: break; case 0x196: case 0x184: case 0x18c: case 0x17c: rate = 3; break; case 0x199: case 0x191: case 0x18f: case 0x187: case 0x17f: rate = 12; break; case 0x2ab: rate = 35; break; case 0x197: case 0x190: case 0x18d: case 0x185: case 0x180: case 0x17d: rate = 6; break; case 0x18e: case 0x188: case 0x186: case 0x19a: case 0x198: case 0x17e: rate = 9; break; case 0x192: rate = 7; break; case 0x19b: case 0x193: case 0x181: rate = 15; break; case 0x18a: rate = 21; break; case 0x19c: rate = 24; break; case 0x2a1: case 0x182: rate = 30; break; case 0x2d4: case 0x194: rate = 40; break; } value = target->max_hp; if (value > 10000) value = 10000; (apwr) += Math_Div(value * rate, 100); };
                dmg = Battle_CalcPower(apwr, bonus, 256);
                dmg *= adjust;
                switch (nibble & 15) {
                case 5:
                    dmg = Math_Div(HpDmgFalloff5[offset] * dmg, 100);
                    break;
                case 8:
                    dmg = Math_Div(dmg * HpDmgFalloff8[offset], 100);
                    break;
                case 6:
                    dmg = Math_Div(dmg * HpDmgFalloff6[offset], 100);
                    break;
                }
                dmg += BattleRandom16Far() & 3;
                { guard = ((target)->guard_level); if (guard != 0) { if (guard == 1) dmg /= 2; else if (guard == 2) dmg = Math_Div(dmg * 2, 5); else dmg = Math_Div(dmg, 10); }; };
                if (((gPartyState.battle_rule_24b == 6 || (GameFlag_TestFar(366) != 0 && ((plan)->command) == 6)) && (cur) > (dmg))) {
                    dmg = cur;
                }
                round++;
            } while (round <= 1);
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;
                if ((u32)target_id <= 7)
                    text = (s32)&MsgDmgEmphP + affinity;
                else
                    text = (s32)&MsgDmgEmphE + affinity;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }
            if (cur <= 0 && (BattleUnit_KeepsOneHp(target_id) != 0))
                cur = 1;
            if (gPartyState.battle_rule_24b == 6) (cur) = 0;
            if (cur <= 0) {
                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                {
                    s32 text;
                    cur = 0;
                    if ((u32)target_id <= 7)
                        text = (s32)&MsgGoesDown;
                    else
                        text = (s32)&MsgFelled;
                    BattleEv_Push(BATTLE_EVENT_TEXT, text);
                }
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }
        case BATTLE_DAMAGE_PP_HEAL:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            apwr = action->power;
            dmg = Battle_CalcRestore(apwr, range == 4 ? 100 : power, 256);
            dmg = Math_Div(dmg * PpHealFalloff[offset], 100);
            dmg *= adjust;
            pp += dmg;
            if (pp > target->max_pp) {
                pp = target->max_pp;
                dmg = pp - target->pp;
            }
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            if (pp == target->max_pp)
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpRecover);
            }
pp_store:
            target->pp = (s16)pp;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }
        case 0:
        case 7:
        case 9:
            break;
        case BATTLE_DAMAGE_HP_POWER:
            if ((target->guard_level == 4))
                break;
            if (hit != 0) {
            if (action->power == 0)
                break;
            cur = target->hp;
            { if (range != 4) { s32 off; off = range * 4 + 72; bonus = power - ((s16 *)((u8 *)target + off))[1]; } };
            apwr = action->power;
            dmg = Battle_CalcPower(apwr, bonus, 256);
            dmg *= adjust;
            dmg = Math_Div(dmg * HpDmgFalloff[offset], 100);
            { guard = ((target)->guard_level); if (guard != 0) { if (guard == 1) dmg /= 2; else if (guard == 2) dmg = Math_Div(dmg * 2, 5); else dmg = Math_Div(dmg, 10); }; };
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;
                if ((u32)target_id <= 7)
                    text = (s32)&MsgDmgP;
                else
                    text = (s32)&MsgDmgE;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }
            if (cur <= 0 && (BattleUnit_KeepsOneHp(target_id) != 0))
                cur = 1;
            if (cur <= 0) {
                { u16 state; cur = 0; BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id); state = *(u16 *)((u8 *)target + 0x14a); if (state != 0x171 && state != 0x175) { BattleEv_Push(BATTLE_EVENT_UNIT, target_id); if ((u32)target_id <= 7) BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgGoesDown); else BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgFelled); } };
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
dealt = target->hp - cur;
            target->hp = (s16)cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
            }
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgNoEffect);
            break;
        }
    }
    { if (((u8 *)work)[0x868] != 0 && action_id == 0x23d) { BattleEv_Push(BATTLE_EVENT_UNIT, target_id); BattleEv_Push(BATTLE_EVENT_TEXT, 0xca7); ((u8 *)work)[0x868] = 0; } };
    BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
    if (!((action->effect != 0x4b || slot != 0) && (BattleFx_IsReviveFar(action->effect) != 0 || target->hp != 0 || BattleFx_CanAffectDefeatedUnit(action->effect) != 0) && hit != 0)) goto done;
    if ((u32)(action->effect - 3) > 88 - 3)
        goto done;
    switch (action->effect) {
    case EFX_CURE_ALL:
        if (target->delusion != 0) {
            target->delusion = 0;
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureDelusion);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->stun != 0) {
            target->stun = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureStun);
        }
        { if (target->sleep != 0) { target->sleep = 0; BattleEv_Push(BATTLE_EVENT_TEXT, 0xce3); BattleEv_Push(BATTLE_EVENT_UNIT, target_id); } };
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureSeal);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->death_count != 0) {
            target->death_count = 0;
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureCurse);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->evil_spirit != 0) {
            target->evil_spirit = 0;
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureSpirit);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->poison != 0) {
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCurePoison);
            target->poison = 0;
        }
        BattleEv_Push(BATTLE_EVENT_RESET, 0);
        break;
    case EFX_CURE_PART:
        if (target->delusion != 0) {
            target->delusion = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureDelusion);
        }
        if (target->stun != 0) {
            target->stun = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureStun);
        }
        { if (target->sleep != 0) { target->sleep = 0; BattleEv_Push(BATTLE_EVENT_RESET, 0); BattleEv_Push(BATTLE_EVENT_UNIT, target_id); BattleEv_Push(BATTLE_EVENT_TEXT, 0xce3); } };
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureSeal);
        }
        if (target->death_count == 0)
            break;
        target->death_count = 0;
        BattleEv_Push(BATTLE_EVENT_RESET, 0);
        BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureCurse);
        break;
    case EFX_HEAL_60:
    case EFX_HEAL_30:
    case 0x46: case 0x47: case 0x4c:
    {
        s32 old;
        s32 maxu;
        s32 maxv;
        s32 heal;
        u16 *stat_ptr;
        { (stat_ptr) = (u16 *)&target->hp; (old) = *(stat_ptr)--; (heal) = *(s16 *)((stat_ptr) + 1); if (action->effect == 0x4c) { (maxv) = *(s16 *)&target->max_hp; (maxu) = *(u16 *)&target->max_hp; (heal) += Math_Div((maxv) * 2, 5); } else if (action->effect == 0x47) { (maxv) = *(s16 *)&target->max_hp; (maxu) = *(u16 *)&target->max_hp; (heal) += Math_Div((maxv) * 7, 10); } else if (action->effect == 0x46) { (maxu) = *(u16 *)&target->max_hp; (heal) += (s16)(maxu) / 2; } else { (stat_ptr)--; (maxu) = *(stat_ptr)--; (maxv) = *(s16 *)(stat_ptr + 1); if (action->effect == EFX_HEAL_60) (heal) += Math_Div((maxv) * 60, 100); else (heal) += Math_Div((maxv) * 30, 100); } };
        if (heal > (s16)maxu)
            heal = (s16)maxu;
        tmp = heal - (s16)old;
        if (tmp == 0 && nibble != 1)
            break;
        if (heal == (s16)maxu)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpFull);
        else {
            BattleEv_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpRecover);
        }
        target->hp = (s16)heal;
        Owner_RecalculateRatiosFar(target_id);
        break;
    }
    case EFX_PP_RESTORE_7:
    case 0x4d: case 0x4e:
    {
        { s32 heal; s32 old; s32 maxu; s32 maxv; s32 effect; effect = action->effect; heal = target->pp; old = *(u16 *)&target->pp; if (effect == 0x4d) { maxv = *(s16 *)((u8 *)target + 54); maxu = *(u16 *)((u8 *)target + 54); heal += (s16)Math_Div(maxv, 10); } else if (effect == 0x4e) { maxv = target->max_pp; maxu = *(u16 *)&target->max_pp; heal += Math_Div(maxv * 3, 10); } else { maxv = target->max_pp; maxu = *(u16 *)&target->max_pp; heal += Math_Div(maxv * 7, 100); } if (heal > (s16)maxu) heal = (s16)maxu; tmp = heal - (s16)old; if (tmp == 0 && nibble != 11) break; if (heal == (s16)maxu) BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpFull); else { BattleEv_Push(BATTLE_EVENT_VALUE, tmp); BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpRecover); } target->pp = (s16)heal; Owner_RecalculateRatiosFar(target_id); break; };
    }
    case EFX_AGI_SET_UP8:
        (*(s8 *)&(target->agility_modifier)) = 8;
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->agility - copy->agility);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAgiUp);
        break;
    case EFX_AGI_SET_DOWN4:
        am = &(*(s8 *)&(target->agility_modifier));
    {
        u8 v;
        v = -4;
        *am = v;
    }
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->agility - target->agility);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAgiDown);
        break;
    case EFX_ATK_DOWN1:
        target->attack_modifier += -1;
        { if ((target->attack_modifier) < -4) (target->attack_modifier) = -4; if ((target->attack_modifier) > 4) (target->attack_modifier) = 4; };
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->attack - target->attack);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAtkDown);
        target->attack_modifier_turns = 7;
        break;
    case EFX_ATK_DOWN2:
        target->attack_modifier += -2;
        { if ((target->attack_modifier) < -4) (target->attack_modifier) = -4; if ((target->attack_modifier) > 4) (target->attack_modifier) = 4; };
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->attack - target->attack);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAtkDown);
        target->attack_modifier_turns = 7;
        break;
    case EFX_ATK_UP1:
        target->attack_modifier += 1;
        { if ((target->attack_modifier) < -4) (target->attack_modifier) = -4; if ((target->attack_modifier) > 4) (target->attack_modifier) = 4; };
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->attack - copy->attack);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAtkUp);
        target->attack_modifier_turns = 7;
        break;
    case EFX_ATK_UP2:
        target->attack_modifier += 2;
        { if ((target->attack_modifier) < -4) (target->attack_modifier) = -4; if ((target->attack_modifier) > 4) (target->attack_modifier) = 4; };
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->attack - copy->attack);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAtkUp);
        target->attack_modifier_turns = 7;
        break;
    case EFX_DEF_DOWN1:
        target->defense_modifier += -1;
        { if ((target->defense_modifier) < -4) (target->defense_modifier) = -4; if ((target->defense_modifier) > 4) (target->defense_modifier) = 4; };
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->defense - target->defense);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDefDown);
        target->defense_modifier_turns = 7;
        break;
    case EFX_DEF_DOWN2:
        target->defense_modifier += -2;
        { if ((target->defense_modifier) < -4) (target->defense_modifier) = -4; if ((target->defense_modifier) > 4) (target->defense_modifier) = 4; };
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->defense - target->defense);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDefDown);
        target->defense_modifier_turns = 7;
        break;
    case EFX_DEF_UP1:
        target->defense_modifier += 1;
        { if ((target->defense_modifier) < -4) (target->defense_modifier) = -4; if ((target->defense_modifier) > 4) (target->defense_modifier) = 4; };
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->defense - copy->defense);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDefUp);
        target->defense_modifier_turns = 7;
        break;
    case EFX_DEF_UP2:
        target->defense_modifier += 2;
        { if ((target->defense_modifier) < -4) (target->defense_modifier) = -4; if ((target->defense_modifier) > 4) (target->defense_modifier) = 4; };
        BattleUnit_Recalculate(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->defense - copy->defense);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDefUp);
        target->defense_modifier_turns = 7;
        break;
    case EFX_REVIVE_FULL:
        if (target->hp != 0)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgRevived);
        target->hp = target->max_hp;
        Owner_RecalculateRatiosFar(target_id);
        break;
    case EFX_REVIVE_HALF:
        if (target->hp != 0)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgRevived);
        target->hp = (s16)(target->max_hp / 2);
        Owner_RecalculateRatiosFar(target_id);
        break;
    case EFX_REVIVE_80:
        if (target->hp != 0)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgRevived);
        target->hp = (s16)Math_Div(target->max_hp * 8, 10);
        Owner_RecalculateRatiosFar(target_id);
        break;
    case 0x49: if (target->hp != 0) break; BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgRevived); target->hp = (s16)Math_Div(target->max_hp * 6, 10); Owner_RecalculateRatiosFar(target_id); break;
    case EFX_CURE_POISON:
        if (target->poison != 0)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCurePoison);
        target->poison = 0;
        break;
    case EFX_RES_DOWN1:
        { target->res_modifier += (-1); { if ((target->res_modifier) < -4) (target->res_modifier) = -4; if ((target->res_modifier) > 4) (target->res_modifier) = 4; }; BattleEv_Push(BATTLE_EVENT_VALUE, ((copy->res_modifier - target->res_modifier) * 20)); BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgResDown)); target->res_modifier_turns = 7; };
        break;
    case EFX_RES_DOWN2:
    {
        target->res_modifier += -2;
        if (-4 > target->res_modifier)
            target->res_modifier = -4;
        if (target->res_modifier > 4)
            target->res_modifier = 4;
        BattleEv_Push(BATTLE_EVENT_VALUE, (copy->res_modifier - target->res_modifier) * 20);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgResDown);
        target->res_modifier_turns = 7;
    }
        break;
    case EFX_RES_UP1:
        { target->res_modifier += (1); { if ((target->res_modifier) < -4) (target->res_modifier) = -4; if ((target->res_modifier) > 4) (target->res_modifier) = 4; }; BattleEv_Push(BATTLE_EVENT_VALUE, ((target->res_modifier - copy->res_modifier) * 20)); BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgResUp)); target->res_modifier_turns = 7; };
        break;
    case EFX_RES_UP2:
    {
        target->res_modifier += 2;
        if (*(rm = &(*(s8 *)&(target->res_modifier))) < -4)
            target->res_modifier = -4;
        if (target->res_modifier > 4)
            target->res_modifier = 4;
        BattleEv_Push(BATTLE_EVENT_VALUE, (target->res_modifier - copy->res_modifier) * 20);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgResUp);
        target->res_modifier_turns = 7;
    }
        break;
    case EFX_POISON:
        if (target->poison != 0)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPoisoned);
        target->poison = 1;
        break;
    case EFX_VENOM:
        if (target->poison > 1)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgVenom);
        target->poison = 2;
        break;
    case EFX_DELUSION:
    case 0x4b:
        { BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgDelusion)); (target->delusion) = (7); };
        break;
    case EFX_CONFUSE:
        { BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgConfused)); (target->confusion) = (7); };
        break;
    case EFX_CHARM:
        { BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgCharmed)); (target->charm) = (7); };
        break;
    case EFX_STUN:
    case 0x55:
        { BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgStunned)); (target->stun) = (7); };
        break;
    case EFX_SLEEP:
        { BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgAsleep)); (target->sleep) = (7); };
        break;
    case EFX_PSY_BLOCK:
        { if ((u32)target_id <= 7) BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgPsyBlock)); else BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgPsySeal)); };
        target->psy_seal |= 7;
        break;
    case EFX_PSY_SEAL:
        { if ((u32)target_id <= 7) BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgPsyBlock)); else BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgPsySeal)); };
        target->psy_seal |= 16;
        break;
    case EFX_INSTANT_DOWN:
        if ((BattleUnit_KeepsOneHp(target_id) != 0))
            break;
        BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
        if (target->status_12a == 2)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgKoDown);
        else if (action_id == 219)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgSuffocate);
        else
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgSpiritDrain);
        target->hp = 0;
        Owner_RecalculateRatiosFar(target_id);
        break;
    case EFX_REFRAIN:
        { BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgHpRegen)); (target->refrain) = (5); };
        break;
    case EFX_REFLECT:
        { BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgReflect)); (target->reflect) = (7); };
        break;
    case EFX_DRAIN_HP:
    case EFX_DRAIN_HP_HALF:
    {
        { s32 heal; s32 amt; heal = actor->hp; if (action->effect == EFX_DRAIN_HP_HALF) dealt /= 2; amt = dealt; dmg = amt; heal += amt; if (heal > actor->max_hp) { heal = actor->max_hp; amt = heal - actor->hp; } BattleEv_Push(BATTLE_EVENT_RESET, 0); BattleEv_Push(BATTLE_EVENT_UNIT, actor_id); if (heal == actor->max_hp) BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpFull); else { BattleEv_Push(BATTLE_EVENT_VALUE, amt); BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpRecover); } actor->hp = (s16)heal; Owner_RecalculateRatiosFar(actor_id); };
        break;
    }
    case EFX_DRAIN_PP:
    {
        s32 heal;
        s32 amt;
        heal = actor->pp;
        amt = dealt;
        heal += amt;
        if (heal > actor->max_pp) {
            heal = actor->max_pp;
            amt = heal - actor->pp;
        }
        BattleEv_Push(BATTLE_EVENT_RESET, 0);
        BattleEv_Push(BATTLE_EVENT_UNIT, actor_id);
        if (heal == actor->max_pp)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpFull);
        else {
            BattleEv_Push(BATTLE_EVENT_VALUE, amt);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpRecover);
        }
        actor->pp = (s16)heal;
        Owner_RecalculateRatiosFar(actor_id);
        break;
    }
    case EFX_PP_LEECH:
        dmg = Math_Div(dealt, 10);
        if (target->pp < dmg)
            dmg = target->pp;
        if (actor->pp + dmg > actor->max_pp)
            dmg = actor->max_pp - actor->pp;
        if (dmg == 0)
            break;
        BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
        { if ((u32)target_id <= 7) BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgLeechTake)); else BattleEv_Push(BATTLE_EVENT_TEXT, ((s32)&MsgLeechGain)); };
        Owner_AdjustSecondValueFar(actor_id, dmg);
        break;
    case 0x54: dmg = (s16)Math_Div(target->max_pp, 10); if (target->pp < dmg) dmg = target->pp; if (dmg == 0) break; BattleEv_Push(BATTLE_EVENT_VALUE, dmg); if ((u32)target_id <= 7) BattleEv_Push(BATTLE_EVENT_TEXT, 0xcbb); else BattleEv_Push(BATTLE_EVENT_TEXT, 0xcba); Owner_AdjustSecondValueFar(target_id, -dmg); break;
    case EFX_BUFF_CLEAR:
        if (target->attack_modifier > 0) {
            target->attack_modifier = 0;
            target->attack_modifier_turns = 0;
        }
        if (target->defense_modifier > 0) {
            target->defense_modifier = 0;
            target->defense_modifier_turns = 0;
        }
        if (target->res_modifier > 0) {
            target->res_modifier = 0;
            target->res_modifier_turns = 0;
        }
        if (target->agility_modifier > 0)
            target->agility_modifier = 0;
        target->status_12c = 0;
        target->status_12d = 0;
        target->status_12e = 0;
        target->status_12f = 0;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgBuffsReset);
        break;
    case EFX_EVIL_SPIRIT:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgEvilSpirit);
        target->evil_spirit = 1;
        break;
    case EFX_DEATH_CURSE:
    case 0x50:
        if (target->death_count == 0) {
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDeathCurse);
            target->death_count = 7;
            break;
        }
        if (target->death_count <= 1)
            break;
        target->death_count -= 1;
        BattleEv_Push(BATTLE_EVENT_VALUE, target->death_count);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDeathCount);
        break;
    case EFX_READY:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgReadies);
        ((u8 *)target)[0x144] = 2;
        break;
    case EFX_CHALLENGE:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgChallenge);
        target->battle_end_state = 1;
        if ((u32)target_id <= 7)
            ((u8 *)work)[67] |= 2;
        break;
    case EFX_IMMOBILIZE:
    case 0x53:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgImmobile);
        target->cannot_move = 1;
        break;
    case EFX_GUARD1:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAura);
        if (((target)->guard_level) > 0)
            break;
        g1 = 1;
        (*(s8 *)&(target->guard_level)) = g1;
        break;
    case 0x48: BattleEv_Push(BATTLE_EVENT_TEXT, 0xcdd); if ((u32)target->guard_level > 1) break; target->guard_level = 2; break;
    case EFX_GUARD2:
    case 0x58:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAura2);
        if (((target)->guard_level) > 2)
            break;
        (*(s8 *)&(target->guard_level)) = 3;
        break;
    case 0x4f: BattleEv_Push(BATTLE_EVENT_TEXT, 0xcdf); if ((u32)target->guard_level > 3) break; target->guard_level = 4; break;
    case EFX_TEXT_NONE:
        BattleEv_Push(BATTLE_EVENT_TEXT, (u32)-1);
        break;
    case 0x51: BattleEv_Push(BATTLE_EVENT_TEXT, 0xc8f); ((u8 *)work)[0x47] = 1; break; case 0x52: BattleEv_Push(BATTLE_EVENT_TEXT, 0xca6); BattleEv_Push(15, target_id); break; case 0x4a: BattleEv_Push(BATTLE_EVENT_TEXT, 0xce0); ((u8 *)target)[0x143] = 1; break;
    default:
        break;
    }
done:
    BattleEv_Push(BATTLE_EVENT_RESET, 0);
    { if (((plan)->command) != 9 && *(s8 *)((u8 *)target + 0x143) != 0 && action->target_mode == 1 && target->hp != 0) { *(s32 *)((u8 *)work + 0x858) = target_id; *(s32 *)((u8 *)work + 0x85c) = actor_id; } };
    if (target->hp != 0) {
        if (target->sleep != 0)
        if (target->sleep <= 6
            && dealt > 0 && (3 & BattleRandom16Far()) == 0) {
            target->sleep = 0;
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgWakes);
        }
    }
    Sys_Free(copy);
    BattleUnit_Recalculate(target_id);
    UiWindow_DrawPartyStatusContentsFar(Ram_HeapSlots->battle_work->party_status_mode);
    if (target->hp != 0)
        BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
    if ((action_id != 0x193 && actor->evil_spirit != 0)
        && (BattleRandom16Far() & 3) == 0 && dealt > 0) {
        s32 share;
        share = dealt >> 2;
        if (share == 0)
            share = 1;
        ((plan)->pending_amount_60) += share;
    }
}
