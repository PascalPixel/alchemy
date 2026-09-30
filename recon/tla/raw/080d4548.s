.syntax unified
	.thumb
	.global Func_080d4548
	.thumb_func
Func_080d4548:
	push {lr}
	movs r1, #213
	lsls r1, r1, #4
	movs r0, #108
	bl Runtime_AllocateBlock
	movs r3, #230
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r0, [r0]
	bl Object_CommitPosition
	movs r0, #2
	bl Battle_WaitMode0
	pop {pc}
