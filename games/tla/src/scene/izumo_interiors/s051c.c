#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

extern const u8 Data_02002b40[];
extern const u8 Data_02002cf0[];
extern const u8 Data_02002d80[];
extern const u8 Data_02002f78[];
extern const u8 Data_02003164[];
extern const u8 Data_0200323c[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void Func_020021c4(void);
void Func_020021cc(void);
void Func_020022bc(s32 skip);
void Func_02002244(s32 message);
void Func_02002254(s32 speaker, s32 flags);
void Func_0200226c(s32 actor, s32 animation, s32 frames);
s32 Func_020022dc(void);
void Func_02002304(s32 slot, s32 actor);
void Func_0200230c(s32 actor);
void Func_02002314(s32 slot, s32 actor);
void Func_0200061c(s32 mode);
void ObjectMotion_SetVariantCallback(s32 actor, s32 variant);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_OffsetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
s32 PartyInventory_FindOwner(s32 item);
void UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);

static inline s32 FlagCheck(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void ActorTurn(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void ActorAnimate(s32 actor, s32 animation, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200226c(actor, animation, frames);
}

static inline void LeaderAnimate(s32 animation, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200226c(gPartyState.currentOwner, animation, frames);
}

s32 Func_0200051c(void)
{
    Func_020021c4();
    Func_020022bc(0);
    if (!FlagCheck(0x925)) {
        Func_02002244((s32)MsgSusaDonT);
        Func_02002254(17, 0);
        Battle_WaitMode0(60);
        Func_02002254(17, 0);
        Battle_WaitMode0(60);
        Battle_WaitMode0(15);
        Func_02002254(17, 0);
        Battle_WaitMode0(60);
        Battle_WaitMode0(15);
        Func_02002254(17, 0);
        Battle_WaitMode0(60);
        ObjectMotion_SetVariantCallback(17, 1);
        Battle_WaitMode0(30);
        Func_02002254(17, 0);
        Battle_WaitMode0(60);
        FlagSet(0x925);
    } else {
        Func_02002244((s32)MsgSusaPleaseDon);
        Func_02002254(17, 0);
    }
    Object_GetById(gPartyState.currentOwner)->actionFlags &= ~1;
    ObjectMotion_OffsetPositionAndReset(gPartyState.currentOwner, 0, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
    Object_GetById(gPartyState.currentOwner)->actionFlags |= 1;
    Func_020021cc();
    return 0;
}

void Func_02000604(void)
{
    Func_0200061c(0);
}

void Func_02000610(void)
{
    Func_0200061c(1);
}
