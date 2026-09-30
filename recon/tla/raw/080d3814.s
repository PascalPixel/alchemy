.syntax unified
	.thumb
	.global Func_080d3814
	.thumb_func
Func_080d3814:
	push {r5, lr}
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080d3836
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #60]
	str r3, [r5, #64]
	bl Object_ResetMotion
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetMode
.L_080d3836:
	pop {r5, pc}
