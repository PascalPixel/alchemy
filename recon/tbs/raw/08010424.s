.syntax unified
	.thumb
	.global Map_CopyMetatileIndicesRect
	.thumb_func
Map_CopyMetatileIndicesRect:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r4, r3, #0
	mov r11, r2
	lsls r1, r1, #7
	lsls r3, r4, #7
	ldr r2, .L_08010540
	adds r1, r1, r0
	add r3, r11
	lsls r1, r1, #2
	lsls r3, r3, #2
	sub sp, #36
	adds r3, r3, r2
	adds r1, r1, r2
	str r3, [sp, #4]
	str r1, [sp, #8]
	ldr r3, .L_08010544
	movs r0, #130
	ldr r3, [r3]
	lsls r0, r0, #1
	adds r2, r3, r0
	add r0, sp, #12
	mov r9, r0
	movs r6, #2
.L_0801045e:
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
	bge .L_0801045e
	ldr r3, [sp, #72]
	adds r7, r4, #0
	adds r3, r7, r3
	cmp r7, r3
	bcs .L_0801052c
	ldr r1, [sp, #68]
	str r3, [sp, #0]
	movs r3, #128
	subs r3, r3, r1
	lsls r3, r3, #2
	mov r8, r3
.L_0801048a:
	ldr r2, [sp, #68]
	mov r1, r11
	adds r3, r1, r2
	cmp r1, r3
	bcs .L_08010518
	mov r12, r7
	mov r4, r12
	mov lr, r3
	movs r3, #15
	ands r4, r3
	mov r10, r3
	mov r12, r4
.L_080104a2:
	ldr r2, [sp, #8]
	ldmia r2!, {r5}
	adds r0, r2, #0
	ldr r4, [sp, #4]
	str r0, [sp, #8]
	ldr r3, .L_08010548
	ldr r2, .L_0801054c
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
.L_080104d0:
	ldr r3, [r0]
	cmp r3, r1
	bgt .L_08010504
	adds r3, #16
	cmp r3, r1
	ble .L_08010504
	ldr r3, [r0, #4]
	cmp r3, r7
	bgt .L_08010504
	adds r3, #12
	cmp r3, r7
	ble .L_08010504
	lsls r3, r5, #3
	ldr r2, .L_08010550
	ldr r5, .L_08010554
	adds r0, r4, r2
	adds r2, r3, r5
	ldr r2, [r2]
	str r2, [r0]
	ldr r0, .L_08010558
	adds r2, r3, r0
	ldr r3, .L_0801055c
	adds r0, r4, r3
	ldr r3, [r2]
	str r3, [r0]
	b .L_08010512
.L_08010504:
	movs r2, #128
	lsls r2, r2, #4
	adds r6, #1
	adds r4, r4, r2
	adds r0, #8
	cmp r6, #2
	ble .L_080104d0
.L_08010512:
	adds r1, #1
	cmp r1, lr
	bcc .L_080104a2
.L_08010518:
	ldr r3, [sp, #8]
	ldr r4, [sp, #4]
	ldr r5, [sp, #0]
	add r3, r8
	add r4, r8
	adds r7, #1
	str r3, [sp, #8]
	str r4, [sp, #4]
	cmp r7, r5
	bcc .L_0801048a
.L_0801052c:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_08010540:
	.4byte gMapCellBuffer
.L_08010544:
	.4byte gMapWork
.L_08010548:
	.4byte 0x00000fff
.L_0801054c:
	.4byte 0xfffff000
.L_08010550:
	.4byte 0x06002800
.L_08010554:
	.4byte gMapBlocks
.L_08010558:
	.4byte gMapBlocks + 0x4
.L_0801055c:
	.4byte 0x06002840
