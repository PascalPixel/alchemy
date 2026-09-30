.syntax unified
	.thumb
	.global Func_08191530
	.thumb_func
Func_08191530:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_081915a4
	movs r5, #192
	ldrh r3, [r3, #4]
	sub sp, #28
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	ldr r1, [r5, #92]
	str r3, [sp, #16]
	adds r3, r5, #0
	adds r3, #176
	ldr r3, [r3]
	mov r9, r0
	movs r0, #3
	str r3, [sp, #12]
	mov r10, r1
	bl Func_081435e0
	ldr r3, .L_08191594
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08191598
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0819159c
	subs r2, #2
	strh r3, [r2]
	ldr r3, .L_081915a0
	subs r2, #68
	strh r3, [r2]
	subs r2, #2
	ldrh r1, [r2]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r1
	strh r3, [r2]
	movs r1, #19
	ldrh r3, [r2]
	movs r0, #104
	strh r3, [r2]
	movs r2, #239
	b .L_081915a8
.L_08191594:
	.4byte 0x00007741
.L_08191598:
	.4byte 0x00001010
.L_0819159c:
	.4byte 0x00003f42
.L_081915a0:
	.4byte 0x00000787
.L_081915a4:
	.4byte Data_03001120
.L_081915a8:
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #50
	add r2, r10
	str r3, [r2]
	bl Func_081963ec
	ldr r5, [r5, #104]
	ldr r0, .L_0819191c
	str r5, [sp, #20]
	bl Resource_GetTableEntry
	movs r5, #224
	adds r7, r0, #0
	movs r0, #160
	lsls r0, r0, #19
	lsls r5, r5, #3
	movs r2, #128
	adds r1, r7, #0
	ldr r6, .L_08191920
	adds r7, #128
	adds r0, #192
	add r5, r10
	mov lr, r6
	.2byte 0xf800
	adds r1, r5, #0
	adds r0, r7, #0
	bl Func_0801587c
	ldr r0, .L_08191924
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #160
	lsls r0, r0, #19
	adds r1, r7, #0
	movs r2, #128
	adds r0, #192
	mov lr, r6
	.2byte 0xf800
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #7
.L_08191608:
	ldrb r3, [r5]
	adds r1, #1
	adds r3, #96
	strb r3, [r5]
	adds r5, #1
	cmp r1, r2
	bne .L_08191608
	ldr r5, .L_08191928
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl Func_080145a8
	movs r1, #224
	movs r3, #128
	lsls r1, r1, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r4, [sp, #20]
	add r1, r10
	movs r2, #0
	movs r3, #0
	mov r0, r9
	mov lr, r4
	.2byte 0xf800
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0819191c
	mov r1, r9
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r0, #128
	movs r1, #0
	movs r4, #1
	lsls r0, r0, #7
	mov r2, r9
.L_0819166e:
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_08191676
	strb r4, [r2]
.L_08191676:
	adds r1, #1
	adds r2, #1
	cmp r1, r0
	bne .L_0819166e
	movs r0, #1
	movs r3, #3
	mov r8, r0
	movs r2, #7
	movs r1, #8
	movs r0, #104
	str r3, [sp, #0]
	bl Func_08196404
	movs r3, #192
	movs r1, #200
	lsls r3, r3, #18
	lsls r1, r1, #4
	ldr r0, .L_0819192c
	ldr r6, [r3, #104]
	bl Func_080145a8
	ldr r2, [sp, #12]
	movs r5, #128
	mov r1, r8
	str r1, [r2, #16]
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	mov r1, r9
	movs r2, #0
	ldr r0, .L_08191930
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	str r5, [sp, #0]
	str r5, [sp, #4]
	movs r2, #128
	movs r3, #0
	ldr r0, .L_08191930
	mov lr, r6
	.2byte 0xf800
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	mov r4, r8
	add r3, r10
	str r4, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r4, .L_08191934
	movs r0, #0
	mov r11, r0
	b .L_081918cc
.L_081916e2:
	mov r1, r11
	cmp r1, #0
	bne .L_081916fa
	movs r0, #160
	str r4, [sp, #8]
	ldr r3, .L_08191938
	lsls r0, r0, #19
	movs r1, #128
	ldr r2, .L_0819193c
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #8]
.L_081916fa:
	mov r2, r11
	cmp r2, #119
	ble .L_08191710
	movs r2, #1
	negs r2, r2
	adds r0, r2, #0
	adds r1, r2, #0
	str r4, [sp, #8]
	bl Func_08164a4c
	b .L_08191722
.L_08191710:
	movs r3, #3
	mov r0, r11
	ands r3, r0
	cmp r3, #1
	bne .L_08191724
	ldr r0, .L_08191940
	str r4, [sp, #8]
	bl Func_0815f0a0
.L_08191722:
	ldr r4, [sp, #8]
.L_08191724:
	mov r1, r11
	ldr r2, .L_08191944
	lsrs r3, r1, #31
	add r3, r11
	asrs r3, r3, #1
	lsls r5, r1, #6
	movs r6, #133
	strh r3, [r2, #4]
	lsls r6, r6, #2
	adds r0, r5, #0
	str r4, [sp, #8]
	bl Trig_Sin
	adds r1, r6, #0
	bl Math_Div
	mov r9, r0
	adds r0, r5, #0
	bl Trig_Cos
	adds r1, r6, #0
	bl Math_Div
	ldr r7, .L_08191948
	mov r10, r0
	ldr r4, [sp, #8]
	ldrh r3, [r4]
	adds r1, r3, #0
	strh r4, [r4]
	ldrh r2, [r7]
	cmp r2, #31
	bgt .L_08191782
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r3, #4
	adds r2, #1
	stmia r3!, {r0}
	strh r2, [r7]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08191782:
	strh r1, [r4]
	ldrh r3, [r4]
	adds r1, r3, #0
	strh r4, [r4]
	ldrh r2, [r7]
	cmp r2, #31
	bgt .L_081917b0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r3, #4
	adds r2, #1
	mov r0, r9
	stmia r3!, {r0}
	strh r2, [r7]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #34
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_081917b0:
	strh r1, [r4]
	ldrh r3, [r4]
	adds r1, r3, #0
	strh r4, [r4]
	ldrh r2, [r7]
	cmp r2, #31
	bgt .L_081917e0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r7
	mov r0, r9
	adds r3, #4
	strh r2, [r7]
	negs r2, r0
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #36
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_081917e0:
	strh r1, [r4]
	ldrh r3, [r4]
	adds r1, r3, #0
	strh r4, [r4]
	ldrh r2, [r7]
	cmp r2, #31
	bgt .L_0819180e
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r3, #4
	adds r2, #1
	mov r0, r10
	stmia r3!, {r0}
	strh r2, [r7]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #38
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0819180e:
	strh r1, [r4]
	ldrh r3, [r4]
	mov r8, r3
	strh r4, [r4]
	ldrh r3, [r7]
	cmp r3, #31
	bgt .L_08191862
	lsls r6, r3, #1
	adds r6, r6, r3
	movs r0, #136
	adds r3, #1
	str r4, [sp, #8]
	strh r3, [r7]
	mov r1, r10
	ldr r3, .L_0819194c
	lsls r0, r0, #24
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	movs r0, #184
	lsls r0, r0, #24
	mov r1, r9
	ldr r2, .L_0819194c
	mov lr, r2
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #15
	lsls r6, r6, #2
	adds r5, r5, r0
	adds r5, r5, r3
	adds r6, r6, r7
	adds r6, #4
	asrs r5, r5, #8
	stmia r6!, {r5}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #40
	stmia r6!, {r3}
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6]
	ldr r4, [sp, #8]
.L_08191862:
	mov r0, r8
	strh r0, [r4]
	ldrh r3, [r4]
	mov r8, r3
	strh r4, [r4]
	ldrh r3, [r7]
	cmp r3, #31
	bgt .L_081918ba
	lsls r6, r3, #1
	mov r2, r9
	adds r6, r6, r3
	movs r0, #136
	adds r3, #1
	negs r1, r2
	str r4, [sp, #8]
	strh r3, [r7]
	lsls r0, r0, #24
	ldr r3, .L_0819194c
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	movs r0, #184
	lsls r0, r0, #24
	mov r1, r10
	ldr r2, .L_0819194c
	mov lr, r2
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #15
	lsls r6, r6, #2
	adds r5, r5, r0
	adds r5, r5, r3
	adds r6, r6, r7
	adds r6, #4
	asrs r5, r5, #8
	stmia r6!, {r5}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #44
	stmia r6!, {r3}
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6]
	ldr r4, [sp, #8]
.L_081918ba:
	mov r0, r8
	strh r0, [r4]
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
	ldr r4, [sp, #8]
	movs r1, #1
	add r11, r1
.L_081918cc:
	ldr r2, .L_08191950
	cmp r11, r2
	beq .L_081918ee
	ldr r1, .L_08191954
	movs r2, #2
	ldr r3, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081918e2
	movs r3, #0
	mov r11, r3
.L_081918e2:
	ldr r3, [r1, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_081918ee
	b .L_081916e2
.L_081918ee:
	add r4, sp, #16
	ldr r3, .L_08191944
	ldrh r4, [r4]
	movs r2, #0
	strh r4, [r3, #4]
	ldr r0, [sp, #12]
	str r2, [r0, #16]
	ldr r0, .L_0819192c
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0819191c:
	.4byte 0x000000cc
.L_08191920:
	.4byte IwramCopyWords
.L_08191924:
	.4byte 0x00000178
.L_08191928:
	.4byte Func_08143000
.L_0819192c:
	.4byte Func_08143174
.L_08191930:
	.4byte gMapCellBuffer
.L_08191934:
	.4byte 0x04000208
.L_08191938:
	.4byte IwramFillWords
.L_0819193c:
	.4byte 0x7fff7fff
.L_08191940:
	.4byte 0x00000130
.L_08191944:
	.4byte Data_03001120
.L_08191948:
	.4byte Data_020038e0
.L_0819194c:
	.4byte IwramMulQ16
.L_08191950:
	.4byte 0x0004c2c0
.L_08191954:
	.4byte gInput
