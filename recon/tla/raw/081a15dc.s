.syntax unified
	.thumb
	.global Func_081a15dc
	.thumb_func
Func_081a15dc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r7, r1, #0
	adds r4, r0, #0
	movs r1, #0
	mov r10, r2
	str r1, [sp, #0]
	cmp r4, #0
	bne .L_081a1648
	ldr r3, .L_081a1788
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_081a1610
	movs r1, #192
	movs r2, #160
	lsls r1, r1, #19
	lsls r2, r2, #19
	ldr r3, .L_081a178c
	b .L_081a1616
.L_081a1610:
	ldr r1, .L_081a1790
	ldr r2, .L_081a1794
	ldr r3, .L_081a1798
.L_081a1616:
	mov r11, r1
	mov r8, r2
	add r4, sp, #4
	str r3, [r4]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	adds r1, r7, #0
	ldr r2, .L_081a179c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #0
	str r3, [r4]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	mov r1, r8
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_081a16e8
.L_081a1648:
	ldr r3, .L_081a1788
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_081a165c
	movs r2, #192
	movs r3, #160
	lsls r2, r2, #19
	lsls r3, r3, #19
	b .L_081a1664
.L_081a165c:
	ldr r1, .L_081a17a0
	ldr r2, .L_081a1790
	ldr r3, .L_081a1794
	str r1, [sp, #0]
.L_081a1664:
	mov r11, r2
	mov r8, r3
	add r0, sp, #4
	movs r3, #0
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	mov r1, r11
	ldr r2, .L_081a17a4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r4, #0
	bl Resource_GetTableEntry
	mov r9, r0
	ldr r5, .L_081a17a8
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081a17ac
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #1
	adds r1, r7, #0
	ldr r2, [sp, #0]
	add r0, r9
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	ldr r1, .L_081a17b0
	ldr r0, .L_081a17b4
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_081a16e6
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r2, #1
	adds r3, #4
	strh r2, [r1]
	mov r1, r9
	stmia r3!, {r1}
	mov r2, r8
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #64
	str r2, [r3]
.L_081a16e6:
	strh r4, [r0]
.L_081a16e8:
	ldr r3, .L_081a17b8
	ldr r2, .L_081a17bc
	str r7, [r3]
	ldr r3, .L_081a17c0
	mov r1, r10
	ldr r3, [r3]
	mov r12, r7
	lsrs r3, r3, #19
	lsls r3, r3, #6
	mov r9, r3
	ldr r3, .L_081a17c4
	mov r7, r11
	strh r1, [r3]
	mov r3, r10
	strh r3, [r2]
	ldr r2, .L_081a17c8
	mov r1, r10
	lsls r3, r1, #16
	str r3, [r2]
	movs r3, #128
	movs r2, #0
	lsls r3, r3, #5
	mov r8, r2
	mov r11, r3
.L_081a1718:
	mov r3, r10
	cmp r3, #0
	bge .L_081a1720
	adds r3, #7
.L_081a1720:
	movs r1, #224
	lsls r1, r1, #3
	adds r1, #255
	asrs r3, r3, #3
	mov r4, r9
	lsls r5, r3, #6
	ands r4, r1
	mov lr, r1
	movs r6, #30
.L_081a1732:
	mov r2, r12
	adds r0, r2, r5
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r7, r4
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r4, #64
	mov r3, lr
	subs r6, #1
	adds r5, #64
	ands r4, r3
	cmp r6, #0
	bge .L_081a1732
	movs r2, #1
	movs r1, #128
	add r8, r2
	lsls r1, r1, #4
	mov r3, r8
	add r12, r11
	adds r7, r7, r1
	cmp r3, #14
	ble .L_081a1718
	ldr r1, .L_081a1788
	add sp, #8
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r2, .L_081a17cc
	ldr r3, .L_081a1784
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081a1784:
	.4byte 0x00000000
.L_081a1788:
	.4byte Data_02007514
.L_081a178c:
	.4byte 0x01010101
.L_081a1790:
	.4byte 0x06008000
.L_081a1794:
	.4byte 0x05000100
.L_081a1798:
	.4byte 0x81818181
.L_081a179c:
	.4byte 0x85003c00
.L_081a17a0:
	.4byte 0x80808080
.L_081a17a4:
	.4byte 0x85000200
.L_081a17a8:
	.4byte 0x00000234
.L_081a17ac:
	.4byte SentouKouka_Tenkai2Code
.L_081a17b0:
	.4byte Data_020038e0
.L_081a17b4:
	.4byte 0x04000208
.L_081a17b8:
	.4byte Data_02007528
.L_081a17bc:
	.4byte gScrollTarget
.L_081a17c0:
	.4byte Data_02007518
.L_081a17c4:
	.4byte Data_02007520
.L_081a17c8:
	.4byte Data_0200751c
.L_081a17cc:
	.4byte Data_02007516
