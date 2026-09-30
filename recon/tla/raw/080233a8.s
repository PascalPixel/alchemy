.syntax unified
	.thumb
	.global ObjectDispatch_Initialize
	.thumb_func
ObjectDispatch_Initialize:
	push {lr}
	cmp r0, #0
	beq .L_080233cc
	ldr r3, .L_080233c8
	movs r2, #0
	strh r2, [r0, #4]
	adds r2, r0, #0
	adds r2, #91
	str r1, [r0]
	strb r3, [r2]
	adds r2, #2
	strb r3, [r2]
	subs r2, #6
	strb r3, [r2]
	b .L_080233cc
	.2byte 0x0000
.L_080233c8:
	.4byte 0x00000000
.L_080233cc:
	pop {pc}
	.2byte 0x0000
