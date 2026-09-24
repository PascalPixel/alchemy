#include "FIXED_MATH.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"

extern s8 Data_0809f160[];

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void Animation_ApplyChildValuesFar(void *, s32);
void Object_SetAction(void *, s32);

struct GlobalState {
    u8 unknown_000[0x249];
    u8 saved_byte;
    u8 unknown_24a[6];
    u32 saved_callback;
};

extern struct GlobalState Data_02000240;
#define PARTY_STATE Data_02000240

void BattleEffect_SetRandomTableValueOnObject(s32 arg0)
{
    s8 *table = Data_0809f160;
    s32 index = Rand();
    Animation_ApplyChildValuesFar((void *)arg0, table[(u32)(index * 8) >> 16]);
}

void BattleEffect_PauseObject(s32 arg0)
{
    u8 *object;
    u8 *entry;

    object = ObjectTable_Get(arg0);
    if (object != NULL) {
        PARTY_STATE.saved_callback = *(u32 *)(object + 0x6C);
        PARTY_STATE.saved_byte = 0;
        if (object[0x54] == 1) {
            entry = *(u8 **)(*(u8 **)(object + 0x50) + 0x28);
            if (entry != NULL) {
                PARTY_STATE.saved_byte = entry[5];
            }
        }
        *(u32 *)(object + 0x6C) = (u32)BattleEffect_SetRandomTableValueOnObject;
        object[0x5B] = 1;
        Object_SetAction(object, 0);
    }
}

void BattleFx_ResumeObject(void)
{
    u8 *object = ObjectTable_Get();
    if (object != 0) {
        if (*(void (**)(s32))(object + 0x6C) == BattleEffect_SetRandomTableValueOnObject) {
            u8 *state = (u8 *)&PARTY_STATE;
            *(s32 *)(object + 0x6C) = *(s32 *)(state + 0x250);
            *(s32 *)(state + 0x250) = 0;
            Animation_ApplyChildValuesFar(object, *(s8 *)(state + 0x249));
        }
        object[0x5B] = 0;
        Object_SetAction(object, 16);
    }
}
