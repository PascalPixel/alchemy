@ C2: aligned 8-byte veneers: each entry of the list is one overlay_veneer, ldr r4, bx r4 and a target word.
#include "stub.inc"
	.global FarCall_RuntimeTable
FarCall_RuntimeTable:
#include "../../../include/farcall/runtime.h"
