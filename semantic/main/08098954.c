typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Triple {
    s32 values[3];
};

extern u8 *Data_03001f30;
void Func_08097384(void);
void *Func_08098a84(s32, s32, s32, s32);
void Func_080030f8(u32);
void Func_08096bec(void *, s32, s32);
void Func_08009158(void *);
void Func_080f9010(s32);
void *Func_08096c80(s32, s32, s32, s32);
void Func_08009098(void *, const void *);
s32 Func_08004458(void);
void Func_080090d0(void *);
void Func_0809748c(void);

void Func_08098954(void)
{
    u8 *state = Data_03001f30;
    void *objects[2];
    struct Triple position;
    struct Triple *positionPointer;
    void **objectPointer;
    void *object;
    s32 count;
    s32 scale;

    Func_08097384();
    position.values[0] = *(s32 *)(state + 4);
    position.values[1] = *(s32 *)(state + 8) + 0x100000;
    position.values[2] = *(s32 *)(state + 12);
    objects[0] = Func_08098a84(
        position.values[0] + 0x200000,
        position.values[1],
        position.values[2],
        0x8000
    );
    objects[1] = Func_08098a84(
        position.values[0] - 0x200000,
        position.values[1],
        position.values[2],
        0
    );
    objectPointer = objects;
    Func_080030f8(15);
    count = 1;
    do {
        object = *objectPointer++;
        if (object != 0) {
            Func_08096bec(object, 0x180000, *(u16 *)((u8 *)object + 6));
        }
        count--;
    } while (count >= 0);
    Func_08009158(objects[0]);
    Func_080f9010(0x86);
    scale = 0x20000;
    positionPointer = &position;
    count = 23;
    do {
        s32 random;
        s32 speed;

        positionPointer->values[0] = *(s32 *)(state + 4);
        positionPointer->values[1] = *(s32 *)(state + 8) + 0x100000;
        positionPointer->values[2] = *(s32 *)(state + 12);
        object = Func_08096c80(
            0x11d,
            positionPointer->values[0],
            positionPointer->values[1],
            positionPointer->values[2]
        );
        if (object != 0) {
            Func_08009098(object, (const void *)0x0809f0d4);
            random = Func_08004458();
            *(s32 *)((u8 *)object + 0x34) = scale;
            *(s32 *)((u8 *)object + 0x30) = random + scale;
            *(s8 *)((u8 *)object + 0x55) = 0;
            speed = Func_08004458() * 0x18 + 0x80000;
            Func_08096bec(object, speed, Func_08004458());
        }
        count--;
    } while (count >= 0);
    Func_080090d0(objects[0]);
    Func_080090d0(objects[1]);
    Func_0809748c();
}
