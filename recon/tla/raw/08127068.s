.syntax unified
	.thumb
	.global Func_08127068
	.thumb_func
Func_08127068:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r0, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r2, [r3]
	movs r1, #152
	movs r3, #1
	lsls r1, r1, #5
	str r3, [r2, #8]
	adds r1, #208
	movs r0, #92
	bl Runtime_AllocateBlock
	movs r1, #128
	mov r9, r0
	lsls r1, r1, #7
	movs r0, #96
	movs r7, #142
	bl Runtime_AllocateHeapBlock
	lsls r7, r7, #5
	movs r1, #1
	movs r2, #15
	mov r11, r1
	add r7, r9
	mov r10, r2
.L_081270ac:
	bl Random16
	adds r5, r0, #0
	bl Random16
	movs r3, #128
	lsls r3, r3, #9
	adds r3, r3, r0
	lsrs r6, r3, #1
	adds r0, r5, #0
	mov r8, r3
	bl Trig_Cos
	ldr r2, .L_0812728c
	adds r1, r6, #0
	mov lr, r2
	.2byte 0xf800
	str r0, [r7]
	adds r0, r5, #0
	bl Trig_Sin
	adds r1, r6, #0
	ldr r3, .L_0812728c
	mov lr, r3
	.2byte 0xf800
	ldr r2, [r7]
	mov r1, r11
	adds r3, r2, #0
	ands r3, r1
	str r0, [r7, #4]
	cmp r3, #0
	beq .L_081270f0
	negs r3, r2
	str r3, [r7]
.L_081270f0:
	ldr r2, [r7, #4]
	mov r1, r11
	adds r3, r2, #0
	ands r3, r1
	cmp r3, #0
	beq .L_08127100
	negs r3, r2
	str r3, [r7, #4]
.L_08127100:
	bl Random16
	movs r2, #128
	lsls r2, r2, #8
	adds r0, r0, r2
	lsrs r0, r0, #2
	ldr r3, [r7, #4]
	str r0, [r7, #8]
	ldr r0, [r7]
	asrs r2, r3, #8
	negs r0, r0
	negs r3, r3
	asrs r1, r0, #7
	asrs r3, r3, #7
	asrs r0, r0, #8
	adds r1, r1, r2
	adds r3, r3, r0
	str r1, [r7, #12]
	str r3, [r7, #16]
	mov r1, r8
	movs r3, #0
	movs r2, #1
	str r3, [r7, #20]
	negs r2, r2
	lsrs r3, r1, #13
	adds r3, #1
	add r10, r2
	str r3, [r7, #24]
	mov r3, r10
	adds r7, #28
	cmp r3, #0
	bge .L_081270ac
	movs r1, #128
	movs r5, #156
	ldr r7, .L_0812728c
	lsls r1, r1, #5
	lsls r5, r5, #5
	movs r2, #2
	mov r8, r1
	movs r6, #0
	add r5, r9
	mov r10, r2
.L_08127154:
	adds r0, r6, #0
	bl Trig_Cos
	mov r1, r8
	mov lr, r7
	.2byte 0xf800
	str r0, [r5]
	adds r0, r6, #0
	bl Trig_Sin
	mov r1, r8
	mov lr, r7
	.2byte 0xf800
	str r0, [r5, #4]
	adds r0, r6, #0
	bl Trig_Cos
	movs r1, #128
	lsls r1, r1, #2
	mov lr, r7
	.2byte 0xf800
	str r0, [r5, #8]
	adds r0, r6, #0
	bl Trig_Sin
	movs r1, #128
	lsls r1, r1, #2
	mov lr, r7
	.2byte 0xf800
	movs r3, #170
	movs r1, #1
	lsls r3, r3, #7
	negs r1, r1
	adds r3, #85
	add r10, r1
	movs r2, #0
	adds r6, r6, r3
	mov r3, r10
	str r0, [r5, #12]
	str r2, [r5, #16]
	adds r5, #20
	cmp r3, #0
	bge .L_08127154
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #188
	add r3, r9
	str r2, [r3]
	movs r3, #158
	lsls r3, r3, #5
	add r3, r9
	str r2, [r3]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #204
	add r3, r9
	str r2, [r3]
	movs r3, #192
	lsls r3, r3, #18
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, [r3, #96]
	ldr r3, .L_08127290
	mov lr, r3
	.2byte 0xf800
	ldr r6, .L_08127294
	adds r0, r6, #0
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	adds r1, r5, #0
	ldr r3, .L_08127298
	movs r2, #128
	lsls r0, r0, #19
	adds r5, #128
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	adds r0, r5, #0
	bl Func_0801587c
	ldr r1, [sp, #4]
	cmp r1, #1
	beq .L_08127214
	cmp r1, #1
	bgt .L_08127208
	cmp r1, #0
	beq .L_08127210
	b .L_0812721c
.L_08127208:
	ldr r2, [sp, #4]
	cmp r2, #2
	beq .L_08127218
	b .L_0812721c
.L_08127210:
	ldr r0, .L_0812729c
	b .L_0812721e
.L_08127214:
	adds r0, r6, #0
	b .L_0812721e
.L_08127218:
	ldr r0, .L_081272a0
	b .L_0812721e
.L_0812721c:
	ldr r0, .L_081272a4
.L_0812721e:
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	lsls r1, r1, #19
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #0
	subs r3, #172
	str r2, [r3]
	adds r3, #4
	str r2, [r3]
	movs r1, #128
	lsls r1, r1, #1
	subs r3, #12
	strh r1, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r1, [r3]
	movs r0, #104
	movs r1, #3
	movs r5, #144
	bl Func_08138048
	lsls r5, r5, #3
	movs r1, #19
	movs r0, #188
	bl Func_08138048
	adds r1, r5, #0
	ldr r0, .L_081272a8
	bl Func_080145a8
	adds r1, r5, #0
	ldr r0, .L_081272ac
	bl Func_080145a8
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0812728c:
	.4byte IwramMulQ16
.L_08127290:
	.4byte IwramClearWords
.L_08127294:
	.4byte 0x0000018d
.L_08127298:
	.4byte IwramCopyWords
.L_0812729c:
	.4byte 0x0000018c
.L_081272a0:
	.4byte 0x0000018e
.L_081272a4:
	.4byte 0x0000018f
.L_081272a8:
	.4byte Func_08126e00
.L_081272ac:
	.4byte Func_08127038
