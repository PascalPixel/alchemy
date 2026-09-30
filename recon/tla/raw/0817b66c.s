.syntax unified
	.thumb
	.global Func_0817b66c
	.thumb_func
Func_0817b66c:
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
	mov r11, r0
	movs r0, #0
	sub sp, #32
	mov r10, r3
	bl Func_081435e0
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0817b718
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	movs r2, #0
	add r3, r10
	str r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	movs r1, #200
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, .L_0817b71c
	bl Func_080145a8
	mov r2, r11
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl Func_08118088 + 0x10
	mov r1, r11
	add r5, sp, #20
	movs r3, #36
	ldrsh r0, [r1, r3]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r3, .L_0817b714
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r2, [r5]
	movs r1, #128
	movs r3, #64
	subs r3, r3, r2
	lsls r1, r1, #19
	lsls r3, r3, #8
	adds r1, #40
	str r3, [r1]
	mov r3, sp
	adds r3, #12
	str r3, [sp, #8]
	movs r2, #0
	mov r9, r2
.L_0817b700:
	mov r1, r9
	cmp r1, #0
	bne .L_0817b77c
	movs r2, #0
	movs r6, #128
	movs r7, #0
	mov r8, r2
	lsls r6, r6, #16
	mov r5, r10
	b .L_0817b720
.L_0817b714:
	.4byte 0x00000100
.L_0817b718:
	.4byte 0x00000139
.L_0817b71c:
	.4byte Func_08143000
.L_0817b720:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r5]
	str r6, [r5, #4]
	bl Random16
	mov r3, r8
	str r3, [r5, #12]
	str r0, [r5, #8]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #32
	negs r3, r3
	lsls r3, r3, #10
	str r3, [r5, #16]
	bl Random16
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	movs r1, #128
	ands r3, r0
	lsls r1, r1, #3
	adds r2, r3, r1
	movs r3, #1
	ands r3, r7
	str r2, [r5, #20]
	cmp r3, #0
	beq .L_0817b76a
	negs r3, r2
	str r3, [r5, #20]
.L_0817b76a:
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #12
	adds r7, #1
	str r2, [r5, #24]
	adds r6, r6, r3
	adds r5, #28
	cmp r7, #32
	bne .L_0817b720
.L_0817b77c:
	bl Func_08014de4
	ldr r2, .L_0817b948
	movs r3, #104
	str r3, [r2, #16]
	movs r7, #0
	movs r5, #20
.L_0817b78a:
	cmp r9, r5
	bne .L_0817b7ce
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #4
	str r3, [r2]
	movs r0, #133
	bl Audio_PlayCue
	mov r2, r11
	movs r3, #6
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	movs r2, #5
	bl Func_0814cd48
	mov r1, r11
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl Func_0815f000
.L_0817b7ce:
	adds r7, #1
	adds r5, #6
	cmp r7, #8
	bne .L_0817b78a
	mov r2, r9
	cmp r2, #90
	bne .L_0817b81c
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	movs r3, #4
	add r2, r10
	str r3, [r2]
	movs r0, #134
	bl Func_08118088 + 0x60
	mov r1, r11
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r3, #10
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	mov r3, r11
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl Func_0815f000
.L_0817b81c:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	movs r3, #0
	adds r6, r0, #0
	str r3, [r6, #20]
	ldr r2, .L_0817b94c
	ldr r3, [sp, #12]
	ldr r1, [sp, #8]
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_0817b950
	str r1, [r6, #16]
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #12]
	movs r3, #224
	lsls r3, r3, #3
	add r3, r10
	str r3, [r1, #4]
	movs r3, #9
	str r3, [r6]
	ldr r3, .L_0817b954
	mov r2, r8
	str r3, [r6, #8]
	str r2, [r6, #12]
	movs r7, #0
	mov r5, r10
.L_0817b862:
	mov r3, r9
	cmp r3, #80
	bne .L_0817b89c
	ldr r3, .L_0817b958
	str r3, [r5, #4]
	bl Random16
	movs r3, #192
	lsls r3, r3, #2
	ldr r1, .L_0817b95c
	adds r3, #255
	ands r3, r0
	lsls r2, r7, #10
	adds r2, r2, r3
	adds r2, r2, r1
	movs r1, #128
	lsls r3, r2, #10
	lsls r1, r1, #15
	adds r3, r3, r1
	str r3, [r5]
	movs r3, #192
	lsls r3, r3, #12
	str r2, [r5, #8]
	negs r2, r2
	lsls r2, r2, #5
	str r3, [r5, #16]
	movs r3, #0
	str r2, [r5, #12]
	str r3, [r5, #20]
.L_0817b89c:
	bl Func_08014de4
	ldr r3, .L_0817b960
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	movs r0, #128
	lsls r0, r0, #8
	bl Func_0801521c
	ldr r0, [r5, #8]
	bl Func_080150e4
	movs r2, #4
	ldr r0, .L_0817b964
	mov r1, r8
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	mov r2, r9
	cmp r2, #79
	ble .L_0817b8e0
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	b .L_0817b8ea
.L_0817b8e0:
	adds r0, r5, #0
	movs r1, #64
	ldr r2, .L_0817b968
	bl BattleFxKernels_IntegrateVector3
.L_0817b8ea:
	adds r7, #1
	adds r5, #28
	cmp r7, #8
	bne .L_0817b862
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	ldr r2, .L_0817b948
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
	movs r1, #4
	movs r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r10
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r9, r3
	mov r1, r9
	cmp r1, #96
	beq .L_0817b930
	b .L_0817b700
.L_0817b930:
	ldr r0, .L_0817b96c
	bl Func_08014644
	bl Func_08143bb8
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0817b948:
	.4byte gCameraSceneParameters
.L_0817b94c:
	.4byte 0xffffff00
.L_0817b950:
	.4byte 0xffff00ff
.L_0817b954:
	.4byte Data_08199450
.L_0817b958:
	.4byte 0xffd00000
.L_0817b95c:
	.4byte 0xffffee00
.L_0817b960:
	.4byte 0xffc00000
.L_0817b964:
	.4byte Data_08199474
.L_0817b968:
	.4byte 0xfffff000
.L_0817b96c:
	.4byte Func_08143000
