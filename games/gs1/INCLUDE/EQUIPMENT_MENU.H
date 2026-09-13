#ifndef ALCHEMY_EQUIPMENT_MENU_H
#define ALCHEMY_EQUIPMENT_MENU_H

#include "TYPES.H"

void EquipmentMenu_UpdateCompatibilityIndicators(void);
void EquipmentMenu_StartCompatibilityIndicators(void);
extern u8 Data_080a3c09;
#define EquipmentMenu_CompatibilityUpdateEntry Data_080a3c09

#endif
