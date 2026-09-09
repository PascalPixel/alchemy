/*
 * Overlay resource_380: the per-actor talk handlers of the party
 * introduction scene.
 */

#include "types.h"

#define SceneDialogue_RunActorTenDialogue Func_0200274c
#define SceneDialogue_RunActor11TalkLine Func_0200276c
#define SceneDialogue_ShowLine10C8 Func_0200278c
#define SceneDialogue_RunActor14TalkLine Func_020027ac
#define SceneDialogue_RunLine1072WithPair9And10 Func_020027cc
void Func_02007106(void);
void Func_020071aa(s32 line);
void Func_020071c2(s32 id, s32 arg1);
void Func_02007126(void);
void Func_020071ca(s32 no);
void Func_020071e2(s32 no, s32 val);
void Func_02007146(void);
void Func_020071ea(s32 dialogueId);
void Func_02007202(s32 id, s32 arg1);
void Func_02007166(void);
void Func_0200720a(s32 no);
void Func_02007222(s32 no, s32 val);
void Func_02006a26(s32 a, s32 b);
void Func_02007186(void);
void Func_0200722a(s32 no);

/*
 * Talk handler for actor 10: the scripted-scene bracket around one line of
 * dialogue, with no branch and no state. It is published into a script record
 * rather than called from the image.
 *
 * The thirty-two-byte owner includes its one pool word, which carries the
 * line id, and the zero alignment halfword after it. The second argument of
 * the tail call is zero at every site in this bank and its role is not
 * established.
 */
void SceneDialogue_RunActorTenDialogue(void)
{
extern void Func_020070ec(void);

    Func_020070ec();
    Func_020071aa(0x10ca);
    Func_020071c2(10, 0);
    Func_02007106();
}

/*
 * Talk handler for actor 11, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_RunActor11TalkLine(void)
{
extern void Func_0200710c(void);

    Func_0200710c();
    Func_020071ca(0x10c7);
    Func_020071e2(11, 0);
    Func_02007126();
}

/*
 * Talk handler for actor 13, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_ShowLine10C8(void)
{
extern void Func_0200712c(void);

    Func_0200712c();
    Func_020071ea(0x10c8);
    Func_02007202(13, 0);
    Func_02007146();
}

/*
 * Talk handler for actor 14, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_RunActor14TalkLine(void)
{
extern void Func_0200714c(void);

    Func_0200714c();
    Func_0200720a(0x10cc);
    Func_02007222(14, 0);
    Func_02007166();
}

/*
 * Same bracket and line call as the talk handlers, but the tail pairs actors
 * 9 and 10 through the overlay's own call.
 */
void SceneDialogue_RunLine1072WithPair9And10(void)
{
    Func_0200716c();
    Func_0200722a(0x1072);
    Func_02006a26(9, 10);
    Func_02007186();
}
