#include "VILLAGE.H"
extern u8 MsgFieldPeeredWell[];
extern u8 MsgRunpaWellFrogs[];

void FloatingNut_Catch(void)
{
    Event_Begin();
    Actor_SetPosition(ACTOR_FLOATING_NUT, 0, 0);
    GameFlag_Set(FLAG_LUNPA_NUT_CAUGHT);
    Item_ShowFound(ITEM_NUT, 3);
    Party_GiveItem(ITEM_NUT, 0);
    Event_End();
}

/* The frozen puddle stands as a pillar of ice the party can climb. */
void HiddenPuddle_Freeze(void)
{
    struct FieldActor *pillar;

    pillar = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    Actor_Get(ACTOR_PARTY_LEADER);
    Map_CopyCellAttributes(17, 4, 1, 1, 14, 4);
    Map_CopyCellAttributes(15, 3, 1, 1, 15, 4);
    Map_CopyCellAttributes(15, 3, 1, 1, 13, 4);
    if (pillar != NULL) {
        Actor_SetSpriteFlags(pillar, 0);
        pillar->motion_flags = ACTOR_FALLS;
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    GameFlag_Set(FLAG_LUNPA_PUDDLE_FROZEN);
}

/* A leader standing on the pillar draws above it. */
void IcePillar_UpdateDrawOrder(void)
{
    if (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
        Actor_Get(ACTOR_HIDDEN_PUDDLE)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    } else {
        Actor_Get(ACTOR_HIDDEN_PUDDLE)->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
}

void Reveal_ShowSecrets(void)
{
    struct FieldActor *actor;
    s32 cell_x;
    s32 cell_z;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    cell_x = actor->x.fixed / CELL_SIZE;
    cell_z = actor->z.fixed / CELL_SIZE;
    if (GameFlag_IsSet(FLAG_LUNPA_PSYNERGY_STONE) == 0) {
        if (cell_x == 7 && cell_z == 16) {
            Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
        }
        MapObject_SetPosition(MAP_OBJECT_PSYNERGY_STONE, -1, -1);
        Map_CopyCellAttributes(28, 31, 1, 1, 7, 16);
    }
    Map_CopyCells(47, 4, 1, 1, 46, 4);
    Map_CopyCellAttributes(34, 37, 3, 3, 13, 3);
    Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, PIXELS(232), PIXELS(72));
    Actor_Get(ACTOR_HIDDEN_PUDDLE)->y.fixed = 0;
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        Map_CopyCells(41, 49, 3, 4, 1, 14);
        Map_CopyCells(44, 49, 3, 4, 33, 14);
        Map_CopyCells(47, 49, 3, 4, 1, 46);
    } else {
        Actor_SetPosition(ACTOR_SWITCH_GLINT, PIXELS(56), PIXELS(268));
        Actor_SetSpriteFlags(Actor_Get(ACTOR_SWITCH_GLINT), 0);
        actor = Actor_Get(ACTOR_SWITCH_GLINT);
        if (actor != NULL) {
            actor->motion_flags = GLINT_MOTION_FLAGS;
            actor->y.fixed = PIXELS(16);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            actor->scale_x = GLINT_SCALE_X;
            actor->scale_y = GLINT_SCALE_Y;
        }
    }
    Task_AddCallback(IcePillar_UpdateDrawOrder, TASK_PRIORITY_SCENE);
    GameFlag_Clear(FLAG_LUNPA_SECRETS_HIDDEN);
}

void Reveal_HideSecrets(void)
{
    struct FieldActor *puddle;

    Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetPosition(ACTOR_SWITCH_GLINT, 0, 0);
    Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, 0, 0);
    Map_CopyCells(38, 38, 1, 1, 46, 4);
    Map_CopyCellAttributes(37, 37, 3, 3, 13, 3);
    Map_CopyCellAttributes(37, 37, 1, 1, 14, 2);
    Map_CopyCellAttributes(8, 16, 1, 1, 7, 16);
    MapObject_SetPosition(MAP_OBJECT_PSYNERGY_STONE, 0, 0);
    Map_CopyCells(32, 42, 3, 2, 1, 15);
    GameFlag_Clear(FLAG_LUNPA_PUDDLE_FROZEN);
    Actor_SetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
    puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    puddle->update = NULL;
    Object_SetPartPalettes(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
    Task_RemoveCallback(IcePillar_UpdateDrawOrder);
    GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
}

void Well_Search(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldPeeredWell, 1);
    Message_ShowCentered((s32)MsgRunpaWellFrogs, 1);
    Event_End();
}
