.syntax unified
	.thumb
	.global Func_080ca368
	.thumb_func
Func_080ca368:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	adds r6, r1, #0
	ldr r5, .L_080ca3e8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ca3e2
	ldr r1, .L_080ca3ec
	movs r0, #0
	ldrsh r3, [r5, r0]
	asrs r0, r1, #16
	ldrh r2, [r5]
	cmp r3, r0
	beq .L_080ca3e2
	adds r4, r1, #0
	ldr r1, .L_080ca3f0
	mov r12, r0
	adds r3, r1, #4
	mov lr, r3
	movs r3, #8
	adds r3, r3, r1
	mov r8, r3
.L_080ca3a0:
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, r7
	bne .L_080ca3d6
	movs r0, #2
	ldrsh r3, [r5, r0]
	asrs r2, r4, #16
	cmp r3, r2
	beq .L_080ca3ba
	cmp r6, r2
	beq .L_080ca3ba
	cmp r3, r6
	bne .L_080ca3d6
.L_080ca3ba:
	movs r2, #8
	ldrsh r3, [r5, r2]
	lsls r3, r3, #16
	str r3, [r1]
	movs r0, #10
	ldrsh r3, [r5, r0]
	mov r1, lr
	lsls r3, r3, #16
	str r3, [r1]
	movs r2, #12
	ldrsh r3, [r5, r2]
	mov r0, r8
	str r3, [r0]
	b .L_080ca3e2
.L_080ca3d6:
	adds r5, #16
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldrh r2, [r5]
	cmp r3, r12
	bne .L_080ca3a0
.L_080ca3e2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080ca3e8:
	.4byte Data_080ef824
.L_080ca3ec:
	.4byte 0xffff0000
.L_080ca3f0:
	.4byte Data_020004b8
