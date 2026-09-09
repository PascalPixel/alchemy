#include "types.h"

#define SceneDialogue_RunActorEightFlaggedDialogue Func_0200345c
#define SceneDialogue_RunActorEightFollowupDialogue Func_020034a8
#define SceneDialogue_RunActorEightDialogue Func_020034f0
/* Close scene 8 when facing inward; otherwise choose its story line. */
extern s32 Func_02006708(void);
void Func_02006b92(s32 scene);
void Func_02006a60(void);
s32 Func_02006a46(s32 flag);
void Func_02006b28(s32 message);
void Func_02006b30(s32 message);
void Func_02006b48(s32 actor, s32 mode);
void Func_02006a8c(void);
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
extern s32 Func_02006754(void);
void Func_02006bde(s32 scene);
void Func_02006aac(void);
void Func_02006b6a(s32 message);
s32 Func_02006a98(s32 flag);
void Func_02006b7a(s32 message);
void Func_02006b92_a(s32 actor, s32 mode);
void Func_02006ad6(void);
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */
extern s32 Func_0200679c(void);
void Func_02006c26(s32 scene);
void Func_02006af4(void);
void Func_02006bb2(s32 message);
void Func_02006bca(s32 actor, s32 mode);
void Func_02006b0e(void);

void SceneDialogue_RunActorEightFlaggedDialogue(void)
{
    if (Func_02006708() != 0) {
        Func_02006b92(8);
        return;
    }

    Func_02006a60();
    if (Func_02006a46(0x845) != 0)
        Func_02006b28(0x171c);
    else
        Func_02006b30(0x1408);
    Func_02006b48(8, 0);
    Func_02006a8c();
}

void SceneDialogue_RunActorEightFollowupDialogue(void)
{
    if (Func_02006754() != 0) {
        Func_02006bde(8);
        return;
    }

    Func_02006aac();
    Func_02006b6a(0x190a);
    if (Func_02006a98(0x909) != 0)
        Func_02006b7a(0x1951);
    Func_02006b92_a(8, 0);
    Func_02006ad6();
}

void SceneDialogue_RunActorEightDialogue(void)
{
    if (Func_0200679c() != 0) {
        Func_02006c26(8);
        return;
    }

    Func_02006af4();
    Func_02006bb2(0x1823);
    Func_02006bca(8, 0);
    Func_02006b0e();
}
