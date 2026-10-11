#ifndef ALCHEMY_UNIT_FIELD_KORIMA_HEYA_ENTRY_LAYOUT_H
#define ALCHEMY_UNIT_FIELD_KORIMA_HEYA_ENTRY_LAYOUT_H

#include "types.h"

struct Resource390TableEntry;

void SceneData_InitRecordTable(struct Resource390TableEntry *entry);
u8 *SceneData_GetPrimaryTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSecondaryTable(void);
u8 *SceneData_GetPreparedRecords(void);
void FieldScene_RunActor16MessageBranch(void);
void FieldScene_RunActor17MessageBranch(void);
void FieldScene_RunActor18MessageBranch(void);
void FieldScene_RunActor19MessageBranch(void);
u8 *SceneData_GetFourthTable(void);
s32 FieldScene_SetupEntryLayoutsBySelector(void);

#endif
