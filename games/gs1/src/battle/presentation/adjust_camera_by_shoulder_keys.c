#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "battle_msg.h"
#include "battle_escape.h"
#include "battle_presentation.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "battle_party.h"

/* battle/presentation/cam/shoulder.c */
extern u16 Value_fffffe00;

void BattlePres_AdjustCameraByShoulderKeys(void)
{
    void **slot = (void **)ADDR_03001E80;
    u8 *cam = slot[0];
    u8 *trans = slot[32];
    volatile u32 *keys = (volatile u32 *)ADDR_03001AE8;

    if ((*keys & 512) != 0) {
        *(u16 *)(cam + 54) += 512;
    }
    if ((*keys & 256) != 0) {
        *(u16 *)(cam + 54) += (u16)(u32)&Value_fffffe00;
    }
    if (*(u32 *)(trans + 20) == 0) {
        Battle_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

/* battle/presentation/act/run.c */
struct BattlePresentationTransition {
    s32 battle_value;
    s32 timer;
    u8 reserved08[12];
    s32 flag;
};

extern struct BattlePresentationTransition *gIw;

void *Runtime_GetObject(s32);

void UiText_ShowMessageAndWait(s32);
void WaitFrames(s32);

s32 BattlePres_RunAction(s16 *action)
{
    struct BattlePresentationTransition *transition;
    s32 actor_id;
    s32 battle_mode;
    u8 *actor;

    actor_id = action[0];
    actor = Runtime_GetObject(actor_id);
    if (*(s16 *)(actor + 0x38) == 0)
        return -1;

    action[5] = BattleTarget_ReplaceDefeated((u8 *)action);
    transition = gIw;
    if (action[0] > 4)
        battle_mode = -0x2000;
    else
        battle_mode = 0x2000;
    transition->battle_value = battle_mode;
    transition->timer = 60;
    Battle_Run();

    switch (action[3]) {
    case 99:
        UiText_ShowMessageAndWait((s32)&Value_00000843);
        if (BattleEscape_PlayRun(action)!= 0)
            return 1;
        break;
    case 3:
        WaitFrames(45);
        Battle_Check(action);
        break;
    case 2:
        WaitFrames(45);
        BattlePres_RunUnitAction(action);
        break;
    case 0:
    default: {
        struct BattlePresentationTransition *tr = gIw;
        tr->flag = 0;
        BattlePres_RunUnitAction(action);
        tr->flag = 0;
        break;
    }
    case 1:
        BattlePres_RunPairedUnitTransition(action);
        break;
    }

    UiWork_ResetFreeChannel();
    return 0;
}

/* battle/object/is_valid_id.c */
s32 BattleObject_IsValidId(u32 object_id)
{
    if (object_id <= 7) {
        return 0;
    }
    if (object_id - 128 <= 5) {
        return 0;
    }
    return -1;
}

/* battle/escape/play_run.c */
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16


s32 BattleEscape_PlayRun(s16 *action)
{
    s16 party_members[14];
    s32 party_size;
    s32 animated;
    s32 member_slot;

    (void)action;
    if (((u32)(Rand() << 4) >> 16) != 0) {
        party_size = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            party_members);
        animated = 0;
        if (party_size != 0) {
            member_slot = 0;
            do {
                Battle_Do(party_members[member_slot]);
                animated++;
                WaitFrames(8);
                member_slot++;
            } while (animated != party_size);
        }
        WaitFrames(22);
        return 1;
    }
    UiText_ShowMessageAndWait((s32)&Value_00000844);
    return 0;
}

/* battle/presentation/misc/msg_field38.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))


void UiText_DrawQuantity(s32, s32);

s32 BattlePres_ShowMessageWhenField38Positive(s16 *script)
{
    s32 object_id;
    s32 result;
    void *object;

    object_id = *script;
    object = Runtime_GetObject(object_id);
    if (Battle_Check(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (FIELD(object, s16 *, 0x38) <= 0) {
        return result;
    }
    Battle_Run();
    UiText_DrawQuantity(object_id, 1);
    UiText_ShowMessageAndWait((s32)&Value_00000816);
    return 0;
}
