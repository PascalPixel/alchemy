#include "types.h"

extern u8 Value_00002428;

void Func_02007452(s32);
void Func_0200746a(s32, s32);
void Func_0200742a(s32, s32);
void Func_02007468(s32);
void Func_02007480(s32, s32);
void Func_02007458_a(s32, s32);
void Func_02007458_b(s32, s32);
void Func_0200747e(s32);
void Func_02007496_a();
void Func_02007496_b();
void Func_020074ae(s32, s32);

void RunActorScriptedSequenceD(s32 actor_id)
{
    u8 *t = &Value_00002428;

    Func_02007452((s32)t);
    Func_0200746a(actor_id, 0);
    Func_0200742a(actor_id, 4);
    Func_02007468((s32)(t + 1));
    Func_02007480(actor_id, 0);
    Func_02007458_a(actor_id, 1);
    Func_0200747e((s32)(t + 2));
    Func_02007496_a(actor_id, 0);
    Func_02007458_b(actor_id, 3);
    Func_02007496_b((s32)(t + 3));
    Func_020074ae(actor_id, 0);
}
