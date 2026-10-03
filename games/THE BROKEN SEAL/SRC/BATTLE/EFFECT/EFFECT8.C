#include "BATTLE_EFFECT_WORK.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"

extern u8 gBattleFxWork[];
typedef void (*Callback)(s32 *);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void BattleEffect_RunCirclingFallingScene(s32 *);
void BattleEffect_RunEmberColumns(s32 *);
void BattleFx_RunObjectRow(s32 *);
void BattleEffect_RunStagedParticles(s32 *);
void BattleFx_InitializeMode6(s32 *);
void BattleFx_RenderAnimationMode0(s32 arg0);
void Unnamed_080d1714(s32 *);
void Unnamed_080ea0d8(s32 *);
void BattleFx_InitializeMode10(s32 *);
void BattleFx_InitializeDefaultMode(s32 arg0);
void BattleFx_InitializeMode12(s32 *);
extern Callback BattleFx_ModeHandlers[];

extern u8 gMapCellBuffer[];

void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);


void BattleFx_InitializeMode(s32 *arg0)
{
    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, sizeof(struct BattleEffectWork));
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
            BattleFx_RunObjectRow(arg0);
            break;
        case 4:
            BattleEffect_RunStagedParticles(arg0);
            break;
        case 5:
            BattleEffect_RunDitherDissolveScene((struct BattleEffectArgument *)arg0);
            break;
        case 6:
            BattleFx_InitializeMode6(arg0);
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
            BattleFx_InitializeMode10(arg0);
            break;
        case 11:
            BattleFx_InitializeDefaultMode(arg0);
            break;
        case 12:
            BattleFx_InitializeMode12(arg0);
            break;
        }
    }

    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}

void BattleFx_DispatchMode(s32 *state)
{
    s32 index;
    struct BattleEffectWork *work;

    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, sizeof(struct BattleEffectWork));
    Runtime_AllocateHeapBlock(40, 0x4000);

    work = *(struct BattleEffectWork **)gBattleFxWork;
    index = state[0];
    work->effect = (struct BattleEffectArgument *)state;
    if (index == 0)
        state[6] = 0;
    else
        BattleFx_ModeHandlers[index - 1](state);

    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}

/* Builds the per-line WIN0H table for lines 8-135 (the edge at the start of
   the map cell buffer less each line's inset, clamped to the screen; other
   lines closed) and arms the H-blank DMA that feeds it to WIN0H. */
void BattleFx_BuildWindowEdgeTable(void)
{
    u16 *edge;
    u16 *line;
    u8 *inset;
    s32 i;
    s32 right;

    edge = (u16 *)gMapCellBuffer;
    line = (u16 *)(Ram_MapCellBuffer + 0x82);
    inset = Ram_MapCellBuffer + 2;
    for (i = 0; i != 160; i++) {
        if ((u32)(i - 8) <= 127) {
            right = *edge - inset[i - 8];
            if (right < 0)
                right = 0;
            if (right > 240)
                right = 240;
            line[i] = right;
        } else {
            line[i] = 0xfff1;
        }
    }
    {
        volatile u16 *channel = (volatile u16 *)0x040000b0;
        channel[5] &= 0xc5ff;
        channel[5] &= 0x7fff;
        (void)channel[5];
        Dma_Set(gMapCellBuffer + 0x82, (void *)0x04000040, 0xa2600001, (volatile u32 *)channel);
    }
}

void BattleFx_SelectLivingTargets(struct BattleEffectArgument *argument)
{
    s16 targets[14];
    s32 count;
    s32 i;

    count = 0;
    if (argument->actors[0] > 127) {
        for (i = 0; i != 6; i++) {
            s32 unit = i + 128;

            if (Owner_GetStateFar(unit)->hp > 0)
                targets[count++] = unit;
        }
    } else {
        for (i = 0; i != 8; i++) {
            if (Owner_GetStateFar(i)->hp > 0)
                targets[count++] = i;
        }
    }
    targets[count] = 0xff;
    BattleActor_SpawnObjectsForListFar(targets, 0);
}
