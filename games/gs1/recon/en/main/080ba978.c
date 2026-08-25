#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct PresentationInput { u8 primary; u8 reserved_01; u8 secondary; u8 reserved_03[0x4d]; s32 coordinate; u8 reserved_54[4]; u32 flags; s32 script; };
struct PresentationWork { u8 bytes[84]; };
struct MotionEntry { u8 reserved_00[39]; u8 count; void *children[1]; };
struct MotionChild { u8 reserved_00[5]; u8 value; };
struct PresentationTransition { s32 angle; s32 frames; };

extern struct PresentationTransition *Data_03001f00;
extern u8 *Data_03001e74;
s32 Func_080041d8(void *, s32);
u16 Func_080044d0(s32, s32);
void Func_08009080(void *, s32);
void Func_08009088(void *, s32);
void Func_08015130(s32);
void **Func_080b7dd0(u8);
struct MotionEntry *Func_080b7f70(void *, s32);
void Func_080b8000(s16);
void Func_080b9d34(void *, struct PresentationWork *);
u32 Func_080bb938(void);
u32 Func_080bbabc(u32, u32);
void Func_080be02c(void);
void Func_080c10e8(s32, s32);
void Func_080c1798(s32, s32, s32, s32);
void Func_080c1a14(void);
void Func_080c9008(struct PresentationWork *);
void Func_080c9018(struct PresentationWork *);
void Func_080f9010(s32);

s32 Func_080ba978(struct PresentationInput *input, s32 flags)
{
    s32 saved_loop;
    struct PresentationWork work;
    struct PresentationTransition *transition = Data_03001f00;
    void *object;
    s32 i;

    if (input->flags & 0x40000) {
        transition->angle = input->primary <= 7 ? -0x2000 : 0x5000;
        transition->frames = 60;
    } else {
        u8 *actor = *Func_080b7dd0(input->primary);
        s16 angle = Func_080044d0(FIELD(actor, s32, 8), FIELD(actor, s32, 16));
        s32 current = angle + (input->primary <= 7 ? -0x1800 : 0x1800);
        s32 target = input->primary <= 7 ? 0x2000 : -0x2000;
        s32 delta = (target - current) * 3;
        if (delta < 0)
            delta += 3;
        current += delta >> 2;
        if ((input->secondary <= 7) == (input->primary <= 7))
            current = input->primary <= 7 ? 0x2400 : -0x2400;
        if (transition->angle != current)
            transition->angle = current;
    }
    if (input->flags & 0x80000) {
        transition->angle = input->primary <= 7 ? -0x2400 : 0x2000;
        transition->frames = 60;
    }

    Func_080b9d34(input, &work);
    i = flags & 1;
    if (i)
        FIELD(&work, s32, 28) = 1;
    Func_080c10e8(0, 0);
    Func_08015130(Data_03001e74[65] & ~1);
    object = *Func_080b7dd0(FIELD(&work, u8, 8));
    Func_08009080(object, 3);
    Func_08009088(object, 16);
    Func_080f9010(0x9a);
    if (flags & 2)
        Func_080c1798(FIELD(&work, s32, 8), input->coordinate, 1, 0);
    else if (!i)
        Func_080c1798(FIELD(&work, s32, 8), input->coordinate, 0, 0);
    FIELD(&work, s32, 4) = input->secondary <= 7;

    for (i = 0; i < FIELD(&work, s32, 20); i++) {
        struct MotionEntry *entry = Func_080b7f70(
            *Func_080b7dd0(FIELD(&work, u8, 36 + i * 2)), 0);
        s32 j;
        for (j = 0; j != entry->count - 1; j++)
            FIELD(&work, u8, 52 + i * 4 + j) =
                ((struct MotionChild *)entry->children[j])->value;
    }
    if (input->script != 0) {
        if (input->script == 1) {
            Func_080bbabc(0, input->primary);
            Func_080bbabc(4, 0x856);
        } else {
            Func_080bbabc(4, 0x855);
        }
        Func_080bb938();
        Func_080c1a14();
    } else {
        Func_080041d8((void *)0x080bd899, 0xc80);
        if (FIELD(&work, s32, 0)) {
            if (input->flags & 0x4000)
                Func_080c9008(&work);
            else
                Func_080c9018(&work);
        } else {
            Func_080c1a14();
        }
        Func_080be02c();
        Func_08009080(object, 1);
        for (i = 0; i < FIELD(&work, s32, 20); i++) {
            saved_loop = i;
            Func_080b8000(FIELD(&work, s16, 36 + i * 2));
            i = saved_loop;
        }
    }
    return 0;
}
