@ The scrolling slideshow entry point.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_ScrollTable
FarCall_ScrollTable:
	export Credits_RunFar, Credits_Run
