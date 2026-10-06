#ifndef KORASHIAMU_IRIGUCHI_STATUS_H
#define KORASHIAMU_IRIGUCHI_STATUS_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define RECORD_17 17

extern u8 KorashiamuIriguchi_ActionTable1[];
extern u8 KorashiamuIriguchi_ActionTable2[];
extern u8 KorashiamuIriguchi_ActionTable3[];
extern u8 KorashiamuIriguchi_ActionTable4[];
extern u8 KorashiamuIriguchi_ActionTable5[];
extern u8 KorashiamuIriguchi_ActionTable6[];
extern u8 KorashiamuIriguchi_ActionTable7[];
extern u8 KorashiamuIriguchi_ActionTable8[];
extern u8 KorashiamuIriguchi_ActionTable9[];

void Party_AddActiveOwner();
void Party_RemoveActiveOwner();
void Object_SetActionCallbackAndRefreshById();
void Object_SetActionById();
void InventorySnapshot_Take();

void FieldScene_RunMiddleSequence(void);

#endif
