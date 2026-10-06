/* FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
#include "TYPES.H"
#include "IO_REG.H"
#include "ANIMSPR.H"

extern struct AnimationObject *gSpriteObjects;
extern volatile u32 gKeysHeld;
extern s32 gFrameTick;

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *point);

void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *out)
{
    /* FAKEMATCH: prior ordinary and permuted shapes kept the angle in r6; constrain the measured angle register while preserving all truncations. */
    s32 angle;
    s32 i;
    u8 kind;

    angle = 0;
    kind = gSpriteObjects->entries[0]->kind;
    if ((gKeysHeld & KEY_B) != 0)
        angle = (s16)(gFrameTick << 8);
    {
        register s32 a asm("r5") = angle; /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
        register struct AnimationPosition *p asm("r7") = out; /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
        asm("" : "+r"(a), "+r"(p)); /* FAKEMATCH: native register lifetime and shift scheduling. */
        angle = a;
        out = p;
    }
    switch ((u32)kind) {
        u16 tmp2;
    case 3:
        for (i = 0; i < 6; i++) {
            {
            register s32 zero asm("r3"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            register s32 px asm("r2"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            angle <<= 16;
            asm volatile("" : "+r"(angle) : : "r2", "r3"); /* FAKEMATCH: keeps the angle shift before low-register staging. */
            px = x << 16;
            zero = 0;
            asm("" : "+r"(angle) : "r"(zero)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            out->x = px;
            angle = (u32)angle >> 16;
            out->y = zero;
            out->z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2aaa);
        }
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        for (i = 0; i < 8; i++) {
            {
            register s32 zero asm("r3"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            register s32 px asm("r2"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            angle <<= 16;
            asm volatile("" : "+r"(angle) : : "r2", "r3"); /* FAKEMATCH: keeps the angle shift before low-register staging. */
            px = x << 16;
            zero = 0;
            asm("" : "+r"(angle) : "r"(zero)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            out->x = px;
            angle = (u32)angle >> 16;
            out->y = zero;
            out->z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2000);
        }
        break;
    case 4:
    case 6:
        for (i = 0; i < 10; i++) {
            {
            register s32 zero asm("r3"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            register s32 px asm("r2"); /* FAKEMATCH: native register assignment; ordinary source scored 7745. */
            angle <<= 16;
            asm volatile("" : "+r"(angle) : : "r2", "r3"); /* FAKEMATCH: keeps the angle shift before low-register staging. */
            px = x << 16;
            zero = 0;
            asm("" : "+r"(angle) : "r"(zero)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            out[0].x = px;
            angle = (u32)angle >> 16;
            out->y = zero;
            out[0].z = z << 16;
            }
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x1999);
        }
        break;
    case 20: {
        struct AnimationPosition *pair;
        tmp2 = (u16)angle;
        angle = (s16)(tmp2 + 0x4000);
        i = 0;
        pair = out;
        while (i < 2) {
            pair[0].x = x << 16;
            pair[0].y = 0;
            pair[0].z = z << 16;
            angle <<= 16;
            asm("" : "+r"(angle)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (u32)angle >> 16;
            {
                register s32 a asm("r1") = (u16)angle; /* FAKEMATCH: consumed first-call angle, ordinary295 ->235 with its point argument. */
                register s32 *point asm("r2") = &out[0].x; /* FAKEMATCH: consumed first-call point, ordinary295 ->235 with its angle. */
                Vector_AddPolarOffset(0x280000, a, point);
            }
            pair[1].x = x << 16;
            asm volatile("" : : : "r2", "memory"); /* FAKEMATCH: orders the paired x store before the remaining coordinates. */
            {
                register s32 *point asm("r2") = &out[1].x; /* FAKEMATCH: stage the consumed second point before y/z stores; register/order-only235 ->120. */
                pair[1].y = 0;
                pair[1].z = z << 16;
                Vector_AddPolarOffset(0x280000, (u16)angle, point);
            }
            pair += 2;
            out += 2;
            angle = (s16)(angle + (u32)0x8000);
            i++;
        }
        break;
    }
    default:
        i = 0;
        {
            s32 a = (u32)angle << 16;
            asm("" : "+r"(a)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (s32)((u32)a + 0x20000000) >> 16;
        }
        while (i < 4) {
            angle <<= 16;
            asm("" : "+r"(angle)); /* FAKEMATCH: native register lifetime and shift scheduling. */
            angle = (u32)angle >> 16;
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            ++out;
            (u16)(angle = (s16)(angle + 0x4000));
            i++;
        }
        break;
    }
}
