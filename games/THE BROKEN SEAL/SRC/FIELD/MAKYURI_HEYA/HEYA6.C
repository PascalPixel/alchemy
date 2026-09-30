#include "PROBE.H"
#include "MAP_RENDER_WORK.H"

void DisplayBlend_EnableRunScript(void);

/* The map layer the lift is drawn on, scrolled by its vertical offset. */
struct LiftLayer {
    u8 unknown_00[12];
    s32 offset;
    u8 unknown_10[12];
    s32 unknown_1c;
};

/* The lift rises into the room with the leader and actor 13 on it, easing to
 * a stop; the leader hops off onto the landing, and the lift sinks back down
 * the shaft, speeding up, before the landing's cells close over it. */
void MakyuriHeya_RideLift(void)
{
    struct LiftLayer *layer;
    s32 speed;

    layer = (struct LiftLayer *)((u8 *)gMapWork + 356);
    speed = 0x9c28;
    layer->offset = 0x04890000;
    layer->unknown_1c = 0;
    Actor_Get(0)->motion_flags = 0;
    Actor_Get(0)->z.fixed += -0x890000;
    Actor_Get(0)->target_z = Actor_Get(0)->z.fixed;
    Actor_Get(13)->motion_flags = 0;
    Actor_SetPosition(13, 170 << 18, 220 << 17);
    Actor_Get(13)->z.fixed += -0x890000;
    Actor_Get(13)->target_z = Actor_Get(13)->z.fixed;
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(60);
    DisplayBlend_EnableRunScript();
    Audio_PlayCue(223);
    for (;;) {
        layer->offset -= speed;
        Actor_Get(0)->z.fixed += speed;
        Actor_Get(0)->target_z = Actor_Get(0)->z.fixed;
        Actor_Get(13)->z.fixed += speed;
        Actor_Get(13)->target_z = Actor_Get(13)->z.fixed;
        if (layer->offset > 0x04000000) {
            if ((gFrameCount & 15) == 0 && speed > 0xccb)
                speed += -0x560;
            Task_Wait(1);
            continue;
        }
        break;
    }
    layer->offset = 0x04000000;
    Map_Redraw();
    Task_Wait(2);
    Actor_Get(0)->motion_flags = 3;
    Actor_Get(13)->z.fixed = 220 << 17;
    Actor_Get(13)->target_z = Actor_Get(13)->z.fixed;
    Event_Wait(30);
    Actor_WalkToAndWait(0, 704, 584);
    *(s32 *)Actor_Get(0)->unknown_44 = 0;
    Actor_SetAnimation(0, 6);
    Event_Wait(6);
    Actor_SetAnimation(0, 7);
    Actor_Get(0)->speed = 0x30000;
    Actor_Get(0)->acceleration = 0x20000;
    Audio_PlayCue(152);
    Actor_Get(0)->velocity_y = 0x40000;
    Actor_SetSpriteFlags(Actor_Get(0), 0);
    Actor_SetDestination(0, 736, 584);
    Actor_WaitForMove(0);
    Actor_SetSpriteFlags(Actor_Get(0), 1);
    *(s32 *)Actor_Get(0)->unknown_44 = 0x4000;
    Actor_SetAnimation(0, 6);
    Event_Wait(6);
    Actor_FaceDirection(0, 0x8000, 30);
    Audio_PlayCue(223);
    for (;;) {
        layer->offset += speed;
        Actor_Get(13)->z.fixed -= speed;
        Actor_Get(13)->target_z = Actor_Get(13)->z.fixed;
        if (layer->offset >= 0x04890000)
            break;
        if ((gFrameCount & 7) == 0 && speed <= 0xcccc)
            speed += 0x1999;
        Task_Wait(1);
    }
    layer->offset = 0x04000000;
    Map_CopyCellsTo(45, 91, 40, 91, 5, 4);
    Map_CopyCellAttributes(104, 34, 5, 4, 40, 34);
    Map_Redraw();
    Task_Wait(2);
    Actor_SetPosition(13, 0, 0);
    Event_Wait(30);
    Actor_Get(0)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
}

void MakyuriHeya_CastPsynergyAtActor11(void)
{
    u8 *p5;
    u8 *addr;
    u8 v;

    p5 = *(volatile s32 *)gEffectWork;
    Actor_SetPosition(11, 0x3480000, 0x2580000);
    Psynergy_Begin(93, 1);
    Psynergy_SetTarget(3, 11);
    addr = p5 + 0x71c;
    v = *addr | 8;
    *addr = v;
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
}

void SceneState_SetRecordTableValue(s32 key, s32 value)
{
    s32 slot = PartyInventory_FindOwner(key);

    if (slot != -1) {
        s32 index = Inventory_Find(slot, key);

        if (index != -1) {
            Owner_GetState(slot)->tbl[index] = value;
        }
    }
}
