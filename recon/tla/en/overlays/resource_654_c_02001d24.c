/* Near miss (80 of 656 instructions differ): the scratch register that
 * reloads gPartyState.current_owner's address from r9 (native r1/r3/r2,
 * draft r2/r3), the fall speed's register stored as the motion flags byte
 * (native strb of 0xffff0000, draft the known zero in r6), and the order of
 * one ldr against a speed constant. Destined for the end of event6.c. */
#include "../../../../games/tla/src/field/overlay_654/shrine_of_the_sea_god.h"
#include "text/msg_ids.h"

extern const u8 Data_02003c58[];

void Func_02003990(struct KabeObject *object, s32 x, s32 y, s32 z);
void Func_02003a90(s32 actor, s32 angle);

static inline void Kabe_ObjAngle(s32 actor, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02003a90(actor, angle);
}
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);

void Func_02001d24(void)
{
    struct KabeObject *object;
    struct KabeObject *leader;
    struct KabeSprite *sprite;
    const void *callback;

    if (Kabe_FlagChk(0x873))
        return;
    Func_020039d8();
    Func_02003b08(0);
    Func_02003ad0()->motion_flags = 0;
    Kabe_CamMove(170 << 17, -1, 176 << 16, 1);
    Kabe_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
    Kabe_ObjSpeed(14, 0xcccc, 0x6666);
    Kabe_ObjWalk(gPartyState.current_owner, 344, 184);
    Kabe_ObjTurn(gPartyState.current_owner, 0x2000, 0);
    Kabe_ObjPlace(14, 344 << 16, 184 << 16);
    WaitFrames(1);
    Kabe_ObjWalk(14, 356, 196);
    Kabe_ObjAngle(14, 0xb000);
    Func_02003a70((s32)MsgFieldPraiseRopeWork);
    Kabe_MesPut(14, 0);
    Kabe_ObjModeWait(gPartyState.current_owner, 3);
    Kabe_MesPut(13, 0);
    Kabe_ObjPlace(13, 184 << 16, 168 << 16);
    Kabe_ObjTurn(gPartyState.current_owner, 0x8000, 0);
    Kabe_ObjTurn(14, 0x8000, 0);
    Kabe_CamSpeed(0x4cccc, 0x9999);
    Kabe_CamMove(232 << 16, 64 << 16, 176 << 16, 1);
    Kabe_ObjSpeed(13, 0x19999, 0xcccc);
    Kabe_ObjWalk(13, 232, 168);
    Kabe_Wait(20);
    Kabe_MesPut(13, 0);
    Kabe_CamSpeed(0x13333, 0x2666);
    Kabe_CamMove(174 << 17, -1, 216 << 16, 1);
    object = Object_GetById(13);
    sprite = object->sprite;
    ObjectMotion_SetActionVariant(13, 1);
    Kabe_ObjSpeed(13, 0x19999, 0xcccc);
    Kabe_ObjLaunch(13, 4, 0);
    Kabe_ObjCommit(13, 248, 168);
    Kabe_ObjMode(13, 6);
    object->motion_flags = object->field_18 = -0x10000;
    sprite->rotation = 0xd000;
    object->y += 0x60000;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(13), 0);
    Kabe_ObjSpeed(13, 0x6666, 0x3333);
    Func_02003990(object, 164 << 17, 152 << 14, 168 << 16);
    ObjectMotion_CommitCurrentPositionAndActivate(13);
    object->motion_flags = 3;
    object->height = 0x200000;
    object->field_18 = 0x10000;
    sprite->rotation = 0;
    Kabe_ObjMode(13, 1);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(13), 1);
    Kabe_ObjSpeed(13, 0x19999, 0xcccc);
    Kabe_ObjLaunch(13, 4, 0);
    Kabe_ObjCommit(13, 328, 184);
    Kabe_ObjTurn(13, 0, 20);
    Kabe_Jump(13, 256, 40);
    ObjectMotion_SetActionVariant(13, 2);
    Kabe_MesPut(0x400d, 0);
    Kabe_MesPut(0x200e, 0);
    Kabe_Jump(13, 257, 20);
    Kabe_MesPut(0x400d, 0);
    Kabe_ObjTurn(14, 0xb000, 40);
    Kabe_ObjTurn(14, 0x8000, 0);
    Func_02003a78(0x200e, 0, 60);
    Kabe_MesPut(0x400d, 0);
    Kabe_ObjMode(13, 3);
    Kabe_ObjPlace(12, 394 << 16, 286 << 16);
    WaitFrames(1);
    Kabe_ObjSpeed(12, 0x19999, 0xcccc);
    Kabe_ObjWalk(12, 394, 248);
    Kabe_ObjTurn(12, 0xb000, 0);
    Kabe_ObjLaunch(12, 4, 20);
    Kabe_MesPut(0x400c, 0);
    Kabe_Jump(13, 256, 0);
    Kabe_ObjTurn(13, 0x3000, 0);
    Kabe_ObjTurn(gPartyState.current_owner, 0x2000, 0);
    Kabe_ObjTurn(14, 0x3000, 0);
    Kabe_CamSpeed(0x4cccc, 0x9999);
    Kabe_CamMove(188 << 17, -1, 264 << 16, 1);
    Func_02003ac8();
    Kabe_MesPut(12, 0);
    Kabe_CamSpeed(0x26666, 0x4ccc);
    Kabe_CamMove(174 << 17, -1, 216 << 16, 1);
    Object_LinkObjectAndSetCallback(gPartyState.current_owner, 12);
    Object_LinkObjectAndSetCallback(14, 12);
    Object_LinkObjectAndSetCallback(13, 12);
    Kabe_ObjWalk(12, 394, 220);
    Kabe_ObjWalk(12, 370, 198);
    Kabe_ObjSpeed(14, 0x19999, 0xcccc);
    Kabe_ObjCommit(14, 368, 180);
    Object_LinkObjectAndSetCallback(14, 12);
    Kabe_ObjWalk(12, 336, 198);
    Kabe_ObjTurn(12, 0xb000, 0);
    ObjectMotion_EnableActionAndResetMotion(13);
    Kabe_MesPut(0x400d, 0);
    ObjectMotion_ArmCallback(12, 0xd000, 0);
    Kabe_MesPut(12, 0);
    Kabe_ObjAngle(12, 0xb000);
    Kabe_ObjLaunch(12, 4, 20);
    Func_02003a78(12, 0, 20);
    Kabe_Jump(13, 261, 40);
    Kabe_Jump(12, 257, 60);
    Kabe_MesPut(12, 0);
    Kabe_ObjMode(13, 4);
    Kabe_MesPut(0x400d, 0);
    Kabe_Jump(12, 256, 20);
    Kabe_MesPut(12, 0);
    Kabe_ObjTurn(13, 0x8000, 20);
    Kabe_MesPut(0x400d, 0);
    Kabe_ObjTurn(12, 0x8000, 0);
    Kabe_MesPut(12, 0);
    Kabe_ObjModeWait(13, 4);
    Kabe_MesPut(0x400d, 0);
    Kabe_Jump(12, 265, 40);
    Kabe_MesPut(12, 0);
    Kabe_Jump(12, 256, 0);
    Kabe_Jump(13, 256, 0);
    Kabe_ObjTurn(12, 0xb000, 0);
    Kabe_ObjTurn(13, 0x3000, 40);
    Kabe_ObjLaunch(13, 4, 40);
    Kabe_MesPut(0x400d, 0);
    Kabe_ObjMode(12, 3);
    Kabe_MesPut(12, 0);
    Kabe_ObjModeWait(13, 3);
    Kabe_MesPut(0x400d, 0);
    Kabe_MesPut(12, 0);
    Kabe_ObjMode(12, 3);
    Kabe_ObjModeWait(13, 3);
    Kabe_ObjSpeed(12, 0x13333, 0x9999);
    Kabe_ObjSpeed(13, 0x13333, 0x9999);
    callback = Data_02003c58;
    ObjectMotion_EnableActionAndSetCallback(12, callback);
    Kabe_Wait(10);
    Object_SetActionCallbackAndRefreshById(13, callback);
    Kabe_Wait(40);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    ObjectMotion_EnableActionAndResetMotion(14);
    WaitFrames(1);
    Func_02003a78(14, 0, 40);
    Kabe_ObjPlace(13, 0, 0);
    Kabe_ObjPlace(12, 0, 0);
    Kabe_MesPut(14, 0);
    Kabe_ObjTurn(gPartyState.current_owner, 0, 0);
    Kabe_ObjTurn(14, 0x8000, 20);
    Kabe_MesPut(14, 0);
    Kabe_ObjMode(14, 2);
    leader = Object_GetById(gPartyState.current_owner);
    if (leader != NULL)
        ObjectMotion_ResetAndSetPosition(14, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
    ObjectMotion_CommitCurrentPositionAndActivate(14);
    Kabe_ObjPlace(14, 0, 0);
    Kabe_FlagSet(0x873);
    Func_020039e0();
}
