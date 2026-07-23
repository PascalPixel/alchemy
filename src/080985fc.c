typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Triple {
    s32 values[3];
};

extern u8 *Data_03001f30;
void Func_08098698(void);
void Func_080f9010(s32);
s32 Func_08004458(void);
void Func_0800447c(s32, s32, struct Triple *);
void *Func_08096c80(s32, s32, s32, s32);
void Func_08009098(void *, const void *);
void Func_080030f8(u32);
void Func_0809748c(void);

void Func_080985fc(void)
{
    u8 *state = Data_03001f30;
    struct Triple arguments;
    struct Triple *argumentPointer;
    s32 count;

    Func_08098698();
    Func_080f9010(0x86);
    argumentPointer = &arguments;
    count = 4;
    do {
        void *object;
        s32 random;

        argumentPointer->values[0] = *(s32 *)(state + 4);
        argumentPointer->values[2] = *(s32 *)(state + 12);
        random = (Func_08004458() * 6) + 0x40000;
        Func_0800447c(random, Func_08004458(), argumentPointer);
        argumentPointer->values[1] = *(s32 *)(state + 8);
        object = Func_08096c80(
            0xD9,
            argumentPointer->values[0],
            argumentPointer->values[1],
            argumentPointer->values[2]
        );
        if (object != 0) {
            Func_08009098(object, (const void *)0x0809F11C);
            *((u8 *)object + 0x55) = 2;
        }
        Func_080030f8((((u32)Func_08004458() * 2) >> 16) + 2);
        count--;
    } while (count >= 0);
    Func_080030f8(0x1E);
    Func_0809748c();
}
