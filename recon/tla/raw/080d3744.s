.syntax unified
	.thumb
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	push {r5, r6, lr}
	adds r3, r0, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	adds r6, r1, #0
	cmp r3, #1
	bne .L_080d377e
	ldr r5, [r0, #80]
	movs r0, #0
	ldrb r4, [r5, #27]
	cmp r0, r4
	bge .L_080d377a
	adds r1, r5, #0
	adds r1, #40
.L_080d3764:
	ldmia r1!, {r2}
	cmp r2, #0
	beq .L_080d3774
	ldr r3, [r2, #16]
	cmp r3, #0
	beq .L_080d3774
	strb r6, [r2, #5]
	ldrb r4, [r5, #27]
.L_080d3774:
	adds r0, #1
	cmp r0, r4
	blt .L_080d3764
.L_080d377a:
	movs r3, #1
	strb r3, [r5, #25]
.L_080d377e:
	pop {r5, r6, pc}
