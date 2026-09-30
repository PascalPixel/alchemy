#include "MURA.H"

void FieldScene_RunScene38b_020008f0(void)
{

    struct FieldActor *record;
    s16 sub_state;

    if (GameFlag_IsSet(0x845) != 0) {
        ((void (*)())Engine_ActorSetPosition)(9, 0, 0);
        Actor_FaceDirection(14, 0x3000, 0);
        Actor_FaceDirection(15, 0x5000, 0);
    } else {
        record = Actor_Get(9);
        Actor_SetSpriteFlags(record, 0);
        Actor_SetPosition(21, 0, 0);
    }
    record = Actor_Get(8);
    record->scale_y = 0x18000;
    {
        s32 off = 450;
        sub_state = *(s16 *)((u8 *)&gGameState + off);
    }
    if (sub_state == 10) {
        Actor_SetPosition(8, 0, 0);
    } else {
        if (sub_state == 9) {
            GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
        }
    }
    if (GameFlag_IsSet(0x109) == 0) {
        {
            s32 off = 450;
            sub_state = *(s16 *)((u8 *)&gGameState + off);
        }
        if (sub_state == 11) {
            Actor_SetPosition(20, 0xf80000, 0xd80000);
        }
    }
    Scene_UpdatePuzzleActors();
    if (GameFlag_IsSet(0x84a) != 0) {
        if (GameFlag_IsSet(0x84b) == 0) {
            GameFlag_Set(0x304);
        }
    }
}

void Scene_UpdatePuzzleActors(void)
{
    s32 p10;
    s32 p9;
    s32 rec7;
    s32 record;
    s32 p6;
    s32 row;

    rec7 = Engine_ActorGet(ACTOR_PARTY_LEADER);
    record = Engine_ActorGet(20);
    row = *(s32 *)(record + 16) >> 20;
    p9 = (*(s32 *)(rec7 + 8) >> 20);
    p10 = (*(s32 *)(rec7 + 16) >> 20);
    p6 = *(s32 *)(record + 8);
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 12);
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 13);
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 14);
    Map_CopyCellAttributes(1, 0, 1, 1, (p6 >> 20), row);
    if (((s32)p6 >> 20) == 16) {
        if (row == 13) {
            goto L_02000a60;
        }
    }
    Map_CopyCellAttributes(0, 0, 1, 1, 16, 13);
    L_02000a60:;
    if (p9 == 16) {
        if (p10 == 13) {
            Event_Begin();
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
            if (row == 13) {
                Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x106, 196);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
            } else {
                Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x11e, 218);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
            }
            Event_End();
        }
    }
}

void FieldScene_RunScene38bSequenceA(void)
{
    u32 i;
    struct FieldActor *rec;
    s32 rec7;
    struct FieldActor *rec8;
    s32 record;

    rec8 = Engine_ActorGet(10);
    rec = Engine_ActorGet(11);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 0);
    rec7 = GameFlag_IsSet(0x845);
    if (rec7 != 0) {
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Map_CopyCellsTo(56, 15, 40, 15, 1, 2);
        Map_CopyCellAttributes(26, 15, 1, 3, 10, 15);
        if (GameFlag_IsSet(0x849) == 0) {
            if (GameFlag_IsSet(0x848) != 0) {
                goto L_02000c92;
            }
            ((void (*)())Engine_ActorSetPosition)(14, 0, 0);
        }
        Actor_FaceDirection(12, 0xd000, 0);
        Actor_FaceDirection(13, 0xb000, 0);
    } else {
        Actor_SetPosition(12, 0, 0);
        Actor_SetPosition(13, 0, 0);
        Actor_SetPosition(14, 0, 0);
        record = Actor_Get(9);
        Actor_SetSpriteFlags(record, 0);
        record = Actor_Get(10);
        Actor_SetSpriteFlags(record, 0);
        record = Actor_Get(11);
        Actor_SetSpriteFlags(record, 0);
        rec8->motion_flags = rec7;
        record = GameFlag_IsSet(0x881);
        if (record != 0) {
            *((u8 *)Engine_ActorGet(9) + 89) |= 16;
            *((u8 *)Engine_ActorGet(16) + 89) |= 16;
            *((u8 *)Engine_ActorGet(11) + 89) |= 16;
            Actor_SetPosition(16, 0x8e0000, 0x9c0000);
            record = Actor_Get(16);
            Actor_SetSpriteFlags(record, 0);
            Actor_SetPosition(10, 0x8e0000, 0x9c0000);
            rec8->sprite->rotation = 0x4000;
            rec8->y.fixed += -0x80000;
            if (GameFlag_IsSet(0x848) != 0) {
                Actor_SetPosition(11, 0x840000, 0xba0000);
                goto L_02000c92;
            }
            Actor_SetPosition(11, 0x580000, 0xc40000);
            Actor_SetSpritePriority(11, 3);
            rec->collision_flags |= 4;
        } else {
            rec8->y.fixed = 0x200000;
            rec->motion_flags = record;
            rec->y.fixed = 0x300000;
        }
    }
    L_02000c92:;
    ActorPresentation_RepaintTenCellsAndActorEightCell();
}

/*
 * Ten (x, z) tile pairs, held in the overlay's own writable image.  Overlay
 * data lives in EWRAM and is deliberately not const.
 */

/*
 * Slot accessor: Engine_ActorGet(slot) returns the actor record, or NULL.
 * Typed as a byte pointer so the +0x08 and +0x10 field reads are explicit.
 */

/*
 * The six-argument renderer ABI: four register arguments plus two stack
 * words, here the tile x and tile z of the cell being repainted.  The two
 * names are separate per-site call words that reach the same renderer.
 */

/*
 * Repaint ten fixed collision cells and then actor 8's own cell.  The
 * 92-byte owner includes the alignment halfword and the single pool word
 * that follows the code; that word holds the address of Mura_RepaintCells, which
 * is in-image data rather than a RAM global.  The two renderer calls must
 * keep their separate call words -- naming one renderer for both changes the
 * displacement emitted at each site.
 */
void ActorPresentation_RepaintTenCellsAndActorEightCell(void)
{
    u8 *actor;
    s32 tx;
    s32 tz;
    u32 i;

    /*
     * Slot 8 is the scene's own actor; the accessor result is not
     * null-checked here.  The 20-bit shift is one signed arithmetic shift:
     * 16 takes the fixed-point coordinate to pixels, the further 4 take it
     * to the 16-pixel tile grid.
     */
    actor = Actor_Get(8);
    tx = *(s32 *)(actor + 0x08) >> 20;
    tz = *(s32 *)(actor + 0x10) >> 20;

    /*
     * Ten fixed cells from the table, then the actor's own cell.  The table
     * is walked by the byte index itself rather than by a 0..9 counter
     * scaled by two, so the loop steps the byte offset directly.
     */
    for (i = 0; i < 20; i += 2) {
        s32 x = (s32)Mura_RepaintCells[i];
        s32 z = (s32)Mura_RepaintCells[i + 1];
        Map_CopyCellAttributes(1, 0, 1, 1, x, z);
    }

    /*
     * The same repaint with 0 rather than 1 in the first argument.  What
     * that selector chooses is not established.
     */
    Map_CopyCellAttributes(0, 0, 1, 1, tx, tz);
}

void FieldScene_RunScene38b_02000d10(void)
{
    s32 arg0;
    s32 rec7;
    s32 record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    FieldScene_DrawTilesByActor8Row();
    record = ReadU16Elem((u16 *)&gGameState, 225);
    if ((u32)((record - 3) << 16) <= 0x10000) {
        if (GameFlag_IsSet(0x109) == 0) {
            rec7 = Engine_ActorGet(ACTOR_PARTY_LEADER);
            Event_Begin();
            arg0 = *(s32 *)(rec7 + 8);
            *(s32 *)(rec7 + 12) = 0x100000;
            Camera_MoveTo(arg0, 0x100000, *(s32 *)(rec7 + 16), 0);
            Map_Redraw();
            Event_End();
            Task_Wait(1);
        }
    }
}

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z)
{

    s32 **tbl = (s32 **)((u8 *)gEventWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}
