#ifndef ALCHEMY_EQUIPMENT_MENU_H
#define ALCHEMY_EQUIPMENT_MENU_H

#include "types.h"

void Func_080a3c08(void);
void Func_080a3c98(void);
extern u8 Data_080a3c09;

#define EquipmentMenu_UpdateCompatibilityIndicators Func_080a3c08
#define EquipmentMenu_StartCompatibilityIndicators  Func_080a3c98
#define EquipmentMenu_CompatibilityUpdateEntry       Data_080a3c09

#endif
