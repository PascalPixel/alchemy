#include "scene.h"

void Func_02000038(void)
{
    Func_02001a04(12, 1, 13);
}

void Func_02000048(struct FieldActor *object)
{
    struct FieldSprite *sprite = object->sprite;
    s16 time = object->timer++;

    sprite->rotation = Iwram_MulQ16(Math_Sine(time << 12), 0x100);
    if ((time & 15) == 0)
        Func_02001a2c(155);
}
