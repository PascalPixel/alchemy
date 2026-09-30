.syntax unified
	.thumb
	.global Event_CallWithLastActiveObjectId
	.thumb_func
Event_CallWithLastActiveObjectId:
	push {r5, lr}
	adds r5, r0, #0
	bl ObjectTable_FindLastActiveId
	adds r1, r0, #0
	adds r0, r5, #0
	bl Event_SpawnObjectTable
	pop {r5, pc}
	.2byte 0x0000
