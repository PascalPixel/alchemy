/* Near miss: listing 53 instructions, draft 53, common 52, differ 2; the rest differs in instruction order. */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_RUNTIME.H"
#include "IWRAM_CALL.H"

struct EffectSprite {
    u8 unknown_00[4];
    u16 attr0_0 : 13;
    u16 color : 1;
    u16 attr0_14 : 2;
    u16 attr1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[2];
    s32 x;
    s32 y;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_1c[2];
    u16 depth;
    u8 unknown_20[8];
};

struct EffectParticle {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

struct RiseWork {
    u8 unknown_0000[0x18a8];
    struct ObjectRuntime *object;
    s16 done;
    s16 timer;
};

struct AnchorWork {
    u8 unknown_0000[0x1930];
    struct ObjectRuntime *anchor;
};

struct ModeEvent {
    u8 unknown_000[352];
    u16 cue;
};

void Func_080eb01c(struct EffectSprite *sprite);
void Camera_WorldToScreen(s32 *position);
void BattleFx_IntegrateVector3(struct EffectParticle *particle, s32 frames, s32 flags);
struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
s32 Trig_Sin(s32 angle);

void Func_080e70f8(s32 id)
{
    struct ModeEvent *event = Ram_HeapSlots->event_work;
    struct ObjectRuntime *anchor;
    struct ObjectRuntime *object;

    /* FAKEMATCH: ⚓️ loads the event work before it builds the anchor offset */
    asm volatile("");
    anchor = ((struct AnchorWork *)Ram_HeapSlots->battle_fx_work[0])->anchor;
    object = Object_GetById(id);

    if (anchor != NULL) {
        object->x = anchor->x;
        object->y = anchor->y;
        object->z = anchor->z;
    }
    if (*((s8 *)event + 394) == 3) {
        object->motion_flags = 2;
        object->terrain_height = 0;
    } else {
        object->motion_flags = 3;
    }
    object->gravity = 0x170a;
    WaitFrames(50);
    event->cue = id | 0x1000;
}
