/* resource_3bb 0x02009ffc..0x0200a114: two owners that compiled exactly with
 * GCC 2.96 before the overlay was split into per-run sources; written against
 * the old single TASK.C declarations (see FIELD/KOROSSEO_KABE/TASK.H).
 * Remaining difference: both load the scene numbers 0x8f and 0x90 from their
 * literal pools, which only a link-time symbol explains, and the main image
 * defines no such names (the old Value_ equates were aliases). The calls it
 * still spells by address need the names of the veneers they reach before
 * adopting. */
#include "TASK.H"
extern u8 MsgKorosseoStageFirstFinalsMatch[];
extern u8 MsgKorosseoStageSecondFinalsMatch[];
extern u8 MsgKorosseoStageThirdFinalsMatch[];
extern u8 MsgKorosseoWouldYouLikeHearDescription[];

extern u8 Value_0000008f;
extern u8 Value_00000090;
extern s16 SceneStateHalfwords[];
void Func_02006084(void);
void Func_02005ebc(s32, s32);
s32 Func_02005f1e(s32);
void Func_02005f72(s32, s32);

s32 KorosseoKabe_RunStateInteraction(s32 a, s32 b)
{

    s32 v;
    s32 id;
    s32 r;

    Func_02006084();
    Func_02005ebc(b, 5);
    v = SceneStateHalfwords[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        id = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(id);
    Event_ShowMessage(a, 0);
    if (GameFlag_IsSet(b + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(b + 520) != 0) {
        r = Func_02005f1e(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    GameFlag_Set(b + 520);
    Event_SetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
    Event_OpenMessage(a, 0);
    return Event_ChooseYesNo(0, 0);
}

void KorosseoKabe_ShowFollowUpPrompt(s32 a, s32 b)
{

    s32 v;
    s32 id;

    Func_02005f72(b, 5);
    v = SceneStateHalfwords[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        id = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(id + 1);
    Event_ShowMessage(a, 0);
}
