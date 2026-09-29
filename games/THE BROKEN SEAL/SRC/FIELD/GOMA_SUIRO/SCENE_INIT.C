#include "GOMA.H"
#include "SCENE_IDS.H"

extern u8 *gEventWork;

void Engine_GameFlagClear();
void Engine_ActorSetSpriteFlags();
s32 Engine_GameFlagIsSet();
u8 *Engine_ActorGet();
void Map_CopyCellAttributeRect();
void Engine_ActorSetPosition();
void Object_SetModeById();

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

/* The waterway's scene start: open with the window transition; in the
   first area, entering by the fifth entrance clears flag 0x12f, and
   otherwise actor 8 is set up, placed where flag 0x864 records it. */
s32 FieldScene_PlaceActor8OnEntry(void)
{
    extern u8 Data_02000240[];

    u8 *record;
    u8 *base;

    *(s32 *)(gEventWork + 0x1c0) = 0x204;
    base = Data_02000240;
    if (*(s16 *)(base + 0x1c0) == (s32)&SceneId_GomaSuiro1) {
        if (*(s16 *)(base + 0x1c2) == 5) {
            Call1(Engine_GameFlagClear, 0x12f);
        } else {
            SetFlagBits(Engine_ActorGet(8) + 89, 16);
            if (Value1(Engine_GameFlagIsSet, 0x864) != 0) {
                Call3(Engine_ActorSetPosition, 8, 0x15a0000, 0x1240000);
                record = Engine_ActorGet(8);
                Engine_ActorSetSpriteFlags((s32)record, 0);
                *(u8 *)(Engine_ActorGet(8) + 35) |= 2;
                Object_SetModeById(8, 2);
                Call6(Map_CopyCellAttributeRect, 19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}

