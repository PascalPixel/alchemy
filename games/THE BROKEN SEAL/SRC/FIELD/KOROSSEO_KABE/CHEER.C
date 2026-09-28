#include "TASK.H"

/* The game state's cells, read here as bytes. */
extern u8 gCell[];

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    s32 rec;
    s32 record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = Value1(Engine_ActorGet, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Party_CountActiveOwners);
        for (i = 0; i < count; i++) {
            buf[i] = gCell[504 + i];
        }
        if (count <= 1) {
            Event_SetMessage(MSG_DO_YOUR_BEST);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (GameFlag_IsSet(base + 512) != 0) {
            Event_SetMessage(MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE);
            Event_ShowMessage(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Task_Wait(6);
        } else {
            Event_SetMessage(MSG_WOULD_LIKE_FRIEND_CHEER_FOR);
            Event_OpenMessage(owner, 0);
            state = Event_ChooseYesNo(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Party_RemoveActiveOwner((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Party_AddActiveOwner((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Menu_OpenCharacterSelector);
            for (i = 0; i < count; i++) {
                Party_RemoveActiveOwner((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Party_AddActiveOwner((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Event_SetMessage(MSG_IF_KNOW_WHO_WANT_CHEER);
    Event_ShowMessage(owner, 0);
    return;
L_main:
    ((void (*)())UiWork_PushValueSlot)(obj, 1);
    Event_SetMessage(MSG_ROBIN_WILL_CHEER_FOR_WAY);
    Event_ShowMessage(owner, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(obj, 0x10000, 0x8000);
    Actor_SetSpeed(owner, 0x10000, 0x8000);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(obj, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    hi = p11 + 16;
    Actor_WalkToAndWait(obj, p9, hi);
    lo = p9 + 16;
    Value3(Engine_ActorWalkToAndWait, 0, lo, hi);
    Actor_FaceEachOther(obj, ACTOR_PARTY_LEADER, 30);
    Actor_SetAnimation(obj, 3);
    tail = hi - 32;
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(owner, p9, tail);
    Value3(Engine_ActorWalkTo, owner, lo, tail);
    Object_LinkObjectAndSetCallback(0, obj);
    Actor_WalkToAndWait(obj, p9, tail);
    Actor_SetAnimation(owner, 1);
    Actor_FaceDirection(owner, 0x8000, 0);
    Actor_WalkToAndWait(obj, p9, p11 - 48);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkToAndWait(owner, p9, p11);
    Party_RemoveActiveOwner(obj);
    GameFlag_Set(base + 512);
    rec = Value1(Engine_ActorGet, obj);
    sx = *(s32 *)(rec + 8) >> 20;
    GameFlag_SetByte((obj << 4) + 880, sx);
    sy = *(s32 *)(rec + 16) >> 20;
    GameFlag_SetByte((obj << 4) + 888, sy);
}

/*
* Look up an object by arg0, then scan the first 15 halfword
 * entries of its table at offset 0xd8 for one equal to arg1, calling a handler
* with each matching index.  The callees are identified by call shape only, and
 * the table's role is inferred from this scan alone.
 */
void OverlayObject_NotifyMatchingEntries(s32 no, s32 val)
{
    u16 *tbl = Owner_GetState(no);
    s32 i;

    Inventory_AddItem(no, val);

    tbl = (u16 *)((char *)tbl + 0xd8);
    for (i = 0; i <= 14; i++) {
        if (tbl[i] == val) {
            Inventory_Equip(no, i);
        }
    }
}
