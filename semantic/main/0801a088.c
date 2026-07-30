typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_0801a5a4(void *, s32);
void *Func_08077018(s32);

/* Queues the base and optional modifier records for an encoded effect ID. */
s32 Func_0801a088(s32 arg0, s32 arg1) {
    void *sp0;
    s32 sp4;
    s32 sp8;
    s32 temp_r0_2;
    s32 var_sl;
    u8 temp_r0;
    void *temp_r5;

    sp8 = arg0;
    sp4 = 0;
    var_sl = 0;
    sp0 = Func_08077018(0x1FF & sp8);
    temp_r5 = *(void **)0x03001E94;
    if (temp_r5 == 0) {
        return -1;
    }
    if (1 & arg1) {
        M2C_FIELD(temp_r5, s32 *, 0x604) = (s32) M2C_FIELD((void *)0x08029A10, s32 *, 8);
        M2C_FIELD(temp_r5, s16 *, 0x600) = 2;
        M2C_FIELD(temp_r5, s16 *, 0x602) = 2;
        Func_0801a5a4(temp_r5, 0);
        sp4 = 1;
    }
    M2C_FIELD(temp_r5, s32 *, 0x604) = (s32) M2C_FIELD((M2C_FIELD(sp0, u16 *, 6) * 4), s32 *, 0x08029EE4);
    M2C_FIELD(temp_r5, s16 *, 0x600) = 2;
    M2C_FIELD(temp_r5, s16 *, 0x602) = 2;
    Func_0801a5a4(temp_r5, sp4);
    if ((8 & arg1) && (0x400 & sp8)) {
        M2C_FIELD(temp_r5, s32 *, 0x604) = (s32) M2C_FIELD((void *)0x08029ACC, s32 *, 4);
        M2C_FIELD(temp_r5, s16 *, 0x600) = 2;
        M2C_FIELD(temp_r5, s16 *, 0x602) = 2;
        Func_0801a5a4(temp_r5, 1);
    }
    if ((0x10 & arg1) && (0x200 & sp8)) {
        M2C_FIELD(temp_r5, s32 *, 0x604) = (s32) M2C_FIELD((void *)0x08029ACC, s32 *, 0);
        M2C_FIELD(temp_r5, s16 *, 0x600) = 2;
        M2C_FIELD(temp_r5, s16 *, 0x602) = 2;
        Func_0801a5a4(temp_r5, 1);
    }
    if ((0x20 & arg1) && (0x200 & sp8)) {
        temp_r0 = M2C_FIELD(sp0, u8 *, 3);
        if ((1 & temp_r0) && (2 & temp_r0)) {
            M2C_FIELD(temp_r5, s32 *, 0x604) = (s32) M2C_FIELD((void *)0x08029ACC, s32 *, 8);
            M2C_FIELD(temp_r5, s16 *, 0x600) = 2;
            M2C_FIELD(temp_r5, s16 *, 0x602) = 2;
            Func_0801a5a4(temp_r5, 1);
        }
    }
    if (2 & arg1) {
        var_sl = ((u32) (0xF800 & sp8) >> 0xB) + 1;
        if (var_sl <= 1) {
            var_sl = 0;
        }
    }
    if (4 & arg1) {
        var_sl = ((u32) (0xF800 & sp8) >> 0xB) + 1;
    }
    if ((var_sl != 0) && (var_sl <= 0x1E)) {
        M2C_FIELD(temp_r5, s32 *, 0x604) = (s32) M2C_FIELD((Func_080022fc(var_sl, 0xA) * 4), s32 *, 0x08029B68);
        M2C_FIELD(temp_r5, s16 *, 0x600) = 2;
        M2C_FIELD(temp_r5, s16 *, 0x602) = 2;
        Func_0801a5a4(temp_r5, 1);
        temp_r0_2 = Func_080022ec(var_sl, 0xA);
        if (temp_r0_2 != 0) {
            M2C_FIELD(temp_r5, s32 *, 0x604) = (s32) M2C_FIELD(((temp_r0_2 * 4) + 0x24), s32 *, 0x08029B68);
            M2C_FIELD(temp_r5, s16 *, 0x600) = 2;
            M2C_FIELD(temp_r5, s16 *, 0x602) = 2;
            Func_0801a5a4(temp_r5, 1);
        }
    }
    return 0x100;
}
