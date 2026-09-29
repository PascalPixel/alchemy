#include "HAIDIA_BABI.H"

/* The Mythril Bag scene and the sparkles that rise from the bag. */

/* A sparkle that follows its anchor while it rises. The anchor pointer is
 * read before the frame counter is stored: the reference hoists
 * `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Sparkle {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Sparkle *anchor;         /* 0x68 */
};

/* The bag's two motion scripts, in the overlay's read-only data. */
extern s32 gHaidiaBabiBagLiftScript[];
extern s32 gHaidiaBabiBagShowScript[];

void ObjectDispatch_WaitForValue16();
void HaidiaBabi_SpawnEffectPair();
/* The overlay's veneer into the resident unsigned remainder. */
u32 Engine_MathModulo();

void ActorPresentation_SetTwoSceneCells(void)
{
    {
        s32 extent = 2;

        Map_CopyCellsTo(22, 85, 25, 85, extent, extent);
    }
    {
        s32 extent = 25;

        Map_CopyCellAttributes(25, 15, 2, 2, extent, extent);
    }
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 rec7;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(18, 0x1e00000, 0xca0000);
    Task_Wait(1);
    Camera_FollowActor(18, 1);
    rec7 = 0;
    actor = (struct FieldActor *)Value4(Engine_ObjectCreate, 22, 0x1480000, 0x20000, 0xc30000);
    actor->motion_flags = rec7;
    sprite = actor->sprite;
    actor->y.fixed = 0x50000;
    sprite->part_count = rec7;
    sprite->full_color = 0;
    sprite->palette = 0;
    rec7 = Value2(Engine_HeapAllocate, 17, 0x608);
    Item_LoadIcon(ITEM_MYTHRIL_BAG);
    Vram_Load(sprite->vram_block, 128, rec7 + 0x400);
    Heap_Release(17);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_OpenScreen();
    Actor_SetSpeed(18, 0x10000, 0x8000);
    Actor_WalkToAndWait(18, 0x1e0, 176);
    Actor_WalkToAndWait(18, 0x1a4, 164);
    Actor_WalkToAndWait(18, 0x146, 185);
    Actor_FaceDirection(18, 0x4000, 10);
    Engine_ObjectSetScript(actor, gHaidiaBabiBagLiftScript);
    ObjectDispatch_WaitForValue16(actor);
    Object_SetScript(actor, gHaidiaBabiBagShowScript);
    ObjectDispatch_WaitForValue16(actor);
    Event_Wait(20);
    Engine_ObjectDispatchRelease(actor);
    Actor_Jump(18, 2, 20);
    Actor_FaceDirection(18, 0, 40);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(22);
}

void SceneEffect_UpdateObjectByFrameParity(u8 *obj)
{
    if ((gFrameCount & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((gFrameCount & 15) == 0) {
        HaidiaBabi_SpawnEffectPair(obj);
    }
}

void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    volatile s32 *frames = (volatile s32 *)&gFrameCount;

    if ((*frames & 1) != 0) {
        Object_SetPartPalettes(obj, Engine_MathModulo((s32)((u32)*frames >> 1), 6));
    }
    if ((*frames & 15) == 0) {
        HaidiaBabi_SpawnEffectPair(obj);
    }
}

void OverlayObject_ApplyRandomSlotOnOddFrames(s32 obj)
{
    volatile s32 *frames = (volatile s32 *)&gFrameCount;

    if ((*frames & 1) != 0) {
        s32 slot = Engine_MathModulo((u32)*frames >> 1, 6);

        Object_SetPartPalettes(obj, slot);
    }
}

void SceneEffect_UpdateAnchoredRiseFrame(struct Sparkle *self)
{
    struct Sparkle *anchor;
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

void OverlayObject_UpdateArcFromAnchor(struct Sparkle *obj)
{
    struct Sparkle *anchor;
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
