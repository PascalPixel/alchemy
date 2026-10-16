@ Resource animation metadata lookup.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_ResourceTable
FarCall_ResourceTable:
	export Resource_GetMetadataRecordFar, Resource_GetMetadataRecord
