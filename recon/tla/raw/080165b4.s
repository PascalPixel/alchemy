.syntax unified
	.thumb
	.global Func_080165b4
	.thumb_func
Func_080165b4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	movs r3, #0
	adds r4, r0, #0
	str r3, [sp, #4]
	ldr r2, .L_08016688
	strh r3, [r2]
	ldr r3, .L_0801668c
	movs r7, #3
	adds r1, r3, #0
	adds r1, #64
.L_080165d2:
	ldr r2, [r1, #16]
	ldr r3, [r1]
	subs r7, #1
	str r3, [r1, #16]
	stmia r1!, {r2}
	cmp r7, #0
	bge .L_080165d2
	ldr r1, .L_08016690
	movs r0, #0
	ldr r3, [r1]
	mov lr, sp
	str r3, [sp, #0]
	str r0, [r1]
	movs r2, #1
	ldr r3, .L_08016688
	strh r2, [r3]
	subs r3, r1, #4
	strb r0, [r3, #3]
	mov r9, r3
	subs r2, #2
	adds r6, r1, #0
	movs r7, #0
	mov r10, r9
	mov r8, r2
	adds r6, #76
	mov r12, r4
.L_08016606:
	ldr r2, [r6]
	movs r0, #0
	movs r1, #0
.L_0801660c:
	ldrh r3, [r2]
	adds r1, #1
	adds r2, #2
	adds r0, r0, r3
	cmp r1, #13
	bls .L_0801660c
	mov r3, lr
	ldrb r4, [r3, r7]
	cmp r4, #1
	bne .L_0801664e
	lsls r5, r0, #16
	asrs r3, r5, #16
	cmp r3, r8
	bne .L_08016662
	ldr r0, [r6]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, #4
	mov r1, r12
	adds r2, #6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r1, r10
	ldrb r3, [r1, #3]
	ldr r2, .L_0801668c
	lsls r4, r7
	orrs r4, r3
	strb r4, [r1, #3]
	mov r9, r2
	b .L_08016650
.L_0801664e:
	lsls r5, r0, #16
.L_08016650:
	asrs r3, r5, #16
	cmp r3, r8
	bne .L_08016662
	ldr r2, [r6]
	ldrh r3, [r2, #2]
	mvns r3, r3
	strh r3, [r2, #2]
	ldr r3, .L_0801668c
	mov r9, r3
.L_08016662:
	movs r1, #24
	adds r7, #1
	adds r6, #4
	add r12, r1
	cmp r7, #1
	ble .L_08016606
	mov r2, r9
	ldrb r3, [r2, #2]
	ldrb r2, [r2, #3]
	mov r1, r9
	ldrb r0, [r1, #3]
	orrs r3, r2
	strb r3, [r1, #2]
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08016688:
	.4byte 0x04000208
.L_0801668c:
	.4byte Data_02005360
.L_08016690:
	.4byte Data_02005364
