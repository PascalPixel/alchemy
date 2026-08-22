#ifndef GS2_BATTLE_DELTA_H
#define GS2_BATTLE_DELTA_H

#define BATTLE_SECOND_ACTOR

struct BattleWorkPage {
    unsigned char unused_00[0x24];
    void *work;
};

#define BATTLE_WORK (((struct BattleWorkPage *)0x03000000)->work)
#define BATTLE_RANGE_EARLY
#define BATTLE_OFFSET_LATE
#define BATTLE_ACTION_ID_EARLY
#define BATTLE_NARROW_TARGET_ID
#define BATTLE_AFFINITY_CARRIER
#define BATTLE_SURVIVES_KO() (Func_081203a8(target_id) != 0)
#define BATTLE_KO_TAIL()                                                   \
    {                                                                      \
        u16 state;                                                        \
                                                                           \
        cur = 0;                                                          \
        BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);          \
        state = *(u16 *)((u8 *)target + 0x14a);                            \
        if (state != 0x171 && state != 0x175) {                            \
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                \
            if ((u32)target_id <= 7)                                       \
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_GOES_DOWN);        \
            else                                                           \
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_FELLED);           \
        }                                                                  \
    }
#define BATTLE_DAMAGE_KO_CONDITION(cur, dmg)                                \
    ((*(u8 *)0x0200048b == 6                                               \
      || (BattleFlag_Test(366) != 0 && *cmd == 6))                         \
     && (cur) > (dmg))
#define BATTLE_AFTER_DAMAGE(cur)                                           \
    if (*(u8 *)0x0200048b == 6)                                           \
        (cur) = 0
#define BATTLE_SCALE_DEFENSE()                                            \
    if (half != 0) {                                                      \
        scale = (u32)scale >> 1;                                          \
        if (half == 2)                                                    \
            scale = 0;                                                    \
    }
#define BATTLE_ATTACK_STAT kind
#define BATTLE_ATTACK_DIRECT_STAT (actor->attack)
#define BATTLE_ATTACK_STACK_ORDER
#define BATTLE_ATTACK_LOADS()                                                \
    hp0 = target->hp;                                                        \
    scale = target->defense
#define BATTLE_ATTACK_HP hp0
#define BATTLE_ATTACK_REPORT()                                               \
    {                                                                         \
        BATTLE_ATTACK_HP -= dmg;                                              \
        BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);                \
        BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                       \
        BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);                            \
    }
#define BATTLE_PP_DAMAGE_LIMIT(dmg, pp)                                    \
    if ((dmg) > (pp))                                                      \
        (dmg) = (pp)
#define BATTLE_DAMAGE_ROUND_DECL s32 round
#define BATTLE_DAMAGE_ROUND round
#define BATTLE_HP_HEAL_GATE(cur)                                           \
    if ((cur) == 0)                                                       \
        break
#define BATTLE_ATTACK_PREP()                                              \
    {                                                                     \
        kind = actor->attack;                                             \
        if (action_id == 0x138 || action_id == 0x13c) {                  \
            kind = BattleUnit_Get(((u8 *)plan)[0])->attack;              \
            if (action_id == 0x138)                                      \
                kind += BattleUnit_Get(((u8 *)plan)[2])->attack;         \
        }                                                                 \
    }
#define BATTLE_DAMAGE_BLOCKED() (target->guard_level == 4)
#define BATTLE_GUARD_VALUE(target) ((target)->guard_level)
#define BATTLE_EVIL_SPIRIT_ACTIVE()                                        \
    (action_id != 0x193 && actor->evil_spirit != 0)
#define BATTLE_DAMAGE_GATE() (action->effect != 0x4b || slot == 0)
#define BATTLE_POWER_BONUS(dmg)                                             \
    if (action_id == 0x2ab || action_id == 0x2a1 || action_id == 0x2d4      \
        || *cmd == 6 || *cmd == 10) {                                      \
        s32 rate;                                                          \
                                                                             \
        rate = 0;                                                          \
        switch (action_id) {                                               \
        default:                                                           \
            break;                                                         \
        case 0x196:                                                        \
        case 0x184:                                                        \
        case 0x18c:                                                        \
        case 0x17c:                                                        \
            rate = 3;                                                      \
            break;                                                         \
        case 0x199:                                                        \
        case 0x187:                                                        \
        case 0x17f:                                                        \
            rate = 12;                                                     \
            break;                                                         \
        case 0x2ab:                                                        \
            rate = 35;                                                     \
            break;                                                         \
        case 0x197:                                                        \
        case 0x190:                                                        \
        case 0x18d:                                                        \
        case 0x185:                                                        \
        case 0x17d:                                                        \
            rate = 6;                                                      \
            break;                                                         \
        case 0x18e:                                                        \
        case 0x188:                                                        \
        case 0x186:                                                        \
            rate = 9;                                                      \
            break;                                                         \
        case 0x192:                                                        \
            rate = 7;                                                      \
            break;                                                         \
        case 0x19b:                                                        \
        case 0x193:                                                        \
        case 0x181:                                                        \
            rate = 15;                                                     \
            break;                                                         \
        case 0x18a:                                                        \
            rate = 21;                                                     \
            break;                                                         \
        case 0x19c:                                                        \
            rate = 24;                                                     \
            break;                                                         \
        case 0x2a1:                                                        \
        case 0x182:                                                        \
            rate = 30;                                                     \
            break;                                                         \
        case 0x2d4:                                                        \
        case 0x194:                                                        \
            rate = 40;                                                     \
            break;                                                         \
        }                                                                  \
        value = target->max_hp;                                            \
        if (value > 10000)                                                 \
            value = 10000;                                                 \
        (dmg) += Math_Div(value * rate, 100);                              \
    }

#define BATTLE_GUARD_DAMAGE()                                                \
    if (guard != 0) {                                                       \
        if (guard == 1)                                                     \
            dmg /= 2;                                                       \
        else if (guard == 2)                                                \
            dmg = Math_Div(dmg * 2, 5);                                    \
        else                                                                \
            dmg = Math_Div(dmg, 10);                                       \
    }

/* GS2 keeps the plan size but inserts a second actor byte before the target
 * arrays and moves the command halfword from 0x48 to 0x4a. */
#define BATTLE_PLAN_ACTOR_ID(plan, action_id)                                \
    (((action_id) == 0x138 || (action_id) == 0x13c)                         \
         ? (plan)->actor_id2                                                  \
         : (plan)->actor_id)
#define BATTLE_PLAN_TARGET_ID(plan, slot) ((plan)->target_ids[(slot)])
#define BATTLE_PLAN_OFFSET(plan, slot) ((plan)->target_offsets[(slot)])
#define BATTLE_PLAN_ADJUST(plan, slot) ((plan)->target_adjustments[(slot)])
#define BATTLE_PLAN_MODIFIER(plan, slot) ((plan)->target_modifiers[(slot)])
#define BATTLE_PLAN_RESULT(plan, slot) ((plan)->target_results[(slot)])
#define BATTLE_PLAN_COMMAND(plan) ((plan)->command)
#define BATTLE_PLAN_PENDING(plan) ((plan)->pending_amount_60)
#define BATTLE_SET_COMMAND()

/* GS2 reserves element zero of each target array for its second actor. */
#define BATTLE_PLAN_LOADS()                                                  \
    {                                                                        \
        actor_id = BATTLE_PLAN_ACTOR_ID(plan, action_id);                    \
        range = plan->range_index;                                           \
        target_id = BATTLE_PLAN_TARGET_ID(plan, slot);                       \
        adjust = BATTLE_PLAN_ADJUST(plan, slot);                             \
        action_id = plan->action_id;                                         \
        modifier = BATTLE_PLAN_MODIFIER(plan, slot);                         \
    }

#define BATTLE_CURE_SLEEP_ALL()                                             \
    {                                                                        \
        if (target->sleep != 0) {                                            \
            target->sleep = 0;                                               \
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0xce3);                     \
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                  \
        }                                                                    \
    }
#define BATTLE_CURE_SLEEP_PART()                                            \
    {                                                                        \
        if (target->sleep != 0) {                                            \
            target->sleep = 0;                                               \
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);                         \
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                  \
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0xce3);                     \
        }                                                                    \
    }

/* The GS2 runtime copy entry is an ARMv4T function pointer. */
#define BATTLE_COPY_UNIT(copy, target, size)                                 \
    ((void (*)(void *, const void *, s32))0x03000730)(                       \
        (copy), (target), (size))

#define BATTLE_AFTER_COPY()                                                   \
    {                                                                         \
        s32 state;                                                            \
                                                                              \
        cmd = &BATTLE_PLAN_COMMAND(plan);                                     \
        state = *(u16 *)((u8 *)target + 0x14a);                               \
        if (state < 0x65)                                                     \
            goto gs2_state_guard;                                             \
        {                                                                     \
            if (state > 0x67) {                                               \
                if (state == 0xdd) {                                          \
                    if (*cmd == 1                                             \
                        && ((u32)(actor_id ^ target_id) >> 7) != 0) {          \
                        BattleEvent_Push(11, target_id);                       \
                        BattleEvent_Push(0, target_id);                        \
                        BattleEvent_Push(4, 0xcb5);                            \
                        goto done;                                             \
                    }                                                          \
                    goto gs2_state_guard;                                     \
                }                                                              \
                cmd = &BATTLE_PLAN_COMMAND(plan);                             \
                goto gs2_state_guard;                                         \
            }                                                                  \
            if (action_id != 0x23d) {                                         \
                if (((u8 *)work)[0x868] != 0) {                               \
                    if (((u32)(actor_id ^ target_id) >> 7) != 0) {            \
                        BattleEvent_Push(11, target_id);                       \
                        BattleEvent_Push(0, target_id);                        \
                        BattleEvent_Push(4, 0xcb4);                            \
                        cmd = &BATTLE_PLAN_COMMAND(plan);                      \
                        goto done;                                            \
                    }                                                          \
                    cmd = &BATTLE_PLAN_COMMAND(plan);                          \
                    goto gs2_state_guard;                                     \
                }                                                              \
                cmd = &BATTLE_PLAN_COMMAND(plan);                              \
                goto gs2_state_guard;                                         \
            }                                                                  \
            cmd = &BATTLE_PLAN_COMMAND(plan);                                  \
            goto gs2_state_guard;                                             \
        }                                                                      \
    gs2_state_guard:                                                          \
        if (target->guard_level == 4) {                                       \
            if (((u32)(actor_id ^ target_id) >> 7) != 0) {                    \
                BattleEvent_Push(11, target_id);                              \
                BattleEvent_Push(0, target_id);                               \
                BattleEvent_Push(4, 0xcab);                                   \
                goto done;                                                    \
            }                                                                 \
        }                                                                     \
    }

#undef BATTLE_AFTER_COPY
#define BATTLE_AFTER_COPY()                                                   \
    {                                                                         \
        s32 state;                                                            \
                                                                              \
        state = *(u16 *)((u8 *)target + 0x14a);                               \
        switch (state) {                                                      \
        case 0xdd:                                                            \
            cmd = &BATTLE_PLAN_COMMAND(plan);                                 \
            if (*cmd == 1                                                     \
                && ((u32)(actor_id ^ target_id) >> 7) != 0) {                 \
                BattleEvent_Push(11, target_id);                              \
                BattleEvent_Push(0, target_id);                               \
                BattleEvent_Push(4, 0xcb5);                                   \
                goto done;                                                    \
            }                                                                \
            break;                                                           \
        case 0x65:                                                            \
        case 0x66:                                                            \
        case 0x67:                                                            \
            if (action_id != 0x23d) {                                         \
                if (((u8 *)work)[0x868] != 0) {                               \
                    if (((u32)(actor_id ^ target_id) >> 7) != 0) {            \
                        BattleEvent_Push(11, target_id);                       \
                        BattleEvent_Push(0, target_id);                        \
                        BattleEvent_Push(4, 0xcb4);                            \
                        cmd = &BATTLE_PLAN_COMMAND(plan);                      \
                        goto done;                                             \
                    }                                                          \
                    cmd = &BATTLE_PLAN_COMMAND(plan);                          \
                    goto gs2_state_guard;                                     \
                }                                                              \
                cmd = &BATTLE_PLAN_COMMAND(plan);                              \
                goto gs2_state_guard;                                         \
            }                                                                \
            cmd = &BATTLE_PLAN_COMMAND(plan);                                  \
            goto gs2_state_guard;                                             \
        default:                                                             \
            cmd = &BATTLE_PLAN_COMMAND(plan);                                 \
            break;                                                           \
        }                                                                     \
    gs2_state_guard:                                                          \
        if (target->guard_level == 4) {                                       \
            if (((u32)(actor_id ^ target_id) >> 7) != 0) {                    \
                BattleEvent_Push(11, target_id);                              \
                BattleEvent_Push(0, target_id);                               \
                BattleEvent_Push(4, 0xcab);                                   \
                goto done;                                                    \
            }                                                                 \
        }                                                                     \
    }

#define BATTLE_BEFORE_HIT()                                                    \
    {                                                                         \
        if (action_id == 0x2ae || action_id == 0x165)                         \
            Func_08125390(target_id, 0, 0);                                   \
        if ((u32)(action_id - 0x2d0) <= 1)                                    \
            Func_08125390(target_id, 1, 0);                                   \
        if (action_id == 0x2d8)                                               \
            Func_08125390(target_id, 1, 1);                                   \
    }

/* Effects 50/51 retain the original summon path.  Effects 86/87 add the
 * random and queued summon sources used by the sequel. */
#define BATTLE_SUMMON_BLOCK()                                                \
    {                                                                        \
        if ((u8)(action->effect + 206) <= 1 || action->effect == 0x56        \
            || action->effect == 0x57) {                                     \
            s32 st;                                                          \
            s32 rec;                                                         \
            s32 state_save;                                                  \
            s32 cursor;                                                      \
            s32 msg;                                                         \
            u8 efx;                                                          \
                                                                             \
            state_save = (s32)((u8 *)actor + 0x14a);                         \
            st = *(u16 *)state_save;                                         \
            rec = Summon_FindSlot();                                         \
            affi = -1;                                                       \
            efx = action->effect;                                            \
            if (efx == EFX_STANDBY_WORK) {                                   \
                st = Summon_ClassId(*(s32 *)work);                            \
            } else if (efx == 0x56) {                                        \
                if (*(u16 *)state_save == 0xa4)                              \
                    st = (BattleRandom_Next() & 3) + 0x17a;                  \
                else                                                         \
                    st = 0x51;                                               \
            } else if (efx == 0x57) {                                        \
                if (((s8 *)work)[0x56b] != 0) {                              \
                    s32 qi;                                                  \
                                                                             \
                    qi = ((s8 *)work)[0x56a];                                \
                    st = *(u16 *)((u8 *)work + 0x564 + qi * 2);              \
                    affi = ((u8 *)work)[0x568 + qi];                          \
                } else                                                       \
                    hit = 0;                                                 \
            }                                                                \
            if ((u32)st > 0xffff)                                            \
                hit = 0;                                                     \
            if (hit != 0 && Summon_ClassValid(st) != 0 && rec >= 0) {        \
                if (affi == -1) {                                            \
                    affi = Summon_TakeCharge(st, 1);                          \
                    if (affi & 0x8000)                                       \
                        Summon_ResetCharge(st);                               \
                }                                                            \
                BattleUnit_Assign(rec, st, affi & 0x7fff);                   \
                if (action->effect == 0x57) {                                \
                    s32 qi;                                                  \
                                                                             \
                    ((u8 *)work)[0x56b]--;                                   \
                    qi = ((s8 *)work)[0x56a] + 1;                            \
                    ((s8 *)work)[0x56a] =                                   \
                        qi - (((qi + (qi >> 31)) >> 1) * 2);                 \
                }                                                            \
                if (*(u16 *)((u8 *)actor + 0x14a) == 0xa4) {                 \
                    Func_081203c8((u8 *)work + 0x66, rec);                    \
                } else {                                                     \
                    affi = (s32)((u8 *)work + 2);                            \
                    {                                                        \
                        s32 off;                                             \
                        s32 i;                                               \
                        s32 j;                                               \
                                                                             \
                        off = 100;                                           \
                        i = 0;                                               \
                        state_save = 0;                                      \
                        if (*(s16 *)((u8 *)affi + off) == 254) {             \
                            *(s16 *)((u8 *)affi + off) = rec;                \
                        } else {                                             \
                            s32 woff;                                        \
                            s32 next;                                        \
                                                                             \
                            j = 0;                                           \
                            woff = 100;                                      \
                            for (;;) {                                       \
                                cursor = j + 100;                            \
                                n = *(s16 *)(cursor + affi);                 \
                                if (n == 255) {                              \
                                    *(s16 *)(affi + cursor) = rec;           \
                                    next = state_save + 102;                 \
                                    *(s16 *)(affi + next) = n;               \
                                    break;                                   \
                                }                                            \
                                i++;                                         \
                                next = j + 2;                                \
                                j = next;                                    \
                                if (i > 5)                                   \
                                    break;                                   \
                                state_save = next;                           \
                                woff = next + 100;                           \
                                if (*(s16 *)(woff + affi) == 254) {         \
                                    *(s16 *)(woff + affi) = rec;             \
                                    break;                                   \
                                }                                            \
                            }                                                \
                        }                                                    \
                    }                                                        \
                }                                                            \
                Summon_Refresh();                                            \
                {                                                            \
                    s32 x;                                                   \
                    s32 y;                                                   \
                                                                             \
                    cursor = (s32)Actor_GetObject(rec);                       \
                    x = *(s32 *)(cursor + 12);                               \
                    if (x < 0)                                               \
                        x += 0xffff;                                         \
                    y = *(s32 *)(cursor + 16);                               \
                    x >>= 16;                                                \
                    if (y < 0)                                               \
                        y += 0xffff;                                         \
                    y >>= 16;                                                \
                    Actor_Place((void *)cursor, rec, x, y);                  \
                }                                                            \
                Actor_Commit();                                              \
                {                                                            \
                    s32 listed;                                              \
                                                                             \
                    listed = Actor_ListSlots(saved);                          \
                    if (listed > 0) {                                        \
                        u16 *q;                                              \
                                                                             \
                        q = (u16 *)saved;                                    \
                        count = listed;                                      \
                        do {                                                 \
                            Actor_RefreshSlot(*q++);                         \
                            count--;                                         \
                        } while (count != 0);                                \
                    }                                                        \
                }                                                            \
                BattleEvent_Push(BATTLE_EVENT_UNIT, rec);                    \
                if (action->effect == 0x57) {                                \
                    msg = 0xd64;                                              \
                    goto gs2_summon_msg;                                      \
                } else if (action_id != 0x1f7) {                             \
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0xd56);              \
                } else {                                                     \
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0xd54);              \
                }                                                            \
            } else if (action_id == 0x1f7) {                                 \
                msg = 0xd55;                                                  \
            gs2_summon_msg:                                                   \
                BattleEvent_Push(BATTLE_EVENT_TEXT, msg);                    \
            } else {                                                         \
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0xd57);                  \
            }                                                                \
        }                                                                     \
    }

#define BATTLE_HIT_PREP()
#define BATTLE_HIT_EFFECTS()                                                \
    if (action->effect == EFX_IMMOBILIZE || action->effect == 0x53) {      \
        s32 hidx;                                                          \
                                                                           \
        hit = 0;                                                           \
        hidx = 748;                                                        \
        if (*(s16 *)(BytePtr(work) + hidx) == target_id) {                \
            hit = 1;                                                      \
        } else {                                                           \
            n = 0;                                                        \
        gs2_scan_next:                                                     \
            n++;                                                          \
            if ((u32)n <= 19) {                                           \
                s32 idx;                                                  \
                                                                           \
                idx = ((n << 1) << 3) + 748;                             \
                if (*(s16 *)(BytePtr(work) + idx) == target_id)           \
                    hit = 1;                                              \
                else                                                      \
                    goto gs2_scan_next;                                   \
            }                                                             \
        }                                                                 \
    } else if (action->effect == EFX_HALF_DEF) {                           \
        half = 1;                                                         \
    } else if (action->effect == EFX_LETHAL) {                             \
        crush = 1;                                                        \
    } else if (action->effect == EFX_INSTANT_DOWN) {                       \
        skip = 1;                                                         \
    } else if (action->effect == EFX_ACTOR_FLASH) {                        \
        if (actor->hp != 0)                                               \
            BattleEvent_Push(BATTLE_EVENT_ACTOR_EFFECT, actor_id);        \
    } else if (action->effect == EFX_DRAIN_PP) {                           \
        if (target->pp != 0)                                              \
            nibble = 10;                                                  \
        else                                                              \
            hit = 0;                                                     \
    } else if (action->effect == 0x5a) {                                   \
        half = 2;                                                         \
    } else if (action->effect == 0x5b) {                                   \
        half = 2;                                                         \
    }

#define BATTLE_BEFORE_EFFECTS()                                            \
    {                                                                      \
        if (((u8 *)work)[0x868] != 0 && action_id == 0x23d) {             \
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                \
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0xca7);                    \
            ((u8 *)work)[0x868] = 0;                                       \
        }                                                                  \
    }
#define BATTLE_EFFECT_GATE()                                               \
    if (!((action->effect != 0x4b || slot != 0)                            \
          && (BattleEffect_Classify(action->effect) != 0                  \
              || target->hp != 0                                          \
              || BattleEffect_OnDead(action->effect) != 0)                \
          && hit != 0))                                                    \
        goto done
#define BATTLE_EXTRA_EFFECT_CASES

/* The sequel adds three HP-restoration grades to the original pair.  Keeping
 * the nested switch is significant: GCC emits the same irregular dispatch
 * visible inside the outer effect switch. */
#define BATTLE_HEAL_CASES                                                  \
    case 0x46:                                                             \
    case 0x47:                                                             \
    case 0x4c:
#define BATTLE_HEAL_PREP(old, heal, maxu, maxv, stat_ptr)                  \
    {                                                                      \
        (stat_ptr) = (u16 *)&target->hp;                                   \
        (old) = *(stat_ptr)--;                                             \
        (heal) = *(s16 *)(stat_ptr + 1);                                   \
        if (action->effect == 0x4c) {                                      \
            (stat_ptr) = (u16 *)&target->max_hp;                           \
            (maxv) = *(s16 *)(stat_ptr);                                   \
            (maxu) = *(stat_ptr)--;                                        \
            (heal) += Math_Div((maxv) * 2, 5);                             \
        } else if (action->effect == 0x47) {                               \
            (stat_ptr) = (u16 *)&target->max_hp;                           \
            (maxv) = *(s16 *)(stat_ptr);                                   \
            (maxu) = *(stat_ptr)--;                                        \
            (heal) += Math_Div((maxv) * 7, 10);                            \
        } else if (action->effect == 0x46) {                               \
            (stat_ptr) = (u16 *)&target->max_hp;                           \
            (maxu) = *(stat_ptr)--;                                        \
            (heal) += (s16)(maxu) / 2;                                     \
        } else {                                                           \
            (stat_ptr) = (u16 *)&target->max_hp;                           \
            (maxu) = *(stat_ptr)--;                                        \
            (maxv) = *(s16 *)(stat_ptr + 1);                               \
            if (action->effect == EFX_HEAL_60)                             \
                (heal) += Math_Div((maxv) * 60, 100);                      \
            else                                                           \
                (heal) += Math_Div((maxv) * 30, 100);                      \
        }                                                                  \
    }

#define BATTLE_PP_HEAL_CASES                                               \
    case 0x4d:                                                             \
    case 0x4e:
#define BATTLE_PP_HEAL_AMOUNT(heal, maxv)                                  \
    {                                                                      \
        s16 inc;                                                           \
                                                                           \
        if (action->effect == 0x4d) {                                      \
            inc = (s16)Math_Div((maxv), 10);                               \
        } else {                                                           \
            s32 num;                                                       \
            s32 den;                                                       \
                                                                           \
            if (action->effect == 0x4e) {                                  \
                num = (maxv) * 3;                                          \
                den = 10;                                                  \
            } else {                                                       \
                num = (maxv) * 7;                                          \
                den = 100;                                                 \
            }                                                              \
            inc = (s16)Math_Div(num, den);                                 \
        }                                                                  \
        (heal) += inc;                                                      \
    }

#define BATTLE_PP_RESTORE_BODY()                                           \
    {                                                                      \
        s32 heal;                                                         \
        s32 old;                                                          \
        s32 maxu;                                                         \
        s32 inc;                                                          \
        s32 maxv;                                                         \
        u16 *stat_ptr;                                                    \
                                                                           \
        stat_ptr = (u16 *)&target->pp;                                    \
        old = *stat_ptr--;                                                \
        heal = *(s16 *)(stat_ptr + 1);                                    \
        if (action->effect == 0x4d) {                                     \
            maxv = *(s16 *)((u8 *)target + 54);                           \
            maxu = *(u16 *)((u8 *)target + 54);                           \
            inc = (s16)Math_Div(maxv, 10);                                \
        } else if (action->effect == 0x4e) {                              \
            maxv = *(s16 *)((u8 *)target + 54);                           \
            maxu = *(u16 *)((u8 *)target + 54);                           \
            inc = Math_Div(maxv * 3, 10);                                 \
        } else {                                                          \
            maxv = *(s16 *)((u8 *)target + 54);                           \
            maxu = *(u16 *)((u8 *)target + 54);                           \
            inc = Math_Div(maxv * 7, 100);                                \
        }                                                                 \
        heal += inc;                                                      \
        if (heal > (s16)maxu)                                             \
            heal = (s16)maxu;                                             \
        tmp = heal - (s16)old;                                            \
        if (tmp == 0 && nibble != 11)                                     \
            break;                                                        \
        if (heal == (s16)maxu)                                            \
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_FULL);             \
        else {                                                            \
            BattleEvent_Push(BATTLE_EVENT_VALUE, tmp);                    \
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_RECOVER);          \
        }                                                                 \
        target->pp = (s16)heal;                                           \
        BattleUnit_UpdateRatios(target_id);                               \
        break;                                                            \
    }

#define BATTLE_REVIVE_CASES                                                \
    case 0x49:                                                             \
        if (target->hp != 0)                                               \
            break;                                                         \
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_REVIVED);                  \
        target->hp = (s16)Math_Div(target->max_hp * 6, 10);                \
        BattleUnit_UpdateRatios(target_id);                                \
        break;

#define BATTLE_DELUSION_CASES                                              \
    case 0x4b:
#define BATTLE_STUN_CASES                                                  \
    case 0x55:
#define BATTLE_DEATH_CASES                                                 \
    case 0x50:

/* A second PP-transfer effect restores one tenth of the target's maximum PP
 * and reports the gain from the target's side. */
#define BATTLE_PP_LEECH_CASES                                              \
    case 0x54:                                                             \
        dmg = Math_Div(target->max_pp, 10);                                \
        if (target->pp < dmg)                                              \
            dmg = target->pp;                                              \
        if (dmg == 0)                                                      \
            break;                                                         \
        BattleEvent_Push(BATTLE_EVENT_VALUE, (u8)dmg);                     \
        if ((u32)target_id <= 7)                                           \
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0xcbb);                    \
        else                                                               \
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0xcba);                    \
        BattleUnit_Drain(target_id, -dmg);                                 \
        break;

#define BATTLE_IMMOBILIZE_CASES                                           \
    case 0x53:
#define BATTLE_GUARD1_CASES
#define BATTLE_BETWEEN_GUARD_CASES                                        \
    case 0x48:                                                             \
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0xcdd);                        \
        if ((u32)target->guard_level > 1)                                  \
            break;                                                         \
        target->guard_level = 2;                                           \
        break;
#define BATTLE_GUARD2_CASES                                                \
    case 0x58:
#define BATTLE_GUARD2_THRESHOLD 2
#define BATTLE_GUARD2_LEVEL 3
#define BATTLE_POST_GUARD_CASES                                           \
    case 0x4f:                                                             \
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0xcdf);                        \
        if ((u32)target->guard_level > 3)                                  \
            break;                                                         \
        target->guard_level = 4;                                           \
        break;
#define BATTLE_AFTER_TEXT_CASES                                           \
    case 0x51:                                                             \
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0xc8f);                        \
        ((u8 *)work)[0x47] = 1;                                            \
        break;                                                             \
    case 0x52:                                                             \
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0xca6);                        \
        BattleEvent_Push(15, target_id);                                   \
        break;                                                             \
    case 0x4a:                                                             \
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0xce0);                        \
        ((u8 *)target)[0x143] = 1;                                         \
        break;

#define BATTLE_DONE_PREP()                                                \
    {                                                                      \
        if (*cmd != 9 && (s8)((u8 *)target)[0x143] != 0                   \
            && action->target_mode == 1 && target->hp != 0) {             \
            *(s32 *)((u8 *)work + 0x858) = target_id;                      \
            *(s32 *)((u8 *)work + 0x85c) = actor_id;                       \
        }                                                                  \
    }

#define BATTLE_EFFECT_MAX 88

#endif
