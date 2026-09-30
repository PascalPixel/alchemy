.syntax unified
	.thumb
	.global Func_080147d8
	.thumb_func
Func_080147d8:
	push {r5, r6, r7, lr}
	ldr r3, .L_08014804
	adds r7, r0, #0
	ldrb r3, [r3]
	ldr r6, .L_08014808
	asrs r7, r7, #8
	cmp r3, #1
	bne .L_08014802
	movs r5, #25
	subs r6, #8
.L_080147ec:
	subs r5, #1
	cmp r5, #0
	beq .L_08014802
	adds r6, #8
	ldrb r3, [r6, #5]
	cmp r3, r7
	bne .L_080147ec
	ldr r0, [r6]
	mov lr, r0
	.2byte 0xf800
	b .L_080147ec
.L_08014802:
	pop {r5, r6, r7, pc}
.L_08014804:
	.4byte gSchedulerTaskCount
.L_08014808:
	.4byte gSchedulerTaskTable
