.syntax unified
	.thumb
	.global Func_0802a6b8
	.thumb_func
Func_0802a6b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #193
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	adds r5, r0, #0
	strh r3, [r1]
	movs r0, #0
	sub sp, #8
	bl Func_08013eb4
	ldr r2, .L_0802a8f8
	lsls r3, r5, #1
	adds r3, r3, r5
	movs r5, #216
	lsls r5, r5, #1
	lsls r3, r3, #2
	adds r3, r3, r2
	adds r1, r5, #0
	movs r0, #32
	str r3, [sp, #0]
	bl Runtime_AllocateBlock
	adds r1, r5, #0
	ldr r3, .L_0802a8fc
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #0]
	ldr r3, .L_0802a900
	ldrh r0, [r2]
	adds r0, r0, r3
	bl Resource_GetTableEntry
	adds r7, r0, #0
	ldr r3, [r7, #36]
	ldr r1, .L_0802a904
	adds r0, r7, r3
	bl Func_0801587c
	bl Tilemap_DecodeStagedBuffer
	movs r3, #1
	add r0, sp, #4
	negs r3, r3
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_0802a908
	ldr r2, .L_0802a90c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r7, #40]
	adds r0, r7, r3
	bl Func_0801587c
	ldr r3, [r7, #44]
	ldr r1, .L_0802a910
	adds r0, r7, r3
	bl Func_0801587c
	bl Func_0802a5e4
	ldr r3, [r7, #48]
	ldr r1, .L_0802a914
	adds r0, r7, r3
	bl Func_0801587c
	ldr r0, [r7, #52]
	cmp r0, #0
	beq .L_0802a76a
	ldr r5, .L_0802a918
	adds r0, r7, r0
	adds r1, r5, #0
	bl Func_0801587c
	adds r0, r5, #0
	bl Func_0802cc9c
.L_0802a76a:
	ldr r0, [r7, #56]
	cmp r0, #0
	beq .L_0802a780
	ldr r5, .L_0802a91c
	adds r0, r7, r0
	adds r1, r5, #0
	bl Func_0801587c
	adds r0, r5, #0
	bl Func_0802ce4c
.L_0802a780:
	ldr r3, [r7, #60]
	ldr r1, .L_0802a920
	adds r0, r7, r3
	bl Func_0801587c
	ldrb r3, [r7]
	mov r2, r8
	adds r2, #236
	lsls r3, r3, #19
	str r3, [r2]
	adds r2, #4
	ldrb r3, [r7, #1]
	mov r1, r8
	lsls r3, r3, #19
	str r3, [r2]
	adds r1, #244
	ldrb r3, [r7, #2]
	mov r0, r8
	lsls r3, r3, #19
	str r3, [r1]
	adds r0, #248
	ldrb r3, [r7, #3]
	lsls r3, r3, #19
	str r3, [r0]
	movs r3, #228
	add r3, r8
	mov r11, r3
	mov r2, r11
	movs r3, #0
	str r3, [r2]
	movs r2, #232
	add r2, r8
	str r3, [r2]
	mov r9, r2
	movs r3, #130
	ldrb r2, [r7, #4]
	lsls r3, r3, #1
	add r3, r8
	strb r2, [r3]
	ldrb r3, [r7, #5]
	movs r2, #6
	adds r2, #255
	add r2, r8
	strb r3, [r2]
	ldrb r3, [r7, #6]
	movs r2, #131
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r2]
	movs r2, #2
	adds r2, #255
	movs r3, #2
	add r2, r8
	strb r3, [r2]
	ldr r3, [r1]
	cmp r3, #0
	bne .L_0802a7f8
	movs r3, #128
	lsls r3, r3, #20
	str r3, [r1]
.L_0802a7f8:
	ldr r3, [r0]
	cmp r3, #0
	bne .L_0802a804
	movs r3, #128
	lsls r3, r3, #20
	str r3, [r0]
.L_0802a804:
	movs r5, #132
	lsls r5, r5, #1
	adds r6, r7, #0
	movs r3, #2
	add r5, r8
	adds r6, #12
	mov r10, r3
.L_0802a812:
	ldrb r4, [r6]
	ldrb r0, [r6, #1]
	lsls r3, r4, #19
	str r3, [r5, #8]
	lsls r3, r0, #19
	str r3, [r5, #12]
	movs r3, #3
	ldrsb r3, [r6, r3]
	ldrb r2, [r6, #6]
	lsls r3, r3, #12
	str r3, [r5, #20]
	movs r3, #4
	ldrsb r3, [r6, r3]
	lsrs r0, r0, #1
	lsls r3, r3, #12
	str r3, [r5, #24]
	movs r3, #5
	ldrsb r3, [r6, r3]
	lsrs r4, r4, #1
	lsls r3, r3, #12
	str r3, [r5, #28]
	movs r3, #127
	ands r3, r2
	ldrb r2, [r6, #7]
	strh r3, [r5, #40]
	movs r3, #127
	ands r3, r2
	movs r2, #0
	str r2, [r5, #32]
	str r2, [r5, #36]
	lsls r0, r0, #7
	ldr r2, .L_0802a910
	strh r0, [r5, #46]
	movs r1, #2
	ldrsb r1, [r6, r1]
	adds r0, r0, r4
	lsls r3, r3, #7
	strh r3, [r5, #42]
	lsls r3, r0, #2
	adds r3, r3, r2
	lsls r1, r1, #12
	str r3, [r5, #48]
	ldr r3, .L_0802a914
	str r1, [r5, #16]
	strh r4, [r5, #44]
	adds r0, r0, r3
	mov r2, r11
	str r0, [r5, #52]
	ldr r3, .L_0802a924
	ldr r0, [r2]
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r5, #8]
	mov r2, r9
	adds r0, r0, r3
	str r0, [r5]
	ldr r1, [r5, #20]
	ldr r0, [r2]
	ldr r3, .L_0802a924
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r5, #12]
	movs r2, #1
	negs r2, r2
	add r10, r2
	adds r0, r0, r3
	mov r3, r10
	str r0, [r5, #4]
	adds r6, #8
	adds r5, #56
	cmp r3, #0
	bge .L_0802a812
	movs r3, #128
	lsls r3, r3, #5
	mov r2, r8
	strh r3, [r2, #20]
	movs r1, #130
	lsls r1, r1, #1
	add r1, r8
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_0802a8bc
	movs r3, #192
	lsls r3, r3, #5
	strh r3, [r2, #20]
.L_0802a8bc:
	movs r0, #6
	adds r0, #255
	add r0, r8
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_0802a8d4
	mov r2, r8
	ldrh r3, [r2, #20]
	ldr r2, .L_0802a8f0
	orrs r3, r2
	mov r2, r8
	strh r3, [r2, #20]
.L_0802a8d4:
	movs r3, #131
	lsls r3, r3, #1
	add r3, r8
	mov r12, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802a928
	mov r2, r8
	ldrh r3, [r2, #20]
	ldr r2, .L_0802a8f4
	orrs r3, r2
	mov r2, r8
	strh r3, [r2, #20]
	b .L_0802a928
.L_0802a8f0:
	.4byte 0x00000400
.L_0802a8f4:
	.4byte 0x00000200
.L_0802a8f8:
	.4byte Data_0802f380
.L_0802a8fc:
	.4byte IwramClearWords
.L_0802a900:
	.4byte 0x0000026c
.L_0802a904:
	.4byte Data_02010001
.L_0802a908:
	.4byte Data_0202c000
.L_0802a90c:
	.4byte 0x85000800
.L_0802a910:
	.4byte gMapCellBuffer
.L_0802a914:
	.4byte Data_02024000
.L_0802a918:
	.4byte Data_0202d000
.L_0802a91c:
	.4byte Data_0202de00
.L_0802a920:
	.4byte Data_0202e000
.L_0802a924:
	.4byte IwramMulQ16
.L_0802a928:
	ldrb r3, [r7, #7]
	ldrb r2, [r1]
	lsls r3, r3, #2
	orrs r2, r3
	movs r3, #160
	lsls r3, r3, #3
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #14
	strh r2, [r3]
	ldrb r2, [r0]
	ldrb r3, [r7, #8]
	lsls r3, r3, #2
	orrs r2, r3
	movs r3, #192
	lsls r3, r3, #3
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #12
	strh r2, [r3]
	mov r3, r12
	ldrb r2, [r3]
	ldrb r3, [r7, #9]
	lsls r3, r3, #2
	orrs r2, r3
	movs r3, #224
	lsls r3, r3, #3
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #10
	strh r2, [r3]
	movs r5, #184
	lsls r5, r5, #1
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0802a982
	adds r0, r5, #0
	bl GameFlag_ClearBit
	b .L_0802aa24
.L_0802a982:
	movs r2, #128
	lsls r2, r2, #7
	mov r10, r2
	mov r0, r10
	bl Runtime_BumpAllocate
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0802aa24
	movs r3, #160
	lsls r3, r3, #19
	movs r2, #0
	ldrsh r5, [r3, r2]
	mov r8, r3
	ldr r3, [sp, #0]
	ldr r6, .L_0802aa58
	ldrh r0, [r3, #2]
	adds r0, r0, r6
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_0801587c
	movs r2, #224
	adds r1, r7, #0
	strh r5, [r7]
	lsls r2, r2, #1
	ldr r5, .L_0802aa5c
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	ldr r2, [sp, #0]
	ldrh r0, [r2, #4]
	adds r0, r0, r6
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_080158cc
	mov r2, r10
	adds r1, r7, #0
	ldr r0, .L_0802aa60
	mov lr, r5
	.2byte 0xf800
	ldr r3, [sp, #0]
	ldrh r0, [r3, #6]
	adds r0, r0, r6
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_080158cc
	adds r1, r7, #0
	mov r2, r10
	ldr r0, .L_0802aa64
	mov lr, r5
	.2byte 0xf800
	ldr r2, [sp, #0]
	ldrh r0, [r2, #8]
	adds r0, r0, r6
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_080158cc
	adds r1, r7, #0
	mov r2, r10
	ldr r0, .L_0802aa68
	mov lr, r5
	.2byte 0xf800
	ldr r3, [sp, #0]
	ldrh r0, [r3, #10]
	adds r0, r0, r6
	bl Resource_GetTableEntry
	ldr r1, .L_0802aa6c
	bl Func_080158cc
	adds r0, r7, #0
	bl Sys_Free
.L_0802aa24:
	movs r3, #128
	lsls r3, r3, #19
	movs r2, #0
	adds r3, #76
	strh r2, [r3]
	adds r3, #4
	strh r2, [r3]
	movs r2, #160
	lsls r2, r2, #1
	subs r3, #80
	strh r2, [r3]
	ldr r0, .L_0802aa70
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #133
	bl Func_080145a8
	movs r0, #2
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802aa58:
	.4byte 0x0000026c
.L_0802aa5c:
	.4byte IwramCopyWords
.L_0802aa60:
	.4byte 0x06004000
.L_0802aa64:
	.4byte 0x06008000
.L_0802aa68:
	.4byte 0x0600c000
.L_0802aa6c:
	.4byte Data_02028000
.L_0802aa70:
	.4byte Func_0802ad84
