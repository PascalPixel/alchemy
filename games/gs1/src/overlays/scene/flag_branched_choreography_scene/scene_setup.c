#include "types.h"

#define SceneData_SelectTableac9cByState Func_02000100
#define SceneDialogue_RunActor10Message1420 Func_02000130
#define FieldScene_RunSlotSubjectBranch Func_020004ec
extern s16 Data_02000240[];
extern u8 Value_00000021;
extern u8 Data_0200aca8[];
extern u8 Data_0200ac9c[];
extern u8 Value_00001420;

void Func_020025a4(void);
void Func_02002672(s32);
s32 Func_0200269a(s32, s32);
void Func_020025be(void);
void Func_0200296a(void);
s32 Func_02002ab4(s32, s32, s32);
/* 0x02002996 serves two imports: the two-argument reader that guards the
 * branch and the one-argument flag clear inside its first arm. */
extern s32 Func_02002996_a(s32, s32);
void Func_02002996_b(s32);
void Func_02002a1e(s32, s32);
void Func_0200297c(s32);
void Func_02002982(s32);
void Func_02002990(s32);
void Func_02002af6(s32);
void Func_02002a46(s32, s32);
void Func_02002964(s32);
void Func_020029c8(void);

s32 SceneData_SelectTableac9cByState(void) {
    if (Data_02000240[224] == (s32)&Value_00000021) {
        return (s32)Data_0200aca8;
    }
    return (s32)Data_0200ac9c;
}

void SceneDialogue_RunActor10Message1420(void)
{
    Func_020025a4();
    Func_02002672((s32)&Value_00001420);
    Func_0200269a(10, 0);
    Func_020025be();
}

void FieldScene_RunSlotSubjectBranch(s32 slot, s32 subject, s32 flag)
{
    s32 record;

    Func_0200296a();

    record = Func_02002ab4(0, slot, subject);

    if (Func_02002996_a(subject, 0) != -1) {
        Func_02002a1e(slot, 2);
        Func_0200297c(0x84E);
        Func_02002982(flag);
        Func_02002990(0x322);
        Func_02002996_b(0x202);
    } else {
        Func_02002af6(125);
        Func_02002a46(slot, 5);
    }

    Func_02002964(record);
    Func_020029c8();
}
