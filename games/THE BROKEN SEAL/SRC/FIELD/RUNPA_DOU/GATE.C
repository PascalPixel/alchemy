#include "CAVE.H"
extern u8 MsgFieldFlippedSwitch[];

const struct SceneEvent *Scene_GetEvents(void)
{
    return gCaveEvents;
}

void HiddenPuddle_Freeze(void)
{
    struct FieldActor *puddle;

    puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    if (puddle != NULL) {
        Actor_SetSpriteFlags(puddle, 0);
    }
}

/* The pillar raised from the south puddle changes the ground it stands on. */
void SouthPuddle_Freeze(void)
{
    struct FieldActor *puddle;

    puddle = Actor_Get(ACTOR_SOUTH_PUDDLE);
    if (puddle != NULL) {
        puddle->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
        puddle->motion_flags = 0;
    }
    Map_CopyCellAttributes(7, 32, 1, 1, 8, 32);
    GameFlag_Set(FLAG_CAVE_SOUTH_PILLAR);
}

/* The gate rises onto the frozen pillar and stays there. */
void Gate_DrawPropped(void)
{
    struct FieldActor *pillar;

    pillar = Actor_Get(ACTOR_GATE_PUDDLE);
    Actor_SetAnimation(ACTOR_GATE_PUDDLE, PUDDLE_ANIM_FROZEN);
    if (pillar != NULL) {
        Actor_SetSpriteFlags(pillar, 0);
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    Map_CopyCells(41, 87, 2, 5, 21, 59);
    Task_Wait(4);
    Map_CopyCells(3, 93, 1, 1, 24, 62);
    Map_CopyCells(1, 94, 1, 1, 21, 55);
    Map_CopyCells(43, 87, 2, 5, 21, 58);
    Task_Wait(4);
    Map_CopyCells(41, 87, 2, 5, 21, 58);
    Task_Wait(4);
    Task_Wait(4);
    Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
    Map_CopyCellAttributes(21, 11, 1, 1, 22, 15);
    Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

void GatePuddle_Freeze(void)
{
    struct FieldActor *pillar;

    pillar = Actor_Get(ACTOR_GATE_PUDDLE);
    GameFlag_Set(FLAG_CAVE_GATE_PROPPED);
    if (pillar != NULL) {
        Actor_SetSpriteFlags(pillar, 0);
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) == 0) {
        Audio_PlayCue(SOUND_GATE_MOVE);
        Gate_DrawPropped();
        Audio_PlayCue(SOUND_PUZZLE_SOLVED);
        GameFlag_Set(FLAG_CAVE_GATE_RAISED);
    }
}

void NorthPuddle_Freeze(void)
{
    GameFlag_Set(FLAG_CAVE_NORTH_PILLAR);
}

void Gate_Lower(void)
{
    if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) != 0) {
        Map_CopyCells(41, 86, 2, 6, 21, 57);
        Task_Wait(4);
        Map_CopyCells(43, 86, 2, 6, 21, 57);
        Task_Wait(4);
        Map_CopyCells(41, 86, 2, 6, 21, 58);
        Task_Wait(4);
        Map_CopyCells(43, 86, 2, 6, 21, 58);
        Task_Wait(4);
    }
    Map_CopyCells(2, 93, 1, 1, 24, 62);
    Map_CopyCells(2, 94, 1, 1, 21, 55);
    Map_CopyCells(41, 86, 2, 6, 21, 59);
    Task_Wait(4);
    Map_CopyCells(1, 93, 1, 1, 24, 62);
    Map_CopyCells(3, 94, 1, 1, 21, 55);
    Map_CopyCells(43, 86, 2, 6, 21, 59);
    Task_Wait(4);
    Actor_SetSpritePriority(ACTOR_GATE_PUDDLE, GATE_PILLAR_PRIORITY);
    Map_CopyCellAttributes(19, 17, 1, 1, 22, 15);
}

void LoweringSwitch_Flip(void)
{
    if (GameFlag_IsSet(FLAG_CAVE_GATE_LOWERED) == 0) {
        if (GameFlag_IsSet(FLAG_CAVE_GATE_PROPPED) == 0) {
            Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(SOUND_GATE_MOVE);
            Gate_Lower();
            GameFlag_Set(FLAG_CAVE_GATE_LOWERED);
            GameFlag_Clear(FLAG_CAVE_GATE_RAISED);
        }
    }
}

void Gate_Raise(void)
{
    Map_CopyCells(41, 87, 2, 5, 21, 59);
    Task_Wait(4);
    Map_CopyCells(2, 93, 1, 1, 24, 62);
    Map_CopyCells(2, 94, 1, 1, 21, 55);
    Map_CopyCells(43, 87, 2, 5, 21, 58);
    Task_Wait(4);
    Map_CopyCells(3, 93, 1, 1, 24, 62);
    Map_CopyCells(1, 94, 1, 1, 21, 55);
    Map_CopyCells(41, 87, 2, 5, 21, 58);
    Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
    Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

void RaisingSwitch_Flip(void)
{
    if (GameFlag_IsSet(FLAG_CAVE_GATE_PROPPED) == 0) {
        if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) == 0) {
            Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(SOUND_GATE_MOVE);
            Gate_Raise();
            GameFlag_Set(FLAG_CAVE_GATE_RAISED);
            GameFlag_Clear(FLAG_CAVE_GATE_LOWERED);
        }
    }
}
