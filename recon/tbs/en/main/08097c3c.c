/* DRAFT of FunctionHead_08097c3c: the effect that lets the player push a
   target one step: a preview object shows the step tried, and a step that
   fits moves the target (and the source object with it when the source
   stands in the way and its next cell is free).
   Not exact: 836 of 836 bytes, the blocks in the reference's order, and
   r5 to r10 in its roles. Remaining: the reference keeps the source pointer
   on the stack ([sp+20]) and gives r11 to the loop's copy of 0x100000, with
   that constant in r5 before the loop; here the source takes r11 and the
   constant is rebuilt at each use. The reference also reloads the cell mask
   after the first cell test and parks it in ip. */
#include "TYPES.H"

struct FxPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct FxObject {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;
    s32 z;
    s32 ground;                 /* 0x14 */
    u8 unknown_18[10];
    u8 mode;                    /* 0x22 */
    u8 unknown_23;
    s32 velocity_x;             /* 0x24 */
    s32 velocity_y;             /* 0x28 */
    s32 velocity_z;             /* 0x2c */
    s32 speed;                  /* 0x30 */
    s32 lift;                   /* 0x34 */
    u8 unknown_38[12];
    s32 gravity;                /* 0x44 */
    u8 unknown_48[13];
    u8 flag;                    /* 0x55 */
    u8 unknown_56[4];
    u8 unknown_5a;
    u8 unknown_5b;
    u8 unknown_5c[12];
    struct FxObject *linked;    /* 0x68 */
    void *callback;             /* 0x6c */
};

struct EffectWork {
    s32 direction;
    s32 x;
    s32 y;
    s32 z;
    struct FxObject *source;    /* 0x10 */
    struct FxObject *target;    /* 0x14 */
    s16 source_id;              /* 0x18 */
    u8 unknown_1a[0x1e];
    void *callback;             /* 0x38 */
    const u8 *script;           /* 0x3c */
    u8 unknown_40[4];
    u8 palette;                 /* 0x44 */
};

extern struct EffectWork *gEffectWork;
extern const u8 BattleFx_SourceHoldScript[];
extern const u8 Data_0809f118[];
extern volatile u32 gKeysHeld;
extern volatile u32 gKeyState;
extern volatile u32 gFrameCount;

void BattleEffect_InitializeSharedScene(void);
void ObjectDispatch_InitializeFar(struct FxObject *object, const u8 *script);
struct FxObject *BattleFx_StartItemBreak(struct FxObject *source);
void BattleFx_PrepareBufferInterpolation(void);
void BattleFx_SnapScaleToFull(struct FxObject *object);
void WaitFrames(s32 frames);
void Object_SetMoveTargetFar(struct FxObject *object, s32 x, s32 y, s32 z);
void Object_CommitPosition(struct FxObject *object);
void ObjectGroup_ApplyRandomChildValues(void);
void AudioCommand_PlayFar(s32 cue);
s32 BattleFx_GetCycledTableWord(u32 keys);
void Object_SetMode(struct FxObject *object, s32 mode);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct FxPosition *position);
struct FxObject *ScriptObject_FindOverlappingEntryFar(struct FxObject *object, struct FxPosition *position);
s32 Object_CheckMovementCollision(struct FxObject *object, struct FxPosition *position);
s32 Map_GetCellAttributeLowNibbleFar(s32 layer, s32 x, s32 z);
struct FxObject *Object_GetById(s32 id);
void Animation_ApplyChildValuesFar(struct FxObject *object, s32 value);
void EffectRuntime_StopCurrentObject(void);
void UpdateRisingParticleBurst(struct FxObject *object);

void FunctionHead_08097c3c(void)
{
    struct FxPosition position;
    struct FxPosition nearby;
    struct EffectWork *work;
    struct FxObject *source;
    struct FxObject *target;
    struct FxObject *preview;
    struct FxObject *hit;
    s32 x;
    s32 z;
    s32 angle;
    s32 moved;
    s32 direction;
    s32 keys;
    s32 cell_x;
    s32 cell_z;
    struct FxPosition *p;

    work = gEffectWork;
    source = work->source;
    target = work->target;
    angle = work->direction + 0x8000;
    moved = 0;
    if (target == NULL)
        return;
    BattleEffect_InitializeSharedScene();
    source->linked = target;
    ObjectDispatch_InitializeFar(source, BattleFx_SourceHoldScript);
    preview = BattleFx_StartItemBreak(source);
    if (preview == NULL) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    preview->linked = target;
    p = &position;
    p->x = target->x;
    p->y = target->y + 0x100000;
    p->z = target->z;
    Vector_AddPolarOffset(0x100000, angle, p);
    Object_SetMoveTargetFar(preview, p->x, p->y, p->z);
    BattleFx_SnapScaleToFull(preview);
    preview->speed = 0x40000;
    preview->lift = 0x8000;
    preview->flag = 4;
    target->callback = ObjectGroup_ApplyRandomChildValues;
    target->speed = 0x6666;
    target->lift = 0x3333;
    target->unknown_5a = moved;
    target->mode = 2;
    for (;;) {
        WaitFrames(1);
        keys = gKeyState & 0x303;
        if (keys != 0)
            break;
        direction = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
        if (direction == 0xffff) {
            position.x = target->x;
            position.y = target->y + 0x100000;
            position.z = target->z;
            Vector_AddPolarOffset(0x100000, angle, &position);
            Object_SetMoveTargetFar(preview, position.x, position.y, position.z);
            Object_SetMode(preview, 1);
            preview->velocity_x = 0;
            preview->velocity_y = 0;
            preview->velocity_z = 0;
            continue;
        }
        position.x = target->x;
        position.y = target->y + 0x100000;
        position.z = target->z;
        Vector_AddPolarOffset(0x100000, angle, &position);
        Vector_AddPolarOffset(0x20000, direction, &position);
        Object_SetMoveTargetFar(preview, position.x, position.y, position.z);
        Object_CommitPosition(preview);
        position.x = target->x;
        position.y = target->y;
        position.z = target->z;
        Vector_AddPolarOffset(0x100000, direction, &position);
        nearby.x = target->x;
        nearby.y = target->y;
        nearby.z = target->z;
        Vector_AddPolarOffset(0x200000, direction, &nearby);
        if (Object_CheckMovementCollision(target, &position) > 0)
            goto blocked;
        hit = ScriptObject_FindOverlappingEntryFar(target, p);
        if (hit == NULL)
            goto move;
        if (hit != source)
            goto blocked;
        cell_x = source->x & 0xfff00000;
        cell_z = source->z & 0xfff00000;
        if (cell_x == (p->x & 0xfff00000) && cell_z == (p->z & 0xfff00000))
            goto blocked;
        if (cell_x != (nearby.x & 0xfff00000) || cell_z != (nearby.z & 0xfff00000))
            goto move;
        if (Map_GetCellAttributeLowNibbleFar(source->mode, nearby.x, nearby.z) == 0)
            goto move_source;
    blocked:
        Object_SetMode(preview, 4);
        if ((gFrameCount & 15) == 0)
            AudioCommand_PlayFar(114);
        continue;
    move_source:
        moved = 1;
    move:
        AudioCommand_PlayFar(175);
        x = position.x;
        z = position.z;
        Object_SetMode(preview, Data_0809f118[(u16)(angle - direction) >> 14]);
        WaitFrames(15);
        target->unknown_5b = 0;
        target->speed = 0x3333;
        target->lift = 0x3333;
        Object_SetMoveTargetFar(target, position.x, position.y, position.z);
        preview->flag = 0;
        preview->speed = 0x3333;
        preview->lift = 0x3333;
        Vector_AddPolarOffset(0x100000, direction, &position);
        Object_SetMoveTargetFar(preview, position.x, position.y + 0x100000, position.z);
        if (moved == 1) {
            Object_GetById(work->source_id)->unknown_5a &= 0xfe;
            source->speed = 0x3333;
            source->lift = 0x3333;
            Object_SetMoveTargetFar(source, nearby.x, nearby.y, nearby.z);
        }
        Object_CommitPosition(target);
        target->x = x;
        target->z = z;
        target->velocity_x = 0;
        target->velocity_z = 0;
        break;
    }
    Animation_ApplyChildValuesFar(target, work->palette);
    ObjectDispatch_InitializeFar(target, work->script);
    target->callback = work->callback;
    EffectRuntime_StopCurrentObject();
    if (moved == 1)
        Object_GetById(work->source_id)->unknown_5a |= 1;
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(preview);
}
