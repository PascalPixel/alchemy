.syntax unified
	.thumb
	.global Func_0811843c
	.thumb_func
Func_0811843c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, .L_081184c8
	ldr r1, .L_081184cc
	ldr r6, .L_081184d0
	movs r2, #148
	sub sp, #4
	mov r8, r0
	movs r7, #0
	mov r9, r1
	movs r4, #0
	mov r10, r2
.L_0811845a:
	mov r3, r9
	ldrb r0, [r3, r7]
	str r4, [sp, #0]
	bl Owner_GetState
	movs r3, #0
	adds r5, r0, #0
	mov r0, r8
	strb r3, [r0, r7]
	mov r1, r9
	ldrb r0, [r1, r7]
	bl Func_08123534
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_0811847c
	b .L_081185a6
.L_0811847c:
	movs r3, #1
	mov r2, r8
	strb r3, [r2, r7]
	cmp r7, #2
	ble .L_081184a4
	movs r0, #56
	ldrsh r2, [r5, r0]
	mov r1, r10
	ldrh r3, [r5, #56]
	cmp r2, #0
	bgt .L_08118494
	ldr r3, .L_081184c4
.L_08118494:
	mov r2, r8
	ldr r0, .L_081184d4
	strh r3, [r2, r1]
	ldrh r2, [r5, #58]
	adds r3, r4, r0
	strh r2, [r3, #2]
	cmp r7, #2
	bgt .L_081184d8
.L_081184a4:
	mov r1, r8
	adds r3, r4, r1
	adds r1, r3, #0
	movs r3, #150
	lsls r3, r3, #1
	adds r1, #8
	adds r2, r5, r3
	movs r0, #3
.L_081184b4:
	ldrb r3, [r2]
	subs r0, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r0, #0
	bge .L_081184b4
	b .L_081184d8
.L_081184c4:
	.4byte 0x00000001
.L_081184c8:
	.4byte Data_0200ff58
.L_081184cc:
	.4byte Data_0812a16c
.L_081184d0:
	.4byte Data_0200ff6c
.L_081184d4:
	.4byte Data_0200ffec
.L_081184d8:
	movs r0, #152
	lsls r0, r0, #1
	adds r3, r5, r0
	ldrb r3, [r3]
	movs r1, #50
	adds r1, #255
	strb r3, [r6]
	adds r3, r5, r1
	ldrb r3, [r3]
	movs r2, #153
	lsls r2, r2, #1
	strb r3, [r6, #1]
	adds r3, r5, r2
	ldrb r3, [r3]
	adds r0, #3
	strb r3, [r6, #2]
	adds r3, r5, r0
	ldrb r3, [r3]
	adds r1, #3
	strb r3, [r6, #3]
	adds r3, r5, r1
	ldrb r3, [r3]
	adds r2, #3
	strb r3, [r6, #4]
	adds r3, r5, r2
	ldrb r3, [r3]
	adds r0, #3
	strb r3, [r6, #5]
	adds r3, r5, r0
	ldrb r3, [r3]
	adds r1, #3
	strb r3, [r6, #6]
	adds r3, r5, r1
	ldrb r3, [r3]
	adds r2, #3
	strb r3, [r6, #7]
	adds r3, r5, r2
	ldrb r3, [r3]
	adds r0, #3
	strb r3, [r6, #8]
	adds r3, r5, r0
	ldrb r3, [r3]
	adds r1, #3
	strb r3, [r6, #9]
	adds r3, r5, r1
	ldrb r3, [r3]
	adds r2, #3
	strb r3, [r6, #10]
	adds r3, r5, r2
	ldrb r3, [r3]
	adds r0, #3
	strb r3, [r6, #11]
	adds r3, r5, r0
	ldrb r3, [r3]
	adds r1, #3
	strb r3, [r6, #12]
	adds r3, r5, r1
	ldrb r3, [r3]
	adds r2, #3
	strb r3, [r6, #13]
	adds r3, r5, r2
	ldrb r3, [r3]
	adds r0, #3
	strb r3, [r6, #14]
	adds r3, r5, r0
	ldrb r3, [r3]
	adds r1, #3
	strb r3, [r6, #15]
	adds r3, r5, r1
	ldrb r3, [r3]
	adds r2, #3
	strb r3, [r6, #16]
	adds r3, r5, r2
	ldrb r3, [r3]
	adds r0, #3
	strb r3, [r6, #17]
	adds r3, r5, r0
	ldrb r3, [r3]
	adds r1, #3
	strb r3, [r6, #18]
	adds r3, r5, r1
	ldrb r3, [r3]
	adds r2, #3
	strb r3, [r6, #19]
	adds r3, r5, r2
	ldrb r3, [r3]
	adds r0, #3
	strb r3, [r6, #20]
	adds r3, r5, r0
	ldrb r3, [r3]
	adds r1, #3
	strb r3, [r6, #21]
	adds r3, r5, r1
	ldrb r3, [r3]
	adds r2, #3
	strb r3, [r6, #22]
	adds r3, r5, r2
	ldrb r3, [r3]
	adds r0, #3
	strb r3, [r6, #23]
	adds r3, r5, r0
	ldrb r3, [r3]
	strb r3, [r6, #24]
.L_081185a6:
	movs r1, #4
	adds r7, #1
	adds r4, #4
	add r10, r1
	adds r6, #28
	cmp r7, #4
	bgt .L_081185b6
	b .L_0811845a
.L_081185b6:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
