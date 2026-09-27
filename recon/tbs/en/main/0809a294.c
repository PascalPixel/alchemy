/* Not-yet-C: complete 304-byte owner, two differing halfwords.
 * A typed SpawnShard boundary recovers the position base in r5, anchor walk
 * in r7 and later r5-to-r7 copy; all other instructions and pools match.
 * Remaining: sched2 emits the r3-to-r8 counter copy before r5-to-r7,
 * rather than after it. Counter insn166 precedes hoisted cursor insn356.
 * Moving the whole shard loop into the helper gives 284 bytes / 68 edits;
 * an explicit outer cursor loses the separate copy (304 / 21). A do-loop
 * latch preserves this two-halfword residual. Stop those scheduling axes.
 * The twin at 08098954 differs only in its item-break spawner. */
#include "TYPES.H"
#include "SYSTEM.H"

struct BattleEffectScene {
    u8 reserved_00[4];
    s32 x;
    s32 y;
    s32 z;
};

struct ScatterShard {
    u8 reserved_00[0x30];
    s32 speed;                      /* 0x30 */
    s32 lift;                       /* 0x34 */
    u8 reserved_38[0x55 - 0x38];
    u8 flag;                        /* 0x55 */
};

struct ScatterPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern struct BattleEffectScene *gEffectWork;

void BattleEffect_InitializeSharedScene(void);
void *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void set_target_position_from_magnitude_angle(
    void *object, s32 magnitude, s32 angle);
void Object_CommitPosition(void *object);
void Audio_PlayCue(s32 sound);
struct ScatterShard *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(void *object, s32 data);
void Func_080090d0(void *object);
void BattleFx_PrepareBufferInterpolation(void);

static __inline__ struct ScatterShard *SpawnShard(
    struct BattleEffectScene *scene, struct ScatterPosition *position)
{
    /* FAKEMATCH: the inline interface preserves the separate shard cursor. */
    position->x = scene->x;
    position->y = scene->y + 0x100000;
    position->z = scene->z;
    return Object_Spawn(0x11d, position->x, position->y, position->z);
}

void RunBattleEffect11(void)
{
    struct BattleEffectScene *scene;
    struct ScatterShard *obj;
    void *anchors[2];
    struct ScatterPosition position;
    void **walk;
    s32 index;
    s32 magnitude;

    scene = gEffectWork;
    BattleEffect_InitializeSharedScene();
    position.x = scene->x;
    position.y = scene->y + 0x100000;
    position.z = scene->z;
    anchors[0] = BattleFx_SpawnItemBreakMode1(
        position.x + 0x200000, position.y, position.z, 0x8000);
    anchors[1] = BattleFx_SpawnItemBreakMode1(
        position.x - 0x200000, position.y, position.z, 0);

    WaitFrames(15);
    walk = anchors;
    for (index = 0; index < 2; index++) {
        obj = *walk++;
        if (obj != 0)
            set_target_position_from_magnitude_angle(
                obj, 0x180000, *(u16 *)((u8 *)obj + 6));
    }

    Object_CommitPosition(anchors[0]);
    Audio_PlayCue(134);
    for (index = 23; index >= 0; index--) {
        obj = SpawnShard(scene, &position);
        if (obj != 0) {
            ObjectDispatch_InitializeFar(obj, 0x0809f0d4);
            obj->speed = Random16() + 0x20000;
            obj->lift = 0x20000;
            obj->flag = 0;
            magnitude = Random16() * 24 + 0x80000;
            set_target_position_from_magnitude_angle(obj, magnitude, Random16());
        }
    }

    Func_080090d0(anchors[0]);
    Func_080090d0(anchors[1]);
    BattleFx_PrepareBufferInterpolation();
}
