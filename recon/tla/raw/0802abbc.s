.syntax unified
	.thumb
	.global Func_0802abbc
	.thumb_func
Func_0802abbc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0802ac80
	adds r6, r1, #0
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	adds r1, r1, r3
	movs r3, #132
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r3, .L_0802ac84
	lsls r0, r0, #11
	adds r3, r3, r0
	mov r10, r3
	lsrs r3, r2, #31
	adds r3, r2, r3
	movs r0, #127
	asrs r3, r3, #1
	ands r3, r0
	mov lr, r0
	movs r0, #30
	ands r2, r0
	lsls r2, r2, #5
	ldrh r4, [r1, #42]
	mov r8, r2
	movs r2, #254
	lsls r2, r2, #6
	ldrh r5, [r1, #46]
	lsls r7, r3, #7
	cmp r4, r2
	beq .L_0802ac16
	ldr r2, .L_0802ac80
	lsls r3, r5, #2
	subs r7, r7, r5
	adds r2, r2, r3
	ands r7, r4
	mov r12, r2
.L_0802ac16:
	lsrs r3, r6, #31
	ldrh r5, [r1, #40]
	adds r3, r6, r3
	asrs r4, r3, #1
	mov r3, lr
	ands r4, r3
	ldrh r1, [r1, #44]
	ands r0, r6
	ands r4, r5
	cmp r5, #127
	beq .L_0802ac34
	subs r4, r4, r1
	lsls r3, r1, #2
	ands r4, r5
	add r12, r3
.L_0802ac34:
	movs r2, #30
	movs r6, #0
	mov lr, r2
.L_0802ac3a:
	adds r3, r7, r4
	lsls r3, r3, #2
	mov r2, r12
	ldr r1, [r3, r2]
	ldr r2, .L_0802ac88
	lsls r1, r1, #21
	lsrs r1, r1, #18
	adds r3, r1, r2
	mov r2, r8
	adds r2, r2, r0
	lsls r2, r2, #1
	ldr r3, [r3]
	mov r9, r2
	add r9, r10
	mov r2, r9
	str r3, [r2]
	ldr r2, .L_0802ac8c
	adds r4, #1
	adds r3, r1, r2
	ldr r3, [r3]
	mov r2, r9
	str r3, [r2, #64]
	adds r0, #2
	mov r3, lr
	adds r6, #1
	ands r4, r5
	ands r0, r3
	cmp r6, #15
	bls .L_0802ac3a
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802ac80:
	.4byte gMapCellBuffer
.L_0802ac84:
	.4byte 0x06002800
.L_0802ac88:
	.4byte gMapBlocks
.L_0802ac8c:
	.4byte Data_02020004
