/* Draft of resource_38e 0x02008184 (SceneDialogue_RunActor9Message13c0): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgBiribinoHaveYouSeenBarricadeWe). The
 * listing keeps these rows until the draft is adopted. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"
extern u8 MsgBiribinoHaveYouSeenBarricadeWe[];

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};


struct SceneHandle {
    u8 unknown_00[9];
    u8 flags09;                     /* 0x09 */
};

struct SceneEntity {
    u8 unknown_00[0x23];
    u8 fp;                     /* 0x23 */
    u8 unknown_24[0x2c];
    struct SceneHandle *h;     /* 0x50 */
};

/* Entity and handle as the lobe-orbit callback reads them. */
struct SceneHandle_0200090c {
    u8 unknown_00[30];
    s16 field1e;                    /* 0x1e */
};

struct SceneEntity_0200090c {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    u8 unknown_10[0x20];
    s32 phase;                      /* 0x30 */
    u8 unknown_34[4];
    s32 origin_x;                    /* 0x38 */
    s32 origin_y;                    /* 0x3c */
    u8 unknown_40[0x10];
    struct SceneHandle_0200090c *handle;     /* 0x50 */
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern s16 Data_02000240[];
extern u8 Value_00000022;
extern u8 Data_02008c7c[];
extern u8 Data_02008c64[];
extern u8 Data_02008d30[];
extern u8 Data_02008d24[];
extern u8 UpdateOrbitingSceneObject;

s32 CalculateFacingAngle(s32, s32);
struct SceneEntity *Func_02000fc4();
void Func_02000a00(void);
OrbitingSceneObject *GetOrbitingSceneObject(void);
u8 *AllocateEffectTransfer(s32, s32);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Data_02000240 is the shared cross-overlay scene workspace; Data_03001ebc is
 * a pointer cell holding the per-overlay workspace base, not the workspace
 * itself. The imports above are old-style because their arity varies between
 * call sites. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void SceneDialogue_RunActor9Message13c0(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoHaveYouSeenBarricadeWe);
    Event_AskYesNo(9, 0);
    Event_End();
}
