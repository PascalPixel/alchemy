.syntax unified
	.thumb
	.global Func_08014840
	.thumb_func
Func_08014840:
	push {r5, r6, r7, lr}
	ldr r3, .L_08014870
	adds r7, r0, #0
	ldrb r3, [r3]
	ldr r5, .L_08014874
	asrs r7, r7, #8
	cmp r3, #1
	bne .L_0801486e
	movs r6, #25
	subs r5, #8
.L_08014854:
	subs r6, #1
	cmp r6, #0
	beq .L_0801486e
	adds r5, #8
	ldrb r3, [r5, #5]
	cmp r3, #4
	beq .L_0801486e
	cmp r3, r7
	bne .L_08014854
	ldr r0, [r5]
	mov lr, r0
	.2byte 0xf800
	b .L_08014854
.L_0801486e:
	pop {r5, r6, r7, pc}
.L_08014870:
	.4byte gSchedulerTaskCount
.L_08014874:
	.4byte gSchedulerTaskTable
