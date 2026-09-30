.syntax unified
	.thumb
	.global Func_080d36f4
	.thumb_func
Func_080d36f4:
	push {r5, r6, lr}
	adds r3, r0, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #1
	bne .L_080d373a
	ldr r3, .L_080d373c
	ldr r5, [r0, #80]
	ldr r3, [r3]
	ldr r1, .L_080d3740
	lsrs r3, r3, #1
	movs r2, #3
	ands r3, r2
	ldrb r6, [r1, r3]
	ldrb r1, [r5, #27]
	movs r0, #0
	cmp r0, r1
	bge .L_080d3736
	adds r4, r5, #0
	adds r4, #40
.L_080d3720:
	ldmia r4!, {r2}
	cmp r2, #0
	beq .L_080d3730
	ldr r3, [r2, #16]
	cmp r3, #0
	beq .L_080d3730
	strb r6, [r2, #5]
	ldrb r1, [r5, #27]
.L_080d3730:
	adds r0, #1
	cmp r0, r1
	blt .L_080d3720
.L_080d3736:
	movs r3, #1
	strb r3, [r5, #25]
.L_080d373a:
	pop {r5, r6, pc}
.L_080d373c:
	.4byte Data_0300122c
.L_080d3740:
	.4byte Data_080f088c
