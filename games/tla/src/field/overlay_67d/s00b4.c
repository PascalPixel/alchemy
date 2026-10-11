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

static inline s32 FlagChk(s32 flag)
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
    Func_0200226c(gPartyState.current_owner, animation, frames);
}

const u8 *Func_020000b4(void)
{
    if (FlagChk(0x927))
        return Data_02002d80;
    if (gPartyState.entrance == 7)
        return Data_02002cf0;
    return Data_02002b40;
}

const u8 *Func_020000f0(void)
{
    if (FlagChk(0x927))
        return Data_0200323c;
    if (gPartyState.entrance == 7)
        return Data_02003164;
    return Data_02002f78;
}

void Func_0200012c(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing load; as a plain if the ARM9TDMI schedule builds 0x2000 first (differ 2). */
    do {
        if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
            Func_0200230c(actor);
        } else {
            Func_020021c4();
            Func_020022bc(0);
            if (FlagChk(0x927)) {
                Func_02002244((s32)MsgNowThatPeace);
                Func_02002254(12, 0);
            } else {
                Func_02002244((s32)MsgWhyWouldOur);
                Func_02002254(22, 0);
            }
            Func_020021cc();
        }
    } while (0);
}

void Func_020001a8(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing load; as a plain if the ARM9TDMI schedule builds 0x2000 first (differ 2). */
    do {
        if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
            Func_02002314(9, actor);
        } else {
            Func_020021c4();
            Func_020022bc(0);
            if (FlagChk(0x927))
                Func_02002244((s32)MsgBriggsFromChampa);
            else
                Func_02002244((s32)MsgOfCourseI);
            Func_02002254(actor, 0);
            Func_020021cc();
        }
    } while (0);
}

void Func_02000220(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing load; as a plain if the ARM9TDMI schedule builds 0x2000 first (differ 2). */
    do {
        if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
            Func_02002304(24, actor);
        } else {
            Func_020021c4();
            Func_020022bc(0);
            if (FlagChk(0x927))
                Func_02002244((s32)MsgYouMustSee);
            else
                Func_02002244((s32)MsgIzumoSLeaders);
            Func_02002254(actor, 0);
            Func_020021cc();
        }
    } while (0);
}

void Func_02000298(void)
{
    Func_020021c4();
    Func_020022bc(0);
    if (!FlagChk(0x922)) {
        Func_02002244((s32)MsgSusaSNot);
        Func_02002254(8, 0);
        Battle_WaitMode0(30);
        ActorAnimate(8, 265, 0);
        Battle_WaitMode0(60);
        Func_02002254(8, 0);
        Battle_WaitMode0(30);
        ActorAnimate(8, 261, 0);
        Battle_WaitMode0(60);
        Func_02002254(8, 0);
        Battle_WaitMode0(30);
        ObjectMotion_SetVariantCallback(8, 1);
        Battle_WaitMode0(60);
        Func_02002254(8, 0);
        Battle_WaitMode0(30);
        FlagSet(0x922);
    }
    Object_SetModeById(8, 4);
    Battle_WaitMode0(60);
    Func_02002244((s32)MsgIfTheSerpent);
    Func_02002254(8, 0);
    Func_020021cc();
}

void Func_02000358(void)
{
    FlagSet(0x923);
    Func_02002244((s32)MsgIPrayThat);
    Func_02002254(8, 0);
}

void Func_02000378(void)
{
    void *work = Ram_HeapSlots->event_work;

    Func_020021c4();
    Func_020022bc(0);
    ObjectMotion_SetAngleToward(15, gPartyState.current_owner, 0);
    if (FlagChk(0x92c)) {
        Func_02002244((s32)MsgWithThisDancing);
        Func_02002254(15, 0);
    } else {
        Func_02002244((s32)MsgIfYouHave);
        Func_02002254(15, 0);
        if (PartyInventory_FindOwner(451) != -1)
            FIELD_AT(work, u16, 346) = 1;
    }
    ActorTurn(15, 0x4000, 0);
    Func_020021cc();
}

void Func_02000400(void)
{
    s32 msg;

    Func_020021c4();
    Func_020022bc(0);
    msg = (s32)MsgTheyVeBeen;
    Func_02002244(msg);
    UiText_OpenMessageAtObject(16, 0);
    if (Inventory_PromptAndSetObjectMode(Func_020022dc(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02002244(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02002244(msg + 2);
    }
    Func_02002254(16, 0);
    Func_020021cc();
}
