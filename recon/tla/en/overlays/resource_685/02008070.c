/* Func_020004d4: differ 55. The native builds the transition id from the register that held its offset (428 then +85: both in r2), reads the entrance again with ldrsh for each of its two tests and keeps r1 free; here the entrance is also loaded unextended for the second test (ldrh, shifted compare), which takes r2 and moves every later constant. */
#include "../../../../../games/tla/src/scene/champa_cliff_caves/scene.h"
#include "scene_ids.h"

extern u32 Data_0200375c[];
extern u32 Data_020037a4[];
extern u32 Data_0200381c[];
extern u32 Data_02003864[];
extern u32 Data_020038ac[];
extern u32 Data_0200390c[];
extern u32 Data_02003984[];
extern u32 Data_020039cc[];
extern u32 Data_02003aec[];
extern u32 Data_02003b94[];
extern u32 Data_02003c0c[];
extern u32 Data_02003c84[];

struct EventTransition {
    u8 unk000[0x1ac];
    s32 type;
    u8 unk1b0[4];
    s32 frames;
};

void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Func_020035ac(s32 value);
void Func_0200361c(s32 a, s32 b);
void Func_020034d4(s32 actor, s32 x, s32 z);
void Func_02001774(void);
void Func_02001868(void);

s32 Func_020004d4(void)
{
    struct PartyState *party;
    s32 scene;
    s32 *type = &((struct EventTransition *)Ram_HeapSlots->eventWork)->type;

#ifdef TLA_EDITION_JA
    *type = 516;
#else
    *type = 513;
#endif
    party = &gPartyState;
    if (party->scene != (s32)&SceneId_Ov685Room1)
        Object_GetById(party->currentOwner)->priorityFlags |= 0x20;
    Func_020035ac(0);
    scene = party->scene;
    if (scene == (s32)&SceneId_Ov685Room4) {
        if (party->entrance == 98)
            Func_02001774();
        if (party->entrance == 99) {
            party->secondary = 23;
            party->entrance = 23;
            Func_02001868();
        }
        if (GameFlag_Test(0x977) && !GameFlag_Test(0x978)) {
            Func_0200361c(10, 326);
            Func_020034d4(10, 664 << 16, 232 << 16);
            Object_GetById(10)->motionFlags = 4;
        }
        if (GameFlag_Test(0x977))
            Func_020034d4(8, 664 << 16, 200 << 16);
    } else if (scene == (s32)&SceneId_Ov685Room3) {
        if (GameFlag_Test(0x97e)) {
            Object_SetModeById(8, 11);
            ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
        }
    }
    return 0;
}
