typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

#define FIELD_S16(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define FIELD_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

u8 *Func_0808ba1c(s32);
void Func_08009140(void *);
void Func_08009080(void *, s32);
void Func_08009150(void *, s32, s32, s32);
void Func_08009158(void *);
void Func_08092b08(s32, s32);

void Func_08092208(s32 arg0, s32 arg1, s32 arg2) {
    u8 *object;
    s16 angle;
    s32 adjusted;
    s16 remainder;

    object = Func_0808ba1c(arg0);
    if (object != 0) {
        angle = FIELD_S16(object, 0x0A);
        adjusted = angle;
        if (angle < 0) {
            adjusted += 15;
        }
        adjusted >>= 4;
        adjusted *= 16;
        remainder = angle - adjusted;
        object[0x5B] = 0;
        Func_08009140(object);
        Func_08009080(object, 2);
        Func_08009150(object,
            FIELD_S32(object, 0x08) + ((8 - remainder) << 16),
            FIELD_S32(object, 0x0C), FIELD_S32(object, 0x10));
        Func_08009158(object);
        Func_08092b08(arg0, arg1);
        Func_08009150(object, FIELD_S32(object, 0x08),
            FIELD_S32(object, 0x0C),
            FIELD_S32(object, 0x10) + (arg2 << 16));
    }
}
