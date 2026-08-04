#include "resource_384.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

void Func_02000466(s32);
u8 *Func_02000492(s32);
u8 *Func_020004a4(s32);
u8 *Func_020004b4(s32);
u8 *Func_020004c2(s32);

#define ClearStoryFlag         Func_02000466
#define GetMode10Actor         Func_02000492
#define GetFirstDefaultActor   Func_020004a4
#define GetSecondDefaultActor  Func_020004b4
#define GetThirdDefaultActor   Func_020004c2

/* Keeping this as a small ordinary-C helper preserves the standalone store in
 * the special-step branch; folding it into the root enables tail merging. */
static inline void SetActorFlag(u8 *record, u8 bits)
{
    *record |= bits;
}

/* Publish this scene's script state, then update actor flags for the current
 * scene step. Data_02000240[225] is the shared signed scene-step field. */
s32 Func_020001e4(void)
{
    s16 mode;

    *(s32 *)(Data_03001ebc + 448) = RESOURCE384_SCRIPT_STATE;
    mode = Data_02000240[225];

    if (mode == 2) {
        ClearStoryFlag(0x12f);
    } else if (mode == RESOURCE384_SPECIAL_STEP) {
        SetActorFlag(GetMode10Actor(RESOURCE384_ACTOR_8) + 89, RESOURCE384_ACTOR_FLAG);
    } else {
        u8 bits = RESOURCE384_ACTOR_FLAG;
        u8 *record = GetFirstDefaultActor(RESOURCE384_ACTOR_8) + 89;
        u8 value = *record;
        value |= bits;
        *record = value;
        record = GetSecondDefaultActor(RESOURCE384_ACTOR_9) + 89;
        value = *record;
        value |= bits;
        *record = value;
        record = GetThirdDefaultActor(RESOURCE384_ACTOR_10) + 89;
        bits |= *record;
        *record = bits;
    }

    return 0;
}
