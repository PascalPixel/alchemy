#include "IMIRU_FUCHIN.H"
#include "CALL.H"
/* Spawn a scripted effect with optional palette, priority and scale rates.
 * Complete 352-byte owner, including its three-word pool, matches exactly.
 * FAKEMATCH: retain the local script-table copy and branch-local divide
 * tails so the compiler reloads the script and prepares both call arguments
 * in the observed lifetime. Shared FIELD_EFFECT types recover the remaining
 * object, sprite and options layout without private byte-offset casts. */
#include "FIELD_EFFECT.H"
#include "TYPES.H"

void ImiruFuchin_ApplyEntrySetup(void);
void FieldScene_RunScene39aSequenceA(void);

extern s32 ImiruFuchin_TrackLeader;
void ImiruFuchin_ApplyRoomLayout(void);
void ImiruFuchin_PlaceDragonsEye(void);
struct Actor_39a *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 sprite);
void BattleFx_StartFadeOverlay(s32 mode);

void DialogueLayout_ConfigureGroupOne(void);
void DialogueLayout_ConfigureGroupTwo(void);
void DialogueLayout_ConfigureGroupThree(void);
void FieldScene_RunFlagBranchedLayoutSteps(void);

void OverlayObject_AdvancePositionByDelta();

/* The overlay's three effect scripts, at the start of its read-only data. */
extern const s32 *const gEffectScripts[];

struct ScriptTable {
    const s32 *script[3];
};

extern u32 gFrameCount;
void Engine_AudioPlayCue();
void Effect_Spawn();

struct DustParams {
    s32 count;
    s32 kind;
    s32 spreadX;
    s32 spreadY;
    s32 growX;
    s32 growY;
};

/* The work in slot 56, whose byte at +52 marks a fade under way. */
struct FadeWork {
    u8 unknown_00[52];
    u8 active;
};

/* The scene start around Imil: open with the window transition; the first
   area runs its opening sequence until flag 0x109 is set, and otherwise the
   areas are set up for the entrance. */
s32 ImiruFuchin_ApplyEntryHook(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (GameFlag_IsSet(0x109) == 0 && gGameState.scene == (s32)&SceneId_ImiruFuchin1) {
        GameFlag_Set(0x144);
        FieldScene_RunScene39aSequenceA();
    } else {
        ImiruFuchin_ApplyEntrySetup();
    }
    return 0;
}

void FieldScene_RunScene39aSequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    record = Engine_ActorGet(8);
    Actor_SetSpriteFlags(record, 0);
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x108, 196);
    Event_End();
}

void ImiruFuchin_ApplyEntrySetup(void)
{
    u8 *actor;
    s32 value;

    ImiruFuchin_ApplyRoomLayout();
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin4) {
        if (!GameFlag_IsSet(0xf13) && gGameState.entrance == 1) {
            ImiruFuchin_PlaceDragonsEye();
        }
        if ((u16)(gGameState.entrance - 2) <= 3) {
            OverlayObject_CreateAndInitialize(0x9c0000, 0, 0x1c40000, 223);
            OverlayObject_CreateAndInitialize(0xbc0000, 0, 0x1c40000, 223);
        }
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin7) {
        /* FAKEMATCH: one zero clears the flag and both of actor 8's words,
         * and the variable is reused for the tracking work below, so the zero
         * and then the work share one register. */
        value = 0;
        actor = (u8 *)Actor_Get(8);
        ImiruFuchin_TrackLeader = value;
        actor[85] = value;
        *(s32 *)(actor + 12) = value;
        Actor_SetSpritePriority(8, 1);
        Actor_SetChildValue(8, 15);
        switch (gGameState.entrance) {
        case 1:
        case 2:
            BattleFx_StartFadeOverlay(0);
            ImiruFuchin_TrackLeader = 1;
            break;
        case 5:
            BattleFx_StartFadeOverlay(0);
            ImiruFuchin_TrackLeader = 1;
            value = *(s32 *)(gWorkSlot + 36);
            ((struct TrackingWork *)value)->actor = NULL;
            break;
        }
        if (gGameState.entrance <= 6) {
            if (GameFlag_IsSet(0x820)) {
                Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
                Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
            } else {
                gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
                ColorBuffer_ApplySource(0x203108, 1);
                ColorBuffer_ApplyTarget(0x203108, 1);
                ColorBuffer_Interpolate(1);
                Task_Wait(1);
            }
        }
    }
}

/* Copy the room's cell attributes for the way it is entered, then run the
 * room's layout step. */
void ImiruFuchin_ApplyRoomLayout(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin2) {
        Map_CopyCellAttributes(8, 29, 15, 5, 8, 42);
        DialogueLayout_ConfigureGroupOne();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin3) {
        Map_CopyCellAttributes(12, 8, 10, 18, 0, 28);
        DialogueLayout_ConfigureGroupTwo();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && gGameState.entrance != 1) {
        Map_CopyCellAttributes(12, 21, 9, 16, 12, 3);
        DialogueLayout_ConfigureGroupThree();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin5) {
        if (gGameState.entrance == 1 || gGameState.entrance == 2) {
            Map_CopyCellAttributes(14, 10, 9, 8, 22, 20);
        } else {
            Map_CopyCellAttributes(7, 45, 11, 4, 20, 45);
        }
        FieldScene_RunFlagBranchedLayoutSteps();
    }
}

/* Hop the leader across the gap the touched trigger stands for. */
void ImiruFuchin_HopOnTrigger(void)
{
    s32 trigger = gEventWork->touched_trigger;

    if (gGameState.scene == (s32)&SceneId_ImiruFuchin3) {
        if (trigger == 17) {
            ImiruFuchin_HopBy(0, -32);
        } else {
            ImiruFuchin_HopBy(-32, 0);
        }
    }
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && trigger == 25 && GameFlag_IsSet(0x309)) {
        ImiruFuchin_HopBy(0, 32);
    }
}

void FieldScene_ApplyOffset0Neg32(void)
{
    ImiruFuchin_HopBy(0, -32);
}

void SceneState_ApplyOffsetMinus32(void)
{
    ImiruFuchin_HopBy(-32, 0);
}

void ImiruFuchin_HopBy(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    Engine_ActorSetDestinationOffset(0, a0, a1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 7);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 6);
    Event_End();
}

void ImiruFuchin_PlaceDragonsEye(void)
{

    u8 *rec;
    s32 rec7;
    s32 record;
    u8 *p6;

    record = 0;
    rec = Value4(Engine_ObjectCreate, 22, 0xf80000, 0x80000, 0x980000);
    if ((s32)rec != 0) {
        p6 = *(u8 **)(rec + 80);
        p6[38] = record;
        p6[39] = record;
        *((s8 *)p6 + 5) &= -33;
        p6[9] &= 15;
        rec[85] = record;
        rec[92] = 1;
        rec7 = Engine_HeapAllocate(17, 0x608);
        Item_LoadIcon(ITEM_DRAGONS_EYE);
        Vram_Load(p6[28], 128, (rec7 + 0x400));
        Heap_Release(17);
        *(s32 *)gImiruFuchinDragonsEye = (s32)rec;
    }
}

/* Returns a value: the reference sets r1 before r0 at this site. */
void ImiruFuchin_TakeDragonsEye(void)
{

    Event_Begin();

    /* r5 holds &gImiruFuchinDragonsEye across the calls; the word is reloaded before
     * the second test. */
    if (gImiruFuchinDragonsEye[0] != 0) {
        Engine_RunRisingObjectSequence(gImiruFuchinDragonsEye[0], 3);
    }

    Party_GiveItem((s32) 0xE6, 0);
    GameFlag_Set((s32) 0xF13);

    if (gImiruFuchinDragonsEye[0] != 0) {
        Engine_ObjectDispatchRelease(gImiruFuchinDragonsEye[0]);
    }

    Event_End();
}

void OverlayObject_AdvancePositionByDelta(struct MovingObject *object)
{
    object->x += object->dx;
    object->y += object->dy;
    object->z += object->dz;
    object->sub_x += object->sub_dx;
    object->sub_y += object->sub_dy;
}

s32 OverlayObject_ApplyValue15(s32 obj)
{
    Object_SetPalette(obj, 15);
    return 0;
}

void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
                  const struct EffectOptions *extra)
{
    struct ScriptTable table;
    struct FieldEffect *obj;
    struct FieldSprite *spr;
    const s32 *script;

    table = *(struct ScriptTable *)gEffectScripts;
    obj = (struct FieldEffect *)Engine_ObjectCreate(222, x, y, z);
    if (obj == 0)
        return;
    spr = obj->sprite;
    Engine_ObjectSetAnimation((struct FieldActor *)obj, (flags + 1) & EFFECT_SCRIPT_MASK);
    Engine_ObjectSetScript((struct FieldActor *)obj, table.script[flags & EFFECT_SCRIPT_MASK]);
    obj->motion_flags = 0;
    spr->flags = 0;
    obj->update = OverlayObject_AdvancePositionByDelta;
    obj->velocity_x = velocity_x;
    obj->velocity_y = velocity_y;
    obj->velocity_z = velocity_z;
    obj->scale_rate_x = 0;
    obj->scale_rate_y = 0;
    spr->priority = 1;
    if ((flags & 0xffff0000) == 0 || extra == 0)
        return;
    if (flags & EFFECT_USE_PALETTE)
        ObjectGroup_SetChildValue((struct FieldActor *)obj, extra->palette);
    if (flags & EFFECT_USE_PRIORITY) {
        obj->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
        spr->priority = extra->priority;
    }
    if (flags & EFFECT_USE_START_SCALE) {
        obj->scale_x = extra->start_scale_x;
        obj->scale_y = extra->start_scale_y;
    }
    if (flags & EFFECT_SCALE_TO_TARGET) {
        script = table.script[flags & EFFECT_SCRIPT_MASK];
        if (flags & EFFECT_USE_START_SCALE) {
            obj->scale_rate_x = Engine_MathDivide(extra->target_scale_x - obj->scale_x, script[3]);
            obj->scale_rate_y = Engine_MathDivide(extra->target_scale_y - obj->scale_y, script[3]);
        } else {
            obj->scale_rate_x = Engine_MathDivide(extra->target_scale_x - 0x10000, script[3]);
            obj->scale_rate_y = Engine_MathDivide(extra->target_scale_y - 0x10000, script[3]);
        }
    }
}

/* Every fourth frame, blow a puff of dust across the cave mouth. */
void ImiruFuchin_BlowCaveMouthDust(void)
{
    struct DustParams params;
    struct DustParams *p;
    s32 phase;
    s32 dx;
    s32 dy;

    phase = *(volatile s32 *)&gFrameCount & 3;
    if (phase != 0)
        return;
    p = &params;
    p->kind = 10;
    p->spreadX = 0x8000;
    p->spreadY = 0x8000;
    p->growX = 0x1cccc;
    p->growY = 0x1cccc;
    if ((*(volatile s32 *)&gFrameCount & 7) == 0)
        Engine_AudioPlayCue(136);
    dx = -0x10000 - ((((u32)Engine_RandomNext() << 1) >> 16) << 16);
    dy = -(s32)((((u32)Engine_RandomNext() * 3) >> 16) * 0x3333);
    Effect_Spawn(0x1340000, 0x400000, 0xde0000, dx, dy, phase, 0xd0001, p);
}

/*
 * The cave-mouth wind: four passes of the dust blower, scheduled as a
 * callback while the map opens.
 */
void FieldScene_RunFourPassCallbackSequence(void)
{
    s32 pass;
    s32 step;
    s32 span;
    s32 one;

    Audio_PlayCue(19);
    Audio_PlayCue(182);
    Event_Begin();
    Battle_ResetEffectCounter();

    /* 8, 7 and 1 are locals held across the loop, not literals: the first
     * call takes 8 as an immediate for argument 4 and from a register for
     * argument 5, which a literal cannot produce. */
    pass = 0;
    step = 8;
    span = 7;
    one = 1;
    do {
        ColorBuffer_ApplyTarget((s32)0x204318, 1);
        ColorBuffer_Interpolate(1);
        Task_Wait(2);
        if (pass == 0) {
            Map_CopyCellsTo(30, 8, 12, 8, step, span);
            Map_CopyCellsTo(30, 57, 19, 57, one, one);
        }
        ColorBuffer_ApplyTarget((s32)0x203108, 1);
        ColorBuffer_Interpolate(1);
        Task_Wait(2);
        /* The increment belongs to the loop test, not the body: `pass++;` as
         * a statement would not place it after the last call.  The compare is
         * unsigned against 3, so the body runs for pass 0 to 3. */
    } while ((unsigned int)++pass <= 3);

    Task_Wait(30);
    /* 0xc80 is built by shifting a small immediate, not loaded whole. */
    Engine_TaskAddCallback((void *)ImiruFuchin_BlowCaveMouthDust, (s32)0xc80);
    Task_Wait(40);
    ColorBuffer_ApplyTarget((s32)0x201090, 1);
    ColorBuffer_Interpolate(40);
    Task_Wait(80);
    Engine_TaskRemoveCallback((void *)ImiruFuchin_BlowCaveMouthDust);
    Task_Wait(20);
    /* 0x10000 is built by shifting a small immediate, not loaded whole. */
    ColorBuffer_ApplyTarget((s32)0x10000, 1);
    ColorBuffer_Interpolate(80);
    /* Same import as in the loop, one argument here. */
    Task_Wait(80);
    /* 0x820 is built by shifting a small immediate, not loaded whole. */
    GameFlag_Set((s32)0x820);
    PartyInventory_Discard(230);
    Audio_PlayCueFromEventWork();
    /* Same import as the first call, no argument register written here. */
    Event_End();
}

void SceneState_SetValue17e1(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_DRAGONS_FLAME_ILLUMINATES_PATH_TRUTH, 1);
    Event_End();
}

void SceneDialogue_RunLine17e2(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_SECRET_KI_SHALL_REVEALED_DISCIPLES, 1);
    Event_End();
}

void FieldScene_RunScriptedStep17E3(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_RAYS_LIGHT_GIVE_BIRTH_SHADOWS, 1);
    Event_End();
}

/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void SceneState_SetWorkspace370ByFlag820(void)
{

    Event_Begin();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (GameFlag_IsSet((s32)0x820) != 0) {
        Message_ShowCentered((s32)0x17e5, 1);
    } else {
        Message_ShowCentered((s32)0x17e4, 1);
        if (PartyInventory_FindOwner((s32)0xe6) != -1) {
            u8 *workspace = (u8 *)gEventWork;

            /* movs r1,#0xb9 / lsls r1,#1 gives the byte offset 370. */
            /*
             * The store goes through a pointer local and an s32 value local,
             * in that order. Storing the literal directly builds the constant
             * in HImode and loads it from the literal pool, costing a pool
             * word; splitting the address out first also fixes which register
             * holds it.
             */
            {
                u16 *slot = (u16 *)(workspace + 370);
                s32 one = 1;

                *slot = (u16)one;
            }
        }
    }
    Event_End();
}

/* While the stage is early enough, start the fade-in: raise the fade flag,
 * set the three light flags of the work in slot 31 and interpolate the
 * palette over 16 frames. */
void ImiruFuchin_StartFadeIn(void)
{
    struct FadeWork *fade;
    u8 *work;

    if (gGameState.entrance <= 6) {
        fade = *(gWorkSlot + 56);
        work = *(gWorkSlot + 31);
        fade->active = 1;
        work[0x53e] = 0;
        work[0x53c] = 1;
        work[0x53d] = 1;
        ColorBuffer_ApplySource(0, 1);
        ColorBuffer_ApplyTarget(0x203108, 1);
        ColorBuffer_Interpolate(16);
        Task_Wait(16);
    }
}

void SceneActor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = gImiruFuchinKeyHeadings[(*(u32 *)gKeysHeld >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Object_SetAnimation(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Object_SetAnimation(o, 2);
    ObjectDispatch_ApplyValueToChildren(o, 0x30);
}

/*
 * Pathing step for resource_39a.  r0 holds the popped return address, so
 * nothing is returned, and the seven pool words after the return belong to
 * the owner.  Frame: sp+0 is the goal marker, sp+4 the heading, and
 * sp+8..sp+19 the three-word probe position handed to the stepping imports by
 * address.  The x and z assignment order and the inline stepping wrapper are
 * what reproduce the reference; do not reorder or respell them.
 */
void SceneActor_StepSubjectAlongHeading(void)
{

    struct PathSubject *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *subject_id;

    subject = ObjectTable_Get(gGameState.selected_actor);

    for (;;) {
        heading = gImiruFuchinHeadings[(*(u32 *)gKeysHeld >> 4) & 15];
        /*
         * The test is on heading << 16 against 0xffff0000, the signed
         * halfword -1 meaning "no heading".
         */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Event_Begin();

        /* The 0x80000 bias is built by shifting, not loaded as a constant. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = GetMapCellCollision((s32)*subject_id, x, z);
        /*
         * 0x100000 is built by shifting, not loaded as a constant.  The probe
         * block is passed by address and is advanced by the callee.
         */
        Vector_AddPolarOffset((s32)0x100000, heading, probe);

        marker = GetMapCellCollision((s32)*subject_id, probe[0], probe[2]);
        if (marker == 255
                || Map_GetTerrainHeight((s32)*subject_id, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before 0x02004392. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Object_SetPosition(subject, x, subject->y, z);
        /*
         * Same call word as the marker lookup, but a two-argument command, so
         * it keeps its own declaration.
         */
        Object_SetAnimation(subject, 2);
        ObjectDispatch_ApplyValueToChildren(subject, 48);
        Object_CommitPosition(subject);
        subject->callback = (void *)SceneActor_TurnTowardTableAngle;

        goto advance_probe;
continue_probe:
        if (Map_GetTerrainHeight((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finish_probe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Object_SetPosition(subject, probe[0], probe[1], probe[2]);
        Object_CommitPosition(subject);
        if (marker != goal) {
            goto blocked;
        }

advance_probe:
        AdvanceProbe(heading, probe);
        marker = GetMapCellCollision((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continue_probe;
        }

finish_probe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Object_SetPosition(subject, x, subject->y, z);
        Object_CommitPosition(subject);
        Task_Wait(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built by shifting, not loaded as a constant. */
    subject->state_052 = 0x4000;

tail:
    Task_Wait(10);
    Event_End();
}
