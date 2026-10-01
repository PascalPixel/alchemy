.syntax unified
	.thumb
	.global Func_08010788
	.thumb_func
Func_08010788:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	ldr r5, [sp, #72]
	lsls r1, r1, #7
	adds r1, r1, r0
	ldr r0, [sp, #68]
	adds r4, r3, #0
	lsls r3, r5, #7
	adds r3, r3, r0
	mov r11, r2
	ldr r2, .L_080108a4
	lsls r1, r1, #2
	lsls r3, r3, #2
	adds r1, r1, r2
	adds r3, r3, r2
	str r1, [sp, #8]
	str r3, [sp, #4]
	ldr r3, .L_080108a8
	movs r1, #130
	ldr r3, [r3]
	lsls r1, r1, #1
	add r0, sp, #12
	adds r2, r3, r1
	mov r9, r0
	movs r6, #2
.L_080107c6:
	ldr r3, [r2]
	asrs r3, r3, #20
	str r3, [r0]
	ldr r3, [r2, #4]
	subs r6, #1
	asrs r3, r3, #20
	str r3, [r0, #4]
	adds r2, #48
	adds r0, #8
	cmp r6, #0
	bge .L_080107c6
	adds r7, r5, #0
	adds r3, r7, r4
	cmp r7, r3
	bge .L_08010892
	str r3, [sp, #0]
	mov r2, r11
	movs r3, #128
	subs r3, r3, r2
	lsls r3, r3, #2
	mov r8, r3
.L_080107f0:
	ldr r1, [sp, #68]
	mov r4, r11
	adds r3, r1, r4
	cmp r1, r3
	bge .L_0801087e
	mov r12, r7
	movs r5, #15
	mov r0, r12
	ands r0, r5
	mov lr, r3
	mov r10, r5
	mov r12, r0
.L_08010808:
	ldr r3, [sp, #8]
	ldmia r3!, {r5}
	adds r2, r3, #0
	ldr r4, [sp, #4]
	str r2, [sp, #8]
	ldr r3, .L_080108ac
	ldr r2, .L_080108b0
	ands r5, r3
	ldr r3, [r4]
	ands r3, r2
	orrs r3, r5
	stmia r4!, {r3}
	adds r2, r1, #0
	adds r0, r4, #0
	mov r3, r10
	mov r4, r12
	ands r2, r3
	lsls r3, r4, #5
	adds r3, r3, r2
	str r0, [sp, #4]
	movs r6, #0
	mov r0, r9
	lsls r4, r3, #2
.L_08010836:
	ldr r3, [r0]
	cmp r3, r1
	bgt .L_0801086a
	adds r3, #16
	cmp r3, r1
	ble .L_0801086a
	ldr r3, [r0, #4]
	cmp r3, r7
	bgt .L_0801086a
	adds r3, #12
	cmp r3, r7
	ble .L_0801086a
	lsls r3, r5, #3
	ldr r2, .L_080108b4
	ldr r5, .L_080108b8
	adds r0, r4, r2
	adds r2, r3, r5
	ldr r2, [r2]
	str r2, [r0]
	ldr r0, .L_080108bc
	adds r2, r3, r0
	ldr r3, .L_080108c0
	adds r0, r4, r3
	ldr r3, [r2]
	str r3, [r0]
	b .L_08010878
.L_0801086a:
	movs r2, #128
	lsls r2, r2, #4
	adds r6, #1
	adds r4, r4, r2
	adds r0, #8
	cmp r6, #2
	ble .L_08010836
.L_08010878:
	adds r1, #1
	cmp r1, lr
	blt .L_08010808
.L_0801087e:
	ldr r3, [sp, #8]
	ldr r4, [sp, #4]
	ldr r5, [sp, #0]
	add r3, r8
	add r4, r8
	adds r7, #1
	str r3, [sp, #8]
	str r4, [sp, #4]
	cmp r7, r5
	blt .L_080107f0
.L_08010892:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080108a4:
	.4byte gMapCellBuffer
.L_080108a8:
	.4byte gMapWork
.L_080108ac:
	.4byte 0x00000fff
.L_080108b0:
	.4byte 0xfffff000
.L_080108b4:
	.4byte 0x06002800
.L_080108b8:
	.4byte gMapBlocks
.L_080108bc:
	.4byte gMapBlocks + 0x4
.L_080108c0:
	.4byte 0x06002840
