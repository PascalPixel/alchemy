.syntax unified
	.thumb
	.global Func_08181910
	.thumb_func
Func_08181910:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	ldr r3, [r3, #96]
	sub sp, #64
	str r3, [sp, #40]
	mov r10, r0
	ldr r3, [r0, #4]
	mov r8, r1
	cmp r3, #0
	bne .L_08181940
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #1
	bl BattleFx_BeginTiledCanvas
	b .L_0818194a
.L_08181940:
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #1
	bl BattleFx_BeginTiledCanvasFilled
.L_0818194a:
	ldr r3, .L_08181978
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0818197c
	adds r2, #50
	strh r3, [r2]
	ldr r0, .L_08181980
	movs r2, #1
	movs r3, #1
	ldr r1, .L_08181984
	bl Resource_LoadAndDecompress
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08181988
	movs r0, #104
	movs r1, #35
	bl Func_081963ec
	b .L_08181990
.L_08181978:
	.4byte 0x00000100
.L_0818197c:
	.4byte 0x00000610
.L_08181980:
	.4byte 0x000000f5
.L_08181984:
	.4byte gMapCellBuffer
.L_08181988:
	movs r0, #104
	movs r1, #39
	bl Func_081963ec
.L_08181990:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r2, #239
	lsls r2, r2, #7
	str r3, [sp, #32]
	add r2, r8
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r8
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08181bbc
	bl Scheduler_AddOrUpdateCallback
	movs r3, #0
	str r3, [sp, #28]
	str r3, [sp, #24]
	str r3, [sp, #20]
	mov r2, sp
	adds r2, #52
	mov r1, r10
	movs r4, #36
	ldrsh r0, [r1, r4]
	adds r1, r2, #0
	str r2, [sp, #16]
	bl Func_0815e20c
	movs r3, #0
	mov r9, r3
.L_081819d6:
	movs r4, #0
	mov r1, r9
	str r4, [sp, #12]
	cmp r1, #0
	bne .L_08181a40
	movs r0, #206
	bl Audio_PlayCue
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08181a00
	movs r3, #128
	mov r4, r8
	lsls r3, r3, #14
	str r3, [r4]
	ldr r3, .L_08181bc0
	str r3, [r4, #4]
	ldr r3, .L_08181bc4
	str r3, [r4, #8]
	b .L_08181a0e
.L_08181a00:
	ldr r3, .L_08181bc8
	mov r1, r8
	str r3, [r1]
	ldr r3, .L_08181bc0
	str r3, [r1, #4]
	ldr r3, .L_08181bc4
	str r3, [r1, #8]
.L_08181a0e:
	movs r3, #0
	mov r2, r8
	str r3, [r2, #20]
	str r3, [r2, #24]
	movs r3, #192
	lsls r3, r3, #13
	str r3, [sp, #24]
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_08181a30
	ldr r1, [sp, #16]
	movs r2, #240
	ldr r3, [r1]
	lsls r2, r2, #12
	adds r3, #176
	b .L_08181a3a
.L_08181a30:
	ldr r4, [sp, #16]
	ldr r1, .L_08181bcc
	ldr r3, [r4]
	ldr r2, .L_08181bd0
	adds r3, r3, r1
.L_08181a3a:
	lsls r3, r3, #16
	str r3, [sp, #28]
	str r2, [sp, #20]
.L_08181a40:
	mov r3, r9
	cmp r3, #61
	ble .L_08181a6a
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_08181a5a
	ldr r1, [sp, #28]
	movs r2, #128
	lsls r2, r2, #10
	adds r1, r1, r2
	str r1, [sp, #28]
	b .L_08181a62
.L_08181a5a:
	ldr r3, [sp, #28]
	ldr r4, .L_08181bd4
	adds r3, r3, r4
	str r3, [sp, #28]
.L_08181a62:
	ldr r1, [sp, #24]
	ldr r2, .L_08181bd8
	adds r1, r1, r2
	str r1, [sp, #24]
.L_08181a6a:
	ldr r4, [sp, #20]
	ldr r3, [sp, #28]
	subs r3, r3, r4
	str r3, [sp, #28]
	lsls r3, r4, #4
	subs r3, r3, r4
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_08181a7e
	adds r3, #63
.L_08181a7e:
	ldr r1, [sp, #28]
	asrs r3, r3, #6
	asrs r2, r1, #16
	str r3, [sp, #20]
	cmp r2, #120
	ble .L_08181a92
	subs r2, #120
	str r2, [sp, #12]
	movs r2, #120
	b .L_08181a9a
.L_08181a92:
	cmp r2, #0
	bge .L_08181a9a
	str r2, [sp, #12]
	movs r2, #0
.L_08181a9a:
	movs r3, #128
	lsls r3, r3, #19
	negs r2, r2
	adds r3, #40
	lsls r2, r2, #8
	str r2, [r3]
	mov r3, r9
	subs r3, #24
	cmp r3, #39
	bhi .L_08181ad6
	mov r2, r10
	ldr r3, [r2, #4]
	movs r2, #64
	cmp r3, #0
	beq .L_08181aba
	movs r2, #5
.L_08181aba:
	ldr r3, [sp, #12]
	ldr r4, [sp, #24]
	movs r1, #59
	str r1, [sp, #0]
	movs r1, #89
	adds r2, r2, r3
	str r1, [sp, #4]
	asrs r3, r4, #16
	ldr r0, [sp, #40]
	ldr r1, .L_08181bdc
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	b .L_08181b16
.L_08181ad6:
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08181aee
	mov r2, r9
	lsls r3, r2, #1
	adds r2, r3, #0
	adds r2, #16
	cmp r2, #64
	ble .L_08181afc
	movs r2, #64
	b .L_08181afc
.L_08181aee:
	mov r3, r9
	lsls r2, r3, #1
	movs r3, #57
	subs r2, r3, r2
	cmp r2, #8
	bgt .L_08181afc
	movs r2, #9
.L_08181afc:
	ldr r1, [sp, #24]
	ldr r4, [sp, #12]
	asrs r3, r1, #16
	movs r1, #55
	str r1, [sp, #0]
	movs r1, #88
	adds r2, r2, r4
	str r1, [sp, #4]
	ldr r0, [sp, #40]
	ldr r1, .L_08181be0
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
.L_08181b16:
	movs r6, #0
	movs r7, #0
	mov r5, r8
.L_08181b1c:
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #0
	ble .L_08181c04
	cmp r3, #1
	bne .L_08181b2e
	str r7, [r5, #12]
	str r7, [r5, #16]
.L_08181b2e:
	ldr r2, [r5, #24]
	cmp r2, #24
	bne .L_08181b3e
	ldr r3, .L_08181be4
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #16]
.L_08181b3e:
	cmp r2, #33
	bne .L_08181b78
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #12]
	ldr r3, .L_08181be8
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	str r3, [r5, #16]
	add r2, r8
	movs r3, #4
	str r3, [r2]
	movs r1, #0
	movs r0, #133
	str r1, [sp, #20]
	bl Audio_PlayCue
	mov r3, r10
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	ldr r2, [r5, #24]
.L_08181b78:
	cmp r2, #35
	bne .L_08181b80
	str r7, [r5, #12]
	str r7, [r5, #16]
.L_08181b80:
	mov r4, r9
	cmp r4, #63
	ble .L_08181ba6
	ldr r1, .L_08181bec
	lsls r3, r4, #7
	adds r2, r3, r1
	ldr r4, .L_08181bf0
	ldr r3, [r5, #16]
	movs r1, #128
	adds r3, r3, r4
	lsls r1, r1, #3
	str r3, [r5, #16]
	cmp r2, r1
	ble .L_08181ba0
	movs r2, #128
	lsls r2, r2, #3
.L_08181ba0:
	ldr r3, [r5, #8]
	subs r3, r3, r2
	str r3, [r5, #8]
.L_08181ba6:
	ldr r3, [r5, #24]
	cmp r3, #8
	ble .L_08181c04
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08181bf4
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	b .L_08181bfa
.L_08181bbc:
	.4byte Func_08143000
.L_08181bc0:
	.4byte 0xffc00000
.L_08181bc4:
	.4byte 0xffffe3d0
.L_08181bc8:
	.4byte 0xffe00000
.L_08181bcc:
	.4byte 0xfffffed0
.L_08181bd0:
	.4byte 0xfff10000
.L_08181bd4:
	.4byte 0xfffe0000
.L_08181bd8:
	.4byte 0xfffb5556
.L_08181bdc:
	.4byte Data_020112e8
.L_08181be0:
	.4byte gMapCellBuffer
.L_08181be4:
	.4byte 0xfffc0000
.L_08181be8:
	.4byte 0xfff80000
.L_08181bec:
	.4byte 0xffffe000
.L_08181bf0:
	.4byte 0xffffc000
.L_08181bf4:
	ldr r3, [r5]
	ldr r2, [r5, #12]
	subs r3, r3, r2
.L_08181bfa:
	str r3, [r5]
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #4]
.L_08181c04:
	adds r6, #1
	adds r5, #28
	cmp r6, #1
	bne .L_08181b1c
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r11, r0
	movs r0, #1
	bl Func_081969f8
	mov r3, r9
	movs r4, #4
	subs r3, #50
	negs r4, r4
	adds r7, r0, #0
	movs r5, #0
	str r3, [sp, #8]
	cmp r3, r4
	bne .L_08181c32
	movs r0, #212
	bl Audio_PlayCue
.L_08181c32:
	ldr r1, [sp, #8]
	cmp r1, #0
	bne .L_08181c78
	movs r0, #134
	bl Func_081180e8
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r8
	movs r3, #12
	str r3, [r2]
	mov r3, r10
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r1, #128
	lsls r1, r1, #7
	ldr r3, .L_08181e74
	ldr r0, [sp, #40]
	ldr r2, .L_08181e78
	mov lr, r3
	.2byte 0xf800
	mov r1, r10
	movs r4, #36
	ldrsh r0, [r1, r4]
	movs r1, #4
	bl Func_08118088
.L_08181c78:
	mov r3, r8
	ldr r2, [r3, #24]
	mov r3, r9
	subs r3, #44
	cmp r3, #13
	bhi .L_08181c8c
	mov r4, r8
	ldr r3, [r4, #20]
	adds r3, #1
	str r3, [r4, #20]
.L_08181c8c:
	cmp r2, #0
	bge .L_08181c92
	b .L_08181e20
.L_08181c92:
	movs r6, #177
	lsls r6, r6, #8
	mov r1, r9
	adds r6, #224
	cmp r1, #63
	ble .L_08181ca8
	lsls r3, r1, #9
	movs r2, #128
	subs r3, r6, r3
	lsls r2, r2, #8
	adds r6, r3, r2
.L_08181ca8:
	cmp r6, #0
	ble .L_08181d6e
	movs r3, #9
	str r5, [r7, #20]
	str r3, [r7]
	add r5, sp, #44
	mov r3, r11
	str r3, [r7, #12]
	str r5, [r7, #16]
	bl Func_08014de4
	mov r1, r8
	ldr r4, [sp, #12]
	ldr r0, [r1]
	lsls r3, r4, #16
	adds r0, r0, r3
	movs r2, #0
	ldr r1, [r1, #4]
	bl Func_08015160
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_08181ce0
	movs r0, #128
	lsls r0, r0, #8
	bl Func_08015068
.L_08181ce0:
	mov r3, r8
	ldr r0, [r3, #8]
	bl Func_080150e4
	adds r0, r6, #0
	bl Func_0801521c
	movs r3, #5
	strb r3, [r5, #1]
	ldr r3, .L_08181e7c
	movs r4, #7
	str r3, [r5, #4]
	ldr r3, .L_08181e80
	strb r4, [r5]
	str r3, [r7, #8]
	bl Func_08014e38
	mov r1, r8
	ldr r0, [r1, #20]
	lsls r0, r0, #11
	bl Trig_Sin
	ldr r2, .L_08181e84
	asrs r0, r0, #3
	adds r0, r0, r2
	bl Func_080150e4
	mov r1, r11
	movs r2, #4
	ldr r0, .L_08181e88
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	bl Func_08014ea8
	ldr r3, .L_08181e8c
	mov r4, r8
	str r3, [r7, #8]
	movs r3, #7
	strb r3, [r5]
	movs r3, #6
	ldr r0, [r4, #20]
	strb r3, [r5, #1]
	ldr r3, .L_08181e90
	lsls r0, r0, #11
	str r3, [r5, #4]
	bl Trig_Sin
	adds r3, r0, #0
	movs r0, #128
	asrs r3, r3, #3
	lsls r0, r0, #6
	subs r0, r0, r3
	bl Func_080150e4
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r2, #0
	ldr r1, .L_08181e94
	bl Func_080151e4
	ldr r0, .L_08181e88
	mov r1, r11
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08181d6e:
	ldr r1, [sp, #8]
	cmp r1, #7
	bhi .L_08181e20
	mov r2, r9
	lsls r3, r2, #13
	ldr r2, .L_08181e98
	subs r6, r2, r3
	bl Func_08014de4
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_08181d94
	ldr r0, .L_08181e9c
	movs r1, #0
	movs r2, #0
	bl Func_08015160
	b .L_08181da0
.L_08181d94:
	movs r0, #128
	lsls r0, r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_08015160
.L_08181da0:
	adds r1, r6, #0
	adds r0, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #1
	bne .L_08181dba
	movs r0, #128
	lsls r0, r0, #8
	bl Func_08015068
.L_08181dba:
	movs r3, #6
	str r3, [r7]
	ldr r3, .L_08181ea0
	add r2, sp, #44
	str r3, [r7, #8]
	movs r3, #5
	strb r3, [r2]
	strb r3, [r2, #1]
	ldr r3, .L_08181ea4
	movs r5, #172
	str r3, [r2, #4]
	lsls r5, r5, #7
	movs r6, #0
	adds r5, #80
.L_08181dd6:
	bl Func_08014e38
	adds r0, r5, #0
	bl Func_080150e4
	movs r1, #1
	ands r1, r6
	movs r2, #192
	lsls r2, r2, #8
	lsls r1, r1, #15
	adds r1, r1, r2
	movs r0, #128
	movs r2, #128
	lsls r2, r2, #8
	lsls r0, r0, #7
	bl Func_080151e4
	movs r0, #128
	lsls r0, r0, #9
	bl Func_0801521c
	mov r1, r11
	movs r2, #4
	ldr r0, .L_08181ea8
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	bl Func_08014ea8
	movs r3, #128
	lsls r3, r3, #7
	adds r6, #1
	adds r5, r5, r3
	cmp r6, #4
	bne .L_08181dd6
.L_08181e20:
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r11
	bl Sys_Free
	movs r1, #8
	movs r0, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r8
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r4, #1
	add r9, r4
	mov r1, r9
	cmp r1, #90
	beq .L_08181e56
	b .L_081819d6
.L_08181e56:
	ldr r0, .L_08181eac
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08181e74:
	.4byte IwramFillWords
.L_08181e78:
	.4byte 0x3f3f3f3f
.L_08181e7c:
	.4byte Data_0201476b
.L_08181e80:
	.4byte Data_0819962c
.L_08181e84:
	.4byte 0xffffe000
.L_08181e88:
	.4byte Data_08199650
.L_08181e8c:
	.4byte Data_08199608
.L_08181e90:
	.4byte Data_0201276b
.L_08181e94:
	.4byte 0x00015f90
.L_08181e98:
	.4byte 0x00078e20
.L_08181e9c:
	.4byte 0xfff00000
.L_08181ea0:
	.4byte Data_08199244
.L_08181ea4:
	.4byte Data_0201576b
.L_08181ea8:
	.4byte Data_081991f0
.L_08181eac:
	.4byte Func_08143000
