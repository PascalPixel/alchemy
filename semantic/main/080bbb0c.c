/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * Note what the previous draft had already half-seen: it passed
 * 0x03001388 as a fourth ARGUMENT.  That value was never an argument --
 * it is the callee, and the register load that produced it is the call
 * target, not a parameter.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
s32 Func_080022f4(s32, s32);
void Func_08002df0(void *);
void *Func_08004938(s32);
void Func_08015130(s32);
u8 *Func_08077008(s32);
void Func_08077010(s32);
u8 *Func_08077080(s32);
s32 Func_080770c0(s32);
void Func_08077120(s32, s32);
void Func_08077128(s32);
void Func_08077140(s32, s32, s32);
s32 Func_08077178(s32, s32, s32, s32, s32);
s32 Func_08077180(u16, u32, u16, s32);
s32 Func_08077188(u16, s32, s32);
s32 Func_08077190(u16, s32, s32);
s32 Func_080771a0(void);
s32 Func_080772b8(s32);
s32 Func_080b6ae0(s16 *);
void Func_080b6c90(void);
s32 Func_080b6cdc(s32);
void Func_080b6f44(void *, s32, s32, s32);
s32 Func_080b7514(void);
void Func_080b7548(void);
void *Func_080b7dd0(s32);
void Func_080b8000(s32);
u32 Func_080bbabc(u32, u32);
s32 Func_080bbae8(s32);
s32 Func_080c1df4(s32, s32);
s32 Func_080c1f50(s32);
u32 Func_080c1fa8(s32);

/*
 * Resolve one target from a queued battle action.  The request identifies the
 * acting combatant, action, target list, range slot, and per-target modifiers.
 * The routine applies the action's HP/PP/stat/status effect to the live target,
 * queues the corresponding battle messages, and then performs common cleanup.
 *
 * The original owner also contained two internal branch-with-link
 * continuations at 0x080bd236 and 0x080bd2c0.  They are represented below as
 * direct message emission followed by `goto finalize`; they are not separate
 * ABI-callable functions.
 */
s32 Func_080bbb0c(void *arg0, s32 arg1) {
    s16 queued_ids[7];
    s16 *sp4;
    s32 sp8;
    s32 spC;
    u8 *target_snapshot;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 action_allowed;
    s32 sp28;
    u32 health_delta;
    s32 sp30;
    s32 sp34;
    u8 *battle_state;
    s32 sp3C;
    s32 action_id;
    s32 actor_id;
    u8 *actor_state;
    u8 *action;
    u8 *request;
    s16 *var_r6;
    s16 temp_r0_3;
    s16 temp_r1_3;
    s16 temp_r2_3;
    s16 temp_r2_5;
    s16 temp_r2_6;
    s16 temp_r2_7;
    s16 temp_r3_11;
    s16 temp_r3_26;
    s16 temp_r3_27;
    s16 temp_r3_28;
    s16 temp_r3_7;
    s16 temp_r5_4;
    s16 temp_r5_5;
    s16 temp_r5_6;
    s16 temp_r6;
    s16 temp_r6_10;
    s16 temp_r6_2;
    s16 temp_r6_3;
    s16 temp_r6_4;
    s16 temp_r6_5;
    s16 temp_r6_6;
    s16 temp_r6_7;
    s16 temp_r6_9;
    u8 *target_state;
    s32 temp_r0_4;
    s32 temp_r0_6;
    s32 temp_r3_4;
    s32 temp_r5;
    s32 temp_r5_3;
    s32 temp_r8;
    s32 range_index;
    s32 var_lr;
    s32 var_r0;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r0_7;
    s32 var_r1;
    s32 var_r1_12;
    s32 var_r1_2;
    s32 var_r1_8;
    s32 var_r2_3;
    s32 var_r2_5;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r4;
    s32 var_r5_11;
    s32 var_r5_14;
    s32 var_r5_15;
    s32 var_r5_16;
    s32 var_r5_18;
    s32 var_r5_19;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r5_4;
    s32 var_r5_5;
    s32 var_r6_2;
    s32 var_r6_3;
    s32 var_r6_4;
    s32 var_r6_5;
    s32 var_r8;
    s32 var_r8_2;
    s8 *var_r1_20;
    s8 *var_r2_6;
    s8 *var_r2_7;
    s8 *var_r2_8;
    s8 temp_r3_12;
    s8 temp_r3_3;
    s8 temp_r3_5;
    s8 temp_r3_8;
    s8 temp_r3_9;
    s32 range_distance;
    s8 var_r0_2;
    u16 temp_r0_7;
    u16 temp_r1_2;
    u16 temp_r3_10;
    u16 temp_r3_21;
    u16 temp_r3_6;
    u16 temp_r5_2;
    u16 temp_r6_8;
    u16 temp_r8_2;
    u16 effect_scale;
    u16 var_r5_10;
    u32 temp_r2_2;
    u32 temp_r3_2;
    u32 var_r1_10;
    u32 var_r1_11;
    u32 var_r1_13;
    u32 var_r1_14;
    u32 var_r1_15;
    u32 var_r1_21;
    u32 var_r1_22;
    u32 var_r1_3;
    u32 var_r1_4;
    u32 var_r1_5;
    u32 var_r1_6;
    u32 var_r1_7;
    u32 var_r1_9;
    u32 var_r2_4;
    u32 var_r5_12;
    u32 var_r5_13;
    u32 var_r5_17;
    u32 var_r5_6;
    u32 var_r5_7;
    u32 var_r5_8;
    u32 var_r5_9;
    u32 var_r6_6;
    u8 temp_r0_8;
    u8 temp_r2_4;
    u8 temp_r3_13;
    u8 temp_r3_14;
    u8 temp_r3_15;
    u8 temp_r3_16;
    u8 temp_r3_17;
    u8 temp_r3_18;
    u8 temp_r3_19;
    u8 temp_r3_20;
    u8 temp_r3_22;
    u8 temp_r3_23;
    u8 temp_r3_24;
    u8 temp_r3_25;
    u8 temp_r3_29;
    u8 temp_r3_30;
    u8 target_id;
    u8 var_r1_16;
    u8 var_r1_17;
    u8 var_r1_18;
    u8 var_r1_19;
    u8 var_r5;
    u8 *temp_r0_5;
    u8 *temp_r1;
    u8 *temp_r2;
    u8 *temp_r4_2;
    u8 *var_r2;
    u8 *var_r2_2;

    request = (u8 *) arg0;
    sp3C = 0;
    sp34 = 0;
    health_delta = 0;
    sp28 = 0;
    sp1C = 0;
    sp14 = 0;
    battle_state = *(void **)0x03001E74;
    target_snapshot = Func_08004938(0x14C);
    actor_id = (s32) M2C_FIELD(request, u8, 0);
    temp_r2 = request + 2;
    action_id = M2C_FIELD(request, s32, 0x4C);
    target_id = *(temp_r2 + arg1);
    sp30 = *((s8 *) temp_r2 + (arg1 + 0x1C));
    range_index = M2C_FIELD(arg0, s32, 0x50);
    sp20 = *((s8 *) request + (arg1 + 0x2C));
    action = Func_08077080(action_id);
    actor_state = Func_08077008(actor_id);
    target_state = Func_08077008((s32) target_id);
    ((WordCopy)0x03001388)(target_snapshot, target_state, 0x14C);
    if (M2C_FIELD(action, u8, 8) != 0xFF) {
        range_distance = *((s8 *) request + (arg1 + 0x10));
        if (range_distance < 0) {
            range_distance = -range_distance;
        }
    } else {
        range_distance = 0;
    }
    if (range_index != 4) {
        temp_r4_2 = target_state + 0x24;
        temp_r0_3 = M2C_FIELD(((range_index * 4) + target_state), s16, 0x26);
        var_r1 = 0;
        if ((s32) temp_r0_3 >= (s32) M2C_FIELD(temp_r4_2, s16, 2)) {
            var_r2 = temp_r4_2;
loop_7:
            var_r1 += 1;
            var_r2 += 4;
            if (var_r1 <= 3) {
                if ((s32) temp_r0_3 >= (s32) M2C_FIELD(var_r2, s16, 2)) {
                    goto loop_7;
                }
            }
        }
        if (var_r1 == 4) {
            sp14 = -1;
        }
        var_r1_2 = 0;
        if ((s32) temp_r0_3 <= (s32) M2C_FIELD(temp_r4_2, s16, 2)) {
            var_r2_2 = target_state + 0x24;
loop_13:
            var_r1_2 += 1;
            var_r2_2 += 4;
            if (var_r1_2 <= 3) {
                if ((s32) temp_r0_3 <= (s32) M2C_FIELD(var_r2_2, s16, 2)) {
                    goto loop_13;
                }
            }
        }
        if (var_r1_2 == 4) {
            sp14 = 1;
        }
    }
    temp_r2_2 = M2C_FIELD(request, u32, 0x50);
    if (temp_r2_2 <= 3U) {
        sp4 = (s16 *)(request + 0x48);
        if (M2C_FIELD(request, s16, 0x48) != 2) {
            spC = M2C_FIELD(actor_state, s16, (temp_r2_2 * 4) + 0x48);
        } else {
            goto block_21;
        }
    } else {
        sp4 = (s16 *)(request + 0x48);
block_21:
        spC = 0x64;
    }
    if ((*sp4 == 5) && (temp_r2_2 <= 3U) && (sp14 > 0)) {
        temp_r5 = spC - M2C_FIELD((target_state + ((temp_r2_2 * 4) + 0x48)), s16, 2);
        temp_r5 += 0x1E;
        temp_r5 *= 0x28F;
        if (temp_r5 > (Func_080771a0() & 0xFFFF)) {
            Func_080bbabc(0xDU, 5U);
        }
    }
    sp18 = 0xF & M2C_FIELD(action, u8, 1);
    var_r0_2 = *(temp_r2 + (arg1 + 0x38));
    if (var_r0_2 == -1) {
        var_r0_2 = (s8) Func_08077178(
            actor_id,
            target_id,
            range_index,
            M2C_FIELD(action, u8, 3),
            M2C_FIELD((u8 *)0x080C2AB8, u8, range_distance)
        );
    }
    action_allowed = (s32) var_r0_2;
    if ((u32) ((M2C_FIELD(action, u8, 3) + 0xCE) << 0x18) > 0x01000000U) {

    } else {
        var_r5 = M2C_FIELD(actor_state, u8, 0x128);
        temp_r8 = Func_080b7514();
        if (M2C_FIELD(action, u8, 3) == 0x33) {
            var_r5 = Func_080c1fa8(M2C_FIELD(battle_state, s32, 0));
        }
        if ((action_allowed != 0) && (Func_080b6cdc(var_r5) != 0) && (temp_r8 >= 0)) {
            temp_r0_4 = Func_080c1df4(var_r5, 1);
            if (0x8000 & temp_r0_4) {
                Func_080c1f50(var_r5);
            }
            Func_08077140(temp_r8, var_r5, 0x7FFF & temp_r0_4);
            temp_r1 = battle_state + 2;
            var_r5_2 = 0;
            var_lr = 0;
            var_r0_3 = 0x64;
            var_r4 = 0;
            if (M2C_FIELD(temp_r1, s16, 0x64) == 0xFE) {
                M2C_FIELD(temp_r1, s16, 0x64) = (s16) temp_r8;
            } else {
loop_41:
                temp_r2_3 = M2C_FIELD(temp_r1, s16, var_r0_3);
                if (temp_r2_3 != 0xFF) {
                    var_r5_2 += 1;
                    var_r0_3 += 2;
                    var_r4 += 2;
                    if (var_r5_2 <= 5) {
                        var_lr = var_r4;
                        if (M2C_FIELD(temp_r1, s16, var_r0_3) == 0xFE) {
                            M2C_FIELD(temp_r1, s16, var_r0_3) = (s16) temp_r8;
                        } else {
                            goto loop_41;
                        }
                    }
                } else {
                    M2C_FIELD(temp_r1, s16, var_r0_3) = (s16) temp_r8;
                    M2C_FIELD(temp_r1, s16, var_lr + 0x66) = temp_r2_3;
                }
            }
            Func_080b7548();
            temp_r0_5 = Func_080b7dd0(temp_r8);
            var_r2_3 = M2C_FIELD(temp_r0_5, s32, 0xC);
            if (var_r2_3 < 0) {
                var_r2_3 += 0xFFFF;
            }
            var_r3 = M2C_FIELD(temp_r0_5, s32, 0x10);
            if (var_r3 < 0) {
                var_r3 += 0xFFFF;
            }
            Func_080b6f44(temp_r0_5, temp_r8, var_r2_3 >> 0x10, var_r3 >> 0x10);
            Func_080b6c90();
            temp_r0_6 = Func_080b6ae0(queued_ids);
            if (temp_r0_6 > 0) {
                var_r6 = queued_ids;
                var_r5_3 = temp_r0_6;
                do {
                    temp_r0_7 = (u16) *var_r6;
                    var_r5_3 -= 1;
                    var_r6++;
                    Func_080b8000((s32) temp_r0_7);
                } while (var_r5_3 != 0);
            }
            Func_080bbabc(0U, (u32) temp_r8);
            if (action_id != 0x1F7) {
                var_r1_3 = 0x8F5;
            } else {
                var_r1_3 = 0x8F3;
            }
            goto block_57;
        }
        if (action_id == 0x1F7) {
            var_r1_3 = 0x8F4;
block_57:
            Func_080bbabc(4U, var_r1_3);
        } else {
            Func_080bbabc(4U, 0x8F6U);
        }
    }
    if (action_allowed != 0) {
        temp_r2_4 = M2C_FIELD(action, u8, 3);
        switch (temp_r2_4) {
        case 0x35:
            action_allowed = 0;
            var_r2_4 = 0;
            if (M2C_FIELD(battle_state, s16, 0x2EC) == target_id) {
                action_allowed = 1;
            } else {
loop_63:
                var_r2_4 += 1;
                if (var_r2_4 <= 0x13U) {
                    if (*(battle_state + ((var_r2_4 * 0x10) + 0x2EC)) == target_id) {
                        action_allowed = 1;
                    } else {
                        goto loop_63;
                    }
                }
            }
            break;
        case 0x23:
            sp34 = 1;
            break;
        case 0x22:
            sp28 = 1;
            break;
        case 0x1B:
            sp1C = 1;
            break;
        case 0x37:
            if (M2C_FIELD(actor_state, s16, 0x38) != 0) {
                Func_080bbabc(0xCU, (u32) actor_id);
            }
            break;
        case 0x20:
            if (M2C_FIELD(target_state, s16, 0x3A) != 0) {
                sp18 = 0xA;
            } else {
                action_allowed = 0;
            }
            break;
        }
    }
    if (sp1C != 0) {

    } else if ((M2C_FIELD(target_state, s16, 0x38) == 0) && (Func_080772b8(M2C_FIELD(action, u8, 3)) == 0)) {

    } else if ((u32) (sp18 + 1) > 0xCU) {

    } else {
        switch (sp18) {
        case 3:
        case 4:
            temp_r1_2 = M2C_FIELD(target_state, u16, 0x3E);
            temp_r6 = M2C_FIELD(target_state, s16, 0x38);
            effect_scale = temp_r1_2;
            if (sp34 != 0) {
                effect_scale = (u16) (temp_r1_2 >> 1);
            }
            var_r8 = 1;
loop_90:
            if (range_index != 4) {
                sp3C = spC - M2C_FIELD((target_state + ((range_index * 4) + 0x48)), s16, 2);
            }
            if (var_r8 == 0) {
                sp3C = 0;
            }
            temp_r5_2 = M2C_FIELD(action, u16, 0xA);
            if (sp18 == 4) {
                var_r0_4 = Func_080022ec(Func_08077180(M2C_FIELD(actor_state, u16, 0x3C), (u32) effect_scale, 0U, sp3C) * temp_r5_2, 0xA);
            } else {
                var_r0_4 = Func_08077180(M2C_FIELD(actor_state, u16, 0x3C), (u32) effect_scale, temp_r5_2, sp3C);
            }
            var_r5_4 = var_r0_4 * sp30;
            if (sp20 != 0) {
                if (sp20 == 1) {
                    var_r0_5 = var_r5_4 * 5;
                    if (var_r0_5 < 0) {
                        var_r0_5 += 3;
                    }
                    var_r5_5 = var_r0_5 >> 2;
                } else {
                    temp_r3_2 = var_r5_4 * 3;
                    var_r5_5 = (s32) (temp_r3_2 + (temp_r3_2 >> 0x1F)) >> 1;
                }
                var_r5_4 = var_r5_5 + Func_080022f4(M2C_FIELD(target_state, u8, 0xF), 5) + 6;
                if (var_r8 == 0) {
                    Func_080bbabc(6U, 0U);
                    var_r1_4 = 0x822;
                    if ((u32) target_id <= 7U) {
                        var_r1_4 = 0x823;
                    }
                    Func_080bbabc(5U, var_r1_4);
                }
            }
            var_r5_6 = var_r5_4 + (3 & Func_080771a0());
            temp_r3_3 = (s8) M2C_FIELD(target_state, u8, 0x12B);
            if (temp_r3_3 != 0) {
                if (temp_r3_3 == 1) {
                    var_r5_6 = (u32) ((s32) (var_r5_6 + (var_r5_6 >> 0x1F)) >> 1);
                } else {
                    var_r5_6 = (u32) Func_080022ec((s32) var_r5_6, 0xA);
                }
            }
            if ((s32) var_r5_6 <= 0) {
                var_r5_6 = 1;
            }
            if (sp28 != 0) {
                temp_r3_4 = temp_r6 - 1;
                if ((s32) var_r5_6 < temp_r3_4) {
                    var_r5_6 = (u32) temp_r3_4;
                    if ((s32) var_r5_6 <= 0) {
                        var_r5_6 = 1;
                    }
                }
            }
            if ((Func_080770c0(0x16E) != 0) && (*sp4 == 5) && ((s32) temp_r6 <= (s32) var_r5_6)) {
                var_r5_6 = temp_r6 - 1;
            }
            var_r8 += 1;
            if (var_r8 <= 1) {
                goto loop_90;
            }
            Func_080bbabc(8U, (u32) target_id);
            Func_080bbabc(0U, (u32) target_id);
            var_r6_2 = temp_r6 - var_r5_6;
            Func_080bbabc(1U, var_r5_6);
            if ((u32) target_id <= 7U) {
                var_r1_5 = sp14 + 0x834;
            } else {
                var_r1_5 = sp14 + 0x831;
            }
            Func_080bbabc(4U, var_r1_5);
            if (var_r6_2 > 0) {
block_245:
                Func_080bbabc(0xBU, (u32) target_id);
            } else {
                Func_080bbabc(9U, (u32) target_id);
                Func_080bbabc(0U, (u32) target_id);
                var_r6_2 = 0;
                if ((u32) target_id > 7U) {
block_243:
                    var_r1_6 = 0x824;
                } else {
                    var_r1_6 = 0x825;
                }
block_244:
                Func_080bbabc(4U, var_r1_6);
            }
block_246:
block_247:
            health_delta = M2C_FIELD(target_state, s16, 0x38) - var_r6_2;
            M2C_FIELD(target_state, s16, 0x38) = (s16) var_r6_2;
            Func_08077128(target_id);
            break;
        case 10:
            if (M2C_FIELD(action, u16, 0xA) == 0) {

            } else {
                temp_r6_2 = M2C_FIELD(target_state, s16, 0x3A);
                if (range_index != 4) {
                    sp3C = spC - M2C_FIELD((target_state + ((range_index * 4) + 0x48)), s16, 2);
                }
                temp_r3_5 = (s8) M2C_FIELD(target_state, u8, 0x12B);
                var_r5_7 = Func_080022ec(
                    M2C_FIELD((u8 *)0x080C2AC0, s32, range_distance * 4) *
                        Func_08077188(M2C_FIELD(action, u16, 0xA), sp3C, 0x100),
                    0x64
                ) * sp30;
                if (temp_r3_5 != 0) {
                    if (temp_r3_5 == 1) {
                        var_r5_7 = (u32) ((s32) (var_r5_7 + (var_r5_7 >> 0x1F)) >> 1);
                    } else {
                        var_r5_7 = (u32) Func_080022ec((s32) var_r5_7, 0xA);
                    }
                }
                if ((M2C_FIELD(action, u8, 3) == 0x20) && ((s32) var_r5_7 > (s32) temp_r6_2)) {
                    var_r5_7 = (u32) temp_r6_2;
                }
                Func_080bbabc(8U, (u32) target_id);
                Func_080bbabc(1U, var_r5_7);
                Func_080bbabc(0U, (u32) target_id);
                if ((u32) target_id <= 7U) {
                    var_r1_7 = 0x82A;
                } else {
                    var_r1_7 = 0x829;
                }
                var_r6_3 = temp_r6_2 - var_r5_7;
                Func_080bbabc(4U, var_r1_7);
                if (var_r6_3 <= 0) {
                    var_r6_3 = 0;
                }
                Func_080bbabc(0xBU, (u32) target_id);
                health_delta = M2C_FIELD(target_state, s16, 0x3A) - var_r6_3;
                M2C_FIELD(target_state, s16, 0x3A) = (s16) var_r6_3;
                Func_08077128(target_id);
            }
            break;
        case 1:
            temp_r3_6 = M2C_FIELD(action, u16, 0xA);
            if (temp_r3_6 == 0) {

            } else {
                temp_r6_3 = M2C_FIELD(target_state, s16, 0x38);
                var_r1_8 = spC;
                if (range_index == 4) {
                    var_r1_8 = 0x64;
                }
                temp_r5_3 = Func_080022ec(
                    M2C_FIELD((u8 *)0x080C2AD8, s32, range_distance * 4) *
                        Func_08077190(temp_r3_6, var_r1_8, 0x100),
                    0x64
                ) * sp30;
                var_r5_8 = temp_r5_3 + (3 & Func_080771a0());
                temp_r3_7 = M2C_FIELD(target_state, s16, 0x34);
                var_r6_2 = temp_r6_3 + var_r5_8;
                if (var_r6_2 > (s32) temp_r3_7) {
                    var_r6_2 = (s32) temp_r3_7;
                    var_r5_8 = var_r6_2 - M2C_FIELD(target_state, s16, 0x38);
                }
                Func_080bbabc(0U, (u32) target_id);
                if (var_r6_2 == M2C_FIELD(target_state, s16, 0x34)) {
                    Func_080bbabc(4U, 0x820U);
                } else {
                    Func_080bbabc(1U, var_r5_8);
                    Func_080bbabc(4U, 0x81DU);
                }
                goto block_247;
            }
            break;
        case -1:
            if (M2C_FIELD(action, u16, 0xA) == 0) {

            } else {
                temp_r6_4 = M2C_FIELD(target_state, s16, 0x3A);
                if (range_index != 4) {
                    sp3C = spC - M2C_FIELD((target_state + ((range_index * 4) + 0x48)), s16, 2);
                }
                temp_r3_8 = (s8) M2C_FIELD(target_state, u8, 0x12B);
                var_r5_9 = Func_080022ec(
                    M2C_FIELD((u8 *)0x080C2AF0, s32, range_distance * 4) *
                        Func_08077188(M2C_FIELD(action, u16, 0xA), sp3C, 0x100),
                    0x64
                ) * sp30;
                if (temp_r3_8 != 0) {
                    if (temp_r3_8 == 1) {
                        var_r5_9 = (u32) ((s32) (var_r5_9 + (var_r5_9 >> 0x1F)) >> 1);
                    } else {
                        var_r5_9 = (u32) Func_080022ec((s32) var_r5_9, 0xA);
                    }
                }
                Func_080bbabc(8U, (u32) target_id);
                Func_080bbabc(1U, var_r5_9);
                Func_080bbabc(0U, (u32) target_id);
                if ((u32) target_id <= 7U) {
                    var_r1_9 = 0x827;
                } else {
                    var_r1_9 = 0x826;
                }
                var_r6_4 = temp_r6_4 - var_r5_9;
                Func_080bbabc(4U, var_r1_9);
                if (var_r6_4 <= 0) {
                    var_r6_4 = 0;
                }
                Func_080bbabc(0xBU, (u32) target_id);
block_226:
                M2C_FIELD(target_state, s16, 0x3A) = (s16) var_r6_4;
                Func_08077128(target_id);
            }
            break;
        case 5:
        case 6:
        case 8:
            if (M2C_FIELD(action, u16, 0xA) == 0) {

            } else {
                temp_r6_5 = M2C_FIELD(target_state, s16, 0x38);
                var_r8_2 = 1;
loop_175:
                if (range_index != 4) {
                    sp3C = spC - M2C_FIELD((target_state + ((range_index * 4) + 0x48)), s16, 2);
                }
                if (var_r8_2 == 0) {
                    sp3C = 0;
                }
                var_r5_10 = M2C_FIELD(action, u16, 0xA);
                if (*sp4 == 6) {
                    switch (action_id) {
                    case 380:
                    case 386:
                    case 392:
                    case 398:
                        sp8 = 3;
                        break;
                    case 381:
                    case 387:
                    case 393:
                    case 399:
                        sp8 = 6;
                        break;
                    case 382:
                    case 388:
                    case 394:
                    case 400:
                        sp8 = 9;
                        break;
                    case 383:
                    case 389:
                    case 395:
                    case 401:
                        sp8 = 0xC;
                        break;
                    }
                    var_r5_10 += Func_080022ec(sp8 * M2C_FIELD(target_state, s16, 0x34), 0x64);
                }
                var_r5_11 = Func_08077188(var_r5_10, sp3C, 0x100) * sp30;
                switch (sp18) {
                case 5:
                    var_r2_5 = 0x080C2B08;
                    var_r3_2 = range_distance * 4;
block_196:
                    var_r5_11 = Func_080022ec(
                        M2C_FIELD((u8 *)var_r2_5, s32, var_r3_2) * var_r5_11,
                        0x64
                    );
                    break;
                case 8:
                    var_r2_5 = 0x080C2B20;
                    var_r3_2 = range_distance * 4;
                    goto block_196;
                case 6:
                    var_r2_5 = 0x080C2B38;
                    var_r3_2 = range_distance * 4;
                    goto block_196;
                }
                var_r5_12 = var_r5_11 + (3 & Func_080771a0());
                temp_r3_9 = (s8) M2C_FIELD(target_state, u8, 0x12B);
                if (temp_r3_9 != 0) {
                    if (temp_r3_9 == 1) {
                        var_r5_12 = (u32) ((s32) (var_r5_12 + (var_r5_12 >> 0x1F)) >> 1);
                    } else {
                        var_r5_12 = (u32) Func_080022ec((s32) var_r5_12, 0xA);
                    }
                }
                if ((Func_080770c0(0x16E) != 0) && (*sp4 == 6) && ((s32) temp_r6_5 > (s32) var_r5_12)) {
                    var_r5_12 = (u32) temp_r6_5;
                }
                var_r8_2 += 1;
                if (var_r8_2 <= 1) {
                    goto loop_175;
                }
                Func_080bbabc(8U, (u32) target_id);
                Func_080bbabc(1U, var_r5_12);
                Func_080bbabc(0U, (u32) target_id);
                if ((u32) target_id <= 7U) {
                    var_r1_10 = sp14 + 0x834;
                } else {
                    var_r1_10 = sp14 + 0x831;
                }
                var_r6_2 = temp_r6_5 - var_r5_12;
                Func_080bbabc(4U, var_r1_10);
                if (var_r6_2 <= 0) {
                    Func_080bbabc(9U, (u32) target_id);
                    Func_080bbabc(0U, (u32) target_id);
                    var_r6_2 = 0;
                    if ((u32) target_id <= 7U) {
                        var_r1_11 = 0x825;
                    } else {
                        var_r1_11 = 0x824;
                    }
                    Func_080bbabc(4U, var_r1_11);
                } else {
                    Func_080bbabc(0xBU, (u32) target_id);
                }
                goto block_247;
            }
            break;
        case 11:
            temp_r3_10 = M2C_FIELD(action, u16, 0xA);
            if (temp_r3_10 == 0) {

            } else {
                temp_r6_6 = M2C_FIELD(target_state, s16, 0x3A);
                var_r1_12 = spC;
                if (range_index == 4) {
                    var_r1_12 = 0x64;
                }
                var_r5_13 = Func_080022ec(
                    M2C_FIELD((u8 *)0x080C2B50, s32, range_distance * 4) *
                        Func_08077190(temp_r3_10, var_r1_12, 0x100),
                    0x64
                ) * sp30;
                temp_r3_11 = M2C_FIELD(target_state, s16, 0x36);
                var_r6_4 = temp_r6_6 + var_r5_13;
                if (var_r6_4 > (s32) temp_r3_11) {
                    var_r6_4 = (s32) temp_r3_11;
                    var_r5_13 = var_r6_4 - M2C_FIELD(target_state, s16, 0x3A);
                }
                Func_080bbabc(0U, (u32) target_id);
                if (var_r6_4 == M2C_FIELD(target_state, s16, 0x36)) {
                    Func_080bbabc(4U, 0x821U);
                } else {
                    Func_080bbabc(1U, var_r5_13);
                    Func_080bbabc(4U, 0x81EU);
                }
                goto block_226;
            }
            break;
        case 2:
            if (action_allowed == 0) {
                Func_080bbabc(0xBU, (u32) target_id);
                Func_080bbabc(0U, (u32) target_id);
                Func_080bbabc(4U, 0x854U);
            } else if (M2C_FIELD(action, u16, 0xA) == 0) {

            } else {
                temp_r6_7 = M2C_FIELD(target_state, s16, 0x38);
                if (range_index != 4) {
                    sp3C = spC - M2C_FIELD((target_state + ((range_index * 4) + 0x48)), s16, 2);
                }
                var_r5_14 = Func_080022ec(
                    M2C_FIELD((u8 *)0x080C2B68, s32, range_distance * 4) *
                        (Func_08077188(M2C_FIELD(action, u16, 0xA), sp3C, 0x100) * sp30),
                    0x64
                );
                temp_r3_12 = (s8) M2C_FIELD(target_state, u8, 0x12B);
                if (temp_r3_12 != 0) {
                    if (temp_r3_12 == 1) {
                        var_r5_14 = (s32) (var_r5_14 + ((u32) var_r5_14 >> 0x1F)) >> 1;
                    } else {
                        var_r5_14 = Func_080022ec(var_r5_14, 0xA);
                    }
                }
                Func_080bbabc(8U, (u32) target_id);
                Func_080bbabc(1U, (u32) var_r5_14);
                Func_080bbabc(0U, (u32) target_id);
                if ((u32) target_id <= 7U) {
                    var_r1_13 = 0x827;
                } else {
                    var_r1_13 = 0x826;
                }
                var_r6_2 = temp_r6_7 - var_r5_14;
                Func_080bbabc(4U, var_r1_13);
                if (var_r6_2 <= 0) {
                    Func_080bbabc(9U, (u32) target_id);
                    Func_080bbabc(0U, (u32) target_id);
                    var_r6_2 = 0;
                    if ((u32) target_id <= 7U) {
                        var_r1_6 = 0x825;
                    } else {
                        goto block_243;
                    }
                    goto block_244;
                }
                goto block_245;
                goto block_246;
            }
            break;
        }
    }
    Func_080bbabc(0U, (u32) target_id);
    if ((Func_080772b8(M2C_FIELD(action, u8, 3)) == 0) && (M2C_FIELD(target_state, s16, 0x38) == 0) && (Func_080bbae8((s32) M2C_FIELD(action, u8, 3)) == 0)) {
        goto finalize;
    }
    if (action_allowed == 0) {
        goto finalize;
    }
    if ((u32) (M2C_FIELD(action, u8, 3) - 3) > 0x42U) {
        goto finalize;
    }
    switch ((s32) M2C_FIELD(action, u8, 3) - 3) {
    case 0x3D:
        if (M2C_FIELD(target_state, u8, 0x138) != 0) {
            M2C_FIELD(target_state, u8, 0x138) = 0U;
            Func_080bbabc(4U, 0x88BU);
            Func_080bbabc(0U, (u32) target_id);
        }
        if (M2C_FIELD(target_state, u8, 0x13B) != 0) {
            M2C_FIELD(target_state, u8, 0x13B) = 0U;
            Func_080bbabc(7U, 0U);
            Func_080bbabc(0U, (u32) target_id);
            Func_080bbabc(4U, 0x88DU);
        }
        M2C_FIELD(target_state, u8, 0x13C) = 0U;
        if (M2C_FIELD(target_state, u8, 0x13D) != 0) {
            M2C_FIELD(target_state, u8, 0x13D) = 0U;
            Func_080bbabc(4U, 0x88CU);
            Func_080bbabc(0U, (u32) target_id);
        }
        if (M2C_FIELD(target_state, u8, 0x141) != 0) {
            M2C_FIELD(target_state, u8, 0x141) = 0U;
            Func_080bbabc(4U, 0x894U);
            Func_080bbabc(0U, (u32) target_id);
        }
        if (M2C_FIELD(target_state, u8, 0x140) != 0) {
            M2C_FIELD(target_state, u8, 0x140) = 0U;
            Func_080bbabc(4U, 0x88FU);
            Func_080bbabc(0U, (u32) target_id);
        }
        if (M2C_FIELD(target_state, s8, 0x131) != 0) {
            Func_080bbabc(4U, 0x884U);
            M2C_FIELD(target_state, s8, 0x131) = 0;
        }
        Func_080bbabc(7U, 0U);
        goto finalize;

    case 0x1:
        if (M2C_FIELD(target_state, u8, 0x138) != 0) {
            M2C_FIELD(target_state, u8, 0x138) = 0U;
            Func_080bbabc(7U, 0U);
            Func_080bbabc(0U, (u32) target_id);
            Func_080bbabc(4U, 0x88BU);
        }
        if (M2C_FIELD(target_state, u8, 0x13B) != 0) {
            M2C_FIELD(target_state, u8, 0x13B) = 0U;
            Func_080bbabc(7U, 0U);
            Func_080bbabc(0U, (u32) target_id);
            Func_080bbabc(4U, 0x88DU);
        }
        M2C_FIELD(target_state, u8, 0x13C) = 0U;
        if (M2C_FIELD(target_state, u8, 0x13D) != 0) {
            M2C_FIELD(target_state, u8, 0x13D) = 0U;
            Func_080bbabc(7U, 0U);
            Func_080bbabc(0U, (u32) target_id);
            Func_080bbabc(4U, 0x88CU);
        }
        if (M2C_FIELD(target_state, u8, 0x141) == 0) {
            goto finalize;
        }
        M2C_FIELD(target_state, u8, 0x141) = 0;
        Func_080bbabc(7U, 0U);
        Func_080bbabc(0U, (u32) target_id);
        Func_080bbabc(4U, 0x894U);
        goto finalize;

    case 0x3A:
    case 0x3B:
        temp_r5_4 = M2C_FIELD(target_state, s16, 0x38);
        temp_r8_2 = (u16) M2C_FIELD(target_state, s16, 0x38);
        temp_r6_8 = (u16) M2C_FIELD(target_state, s16, 0x34);
        temp_r2_5 = M2C_FIELD(target_state, s16, 0x34);
        if (M2C_FIELD(action, u8, 3) == 0x3D) {
            var_r0_6 = temp_r2_5 * 0x3C;
        } else {
            var_r0_6 = temp_r2_5 * 0x1E;
        }
        var_r5_15 = temp_r5_4 + Func_080022ec(var_r0_6, 0x64);
        temp_r2_6 = (s16) temp_r6_8;
        if (var_r5_15 > (s32) temp_r2_6) {
            var_r5_15 = (s32) temp_r2_6;
        }
        var_r1_14 = var_r5_15 - (s16) temp_r8_2;
        if ((var_r1_14 == 0) && (sp18 != 1)) {
            goto finalize;
        }
        if (var_r5_15 == (s32) temp_r2_6) {
            Func_080bbabc(4U, 0x820U);
        } else {
            Func_080bbabc(1U, var_r1_14);
            Func_080bbabc(4U, 0x81DU);
        }
        M2C_FIELD(target_state, s16, 0x38) = (s16) var_r5_15;
block_402:
        Func_08077128(target_id);
        break;
    case 0x3C:
        temp_r6_9 = M2C_FIELD(target_state, s16, 0x36);
        temp_r5_5 = M2C_FIELD(target_state, s16, 0x3A);
        var_r5_16 = temp_r5_5 + Func_080022ec(temp_r6_9 * 7, 0x64);
        if (var_r5_16 > (s32) temp_r6_9) {
            var_r5_16 = (s32) temp_r6_9;
        }
        var_r1_15 = var_r5_16 - temp_r5_5;
        if ((var_r1_15 == 0) && (sp18 != 0xB)) {
            goto finalize;
        }
        if (var_r5_16 == temp_r6_9) {
            Func_080bbabc(4U, 0x821U);
        } else {
            Func_080bbabc(1U, var_r1_15);
            Func_080bbabc(4U, 0x81EU);
        }
        M2C_FIELD(target_state, s16, 0x3A) = (s16) var_r5_16;
        goto block_402;
    case 0x38:
        M2C_FIELD(target_state, s8, 0x147) = 8;
        M2C_FIELD(target_state, s8, 0x146) = 5;
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_state, u16, 0x40) - M2C_FIELD(target_snapshot, u16, 0x40));
        Func_080bbabc(4U, 0x877U);
        goto finalize;

    case 0x37:
        M2C_FIELD(target_state, s8, 0x147) = -4;
        M2C_FIELD(target_state, s8, 0x146) = 5;
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_snapshot, u16, 0x40) - M2C_FIELD(target_state, u16, 0x40));
        Func_080bbabc(4U, 0x878U);
        goto finalize;

    case 0x6:
        temp_r3_13 = M2C_FIELD(target_state, u8, 0x133) - 1;
        M2C_FIELD(target_state, u8, 0x133) = temp_r3_13;
        if ((s32) (s8) temp_r3_13 < -4) {
            M2C_FIELD(target_state, u8, 0x133) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 4) {
            M2C_FIELD(target_state, u8, 0x133) = 4U;
        }
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_snapshot, u16, 0x3C) - M2C_FIELD(target_state, u16, 0x3C));
        Func_080bbabc(4U, 0x860U);
        var_r2_6 = target_state + 0x132;
block_405:
        *var_r2_6 = 7;
        break;
    case 0x5:
        temp_r3_14 = M2C_FIELD(target_state, u8, 0x133) - 2;
        M2C_FIELD(target_state, u8, 0x133) = temp_r3_14;
        if ((s32) (s8) temp_r3_14 < -4) {
            M2C_FIELD(target_state, u8, 0x133) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 4) {
            M2C_FIELD(target_state, u8, 0x133) = 4U;
        }
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_snapshot, u16, 0x3C) - M2C_FIELD(target_state, u16, 0x3C));
        Func_080bbabc(4U, 0x860U);
        var_r2_6 = target_state + 0x132;
        goto block_405;
    case 0x4:
        temp_r3_15 = M2C_FIELD(target_state, u8, 0x133) + 1;
        M2C_FIELD(target_state, u8, 0x133) = temp_r3_15;
        if ((s32) (s8) temp_r3_15 < -4) {
            M2C_FIELD(target_state, u8, 0x133) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 4) {
            M2C_FIELD(target_state, u8, 0x133) = 4U;
        }
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_state, u16, 0x3C) - M2C_FIELD(target_snapshot, u16, 0x3C));
        Func_080bbabc(4U, 0x861U);
        var_r0_7 = 0x99;
block_404:
        var_r2_6 = target_state + (var_r0_7 * 2);
        goto block_405;
    case 0x3:
        temp_r3_16 = M2C_FIELD(target_state, u8, 0x133) + 2;
        M2C_FIELD(target_state, u8, 0x133) = temp_r3_16;
        if ((s32) (s8) temp_r3_16 < -4) {
            M2C_FIELD(target_state, u8, 0x133) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 4) {
            M2C_FIELD(target_state, u8, 0x133) = 4U;
        }
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_state, u16, 0x3C) - M2C_FIELD(target_snapshot, u16, 0x3C));
        Func_080bbabc(4U, 0x861U);
        var_r2_6 = target_state + 0x132;
        goto block_405;
    case 0xA:
        temp_r3_17 = M2C_FIELD(target_state, u8, 0x135) - 1;
        M2C_FIELD(target_state, u8, 0x135) = temp_r3_17;
        if ((s32) (s8) temp_r3_17 < -4) {
            M2C_FIELD(target_state, u8, 0x135) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 4) {
            M2C_FIELD(target_state, u8, 0x135) = 4U;
        }
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_snapshot, u16, 0x3E) - M2C_FIELD(target_state, u16, 0x3E));
        Func_080bbabc(4U, 0x862U);
        var_r2_6 = target_state + 0x134;
        goto block_405;
    case 0x9:
        temp_r3_18 = M2C_FIELD(target_state, u8, 0x135) - 2;
        M2C_FIELD(target_state, u8, 0x135) = temp_r3_18;
        if ((s32) (s8) temp_r3_18 < -4) {
            M2C_FIELD(target_state, u8, 0x135) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 4) {
            M2C_FIELD(target_state, u8, 0x135) = 4U;
        }
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_snapshot, u16, 0x3E) - M2C_FIELD(target_state, u16, 0x3E));
        Func_080bbabc(4U, 0x862U);
        var_r0_7 = 0x9A;
        goto block_404;
    case 0x8:
        temp_r3_19 = M2C_FIELD(target_state, u8, 0x135) + 1;
        M2C_FIELD(target_state, u8, 0x135) = temp_r3_19;
        if ((s32) (s8) temp_r3_19 < -4) {
            M2C_FIELD(target_state, u8, 0x135) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 4) {
            M2C_FIELD(target_state, u8, 0x135) = 4U;
        }
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_state, u16, 0x3E) - M2C_FIELD(target_snapshot, u16, 0x3E));
        Func_080bbabc(4U, 0x863U);
        var_r2_6 = target_state + 0x134;
        goto block_405;
    case 0x7:
        temp_r3_20 = M2C_FIELD(target_state, u8, 0x135) + 2;
        M2C_FIELD(target_state, u8, 0x135) = temp_r3_20;
        if ((s32) (s8) temp_r3_20 < -4) {
            M2C_FIELD(target_state, u8, 0x135) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 4) {
            M2C_FIELD(target_state, u8, 0x135) = 4U;
        }
        Func_08077010(target_id);
        Func_080bbabc(1U, M2C_FIELD(target_state, u16, 0x3E) - M2C_FIELD(target_snapshot, u16, 0x3E));
        Func_080bbabc(4U, 0x863U);
        var_r2_6 = target_state + 0x134;
        goto block_405;
    case 0x2:
        if (M2C_FIELD(target_state, s16, 0x38) != 0) {

        } else {
            Func_080bbabc(4U, 0x864U);
            M2C_FIELD(target_state, s16, 0x38) = (s16) (u16) M2C_FIELD(target_state, s16, 0x34);
            Func_08077128(target_id);
        }
        break;
    case 0x35:
        if (M2C_FIELD(target_state, s16, 0x38) != 0) {

        } else {
            Func_080bbabc(4U, 0x864U);
            temp_r3_21 = (u16) M2C_FIELD(target_state, s16, 0x34);
            M2C_FIELD(target_state, s16, 0x38) = (s16) ((s32) ((s16) temp_r3_21 + ((u32) (temp_r3_21 << 0x10) >> 0x1F)) >> 1);
            goto block_402;
        }
        break;
    case 0x36:
        if (M2C_FIELD(target_state, s16, 0x38) != 0) {

        } else {
            Func_080bbabc(4U, 0x864U);
            M2C_FIELD(target_state, s16, 0x38) = (s16) Func_080022ec(M2C_FIELD(target_state, s16, 0x34) * 8, 0xA);
            goto block_402;
        }
        break;
    case 0x0:
        if (M2C_FIELD(target_state, s8, 0x131) != 0) {
            Func_080bbabc(4U, 0x884U);
        }
        M2C_FIELD(target_state, s8, 0x131) = 0;
        break;
    case 0xE:
        temp_r3_22 = M2C_FIELD(target_state, u8, 0x137) - 1;
        M2C_FIELD(target_state, u8, 0x137) = temp_r3_22;
        if ((s32) (s8) temp_r3_22 < -4) {
            M2C_FIELD(target_state, u8, 0x137) = 0xFCU;
        }
        var_r1_16 = M2C_FIELD(target_state, u8, 0x137);
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x137) > 4) {
            M2C_FIELD(target_state, u8, 0x137) = 4U;
            var_r1_16 = 4;
        }
        Func_080bbabc(1U, (M2C_FIELD(target_snapshot, s8, 0x137) - (s8) var_r1_16) * 0x14);
        Func_080bbabc(4U, 0x865U);
        var_r0_7 = 0x9B;
        goto block_404;
    case 0xD:
        temp_r3_23 = M2C_FIELD(target_state, u8, 0x137) - 2;
        M2C_FIELD(target_state, u8, 0x137) = temp_r3_23;
        if ((s32) (s8) temp_r3_23 < -4) {
            M2C_FIELD(target_state, u8, 0x137) = 0xFCU;
        }
        var_r1_17 = M2C_FIELD(target_state, u8, 0x137);
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x137) > 4) {
            M2C_FIELD(target_state, u8, 0x137) = 4U;
            var_r1_17 = 4;
        }
        Func_080bbabc(1U, (M2C_FIELD(target_snapshot, s8, 0x137) - (s8) var_r1_17) * 0x14);
        Func_080bbabc(4U, 0x865U);
        var_r0_7 = 0x9B;
        goto block_404;
    case 0xC:
        temp_r3_24 = M2C_FIELD(target_state, u8, 0x137) + 1;
        M2C_FIELD(target_state, u8, 0x137) = temp_r3_24;
        if ((s32) (s8) temp_r3_24 < -4) {
            M2C_FIELD(target_state, u8, 0x137) = 0xFCU;
        }
        var_r1_18 = M2C_FIELD(target_state, u8, 0x137);
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x137) > 4) {
            M2C_FIELD(target_state, u8, 0x137) = 4U;
            var_r1_18 = 4;
        }
        Func_080bbabc(1U, ((s8) var_r1_18 - (s8) (u8) M2C_FIELD(target_snapshot, s8, 0x137)) * 0x14);
        Func_080bbabc(4U, 0x866U);
        var_r0_7 = 0x9B;
        goto block_404;
    case 0xB:
        temp_r3_25 = M2C_FIELD(target_state, u8, 0x137) + 2;
        M2C_FIELD(target_state, u8, 0x137) = temp_r3_25;
        if ((s32) (s8) temp_r3_25 < -4) {
            M2C_FIELD(target_state, u8, 0x137) = 0xFCU;
        }
        var_r1_19 = M2C_FIELD(target_state, u8, 0x137);
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x137) > 4) {
            M2C_FIELD(target_state, u8, 0x137) = 4U;
            var_r1_19 = 4;
        }
        Func_080bbabc(1U, ((s8) var_r1_19 - (s8) (u8) M2C_FIELD(target_snapshot, s8, 0x137)) * 0x14);
        Func_080bbabc(4U, 0x866U);
        var_r0_7 = 0x9B;
        goto block_404;
    case 0xF:
        if (M2C_FIELD(target_state, s8, 0x131) != 0) {

        } else {
            Func_080bbabc(4U, 0x867U);
            M2C_FIELD(target_state, s8, 0x131) = 1;
        }
        break;
    case 0x10:
        if ((s32) M2C_FIELD(target_state, s8, 0x131) > 1) {

        } else {
            Func_080bbabc(4U, 0x874U);
            M2C_FIELD(target_state, s8, 0x131) = 2;
        }
        break;
    case 0x11:
        Func_080bbabc(4U, 0x868U);
        var_r2_6 = target_state + 0x138;
        goto block_405;
    case 0x12:
        Func_080bbabc(4U, 0x869U);
        M2C_FIELD(target_state, s8, 0x139) = 7;
        break;
    case 0x13:
        Func_080bbabc(4U, 0x86AU);
        var_r2_6 = target_state + 0x13A;
        goto block_405;
    case 0x14:
        Func_080bbabc(4U, 0x86BU);
        var_r2_6 = target_state + 0x13B;
        goto block_405;
    case 0x15:
        Func_080bbabc(4U, 0x86CU);
        var_r0_7 = 0x9E;
        goto block_404;
    case 0x16:
        if ((u32) target_id <= 7U) {
            Func_080bbabc(4U, 0x86DU);
        } else {
            Func_080bbabc(4U, 0x876U);
        }
        var_r1_20 = target_state + 0x13D;
        var_r3_3 = 7;
block_394:
        *var_r1_20 = var_r3_3 | M2C_FIELD(target_state, u8, 0x13D);
        break;
    case 0x40:
        if ((u32) target_id <= 7U) {
            Func_080bbabc(4U, 0x86DU);
        } else {
            Func_080bbabc(4U, 0x876U);
        }
        var_r1_20 = target_state + 0x13D;
        var_r3_3 = 0x10;
        goto block_394;
    case 0x18:
        Func_080bbabc(9U, (u32) target_id);
        if (M2C_FIELD(target_state, u8, 0x12A) == 2) {
            var_r1_21 = 0x84F;
            goto block_399;
        }
        if (action_id == 0xDB) {
            var_r1_21 = 0x850;
block_399:
            Func_080bbabc(4U, var_r1_21);
        } else {
            Func_080bbabc(4U, 0x84CU);
        }
        M2C_FIELD(target_state, s16, 0x38) = 0;
        goto block_402;
    case 0x1A:
        Func_080bbabc(4U, 0x86FU);
        var_r0_7 = 0x9F;
        goto block_404;
    case 0x1B:
        Func_080bbabc(4U, 0x870U);
        M2C_FIELD(target_state, s8, 0x13F) = 7;
        break;
    case 0x1C:
    case 0x39:
        temp_r6_10 = M2C_FIELD(actor_state, s16, 0x38);
        var_r5_17 = health_delta;
        if (M2C_FIELD(action, u8, 3) == 0x3C) {
            var_r5_17 = (u32) ((s32) (var_r5_17 + (var_r5_17 >> 0x1F)) >> 1);
        }
        temp_r3_26 = M2C_FIELD(actor_state, s16, 0x34);
        var_r6_5 = temp_r6_10 + var_r5_17;
        if (var_r6_5 > (s32) temp_r3_26) {
            var_r6_5 = (s32) temp_r3_26;
            var_r5_17 = var_r6_5 - temp_r6_10;
        }
        Func_080bbabc(7U, 0U);
        Func_080bbabc(0U, (u32) actor_id);
        if (var_r6_5 == M2C_FIELD(actor_state, s16, 0x34)) {
            Func_080bbabc(4U, 0x820U);
        } else {
            Func_080bbabc(1U, var_r5_17);
            Func_080bbabc(4U, 0x81DU);
        }
        M2C_FIELD(actor_state, s16, 0x38) = (s16) var_r6_5;
block_421:
        Func_08077128((u8) actor_id);
        break;
    case 0x1D:
        temp_r5_6 = M2C_FIELD(actor_state, s16, 0x3A);
        var_r6_6 = health_delta;
        temp_r3_27 = M2C_FIELD(actor_state, s16, 0x36);
        var_r5_18 = temp_r5_6 + var_r6_6;
        if (var_r5_18 > (s32) temp_r3_27) {
            var_r5_18 = (s32) temp_r3_27;
            var_r6_6 = var_r5_18 - temp_r5_6;
        }
        Func_080bbabc(7U, 0U);
        Func_080bbabc(0U, (u32) actor_id);
        if (var_r5_18 == M2C_FIELD(actor_state, s16, 0x36)) {
            Func_080bbabc(4U, 0x821U);
        } else {
            Func_080bbabc(1U, var_r6_6);
            Func_080bbabc(4U, 0x81EU);
        }
        M2C_FIELD(actor_state, s16, 0x3A) = (s16) var_r5_18;
        goto block_421;
    case 0x42:
        var_r5_19 = Func_080022ec((s32) health_delta, 0xA);
        temp_r3_28 = M2C_FIELD(target_state, s16, 0x3A);
        if ((s32) temp_r3_28 < var_r5_19) {
            var_r5_19 = (s32) temp_r3_28;
        }
        temp_r1_3 = M2C_FIELD(actor_state, s16, 0x3A);
        temp_r2_7 = M2C_FIELD(actor_state, s16, 0x36);
        if ((s32) (temp_r1_3 + var_r5_19) > (s32) temp_r2_7) {
            var_r5_19 = temp_r2_7 - temp_r1_3;
        }
        if (var_r5_19 == 0) {

        } else {
            Func_080bbabc(1U, (u32) var_r5_19);
            if ((u32) target_id <= 7U) {
                Func_080bbabc(4U, 0x85FU);
            } else {
                Func_080bbabc(4U, 0x85EU);
            }
            Func_08077120(actor_id, var_r5_19);
        }
        break;
    case 0x1E:
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 0) {
            M2C_FIELD(target_state, u8, 0x133) = 0U;
            M2C_FIELD(target_state, s8, 0x132) = 0;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 0) {
            M2C_FIELD(target_state, u8, 0x135) = 0U;
            M2C_FIELD(target_state, s8, 0x134) = 0;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x137) > 0) {
            M2C_FIELD(target_state, u8, 0x137) = 0U;
            M2C_FIELD(target_state, s8, 0x136) = 0;
        }
        if ((s32) M2C_FIELD(target_state, s8, 0x147) > 0) {
            M2C_FIELD(target_state, s8, 0x147) = 0;
        }
        M2C_FIELD(target_state, s8, 0x12C) = 0;
        M2C_FIELD(target_state, s8, 0x12D) = 0;
        M2C_FIELD(target_state, s8, 0x12E) = 0;
        M2C_FIELD(target_state, s8, 0x12F) = 0;
        var_r1_22 = 0x896;
block_446:
        Func_080bbabc(4U, var_r1_22);
        break;
    case 0x17:
        Func_080bbabc(4U, 0x872U);
        var_r2_7 = target_state + 0x140;
block_452:
        *var_r2_7 = 1;
        break;
    case 0x19:
        temp_r3_29 = M2C_FIELD(target_state, u8, 0x141);
        if (temp_r3_29 == 0) {
            Func_080bbabc(4U, 0x873U);
            M2C_FIELD(target_state, u8, 0x141) = 7U;
        } else if ((u32) temp_r3_29 > 1U) {
            M2C_FIELD(target_state, u8, 0x141) = (u8) (temp_r3_29 + 0xFF);
            Func_080bbabc(1U, (u32) M2C_FIELD(target_state, u8, 0x141));
            var_r1_22 = 0x875;
            goto block_446;
        }
        break;
    case 0x3F:
        Func_080bbabc(4U, 0x87DU);
        var_r2_8 = target_state + 0x144;
block_454:
        *var_r2_8 = 2;
        break;
    case 0x33:
        Func_080bbabc(4U, 0x87EU);
        M2C_FIELD(target_state, s8, 0x148) = 1;
        if ((u32) target_id <= 7U) {
            M2C_FIELD(battle_state, u8, 0x43) = (u8) (M2C_FIELD(battle_state, u8, 0x43) | 2);
        }
        break;
    case 0x32:
        Func_080bbabc(4U, 0x87FU);
        var_r2_7 = target_state + 0x145;
        goto block_452;
    case 0x2B:
        Func_080bbabc(4U, 0x881U);
        var_r2_7 = target_state + 0x12B;
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x12B) <= 0) {
            goto block_452;
        }
        break;
    case 0x2C:
        Func_080bbabc(4U, 0x882U);
        var_r2_8 = target_state + 0x12B;
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x12B) <= 1) {
            goto block_454;
        }
        break;
    case 0x2A:
        Func_080bbabc(4U, -1U);
        break;
    }
finalize:
    Func_080bbabc(7U, 0U);
    if (M2C_FIELD(target_state, s16, 0x38) != 0) {
        temp_r3_30 = M2C_FIELD(target_state, u8, 0x13C);
        if ((temp_r3_30 != 0) && ((u32) temp_r3_30 <= 6U) && ((s32) health_delta > 0)) {
            temp_r0_8 = Func_080771a0() & 3;
            if (temp_r0_8 == 0) {
                M2C_FIELD(target_state, u8, 0x13C) = temp_r0_8;
                Func_080bbabc(0U, (u32) target_id);
                Func_080bbabc(4U, 0x883U);
            }
        }
    }
    Func_08002df0((s16 *) target_snapshot);
    Func_08077010(target_id);
    Func_08015130(M2C_FIELD(*(void **)0x03001E74, u8, 0x41));
    if (M2C_FIELD(target_state, s16, 0x38) != 0) {
        Func_080bbabc(0xBU, (u32) target_id);
    }
    var_r0 = 0x140;
    if (M2C_FIELD(actor_state, u8, 0x140) != 0) {
        var_r0 = Func_080771a0() & 3;
        if ((var_r0 == 0) && ((s32) health_delta > 0)) {
            var_r0 = (s32) health_delta >> 2;
            if (var_r0 == 0) {
                var_r0 = 1;
            }
            M2C_FIELD(request, s32, 0x60) = (s32) (M2C_FIELD(request, s32, 0x60) + var_r0);
        }
    }
    return var_r0;
}
