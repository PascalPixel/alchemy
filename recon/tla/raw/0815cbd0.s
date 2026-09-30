.syntax unified
	.thumb
	.global Func_0815cbd0
	.thumb_func
Func_0815cbd0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r6, #192
	lsls r6, r6, #18
	ldr r2, [r6, #96]
	ldr r3, [r6, #92]
	mov r9, r0
	movs r0, #2
	sub sp, #20
	mov r10, r3
	mov r11, r2
	bl Func_081435e0
	ldr r3, .L_0815cc30
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0815cc34
	add r5, sp, #8
	adds r2, #50
	strh r3, [r2]
	adds r0, r5, #0
	bl Func_0815e22c
	ldr r2, [r5]
	movs r1, #128
	movs r3, #64
	subs r3, r3, r2
	lsls r1, r1, #19
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0815cc38
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #239
	b .L_0815cc3c
.L_0815cc30:
	.4byte 0x00000100
.L_0815cc34:
	.4byte 0x00001000
.L_0815cc38:
	.4byte 0x0000011b
.L_0815cc3c:
	lsls r2, r2, #7
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #0
	add r2, r10
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0815cc98
	bl Func_080145a8
	movs r0, #143
	bl Audio_PlayCue
	movs r3, #32
	movs r7, #0
	mov r8, r3
.L_0815cc66:
	cmp r7, #8
	bgt .L_0815cc78
	ldr r2, .L_0815cc90
	lsls r3, r7, #1
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0815cc78:
	cmp r7, #53
	ble .L_0815cc9c
	ldr r2, .L_0815cc94
	lsls r3, r7, #1
	subs r2, r2, r3
	ldr r3, .L_0815cc90
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	b .L_0815cc9c
.L_0815cc90:
	.4byte 0x00001000
.L_0815cc94:
	.4byte 0x0000007c
.L_0815cc98:
	.4byte Func_08143000
.L_0815cc9c:
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	mov r2, r8
	str r2, [sp, #0]
	str r2, [sp, #4]
	movs r3, #192
	movs r1, #224
	lsls r3, r3, #18
	lsls r1, r1, #3
	ldr r4, [r3, #104]
	movs r2, #33
	movs r3, #41
	add r1, r10
	mov r0, r11
	mov lr, r4
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #39
	movs r0, #104
	bl Func_081963ec
	mov r2, r8
	str r2, [sp, #0]
	str r2, [sp, #4]
	movs r3, #192
	movs r1, #224
	lsls r3, r3, #18
	lsls r1, r1, #3
	ldr r4, [r3, #104]
	movs r2, #64
	movs r3, #41
	add r1, r10
	mov r0, r11
	mov lr, r4
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #43
	movs r0, #104
	bl Func_081963ec
	mov r2, r8
	str r2, [sp, #0]
	str r2, [sp, #4]
	movs r3, #192
	movs r1, #224
	lsls r3, r3, #18
	lsls r1, r1, #3
	ldr r4, [r3, #104]
	movs r2, #33
	movs r3, #72
	add r1, r10
	mov r0, r11
	mov lr, r4
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #47
	movs r0, #104
	bl Func_081963ec
	mov r2, r8
	str r2, [sp, #0]
	str r2, [sp, #4]
	movs r3, #192
	movs r1, #224
	lsls r3, r3, #18
	lsls r1, r1, #3
	ldr r4, [r3, #104]
	mov r0, r11
	add r1, r10
	movs r2, #64
	movs r3, #72
	mov lr, r4
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	cmp r7, #32
	bne .L_0815cd4e
	movs r0, #143
	bl Func_08118088 + 0x60
.L_0815cd4e:
	mov r2, r9
	ldr r3, [r2, #20]
	movs r5, #0
	cmp r3, #0
	beq .L_0815cd7e
	movs r6, #36
.L_0815cd5a:
	cmp r7, #10
	bne .L_0815cd76
	mov r3, r9
	ldrsh r0, [r6, r3]
	movs r3, #8
	movs r2, #1
	str r3, [sp, #0]
	negs r2, r2
	adds r3, r5, #0
	movs r1, #7
	bl Func_0814cd48
	mov r2, r9
	ldr r3, [r2, #20]
.L_0815cd76:
	adds r5, #1
	adds r6, #2
	cmp r5, r3
	bne .L_0815cd5a
.L_0815cd7e:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #63
	beq .L_0815cd9c
	b .L_0815cc66
.L_0815cd9c:
	ldr r0, .L_0815cdd8
	bl Func_08014644
	movs r1, #240
	ldr r5, .L_0815cddc
	lsls r1, r1, #6
	ldr r0, .L_0815cde0
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	lsls r1, r1, #6
	mov r0, r11
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	mov r2, r9
	str r3, [r2, #28]
	ldr r0, .L_0815cde4
	bl Func_08014644
	mov r0, r9
	bl Func_081504c0
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0815cdd8:
	.4byte Func_08143000
.L_0815cddc:
	.4byte IwramClearWords
.L_0815cde0:
	.4byte 0x06004000
.L_0815cde4:
	.4byte Func_08143488
