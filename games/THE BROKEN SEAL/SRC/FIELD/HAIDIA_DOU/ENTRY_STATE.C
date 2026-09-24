#include "TYPES.H"
#include "FIELD_EVENT.H"

void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void *OverlayObject_CreateConfiguredB(s32 x, s32 y, s32 z, s32 kind);
void DialogueLayout_ConfigureRowsByFlag301(void);
s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);
void FieldScene_RedrawActorFootprint(s32 actor);
void SceneActor_FaceActorZero(union FieldObject *object);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_0000005e[];
extern u8 Data_0000005f[];

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Vale Cave entry: by the room and entrance, set the sprite priorities, spawn the stage objects and restore the opened cells and footprints the story flags record. */
void HaidiaDou_ApplyEntryState(void)
{
    s32 flag;

    if (Data_02000240_t.halves[224][0] == (s32)Data_0000005e) {
        switch (Data_02000240_t.halves[225][0]) {
        case 1:
        case 2:
        case 3:
        case 4:
            Engine_ActorSetSpritePriority(15, 3);
            Engine_ActorSetSpritePriority(13, 3);
            OverlayObject_CreateConfiguredB(0x780000, 0, 0xe80000, 223);
            break;
        case 5:
        case 6:
        case 7:
            if (Engine_GameFlagIsSet(0x70) != 0) {
                break;
            }
            if (Engine_GameFlagIsSet(0x302) == 0) {
                break;
            }
            Engine_GameFlagSet(0x200);
            if (Data_02000240_t.halves[225][0] == 5) {
                Engine_GameFlagSet(0x201);
            }
            Engine_TaskWait(1);
            if (Engine_GameFlagIsSet(0x109) != 0) {
                break;
            }
            Call3(Engine_ActorSetPosition, 8, 0x3180000, 0x1180000);
            Engine_ActorGet(8)->update = SceneActor_FaceActorZero;
            break;
        case 8:
        case 9:
        case 10:
            OverlayObject_CreateConfiguredB(0x2820000, 0, 0x2280000, 20);
            Call6(Engine_MapCopyCellAttributes, 23, 34, 13, 3, 0, 34);
            DialogueLayout_ConfigureRowsByFlag301();
            if (Engine_GameFlagIsSet(0x200) != 0) {
                Call6(Engine_MapCopyCellAttributes, 23, 41, 1, 1, 23, 39);
            }
            if (Engine_GameFlagIsSet(0x201) != 0) {
                Call6(Engine_MapCopyCellAttributes, 31, 39, 2, 1, 27, 41);
            }
            break;
        }
    } else if (Data_02000240_t.halves[224][0] == (s32)Data_0000005f) {
        switch (Data_02000240_t.halves[225][0]) {
        case 1:
        case 2:
        case 3:
            if (Engine_GameFlagIsSet(0x202) != 0) {
                StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
                StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
            } else {
                FieldScene_RedrawActorFootprint(9);
            }
            if (Engine_GameFlagIsSet(0x203) != 0) {
                StagedActor_FillGridAttributeRectangle(2, 16, 16, 1, 4, 0);
                StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, 4, 0);
            } else {
                FieldScene_RedrawActorFootprint(10);
            }
            flag = Engine_GameFlagIsSet(0x205);
            if (flag != 0) {
                StagedActor_FillGridAttributeRectangle(0, 13, 19, 4, 2, 0);
            } else if (Engine_GameFlagIsSet(0x204) != 0) {
                StagedActor_FillGridAttributeRectangle(0, 13, 15, 4, 2, flag);
                Call6(Engine_MapCopyCellAttributes, 14, 17, 2, 1, 14, 16);
                Call6(Engine_MapCopyCellAttributes, 14, 13, 1, 1, 14, 15);
            } else {
                FieldScene_RedrawActorFootprint(11);
                Engine_ActorSetSpritePriority(11, 3);
            }
            break;
        }
    }
}
