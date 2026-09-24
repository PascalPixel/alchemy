#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 StagedActor_FindAtTile(s32 *pos, struct FieldActor *actor);
void FieldScene_RunScene3b3_02001fd4(void);
s32 FieldScene_RunScene3b3SequenceD(void);
void Main_0808a2a8(void);

/* Runs the scene when a staged actor stands in the cell a step further along z,
 * otherwise falls back to sequence D. */
void TakaraHashira_Func02002040(void)
{
    struct FieldActor *leader = Engine_ActorGet(0);
    s32 pos[3];
    s32 *p = pos;

    p[0] = leader->x.fixed;
    p[1] = leader->y.fixed;
    p[2] = leader->z.fixed + 0x100000;
    if (StagedActor_FindAtTile(p, leader) != 0) {
        FieldScene_RunScene3b3_02001fd4();
    } else if (FieldScene_RunScene3b3SequenceD() == 0) {
        Main_0808a2a8();
    }
}
