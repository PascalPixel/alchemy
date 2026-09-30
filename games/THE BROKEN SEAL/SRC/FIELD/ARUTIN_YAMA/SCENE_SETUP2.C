#include "YAMA.H"

void FieldScene_RunScene3a4_02002934(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 rec7;
    s32 record;
    s16 flag;

    rec7 = GameFlag_IsSet(0x909);
    if (rec7 != 0) {
        Actor_SetPosition(8, 0, 0);
        ((void (*)())Engine_ActorSetPosition)(9, 0, 0);
    } else {
        record = Actor_Get(8);
        Actor_SetSpriteFlags(record, 0);
        Actor_SetSpritePriority(9, 3);
        record = Actor_Get(9);
        Actor_SetSpriteFlags(record, 0);
        *(u8 *)((u8 *)Engine_ActorGet(9) + 89) = rec7;
    }
    flag = gGameState.entrance;
    if (flag == 1 || flag == 98) {
        if (GameFlag_IsSet(0x109) == 0) {
            rec7 = Engine_ActorGet(ACTOR_PARTY_LEADER);
            Event_Begin();
            *(s32 *)(rec7 + 12) = 0x100000;
            Event_End();
        }
    } else if (flag == 99) {
        if (GameFlag_IsSet(0x109) == 0) {
            RunEventScript01();
        }
    }
    /* unlifted: 0x020029bc..0x020029c2 (2) */
}

void FieldScene_RunScene3a4_020029dc(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 record;

    record = Actor_Get(9);
    Actor_SetSpriteFlags(record, 0);
    if (gGameState.entrance == 2) {
        Actor_SetPosition(9, 0xb80000, 0x1480000);
    }
}

/*
 * Clear the record byte at +0x55, then rewrite the handle flags at +9 as
 * (flags & ~0x0c) | 0x04 -- the mask is built from the zero already in v,
 * not spelled as a constant. The rate address is held in a local and
 * stored to both +24 and +28. The 50-byte owner includes its one pool
 * word.
 */
void SceneActor_SetMode3AndRate4ccc(u8 *rec)
{
    extern u32 Data_03001e40;

    u8 *p = rec + 0x55;
    s32 v = 0;
    u8 *h;

    *p = v;
    h = *(u8 **)(rec + 80);
    v -= 13;
    v &= h[9];
    v |= 4;
    h[9] = (u8)v;
    Object_SetPalette(rec, 3);
    Actor_SetSpriteFlags(rec, 0);
    {
        s32 rate = 0x4ccc;

        *(s32 *)(rec + 24) = rate;
        *(s32 *)(rec + 28) = rate;
    }
}
