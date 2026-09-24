#include "TYPES.H"
#include "DMA.H"
#include "FIELD_EVENT.H"

void Main_080091c8(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void SceneActor_PlaceFiveActorsInRow(s32 spacing);
void SceneEffect_SetupBlendByFlag201(void);
void FieldScene_RedrawActorFootprint(s32 actor);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000092[];
extern u8 Data_00000093[];
extern u8 Data_00000094[];
extern u8 Data_00000095[];
extern u8 Data_00000096[];
extern u8 Data_00000097[];

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Altmiller Cave entry: number the entrance by area, then per area restore the lifts, the cells and the row of five actors from the story flags; open with the blend or the plain transition. */
s32 ArutamiraDou_ApplyEntryState(void)
{
    s8 *state;
    struct FieldActor *actor;
    volatile s32 zero;
    s32 i;

    if (Data_02000240_t.halves[225][0] == 0) {
        if (Data_02000240_t.halves[224][0] == (s32)Data_00000093) {
            Data_02000240_t.halves[225][0] = 10;
        }
        if (Data_02000240_t.halves[224][0] == (s32)Data_00000094) {
            Data_02000240_t.halves[225][0] = 20;
        }
        if (Data_02000240_t.halves[224][0] == (s32)Data_00000095) {
            Data_02000240_t.halves[225][0] = 30;
        }
        if (Data_02000240_t.halves[224][0] == (s32)Data_00000096) {
            Data_02000240_t.halves[225][0] = 40;
        }
        if (Data_02000240_t.halves[224][0] == (s32)Data_00000097) {
            Data_02000240_t.halves[225][0] = 50;
        }
    }
    Engine_GameFlagSet(0x200);
    Call1(Engine_GameFlagClear, 0x201);
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000092) {
        if (Data_02000240_t.halves[225][0] == 1) {
            if (!Engine_GameFlagIsSet(0x109)) {
                *(u8 *)0x02001004 = 0;
            }
            Call1(Engine_GameFlagSet, 0x201);
        }
        if (Data_02000240_t.halves[225][0] == 2) {
            if (!Engine_GameFlagIsSet(0x109)) {
                *(u8 *)0x02001004 = 5;
            }
            Call1(Engine_GameFlagSet, 0x201);
        }
    }
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000093) {
        if (Engine_GameFlagIsSet(0x962)) {
            Engine_ActorSetPosition(8, 0, 0);
        } else {
            actor = Engine_ActorGet(8);
            ((s8 *)actor->sprite)[9] = (((s8 *)actor->sprite)[9] & ~0xc) | 4;
            ((u8 *)actor->sprite)[38] = 2;
            actor->sprite->rotation = 0x4000;
        }
    }
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000095) {
        Engine_GameFlagClear(0x200);
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        if (Engine_GameFlagIsSet(0x211)) {
            Engine_ActorSetAnimation(11, 5);
            Call6(Engine_MapCopyCellAttributes, 76, 16, 1, 1, 73, 17);
        } else {
            Engine_ActorGet(11)->priority_flags |= 2;
        }
        Engine_ActorSetSpriteFlags(Engine_ActorGet(11), 0);
        if (Engine_GameFlagIsSet(0x212)) {
            Call6(Engine_MapCopyCellAttributes, 30, 20, 1, 1, 32, 20);
        }
    }
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000097) {
        Engine_GameFlagClear(0x200);
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        /* FAKEMATCH: the three callbacks are stored as s32, so the entrance read
         * after them may alias them and stays below the last store. */
        *(s32 *)&Engine_ActorGet(8)->update = 0x200b611;
        *(s32 *)&Engine_ActorGet(9)->update = 0x200b611;
        *(s32 *)&Engine_ActorGet(10)->update = 0x200b611;
        if (Data_02000240_t.halves[225][0] == 52) {
            zero = 0;
            Dma_Set(&zero, *(void **)0x0200bf6c, 0x85000003, (volatile u32 *)0x040000d4);
            if (!Engine_GameFlagIsSet(0x109)) {
                u8 *race = (u8 *)0x02001000;

                race[0] = 0;
                race[1] = 0;
                race[2] = 4;
            }
        }
        state = (s8 *)0x02001001;
        if (state[0] == 99) {
            Call6(Main_080091c8, 41, 55, 3, 2, 30, 55);
            Call6(Engine_MapCopyCellAttributes, 42, 8, 1, 1, 31, 8);
        }
        if (state[0] == 2) {
            SceneActor_PlaceFiveActorsInRow(Engine_MathDivide(state[1] << 16, 5) + 0x4000);
        }
        for (i = 0; i < 5; i++) {
            actor = Engine_ActorGet(i + 11);
            actor->motion_flags = 0;
            actor->collision_flags = 0;
            actor->scale_x = 0x10000;
            actor->scale_y = 0x10000;
            Engine_ActorSetSpriteFlags(Engine_ActorGet(i + 11), 0);
            Engine_ActorSetAnimation(i + 11, i + 1);
        }
        Engine_ActorSetChildValue(11, 1);
        Engine_ActorSetChildValue(12, 4);
        Engine_ActorSetChildValue(13, 11);
        Engine_ActorSetChildValue(14, 2);
        Engine_ActorSetChildValue(15, 3);
        Engine_ActorSetChildValue(16, 6);
        Engine_ActorSetChildValue(17, 6);
        Engine_ActorSetChildValue(18, 6);
        Engine_ActorSetChildValue(19, 6);
        Engine_ActorSetChildValue(20, 6);
        Engine_ActorGet(16)->sprite->priority = 3;
        Engine_ActorGet(20)->sprite->priority = 3;
        Engine_ActorGet(16)->priority_flags = 2;
        Engine_ActorGet(20)->priority_flags = 2;
        Engine_ActorSetSpriteFlags(Engine_ActorGet(16), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(20), 0);
    }
    if (Engine_GameFlagIsSet(0x200)) {
        SceneEffect_SetupBlendByFlag201();
    } else {
        gEventWork->start_transition = 0x204;
        gEventWork->transition_frames = 24;
    }
    return 0;
}
