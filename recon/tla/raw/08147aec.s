.syntax unified
	.thumb
	.global Func_08147aec
	.thumb_func
Func_08147aec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #88
	str r0, [sp, #64]
	str r1, [sp, #60]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	str r0, [sp, #56]
	movs r0, #0
	ldr r1, [r3, #48]
	str r1, [sp, #52]
	ldr r2, [r3, #92]
	ldr r3, [r3, #100]
	mov r10, r2
	str r3, [sp, #44]
	bl Func_081435e0
	ldr r3, .L_08147b58
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_08147b5c
	subs r2, #70
	strh r3, [r2]
	mov r3, sp
	adds r3, #68
	adds r1, r3, #0
	movs r0, #0
	str r3, [sp, #40]
	bl Func_08144aac
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08147b60
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #220
	lsls r1, r1, #6
	ldr r0, .L_08147b64
	add r1, r10
	movs r2, #1
	movs r3, #0
	b .L_08147b68
	.2byte 0x0000
.L_08147b58:
	.4byte 0x00001010
.L_08147b5c:
	.4byte 0x00000784
.L_08147b60:
	.4byte 0x0000013e
.L_08147b64:
	.4byte 0x00000178
.L_08147b68:
	bl Func_08157cf4
	ldr r0, .L_08147dc8
	ldr r1, [sp, #44]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r6, [sp, #60]
	cmp r6, #1
	bne .L_08147b92
	ldr r0, .L_08147dcc
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08147dd0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08147b92:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #75
	add r2, r10
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08147dd4
	bl Func_080145a8
	movs r0, #0
	str r0, [sp, #24]
	ldr r2, [sp, #64]
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl Func_08118088 + 0x10
	ldr r3, [r0]
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_08147bce
	movs r3, #1
	str r3, [sp, #24]
.L_08147bce:
	ldr r0, [sp, #60]
	movs r6, #1
	str r6, [sp, #28]
	cmp r0, #0
	beq .L_08147bea
	ldr r1, [sp, #64]
	movs r2, #1
	ldr r3, [r1, #4]
	negs r2, r2
	str r2, [sp, #28]
	cmp r3, #1
	beq .L_08147bea
	movs r3, #1
	str r3, [sp, #28]
.L_08147bea:
	ldr r6, [sp, #60]
	cmp r6, #1
	bne .L_08147c1a
	ldr r1, [sp, #64]
	add r5, sp, #76
	ldr r0, [r1, #8]
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5]
	movs r3, #66
	str r3, [r5, #4]
	ldr r2, [sp, #64]
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_08147c16
	movs r3, #76
	b .L_08147c18
.L_08147c16:
	movs r3, #44
.L_08147c18:
	str r3, [r5]
.L_08147c1a:
	ldr r3, [sp, #24]
	cmp r3, #1
	bne .L_08147c2c
	ldr r0, .L_08147dd8
	movs r6, #220
	lsls r6, r6, #16
	str r6, [sp, #32]
	str r0, [sp, #36]
	b .L_08147c36
.L_08147c2c:
	ldr r2, .L_08147ddc
	movs r1, #212
	lsls r1, r1, #16
	str r1, [sp, #32]
	str r2, [sp, #36]
.L_08147c36:
	movs r2, #1
	mov r3, r10
	movs r7, #0
	negs r2, r2
	adds r3, #24
.L_08147c40:
	adds r7, #1
	str r2, [r3]
	adds r3, #28
	cmp r7, #64
	bne .L_08147c40
	movs r5, #168
	lsls r5, r5, #2
	movs r7, #0
	movs r6, #127
	add r5, r10
.L_08147c54:
	ldr r3, [sp, #28]
	cmp r3, #1
	bne .L_08147c64
	bl Random16
	ands r0, r6
	adds r0, #128
	b .L_08147c6c
.L_08147c64:
	bl Random16
	ands r0, r6
	subs r0, #128
.L_08147c6c:
	str r0, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	subs r3, #72
	str r3, [r5, #4]
	bl Random16
	movs r3, #31
	ands r3, r0
	negs r3, r3
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #16
	bne .L_08147c54
	ldr r3, .L_08147de0
	movs r1, #1
	movs r2, #128
	movs r7, #0
	negs r1, r1
	lsls r2, r2, #2
.L_08147c9a:
	adds r7, #1
	str r1, [r3]
	adds r3, #28
	cmp r7, r2
	bne .L_08147c9a
	ldr r6, [sp, #60]
	cmp r6, #0
	bne .L_08147ccc
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #240
	add r3, r10
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r0, #1
	bl WaitFrames
	movs r1, #196
	lsls r1, r1, #1
	adds r1, #255
	movs r0, #8
	movs r2, #2
	bl Func_08152404
.L_08147ccc:
	ldr r1, [sp, #52]
	movs r0, #0
	adds r1, #12
	str r0, [sp, #48]
	str r1, [sp, #12]
.L_08147cd6:
	ldr r3, .L_08147de4
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08147d6a
	ldr r2, [sp, #48]
	cmp r2, #48
	ble .L_08147d6a
	cmp r2, #159
	bgt .L_08147d6a
	ldr r3, [sp, #60]
	cmp r3, #0
	bne .L_08147d32
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	add r3, r10
	ldr r0, [r3]
	movs r1, #8
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #224
	add r3, r10
	ldr r0, [r3]
	movs r1, #9
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #232
	add r3, r10
	ldr r0, [r3]
	movs r1, #10
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #236
	add r3, r10
	ldr r0, [r3]
	movs r1, #11
	bl Animation_ApplyChildArgumentFar
.L_08147d32:
	ldr r6, [sp, #64]
	movs r7, #0
	ldr r3, [r6, #20]
	cmp r3, #0
	beq .L_08147d66
	movs r5, #36
.L_08147d3e:
	ldr r1, [sp, #64]
	movs r3, #0
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r1, #10
	movs r2, #5
	subs r3, #1
	bl Func_0814cd48
	ldr r3, [sp, #64]
	movs r1, #4
	ldrsh r0, [r5, r3]
	bl Func_08118088
	ldr r0, [sp, #64]
	adds r7, #1
	ldr r3, [r0, #20]
	adds r5, #2
	cmp r7, r3
	bne .L_08147d3e
.L_08147d66:
	movs r1, #160
	str r1, [sp, #48]
.L_08147d6a:
	bl Func_08014de4
	ldr r0, [sp, #52]
	ldr r1, [sp, #12]
	bl Func_080156e8
	ldr r2, [sp, #48]
	cmp r2, #178
	bne .L_08147d82
	movs r0, #134
	bl Func_08118088 + 0x60
.L_08147d82:
	ldr r3, [sp, #48]
	cmp r3, #128
	bne .L_08147d94
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	str r3, [r2]
.L_08147d94:
	ldr r6, [sp, #48]
	cmp r6, #176
	bne .L_08147df0
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #3
	str r3, [r2]
	movs r2, #238
	ldr r3, .L_08147de8
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	str r3, [r2]
	ldr r0, .L_08147dec
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08147dd0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_08147e22
	.2byte 0x0000
.L_08147dc8:
	.4byte 0x00000134
.L_08147dcc:
	.4byte 0x00000188
.L_08147dd0:
	.4byte IwramCopyWords
.L_08147dd4:
	.4byte Func_08143000
.L_08147dd8:
	.4byte 0xffcc0000
.L_08147ddc:
	.4byte 0xffc40000
.L_08147de0:
	.4byte Data_02010018
.L_08147de4:
	.4byte gInput
.L_08147de8:
	.4byte Data_02020202
.L_08147dec:
	.4byte 0x00000178
.L_08147df0:
	ldr r3, [sp, #48]
	subs r3, #160
	cmp r3, #15
	bhi .L_08147e22
	movs r3, #239
	lsls r3, r3, #7
	add r3, r10
	movs r2, #1
	str r2, [r3]
	movs r2, #238
	lsls r2, r2, #7
	ldr r3, .L_08148168
	adds r2, #132
	add r2, r10
	str r3, [r2]
	ldr r0, [sp, #48]
	cmp r0, #173
	ble .L_08147e18
	ldr r3, .L_0814816c
	str r3, [r2]
.L_08147e18:
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Func_08164a4c
.L_08147e22:
	ldr r3, [sp, #48]
	subs r3, #33
	cmp r3, #142
	bhi .L_08147eaa
	ldr r3, [sp, #48]
	movs r1, #0
	movs r2, #1
	mov r8, r1
	str r2, [sp, #20]
	cmp r3, #103
	ble .L_08147e3c
	movs r6, #8
	str r6, [sp, #20]
.L_08147e3c:
	ldr r1, .L_08148170
	ldr r6, .L_08148174
	movs r0, #127
	movs r7, #0
	mov r11, r0
	mov r9, r1
.L_08147e48:
	ldr r3, [r6, #24]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_08147e9e
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r6]
	movs r3, #224
	lsls r3, r3, #15
	str r3, [r6, #4]
	bl Random16
	movs r5, #3
	mov r1, r9
	mov r3, r11
	ands r5, r7
	ands r0, r3
	ldrb r3, [r1, r5]
	adds r0, r0, r3
	lsls r0, r0, #9
	str r0, [r6, #12]
	bl Random16
	mov r1, r9
	ldrb r3, [r1, r5]
	mov r2, r11
	ands r0, r2
	adds r0, r0, r3
	negs r0, r0
	movs r3, #0
	lsls r0, r0, #11
	str r0, [r6, #16]
	str r3, [r6, #24]
	ldr r3, [sp, #20]
	movs r2, #1
	add r8, r2
	cmp r8, r3
	beq .L_08147eaa
.L_08147e9e:
	movs r0, #128
	adds r7, #1
	lsls r0, r0, #2
	adds r6, #28
	cmp r7, r0
	bne .L_08147e48
.L_08147eaa:
	ldr r3, [sp, #48]
	subs r3, #41
	cmp r3, #86
	bhi .L_08147f7c
	ldr r2, [sp, #48]
	movs r3, #1
	movs r1, #0
	ands r3, r2
	str r1, [sp, #16]
	cmp r3, #0
	beq .L_08147f7c
	movs r5, #140
	movs r6, #76
	movs r3, #7
	lsls r5, r5, #3
	add r6, sp
	movs r7, #0
	mov r11, r3
	add r5, r10
	mov r9, r6
.L_08147ed2:
	ldr r3, [r5, #24]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_08147f74
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	mov r8, r3
	bl Random16
	movs r3, #248
	lsls r3, r3, #5
	movs r1, #156
	ldr r2, [sp, #60]
	adds r3, #255
	lsls r1, r1, #7
	ands r3, r0
	adds r1, #32
	adds r6, r3, r1
	cmp r2, #0
	bne .L_08147f30
	ldr r3, [sp, #24]
	cmp r3, #1
	bne .L_08147f1c
	bl Random16
	mov r1, r11
	ands r0, r1
	adds r0, #78
	movs r3, #156
	lsls r0, r0, #16
	lsls r3, r3, #15
	str r0, [r5]
	b .L_08147f48
.L_08147f1c:
	bl Random16
	mov r2, r11
	ands r0, r2
	adds r0, #78
	movs r3, #140
	lsls r0, r0, #16
	lsls r3, r3, #15
	str r0, [r5]
	b .L_08147f48
.L_08147f30:
	bl Random16
	mov r3, r11
	mov r1, r9
	ands r0, r3
	ldr r3, [r1]
	adds r0, r0, r3
	subs r0, #8
	lsls r0, r0, #16
	str r0, [r5]
	ldr r3, [r1, #4]
	lsls r3, r3, #16
.L_08147f48:
	str r3, [r5, #4]
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r5, #16]
	movs r3, #0
	str r3, [r5, #24]
	ldr r2, [sp, #16]
	adds r2, #1
	str r2, [sp, #16]
	cmp r2, #1
	beq .L_08147f7c
.L_08147f74:
	adds r7, #1
	adds r5, #28
	cmp r7, #24
	bne .L_08147ed2
.L_08147f7c:
	ldr r3, [sp, #48]
	cmp r3, #48
	bne .L_08147f88
	movs r0, #141
	bl Audio_PlayCue
.L_08147f88:
	ldr r6, [sp, #48]
	cmp r6, #128
	bne .L_08147f94
	movs r0, #145
	bl Audio_PlayCue
.L_08147f94:
	ldr r3, [sp, #48]
	subs r3, #129
	cmp r3, #46
	bhi .L_08148030
	movs r1, #76
	movs r0, #0
	add r1, sp
	mov r11, r0
	movs r7, #0
	mov r9, r1
	mov r5, r10
.L_08147faa:
	ldr r3, [r5, #24]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_08148028
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	mov r8, r3
	bl Random16
	movs r3, #248
	lsls r3, r3, #5
	adds r3, #255
	ands r3, r0
	ldr r1, [sp, #60]
	ldr r0, .L_08148178
	adds r6, r3, r0
	cmp r1, #0
	bne .L_08147ff0
	ldr r2, [sp, #24]
	cmp r2, #1
	bne .L_08147fe4
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5]
	b .L_08147ffc
.L_08147fe4:
	movs r3, #136
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #15
	b .L_08147ffc
.L_08147ff0:
	mov r0, r9
	ldr r3, [r0]
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, [r0, #4]
	lsls r3, r3, #16
.L_08147ffc:
	str r3, [r5, #4]
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	movs r1, #1
	asrs r3, r3, #6
	add r11, r1
	str r3, [r5, #16]
	mov r2, r11
	movs r3, #0
	str r3, [r5, #24]
	cmp r2, #1
	beq .L_08148030
.L_08148028:
	adds r7, #1
	adds r5, #28
	cmp r7, #24
	bne .L_08147faa
.L_08148030:
	ldr r3, [sp, #48]
	cmp r3, #175
	bgt .L_08148096
	movs r7, #0
	mov r5, r10
.L_0814803a:
	ldr r1, [r5, #24]
	cmp r1, #0
	blt .L_0814808e
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r6, #224
	lsls r6, r6, #3
	add r1, r10
	movs r0, #2
	ldrsh r2, [r5, r0]
	adds r1, r1, r6
	movs r6, #6
	ldrsh r3, [r5, r6]
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	subs r2, #16
	subs r3, #32
	str r0, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #12]
	ldr r0, [sp, #28]
	adds r2, r0, #0
	muls r2, r3
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #24
	bne .L_0814808e
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_0814808e:
	adds r7, #1
	adds r5, #28
	cmp r7, #24
	bne .L_0814803a
.L_08148096:
	movs r5, #140
	lsls r5, r5, #3
	movs r7, #0
	movs r6, #1
	add r5, r10
.L_081480a0:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_081480ee
	movs r1, #2
	ldrsh r2, [r5, r1]
	movs r1, #2
	movs r0, #6
	ldrsh r3, [r5, r0]
	str r1, [sp, #4]
	str r6, [sp, #0]
	ldr r1, [sp, #40]
	subs r3, #1
	ldr r4, [r1, #4]
	ldr r0, [sp, #56]
	ldr r1, .L_0814817c
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #12]
	ldr r0, [sp, #28]
	ldr r1, .L_08148180
	adds r2, r0, #0
	muls r2, r3
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	adds r2, r2, r1
	adds r3, #1
	str r2, [r5, #16]
	str r3, [r5, #24]
	cmp r3, #48
	bne .L_081480ee
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_081480ee:
	adds r7, #1
	adds r5, #28
	cmp r7, #24
	bne .L_081480a0
	ldr r2, [sp, #48]
	cmp r2, #175
	bgt .L_081481ba
	ldr r5, .L_08148174
	movs r7, #0
.L_08148100:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_081481ae
	ldr r2, .L_08148184
	movs r3, #3
	ands r3, r7
	ldrb r0, [r2, r3]
	ldr r2, .L_08148188
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #44]
	movs r6, #2
	ldrsh r2, [r5, r6]
	adds r1, r3, r1
	lsrs r3, r0, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #56]
	ldr r4, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	ldr r1, [r5, #12]
	ldr r0, [sp, #28]
	ldr r3, [r5]
	adds r2, r0, #0
	muls r2, r1
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [sp, #48]
	cmp r3, #128
	ble .L_08148194
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_08148160
	ldr r6, .L_0814818c
	adds r3, r1, r6
	str r3, [r5, #12]
	b .L_0814819a
.L_08148160:
	ldr r0, .L_08148190
	adds r3, r1, r0
	str r3, [r5, #12]
	b .L_0814819a
.L_08148168:
	.4byte 0x10101010
.L_0814816c:
	.4byte 0x3f3f3f3f
.L_08148170:
	.4byte Data_08197968
.L_08148174:
	.4byte gMapCellBuffer
.L_08148178:
	.4byte 0xffffb1e0
.L_0814817c:
	.4byte Data_0819796c
.L_08148180:
	.4byte 0xfffffc00
.L_08148184:
	.4byte Data_0819796e
.L_08148188:
	.4byte Data_08197410
.L_0814818c:
	.4byte 0xffff8000
.L_08148190:
	.4byte 0xffffe000
.L_08148194:
	ldr r1, .L_08148438
	adds r3, r2, r1
	str r3, [r5, #16]
.L_0814819a:
	ldr r3, [r5, #24]
	movs r2, #128
	adds r3, #1
	lsls r2, r2, #1
	str r3, [r5, #24]
	cmp r3, r2
	bne .L_081481ae
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_081481ae:
	movs r3, #128
	adds r7, #1
	lsls r3, r3, #2
	adds r5, #28
	cmp r7, r3
	bne .L_08148100
.L_081481ba:
	ldr r6, [sp, #48]
	cmp r6, #128
	bne .L_081481cc
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #48
	str r3, [r2]
.L_081481cc:
	ldr r0, [sp, #60]
	cmp r0, #0
	bne .L_081481e4
	ldr r1, [sp, #48]
	cmp r1, #48
	bne .L_081481e4
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #8
	str r3, [r2]
.L_081481e4:
	ldr r3, [sp, #48]
	subs r3, #40
	cmp r3, #7
	bhi .L_081481fe
	ldr r2, [sp, #32]
	ldr r6, [sp, #36]
	ldr r3, .L_0814843c
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r2, r3
	adds r0, r6, r0
	str r3, [sp, #32]
	str r0, [sp, #36]
.L_081481fe:
	ldr r1, [sp, #60]
	cmp r1, #0
	bne .L_081482ac
	ldr r2, [sp, #48]
	cmp r2, #128
	bne .L_0814824a
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	add r3, r10
	ldr r0, [r3]
	movs r1, #8
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #224
	add r3, r10
	ldr r0, [r3]
	movs r1, #9
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #232
	add r3, r10
	ldr r0, [r3]
	movs r1, #10
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #236
	add r3, r10
	ldr r0, [r3]
	movs r1, #11
	bl Animation_ApplyChildArgumentFar
.L_0814824a:
	ldr r3, [sp, #48]
	cmp r3, #176
	bne .L_08148290
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	add r3, r10
	ldr r0, [r3]
	movs r1, #0
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #224
	add r3, r10
	ldr r0, [r3]
	movs r1, #1
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #232
	add r3, r10
	ldr r0, [r3]
	movs r1, #3
	bl Animation_ApplyChildArgumentFar
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #236
	add r3, r10
	ldr r0, [r3]
	movs r1, #4
	bl Animation_ApplyChildArgumentFar
.L_08148290:
	ldr r6, [sp, #24]
	cmp r6, #1
	bne .L_081482a2
	movs r0, #6
	ldr r1, [sp, #32]
	ldr r2, [sp, #36]
	bl Func_0816442c
	b .L_081482ac
.L_081482a2:
	movs r0, #3
	ldr r1, [sp, #32]
	ldr r2, [sp, #36]
	bl Func_0816442c
.L_081482ac:
	ldr r0, [sp, #48]
	cmp r0, #138
	bne .L_081482e4
	ldr r1, [sp, #64]
	movs r7, #0
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_081482e4
	movs r5, #36
.L_081482be:
	ldr r2, [sp, #64]
	movs r1, #10
	ldrsh r0, [r5, r2]
	movs r3, #0
	str r3, [sp, #0]
	movs r2, #5
	subs r3, #1
	bl Func_0814cd48
	ldr r6, [sp, #64]
	adds r7, #1
	ldrsh r0, [r5, r6]
	movs r1, #4
	bl Func_08118088
	ldr r3, [r6, #20]
	adds r5, #2
	cmp r7, r3
	bne .L_081482be
.L_081482e4:
	ldr r2, [sp, #48]
	cmp r2, #175
	bgt .L_081483ac
	movs r5, #168
	movs r3, #0
	movs r6, #20
	movs r0, #5
	lsls r5, r5, #2
	movs r7, #0
	mov r11, r3
	mov r8, r6
	mov r9, r0
	add r5, r10
.L_081482fe:
	ldr r6, [r5, #4]
	cmp r6, #55
	ble .L_0814834c
	ldr r3, [r5, #24]
	cmp r3, #11
	bhi .L_08148340
	lsrs r4, r3, #31
	ldr r2, .L_08148440
	adds r4, r3, r4
	asrs r4, r4, #1
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_08148444
	movs r2, #220
	ldrb r0, [r3, r4]
	lsls r2, r2, #6
	add r1, r10
	adds r1, r1, r2
	ldr r2, [r5]
	lsrs r3, r0, #1
	subs r2, r2, r3
	ldr r3, .L_08148448
	ldrb r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_0814844c
	adds r3, r6, r3
	ldrb r0, [r0, r4]
	ldr r4, [sp, #68]
	str r0, [sp, #4]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
.L_08148340:
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #12
	bne .L_081483a4
	mov r3, r11
	b .L_081483a2
.L_0814834c:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_081483a0
	ldr r0, [sp, #28]
	ldr r2, [r5]
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	subs r2, r2, r3
	adds r3, r6, #6
	str r2, [r5]
	str r3, [r5, #4]
	ldr r1, [sp, #48]
	movs r4, #10
	cmp r1, #47
	bgt .L_0814837a
	cmp r3, #55
	ble .L_0814837a
	movs r0, #136
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r4, [sp, #8]
.L_0814837a:
	ldr r2, .L_08148450
	mov r3, r8
	subs r3, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #44]
	mov r3, r9
	adds r1, r2, r1
	ldr r2, [r5]
	mov r6, r8
	subs r2, r2, r3
	ldr r3, [r5, #4]
	ldr r0, [sp, #56]
	str r4, [sp, #0]
	adds r3, #30
	str r6, [sp, #4]
	ldr r4, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	b .L_081483a4
.L_081483a0:
	adds r3, #1
.L_081483a2:
	str r3, [r5, #24]
.L_081483a4:
	adds r7, #1
	adds r5, #28
	cmp r7, #16
	bne .L_081482fe
.L_081483ac:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #48]
	adds r0, #1
	str r0, [sp, #48]
	cmp r0, #208
	beq .L_081483d6
	b .L_08147cd6
.L_081483d6:
	ldr r0, .L_08148454
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #60]
	cmp r1, #0
	bne .L_08148426
	ldr r2, [sp, #24]
	cmp r2, #1
	bne .L_08148400
	movs r0, #6
	ldr r1, [sp, #32]
	ldr r2, [sp, #36]
	bl Func_0816467c
	b .L_0814840a
.L_08148400:
	movs r0, #3
	ldr r1, [sp, #32]
	ldr r2, [sp, #36]
	bl Func_0816467c
.L_0814840a:
	ldr r3, [sp, #60]
	cmp r3, #0
	bne .L_08148426
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #220
	movs r7, #0
	add r5, r10
.L_0814841a:
	ldmia r5!, {r0}
	adds r7, #1
	bl Func_08020040 + 0x8
	cmp r7, #8
	bne .L_0814841a
.L_08148426:
	bl Func_08143bb8
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08148438:
	.4byte 0xfffffc00
.L_0814843c:
	.4byte 0xfff80000
.L_08148440:
	.4byte Data_08197984
.L_08148444:
	.4byte Data_08197972
.L_08148448:
	.4byte Data_0819797e
.L_0814844c:
	.4byte Data_08197978
.L_08148450:
	.4byte Data_08197410
.L_08148454:
	.4byte Func_08143000
