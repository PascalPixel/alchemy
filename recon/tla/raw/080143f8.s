.syntax unified
	.thumb
	.global Resource_GetBuffer
	.thumb_func
Resource_GetBuffer:
	push {lr}
	adds r2, r1, #0
	ldr r1, .L_08014408
	lsls r3, r0, #2
	ldrh r1, [r1, r3]
	bl VramBlock_LoadCached
	pop {pc}
.L_08014408:
	.4byte ResourceTableEntries
