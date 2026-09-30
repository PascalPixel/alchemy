.syntax unified
	.thumb
	.global Func_0815e170
	.thumb_func
Func_0815e170:
	push {r5, r6, lr}
	ldr r3, [r0]
	adds r6, r2, #0
	ldr r2, [r0, #12]
	ldr r4, [r0, #16]
	adds r3, r3, r2
	str r3, [r0]
	ldr r3, [r0, #4]
	ldr r5, [r0, #20]
	adds r3, r3, r4
	str r3, [r0, #4]
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	adds r3, r1, #0
	muls r3, r2
	cmp r3, #0
	bge .L_0815e196
	adds r3, #63
.L_0815e196:
	asrs r3, r3, #6
	str r3, [r0, #12]
	adds r3, r4, r6
	adds r2, r1, #0
	muls r2, r3
	cmp r2, #0
	bge .L_0815e1a6
	adds r2, #63
.L_0815e1a6:
	muls r1, r5
	asrs r3, r2, #6
	str r3, [r0, #16]
	cmp r1, #0
	bge .L_0815e1b2
	adds r1, #63
.L_0815e1b2:
	asrs r3, r1, #6
	str r3, [r0, #20]
	pop {r5, r6, pc}
