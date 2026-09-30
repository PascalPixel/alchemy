.syntax unified
	.thumb
	.global Func_08148458
	.thumb_func
Func_08148458:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	str r0, [sp, #48]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	movs r6, #224
	str r0, [sp, #44]
	movs r0, #0
	ldr r1, [r3, #92]
	lsls r6, r6, #3
	str r1, [sp, #40]
	ldr r3, [r3, #100]
	str r3, [sp, #36]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_081484bc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r2, sp
	adds r2, #52
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #32]
	bl Func_08144aac
	ldr r3, [sp, #40]
	ldr r0, .L_081484c0
	adds r1, r3, r6
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #208
	ldr r7, [sp, #40]
	lsls r2, r2, #4
	adds r2, #228
	adds r1, r7, r2
	ldr r0, .L_081484c4
	movs r2, #1
	movs r3, #0
	b .L_081484c8
.L_081484bc:
	.4byte 0x00001010
.L_081484c0:
	.4byte 0x0000012f
.L_081484c4:
	.4byte 0x00000146
.L_081484c8:
	bl Func_08157cf4
	ldr r0, .L_08148850
	ldr r1, [sp, #36]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r6, #238
	movs r3, #239
	lsls r3, r3, #7
	lsls r6, r6, #7
	adds r2, r7, r3
	adds r6, #132
	movs r3, #2
	str r3, [r2]
	movs r1, #200
	adds r2, r7, r6
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08148854
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_08148858
	ldr r2, .L_0814885c
	movs r0, #128
	movs r1, #176
	movs r7, #0
	lsls r0, r0, #17
	lsls r1, r1, #15
	str r3, [sp, #20]
	ldr r3, [sp, #40]
	str r7, [sp, #12]
	str r0, [sp, #24]
	str r1, [sp, #28]
	str r2, [sp, #16]
	movs r6, #0
	movs r2, #1
	mov r8, r6
	negs r2, r2
	adds r3, #24
.L_0814851c:
	movs r7, #1
	add r8, r7
	mov r0, r8
	str r2, [r3]
	adds r3, #28
	cmp r0, #64
	bne .L_0814851c
	ldr r2, [sp, #40]
	movs r3, #168
	movs r1, #0
	lsls r3, r3, #2
	mov r8, r1
	adds r5, r2, r3
.L_08148536:
	bl Random16
	movs r3, #127
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #56
	str r3, [r5, #4]
	bl Random16
	movs r6, #1
	movs r3, #15
	ands r3, r0
	add r8, r6
	negs r3, r3
	mov r7, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #16
	bne .L_08148536
	ldr r3, .L_08148860
	movs r0, #0
	movs r1, #1
	movs r2, #128
	mov r8, r0
	negs r1, r1
	lsls r2, r2, #3
.L_08148572:
	movs r6, #1
	add r8, r6
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_08148572
	ldr r7, [sp, #40]
	movs r0, #240
	lsls r0, r0, #7
	adds r0, #240
	adds r3, r7, r0
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r0, #1
	bl WaitFrames
	movs r1, #197
	lsls r1, r1, #1
	adds r1, #255
	movs r0, #12
	movs r2, #2
	bl Func_08152404
	movs r1, #0
	mov r9, r1
.L_081485a6:
	ldr r3, .L_08148864
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081485c0
	mov r2, r9
	cmp r2, #32
	ble .L_081485c0
	cmp r2, #97
	bgt .L_081485c0
	movs r3, #98
	mov r9, r3
.L_081485c0:
	mov r6, r9
	cmp r6, #120
	bne .L_081485cc
	movs r0, #134
	bl Func_081180e8
.L_081485cc:
	mov r7, r9
	cmp r7, #15
	bgt .L_081485d8
	ldr r0, [sp, #12]
	adds r0, #2
	str r0, [sp, #12]
.L_081485d8:
	mov r1, r9
	cmp r1, #99
	bgt .L_08148622
	ldr r3, [sp, #24]
	ldr r2, [sp, #16]
	ldr r0, [sp, #16]
	adds r2, r2, r3
	ldr r7, [sp, #28]
	ldr r6, [sp, #20]
	movs r3, #58
	muls r3, r0
	adds r6, r6, r7
	str r2, [sp, #24]
	str r6, [sp, #28]
	cmp r3, #0
	bge .L_081485fa
	adds r3, #63
.L_081485fa:
	ldr r1, [sp, #20]
	asrs r3, r3, #6
	str r3, [sp, #16]
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_0814860c
	adds r3, #63
.L_0814860c:
	asrs r3, r3, #6
	str r3, [sp, #20]
	ldr r2, [sp, #24]
	ldr r3, .L_08148868
	cmp r2, r3
	bgt .L_08148622
	ldr r6, [sp, #16]
	movs r7, #128
	lsls r7, r7, #8
	adds r7, r6, r7
	str r7, [sp, #16]
.L_08148622:
	movs r0, #1
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	bl Func_0816442c
	mov r0, r9
	cmp r0, #28
	bne .L_081486b6
	ldr r7, .L_0814886c
	movs r1, #0
	movs r2, #63
	mov r8, r1
	mov r10, r2
.L_0814863c:
	ldr r3, [r7, #24]
	movs r6, #1
	negs r6, r6
	cmp r3, r6
	bne .L_081486a8
	bl Random16
	adds r6, r0, #0
	mov r0, r10
	ands r6, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	movs r1, #128
	lsls r1, r1, #14
	asrs r3, r3, #3
	adds r3, r3, r1
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r2, #192
	lsls r2, r2, #15
	asrs r3, r3, #2
	adds r3, r3, r2
	str r3, [r7, #4]
	bl Random16
	mov r3, r10
	ands r0, r3
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r7, #12]
	bl Random16
	mov r6, r10
	ands r0, r6
	negs r0, r0
	subs r0, #8
	lsls r0, r0, #13
	movs r3, #0
	str r0, [r7, #16]
	str r3, [r7, #24]
.L_081486a8:
	movs r0, #1
	movs r1, #128
	add r8, r0
	lsls r1, r1, #1
	adds r7, #28
	cmp r8, r1
	bne .L_0814863c
.L_081486b6:
	mov r2, r9
	subs r2, #32
	str r2, [sp, #8]
	cmp r2, #47
	bhi .L_08148750
	ldr r7, .L_0814886c
	movs r3, #0
	movs r6, #63
	mov r11, r3
	mov r8, r3
	mov r10, r6
.L_081486cc:
	ldr r3, [r7, #24]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_08148742
	bl Random16
	mov r1, r10
	adds r6, r0, #0
	ands r6, r1
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	movs r2, #128
	lsls r2, r2, #14
	asrs r3, r3, #3
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r6, #192
	lsls r6, r6, #15
	asrs r3, r3, #2
	adds r3, r3, r6
	str r3, [r7, #4]
	bl Random16
	mov r1, r10
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r7, #12]
	bl Random16
	mov r2, r10
	movs r3, #0
	ands r0, r2
	negs r0, r0
	str r3, [r7, #24]
	movs r3, #1
	subs r0, #8
	add r11, r3
	lsls r0, r0, #13
	mov r6, r11
	str r0, [r7, #16]
	cmp r6, #16
	beq .L_08148750
.L_08148742:
	movs r0, #1
	movs r1, #128
	add r8, r0
	lsls r1, r1, #3
	adds r7, #28
	cmp r8, r1
	bne .L_081486cc
.L_08148750:
	mov r2, r9
	cmp r2, #0
	bne .L_0814875c
	movs r0, #164
	bl Audio_PlayCue
.L_0814875c:
	mov r3, r9
	cmp r3, #32
	bne .L_08148768
	movs r0, #145
	bl Audio_PlayCue
.L_08148768:
	mov r6, r9
	cmp r6, #80
	bne .L_08148774
	movs r0, #144
	bl Audio_PlayCue
.L_08148774:
	ldr r7, [sp, #8]
	cmp r7, #47
	bhi .L_081487e4
	ldr r2, [sp, #40]
	movs r6, #208
	movs r0, #0
	lsls r6, r6, #4
	adds r6, #228
	mov r8, r0
	ldr r0, .L_08148870
	adds r2, r2, r6
	mov r1, r9
	ldr r6, .L_08148874
	movs r7, #34
	lsls r3, r1, #4
	mov r11, r7
	mov r10, r2
	adds r7, r3, r0
.L_08148798:
	adds r0, r7, #0
	movs r1, #104
	bl Math_Mod
	ldrb r3, [r6, #1]
	ldrb r2, [r6]
	adds r5, r0, #0
	mov r1, r11
	movs r0, #104
	subs r3, r3, r5
	str r1, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #52]
	subs r2, #17
	subs r3, #104
	ldr r0, [sp, #44]
	mov r1, r10
	mov lr, r4
	.2byte 0xf800
	ldrb r2, [r6]
	ldrb r3, [r6, #1]
	mov r1, r11
	subs r2, #17
	subs r3, r3, r5
	str r1, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #52]
	ldr r0, [sp, #44]
	mov r1, r10
	mov lr, r4
	.2byte 0xf800
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r6, #2
	adds r7, #25
	cmp r3, #3
	bne .L_08148798
.L_081487e4:
	mov r6, r9
	cmp r6, #95
	bgt .L_0814882e
	ldr r0, [sp, #40]
	movs r1, #224
	lsls r1, r1, #3
	movs r7, #0
	adds r0, r0, r1
	mov r8, r7
	mov r10, r0
	movs r5, #32
	movs r7, #120
.L_081487fc:
	mov r2, r8
	lsls r1, r2, #5
	mov r2, r9
	cmp r2, #0
	bge .L_08148808
	adds r2, #3
.L_08148808:
	movs r3, #31
	ldr r6, [sp, #12]
	asrs r2, r2, #2
	ands r2, r3
	adds r2, r1, r2
	subs r2, #32
	mov r1, r10
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #52]
	ldr r0, [sp, #44]
	subs r3, r7, r6
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #5
	bne .L_081487fc
.L_0814882e:
	ldr r5, .L_0814886c
	movs r2, #0
	mov r8, r2
.L_08148834:
	ldr r3, [r5, #24]
	cmp r3, #0
	bge .L_0814883c
	b .L_0814894c
.L_0814883c:
	mov r0, r8
	movs r1, #3
	bl Math_Mod
	ldr r3, [r5, #16]
	adds r4, r0, #2
	cmp r3, #0
	ble .L_08148878
	adds r4, #2
	b .L_08148878
.L_08148850:
	.4byte 0x00000134
.L_08148854:
	.4byte Func_08143000
.L_08148858:
	.4byte 0xfffc0000
.L_0814885c:
	.4byte 0xfff00000
.L_08148860:
	.4byte Data_02010018
.L_08148864:
	.4byte gInput
.L_08148868:
	.4byte 0x0077ffff
.L_0814886c:
	.4byte gMapCellBuffer
.L_08148870:
	.4byte 0xffffff00
.L_08148874:
	.4byte Data_08197990
.L_08148878:
	mov r6, r9
	cmp r6, #68
	ble .L_08148884
	cmp r4, #5
	bgt .L_08148884
	movs r4, #6
.L_08148884:
	mov r7, r9
	cmp r7, #70
	ble .L_08148890
	cmp r4, #6
	bgt .L_08148890
	movs r4, #7
.L_08148890:
	mov r0, r9
	cmp r0, #72
	ble .L_0814889c
	cmp r4, #7
	bgt .L_0814889c
	movs r4, #8
.L_0814889c:
	mov r1, r9
	cmp r1, #74
	ble .L_081488a8
	cmp r4, #8
	bgt .L_081488a8
	movs r4, #9
.L_081488a8:
	mov r2, r9
	cmp r2, #76
	ble .L_081488b0
	movs r4, #10
.L_081488b0:
	movs r6, #4
	cmp r3, #0
	bgt .L_081488b8
	movs r6, #0
.L_081488b8:
	ldr r2, .L_08148a44
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #36]
	movs r7, #2
	ldrsh r2, [r5, r7]
	adds r1, r3, r1
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #4]
	str r4, [sp, #0]
	ldr r0, [sp, #32]
	subs r3, r3, r4
	ldr r4, [r6, r0]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	ldr r1, [r5, #16]
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r5, #4]
	mov r2, r9
	adds r3, r3, r1
	str r3, [r5, #4]
	cmp r2, #80
	ble .L_08148900
	ldr r6, .L_08148a48
	adds r3, r1, r6
	b .L_0814890e
.L_08148900:
	ldr r3, .L_08148a4c
	movs r2, #3
	mov r7, r8
	ands r2, r7
	lsls r2, r2, #2
	ldr r3, [r3, r2]
	adds r3, r1, r3
.L_0814890e:
	str r3, [r5, #16]
	ldr r2, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_0814891e
	adds r3, #63
.L_0814891e:
	ldr r2, [r5, #16]
	asrs r3, r3, #6
	str r3, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r2, r3, #1
	cmp r2, #0
	bge .L_08148930
	adds r2, #63
.L_08148930:
	ldr r3, [r5, #24]
	asrs r2, r2, #6
	adds r3, #1
	str r2, [r5, #16]
	str r3, [r5, #24]
	cmp r2, #0
	ble .L_0814894c
	movs r0, #6
	ldrsh r3, [r5, r0]
	cmp r3, #104
	ble .L_0814894c
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_0814894c:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r5, #28
	cmp r8, r2
	beq .L_0814895c
	b .L_08148834
.L_0814895c:
	mov r3, r9
	cmp r3, #79
	bgt .L_081489d6
	ldr r7, [sp, #48]
	movs r6, #0
	ldr r3, [r7, #20]
	mov r8, r6
	cmp r3, #0
	beq .L_081489d6
	adds r7, #36
.L_08148970:
	mov r0, r9
	cmp r0, #29
	ble .L_081489cc
	movs r1, #12
	bl Math_Mod
	adds r6, r0, #0
	cmp r6, #0
	bne .L_081489ac
	movs r1, #0
	ldrsh r0, [r7, r1]
	bl GetBattleObjectSlotFar
	movs r3, #1
	ldr r5, [r0]
	negs r3, r3
	movs r2, #0
	ldrsh r0, [r7, r2]
	movs r1, #7
	movs r2, #5
	str r6, [sp, #0]
	bl Func_0814cd48
	movs r3, #144
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r5, #72]
.L_081489ac:
	cmp r6, #6
	bne .L_081489c8
	movs r3, #0
	ldrsh r0, [r7, r3]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #0
	subs r3, #1
	movs r2, #5
	bl Func_0814cd48
	ldr r6, [sp, #48]
	ldr r3, [r6, #20]
	b .L_081489cc
.L_081489c8:
	ldr r0, [sp, #48]
	ldr r3, [r0, #20]
.L_081489cc:
	movs r1, #1
	add r8, r1
	adds r7, #2
	cmp r8, r3
	bne .L_08148970
.L_081489d6:
	ldr r3, [sp, #40]
	movs r6, #240
	lsls r6, r6, #7
	adds r6, #232
	adds r2, r3, r6
	movs r7, #1
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	add r9, r7
	bl WaitFrames
	mov r0, r9
	cmp r0, #124
	beq .L_081489f6
	b .L_081485a6
.L_081489f6:
	ldr r0, .L_08148a50
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r0, #1
	bl Func_0816467c
	movs r3, #238
	ldr r2, [sp, #40]
	lsls r3, r3, #7
	movs r1, #0
	adds r3, #220
	mov r8, r1
	adds r5, r2, r3
.L_08148a20:
	movs r6, #1
	add r8, r6
	ldmia r5!, {r0}
	mov r7, r8
	bl ResourceObject_ReleaseFar
	cmp r7, #12
	bne .L_08148a20
	bl Func_08143bb8
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08148a44:
	.4byte Data_08197410
.L_08148a48:
	.4byte 0xffff8000
.L_08148a4c:
	.4byte Data_08197998
.L_08148a50:
	.4byte Func_08143000
