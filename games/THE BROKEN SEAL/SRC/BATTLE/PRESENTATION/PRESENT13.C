#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_MSG.H"

struct BattlePresentationState {
    s32 mode;
    u8 unknown_04[12];
    s32 active;
};

struct BattleSceneContext {
    u8 unknown_00[0x45];
    u8 encounter_mode;
};

struct BattleTrigger {
    u8 unit_id;
};

struct BattleUnit *Owner_GetStateFar(s32 unit_id);
void BattlePres_SetActorModes(u16 *, s32);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void BattlePresentation_WaitForAdvance(void);
void BattleMotion_SetupEscapeObject(s32 unit_id);
void WaitFrames(s32 frames);
u32 Random16(void);
void BattleActor_RemoveFromLists(s32 unit_id);
void ActivateBattleObjectSlot(s32 unit_id);
extern u8 gTransitionWork[];

s32 BattlePres_BuildTargetList(void *desc, void *output)
{
    s32 sp0;
    void *sp4;
    s16 *dst;
    s32 v;
    s32 cnt;
    s32 i;
    u8 *p;
    void *out;

    out = output;
    cnt = 0;
    FIELD_AT_OFFSET(out, s32 *, 0x1C) = 0;
    v = FIELD_AT_OFFSET(desc, s32 *, 0x58);
    FIELD_AT_OFFSET(out, s32 *, 0) = (s32)(0xFFF & v);
    FIELD_AT_OFFSET(out, u32 *, 0x18) = (u32)((u32)(v & 0x3000) >> 0xC);
    FIELD_AT_OFFSET(out, s32 *, 8) = (s32)FIELD_AT_OFFSET(desc, u8 *, 0);
    i = 0;
    if (i < (s32)FIELD_AT_OFFSET(desc, s8 *, 1)) {
        p = desc + 2;
        dst = out + 0x24;
        do {
            sp4 = out;
            sp0 = cnt;
            if ((Owner_GetStateFar(*p)->hp != 0) || (FIELD_AT_OFFSET(desc, s32 *, 0x58) & 0x10000)) {
                cnt += 1;
                *dst = (s16)*p;
                dst += 1;
            }
            i += 1;
            p += 1;
        } while (i < (s32)FIELD_AT_OFFSET(desc, s8 *, 1));
    }
    if (cnt == 0) {
        FIELD_AT_OFFSET(out, s16 *, 0x24) = (s16)FIELD_AT_OFFSET(desc, u8 *, 2);
        cnt = 1;
    }
    FIELD_AT_OFFSET(out, s32 *, 0xC) = (s32)FIELD_AT_OFFSET(desc, u8 *, 2);
    FIELD_AT_OFFSET(out, s32 *, 0x14) = cnt;
    FIELD_AT_OFFSET(out, s32 *, 0x10) = 1;
}

s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BattleTrigger *trigger)
{
    u8 *presentation_addr = gTransitionWork;
    struct BattlePresentationState *presentation =
        *(struct BattlePresentationState **)presentation_addr;
    struct BattleSceneContext *scene =
        *(struct BattleSceneContext **)(presentation_addr - 140);
    s32 completed = 0;
    s32 party_mode;

    presentation->mode = 0x2000;
    presentation->active = 1;
    BattlePres_SetActorModes(0, 0);

    if (trigger->unit_id <= 7) {
        /* FAKEMATCH: an opaque copy keeps cse from folding party_mode to the constant zero */
        asm("mov %0, %1" : "=r"(party_mode) : "r"(completed));

        if (scene->encounter_mode != 2) {
            party_mode = 1;
        }

        if (!party_mode) {
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgNoEscape);
            BattlePresentation_WaitForAdvance();
        } else {
            s16 unit_ids[14];
            s32 index = BattleParty_ListLivingUnits(1, (u16 *)unit_ids) - 1;

            while (index != -1) {
                struct BattleUnit *unit = Owner_GetStateFar(unit_ids[index]);

                if (unit->stun == 0 && unit->sleep == 0) {
                    BattleMotion_SetupEscapeObject(unit_ids[index]);
                    WaitFrames(8);
                }
                index--;
            }
            WaitFrames(22);
            completed = 1;
        }
    } else if (((Random16() * 10) >> 16) <= 6) {
        s16 event[2];

        event[0] = trigger->unit_id;
        event[1] = 0xff;
        BattleMotion_SetupEscapeObject(event[0]);
        WaitFrames(8);
        BattleActor_RemoveFromLists(trigger->unit_id);
        ActivateBattleObjectSlot(trigger->unit_id);
    } else {
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgNoEscape);
        BattlePresentation_WaitForAdvance();
    }

    presentation->active = 0;
    return completed;
}
