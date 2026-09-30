#ifndef KORASHIAMU_IRIGUCHI_STATUS_H
#define KORASHIAMU_IRIGUCHI_STATUS_H

/* Declarations shared by the Colosso entrance's scene code. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define RECORD_17 17

/* The scene's action tables, in the overlay's read-only data. */
extern u8 KorashiamuIriguchi_ActionTable1[];
extern u8 KorashiamuIriguchi_ActionTable2[];
extern u8 KorashiamuIriguchi_ActionTable3[];
extern u8 KorashiamuIriguchi_ActionTable4[];
extern u8 KorashiamuIriguchi_ActionTable5[];
extern u8 KorashiamuIriguchi_ActionTable6[];
extern u8 KorashiamuIriguchi_ActionTable7[];
extern u8 KorashiamuIriguchi_ActionTable8[];
extern u8 KorashiamuIriguchi_ActionTable9[];

/* Import veneers outside the engine vocabulary, named by the main-image code
 * each one reaches; the main image leaves the routine that stores the party's
 * items in gInventorySnapshot unnamed, so its veneer is named for that.
 * Old-style declarations: arities vary between call sites. */
void Party_AddActiveOwner();
void Party_RemoveActiveOwner();
void Object_SetActionCallbackAndRefreshById();
void Object_SetActionById();
void InventorySnapshot_Take();

void FieldScene_RunMiddleSequence(void);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ __attribute__((always_inline)) void bump_step_now(s32 amount)
{
    gEventWork->message += amount;
}

#endif
