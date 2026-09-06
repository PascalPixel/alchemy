#include "types.h"

#if defined(GS1_EDITION_JA)
#define Value_00002421 Value_000025aa
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define Value_00002421 Value_00002403
#endif

extern u8 Value_00002421;

void Func_02007396(s32);
void Func_020073ae(s32, s32);
void Func_02007386_a(s32, s32); void Func_02007386_b(s32, s32);
void Func_020073ac(s32);
void Func_020073c4_a(); void Func_020073c4_b();
void Func_020073dc(s32, s32);

void RunActorScriptedSequenceC(s32 actor_id)
{
    u8 *t = &Value_00002421;

    Func_02007396((s32)t);
    Func_020073ae(actor_id, 0);
    Func_02007386_a(actor_id, 1);
    Func_020073ac((s32)(t + 1));
    Func_020073c4_a(actor_id, 0);
    Func_02007386_b(actor_id, 4);
    Func_020073c4_b((s32)(t + 2));
    Func_020073dc(actor_id, 0);
}
