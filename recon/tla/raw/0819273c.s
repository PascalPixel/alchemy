.syntax unified
	.thumb
	.global Func_0819273c
	.thumb_func
Func_0819273c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	sub sp, #52
	mov r10, r0
	movs r0, #48
	str r3, [sp, #4]
	mov r8, r1
	mov r11, r2
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r3, .L_08192814
	add r5, sp, #28
	adds r2, r5, #0
	mov r9, r0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldr r2, .L_08192818
	ldr r3, [sp, #8]
	movs r0, #4
	ands r3, r2
	ldr r2, .L_0819281c
	orrs r3, r0
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #3
	ldr r1, [sp, #4]
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #4
	mov r4, r10
	adds r2, #2
	add r6, sp, #8
	str r3, [sp, #8]
	mov r0, r10
	negs r3, r4
	adds r2, r1, r2
	mov r1, r11
	str r2, [r6, #4]
	strb r3, [r5]
	strb r4, [r5, #4]
	strb r3, [r5, #8]
	strb r0, [r5, #12]
	strb r3, [r5, #16]
	strb r0, [r5, #20]
	strb r1, [r5, #21]
	strb r1, [r5, #17]
	bl Func_08014de4
	mov r2, r8
	ldr r3, .L_08192820
	lsls r2, r2, #16
	mov r8, r2
	mov r1, r11
	add r8, r3
	movs r3, #63
	subs r3, r3, r1
	add r0, sp, #16
	mov r4, r8
	lsls r3, r3, #16
	movs r2, #0
	str r4, [r0]
	str r3, [r0, #4]
	str r2, [r0, #8]
	bl Func_08015128
	mov r4, r9
	movs r3, #4
	str r3, [r4]
	ldr r3, .L_08192824
	str r6, [r4, #16]
	str r3, [r4, #8]
	str r7, [r4, #12]
	adds r1, r7, #0
	movs r2, #6
	adds r0, r5, #0
	bl Func_08196958
	mov r0, r9
	bl Func_08196a7c
	mov r0, r9
	bl Sys_Free
	adds r0, r7, #0
	bl Sys_Free
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08192814:
	.4byte Data_08196f30
.L_08192818:
	.4byte 0xffffff00
.L_0819281c:
	.4byte 0xffff00ff
.L_08192820:
	.4byte 0xffc10000
.L_08192824:
	.4byte Data_08199f4c
