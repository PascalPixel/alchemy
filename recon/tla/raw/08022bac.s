.syntax unified
	.thumb
	.global Animation_ApplyChildValue
	.thumb_func
Animation_ApplyChildValue:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldrb r4, [r5, #27]
	movs r0, #0
	adds r6, r1, #0
	cmp r0, r4
	bge .L_08022bd4
	adds r1, r5, #0
	adds r1, #40
.L_08022bbe:
	ldmia r1!, {r2}
	cmp r2, #0
	beq .L_08022bce
	ldr r3, [r2, #12]
	cmp r3, #0
	beq .L_08022bce
	strb r6, [r2, #21]
	ldrb r4, [r5, #27]
.L_08022bce:
	adds r0, #1
	cmp r0, r4
	blt .L_08022bbe
.L_08022bd4:
	pop {r5, r6, pc}
	.2byte 0x0000
