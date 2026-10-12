#include "ov664.h"

extern u32 Data_0200654c[];
extern u32 Data_02006564[];
extern u32 Data_0200663c[];
extern u32 Data_020066b4[];
extern u32 Data_0200675c[];
extern u32 Data_020068ac[];
extern u32 Data_020068f4[];
extern u32 Data_020069e4[];
extern u32 Data_02006aec[];
extern u32 Data_02006bac[];

s32 Math_Cosine(s32 angle);
void ObjectDispatch_ApplyValueToChildren(struct FieldActor *object, s32 value);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Func_020004d4(struct FieldActor *object);

void Func_02000550(struct FieldActor *object)
{
    u8 *work = Ram_HeapSlots->event_work;

    if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 || *(s16 *)(work + 350) != 0
        || *(s16 *)(work + 360) != 0) {
        ObjectDispatch_ApplyValueToChildren(object, 0);
        Map_EnableUpdateCallback();
        return;
    }
    ObjectDispatch_ApplyValueToChildren(object, 16);
    Map_DisableUpdateCallback();
    if (BYTE_AT(object, 99) != 0) {
        if ((gFrameCount & 7) == 0)
            BYTE_AT(object, 99)--;
        return;
    }
    if (*(s16 *)((u8 *)object + 102) == 0)
        Func_020004d4(object);
    do { } while (0); /* FAKEMATCH: the loop note keeps the timer load after the reset call (stated plainly it is loaded before the test as well, 13 differ). */
    (*(s16 *)((u8 *)object + 102))--;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native stores the timer before it loads z (2 differ without it). */
    object->x += Math_Cosine(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
    object->y += WORD_AT(object, 48);
}
