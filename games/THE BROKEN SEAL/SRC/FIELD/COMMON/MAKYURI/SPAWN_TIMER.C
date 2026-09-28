#include "TYPES.H"
#include "FIELD_EVENT.H"

struct SpawnPoint;

struct EventSpawns {
    u8 unknown_00[20];
    struct SpawnPoint *points[1];
};

struct SceneTimer {
    u8 unknown_00[8];
    s32 count;
};

/* The IWRAM event globals: the event work, and at +0x20 the scene work. */
struct EventGlobals {
    struct EventSpawns *event;
    u8 unknown_04[0x1c];
    struct SceneTimer **scene;
};

void OverlayObject_SpawnKind24AtObject(struct SpawnPoint *point);

extern struct EventGlobals gWork;

/* Mercury Lighthouse spawn timer: count the scene timer down, and when it runs out spawn an object at the current spawn point and restart it at 10 to 39 frames. */
void Makyuri_TickSpawnTimer(void)
{
    struct SceneTimer *timer;
    struct SpawnPoint *point;

    /* FAKEMATCH: the event work is reached back from the scene field's
     * address, so the pool holds the +0x20 address and the base is derived. */
    {
        struct SceneTimer ***scene = &gWork.scene;

        timer = **scene;
        point = ((struct EventGlobals *)((u8 *)scene - 0x20))->event->points[gGameState.selected_actor];
    }
    if (timer->count != 0) {
        timer->count--;
    } else {
        OverlayObject_SpawnKind24AtObject(point);
        timer->count = ((u32)(Engine_RandomNext() * 30) >> 16) + 10;
    }
}
