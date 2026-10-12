#ifndef ALCHEMY_OBJECT_DISPATCH_H
#define ALCHEMY_OBJECT_DISPATCH_H

#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field4.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "types.h"

struct DispatchObject
{
	u32 value_00;
	s16 value_04;
	u8	unknown_06[0x2a];
	s32 value_30;
	s32 value_34;
	u8	unknown_38[0x18];
	union
	{
		void  *child;
		void **children;
	} target;
	u8	kind;
	u8	unknown_55[2];
	u8	value_57;
	u8	unknown_58[3];
	u8	value_5b;
	u8	unknown_5c;
	u8	value_5d;
	u8	unknown_5e[6];
	s16 value_64;
	u8	unknown_66[2];
	s32 argument;
	u8	unknown_6c[4];
};

extern const u32 ObjectDispatch_Table0Script[];
extern const u32 ObjectDispatch_Table1Script[];
extern const u32 ObjectDispatch_Table2Script[];
extern const u32 ObjectDispatch_Table3Script[];
extern const u32 ObjectDispatch_Table4Script[];
extern const u32 ObjectDispatch_Table5Script[];
extern const u32 ObjectDispatch_Table6Script[];

#endif
