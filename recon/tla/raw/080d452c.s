.syntax unified
	.thumb
	.global Func_080d452c
	.thumb_func
Func_080d452c:
	push {r5, lr}
	adds r5, r1, #0
	bl ObjectTable_Get
	adds r3, r0, #0
	cmp r3, #0
	beq .L_080d4546
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	ldr r2, [r3, #16]
	adds r3, r5, #0
	bl Motion_CamBounds
.L_080d4546:
	pop {r5, pc}
