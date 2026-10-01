.syntax unified
	.thumb
	.global Func_0814fab8
	.thumb_func
Func_0814fab8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #152
	str r0, [sp, #76]
	movs r3, #192
	lsls r3, r3, #18
	mov r11, r1
	ldr r1, [r3, #92]
	movs r2, #0
	str r1, [sp, #72]
	movs r0, #0
	ldr r3, [r3, #96]
	str r2, [sp, #44]
	str r3, [sp, #68]
	bl BattleFx_BeginCanvasLayer
	ldr r3, [sp, #76]
	ldr r2, [r3, #28]
	cmp r2, #1
	bne .L_0814fb64
	mov r4, r11
	cmp r4, #3
	beq .L_0814faf4
	cmp r4, #6
	bne .L_0814fafa
.L_0814faf4:
	ldr r5, [sp, #76]
	movs r3, #32
	b .L_0814fb20
.L_0814fafa:
	mov r3, r11
	subs r3, #7
	cmp r3, #1
	bls .L_0814fb08
	mov r1, r11
	cmp r1, #11
	bne .L_0814fb12
.L_0814fb08:
	ldr r3, [sp, #76]
	add r5, sp, #140
	ldr r1, [r3, #4]
	movs r3, #33
	b .L_0814fb24
.L_0814fb12:
	mov r4, r11
	cmp r4, #2
	beq .L_0814fb1c
	cmp r4, #4
	bne .L_0814fb36
.L_0814fb1c:
	ldr r5, [sp, #76]
	movs r3, #35
.L_0814fb20:
	ldr r1, [r5, #4]
	add r5, sp, #140
.L_0814fb24:
	eors r1, r2
	lsls r1, r1, #4
	orrs r1, r3
	ldr r0, [sp, #76]
	add r3, sp, #128
	adds r2, r5, #0
	bl Func_0815585c
	b .L_0814fb58
.L_0814fb36:
	mov r1, r11
	cmp r1, #10
	beq .L_0814fb56
	ldr r3, [sp, #76]
	add r5, sp, #140
	ldr r1, [r3, #4]
	movs r3, #34
	eors r1, r2
	lsls r1, r1, #4
	orrs r1, r3
	ldr r0, [sp, #76]
	add r3, sp, #128
	adds r2, r5, #0
	bl Func_0815585c
	b .L_0814fb58
.L_0814fb56:
	add r5, sp, #140
.L_0814fb58:
	ldr r0, [r5]
	movs r1, #5
	lsls r0, r0, #2
	bl Math_Div
	str r0, [r5]
.L_0814fb64:
	mov r4, r11
	cmp r4, #10
	bne .L_0814fb80
	ldr r5, [sp, #72]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #180
	adds r2, r5, r1
	movs r3, #24
	str r3, [r2]
	movs r3, #53
	str r3, [sp, #44]
	movs r2, #0
	mov r11, r2
.L_0814fb80:
	ldr r3, .L_0814fbb8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r4, [sp, #72]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r4, r5
	ldr r0, .L_0814fbbc
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #208
	ldr r2, [sp, #72]
	lsls r3, r3, #4
	adds r3, #14
	adds r1, r2, r3
	ldr r0, .L_0814fbc0
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	mov r4, r11
	cmp r4, #3
	beq .L_0814fbd0
	b .L_0814fbc4
.L_0814fbb8:
	.4byte 0x000000cc
.L_0814fbbc:
	.4byte 0x00000137
.L_0814fbc0:
	.4byte 0x0000017b
.L_0814fbc4:
	cmp r4, #5
	beq .L_0814fbd0
	cmp r4, #8
	beq .L_0814fbd0
	cmp r4, #12
	bne .L_0814fbf8
.L_0814fbd0:
	ldr r5, [sp, #72]
	movs r2, #200
	lsls r2, r2, #6
	adds r2, #142
	adds r1, r5, r2
	movs r3, #1
	ldr r0, .L_0814fe00
	movs r2, #1
	bl Resource_LoadAndDecompress
	mov r3, r11
	cmp r3, #3
	bne .L_0814fbee
	ldr r0, .L_0814fe04
	b .L_0814fc30
.L_0814fbee:
	mov r4, r11
	cmp r4, #5
	beq .L_0814fc2e
	ldr r0, .L_0814fe08
	b .L_0814fc30
.L_0814fbf8:
	mov r5, r11
	cmp r5, #4
	bne .L_0814fc14
	ldr r2, [sp, #72]
	movs r3, #200
	lsls r3, r3, #6
	adds r3, #142
	adds r1, r2, r3
	ldr r0, .L_0814fe0c
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_0814fcc2
.L_0814fc14:
	mov r4, r11
	cmp r4, #9
	bne .L_0814fc44
	ldr r5, [sp, #72]
	movs r2, #200
	lsls r2, r2, #6
	adds r2, #142
	adds r1, r5, r2
	movs r3, #0
	movs r2, #1
	ldr r0, .L_0814fe0c
	bl Resource_LoadAndDecompress
.L_0814fc2e:
	ldr r0, .L_0814fe10
.L_0814fc30:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0814fe14
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0814fcc2
.L_0814fc44:
	mov r3, r11
	cmp r3, #0
	bne .L_0814fc60
	ldr r4, [sp, #72]
	movs r5, #200
	lsls r5, r5, #6
	adds r5, #142
	ldr r0, .L_0814fe18
	adds r1, r4, r5
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_0814fc74
.L_0814fc60:
	ldr r2, [sp, #72]
	movs r3, #200
	lsls r3, r3, #6
	adds r3, #142
	adds r1, r2, r3
	ldr r0, .L_0814fe1c
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_0814fc74:
	mov r4, r11
	cmp r4, #0
	bne .L_0814fc7e
	ldr r0, .L_0814fe10
	b .L_0814fcb0
.L_0814fc7e:
	mov r5, r11
	cmp r5, #2
	beq .L_0814fc88
	cmp r5, #4
	bne .L_0814fc8c
.L_0814fc88:
	ldr r0, .L_0814fe20
	b .L_0814fcb0
.L_0814fc8c:
	mov r1, r11
	cmp r1, #6
	bne .L_0814fc96
	ldr r0, .L_0814fe04
	b .L_0814fcb0
.L_0814fc96:
	mov r2, r11
	cmp r2, #7
	beq .L_0814fca0
	cmp r2, #11
	bne .L_0814fca4
.L_0814fca0:
	ldr r0, .L_0814fe08
	b .L_0814fcb0
.L_0814fca4:
	mov r3, r11
	cmp r3, #1
	bne .L_0814fcae
	ldr r0, .L_0814fe10
	b .L_0814fcb0
.L_0814fcae:
	ldr r0, .L_0814fe24
.L_0814fcb0:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0814fe14
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0814fcc2:
	mov r4, r11
	cmp r4, #3
	beq .L_0814fccc
	cmp r4, #6
	bne .L_0814fcda
.L_0814fccc:
	ldr r5, [sp, #72]
	movs r2, #216
	lsls r2, r2, #7
	adds r2, #192
	adds r1, r5, r2
	ldr r0, .L_0814fe04
	b .L_0814fd18
.L_0814fcda:
	mov r3, r11
	cmp r3, #12
	bne .L_0814fce6
	bl Func_0815b410
	b .L_0814fd36
.L_0814fce6:
	mov r4, r11
	cmp r4, #2
	beq .L_0814fcf0
	cmp r4, #4
	bne .L_0814fcfe
.L_0814fcf0:
	ldr r5, [sp, #72]
	movs r2, #216
	lsls r2, r2, #7
	adds r2, #192
	adds r1, r5, r2
	ldr r0, .L_0814fe20
	b .L_0814fd18
.L_0814fcfe:
	mov r3, r11
	subs r3, #7
	cmp r3, #1
	bls .L_0814fd0c
	mov r3, r11
	cmp r3, #11
	bne .L_0814fd22
.L_0814fd0c:
	ldr r4, [sp, #72]
	movs r5, #216
	lsls r5, r5, #7
	adds r5, #192
	ldr r0, .L_0814fe08
	adds r1, r4, r5
.L_0814fd18:
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_0814fd36
.L_0814fd22:
	ldr r2, [sp, #72]
	movs r3, #216
	lsls r3, r3, #7
	adds r3, #192
	adds r1, r2, r3
	ldr r0, .L_0814fe10
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_0814fd36:
	ldr r5, .L_0814fe28
	movs r4, #0
	mov r10, r4
.L_0814fd3c:
	bl Random16
	movs r1, #200
	bl Math_ModU
	subs r0, #100
	lsls r0, r0, #14
	str r0, [r5]
	bl Random16
	movs r1, #200
	bl Math_ModU
	subs r0, #100
	lsls r0, r0, #15
	str r0, [r5, #4]
	bl Random16
	movs r1, #200
	bl Math_ModU
	movs r1, #1
	subs r0, #100
	movs r2, #128
	lsls r0, r0, #14
	movs r3, #0
	add r10, r1
	lsls r2, r2, #2
	str r0, [r5, #8]
	str r3, [r5, #24]
	adds r5, #28
	cmp r10, r2
	bne .L_0814fd3c
	ldr r4, [sp, #76]
	ldr r3, [r4, #20]
	cmp r3, #1
	bne .L_0814fda4
	movs r5, #36
	ldrsh r0, [r4, r5]
	add r5, sp, #116
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r0, [r5]
	movs r1, #5
	negs r0, r0
	lsls r0, r0, #2
	bl Math_Div
	adds r0, #64
	str r0, [sp, #52]
	b .L_0814fdb6
.L_0814fda4:
	ldr r1, [sp, #76]
	movs r2, #64
	ldr r3, [r1, #4]
	negs r2, r2
	str r2, [sp, #52]
	cmp r3, #1
	beq .L_0814fdb6
	movs r3, #0
	str r3, [sp, #52]
.L_0814fdb6:
	ldr r4, [sp, #52]
	movs r2, #128
	lsls r2, r2, #19
	lsls r3, r4, #8
	adds r2, #40
	str r3, [r2]
	ldr r5, [sp, #72]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r5, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r5, r3
	movs r1, #200
	movs r3, #50
	str r3, [r2]
	ldr r0, .L_0814fe2c
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r4, [sp, #76]
	movs r5, #7
	ldr r3, [r4, #4]
	str r5, [sp, #48]
	cmp r3, #1
	beq .L_0814fdf4
	movs r1, #3
	str r1, [sp, #48]
.L_0814fdf4:
	ldr r4, [sp, #76]
	ldr r2, [sp, #44]
	ldr r3, [r4, #20]
	mov r9, r2
	b .L_08150480
	.2byte 0x0000
.L_0814fe00:
	.4byte 0x00000174
.L_0814fe04:
	.4byte 0x00000156
.L_0814fe08:
	.4byte 0x00000154
.L_0814fe0c:
	.4byte 0x0000016a
.L_0814fe10:
	.4byte 0x00000150
.L_0814fe14:
	.4byte IwramCopyWords
.L_0814fe18:
	.4byte 0x0000015f
.L_0814fe1c:
	.4byte 0x0000015e
.L_0814fe20:
	.4byte 0x00000152
.L_0814fe24:
	.4byte 0x0000017f
.L_0814fe28:
	.4byte gMapCellBuffer
.L_0814fe2c:
	.4byte Func_08143000
.L_0814fe30:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	mov r5, r9
	str r3, [sp, #40]
	cmp r5, #0
	bne .L_0814fe44
	movs r0, #142
	bl Audio_PlayCue
.L_0814fe44:
	mov r1, r11
	cmp r1, #6
	bne .L_0814fe5a
	movs r3, #3
	mov r2, r9
	ands r3, r2
	cmp r3, #0
	bne .L_0814fe5a
	ldr r0, .L_0815006c
	bl Func_0815f0a0
.L_0814fe5a:
	mov r3, r11
	cmp r3, #11
	bne .L_0814fe70
	movs r3, #3
	mov r4, r9
	ands r3, r4
	cmp r3, #0
	bne .L_0814fe70
	ldr r0, .L_08150070
	bl Func_0815f0a0
.L_0814fe70:
	mov r5, r9
	cmp r5, #80
	bne .L_0814fe7c
	movs r0, #0
	bl Func_081180e8
.L_0814fe7c:
	ldr r1, [sp, #76]
	ldr r1, [r1, #28]
	mov r8, r1
	cmp r1, #1
	bne .L_0814ff44
	mov r2, r9
	lsls r6, r2, #11
	adds r0, r6, #0
	bl Trig_Sin
	add r5, sp, #140
	lsls r3, r0, #2
	ldr r2, [r5]
	adds r3, r3, r0
	ldr r4, [sp, #52]
	lsls r3, r3, #2
	asrs r3, r3, #16
	adds r3, r3, r2
	adds r3, r3, r4
	subs r3, #20
	adds r0, r6, #0
	mov r10, r3
	bl Trig_Cos
	ldr r3, [r5, #4]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, [sp, #48]
	movs r4, #4
	mov r5, r8
	eors r3, r4
	adds r7, r0, #0
	movs r1, #7
	movs r2, #7
	movs r0, #104
	str r5, [sp, #0]
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r4, #4
	str r3, [sp, #60]
	mov r8, r3
	ldr r3, [sp, #48]
	movs r5, #0
	eors r3, r4
	movs r0, #188
	movs r1, #7
	movs r2, #7
	str r5, [sp, #0]
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [r3]
	mov r5, r9
	subs r7, #24
	str r4, [sp, #64]
	cmp r5, #32
	ble .L_0814ff02
	lsls r3, r5, #1
	subs r3, r7, r3
	adds r7, r3, #0
	adds r7, #64
.L_0814ff02:
	ldr r1, [sp, #72]
	movs r2, #216
	lsls r2, r2, #7
	adds r2, #192
	adds r6, r1, r2
	movs r5, #40
	adds r3, r7, #0
	str r4, [sp, #8]
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #68]
	adds r1, r6, #0
	mov r2, r10
	mov lr, r8
	.2byte 0xf800
	mov r3, r9
	ldr r4, [sp, #8]
	cmp r3, #3
	bgt .L_0814ff38
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #68]
	adds r1, r6, #0
	mov r2, r10
	adds r3, r7, #0
	mov lr, r4
	.2byte 0xf800
.L_0814ff38:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_0814ff44:
	movs r4, #0
	str r4, [sp, #56]
	ldr r5, [sp, #76]
	ldr r3, [r5, #20]
	cmp r3, #0
	bne .L_0814ff52
	b .L_08150460
.L_0814ff52:
	ldr r1, [sp, #40]
	mov r2, sp
	adds r1, #12
	adds r2, #80
	mov r3, sp
	mov r4, sp
	mov r12, r9
	str r1, [sp, #24]
	str r2, [sp, #36]
	adds r3, #92
	adds r4, #104
	mov r5, r12
	movs r1, #0
	movs r2, #36
	str r3, [sp, #32]
	str r4, [sp, #28]
	str r5, [sp, #20]
	str r1, [sp, #16]
	str r2, [sp, #12]
.L_0814ff78:
	ldr r3, [sp, #12]
	ldr r5, [sp, #76]
	ldrsh r0, [r3, r5]
	bl GetBattleObjectSlotFar
	ldr r1, [sp, #12]
	ldr r3, [sp, #76]
	ldr r5, [r0]
	ldrsh r0, [r1, r3]
	bl Battle_GetObjectTableValueFar
	movs r1, #3
	lsls r0, r0, #1
	bl Math_Div
	ldr r3, [sp, #16]
	adds r6, r0, #0
	adds r3, #80
	cmp r9, r3
	bne .L_0814ffa6
	movs r0, #212
	bl Audio_PlayCue
.L_0814ffa6:
	bl Func_08014de4
	ldr r0, [sp, #40]
	ldr r1, [sp, #24]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	ldr r4, [sp, #36]
	str r3, [r4]
	str r6, [r4, #4]
	ldr r3, [r5, #16]
	str r3, [r4, #8]
	ldr r0, [sp, #36]
	bl SceneTransform_ApplyPosition
	ldr r5, [sp, #44]
	cmp r5, #0
	beq .L_0814ffec
	ldr r3, [sp, #16]
	adds r3, #53
	cmp r9, r3
	beq .L_0814ffd4
	b .L_08150188
.L_0814ffd4:
	ldr r1, [sp, #12]
	ldr r3, [sp, #76]
	ldrsh r0, [r1, r3]
	movs r3, #16
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	ldr r3, [sp, #56]
	bl Func_0814cd48
	b .L_08150188
.L_0814ffec:
	ldr r3, [sp, #16]
	adds r3, #48
	cmp r9, r3
	bne .L_0815000a
	ldr r1, [sp, #76]
	ldr r4, [sp, #12]
	movs r3, #16
	movs r2, #1
	ldrsh r0, [r4, r1]
	negs r2, r2
	str r3, [sp, #0]
	movs r1, #7
	ldr r3, [sp, #56]
	bl Func_0814cd48
.L_0815000a:
	ldr r2, [sp, #16]
	cmp r9, r2
	bgt .L_08150012
	b .L_08150188
.L_08150012:
	movs r3, #1
	str r3, [sp, #0]
	ldr r3, [sp, #48]
	movs r1, #7
	movs r2, #7
	movs r0, #104
	bl Func_08196404
	movs r4, #192
	lsls r4, r4, #18
	ldr r4, [r4, #104]
	movs r1, #7
	ldr r3, [sp, #48]
	movs r5, #0
	movs r0, #188
	movs r2, #7
	str r4, [sp, #60]
	str r5, [sp, #0]
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	mov r1, r11
	str r3, [sp, #64]
	cmp r1, #0
	bne .L_08150056
	mov r2, r9
	negs r0, r2
	lsls r0, r0, #10
	bl SceneTransform_ApplyPitch
	b .L_08150084
.L_08150056:
	mov r3, r11
	cmp r3, #1
	beq .L_08150084
	cmp r3, #2
	bne .L_08150074
	mov r4, r9
	lsls r0, r4, #10
	bl Func_08015068
	b .L_08150084
	.2byte 0x0000
.L_0815006c:
	.4byte 0x000000b9
.L_08150070:
	.4byte 0x00000152
.L_08150074:
	mov r1, r9
	lsls r5, r1, #10
	adds r0, r5, #0
	bl Func_08015068
	adds r0, r5, #0
	bl Func_080150e4
.L_08150084:
	ldr r3, [sp, #56]
	movs r2, #0
	mov r10, r2
	ldr r4, [sp, #32]
	lsls r2, r3, #6
	ldr r5, .L_081503c0
	lsls r3, r3, #9
	subs r3, r3, r2
	lsls r3, r3, #2
	mov r8, r4
	adds r6, r3, r5
.L_0815009a:
	ldr r3, [sp, #16]
	add r3, r10
	cmp r9, r3
	ble .L_08150170
	ldr r3, [r6]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r6, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	ldr r3, [r6, #8]
	adds r0, r0, r2
	asrs r3, r3, #8
	adds r1, r3, #0
	muls r1, r3
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_081503c4
	mov lr, r3
	.2byte 0xf800
	asrs r7, r0, #9
	cmp r7, #0
	beq .L_08150170
	mov r1, r8
	adds r0, r6, #0
	bl Func_0815e1ec
	mov r2, r8
	ldr r0, [r2]
	movs r1, #5
	lsls r0, r0, #2
	bl Math_Div
	mov r4, r8
	ldr r3, [sp, #52]
	ldr r2, [r4, #8]
	movs r5, #58
	adds r0, r0, r3
	adds r5, #255
	str r0, [r4]
	cmp r2, r5
	bgt .L_081500fc
	ldr r1, [sp, #32]
	movs r3, #157
	lsls r3, r3, #1
	str r3, [r1, #8]
	adds r2, r3, #0
.L_081500fc:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #122
	cmp r2, r3
	ble .L_0815010c
	ldr r2, [sp, #32]
	str r3, [r2, #8]
	adds r2, r3, #0
.L_0815010c:
	ldr r4, .L_081503c8
	adds r3, r2, r4
	cmp r3, #0
	bge .L_08150118
	adds r3, r2, #0
	subs r3, #251
.L_08150118:
	asrs r3, r3, #6
	movs r0, #6
	subs r0, r0, r3
	ldr r2, .L_081503cc
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r5, [sp, #72]
	ldr r3, [sp, #32]
	movs r2, #224
	adds r1, r5, r1
	lsls r2, r2, #3
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r3, [r3, #4]
	subs r2, r2, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #68]
	ldr r4, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	ldr r5, [r6]
	adds r1, r7, #0
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6]
	ldr r5, [r6, #4]
	adds r1, r7, #0
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6, #4]
	ldr r5, [r6, #8]
	adds r1, r7, #0
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6, #8]
.L_08150170:
	movs r5, #1
	add r10, r5
	mov r1, r10
	adds r6, #28
	cmp r1, #32
	bne .L_0815009a
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_08150188:
	ldr r3, [sp, #28]
	movs r2, #0
	str r2, [r3]
	str r2, [r3, #4]
	str r2, [r3, #8]
	add r4, sp, #92
	adds r1, r4, #0
	ldr r0, [sp, #28]
	bl Func_0815e1ec
	ldr r0, [sp, #92]
	movs r1, #5
	lsls r0, r0, #2
	bl Math_Div
	ldr r5, [sp, #52]
	ldr r3, [sp, #16]
	adds r0, r0, r5
	adds r3, #52
	str r0, [sp, #92]
	cmp r9, r3
	blt .L_0815021a
	ldr r3, [sp, #16]
	adds r3, #76
	cmp r9, r3
	bge .L_0815021a
	ldr r3, [sp, #20]
	subs r3, #52
	cmp r3, #0
	bge .L_081501c8
	ldr r3, [sp, #20]
	subs r3, #49
.L_081501c8:
	asrs r0, r3, #2
	movs r1, #6
	bl Math_Mod
	movs r1, #1
	adds r5, r0, #0
	str r1, [sp, #0]
	ldr r3, [sp, #48]
	movs r1, #7
	movs r2, #7
	movs r0, #104
	bl Func_08196404
	lsls r1, r5, #1
	adds r1, r1, r5
	ldr r3, [sp, #72]
	lsls r1, r1, #3
	adds r1, r1, r5
	movs r2, #192
	lsls r2, r2, #18
	lsls r1, r1, #6
	ldr r4, [r2, #104]
	adds r1, r3, r1
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	movs r5, #208
	lsls r5, r5, #4
	movs r0, #40
	adds r5, #14
	str r0, [sp, #0]
	str r0, [sp, #4]
	str r4, [sp, #60]
	adds r1, r1, r5
	subs r2, #20
	subs r3, #20
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_0815021a:
	mov r1, r11
	cmp r1, #0
	bne .L_0815028c
	ldr r3, [sp, #16]
	adds r3, #80
	cmp r9, r3
	bge .L_0815022a
	b .L_0815043e
.L_0815022a:
	ldr r3, [sp, #16]
	adds r3, #108
	cmp r9, r3
	blt .L_08150234
	b .L_0815043e
.L_08150234:
	ldr r3, [sp, #20]
	subs r3, #80
	cmp r3, #0
	bge .L_08150240
	ldr r3, [sp, #20]
	subs r3, #77
.L_08150240:
	asrs r0, r3, #2
	movs r1, #7
	bl Math_Mod
	movs r2, #1
	adds r5, r0, #0
	str r2, [sp, #0]
	ldr r3, [sp, #48]
	movs r1, #7
	movs r2, #7
	movs r0, #104
	bl Func_08196404
	lsls r1, r5, #4
	subs r1, r1, r5
	ldr r5, [sp, #72]
	movs r2, #200
	movs r3, #192
	lsls r1, r1, #6
	lsls r2, r2, #6
	lsls r3, r3, #18
	adds r2, #142
	adds r1, r5, r1
	ldr r4, [r3, #104]
	adds r1, r1, r2
	ldr r3, [sp, #96]
	ldr r2, [sp, #92]
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	str r4, [sp, #60]
	subs r2, #12
	subs r3, #20
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	b .L_081503b6
.L_0815028c:
	mov r3, r11
	cmp r3, #3
	beq .L_0815029e
	cmp r3, #5
	beq .L_0815029e
	cmp r3, #8
	beq .L_0815029e
	cmp r3, #12
	bne .L_08150308
.L_0815029e:
	ldr r3, [sp, #16]
	adds r3, #80
	cmp r9, r3
	bge .L_081502a8
	b .L_0815043e
.L_081502a8:
	ldr r3, [sp, #16]
	adds r3, #104
	cmp r9, r3
	blt .L_081502b2
	b .L_0815043e
.L_081502b2:
	ldr r3, [sp, #20]
	subs r3, #80
	cmp r3, #0
	bge .L_081502be
	ldr r3, [sp, #20]
	subs r3, #77
.L_081502be:
	asrs r0, r3, #2
	movs r1, #6
	bl Math_Mod
	movs r4, #1
	ldr r3, [sp, #48]
	adds r5, r0, #0
	movs r1, #7
	movs r2, #7
	movs r0, #104
	str r4, [sp, #0]
	bl Func_08196404
	ldr r2, [sp, #72]
	movs r3, #200
	movs r1, #192
	lsls r5, r5, #11
	lsls r3, r3, #6
	lsls r1, r1, #18
	adds r5, r2, r5
	adds r3, #142
	ldr r4, [r1, #104]
	adds r5, r5, r3
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	movs r1, #32
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	str r4, [sp, #60]
	subs r2, #16
	subs r3, #32
	ldr r0, [sp, #68]
	adds r1, r5, #0
	mov lr, r4
	.2byte 0xf800
	b .L_081503b6
.L_08150308:
	mov r4, r11
	cmp r4, #4
	beq .L_08150312
	cmp r4, #9
	bne .L_081503d0
.L_08150312:
	ldr r3, [sp, #16]
	adds r3, #80
	cmp r9, r3
	bge .L_0815031c
	b .L_0815043e
.L_0815031c:
	ldr r3, [sp, #16]
	adds r3, #104
	cmp r9, r3
	blt .L_08150326
	b .L_0815043e
.L_08150326:
	ldr r0, [sp, #20]
	movs r1, #6
	subs r0, #80
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	bl Math_Mod
	movs r1, #1
	str r1, [sp, #0]
	ldr r3, [sp, #48]
	adds r5, r0, #0
	movs r1, #7
	movs r2, #7
	movs r0, #104
	bl Func_08196404
	movs r2, #192
	movs r4, #1
	lsls r2, r2, #18
	ldr r2, [r2, #104]
	str r4, [sp, #0]
	ldr r4, [sp, #48]
	movs r3, #8
	str r2, [sp, #60]
	orrs r3, r4
	mov r8, r2
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [sp, #72]
	adds r3, #188
	ldr r3, [r3]
	movs r2, #200
	lsls r5, r5, #11
	lsls r2, r2, #6
	adds r5, r1, r5
	adds r2, #142
	str r3, [sp, #64]
	adds r5, r5, r2
	mov r10, r3
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	movs r4, #64
	movs r6, #32
	str r4, [sp, #0]
	str r4, [sp, #8]
	str r6, [sp, #4]
	ldr r0, [sp, #68]
	subs r2, #32
	subs r3, #24
	adds r1, r5, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	ldr r4, [sp, #8]
	subs r2, #32
	adds r3, #8
	str r4, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #68]
	adds r1, r5, #0
	mov lr, r10
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_081503b6:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	b .L_0815043e
	.2byte 0x0000
.L_081503c0:
	.4byte gMapCellBuffer
.L_081503c4:
	.4byte IwramFillWords + 0x74
.L_081503c8:
	.4byte 0xfffffec6
.L_081503cc:
	.4byte Data_08197424
.L_081503d0:
	ldr r3, [sp, #16]
	adds r3, #80
	cmp r9, r3
	blt .L_0815043e
	ldr r3, [sp, #16]
	adds r3, #104
	cmp r9, r3
	bge .L_0815043e
	ldr r3, [sp, #20]
	subs r3, #80
	cmp r3, #0
	bge .L_081503ec
	ldr r3, [sp, #20]
	subs r3, #77
.L_081503ec:
	asrs r0, r3, #2
	movs r1, #6
	bl Math_Mod
	movs r3, #0
	adds r5, r0, #0
	str r3, [sp, #0]
	movs r1, #7
	ldr r3, [sp, #48]
	movs r2, #7
	movs r0, #104
	bl Func_08196404
	movs r1, #192
	lsls r1, r1, #18
	ldr r4, [r1, #104]
	lsls r1, r5, #1
	adds r1, r1, r5
	ldr r2, [sp, #72]
	lsls r1, r1, #3
	adds r1, r1, r5
	movs r3, #200
	lsls r1, r1, #6
	lsls r3, r3, #6
	adds r1, r2, r1
	adds r3, #142
	adds r1, r1, r3
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	movs r0, #40
	str r0, [sp, #0]
	str r0, [sp, #4]
	str r4, [sp, #60]
	subs r2, #20
	subs r3, #20
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_0815043e:
	ldr r4, [sp, #20]
	ldr r5, [sp, #16]
	ldr r1, [sp, #12]
	ldr r2, [sp, #56]
	subs r4, #8
	adds r5, #8
	adds r1, #2
	adds r2, #1
	str r4, [sp, #20]
	str r5, [sp, #16]
	str r1, [sp, #12]
	str r2, [sp, #56]
	ldr r4, [sp, #76]
	ldr r3, [r4, #20]
	cmp r2, r3
	beq .L_08150460
	b .L_0814ff78
.L_08150460:
	bl Func_081434f8
	movs r1, #240
	ldr r5, [sp, #72]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r5, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #76]
	movs r2, #1
	ldr r3, [r4, #20]
	add r9, r2
.L_08150480:
	lsls r3, r3, #3
	adds r3, #108
	cmp r9, r3
	beq .L_0815048a
	b .L_0814fe30
.L_0815048a:
	ldr r0, .L_081504a4
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #152
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081504a4:
	.4byte Func_08143000
