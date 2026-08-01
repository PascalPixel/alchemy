typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08077058(s32, s32);
void *Func_08077008(s32);
void *Func_08077018(s32);
s32 Func_0808ddec(s32);
void *Func_0808e14c(s32);
void Func_08096fb0(u16, s32);
void Func_08097194(void);

/*
 * __call_via_rN veneer site, resolved per-site against the ROM.
 *
 * The `bl Func_080072f0` at 0x0808e36c is `__call_via_r3` -- an indirect call
 * through r3, and the draft's fourth argument is the callee.
 *
 * WHERE r3 COMES FROM, and a warning about the tool. `tools/veneer_resolve.ts`
 * reports this site as the RETURN of the `bl 0x08091750` at 0x0808e360. That
 * is wrong, and wrong in an instructive way: 0x0808e360 is the LINEAR
 * predecessor, not the control-flow predecessor. The instruction after it,
 * `b.n 0x0808e370` at 0x0808e364, branches PAST the veneer call entirely. The
 * only way to reach 0x0808e366 is the `bge.n 0x808e366` at 0x0808e33c, and on
 * that path r3 was set two instructions earlier by `ldr r3, [r6, #8]` at
 * 0x0808e336 and nothing writes it again.
 *
 * So the callee is the owner record's field at +8. The guard is the same
 * shape found at 0x0808d9a4: `cmp r3, #0x10000; bge` -- small values are
 * packed script ids handled by the 0x08091750 path, large values are
 * addresses called directly. Same offset, same tagged-word discipline.
 *
 * Two instruments agree here: the branch-target scan above, and this file's
 * own reconstructed control flow, which already placed the call in the
 * `temp_r3_3 >= 0x10000` arm.
 *
 * ARITY: three. r0 = fp, r1 = sl, r2 = [sp, #4], all set in the block at
 * 0x0808e366; callee in the r3 slot, which is why the draft read it as a
 * fourth argument.
 *
 * Resolves an item owner and dispatches its field-use or scripted behavior.
 */

/// An owner record's +8 field when it holds an address rather than a packed
/// script id.
typedef void (*OwnerHandler)(s32 itemId, s32 slot, s32 arg);
s32 Func_0808e23c(s32 arg0, s32 arg1) {
    s32 sp0;
    s32 sp4;
    s32 temp_r0;
    void *temp_r0_2;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r5;
    s32 temp_r6;
    s32 temp_r8;
    s32 var_r0;
    s32 var_r1;
    s32 var_r1_2;
    s32 var_r4;
    s32 var_r6;
    s32 var_r7;
    s32 var_sl;
    u16 *var_r0_2;
    u16 *var_r0_3;
    u16 temp_r2;
    u16 temp_r5_2;
    u8 *var_r5;
    void *temp_r8_2;

    sp0 = -1;
    temp_r2 = 0x3FF & arg0;
    temp_r0 = (arg0 >> 0xA) & 0xF;
    var_sl = temp_r0;
    sp4 = arg1;
    var_r7 = 0;
    temp_r8 = Func_08077148(temp_r0);
    if (var_sl == 0xF) {
        var_sl = 0;
        var_r6 = 0;
        if (temp_r8 > 0) {
            var_r5 = (u8 *)0x02000438;
            do {
                var_r4 = 0;
                var_r0_2 = (u16 *)((u8 *)Func_08077008((s32)*var_r5) + 0xD8);
                var_r1 = 0xE;
loop_4:
                temp_r3 = 0x1FF & *var_r0_2;
                var_r0_2 += 2;
                if (temp_r3 == temp_r2) {
                    var_r4 += 1;
                }
                var_r1 -= 1;
                if (var_r1 >= 0) {
                    goto loop_4;
                }
                if (var_r7 < var_r4) {
                    var_r7 = var_r4;
                    var_sl = (s32) *var_r5;
                }
                var_r6 += 1;
                var_r5 += 1;
            } while (var_r6 < temp_r8);
        }
    } else {
        var_r0_3 = (u16 *)((u8 *)Func_08077008(var_sl) + 0xD8);
        var_r1_2 = 0xE;
        do {
            temp_r3_2 = 0x1FF & *var_r0_3;
            var_r0_3 += 2;
            if (temp_r3_2 == temp_r2) {
                var_r7 += 1;
            }
            var_r1_2 -= 1;
        } while (var_r1_2 >= 0);
    }
    if (var_r7 == 0) {
        Func_08015040((void *)0x927, 1);
        return -1;
    }
    temp_r0_2 = Func_0808e14c(temp_r2);
    if ((temp_r0_2 != 0) && (M2C_FIELD(temp_r0_2, s32 *, 8) != 0)) {
        Func_080770d0(0x143);
        Func_080770d0(0x142);
        if (!(0x400 & M2C_FIELD(temp_r0_2, u16 *, 4))) {
            Func_08015120(var_sl, 1);
            Func_08015120((s32) temp_r2, 2);
            Func_08015040((void *)0x91C, 1);
        }
        temp_r3_3 = M2C_FIELD(temp_r0_2, s32 *, 8);
        if (temp_r3_3 < 0x10000) {
            temp_r5 = Func_0808ddec(*(s32 *)0x02000434);
            Func_080916b0();
            Func_08092b94((s16) M2C_FIELD(temp_r0_2, s32 *, 8));
            Func_08092f84(temp_r5, 0);
            Func_08091750();
        } else {
            ((OwnerHandler) temp_r3_3)((s32) temp_r2, var_sl, sp4);
        }
        sp0 = 0;
        goto block_32;
    }
    Func_080770d0(0x143);
    Func_080770c8(0x142);
    temp_r5_2 = M2C_FIELD(Func_08077018((s32) temp_r2), u16 *, 0x28);
    temp_r8_2 = *(void **)0x03001EBC;
    if (temp_r5_2 != 0) {
        Func_080770c8(0x145);
        Func_080770d0(0x142);
        if ((temp_r5_2 == 0x95) && (Func_080770c0(0x144) == 0)) {
            Func_08015120((s32) temp_r2, 2);
            Func_08015040((void *)0x924, 0xD);
            temp_r6 = Func_08091d84(1);
            Func_08015140();
            var_r0 = 0;
            if (temp_r6 == 0) {
                *(u16 *)0x02000400 = *(u16 *)0x02000480;
                *(u16 *)0x02000402 = *(u16 *)0x02000482;
                M2C_FIELD(temp_r8_2, s16 *, 0x170) = 0x3E7;
                goto block_30;
            }
        } else {
block_30:
            Func_08015120(var_sl, 1);
            Func_08015120((s32) temp_r2, 2);
            Func_08015040((void *)0x91C, 1);
            Func_08096fb0(temp_r5_2, 0);
            M2C_FIELD(temp_r8_2, s8 *, 0xCC6) = 1;
            Func_08096810();
            M2C_FIELD(temp_r8_2, s8 *, 0xCC6) = 0;
            Func_08097194();
            if (1 & M2C_FIELD(Func_08077018((s32) temp_r2), u8 *, 0xC)) {
                Func_080770c8(0x143);
            }
            goto block_32;
        }
    } else {
block_32:
        if (Func_080770c0(0x142) != 0) {
            Func_08015040((void *)0x927, 1);
        }
        if (Func_080770c0(0x143) != 0) {
            Func_08077058(var_sl, sp4);
        }
        var_r0 = sp0;
    }
    return var_r0;
}
