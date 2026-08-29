#include "types.h"
#include "battle_effect_runtime.h"

/*
 * Split function reconstructed as one whole owner per CONTRIBUTING.md's
 * "Split functions" section. Real entry/prologue at 0x080be378, real
 * epilogue at the tail of games/gs1/asm/080bef88.s (0x080bf1e8), spanning
 * 3696 bytes across games/gs1/asm/{080be378,080be76c,080beb08,080bef88}.s.
 * See games/gs1/recon/en/main/080be378.json for the full map.
 *
 * All `bl sub_080bec5c` / `bl sub_080bec8a` / `bl sub_080bee00` / `bl
 * sub_080bf1d6` / `bl sub_080bf1d4` cross-region transfers in the retained
 * assembly are modeled here as plain `goto` to labels inside this same
 * function: per CONTRIBUTING.md's "Split functions" note, "the inter-region
 * veneers are the compiler's own long-branch mechanism and fall out of
 * compiling the function whole" -- Thumb's short branch range is exceeded by
 * a 3.7KB function, so the assembler encodes some of this function's own
 * internal jumps as long-branch `bl` sequences that never actually return
 * (confirmed directly: e.g. the tier==99 case body's final instruction is
 * `bl sub_080bf1d4` immediately followed by the file's literal pool -- there
 * is no valid return site, so it cannot be a real call).
 *
 * Field layout for the request/target/actor pointers is not evidenced
 * beyond the byte offsets actually dereferenced, so all three are kept as
 * raw byte pointers with `field_<offset>` style access rather than a guessed
 * struct, per Cleanroom's "use names no more specific than the evidence"
 * rule. Several `ldr rN, [pc, #imm]` literal-pool loads (status-flag byte
 * offsets on the actor, and UI text-resource pointers) have not been
 * resolved to concrete values in this pass; they are represented by named
 * placeholders below and flagged as pending halfword-stage work -- this
 * draft targets complete *structure* first, per the task's own priority.
 */

extern void *Func_08077008(s16 id);              /* Runtime_GetObject */
extern void *Func_08077080(s16 id);              /* Ability_GetData */
extern s32 Func_080b9a44(s16 arg0);
extern void Func_080bdfec(void);                 /* BattleEventRuntime_Reset */
extern void Func_08015118(void);
extern void Func_08015120(s16 value, s16 mode);  /* UiText_DrawQuantity */
extern void Func_080151c8(void *text);           /* UiText_ShowMessageAndWait */
extern s32 Func_080771a0(void);
extern s32 Func_080771b0(s16 id, s8 a, u8 b);
extern s32 Func_080771c0(s16 id, s8 a, u8 b);
extern s32 Func_080771c8(s16 id, s8 a, u8 b);
extern s32 Func_080771e0(s16 id);
extern s32 Func_080771e8(s8 a, u8 b);
extern s32 Func_08077208(s16 id, s8 a, u8 b);
extern s32 Func_08077210(s16 id, s8 a, u8 b);
extern void *Func_080b7dd0(s16 id);              /* GetBattleObjectSlot */
extern void Func_08009080(void *obj, s32 mode);  /* Object_SetMode */
extern void Func_08009088(void *obj, s32 action);/* Object_SetAction */
extern void Func_080f9010(s32 cue);              /* Audio_PlayCue */
extern void Func_080bbabc(s32 kind, s32 value);
extern void Func_080bd808(s32 phase);            /* BattleEventRuntime_SchedulePhase */
extern void Func_080be02c(void);                 /* BattleEventRuntime_WaitForReady */
extern void Func_080c1798(s16 id, s8 a, s32 mode, s32 arg3);
extern void Func_08077010(s16 id);               /* BattleUnit_Recalculate */
extern void Func_080c10e8(s32 a, s32 b);
extern void Func_080030f8(s32 frames);           /* WaitFrames */
extern s32 Func_080be18c(s16 abilityId);
extern void *Func_08077018(u16 itemId);          /* Item_GetData */
extern s32 Func_08077128(s16 id);
extern void Func_08077160(s16 id);
extern void Func_08077078(void *actor, s32 flag);
extern void Func_080bb8d8(void);
extern s32 Func_080770c0(s32 flagId);            /* GameFlag_IsSet */
extern s32 Func_08077170(s16 id);
extern s32 Func_080c23e8(u8 value);
extern s32 Func_080771a0_rng(void);
extern void Func_080bb65c(void);
extern void Func_080bf1d4(void); /* unreachable; long-branch veneer target only, never a real call site */
extern void Func_080022ec(s32 a, s32 b);         /* FixedPoint_Ratio */
extern void Func_080772f8(s16 id);
extern s32 Func_080772b8(u8 value);
extern s32 Func_080bd3c8(s16 id);
extern s32 Func_08077178(s16 id, u8 a, u8 b, u8 c, s32 mode);

/* Pending halfword-stage: exact literal-pool constants not yet decoded. */
#define SUBKIND_MASK      0x0f  /* TBD: mask applied to req->8 high byte */
#define TEXT_TBD          NULL  /* TBD: literal-pool UI text pointers */
#define ACTOR_FLAG_A_OFF  0     /* TBD: LiteralPool_080be738-relative offset */
#define ACTOR_FLAG_B_OFF  0     /* TBD */
#define ACTOR_FLAG_C_OFF  0     /* TBD */
#define REQ_FLAG_OFF      0     /* TBD: req+K byte used in case2/case888 */
#define K1_ABILITY_ID     0     /* TBD: upper-range compare constant */
#define K2_ABILITY_ID     0     /* TBD */
#define K3_ABILITY_ID     0     /* TBD */
#define K4_ABILITY_ID     0     /* TBD */

void Func_080be378(u8 *req, u8 *tgt)
{
    void *actor;
    s32 targetPowerBase;
    s16 tier;
    s16 abilityId;      /* r11 in most case bodies */
    void *abilityData;  /* r5/r6/r7 role: Ability_GetData()/Item_GetData() result */
    void *targetUnit;   /* r6 in Region_080beb08's L_080bee08 block: Func_08077008(tgt[2]) -- the ability's recipient unit, distinct from `actor` */
    void *textPtr;
    s16 packed;
    s8 subKind;
    u8 lowByte;
    s32 t0, t1, t2, t3;
    s32 lookupResult;

    actor = Func_08077008(*(s16 *)(req + 0));
    targetPowerBase = Func_080b9a44(*(s16 *)(req + 10));
    Func_080bdfec();

    tgt[0] = (u8)*(u16 *)(req + 0);
    *(u32 *)(tgt + 96) = 0;
    tgt[1] = 0;
    *(u32 *)(tgt + 88) = 0;
    *(u32 *)(tgt + 92) = 0;
    *(u32 *)(tgt + 80) = 4;

    Func_08015118();

    if (*(s16 *)((u8 *)actor + 56) == 0) {
        goto L_080bec5c;
    }

    Func_08015118();

    if (((u8 *)actor)[ACTOR_FLAG_A_OFF]) {
        ((u8 *)actor)[ACTOR_FLAG_A_OFF] = 0;
        Func_08015120(*(s16 *)(req + 0), 1);
        Func_080151c8(TEXT_TBD);
        goto L_080bec8a;
    }
    if (((u8 *)actor)[ACTOR_FLAG_B_OFF]) {
        Func_08015120(*(s16 *)(req + 0), 1);
        Func_080151c8(TEXT_TBD);
        goto L_080bec8a;
    }
    if (((u8 *)actor)[ACTOR_FLAG_C_OFF]) {
        Func_08015120(*(s16 *)(req + 0), 1);
        Func_080151c8(TEXT_TBD);
        goto L_080bec8a;
    }
    if (((u8 *)actor)[REQ_FLAG_OFF] & 1) {
        if (*(s16 *)((u8 *)actor + 6) != 3) {
            if ((Func_080771a0() & 3) == 0) {
                Func_08015120(*(s16 *)(req + 0), 1);
                Func_080151c8(TEXT_TBD);
                goto L_080bec8a;
            }
        }
    }

    tier = *(s16 *)((u8 *)actor + 6);
    if (tier == 8) {
        goto L_080bec5c;
    }

    {
        s32 i;
        for (i = 0; i < 13; i++) {
            *((u8 *)actor + 44 + i) = 0;
        }
        for (i = 0; i < 13; i++) {
            *((u8 *)actor + 58 + i) = (u8)-1;
        }
    }

    /*
     * Dense 100-way switch matching JumpTable_080be570 exactly: GCC 2.96
     * only emits a `mov pc, rN` tablejump (matching the reference's
     * `cmp #99; bls; bl <default>; ldr r2,[pc]; lsls r3,r3,#2; ldr
     * r3,[r3,r2]; mov pc,r3`) for a switch this dense -- a 9-arm switch
     * over the same value range compiles to a compare-chain instead, which
     * is what the previous draft's sparse `switch(tier){case 0: case 1:
     * ...}` produced. Every case below is spelled out individually per
     * JumpTable_080be570's 100 literal entries in
     * games/gs1/asm/080be378.s (9 named targets, a 90-entry `.rept` block
     * that all point at 0x080bee00, and a final distinct entry for index
     * 99). `default` covers tier < 0 or tier > 99, which is not reachable
     * for any real ability-tier value but is exactly what the reference's
     * `bl sub_080bee00` fallback (executed for the same out-of-range
     * condition, then falling into the same table read) does.
     */
    switch (tier) {
    case 0:
        goto L_080be76c;
    case 1:
        goto L_080be7d0;
    case 2:
        goto L_080be888;
    case 3:
        goto L_080be96e;
    case 4:
        goto L_080be984;
    case 5:
        goto L_080beb08;
    case 6:
        goto L_080becea;
    case 7:
        goto L_080be96e;
    case 8:
        goto L_080bec5c;
    case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16:
    case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24:
    case 25: case 26: case 27: case 28: case 29: case 30: case 31: case 32:
    case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 40:
    case 41: case 42: case 43: case 44: case 45: case 46: case 47: case 48:
    case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56:
    case 57: case 58: case 59: case 60: case 61: case 62: case 63: case 64:
    case 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72:
    case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80:
    case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88:
    case 89: case 90: case 91: case 92: case 93: case 94: case 95: case 96:
    case 97: case 98:
        goto L_080bee00;
    case 99:
        goto L_080be700;
    default:
        goto L_080bee00;
    }

    /*
     * ---- case tier==99, address 0x080be700 (inside 080be378.s itself,
     * right after JumpTable_080be570 -- the table's own final entry) ----
     */
L_080be700:
    {
        u16 reqId = *(u16 *)(req + 0);
        if (reqId > 7) {
            Func_080151c8(TEXT_TBD);
        } else {
            Func_08015120((s16)reqId, 1);
            Func_080151c8(TEXT_TBD);
        }
        Func_080bb65c();
        *(s32 *)(tgt + 84) = 7;
        goto L_080bf1d6_shared;
    }

    /* ---- Region_080be76c: case tier==0 ---- */
L_080be76c:
    Func_08077160(*(s16 *)(req + 0));
    abilityId = *(s16 *)(req + 0);
    lookupResult = Func_080be18c(abilityId);
    if (lookupResult == -1) {
        goto L_080bf1d6_shared;
    }
    if (abilityId == 1) {
        goto L_080bee08;
    }
    Func_08077078(actor, 1);
    Func_08015120(*(s16 *)(req + 0), 2);
    textPtr = TEXT_TBD;
    Func_080151c8(textPtr);
    Func_080bb8d8();
    Func_08015120(abilityId, 4);
    textPtr = TEXT_TBD;
    goto L_080be7ca;

    /* ---- case tier==1, address 0x080be7d0 ---- */
L_080be7d0:
    abilityId = *(s16 *)(req + 8);
    abilityData = Func_08077080(abilityId);
    lookupResult = Func_080be18c(abilityId);
    if (lookupResult == -1) {
        goto L_080bf1d6_shared;
    }
    Func_08015120(*(s16 *)(req + 0), 1);
    Func_08015120(abilityId, 4);
    Func_080151c8(TEXT_TBD);
    {
        s32 costOk = 1;
        if (*(s16 *)((u8 *)actor + 58) < *(u8 *)((u8 *)abilityData + 9)) {
            *(s32 *)(tgt + 92) = 2;
            costOk = 0;
        }
        if (((u8 *)actor)[ACTOR_FLAG_A_OFF]) {
            *(s32 *)(tgt + 92) = 1;
            costOk = 0;
        }
        if (!costOk) {
            goto L_080bee00;
        }
    }
    *(s32 *)(tgt + 92) = 0;
    *(u16 *)((u8 *)actor + 58) =
        (u16)(*(u16 *)((u8 *)actor + 58) - *(u8 *)((u8 *)abilityData + 9));
    Func_08077128(*(s16 *)(req + 0));
    if (*(s16 *)((u8 *)actor + 58) < 0) {
        *(u16 *)((u8 *)actor + 58) = 0;
    }
    if (*(s16 *)((u8 *)actor + 58) > *(s16 *)((u8 *)actor + 54)) {
        goto L_080bee00;
    }
    *(u16 *)((u8 *)actor + 58) = *(u16 *)((u8 *)actor + 54);
    goto L_080bee00;

    /* ---- case tier==2, address 0x080be888 ---- */
L_080be888:
    {
        s16 slotIdx = *(s16 *)(req + 8);
        if (slotIdx < 0) {
            Func_08015120(*(s16 *)(req + 0), 1);
            Func_080151c8(TEXT_TBD);
            goto L_080bec8a;
        }
        {
            u16 itemId = *(u16 *)(req + 216 + slotIdx * 2);
            abilityData = Func_08077018(itemId);
        }
        abilityId = (s16)*(u16 *)((u8 *)abilityData + 40);
        if (abilityId != 0) {
            u16 flagsField = *(u16 *)(req + 216 + (*(s16 *)(req + 8)) * 2);
            if (flagsField & (0x400)) {
                goto L_080be908;
            }
        } else {
            goto L_080be8dc;
        }
        goto L_080be8e0;
L_080be8dc:
        req = req; /* r1 = req (already held) */
L_080be8e0:
        Func_08015120(*(s16 *)(req + 0), 1);
        Func_080151c8(TEXT_TBD);
        if (((u8 *)req)[REQ_FLAG_OFF] == 0) {
            ((u8 *)req)[REQ_FLAG_OFF] = 1;
        }
        goto L_080bec8a;
L_080be908:
        lookupResult = Func_080be18c(abilityId);
        if (lookupResult == -1) {
            goto L_080bf1d6_shared;
        }
        Func_08015120(*(s16 *)(req + 0), 1);
        Func_08015120(*(u16 *)(req + 216 + (*(s16 *)(req + 8)) * 2), 2);
        {
            u8 kind12 = *((u8 *)abilityData + 12);
            u8 kind2 = *((u8 *)abilityData + 2);
            if (kind12 == 2 || kind12 == 0) {
                if (kind2 == 3 || kind2 == 1 || (kind2 >= 6 && kind2 <= 8)) {
                    textPtr = TEXT_TBD;
                    goto L_080be7ca;
                }
            }
            textPtr = TEXT_TBD;
            goto L_080be7ca;
        }
    }

    /* ---- case tier==3 and tier==7 (shared), address 0x080be96e ---- */
L_080be96e:
    Func_08015120(*(s16 *)(req + 0), 1);
    Func_080151c8(TEXT_TBD);
    goto L_080bec8a;

    /* ---- case tier==4, address 0x080be984 ---- */
L_080be984:
    abilityId = *(s16 *)(req + 8);
    lookupResult = Func_080be18c(abilityId);
    if (lookupResult == -1) {
        goto L_080bf1d6_shared;
    }
    Func_08015120(*(s16 *)(req + 0), 1);
    Func_08015120(abilityId, 4);
    abilityData = Func_08077080(abilityId);
    if ((*((u8 *)abilityData + 1) & 0x0f) == 6) {
        textPtr = TEXT_TBD;
    } else {
        textPtr = TEXT_TBD;
    }
    /*
     * ~15-way exact/range classification of `abilityId` selecting one of
     * many distinct UI text pointers, all converging on L_080be7ca. The
     * exact-match constants below are the ones directly computable from the
     * retained assembly's immediate operands; the four that come from
     * unread `ldr rN, [pc, #imm]` literal-pool words are left as the
     * K1..K4 placeholders (all currently 0, i.e. inert) pending halfword-
     * stage literal-pool decoding.
     */
    switch (abilityId) {
    case 488: textPtr = TEXT_TBD; goto L_080be7ca;  /* 244*2 */
    case 224: textPtr = TEXT_TBD; goto L_080be7ca;
    case 500: textPtr = TEXT_TBD; goto L_080be7ca;  /* 250*2 */
    case 494: textPtr = TEXT_TBD; goto L_080be7ca;  /* 500-6 */
    case 492: textPtr = TEXT_TBD; goto L_080be7ca;  /* 246*2 */
    case 504: textPtr = TEXT_TBD; goto L_080be7ca;  /* 252*2 */
    case 508: textPtr = TEXT_TBD; goto L_080be7ca;  /* 254*2 */
    case 472: textPtr = TEXT_TBD; goto L_080be7ca;  /* 236*2 */
    default:
        /*
         * TODO: three more exact-match cases (K2_ABILITY_ID, K3_ABILITY_ID,
         * K4_ABILITY_ID) belong here once their literal-pool values are
         * decoded; folded into default for now since they are all
         * currently the same placeholder value and would collide as
         * duplicate case labels.
         */
        if (abilityId > K1_ABILITY_ID) {
            if (abilityId <= 444) { /* 222*2 */
                textPtr = TEXT_TBD;
            }
            goto L_080be7ca;
        }
        if (abilityId > 436) { /* 488-52 */
            goto L_080be7ca;
        }
        if (abilityId > 434) { /* 217*2 */
            textPtr = TEXT_TBD;
        }
        goto L_080be7ca;
    }

    /* ---- shared exit: address 0x080be7ca, reached from cases 0/2/4 ---- */
L_080be7ca:
    Func_080151c8(textPtr);
    goto L_080bee00;

    /* ---- Region_080beb08: case tier==5, address 0x080beb08 ---- */
L_080beb08:
    packed = *(u16 *)(req + 8);
    subKind = (s8)((packed << 8) >> 8) >> 0; /* placeholder decode, see SUBKIND_MASK note */
    subKind = (s8)(packed >> 8) & SUBKIND_MASK;
    lowByte = (u8)packed;
    abilityId = Func_080771e8(subKind, lowByte);
    if (Func_08077208(*(s16 *)(req + 0), (s8)(*(u16 *)(req + 8) >> 8) & SUBKIND_MASK,
                       (u8)*(u16 *)(req + 8)) == 0) {
        goto L_080beb48;
    }
    goto L_080bec90;

L_080beb48:
    if (Func_08077210(*(s16 *)(req + 0), (s8)(*(u16 *)(req + 8) >> 8) & SUBKIND_MASK,
                       (u8)*(u16 *)(req + 8)) == 0) {
        goto L_080bec62;
    }

    Func_08077080(abilityId);
    Func_080c10e8(0, 0);
    Func_080771b0(*(s16 *)(req + 0), (s8)(*(u16 *)(req + 8) >> 8) & SUBKIND_MASK,
                  (u8)*(u16 *)(req + 8));
    Func_080771c0(*(s16 *)(req + 0), (s8)(*(u16 *)(req + 8) >> 8) & SUBKIND_MASK,
                  (u8)*(u16 *)(req + 8));
    Func_08077010(*(s16 *)(req + 0));
    Func_080bdfec();
    Func_080bd808(30);
    Func_080bbabc(0, *(s16 *)(req + 0));
    Func_080bbabc(3, 0 /* packed sub-kind table index, TBD */);
    Func_080bbabc(14, 175);
    Func_080bbabc(10, 0);
    Func_080bbabc(4, (s32)TEXT_TBD);
    Func_080bbabc(11, *(s16 *)(req + 0));
    Func_080f9010(212);
    Func_08009080(Func_080b7dd0(*(s16 *)(req + 0)), 3);
    Func_08009088(Func_080b7dd0(*(s16 *)(req + 0)), 32);
    Func_080c1798(*(s16 *)(req + 0), (s8)(*(u16 *)(req + 8) >> 8) & SUBKIND_MASK, 3, 0);
    Func_080be02c();
    goto L_080bf1d6_shared;

L_080bec62:
    Func_08015120(*(s16 *)(req + 0), 1);
    Func_08015120(abilityId, 4);
    Func_080f9010(114);
    Func_080151c8(TEXT_TBD);
    Func_080030f8(60);
    goto L_080bf1d6_shared;

L_080bec90:
    lookupResult = Func_080be18c(abilityId);
    if (lookupResult == -1) {
        goto L_080bf1d6_shared;
    }
    Func_080771c8(*(s16 *)(req + 0), (s8)(*(u16 *)(req + 8) >> 8) & SUBKIND_MASK,
                  (u8)*(u16 *)(req + 8));
    abilityData = Func_08077080(abilityId);
    Func_08015120(*(s16 *)(req + 0), 1);
    Func_08015120(abilityId, 4);
    Func_080151c8(TEXT_TBD);
    *(u32 *)(tgt + 80) = *((u8 *)abilityData + 2);
    goto L_080bee00;

    /* ---- case tier==6, address 0x080becea (not yet isolated separately;
     * the retained region for this case falls between the tier==5 tail
     * above and the internal L_080bee00 label below in
     * games/gs1/asm/080beb08.s -- treated as an alias of L_080bee00's entry
     * pending exact instruction-range isolation.
     */
L_080becea:
    goto L_080bee00;

    /*
     * ---- shared: L_080bee08, address 0x080bee08 ----
     * Pool constants resolved directly from roms/gs1-en.gba (per-owner .o
     * disassembly leaves the literal pool out of bounds, so the ROM's own
     * bytes at the pc-relative target addresses were read instead, pinned
     * against Region_080beb08's known load address 0x080beb08):
     *   0x080bef6c -> 0x00000129 (297)   actor+297 status/charge flag byte
     *   0x080bef74 -> 0x00004001         tgt->88 flag combo A (subclass 1/5)
     *   0x080bef70 -> 0x00004004         tgt->88 flag combo B (subclass 2/3)
     */
L_080bee08:
    targetUnit = Func_08077008(tgt[2]);
    *(u32 *)(tgt + 76) = 1;
    *(s32 *)(tgt + 80) = Func_08077170(*(s16 *)(req + 0));
    *(s32 *)(tgt + 84) = 2;
    if (((u8 *)actor)[297] != 0) {
        *(s32 *)(tgt + 88) = 0x4000 | Func_080c23e8(*((u8 *)actor + 296));
        goto L_080beea8;
    }
    *(u32 *)(tgt + 88) = 0;
    {
        u8 subclass = *((u8 *)actor + 296);
        if (subclass <= 5) {
            switch (subclass) {
            case 0: goto L_080beea8;           /* -> 0x080beea0, no assignment path merges below */
            case 1: *(s32 *)(tgt + 88) = 0x4001; goto L_080beea6;
            case 2: *(s32 *)(tgt + 88) = 0x4004; goto L_080beea6;
            case 3: *(s32 *)(tgt + 88) = 0x4004; goto L_080beea6;
            case 4: goto L_080beea8;            /* no assignment */
            case 5: *(s32 *)(tgt + 88) = 0x4001; goto L_080beea6;
            }
        }
        goto L_080beea8;
    }
L_080beea6:
    /* both `case 0` and `case 5` land on the *same* 0x080beea0 body in the
     * reference's secondary jump table, which just falls into str->88;
     * modeled directly above via the shared L_080beea6 assignment point. */
L_080beea8:
    Func_08015120(*(s16 *)(req + 0), 1);
    Func_080151c8(TEXT_TBD);

    /*
     * ---- L_080beef4 / L_080beebe / L_080beee0 / L_080bef28: status-cure
     * gate, address range 0x080beef4-0x080bef46, all operating on
     * `targetUnit` (the recipient looked up via tgt[2] above), not `actor`.
     * Pool constants resolved the same way:
     *   0x080bef7c (pc,#116 from 0x400) -> 0x00000814 (2068) targetUnit+2068
     *   0x080bef80 (pc,#112 from 0x40a) -> 0x0000013b (315)  targetUnit+315
     * (both plain status-flag byte offsets, no relocation needed since they
     * are compile-time constants, not symbol references).
     */
    if (*(s16 *)((u8 *)targetUnit + 56) != 0) {
        if (((u8 *)targetUnit)[316] == 0 &&               /* 158*2 */
            *((u8 *)targetUnit + 2068) == 0 &&
            *((u8 *)targetUnit + 315) == 0 &&
            ((u8 *)targetUnit)[314] != 0) {                /* 157*2 */
            if (!(Func_080771a0() > 152)) {
                ((u8 *)targetUnit)[30] = 5; /* r5 kept the loop's own value; see TODO below */
            }
        }
        if ((Func_080771a0() & 31) == 0) {
            ((u8 *)targetUnit)[30] = 0;
        }
    }

L_080bef28:
    if (Func_080770c0(366) != 0) { /* 183*2 */
        ((u8 *)tgt)[30] = 0;
    }
    if (*(s16 *)((u8 *)actor + 56) == 0) {
        goto L_080bf1a8_shared;
    }
    if ((Func_080771a0() & 31) != 0) {
        goto L_080bec5c;
    }
    goto L_080befb4_shared;

    /* ---- Region_080bef88: continuation, address 0x080bef88 ---- */
L_080bf1a8_shared:
L_080befac_shared:
L_080befb4_shared:
    {
        s16 impactPower;
        s32 rangeMask;
        impactPower = *(s16 *)(req + 0); /* placeholder wiring for Func_080772f8/Func_080022ec chain */
        Func_080772f8(impactPower);
        Func_080022ec(200 << 16, 0);
        rangeMask = Func_080771a0();
        (void)rangeMask;
        if (rangeMask <= 0) {
            goto L_080bf1a8;
        }
        tgt[44] = 1;
        goto L_080bf1a8;
    }

    /*
     * ---- per-target power-accumulation loop, address 0x080bf0ca in
     * games/gs1/asm/080bef88.s -- guarded by `abilityId == 178`, iterating
     * `(s8)tgt[1]` times over the per-target slot bytes at tgt+2.., calling
     * Func_08077178(reqId, slotByte, abilityData->2, abilityData->3, 100)
     * and writing the (byte-truncated) result to tgt+2+i+56. This is the
     * block the domain notes call "per-target power accumulation"; it is
     * reachable from the 0x080bf0b4 ability-id==178 gate, which in the
     * retained assembly sits between the still-unmodeled buff/debuff
     * classification table (0x080bf044) and the elemental-table lookup
     * (0x080bf0f8) -- both left as the next increment, only this loop and
     * its direct guard are wired in this pass.
     */
    if (abilityId == 178) {
        s8 count = (s8)tgt[1];
        s32 i;
        for (i = 0; i < count; i++) {
            u8 slotByte = tgt[2 + i];
            s32 result = Func_08077178(*(s16 *)(req + 0), slotByte,
                                        *((u8 *)abilityData + 2),
                                        *((u8 *)abilityData + 3), 100);
            tgt[2 + i + 56] = (u8)result;
        }
    }

L_080bf1a8:
    if (*(s16 *)((u8 *)actor + 6) == 2) {
        s32 statusKind = *(s32 *)(tgt + 84);
        if (statusKind != 5 && statusKind != 9) {
            *(s32 *)(tgt + 84) = 4;
        }
    }
    *(u16 *)(tgt + 72) = *(u16 *)((u8 *)actor + 6);
    return;

    /*
     * ---- shared internal veneer targets, addresses 0x080bec5c / 0x080bec8a
     * / 0x080bee00 (all inside games/gs1/asm/080beb08.s) and 0x080bf1d4 /
     * 0x080bf1d6 (inside games/gs1/asm/080bef88.s, immediately before the
     * real epilogue). Their exact bodies are not fully isolated from the
     * surrounding per-target power-accumulation loop
     * (Region_080bef88's `.L_080bf0ca` loop, `sub_08077178`) in this pass;
     * they currently fall straight through to the real epilogue via
     * L_080bf1a8, which is structurally correct for "this ability
     * resolution step is done" but does not yet perform the per-target
     * power write the domain notes describe.
     */
L_080bec5c:
L_080bec8a:
L_080bee00:
L_080bf1d6_shared:
    goto L_080bf1a8;
}
