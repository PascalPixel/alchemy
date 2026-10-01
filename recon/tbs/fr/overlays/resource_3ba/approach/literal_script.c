/* NONMATCHING: Colosso approach script, 2026-10-01.
 * The complete 276-byte callback retains its old script pointer literal.
 * French moves the owned script with the additional sprite-rendering code;
 * the remaining difference is that stale four-byte pool entry.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
s32 SceneActor_ApplyValueAndMatchingSlots();
extern u8 MsgKorosseoRobinGotItem[];

s32 FieldScene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b)
{
    extern u8 Data_02000240[];

    u8 *work = gKorosseoWork;
    u8 *shared;
    u8 *rec;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = GameFlag_IsSet(0x211);

    shared = Data_02000240;
    rec = ((u8 *(*)())Object_GetById)(*(s32 *)(shared + 500));

    if (*(s32 *)(work + 232) < *(s32 *)(rec + 8)) {
        x = *(s32 *)(work + 232) + 0xc0000;
    } else {
        x = *(s32 *)(work + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(work + 236) + 0x100000;
        cuep = (u16 *)(work + 228);
    } else {
        z = *(s32 *)(work + 236) - 0x100000;
        cuep = (u16 *)(work + 226);
    }

    waitp = (s16 *)(rec + 100);
    *waitp = *cuep;
    *(s32 *)(rec + 52) = 0x4000;
    *(s32 *)(rec + 48) = 0x10000;

    Object_SetMoveTarget(rec, x, 0, z);
    GameFlag_Set(0x211);
    Object_SetScript(rec, (void *)0x0200c6fc);

    while (*waitp != 0) {
        Engine_TaskWait(1);
    }

    if (flag == 0) {
        SceneActor_ApplyValueAndMatchingSlots(0, handle_a);
        UiWork_PushValueSlot(handle_a, 2);
    } else {
        SceneActor_ApplyValueAndMatchingSlots(0, handle_b);
        UiWork_PushValueSlot(handle_b, 2);
    }

    shared = Data_02000240;
    UiWork_PushValueSlot(*(s32 *)(shared + 500), 1);
    Engine_MessageShowCentered((s32)MsgKorosseoRobinGotItem, 3);
    ObjectDispatch_WaitForValue16(rec);

    return flag;
}
