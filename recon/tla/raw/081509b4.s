.syntax unified
	.thumb
	.global Func_081509b4
	.thumb_func
Func_081509b4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	str r0, [sp, #20]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	ldr r1, [r5, #96]
	mov r10, r0
	movs r0, #0
	str r1, [sp, #16]
	bl Func_081435e0
	ldr r3, .L_08150a10
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_08150a14
	movs r1, #224
	adds r2, #2
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_08150a18
	movs r2, #1
	movs r3, #1
	add r1, r10
	bl Func_08157cf4
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
	ldr r5, [r5, #104]
	ldr r4, [sp, #20]
	str r5, [sp, #8]
	movs r2, #36
	ldrsh r3, [r4, r2]
	cmp r3, #127
	ble .L_08150a26
	b .L_08150a1c
.L_08150a10:
	.4byte 0x00003f46
.L_08150a14:
	.4byte 0x00001010
.L_08150a18:
	.4byte 0x00000178
.L_08150a1c:
	movs r0, #0
	movs r1, #1
	mov r11, r0
	mov r9, r1
	b .L_08150a30
.L_08150a26:
	movs r3, #1
	movs r2, #64
	negs r3, r3
	mov r11, r2
	mov r9, r3
.L_08150a30:
	ldr r6, .L_08150c00
	movs r4, #0
	mov r8, r4
	movs r7, #0
	mov r5, r10
.L_08150a3a:
	adds r0, r6, #0
	bl Trig_Sin
	lsls r0, r0, #5
	asrs r0, r0, #16
	mov r3, r9
	muls r3, r0
	add r3, r11
	adds r3, #20
	str r3, [r5]
	adds r0, r6, #0
	bl Trig_Cos
	lsls r0, r0, #4
	asrs r0, r0, #16
	adds r0, #40
	movs r1, #1
	str r0, [r5, #4]
	add r8, r1
	movs r0, #128
	lsls r0, r0, #5
	mov r2, r8
	str r7, [r5, #24]
	adds r6, r6, r0
	subs r7, #4
	adds r5, #28
	cmp r2, #9
	bne .L_08150a3a
	movs r3, #239
	lsls r3, r3, #7
	add r3, r10
	movs r2, #2
	str r2, [r3]
	ldr r4, [sp, #20]
	ldr r3, [r4, #24]
	cmp r3, #2
	bne .L_08150a90
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #75
	b .L_08150a9a
.L_08150a90:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
.L_08150a9a:
	str r3, [r2]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08150c04
	bl Func_080145a8
	movs r0, #136
	bl Audio_PlayCue
	movs r0, #0
	str r0, [sp, #12]
.L_08150ab0:
	ldr r1, [sp, #12]
	cmp r1, #24
	bne .L_08150abc
	movs r0, #133
	bl Func_081180e8
.L_08150abc:
	movs r2, #0
	mov r8, r2
	mov r6, r10
.L_08150ac2:
	ldr r3, [r6, #24]
	cmp r3, #23
	bhi .L_08150b78
	adds r2, r3, #0
	cmp r3, #0
	bge .L_08150ad0
	adds r2, r3, #3
.L_08150ad0:
	ldr r3, .L_08150c08
	asrs r5, r2, #2
	lsls r7, r5, #1
	ldrh r1, [r3, r7]
	ldr r0, .L_08150c0c
	movs r4, #224
	lsls r4, r4, #3
	add r1, r10
	adds r1, r1, r4
	ldrb r4, [r0, r5]
	ldr r2, [r6]
	lsrs r3, r4, #1
	subs r2, r2, r3
	ldr r3, .L_08150c10
	ldrb r0, [r3, r5]
	mov r9, r3
	ldr r3, [r6, #4]
	str r4, [sp, #0]
	ldr r4, .L_08150c14
	adds r3, r3, r0
	ldrb r0, [r4, r5]
	mov r11, r4
	str r0, [sp, #4]
	ldr r4, [sp, #8]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [sp, #20]
	ldr r3, [r0, #24]
	cmp r3, #0
	beq .L_08150b42
	ldr r2, .L_08150c08
	ldr r0, .L_08150c0c
	ldrh r1, [r2, r7]
	ldrb r4, [r0, r5]
	ldr r2, [r6]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r10
	adds r1, r1, r3
	lsrs r3, r4, #1
	subs r2, r2, r3
	mov r3, r9
	ldrb r0, [r3, r5]
	ldr r3, [r6, #4]
	str r4, [sp, #0]
	mov r4, r11
	adds r3, r3, r0
	ldrb r0, [r4, r5]
	subs r3, #16
	str r0, [sp, #4]
	ldr r4, [sp, #8]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [sp, #20]
	ldr r3, [r0, #24]
.L_08150b42:
	cmp r3, #2
	bne .L_08150b76
	ldr r2, .L_08150c08
	ldr r0, .L_08150c0c
	ldrh r1, [r2, r7]
	ldrb r4, [r0, r5]
	ldr r2, [r6]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r10
	adds r1, r1, r3
	lsrs r3, r4, #1
	subs r2, r2, r3
	mov r3, r9
	ldrb r0, [r3, r5]
	ldr r3, [r6, #4]
	str r4, [sp, #0]
	mov r4, r11
	adds r3, r3, r0
	ldrb r0, [r4, r5]
	subs r3, #32
	str r0, [sp, #4]
	ldr r4, [sp, #8]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
.L_08150b76:
	ldr r3, [r6, #24]
.L_08150b78:
	movs r0, #1
	add r8, r0
	adds r3, #1
	mov r1, r8
	str r3, [r6, #24]
	adds r6, #28
	cmp r1, #9
	bne .L_08150ac2
	ldr r4, [sp, #20]
	movs r2, #0
	ldr r3, [r4, #20]
	mov r8, r2
	cmp r3, #0
	beq .L_08150bc0
	movs r6, #36
	movs r5, #16
.L_08150b98:
	ldr r0, [sp, #12]
	cmp r0, r5
	bne .L_08150bb4
	ldr r1, [sp, #20]
	movs r3, #12
	ldrsh r0, [r6, r1]
	str r3, [sp, #0]
	movs r1, #10
	mov r3, r8
	movs r2, #5
	bl Func_0814cd48
	ldr r4, [sp, #20]
	ldr r3, [r4, #20]
.L_08150bb4:
	movs r0, #1
	add r8, r0
	adds r6, #2
	adds r5, #8
	cmp r8, r3
	bne .L_08150b98
.L_08150bc0:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #12]
	adds r1, #1
	str r1, [sp, #12]
	cmp r1, #60
	beq .L_08150be2
	b .L_08150ab0
.L_08150be2:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08150c04
	bl Func_08014644
	bl Func_08143bb8
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08150c00:
	.4byte 0xffffc000
.L_08150c04:
	.4byte Func_08143000
.L_08150c08:
	.4byte Data_0819747a
.L_08150c0c:
	.4byte Data_08197467
.L_08150c10:
	.4byte Data_08197473
.L_08150c14:
	.4byte Data_0819746d
