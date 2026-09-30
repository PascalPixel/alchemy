#include "TYPES.H"

struct BattleActionRecord {
    s16 unit_id;
    u8 reserved_02[2];
    u16 value;
    s16 kind;
    u8 reserved_08[8];
};

struct BattleTransitionWork {
    s32 angle;
};

struct CharacterRuntimeRecord {
    u8 reserved_00[0x12b];
    u8 presentation_side;
};

extern u8 *gBattleWork;
extern struct BattleTransitionWork *gTransitionWork;

void BattleUnit_ClearField12bForGroup(void);
void Palette_CopyBanksWithBrightnessOffset(s32 offset);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void Camera_ConfigureScene(s32 scene);
void UiWork_FinalizeSharedSlotFar(void);
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *actions);
s32 BattleEscape_CheckSuccess(void);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
s32 BattlePres_WaitSync(void);
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *actions);
void BattleQueue_SortByPriority(struct BattleActionRecord *actions, s32 count);
struct CharacterRuntimeRecord *Owner_GetStateFar(s32 unit_id);
void Camera_InitDefaultTransform(void);

/*
 * Builds the turn's action list: clears the twenty queued records, lets the
 * party choose (unless an escape already ended the choice), appends the
 * linked player's or the opponents' actions, sorts them by priority and
 * marks the side each acting unit presents from. Returns the action count,
 * or -1 when the link fails.
 */

s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions)
{
    struct CharacterRuntimeRecord *character;
    u8 *battle;
    u8 *mode;
    s32 count;
    s32 added;
    s32 i;

    battle = gBattleWork;
    {
        struct BattleActionRecord *queued = (struct BattleActionRecord *)(battle + 187 * 4);
        u32 n;

        for (n = 0; n < 20; n++) {
            queued[n].unit_id = 0xff;
            queued[n].value = 0x8000;
        }
    }

    BattleUnit_ClearField12bForGroup();
    Palette_CopyBanksWithBrightnessOffset(8);
    GameFlag_SetBitFar(0x16b);
    mode = battle + 69;
    Camera_ConfigureScene(0);
    UiWork_FinalizeSharedSlotFar();

    if (*mode != 2) {
        count = BattlePres_BuildUnitEntries(actions);
        if (count < 0)
            goto finish;
        if (count != 0 && actions[0].kind == 99 && BattleEscape_CheckSuccess() == 0)
            *mode = 2;
    } else {
        count = 0;
    }

    if (gBattleWork[68] != 0) {
        added = BattlePresentation_AppendLinkedActions(actions, count);
        if (BattlePres_WaitSync() < 0) {
            count = -1;
            goto finish;
        }
        count += added;
        if (added < 0) {
            count = -1;
            goto finish;
        }
    } else {
        count += BattlePres_BuildOpponentEntries(actions + count);
    }

    BattleQueue_SortByPriority(actions, count);
    if (count > 0) {
        struct BattleActionRecord *action = actions;

        i = count;
        do {
            if (action->kind == 3 || action->kind == 7) {
                character = Owner_GetStateFar(action->unit_id);
                character->presentation_side = action->kind == 3 ? 1 : 2;
            }
            action++;
            i--;
        } while (i != 0);
    }

finish:
    GameFlag_ClearBitFar(0x16b);
    Camera_InitDefaultTransform();
    gTransitionWork->angle = 0x2000;
    return count;
}
