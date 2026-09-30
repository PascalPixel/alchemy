.syntax unified
	.thumb
	.global Func_080cee80
	.thumb_func
Func_080cee80:
	push {lr}
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080ceea8
	ldr r0, [r0, #80]
	cmp r0, #0
	beq .L_080ceea8
	ldrb r1, [r0, #5]
	movs r3, #8
	movs r2, #63
	strb r3, [r0, #23]
	adds r3, r2, #0
	ands r3, r1
	strb r3, [r0, #5]
	ldrb r3, [r0, #7]
	ands r2, r3
	movs r3, #64
	orrs r2, r3
	strb r2, [r0, #7]
.L_080ceea8:
	pop {pc}
	.2byte 0x0000
