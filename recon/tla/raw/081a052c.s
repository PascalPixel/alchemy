.syntax unified
	.thumb
	.global Func_081a052c
	.thumb_func
Func_081a052c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r1, #0
	cmp r0, #0
	bne .L_081a0544
	adds r0, r5, #0
	bl Func_081a04d0
	b .L_081a065c
.L_081a0544:
	bl Resource_GetTableEntry
	adds r7, r0, #0
	cmp r5, #0
	bne .L_081a0560
	movs r2, #0
	mov r10, r2
	movs r3, #192
	movs r2, #160
	lsls r3, r3, #19
	lsls r2, r2, #19
	mov r8, r3
	mov r9, r2
	b .L_081a056c
.L_081a0560:
	ldr r3, .L_081a05e0
	ldr r2, .L_081a05e4
	mov r10, r3
	ldr r3, .L_081a05e8
	mov r8, r2
	mov r9, r3
.L_081a056c:
	ldr r5, .L_081a05ec
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081a05f0
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #1
	adds r0, r7, r2
	mov r1, r8
	mov r2, r10
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	ldr r3, .L_081a05dc
	ldr r2, .L_081a05f4
	mov lr, r3
	mov r12, r2
	movs r6, #127
.L_081a05aa:
	movs r3, #0
	ldrsh r2, [r7, r3]
	mov r3, lr
	lsls r2, r2, #16
	lsrs r0, r2, #26
	lsrs r1, r2, #21
	ands r0, r3
	ands r1, r3
	movs r3, #248
	lsls r3, r3, #13
	ands r3, r2
	lsrs r4, r3, #16
	lsls r3, r1, #1
	adds r3, r3, r4
	lsls r3, r3, #1
	adds r5, r3, r0
	lsls r3, r0, #2
	adds r3, r3, r0
	adds r2, r5, r3
	adds r7, #2
	cmp r2, #0
	bge .L_081a05f8
	adds r2, #15
	b .L_081a05f8
	.2byte 0x0000
.L_081a05dc:
	.4byte 0x0000001f
.L_081a05e0:
	.4byte 0x80808080
.L_081a05e4:
	.4byte 0x06008000
.L_081a05e8:
	.4byte 0x05000100
.L_081a05ec:
	.4byte 0x00000234
.L_081a05f0:
	.4byte SentouKouka_TenkaiCode
.L_081a05f4:
	.4byte Flash_Handler3
.L_081a05f8:
	lsls r3, r1, #2
	adds r3, r3, r1
	asrs r0, r2, #4
	adds r2, r5, r3
	cmp r2, #0
	bge .L_081a0606
	adds r2, #15
.L_081a0606:
	lsls r3, r4, #2
	adds r3, r3, r4
	adds r3, r5, r3
	asrs r1, r2, #4
	cmp r3, #0
	bge .L_081a0614
	adds r3, #15
.L_081a0614:
	asrs r4, r3, #4
	lsls r2, r1, #5
	lsls r3, r0, #10
	orrs r3, r2
	orrs r3, r4
	mov r2, r12
	strh r3, [r2]
	subs r6, #1
	movs r3, #2
	add r12, r3
	cmp r6, #0
	bge .L_081a05aa
	ldr r1, .L_081a0668
	ldr r0, .L_081a066c
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_081a065a
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	ldr r2, .L_081a0670
	adds r3, r3, r1
	adds r3, #4
	stmia r3!, {r2}
	mov r2, r9
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #64
	str r2, [r3]
.L_081a065a:
	strh r4, [r0]
.L_081a065c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a0668:
	.4byte Data_020038e0
.L_081a066c:
	.4byte 0x04000208
.L_081a0670:
	.4byte Flash_Handler3
