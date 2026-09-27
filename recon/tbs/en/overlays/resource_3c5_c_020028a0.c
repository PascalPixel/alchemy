/* NONMATCHING: candidate 1256/reference 1240 bytes, 596 differing halfwords,
 * 193 aligned edits. Complete own-ROM extent 020028a0..02002d78 includes
 * the eight-entry entrance switch and pools at 2904, 29ec and 2d4c.
 * ENTRY.INC slot 0 calls this setup owner; all calls and field accesses audited.
 * 2026-09-26 baseline 1236/580/275 had no missing body calls. Correcting
 * SpawnConfiguredObject's pointer return and SupplementalSequenceOne's s32
 * argument gave 1240/580/271; all three spawn argument sequences then matched.
 * Shared GameState/EventWork ownership gave 1240/578/270 and restored the
 * transition-store before state-load ordering. Existing Actor_SetPosition,
 * Actor_SetSpeed and Actor_FaceDirection wrappers gave this retained model:
 * their argument and constant-reload sequences match throughout the 0xae arm.
 * Remaining: early pool placement, shared map-copy tail, scene value in ip,
 * entrance-switch index copy, and redundant zero before supplemental call.
 * Three structural trials closed; no declaration sweeps or DONE credit. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_ApplyRectsAtActors8And9(void);
void SceneState_ConfigureRegion82_7AndApply768(void);
u8 *OverlayObject_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind);
void FieldScene_RunScene3c5SequenceA(s32 value);
void FieldScene_RunSupplementalSequenceOne(s32 value);

struct PanelSprite {
    u8 unknown_00[9];
    u8 flags;
    u8 unknown_0a[0x14];
    u16 angle;
    u8 unknown_20[6];
    u8 fade;
};

struct PanelActor {
    u8 unknown_00[0x23];
    u8 priority_flags;
    u8 unknown_24[0x2c];
    struct PanelSprite *sprite;
    u8 unknown_54[5];
    u8 collision_flags;
    u8 unknown_5a[4];
    u16 delay;
};

extern u8 Data_00000000[];
extern u8 Data_000000ae[];
extern u8 Data_000000af[];
extern u8 Data_000000b0[];
extern u8 Data_000000b1[];

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 BabiIriguchi_SetupScene(void)
{
    struct FieldActor *actor;
    struct PanelActor *panel;
    s32 scene;
    struct GameState *state;
    s32 x;
    s32 z;
    s32 flag;

    Engine_TaskWait(1);
    gEventWork->start_transition = 0x204;
    state = &gGameState;
    scene = state->scene;
    if (scene != (s32)Data_000000b1) {
        state->retreat_entrance = 1;
        state->retreat_scene = (s32)Data_000000b0;
    } else {
        actor = Engine_ActorGet(12);
        x = actor->x.fixed >> 20;
        if (x == 20) {
            z = actor->z.fixed >> 20;
            if (z == 12) {
                Call6(Engine_MapCopyCellAttributes, 38, 12, 1, 1, x, z);
            }
        }
        return 0;
    }
    if (scene == (s32)Data_000000b0) {
        Engine_ActorSetChildValue(8, 6);
        Engine_ActorSetChildValue(9, 6);
        if (state->entrance == 5 && Engine_GameFlagIsSet(0x109) == 0) {
            Actor_SetPosition(9, 0x1380000, 0x1480000);
        }
        SceneState_ApplyRectsAtActors8And9();
        actor = Engine_ActorGet(9);
        actor->y.fixed = *(s32 *)Engine_ActorGet(9)->unknown_14;
        if (Engine_GameFlagIsSet(0x300) != 0) {
            Engine_ActorSetAnimation(10, 4);
            Engine_ActorGet(10)->collision_flags = 0xfe;
            SceneState_ConfigureRegion82_7AndApply768();
        }
        panel = (struct PanelActor *)Engine_ActorGet(11);
        panel->collision_flags = 0;
        panel->priority_flags = 0;
        panel->delay = 0;
        ((u8 *)panel->sprite)[9] |= 12;
        ((u8 *)panel->sprite)[38] = (s32)Data_00000000;
        panel->sprite->angle = 0xc000;
        Engine_ActorSetAnimation(11, 0);
        panel = (struct PanelActor *)Engine_ActorGet(12);
        panel->collision_flags = (s32)Data_00000000;
        panel->priority_flags = (s32)Data_00000000;
        panel->delay = 30;
        ((u8 *)panel->sprite)[9] |= 12;
        ((u8 *)panel->sprite)[38] = (s32)Data_00000000;
        panel->sprite->angle = 0x4000;
        Engine_ActorSetAnimation(12, 0);
        panel = (struct PanelActor *)Engine_ActorGet(13);
        panel->collision_flags = (s32)Data_00000000;
        panel->priority_flags = (s32)Data_00000000;
        panel->delay = 60;
        ((u8 *)panel->sprite)[9] |= 12;
        ((u8 *)panel->sprite)[38] = (s32)Data_00000000;
        panel->sprite->angle = 0x8000;
        Engine_ActorSetAnimation(13, 0);
        panel = (struct PanelActor *)Engine_ActorGet(14);
        panel->collision_flags = (s32)Data_00000000;
        panel->priority_flags = (s32)Data_00000000;
        panel->delay = 90;
        ((u8 *)panel->sprite)[9] |= 12;
        ((u8 *)panel->sprite)[38] = (s32)Data_00000000;
        panel->sprite->angle = 0x8000;
        Engine_ActorSetAnimation(14, 0);
    } else if (scene == (s32)Data_000000af) {
        switch (state->entrance) {
        case 10:
            Engine_GameFlagSet(0x980);
        case 11:
            if (Engine_GameFlagIsSet(0x980) != 0) {
                Call6(Engine_MapCopyCellsTo, 120, 7, 109, 7, 1, 3);
                Call6(Engine_MapCopyCellAttributes, 45, 8, 1, 1, 45, 9);
            }
            break;
        case 14:
            OverlayObject_SpawnConfiguredObject(0x1b80000, 0, 0x1220000, 223);
            Call6(Engine_MapCopyCellAttributes, 22, 13, 1, 1, 27, 13);
            FieldScene_RunScene3c5SequenceA(14);
            break;
        case 16:
            OverlayObject_SpawnConfiguredObject(0x1c00000, 0, 0x1220000, 223);
            Call6(Engine_MapCopyCellAttributes, 22, 12, 1, 1, 28, 10);
            FieldScene_RunScene3c5SequenceA(16);
            break;
        case 17:
            OverlayObject_SpawnConfiguredObject(0xe80000, 0, 0x2520000, 223);
            Call6(Engine_MapCopyCellAttributes, 22, 12, 1, 1, 14, 33);
            FieldScene_RunScene3c5SequenceA(17);
            break;
        case 12:
        case 13:
        case 15:
            flag = Engine_GameFlagIsSet(0x109);
            if (flag == 0) {
                FieldScene_RunSupplementalSequenceOne(flag);
            }
            break;
        }
    } else if (scene == (s32)Data_000000ae) {
        Engine_ActorGet(8)->unknown_5a &= 0xfe;
        Engine_ActorGet(9)->unknown_5a &= 0xfe;
        Actor_SetSpeed(8, 0x10000, 0x8000);
        Actor_SetSpeed(9, 0x10000, 0x8000);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            if (state->entrance == 1) {
                Engine_GameFlagSet(0x301);
            } else {
                Engine_GameFlagClear(0x301);
            }
        }
        if (Engine_GameFlagIsSet(0x988) == 0) {
            Actor_SetPosition(10, -0x400000, -0x400000);
            Actor_SetPosition(11, 0x1180000, 0x1280000);
            Actor_SetPosition(12, 0x1380000, 0xf80000);
            Actor_SetPosition(13, 0x1280000, 0xf80000);
            Actor_SetPosition(14, 0x1400000, 0x1280000);
            Actor_FaceDirection(11, 0, 0);
            Actor_FaceDirection(12, 0xc000, 0);
            Actor_FaceDirection(13, 0xc000, 0);
            Actor_FaceDirection(14, 0x8000, 0);
            Engine_EventWait(5);
        } else if (Engine_GameFlagIsSet(0x989) != 0) {
            Actor_SetPosition(10, 0x1380000, 0x1380000);
            Actor_FaceDirection(10, 0xb000, 0);
            Actor_FaceDirection(11, 0xb000, 0);
            Actor_FaceDirection(12, 0xb000, 0);
            Actor_FaceDirection(13, 0xb000, 0);
            Actor_FaceDirection(14, 0xb000, 0);
            Engine_EventWait(5);
        }
        if (Engine_GameFlagIsSet(0x985) != 0) {
            Actor_SetPosition(8, 0x1180000, 0xf00000);
            Actor_SetPosition(9, 0x1480000, 0xf00000);
            Actor_FaceDirection(8, 0x8000, 0);
            Actor_FaceDirection(9, 0, 0);
            Call6(Engine_MapCopyCellAttributes, 81, 14, 4, 1, 17, 14);
        }
        if (gGameState.entrance == 3) {
            flag = Engine_GameFlagIsSet(0x109);
            if (flag == 0) {
                FieldScene_RunSupplementalSequenceOne(flag);
            }
        }
    } else {
        Engine_ActorSetAnimation(12, 2);
    }
    return 0;
}
