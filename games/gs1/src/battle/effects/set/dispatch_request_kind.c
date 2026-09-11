#include "types.h"
#include "scene.h"
#include "abi/battle/effects/set/dispatch_request_kind.h"

struct BattleEffectRequest {
    u8 reserved_000[0x14];
    void *object;
    s16 source_id;
    s16 target_id;
    u8 reserved_01c[2];
    s16 battle_mode;
    u8 running;
};

struct BattleEffectState {
    u8 reserved_000[0xcb8];
    s16 active;
};

struct BattleEffectGlobals {
    u8 reserved_000[0x24a];
    s16 selected_id;
};

extern struct BattleEffectRequest *gIw;
extern struct BattleEffectGlobals gCell;

void BattleFx_DispatchRequestKind(void)
{
    struct BattleEffectRequest *request = gIw;
    struct BattleEffectState *battle = *(struct BattleEffectState **)((u8 *)&gIw - 0x74);
    s32 battle_mode = request->battle_mode;
    s32 target_id = request->target_id;

    request->running = 0;
    switch (battle_mode) {
    case 2:
        if (battle->active != 0)
            Battle_Run();
        if (gCell.selected_id != request->target_id)
            *(u8 *)((u8 *)request->object + 91) = 1;
        Battle_Apply(request->source_id, target_id);
        break;
    case 1:
        Battle_Do(target_id);
        break;
    case 7:
        Battle_Do2(target_id);
        break;
    case 11:
        Battle_Do3(target_id);
        break;
    case 4:
        Battle_Do4(target_id);
        break;
    case 5:
        Battle_Do5(target_id);
        break;
    case 6:
        Battle_Do6(target_id);
        break;
    case 12:
        Battle_Do7(target_id);
        break;
    case 9:
        if (gCell.selected_id != -1) {
            Battle_Do8(gCell.selected_id);
            gCell.selected_id = -1;
        }
        Battle_Do9(target_id);
        gCell.selected_id = target_id;
        Battle_Do10(target_id);
        break;
    case 3:
        Battle_Do11(target_id);
        break;
    case 14:
        Battle_Do12(target_id);
        break;
    case 13:
        Battle_Do13(target_id);
        break;
    case 8:
        Battle_Run2();
        break;
    case 10:
        Battle_Run3();
        break;
    case 15:
        Battle_Run4();
        break;
    case 16:
        Battle_Run5();
        break;
    }
}
