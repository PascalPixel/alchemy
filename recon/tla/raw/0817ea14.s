.syntax unified
	.thumb
	.global Func_0817ea14
	.thumb_func
Func_0817ea14:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #144
	lsls r3, r3, #1
	mov lr, r0
	adds r6, r1, #0
	mov r12, r2
	movs r7, #0
	mov r8, r3
	movs r5, #0
.L_0817ea2a:
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r0, r3, #1
	mov r3, lr
	movs r1, #0
	adds r4, r5, r3
.L_0817ea36:
	lsrs r3, r1, #31
	adds r3, r1, r3
	ldrb r2, [r4]
	asrs r3, r3, #1
	adds r3, r0, r3
	adds r1, #1
	adds r4, #1
	strb r2, [r6, r3]
	cmp r1, #40
	bne .L_0817ea36
	adds r7, #1
	add r5, r12
	cmp r7, r8
	bne .L_0817ea2a
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
