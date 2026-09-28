#include "STAGED_MOTION.H"

void Effect_PlayStepSound(void)
{
    if ((*(u32 *)&gFrameCount & 15) == 0)
        Audio_PlayCue(0x83);
}

void FieldScene_RunScriptedStepEE4(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_DOOR_WONT_OPEN_FORCE_IMPACT, 1);
    Event_End();
}

void FieldScene_RunScene373SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (GameFlag_IsSet(0x241) != 0) {
        rec7 = GameFlag_IsSet(0x106);
        if (rec7 != 0) {
            goto L_02005a8a;
        }
        *(u8 *)(Object_GetById(22) + 91) = rec7;
        GameFlag_Clear(0x241);
    } else {
        if (GameFlag_IsSet(0x106) != 0) {
            *(u8 *)(Object_GetById(22) + 91) = 1;
            GameFlag_Set(0x241);
        }
    }
    L_02005a8a:;
}

void SceneActor_SetFlagByteBySlotZeroPosition(void)
{
    s32 *g = Actor_Get(ACTOR_PARTY_LEADER);
    u8 *q;
    if (GameFlag_IsSet(0x87a) != 0)
        q = Actor_Get(21);
    else
        q = Actor_Get(20);
    if (q != 0) {
        if (g[3] > 0xc80000)
            q[0x23] = 3;
        else
            q[0x23] = 1;
    }
}

s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Math_Sin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Math_Cos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Math_Cos(p[12] + 0x8000) / 8;
    a = Random_Next();
    b = Random_Next();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = Object_GetById();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = Runtime_AllocateHeapBlock(17, 0x608);
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateOrbitPosition;
    actor->state = zero;
}

void OverlayObject_UpdateOnFrameBit1(s32 p)
{

    if ((gFrameCount & 2) != 0)
        Object_SetPartPalettes(p, 7);
    else
        Object_SetPartPalettes(p, 0);
    if ((gFrameCount & 0xf) == 0)
        WorldMap_CreateLinkedEffects(p);
}

void SceneEffect_UpdateObjectOnOddFrames(s32 p)
{
    extern volatile u32 gFrameCount;

    if ((gFrameCount & 1) != 0)
        Object_SetPartPalettes(p, (gFrameCount >> 1) % 6);
    if ((gFrameCount & 0xf) == 0)
        WorldMap_CreateLinkedEffects(p);
}

void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p)
{
    extern volatile u32 gFrameCount;

    if ((gFrameCount & 1) != 0)
        Object_SetPartPalettes(p, (gFrameCount >> 1) % 6);
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
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
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}
