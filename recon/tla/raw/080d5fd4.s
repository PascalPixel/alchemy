.syntax unified
	.thumb
	.global Func_080d5fd4
	.thumb_func
Func_080d5fd4:
	push {lr}
	ldr r3, .L_080d5ff4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl ObjectTable_Get
	movs r1, #134
	lsls r1, r1, #1
	ldr r0, [r0, #80]
	bl ResourceMetadata_RegisterFar
	movs r3, #15
	strb r3, [r0, #5]
	pop {pc}
.L_080d5ff4:
	.4byte gPartyState
