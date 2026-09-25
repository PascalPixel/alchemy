#include "TYPES.H"
#include "FIELD_EVENT.H"

struct SwitchCell {
    u32 x;
    u32 z;
};

struct SwitchEffect {
    s32 active;
    u8 unknown_04[0x5f];
    u8 finished;
};

struct FieldActor *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind);
void OverlayObject_WaitUntilIdle(struct FieldActor *object);
struct SwitchEffect *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, const void *script);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

extern struct SwitchCell Data_0200d128[];
extern u8 Data_0200d488[];
extern u8 Data_0200d508[];
extern u16 Data_0200dd12[];

/* Venus Lighthouse room: after actors 9 to 11 are pushed, each block that
 * rests on one of the four switch cells and shares its cell with no other
 * block sinks into it and marks it. Once all three are down, two effects
 * play and flag 0x302 (block 9 on the first switch) or 0x303 is set, then
 * 0x304. */
void VinasuHeya_SettlePushedBlocks(void)
{
    struct FieldActor *marker = 0;
    struct FieldActor *leader = Engine_ActorGet(0);
    struct FieldActor *block;
    struct FieldActor *other;
    struct SwitchEffect *first;
    struct SwitchEffect *second;
    u8 *flags;
    u32 id;
    u32 i;
    u32 slot;
    s32 priority;

    Engine_EventBegin();
    Call6((void (*)())Engine_MapCopyCellAttributes, 108, 39, 13, 7, 44, 39);
    for (id = 9; id <= 11; id++) {
        block = Engine_ActorGet(id);
        flags = &block->priority_flags;
        if (*flags != 2) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 47, 39, 1, 1, block->x.fixed >> 20, block->z.fixed >> 20);
        } else {
            Call6((void (*)())Engine_MapCopyCellAttributes, 46, 39, 1, 1, block->x.fixed >> 20, block->z.fixed >> 20);
        }
        slot = 5;
        for (i = 0; i <= 3; i++) {
            if (block->x.fixed >> 20 == Data_0200d128[i].x && block->z.fixed >> 20 == Data_0200d128[i].z
                && block->y.fixed >= 0) {
                slot = i;
                break;
            }
        }
        if (slot == 5) {
            continue;
        }
        for (i = 9; i <= 11; i++) {
            other = Engine_ActorGet(i);
            if (id != i && block->x.fixed >> 20 == other->x.fixed >> 20
                && block->z.fixed >> 20 == other->z.fixed >> 20) {
                slot = 5;
                break;
            }
        }
        if (slot == 5) {
            continue;
        }
        priority = leader->sprite->priority;
        if (leader->z.fixed >> 20 <= Data_0200d128[slot].z) {
            marker = OverlayObject_PrepareObjectWithCommand15(block->x.fixed, block->y.fixed,
                                                              block->z.fixed - 0x40000, 20);
            Engine_ActorSetSpritePriority(0, 3);
        }
        Engine_ActorSetSpriteFlags(Engine_ActorGet(id), 0);
        block->unknown_22 = 0;
        block->motion_flags = 3;
        *(s32 *)&block->unknown_44[0] = 0;
        *(s32 *)&block->unknown_44[4] = 0x1999;
        Call6((void (*)())Engine_MapCopyCellAttributes, 42, 41, 1, 1, Data_0200d128[slot].x, Data_0200d128[slot].z);
        OverlayObject_WaitUntilIdle(block);
        Engine_AudioPlayCue(188);
        block->collision_flags = 0;
        block->motion_flags = 0;
        block->y.fixed = -0x100000;
        Engine_ActorSetSpritePriority(id, 3);
        {
            s32 two = 2; /* FAKEMATCH: a block-local word keeps the 2 from being held for the later & 2 */

            *flags = two;
        }
        Call6((void (*)())Engine_MapCopyCellAttributes, 46, 39, 1, 1, Data_0200d128[slot].x, Data_0200d128[slot].z);
        Engine_ActorSetSpritePriority(0, priority);
        Engine_ActorGet(0)->priority_flags |= 1;
        if (marker != 0) {
            Engine_ObjectDispatchRelease(marker);
        }
        if (Engine_GameFlagIsSet(0x304)) {
            Engine_EventEnd();
            return;
        }
        if ((Engine_ActorGet(9)->priority_flags & Engine_ActorGet(10)->priority_flags
             & Engine_ActorGet(11)->priority_flags & 2) == 0) {
            continue;
        }
        first = SceneEffect_SpawnEffect284AtCell(888, 680, Data_0200d488);
        second = SceneEffect_SpawnEffect284AtCell(888, 680, Data_0200d508);
        while (first->active != 0 || second->active != 0) {
            if (first->finished != 0 || second->finished != 0) {
                Engine_EventWait(30);
                Engine_AudioPlayCue(158);
                Engine_MapAnimateCells(Data_0200dd12, 109, 37);
                Call6((void (*)())Engine_MapCopyCellAttributes, 45, 37, 1, 1, 45, 38);
                if (Engine_ActorGet(9)->x.fixed >> 20 == Data_0200d128[0].x
                    && Engine_ActorGet(9)->z.fixed >> 20 == Data_0200d128[0].z) {
                    Engine_GameFlagSet(0x302);
                } else {
                    Engine_GameFlagSet(0x303);
                }
                Engine_GameFlagSet(0x304);
                break;
            }
            Engine_TaskWait(1);
        }
    }
    Engine_EventEnd();
}
