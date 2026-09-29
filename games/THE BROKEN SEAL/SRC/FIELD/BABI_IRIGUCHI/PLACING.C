#include "IRIGUCHI.H"

void SceneState_SetRuntimeByte34(void)
{
    FIELD_AT_OFFSET(*(void **)gEffectWork, s8 *, 0x34) = 1;
}

void ActorPresentation_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Actor_Get(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Iriguchi_CopyCellAttributes(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

void SceneActor_PushObjectAheadIfLevel(void)
{
    struct LevelCheckRecord *p = Actor_Get(ACTOR_PARTY_LEADER);
    struct LevelCheckRecord *q = BabiIriguchi_FindActorAhead(p);
    s32 diff;

    if (q == 0) {
        return;
    }

    diff = q->y - p->y;

    if (diff >= 0) {
        /* Written with an empty arm on purpose: the reference branches away on
         * the *return* condition (`bge`), and spelling this as a plain
         * `if (diff >= 0x80000) return;` inverts it to `blt`. Arm order
         * decides the branch sense; no flag moves it. */
        if (diff < 0x80000) {
        } else {
            return;
        }
    } else if (p->y - q->y >= 0x80000) {
        return;
    }

    StagedActor_AdvancePair();
}

