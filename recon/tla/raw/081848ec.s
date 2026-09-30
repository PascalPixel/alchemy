.syntax unified
	.thumb
	.global Func_081848ec
	.thumb_func
Func_081848ec:
	mov r0, r11
	cmp r0, #0
	bne .L_08184916
	ldr r3, .L_08184928
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, [sp, #164]
	movs r1, #3
	movs r2, #1
	mov r9, r1
	negs r2, r2
	adds r3, #108
.L_08184908:
	movs r4, #1
	add r9, r4
	mov r5, r9
	str r2, [r3]
	adds r3, #28
	cmp r5, #64
	bne .L_08184908
.L_08184916:
	movs r6, #220
	lsls r6, r6, #1
	cmp r11, r6
	bne .L_08184934
	movs r2, #128
	ldr r3, .L_0818492c
	lsls r2, r2, #19
	b .L_08184930
	.2byte 0x0000
.L_08184928:
	.4byte 0x00000786
.L_0818492c:
	.4byte 0x00000784
.L_08184930:
	adds r2, #12
	strh r3, [r2]
.L_08184934:
	movs r7, #114
	adds r7, #255
	cmp r11, r7
	bgt .L_08184956
	movs r3, #167
	lsls r3, r3, #1
	movs r1, #124
	adds r1, #255
	mov r0, r11
	muls r0, r3
	bl Math_Div
	ldr r1, .L_081849f0
	lsls r0, r0, #16
	adds r1, r0, r1
	str r1, [sp, #136]
	b .L_081849ca
.L_08184956:
	ldr r0, .L_081849f4
	add r0, r11
	cmp r0, #19
	bhi .L_08184976
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #102
	muls r0, r3
	bl Trig_Sin
	movs r2, #172
	lsls r3, r0, #3
	adds r3, r3, r0
	lsls r2, r2, #14
	adds r2, r3, r2
	b .L_081849c8
.L_08184976:
	ldr r3, .L_081849f8
	add r3, r11
	cmp r3, #95
	bhi .L_08184988
	ldr r3, [sp, #136]
	ldr r4, .L_081849fc
	adds r3, r3, r4
	str r3, [sp, #136]
	b .L_081849ca
.L_08184988:
	ldr r3, .L_08184a00
	add r3, r11
	cmp r3, #19
	bhi .L_0818499a
	ldr r5, [sp, #136]
	ldr r6, .L_08184a04
	adds r5, r5, r6
	str r5, [sp, #136]
	b .L_081849ca
.L_0818499a:
	ldr r3, .L_08184a08
	add r3, r11
	cmp r3, #47
	bhi .L_081849ca
	mov r7, r11
	lsls r3, r7, #3
	subs r3, r3, r7
	lsls r3, r3, #3
	ldr r0, .L_08184a0c
	add r3, r11
	movs r1, #234
	lsls r3, r3, #7
	lsls r1, r1, #9
	adds r3, r3, r0
	adds r1, #192
	cmp r3, r1
	ble .L_081849c2
	movs r3, #234
	lsls r3, r3, #9
	adds r3, #192
.L_081849c2:
	ldr r2, [sp, #136]
	lsls r3, r3, #1
	subs r2, r2, r3
.L_081849c8:
	str r2, [sp, #136]
.L_081849ca:
	movs r3, #124
	adds r3, #255
	cmp r11, r3
	bgt .L_08184a14
	movs r3, #104
	movs r1, #124
	mov r0, r11
	muls r0, r3
	adds r1, #255
	bl Math_Div
	ldr r3, .L_081849ec
	ldr r5, .L_08184a10
	subs r3, r3, r0
	strh r3, [r5, #6]
	b .L_08184a34
	.2byte 0x0000
.L_081849ec:
	.4byte 0x00000068
.L_081849f0:
	.4byte 0xfee60000
.L_081849f4:
	.4byte 0xfffffe8e
.L_081849f8:
	.4byte 0xfffffe7a
.L_081849fc:
	.4byte 0xffff8000
.L_08184a00:
	.4byte 0xfffffe1a
.L_08184a04:
	.4byte 0xffff0000
.L_08184a08:
	.4byte 0xfffffe06
.L_08184a0c:
	.4byte 0xffc87150
.L_08184a10:
	.4byte Data_03001120
.L_08184a14:
	ldr r3, .L_08184b38
	ldr r2, .L_08184b3c
	add r3, r11
	cmp r3, #0
	bge .L_08184a22
	ldr r3, .L_08184b40
	add r3, r11
.L_08184a22:
	asrs r3, r3, #2
	movs r4, #208
	strh r3, [r2, #6]
	lsls r4, r4, #15
	lsls r3, r3, #16
	cmp r3, r4
	bls .L_08184a34
	movs r3, #104
	strh r3, [r2, #6]
.L_08184a34:
	ldr r3, .L_08184b44
	add r3, r11
	cmp r3, #59
	bhi .L_08184a52
	lsls r0, r3, #4
	adds r0, r0, r3
	lsls r0, r0, #4
	adds r0, r0, r3
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	bl Trig_Sin
	lsls r0, r0, #4
	str r0, [sp, #124]
.L_08184a52:
	ldr r3, .L_08184b48
	add r3, r11
	cmp r3, #51
	bhi .L_08184a70
	ldr r6, [sp, #124]
	movs r3, #210
	ldr r7, .L_08184b4c
	lsls r3, r3, #1
	mov r5, r11
	subs r3, r3, r5
	lsls r3, r3, #12
	adds r6, r6, r7
	str r3, [sp, #128]
	str r6, [sp, #124]
	b .L_08184ad6
.L_08184a70:
	ldr r3, .L_08184b50
	add r3, r11
	cmp r3, #13
	bhi .L_08184a8e
	ldr r0, [sp, #128]
	movs r1, #234
	lsls r3, r3, #12
	lsls r1, r1, #9
	subs r3, r0, r3
	adds r1, #192
	adds r1, r3, r1
	ldr r2, [sp, #124]
	ldr r3, .L_08184b54
	str r1, [sp, #128]
	b .L_08184ac0
.L_08184a8e:
	ldr r3, .L_08184b58
	add r3, r11
	cmp r3, #8
	bhi .L_08184ab4
	ldr r4, [sp, #128]
	ldr r7, .L_08184b5c
	movs r5, #128
	mov r6, r11
	lsls r5, r5, #6
	lsls r0, r6, #7
	adds r4, r4, r5
	adds r0, r0, r7
	str r4, [sp, #128]
	bl Trig_Sin
	ldr r1, [sp, #124]
	subs r1, r1, r0
	str r1, [sp, #124]
	b .L_08184ad6
.L_08184ab4:
	ldr r3, .L_08184b60
	add r3, r11
	cmp r3, #29
	bhi .L_08184ac6
	ldr r2, [sp, #124]
	ldr r3, .L_08184b64
.L_08184ac0:
	adds r2, r2, r3
	str r2, [sp, #124]
	b .L_08184ad6
.L_08184ac6:
	ldr r3, .L_08184b68
	add r3, r11
	cmp r3, #9
	bhi .L_08184ad6
	ldr r4, [sp, #124]
	ldr r5, .L_08184b6c
	adds r4, r4, r5
	str r4, [sp, #124]
.L_08184ad6:
	mov r6, r11
	cmp r6, #0
	bne .L_08184ae6
	movs r7, #1
	negs r7, r7
	movs r0, #0
	str r7, [sp, #120]
	str r0, [sp, #116]
.L_08184ae6:
	ldr r1, [sp, #116]
	cmp r1, #17
	ble .L_08184aee
	b .L_08184c0a
.L_08184aee:
	ldr r3, .L_08184b70
	lsls r5, r1, #1
	ldrh r3, [r3, r5]
	movs r6, #1
	negs r6, r6
	cmp r11, r3
	bne .L_08184b7e
	ldr r3, .L_08184b74
	ldrh r3, [r3, r5]
	str r3, [sp, #120]
	movs r3, #3
	ands r3, r1
	cmp r3, #1
	beq .L_08184b26
	cmp r3, #1
	bgt .L_08184b14
	cmp r3, #0
	beq .L_08184b1e
	b .L_08184b7e
.L_08184b14:
	cmp r3, #2
	beq .L_08184b2e
	cmp r3, #3
	beq .L_08184b78
	b .L_08184b7e
.L_08184b1e:
	movs r0, #227
	bl Audio_PlayCue
	b .L_08184b7e
.L_08184b26:
	movs r0, #171
	bl Audio_PlayCue
	b .L_08184b7e
.L_08184b2e:
	movs r0, #208
	bl Audio_PlayCue
	b .L_08184b7e
	.2byte 0x0000
.L_08184b38:
	.4byte 0xfffffe84
.L_08184b3c:
	.4byte Data_03001120
.L_08184b40:
	.4byte 0xfffffe87
.L_08184b44:
	.4byte 0xfffffe8e
.L_08184b48:
	.4byte 0xfffffe5c
.L_08184b4c:
	.4byte 0xffffc000
.L_08184b50:
	.4byte 0xfffffe28
.L_08184b54:
	.4byte 0x00012710
.L_08184b58:
	.4byte 0xfffffe1a
.L_08184b5c:
	.4byte 0xffff0d00
.L_08184b60:
	.4byte 0xfffffdd6
.L_08184b64:
	.4byte 0xffff0000
.L_08184b68:
	.4byte 0xfffffdb8
.L_08184b6c:
	.4byte 0xfff80000
.L_08184b70:
	.4byte Data_08199768
.L_08184b74:
	.4byte Data_0819978c
.L_08184b78:
	movs r0, #172
	bl Audio_PlayCue
.L_08184b7e:
	ldr r2, [sp, #120]
	cmp r2, #0
	blt .L_08184ba6
	ldr r3, .L_08184be8
	lsls r0, r2, #5
	ldrh r1, [r3, r5]
	subs r0, r0, r2
	bl Math_Div
	ldr r3, [sp, #120]
	movs r4, #1
	subs r3, #1
	negs r4, r4
	adds r6, r0, #0
	str r3, [sp, #120]
	cmp r3, r4
	bne .L_08184ba6
	ldr r5, [sp, #116]
	adds r5, #1
	str r5, [sp, #116]
.L_08184ba6:
	cmp r6, #0
	blt .L_08184c0a
	ldr r7, [sp, #140]
	movs r1, #160
	movs r4, #160
	lsls r1, r1, #3
	ldr r5, .L_08184be4
	lsls r4, r4, #19
	adds r1, #108
	movs r2, #0
	adds r0, r7, r1
	adds r4, #192
	mov r9, r2
	movs r7, #31
.L_08184bc2:
	ldrh r3, [r0]
	adds r1, r7, #0
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r3, r5
	ands r2, r5
	adds r3, r3, r6
	cmp r1, #31
	ble .L_08184bda
	movs r1, #31
.L_08184bda:
	cmp r2, #31
	ble .L_08184bec
	movs r2, #31
	b .L_08184bec
	.2byte 0x0000
.L_08184be4:
	.4byte 0x0000001f
.L_08184be8:
	.4byte Data_0819978c
.L_08184bec:
	cmp r3, #31
	ble .L_08184bf2
	movs r3, #31
.L_08184bf2:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	strh r3, [r4]
	movs r3, #1
	add r9, r3
	mov r1, r9
	adds r0, #2
	adds r4, #2
	cmp r1, #128
	bne .L_08184bc2
.L_08184c0a:
	movs r3, #0
	movs r4, #0
	movs r2, #0
	str r3, [sp, #208]
	str r4, [sp, #212]
	mov r3, r11
	mov r8, r2
	mov r12, r2
	mov lr, r2
	cmp r3, #0
	bne .L_08184c30
	movs r4, #31
	negs r4, r4
	mov r12, r4
	movs r5, #1
	mov r8, r12
	mov lr, r12
	str r5, [sp, #208]
	b .L_08184d52
.L_08184c30:
	mov r6, r11
	cmp r6, #106
	bne .L_08184c4a
	movs r7, #31
	negs r7, r7
	movs r0, #15
	mov r12, r7
	negs r0, r0
	movs r1, #1
	mov r8, r12
	mov lr, r0
	str r1, [sp, #208]
	b .L_08184d52
.L_08184c4a:
	mov r2, r11
	subs r2, #154
	cmp r2, #59
	bhi .L_08184c6c
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	movs r1, #5
	lsls r2, r3, #1
	subs r2, r1, r2
	mov r12, r2
	subs r1, r1, r3
	movs r2, #1
	mov r8, r12
	mov lr, r1
	str r2, [sp, #208]
	b .L_08184d52
.L_08184c6c:
	mov r2, r11
	subs r2, #226
	cmp r2, #15
	bhi .L_08184c8c
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	subs r3, #31
	mov r12, r3
	ldr r3, .L_08184e6c
	movs r4, #1
	add r3, r11
	mov r8, r12
	mov lr, r3
	str r4, [sp, #208]
	b .L_08184d52
.L_08184c8c:
	mov r5, r11
	cmp r5, #242
	bne .L_08184ca4
	movs r6, #24
	movs r7, #31
	negs r6, r6
	negs r7, r7
	movs r0, #1
	mov r8, r6
	mov r12, r7
	str r0, [sp, #208]
	b .L_08184d52
.L_08184ca4:
	ldr r4, .L_08184e70
	add r4, r11
	cmp r4, #21
	bhi .L_08184cc8
	movs r2, #24
	lsls r3, r4, #1
	negs r2, r2
	subs r2, r2, r3
	mov r8, r2
	movs r2, #31
	negs r2, r2
	subs r2, r2, r3
	negs r4, r4
	movs r1, #1
	mov r12, r2
	mov lr, r4
	str r1, [sp, #208]
	b .L_08184d52
.L_08184cc8:
	ldr r2, .L_08184e74
	add r2, r11
	cmp r2, #40
	bhi .L_08184ce2
	negs r3, r2
	mov r12, r3
	movs r3, #16
	subs r3, r3, r2
	movs r4, #1
	mov r8, r12
	mov lr, r3
	str r4, [sp, #208]
	b .L_08184d52
.L_08184ce2:
	movs r5, #162
	lsls r5, r5, #1
	cmp r11, r5
	bne .L_08184cfc
	movs r6, #31
	movs r7, #1
	mov r8, r6
	mov r12, r6
	mov lr, r6
	add r6, sp, #208
	str r7, [sp, #208]
	str r7, [r6, #4]
	b .L_08184d54
.L_08184cfc:
	ldr r2, .L_08184e78
	add r2, r11
	cmp r2, #59
	bhi .L_08184d1e
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	movs r2, #30
	subs r2, r2, r3
	mov r12, r2
	movs r0, #1
	add r6, sp, #208
	str r0, [sp, #208]
	mov r8, r12
	mov lr, r12
	str r0, [r6, #4]
	b .L_08184d54
.L_08184d1e:
	ldr r5, .L_08184e7c
	add r6, sp, #208
	add r5, r11
	cmp r5, #42
	bhi .L_08184d54
	adds r2, r5, #0
	cmp r5, #0
	bge .L_08184d32
	ldr r2, .L_08184e80
	add r2, r11
.L_08184d32:
	asrs r2, r2, #2
	adds r0, r5, #0
	movs r1, #6
	mov r8, r2
	bl Math_Div
	mov r12, r0
	adds r0, r5, #0
	cmp r0, #0
	bge .L_08184d4a
	ldr r0, .L_08184e84
	add r0, r11
.L_08184d4a:
	movs r1, #1
	str r1, [sp, #208]
	asrs r0, r0, #4
	mov lr, r0
.L_08184d52:
	add r6, sp, #208
.L_08184d54:
	movs r2, #0
	mov r10, r2
.L_08184d58:
	mov r4, r10
	lsls r3, r4, #2
	ldr r3, [r3, r6]
	cmp r3, #0
	beq .L_08184de4
	cmp r4, #0
	bne .L_08184d76
	ldr r7, [sp, #164]
	movs r0, #174
	lsls r0, r0, #7
	adds r0, #2
	adds r5, r7, r0
	movs r1, #240
	ldr r7, .L_08184e88
	b .L_08184d88
.L_08184d76:
	ldr r1, [sp, #140]
	movs r2, #160
	movs r7, #160
	lsls r2, r2, #3
	lsls r7, r7, #19
	adds r2, #108
	adds r5, r1, r2
	adds r7, #192
	movs r1, #128
.L_08184d88:
	movs r3, #1
	mov r9, r3
	cmp r1, #1
	beq .L_08184de4
.L_08184d90:
	ldrh r3, [r5]
	movs r2, #31
	ands r2, r3
	mov r0, r8
	lsls r3, r3, #16
	adds r4, r2, r0
	lsrs r2, r3, #21
	movs r0, #31
	lsrs r3, r3, #26
	ands r2, r0
	ands r3, r0
	add r2, r12
	add r3, lr
	cmp r4, #31
	ble .L_08184db0
	movs r4, #31
.L_08184db0:
	cmp r2, #31
	ble .L_08184db6
	movs r2, #31
.L_08184db6:
	cmp r3, #31
	ble .L_08184dbc
	movs r3, #31
.L_08184dbc:
	cmp r4, #0
	bge .L_08184dc2
	movs r4, #0
.L_08184dc2:
	cmp r2, #0
	bge .L_08184dc8
	movs r2, #0
.L_08184dc8:
	cmp r3, #0
	bge .L_08184dce
	movs r3, #0
.L_08184dce:
	lsls r2, r2, #5
	lsls r3, r3, #10
	orrs r3, r2
	movs r2, #1
	orrs r3, r4
	add r9, r2
	strh r3, [r7]
	adds r5, #2
	adds r7, #2
	cmp r9, r1
	bne .L_08184d90
.L_08184de4:
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #2
	bne .L_08184d58
	ldr r2, .L_08184e8c
	add r2, r11
	cmp r2, #31
	bhi .L_08184e1e
	adds r3, r2, #0
	movs r5, #1
	ands r3, r5
	cmp r3, #0
	bne .L_08184e1e
	ldr r6, [sp, #140]
	movs r7, #160
	lsls r3, r2, #10
	lsls r7, r7, #3
	movs r2, #128
	movs r1, #160
	lsls r2, r2, #9
	adds r7, #108
	lsls r1, r1, #19
	subs r2, r2, r3
	adds r0, r6, r7
	adds r1, #192
	movs r3, #128
	bl ColorBuffer_ScaleFar
.L_08184e1e:
	ldr r0, [sp, #132]
	cmp r0, #0
	bne .L_08184ea0
	movs r1, #84
	adds r1, #255
	cmp r11, r1
	bne .L_08184eea
	movs r2, #1
	ldr r4, [sp, #164]
	movs r0, #240
	lsls r0, r0, #7
	str r2, [sp, #132]
	ldr r7, .L_08184e90
	ldr r6, .L_08184e94
	movs r3, #0
	adds r0, #60
	mov r9, r3
	adds r5, r4, r0
.L_08184e42:
	ldmia r5!, {r3}
	ldr r1, .L_08184e98
	ldrb r3, [r3, #16]
	mov r2, r9
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrh r0, [r3, #2]
	ldr r3, .L_08184e9c
	adds r0, r0, r1
	lsls r1, r2, #10
	movs r2, #128
	adds r1, r1, r3
	lsls r2, r2, #3
	mov lr, r6
	.2byte 0xf800
	movs r4, #1
	add r9, r4
	mov r0, r9
	cmp r0, #4
	bne .L_08184e42
	b .L_08184eea
.L_08184e6c:
	.4byte 0xfffffeff
.L_08184e70:
	.4byte 0xffffff00
.L_08184e74:
	.4byte 0xfffffee6
.L_08184e78:
	.4byte 0xfffffeb4
.L_08184e7c:
	.4byte 0xfffffe17
.L_08184e80:
	.4byte 0xfffffe1a
.L_08184e84:
	.4byte 0xfffffe26
.L_08184e88:
	.4byte 0x05000202
.L_08184e8c:
	.4byte 0xfffffdd6
.L_08184e90:
	.4byte ResourceTableEntries
.L_08184e94:
	.4byte IwramCopyWords
.L_08184e98:
	.4byte 0x06010000
.L_08184e9c:
	.4byte gMapCellBuffer
.L_08184ea0:
	movs r1, #225
	lsls r1, r1, #1
	cmp r11, r1
	bne .L_08184eea
	movs r2, #0
	ldr r3, [sp, #164]
	movs r0, #240
	lsls r0, r0, #7
	str r2, [sp, #132]
	ldr r4, .L_08185224
	ldr r7, .L_08185228
	ldr r6, .L_0818522c
	adds r0, #60
	mov r9, r2
	adds r5, r3, r0
.L_08184ebe:
	ldmia r5!, {r3}
	ldr r1, .L_08185230
	ldrb r3, [r3, #16]
	movs r2, #128
	lsls r3, r3, #2
	adds r3, r3, r4
	ldrh r0, [r3, #2]
	lsls r2, r2, #3
	adds r0, r0, r1
	str r4, [sp, #8]
	adds r1, r6, #0
	mov lr, r7
	.2byte 0xf800
	movs r3, #1
	movs r2, #128
	add r9, r3
	lsls r2, r2, #3
	mov r0, r9
	adds r6, r6, r2
	ldr r4, [sp, #8]
	cmp r0, #4
	bne .L_08184ebe
.L_08184eea:
	movs r1, #169
	lsls r1, r1, #1
	adds r1, #255
	cmp r11, r1
	ble .L_08184ef6
	b .L_0818507c
.L_08184ef6:
	ldr r3, .L_08185234
	add r7, sp, #228
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r2, #0
	str r3, [sp, #200]
	str r4, [sp, #204]
	movs r3, #0
	str r3, [r7, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r7, #4]
	ldr r3, [sp, #164]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #220
	mov r9, r2
	adds r6, r7, #0
	adds r5, r3, r4
.L_08184f1c:
	ldr r3, .L_08185238
	mov r0, r9
	ldrb r3, [r3, r0]
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #12
	adds r3, r3, r1
	str r3, [r6]
	ldr r3, .L_0818523c
	ldr r2, [sp, #136]
	ldrb r3, [r3, r0]
	ldr r4, .L_08185240
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r6, #8]
	cmp r3, r4
	ble .L_08184f52
	movs r0, #168
	lsls r0, r0, #16
	cmp r3, r0
	bgt .L_08184f52
	ldr r0, [r5]
	adds r1, r7, #0
	add r2, sp, #200
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_08184f52:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #4
	cmp r2, #28
	bne .L_08184f1c
	ldr r4, [sp, #164]
	movs r0, #240
	lsls r0, r0, #7
	movs r3, #0
	adds r0, #100
	mov r9, r3
	adds r6, r7, #0
	adds r5, r4, r0
.L_08184f6e:
	mov r1, r9
	lsrs r2, r1, #31
	add r2, r9
	asrs r2, r2, #1
	ldr r4, [sp, #128]
	lsls r3, r2, #1
	subs r3, r1, r3
	lsls r3, r3, #21
	movs r0, #144
	adds r3, r3, r4
	lsls r0, r0, #15
	adds r3, r3, r0
	str r3, [r6]
	ldr r1, [sp, #136]
	ldr r3, [sp, #124]
	lsls r2, r2, #21
	adds r2, r2, r1
	movs r4, #240
	ldr r0, .L_08185240
	adds r2, r2, r3
	lsls r4, r4, #14
	adds r2, r2, r4
	str r2, [r6, #8]
	cmp r2, r0
	ble .L_08184fb4
	movs r1, #168
	lsls r1, r1, #16
	cmp r2, r1
	bgt .L_08184fb4
	ldr r0, [r5]
	adds r1, r7, #0
	add r2, sp, #200
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_08184fb4:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r5, #4
	cmp r3, #5
	bne .L_08184f6e
	movs r4, #0
	ldr r0, [sp, #164]
	movs r1, #240
	mov r9, r4
	lsls r1, r1, #7
	ldr r4, .L_08185244
	adds r1, #120
	adds r6, r7, #0
	adds r5, r0, r1
.L_08184fd2:
	ldr r3, .L_08185248
	mov r2, r9
	ldrb r3, [r3, r2]
	ldr r0, [sp, #128]
	lsls r3, r3, #16
	movs r1, #132
	subs r3, r3, r0
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r6]
	ldrh r3, [r4]
	ldr r2, [sp, #136]
	ldr r0, [sp, #124]
	lsls r3, r3, #16
	adds r3, r3, r2
	movs r1, #232
	ldr r2, .L_08185240
	adds r3, r3, r0
	lsls r1, r1, #13
	adds r3, r3, r1
	adds r4, #2
	str r3, [r6, #8]
	cmp r3, r2
	ble .L_0818501a
	movs r0, #168
	lsls r0, r0, #16
	cmp r3, r0
	bgt .L_0818501a
	ldr r0, [r5]
	adds r1, r7, #0
	add r2, sp, #200
	movs r3, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	ldr r4, [sp, #8]
.L_0818501a:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #4
	cmp r2, #11
	bne .L_08184fd2
	ldr r3, .L_0818524c
	ldr r4, [sp, #164]
	movs r0, #240
	lsls r0, r0, #7
	str r3, [sp, #200]
	adds r0, #76
	movs r3, #0
	mov r9, r3
	adds r6, r7, #0
	adds r5, r4, r0
.L_0818503a:
	ldr r3, .L_08185250
	mov r1, r9
	ldrb r3, [r3, r1]
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, .L_08185254
	ldr r4, [sp, #136]
	ldrb r3, [r3, r1]
	ldr r0, .L_08185240
	lsls r3, r3, #16
	adds r3, r3, r4
	str r3, [r6, #8]
	cmp r3, r0
	ble .L_08185070
	movs r1, #168
	lsls r1, r1, #16
	cmp r3, r1
	bgt .L_08185070
	ldr r0, [r5]
	adds r1, r7, #0
	add r2, sp, #200
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_08185070:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r5, #4
	cmp r3, #6
	bne .L_0818503a
.L_0818507c:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #112]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08185258
	ldr r3, [sp, #192]
	movs r4, #0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_0818525c
	str r4, [sp, #108]
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #192]
	ldr r3, .L_08185260
	add r5, sp, #192
	str r3, [r0, #8]
	movs r3, #6
	str r5, [r0, #16]
	str r3, [r0]
	ldr r6, [sp, #112]
	movs r5, #128
	str r6, [r0, #12]
	mov r8, r0
	mov r9, r4
	lsls r5, r5, #8
.L_081850be:
	ldr r2, .L_08185264
	mov r7, r9
	lsls r3, r7, #1
	ldrh r1, [r2, r3]
	cmp r11, r1
	blt .L_08185130
	adds r3, r1, #0
	adds r3, #8
	cmp r11, r3
	bge .L_08185130
	ldr r3, .L_08185268
	mov r0, r11
	ldrb r2, [r3, r7]
	subs r3, r1, r0
	lsls r3, r3, #3
	mov r1, r8
	str r3, [sp, #108]
	str r3, [r1, #20]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #164]
	lsls r3, r3, #9
	movs r4, #224
	lsls r4, r4, #3
	adds r3, r2, r3
	adds r3, r3, r4
	str r3, [sp, #196]
	bl Func_08014de4
	ldr r3, .L_0818526c
	movs r2, #0
	ldrb r0, [r3, r7]
	ldr r3, .L_08185270
	subs r0, #48
	ldrb r1, [r3, r7]
	lsls r0, r0, #16
	subs r1, #44
	lsls r1, r1, #16
	bl Func_08015160
	movs r0, #0
	bl Func_080150e4
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, .L_08185274
	ldr r1, [sp, #112]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08185130:
	movs r6, #1
	add r9, r6
	mov r7, r9
	cmp r7, #7
	bne .L_081850be
	movs r0, #234
	adds r0, #255
	cmp r11, r0
	bne .L_0818515a
	ldr r3, [sp, #164]
	movs r1, #0
	mov r9, r1
	movs r2, #0
	adds r3, #24
.L_0818514c:
	movs r4, #1
	add r9, r4
	mov r5, r9
	str r2, [r3]
	adds r3, #28
	cmp r5, #3
	bne .L_0818514c
.L_0818515a:
	movs r6, #3
	ldr r5, [sp, #164]
	mov r10, r6
	ldr r6, .L_08185278
	adds r5, #84
.L_08185164:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_081851a6
	asrs r0, r0, #3
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #144]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r7, r1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #168]
	ldr r4, [sp, #156]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #63
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_081851a6:
	movs r7, #1
	add r10, r7
	mov r0, r10
	adds r5, #28
	cmp r0, #64
	bne .L_08185164
	ldr r3, [sp, #164]
	ldr r2, .L_0818527c
	movs r5, #238
	movs r4, #239
	lsls r5, r5, #7
	ldr r7, .L_08185280
	movs r1, #0
	lsls r4, r4, #7
	adds r5, #132
	mov r9, r1
	adds r0, r3, r4
	adds r1, r3, r5
	mov r12, r2
	movs r4, #0
	movs r6, #2
	movs r5, #50
.L_081851d2:
	mov r2, r12
	ldrh r3, [r4, r2]
	adds r2, r3, #0
	adds r2, #242
	cmp r11, r2
	bne .L_081851e4
	movs r2, #1
	str r2, [r0]
	str r7, [r1]
.L_081851e4:
	adds r3, #243
	cmp r11, r3
	bne .L_081851ee
	str r6, [r0]
	str r5, [r1]
.L_081851ee:
	movs r3, #1
	add r9, r3
	mov r2, r9
	adds r4, #2
	cmp r2, #17
	bne .L_081851d2
	ldr r3, .L_08185284
	add r3, r11
	cmp r3, #55
	bhi .L_081852a0
	movs r2, #1
	mov r3, r11
	ands r3, r2
	cmp r3, #0
	beq .L_08185288
	ldr r4, [sp, #164]
	movs r5, #239
	lsls r5, r5, #7
	adds r3, r4, r5
	movs r6, #238
	str r2, [r3]
	lsls r6, r6, #7
	ldr r3, .L_08185280
	adds r6, #132
	adds r2, r4, r6
	b .L_0818529e
	.2byte 0x0000
.L_08185224:
	.4byte ResourceTableEntries
.L_08185228:
	.4byte IwramCopyWords
.L_0818522c:
	.4byte Data_02011000
.L_08185230:
	.4byte 0x06010000
.L_08185234:
	.4byte Data_08196ef8
.L_08185238:
	.4byte Data_081997b0
.L_0818523c:
	.4byte Data_081997cc
.L_08185240:
	.4byte 0x000fffff
.L_08185244:
	.4byte Data_081997f4
.L_08185248:
	.4byte Data_081997e8
.L_0818524c:
	.4byte 0xfffeffff
.L_08185250:
	.4byte Data_0819980a
.L_08185254:
	.4byte Data_08199810
.L_08185258:
	.4byte 0xffffff00
.L_0818525c:
	.4byte 0xffff00ff
.L_08185260:
	.4byte Data_0819931c
.L_08185264:
	.4byte Data_08199816
.L_08185268:
	.4byte Data_08199832
.L_0818526c:
	.4byte Data_08199824
.L_08185270:
	.4byte Data_0819982b
.L_08185274:
	.4byte Data_081991e0
.L_08185278:
	.4byte Data_08197410
.L_0818527c:
	.4byte Data_0819983a
.L_08185280:
	.4byte 0x1f1f1f1f
.L_08185284:
	.4byte 0xfffffde7
.L_08185288:
	ldr r7, [sp, #164]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r7, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #50
.L_0818529e:
	str r3, [r2]
.L_081852a0:
	movs r2, #162
	lsls r2, r2, #1
	cmp r11, r2
	bne .L_081852ae
	movs r0, #206
	bl Audio_PlayCue
.L_081852ae:
	movs r3, #140
	lsls r3, r3, #2
	cmp r11, r3
	bne .L_081852bc
	movs r0, #220
	bl Audio_PlayCue
.L_081852bc:
	movs r4, #128
	lsls r4, r4, #2
	adds r4, #90
	cmp r11, r4
	bne .L_081852cc
	movs r0, #145
	bl Audio_PlayCue
.L_081852cc:
	movs r3, #5
	add r5, sp, #192
	strb r3, [r5]
	adds r6, r5, #0
	movs r3, #7
	str r6, [sp, #52]
	mov r0, r8
	strb r3, [r6, #1]
	movs r3, #6
	ldr r7, [sp, #108]
	str r3, [r0]
	ldr r3, .L_081853f0
	str r7, [r0, #20]
	str r3, [r0, #8]
	str r6, [r0, #16]
	ldr r3, .L_081853f4
	ldr r1, [sp, #112]
	add r3, r11
	str r1, [r0, #12]
	cmp r3, #130
	bls .L_081852f8
	b .L_0818556a
.L_081852f8:
	movs r2, #0
	str r2, [sp, #108]
	str r2, [sp, #36]
	ldr r4, [sp, #164]
	mov r9, r2
.L_08185302:
	movs r3, #13
	mov r5, r9
	muls r5, r3
	ldr r0, [r4, #24]
	ldr r2, .L_081853f8
	adds r3, r5, #0
	adds r3, r3, r0
	lsls r3, r3, #1
	ldrh r3, [r2, r3]
	ldr r6, .L_081853fc
	movs r7, #234
	adds r3, r3, r6
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r7, #255
	adds r3, r3, r7
	cmp r11, r3
	beq .L_0818532a
	b .L_081854a6
.L_0818532a:
	ldr r3, [sp, #36]
	movs r1, #0
	str r1, [sp, #104]
	ldr r2, .L_08185400
	adds r1, r3, r0
	lsls r3, r1, #1
	ldrsh r2, [r2, r3]
	cmp r2, #0
	beq .L_08185350
	ldr r3, .L_08185404
	ldrb r3, [r3, r1]
	lsls r3, r3, #3
	str r3, [r4]
	ldr r3, .L_08185408
	ldrb r3, [r3, r1]
	lsls r3, r3, #3
	str r3, [r4, #4]
	lsls r3, r2, #8
	str r3, [r4, #8]
.L_08185350:
	ldr r6, [sp, #36]
	ldr r3, .L_0818540c
	adds r2, r0, r6
	ldrb r3, [r3, r2]
	lsls r2, r2, #1
	str r3, [r4, #16]
	ldr r3, .L_08185410
	ldrsh r3, [r3, r2]
	str r3, [r4, #20]
	ldr r3, .L_08185414
	ldrsh r2, [r3, r2]
	adds r3, r0, #1
	str r3, [r4, #24]
	movs r3, #1
	negs r3, r3
	str r2, [r4, #12]
	cmp r2, r3
	bne .L_08185376
	b .L_08185556
.L_08185376:
	ldr r3, .L_08185418
	ldr r5, [sp, #144]
	ldrh r3, [r3, #18]
	ldr r2, [r4]
	movs r7, #10
	movs r0, #20
	adds r1, r5, r3
	cmp r2, #0
	bge .L_0818538a
	adds r2, #7
.L_0818538a:
	ldr r3, [sp, #128]
	movs r6, #128
	lsls r6, r6, #12
	subs r5, r6, r3
	lsrs r3, r5, #31
	asrs r2, r2, #3
	adds r3, r5, r3
	asrs r3, r3, #17
	subs r2, #5
	adds r6, r2, r3
	ldr r3, [r4, #4]
	cmp r3, #0
	bge .L_081853a6
	adds r3, #7
.L_081853a6:
	asrs r3, r3, #3
	subs r3, #10
	mov r12, r3
	ldr r2, [sp, #136]
	ldr r3, [sp, #124]
	str r7, [sp, #0]
	adds r2, r2, r3
	asrs r2, r2, #16
	add r12, r2
	str r0, [sp, #4]
	adds r2, r6, #0
	str r4, [sp, #8]
	ldr r0, [sp, #168]
	mov r3, r12
	ldr r6, [sp, #156]
	mov lr, r6
	.2byte 0xf800
	mov r7, r9
	ldr r4, [sp, #8]
	cmp r7, #1
	beq .L_081853e8
	cmp r7, #1
	bgt .L_081853da
	cmp r7, #0
	beq .L_081853e2
	b .L_08185426
.L_081853da:
	mov r0, r9
	cmp r0, #2
	beq .L_0818541c
	b .L_08185426
.L_081853e2:
	movs r0, #186
	str r4, [sp, #8]
	b .L_08185420
.L_081853e8:
	movs r0, #187
	str r4, [sp, #8]
	b .L_08185420
	.2byte 0x0000
.L_081853f0:
	.4byte Data_08199268
.L_081853f4:
	.4byte 0xfffffe17
.L_081853f8:
	.4byte Data_081998a4
.L_081853fc:
	.4byte 0xfffffe14
.L_08185400:
	.4byte Data_08199916
.L_08185404:
	.4byte Data_0819985c
.L_08185408:
	.4byte Data_08199880
.L_0818540c:
	.4byte Data_081998f2
.L_08185410:
	.4byte Data_0819995e
.L_08185414:
	.4byte Data_081999a6
.L_08185418:
	.4byte Data_08197410
.L_0818541c:
	movs r0, #222
	str r4, [sp, #8]
.L_08185420:
	bl Audio_PlayCue
	ldr r4, [sp, #8]
.L_08185426:
	lsrs r3, r5, #31
	adds r3, r5, r3
	ldr r7, [sp, #164]
	asrs r3, r3, #1
	str r3, [sp, #100]
	movs r1, #3
	mov r10, r1
	adds r7, #84
.L_08185436:
	ldr r3, [r7, #24]
	cmp r3, #0
	bge .L_0818549a
	ldr r3, [r4]
	ldr r2, [sp, #100]
	lsls r3, r3, #13
	adds r3, r3, r2
	str r3, [r7]
	ldr r5, [sp, #136]
	ldr r3, [r4, #4]
	ldr r6, [sp, #124]
	lsls r3, r3, #13
	adds r3, r3, r5
	adds r3, r3, r6
	str r3, [r7, #4]
	str r4, [sp, #8]
	bl Random16
	adds r6, r0, #0
	bl Random16
	movs r5, #255
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #16
	str r3, [r7, #24]
	ldr r0, [sp, #104]
	ldr r4, [sp, #8]
	adds r0, #1
	str r0, [sp, #104]
	cmp r0, #10
	beq .L_081854a6
.L_0818549a:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r7, #28
	cmp r2, #64
	bne .L_08185436
.L_081854a6:
	ldr r3, [r4, #12]
	movs r5, #1
	negs r5, r5
	cmp r3, r5
	beq .L_08185556
	ldr r6, [sp, #108]
	mov r7, r8
	str r6, [r7, #20]
	ldr r0, [sp, #164]
	lsls r3, r3, #12
	movs r1, #156
	adds r3, r0, r3
	lsls r1, r1, #6
	adds r3, r3, r1
	str r3, [sp, #196]
	str r4, [sp, #8]
	bl Func_08014de4
	ldr r4, [sp, #8]
	ldr r3, [r4]
	cmp r3, #0
	bge .L_081854d4
	adds r3, #7
.L_081854d4:
	ldr r6, [sp, #128]
	movs r5, #128
	lsls r5, r5, #12
	subs r2, r5, r6
	lsrs r1, r2, #31
	adds r2, r2, r1
	asrs r3, r3, #3
	ldr r1, [r4, #4]
	subs r3, #64
	lsls r3, r3, #16
	asrs r2, r2, #1
	adds r0, r3, r2
	cmp r1, #0
	bge .L_081854f2
	adds r1, #7
.L_081854f2:
	ldr r7, [sp, #136]
	asrs r1, r1, #3
	ldr r2, [sp, #124]
	subs r1, #64
	lsls r1, r1, #16
	adds r1, r1, r7
	adds r1, r1, r2
	movs r2, #0
	str r4, [sp, #8]
	movs r5, #128
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #9
	lsls r5, r5, #8
	adds r1, r2, #0
	adds r0, r5, #0
	bl Func_080151e4
	ldr r4, [sp, #8]
	ldr r0, [r4, #8]
	bl Func_080150e4
	movs r1, #167
	movs r0, #128
	lsls r1, r1, #9
	lsls r0, r0, #11
	adds r1, #32
	adds r2, r5, #0
	bl Func_080151e4
	movs r2, #4
	ldr r0, .L_081856c8
	ldr r1, [sp, #112]
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
	ldr r4, [sp, #8]
	ldr r2, [r4, #16]
	ldr r3, [r4, #4]
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [r4, #20]
	str r3, [r4, #4]
	ldr r3, [r4, #8]
	lsls r2, r2, #1
	adds r3, r3, r2
	str r3, [r4, #8]
.L_08185556:
	ldr r3, [sp, #36]
	movs r5, #1
	add r9, r5
	adds r3, #12
	mov r6, r9
	str r3, [sp, #36]
	adds r4, #28
	cmp r6, #3
	beq .L_0818556a
	b .L_08185302
.L_0818556a:
	movs r7, #128
	lsls r7, r7, #2
	adds r7, #90
	cmp r11, r7
	bne .L_08185582
	movs r1, #128
	ldr r3, .L_081856cc
	ldr r0, [sp, #168]
	lsls r1, r1, #7
	ldr r2, .L_081856d0
	mov lr, r3
	.2byte 0xf800
.L_08185582:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #118
	cmp r11, r0
	bne .L_0818559a
	movs r1, #128
	ldr r3, .L_081856cc
	ldr r0, [sp, #168]
	lsls r1, r1, #7
	ldr r2, .L_081856d4
	mov lr, r3
	.2byte 0xf800
.L_0818559a:
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #122
	cmp r11, r1
	bne .L_081855b2
	movs r1, #128
	ldr r3, .L_081856cc
	ldr r0, [sp, #168]
	lsls r1, r1, #7
	ldr r2, .L_081856d4
	mov lr, r3
	.2byte 0xf800
.L_081855b2:
	ldr r3, .L_081856d8
	add r3, r11
	cmp r3, #43
	bhi .L_08185656
	mov r3, r11
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_081855ee
	bl Random16
	movs r3, #252
	lsls r3, r3, #6
	ldr r5, [sp, #164]
	adds r3, #255
	movs r4, #192
	lsls r4, r4, #7
	ands r3, r0
	adds r3, r3, r4
	str r3, [r5, #4]
	bl Random16
	movs r1, #3
	bl Math_ModU
	movs r6, #128
	lsls r0, r0, #12
	lsls r6, r6, #6
	adds r0, r0, r6
	str r0, [r5, #8]
.L_081855ee:
	ldr r7, [sp, #164]
	movs r0, #224
	ldr r3, [r7, #8]
	lsls r0, r0, #3
	adds r3, r7, r3
	adds r3, r3, r0
	str r3, [sp, #196]
	bl Func_08014de4
	ldr r2, [sp, #128]
	movs r1, #128
	lsls r1, r1, #12
	subs r0, r1, r2
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [sp, #136]
	movs r6, #128
	movs r4, #204
	lsls r4, r4, #16
	lsls r6, r6, #11
	asrs r0, r0, #1
	adds r1, r3, r4
	adds r0, r0, r6
	movs r2, #0
	bl Func_08015160
	movs r5, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r5, r5, #8
	adds r1, r2, #0
	adds r0, r5, #0
	bl Func_080151e4
	ldr r0, [r7, #4]
	bl Func_080150e4
	movs r1, #167
	lsls r1, r1, #9
	adds r0, r6, #0
	adds r1, #32
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, .L_081856c8
	ldr r1, [sp, #112]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08185656:
	ldr r3, .L_081856dc
	add r3, r11
	cmp r3, #15
	bls .L_08185668
	ldr r3, .L_081856e0
	add r3, r11
	cmp r3, #3
	bls .L_08185668
	b .L_081857a0
.L_08185668:
	movs r5, #1
	str r5, [sp, #96]
	bl Random16
	movs r1, #3
	bl Math_ModU
	movs r7, #178
	movs r6, #160
	lsls r7, r7, #1
	lsls r6, r6, #11
	adds r7, #255
	mov r10, r6
	str r0, [sp, #92]
	cmp r11, r7
	ble .L_0818569c
	movs r3, #241
	lsls r3, r3, #8
	adds r3, #149
	mov r0, r11
	muls r0, r3
	ldr r1, .L_081856e4
	adds r3, r0, #0
	subs r3, r6, r3
	adds r1, r1, r3
	mov r10, r1
.L_0818569c:
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r10, r2
	bgt .L_081856ac
	movs r3, #128
	lsls r3, r3, #9
	mov r10, r3
.L_081856ac:
	movs r4, #179
	lsls r4, r4, #1
	adds r4, #255
	cmp r11, r4
	bgt .L_081856f6
	movs r5, #175
	lsls r5, r5, #1
	adds r5, #255
	cmp r11, r5
	ble .L_081856e8
	movs r6, #3
	str r6, [sp, #96]
	b .L_081856f6
	.2byte 0x0000
.L_081856c8:
	.4byte Data_081991d0
.L_081856cc:
	.4byte IwramFillWords
.L_081856d0:
	.4byte 0x3f3f3f3f
.L_081856d4:
	.4byte 0x1f1f1f1f
.L_081856d8:
	.4byte 0xfffffdd8
.L_081856dc:
	.4byte 0xfffffda6
.L_081856e0:
	.4byte 0xfffffd8a
.L_081856e4:
	.4byte 0x02418834
.L_081856e8:
	movs r7, #174
	lsls r7, r7, #1
	adds r7, #255
	cmp r11, r7
	ble .L_081856f6
	movs r0, #2
	str r0, [sp, #96]
.L_081856f6:
	movs r3, #6
	mov r1, r8
	str r3, [r1]
	ldr r2, [sp, #108]
	movs r3, #0
	str r2, [r1, #20]
	ldr r4, [sp, #96]
	mov r9, r3
	cmp r4, #0
	beq .L_081857a0
	ldr r5, [sp, #164]
	movs r7, #128
	movs r6, #128
	lsls r7, r7, #9
	lsls r6, r6, #8
.L_08185714:
	bl Random16
	movs r1, #8
	bl Math_ModU
	movs r1, #4
	subs r0, r0, r1
	lsls r0, r0, #16
	str r0, [r5]
	ldr r2, [sp, #136]
	movs r4, #200
	lsls r4, r4, #16
	adds r3, r2, r4
	str r3, [r5, #4]
	ldr r0, [sp, #92]
	movs r1, #3
	add r0, r9
	bl Math_Mod
	movs r1, #128
	lsls r1, r1, #6
	lsls r0, r0, #12
	adds r0, r0, r1
	str r0, [r5, #8]
	ldr r2, [sp, #164]
	movs r3, #224
	lsls r3, r3, #3
	adds r0, r2, r0
	adds r0, r0, r3
	str r0, [sp, #196]
	bl Func_08014de4
	ldr r1, [r5, #4]
	adds r0, r7, #0
	movs r2, #0
	bl Func_08015160
	mov r0, r10
	adds r1, r7, #0
	adds r2, r6, #0
	bl Func_080151e4
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_08015160
	movs r1, #128
	lsls r1, r1, #10
	adds r2, r7, #0
	adds r0, r6, #0
	bl Func_080151e4
	adds r0, r6, #0
	bl Func_080150e4
	ldr r0, .L_08185b04
	ldr r1, [sp, #112]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
	ldr r0, [sp, #96]
	movs r4, #1
	add r9, r4
	adds r5, #28
	cmp r9, r0
	bne .L_08185714
.L_081857a0:
	movs r1, #138
	lsls r1, r1, #2
	cmp r11, r1
	bne .L_081857b4
	ldr r0, .L_08185b08
	ldr r1, .L_08185b0c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
.L_081857b4:
	movs r2, #148
	lsls r2, r2, #1
	adds r2, #255
	cmp r11, r2
	bne .L_081857e2
	ldr r3, [sp, #164]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r3, r4
	ldr r0, .L_08185b10
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r5, [sp, #164]
	movs r6, #184
	lsls r6, r6, #5
	ldr r0, .L_08185b14
	adds r1, r5, r6
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
.L_081857e2:
	movs r7, #128
	lsls r7, r7, #2
	adds r7, #90
	cmp r11, r7
	bne .L_081857f8
	ldr r0, .L_08185b18
	ldr r1, .L_08185b0c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
.L_081857f8:
	ldr r3, .L_08185b1c
	add r3, r11
	cmp r3, #6
	bhi .L_0818580a
	ldr r0, [sp, #136]
	movs r1, #128
	lsls r1, r1, #13
	adds r0, r0, r1
	str r0, [sp, #136]
.L_0818580a:
	ldr r0, .L_08185b20
	add r0, r11
	cmp r0, #58
	bls .L_08185814
	b .L_08185a34
.L_08185814:
	movs r3, #136
	lsls r3, r3, #5
	adds r3, #72
	adds r7, r0, #0
	muls r7, r3
	ldr r3, .L_08185b24
	movs r2, #16
	negs r2, r2
	str r2, [sp, #108]
	cmp r7, r3
	ble .L_0818582c
	ldr r7, .L_08185b24
.L_0818582c:
	add r4, sp, #192
	adds r5, r4, #0
	movs r3, #6
	strb r3, [r4]
	mov r0, r8
	str r5, [sp, #52]
	strb r3, [r5, #1]
	movs r3, #7
	ldr r6, [sp, #108]
	str r3, [r0]
	ldr r3, .L_08185b28
	str r6, [r0, #20]
	str r5, [r0, #16]
	str r3, [r0, #8]
	ldr r1, [sp, #112]
	movs r4, #184
	str r1, [r0, #12]
	ldr r2, [sp, #164]
	lsls r4, r4, #5
	adds r3, r2, r4
	str r3, [r5, #4]
	bl Func_08014de4
	ldr r6, [sp, #128]
	movs r5, #128
	lsls r5, r5, #12
	subs r5, r5, r6
	ldr r2, [sp, #136]
	lsrs r0, r5, #31
	adds r0, r5, r0
	movs r1, #160
	movs r3, #230
	lsls r3, r3, #16
	lsls r1, r1, #11
	asrs r0, r0, #1
	adds r0, r0, r1
	adds r1, r2, r3
	movs r2, #0
	str r5, [sp, #88]
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	movs r0, #146
	lsls r0, r0, #7
	adds r0, #80
	bl SceneTransform_ApplyPitch
	mov r4, r11
	negs r5, r4
	lsls r5, r5, #10
	adds r0, r5, #0
	bl Func_080150e4
	adds r0, r7, #0
	bl Func_0801521c
	movs r0, #0
	ldr r1, .L_08185b2c
	movs r2, #0
	bl Func_08015160
	ldr r0, .L_08185b30
	ldr r1, [sp, #112]
	movs r2, #4
	bl Func_08196958
	movs r1, #240
	lsls r1, r1, #12
	movs r2, #0
	movs r0, #0
	bl Func_08015160
	mov r0, r8
	bl Func_08196a7c
	ldr r6, [sp, #164]
	movs r0, #224
	ldr r1, [sp, #52]
	lsls r0, r0, #3
	adds r3, r6, r0
	str r3, [r1, #4]
	ldr r3, [sp, #28]
	movs r4, #168
	lsls r4, r4, #6
	movs r6, #168
	movs r2, #0
	adds r4, #170
	lsls r6, r6, #6
	mov r9, r2
	mov r10, r3
	adds r5, r5, r4
	adds r6, #170
.L_081858f0:
	bl Func_08014e38
	adds r0, r6, #0
	bl Func_080150e4
	adds r0, r5, #0
	bl Trig_Cos
	movs r3, #24
	lsls r0, r0, #3
	asrs r0, r0, #16
	negs r3, r3
	subs r3, r3, r0
	mov r0, r8
	str r3, [r0, #20]
	ldr r1, .L_08185b34
	movs r0, #0
	movs r2, #0
	bl Func_08015160
	movs r1, #162
	lsls r1, r1, #1
	adds r1, #255
	cmp r11, r1
	ble .L_08185938
	ldr r0, .L_08185b38
	movs r2, #154
	add r0, r10
	lsls r2, r2, #7
	cmp r0, r2
	ble .L_08185932
	movs r0, #154
	lsls r0, r0, #7
.L_08185932:
	negs r0, r0
	bl SceneTransform_ApplyPitch
.L_08185938:
	ldr r1, [sp, #112]
	movs r2, #4
	ldr r0, .L_08185b3c
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
	bl Func_08014ea8
	movs r3, #170
	movs r4, #1
	lsls r3, r3, #7
	add r9, r4
	adds r3, #85
	mov r0, r9
	adds r5, r5, r3
	adds r6, r6, r3
	cmp r0, #3
	bne .L_081858f0
	movs r2, #167
	lsls r2, r2, #1
	adds r2, #255
	cmp r11, r2
	ble .L_0818597c
	movs r3, #172
	lsls r3, r3, #6
	adds r3, #52
	mov r1, r11
	muls r1, r3
	ldr r4, .L_08185b40
	adds r3, r1, #0
	adds r3, r7, r3
	adds r7, r3, r4
.L_0818597c:
	cmp r7, #0
	ble .L_08185a34
	movs r5, #173
	lsls r5, r5, #1
	adds r5, #255
	cmp r11, r5
	bgt .L_08185a3e
	movs r6, #16
	negs r6, r6
	str r6, [sp, #108]
	cmp r11, r2
	ble .L_081859a4
	mov r0, r11
	movs r1, #128
	lsls r3, r0, #1
	lsls r1, r1, #3
	subs r3, r6, r3
	adds r1, #156
	adds r1, r3, r1
	str r1, [sp, #108]
.L_081859a4:
	ldr r2, [sp, #108]
	movs r3, #64
	negs r3, r3
	cmp r2, r3
	ble .L_08185a34
	ldr r3, .L_08185b0c
	movs r5, #149
	mov r4, r8
	lsls r5, r5, #1
	str r2, [r4, #20]
	adds r5, #255
	str r3, [sp, #196]
	bl Func_08014de4
	cmp r11, r5
	ble .L_081859ec
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #42
	mov r6, r11
	subs r3, r3, r6
	lsls r2, r3, #5
	subs r2, r2, r3
	lsls r2, r2, #2
	adds r2, r2, r3
	lsls r3, r2, #4
	ldr r0, .L_08185b44
	subs r3, r3, r2
	lsls r1, r3, #6
	cmp r1, r0
	bge .L_081859e4
	ldr r1, .L_08185b44
.L_081859e4:
	movs r0, #0
	movs r2, #0
	bl Func_08015160
.L_081859ec:
	ldr r1, [sp, #88]
	ldr r3, [sp, #136]
	lsrs r0, r1, #31
	adds r0, r1, r0
	movs r2, #160
	movs r4, #220
	lsls r4, r4, #16
	lsls r2, r2, #11
	asrs r0, r0, #1
	adds r1, r3, r4
	adds r0, r0, r2
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #9
	movs r1, #250
	adds r0, r2, #0
	lsls r1, r1, #7
	mov r5, r11
	bl Func_080151e4
	lsls r0, r5, #11
	bl Func_080150e4
	adds r0, r7, #0
	bl Func_0801521c
	ldr r0, .L_08185b30
	ldr r1, [sp, #112]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08185a34:
	movs r6, #173
	lsls r6, r6, #1
	adds r6, #255
	cmp r11, r6
	ble .L_08185ab8
.L_08185a3e:
	movs r0, #178
	lsls r0, r0, #1
	movs r7, #0
	adds r0, #255
	str r7, [sp, #108]
	cmp r11, r0
	ble .L_08185a5a
	mov r1, r11
	lsls r3, r1, #3
	movs r2, #153
	negs r3, r3
	lsls r2, r2, #5
	adds r2, r3, r2
	str r2, [sp, #108]
.L_08185a5a:
	ldr r3, [sp, #108]
	movs r4, #64
	negs r4, r4
	cmp r3, r4
	ble .L_08185ab8
	mov r5, r8
	str r3, [r5, #20]
	ldr r3, .L_08185b48
	movs r2, #7
	str r3, [r5, #8]
	str r2, [r5]
	ldr r6, [sp, #52]
	ldr r3, .L_08185b0c
	movs r5, #128
	str r3, [r6, #4]
	add r3, sp, #192
	strb r2, [r3]
	lsls r5, r5, #9
	strb r2, [r6, #1]
	bl Func_08014de4
	adds r0, r5, #0
	ldr r1, .L_08185b34
	movs r2, #0
	bl Func_08015160
	movs r1, #250
	lsls r1, r1, #7
	adds r2, r5, #0
	adds r0, r5, #0
	mov r7, r11
	bl Func_080151e4
	lsls r0, r7, #10
	bl Func_080150e4
	ldr r0, [sp, #32]
	bl Func_0801521c
	ldr r0, .L_08185b30
	ldr r1, [sp, #112]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08185ab8:
	mov r0, r8
	bl Sys_Free
	ldr r0, [sp, #112]
	bl Sys_Free
	ldr r2, .L_08185b4c
	add r2, r11
	cmp r2, #51
	bhi .L_08185b6e
	lsrs r3, r2, #31
	ldr r6, .L_08185b50
	adds r3, r2, r3
	movs r0, #150
	asrs r3, r3, #1
	movs r2, #90
	lsls r0, r0, #2
	add r6, r11
	subs r5, r2, r3
	cmp r11, r0
	ble .L_08185aec
	mov r1, r11
	ldr r2, .L_08185b54
	lsls r3, r1, #4
	adds r3, r5, r3
	adds r5, r3, r2
.L_08185aec:
	cmp r5, #63
	bgt .L_08185af2
	movs r5, #64
.L_08185af2:
	cmp r6, #38
	bgt .L_08185b60
	ldr r0, .L_08185b58
	adds r1, r6, #0
	ldr r2, .L_08185b5c
	bl Func_0815b434
	b .L_08185b62
	.2byte 0x0000
.L_08185b04:
	.4byte Data_081991d0
.L_08185b08:
	.4byte 0x000000b4
.L_08185b0c:
	.4byte Data_02012000
.L_08185b10:
	.4byte 0x000000ee
.L_08185b14:
	.4byte 0x000000ef
.L_08185b18:
	.4byte 0x000000c2
.L_08185b1c:
	.4byte 0xfffffda6
.L_08185b20:
	.4byte 0xfffffdd8
.L_08185b24:
	.4byte 0x00022538
.L_08185b28:
	.4byte Data_08199340
.L_08185b2c:
	.4byte 0xfff10000
.L_08185b30:
	.4byte Data_081991e0
.L_08185b34:
	.4byte 0xfff00000
.L_08185b38:
	.4byte 0xfff64ce0
.L_08185b3c:
	.4byte Data_081991f0
.L_08185b40:
	.4byte 0xff9c6e28
.L_08185b44:
	.4byte 0xfff32ec0
.L_08185b48:
	.4byte Data_08199364
.L_08185b4c:
	.4byte 0xfffffdd6
.L_08185b50:
	.4byte 0xfffffdd7
.L_08185b54:
	.4byte 0xffffda70
.L_08185b58:
	.4byte gMapCellBuffer
.L_08185b5c:
	.4byte 0x00017530
.L_08185b60:
	movs r6, #38
.L_08185b62:
	ldr r0, .L_08185ccc
	movs r1, #62
	adds r2, r5, #0
	adds r3, r6, #0
	bl Func_0818caa8
.L_08185b6e:
	ldr r4, [sp, #164]
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #232
	adds r3, r4, r5
	movs r6, #1
	str r6, [r3]
	movs r0, #1
	bl WaitFrames
	movs r0, #188
	ldr r7, [sp, #32]
	ldr r1, [sp, #28]
	lsls r0, r0, #6
	movs r2, #137
	adds r0, #160
	lsls r2, r2, #3
	adds r7, r7, r0
	adds r1, r1, r2
	str r7, [sp, #32]
	str r1, [sp, #28]
	movs r3, #1
	add r11, r3
	.global Func_08185b9c
	.thumb_func
Func_08185b9c:
	movs r4, #191
	lsls r4, r4, #1
	adds r4, #255
	cmp r11, r4
	beq .L_08185bc8
	mov r5, r11
	cmp r5, #15
	bgt .L_08185bb8
	lsls r2, r5, #1
	subs r2, #30
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_08164b2c
.L_08185bb8:
	ldr r3, .L_08185cd0
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08185bc8
	bl Func_081848ec
.L_08185bc8:
	ldr r7, [sp, #164]
	movs r0, #238
	lsls r0, r0, #7
	movs r6, #0
	adds r0, #220
	mov r9, r6
	adds r5, r7, r0
.L_08185bd6:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #50
	bne .L_08185bd6
	bl Func_08020388
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r3, [sp, #140]
	movs r5, #240
	str r0, [r3, #84]
	ldr r4, [sp, #164]
	lsls r5, r5, #7
	adds r5, #240
	adds r3, r4, r5
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r7, [sp, #172]
	movs r6, #0
	ldr r3, [r7, #20]
	mov r9, r6
	cmp r3, #0
	beq .L_08185c44
	subs r6, #1
	movs r7, #0
	movs r5, #36
.L_08185c16:
	ldr r1, [sp, #172]
	adds r3, r6, #0
	ldrsh r0, [r5, r1]
	movs r1, #1
	adds r2, r6, #0
	str r7, [sp, #0]
	bl Func_0814cd48
	ldr r3, [sp, #172]
	movs r1, #0
	ldrsh r0, [r5, r3]
	adds r2, r6, #0
	adds r3, r6, #0
	str r7, [sp, #0]
	bl Func_0814cd48
	ldr r1, [sp, #172]
	movs r0, #1
	ldr r3, [r1, #20]
	add r9, r0
	adds r5, #2
	cmp r9, r3
	bne .L_08185c16
.L_08185c44:
	bl Func_08014c4c
	bl Func_0814cca8
	ldr r2, .L_08185cd4
	movs r3, #240
	str r3, [r2, #16]
	ldr r2, [sp, #164]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #180
	adds r3, r2, r4
	movs r2, #0
	str r2, [r3]
	ldr r5, [sp, #164]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #184
	adds r3, r5, r6
	str r2, [r3]
	ldr r2, .L_08185cd8
	movs r3, #32
	strh r3, [r2, #6]
	ldr r1, .L_08185cdc
	movs r0, #1
	movs r2, #0
	bl Func_08118040
	movs r2, #16
	negs r2, r2
	adds r1, r2, #0
	movs r0, #0
	bl Func_08164b2c
	ldr r3, .L_08185cc8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	movs r7, #240
	strh r3, [r2]
	ldr r0, .L_08185ce0
	ldr r1, .L_08185ccc
	movs r2, #1
	movs r3, #1
	lsls r7, r7, #4
	bl Func_08157cf4
	adds r1, r5, r7
	movs r2, #1
	ldr r0, .L_08185ce4
	movs r3, #0
	bl Func_08157cf4
	movs r1, #216
	lsls r1, r1, #5
	adds r1, #86
	adds r0, r5, r1
	movs r1, #128
	ldr r3, .L_08185ce8
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	movs r3, #216
	lsls r3, r3, #5
	b .L_08185cec
	.2byte 0x0000
.L_08185cc8:
	.4byte 0x00001010
.L_08185ccc:
	.4byte gMapCellBuffer
.L_08185cd0:
	.4byte gInput
.L_08185cd4:
	.4byte gCameraSceneParameters
.L_08185cd8:
	.4byte Data_03001120
.L_08185cdc:
	.4byte 0x00000075
.L_08185ce0:
	.4byte 0x00000188
.L_08185ce4:
	.4byte 0x00000192
.L_08185ce8:
	.4byte IwramClearWords
.L_08185cec:
	movs r2, #0
	adds r3, #90
	mov r9, r2
	movs r7, #0
	mov r12, r3
.L_08185cf6:
	ldr r0, [sp, #164]
	mov r4, r9
	lsls r3, r4, #12
	movs r6, #0
	lsls r5, r7, #6
	adds r4, r3, r0
.L_08185d02:
	ldr r3, .L_08185e30
	mov r2, r12
	adds r1, r4, r2
	movs r0, #0
	adds r2, r5, r3
.L_08185d0c:
	ldrb r3, [r2]
	adds r0, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r0, #24
	bne .L_08185d0c
	adds r6, #1
	adds r5, #24
	adds r4, #32
	cmp r6, #120
	bne .L_08185d02
	movs r4, #1
	add r9, r4
	mov r5, r9
	adds r7, #45
	cmp r5, #4
	bne .L_08185cf6
	ldr r0, .L_08185e34
	ldr r1, .L_08185e30
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_08185e38
	ldr r1, .L_08185e3c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_08185e40
	ldr r1, .L_08185e44
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r7, [sp, #164]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	movs r6, #0
	adds r2, r7, r0
	movs r3, #2
	adds r1, #132
	str r6, [sp, #84]
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #50
	str r3, [r2]
	ldr r3, .L_08185e48
	movs r2, #3
	ldr r3, [r3, #12]
	mov r11, r6
	ands r3, r2
	cmp r3, #0
	beq .L_08185d80
	b .L_08186536
.L_08185d80:
	mov r2, sp
	adds r2, #184
	str r2, [sp, #56]
	str r6, [sp, #20]
.L_08185d88:
	mov r3, r11
	cmp r3, #0
	bne .L_08185dd8
	movs r1, #128
	ldr r3, .L_08185e4c
	ldr r0, [sp, #168]
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	ldr r4, .L_08185e50
	movs r2, #16
	negs r2, r2
	adds r1, r2, #0
	movs r0, #0
	str r4, [sp, #84]
	bl Func_08164b2c
	ldr r5, [sp, #164]
	movs r6, #239
	movs r7, #238
	lsls r6, r6, #7
	lsls r7, r7, #7
	adds r2, r5, r6
	movs r3, #2
	adds r7, #132
	str r3, [r2]
	adds r2, r5, r7
	movs r3, #50
	str r3, [r2]
	ldr r0, .L_08185e54
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08185e58
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08185dd8:
	ldr r0, [sp, #84]
	ldr r1, .L_08185e5c
	cmp r0, r1
	bgt .L_08185de8
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r0, r2
	str r0, [sp, #84]
.L_08185de8:
	mov r0, r11
	subs r0, #96
	cmp r0, #23
	bhi .L_08185e0c
	mov r2, r11
	movs r1, #16
	subs r2, #112
	negs r0, r0
	negs r1, r1
	bl Func_08164b2c
	movs r4, #238
	ldr r3, [sp, #164]
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #8
	str r3, [r2]
.L_08185e0c:
	mov r5, r11
	cmp r5, #139
	ble .L_08185e7c
	ldr r6, [sp, #164]
	movs r7, #239
	lsls r7, r7, #7
	adds r2, r6, r7
	movs r3, #1
	str r3, [r2]
	cmp r5, #153
	ble .L_08185e64
	movs r0, #238
	lsls r0, r0, #7
	ldr r3, .L_08185e60
	adds r0, #132
	adds r2, r6, r0
	b .L_08185e70
	.2byte 0x0000
.L_08185e30:
	.4byte gMapCellBuffer
.L_08185e34:
	.4byte 0x000000ee
.L_08185e38:
	.4byte 0x000000ef
.L_08185e3c:
	.4byte Data_02011000
.L_08185e40:
	.4byte 0x000000c2
.L_08185e44:
	.4byte Data_02012000
.L_08185e48:
	.4byte gInput
.L_08185e4c:
	.4byte IwramFillWords
.L_08185e50:
	.4byte 0xffc00000
.L_08185e54:
	.4byte 0x00000149
.L_08185e58:
	.4byte IwramCopyWords
.L_08185e5c:
	.4byte 0x000fffff
.L_08185e60:
	.4byte 0x3f3f3f3f
.L_08185e64:
	ldr r1, [sp, #164]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	ldr r3, .L_08185eac
.L_08185e70:
	str r3, [r2]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	bl Func_08164a4c
.L_08185e7c:
	mov r4, r11
	cmp r4, #80
	beq .L_08185e86
	cmp r4, #83
	bne .L_08185eb0
.L_08185e86:
	movs r2, #160
	ldr r1, .L_08185ea8
	lsls r2, r2, #19
	movs r5, #0
	adds r2, #192
	mov r9, r5
.L_08185e92:
	ldrh r3, [r2]
	movs r6, #1
	add r9, r6
	eors r3, r1
	mov r7, r9
	strh r3, [r2]
	adds r2, #2
	cmp r7, #128
	bne .L_08185e92
	b .L_08185eb0
	.2byte 0x0000
.L_08185ea8:
	.4byte 0x00007fff
.L_08185eac:
	.4byte 0x10101010
.L_08185eb0:
	mov r0, r11
	cmp r0, #82
	bne .L_08185ed8
	ldr r1, [sp, #164]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	adds r3, r1, r2
	movs r2, #8
	str r2, [r3]
	movs r0, #208
	bl Audio_PlayCue
	ldr r3, .L_08185f1c
	movs r1, #128
	ldr r0, [sp, #168]
	lsls r1, r1, #7
	ldr r2, .L_08185f20
	mov lr, r3
	.2byte 0xf800
.L_08185ed8:
	mov r3, r11
	cmp r3, #96
	bne .L_08185ee4
	movs r0, #104
	bl Audio_PlayCue
.L_08185ee4:
	mov r4, r11
	cmp r4, #128
	bne .L_08185f24
	ldr r5, [sp, #164]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #168
	adds r2, r5, r6
	movs r3, #16
	str r3, [r2]
	ldr r1, .L_08185f18
	movs r2, #160
	lsls r2, r2, #19
	movs r7, #0
	adds r2, #192
	mov r9, r7
.L_08185f04:
	ldrh r3, [r2]
	movs r0, #1
	eors r3, r1
	add r9, r0
	strh r3, [r2]
	mov r3, r9
	adds r2, #2
	cmp r3, #128
	bne .L_08185f04
	b .L_08185f24
.L_08185f18:
	.4byte 0x00007fff
.L_08185f1c:
	.4byte IwramFillWords
.L_08185f20:
	.4byte 0x3f3f3f3f
.L_08185f24:
	mov r4, r11
	cmp r4, #128
	ble .L_08185f40
	mov r3, r11
	subs r3, #128
	lsls r3, r3, #1
	adds r0, r3, #0
	mov r1, r11
	movs r2, #3
	subs r0, #11
	subs r1, #136
	subs r2, r2, r3
	bl Func_08164b2c
.L_08185f40:
	ldr r6, [sp, #84]
	mov r0, r11
	movs r7, #16
	asrs r5, r6, #16
	cmp r0, #92
	ble .L_08185f50
	mov r7, r11
	subs r7, #75
.L_08185f50:
	mov r1, r11
	cmp r1, #79
	bgt .L_08185f62
	lsls r0, r1, #11
	bl Trig_Sin
	lsls r0, r0, #1
	asrs r0, r0, #16
	adds r7, r7, r0
.L_08185f62:
	cmp r7, #64
	bgt .L_08185f78
	ldr r2, [sp, #164]
	movs r3, #224
	lsls r3, r3, #3
	adds r0, r2, r3
	adds r1, r7, #0
	ldr r2, .L_081861e8
	bl Func_0815b434
	b .L_08185f7a
.L_08185f78:
	movs r7, #64
.L_08185f7a:
	lsrs r3, r7, #31
	adds r3, r7, r3
	ldr r4, [sp, #164]
	asrs r3, r3, #1
	subs r3, r5, r3
	movs r5, #224
	adds r3, #60
	lsls r5, r5, #3
	str r3, [sp, #80]
	adds r2, r3, #0
	adds r0, r4, r5
	movs r1, #60
	adds r3, r7, #0
	mov r6, r11
	bl Func_0818caa8
	cmp r6, #95
	ble .L_08186042
	movs r0, #0
	mov r9, r0
.L_08185fa2:
	movs r1, #3
	mov r4, r9
	ands r4, r1
	str r4, [sp, #8]
	bl Random16
	adds r5, r0, #0
	bl Trig_Sin
	ldr r2, .L_081861ec
	ldr r4, [sp, #8]
	adds r6, r7, #0
	muls r6, r0
	ldrb r3, [r2, r4]
	asrs r6, r6, #17
	lsrs r3, r3, #1
	adds r0, r5, #0
	adds r6, #60
	mov r10, r2
	subs r6, r6, r3
	bl Trig_Cos
	adds r5, r7, #0
	muls r5, r0
	ldr r3, [sp, #80]
	ldr r4, [sp, #8]
	ldr r0, .L_081861f0
	asrs r5, r5, #17
	adds r5, r3, r5
	ldrb r3, [r0, r4]
	mov r8, r0
	lsrs r3, r3, #1
	subs r5, r5, r3
	bl Random16
	ldr r3, .L_081861f4
	movs r1, #3
	ands r0, r1
	ldrb r2, [r3, r0]
	movs r3, #3
	orrs r3, r2
	movs r2, #0
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r4, [sp, #8]
	ldr r2, .L_081861f8
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #164]
	movs r3, #240
	adds r1, r2, r1
	lsls r3, r3, #4
	mov r0, r10
	adds r1, r1, r3
	ldrb r3, [r0, r4]
	mov r2, r8
	str r3, [sp, #0]
	ldr r0, [sp, #168]
	ldrb r3, [r2, r4]
	adds r2, r6, #0
	str r3, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [r3]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r4, #1
	add r9, r4
	mov r5, r9
	cmp r5, #4
	bne .L_08185fa2
.L_08186042:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #76]
	movs r0, #1
	bl Func_081969f8
	mov r6, r11
	adds r7, r0, #0
	cmp r6, #127
	ble .L_0818605c
	b .L_08186198
.L_0818605c:
	movs r6, #167
	lsls r6, r6, #9
	mov r0, r11
	adds r6, #32
	cmp r0, #71
	ble .L_0818606e
	ldr r1, .L_081861fc
	lsls r3, r0, #11
	adds r6, r3, r1
.L_0818606e:
	movs r2, #16
	negs r2, r2
	mov r3, r11
	str r2, [sp, #72]
	cmp r3, #119
	ble .L_08186084
	lsls r2, r3, #3
	movs r3, #236
	lsls r3, r3, #2
	subs r3, r3, r2
	str r3, [sp, #72]
.L_08186084:
	ldr r4, .L_08186200
	cmp r6, r4
	ble .L_0818608c
	ldr r6, .L_08186200
.L_0818608c:
	ldr r3, [sp, #184]
	ldr r2, .L_08186204
	movs r1, #6
	ands r3, r2
	ldr r2, .L_08186208
	ldr r5, [sp, #72]
	orrs r3, r1
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	str r5, [r7, #20]
	str r1, [r7]
	ldr r0, [sp, #56]
	str r3, [sp, #184]
	ldr r3, .L_0818620c
	str r0, [r7, #16]
	str r3, [r7, #8]
	ldr r1, [sp, #76]
	ldr r3, .L_08186210
	str r1, [r7, #12]
	str r3, [r0, #4]
	bl Func_08014de4
	ldr r0, .L_08186214
	ldr r1, [sp, #84]
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	lsls r1, r1, #9
	adds r0, r2, #0
	bl Func_080151e4
	movs r0, #146
	lsls r0, r0, #7
	adds r0, #80
	bl SceneTransform_ApplyPitch
	mov r2, r11
	lsls r5, r2, #9
	adds r0, r5, #0
	bl Func_080150e4
	adds r0, r6, #0
	bl Func_0801521c
	movs r0, #0
	ldr r1, .L_08186218
	movs r2, #0
	bl Func_08015160
	ldr r0, .L_0818621c
	ldr r1, [sp, #76]
	movs r2, #4
	bl Func_08196958
	movs r1, #240
	movs r0, #0
	lsls r1, r1, #12
	movs r2, #0
	bl Func_08015160
	adds r0, r7, #0
	bl Func_08196a7c
	ldr r3, .L_08186220
	ldr r4, [sp, #56]
	movs r0, #168
	lsls r0, r0, #6
	str r3, [r4, #4]
	movs r6, #0
	adds r0, #170
	mov r9, r6
	mov r8, r0
	adds r6, r5, r0
.L_08186128:
	adds r0, r6, #0
	bl Trig_Cos
	ldr r1, [sp, #72]
	adds r5, r0, #0
	lsls r5, r5, #3
	asrs r5, r5, #16
	adds r5, r1, r5
	subs r5, #16
	bl Func_08014e38
	mov r0, r8
	bl Func_080150e4
	movs r0, #0
	movs r2, #0
	str r5, [r7, #20]
	ldr r1, .L_08186224
	bl Func_08015160
	movs r0, #154
	mov r2, r11
	lsls r0, r0, #7
	cmp r2, #63
	ble .L_08186164
	lsls r3, r2, #10
	movs r4, #128
	subs r3, r0, r3
	lsls r4, r4, #9
	adds r0, r3, r4
.L_08186164:
	cmp r0, #0
	bge .L_0818616a
	movs r0, #0
.L_0818616a:
	negs r0, r0
	bl SceneTransform_ApplyPitch
	movs r2, #4
	ldr r1, [sp, #76]
	ldr r0, .L_08186228
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	movs r5, #170
	bl Func_08014ea8
	movs r0, #1
	lsls r5, r5, #7
	add r9, r0
	adds r5, #85
	mov r1, r9
	adds r6, r6, r5
	add r8, r5
	cmp r1, #3
	bne .L_08186128
.L_08186198:
	ldr r3, [sp, #20]
	movs r2, #0
	mov r9, r2
	ldr r1, .L_0818622c
	lsls r2, r3, #7
	subs r2, r2, r3
	ldr r4, .L_08186230
	lsls r3, r2, #4
	lsls r2, r2, #5
	adds r1, r1, r2
	mov r5, r11
	movs r2, #80
	adds r4, r4, r3
	movs r0, #160
	lsls r3, r5, #3
	str r2, [sp, #40]
	negs r3, r3
	lsls r0, r0, #2
	mov r10, r4
	adds r6, r3, r0
	mov r8, r1
.L_081861c2:
	ldr r5, [sp, #40]
	mov r4, r9
	lsls r3, r4, #3
	cmp r11, r5
	blt .L_081862aa
	movs r0, #0
	str r0, [sp, #72]
	cmp r4, #0
	bne .L_08186234
	movs r5, #128
	lsls r5, r5, #6
	adds r3, #96
	add r5, r8
	cmp r11, r3
	blt .L_08186248
	adds r1, r6, #0
	adds r1, #128
	str r1, [sp, #72]
	b .L_08186248
.L_081861e8:
	.4byte 0x00017530
.L_081861ec:
	.4byte Data_08197492
.L_081861f0:
	.4byte Data_08197498
.L_081861f4:
	.4byte Data_081999ee
.L_081861f8:
	.4byte Data_08197486
.L_081861fc:
	.4byte 0xffff0e20
.L_08186200:
	.4byte 0x00022538
.L_08186204:
	.4byte 0xffffff00
.L_08186208:
	.4byte 0xffff00ff
.L_0818620c:
	.4byte Data_08199340
.L_08186210:
	.4byte Data_02011000
.L_08186214:
	.4byte 0xfffe0000
.L_08186218:
	.4byte 0xfff10000
.L_0818621c:
	.4byte Data_081991e0
.L_08186220:
	.4byte gMapCellBuffer
.L_08186224:
	.4byte 0xfff00000
.L_08186228:
	.4byte Data_081991f0
.L_0818622c:
	.4byte 0xfff11e00
.L_08186230:
	.4byte 0xfff88f00
.L_08186234:
	movs r5, #128
	lsls r5, r5, #6
	adds r3, #112
	add r5, r10
	cmp r11, r3
	blt .L_08186248
	movs r2, #128
	lsls r2, r2, #1
	adds r2, r6, r2
	str r2, [sp, #72]
.L_08186248:
	ldr r3, [sp, #72]
	movs r4, #64
	negs r4, r4
	cmp r3, r4
	ble .L_081862aa
	str r3, [r7, #20]
	ldr r3, .L_081864c8
	mov r0, sp
	str r3, [r7, #8]
	movs r3, #6
	str r3, [r7]
	ldr r2, [sp, #56]
	ldr r3, .L_081864cc
	adds r0, #184
	str r3, [r2, #4]
	movs r3, #7
	str r0, [sp, #56]
	strb r3, [r0]
	strb r3, [r2, #1]
	bl Func_08014de4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #13
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #9
	movs r1, #250
	adds r0, r2, #0
	lsls r1, r1, #7
	bl Func_080151e4
	mov r1, r11
	lsls r0, r1, #10
	bl Func_080150e4
	adds r0, r5, #0
	bl Func_0801521c
	ldr r0, .L_081864d0
	ldr r1, [sp, #76]
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_081862aa:
	ldr r4, [sp, #40]
	ldr r2, .L_081864d4
	ldr r3, .L_081864d8
	movs r5, #1
	add r9, r5
	adds r4, #8
	mov r0, r9
	add r10, r2
	adds r6, #64
	add r8, r3
	str r4, [sp, #40]
	cmp r0, #2
	beq .L_081862c6
	b .L_081861c2
.L_081862c6:
	ldr r1, [sp, #56]
	movs r3, #5
	movs r2, #184
	strb r3, [r1]
	add r2, sp
	movs r3, #7
	str r2, [sp, #56]
	strb r3, [r2, #1]
	movs r3, #6
	str r3, [r7]
	ldr r3, .L_081864dc
	mov r4, r11
	str r3, [r7, #8]
	movs r3, #0
	mov r8, r2
	str r3, [r7, #20]
	cmp r4, #0
	bne .L_08186330
	ldr r6, .L_081864e0
	ldr r5, [sp, #164]
	mov r10, r6
	mov r9, r3
	movs r6, #0
.L_081862f4:
	mov r0, r10
	ldrsh r3, [r6, r0]
	str r3, [r5, #8]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	str r3, [r5, #20]
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #0
	beq .L_08186318
	ldr r3, [r5, #20]
	negs r3, r3
	str r3, [r5, #20]
.L_08186318:
	bl Random16
	movs r3, #3
	movs r2, #1
	ands r3, r0
	add r9, r2
	str r3, [r5, #24]
	mov r3, r9
	adds r5, #28
	adds r6, #2
	cmp r3, #16
	bne .L_081862f4
.L_08186330:
	movs r4, #0
	mov r9, r4
	movs r5, #128
.L_08186336:
	cmp r11, r5
	bne .L_08186340
	movs r0, #144
	bl Audio_PlayCue
.L_08186340:
	movs r6, #1
	add r9, r6
	mov r0, r9
	adds r5, #4
	cmp r0, #8
	bne .L_08186336
	ldr r5, [sp, #164]
	movs r1, #0
	mov r9, r1
.L_08186352:
	mov r2, r9
	adds r2, #128
	cmp r11, r2
	blt .L_081863b6
	mov r3, r9
	adds r3, #144
	cmp r11, r3
	bge .L_081863b6
	mov r4, r11
	subs r3, r4, r2
	movs r0, #128
	lsls r6, r3, #13
	lsls r0, r0, #9
	cmp r6, r0
	ble .L_08186374
	movs r6, #128
	lsls r6, r6, #9
.L_08186374:
	ldr r3, [r5, #24]
	ldr r1, [sp, #164]
	movs r2, #216
	lsls r3, r3, #12
	lsls r2, r2, #5
	adds r3, r1, r3
	adds r2, #86
	adds r3, r3, r2
	mov r4, r8
	str r3, [r4, #4]
	bl Func_08014de4
	ldr r0, [r5, #8]
	bl Func_080150e4
	ldr r3, [r5, #8]
	ldr r2, [r5, #20]
	lsls r0, r6, #2
	adds r3, r3, r2
	movs r2, #128
	str r3, [r5, #8]
	adds r1, r6, #0
	lsls r2, r2, #9
	bl Func_080151e4
	ldr r0, .L_081864e4
	ldr r1, [sp, #76]
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_081863b6:
	movs r6, #1
	add r9, r6
	mov r0, r9
	adds r5, #28
	cmp r0, #16
	bne .L_08186352
	adds r0, r7, #0
	bl Sys_Free
	ldr r0, [sp, #76]
	bl Sys_Free
	mov r1, r11
	cmp r1, #133
	ble .L_08186494
	cmp r1, #134
	bne .L_0818643c
	ldr r3, [sp, #164]
	movs r4, #224
	movs r2, #0
	lsls r4, r4, #1
	mov r9, r2
	adds r7, r3, r4
.L_081863e4:
	bl Random16
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #85
	mov r6, r9
	muls r6, r3
	movs r5, #31
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #16
	adds r3, #60
	str r3, [r7]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #16
	adds r3, #60
	str r3, [r7, #4]
	bl Random16
	movs r3, #3
	ands r3, r0
	adds r3, #5
	str r3, [r7, #8]
	bl Random16
	movs r5, #1
	movs r3, #15
	ands r3, r0
	add r9, r5
	negs r3, r3
	mov r6, r9
	str r3, [r7, #24]
	adds r7, #28
	cmp r6, #48
	bne .L_081863e4
.L_0818643c:
	ldr r0, [sp, #164]
	movs r1, #224
	ldr r6, .L_081864e8
	movs r7, #0
	lsls r1, r1, #1
	mov r9, r7
	adds r5, r0, r1
.L_0818644a:
	ldr r0, [r5, #24]
	cmp r0, #7
	bhi .L_08186484
	lsls r0, r0, #12
	bl Trig_Sin
	ldr r3, [r5, #8]
	ldr r2, [sp, #144]
	muls r0, r3
	asrs r0, r0, #16
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	lsrs r3, r0, #31
	adds r1, r2, r1
	ldr r2, [r5]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #168]
	ldr r4, [sp, #156]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r5, #24]
.L_08186484:
	movs r7, #1
	add r9, r7
	adds r3, r0, #1
	mov r0, r9
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #48
	bne .L_0818644a
.L_08186494:
	ldr r1, [sp, #164]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	adds r7, r1, r2
	ldr r3, [r7]
	cmp r3, #0
	ble .L_081864f0
	bl Random16
	adds r5, r0, #0
	bl Random16
	movs r6, #7
	ldr r3, .L_081864ec
	ands r5, r6
	ands r0, r6
	subs r5, #4
	adds r0, #28
	strh r5, [r3, #4]
	strh r0, [r3, #6]
	ldr r3, [r7]
	subs r3, #1
	str r3, [r7]
	b .L_08186504
	.2byte 0x0000
.L_081864c8:
	.4byte Data_08199364
.L_081864cc:
	.4byte Data_02012000
.L_081864d0:
	.4byte Data_081991e0
.L_081864d4:
	.4byte 0xffff4180
.L_081864d8:
	.4byte 0xfffe8300
.L_081864dc:
	.4byte Data_08199268
.L_081864e0:
	.4byte Data_081999f2
.L_081864e4:
	.4byte Data_081991b0
.L_081864e8:
	.4byte Data_08197410
.L_081864ec:
	.4byte Data_03001120
.L_081864f0:
	ldr r2, .L_08186618
	movs r3, #0
	strh r3, [r2, #4]
	ldr r4, [sp, #164]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #164
	adds r3, r4, r5
	ldr r3, [r3]
	strh r3, [r2, #6]
.L_08186504:
	ldr r6, [sp, #164]
	movs r7, #240
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r6, r7
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #20]
	movs r1, #1
	add r11, r1
	adds r0, #3
	mov r2, r11
	str r0, [sp, #20]
	cmp r2, #160
	beq .L_08186536
	ldr r3, .L_0818661c
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08186536
	b .L_08185d88
.L_08186536:
	movs r3, #0
	movs r1, #128
	str r3, [sp, #68]
	str r3, [sp, #64]
	ldr r0, [sp, #168]
	lsls r1, r1, #7
	ldr r3, .L_08186620
	ldr r2, .L_08186624
	mov lr, r3
	.2byte 0xf800
	movs r6, #240
	ldr r4, [sp, #164]
	lsls r6, r6, #7
	adds r6, #232
	adds r2, r4, r6
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	add r7, sp, #152
	ldr r3, .L_08186618
	ldrh r7, [r7]
	movs r1, #206
	strh r7, [r3, #4]
	ldr r0, [sp, #140]
	lsls r1, r1, #3
	adds r3, r0, r1
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #24
	bl Func_08118040
	movs r1, #16
	negs r1, r1
	movs r2, #4
	adds r0, r1, #0
	negs r2, r2
	bl Func_08164b2c
	ldr r2, [sp, #164]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08186628
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r4, [sp, #164]
	movs r6, #240
	lsls r6, r6, #4
	adds r1, r4, r6
	ldr r0, .L_0818662c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_08186630
	ldr r1, [sp, #144]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r7, [sp, #164]
	ldr r5, .L_08186634
	movs r2, #248
	lsls r2, r2, #5
	adds r1, r7, r2
	ldr r0, .L_08186638
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0818663c
	adds r1, r5, #0
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r2, #32
	movs r3, #11
	ldr r0, .L_08186640
	movs r1, #4
	bl Func_08178680
	ldr r3, .L_08186614
	movs r2, #128
	lsls r2, r2, #19
	mov r4, sp
	adds r2, #12
	adds r4, #176
	strh r3, [r2]
	str r4, [sp, #60]
	movs r3, #0
	mov r11, r3
.L_081865f6:
	ldr r3, .L_0818661c
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08186644
	mov r5, r11
	cmp r5, #2
	ble .L_08186644
	cmp r5, #87
	bgt .L_08186644
	movs r6, #88
	mov r11, r6
	b .L_08186644
	.2byte 0x0000
.L_08186614:
	.4byte 0x00000784
.L_08186618:
	.4byte Data_03001120
.L_0818661c:
	.4byte gInput
.L_08186620:
	.4byte IwramFillWords
.L_08186624:
	.4byte 0x3f3f3f3f
.L_08186628:
	.4byte 0x000000f4
.L_0818662c:
	.4byte 0x000000e8
.L_08186630:
	.4byte 0x00000134
.L_08186634:
	.4byte Data_02014000
.L_08186638:
	.4byte 0x0000013e
.L_0818663c:
	.4byte 0x00000188
.L_08186640:
	.4byte Data_02012000
.L_08186644:
	mov r7, r11
	cmp r7, #88
	bne .L_08186690
	movs r1, #128
	ldr r3, .L_081869dc
	lsls r1, r1, #7
	ldr r2, .L_081869e0
	ldr r0, [sp, #168]
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #164]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #3
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	ldr r3, .L_081869e4
	add r4, sp, #152
	str r3, [r2]
	ldrh r4, [r4]
	ldr r3, .L_081869e8
	ldr r2, .L_081869ec
	movs r5, #238
	lsls r5, r5, #7
	strh r4, [r3, #4]
	adds r5, #152
	movs r3, #120
	str r3, [r2, #12]
	movs r6, #0
	adds r3, r0, r5
	str r6, [r3]
	movs r0, #134
	bl Func_081180e8
.L_08186690:
	mov r7, r11
	cmp r7, #0
	bne .L_08186708
	movs r1, #128
	ldr r3, .L_081869dc
	ldr r0, [sp, #168]
	lsls r1, r1, #7
	ldr r2, .L_081869e0
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #164]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #152
	movs r3, #16
	adds r2, r0, r1
	negs r3, r3
	str r3, [r2]
	ldr r2, .L_081869ec
	ldr r3, .L_081869f0
	movs r5, #136
	str r3, [r2, #12]
	movs r3, #120
	str r3, [r2, #16]
	ldr r4, [sp, #152]
	ldr r2, .L_081869e8
	lsls r5, r5, #3
	adds r3, r4, r5
	strh r3, [r2, #4]
	bl Func_0815b410
	ldr r6, [sp, #164]
	movs r7, #239
	lsls r7, r7, #7
	adds r2, r6, r7
	movs r3, #3
	movs r0, #238
	str r3, [r2]
	lsls r0, r0, #7
	ldr r3, .L_081869e4
	adds r0, #132
	adds r2, r6, r0
	str r3, [r2]
	ldr r3, .L_081869f4
	movs r1, #0
	movs r2, #1
	mov r9, r1
	negs r2, r2
.L_081866f0:
	movs r4, #1
	add r9, r4
	mov r5, r9
	str r2, [r3]
	adds r3, #28
	cmp r5, #164
	bne .L_081866f0
	movs r6, #128
	lsls r6, r6, #16
	movs r7, #0
	str r6, [sp, #68]
	str r7, [sp, #64]
.L_08186708:
	mov r0, r11
	cmp r0, #16
	bne .L_08186726
	ldr r1, [sp, #164]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r1, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #75
	str r3, [r2]
.L_08186726:
	mov r3, r11
	subs r3, #16
	cmp r3, #47
	bhi .L_08186734
	ldr r0, .L_081869f8
	bl Func_0815f0a0
.L_08186734:
	mov r5, r11
	cmp r5, #66
	bne .L_08186820
	ldr r7, [sp, #172]
	movs r6, #0
	ldr r3, [r7, #20]
	mov r9, r6
	cmp r3, #0
	beq .L_0818680c
	mov r0, sp
	adds r0, #216
	str r0, [sp, #48]
	str r6, [sp, #24]
	movs r1, #36
	mov r8, r1
.L_08186752:
	ldr r4, [sp, #172]
	mov r2, r8
	ldrsh r0, [r2, r4]
	movs r3, #150
	str r3, [sp, #4]
	movs r5, #128
	movs r2, #128
	movs r3, #128
	lsls r5, r5, #11
	movs r1, #1
	lsls r2, r2, #11
	lsls r3, r3, #12
	str r5, [sp, #0]
	bl Func_0815f000
	ldr r1, [sp, #172]
	mov r6, r8
	movs r3, #16
	ldrsh r0, [r6, r1]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r9
	bl Func_0814cd48
	ldr r3, [sp, #172]
	ldr r1, [sp, #48]
	ldrsh r0, [r6, r3]
	bl Func_0815e21c
	ldr r6, .L_081869fc
	ldr r5, [sp, #24]
	movs r4, #0
	mov r10, r4
	adds r7, r5, r6
.L_08186798:
	ldr r0, [sp, #48]
	movs r6, #254
	ldr r3, [r0]
	lsls r6, r6, #7
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r7, #4]
	bl Random16
	adds r6, #255
	movs r1, #128
	lsls r1, r1, #7
	ands r6, r0
	adds r6, r6, r1
	bl Random16
	movs r5, #127
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r2, #1
	asrs r3, r3, #4
	str r3, [r7, #16]
	add r10, r2
	movs r3, #16
	str r3, [r7, #24]
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_08186798
	ldr r5, [sp, #24]
	movs r6, #224
	lsls r6, r6, #1
	ldr r7, [sp, #172]
	adds r5, r5, r6
	str r5, [sp, #24]
	ldr r3, [r7, #20]
	movs r4, #2
	add r9, r2
	add r8, r4
	cmp r9, r3
	bne .L_08186752
.L_0818680c:
	ldr r0, [sp, #164]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #16
	str r3, [r2]
	movs r0, #145
	bl Audio_PlayCue
.L_08186820:
	mov r3, r11
	subs r3, #72
	cmp r3, #15
	bhi .L_0818684c
	ldr r3, [sp, #164]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #4
	str r3, [r2]
	ldr r5, [sp, #164]
	movs r6, #238
	lsls r6, r6, #7
	ldr r3, .L_08186a00
	adds r6, #132
	adds r2, r5, r6
	str r3, [r2]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	bl Func_08164a4c
.L_0818684c:
	mov r7, r11
	cmp r7, #0
	blt .L_0818692c
	movs r3, #0
	cmp r7, #71
	ble .L_08186862
	lsls r3, r7, #3
	movs r0, #144
	negs r3, r3
	lsls r0, r0, #2
	adds r3, r3, r0
.L_08186862:
	adds r3, #116
	mov r8, r3
	movs r2, #31
	mov r3, r11
	ands r2, r3
	lsls r3, r2, #3
	ldr r4, .L_08186a04
	subs r3, r3, r2
	movs r1, #0
	lsls r3, r3, #3
	mov r9, r1
	adds r7, r3, r4
.L_0818687a:
	mov r5, r8
	lsls r3, r5, #16
	str r3, [r7]
	bl Random16
	movs r1, #112
	bl Math_ModU
	adds r0, #4
	lsls r0, r0, #16
	str r0, [r7, #4]
	bl Random16
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	ands r6, r0
	movs r0, #128
	lsls r0, r0, #8
	adds r6, r6, r0
	bl Random16
	movs r5, #127
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r2, #1
	asrs r3, r3, #6
	add r9, r2
	str r3, [r7, #16]
	movs r1, #0
	mov r3, r9
	str r1, [r7, #24]
	adds r7, #28
	cmp r3, #2
	bne .L_0818687a
	ldr r5, .L_08186a04
	mov r9, r1
.L_081868dc:
	ldr r3, [r5, #24]
	cmp r3, #11
	bhi .L_08186920
	lsrs r1, r3, #31
	ldr r4, [sp, #164]
	adds r1, r3, r1
	movs r7, #2
	ldrsh r2, [r5, r7]
	movs r0, #6
	ldrsh r3, [r5, r0]
	asrs r1, r1, #1
	movs r0, #32
	lsls r1, r1, #11
	movs r6, #248
	str r0, [sp, #0]
	adds r1, r4, r1
	movs r0, #64
	lsls r6, r6, #5
	subs r3, #32
	str r0, [sp, #4]
	adds r1, r1, r6
	subs r2, #16
	ldr r0, [sp, #168]
	ldr r4, [sp, #156]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #63
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_08186920:
	movs r6, #1
	add r9, r6
	mov r7, r9
	adds r5, #28
	cmp r7, #64
	bne .L_081868dc
.L_0818692c:
	mov r0, r11
	cmp r0, #87
	ble .L_08186934
	b .L_08186d30
.L_08186934:
	movs r4, #0
	mov r10, r4
	mov r8, r4
	movs r2, #0
	mov r7, r11
	movs r1, #0
.L_08186940:
	ldr r0, .L_08186a08
	movs r3, #0
	mov r9, r3
	movs r6, #0
	adds r5, r1, r0
.L_0818694a:
	lsls r0, r7, #10
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Sin
	lsls r0, r0, #1
	ldr r4, [sp, #8]
	asrs r0, r0, #16
	adds r0, r6, r0
	adds r0, r0, r4
	subs r0, #5
	strb r0, [r5]
	ldr r2, [sp, #12]
	movs r0, #1
	mov r3, r8
	add r9, r0
	strb r3, [r5, #2]
	mov r3, r9
	strb r2, [r5, #1]
	adds r6, #10
	adds r5, #4
	ldr r1, [sp, #16]
	cmp r3, #2
	bne .L_0818694a
	bl Random16
	ldr r4, [sp, #8]
	movs r3, #7
	ldr r2, [sp, #12]
	ldr r1, [sp, #16]
	movs r5, #1
	ands r3, r0
	add r10, r5
	adds r3, r4, r3
	mov r6, r10
	subs r4, r3, #4
	subs r2, #6
	adds r7, #2
	adds r1, #8
	cmp r6, #12
	bne .L_08186940
	movs r0, #1
	bl Func_081969f8
	ldr r3, .L_08186a0c
	adds r7, r0, #0
	movs r1, #0
	movs r0, #6
	add r2, sp, #176
	mov r4, r11
	str r0, [r7]
	str r2, [r7, #16]
	str r1, [r7, #20]
	strb r1, [r7, #25]
	str r3, [r7, #12]
	cmp r4, #16
	bne .L_081869c2
	ldr r5, .L_08186a10
	str r5, [sp, #64]
.L_081869c2:
	mov r3, r11
	subs r3, #48
	cmp r3, #5
	bhi .L_081869d0
	movs r6, #128
	lsls r6, r6, #12
	str r6, [sp, #64]
.L_081869d0:
	mov r0, r11
	cmp r0, #59
	ble .L_08186a18
	ldr r1, .L_08186a14
	str r1, [sp, #64]
	b .L_08186a18
.L_081869dc:
	.4byte IwramFillWords
.L_081869e0:
	.4byte 0x3f3f3f3f
.L_081869e4:
	.4byte Data_02020202
.L_081869e8:
	.4byte Data_03001120
.L_081869ec:
	.4byte gCameraSceneParameters
.L_081869f0:
	.4byte 0xfffffc38
.L_081869f4:
	.4byte Data_02016d18
.L_081869f8:
	.4byte 0x00000149
.L_081869fc:
	.4byte Data_02017400
.L_08186a00:
	.4byte 0x01010101
.L_08186a04:
	.4byte Data_02016d00
.L_08186a08:
	.4byte gMapCellBuffer
.L_08186a0c:
	.4byte Data_02011000
.L_08186a10:
	.4byte 0xfff80000
.L_08186a14:
	.4byte 0xfff00000
.L_08186a18:
	mov r3, r11
	cmp r3, #31
	ble .L_08186a30
	ldr r4, [sp, #64]
	lsls r3, r4, #1
	adds r3, r3, r4
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_08186a2c
	adds r3, #63
.L_08186a2c:
	asrs r3, r3, #6
	str r3, [sp, #64]
.L_08186a30:
	ldr r5, [sp, #68]
	ldr r6, [sp, #64]
	movs r0, #6
	adds r5, r5, r6
	str r5, [sp, #68]
	strb r0, [r2]
	ldr r1, [sp, #60]
	movs r3, #5
	strb r3, [r1, #1]
	ldr r2, [sp, #164]
	movs r4, #224
	lsls r4, r4, #3
	adds r3, r2, r4
	mov r6, r11
	ldr r5, .L_08186db8
	str r3, [r1, #4]
	movs r2, #127
	lsls r3, r6, #3
	ands r3, r2
	strb r3, [r7, #24]
	str r5, [r7, #8]
	bl Func_08014de4
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	lsls r3, r6, #1
	add r3, r11
	lsls r0, r3, #7
	subs r0, r0, r3
	lsls r0, r0, #4
	mov r8, r0
	bl Trig_Sin
	lsls r6, r6, #13
	mov r10, r6
	adds r5, r0, #0
	mov r0, r10
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	lsls r5, r5, #5
	adds r5, r5, r3
	ldr r0, [sp, #68]
	adds r1, r5, #0
	movs r2, #0
	bl Func_08015160
	movs r0, #216
	lsls r0, r0, #8
	adds r0, #240
	bl Func_0801521c
	bl Func_08014e38
	movs r0, #128
	lsls r0, r0, #7
	bl Func_080150e4
	ldr r0, .L_08186dbc
	bl Func_0801521c
	ldr r1, .L_08186dc0
	movs r2, #24
	ldr r0, .L_08186dc4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	bl Func_08014ea8
	ldr r3, .L_08186dc8
	ldr r2, [sp, #60]
	movs r1, #6
	str r3, [r7, #8]
	movs r3, #0
	strb r1, [r2]
	strb r1, [r2, #1]
	strb r3, [r7, #24]
	ldr r4, [sp, #164]
	movs r5, #240
	lsls r5, r5, #4
	adds r3, r4, r5
	mov r6, r11
	str r3, [r2, #4]
	lsls r0, r6, #10
	bl Trig_Sin
	adds r1, r0, #0
	ldr r0, .L_08186dcc
	lsls r1, r1, #2
	adds r1, r1, r0
	movs r2, #0
	ldr r0, .L_08186dd0
	bl Func_08015160
	ldr r0, .L_08186dd4
	bl Func_080150e4
	mov r0, r8
	bl Trig_Sin
	adds r6, r0, #0
	mov r0, r10
	bl Trig_Cos
	movs r1, #184
	lsls r1, r1, #5
	adds r1, #208
	lsls r3, r0, #1
	add r8, r1
	adds r3, r3, r0
	lsls r3, r3, #3
	lsls r6, r6, #5
	mov r0, r8
	adds r6, r6, r3
	bl Trig_Sin
	movs r2, #128
	lsls r2, r2, #6
	add r10, r2
	adds r5, r0, #0
	mov r0, r10
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r5, r5, #5
	lsls r3, r3, #3
	adds r5, r5, r3
	subs r0, r6, r5
	cmp r0, #0
	bge .L_08186b50
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	adds r0, r0, r3
.L_08186b50:
	ldr r4, .L_08186dd8
	asrs r0, r0, #9
	adds r0, r0, r4
	bl Func_080150e4
	movs r1, #206
	lsls r1, r1, #9
	movs r2, #128
	ldr r0, .L_08186ddc
	adds r1, #64
	lsls r2, r2, #9
	bl Func_080151e4
	ldr r1, .L_08186dc0
	movs r2, #4
	ldr r0, .L_08186de0
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	mov r5, r11
	adds r0, r7, #0
	bl Sys_Free
	cmp r5, #87
	ble .L_08186b88
	b .L_08186d30
.L_08186b88:
	ldr r7, [sp, #164]
	movs r0, #16
	str r0, [sp, #44]
	movs r6, #0
	mov r9, r6
	mov r8, r7
.L_08186b94:
	ldr r1, [sp, #44]
	cmp r11, r1
	bne .L_08186c68
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #64
	mov r2, r8
	str r3, [r2]
	bl Random16
	mov r3, r8
	str r0, [r3, #24]
	ldr r4, [sp, #164]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	movs r3, #4
	adds r2, r4, r5
	str r3, [r2]
	mov r6, r9
	movs r3, #3
	ands r3, r6
	cmp r3, #1
	beq .L_08186be4
	cmp r3, #1
	bgt .L_08186bd2
	cmp r3, #0
	beq .L_08186bdc
	b .L_08186bfa
.L_08186bd2:
	cmp r3, #2
	beq .L_08186bec
	cmp r3, #3
	beq .L_08186bf4
	b .L_08186bfa
.L_08186bdc:
	movs r0, #227
	bl Audio_PlayCue
	b .L_08186bfa
.L_08186be4:
	movs r0, #171
	bl Audio_PlayCue
	b .L_08186bfa
.L_08186bec:
	movs r0, #208
	bl Audio_PlayCue
	b .L_08186bfa
.L_08186bf4:
	movs r0, #172
	bl Audio_PlayCue
.L_08186bfa:
	movs r2, #7
	mov r0, r9
	ands r2, r0
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	ldr r1, .L_08186de4
	adds r3, r3, r2
	movs r7, #0
	lsls r3, r3, #4
	mov r10, r7
	adds r7, r3, r1
.L_08186c12:
	mov r2, r8
	ldr r3, [r2]
	movs r6, #254
	lsls r3, r3, #16
	str r3, [r7]
	movs r3, #208
	lsls r3, r3, #15
	str r3, [r7, #4]
	bl Random16
	lsls r6, r6, #7
	adds r6, #255
	movs r3, #128
	lsls r3, r3, #7
	ands r6, r0
	adds r6, r6, r3
	bl Random16
	movs r5, #127
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r4, #1
	asrs r3, r3, #4
	add r10, r4
	str r3, [r7, #16]
	mov r5, r10
	movs r3, #16
	str r3, [r7, #24]
	adds r7, #28
	cmp r5, #12
	bne .L_08186c12
.L_08186c68:
	ldr r6, [sp, #44]
	cmp r11, r6
	blt .L_08186cae
	adds r3, r6, #0
	adds r3, #8
	cmp r11, r3
	bge .L_08186cae
	mov r7, r8
	ldr r3, [r7, #24]
	ldr r0, .L_08186de8
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r2, #3
	ands r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r1, r2, #4
	subs r1, r1, r2
	ldr r2, [r7]
	movs r3, #24
	str r3, [sp, #0]
	lsls r1, r1, #6
	movs r3, #120
	adds r1, r1, r0
	str r3, [sp, #4]
	subs r2, #12
	movs r3, #0
	ldr r0, [sp, #168]
	ldr r4, [sp, #156]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_08186cae:
	ldr r6, [sp, #44]
	movs r7, #1
	add r9, r7
	movs r5, #28
	adds r6, #8
	mov r0, r9
	add r8, r5
	str r6, [sp, #44]
	cmp r0, #6
	beq .L_08186cc4
	b .L_08186b94
.L_08186cc4:
	ldr r5, .L_08186de4
	movs r1, #0
	mov r9, r1
.L_08186cca:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08186d24
	asrs r0, r0, #2
	adds r0, #2
	ldr r2, .L_08186dec
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #144]
	ldr r7, [sp, #156]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #168]
	mov lr, r7
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #56
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	movs r1, #238
	ldr r0, [sp, #164]
	lsls r1, r1, #7
	adds r1, #152
	adds r3, r0, r1
	ldr r2, [r3]
	ldr r3, [r5, #12]
	lsls r2, r2, #12
	subs r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08186d24:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r5, #28
	cmp r3, #100
	bne .L_08186cca
.L_08186d30:
	ldr r4, [sp, #164]
	movs r5, #238
	ldr r1, .L_08186df0
	lsls r5, r5, #7
	adds r5, #152
	adds r0, r4, r5
	ldr r3, [r1, #12]
	ldr r2, [r0]
	movs r6, #0
	subs r3, r3, r2
	str r3, [r1, #12]
	ldr r1, .L_08186df4
	ldr r2, [r0]
	ldrh r3, [r1, #4]
	mov r9, r6
	adds r3, r3, r2
	strh r3, [r1, #4]
.L_08186d52:
	mov r3, r9
	adds r3, #62
	cmp r11, r3
	bne .L_08186d60
	ldr r3, [r0]
	adds r3, #1
	str r3, [r0]
.L_08186d60:
	movs r7, #1
	add r9, r7
	mov r1, r9
	cmp r1, #16
	bne .L_08186d52
	movs r0, #8
	bl Func_08158d68
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #164]
	lsls r4, r4, #7
	adds r4, #232
	movs r5, #1
	adds r2, r3, r4
	add r11, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	mov r6, r11
	bl WaitFrames
	cmp r6, #104
	beq .L_08186d94
	b .L_081865f6
.L_08186d94:
	ldr r7, [sp, #148]
	movs r3, #0
	str r3, [r7, #16]
	ldr r0, .L_08186df8
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #272
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08186db8:
	.4byte Data_02012000
.L_08186dbc:
	.4byte 0x00042ee0
.L_08186dc0:
	.4byte Data_02011000
.L_08186dc4:
	.4byte gMapCellBuffer
.L_08186dc8:
	.4byte Data_08199340
.L_08186dcc:
	.4byte 0xfffa0000
.L_08186dd0:
	.4byte 0xfffe0000
.L_08186dd4:
	.4byte 0xfffff800
.L_08186dd8:
	.4byte 0xfffffc00
.L_08186ddc:
	.4byte 0x00021170
.L_08186de0:
	.4byte Data_081991e0
.L_08186de4:
	.4byte Data_02017400
.L_08186de8:
	.4byte Data_02014000
.L_08186dec:
	.4byte Data_08197410
.L_08186df0:
	.4byte gCameraSceneParameters
.L_08186df4:
	.4byte Data_03001120
.L_08186df8:
	.4byte Func_08143000
