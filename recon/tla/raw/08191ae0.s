.syntax unified
	.thumb
	.global Func_08191ae0
	.thumb_func
Func_08191ae0:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldrb r4, [r5, #27]
	movs r0, #0
	adds r6, r1, #0
	cmp r0, r4
	bge .L_08191b08
	adds r1, r5, #0
	adds r1, #40
.L_08191af2:
	ldmia r1!, {r2}
	cmp r2, #0
	beq .L_08191b02
	ldr r3, [r2, #16]
	cmp r3, #0
	beq .L_08191b02
	strb r6, [r2, #5]
	ldrb r4, [r5, #27]
.L_08191b02:
	adds r0, #1
	cmp r0, r4
	blt .L_08191af2
.L_08191b08:
	movs r3, #1
	strb r3, [r5, #25]
	pop {r5, r6, pc}
	.2byte 0x0000
