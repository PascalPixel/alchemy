#include "TYPES.H"
#include "FIELD_EVENT.H"

void FieldScene_RunScene39e_020027ec(void);
void FieldScene_RunRoofEnsembleSequence(void);
void *NewEffectObject(s32 x, s32 y, s32 z, s32 kind);
void Main_0808a5e0(s32 value);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_0000003d[];

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Temple entry: in area 0x3d set the entrance selector and, by entrance and story flags, stage the roof and gathering actors; elsewhere restore the opened passages. */
s32 ShianJiin_ApplyEntryState(void)
{
    struct FieldActor *actor;
    s32 set;

    if (Data_02000240_t.halves[224][0] == (s32)Data_0000003d) {
        gEventWork->start_transition = 0x209;
        if (Data_02000240_t.halves[225][0] == 1) {
            if (Engine_GameFlagIsSet(0x88f)) {
                Engine_ActorSetAnimation(8, 6);
            } else {
                Engine_ActorSetAnimation(8, 5);
                if (Engine_GameFlagIsSet(0xf14) && !Engine_GameFlagIsSet(0x893) && !Engine_GameFlagIsSet(0x109)) {
                    FieldScene_RunScene39e_020027ec();
                }
            }
        } else if (Data_02000240_t.halves[225][0] == 2 || Data_02000240_t.halves[225][0] == 4) {
            Engine_GameFlagClear(0x12f);
            set = Engine_GameFlagIsSet(0x895);
            if (set == 0) {
                actor = Engine_ActorGet(19);
                actor->motion_flags = set;
                actor->y.fixed = 0xc0000;
                actor->target_y = 0xc0000;
                actor->scale_x = 0xcccc;
                actor->scale_y = 0x8000;
                actor->sprite->rotation = 0x8000;
                if (Engine_GameFlagIsSet(0x89a)) {
                    Engine_ActorSetPosition(18, 0xf80000, 0xd00000);
                    if (!Engine_GameFlagIsSet(0x89b)) {
                        Engine_ActorSetPosition(16, 0x1000000, 0xf00000);
                        Engine_ActorGet(18)->update = (void *)0x2008325;
                        Engine_ActorGet(13)->update = (void *)0x2008325;
                        Engine_ActorGet(14)->update = (void *)0x2008325;
                        Engine_ActorGet(15)->update = (void *)0x2008325;
                        Engine_ActorGet(16)->update = (void *)0x2008325;
                    }
                }
            } else {
                actor = Engine_ActorGet(19);
                actor->motion_flags = 0;
                actor->y.fixed = 0xc0000;
                actor->target_y = 0xc0000;
                actor->scale_x = 0xcccc;
                actor->scale_y = 0x8000;
                actor->collision_flags |= 8;
                actor->sprite->rotation = 0x8000;
                Call6(Engine_MapCopyCellAttributes, 14, 11, 1, 1, 14, 10);
            }
            NewEffectObject(0x1300000, 0x180000, 0xe00000, 223);
            Engine_ActorSetAnimation(10, 5);
            Engine_ActorSetAnimation(11, 5);
        } else if (Data_02000240_t.halves[225][0] == 3) {
            Engine_GameFlagClear(0x12f);
            if (!Engine_GameFlagIsSet(0x895)) {
                FieldScene_RunRoofEnsembleSequence();
            } else if (!Engine_GameFlagIsSet(0x8b2)) {
                Engine_ActorSetPosition(8, 0, 0);
                Engine_ActorSetPosition(9, 0, 0);
            }
        }
    } else {
        Main_0808a5e0(170);
        Engine_ActorGet(9)->collision_flags |= 16;
        if (Data_02000240_t.halves[225][0] == 3 && Engine_GameFlagIsSet(0xf14) && !Engine_GameFlagIsSet(0x894)) {
            Call6(Engine_MapCopyCellAttributes, 10, 84, 1, 1, 10, 24);
        }
        if (Engine_GameFlagIsSet(0x892)) {
            Call3(Engine_ActorSetPosition, 9, 0x980000, 0x1880000);
            Engine_ActorFaceDirection(9, 0, 0);
            Call6(Engine_MapCopyCellAttributes, 10, 26, 1, 1, 10, 22);
        }
    }
    return 0;
}
