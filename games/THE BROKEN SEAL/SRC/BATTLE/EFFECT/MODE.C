#include "TYPES.H"
#include "GLOBAL_CELLS.H"

typedef void (*Callback)(s32 *);

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void Runtime_ReleaseHeapBlock(s32);
void BattleEffect_RunCirclingFallingScene(s32 *);
void BattleEffect_RunEmberColumns(s32 *);
void Unnamed_080eb754(s32 *);
void BattleEffect_RunStagedParticles(s32 *);
void BattleEffect_RunDitherDissolveScene(s32 *);
void FunctionHead_080ec100(s32 *);
void BattleFx_RenderAnimationMode0(s32 arg0);
void Unnamed_080d1714(s32 *);
void Unnamed_080ea0d8(s32 *);
void FunctionHead_080d765c(s32 *);
void BattleFx_InitializeDefaultMode(s32 arg0);
void FunctionHead_080e15e8(s32 *);

void BattleFx_InitializeMode(s32 *arg0)
{
    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);

    if (*arg0 == 0) {
        BattleFx_InitializeDefaultMode(arg0);
    } else {
        switch (*arg0) {
        case 1:
            BattleEffect_RunCirclingFallingScene(arg0);
            break;
        case 2:
            BattleEffect_RunEmberColumns(arg0);
            break;
        case 3:
            Unnamed_080eb754(arg0);
            break;
        case 4:
            BattleEffect_RunStagedParticles(arg0);
            break;
        case 5:
            BattleEffect_RunDitherDissolveScene(arg0);
            break;
        case 6:
            FunctionHead_080ec100(arg0);
            break;
        case 7:
            BattleFx_RenderAnimationMode0(arg0);
            break;
        case 8:
            Unnamed_080d1714(arg0);
            break;
        case 9:
            Unnamed_080ea0d8(arg0);
            break;
        case 10:
            FunctionHead_080d765c(arg0);
            break;
        case 11:
            BattleFx_InitializeDefaultMode(arg0);
            break;
        case 12:
            FunctionHead_080e15e8(arg0);
            break;
        }
    }

    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}

extern Callback BattleFx_ModeHandlers[];

void BattleFx_DispatchMode(s32 *state)
{
    s32 index;
    s32 **destination;

    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);

    destination = (s32 **)(*(s32 *)ADDR_03001EEC + 0x7828);
    index = state[0];
    *destination = state;
    if (index == 0)
        state[6] = 0;
    else
        BattleFx_ModeHandlers[index - 1](state);

    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}
