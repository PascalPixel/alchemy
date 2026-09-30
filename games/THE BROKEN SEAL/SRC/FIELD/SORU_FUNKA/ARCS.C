/* Scene steps, regions and arcing effects. */
#include "FUNKA.H"
/* The ten arcing effects' origins, laid out after the code. */
extern s32 Funka_ArcOrigins[][2];

/* The IWRAM remainder, reached through an import veneer. */
u32 IwramUnsignedRemainder(u32, u32);
void SoruFunka_SpawnEffectPair();

void SceneActor_MoveTo232_125AndFace4000(s32 no)
{
    Ent_02002820 *rec;

    rec = (Ent_02002820 *)Engine_ActorGet(no);
    Actor_SetPosition(no, 0xe80000, 0x7d0000);
    rec->unk6 = 0x4000;
    Actor_SetSpritePriority(no, 3);
}

void SceneState_ApplyRectsByCondition(s32 a)
{
    if (a != 0) {
        s32 x;
        s32 y;
        x = 1;
        Map_CopyCellsTo(8, 47, 64, 7, x, x);
        y = 2;
        Map_CopyCellsTo(7, 48, 63, 8, y, x);
        Map_CopyCellsTo(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Map_CopyCellsTo(56, 0, 64, 7, x, x);
        Map_CopyCellsTo(56, 0, 63, 8, x, x);
        Map_CopyCellsTo(56, 0, 63, 9, 2, x);
        Map_CopyCellsTo(58, 25, 64, 8, x, x);
    }
    Map_Redraw();
}

void SceneState_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Map_CopyCellsTo(9, 45, 65, 5, n, n);
        Map_CopyCellsTo(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Map_CopyCellsTo(89, 2, 65, 5, n, n);
        Map_CopyCellsTo(102, 32, 67, 6, 1, n);
    }
    Map_Redraw();
}

void FieldScene_RunRandomHalfBranch(void)
{
    if ((gFrameCount & 1) == 0) {
        if (IwramUnsignedRemainder(Random_Next(), 100) > 50) {
            SceneState_ApplyRectsByCondition(1);
        } else {
            SceneState_ApplyRectsByCondition(0);
        }
    }
}

void FieldScene_RunLateRandomHalfBranch(void)
{
    if ((gFrameCount & 1) == 0) {
        if (IwramUnsignedRemainder(Random_Next(), 100) > 50) {
            SceneState_ApplyRectPairByFlag(1);
        } else {
            SceneState_ApplyRectPairByFlag(0);
        }
    }
}

void FieldScene_RunFourWayEffectSequence(u32 mode)
{
    extern u8 *gArcEffects[];
    void Actor_ShowEmote(s32, s32, s32);
    void ColorBuffer_ApplyTarget(s32, s32);
    void ColorBuffer_Interpolate(s32);
    void Audio_PlayCue(s32);

    u32 i, zero;
    s32 x, y, z;
    s32 *pos;
    u8 *obj, *sprite;

    for (i = 0; i <= 15; i++)
        Actor_Destroy(i + 16);
    switch (mode) {
    case 0: ColorBuffer_ApplyTarget(0x4039d2, 1); break;
    case 1: ColorBuffer_ApplyTarget(0x4049d2, 1); break;
    case 2: ColorBuffer_ApplyTarget(0x404a4e, 1); break;
    case 3: ColorBuffer_ApplyTarget(0x403a52, 1); break;
    }
    ColorBuffer_Interpolate(60);
    Audio_PlayCue(214);
    i = 0;
    zero = i;
    for (pos = &Funka_ArcOrigins[0][0]; i <= 9; i++, pos += 2) {
        x = pos[0];
        y = pos[1];
        z = 0;
        switch (mode) {
        case 0: x += 0xe80000; z = 0x900000; break;
        case 1: x += 0xe80000; z = 0x1d00000; break;
        case 2: x += 0x2c70000; z = 0x900000; break;
        case 3: x += 0x2c70000; z = 0x1d00000; break;
        }
        gArcEffectTimers[i] = zero;
        obj = (u8 *)Engine_ObjectCreate(284, x, y, z);
        gArcEffects[i] = obj;
        obj[85] = zero;
        sprite = *(u8 **)(obj + 80);
        sprite[38] = zero;
        ((SpriteMode *)sprite)->mode = 1;
        Object_SetAnimation(obj, 6);
        Task_Wait(6);
    }
    if (mode == 0) {
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
        Actor_ShowEmote(ACTOR_GERALD, 256, 0);
    }
    Task_Wait(20);
    Engine_TaskAddCallback(SceneEffect_AdvanceTenEntryTimers, 3200);
    Audio_PlayCue(246);
    gArcEffectTimers[0] = 1; Task_Wait(6);
    gArcEffectTimers[1] = 1; Task_Wait(6);
    gArcEffectTimers[2] = 1; Task_Wait(6);
    gArcEffectTimers[3] = 1; Task_Wait(6);
    gArcEffectTimers[4] = 1; Task_Wait(6);
    gArcEffectTimers[5] = 1; Task_Wait(6);
    gArcEffectTimers[6] = 1; Task_Wait(6);
    gArcEffectTimers[7] = 1; Task_Wait(6);
    gArcEffectTimers[8] = 1; Task_Wait(6);
    gArcEffectTimers[9] = 1; Task_Wait(6);
    for (;;) {
        for (i = 0; i <= 9; i++) {
            if (gArcEffectTimers[i] != 0) {
                i = 888;
                break;
            }
        }
        if (i != 888)
            break;
        Task_Wait(1);
    }
    Task_Wait(40);
    Engine_TaskRemoveCallback(SceneEffect_AdvanceTenEntryTimers);
    ColorBuffer_ApplyTarget(65536, 1);
    ColorBuffer_Interpolate(40);
}

void SceneEffect_AdvanceTenEntryTimers(void)
{
    extern Ent *gArcEffects[];

    u32 i;
    s32 v;
    Ent_02002ba0 *p;

    for (i = 0; i <= 9; i++) {
        v = gArcEffectTimers[i];
        if (v != 0) {
            p = gArcEffects[i];
            if ((u32)v <= 8) {
                p->unk18 += -0x1ccc;
                p->unk1C += 0x8000;
                p->unkC += 0x4ccc;
                p->unk3C += 0x4ccc;
            } else {
                p->unkC += 0x140000;
                p->unk3C += 0x140000;
            }
            v = gArcEffectTimers[i] + 1;
            gArcEffectTimers[i] = v;
            if ((u32)v > 14) {
                gArcEffectTimers[i] = 0;
            }
        }
    }
}

void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b)
{
    Ent_02002c1c *p;

    /* FAKEMATCH: the view-centre import is passed the tile x it ignores, as
     * the game passes it. */
    p = (Ent_02002c1c *)((s32 (*)())Engine_EventGetViewCenter)(a);
    b = b << 16;
    a = a << 16;
    Camera_MoveTo(a, -1, b, 1);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    p->unk10 = b;
    p->unk8 = a;
    p->unk38 = 0x80000000;
    p->unk40 = 0x80000000;
    p->unk24 = 0;
    p->unk2C = 0;
    Task_Wait(5);
    Map_Redraw();
    Task_Wait(5);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(20);
    Task_Wait(30);
}

void SceneState_ConfigureEightCornerRegions(void)
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 v;

    x = 0;
    Map_CopyCellAttributes(14, 8, 1, 1, 10, x);
    Map_CopyCellAttributes(14, 28, 1, 1, 11, x);
    Map_CopyCellAttributes(44, 8, 1, 1, 12, x);
    Map_CopyCellAttributes(44, 28, 1, 1, 13, x);
    z = 14;
    y = 8;
    Map_CopyCellAttributes(13, 8, 1, 1, z, y);
    w = 28;
    Map_CopyCellAttributes(13, 28, 1, 1, z, w);
    v = 44;
    Map_CopyCellAttributes(43, 8, 1, 1, v, y);
    Map_CopyCellAttributes(43, 28, 1, 1, v, w);
}

void FieldScene_RunVariantStep(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Audio_PlayCue(0x134);
        ColorBuffer_ApplyTarget(0x203a52, 1);
    } else {
        Audio_PlayCue(0x121);
        ColorBuffer_ApplyTarget(0x10000, 1);
    }
    ColorBuffer_Interpolate(b);
    if (c != 0) {
        Event_Wait(c);
    }
}

void FieldScene_RunStepByRuntimeBits(s32 a)
{
    if ((gFrameCount & 2) != 0) {
        Object_SetPartPalettes(a, 7);
    } else {
        Object_SetPartPalettes(a, 0);
    }
    if ((gFrameCount & 15) == 0) {
        SoruFunka_SpawnEffectPair(a);
    }
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = &gFrameCount;

    if (*p & 1) {
        Object_SetPartPalettes(a, IwramUnsignedRemainder(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        SoruFunka_SpawnEffectPair(a);
    }
}

void SceneEffect_UpdateArcOverAnchor(struct Actor_02002e0c *self)
{
    struct Actor_02002e0c *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(self);
        return;
    }

    amplitude = Math_Sin(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}

void SceneEffect_UpdateAnchoredRiseArc(struct Actor_02002e5c *obj)
{
    struct Actor_02002e5c *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(obj);
        return;
    }

    amp = Math_Sin(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}
