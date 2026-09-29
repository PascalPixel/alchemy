/* Draft: FieldScene_RunActor11FlagDialogue, resource_3b1 at 0x02008a80 (listing 0x02000a80).
 * Not linked: its flag and message ids load from the literal pool as link-time constants (Value_0000092b, Value_00001e7e and siblings) that the main image does not define; spelled as plain constants GCC derives one from another and the owner comes out 4 bytes short.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneDontFeelDontTalkMe[];
extern u8 MsgFuneDontTellGoing[];
extern u8 MsgFuneYouFinallyPickedSomeoneDidnt[];


struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};
extern u8 Value_0000092b;
extern u8 Value_00000993;
extern u8 Value_0000092a;
extern u8 Value_0000091a;
extern u8 Value_00000929;
extern u8 Value_00000938;
extern u8 Value_0000092f;
void Func_02001c60(u8 *o);
u8 *Func_02006fd2(s32 n);

void FieldScene_RunActor11FlagDialogue(void)
{
    if (GameFlag_IsSet(0x8A0) != 0) {
        Event_Begin();
        Actor_SetAttachedEffect(11, 0x102);
        Event_Wait(40);
        Event_SetMessage((s32)MsgFuneDontFeelDontTalkMe);
        Event_ShowMessage(11, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        Func_02001c60(o);
        Event_SetMessage((s32)MsgFuneYouFinallyPickedSomeoneDidnt);
        FieldScene_RunStepThen10(11);
        Actor_SetAnimation(o, 2);
        p = Func_02006fd2(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, (s32)&Value_00000993);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, (s32)&Value_0000091a);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, (s32)&Value_00000938);
    } else {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, (s32)&Value_0000092f);
    }
}
