.syntax unified
	.thumb
	.global Func_080d2810
	.thumb_func
Func_080d2810:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080d283e
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d283e
	ldr r6, [r0, #80]
	movs r5, #0
	b .L_080d282e
.L_080d282c:
	adds r5, #1
.L_080d282e:
	cmp r5, #89
	bgt .L_080d283e
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r6, #24]
	cmp r7, r3
	beq .L_080d282c
.L_080d283e:
	pop {r5, r6, r7, pc}
