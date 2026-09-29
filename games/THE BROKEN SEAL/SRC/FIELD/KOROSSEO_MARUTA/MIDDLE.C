/* Choosing a friend to cheer, and equipping a prize item. */
#include "LOG_ROLLING.H"

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    extern s32 GetPartyMemberCount();
    extern void Party_RemoveActiveOwner();
    extern s32 UiText_DrawQuantity();
    extern void Party_AddActiveOwner();
    extern void GameFlag_SetByte();
    extern s32 Menu_OpenCharacterSelector();
    extern void Object_LinkObjectAndSetCallback();

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
        count = Value0(GetPartyMemberCount);
        for (i = 0; i < count; i++) {
            s32 at = 504 + i;

            buf[i] = ((u8 *)&gGameState)[at];
        }
        if (count <= 1) {
            Event_SetMessage(MSG_DO_YOUR_BEST);
            Engine_EventShowMessage(owner, 0);
            return;
        }
        if (GameFlag_IsSet(base + 512) != 0) {
            Event_SetMessage(MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE);
            Engine_EventShowMessage(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Task_Wait(6);
        } else {
            Event_SetMessage(MSG_WOULD_LIKE_FRIEND_CHEER_FOR);
            Event_OpenMessage(owner, 0);
            state = Value2(Engine_EventChooseYesNo, 0, 0);
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
    Engine_EventShowMessage(owner, 0);
    return;
L_main:
    ((void (*)())UiText_DrawQuantity)(obj, 1);
    Call1(Engine_EventSetMessage, 0x207f);
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

void ColossoLogRollingStage_ApplyItemToMatchingSlots(s32 handle, s32 item)
{
    extern u8 *Owner_GetState();
    extern s32 Inventory_AddItem();
    extern void Inventory_Equip();

    u8 *record;
    s32 slot;

    record = Owner_GetState(handle);
    Inventory_AddItem(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Inventory_Equip(handle, slot);
        }
    }
}
