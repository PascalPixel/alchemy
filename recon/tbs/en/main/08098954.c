/* Draft, not exact (2026-09-24): 304 of 304 bytes, 77 differing halfwords.
   Rewritten from the listing; every instruction but the register choice
   matches. Residual: the ROM keeps the position array base in r5 and walks
   the anchors with r7, copying r5 into r7 for the shard loop; here the base
   takes r7 (global allocation order) and the shard loop reuses it with no
   copy, two bytes shorter, padded back by the pool alignment. Tried:
   pointer locals for either block, one pointer for both loops, count-down
   and count-up loops, every declaration order. The twin at main:0809a294 is the same
   function with the other item-break spawner. */
#include "TYPES.H"
#include "SYSTEM.H"

#define BattleEffect_RunItemBreakScatter Func_08098954

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

extern struct BattleEffectScene *gEffectWork;

void BattleEffect_InitializeSharedScene(void);
void *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void set_target_position_from_magnitude_angle(
    void *object, s32 magnitude, s32 angle);
void Object_CommitPosition(void *object);
void Audio_PlayCue(s32 sound);
struct ScatterShard *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(void *object, s32 data);
void Func_080090d0(void *object);
void BattleFx_PrepareBufferInterpolation(void);

void BattleEffect_RunItemBreakScatter(void)
{
    struct BattleEffectScene *scene;
    struct ScatterShard *obj;
    void *anchors[2];
    s32 position[3];
    s32 *pos;
    void **walk;
    s32 index;
    s32 magnitude;

    scene = gEffectWork;
    BattleEffect_InitializeSharedScene();
    position[0] = scene->x;
    position[1] = scene->y + 0x100000;
    position[2] = scene->z;
    anchors[0] = BattleFx_SpawnItemBreakMode3(
        position[0] + 0x200000, position[1], position[2], 0x8000);
    anchors[1] = BattleFx_SpawnItemBreakMode3(
        position[0] - 0x200000, position[1], position[2], 0);

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
    pos = position;
    for (index = 23; index >= 0; index--) {
        pos[0] = scene->x;
        pos[1] = scene->y + 0x100000;
        pos[2] = scene->z;
        obj = Object_Spawn(0x11d, pos[0], pos[1], pos[2]);
        if (obj != 0) {
            ObjectDispatch_InitializeFar(obj, 0x0809f0d4);
            obj->lift = 0x20000;
            obj->speed = Random16() + 0x20000;
            obj->flag = 0;
            magnitude = Random16() * 24 + 0x80000;
            set_target_position_from_magnitude_angle(obj, magnitude, Random16());
        }
    }

    Func_080090d0(anchors[0]);
    Func_080090d0(anchors[1]);
    BattleFx_PrepareBufferInterpolation();
}
