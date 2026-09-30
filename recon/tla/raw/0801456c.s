.syntax unified
	.thumb
	.global Func_0801456c
	.thumb_func
Func_0801456c:
	push {r5, lr}
	ldr r4, .L_080145a0
	movs r5, #1
	negs r5, r5
	ldr r3, .L_080145a4
	ldrh r2, [r3]
	strh r3, [r3]
	movs r1, #0
	ldr r3, [r4]
	cmp r3, r0
	bne .L_08014586
	movs r5, #0
	b .L_08014596
.L_08014586:
	adds r1, #1
	adds r4, #8
	cmp r1, #23
	bgt .L_08014596
	ldr r3, [r4]
	cmp r3, r0
	bne .L_08014586
	adds r5, r1, #0
.L_08014596:
	ldr r3, .L_080145a4
	strh r2, [r3]
	adds r0, r5, #0
	pop {r5, pc}
	.2byte 0x0000
.L_080145a0:
	.4byte Data_02003610
.L_080145a4:
	.4byte 0x04000208
