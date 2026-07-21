typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_080770c0(s32);
void Func_080770d0(s32);
void Func_08094380(s32);
void *Func_0808ba1c(s32);
void Func_080933f8(s32, s32, s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
void Func_08009080(void *, s32);
void Func_0809163c(s32);
void Func_0809335c(s32, s32);
extern s32 Data_02000240[];

s32 Func_08094428(void)
{
    s32 result = 0;
    s32 event = 0x120;

    if (Func_080770c0(event) != 0) {
        Func_08094380(24);
        Func_080770d0(event);
        result = 1;
    } else {
        event = 0x121;
        if (Func_080770c0(event) != 0) {
            Func_08094380(23);
            Func_080770d0(event);
            result = 2;
        } else {
            event = 0x122;
            if (Func_080770c0(event) != 0) {
                s32 object_id;
                void *object;

                Func_080770d0(event);
                object_id = Data_02000240[125];
                object = Func_0808ba1c(object_id);
                *(s32 *)((u8 *)object + 12) += 0x00a00000;
                Func_080933f8(-1, -1, -1, 0);
                while (*(s32 *)((u8 *)object + 12) + *(s32 *)((u8 *)object + 40) >
                       *(s32 *)((u8 *)object + 20)) {
                    Func_080030f8(1);
                }
                Func_080f9010(159);
                *(s32 *)((u8 *)object + 12) = *(s32 *)((u8 *)object + 20);
                Func_08009080(object, 22);
                Func_0809163c(15);
                Func_0809335c(object_id, 1);
                result = 3;
            }
        }
    }
    return result;
}
