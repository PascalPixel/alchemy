/* Steps the actor's column row by row until the terrain rises above its own
 * level, then prepares object 223 or 253 at that step and waits for the actor
 * to fall to it. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_080091a8(s32 layer, s32 x, s32 z);
s32 OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
void SceneActor_WaitHeightBelowLimit(struct FieldActor *actor, s32 limit);

s32 TakaraHashira_LowerActorToLedge(s32 id, s32 far)
{
    struct FieldActor *actor = Engine_ActorGet(id);
    s32 found;
    s32 object;
    s32 height;
    s32 level;
    s32 top;
    u32 row;
    u32 rows;
    s32 z;
    s32 y;
    s32 x;
    s32 kind;

    found = 0;
    object = found;
    Engine_ActorSetSpriteFlags(actor, found);
    height = Main_080091a8(2, actor->x.fixed, actor->z.fixed);
    level = height / 0x100000;
    rows = level;
    if (level < 0) {
        rows = -level;
    }
    rows++;
    for (row = 0; row <= rows; row++) {
        top = Main_080091a8(actor->unknown_22, actor->x.fixed, (row << 20) + actor->z.fixed);
        top /= 0x100000;
        if (level < top) {
            x = ((actor->x.fixed >> 20) << 20) + 0x80000;
            if (far == 0) {
                row += 2;
                z = ((row + (actor->z.fixed >> 20)) << 20) + 0x20000;
                y = top << 20;
                kind = 223;
            } else {
                row += 3;
                z = ((row + (actor->z.fixed >> 20)) << 20) - 0x20000;
                y = top << 20;
                kind = 253;
            }
            /* FAKEMATCH: a do-while(0) around the object and height lets sched2
               issue the height arithmetic first, as the reference does */
            do {
                object = OverlayObject_PrepareObject(x, y, z, kind);
                height = actor->z.fixed - z + y;
            } while (0);
            found = 1;
            break;
        }
    }
    SceneActor_WaitHeightBelowLimit(actor, height);
    actor->x.fixed = 0;
    actor->y.fixed = 0;
    actor->z.fixed = 0;
    if (object != 0) {
        Engine_ObjectDispatchRelease((void *)object);
    }
    return found;
}
