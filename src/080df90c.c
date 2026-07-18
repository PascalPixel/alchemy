typedef unsigned char u8;
typedef signed int s32;

typedef struct {
    u8 unknown00[8];
    s32 x;
    u8 unknown0c[4];
    s32 y;
    u8 unknown14[20];
    s32 unknown28;
    u8 unknown2c[4];
    s32 speed30;
    s32 speed34;
    u8 unknown38[12];
    s32 unknown44;
    s32 kind;
    u8 unknown4c[12];
    u8 enabled;
    u8 unknown59;
    u8 visible;
} Object080df90c;

Object080df90c **Func_080b5098(s32);
s32 Func_080022ec(s32, s32);
void Func_08009140(Object080df90c *);
void Func_08009150(Object080df90c *, s32, s32, s32);
void Func_08009080(Object080df90c *, s32);

void Func_080df90c(s32 first, s32 second, s32 divisor) {
    Object080df90c **firstContext = Func_080b5098(first);
    Object080df90c **secondContext = Func_080b5098(second);
    Object080df90c *object = *firstContext;
    Object080df90c *target = *secondContext;
    s32 scale = 80;
    s32 differenceX = target->x - object->x;
    s32 startX = object->x;
    s32 deltaX = Func_080022ec(scale * differenceX, 100);
    s32 differenceY = target->y - object->y;
    s32 startY = object->y;
    s32 deltaY = Func_080022ec(scale * differenceY, 100);
    s32 x = startX + deltaX;
    s32 y = startY + deltaY;
    s32 shortX = deltaX >> 8;
    s32 shortY = deltaY >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(shortX * shortX + shortY * shortY);
    distance = Func_080022ec(distance << 8, divisor);
    object->speed34 = distance;
    object->speed30 = distance;
    object->enabled = 1;
    object->kind = 0xab85;
    object->unknown28 = 0;
    object->unknown44 = 0;
    object->visible = 1;
    Func_08009140(object);
    Func_08009150(object, x, 0, y);
    Func_08009080(object, 2);
}
