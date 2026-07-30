typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Vector_08095938 {
    s32 x;
    s32 y;
    s32 z;
};

void Func_0800447c(s32, s32, struct Vector_08095938 *);
u8 *Func_08092054(u32);
void Func_080974d8(struct Vector_08095938 *);
void Func_0809bb34(void *);

/*
 * Advance the five-stage orbiting effect. The middle stage eases its origin
 * toward the transformed reference object; every stage updates the two orbit
 * angles and projects them back into the effect's rendered position.
 */
void Func_08095938(void *effect_pointer) {
    u8 *effect;
    u8 *reference;
    struct Vector_08095938 position;
    s16 age;
    u16 angle;
    u16 elevation;
    s8 stage;
    s8 original_stage;
    s32 current;
    s32 difference;

    effect = effect_pointer;
    reference = Func_08092054(*(u32 *)0x02000434);
    age = *(s16 *)(effect + 0x38);
    angle = *(u16 *)(effect + 0x3C);
    elevation = *(u16 *)(effect + 0x3E);
    stage = *(s8 *)(effect + 0x40);
    original_stage = stage;

    switch (stage) {
    case 0:
        angle++;
        elevation++;
        if (age == 60) {
            *(s16 *)(effect + 0x38) = 0;
            stage++;
        }
        break;
    case 1:
        elevation++;
        if (age == 40) {
            *(s16 *)(effect + 0x38) = 0;
            stage++;
        }
        break;
    case 2:
        elevation++;
        position.x = *(s32 *)(reference + 8);
        position.y = *(s32 *)(reference + 0xC) + 0x140000;
        position.z = *(s32 *)(reference + 0x10);
        Func_080974d8(&position);
        current = *(s32 *)(effect + 0x14);
        difference = position.x - current;
        if (difference < 0) {
            difference += 7;
        }
        *(s32 *)(effect + 0x14) = current + (difference >> 3);
        current = *(s32 *)(effect + 0x18);
        difference = position.z - current;
        if (difference < 0) {
            difference += 7;
        }
        *(s32 *)(effect + 0x18) = current + (difference >> 3);
        if (age == 40) {
            *(s16 *)(effect + 0x38) = 0;
            stage++;
        }
        break;
    case 3:
        angle--;
        elevation++;
        if (age == 60) {
            *(s16 *)(effect + 0x38) = 0;
            stage++;
        }
        break;
    case 4:
        Func_0809bb34(effect);
        break;
    default:
        break;
    }

    if (original_stage >= 0 && original_stage <= 3) {
        *(u16 *)(effect + 0x3C) = angle;
        *(u16 *)(effect + 0x3E) = elevation;
        *(s8 *)(effect + 0x40) = stage;
    }
    angle = *(u16 *)(effect + 0x3C);
    elevation = *(u16 *)(effect + 0x3E);
    position.x = *(s32 *)(effect + 0x14);
    position.z = *(s32 *)(effect + 0x18);
    Func_0800447c((s16)angle << 16, (s16)elevation << 11, &position);
    *(s32 *)(effect + 4) = position.x;
    *(s32 *)(effect + 8) = position.z;
}
