#include "types.h"

#if defined(GS1_EDITION_JA)
#define SCENE_TEXT_VALUE 0x98f
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_TEXT_VALUE 0xa5a
#else
#define SCENE_TEXT_VALUE 0x96a
#endif
#if defined(GS1_EDITION_DE)
#define SCENE_WORK_PTR 0x03001f4c
#else
#define SCENE_WORK_PTR 0x03001f3c
#endif
#if defined(GS1_EDITION_FR)
#define SCENE_CALLBACK 0x0200cbcc
#else
#define SCENE_CALLBACK 0x0200cb9c
#endif

extern u8 Data_02000240[];

s32 Func_02007240();           /* veneer to GameFlag_IsSet */
u8 *Func_020072b8();           /* veneer to Scene_GetRecord */
void Func_020072b8_a();
void Func_0200723a();          /* veneer to Object_SetPosition */
void Func_02007220();          /* veneer to Object_SetCallback */
void Func_0200717e();          /* veneer to Func_080000c0 */
s32 Func_02005772();           /* local thunk to Func_020020e8, site A */
s32 Func_02005784();           /* local thunk to Func_020020e8, site B */
void Func_020072aa();          /* veneer to UiText_DrawQuantity, site A */
void Func_020072bc();          /* veneer to UiText_DrawQuantity, site B */
void Func_020072cc();          /* shared veneer, selector refresh + 0x96a */
void Func_020072cc_a();
void Func_0200729a();          /* veneer to Func_08009148 */

s32 Func_02003344(s32 handleA, s32 handleB)
{
    u8 *workspace = *(u8 **)SCENE_WORK_PTR;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Func_02007240(0x211);

    shared = Data_02000240;
    record = Func_020072b8(*(s32 *)(shared + 500));

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cuep = (u16 *)(workspace + 228);
    } else {
        z = *(s32 *)(workspace + 236) - 0x100000;
        cuep = (u16 *)(workspace + 226);
    }

    waitp = (s16 *)(record + 100);
    *waitp = *cuep;
    *(s32 *)(record + 52) = 0x4000;
    *(s32 *)(record + 48) = 0x10000;

    Func_0200723a(record, x, 0, z);
    Func_020072b8_a(0x211);
    Func_02007220(record, (void *)SCENE_CALLBACK);

    while (*waitp != 0) {
        Func_0200717e(1);
    }

    if (flag == 0) {
        Func_02005772(0, handleA);
        Func_020072aa(handleA, 2);
    } else {
        Func_02005784(0, handleB);
        Func_020072bc(handleB, 2);
    }

    shared = Data_02000240;
    Func_020072cc(*(s32 *)(shared + 500), 1);
    Func_020072cc_a(SCENE_TEXT_VALUE, 3);
    Func_0200729a(record);

    return flag;
}
