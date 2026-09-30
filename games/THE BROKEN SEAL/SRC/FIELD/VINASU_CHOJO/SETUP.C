/* Object motion, scene tables and the actors' setup. */
#include "CHOJO.H"
#include "IWRAM_CALL.H"

s32 OverlayObject_StepScaleByCounter(Spr *s)
{
    s16 *p = (s16 *)((u8 *)s + 100);

    switch (*p) {
    case 6:
        s->unk18 += -0x2000;
        s->unk1c += 0x1000;
        break;
    case 4:
        s->unk18 += 0x1000;
        s->unk1c += -0x800;
        break;
    case 2:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        break;
    case 0:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        *p = IwramUnsignedRemainder(Engine_RandomNext(), 80) + 80;
        break;
    }
    {
        s32 t = s->unk64;
        t = t - 1;
        s->unk64 = t;
    }
    return 1;
}

/*
 * Moves an effect like BattleFx_UpdateObjectMotionScaleAndLinkedAngle, and slows its horizontal velocity by a
 * twenty-second across and a twentieth in depth each frame.
 */
void Effect_MoveWithDrag(union FieldObject *object)
{
    s32 velocity_x = object->effect.velocity_x;
    s32 velocity_z;

    object->effect.x += velocity_x;
    object->effect.y += object->effect.velocity_y;
    velocity_z = object->effect.velocity_z;
    object->effect.z += velocity_z;
    object->effect.velocity_x = velocity_x - velocity_x / 22;
    object->effect.velocity_z = velocity_z - velocity_z / 20;
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}

void OverlayObject_DecayRecordField1e(Spr_02000400 *s)
{
    Obj_02000400 *o = s->obj;
    u16 h = o->unk1e;

    if ((s32)((h + 0xffff) << 16) < 0) {
        {
            s32 t = h + (s32)0xfffff600;
            o->unk1e = t;
        }
    }
}

s32 OverlayObject_UpdateHeadingTimer(Spr_02000424 *s)
{
    u16 *q = (u16 *)((u8 *)s + 0x66);
    s32 c = *q;
    s16 v = *(s16 *)q;

    if (v == 0) {
        {
            s32 t = ((u32)(Random_Next() << 15)) >> 16;
            s->unk06 = s->unk06 + t;
        }
        {
            s32 n = ((u32)(Random_Next() * 80)) >> 16;
            *q = n;
            if (n == 0) {
                goto out;
            }
            c = n;
        }
    }
    *q = c - 1;
out:
    return 1;
}

/* Contiguous unnamed leaf-owner run for resource_3c9. */
s32 SceneData_GetTablee3d4(void)
{
    return (s32)Data_0200e3d4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTablee464(void)
{
    return (s32)Data_0200e464;
}

s32 SceneData_GetTablee478(void)
{
    return (s32)Data_0200e478;
}

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_FindNearestSlotOfKindF2(void)
{
    u8 *work = *(u8 **)&gEventWork;
    Spr_020004bc **p;
    s32 best = 0;
    Spr_020004bc *ref;
    s32 limit;
    u32 i;

    limit = 640;
    ref = Actor_Get(ACTOR_PARTY_LEADER);
    i = 8;
    p = (Spr_020004bc **)(work + 0x34);
    do {
        Spr_020004bc *spr = *p++;
        if (spr != 0) {
            if (*spr->obj->unk28 == 0xf2) {
                s32 dist = MeasureFixedPointPositionDistance((u8 *)ref + 8, (u8 *)spr + 8);
                if (dist < limit) {
                    limit = dist;
                    best = i;
                }
            }
        }
        i++;
    } while (i <= 65);
    return best;
}

void InitializeActorZeroMotion(void)
{
    struct SceneActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, angle, position);
    if (Object_CheckMovementCollision(actor, position) == 0) {
        GameFlag_Clear(592);
        SceneActor_SetByte55ForActorZeroAnd12To17();
        Object_SetAnimation(actor, 6);
        Task_Wait(6);
        Object_SetAnimation(actor, 7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        Audio_PlayCue(152);
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        Actor_SetSpriteFlags(actor, 0);
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, (s16)(position[0] >> 16),
                         (s16)(position[2] >> 16));
        Object_SetAnimation(actor, 6);
        Actor_SetSpriteFlags(actor, 1);
        actor->flags55 = flags;
    }
}

void SceneEffect_SpawnAndBobWithActorZero(void)
{
    Spr_020005ec *a = Actor_Get(ACTOR_PARTY_LEADER);
    Spr_020005ec *b;
    Spr_020005ec *r;
    s32 k;

    Event_Begin();
    r = SceneActor_FindNearestSlotOfKindF2();
    Data_0200e6e8 = r;
    if (r != 0) {
        GameFlag_Set(592);
        b = Actor_Get(Data_0200e6e8);
        b->unk55 = 0;
        a->unk55 &= 0xfe;
        b->unk0c += (s32)0xfffd0000;
        a->unk0c += (s32)0xfffd0000;
        a->unk14 += (s32)0xfffd0000;
        Task_Wait(2);
        b->unk0c += (s32)0xfffe0000;
        a->unk0c += (s32)0xfffe0000;
        a->unk14 += (s32)0xfffe0000;
        Task_Wait(10);
        k = 0x20000;
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Task_Wait(4);
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Task_Wait(4);
        b->unk0c += 0x10000;
        a->unk0c += 0x10000;
        a->unk14 += 0x10000;
    }
    Event_End();
}

void SceneActor_SetByte55ForActorZeroAnd12To17(void)
{
    s32 val;

    *((u8 *)Engine_ActorGet(0) + 0x55) = 3;
    val = 4;
    *((u8 *)Engine_ActorGet(12) + 0x55) = val;
    *((u8 *)Engine_ActorGet(13) + 0x55) = val;
    *((u8 *)Engine_ActorGet(14) + 0x55) = val;
    *((u8 *)Engine_ActorGet(15) + 0x55) = val;
    *((u8 *)Engine_ActorGet(16) + 0x55) = val;
    *((u8 *)Engine_ActorGet(17) + 0x55) = val;
}

s32 SceneData_GetTableE6ec(void)
{
    return (s32)Data_0200e6ec;
}

s32 FieldScene_InitActorsAndDispatchBySubstate(void)
{
    s32 v;
    u32 i;
    s32 z;
    s16 mode;
    Spr_0200071c *obj;

    GameFlag_Set(324);
    Task_Wait(1);
    GameFlag_Set(272);
    Actor_SetSpriteFlags(Actor_Get(8), 0);
    Actor_SetSpriteFlags(Actor_Get(9), 0);
    Actor_SetSpriteFlags(Actor_Get(10), 0);
    Actor_SetSpriteFlags(Actor_Get(11), 0);
    v = (s32)0xffff0000;
    Engine_ActorGet(10)->scale_x = v;
    Engine_ActorGet(11)->scale_x = v;
    i = 12;
    z = 0;
    do {
        obj = Actor_Get(i);
        Actor_SetSpriteFlags(Actor_Get(i), z);
        Actor_SetSpritePriority(i, 1);
        obj->unk55 = 4;
        obj->unk23 |= 2;
        obj->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = gGameState.entrance;

    switch (mode) {
    case 1:
        if (GameFlag_IsSet(0x109) == 0) {
            Scene_RunScriptedActorPresentation();
        }
        break;
    case 2:
        Scene_RunPairedActorEffectSequence();
        break;
    case 3:
        FieldScene_RunThreeStepsInBracket();
        break;
    case 0x5d:
        FieldScene_RunBracketedSceneWithFlag282();
        break;
    case 4:
        FieldScene_RunScene3c9_02003924();
        break;
    case 9:
        Event_Begin();
        if (GameFlag_IsSet(0x345) != 0) {
            Inventory_AddItem(0, 65);
        } else if (GameFlag_IsSet(0x346) != 0) {
            Inventory_AddItem(1, 65);
        } else if (GameFlag_IsSet(0x347) != 0) {
            Inventory_AddItem(2, 65);
        } else {
            Inventory_AddItem(3, 65);
        }
        Event_RequestExit(9);
        break;
    }

    if (GameFlag_IsSet(0x109) != 0) {
        s32 slot = SceneActor_FindNearestSlotOfKindF2();

        if (slot != 0) {
            Spr_0200071c *p = (Spr_0200071c *)Engine_ActorGet(slot);
            if (p != 0) {
                p->unk55 = 0;
            }
        }
    }
    return z;
}

void VinasuChojo_ShowMessage(s32 speaker)
{
    Event_ShowMessage(speaker, 0);
    Event_Wait(10);
}

void VinasuChojo_FaceActor(s32 actor, s32 facing)
{
    Actor_FaceDirection(actor, facing, 10);
}
