/* 2026-09-30 Mercury: addresses now spelled by name (gTransitionWork,
   gBattleWork, REG_BLDCNT/REG_BLDALPHA, BattleEvent_Playback) and plain
   integer constants instead of Value_ symbols: 960 of 956 bytes, 380
   differing listing lines. The ROM anchors on gBattleWork (pool) and reaches
   gTransitionWork as base + 140, keeps selection in r9 and a 124-byte frame
   (this spelling: sl, 128). */
/* Draft, not exact (2026-09-24): candidate=956 reference=956 differing_halfwords=437. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
#include "IO_REG.H"
#include "BATTLE_WORK.H"
extern u8 gTransitionWork[];
void BattleEvent_Playback(void);

struct BattlePresentationSelection {
    u8 primary_unit;
    s8 unit_count;
    u8 units[0x4e];
    void *presentation_data;
    u8 unknown_54[4];
    u32 flags;
    s32 message_mode;
};

struct BattlePresentationUnitInfo {
    u8 unknown_00[0x27];
    u8 ability_count;
    void *abilities[1];
};

#define FIELD8(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define FIELD16(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define FIELD32(base, offset) (*(u32 *)((u8 *)(base) + (offset)))

void BattlePresentation_RunUnitTransition(
    struct BattlePresentationSelection *selection,
    s32 mode)
{
    u16 visible_units[14];
    u8 context[0x54];
    u32 primary_unit;
    u32 opposing_unit;
    u32 visible_count;
    u32 refreshed_count;
    u32 index;
    u32 kept_count;
    u32 primary_record;

    BattlePres_BuildTargetList(selection, context);
    primary_unit = selection->primary_unit;
    opposing_unit = selection->units[0];

    if (selection->flags & 0x8000) {
        u32 *transition = *(u32 **)gTransitionWork;
        transition[0] = primary_unit <= 7 ? 0x2000 : 0x00005000;
        transition[1] = 60;
    } else {
        u32 *transition = *(u32 **)gTransitionWork;
        u32 target = primary_unit <= 7 ? 0x00002000 : 0xffffe000;
        if (transition[0] != target) {
            transition[0] = target;
        }
    }

    BattlePres_SetActorModes(0, 0);
    UiWindow_DrawPartyStatusContentsFar((FIELD8((void *)gBattleWork, 0x41)) & ~1);
    primary_record = *(u32 *)GetBattleObjectSlot(primary_unit);
    REG_BLDCNT = 0x3f40;
    visible_count = BattleParty_ListActorIds(3, visible_units);

    for (index = 0; index < visible_count; index++) {
        u16 unit = visible_units[index];
        if (unit != 0xfe) {
            if (unit == primary_unit) {
                Object_SetMode(primary_record, 3);
            } else if ((opposing_unit <= 7) != (unit <= 7)) {
                BattlePres_SetActorRecordMode(unit, 1);
            }
        }
    }

    Audio_PlayCue(0x9a);
    BattleFx_PlayUnitElementEffect(FIELD32(context, 8), selection->presentation_data, 0, 0);
    if (mode & 1) {
        BattlePres_SetActorRecordMode(primary_unit, 1);
    }

    for (index = 0; index < 16; index++) {
        REG_BLDALPHA = (16 - index) | 0x1000;
        WaitFrames(1);
    }

    if (selection->message_mode != 0) {
        if (selection->message_mode == 1) {
            BattleEv_Push(0, primary_unit);
            BattleEv_Push(4, 0x856);
        } else {
            BattleEv_Push(4, 0x855);
        }
        BattleEv_DispatchQueued();
        BattlePres_RunWithZeroArguments();
    } else {
        kept_count = 0;
        for (index = 0; index < visible_count; index++) {
            u16 unit = visible_units[index];
            if (unit == primary_unit) {
                if (!(mode & 1)) {
                    visible_units[kept_count++] = primary_unit;
                }
            } else if ((opposing_unit > 7) != (unit > 7)) {
                visible_units[kept_count++] = unit;
            }
        }
        visible_units[kept_count] = 0xff;
        BattleActor_SpawnObjectsForList(visible_units, 0);

        for (index = 0; index < selection->unit_count; index++) {
            visible_units[index] = selection->units[index];
        }
        visible_units[index] = 0xff;

        for (index = 0; index < FIELD32(context, 0x14); index++) {
            struct BattlePresentationUnitInfo *info;
            u32 ability;
            u32 ability_count;
            u32 unit = FIELD16(context, 0x24 + index * 2);
            info = GetMotionRecord(*(u32 *)GetBattleObjectSlot(unit), 0);
            ability_count = info->ability_count - 1;
            for (ability = 0; ability < ability_count; ability++) {
                FIELD8(context, 0x34 + index * 4 + ability) =
                    FIELD8(info->abilities[ability], 5);
            }
        }

        if (selection->flags & 0x8000) {
            FIELD32(context, 4) = opposing_unit > 7 ? 0 : 1;
        } else {
            FIELD32(context, 4) = selection->units[0] <= 7 ? 1 : 0;
        }
        if (selection->flags & 0x20000) {
            FIELD32(context, 4) ^= 1;
        }

        Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
        if (selection->flags & 0x8000) {
            BattleFx_InitializeModeFar(context);
        } else if (selection->flags & 0x4000) {
            BattleFx_DispatchByIdRangeFar(context);
        } else {
            BattleFx_DispatchModeFar(context);
        }
        BattleEventRuntime_WaitForReady();
    }

    BattleActor_CommitPlacement();
    refreshed_count = BattleParty_ListActorIds(3, visible_units);
    REG_BLDCNT = 0x00003f40;
    for (index = 0; index < refreshed_count; index++) {
        u16 unit = visible_units[index];
        if (unit != 0xfe && unit != primary_unit &&
            ((opposing_unit <= 7) != (unit <= 7))) {
            BattlePres_SetActorRecordMode(unit, 1);
        }
    }
    for (index = 0; index < 16; index++) {
        REG_BLDALPHA = index | 0x1000;
        WaitFrames(1);
    }
    for (index = 0; index < refreshed_count; index++) {
        BattlePres_SetActorRecordMode(visible_units[index], 0);
    }
    BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
    WaitFrames(1);
}
