/* Near misses of overlay 664 (Func_02002bd0 of this draft is adopted in s2bd0.c).
 * Func_02002c40 (56 instructions, 2 differ): the native loads the phase before it starts the
 * radius constant 0xc0000.
 * Func_02003b08 (47, 4): the native loads the flags into r2 and builds the bit 1 in r3 for the
 * second flag update; a plain |= and an inline parameter both give the reverse.
 * Func_02003cac (62, 30): the native keeps the event work pointer in r5 and adds the room
 * offset into it, with the row in r6; the draft allocates them the other way. The row
 * passes through an inline so that 54 - (room - 201) * 4 does not fold. */
#include "../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
s32 __divsi3(s32 a, s32 b);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
s32 Func_02000690(u32 layer, s32 x, s32 y);
void Func_02002790(void);
void Func_02003b7c(s32 x, s32 y);

void Func_02002bd0(struct FieldActor *object)
{
    s32 radius = ((object->y - object->terrainHeight) >> 1) + 0x100000;

    object->x = WORD_AT(object, 68) + Iwram_MulQ16(Math_Cosine((WORD_AT(object, 48) & 255) << 11), radius);
    object->z = WORD_AT(object, 76) + Iwram_MulQ16(Math_Sine((WORD_AT(object, 48) & 255) << 11), radius) / 3;
    object->y += WORD_AT(object, 72);
    WORD_AT(object, 48)++;
}

void Func_02002c40(struct FieldActor *object)
{
    s32 y = object->y;
    s32 phase = WORD_AT(object, 48);
    s32 radius = (y >> 2) + 0xc0000;

    object->x = WORD_AT(object, 68) + Iwram_MulQ16(Math_Cosine((phase & 255) << 11), radius) * 2;
    object->z = WORD_AT(object, 76) + Iwram_MulQ16(Math_Sine((WORD_AT(object, 48) & 255) << 11), radius);
    object->y += WORD_AT(object, 72);
    WORD_AT(object, 48)++;
    Object_SetPartAttribute(object, ((gFrameCount & 2) >> 1) * 9);
}

static inline void FlagOn(u8 *flags, s32 bit)
{
    /* FAKEMATCH: the bit passes through an inline parameter: the native builds it in the register it ORs into (4 differ as a plain |=). */
    *flags = bit | *flags;
}

void Func_02003b08(void)
{
    if (*(s16 *)((u8 *)Object_GetById(gPartyState.currentOwner) + 18) <= 653) {
        Func_02005e14();
        Func_02005f2c(0);
        Func_02005f94(186);
        BYTE_AT(Object_GetById(gPartyState.currentOwner), 90) &= ~1;
        ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x40000, 0x20000);
        ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, 376, 668);
        FlagOn(&BYTE_AT(Object_GetById(gPartyState.currentOwner), 90), 1);
        Func_02005e1c();
    }
}

static inline s32 RowTop(s32 row)
{
    /* FAKEMATCH: the row passes through an inline parameter: the native subtracts 201 and scales before it takes the result from 54 (written in one expression the constants fold, 29 differ). */
    return 54 - row * 4;
}

void Func_02003cac(void)
{
    s16 *room = (s16 *)Ram_HeapSlots->eventWork;
    struct FieldActor *leader = Object_GetById(gPartyState.currentOwner);
    s32 kind = Func_02000690(2, leader->x >> 20, leader->z >> 20) >> 8;
    s32 row;

    room += 181;
    row = RowTop(*room - 201);

    if (kind != 1 && kind != 7) {
        Func_02002790();
        Func_02003b7c(264, row * 8 + 10);
        if (GameFlag_Test(0x200) == 0)
            GameFlag_SetBit(0x200);
        GameFlag_SetBit(*room + 312);
    } else if (GameFlag_Test(0x200)) {
        Func_02003b7c(264, row * 8 + 10);
    }
}
