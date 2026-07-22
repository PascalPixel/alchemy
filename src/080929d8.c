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

void Func_080929d8(void *arg0, s32 arg1) {
    if ((M2C_FIELD(arg0, u8 *, 0x54) & 0xF) == 1) {
        void *container = M2C_FIELD(arg0, void **, 0x50);
        u8 rawCount = M2C_FIELD(container, u8 *, 0x27);

        if (rawCount != 0) {
            void **entry = (void **)((u8 *)container + 0x28);
            u32 count = rawCount;
            do {
                void *item = *entry++;
                if (item != NULL && M2C_FIELD(item, s32 *, 0x10) != 0) {
                    M2C_FIELD(item, s8 *, 5) = arg1;
                }
                count--;
            } while (count != 0);
        }
        M2C_FIELD(container, u8 *, 0x25) = 1;
    }
}
