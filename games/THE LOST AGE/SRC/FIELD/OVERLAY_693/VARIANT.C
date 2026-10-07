#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIELD_EVENT.H"
#include "FIELD_SPRITE.H"

struct FieldActor *Object_GetById(s32 id);

void Func_02000bb8(void)
{
    s32 variant = Object_GetById(gPartyState.current_owner)->sprite->priority;

    ObjectMotion_SetActionVariant(16, variant);
    ObjectMotion_SetActionVariant(8, variant);
    ObjectMotion_SetActionVariant(10, variant);
}

void Func_02000bf0(void)
{
    ObjectMotion_SetActionVariant(13, Object_GetById(gPartyState.current_owner)->sprite->priority);
}
