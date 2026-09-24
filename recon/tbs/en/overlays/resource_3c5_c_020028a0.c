/* NONMATCHING: 1236 of 1240 bytes, 275 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: the game-state base lands in r8 instead of r6 and the area id is not copied to ip, so the first literal pool is not dumped after the Colosso-exit branch; the 0xaf entrance switch keeps an extra copy of its index. The panel actors (11-14) and the 0xae block match. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_ApplyRectsAtActors8And9(void);
void SceneState_ConfigureRegion82_7AndApply768(void);
void OverlayObject_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind);
void FieldScene_RunScene3c5SequenceA(s32 value);
void FieldScene_RunSupplementalSequenceOne(void);

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

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000000[];
extern u8 Data_000000ae[];
extern u8 Data_000000af[];
extern u8 Data_000000b0[];
extern u8 Data_000000b1[];

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Local_020028a0(void)
{
    struct FieldActor *actor;
    struct PanelActor *panel;
    s32 scene;
    union GameStateRows *rows;
    s32 x;
    s32 z;

    Engine_TaskWait(1);
    *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x204;
    rows = &Data_02000240_t;
    scene = rows->halves[224][0];
    if (scene != (s32)Data_000000b1) {
        rows->halves[289][0] = 1;
        rows->halves[288][0] = (s32)Data_000000b0;
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
        if (rows->halves[225][0] == 5 && Engine_GameFlagIsSet(0x109) == 0) {
            Engine_ActorSetPosition(9, 0x1380000, 0x1480000);
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
        switch (rows->halves[225][0]) {
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
            if (Engine_GameFlagIsSet(0x109) == 0) {
                FieldScene_RunSupplementalSequenceOne();
            }
            break;
        }
    } else if (scene == (s32)Data_000000ae) {
        Engine_ActorGet(8)->unknown_5a &= 0xfe;
        Engine_ActorGet(9)->unknown_5a &= 0xfe;
        Engine_ActorSetSpeed(8, 0x10000, 0x8000);
        Engine_ActorSetSpeed(9, 0x10000, 0x8000);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            if (rows->halves[225][0] == 1) {
                Engine_GameFlagSet(0x301);
            } else {
                Engine_GameFlagClear(0x301);
            }
        }
        if (Engine_GameFlagIsSet(0x988) == 0) {
            Engine_ActorSetPosition(10, -0x400000, -0x400000);
            Engine_ActorSetPosition(11, 0x1180000, 0x1280000);
            Engine_ActorSetPosition(12, 0x1380000, 0xf80000);
            Engine_ActorSetPosition(13, 0x1280000, 0xf80000);
            Engine_ActorSetPosition(14, 0x1400000, 0x1280000);
            Engine_ActorFaceDirection(11, 0, 0);
            Engine_ActorFaceDirection(12, 0xc000, 0);
            Engine_ActorFaceDirection(13, 0xc000, 0);
            Engine_ActorFaceDirection(14, 0x8000, 0);
            Engine_EventWait(5);
        } else if (Engine_GameFlagIsSet(0x989) != 0) {
            Engine_ActorSetPosition(10, 0x1380000, 0x1380000);
            Engine_ActorFaceDirection(10, 0xb000, 0);
            Engine_ActorFaceDirection(11, 0xb000, 0);
            Engine_ActorFaceDirection(12, 0xb000, 0);
            Engine_ActorFaceDirection(13, 0xb000, 0);
            Engine_ActorFaceDirection(14, 0xb000, 0);
            Engine_EventWait(5);
        }
        if (Engine_GameFlagIsSet(0x985) != 0) {
            Engine_ActorSetPosition(8, 0x1180000, 0xf00000);
            Engine_ActorSetPosition(9, 0x1480000, 0xf00000);
            Engine_ActorFaceDirection(8, 0x8000, 0);
            Engine_ActorFaceDirection(9, 0, 0);
            Call6(Engine_MapCopyCellAttributes, 81, 14, 4, 1, 17, 14);
        }
        if (Data_02000240_t.halves[225][0] == 3 && Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunSupplementalSequenceOne();
        }
    } else {
        Engine_ActorSetAnimation(12, 2);
    }
    return 0;
}
