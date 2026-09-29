/* Draft: FieldScene_RunScene3b1_020040e8, resource_3b1 at 0x0200c0e8 (listing 0x020040e8).
 * Not linked: its message id loads from the literal pool as a link-time constant (LinkedMessage_Monsters, 0x1e40) that the main image does not define; spelled as a plain constant GCC builds it differently.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneMonsters[];


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
void Func_020089de();
void Func_02008a1a();

void FieldScene_RunScene3b1_020040e8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Func_020089de(15, 1, 1);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(10);
    Actor_FaceDirection(8, 0x3000, 20);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgFuneMonsters);
    Event_ShowMessageAndWait(8, 0, 20);
    Func_02008a1a(9, 14, 0);
}
