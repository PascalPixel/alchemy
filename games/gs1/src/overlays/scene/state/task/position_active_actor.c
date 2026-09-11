#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/task/position_active_actor.h"

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

extern u8 gCell[];

s32 State_Run();           /* veneer to GameFlag_IsSet */
u8 *State_Run2();           /* veneer to Scene_GetRecord */

void State_Run3();          /* veneer to Object_SetPosition */
void State_Run4();          /* veneer to Object_SetCallback */
void State_Run5();          /* veneer to State_Run6 */
s32 State_Run7();           /* local thunk to State_Run8, site A */
s32 State_Run9();           /* local thunk to State_Run8, site B */
void State_Run10();          /* veneer to UiText_DrawQuantity, site A */
void State_Run11();          /* veneer to UiText_DrawQuantity, site B */
void State_Run12();          /* shared veneer, selector refresh + 0x96a */

void State_Run13();          /* veneer to State_Run14 */

s32 State_Run15(s32 handleA, s32 handleB)
{
    u8 *workspace = *(u8 **)SCENE_WORK_PTR;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = State_Run(0x211);

    shared = gCell;
    record = State_Run2(*(s32 *)(shared + 500));

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

    State_Run3(record, x, 0, z);
    State_Run16(0x211);
    State_Run4(record, (void *)SCENE_CALLBACK);

    while (*waitp != 0) {
        State_Run5(1);
    }

    if (flag == 0) {
        State_Run7(0, handleA);
        State_Run10(handleA, 2);
    } else {
        State_Run9(0, handleB);
        State_Run11(handleB, 2);
    }

    shared = gCell;
    State_Run12(*(s32 *)(shared + 500), 1);
    State_Run17(SCENE_TEXT_VALUE, 3);
    State_Run13(record);

    return flag;
}
