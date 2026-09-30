.syntax unified
	.thumb
	.global Func_0817e698
	.thumb_func
Func_0817e698:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #32
	str r0, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	movs r0, #0
	mov r10, r3
	mov r9, r1
	bl Func_081435e0
	movs r1, #142
	lsls r1, r1, #7
	ldr r0, .L_0817e7d0
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r0, #224
	lsls r0, r0, #3
	movs r1, #128
	add r0, r10
	ldr r3, .L_0817e7d4
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	movs r5, #0
	movs r7, #0
	mov r6, r10
.L_0817e6e0:
	movs r0, #162
	adds r3, r7, r5
	lsls r0, r0, #7
	add r3, r10
	adds r0, #142
	adds r2, r3, r0
	movs r3, #157
	lsls r3, r3, #4
	adds r3, #255
	lsls r1, r5, #6
	adds r0, r6, r3
	add r1, r10
	adds r3, #33
	movs r4, #0
	adds r1, r1, r3
.L_0817e6fe:
	ldrb r3, [r2]
	adds r4, #1
	strb r3, [r0]
	adds r0, #1
	ldrb r3, [r2]
	adds r2, #1
	strb r3, [r1]
	subs r1, #1
	cmp r4, #17
	bne .L_0817e6fe
	adds r5, #1
	adds r7, #16
	adds r6, #64
	cmp r5, #49
	bne .L_0817e6e0
	movs r5, #0
	movs r7, #0
	mov r6, r10
.L_0817e722:
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #77
	lsls r1, r5, #6
	adds r0, r6, r2
	mov r2, r10
	adds r3, r1, r2
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #114
	adds r1, r3, r2
	adds r3, r7, #0
	movs r2, #168
	add r3, r10
	lsls r2, r2, #7
	mov r12, r3
	adds r2, #207
	movs r4, #0
	add r2, r12
.L_0817e748:
	ldrb r3, [r2]
	adds r4, #1
	strb r3, [r0]
	adds r0, #1
	ldrb r3, [r2]
	adds r2, #1
	strb r3, [r1]
	subs r1, #1
	cmp r4, #19
	bne .L_0817e748
	adds r5, #1
	adds r7, #19
	adds r6, #64
	cmp r5, #55
	bne .L_0817e722
	movs r5, #0
	movs r7, #0
	movs r6, #0
.L_0817e76c:
	movs r2, #156
	mov r0, r10
	lsls r2, r2, #6
	adds r3, r6, r0
	adds r2, #12
	adds r0, r3, r2
	adds r2, #39
	adds r1, r3, r2
	adds r3, r7, #0
	movs r2, #176
	add r3, r10
	lsls r2, r2, #7
	mov r12, r3
	adds r2, #228
	movs r4, #0
	add r2, r12
.L_0817e78c:
	ldrb r3, [r2]
	adds r4, #1
	strb r3, [r0]
	adds r0, #1
	ldrb r3, [r2]
	adds r2, #1
	strb r3, [r1]
	subs r1, #1
	cmp r4, #20
	bne .L_0817e78c
	adds r5, #1
	adds r7, #20
	adds r6, #64
	cmp r5, #64
	bne .L_0817e76c
	mov r0, r9
	cmp r0, #3
	bne .L_0817e7b4
	ldr r0, .L_0817e7d8
	b .L_0817e7bc
.L_0817e7b4:
	mov r2, r9
	cmp r2, #2
	bne .L_0817e7e4
	ldr r0, .L_0817e7dc
.L_0817e7bc:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817e7e0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0817e7f8
.L_0817e7d0:
	.4byte 0x0000013c
.L_0817e7d4:
	.4byte IwramClearWords
.L_0817e7d8:
	.4byte 0x00000148
.L_0817e7dc:
	.4byte 0x00000129
.L_0817e7e0:
	.4byte IwramCopyWords
.L_0817e7e4:
	ldr r0, .L_0817e84c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817e850
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0817e7f8:
	ldr r2, [sp, #8]
	movs r3, #36
	ldrsh r0, [r2, r3]
	mov r3, sp
	adds r3, #20
	adds r1, r3, #0
	str r3, [sp, #4]
	bl Func_0815e21c
	ldr r3, .L_0817e848
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r0, [sp, #4]
	movs r1, #128
	ldr r2, [r0]
	movs r3, #64
	subs r3, r3, r2
	lsls r1, r1, #19
	lsls r3, r3, #8
	adds r1, #40
	str r3, [r1]
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	movs r1, #200
	b .L_0817e854
	.2byte 0x0000
.L_0817e848:
	.4byte 0x00000100
.L_0817e84c:
	.4byte 0x00000150
.L_0817e850:
	.4byte IwramCopyWords
.L_0817e854:
	str r3, [r2]
	ldr r0, .L_0817e9f8
	lsls r1, r1, #4
	bl Func_080145a8
	mov r2, r9
	cmp r2, #2
	bne .L_0817e870
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #0
	b .L_0817e884
.L_0817e870:
	mov r3, r9
	cmp r3, #1
	beq .L_0817e87a
	cmp r3, #3
	bne .L_0817e88e
.L_0817e87a:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #8
.L_0817e884:
	str r3, [r2]
	movs r0, #212
	bl Audio_PlayCue
	b .L_0817e89a
.L_0817e88e:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #32
	str r3, [r2]
.L_0817e89a:
	mov r2, r9
	movs r0, #0
	lsls r2, r2, #2
	mov r8, r0
	mov r11, r2
.L_0817e8a4:
	mov r3, r8
	cmp r3, #0
	bne .L_0817e8de
	mov r0, r9
	cmp r0, #2
	bne .L_0817e8c8
	ldr r3, [sp, #8]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #32
	movs r2, #1
	str r3, [sp, #0]
	negs r2, r2
	movs r3, #0
	bl Func_0814cd48
	b .L_0817e8de
.L_0817e8c8:
	ldr r3, [sp, #8]
	movs r1, #10
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #32
	movs r2, #1
	str r3, [sp, #0]
	negs r2, r2
	movs r3, #0
	bl Func_0814cd48
.L_0817e8de:
	mov r0, r8
	cmp r0, #24
	bne .L_0817e8ea
	movs r0, #0
	bl Func_08118088 + 0x60
.L_0817e8ea:
	mov r2, r8
	cmp r2, #8
	bne .L_0817e8fc
	mov r3, r9
	cmp r3, #0
	bne .L_0817e8fc
	movs r0, #126
	bl Audio_PlayCue
.L_0817e8fc:
	mov r0, r8
	cmp r0, #31
	bgt .L_0817e99e
	mov r3, r8
	cmp r0, #0
	bge .L_0817e90a
	adds r3, #3
.L_0817e90a:
	asrs r7, r3, #2
	cmp r7, #2
	ble .L_0817e916
	movs r3, #1
	ands r3, r7
	adds r7, r3, #1
.L_0817e916:
	mov r2, r8
	cmp r2, #27
	bgt .L_0817e99e
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0817e9fc
	ldr r3, [sp, #12]
	adds r5, r0, #0
	ands r3, r2
	ldr r2, .L_0817ea00
	movs r0, #6
	orrs r3, r0
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #12]
	movs r2, #224
	lsls r3, r7, #12
	lsls r2, r2, #3
	add r3, r10
	adds r3, r3, r2
	add r2, sp, #12
	str r3, [r2, #4]
	ldr r3, .L_0817ea04
	str r0, [r5]
	str r3, [r5, #8]
	ldr r3, .L_0817ea08
	mov r0, r11
	ldr r3, [r3, r0]
	str r2, [r5, #16]
	str r3, [r5, #20]
	str r6, [r5, #12]
	bl Func_08014de4
	ldr r2, [sp, #4]
	movs r0, #0
	ldr r1, [r2, #4]
	movs r2, #0
	subs r1, #58
	lsls r1, r1, #16
	bl Func_08015160
	ldr r3, .L_0817ea0c
	mov r2, r11
	ldr r0, [r3, r2]
	lsls r0, r0, #1
	bl Func_0801521c
	adds r1, r6, #0
	movs r2, #4
	ldr r0, .L_0817ea10
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	adds r0, r5, #0
	bl Sys_Free
	adds r0, r6, #0
	bl Sys_Free
.L_0817e99e:
	mov r3, r9
	cmp r3, #0
	bne .L_0817e9ae
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	b .L_0817e9b6
.L_0817e9ae:
	movs r0, #16
	movs r1, #16
	bl Func_08158ce0
.L_0817e9b6:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add r8, r0
	mov r2, r8
	cmp r2, #48
	beq .L_0817e9d8
	b .L_0817e8a4
.L_0817e9d8:
	ldr r0, .L_0817e9f8
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0817e9f8:
	.4byte Func_08143000
.L_0817e9fc:
	.4byte 0xffffff00
.L_0817ea00:
	.4byte 0xffff00ff
.L_0817ea04:
	.4byte Data_08199340
.L_0817ea08:
	.4byte Data_081994e0
.L_0817ea0c:
	.4byte Data_081994d0
.L_0817ea10:
	.4byte Data_081991f0
