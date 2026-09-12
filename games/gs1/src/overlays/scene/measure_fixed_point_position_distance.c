#include "types.h"
#include "scene.h"
#include "staged_actor.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"

/* overlays/measure_fixed_point_position_distance.c */
/* overlays/shared/measure_fixed_point_position_distance.c */
/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

typedef s32 (*IwramIntegerSquareRoot)(s32);

s32 CalculateFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

/* overlays/shared/find_actor_at_fixed_point_position.c */
extern u8 gWork[];

s32 *FunctionHead_0200006c(s32 *arg0)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] / 0x10000) == (p[3] / 0x10000)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/* overlays/scene/actor/staged_motion/advance_staged_actor_pair.c */
extern u32 StagedActorStepTable[];

extern struct StagedActor *GetStagedActor(s32 arg0);
extern struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

void MapStagedActor_AdvancePair(void)
{
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 direction_index;
    u32 direction_step;
    s32 move_speed;
    s32 transition_state;

    lead_actor = GetStagedActor(0);
    direction_index = lead_actor->direction_and_kind >> 12;
    direction_step = StagedActorStepTable[direction_index];
    destination[0] = lead_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = lead_actor->y;
    direction_step <<= 16;
    destination[2] = lead_actor->z.value + direction_step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    direction_step = StagedActorStepTable[direction_index];
    destination[0] = next_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = next_actor->y;
    direction_step <<= 16;
    destination[2] = next_actor->z.value + direction_step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    direction_step = StagedActorStepTable[direction_index];
    destination[0] = next_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = next_actor->y;
    direction_step <<= 16;
    destination[2] = next_actor->z.value + direction_step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    transition_state = next_actor->transition_busy;
    if (transition_state != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_speed = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_speed;
    next_actor->move_rate_z = move_speed;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_speed;
    lead_actor->move_rate_z = move_speed;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = transition_state;
    next_actor->unknown_2c = transition_state;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = transition_state;
    lead_actor->unknown_2c = transition_state;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

/* overlays/shared/fill_grid_attribute_rectangle.c */
extern u8 *gCam;
extern u8 gUnk[];

s32 FunctionHead_02000244(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5)
{
    u8 *g = gCam;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (arg0 <= 2) {
            u32 off = arg0 * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = gUnk;
        }
        base += (arg1 + (arg2 << 7)) * 4;
        for (i = 0; i < arg4; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < arg3; j++) {
                p[2] = (u8)arg5;
                p += 4;
            }
        }
    }
    return 0;
}

/* overlays/scene/actor/staged_motion/stop_blocked_actor_motion.c */
typedef struct StagedMoveTarget { s32 unk0; s32 unk4; s32 unk8; } StagedMoveTarget;
typedef struct StagedActorRecord { u8 filler0[0x28]; s16 *unk28; } StagedActorRecord;
typedef struct StagedActor {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    StagedActorRecord *unk50;
} StagedActor;

extern s32 RomBytes_0200e190[];
extern s32 RomBytes_0200e1d0[];
extern StagedActor *Actor_Run(StagedMoveTarget *, StagedActor *);

s32 MapStagedActor_StopBlockedMotion(StagedActor *actor)
{
    StagedMoveTarget target;
    u32 direction_index;
    s32 direction_step;
    StagedActor *blocking_actor;

    direction_index = actor->unk6 >> 12;
    direction_step = RomBytes_0200e190[direction_index];
    target.unk0 = actor->unk8 + (direction_step & 0xffff0000);
    target.unk4 = actor->unkC;
    direction_step = direction_step << 16;
    target.unk8 = actor->unk10 + direction_step;
    blocking_actor = Actor_Run(&target, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = *blocking_actor->unk50->unk28;
        s32 *p = RomBytes_0200e1d0;

        do {
            if (blocking_kind == *p++) goto done;
            kind_index++;
        } while (kind_index <= 5);
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
    direction_step = RomBytes_0200e190[direction_index];
    target.unk0 = actor->unk8 + (direction_step & 0xffff0000);
    target.unk4 = actor->unkC;
    direction_step = direction_step << 16;
    target.unk8 = actor->unk10 + direction_step;
    if (FunctionHead_02006266(actor, &target) > 0) {
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
done:
    return 0;
}

/* overlays/scene/actor/staged_motion/find_clear_actor_position.c */

s32 MapStagedActor_FindClearPosition(s32 *request)
{
    s32 direction_index;
    s32 origin[3];
    u8 *actor;
    s32 step_count;
    s32 tiles_y, tiles_x;
    u8 *active_flag;
    s32 footprint_offset;
    s32 extent_a, extent_b;
    u8 *footprint_table;
    s32 *candidate;
    s32 found;
    request[5] = 0;
    actor = (u8 *)Actor_Place(&direction_index, request + 1, request);
    if (actor == 0)
        return 0;
    active_flag = actor + 0x22;
    *active_flag = 2;
    step_count = 0;
    footprint_table = (u8 *)RomBytes_0200e1e8;
    footprint_offset = request[0] << 4;
    {
        s32 table_offset = footprint_offset + 4;
        extent_a = *(s32 *)(footprint_table + table_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 12;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        tiles_y = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(footprint_table + footprint_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 8;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        tiles_x = (extent_a + extent_b) >> 4;
    }
    {
        u8 *actor_bytes;
        s32 actor_y;
        s32 direction_x;
        candidate = origin;
        direction_x = RomBytes_0200e190[direction_index] & 0xffff0000;
        actor_bytes = actor;
        candidate[0] = *(s32 *)(actor_bytes + 8) + direction_x;
        actor_y = *(s32 *)(actor_bytes + 12);
        candidate[1] = actor_y;
        candidate[2] = *(s32 *)(actor_bytes + 16) + (RomBytes_0200e190[direction_index] << 16);
        request[3] = actor_y;
    }
    for (;;) {
        s32 row, column;
        {
            u8 *table = (u8 *)RomBytes_0200e1e8;
            s32 table_offset = request[0] << 4;
            table_offset += 4;
            request[4] = origin[2] + (*(s32 *)(table + table_offset) << 16);
        }
        row = 0;
        while (row < tiles_y) {
            {
                u8 *table = (u8 *)RomBytes_0200e1e8;
                request[2] = origin[0] + (*(s32 *)(table + (request[0] << 4)) << 16);
            }
            column = 0;
            while (column < tiles_x) {
                s32 *probe = request + 2;
                if (FunctionHead_020064a0(actor, probe) == 2)
                    goto found;
                probe[0] = probe[0] + 0x100000;
                column++;
            }
            request[4] = request[4] + 0x100000;
            row++;
        }
        step_count++;
        origin[0] = origin[0] + (RomBytes_0200e190[direction_index] & 0xffff0000);
        origin[2] = origin[2] + (RomBytes_0200e190[direction_index] << 16);
    }
found:
    *active_flag = 0;
    found = 0;
    if (step_count != 0) {
        s32 direction = RomBytes_0200e190[direction_index];
        s32 direction_x = direction & 0xffff0000;
        s32 offset_x = direction_x *step_count;
        s32 offset_z = (direction << 16) * step_count;
        u8 *actor_bytes = actor;
        request[2] = *(s32 *)(actor_bytes + 8) + offset_x;
        request[3] = *(s32 *)(actor_bytes + 12);
        request[4] = *(s32 *)(actor_bytes + 16) + offset_z;
        found = 1;
    }
    return found;
}

/* overlays/scene/actor/staged_motion/staged_motion.c */
/* overlays/scene/actor/staged_motion/actor_motion.c */

s32 Actor_RunStep18WhenTargetSet(s32 *p)
{
    s32 t = Actor_unk22(0);
    if (p[14] == (s32)0x80000000 && p[16] == (s32)0x80000000)
        return 0;
    Actor_SetMode((s32)p, t, 18, 0);
    return 0;
}

/* overlays/scene/actor/staged_motion/actor_position.c */
extern u8 Value_0000087a;

s32 *Actor_unk43_4(s32);

u8 *Actor_unk44_4(s32);
u8 *AllocateEffectTransfer(s32, s32);

void Actor_SetFlagByteBySlotZeroPosition(void)
{
    s32 *g = Actor_unk43_4(0);
    u8 *q;
    if (Actor_unk23((s32)&Value_0000087a) != 0)
        q = Actor_unk44_4(21);
    else
        q = AllocateEffectTransfer(20);
    if (q != 0) {
        if (g[3] > 0xc80000)
            q[0x23] = 3;
        else
            q[0x23] = 1;
    }
}

/* overlays/scene/actor/staged_motion/actor_presentation.c */
extern char Value_00000806;

void Dialogue_RunActorFourteenTalk(void)
{
    s32 flag = 0x806;
    Actor_unk46_4();
    if (Actor_unk24(0x815) != 0) {
        Actor_unk19_2(0x11c9);
        Actor_ApplyStagedMotion(14, 0);
    } else if (Actor_unk25(flag) == 0) {
        Actor_unk20_2(flag);
        Actor_unk21_2(0xf7c);
        Actor_unk260(14, 0, 4);
        Actor_Apply2(14, 0);
    } else {
        Actor_unk22_2(0xf7e);
        Actor_unk261(14, 0, 4);
        Actor_Apply3(14, 0);
    }
    Actor_unk47_4();
}

/* overlays/scene/actor/staged_motion/effect.c */
extern volatile u32 gIw;

void Effect_UpdateObjectOnOddFrames(s32 p)
{
    if ((gIw & 1) != 0)
        Actor_Apply4(p, Actor_Apply5(gIw >> 1, 6));
    if ((gIw & 0xf) == 0)
        Actor_unk23_2(p);
}

void Effect_UpdateObjectOnOddFramesOnly(s32 p)
{
    if ((gIw & 1) != 0)
        Actor_Apply6(p, Actor_Apply7(gIw >> 1, 6));
}

/* overlays/scene/actor/staged_motion/field_scene.c */
/* Actor IDs the scene sets up: three actors share a common data pointer,
 * a fourth actor (11) is driven through its own sequence of position/speed
 * calls. */
#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11
/* Shared data used for actors A, B, C. */

extern u8 Value;
extern u8 Value4;
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[]; /* Buffer passed by address to SceneWork_SetStepValue_2. */
extern u8 gOvStagedMotion[];
extern u8 gVal4[];
extern u8 gOv2StagedMotion[];
extern u8 Value_00000ee4;

u8 *Actor_unk48_4();
u8 *Actor_unk49_4();

u8 *Actor_unk50_4();

u8 *Actor_unk51_4();

/* Runs four fixed steps in order: a single-argument call, a call that takes
 * the address of the byte at Value4 plus two constants, another
 * three-constant call, and a final single-argument call. */

/* Newly identified engine calls: each macro below names the site's own
 * engine function from its reconstructed C source, and keeps the site's
 * own calling form. */

void Scene_RunFlag807BranchSequence(void)
{
    u32 i;
    s32 record;

    Actor_unk52_4();
    if (Actor_Check(0x807) == 0) {
        Actor_Do(0x807);
        Actor_unk2_2(0xf63);
        Actor_Place(18, 0x103, 0);
        Actor_unk53_4(0, 18, 20);
        Actor_unk54_4(18, 0, 6);
        Actor_unk2_3(18, 0x8000, 30);
        Actor_unk55_4(18, 2, 20);
        Actor_unk56_4(18, 0, 6);
        Actor_unk57_4(18, 0, 10);
        Actor_unk3_3(18, 0x103, 0);
        Actor_unk58_4(18, 0, 10);
        Actor_unk4_3(0, 0x102, 60);
    } else {
        Actor_unk5_3(18, 0x103, 0);
        Actor_unk3_2(0xf66);
        Actor_unk59_4(18, 0, 20);
    }
    Actor_unk60_4();
}

void Scene_RunScene373(void)
{
    u32 i;
    s32 record;

    Actor_unk61_4();
    Actor_unk6_3(0x30000, 0x30000, 0x10000);
    Actor_unk62_4(10);
    Actor_unk7_3(-1, -1, 0xe666);
    Actor_unk4_2(0x1c9a);
    Actor_unk63_4(17, 0, 20);
    Actor_unk64_4(17, 0, 20);
    Actor_unk65_4(17, 0);
    Actor_unk66_4();
}

void Scene_RunScene373(void)
{
    u32 i;
    s32 record;

    Actor_unk67_4(188);
    Actor_unk8_3(0x200f544, 45, 11);
    Actor_unk9_3(0, 0x101, 0x1a4);
    Actor_unk68_4(11);
}

void Scene_RunScene373(void)
{
    u32 i;
    s32 record;

    Actor_unk69_4(158);
    Actor_unk10_3(0x200f55a, 54, 32);
    Actor_unk11_3(0, 0x196, 0x2d7);
    Actor_unk70_4(5);
}

void Scene_RunScene373(void)
{
    u32 i;
    s32 record;

    Actor_unk71_4(158);
    Actor_unk12_3(0x200f570, 45, 39);
    Actor_unk13_3(0, 0x106, 0x325);
    Actor_unk72_4(6);
}

/* Runs four scene primitives in sequence: one single-argument call, one call
 * passing the address of Value plus two small constants, one call
 * with a byte-flag-sized first argument (0) and two larger constants, and a
 * final single-argument call. */
void Scene_RunPrimarySequence(void)
{
    Audio_PlayCue_1(158);
    Actor_unk14_3((s32)&Value, 49, 69); /* main:08009178 */
    Motion_ResetPosMode2_1(0, 326, 0x466); /* object_id 0, x 326, z 0x466 */
    Actor_unk5_2(8); /* main:0808a248 */
}

/* Runs a short scripted step, then two 3-argument setup calls, then another
 * short scripted step; none of the callees' effects are visible here. */
void Scene_RunScene373SequenceA(void)
{
    Audio_PlayCue_1(158);
    Actor_unk15_3(0x200f586, 52, 76); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 0x176, 0x4d6); /* object_id 0, x 0x176, z 0x4d6 */
    Actor_unk73_4(9); /* main:0808a248 */
}

void Scene_RunPrimarySequenceSecond(void)
{
    Audio_PlayCue_1(158);
    Actor_unk16_3((s32)&Value4, 35, 74); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 102, 0x4b6);
    Actor_unk6_2(10); /* main:0808a248 */
}

void Scene_RunScene373SequenceC(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Actor_unk2(0);
    rec7 = Actor_unk3(5);
    Actor_unk74_4();
    *(volatile s32 *)(rec7 + 8) = *(volatile s32 *)(rec8 + 8);
    *(volatile s32 *)(rec7 + 12) = *(volatile s32 *)(rec8 + 12);
    *(volatile s32 *)(rec7 + 16) = *(volatile s32 *)(rec8 + 16);
    *(volatile s32 *)(rec7 + 56) = -0x80000000;
    *(volatile s32 *)(rec7 + 60) = -0x80000000;
    *(volatile s32 *)(rec7 + 64) = -0x80000000;
    *(volatile s32 *)(rec7 + 36) = 0;
    *(volatile s32 *)(rec7 + 40) = 0;
    *(volatile s32 *)(rec7 + 44) = 0;
    *(volatile s32 *)(rec7 + 20) = *(volatile s32 *)(rec8 + 12);
    Actor_unk75_4(1);
    Actor_unk17_3(5, 0x10000, 0x8000);
    Actor_unk18_3(5, 110, 0x11b);
    Actor_unk76_4(0, 5, 2);
    Actor_unk7_2(0xf39);
    if (*(volatile s32 *)(rec8 + 8) < *(volatile s32 *)(rec7 + 8)) {
        Actor_unk19_3(0xa005, 0, 2);
    } else {
        Actor_unk20_3(0x8005, 0, 2);
    }
    Actor_unk77_4(0, 3);
    Actor_unk78_4(2);
    Actor_unk79_4(5, 2);
    record = Actor_unk4(0);
    if (record != 0) {
        Actor_unk80_4(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_unk81_4(5);
    Actor_unk82_4(5, 0, 0);
    Actor_unk21_3(0, 110, 0x12f);
    Actor_unk83_4();
}

void Scene_RunScene373SequenceD(void)
{
    u32 i;
    s32 record;
    s32 base5_f4d;

    if (Actor_unk5(0x808) == 0) {
        Actor_unk84_4();
        Actor_unk22_3(0, 0x10000, 0x8000);
        base5_f4d = (s32)gVal;
        Actor_unk85_4(base5_f4d);
        Actor_unk86_4(15, 0, 2);
        Actor_unk87_4(16, 0, 2);
        Actor_unk88_4((base5_f4d + 2), 1);
        Actor_unk89_4(6);
        Actor_unk23_3(0, 69, 0x366);
        Actor_unk90_4();
    }
}

void Scene_RunScene373SequenceE(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Actor_unk6(22);
    Actor_unk91_4();
    Actor_unk24_3(0, 0x20000, 0x20000);
    Actor_unk92_4(0, 5, 0);
    Actor_unk25_3(0, 215, 0x193);
    rec7[90] |= 1;
    Actor_unk26_3(22, 0xa60000, 0x1770000);
    Actor_unk27_3(22, 0x2000, 20);
    rec7[90] = (rec7[90] ^ 1);
    Actor_unk28_3(22, 0x28000, 0x28000);
    Actor_unk93_4(22, 4, 0);
    Actor_unk29_3(22, 202, 0x18b);
    Actor_unk94_4(0, 1);
    Actor_unk95_4(10);
    Actor_unk30_3(0, 0xb000, 0);
    Actor_unk31_3(22, 0x3000, 24);
    Actor_unk32_3(0, 0x100, 0);
    Actor_unk96_4(0, 2);
    Actor_unk97_4(20);
    Actor_unk33_3(22, 0x18000, 0x10000);
    Actor_Run(0, 0x200f59c);
    Actor_unk98_4(10);
    Actor_unk34_3(22, 0x103, 0);
    Actor_unk2_4(22, 0x200f5ec);
    Actor_unk99_4(0);
    Actor_unk35_3(0, 0x100, 0x1da);
    Actor_unk36_3(0, 0xc000, 0);
    Actor_unk100_4(22);
    Actor_unk37_3(22, 0x100, 0x1c8);
    Actor_unk101_4(0, 1);
    Actor_unk38_3(22, 0x4000, 20);
    Actor_unk102_4(22, 2);
    Actor_unk103_4(20);
    Actor_unk8_2(0xfce);
    Actor_unk104_4(22, 0);
    record = Actor_unk26(22);
    *(volatile s32 *)(record + 108) = 0x200d72d;
    Actor_unk3_4(22, 0x200e248);
    Actor_unk9_2(0x823);
    Actor_unk105_4();
}

void Scene_RunScene373(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 rec8;
    s32 record;

    p8 = a1;
    rec8 = Actor_unk7(22);
    Actor_unk106_4();
    Actor_unk107_4(22, 2);
    Actor_unk39_3(22, 0x100, 20);
    Actor_unk108_4(0, 2);
    Actor_unk40_3(0, 0x102, 40);
    Actor_unk109_4(0, a0);
    Actor_unk110_4(10);
    Actor_unk41_3(22, 0x103, 0);
    Actor_unk111_4(22, p8);
    Actor_unk112_4(0);
    Actor_unk113_4(20);
    Actor_unk114_4(22, 2);
    *(s32 *)(rec8 + 24) = 0x10000;
    *(s32 *)(rec8 + 28) = 0x10000;
    record = Actor_unk8(0);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Actor_unk10_2(0xfce);
    Actor_unk115_4(22, 0);
    record = Actor_unk27(22);
    *(s32 *)(record + 108) = 0x200d72d;
    Actor_unk4_4(22, 0x200e248);
    Actor_unk116_4();
}

/* Runs a scripted movement/pose sequence for actors 0, 1 and 8, reading two
 * lookup records along the way (one 32-bit-field record, one 16-bit-field
 * record) to copy their stored values onto actor 1. */
void Scene_RunSecondaryActorSequence(void)
{
    u32 i;
    s32 record;
    s32 slot_table;

    Battle_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 0);
    Motion_SetAngleToward_1(8, 0, 20);
    slot_table = (s32)gVal2;
    SceneWork_SetStepValue_1(slot_table);
    Motion_SetVarCb_1(8, 2);
    Battle_RunThenWaitIfModeZero_1(8, 0, 20);
    Motion_SetSpeedLim_1(0x10000, 0x2000); /* main:0808a208 */
    Motion_CamBounds_1(0x18e0000, -1, 0x2460000, 1);
    Motion_SetSpeed_1(0, 0xcccc, 0x6666);
    Motion_SetSpeed_2(1, 0xcccc, 0x6666);
    Motion_SetPosReset_1(0, 0x1a4, 0x260);
    Motion_ArmCb_1(0, 0xa000, 0);
    Motion_ArmCb_2(8, 0x3000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Copy the record's fields at +8 and +16 onto actor 1. */
        Motion_SetHPosTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Motion_SetPosReset_2(1, 0x192, 0x260);
    Motion_ArmCb_3(1, 0xd000, 20);
    BattleEv_RunWait_1(0x1001, 0);
    Motion_ArmCb_4(8, 0x5000, 20);
    Motion_CallWaitAnim_1(8, 3); /* main:0808a110 */
    BattleEv_RunWait_2(0x4008, 0);
    Object_SetModeById_1(0, 3);
    Motion_CallWaitAnim_2(1, 3); /* main:0808a110 */
    Battle_WaitMode0_1(20);
    Motion_SetVarCbObj_1(8, 2); /* main:0808a138 */
    Motion_SetSpeed_3(0x4008, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) { /* main:0808a070 */
        bump_step(1);
        Motion_SetVarCb_2(8, 1);
    }
    Battle_RunThenWaitIfModeZero_2(0x4008, 0, 40); /* main:0808a188 */
    BattleFx_SpawnLinked_1(8, 0x105, 60);
    /* Pass the slot table's field at +6 for the slot passed above. */
    SceneWork_SetStepValue_2((slot_table + 6)); /* main:0808a170 */
    Battle_RunThenWaitIfModeZero_3(0x4008, 0, 20); /* main:0808a188 */
    Motion_SetVarCbObj_2(1, 1); /* main:0808a138 */
    Battle_WaitMode0_2(40);
    Battle_RunThenWaitIfModeZero_4(0x1001, 0, 40); /* main:0808a188 */
    Motion_SetVarCbObj_3(8, 1); /* main:0808a138 */
    Motion_ArmCb_5(8, 0xd000, 20);
    BattleEv_RunWait_3(0x4008, 0);
    Motion_CallWaitAnim_3(1, 3); /* main:0808a110 */
    Battle_RunThenWaitIfModeZero_5(0x1001, 0, 120); /* main:0808a188 */
    Battle_RunThenWaitIfModeZero_6(0x4008, 0, 20); /* main:0808a188 */
    BattleFx_SpawnLinked_2(1, 0x105, 40);
    Battle_RunThenWaitIfModeZero_7(0x1001, 0, 40); /* main:0808a188 */
    Motion_CallWaitAnim_4(8, 4); /* main:0808a110 */
    Battle_RunThenWaitIfModeZero_8(0x4008, 0, 20);
    Motion_CallWaitAnim_5(1, 3); /* main:0808a110 */
    Battle_WaitMode0_3(40);
    Motion_ArmCb_6(8, 0x5000, 20);
    Battle_RunThenWaitIfModeZero_9(0x4008, 0, 10); /* main:0808a188 */
    Object_SetModeById_2(0, 3);
    Motion_CallWaitAnim_6(1, 3); /* main:0808a110 */
    Battle_WaitMode0_4(20);
    Motion_CallWaitAnim_7(8, 3);
    Object_SetModeById_3(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Copy the record's fields at +10 and +18 onto actor 1. */
        Motion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_1(1);
    Motion_SetHPosTerrain_2(1, 0, 0);
    GameFlag_Set_1(0x303);
    Battle_SchedShoulder_1(); /* main:0808a020 */
}

/* Runs the actor 0 / 1 / 9 setup sequence: position, speed and animation
 * calls in a fixed order. Two steps look up a record for one actor and
 * copy a coordinate pair out of it (offsets +8/+16 as s32, then +10/+18
 * as s16) into the call configuring another actor. */
void Scene_RunPrimaryActorSequence(void)
{
    u32 i;
    s32 record;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(0x1650000, -1, 0x2e20000, 1);
    ObjectMotion_SetPositionAndReset_1(0, 0x16f, 0x2e9);
    ObjectMotion_ArmCallback_1(0, 0xa000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Copy the s32 coordinate pair at +8/+16 of the looked-up record. */
        ObjectMotion_SetHorizontalPositionWithTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetPositionAndReset_2(1, 0x15a, 0x2e9);
    ObjectMotion_ArmCallback_2(1, 0xd000, 20);
    SceneWork_SetStepValue_1(0x1c53);
    BattleEvent_RunActionAndWait_1(1, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(9, 2);
    BattleEffect_SpawnLinkedResourceObject_1(9, 0x100, 0);
    ObjectMotion_ArmCallback_3(9, 0x3000, 10);
    ObjectMotion_ArmCallback_4(9, 0x5000, 10);
    ObjectMotion_ArmCallback_5(9, 0x3000, 40);
    BattleRuntime_RunThenWaitIfModeZero_1(9, 0, 20);
    Object_SetModeById_1(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_1(1, 3);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(9, 1);
    ObjectMotion_ArmCallback_6(9, 0x5000, 10);
    BattleRuntime_RunThenWaitIfModeZero_2(9, 0, 20);
    ObjectMotion_SetVariantCallback_1(1, 1);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x103, 40);
    BattleRuntime_RunThenWaitIfModeZero_3(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_2(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_4(9, 0, 20);
    Motion_ArmCb_7(0, 0x8000, 0);
    Motion_ArmCb_8(1, 0x1000, 40);
    ObjectMotion_CallThenWaitForAnimationChange_3(9, 4);
    BattleEvent_RunActionAndWait_2(9, 0);
    Motion_ArmCb_9(0, 0xb000, 0);
    Motion_ArmCb_10(1, 0xd000, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(1, 2);
    BattleRuntime_WaitIfModeZero_1(20);
    BattleRuntime_RunThenWaitIfModeZero_5(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_4(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_6(9, 0, 20);
    BattleFx_SpawnLinked_3(0, 0x102, 0);
    BattleFx_SpawnLinked_4(1, 0x102, 80);
    BattleRuntime_RunThenWaitIfModeZero_7(1, 0, 20);
    Motion_SetVarCbObj_4(9, 2);
    BattleRuntime_RunThenWaitIfModeZero_8(9, 0, 20);
    Motion_ArmCb_11(1, 0x1000, 20);
    ObjectMotion_SetSpeedParameters_1(1, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        BattleFx_SpawnLinked_5(1, 0x105, 60);
    } else {
        bump_step(1);
    }
    BattleRuntime_RunThenWaitIfModeZero_9(1, 0, 20);
    Motion_ArmCb_12(1, 0xd000, 10);
    SceneWork_SetStepValue_2((s32)gVal3);
    BattleEvent_RunActionAndWait_3(1, 0);
    ObjectMotion_CallThenWaitForAnimationChange_5(9, 3);
    Battle_RunThenWaitIfModeZero_10(9, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_6(1, 3);
    BattleRuntime_WaitIfModeZero_2(20);
    Motion_ArmCb_13(1, 0x1000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_7(1, 3);
    Motion_CallWaitAnim_8(0, 3);
    Object_SetModeById_2(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Copy the s16 coordinate pair at +10/+18 of the looked-up record. */
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(1, 0, 0);
    GameFlag_Set_1(0x304);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

/* Sets up actors 12, 13, 14 and 20 with shared data and movement/speed
 * parameters, then drives actor 11 through a further sequence of moves. */
void Scene_RunCompanionActorSequence(void)
{
    u32 i;
    s32 actor_data;
    s32 shared_data;

    BattleRuntime_Reset_1();
    actor_data = Scene_GetRecord_1(ACTOR_A);
    Actor_unk117_4(actor_data, 0);
    actor_data = Scene_GetRecord_2(ACTOR_B);
    Actor_unk118_4(actor_data, 0);
    actor_data = Scene_GetRecord_3(ACTOR_C);
    Actor_unk119_4(actor_data, 0);
    Object_SetModeById_1(ACTOR_A, 0);
    Object_SetModeById_2(ACTOR_B, 0);
    Object_SetModeById_3(ACTOR_C, 0);
    Actor_unk120_4(20);
    Actor_unk42_3(0x20000, 0x20000, 0x10000);
    shared_data = SHARED_DATA;
    Motion_EnableActCb_1(ACTOR_A, shared_data);
    Actor_unk121_4(10);
    Motion_EnableActCb_2(ACTOR_B, shared_data);
    Actor_unk43_3(-1, -1, 0xe666);
    Actor_unk122_4(20);
    Motion_MarkActiveAndSetActionCallback_1(ACTOR_C, shared_data);
    BattleEffect_SpawnLinkedResourceObject_1(ACTOR_D, 0x100, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_D, 2);
    ObjectMotion_ArmCallback_1(ACTOR_D, 0xd000, 10);
    SceneWork_SetStepValue_1(0x1c90);
    BattleRuntime_RunThenWaitIfModeZero_1(ACTOR_D, 0, 40);
    ObjectMotion_SetAngleToward_1(ACTOR_D, 0, 20);
    BattleEvent_RunActionAndWait_1(ACTOR_D, 0);
    ObjectMotion_ArmCallback_2(ACTOR_D, 0x8000, 10);
    GameFlag_Set_1(0x305);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void Scene_RunLargeStagingSequence(void)
{
    u32 i;
    s32 rec;
    s32 rec3;
    u8 *rec8;
    u8 *record;
    s32 none;
    s32 none2;
    s32 base7_200e590;
    s32 base5_ee8;
    s32 p1;
    s32 k;
    s32 p2;

    rec3 = Actor_unk9(0);
    rec8 = Actor_unk10(14);
    Actor_unk123_4();
    Actor_unk5_4(-1, -1, -1, 0);
    Actor_unk124_4(1);
    rec = 0;
    {
        u8 *slot = (u8 *)(Actor_unk28() + 85);

        none = 0;
        *slot = rec;
    }
    Actor_unk125_4(1);
    Actor_SetRect(49, 53, 8, 4, 20, 50);
    Actor_unk2_5(2, 102, 84, 41, 2, 1);
    Actor_unk3_5(1, 102, 83, 41, 1, 1);
    Actor_unk4_5(0, 103, 82, 42, 1, 1);
    rec = Actor_unk29(11);
    *(u8 *)(rec + 85) = none;
    k = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    *(s32 *)(rec + 8) = k;
    Actor_unk126_4(rec, 0);
    rec = Actor_unk30(12);
    *(u8 *)(rec + 85) = none;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    *(s32 *)(rec + 8) = k;
    Actor_unk127_4(rec, 0);
    rec = Actor_unk31(13);
    *(u8 *)(rec + 85) = none;
    *(s32 *)(rec + 16) = 0x3500000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 8) = k;
    Actor_unk128_4(rec, 0);
    record = Actor_unk48_4(11);
    Actor_unk129_4((s32)record, 0);
    record = Actor_unk49_4(12);
    Actor_unk130_4((s32)record, 0);
    Actor_unk131_3(0, 11);
    base7_200e590 = (s32)gOv2StagedMotion;
    Actor_unk132_3(0, base7_200e590);
    Actor_unk133_3();
    base5_ee8 = (s32)gVal4;
    Actor_unk134_3(base5_ee8, 0, 0);
    Actor_unk135_3();
    Actor_unk6_4(0x1530000, 0xa00000, 0x4950000, 0);
    Actor_unk136_3();
    Actor_unk137_3(1);
    Actor_unk7_4(0x547a, 0xa8f);
    Actor_unk8_4(0x1280000, 0xa00000, 0x3990000, 1);
    Actor_unk44_3(5, 0x1990000, 0x46e0000);
    Actor_unk138_3(1);
    Actor_unk45_3(5, 0xb333, 0x5999);
    Actor_unk46_3(5, 0x1a4, 0x42c);
    Actor_unk139_3();
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c8) = 60;
    Actor_unk140_3();
    Actor_unk141_3(5);
    Actor_unk47_3(5, 0x10000, 0x8000);
    Actor_unk48_3(5, 0x155, 0x428);
    Actor_unk49_3(5, 0xcccc, 0x6666);
    Actor_unk50_3(5, 0x167, 0x409);
    Actor_unk51_3(8, 0x8000, 0x4000);
    Actor_unk52_3(8, 0x13e, 0x3b3);
    Actor_unk142_3(8, 2);
    Actor_unk53_3(5, 0x19c, 0x409);
    Actor_unk54_3(5, 0x19c, 0x3fb);
    Actor_unk55_3(5, 0x176, 0x3f0);
    Actor_unk56_3(5, 0x15b, 0x3bb);
    Actor_unk57_3(8, 0x13e, 0x3b3);
    Actor_unk143_3(5, 8, 40);
    Actor_unk144_3(8, 2);
    Actor_unk145_3(5, 3);
    Actor_unk146_3(10);
    Actor_unk58_3(8, 0x17b, 0x3f9);
    Actor_unk9_4(0x8000, 0x1000);
    Actor_unk59_3(5, 0x14d, 0x398);
    Actor_unk60_3(5, 0x12b, 0x39c);
    Actor_unk147_3();
    Actor_unk148_3(10);
    Actor_unk61_3(5, 0xf000, 30);
    Actor_unk149_3(5, 2);
    Actor_unk150_3(20);
    Actor_unk10_4(0x20000, 0x4000);
    Actor_unk11_4(0x1830000, 0xa00000, 0x3620000, 1);
    Actor_unk151_3();
    Actor_unk152_3(10, 2, 20);
    Actor_unk153_3((base5_ee8 + 1));
    Actor_unk62_3(0x100a, 0, 10);
    *(s32 *)(rec3 + 24) = 0x10000;
    *(s32 *)(rec3 + 28) = 0x10000;
    Actor_unk154_3(0, 1);
    Actor_unk155_2(10, 0, 40);
    Actor_unk12_4(0, 0x102);
    Actor_unk156_2(0, 2);
    Actor_unk157_2(40);
    Actor_unk158_2(10, 2);
    Actor_unk159_2(40);
    Actor_unk63_3(0x100a, 0, 40);
    Actor_unk160_2(0, 11);
    Actor_unk161_2(0, base7_200e590);
    Actor_unk162_2(5, 1);
    Actor_unk163_2();
    Actor_unk164_2(40);
    Actor_unk165_2(5, 3);
    Actor_unk64_3(5, 0xd000, 10);
    Actor_unk65_3(5, 0x138, 0x2f7);
    Actor_unk66_3(5, 0x169, 0x2f8);
    Actor_unk166_2(20);
    Actor_unk67_3(5, 0x8000, 40);
    Actor_unk167_2(5, 0, 40);
    Actor_unk68_3(0x6001, 0, 10);
    Actor_unk69_3(5, 0x100, 0);
    Actor_unk168_2(5, 4, 40);
    Actor_unk70_3(5, 0xc000, 30);
    Actor_unk169_2(5, 2);
    Actor_unk170_2(40);
    Actor_unk171_2(5, 0, 30);
    Actor_unk172_2(5, 3);
    Actor_unk173_2(10);
    Actor_unk13_4(0x40000, 0x8000);
    Actor_unk14_4(0x18c0000, -1, 0x24c0000, 1);
    Actor_unk71_3(5, 0x1c8, 0x2e3);
    Actor_unk174_2();
    Actor_unk175_2(10);
    Actor_unk72_3(1, 0x105, 40);
    Actor_unk176_2(1, 2);
    Actor_unk177_2(20);
    Actor_unk178_2();
    Actor_unk179_2(1, 17);
    Actor_unk73_3(0x2001, 0, 20);
    Actor_unk180_2(131);
    for (i = 0; i < 60; i++) {
        Actor_unk181_2(Actor_unk32(1));
        Actor_unk182_2(1);
    }
    Actor_unk183_2(1, 1);
    p1 = 0x200d5b1;
    Actor_unk15_4(p1, 0xc80);
    p2 = 0x200d5d1;
    Actor_unk11(p2, 0xc80);
    record = Actor_unk50_4(14);
    Actor_unk184_2((s32)record, 0);
    none2 = 0;
    rec8[85] = none2;
    *(s32 *)((s32)rec8 + 8) = 0x1ac0000;
    *(s32 *)((s32)rec8 + 12) = 0xd00000;
    *(s32 *)((s32)rec8 + 16) = 0x2480000;
    {
        s32 shown = 0x8000;

        *(s32 *)((s32)rec8 + 108) = 0x200d75d;
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    Actor_unk185_2(4);
    Actor_unk74_3(14, 0x20000, 0x20000);
    Actor_unk16_4((s32)rec8, 0x1980000, 0xd00000, 0x2480000);
    Actor_unk186_2(40);
    Actor_unk75_3(9, 0x2666, 0x1333);
    Actor_unk76_3(14, 0x2666, 0x1333);
    Actor_unk187_2(9);
    Actor_unk17_4((s32)rec8, 0x1880000, 0xd00000, 0x2480000);
    Actor_unk77_3(9, 0x17a, 0x248);
    Actor_unk188_2(20);
    Actor_unk78_3(0x2005, 0, 10);
    *(volatile s32 *)((s32)rec8 + 108) = none2;
    Actor_unk189_2(1, 2);
    {
        u8 *record = Actor_unk51_4(1);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_unk190_2(p1);
    Actor_unk191_2(p2);
    Actor_unk192_2(1);
    Actor_unk193_2(1, 0);
    Actor_unk194_2(9, 0);
    Actor_unk79_3(1, 0x3000, 0);
    Actor_unk195_2(1, 1);
    Actor_unk196_2((s32)rec8);
    Actor_unk197_2();
    Actor_unk198_2(10);
    Actor_unk80_3(5, 0x1a8, 0x270);
    Actor_unk199_2(1, 5, 60);
    Actor_unk200_2(1, 2);
    Actor_unk201_2(10);
    Actor_unk81_3(0x6001, 0, 20);
    Actor_unk202_2(5, 3);
    Actor_unk203_2(10);
    Actor_unk204_2(5, 0, 20);
    Actor_unk82_3(1, 0x102, 40);
    Actor_unk205_2(5, 4);
    Actor_unk206_2(10);
    Actor_unk207_2(5, 0, 20);
    Actor_unk83_3(1, 0x101, 80);
    Actor_unk208_2(5, 1);
    Actor_unk209_2(10);
    Actor_unk210_2(5, 0, 20);
    Actor_unk84_3(1, 0x102, 80);
    Actor_unk211_2(5, 4);
    Actor_unk212_2(10);
    Actor_unk213_2(5, 0, 20);
    Actor_unk214_2(1, 3);
    Actor_unk215_2(10);
    Actor_unk85_3(5, 0x184, 0x25c);
    Actor_unk86_3(5, 0xa000, 20);
    Actor_unk216_2(5, 2);
    Actor_unk217_2(10);
    Actor_unk87_3(0x1005, 0, 20);
    Actor_unk218_2(5, 4);
    Actor_unk219_2(10);
    Actor_unk88_3(1, 0x105, 40);
    Actor_unk220_2(1, 2);
    Actor_unk221_2(10);
    Actor_unk222_2(5, 0, 0);
    Actor_unk223_2(1, 0x8000, 40);
    Actor_unk5_5(1, 1, 2, 25, 2, 5, 10, 14, 4, 14, none2);
    Actor_unk224_2(40);
    Actor_unk18_4(5, 0x102);
    Actor_unk19_4(1, 0x102);
    Actor_unk225_2(80);
    Actor_unk89_3(5, 0x101, 40);
    Actor_unk90_3(0x1005, 0, 20);
    Actor_unk20_4(1, 0x102);
    Actor_unk226_2(80);
    Actor_unk227_2(1, 4);
    Actor_unk228_2(10);
    Actor_unk91_3(5, 0x101, 40);
    Actor_unk92_3(0x1005, 0, 20);
    Actor_unk229_2(1, 2);
    Actor_unk230_2(40);
    Actor_unk231_2(1, 4);
    Actor_unk232_2(20);
    Actor_unk93_3(5, 0x101, 80);
    Actor_unk233_2(5, 4);
    Actor_unk234_2(10);
    Actor_unk94_3(0x1005, 0, 10);
    Actor_unk95_3(5, 0x1000, 40);
    Actor_unk235_2(5, 1, 40);
    Actor_unk236_2(1, 2);
    Actor_unk237_2(10);
    Actor_unk96_3(5, 0x17c, 0x26c);
    Actor_unk238_2(10);
    Actor_unk97_3(0x1005, 0, 10);
    Actor_unk98_3(1, 0x5000, 30);
    Actor_unk21_4(1, 0x102);
    Actor_unk239_2(80);
    Actor_unk240_2(1, 2);
    Actor_unk241_2(10);
    Actor_unk99_3(0x6001, 0, 20);
    Actor_unk242_2(5, 4);
    Actor_unk243_2(10);
    Actor_unk100_3(1, 0x101, 80);
    Actor_unk244_2(5, 4);
    Actor_unk245_2(10);
    Actor_unk101_3(1, 0xcccc, 0x6666);
    Actor_unk102_3(1, 0x19c, 0x25c);
    Actor_unk103_3(1, 0x5000, 20);
    Actor_unk246_2(5, 1);
    Actor_unk247_2(10);
    Actor_unk104_3(0x1005, 0, 20);
    Actor_unk105_3(1, 0x101, 80);
    Actor_unk248_2(5, 4, 30);
    Actor_unk106_3(0x1005, 0, 20);
    Actor_unk249_2(1, 3);
    Actor_unk250_2(10);
    Actor_unk107_3(0x6001, 0, 20);
    Actor_unk251_2(30);
    Actor_unk108_3(5, 0xe000, 40);
    Actor_unk252_2(5, 3);
    Actor_unk109_3(0x1005, 0, 20);
    Actor_unk110_3(1, 0x101, 80);
    Actor_unk111_3(5, 0x103, 40);
    Actor_unk112_3(5, 0xcccc, 0x6666);
    Actor_unk113_3(5, 0x1ac, 0x274);
    Actor_unk253_2(20);
    Actor_unk114_3(1, 0x3000, 0);
    Actor_unk254_2(5);
    Actor_unk22_4(0x5001, 0);
    Actor_unk255_2(5, 1);
    Actor_unk256_2(60);
    Actor_unk115_3(5, 0xb000, 30);
    Actor_unk257_2(1, 2);
    Actor_unk116_3(5, 0x1ac, 0x274);
    Actor_unk117_3(5, 0xb000, 20);
    Actor_unk258_2(1, 3);
    Actor_unk259_2(10);
    Actor_unk260_2(5, 4);
    Actor_unk261_2(10);
    Actor_unk118_3(0x2005, 0, 20);
    Actor_unk23_4(1, 0x102);
    Actor_unk262_2(40);
    Actor_unk119_3(0x5001, 0, 20);
    Actor_unk263_2(1, 3);
    Actor_unk264_2(5, 3);
    Actor_unk265_2(10);
    Actor_unk120_3(5, 0x8000, 0x4000);
    Actor_unk121_3(1, 0x8000, 0x4000);
    Actor_unk122_3(5, 0x1c2, 0x2ee);
    Actor_unk123_3(1, 0x1c2, 0x2ee);
    Actor_unk266_2(60);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 60;
    Actor_unk267_2();
    Actor_unk268_2();
    Actor_unk269_2(12);
    Actor_unk270_2();
}

void Scene_RunStep8C(void)
{
    Actor_Apply8(0x8c, 0);
}

void Scene_RunStep9(void)
{
    Actor_unk24_2(9);
    Actor_unk271_2();
}

void Scene_RunStep17(void)
{
    Actor_unk25_2(0x17);
    Actor_unk272_2();
}

void Scene_RunScriptedStepEE4(void)
{
    Actor_unk273();
    Actor_Apply9((s32)&Value_00000ee4, 1);
    Actor_unk274();
}

void Scene_RunScene373SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Actor_unk12(0x241) != 0) {
        rec7 = Actor_unk13(0x106);
        if (rec7 != 0) {
            goto L_02005a8a;
        }
        *(u8 *)(Actor_unk33(22) + 91) = rec7;
        Actor_unk11_2(0x241);
    } else {
        if (Actor_unk14(0x106) != 0) {
            *(u8 *)(Actor_unk34(22) + 91) = 1;
            Actor_unk12_2(0x241);
        }
    }
    L_02005a8a:;
}

/* overlays/scene/actor/staged_motion/geometry.c */
typedef s32(*FixedPointSqrt)(s32);

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((FixedPointSqrt) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

/* overlays/scene/actor/staged_motion/move_and_redraw.c */

void Actor_MoveAndRedraw(StagedActorMovementRequest request)
{
}

/* overlays/scene/actor/staged_motion/orbiting_effect.c */
#define UpdateOrbitingSceneObject Value5

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 Value5;

OrbitingSceneObject *GetOrbitingSceneObject();


s32 Effect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Actor_unk35(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Actor_unk36(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Actor_unk37(p[12] + 0x8000) / 8;
    a = Actor_unk38();
    b = Actor_unk39();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}

/* overlays/scene/actor/staged_motion/overlay_object.c */
extern s32 gIw;

void OvObj_UpdateOnFrameBit1(s32 p)
{
    if ((gIw & 2) != 0)
        Actor_Apply10(p, 7);
    else
        Actor_Apply11(p, 0);
    if ((gIw & 0xf) == 0)
        Actor_unk26_2(p);
}

/* overlays/scene/actor/staged_motion/scene_dialogue.c */

void Dialogue_RunActor181Scene(void)
{
    Actor_unk276();
    Actor_unk262(26, 0, 0);
    Actor_unk27_2(0xfd0);
    Actor_Apply12(0xb5, 3);
    Actor_Apply13(0xb5, 0);
    Actor_unk277();
}

void Dialogue_RunActorTenFlaggedDialogue(void)
{
    Actor_unk278();
    if (Actor_unk40(0x815) != 0) {
        Actor_unk28_2(0x11cc);
        Actor_Apply14(10, 0);
    } else {
        Actor_unk29_2(0xf81);
        Actor_unk263(10, 0, 4);
        Actor_Apply15(10, 0);
    }
    Actor_unk279();
}

void Dialogue_RunActor21FlaggedLine(void)
{
    Actor_unk280();
    if (Actor_unk41(0x202) != 0) {
        Actor_unk30_2(0xf68);
    } else {
        Actor_unk31_2(0xf69);
    }
    Actor_Apply16(21, 0);
    Actor_unk281();
}

void Dialogue_RunActor10LineAndFlag81f(void)
{
    Actor_unk282();
    Actor_unk264(0, 10, 20);
    Actor_unk32_2(0x1c8d);
    Actor_Apply17(10, 0);
    Actor_unk33_2(0x81f);
    Actor_unk283();
}

void Dialogue_RunActorNineteenDialogue(void)
{
    Actor_unk284();
    Actor_Apply18(19, 2);
    Actor_unk34_2(20);
    Actor_unk265(19, 0, 20);
    Actor_unk35_2(0x1c9d);
    Actor_Apply19(19, 0);
    Actor_unk36_2(0x307);
    Actor_unk285();
}

/* overlays/scene/actor/staged_motion/scene_primary_script.c */
extern s32 gOv3StagedMotion[];

void Dialogue_RunFlag815GatedStep(void)
{
    if (Actor_unk42(0x815) != 0 && Actor_unk43(0x87a) == 0) {
        Actor_unk286();
        Actor_unk37_2(0x11b6);
        Actor_Apply20(21, 0);
        if (Actor_Apply21(0, 0) == 0) {
            Actor_unk266(21, 0, 60);
            Actor_Apply22(21, 0);
        } else {
            u8 *b = *(u8 **)0x03001ebc;
            u16 *h = (u16 *)(b + 0x1d8);
            *h = *h + 2;
            Actor_unk38_2(40);
            Actor_Apply23(21, 0);
        }
        Actor_unk287();
    } else {
        Actor_unk39_2(0x9e);
        Actor_unk267((s32)gOv3StagedMotion, 50, 44);
        Actor_unk268(0, 0x154, 0x378);
        Actor_unk40_2(7);
    }
}

/* overlays/scene/actor/staged_motion/scene_setup.c */

void Scene_RunActor181Scene(void)
{
    Actor_unk288();
    Actor_unk269(20, 0, 0);
    Actor_unk41_2(0xfd0);
    Actor_Apply24(0xb5, 3);
    Actor_Apply25(0xb5, 0);
    Actor_unk289();
}

/* overlays/scene/actor/staged_motion/scene_state_interaction.c */
extern s32 gOv4StagedMotion[];
extern s32 gOv5[];
extern s32 gOv6[];

u8 *Actor_unk290(s32);

void State_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Actor_unk291();
    Actor_SetMode2(0, 13, 10, 1);
    Actor_unk42_2(0x204);
    p5 = 20;
    p6 = 50;
    Actor_unk14_5(49, 46, 8, 4, p5, p6);
    Actor_unk292();
}

void State_RunTablePairByActor22State(void)
{
    u8 *rec = Actor_unk290(22);
    if (Actor_unk44(0x823) != 0) {
        u8 *q = rec;
        s32 v;
        q += 100;
        v = *(s16 *)q;
        if (v == 1) {
            Actor_Apply26((s32)gOv4StagedMotion, (s32)gOv5);
        } else if (v == 2) {
            Actor_Apply27((s32)gOv4StagedMotion, (s32)gOv6);
        }
    }
}

/* overlays/scene/actor/staged_motion/shared.c */

void Scene_RunSingleStep(void)
{
    Actor_unk293();
}

/* overlays/scene/actor/staged_motion/staged_actor.c */
/*
 * Reset or tear down a consecutive run of scene actors -- resource_373.
 */
struct Resource373Actor {
    u8 unknown_00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 unknown_14[0x41];
    u8 flag55;
};

struct Resource373Actor *Actor_unk294(s32 selector);

/*
 * The owner extends through its one literal pool word. The run length is
 * unsigned and the selector advances once per iteration. The Func_ aliases
 * name the call words encoded in the overlay image, not verified runtime
 * addresses.
 */
void Actor_ResetActorRun(s32 first, u32 count, s32 mode)
{
    s32 selector = first;
    u32 i;

    if (mode == 0) {
        for (i = 0; i < count; i++) {
            struct Resource373Actor *actor = Actor_unk294(selector);

            actor->flag55 = 0;
            Actor_Apply28(actor, 0);
            actor->field08 = 0x01860000;   /* 0xc3 << 17. */
            actor->field0c = 0x00a00000;   /* 0xa0 << 16. */
            actor->field10 = 0x034a0000;   /* The literal pool word. */
            selector++;
        }
        return;
    }

    for (i = 0; i < count; i++) {
        Actor_unk270(selector, 0, 0);
        selector++;
    }
}

/* overlays/scene/actor/staged_motion/staged_actor_scene_tail.c */

struct SourceEntity {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct ParticleRecord {
    u8 f00[9];
    u8 f09;
};

struct StagedParticle {
    u8 f00[0x48];
    s32 f48;
    s32 f4c;
    struct ParticleRecord *f50;
    u8 f54;
    u8 f55;
    u8 f56[8];
    u16 f5e;
};

/* Emitter fields are named by offset; the layout is not verified. */
struct Resource373Emitter {
    u8 unknown_00[6];
    u16 field06;
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[4];
    s32 field18;
    s32 field1c;
    u8 unknown_20[4];
    s32 field24;
    s32 field28;
    s32 field2c;
    u8 unknown_30[8];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Resource373Particle {
    u8 unknown_00[0x28];
    s32 lifetime;                   /* 0x28 */
    u8 unknown_2c[4];
    s32 field30;
    s32 field34;
    u8 unknown_38[0x10];
    s32 field48;
    u8 unknown_4c[9];
    u8 field55;
};

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

extern s32 gOv7[];

u8 *Actor_unk295();

u8 *Actor_unk296();

u8 *Actor_unk297();

u8 *Actor_unk298();

u8 *Actor_unk299();

u8 *Actor_unk300();

u8 *Actor_unk301();

u8 *Actor_unk302();

u8 *Actor_unk303();

u8 *Actor_unk304();

u8 *Actor_unk305();

u8 *Actor_unk306();
u8 *Actor_unk307();

u8 *Actor_unk308();

u8 *Actor_unk309();
u8 *Actor_unk310();

u8 *Actor_unk311();

u8 *Actor_unk312();
u8 *Actor_unk313();

u8 *Actor_unk314();

u8 *Actor_unk315();

u8 *Actor_unk316();

struct StagedParticle *Actor_unk317(s32, s32, s32, s32);

struct Resource373Particle *Actor_unk318();

/*
 * Staged actor cutscene tail -- resource_373. The long scene script plus the
 * particle, sound, and vertical-motion effects it drives.
 */

/*
 * Each alias names the loader-relocated call word the image holds, not a
 * runtime address. The declarations are old-style because the call sites vary
 * in arity.
 */

/*
 * Calls spelled through these wrappers pass their constants straight into the
 * argument registers. A direct call instead precomputes a costly constant into
 * a pseudo shared with later uses in the block, and a value-returning call
 * sets r0 last of its arguments.
 */

void Scene_RunStagedActorCutsceneSequence(void)
{
    u8 *scene;
    u8 *rec;
    u32 i;
    s32 turn_back;
    s32 turn_side;
    u8 *turned;
    s32 none;
    s32 flag;
    s32 callback_a;
    s32 callback_b;
    s32 callback_c;
    s32 callback_d;
    s32 callback_e;
    s32 callback_f;

    scene = Actor_unk295(0);
    Actor_unk319();
    Actor_unk24_4(-1, -1, -1, 0);
    Actor_unk320(1);
    Actor_unk6_5(49, 53, 8, 4, 20, 50);
    Actor_unk321(2, 102, 84, 41, 2, 1);
    Actor_unk322(1, 102, 83, 41, 1, 1);
    Actor_unk323(0, 103, 82, 42, 1, 1);
    Actor_unk124_3(21, 0x1880000, 0x3800000);
    turned = Actor_unk296(21);
    /*
     * Overwritten at once, but the store must stay: its zero halfword
     * temporary is what the record byte stores below reuse out of a high
     * register.
     */
    *(u16 *)(turned + 6) = 0;
    turn_back = 0xc000;
    *(u16 *)(turned + 6) = turn_back;
    Actor_unk125_3(1, 0x12a0000, 0x2e00000);
    turned = Actor_unk297(1);
    turn_side = 0x4000;
    *(u16 *)(turned + 6) = turn_side;
    Actor_unk126_3(5, 0x12a0000, 0x2f80000);
    turned = Actor_unk298(5);
    *(u16 *)(turned + 6) = turn_side;
    Actor_unk324(0, 11);
    Actor_unk325(0, 0x200e590);
    rec = Actor_unk299(23);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    Actor_unk326(rec, 0);
    rec = Actor_unk300(24);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    Actor_unk327(rec, 0);
    rec = Actor_unk301(25);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3500000;
    Actor_unk328(rec, 0);
    Actor_unk302()[85] = 0;
    Actor_unk329(1);
    Actor_unk25_4(0x17f0000, 0xa00000, 0x36d0000, 0);
    Actor_unk330();
    Actor_unk331(1);
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c8) = 32;
    Actor_unk332();
    Actor_unk127_3(5, 0x8000, turn_side);
    Actor_unk128_3(1, 0x8000, turn_side);
    Actor_unk333(5, 0x200e614);
    Actor_unk334(1, 0x200e5cc);
    Actor_unk335(40);
    Actor_unk336(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_unk26_4(0, 0xb000, 40, 0x10000);
    Actor_unk337(0, 3);
    Actor_unk338(10);
    Actor_unk129_3(0, 0x4ccc, 0x2666);
    Actor_unk130_3(0, 400, 840);
    Actor_unk339(10);
    Actor_unk340(0, turn_back, 30);
    Actor_unk341(0, 1);
    Actor_unk342(20);
    Actor_unk131_2(0, 0x8000, 40);
    Actor_unk343();
    Actor_unk344(0, 17);
    Actor_unk345((s32)Actor_unk346, 3200);
    for (i = 0; i < 40; i++) {
        Actor_unk347(scene);
        Actor_unk348(1);
    }
    Actor_unk349(0, 1);
    /* Callback symbols are pooled loads that stay after the preceding call. */
    callback_a = (s32)Actor_unk350;
    Actor_unk15(callback_a, 3200);
    callback_b = (s32)Actor_unk351;
    Actor_unk27_4(callback_b, 3200);
    Actor_unk132_2(23, 0x3333, 0x1999);
    Actor_unk133_2(23, 390, 832);
    Actor_unk134_2(0, 0xc000, 0);
    Actor_unk135_2(23, 400, 826);
    Actor_unk352(20);
    {
        u8 *record = Actor_unk303(0);
        u8 value = *(volatile u8 *)&record[35]; /* Keeps the byte in its own register. */

        record[35] = (u8)(value | 1);
    }
    Actor_unk353(0, 1);
    Actor_unk354((s32)Actor_unk346);
    Actor_unk355(callback_a);
    Actor_unk356(callback_b);
    Actor_unk357(1);
    Actor_unk358(0, 0);
    Actor_unk359(23, 0);
    Actor_unk360(23, 0, 0);
    Actor_unk361(20);
    Actor_unk362(0, 11);
    Actor_unk363(0, 0x200e590);
    Actor_unk364(120);
    Actor_unk7_5(7, 102, 84, 41, 2, 1);
    Actor_unk365(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_unk366(0, 1);
    Actor_unk367(40);
    Actor_unk368(0, 3);
    Actor_unk136_2(0, 377, 843);
    Actor_unk137_2(0, 0xc000, 40);
    Actor_unk369(0, 0, 20);
    Actor_unk370(0, 17);
    Actor_unk28_4((s32)Actor_unk346, 3200);
    for (i = 0; i < 40; i++) {
        Actor_unk371(scene);
        Actor_unk372(1);
    }
    Actor_unk373(0, 1);
    callback_c = (s32)Actor_unk350;
    Actor_unk16(callback_c, 3200);
    callback_d = (s32)Actor_unk374;
    Actor_unk29_4(callback_d, 3200);
    Actor_unk138_2(24, 0x3333, 0x1999);
    Actor_unk139_2(24, 390, 832);
    Actor_unk140_2(0, 0xc000, 0);
    Actor_unk141_2(24, 377, 828);
    Actor_unk375(20);
    {
        u8 *record = Actor_unk304(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_unk376(0, 1);
    Actor_unk377((s32)Actor_unk346);
    Actor_unk378(callback_c);
    Actor_unk379(callback_d);
    Actor_unk380(1);
    Actor_unk381(0, 0);
    Actor_unk382(24, 0);
    Actor_unk383(24, 0, 0);
    Actor_unk384(20);
    Actor_unk385(0, 11);
    Actor_unk386(0, 0x200e590);
    Actor_unk387(120);
    Actor_unk8_5(6, 102, 83, 41, 1, 1);
    Actor_unk388(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_unk389(0, 1);
    Actor_unk390(40);
    Actor_unk391(0, 3);
    Actor_unk142_2(0, 360, 855);
    Actor_unk143_2(21, 0xb000, 10);
    Actor_unk144_2(0, 0xc000, 30);
    Actor_unk145_2(0, 0xd000, 20);
    Actor_unk392(0, 17);
    Actor_unk30_4((s32)Actor_unk346, 3200);
    for (i = 0; i < 40; i++) {
        Actor_unk393(scene);
        Actor_unk394(1);
    }
    Actor_unk395(0, 1);
    callback_e = (s32)Actor_unk350;
    Actor_unk17(callback_e, 3200);
    callback_f = (s32)Actor_unk396;
    Actor_unk18(callback_f, 3200);
    Actor_unk146_2(25, 0x3333, 0x1999);
    Actor_unk147_2(25, 390, 832);
    Actor_unk148_2(0, 0xc000, 0);
    Actor_unk149_2(25, 360, 837);
    Actor_unk397(20);
    {
        u8 *record = Actor_unk305(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_unk398(0, 1);
    Actor_unk399((s32)Actor_unk346);
    Actor_unk400(callback_e);
    Actor_unk401(callback_f);
    Actor_unk402(1);
    Actor_unk403(0, 0);
    Actor_unk404(25, 0);
    Actor_unk405(25, 0, 0);
    Actor_unk406(20);
    Actor_unk407(0, 11);
    Actor_unk408(0, 0x200e590);
    Actor_unk409(120);
    Actor_unk410();
    Actor_unk9_5(5, 103, 82, 42, 1, 1);
    Actor_unk411(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_unk412(21, 2, 20);
    Actor_unk13_2(0xf03);
    Actor_unk413(21, 0, 10);
    Actor_unk150_2(0, 0x1000, 10);
    Actor_unk414(21, 5, 6, 0);
    Actor_unk151_2(21, 0x4ccc, 0x2666);
    Actor_unk152_2(21, 397, 832);
    Actor_unk415(20);
    Actor_unk153_2(21, 0x4000, 60);
    Actor_unk154_2(21, 0xc000, 60);
    Actor_unk416(21, 3);
    Actor_unk417(21, 3);
    Actor_unk418(21, 0, 10);
    Actor_unk155(21, 372, 832);
    Actor_unk419(20);
    Actor_unk156(21, 0x4000, 40);
    Actor_unk157(21, 0x8000, 40);
    Actor_unk420(21, 3);
    Actor_unk421(21, 3);
    Actor_unk422(21, 0, 20);
    Actor_unk158(21, 0x5000, 20);
    Actor_unk423(21, 3);
    Actor_unk424(21, 0, 10);
    Actor_unk425(0, 2);
    Actor_unk426(20);
    Actor_unk427(21, 4);
    Actor_unk19(21, 0);
    if (Actor_unk45(0, 0) == 0) {
        Actor_unk428(21, 3);
        bump_step(1);
    } else {
        Actor_unk429(21, 4);
    }
    Actor_unk430(21, 0, 20);
    Actor_unk14_2(0xf0a);
    Actor_unk159(21, 386, 841);
    Actor_unk431(10);
    Actor_unk160(21, 0xd000, 60);
    Actor_unk432(21, 2);
    Actor_unk433(21, 0, 20);
    Actor_unk161(21, 0x5000, 30);
    Actor_unk20(21, 0);
    if (Actor_unk46(0, 0) == 1) {
        bump_step(1);
    }
    Actor_unk434(21, 0, 20);
    Actor_unk162(21, 0xd000, 60);
    Actor_unk435(21, 2);
    Actor_unk15_2(0xf0e);
    Actor_unk436(21, 0, 20);
    Actor_unk163(21, 386, 825);
    Actor_unk437(10);
    Actor_unk438(21, 4);
    Actor_unk439(60);
    Actor_unk440(21, 0, 60);
    Actor_unk164(21, 0x5000, 10);
    Actor_unk441(21, 0, 10);
    Actor_unk165(21, 372, 832);
    Actor_unk166(21, 0x5000, 10);
    Actor_unk442(0, 2);
    Actor_unk443(20);
    Actor_unk444(21, 3);
    Actor_unk445(20);
    Actor_unk446(21, 0, 20);
    Actor_unk167(0, 258, 60);
    Actor_unk447(21, 2);
    Actor_unk448(21, 0, 20);
    Actor_unk449(0, 3);
    Actor_unk450(21, 3);
    Actor_unk451(21, 0, 10);
    Actor_unk31_4(0x6666, 0xccc);
    Actor_unk32_4(0x1790000, 0xa00000, 0x35c0000, 1);
    Actor_unk168(5, 0x10000, 0x8000);
    Actor_unk169(1, 0x10000, 0x8000);
    Actor_unk170(1, 369, 904);
    Actor_unk171(5, 392, 904);
    Actor_unk452(1, 1);
    Actor_unk453(5, 10, 11, 0);
    Actor_unk172(5, 0xa000, 0);
    Actor_unk454(5, 0, 10);
    Actor_unk455(21, 2);
    Actor_unk456(10);
    Actor_unk173(21, 0x3000, 0);
    Actor_unk174(0, 0x1000, 20);
    Actor_unk457(5, 4, 0);
    Actor_unk175(5, 392, 843);
    Actor_unk176(5, 0x9000, 0);
    Actor_unk177(21, 0x3000, 0);
    Actor_unk178(0, 0xd000, 20);
    Actor_unk33_4(21, 3);
    Actor_unk458(21, 0, 10);
    Actor_unk459(5, 3);
    Actor_unk460(5, 0, 10);
    Actor_unk461(21, 3);
    Actor_unk462(0, 3);
    Actor_unk463(21, 0, 20);
    Actor_unk464(1, 10, 11, 0);
    Actor_unk179(5, 0x4ccc, 0x2666);
    Actor_unk180(1, 0x4ccc, 0x2666);
    Actor_unk181(1, 392, 843);
    Actor_unk307(5)[90] &= 0xfe;
    Actor_unk182(5, 408, 843);
    Actor_unk465(1);
    {
        u8 *record = Actor_unk308(5);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Actor_unk183(5, 0x8000, 0);
    Actor_unk466(1);
    Actor_unk467(1, 1);
    Actor_unk184(1, 0x8000, 30);
    Actor_unk468(21, 4, 30);
    Actor_unk469(21, 0, 20);
    Actor_unk470(1, 3);
    Actor_unk471(20);
    Actor_unk185(21, 0x5000, 20);
    Actor_unk472(21, 0, 10);
    Actor_unk186(0, 0xd000, 0);
    Actor_unk473(0, 2, 30);
    Actor_unk34_4(0, 258);
    Actor_unk474(60);
    Actor_unk187(21, 0x3000, 40);
    Actor_unk475(1, 2);
    Actor_unk476(1, 0, 20);
    Actor_unk188(21, 257, 80);
    Actor_unk189(21, 0x5000, 30);
    Actor_unk190(0, 258, 60);
    Actor_unk191(21, 0x3000, 0);
    Actor_unk192(0, 0xd000, 20);
    Actor_unk477(21, 0, 10);
    Actor_unk478(1, 3);
    Actor_unk479(80);
    Actor_unk480(5, 1, 30);
    Actor_unk481(1, 2);
    Actor_unk482(5, 2);
    Actor_unk483(10);
    Actor_unk193(21, 261, 60);
    Actor_unk484(21, 0, 10);
    Actor_unk194(5, 0x8000, 0);
    Actor_unk195(1, 0x8000, 10);
    Actor_unk485(5, 2);
    Actor_unk486(20);
    Actor_unk487(5, 0, 10);
    Actor_unk488(21, 2);
    Actor_unk489(40);
    Actor_unk490(21, 0, 10);
    Actor_unk491(1, 3);
    Actor_unk492(5, 3);
    Actor_unk493(20);
    Actor_unk494(21, 4);
    Actor_unk495(21, 0, 10);
    Actor_unk496(1, 3);
    Actor_unk497(5, 3);
    Actor_unk498(21, 3);
    Actor_unk499(21, 0, 10);
    Actor_unk500(1, 3);
    Actor_unk501(5, 3);
    Actor_unk502(20);
    Actor_unk503(21, 2);
    Actor_unk504(21, 0, 20);
    Actor_unk35_4(0x9999, 0x1333);
    Actor_unk36_4(0x1750000, 0xa00000, 0x3450000, 1);
    Actor_unk196(21, 364, 816);
    Actor_unk197(0, 0xd000, 0);
    Actor_unk198(21, 0x3000, 10);
    Actor_unk505(21, 0, 40);
    Actor_unk506(5, 1, 30);
    Actor_unk507(1, 2);
    Actor_unk508(5, 2);
    Actor_unk509(20);
    Actor_unk199(1, 0x8000, 0);
    Actor_unk200(5, 0x8000, 20);
    Actor_unk201(21, 0x5000, 20);
    Actor_unk510(21, 0, 10);
    Actor_unk202(0, 261, 60);
    Actor_unk511(21, 4);
    Actor_unk21(21, 0);
    none = 0; /* One zero shared by the placement call and the byte store. */
    if (Actor_unk47(0, 0) == 1) {
        bump_step(1);
    }
    Actor_unk512(21, 0, 20);
    Actor_unk16_2(0xf27);
    Actor_unk203(21, 259, 0);
    Actor_unk513(21, 3);
    Actor_unk514(21, 0, 10);
    Actor_unk515(21, 4, 0);
    Actor_unk516(21, 3);
    Actor_unk517(21, 7);
    Actor_unk518(5);
    Actor_unk10_5(21, 14, 2, 24, 2, 1, 10, 14, 4, 14, none);
    Actor_unk519(161);
    rec = Actor_unk310(21);
    {
        u8 value = *(volatile u8 *)&rec[90];

        *(u8 *)(*(s32 *)(rec + 80) + 38) = none;
        rec[90] = (u8)(value & 0xfe);
    }
    Actor_unk204(21, 0x30000, 0x18000);
    Actor_unk205(21, 364, 815);
    Actor_unk520(4);
    for (i = 0; i != 4; i++) {
        *(s32 *)(rec + 16) += 0x18000;
        *(s32 *)(rec + 28) += -0x1999;
        Actor_unk521(1);
    }
    Actor_unk522(21, 0, 0);
    Actor_unk206(1, 0x30000, 0x18000);
    Actor_unk523(1, 6, 0);
    Actor_unk207(1, 374, 827);
    Actor_unk524(5, 0);
    Actor_unk208(1, 0xb000, 0);
    Actor_unk209(5, 256, 0);
    Actor_unk525(5, 2);
    Actor_unk210(1, 256, 10);
    Actor_unk526(1, 13);
    Actor_unk527(1, 2, 5);
    Actor_unk528(143);
    Actor_unk211(0, 0x40000, 0x10000);
    Actor_unk11_5(1, 102, 83, 41, 1, 1);
    Actor_unk529(Actor_unk48(1), 0);
    Actor_unk212(0, 0xd000, 10);
    Actor_unk530(1, 3);
    Actor_unk213(-1, -1, 0xe666);
    Actor_unk531();
    Actor_unk214(1, 258, 80);
    Actor_unk532(21, 8);
    *(s32 *)(rec + 28) = 0x8000;
    Actor_unk215(21, 0x16c0000, 0x32b0000);
    for (i = 0; i != 5; i++) {
        *(s32 *)(rec + 28) += 0x1999;
        Actor_unk533(1);
    }
    none = 0; /* Refreshed after the loops for the closing scene store. */
    Actor_unk534(60);
    Actor_unk535(1, 2);
    Actor_unk216(1, 0x5000, 30);
    Actor_unk536(1, 2);
    Actor_unk537(5, 2);
    Actor_unk538(60);
    Actor_unk539(21, 2);
    Actor_unk540(20);
    Actor_unk541(21, 0, 20);
    Actor_unk37_4(0x4ccc, 0x999);
    Actor_unk38_4(0x1740000, 0xa00000, 0x35b0000, 1);
    Actor_unk217(21, 0x30000, 0x18000);
    Actor_unk542(21, 6, 0);
    Actor_unk218(21, 359, 835);
    Actor_unk543(20);
    Actor_unk219(21, 0x4000, 20);
    Actor_unk544(21, 2);
    rec[35] &= 0xfe;
    Actor_unk545(21, 0, 80);
    Actor_unk220(21, 257, 80);
    Actor_unk546(21, 0, 60);
    Actor_unk547(21, 3);
    Actor_unk548(21, 0, 10);
    Actor_unk39_4(21, 258);
    Actor_unk549(80);
    Actor_unk221(1, 0x5000, 40);
    Actor_unk222(1, 258, 80);
    Actor_unk550(1, 2);
    Actor_unk551(1, 0, 20);
    Actor_unk223(1, 0x8000, 20);
    Actor_unk552(1, 3);
    Actor_unk553(10);
    Actor_unk554(1, 3);
    Actor_unk555(Actor_unk49(1), 1);
    Actor_unk556(1, 6, 0);
    Actor_unk557(1, 1);
    Actor_unk224(1, 0x40000, 0x20000);
    rec = Actor_unk311(1);
    rec[90] &= 0xfe;
    Actor_unk225(1, 403, 827);
    Actor_unk40_4(5, 258);
    Actor_unk226(5, 0xc000, 20);
    Actor_unk558(5, 0, 1);
    Actor_unk559(1);
    Actor_unk227(1, 0x5000, 20);
    Actor_unk560(1, 0, 20);
    Actor_unk228(1, 256, 0);
    Actor_unk561(1, 13);
    Actor_unk562(1, 2, 5);
    Actor_unk563(Actor_unk50(1), 0);
    Actor_unk12_5(2, 102, 84, 41, 2, 1);
    Actor_unk564(143);
    Actor_unk229(0, 0x40000, 0x10000);
    Actor_unk565(1, 3);
    Actor_unk230(-1, -1, 0xe666);
    Actor_unk566();
    Actor_unk231(1, 258, 30);
    Actor_unk232(5, 0x4ccc, 0x2666);
    Actor_unk233(5, 408, 855);
    Actor_unk567(60);
    Actor_unk568(1, 2);
    Actor_unk234(21, 261, 60);
    Actor_unk569(5, 3);
    Actor_unk570(0, 3);
    Actor_unk571(80);
    Actor_unk235(1, 0x4000, 30);
    Actor_unk572(1, 3);
    Actor_unk573(10);
    Actor_unk574(5, 4);
    Actor_unk575(80);
    Actor_unk576(21, 3);
    Actor_unk577(10);
    Actor_unk578(21, 0, 10);
    Actor_unk236(5, 0xb000, 0);
    Actor_unk237(0, 0xc000, 0);
    Actor_unk579(21, 0, 60);
    Actor_unk238(21, 0x4000, 60);
    Actor_unk580(21, 4);
    Actor_unk581(60);
    Actor_unk582(21, 0, 80);
    Actor_unk239(21, 261, 80);
    Actor_unk583(21, 0, 60);
    Actor_unk240(0, 0xc000, 0);
    Actor_unk241(0, 257, 0);
    Actor_unk242(5, 257, 0);
    Actor_unk243(1, 257, 60);
    Actor_unk584(21, 4);
    Actor_unk585(30);
    Actor_unk586(21, 0, 60);
    Actor_unk587(1, 2);
    Actor_unk588(5, 2);
    Actor_unk589(20);
    Actor_unk244(5, 0x8000, 60);
    Actor_unk245(21, 0x4000, 30);
    Actor_unk590(21, 0, 30);
    Actor_unk591(5, 3);
    Actor_unk592(10);
    Actor_unk593(5, 0, 20);
    Actor_unk594(21, 0, 30);
    Actor_unk595(21, 4);
    Actor_unk596(20);
    Actor_unk597(21, 0, 20);
    Actor_unk598(5, 3);
    Actor_unk599(20);
    Actor_unk600(1, 3);
    Actor_unk601(10);
    Actor_unk602(1, 0, 20);
    Actor_unk246(0, 0xd000, 0);
    Actor_unk247(21, 256, 0);
    Actor_unk603(21, 3);
    Actor_unk604(30);
    Actor_unk605(21, 0, 60);
    Actor_unk606(1, 3);
    Actor_unk248(1, 0x10000, 0x8000);
    Actor_unk607(Actor_unk51(1), 0);
    Actor_unk608(1, 4, 0);
    Actor_unk249(1, 398, 828);
    Actor_unk609(60);
    Actor_unk250(21, 0x4000, 0);
    Actor_unk251(0, 0xc000, 60);
    Actor_unk610(0, 3);
    Actor_unk611(60);
    Actor_unk612(21, 3);
    Actor_unk613(60);
    rec = Actor_unk313(1);
    flag = 1; /* One shared mark bit for the three record flags. */
    rec[90] |= flag;
    rec = Actor_unk314(5);
    rec[90] |= flag;
    rec = Actor_unk315(0);
    Actor_unk252(1, 0x10000, 0x8000);
    Actor_unk253(5, 0x10000, 0x8000);
    Actor_unk614(0, 0, 0);
    Actor_unk615(5, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18));
    Actor_unk616(1, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18) - 16);
    Actor_unk617(1);
    Actor_unk254(1, 0x5000, 30);
    Actor_unk618(1, 3);
    Actor_unk619(5, 3);
    Actor_unk620(0, 3);
    Actor_unk621(40);
    Actor_unk622(5, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_unk623(5, 0, 0);
    Actor_unk624(1, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_unk625(1, 0, 0);
    Actor_unk626(1, 5);
    Actor_unk41_4(0x1790000, 0xa00000, 0x3770000, 1);
    Actor_unk627(0, 13, 10, 0);
    Actor_unk255(0, 376, 912);
    Actor_unk256(0, 0xc000, 0);
    {
        u8 *record = Actor_unk316(21);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | flag);
    }
    Actor_unk42_4(21, 6, 5, 0);
    Actor_unk257(21, 373, 887);
    Actor_unk258(21, 0x4000, 0);
    Actor_unk259(0, 0xc000, 40);
    Actor_unk628(21, 3);
    Actor_unk629(0, 3);
    Actor_unk630(20);
    Actor_unk631(0, 1);
    Actor_unk632();
    Actor_unk633(100);
    Actor_unk13_5(49, 46, 8, 4, 20, 50);
    Actor_unk17_2(514);
    Actor_unk18_2(303);
    scene[85] = 3;
    *(s32 *)(scene + 12) = 0xa00000;
    *(s32 *)(scene + 60) = 0x80000000;
    *(s32 *)(scene + 40) = none;
    Actor_unk634();
}

void Effect_ConfigureSpawnedParticle(struct SourceEntity *source)
{
    s32 spawn_position[3];
    s32 particle_index;
    struct StagedParticle *particle;
    spawn_position[0] = source->f08;
    spawn_position[1] = source->f0c - (Actor_EntOp(source) << 4) + (s32)0xfff80000;
    spawn_position[2] = source->f10;
    particle_index = Actor_unk52();
    Actor_unk271(((particle_index << 1) + particle_index) << 4, Actor_unk53(), spawn_position);
    particle = Actor_unk317(0x11d, spawn_position[0], spawn_position[1], spawn_position[2]);
    if (particle != 0) {
        particle->f55 = 2;
        particle->f48 = 0x1999;
        particle->f5e = 12;
        Actor_Apply29(particle, 0);
        Actor_Apply30(particle, 0);
        Actor_Apply31(particle, (s32)gOv7);
        {
            struct ParticleRecord *record = particle->f50;
            s32 record_flags = ~12;
            record_flags &= record->f09;
            record_flags |= 4;
            record->f09 = record_flags;
        }
    }
    Actor_unk43_2(0x8a);
}

/*
 * Play a rising-dust burst from a source entity. The owner extends through its
 * four literal pool words. Both loops count down inclusively, so they run 31
 * lift steps and 8 particles. The address handed to Actor_unk635 is passed
 * through as a plain pointer; whether it is animation data or a callback is
 * not established.
 */
void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter)
{
    s32 frame_countdown;

    Actor_unk636(154);

    for (frame_countdown = 30; frame_countdown >= 0; frame_countdown--) {
        emitter->y += 0x10000;              /* 0x80 << 9. */
        emitter->field06 = (u16)(emitter->field06 + 0x2000);  /* 0x80 << 6. */
        emitter->field18 += -2048;          /* The pool word 0xfffff800. */
        emitter->field1c += -2048;
        Actor_unk637(1);
    }

    for (frame_countdown = 7; frame_countdown >= 0; frame_countdown--) {
        struct Resource373Particle *particle =
            Actor_unk318(0x11d, emitter->x, emitter->y, emitter->z);

        if (particle != 0) {
            s32 vertical_speed;

            Actor_unk638(particle, 0);
            Actor_unk635(particle, (const void *)0x0200e6e4);

            vertical_speed = Actor_unk54() + 0x10000;
            particle->field34 = 0x10000;
            particle->field30 = vertical_speed;
            particle->field55 = 2;
            particle->field48 = 0x0a3d;

            particle->lifetime = Actor_unk55() - Actor_unk56();

            Actor_unk639(
                particle,
                ((Actor_unk57() * 3) << 3) + 0x80000,
                Actor_unk58());
        }
    }

    Actor_unk640(131);

    emitter->x = 0;
    emitter->y = 0;
    emitter->z = 0;
    emitter->field38 = (s32)0x80000000;
    emitter->field3c = (s32)0x80000000;
    emitter->field40 = (s32)0x80000000;
    emitter->field24 = 0;
    emitter->field28 = 0;
    emitter->field2c = 0;
}

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
    s32 position[3];
    if (particle != 0) {
        position[0] = particle[2];
        position[1] = particle[3];
        position[2] = particle[4];
        Actor_unk272(delta_x, delta_z, position);
        Actor_SetMode3((s32)particle, position[0], position[1], position[2]);
    }
}

void Effect_PlayStepSound(void)
{
    if ((*(u32 *)0x03001e40 & 15) == 0)
        Actor_unk44_2(0x83);
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Actor_unk45_2((s32)effect);
    } else {
        s32 amplitude = Actor_unk59(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Actor_unk46_2((s32)effect);
    } else {
        s32 amplitude = Actor_unk60(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}

/* overlays/scene/actor/staged_motion/state_update.c */
extern s32 gOv8[];

u8 *Actor_unk641(s32);

u8 *Actor_unk642(s32);

void State_Send210AndApplyRectAt40x84(void)
{
    s32 m, n;
    Actor_unk47_2(0x210);
    m = 10;
    n = 84;
    Actor_unk15_5(40, 84, 7, 4, m, n);
}

void State_Send210AndApplyRect(void)
{
    s32 m, n;
    Actor_unk48_2(0x210);
    m = 10;
    n = 84;
    Actor_unk16_5(40, 89, 7, 4, m, n);
}

void State_ApplyFlag801Branch(void)
{
    if (Actor_unk61(0x801) == 0) {
        Actor_unk643();
    } else {
        Actor_unk49_2(123);
        Actor_unk50_2(1);
    }
}

void State_SetValue123Mode3(void)
{
    Actor_unk51_2(123);
    Actor_unk52_2(3);
}

void State_SetValue123Mode4(void)
{
    Actor_unk53_2(123);
    Actor_unk54_2(4);
}

void State_ApplyValues123And2(void)
{
    Actor_unk55_2(123);
    Actor_unk56_2(2);
}

void State_RunFlag204Step(void)
{
    s32 m, n;
    Actor_unk644();
    m = 20;
    n = 50;
    Actor_unk17_5(49, 53, 8, 4, m, n);
    Actor_SetMode4(0, 10, 11, 1);
    Actor_unk57_2(0x204);
    Actor_unk645();
}

void State_RunTablePairWhenActor22State1(void)
{
    u8 *p = Actor_unk641(22);
    if (Actor_unk62(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 1) {
            Actor_Apply32((s32)gOv8, (s32)gOv5);
        }
    }
}

void State_RunTablePairWhenActor22State2(void)
{
    u8 *p = Actor_unk642(22);
    if (Actor_unk63(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 2) {
            Actor_Apply33((s32)gOv4StagedMotion, (s32)gOv6);
        }
    }
}

void State_SetValue1ThenCall(void)
{
    Actor_unk58_2(1);
    Actor_unk646();
}

void State_SetValue0ThenCall(void)
{
    Actor_unk59_2(0);
    Actor_unk647();
}

void State_SetValue24ThenCall(void)
{
    Actor_unk60_2(0x18);
    Actor_unk648();
}

void State_SetValue25ThenCall(void)
{
    Actor_unk61_2(0x19);
    Actor_unk649();
}

void State_ApplyRectAndRunTwo(void)
{
    s32 e = 22;
    s32 f = 36;
    Actor_unk18_5(17, 0, 3, 1, e, f);
    Actor_unk650();
    Actor_unk651();
}

/* overlays/scene/actor/staged_motion/map_staged_scene_select_primary_data.c */
/* overlays/scene/actor/staged_motion/select_primary_scene_data.c */
extern s32 RomBytes_0200e708[];
s32 MapStagedScene_SelectPrimaryData(void)
{
    return (s32)RomBytes_0200e708;
}

/* overlays/scene/actor/staged_motion/get_empty_scene_data.c */
s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

/* overlays/scene/actor/staged_motion/select_secondary_scene_data.c */
s32 MapStagedScene_SelectSecondaryData(void)
{
    return (s32)RomBytes_0200e870;
}

/* overlays/scene/actor/staged_motion/select_tertiary_scene_data.c */
extern u8 gCell[];
extern s32 gOv2SelectPrimaryData[];
extern s32 gOv3SelectPrimaryData[];
extern s32 gOv4SelectPrimaryData[];

s32 MapStagedScene_SelectTertiaryData(void)
{
    u8 *scene_state = gCell;
    if (*(s16 *)(scene_state + 0x1c2) == 16)
        return (s32)RomBytes_0200ee48;
    if (Actor_CheckSelectPrimaryData(0x87a) != 0)
        return (s32)gOv2SelectPrimaryData;
    if (Actor_unk2SelectPrimaryData(0x815) != 0)
        return (s32)gOv3SelectPrimaryData;
    return (s32)gOv4SelectPrimaryData;
}

/* overlays/scene/actor/staged_motion/select_quaternary_scene_data.c */
extern s32 gOvSelectQuaternaryData[];
extern s32 gOv2SelectQuaternaryData[];
extern s32 gOv3SelectQuaternaryData[];

s32 MapStagedScene_SelectQuaternaryData(void)
{
    if (Actor_CheckSelectQuaternaryData(0x87a) != 0)
        return (s32)gOvSelectQuaternaryData;
    if (Actor_unk2SelectQuaternaryData(0x815) != 0)
        return (s32)gOv2SelectQuaternaryData;
    return (s32)gOv3SelectQuaternaryData;
}
