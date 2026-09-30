.syntax unified
	.thumb
	.global BattleFx_ShrinkObjectAndDestroyFast
	.thumb_func
BattleFx_ShrinkObjectAndDestroyFast:
	push {lr}
	ldr r1, .L_080d8170
	ldr r3, [r0, #28]
	ldr r2, [r0, #24]
	adds r3, r3, r1
	str r3, [r0, #28]
	ldrh r3, [r0, #6]
	adds r2, r2, r1
	movs r1, #128
	lsls r1, r1, #6
	adds r3, r3, r1
	strh r3, [r0, #6]
	movs r3, #192
	lsls r3, r3, #6
	str r2, [r0, #24]
	cmp r2, r3
	bge .L_080d816e
	bl Func_080200c8
.L_080d816e:
	pop {pc}
.L_080d8170:
	.4byte 0xfffffc00
