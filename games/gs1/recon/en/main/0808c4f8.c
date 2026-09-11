#include "types.h"

/*
 * Battle mode entry.  Allocates the 0xCCC-byte battle work block, brings the
 * battle presentation up, then runs the battle's outer turn loop until one of
 * the work block's request slots posts an exit code.
 *
 * Shape recovered from the complete reference disassembly of the owner:
 *
 *   - work: the 0x1B / 0xCCC block from Runtime_AllocateBlock, cleared by a
 *     DMA3 fixed-source word fill before anything reads it.  Its request
 *     slots are a dense s16 band at +0x16C..+0x186 polled highest-priority
 *     first by the inner dispatch chain; every handler clears its own slot.
 *     +0x14 is the actor pointer table, +0x19E the battle mode, +0x1C0 and
 *     +0x1C8 the pair handed to Func_0808fefc / Func_080901c0, +0x1C6 the
 *     flag saying that pair is live.
 *   - g: the shared progress/party block at 0x02000240.  Held in a pointer
 *     because the reference addresses every field of it as base register plus
 *     offset register, never as one folded pool constant; the four cells it
 *     does fold (Data_02000402 / 0200042c / 02000434 / 02000478) are declared
 *     separately for the same reason.
 *   - actor: work's actor table entry selected by Data_02000434.
 *   - mode: 3 when g+0x1DA holds 1, otherwise 2.  Stored to work+0x19E and
 *     passed to Func_08009078; it also picks between Func_08009130/38 and
 *     Func_08009128 and, in the tail, between Func_080090a8 and Func_080090a0.
 *   - result: the returned value.  0 normally, the code a handler posted at
 *     work+0x170 when one is pending, or 999 on the path that ends the battle
 *     from the -1 request.
 *
 * Uncertain and deliberately left neutral: the role of most g fields, the
 * 0x1000 bit OR-ed into work+0x178, the -888 / -889 request codes, and the
 * 136 threshold on the 512-byte census over Data_03001810.
 *
 * Residual: this is a draft, not a match.  Every reference call, branch,
 * store and loop is represented and the call multiset is identical, but the
 * candidate still differs pervasively in register assignment (the reference
 * keeps the work pointer in r8 and the result in r7; this source gets the
 * opposite), in where GCC parks small constants in the literal pools, and the
 * reference's three DMA register stores collapse into one stmia/sub pair that
 * this spelling does not reach.  Those are allocation and peephole outcomes,
 * not missing behaviour.
 */

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

#define Runtime_ReleaseHeapBlock       Func_08002dd8
#define Battle_SetObjectFlag5bWhenMode3   Func_0808c44c
#define Battle_ClearObjectFlag5bWhenMode3 Func_0808c4c0
#define BattleFx_InitializeBuffers Func_08091174
#define Party_ResolveTablePair         Func_0808b1d8
#define Party_CheckMemberValueTotal    Func_080919d8
#define PartyInventory_GiveItem        Func_08091a58
#define ObjectEffect_RunPendingFlagEvent Func_08094428
#define BattleFx_ScheduleCallbackWhenValue24cSet Func_08099810
#define BattleAction_RunDescriptor     Func_0808d828
#define Battle_DispatchInputEvent      Func_0808d8f0
#define BattleFx_FindDescriptorWithOverride Func_0808d5a4
#define BattleFx_RunDescriptorAction Func_0808d5dc
#define BattleFx_RunKind6DescriptorAction Func_0808d7d8
#define BattleCommand_ExecuteSelectedAction Func_0808e680
#define BattleCommand_ExecuteSelectedItem   Func_0808e23c
#define Battle_ResetEffectCounter      Func_0808e118

void *Func_080048f4();
void Func_08002dd8();
void Func_080030f8();
void Func_080040e8();
s32 Func_080072e4();
void Func_08009078();
void Func_08009080();
void Func_080090a0();
void Func_080090a8();
void Func_080090b0();
void Func_080090b8();
void Func_08009110();
void Func_08009118();
void Func_08009128();
void Func_08009130();
void Func_08009138();
void Func_08009140();
void Func_08015000();
void Func_08015040();
void Func_08015120();
void Func_080151e8();
void Func_08015200();
void Func_08015208();
void Func_08015288();
void Func_08015290();
void Func_08015370();
s32 Func_080022ec();
void *Func_08077008();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_080772f0();
void Func_0808ace0();
void Func_0808b1d8();
void Func_0808b674();
void Func_0808ba38();
void Func_0808bb2c();
void Func_0808bc44();
s32 Func_0808bc9c();
void Func_0808bec0();
void Func_0808c44c();
void Func_0808c4c0();
s32 Func_0808ce74();
void Func_0808cf78();
void Func_0808d0c8();
s32 Func_0808d5a4();
void Func_0808d5dc();
void Func_0808d7d8();
void Func_0808d828();
void Func_0808d8f0();
void Func_0808d9a4();
s32 Func_0808ddec();
void Func_0808e118();
void Func_0808e23c();
void Func_0808e680();
void Func_0808e9c0();
void Func_0808fefc();
void Func_080901c0();
void Func_08091174();
void Func_08091200();
void Func_08091660();
s32 Func_080919d8();
void Func_08091a58();
void Func_080941e0();
s32 Func_08094428();
void Func_08095680();
void Func_08095778();
void Func_08099810();
void Func_0809c138();
void Func_0809c3a4();
void Func_080f9010();
s32 Func_080f9070();

extern u8 Data_02000240[];
extern u16 Data_02000402;
extern u16 Data_0200042c;
extern s32 Data_02000434;
extern s32 Data_02000478;
extern u8 Data_02008000[];
extern u8 Data_03001810[];
extern volatile u32 Data_03001ae8;
extern void *Data_03001e68;
extern volatile u8 Data_03001f54;

s32 Func_0808c4f8(void)
{
    s32 zero;
    volatile u32 *dma;
    void *work;
    void *actor;
    void *obj;
    u8 *pos;
    u8 *g;
    u8 *ov;
    s32 mode;
    s32 result;
    s32 sel;
    s32 v;
    s32 ok;
    s32 cnt;
    s32 n;
    u32 i;

    g = Data_02000240;
    ov = Data_02008000;
    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    result = 0;

    zero = 0;
    dma = (volatile u32 *)0x040000D4;
    dma[0] = (u32)&zero;
    dma[1] = (u32)work;
    dma[2] = 0x85000333;

    GameFlag_Clear(0x103);

    FIELD_AT_OFFSET(g, u16 *, 0x1C8) =
        FIELD_AT_OFFSET(g, u16 *, 0x1C0);
    FIELD_AT_OFFSET(g, u16 *, 0x1CA) =
        FIELD_AT_OFFSET(g, u16 *, 0x1C2);
    FIELD_AT_OFFSET(g, u16 *, 0x1CE) = 0xFFFF;
    FIELD_AT_OFFSET(g, s16 *, 0x1D0) = -1;
    FIELD_AT_OFFSET(g, s16 *, 0x1D2) = -1;
    FIELD_AT_OFFSET(g, s16 *, 0x1D4) = -1;

    Scheduler_ResetTaskTable();
    Func_08095778(0);

    if (FIELD_AT_OFFSET(g, s16 *, 0x1DA) == 1) {
        Func_08009118();
        FIELD_AT_OFFSET(g, u8 *, 0x224) = 1;
        mode = 3;
        Func_0808ace0(0);
    } else {
        Func_08009110();
        mode = 2;
        Func_0808ace0(1);
    }
    FIELD_AT_OFFSET(work, s16 *, 0x19E) = mode;
    Func_08009078(mode);
    Func_08015000();
    BattleFx_ResetCounters();
    FIELD_AT_OFFSET(work, s32 *, 0x10) =
        Func_080072e4(FIELD_AT_OFFSET(ov, s32 *, 36));
    Func_0808cf78();
    Func_080072e4(FIELD_AT_OFFSET(ov, s32 *, 28));
    Func_0808b674();
    if (GameFlag_IsSet(0x109) != 0) {
        Func_0808bb2c();
    }
    if (FIELD_AT_OFFSET(g, s16 *, 0x234) != 0) {
        Func_08095680();
    }
    if (mode == 3) {
        Func_08009130();
        Func_08009138(FIELD_AT_OFFSET(g, s32 *, 0x1DC),
                      FIELD_AT_OFFSET(g, s32 *, 0x1E4));
    } else {
        Func_08009128();
    }
    Func_0808e9c0();
    BattleFx_InitializeBuffers();
    BattleFx_ApplyColorToTargetBuffer(0x10000, 0);

    FIELD_AT_OFFSET(work, s32 *, 0x1C0) = 256;
    FIELD_AT_OFFSET(work, s32 *, 0x1C8) = 16;
    FIELD_AT_OFFSET(work, u16 *, 0x1C6) = 0;
    FIELD_AT_OFFSET(work, s32 *, 0x1B0) = 0x199;
    FIELD_AT_OFFSET(work, s32 *, 0x1B4) = 0;
    if (FIELD_AT_OFFSET(g, s16 *, 0x23E) == 2) {
        FIELD_AT_OFFSET(work, s32 *, 0x1B0) = 0;
        GameFlag_Set(0x144);
    }
    FIELD_AT_OFFSET(work, s32 *, 0x1BC) = 0x02010000;
    if (GameFlag_IsSet(0x109) == 0) {
        Party_ResolveTablePair();
        FIELD_AT_OFFSET(g, u16 *, 0x22C) = 16;
        FIELD_AT_OFFSET(g, u16 *, 0x22E) = 0;
        FIELD_AT_OFFSET(g, u16 *, 0x230) = 1;
        FIELD_AT_OFFSET(g, u16 *, 0x24A) = 0xFFFF;
        FIELD_AT_OFFSET(g, u16 *, 0x24C) = 0;
    }
    BattleFx_ScheduleCallbackWhenValue24cSet();
    FIELD_AT_OFFSET(work, u16 *, 0xCC8) = 0xFFFF;
    Func_080072e4(FIELD_AT_OFFSET(ov, s32 *, 4));

    result = FIELD_AT_OFFSET(work, s16 *, 0x170);
    if (result != 0) {
        FIELD_AT_OFFSET(work, u16 *, 0x170) = 0;
        goto done;
    }

    GameFlag_Clear(0x109);
    if (BattleFx_SumCounters() == 0) {
        if (FIELD_AT_OFFSET(work, u16 *, 0x1C6) == 0) {
            Func_0808fefc(FIELD_AT_OFFSET(work, s32 *, 0x1C0),
                          FIELD_AT_OFFSET(work, s32 *, 0x1C8));
            FIELD_AT_OFFSET(work, u16 *, 0x1C6) = 1;
            *(u16 *)0x05000000 = 0;
            if (ObjectEffect_RunPendingFlagEvent() == 0) {
                WaitFrames((FIELD_AT_OFFSET(work, s32 *, 0x1C8) + 1) / 2);
            }
        }
        if (GameFlag_IsSet(0x12F) != 0) {
            GameFlag_Clear(0x12F);
            Func_08015200(FIELD_AT_OFFSET(g, s16 *, 0x1C0));
        }
        if (FIELD_AT_OFFSET(g, s16 *, 0x234) != 0) {
            Func_08095778(FIELD_AT_OFFSET(g, s16 *, 0x234), 1);
            FIELD_AT_OFFSET(g, u16 *, 0x234) = 0;
        }
        if (FIELD_AT_OFFSET(g, u16 *, 0x23C) != 0) {
            Battle_SetObjectFlag5bWhenMode3();
            if (Party_CheckMemberValueTotal(
                    FIELD_AT_OFFSET(g, u16 *, 0x23C)) == 0) {
                PartyInventory_GiveItem(
                    FIELD_AT_OFFSET(g, u16 *, 0x23C), 0);
            }
            Battle_ClearObjectFlag5bWhenMode3();
            FIELD_AT_OFFSET(g, u16 *, 0x23C) = 0;
        }
    }

    for (;;) {
        GameFlag_Set(0x104);
        actor = FIELD_AT_OFFSET(work, void **, 20 + Data_02000434 * 4);
        Object_ResetMotion(actor);
        FIELD_AT_OFFSET(g, s32 *, 0x1DC) =
            FIELD_AT_OFFSET(actor, s32 *, 8);
        FIELD_AT_OFFSET(g, s32 *, 0x1E0) = 0;
        FIELD_AT_OFFSET(g, s32 *, 0x1E4) =
            FIELD_AT_OFFSET(actor, s32 *, 16);
        FIELD_AT_OFFSET(g, s32 *, 0x1E8) =
            FIELD_AT_OFFSET(actor, u16 *, 6);
        Data_0200042c = FIELD_AT_OFFSET(actor, u8 *, 34);

        while (BattleFx_SumCounters() != 0) {
            result = FIELD_AT_OFFSET(work, s16 *, 0x170);
            if (result != 0) {
                FIELD_AT_OFFSET(work, u16 *, 0x170) = 0;
                goto done;
            }
            if (FIELD_AT_OFFSET(work, s16 *, 0x182) != 0) {
                FIELD_AT_OFFSET(work, u16 *, 0xCB6) = 1;
                sel = FIELD_AT_OFFSET(work, s16 *, 0x182);
                if (sel == -1) {
                    Battle_InitializeRenderObject();
                    Battle_SetObjectFlag5bWhenMode3();
                    for (i = 0; i < FIELD_AT_OFFSET(work, s16 *, 0x184); i++) {
                        Object_SetMode(actor, 22);
                        UiText_DrawQuantity(
                            FIELD_AT_OFFSET(work, s16 *, 0x188 + i * 2), 1);
                        UiText_DrawMessage(0x91A, 1);
                    }
                    if (FIELD_AT_OFFSET(work, s16 *, 0x186) == 0) {
                        if (Data_02000434 != 0) {
                            Object_SetMode(actor, 19);
                        } else if (GameFlag_IsSet(32) != 0) {
                            Object_SetMode(actor, 21);
                        } else {
                            Object_SetMode(actor, 37);
                        }
                        Audio_PlayCue(59);
                        UiText_DrawMessage(0x91B, 1);
                        obj = Runtime_GetObject(Data_02000434);
                        FIELD_AT_OFFSET(obj, u16 *, 56) = 1;
                        v = FixedPoint_Ratio(
                            FIELD_AT_OFFSET(obj, s16 *, 56) << 14,
                            FIELD_AT_OFFSET(obj, s16 *, 52));
                        if (v > 0x4000) {
                            v = 0x4000;
                        } else if (v < 0) {
                            v = 0;
                        }
                        FIELD_AT_OFFSET(obj, s16 *, 20) = v;
                        if (FIELD_AT_OFFSET(obj, s16 *, 20) == 0
                            && FIELD_AT_OFFSET(obj, s16 *, 56) != 0) {
                            FIELD_AT_OFFSET(obj, s16 *, 20) = 1;
                        }
                        v = FixedPoint_Ratio(
                            FIELD_AT_OFFSET(obj, s16 *, 58) << 14,
                            FIELD_AT_OFFSET(obj, s16 *, 54));
                        if (v > 0x4000) {
                            v = 0x4000;
                        } else if (v < 0) {
                            v = 0;
                        }
                        FIELD_AT_OFFSET(obj, s16 *, 22) = v;
                        if (FIELD_AT_OFFSET(obj, s16 *, 22) == 0
                            && FIELD_AT_OFFSET(obj, s16 *, 58) != 0) {
                            FIELD_AT_OFFSET(obj, s16 *, 22) = 1;
                        }
                        FIELD_AT_OFFSET(g, u16 *, 0x1C0) =
                            FIELD_AT_OFFSET(g, u16 *, 0x1C4);
                        Data_02000402 =
                            FIELD_AT_OFFSET(g, u16 *, 0x1C6);
                        Battle_ClearObjectFlag5bWhenMode3();
                        result = 999;
                        goto done;
                    }
                    Battle_ClearObjectFlag5bWhenMode3();
                } else if (sel == -888) {
                    Battle_InitializeRenderObject();
                    Func_0809c138(0x1B);
                } else if (sel == -889) {
                    Battle_InitializeRenderObject();
                    BattleFx_RunVisibilityTransition();
                } else {
                    BattleFx_RunKind6DescriptorAction();
                }
                FIELD_AT_OFFSET(work, u16 *, 0xCB6) = 0;
                FIELD_AT_OFFSET(work, u16 *, 0x182) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x17C) != 0) {
                Battle_InitializeRenderObject();
                Func_0808ba38();
                FIELD_AT_OFFSET(g, u16 *, 0x21E) = 0xFFFF;
                FIELD_AT_OFFSET(g, u16 *, 0x1C0) = 510;
                Data_02000402 = FIELD_AT_OFFSET(work, u16 *, 0x17C);
                FIELD_AT_OFFSET(work, u16 *, 0x170) = 999;
                Func_080941e0(FIELD_AT_OFFSET(work, s16 *, 0x17C));
                FIELD_AT_OFFSET(work, s32 *, 0x1A8) = 0;
                FIELD_AT_OFFSET(work, u16 *, 0x17C) = 0;
                Data_02000478 = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x16C) != 0) {
                FIELD_AT_OFFSET(work, u16 *, 0xCB6) = 1;
                BattleAction_RunDescriptor(FIELD_AT_OFFSET(work, s16 *, 0x16C));
                FIELD_AT_OFFSET(work, u16 *, 0xCB6) = 0;
                FIELD_AT_OFFSET(work, u16 *, 0x16C) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x16E) != 0) {
                Battle_DispatchInputEvent(FIELD_AT_OFFSET(work, s16 *, 0x16E));
                FIELD_AT_OFFSET(work, u16 *, 0x16E) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x174) != 0) {
                Func_08015208();
                v = Func_0808ddec(Data_02000434);
                ok = 0;
                if (v != -1) {
                    ok = BattleFx_FindDescriptorWithOverride() != 0;
                }
                if (ok != 0) {
                    FIELD_AT_OFFSET(work, u16 *, 0x178) = v | 0x1000;
                    FIELD_AT_OFFSET(work, u16 *, 0x172) = 0;
                } else {
                    v = Func_0808ce74();
                    if (v != 0) {
                        FIELD_AT_OFFSET(work, u16 *, 0x17A) = v;
                        FIELD_AT_OFFSET(work, u16 *, 0x172) = 0;
                    } else {
                        FIELD_AT_OFFSET(work, u16 *, 0x172) = 1;
                    }
                }
                FIELD_AT_OFFSET(work, u16 *, 0x174) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x172) != 0) {
                Func_08015208();
                Battle_InitializeRenderObject();
                Audio_PlayCue(111);
                Battle_SetObjectFlag5bWhenMode3();
                GameFlag_Set(0x106);
                if (Data_03001f54 != 0 && (Data_03001ae8 & 2) != 0
                    && (Data_03001ae8 & 4) != 0) {
                    Func_08015290();
                } else if (GameFlag_IsSet(0x107) != 0) {
                    FIELD_AT_OFFSET(work, u16 *, 0x182) = 250;
                } else {
                    FIELD_AT_OFFSET(work, u16 *, 0xCC0) = 0;
                    Func_080151e8();
                    FIELD_AT_OFFSET(work, u16 *, 0xCC0) = 1;
                }
                Battle_ClearObjectFlag5bWhenMode3();
                GameFlag_Clear(0x106);
                Func_080772f0();
                FIELD_AT_OFFSET(work, u16 *, 0x172) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x178) != 0) {
                Battle_SetObjectFlag5bWhenMode3();
                BattleFx_RunDescriptorAction(
                    FIELD_AT_OFFSET(work, u16 *, 0x178) & 0xFFF);
                Battle_ClearObjectFlag5bWhenMode3();
                FIELD_AT_OFFSET(work, u16 *, 0x178) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x17A) != 0) {
                Battle_SetObjectFlag5bWhenMode3();
                Func_0808d9a4(FIELD_AT_OFFSET(work, s16 *, 0x17A));
                Battle_ClearObjectFlag5bWhenMode3();
                FIELD_AT_OFFSET(work, u16 *, 0x17A) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x17E) != 0) {
                Func_08015208();
                Battle_SetObjectFlag5bWhenMode3();
                BattleCommand_ExecuteSelectedAction(
                    FIELD_AT_OFFSET(work, s16 *, 0x17E));
                Battle_ClearObjectFlag5bWhenMode3();
                FIELD_AT_OFFSET(work, u16 *, 0x17E) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x180) != 0) {
                Battle_SetObjectFlag5bWhenMode3();
                BattleCommand_ExecuteSelectedItem(
                    FIELD_AT_OFFSET(work, s16 *, 0x180),
                    FIELD_AT_OFFSET(work, s16 *, 0x19A));
                Battle_ClearObjectFlag5bWhenMode3();
                FIELD_AT_OFFSET(work, u16 *, 0x180) = 0;
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x176) != 0) {
                Audio_PlayCue(111);
                Battle_InitializeRenderObject();
                Battle_SetObjectFlag5bWhenMode3();
                GameFlag_Set(0x106);
                if (Data_03001f54 != 0 && (Data_03001ae8 & 2) != 0) {
                    Func_08015288();
                } else if (Data_03001f54 != 0 && (Data_03001ae8 & 0x200) != 0) {
                    Func_0808d0c8();
                } else if (GameFlag_IsSet(0x107) != 0) {
                    FIELD_AT_OFFSET(work, u16 *, 0x182) = 250;
                } else {
                    Battle_ResetEffectCounter();
                    Func_08015208();
                    FIELD_AT_OFFSET(g, u16 *, 0x21E) = Func_080f9070();
                    if (GameFlag_IsSet(0x17E) == 0) {
                        obj = Data_03001e68;
                        pos = Data_03001810;
                        cnt = 0;
                        n = 512;
                        do {
                            if (*pos == 0xFF) {
                                cnt++;
                            }
                            pos++;
                            n--;
                        } while (n != 0);
                        if (cnt - 136 < 0) {
                            FIELD_AT_OFFSET(obj, u16 *, 4) = 1;
                            WaitFrames(1);
                        }
                        Func_08015370(0);
                        FIELD_AT_OFFSET(obj, u16 *, 4) = 0;
                    } else {
                        UiText_DrawMessage(0xC2F, 1);
                    }
                }
                Battle_ClearObjectFlag5bWhenMode3();
                GameFlag_Clear(0x106);
                FIELD_AT_OFFSET(work, u16 *, 0x176) = 0;
            }
        }

        GameFlag_Clear(0x104);
        actor = FIELD_AT_OFFSET(work, void **, 20 + Data_02000434 * 4);
        if (actor != NULL) {
            v = FIELD_AT_OFFSET(g, u8 *, 0x1F2);
            if (v == 2) {
                Func_080090b8(actor);
            } else if (v == 1) {
                Func_080090b0(actor);
            } else if (FIELD_AT_OFFSET(work, s16 *, 0x19E) == 3) {
                Func_080090a8(actor);
            } else {
                Func_080090a0(actor);
            }
        }
        do {
            WaitFrames(1);
            actor = FIELD_AT_OFFSET(work, void **, 20 + Data_02000434 * 4);
            if (Data_03001f54 == 0 || GameFlag_IsSet(0x163) == 0) {
                Func_0808bec0(FIELD_AT_OFFSET(actor, u8 *, 34),
                              FIELD_AT_OFFSET(actor, s32 *, 8),
                              FIELD_AT_OFFSET(actor, s32 *, 12),
                              FIELD_AT_OFFSET(actor, s32 *, 16));
            }
        } while (BattleFx_SumCounters() == 0);
    }

done:
    if (FIELD_AT_OFFSET(work, u16 *, 0x1C6) != 0) {
        Func_080901c0(FIELD_AT_OFFSET(work, s32 *, 0x1C0),
                      FIELD_AT_OFFSET(work, s32 *, 0x1C8));
        FIELD_AT_OFFSET(work, u16 *, 0x1C6) = 0;
        WaitFrames(FIELD_AT_OFFSET(work, s32 *, 0x1C8));
    }
    Runtime_ReleaseHeapBlock(0x1B);
    return result;
}
