#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_0808a408(s32 value);
void Main_080091b8(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void FieldScene_PlaceAndPinSlots8And9(void);
void FieldScene_PlaceAndPinSlots10And11(void);
void FieldScene_RunScene3c4_02002480(void);
void FieldScene_RunLateSequenceHead(void);
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call5(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    f(a0, a1, a2, a3, a4);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Local_0200252c(void);
void Local_02001abc(s32 mode);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_000000ac[];
extern u8 Data_000000ad[];
extern u8 Data_000000b0[];

#define ACTOR_UPDATE_IDLE ((void (*)(union FieldObject *))0x2008ec9)
#define ACTOR_UPDATE_PANEL ((void (*)(union FieldObject *))0x2009a99)
#define SCENE_TASK ((void (*)(void))0x2008e21)

/* Entry setup for the underground passage: by area and entrance, restores the lift cells, pins and parks the paired actors and re-applies each flagged block. */
s32 FieldScene_InitializeActorGroups(void)
{
    gEventWork->start_transition = 0x204;
    if (Data_02000240_t.halves[224][0] == (s32)Data_000000ac || Data_02000240_t.halves[224][0] == (s32)Data_000000ad) {
        Main_0808a408(0);
        Data_02000240_t.halves[289][0] = 1;
        Data_02000240_t.halves[288][0] = (s32)Data_000000ac;
    }
    if (Data_02000240_t.halves[224][0] == (s32)Data_000000ac) {
        switch (Data_02000240_t.halves[225][0]) {
        case 1:
        case 2:
            if (Engine_GameFlagIsSet(0x982)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 121, 4, 74, 9, 5, 8);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 9, 73, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 81, 9, 75, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 9, 77, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 9, 79, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 11, 78, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 13, 79, 3, 2);
            } else {
                if (!Engine_GameFlagIsSet(0x983))
                    break;
                Call6((void (*)())Engine_MapCopyCellsTo, 121, 13, 74, 9, 5, 8);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 85, 11, 74, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 13, 75, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 85, 11, 76, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 11, 78, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 13, 79, 3, 2);
            }
            break;
        case 3:
        case 4:
            FieldScene_PlaceAndPinSlots8And9();
            Engine_ActorGet(8)->motion_flags = 0;
            Engine_ActorGet(9)->motion_flags = 0;
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(8), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(9), 0);
            Engine_ActorGet(8)->update = ACTOR_UPDATE_IDLE;
            Engine_ActorGet(9)->update = ACTOR_UPDATE_IDLE;
            Call2((void (*)())Engine_TaskAddCallback, SCENE_TASK, 0xc80);
            break;
        case 5:
        case 6:
        case 7:
            if (Engine_GameFlagIsSet(0x982))
                Call6((void (*)())Main_080091b8, 23, 17, 1, 2, 30, 8);
            if (Engine_GameFlagIsSet(0x983))
                Call6((void (*)())Main_080091b8, 23, 17, 1, 2, 32, 10);
            break;
        case 8:
        case 9:
            FieldScene_PlaceAndPinSlots10And11();
            Engine_ActorGet(10)->motion_flags = 0;
            Engine_ActorGet(11)->motion_flags = 0;
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(10), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(11), 0);
            Engine_ActorGet(10)->update = ACTOR_UPDATE_IDLE;
            Engine_ActorGet(11)->update = ACTOR_UPDATE_IDLE;
            Call2((void (*)())Engine_TaskAddCallback, SCENE_TASK, 0xc80);
            break;
        case 10:
        case 11:
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(18), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(19), 0);
            Call2((void (*)())Engine_ActorSetAnimation, 18, 2);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(20), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(21), 0);
            Call2((void (*)())Engine_ActorSetChildValue, 20, 15);
            Call2((void (*)())Engine_ActorSetChildValue, 21, 15);
            if (Engine_GameFlagIsSet(0x971)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 59, 8, 49, 8, 1, 3);
                Call6((void (*)())Engine_MapCopyCellAttributes, 51, 8, 1, 1, 49, 8);
                Engine_ActorGet(18)->priority_flags |= 2;
                Call2((void (*)())Engine_ActorSetAnimation, 18, 3);
                Call6((void (*)())Engine_MapCopyCellAttributes, 45, 4, 1, 1, 46, 8);
                Call3((void (*)())Engine_ActorSetPosition, 18, 186 << 18, 136 << 16);
                Engine_ActorGet(18)->y.fixed = -0x100000;
                Call3((void (*)())Engine_ActorSetPosition, 20, 186 << 18, 136 << 16);
            }
            if (Engine_GameFlagIsSet(0x200)) {
                Call2((void (*)())Engine_ActorSetChildValue, 20, 0);
                Call2((void (*)())Engine_ActorSetAnimation, 20, 5);
            }
            if (Value1(Engine_GameFlagIsSet, 0x202))
                Call2((void (*)())Engine_ActorSetAnimation, 19, 2);
            if (Engine_GameFlagIsSet(0x972)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 59, 8, 45, 14, 1, 3);
                Call6((void (*)())Engine_MapCopyCellAttributes, 51, 8, 1, 1, 45, 14);
                Engine_ActorGet(19)->priority_flags |= 2;
                Call2((void (*)())Engine_ActorSetAnimation, 19, 3);
                Call6((void (*)())Engine_MapCopyCellAttributes, 45, 4, 1, 1, 48, 14);
                Call3((void (*)())Engine_ActorSetPosition, 19, 194 << 18, 232 << 16);
                Engine_ActorGet(19)->y.fixed = -0x100000;
                Call3((void (*)())Engine_ActorSetPosition, 21, 194 << 18, 232 << 16);
                Engine_GameFlagSet(0x202);
            }
            if (Engine_GameFlagIsSet(0x201)) {
                Call2((void (*)())Engine_ActorSetChildValue, 21, 0);
                Call2((void (*)())Engine_ActorSetAnimation, 21, 5);
            }
            break;
        case 12:
        case 13:
            FieldScene_RunScene3c4_02002480();
            Engine_ActorGet(12)->motion_flags = 0;
            Engine_ActorGet(13)->motion_flags = 0;
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(15), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(16), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(17), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(12), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(13), 0);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(14), 0);
            Engine_ActorGet(12)->update = ACTOR_UPDATE_IDLE;
            Engine_ActorGet(13)->update = ACTOR_UPDATE_IDLE;
            Engine_ActorGet(14)->update = ACTOR_UPDATE_IDLE;
            Call2((void (*)())Engine_TaskAddCallback, SCENE_TASK, 0xc80);
            break;
        }
    } else {
        switch (Data_02000240_t.halves[225][0]) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            Data_02000240_t.halves[289][0] = 1;
            Data_02000240_t.halves[288][0] = (s32)Data_000000b0;
            Engine_GameFlagClear(0x12f);
            Call2((void (*)())Engine_ActorSetChildValue, 17, 6);
            Call2((void (*)())Engine_ActorSetChildValue, 18, 6);
            if (Engine_GameFlagIsSet(0x974))
                Call3((void (*)())Engine_ActorSetPosition, 17, 182 << 18, 156 << 17);
            if (Engine_GameFlagIsSet(0x975))
                Call3((void (*)())Engine_ActorSetPosition, 18, 186 << 18, 156 << 17);
            Local_0200252c();
            break;
        case 6:
        case 7:
            Call2((void (*)())Engine_ActorSetSpritePriority, 8, 1);
            Engine_ActorGet(8)->motion_flags = 0;
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(8), 0);
            Call2((void (*)())Engine_ActorSetSpritePriority, 9, 1);
            Call2((void (*)())Engine_ActorSetChildValue, 9, 15);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(9), 0);
            Engine_ActorGet(9)->motion_flags = 0;
            if (!Engine_GameFlagIsSet(0x204))
                break;
            Call2((void (*)())Engine_ActorSetChildValue, 9, 0);
            Call2((void (*)())Engine_ActorSetAnimation, 9, 5);
            Call6((void (*)())Engine_MapCopyCellAttributes, 26, 8, 1, 1, Engine_ActorGet(9)->x.fixed >> 20, Engine_ActorGet(9)->z.fixed >> 20);
            Engine_ActorGet(9)->update = ACTOR_UPDATE_IDLE;
            Engine_ActorGet(8)->update = ACTOR_UPDATE_IDLE;
            break;
        case 4:
        case 5:
            if (!Engine_GameFlagIsSet(0x109)) {
                Engine_ActorGet(10)->motion_flags = 0;
                Engine_ActorGet(11)->motion_flags = 0;
                Engine_ActorGet(10)->y.fixed = -0x300000;
                Engine_ActorGet(11)->y.fixed = -0x300000;
                Engine_ActorGet(10)->priority_flags |= 2;
                Engine_ActorGet(11)->priority_flags |= 2;
                Engine_ActorGet(10)->collision_flags &= 0xfe;
                Engine_ActorGet(11)->collision_flags &= 0xfe;
                Engine_ActorGet(10)->unknown_64 = 3;
                Engine_ActorGet(11)->unknown_64 = 3;
                Call2((void (*)())Engine_ActorSetSpritePriority, 10, 1);
                Call2((void (*)())Engine_ActorSetSpritePriority, 11, 1);
                Engine_ActorGet(12)->motion_flags = 0;
                Engine_ActorGet(13)->motion_flags = 0;
                Engine_ActorGet(14)->motion_flags = 0;
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(12), 0);
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(13), 0);
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(14), 0);
                Engine_ActorGet(12)->unknown_64 = 0;
                Engine_ActorGet(13)->unknown_64 = 0;
                Engine_ActorGet(14)->unknown_64 = 0;
                if (Data_02000240_t.halves[225][0] != 5)
                    break;
                Engine_ActorGet(10)->y.fixed = -0x200000;
                Engine_ActorGet(11)->y.fixed = -0x400000;
                Engine_ActorGet(10)->unknown_64 = 2;
                Engine_ActorGet(11)->unknown_64 = 4;
                Call3((void (*)())Engine_ActorSetPosition, 12, 200 << 16, 152 << 16);
                Engine_ActorGet(12)->unknown_64 = 11;
                Engine_ActorGet(12)->update = ACTOR_UPDATE_PANEL;
                Engine_ActorGet(12)->priority_flags |= 2;
                Call3((void (*)())Engine_ActorSetPosition, 13, 200 << 16, 152 << 16);
                Engine_ActorGet(13)->unknown_64 = 12;
                Engine_ActorGet(13)->update = ACTOR_UPDATE_PANEL;
                Engine_ActorGet(13)->priority_flags |= 2;
                Call3((void (*)())Engine_ActorSetPosition, 14, 136 << 16, 152 << 16);
                Engine_ActorGet(14)->unknown_64 = 10;
                Engine_ActorGet(14)->update = ACTOR_UPDATE_PANEL;
                Engine_ActorGet(14)->priority_flags |= 2;
                Engine_EventWait(2);
                Engine_GameFlagSet(0x200);
                Engine_GameFlagSet(0x201);
                Engine_GameFlagSet(0x202);
            }
            Local_02001abc(0);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x982))
                Call6((void (*)())Main_080091b8, 10, 30, 1, 2, 16, 30);
            if (Engine_GameFlagIsSet(0x983))
                Call6((void (*)())Main_080091b8, 10, 30, 1, 2, 22, 30);
            Engine_GameFlagSet(0x973);
            break;
        case 12:
            Call6((void (*)())Engine_MapCopyCellAttributes, 8, 49, 1, 1, 8, 113);
            FieldScene_RunLateSequenceHead();
            Call2((void (*)())Engine_TaskAddCallback, SCENE_TASK, 0xc80);
            break;
        case 13:
        case 14:
            Engine_EventWait(1);
            if (Engine_GameFlagIsSet(0x984)) {
                Call6((void (*)())Main_080091b8, 24, 59, 1, 2, 32, 46);
                Call3((void (*)())Engine_ActorSetPosition, 19, 204 << 17, 198 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 20, 188 << 17, 198 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 21, 204 << 17, 190 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 22, 188 << 17, 190 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 23, 196 << 17, 194 << 18);
            }
            Engine_ActorGet(19)->motion_flags &= 0xfe;
            Engine_ActorGet(20)->motion_flags &= 0xfe;
            Engine_ActorGet(21)->motion_flags &= 0xfe;
            Engine_ActorGet(22)->motion_flags &= 0xfe;
            Engine_ActorGet(23)->motion_flags &= 0xfe;
            Call2((void (*)())Engine_ActorSetChildValue, 19, 4);
            Call2((void (*)())Engine_ActorSetChildValue, 20, 1);
            Call2((void (*)())Engine_ActorSetChildValue, 21, 4);
            Call2((void (*)())Engine_ActorSetChildValue, 22, 10);
            Call2((void (*)())Engine_ActorSetChildValue, 23, 0);
            Call2((void (*)())Engine_ActorSetAnimation, 19, 2);
            Call2((void (*)())Engine_ActorSetAnimation, 23, 2);
            Call6((void (*)())Engine_MapCopyCellAttributes, 20, 56, 1, 1, Engine_ActorGet(19)->x.fixed >> 20, Engine_ActorGet(19)->z.fixed >> 20);
            Call6((void (*)())Engine_MapCopyCellAttributes, 20, 56, 1, 1, Engine_ActorGet(20)->x.fixed >> 20, Engine_ActorGet(20)->z.fixed >> 20);
            Call6((void (*)())Engine_MapCopyCellAttributes, 20, 56, 1, 1, Engine_ActorGet(21)->x.fixed >> 20, Engine_ActorGet(21)->z.fixed >> 20);
            Call6((void (*)())Engine_MapCopyCellAttributes, 20, 56, 1, 1, Engine_ActorGet(22)->x.fixed >> 20, Engine_ActorGet(22)->z.fixed >> 20);
            Call6((void (*)())Engine_MapCopyCellAttributes, 20, 56, 1, 1, Engine_ActorGet(23)->x.fixed >> 20, Engine_ActorGet(23)->z.fixed >> 20);
            break;
        case 17:
            Call6((void (*)())Engine_MapCopyCellAttributes, 49, 43, 1, 1, 49, 107);
            FieldScene_RunLateSequenceHead();
            break;
        }
    }
    return 0;
}
