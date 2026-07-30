typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08077cb8();
void Func_08078ee8(s32 *, s32, s32, s32);
void Func_0807961c(s32);

/* Clears the shared runtime arenas and establishes their initial state. */
s32 Func_08077d38(void) {
    s32 sp0;
    s32 sp4;
    s32 temp_r0;
    s32 temp_r4;
    void *temp_r3;

    sp4 = 0;
    M2C_FIELD((void *)0x040000D4, s32 **, 0) = &sp4;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x02000240;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x850000B0;
    temp_r3 = (void *)0x040000D4;
    sp4 = 0;
    M2C_FIELD(temp_r3, s32 **, 0) = &sp4;
    M2C_FIELD(temp_r3, s32 *, 4) = 0x02001000;
    M2C_FIELD(temp_r3, s32 *, 8) = 0x850003E1;
    do {
        temp_r4 = M2C_FIELD((void *)0x040000D4, s32 *, 8) & 0x80000000;
    } while (temp_r4 != 0);
    sp4 = temp_r4;
    M2C_FIELD((void *)0x040000D4, s32 **, 0) = &sp4;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x02000040;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85000080;
    *(s8 *)0x02001104 = 0xFF;
    sp4 = temp_r4;
    M2C_FIELD((void *)0x040000D4, s32 **, 0) = &sp4;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x02000500;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85000298;
    sp0 = temp_r4;
    Func_08078ee8(&sp4, 0x02000500, 0x85000298, (0x040000D4 + 0xC) - 0xC);
    *(s16 *)0x02000450 = 1;
    *(s16 *)0x02000452 = 2;
    *(s16 *)0x02000454 = 4;
    *(s16 *)0x02000456 = 8;
    *(s16 *)0x02000458 = 0x200;
    *(s16 *)0x0200045A = 0x100;
    *(s16 *)0x0200045C = 2;
    *(s16 *)0x02000460 = (s16) sp0;
    *(s16 *)0x02000462 = (s16) sp0;
    *(s32 *)0x02000434 = sp0;
    Func_0807961c(0);
    M2C_FIELD((void *)0x02000240, s32 *, 0x10) = sp0;
    *(s8 *)0x0200044C = 1;
    M2C_FIELD((void *)0x02000240, s8 *, 0x20A) = 1;
    M2C_FIELD((void *)0x02000240, s8 *, 0x20B) = 1;
    M2C_FIELD((void *)0x02000240, s8 *, 0x205) = 0;
    M2C_FIELD((void *)0x02000240, s8 *, 0x206) = 8;
    M2C_FIELD((void *)0x02000240, s32 *, 0) = sp0;
    temp_r0 = Func_08077cb8();
    M2C_FIELD((void *)0x02000240, s32 *, 0x2B8) = temp_r0;
    *(s32 *)0x03001C9C = sp0;
    *(u8 *)0x03001D08 = 0;
    M2C_FIELD((void *)0x02000240, s32 *, 4) = sp0;
    M2C_FIELD((void *)0x02000240, u8 *, 0x22A) = (u8) *(u8 *)0x03001D08;
    *(s16 *)0x03001D24 = (s16) sp0;
    *(s16 *)0x02002004 = -1;
    M2C_FIELD((void *)0x02000240, s8 *, 0x11D) = 4;
    M2C_FIELD((void *)0x02000240, s8 *, 0x11E) = 4;
    M2C_FIELD((void *)0x02000240, s8 *, 0x11F) = 4;
    M2C_FIELD((void *)0x02000240, s8 *, 0x120) = 8;
    M2C_FIELD((void *)0x02000240, s8 *, 0x121) = 8;
    M2C_FIELD((void *)0x02000240, s8 *, 0x122) = 8;
    M2C_FIELD((void *)0x02000240, s8 *, 0x123) = 0x10;
    M2C_FIELD((void *)0x02000240, s8 *, 0x124) = 0x10;
    M2C_FIELD((void *)0x02000240, s8 *, 0x125) = 0x10;
    M2C_FIELD((void *)0x02000240, s8 *, 0x126) = 0x20;
    M2C_FIELD((void *)0x02000240, s8 *, 0x127) = 0x20;
    M2C_FIELD((void *)0x02000240, s8 *, 0x128) = 0x20;
    M2C_FIELD((void *)0x02000240, s8 *, 0x129) = 0x40;
    M2C_FIELD((void *)0x02000240, s8 *, 0x12A) = 0x40;
    M2C_FIELD((void *)0x02000240, s8 *, 0x12B) = 0x40;
    return temp_r0;
}
