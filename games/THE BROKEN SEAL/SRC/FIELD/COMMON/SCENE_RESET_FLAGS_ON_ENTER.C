
#include "TYPES.H"

struct SceneRecord {
    s16 resource_id;
    s8 group;
    s8 variant;
    u16 map_index;
    u16 reserved;
};

struct FieldProgress {
    u8 unknown_000[0x1c0];
    s16 scene;
    s16 entrance;
    u8 unknown_1c4[8];
    s16 group;
    u8 unknown_1ce[0x50];
    u16 unknown_21e;
    u8 unknown_220[0x12];
    s16 unknown_232;
    u8 unknown_234[4];
    s32 unknown_238;
    u8 unknown_23c[2];
    s16 variant;
    u16 return_scene;
    u16 return_entrance;
};

extern struct FieldProgress Data_02000240;
extern struct SceneRecord Field_SceneTable[];

void GameFlag_ClearBitFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_RefreshLureCapFar(void);

#define SCENE_RECORD(id) (&Field_SceneTable[id])

/* Entering a scene: unless the scene keeps its state, clears the scene
   flags 0x200-0x2ff, the area flags 0x300-0x3ff when the scene group
   changes (recording the return point), and the per-visit flags; then
   records the group and variant and sets the group's visited flag. */
void Scene_ResetFlagsOnEnter(s32 unused, s32 keep)
{
    s32 group = SCENE_RECORD(Data_02000240.scene)->group;
    s32 flag;

    if (keep == 0) {
        for (flag = 0x200; flag <= 0x2ff; flag++)
            GameFlag_ClearBitFar(flag);
        if (group != Data_02000240.group) {
            for (flag = 0x300; flag <= 0x3ff; flag++)
                GameFlag_ClearBitFar(flag);
            GameFlag_SetBitFar(0x12f);
            Data_02000240.unknown_238 = 0;
            Data_02000240.unknown_232 = 0;
            GameFlag_ClearBitFar(0x110);
            GameFlag_ClearBitFar(0x111);
            GameFlag_ClearBitFar(0x112);
            GameFlag_ClearBitFar(0x113);
            Data_02000240.return_scene = Data_02000240.scene;
            Data_02000240.return_entrance = Data_02000240.entrance;
        }
        for (flag = 0x80; flag <= 0xdf; flag++)
            GameFlag_ClearBitFar(flag);
        GameFlag_ClearBitFar(0x16c);
        GameFlag_ClearBitFar(0x144);
        GameFlag_ClearBitFar(0x161);
        GameFlag_ClearBitFar(0x123);
        GameFlag_ClearBitFar(0x11c);
        Data_02000240.unknown_21e = 0xffff;
    }
    Data_02000240.group = group;
    GameFlag_SetBitFar((group & 0x7f) + 0x180);
    Data_02000240.variant = SCENE_RECORD(Data_02000240.scene)->variant;
    if (Data_02000240.variant == 2)
        GameFlag_SetBitFar(0x123);
    GameFlag_RefreshLureCapFar();
}
