.syntax unified
	.thumb
	.global Func_081a6ea0
	.thumb_func
Func_081a6ea0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #112
	movs r0, #172
	sub sp, #36
	bl Runtime_AllocateHeapBlock
	ldr r3, .L_081a6ef4
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r3, #0
	str r3, [sp, #0]
	movs r3, #128
	mov r11, sp
	lsls r3, r3, #19
	movs r1, #224
	mov r9, r0
	adds r3, #212
	mov r0, r11
	lsls r1, r1, #19
	ldr r2, .L_081a6ef8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081a6efc
	bl Resource_GetTableEntry
	ldr r5, .L_081a6f00
	adds r6, r0, #0
	adds r6, #32
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0801591c
	movs r3, #128
	b .L_081a6f04
	.2byte 0x0000
.L_081a6ef4:
	.4byte 0x00000000
.L_081a6ef8:
	.4byte 0x85000100
.L_081a6efc:
	.4byte 0x0000001a
.L_081a6f00:
	.4byte gMapCellBuffer
.L_081a6f04:
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_081a6f70
	ldr r2, .L_081a6f74
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_081a6f78
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r6, r0, #0
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_081a6f6c
	movs r2, #160
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #2
	adds r6, r6, r0
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0801591c
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	adds r0, r5, #0
	lsls r1, r1, #19
	ldr r2, .L_081a6f7c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl WaitFrames
	ldr r7, .L_081a6f80
	b .L_081a6f84
.L_081a6f6c:
	.4byte 0x00007fff
.L_081a6f70:
	.4byte 0x06010000
.L_081a6f74:
	.4byte 0x84001b00
.L_081a6f78:
	.4byte 0x00000019
.L_081a6f7c:
	.4byte 0x84002580
.L_081a6f80:
	.4byte 0x0600f800
.L_081a6f84:
	ldr r1, .L_081a6f90
	movs r2, #0
	mov r8, r2
	mov r10, r2
.L_081a6f8c:
	movs r6, #29
	b .L_081a6f94
.L_081a6f90:
	.4byte 0x000001ff
.L_081a6f94:
	mov r2, r8
	movs r0, #128
	lsls r3, r2, #16
	lsls r0, r0, #9
	adds r3, r3, r0
	asrs r3, r3, #16
	subs r6, #1
	strh r2, [r7]
	mov r8, r3
	adds r7, #2
	cmp r6, #0
	bge .L_081a6f94
	movs r2, #1
	add r10, r2
	strh r1, [r7]
	mov r3, r10
	adds r7, #2
	strh r1, [r7]
	adds r7, #2
	cmp r3, #19
	ble .L_081a6f8c
	ldr r3, .L_081a6fe0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	ldr r2, .L_081a6fe4
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r3, .L_081a6fe8
	movs r2, #0
	movs r6, #3
	b .L_081a6fec
	.2byte 0x0000
.L_081a6fe0:
	.4byte 0x00001f83
.L_081a6fe4:
	.4byte 0x00000000
.L_081a6fe8:
	.4byte Data_03001120
.L_081a6fec:
	subs r6, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r6, #0
	bge .L_081a6fec
	movs r2, #192
	lsls r2, r2, #18
	adds r2, #128
	ldr r1, [r2]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	movs r6, #4
	adds r3, #212
	lsls r0, r0, #19
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_081a81d4
	ldr r3, .L_081a7064
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #10
	bl Func_081a8228
	movs r3, #176
	add r5, sp, #4
	strh r3, [r5, #2]
	ldr r3, .L_081a7068
	movs r2, #0
	str r3, [r5, #8]
	ldr r3, .L_081a706c
	mov r0, r8
	str r3, [r5, #12]
	movs r3, #1
	strh r2, [r5]
	strh r0, [r5, #20]
	strh r2, [r5, #22]
	mov r0, r9
	mov r2, r10
	mov r8, r3
	str r3, [r5, #28]
	movs r3, #10
	strh r3, [r0, #10]
	strh r2, [r5, #24]
	movs r3, #25
	mov r2, r9
	strh r6, [r5, #4]
	b .L_081a7070
.L_081a7064:
	.4byte 0x00001240
.L_081a7068:
	.4byte Data_02019600
.L_081a706c:
	.4byte 0x06009600
.L_081a7070:
	str r7, [r5, #16]
	strh r3, [r2, #12]
	adds r0, r5, #0
	bl Func_081a68b4
	cmp r0, #0
	bne .L_081a715e
	movs r3, #200
	strh r3, [r5, #2]
	movs r3, #3
	strh r3, [r5, #4]
	adds r0, r5, #0
	bl Func_081a68b4
	cmp r0, #0
	bne .L_081a715e
	movs r3, #216
	strh r3, [r5, #2]
	movs r3, #2
	strh r3, [r5, #4]
	adds r0, r5, #0
	bl Func_081a68b4
	adds r7, r0, #0
	cmp r7, #0
	bne .L_081a715e
	ldr r0, .L_081a71ac
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_081a71b0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #251
	mov r10, r0
	mov r3, r9
	mov r2, r10
	strh r2, [r3, #2]
	strh r7, [r3]
	movs r3, #255
	lsls r3, r3, #8
	mov r0, r9
	adds r3, #253
	mov r2, r9
	strh r7, [r0, #4]
	strh r3, [r2, #6]
	mov r3, r9
	strh r7, [r3, #8]
	mov r0, r8
	movs r3, #224
	strh r0, [r5, #4]
	strh r3, [r5, #2]
	adds r0, r5, #0
	bl Func_081a68b4
	adds r6, r0, #0
	cmp r6, #0
	bne .L_081a715e
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_081a71b4
	bl Func_080145a8
	movs r3, #160
	strh r3, [r5, #2]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5, #4]
	ldr r3, .L_081a71b8
	strh r6, [r5, #22]
	str r3, [r5, #8]
	ldr r3, .L_081a71bc
	adds r0, r5, #0
	str r3, [r5, #12]
	ldr r3, .L_081a71c0
	str r3, [r5, #16]
	movs r3, #225
	lsls r3, r3, #1
	strh r3, [r5, #20]
	movs r3, #11
	strh r3, [r5, #24]
	movs r3, #8
	str r3, [r5, #28]
	bl Func_081a68b4
	adds r6, r0, #0
	cmp r6, #0
	bne .L_081a715e
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_081a81d4
	movs r0, #20
	bl Func_081a8228
	mov r2, r10
	mov r3, r8
	strh r6, [r5, #2]
	strh r2, [r5, #4]
	str r3, [r5, #28]
	adds r0, r5, #0
	bl Func_081a68b4
	cmp r0, #0
	bne .L_081a7172
	movs r0, #120
	bl Func_081a6094
	b .L_081a7172
.L_081a715e:
	movs r0, #0
	movs r1, #0
	bl Func_081a81d4
	movs r0, #10
	bl Func_081a8228
	movs r0, #10
	bl WaitFrames
.L_081a7172:
	movs r3, #0
	mov r0, r11
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #224
	adds r3, #212
	lsls r1, r1, #19
	ldr r2, .L_081a71c4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081a71b4
	bl Func_08014644
	movs r0, #172
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a71ac:
	.4byte 0x0000001a
.L_081a71b0:
	.4byte 0x05000200
.L_081a71b4:
	.4byte Func_081a6b50
.L_081a71b8:
	.4byte Data_02017080
.L_081a71bc:
	.4byte 0x06007800
.L_081a71c0:
	.4byte 0x0600f9c2
.L_081a71c4:
	.4byte 0x85000100
