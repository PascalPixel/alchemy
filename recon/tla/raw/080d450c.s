.syntax unified
	.thumb
	.global Func_080d450c
	.thumb_func
Func_080d450c:
	push {r5, lr}
	adds r5, r1, #0
	bl ObjectTable_Get
	adds r3, r0, #0
	cmp r3, #0
	beq .L_080d4528
	movs r1, #1
	ldr r0, [r3, #8]
	ldr r2, [r3, #16]
	negs r1, r1
	adds r3, r5, #0
	bl Motion_CamBounds
.L_080d4528:
	pop {r5, pc}
	.2byte 0x0000
