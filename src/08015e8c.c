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

void *Func_08015e8c(void) {
    register void *temp_r0 asm("r0");
    register void **head asm("r1");
    register void *temp_r2 asm("r2");
    register u8 *temp_r3 asm("r3");
    register u32 offset asm("r2");

    temp_r3 = *(u8 **)0x03001E8C;
    offset = 0xD98;
    asm volatile("" : "+r" (temp_r3), "+r" (offset));
    head = (void **) (temp_r3 + offset);
    temp_r0 = *head;
    asm volatile("" : "+r" (temp_r0), "+r" (head));
    if (temp_r0 != NULL) {
        temp_r2 = *(void **)temp_r0;
        asm volatile("" : "+r" (temp_r2));
        if (temp_r2 == NULL) {
            M2C_FIELD(temp_r3, void **, 0xD9C) = head;
        }
        *head = temp_r2;
        *(void **)temp_r0 = NULL;
    }
    return temp_r0;
}
