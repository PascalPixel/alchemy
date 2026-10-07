#include "TYPES.H"
#include "BATTLE.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "BATTLE_CALC.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_MSG.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_SUMMON.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_ACTOR.H"
#include "BATTLE_PRESENTATION.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RUNTIME_MEM.H"

#define MIKATA      2           /* 味方 */

#define MIKATA_CHK(id)  ((id) >= 0 && (id) < 8)

#define STEP_LIM(v) \
    { \
        if ((v) < -4) \
            (v) = -4; \
        if ((v) > 4) \
            (v) = 4; \
    }

#define ELM_BONUS() \
    { \
        if (elm != 4) \
            bonus = pow - target->elements[elm].resist; \
    }

#define MES_PUSH(m)     BtlEvPush(BEV_MES, (u32)(m))

#define MES_SIDE(m, e) \
    { \
        if (MIKATA_CHK(tid)) \
            MES_PUSH(m); \
        else \
            MES_PUSH(e); \
    }

#define GUARD_CUT() \
    { \
        guard = target->guard_level; \
        if (guard != 0) { \
            if (guard == 1) \
                dmg /= 2; \
            else \
                dmg /= 10; \
        } \
    }

#define RES_STEP(d, v, m) \
    { \
        target->res_modifier += (d); \
        STEP_LIM(target->res_modifier); \
        BtlEvPush(BEV_NUM, (v)); \
        MES_PUSH(m); \
        target->res_modifier_turns = 7; \
    }

#define STAT_SET(f, m) \
    { \
        MES_PUSH(m); \
        (f) = 7; \
    }

s32 BtlTgtExec(struct BtlPlan *plan, s32 slot)
{
    s32 dist;
    struct BtlAct *action;
    struct BtlUnit *actor;
    s32 aid;
    s32 act_no;
    s32 bonus;
    struct BtlWork *work;
    s32 half;
    s32 adj;
    s32 dealt;
    s32 crush;
    s32 hit;
    s32 crit;
    s32 skip;
    s32 type;
    s32 aff;
    struct BtlUnit *bak;
    s32 pow;
    s32 hp;
    s32 rate;
    s16 *cmd;
    s32 tid;
    s8 *rm;
    s32 elm;
    struct BtlUnit *target;
    u32 n;
    s32 value;
    s32 dmg;
    s32 scale;
    s32 pass;
    s8 *am;
    s32 guard;
    s16 list[8];
    s32 cnt;
    s32 tmp;
    s32 size;
    bonus = 0;
    work = BtlWk;
    half = 0;
    dealt = 0;
    crush = 0;
    skip = 0;
    aff = 0;
    size = sizeof(struct BtlUnit);
    bak = MemAlloc(size);

    aid = plan->actor_id;
    tid = plan->tgt_no[slot];
    act_no = plan->action_id;
    elm = plan->elm;
    adj = plan->tgt_adj[slot];
    crit = plan->tgt_crit[slot];

    action = BtlActGet(act_no);
    actor = UnitGetFar(aid);
    target = UnitGetFar(tid);
    Iwram_CopyWords(bak, target, size);

    if (action->range != 255) {
        dist = plan->tgt_dist[slot];
        if (dist < 0)
            dist = -dist;
    } else {
        dist = 0;
    }

    if (elm != 4) {
        s32 i;

        value = target->base_elements[elm].resist;
        for (i = 0; i < 4 && value >= target->base_elements[i].resist; i++)
            ;
        if (i == 4)
            aff = -1;

        for (i = 0; i < 4 && value <= target->base_elements[i].resist; i++)
            ;
        if (i == 4)
            aff = 1;
    }

    if (plan->elm >= 0 && plan->elm <= 3) {
        cmd = &plan->command;
        if (*cmd != 2) {
            pow = actor->elements[plan->elm].power;
            goto pow_end;
        }
    } else
        cmd = &plan->command;
    pow = 100;
pow_end:

    if (plan->command == 5 && (plan->elm >= 0 && plan->elm <= 3) && aff > 0) {
        s32 chance;

        chance = pow - target->elements[plan->elm].resist;
        chance += 30;
        chance *= 65536 / 100;
        if (chance > (BtlRndFar() & 0xffff))
            BtlEvPush(BEV_SCR, 5);
    }

    type = action->target_flags & 15;
    {
        s32 res;

        res = plan->tgt_hit[slot];
        if (res == -1)
            hit = BtlHitChk(
                aid, tid, elm, action->effect,
                HitFalloff[dist]);
        else
            hit = res;
    }
    if (action->effect == 50 || action->effect == 51) {
        s32 cls;
        s32 ent;

        cls = actor->class_id;
        ent = Summon_FindSlot();
        if (action->effect == 51)
            cls = BtlGrpRnd(work->enemy_group);
        if (hit != 0 && Summon_ClassValid(cls) != 0 && ent >= 0) {
            s32 chg;

            chg = Summon_TakeCharge(cls, 1);
            if (chg & 0x8000)
                Summon_ResetCharge(cls);
            BtlUnitSetFar(ent, cls, chg & 0x7fff);
            {
                s32 i;

                for (i = 0; i < 6; i++) {
                    if (work->enemy_units[i] == 254) {
                        work->enemy_units[i] = ent;
                        break;
                    }
                    if (work->enemy_units[i] == 255) {
                        work->enemy_units[i] = ent;
                        work->enemy_units[i + 1] = 255;
                        break;
                    }
                }
            }
            Summon_Refresh();
            {
                struct BtlObj *obj;

                obj = BtlObjGet(ent);
                BtlObjMake(obj, ent,
                    obj->anchor_x / 0x10000, obj->anchor_z / 0x10000);
            }
            BtlObjPut();
            {
                s32 num;

                num = BtlEnmList(list);
                for (cnt = 0; cnt < num; cnt++)
                    ObjMotRst((u16)list[cnt]);
            }
            BtlEvPush(BEV_UNIT, ent);
            if (act_no != 503)
                MES_PUSH(MsgAppears);
            else
                MES_PUSH(MsgSplitOff);
        } else if (act_no == 503) {
            MES_PUSH(MsgSplitFail);
        } else {
            MES_PUSH(MsgNoOneCame);
        }
    }

    if (hit != 0) {
        if (action->effect == 53) {
            hit = 0;
            for (n = 0; n < 20; n++) {
                if (work->actions[n].unit_id == tid) {
                    hit = 1;
                    break;
                }
            }
        } else if (action->effect == 35) {
            half = 1;
        } else if (action->effect == 34) {
            crush = 1;
        } else if (action->effect == 27) {
            skip = 1;
        } else if (action->effect == 55) {
            if (actor->hp != 0)
                BtlEvPush(BEV_OBJ_EFX, aid);
        } else if (action->effect == 32) {
            if (target->pp != 0)
                type = 10;
            else
                hit = 0;
        }
    }

    if (skip == 0
        && (target->hp != 0 || BtlEfxRevChkFar(action->effect) != 0)) {
        s32 pp;
        s32 cur;
        s32 ap;

        switch (type) {
        case 3:
        case 4:
        {
            s32 def;

            hp = target->hp;
            def = target->defense;
            scale = def;
            if (half != 0)
                scale = (u32)def >> 1;
            pass = 1;
            do {
                ELM_BONUS();
                if (pass == 0)
                    bonus = 0;
                ap = action->power;
                if (type == 4)
                    dmg = BtlAtkCalc(actor->attack, scale, 0,
                                          bonus)
                            * ap / 10;
                else {
                    dmg = BtlAtkCalc(actor->attack, scale, ap,
                                            bonus);
                }
                dmg *= adj;
                if (crit != 0) {
                    if (crit == 1)
                        dmg = dmg * 5 / 4;
                    else
                        dmg = dmg * 3 / 2;
                    dmg += (target->level / 5) + 6;
                    if (pass == 0) {
                        BtlEvPush(BEV_MARK, 0);
                        {
                            s32 mes;

                            mes = (s32)MsgCritical;
                            if (MIKATA_CHK(tid))
                                mes += 1;
                            BtlEvPush(BEV_MES_ADD, mes);
                        }
                    }
                }
                dmg += BtlRndFar() & 3;
                GUARD_CUT();
                if (dmg <= 0)
                    dmg = 1;
                if (crush != 0) {
                    if (dmg < hp - 1) {
                        dmg = hp - 1;
                        if (dmg <= 0)
                            dmg = 1;
                    }
                }
                if (GameFlag_TestFar(366) != 0 && *cmd == 5
                    && hp <= dmg) {
                    dmg = hp - 1;
                }
                pass++;
            } while (pass <= 1);
            hp -= dmg;
            BtlEvPush(BEV_OBJ_BGN, tid);
            BtlEvPush(BEV_UNIT, tid);
            BtlEvPush(BEV_NUM, dmg);
            {
                s32 mes;

                if (MIKATA_CHK(tid))
                    mes = (s32)MsgDmgEmphP + aff;
                else
                    mes = (s32)MsgDmgEmphE + aff;
                BtlEvPush(BEV_MES, mes);
            }

            if (hp <= 0) {
                BtlEvPush(BEV_OBJ_RES, tid);
                BtlEvPush(BEV_UNIT, tid);
                hp = 0;
                MES_SIDE(MsgGoesDown, MsgFelled);
            } else
                BtlEvPush(BEV_OBJ_END, tid);
            dealt = target->hp - hp;
            target->hp = hp;
            UnitRatioFar(tid);
            break;
        }

        case 10:        /* EPダメージ */
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            ELM_BONUS();
            ap = action->power;
            dmg = BtlPowCalc(ap, bonus, 256);
            dmg = dmg * PpLossFalloff[dist] / 100;
            dmg *= adj;
            GUARD_CUT();
            if (action->effect == 32 && dmg > pp)
                dmg = pp;

            BtlEvPush(BEV_OBJ_BGN, tid);
            BtlEvPush(BEV_NUM, dmg);
            BtlEvPush(BEV_UNIT, tid);
            {
                s32 mes;

                if (MIKATA_CHK(tid))
                    mes = (s32)MsgPpLossP;
                else
                    mes = (s32)MsgPpLossE;
                BtlEvPush(BEV_MES, mes);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BtlEvPush(BEV_OBJ_END, tid);
            dealt = target->pp - pp;
            target->pp = pp;
            UnitRatioFar(tid);
            break;
        }

        case 1:         /* HPかいふく */
        {
            if (action->power == 0)
                break;
            cur = target->hp;

            ap = action->power;
            dmg = BtlRcvCalc(ap, elm == 4 ? 100 : pow, 256);
            dmg = dmg * HpHealFalloff[dist] / 100;
            dmg *= adj;
            dmg += BtlRndFar() & 3;
            cur += dmg;
            if (cur > target->max_hp) {
                cur = target->max_hp;
                dmg = cur - target->hp;
            }
            BtlEvPush(BEV_UNIT, tid);
            if (cur == target->max_hp)
                MES_PUSH(MsgHpFull);
            else {
                BtlEvPush(BEV_NUM, dmg);
                MES_PUSH(MsgHpRecover);
            }
            dealt = target->hp - cur;
            target->hp = cur;
            UnitRatioFar(tid);
            break;
        }

        case -1:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            ELM_BONUS();
            ap = action->power;
            dmg = BtlPowCalc(ap, bonus, 256);
            dmg = dmg * PpDmgFalloff[dist] / 100;
            dmg *= adj;
            GUARD_CUT();
            BtlEvPush(BEV_OBJ_BGN, tid);
            BtlEvPush(BEV_NUM, dmg);
            BtlEvPush(BEV_UNIT, tid);
            {
                s32 mes;

                if (MIKATA_CHK(tid))
                    mes = (s32)MsgDmgP;
                else
                    mes = (s32)MsgDmgE;
                BtlEvPush(BEV_MES, mes);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BtlEvPush(BEV_OBJ_END, tid);
            target->pp = pp;
            UnitRatioFar(tid);
            break;
        }

        case 5:
        case 6:
        case 8:
        {
            s32 round;

            if (action->power == 0)
                break;
            cur = target->hp;
            round = 1;
            do {
                ELM_BONUS();
                if (round == 0)
                    bonus = 0;
                ap = action->power;
                if (*cmd == 6) {
                    s32 item;

                    item = act_no - 380;
                    if (item >= 0 && item <= 21) {
                        switch (item) {
                        case 0:
                        case 6:
                        case 12:
                        case 18:
                            rate = 3;
                            break;
                        case 1:
                        case 7:
                        case 13:
                        case 19:
                            rate = 6;
                            break;
                        case 2:
                        case 8:
                        case 14:
                        case 20:
                            rate = 9;
                            break;
                        case 3:
                        case 9:
                        case 15:
                        case 21:
                            rate = 12;
                            break;
                        }
                    }
                    ap += target->max_hp * rate / 100;
                }
                dmg = BtlPowCalc(ap, bonus, 256);
                dmg *= adj;
                switch (type & 15) {
                case 5:
                    dmg = HpDmgFalloff5[dist] * dmg / 100;
                    break;
                case 8:
                    dmg = dmg * HpDmgFalloff8[dist] / 100;
                    break;
                case 6:
                    dmg = dmg * HpDmgFalloff6[dist] / 100;
                    break;
                }
                dmg += BtlRndFar() & 3;
                GUARD_CUT();
                if (GameFlag_TestFar(366) != 0 && *cmd == 6 && cur > dmg) {
                    dmg = cur;
                }
                round++;
            } while (round <= 1);
            BtlEvPush(BEV_OBJ_BGN, tid);
            BtlEvPush(BEV_NUM, dmg);
            BtlEvPush(BEV_UNIT, tid);
            {
                s32 mes;

                if (MIKATA_CHK(tid))
                    mes = (s32)MsgDmgEmphP + aff;
                else
                    mes = (s32)MsgDmgEmphE + aff;
                BtlEvPush(BEV_MES, mes);
                cur -= dmg;
            }

            if (cur <= 0) {
                BtlEvPush(BEV_OBJ_RES, tid);
                BtlEvPush(BEV_UNIT, tid);
                {
                    s32 mes;

                    cur = 0;
                    if (MIKATA_CHK(tid))
                        mes = (s32)MsgGoesDown;
                    else
                        mes = (s32)MsgFelled;
                    BtlEvPush(BEV_MES, mes);
                }
            } else
                BtlEvPush(BEV_OBJ_END, tid);
            dealt = target->hp - cur;
            target->hp = cur;
            UnitRatioFar(tid);
            break;
        }

        case 11:        /* EPかいふく */
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            ap = action->power;
            dmg = BtlRcvCalc(ap, elm == 4 ? 100 : pow, 256);
            dmg = dmg * PpHealFalloff[dist] / 100;
            dmg *= adj;
            pp += dmg;
            if (pp > target->max_pp) {
                pp = target->max_pp;
                dmg = pp - target->pp;
            }
            BtlEvPush(BEV_UNIT, tid);
            if (pp == target->max_pp)
                MES_PUSH(MsgPpFull);
            else {
                BtlEvPush(BEV_NUM, dmg);
                MES_PUSH(MsgPpRecover);
            }
            target->pp = pp;
            UnitRatioFar(tid);
            break;
        }

        case 0:
        case 7:
        case 9:
            break;

        case 2:
            if (hit != 0) {
                if (action->power == 0)
                    break;
                cur = target->hp;
                ELM_BONUS();
                ap = action->power;
                dmg = BtlPowCalc(ap, bonus, 256);
                dmg *= adj;
                dmg = dmg * HpDmgFalloff[dist] / 100;
                GUARD_CUT();
                BtlEvPush(BEV_OBJ_BGN, tid);
                BtlEvPush(BEV_NUM, dmg);
                BtlEvPush(BEV_UNIT, tid);
                {
                    s32 mes;

                    if (MIKATA_CHK(tid))
                        mes = (s32)MsgDmgP;
                    else
                        mes = (s32)MsgDmgE;
                    BtlEvPush(BEV_MES, mes);
                    cur -= dmg;
                }

                if (cur <= 0) {
                    BtlEvPush(BEV_OBJ_RES, tid);
                    BtlEvPush(BEV_UNIT, tid);
                    cur = 0;
                    MES_SIDE(MsgGoesDown, MsgFelled);
                } else
                    BtlEvPush(BEV_OBJ_END, tid);
                dealt = target->hp - cur;
                target->hp = cur;
                UnitRatioFar(tid);
                break;
            }
            BtlEvPush(BEV_OBJ_END, tid);
            BtlEvPush(BEV_UNIT, tid);
            MES_PUSH(MsgNoEffect);
            break;

        }
    }

    BtlEvPush(BEV_UNIT, tid);
    if ((BtlEfxRevChkFar(action->effect) != 0 || target->hp != 0
            || BtlEfxDeadChk(action->effect) != 0)
        && hit != 0) {
        switch (action->effect) {

        case 64:
            if (target->delusion != 0) {
                target->delusion = 0;
                MES_PUSH(MsgCureDelusion);
                BtlEvPush(BEV_UNIT, tid);
            }
            if (target->stun != 0) {
                target->stun = 0;
                BtlEvPush(BEV_CLR, 0);
                BtlEvPush(BEV_UNIT, tid);
                MES_PUSH(MsgCureStun);
            }
            target->sleep = 0;
            if (target->psy_seal != 0) {
                target->psy_seal = 0;
                MES_PUSH(MsgCureSeal);
                BtlEvPush(BEV_UNIT, tid);
            }
            if (target->death_count != 0) {
                target->death_count = 0;
                MES_PUSH(MsgCureCurse);
                BtlEvPush(BEV_UNIT, tid);
            }
            if (target->evil_spirit != 0) {
                target->evil_spirit = 0;
                MES_PUSH(MsgCureSpirit);
                BtlEvPush(BEV_UNIT, tid);
            }
            if (target->poison != 0) {
                MES_PUSH(MsgCurePoison);
                target->poison = 0;
            }
            BtlEvPush(BEV_CLR, 0);
            break;

        case 4:
            if (target->delusion != 0) {
                target->delusion = 0;
                BtlEvPush(BEV_CLR, 0);
                BtlEvPush(BEV_UNIT, tid);
                MES_PUSH(MsgCureDelusion);
            }
            if (target->stun != 0) {
                target->stun = 0;
                BtlEvPush(BEV_CLR, 0);
                BtlEvPush(BEV_UNIT, tid);
                MES_PUSH(MsgCureStun);
            }
            target->sleep = 0;
            if (target->psy_seal != 0) {
                target->psy_seal = 0;
                BtlEvPush(BEV_CLR, 0);
                BtlEvPush(BEV_UNIT, tid);
                MES_PUSH(MsgCureSeal);
            }
            if (target->death_count == 0)
                break;
            target->death_count = 0;
            BtlEvPush(BEV_CLR, 0);
            BtlEvPush(BEV_UNIT, tid);
            MES_PUSH(MsgCureCurse);
            break;

        case 61:        /* HPかいふく */
        case 62:        /* HPかいふく */
        {
            s32 old;
            s32 maxu;
            s32 maxv;
            s32 heal;
            u16 *pt;

            /* FAKEMATCH: walking a pointer down from hp to max_hp reads each stat twice, unsigned and signed, as the ROM does; plain f reads share one load. */
            pt = (u16 *)&target->hp;
            old = *pt--;
            heal = *(s16 *)(pt + 1);
            pt--;
            maxu = *pt--;
            maxv = *(s16 *)(pt + 1);
            if (action->effect == 61)
                heal += maxv * 60 / 100;
            else
                heal += maxv * 30 / 100;
            if (heal > (s16)maxu)
                heal = (s16)maxu;
            tmp = heal - (s16)old;
            if (tmp == 0 && type != 1)
                break;
            if (heal == (s16)maxu)
                MES_PUSH(MsgHpFull);
            else {
                BtlEvPush(BEV_NUM, tmp);
                MES_PUSH(MsgHpRecover);
            }
            target->hp = heal;
            UnitRatioFar(tid);
            break;
        }

        case 63:        /* EPかいふく */
        {
            s32 old;
            s32 maxv;
            s32 heal;

            heal = target->pp;
            maxv = target->max_pp;
            old = heal;
            heal += maxv * 7 / 100;
            if (heal > maxv)
                heal = maxv;
            tmp = heal - old;
            if (tmp == 0 && type != 11)
                break;
            if (heal == maxv)
                MES_PUSH(MsgPpFull);
            else {
                BtlEvPush(BEV_NUM, tmp);
                MES_PUSH(MsgPpRecover);
            }
            target->pp = heal;
            UnitRatioFar(tid);
            break;
        }

        case 59:        /* すばやさアップ */
            target->agility_modifier = 8;
            target->agility_modifier_turns = 5;
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, target->agility - bak->agility);
            MES_PUSH(MsgAgiUp);
            break;

        case 58:        /* すばやさダウン */
            am = &target->agility_modifier;
        {
            u8 v;

            v = -4;
            *am = v;
        }
            target->agility_modifier_turns = 5;
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, bak->agility - target->agility);
            MES_PUSH(MsgAgiDown);
            break;

        case 9:         /* こうげき下がる */
            target->attack_modifier -= 1;
            STEP_LIM(target->attack_modifier);
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, bak->attack - target->attack);
            MES_PUSH(MsgAtkDown);
            target->attack_modifier_turns = 7;
            break;

        case 8:         /* こうげき下がる */
            target->attack_modifier -= 2;
            STEP_LIM(target->attack_modifier);
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, bak->attack - target->attack);
            MES_PUSH(MsgAtkDown);
            target->attack_modifier_turns = 7;
            break;

        case 7:         /* こうげき上がる */
            target->attack_modifier += 1;
            STEP_LIM(target->attack_modifier);
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, target->attack - bak->attack);
            MES_PUSH(MsgAtkUp);
            target->attack_modifier_turns = 7;
            break;

        case 6:         /* こうげき上がる */
            target->attack_modifier += 2;
            STEP_LIM(target->attack_modifier);
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, target->attack - bak->attack);
            MES_PUSH(MsgAtkUp);
            target->attack_modifier_turns = 7;
            break;

        case 13:        /* ぼうぎょ下がる */
            target->defense_modifier -= 1;
            STEP_LIM(target->defense_modifier);
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, bak->defense - target->defense);
            MES_PUSH(MsgDefDown);
            target->defense_modifier_turns = 7;
            break;

        case 12:        /* ぼうぎょ下がる */
            target->defense_modifier -= 2;
            STEP_LIM(target->defense_modifier);
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, bak->defense - target->defense);
            MES_PUSH(MsgDefDown);
            target->defense_modifier_turns = 7;
            break;

        case 11:        /* ぼうぎょ上がる */
            target->defense_modifier += 1;
            STEP_LIM(target->defense_modifier);
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, target->defense - bak->defense);
            MES_PUSH(MsgDefUp);
            target->defense_modifier_turns = 7;
            break;

        case 10:        /* ぼうぎょ上がる */
            target->defense_modifier += 2;
            STEP_LIM(target->defense_modifier);
            UnitCalcFar(tid);
            BtlEvPush(BEV_NUM, target->defense - bak->defense);
            MES_PUSH(MsgDefUp);
            target->defense_modifier_turns = 7;
            break;

        case 5:         /* ふっき */
            if (target->hp != 0)
                break;
            MES_PUSH(MsgRevived);
            target->hp = target->max_hp;
            UnitRatioFar(tid);
            break;

        case 56:        /* ふっき */
            if (target->hp != 0)
                break;
            MES_PUSH(MsgRevived);
            target->hp = target->max_hp / 2;
            UnitRatioFar(tid);
            break;

        case 57:        /* ふっき */
            if (target->hp != 0)
                break;
            MES_PUSH(MsgRevived);
            target->hp = target->max_hp * 8 / 10;
            UnitRatioFar(tid);
            break;

        case 3:         /* 毒がきえる */
            if (target->poison != 0)
                MES_PUSH(MsgCurePoison);
            target->poison = 0;
            break;

        case 17:        /* たいせいダウン */
            RES_STEP(-1, (bak->res_modifier - target->res_modifier) * 20, MsgResDown);
            break;

        case 16:        /* たいせいダウン */
        {
            target->res_modifier -= 2;
            if (-4 > target->res_modifier)
                target->res_modifier = -4;
            if (target->res_modifier > 4)
                target->res_modifier = 4;
            BtlEvPush(BEV_NUM, (bak->res_modifier - target->res_modifier) * 20);
            MES_PUSH(MsgResDown);
            target->res_modifier_turns = 7;
        }
            break;

        case 15:        /* たいせいアップ */
            RES_STEP(1, (target->res_modifier - bak->res_modifier) * 20, MsgResUp);
            break;

        case 14:        /* たいせいアップ */
        {
            target->res_modifier += 2;
            if (*(rm = &target->res_modifier) < -4)
                target->res_modifier = -4;
            if (target->res_modifier > 4)
                target->res_modifier = 4;
            BtlEvPush(BEV_NUM, (target->res_modifier - bak->res_modifier) * 20);
            MES_PUSH(MsgResUp);
            target->res_modifier_turns = 7;
        }
            break;

        case 18:        /* 毒 */
            if (target->poison != 0)
                break;
            MES_PUSH(MsgPoisoned);
            target->poison = 1;
            break;

        case 19:        /* もう毒 */
            if (target->poison > 1)
                break;
            MES_PUSH(MsgVenom);
            target->poison = 2;
            break;

        case 20:        /* まぼろし */
            STAT_SET(target->delusion, MsgDelusion);
            break;

        case 21:        /* こんらん */
            STAT_SET(target->confusion, MsgConfused);
            break;

        case 22:        /* こころをうばう */
            STAT_SET(target->charm, MsgCharmed);
            break;

        case 23:        /* しびれ */
            STAT_SET(target->stun, MsgStunned);
            break;

        case 24:        /* ねむり */
            STAT_SET(target->sleep, MsgAsleep);
            break;

        case 25:        /* エナジーふうじ */
            MES_SIDE(MsgPsyBlock, MsgPsySeal);
            target->psy_seal |= 7;
            break;

        case 67:        /* エナジーふうじ */
            MES_SIDE(MsgPsyBlock, MsgPsySeal);
            target->psy_seal |= 16;
            break;

        case 27:        /* せんとうふのう */
            BtlEvPush(BEV_OBJ_RES, tid);
            if (target->side == MIKATA)
                MES_PUSH(MsgKoDown);
            else if (act_no == 219)
                MES_PUSH(MsgSuffocate);
            else
                MES_PUSH(MsgSpiritDrain);
            target->hp = 0;
            UnitRatioFar(tid);
            break;

        case 29:        /* リフレイン */
            STAT_SET(target->refrain, MsgRefrain);
            break;

        case 30:        /* リフレクト */
            STAT_SET(target->reflect, MsgReflect);
            break;

        case 31:
        case 60:
        {
            s32 heal;

            heal = actor->hp;
            dmg = dealt;
            if (action->effect == 60)
                dmg /= 2;
            heal += dmg;
            if (heal > actor->max_hp) {
                heal = actor->max_hp;
                dmg = heal - actor->hp;
            }
            BtlEvPush(BEV_CLR, 0);
            BtlEvPush(BEV_UNIT, aid);
            if (heal == actor->max_hp)
                MES_PUSH(MsgHpFull);
            else {
                BtlEvPush(BEV_NUM, dmg);
                MES_PUSH(MsgHpRecover);
            }
            actor->hp = heal;
            UnitRatioFar(aid);
            break;
        }

        case 32:
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
            BtlEvPush(BEV_CLR, 0);
            BtlEvPush(BEV_UNIT, aid);
            if (heal == actor->max_pp)
                MES_PUSH(MsgPpFull);
            else {
                BtlEvPush(BEV_NUM, amt);
                MES_PUSH(MsgPpRecover);
            }
            actor->pp = heal;
            UnitRatioFar(aid);
            break;
        }

        case 69:        /* EPをうばう */
            dmg = dealt / 10;
            if (target->pp < dmg)
                dmg = target->pp;
            if (actor->pp + dmg > actor->max_pp)
                dmg = actor->max_pp - actor->pp;
            if (dmg == 0)
                break;
            BtlEvPush(BEV_NUM, dmg);
            MES_SIDE(MsgLeechTake, MsgLeechGain);
            UnitEpAddFar(aid, dmg);
            break;

        case 33:        /* つよさがもどる */
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
            target->element_modifier[0] = 0;
            target->element_modifier[1] = 0;
            target->element_modifier[2] = 0;
            target->element_modifier[3] = 0;
            MES_PUSH(MsgBuffsReset);
            break;

        case 26:        /* あくりょう */
            MES_PUSH(MsgEvilSpirit);
            target->evil_spirit = 1;
            break;

        case 28:        /* しりょう */
            if (target->death_count == 0) {
                MES_PUSH(MsgDeathCurse);
                target->death_count = 7;
                break;
            }
            if (target->death_count <= 1)
                break;
            target->death_count -= 1;
            BtlEvPush(BEV_NUM, target->death_count);
            MES_PUSH(MsgDeathCount);
            break;

        case 66:        /* そなえ */
            MES_PUSH(MsgReadies);
            target->ready_pose = 2;
            break;

        case 54:        /* ちょうはつ */
            MES_PUSH(MsgChallenge);
            target->battle_end_state = 1;
            if (MIKATA_CHK(tid))
                work->chal_flg |= 2;
            break;

        case 53:        /* みうごき */
            MES_PUSH(MsgImmobile);
            target->cannot_move = 1;
            break;

        case 46:        /* まもりのオーラ */
            MES_PUSH(MsgAura);
            if (target->guard_level > 0)
                break;
            target->guard_level = 1;
            break;

        case 47:        /* まもりのオーラ */
            MES_PUSH(MsgAura2);
            if (target->guard_level > 1)
                break;
            target->guard_level = 2;
            break;

        case 45:
            BtlEvPush(BEV_MES, -1);
            break;

        default:
            break;
        }
    }

    BtlEvPush(BEV_CLR, 0);

    if (target->hp != 0) {
        if (target->sleep != 0)
            if (target->sleep <= 6
                && dealt > 0 && (3 & BtlRndFar()) == 0) {
                target->sleep = 0;
                BtlEvPush(BEV_UNIT, tid);
                MES_PUSH(MsgWakes);
            }
    }
    MemFree(bak);
    UnitCalcFar(tid);
    PtyStDrawFar(BtlWk->party_status_mode);
    if (target->hp != 0)
        BtlEvPush(BEV_OBJ_END, tid);
    if (actor->evil_spirit != 0
        && (BtlRndFar() & 3) == 0 && dealt > 0) {
        s32 share;

        share = dealt >> 2;
        if (share == 0)
            share = 1;
        plan->spirit_dmg += share;
    }
}
