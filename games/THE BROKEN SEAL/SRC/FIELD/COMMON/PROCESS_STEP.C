#include "TYPES.H"
#include "IWRAM_CALL.H"

/*
 * Runs what one step of the party leader costs and triggers on the field:
 * reads the tile under the new position (its event byte opens map regions
 * and records the step and warp events), advances the encounter step
 * counter by the leader's speed, runs the step effects every whole count,
 * steps the poison and Avoid timers, and when the step effects knocked a
 * party member out, records who fell.
 */

struct StepTile {
    u8 unknown_0[2];
    u8 event;                       /* 0x02 */
    u8 unknown_3;
};

struct StepLayer {
    struct StepTile *tiles;         /* 0x00 */
    u8 unknown_04[0x2c];
};

struct StepMapWork {
    u8 unknown_000[0x130];
    struct StepLayer layers[3];     /* 0x130 */
};

struct StepActor {
    u8 unknown_00[8];
    u8 motion[0x20];                /* 0x08 */
    s32 unknown_28;                 /* 0x28 */
    u8 unknown_2c[4];
    s32 speed;                      /* 0x30 */
    u8 unknown_34[4];
    s32 unknown_38;                 /* 0x38 */
};

struct StepWork {
    u8 unknown_000[0x14];
    struct StepActor *actors[1];    /* 0x014 */
    u8 unknown_018[0x16c - 0x18];
    s16 step_event;                 /* 0x16c */
    s16 warp_event;                 /* 0x16e */
    u8 unknown_170[0xc];
    u16 unknown_17c;                /* 0x17c */
    s16 unknown_17e;                /* 0x17e */
    u8 unknown_180[2];
    u16 unknown_182;                /* 0x182 */
    s16 fallen_count;               /* 0x184 */
    s16 standing_count;             /* 0x186 */
    u16 fallen[11];                 /* 0x188 */
    s16 mode;                       /* 0x19e */
    u8 encounter_full[4];           /* 0x1a0 */
    u8 unknown_1a4[0xc];
    s32 encounter_rate;             /* 0x1b0 */
    s32 encounter_steps;            /* 0x1b4 */
    struct StepTile *tile;          /* 0x1b8 */
    struct StepTile *previous_tile; /* 0x1bc */
};

struct StepState {
    u8 unknown_000[0x1f2];
    u8 unknown_1f2;                 /* 0x1f2 */
    u8 unknown_1f3;
    s32 selected_actor;             /* 0x1f4 */
    u8 party[0x34];                 /* 0x1f8 */
    s16 unknown_22c;                /* 0x22c */
    s16 unknown_22e;                /* 0x22e */
    s16 unknown_230;                /* 0x230 */
    s16 unknown_232;                /* 0x232 */
    u8 unknown_234[0xa];
    s16 unknown_23e;                /* 0x23e */
    u8 unknown_240[4];
    s32 unknown_244;                /* 0x244 */
};

struct StepOwner {
    u8 unknown_000[0x38];
    s16 hp;                         /* 0x038 */
    u8 unknown_03a[0x131 - 0x3a];
    u8 unknown_131;                 /* 0x131 */
};

extern struct StepWork *Data_03001ebc;
extern struct StepState Data_02000240;

s32 Party_CountActiveOwnersFar(void);
struct StepOwner *Owner_GetStateFar(s32 owner);
s32 GameFlag_TestFar(s32 flag);
void UpdateMapRegionAtPosition(s32 x, s32 y, s32 z);
s32 BattleFx_ApplyLookupResult(void *motion, s32 speed);
s32 EffectRuntime_LookupByTableEntry(s32 kind, s32 speed);
void BattleParty_ApplyDrain(s32 steps, s32 unused);
s32 Func_080b50f8(void);
void Audio_PlayCue(s32 cue);
void Event_ClearInvalidPackedValues(void);
s32 BattleParty_ApplyStatusDamage(void);
void Func_08093874(s32 actor, s32 event);
void BattleParty_ApplyHealthDelta(s32 amount, s32 flags);

void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z)
{
    struct StepWork *work = Data_03001ebc;
    /* FAKEMATCH: the map work cell is reached as gEventWork[-19]; the ROM loads
       0x03001ebc once and subtracts 76 to reach 0x03001e70. */
    struct StepMapWork *map = ((struct StepMapWork **)&Data_03001ebc)[-19];
    s32 selected = Data_02000240.selected_actor;
    struct StepActor *actor = work->actors[selected];
    s16 hp[8];
    struct StepTile *tile;
    struct StepOwner *owner;
    u32 count;
    s32 fell;
    u32 i;
    u32 event;
    s32 speed;
    s32 steps;
    s32 n;
    s32 sum;
    s32 full;

    fell = 0;
    if (work == NULL)
        return;

    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++)
        hp[i] = Owner_GetStateFar(Data_02000240.party[i])->hp;

    if (work->mode == 3) {
        tile = &((struct StepTile *)0x02020000)[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)];
    } else {
        if ((u32)layer <= 2)
            tile = map->layers[layer].tiles;
        else
            tile = (struct StepTile *)0x02010000;
        tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
    }
    event = tile->event;
    work->previous_tile = work->tile;
    work->tile = tile;
    if (event != 0)
        UpdateMapRegionAtPosition(x, y, z);
    if (event >= 1 && event <= 239)
        work->step_event = event;
    if (event >= 252 && event <= 254)
        work->warp_event = event;

    if (Data_02000240.unknown_1f2 == 0 && actor != NULL && actor->unknown_38 != (s32)0x80000000) {
        speed = actor->speed;
        if (GameFlag_TestFar(0x167))
            speed <<= 1;
        if (work->mode == 3) {
            full = 1;
            work->unknown_17c = BattleFx_ApplyLookupResult(actor->motion, speed);
        } else if (event == 240 || event == 241) {
            full = work->encounter_full[event - 239];
            work->unknown_17c = EffectRuntime_LookupByTableEntry(event - 239, speed);
        } else {
            full = work->encounter_full[0];
            work->unknown_17c = EffectRuntime_LookupByTableEntry(0, speed);
        }
        steps = Iwram_MulQ16(work->encounter_rate, actor->speed);
        if (!full)
            steps /= 2;
        sum = work->encounter_steps + steps;
        work->encounter_steps = sum;
        if (sum > 0xffff) {
            n = sum / 0x10000;
            work->encounter_steps = sum & 0xffff;
            BattleParty_ApplyDrain(n, 0);
            if (Func_080b50f8())
                Audio_PlayCue(139);
            Event_ClearInvalidPackedValues();
            fell = BattleParty_ApplyStatusDamage();
        }
        if (Data_02000240.unknown_22e == 0 && event == 250) {
            if (work->previous_tile->event == 250)
                Data_02000240.unknown_232 += actor->speed / 0x10000;
            else
                Data_02000240.unknown_232 = Data_02000240.unknown_22c / 2;
        }
        if (Data_02000240.unknown_244 != 0 && Data_02000240.unknown_23e != 2) {
            Data_02000240.unknown_244 -= actor->speed;
            if (Data_02000240.unknown_244 <= 0) {
                Data_02000240.unknown_244 = 1;
                if (work->unknown_17e == 0)
                    work->unknown_17e = 0x2096;
            }
        }
    }

    if (Data_02000240.unknown_22e == 1) {
        Data_02000240.unknown_232++;
        if (Data_02000240.unknown_232 == Data_02000240.unknown_22c / 2)
            Func_08093874(selected, 0x101);
        if (Data_02000240.unknown_232 == Data_02000240.unknown_22c)
            Func_08093874(selected, 0x100);
    }
    if (Data_02000240.unknown_232 >= Data_02000240.unknown_22c) {
        n = Data_02000240.unknown_230;
        Data_02000240.unknown_232 = 0;
        BattleParty_ApplyHealthDelta(-(n & 255), n & 0x100);
        fell++;
    }

    if (fell) {
        work->fallen_count = 0;
        work->standing_count = 0;
        actor->unknown_28 = 0x40000;
        Func_08093874(selected, 0x102);
        for (i = 0; i < count; i++) {
            owner = Owner_GetStateFar(Data_02000240.party[i]);
            if (owner->hp > 0) {
                work->standing_count++;
            } else if (hp[i] != 0) {
                work->fallen[work->fallen_count++] = Data_02000240.party[i];
                work->unknown_182 = 0xffff;
                owner->unknown_131 = 0;
            }
        }
    }
}
