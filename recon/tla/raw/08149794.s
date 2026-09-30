.syntax unified
	.thumb
	.global Func_08149794
	.thumb_func
Func_08149794:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r9, r0
	ldr r0, [r5, #92]
	sub sp, #48
	str r0, [sp, #36]
	movs r0, #1
	ldr r1, [r5, #96]
	movs r7, #224
	str r1, [sp, #32]
	lsls r7, r7, #3
	ldr r2, [r5, #100]
	str r2, [sp, #24]
	bl Func_081435e0
	ldr r3, .L_08149800
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r1, #23
	movs r0, #188
	str r3, [sp, #40]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r4, sp
	adds r4, #40
	str r4, [sp, #12]
	str r3, [r4, #4]
	ldr r5, [sp, #36]
	ldr r0, .L_08149804
	adds r1, r5, r7
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r0, .L_08149808
	movs r3, #0
	b .L_0814980c
	.2byte 0x0000
.L_08149800:
	.4byte 0x00001010
.L_08149804:
	.4byte 0x00000194
.L_08149808:
	.4byte 0x00000134
.L_0814980c:
	ldr r1, [sp, #24]
	movs r2, #0
	bl Func_08157cf4
	mov r0, r9
	ldr r3, [r0, #24]
	cmp r3, #2
	beq .L_08149830
	ldr r0, .L_08149b64
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08149b68
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08149830:
	ldr r3, .L_08149b6c
	movs r1, #0
	movs r2, #128
	mov r8, r1
	lsls r2, r2, #3
.L_0814983a:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0814983a
	ldr r5, [sp, #36]
	movs r7, #239
	movs r0, #238
	lsls r7, r7, #7
	lsls r0, r0, #7
	adds r2, r5, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	movs r1, #200
	adds r2, r5, r0
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08149b70
	bl Func_080145a8
	mov r1, r9
	ldr r2, [r1, #24]
	ldr r3, .L_08149b74
	movs r7, #42
	ldrb r3, [r3, r2]
	negs r7, r7
	str r3, [sp, #20]
	ldr r4, [sp, #20]
	ldr r5, [sp, #20]
	movs r3, #0
	lsls r4, r4, #3
	mov r11, r3
	subs r3, r4, r5
	str r4, [sp, #8]
	cmp r3, r7
	bne .L_0814988a
	b .L_08149b40
.L_0814988a:
	b .L_08149890
.L_0814988c:
	mov r0, r9
	ldr r2, [r0, #24]
.L_08149890:
	cmp r2, #2
	bne .L_081498c8
	mov r1, r11
	cmp r1, #63
	bgt .L_081498c8
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #48]
	movs r2, #128
	mov r3, r11
	lsls r2, r2, #1
	cmp r3, #55
	ble .L_081498b4
	mov r4, r11
	movs r3, #176
	lsls r2, r4, #3
	lsls r3, r3, #2
	subs r2, r3, r2
.L_081498b4:
	mov r5, r9
	ldr r3, [r5, #4]
	cmp r3, #1
	bne .L_081498c2
	ldrh r3, [r1, #54]
	subs r3, r3, r2
	b .L_081498c6
.L_081498c2:
	ldrh r3, [r1, #54]
	adds r3, r3, r2
.L_081498c6:
	strh r3, [r1, #54]
.L_081498c8:
	mov r7, r11
	cmp r7, #24
	bne .L_081498d4
	movs r0, #134
	bl Func_081180e8
.L_081498d4:
	ldr r1, [sp, #20]
	movs r0, #0
	str r0, [sp, #28]
	cmp r1, #0
	bne .L_081498e0
	b .L_08149a7e
.L_081498e0:
	ldr r2, [sp, #28]
	lsls r5, r2, #3
	cmp r11, r5
	bne .L_081498fc
	movs r0, #134
	bl Audio_PlayCue
	movs r1, #128
	ldr r3, .L_08149b78
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_08149b7c
	mov lr, r3
	.2byte 0xf800
.L_081498fc:
	adds r3, r5, #4
	mov r10, r3
	cmp r11, r5
	bge .L_08149906
	b .L_08149a34
.L_08149906:
	adds r3, #5
	cmp r11, r3
	blt .L_0814990e
	b .L_08149a34
.L_0814990e:
	adds r3, r5, #1
	adds r6, r5, #2
	cmp r11, r3
	blt .L_08149946
	cmp r11, r6
	bge .L_0814994e
	mov r7, r9
	ldr r2, [r7, #4]
	ldr r0, [sp, #28]
	lsls r3, r2, #3
	ldr r1, .L_08149b80
	subs r3, r3, r2
	adds r3, r0, r3
	ldrb r2, [r1, r3]
	movs r3, #48
	str r3, [sp, #0]
	movs r3, #112
	str r3, [sp, #4]
	ldr r3, [sp, #36]
	movs r7, #224
	lsls r7, r7, #3
	adds r1, r3, r7
	subs r2, #24
	ldr r4, [sp, #40]
	ldr r0, [sp, #32]
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
.L_08149946:
	adds r0, r5, #4
	mov r10, r0
	cmp r11, r6
	blt .L_08149a34
.L_0814994e:
	adds r5, #4
	mov r10, r5
	cmp r11, r10
	bge .L_08149982
	mov r1, r9
	ldr r2, [r1, #4]
	ldr r4, .L_08149b80
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, [sp, #28]
	ldr r5, [sp, #36]
	adds r3, r2, r3
	ldrb r2, [r4, r3]
	movs r7, #224
	movs r3, #48
	str r3, [sp, #0]
	lsls r7, r7, #5
	movs r3, #112
	str r3, [sp, #4]
	subs r2, #24
	ldr r4, [sp, #40]
	ldr r0, [sp, #32]
	adds r1, r5, r7
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
.L_08149982:
	cmp r11, r6
	bne .L_08149a34
	movs r0, #0
	str r0, [sp, #16]
	ldr r7, .L_08149b84
	mov r8, r0
.L_0814998e:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_08149a0e
	bl Random16
	movs r6, #192
	lsls r6, r6, #2
	adds r6, #255
	ands r6, r0
	bl Random16
	mov r3, r9
	ldr r2, [r3, #4]
	movs r5, #254
	lsls r5, r5, #7
	ldr r4, [sp, #28]
	adds r5, #255
	lsls r3, r2, #3
	ands r5, r0
	ldr r0, .L_08149b80
	subs r3, r3, r2
	adds r3, r4, r3
	ldrb r3, [r0, r3]
	ldr r1, .L_08149b88
	lsls r3, r3, #16
	str r3, [r7]
	movs r3, #208
	adds r5, r5, r1
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	ldr r1, [sp, #16]
	mov r3, r9
	adds r1, #1
	str r1, [sp, #16]
	ldr r1, .L_08149b8c
	ldr r2, [r3, #24]
	ldr r4, [sp, #16]
	lsls r3, r2, #2
	ldrh r3, [r1, r3]
	cmp r4, r3
	beq .L_08149a22
.L_08149a0e:
	movs r5, #1
	movs r0, #128
	add r8, r5
	lsls r0, r0, #3
	adds r7, #28
	cmp r8, r0
	bne .L_0814998e
	mov r1, r9
	ldr r2, [r1, #24]
	ldr r1, .L_08149b8c
.L_08149a22:
	lsls r2, r2, #2
	ldr r4, [sp, #36]
	movs r5, #238
	adds r2, #2
	lsls r5, r5, #7
	ldrh r2, [r1, r2]
	adds r5, #168
	adds r3, r4, r5
	str r2, [r3]
.L_08149a34:
	cmp r11, r10
	bne .L_08149a70
	mov r0, r9
	ldr r3, [r0, #20]
	movs r7, #0
	mov r8, r7
	cmp r3, #0
	beq .L_08149a70
	movs r5, #36
.L_08149a46:
	mov r1, r9
	ldrsh r0, [r5, r1]
	movs r1, #1
	bl Func_08118088
	mov r3, r9
	ldrsh r0, [r5, r3]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	movs r2, #5
	bl Func_0814cd48
	mov r0, r9
	ldr r3, [r0, #20]
	movs r7, #1
	add r8, r7
	adds r5, #2
	cmp r8, r3
	bne .L_08149a46
.L_08149a70:
	ldr r1, [sp, #28]
	ldr r2, [sp, #20]
	adds r1, #1
	str r1, [sp, #28]
	cmp r1, r2
	beq .L_08149a7e
	b .L_081498e0
.L_08149a7e:
	ldr r6, .L_08149b84
	movs r3, #0
	mov r8, r3
.L_08149a84:
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_08149b00
	subs r3, #1
	movs r2, #128
	lsls r2, r2, #5
	str r3, [r6, #24]
	adds r0, r6, #0
	movs r1, #60
	bl BattleFxKernels_IntegrateVector2
	ldr r2, [r6, #4]
	movs r4, #208
	lsls r4, r4, #15
	cmp r2, r4
	ble .L_08149ab2
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_08149b00
.L_08149ab2:
	ldr r3, [r6]
	asrs r7, r3, #16
	cmp r3, #0
	blt .L_08149b00
	cmp r7, #119
	bgt .L_08149b00
	cmp r2, #0
	blt .L_08149b00
	ldr r0, [r6, #24]
	asrs r2, r2, #16
	mov r12, r2
	cmp r0, #0
	bge .L_08149ace
	adds r0, #7
.L_08149ace:
	asrs r0, r0, #3
	adds r0, #1
	mov r5, r8
	ldr r2, .L_08149b90
	movs r4, #1
	ands r4, r5
	lsls r5, r0, #1
	subs r3, r5, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #24]
	str r0, [sp, #0]
	adds r1, r2, r1
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r2, r7, r2
	mov r7, r12
	subs r3, r7, r0
	str r5, [sp, #4]
	ldr r0, [sp, #12]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
.L_08149b00:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r6, #28
	cmp r8, r2
	bne .L_08149a84
	movs r0, #8
	movs r1, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #36]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r7, [sp, #8]
	ldr r0, [sp, #20]
	movs r5, #1
	subs r3, r7, r0
	add r11, r5
	adds r3, #42
	cmp r11, r3
	beq .L_08149b40
	b .L_0814988c
.L_08149b40:
	ldr r0, .L_08149b70
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08149b64:
	.4byte 0x00000122
.L_08149b68:
	.4byte IwramCopyWords
.L_08149b6c:
	.4byte Data_02010018
.L_08149b70:
	.4byte Func_08143000
.L_08149b74:
	.4byte Data_081979da
.L_08149b78:
	.4byte IwramFillWords
.L_08149b7c:
	.4byte 0x10101010
.L_08149b80:
	.4byte Data_081979cc
.L_08149b84:
	.4byte gMapCellBuffer
.L_08149b88:
	.4byte 0xffffc000
.L_08149b8c:
	.4byte Data_081979c0
.L_08149b90:
	.4byte Data_08197410
