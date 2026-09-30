.syntax unified
	.thumb
	.global Func_081196fc
	.thumb_func
Func_081196fc:
	push {lr}
	movs r1, #76
	movs r0, #48
	bl Runtime_AllocateBlock
	movs r1, #128
	adds r3, r0, #0
	adds r3, #12
	movs r2, #0
	lsls r1, r1, #15
	str r2, [r3]
	str r1, [r3, #4]
	str r2, [r3, #8]
	movs r3, #180
	lsls r3, r3, #16
	str r3, [r0, #4]
	movs r3, #160
	lsls r3, r3, #6
	strh r3, [r0, #54]
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #17
	str r2, [r0]
	str r1, [r0, #8]
	str r3, [r0, #32]
	pop {pc}
