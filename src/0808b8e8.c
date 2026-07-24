typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080090d0(void *);

void Func_0808b8e8(s32 _v1arg) {
    s32 _v1t60;
    s32 _v1t58;
    s32 _v1t65;
    s32 _v1t36;
    s32 _v1live51;
    s32 _v1t78;
    s32 _v1t74;
    s32 _v1d31;
    s32 _v1live50;
    s32 _v1live49;
    s32 _v1d1;
    s32 temp_r2_2;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r7;
    s32 temp_r8;
    s32 temp_r9;
    s32 temp_sl;
    s32 var_r6;
    void *temp_r2;
    void *temp_r1;
    void *temp_r0;
    void *temp_r2_3;
    void **var_r5;

    (s32) temp_r1 = *(void **)0x03001EBC;
    (s32) (s32) _v1t74 = (M2C_FIELD(temp_r1, void **, 0x1E0)); (s32) _v1d31 = 0; _v1live50 = (s32)(_v1t74);
    temp_r2 = _v1live50;
    _v1t58 = ((temp_r2)); _v1live51 = (s32)(*(s32 *)((u8 *)_v1t58 + 8));
    temp_r3 = _v1live51;
    _v1d1 = 0; (s32) temp_r9 = temp_r3 + 0xFF600000;
    temp_sl = temp_r3 + 0xA00000;
    _v1t78 = (*(s32 *)((u8 *)(temp_r2) + 0x10)); (s32) (s32) temp_r3_2 = _v1t78;
    temp_r8 = temp_r3_2 + 0xFF380000;
    temp_r7 = temp_r3_2 + 0x640000;
    var_r5 = temp_r1 + 0x34;
    var_r6 = 0x39;
    do {
        (s32) temp_r0 = *var_r5;
        if ((temp_r0 != NULL) && ((temp_r3_3 = *(s32 *)((u8 *)(temp_r0) + 8), (s32) temp_r2_2 = *(u32 *)((u8 *)(temp_r0) + 0x10), (temp_r3_3 != 0)) || (temp_r2_2 != 0)) && ((temp_r3_3 < temp_r9) || (temp_r3_3 > temp_sl) || (temp_r2_2 < temp_r8) || (temp_r2_2 > temp_r7))) {
            _v1t36 = ((temp_r0)); *(s8 *)((u8 *)_v1t36 + 0x54) = 1;
            _v1t65 = (M2C_FIELD(temp_r0, void **, 0x50)); _v1live49 = (s32)(_v1t65);
            temp_r2_3 = _v1live49;
            _v1t60 = ((temp_r2_3)); *(s8 *)((u8 *)_v1t60 + 0x1D) = (u8) (*(s8 *)((u8 *)(temp_r2_3) + 0x1D) & ~1);
            Func_080090d0(temp_r0);
            _v1t65 = 0;
            *var_r5 = (void *)_v1t65;
        }
        (s32) var_r5 += 4;
        var_r6 -= 1;
    } while (var_r6 >= 0);
}
