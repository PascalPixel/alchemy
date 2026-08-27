#include "types.h"

/*
 * Companion to RunBattleEffect07 (0x08098954) and Func_08099da4 (the
 * mode-11 twin): this is the mode-7 case handler inside the second,
 * "targeted" battle-effect dispatcher Func_08096960 --
 *     case 7: Func_08098848(target_id); break;
 * target_id is loaded into r0 by every case in that switch, but this
 * body never reads r0 (confirmed against the retained assembly: r0 is
 * first written, never read, before its first use), matching the same
 * pattern already adopted for RunBurstParticleMainObject (0x080985a8)
 * and StartOrbitingParticleEffect (0x08099128), which also ignore the
 * argument their caller passes.
 *
 * scene->0x10 (main_object) and scene->0x14 (child) are the exact same
 * two fields read by Func_08099da4 (the mode-11 sibling), which performs
 * an almost byte-identical opening sequence: guard on child != 0, call
 * Func_08097384, link main_object->0x68 = child, then
 * Object_SetCallback(main_object, 0x0809f0bc), then spawn two objects at
 * scene->{x,y,z} +/- 0x200000 in x, +0x100000 in y. Func_08099da4 spawns
 * through SpawnItemBreakEffectMode1 (0x0809a3c4); this owner spawns
 * through SpawnItemBreakEffectMode3 (0x08098a84), matching the spawn
 * helper RunBattleEffect07 (0x08098954) itself uses for the same mode.
 */

struct BattleEffectScene {
    u8 reserved_00[4];
    s32 x;
    s32 y;
    s32 z;
    void *main_object;
    void *child;
};

extern struct BattleEffectScene *Data_03001f30;

void Func_08097384(void);
void *SpawnItemBreakEffectMode3(s32 x, s32 y, s32 z, s32 angle);
void Object_SetCallback(void *object, const void *callback);
void WaitFrames(s32 frames);
void ObjectMotion_SetTargetPositionFromMagnitudeAngle(
    void *object, s32 magnitude, s32 angle);
void Object_CommitPosition(void *object);
void Audio_PlayCue(s32 sound);
void ObjectGroup_ApplyRandomChildValues(void);
void UpdateRisingParticleBurst(void *effect);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_08098848(void)
{
    struct BattleEffectScene *scene;
    void *main_object;
    void *child;
    void *anchors[2];
    s32 position[3];
    s32 x, y, z;
    s32 index;
    s32 start_y;

    scene = Data_03001f30;
    child = scene->child;
    main_object = scene->main_object;
    if (child == 0)
        return;

    Func_08097384();
    *(void **)((u8 *)main_object + 0x68) = child;
    Object_SetCallback(main_object, (const void *)0x0809f0bc);

    x = scene->x;
    position[0] = x;
    y = scene->y + 0x100000;
    position[1] = y;
    z = scene->z;
    position[2] = z;
    anchors[0] = SpawnItemBreakEffectMode3(x + 0x200000, y, z, 0x8000);
    anchors[1] = SpawnItemBreakEffectMode3(
        position[0] - 0x200000, position[1], position[2], 0);

    WaitFrames(15);
    for (index = 0; index < 2; index++) {
        void *anchor = anchors[index];
        if (anchor != 0)
            ObjectMotion_SetTargetPositionFromMagnitudeAngle(
                anchor, 0xe0000, *(u16 *)((u8 *)anchor + 6));
    }

    Object_CommitPosition(anchors[0]);
    *(void (**)(void))((u8 *)child + 0x6c) = ObjectGroup_ApplyRandomChildValues;
    Audio_PlayCue(130);
    /* Reference schedules the reload of anchors[0] (needed for the guard
       below) ahead of this store; every source-level ordering tried here
       keeps the two swapped relative to one another (2 differing
       halfwords, wrong_instructions=0, objdiff class=ordering) -- a
       GCC-2.96 -O2 scheduler placement, not a semantic difference. */
    *(u8 *)((u8 *)child + 0x55) = 4;

    start_y = *(s32 *)((u8 *)child + 0xc);
    if (anchors[0] != 0 && anchors[1] != 0 &&
        start_y <= start_y + 0x200000) {
        do {
            *(s32 *)((u8 *)anchors[0] + 0xc) += 0x4000;
            *(s32 *)((u8 *)anchors[1] + 0xc) += 0x4000;
            *(s32 *)((u8 *)child + 0xc) += 0x4000;
            WaitFrames(1);
        } while (*(s32 *)((u8 *)child + 0xc) <= start_y + 0x200000);
    }

    UpdateRisingParticleBurst(anchors[0]);
    UpdateRisingParticleBurst(anchors[1]);
    BattleEffect_PrepareBufferInterpolation();
}
