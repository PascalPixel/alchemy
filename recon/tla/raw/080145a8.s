.syntax unified
	.thumb
	.global Func_080145a8
	.thumb_func
Func_080145a8:
	push {r5, r6, lr}
	ldr r3, .L_08014600
	ldr r4, .L_08014604
	ldrb r3, [r3]
	movs r5, #1
	negs r5, r5
	ldr r3, .L_08014608
	ldrh r2, [r3]
	adds r6, r2, #0
	strh r3, [r3]
	movs r2, #0
	ldr r3, [r4]
	cmp r3, r0
	bne .L_080145ca
	strh r1, [r4, #4]
	movs r5, #0
	b .L_080145dc
.L_080145ca:
	adds r2, #1
	adds r4, #8
	cmp r2, #23
	bgt .L_080145dc
	ldr r3, [r4]
	cmp r3, r0
	bne .L_080145ca
	strh r1, [r4, #4]
	adds r5, r2, #0
.L_080145dc:
	movs r3, #1
	negs r3, r3
	ldr r4, .L_08014604
	cmp r5, r3
	bne .L_08014624
	ldr r3, [r4]
	movs r2, #0
	cmp r3, #0
	bne .L_0801460c
	ldr r3, .L_080145fc
	str r0, [r4]
	strh r1, [r4, #4]
	strb r3, [r4, #6]
	movs r5, #0
	b .L_08014624
	.2byte 0x0000
.L_080145fc:
	.4byte 0x00000000
.L_08014600:
	.4byte Data_03001108
.L_08014604:
	.4byte Data_02003610
.L_08014608:
	.4byte 0x04000208
.L_0801460c:
	adds r2, #1
	adds r4, #8
	cmp r2, #23
	bgt .L_08014624
	ldr r3, [r4]
	cmp r3, #0
	bne .L_0801460c
	ldr r3, .L_08014630
	str r0, [r4]
	strh r1, [r4, #4]
	strb r3, [r4, #6]
	adds r5, r2, #0
.L_08014624:
	bl Func_0801451c
	ldr r3, .L_08014634
	strh r6, [r3]
	adds r0, r5, #0
	b .L_08014638
.L_08014630:
	.4byte 0x00000000
.L_08014634:
	.4byte 0x04000208
.L_08014638:
	pop {r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00004770
	.4byte 0x00004770
