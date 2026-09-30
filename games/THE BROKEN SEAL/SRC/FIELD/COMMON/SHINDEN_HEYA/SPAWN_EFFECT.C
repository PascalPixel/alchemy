#include "TYPES.H"
#include "CALL.H"

s32 Engine_ActorGet();
s32 Engine_ObjectCreate();
void Engine_ObjectSetScript();
void SceneEffect_StepEllipseOrbit();
/* The motion script the owner effect runs before deleting itself. */
extern const s32 ShindenHeya_OwnerEffectScript[];

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

/* Spawns the effect object above actor a0 with its script, zeroes its state and copies the owner's sprite mode. */
void ShindenHeya_SpawnOwnerEffect(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 p8b;
    s32 rec;
    u8 *rec8;
    s32 record;
    u8 *p5;

    p8 = a1;
    rec = Engine_ActorGet();
    if (rec != 0) {
        rec8 = Value4(Engine_ObjectCreate, 0x11d, *(s32 *)(rec + 8), (*(s32 *)(rec + 12) + 0x2d0000), *(s32 *)(rec + 16));
        if ((s32)rec8 != 0) {
            p5 = *(s32 *)((s32)rec8 + 80);
            Engine_ObjectSetScript((s32)rec8, (s32)ShindenHeya_OwnerEffectScript);
            {
                s32 zero = 0;

                rec8[85] = zero;
                *(u16 *)(rec8 + 100) = zero;
            }
            *(u16 *)(rec8 + 102) = p8;
            *(s32 *)((s32)rec8 + 108) = (s32)SceneEffect_StepEllipseOrbit;
            { u16 v = 0; p5[38] = v; }
            { u8 m = ((struct Flags9 *)(*(s32 *)(rec + 80)))->mode; *(s32 *)((s32)rec8 + 104) = rec; ((struct Flags9 *)p5)->mode = m; } /* FAKEMATCH: the mode is read into a temporary so the owner store schedules first */
        }
    }
}
