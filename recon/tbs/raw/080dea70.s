.syntax unified
	.thumb
	.global BattleFx_RunProjectileVolley
	.thumb_func
BattleFx_RunProjectileVolley:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080dec18
	adds r2, r3, #0
	adds r5, r0, #0
	ldmia r2!, {r0}
	ldr r2, [r2]
	sub sp, #156
	str r2, [sp, #132]
	adds r2, r3, #0
	subs r2, #108
	ldr r2, [r2]
	str r2, [sp, #108]
	ldr r3, [r3, #8]
	mov r8, r1
	movs r1, #0
	str r3, [sp, #104]
	str r1, [sp, #100]
	ldr r3, .L_080dec1c
	mov r11, r0
	add r3, r11
	ldr r2, [r5, #24]
	str r5, [r3]
	mov r3, r8
	str r2, [sp, #92]
	cmp r3, #10
	bne .L_080deab8
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	b .L_080deabe
.L_080deab8:
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
.L_080deabe:
	ldr r6, .L_080dec1c
	add r6, r11
	ldr r2, [r6]
	ldr r3, [r2, #28]
	cmp r3, #1
	bne .L_080deae0
	add r3, sp, #140
	ldr r2, [r2, #4]
	str r3, [sp, #0]
	add r3, sp, #136
	str r3, [sp, #4]
	adds r0, r5, #0
	movs r1, #1
	movs r3, #2
	bl BattleFx_PrepareCanvasEffect
	ldr r2, [r6]
.L_080deae0:
	mov r4, r8
	cmp r4, #5
	bne .L_080deb02
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_080deaf0
	movs r3, #2
	b .L_080deb0a
.L_080deaf0:
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #46
	movs r1, #7
	movs r2, #7
	movs r3, #3
	bl BattleEffect_LoadWork
	b .L_080deb28
.L_080deb02:
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_080deb1a
	movs r3, #3
.L_080deb0a:
	str r3, [sp, #0]
	movs r0, #46
	movs r1, #7
	movs r2, #7
	movs r3, #7
	bl BattleEffect_LoadWork
	b .L_080deb28
.L_080deb1a:
	movs r3, #3
	movs r0, #46
	movs r1, #7
	movs r2, #7
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
.L_080deb28:
	ldr r3, .L_080dec20
	adds r3, #184
	ldr r3, [r3]
	ldr r0, .L_080dec24
	str r3, [sp, #112]
	ldr r1, [sp, #104]
	movs r2, #0
	movs r3, #0
	mov r6, r8
	bl Resource_LoadAndDecompress
	cmp r6, #0
	beq .L_080deb4a
	cmp r6, #5
	beq .L_080deb50
	cmp r6, #8
	bne .L_080debe8
.L_080deb4a:
	mov r7, r8
	cmp r7, #5
	bne .L_080deb54
.L_080deb50:
	movs r0, #2
	str r0, [sp, #92]
.L_080deb54:
	mov r1, r8
	cmp r1, #8
	bne .L_080deb5e
	movs r2, #0
	str r2, [sp, #92]
.L_080deb5e:
	ldr r3, [sp, #92]
	cmp r3, #0
	bne .L_080deb6c
	movs r1, #128
	lsls r1, r1, #5
	ldr r0, .L_080dec28
	b .L_080deb78
.L_080deb6c:
	ldr r4, [sp, #92]
	cmp r4, #1
	bne .L_080deb84
	movs r1, #128
	lsls r1, r1, #5
	ldr r0, .L_080dec2c
.L_080deb78:
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_080deb94
.L_080deb84:
	movs r1, #128
	lsls r1, r1, #5
	ldr r0, .L_080dec30
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
.L_080deb94:
	mov r6, r8
	cmp r6, #5
	bne .L_080debae
	ldr r0, .L_080dec34
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080dec38
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.L_080debae:
	movs r1, #128
	lsls r1, r1, #6
	ldr r0, .L_080dec3c
	add r1, r11
	movs r2, #1
	movs r3, #0
	mov r7, r8
	bl Resource_LoadAndDecompress
	cmp r7, #5
	bne .L_080debd8
	ldr r0, .L_080dec34
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080dec38
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.L_080debd8:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080dec40
	movs r3, #75
	b .L_080decd4
.L_080debe8:
	mov r0, r8
	cmp r0, #1
	bne .L_080dec4c
	mov r1, r11
	movs r2, #1
	movs r3, #1
	ldr r0, .L_080dec44
	bl Resource_LoadAndDecompress
	ldr r2, .L_080dec48
	ldr r3, .L_080dec14
	strh r3, [r2]
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080dec40
	add r3, r11
	mov r1, r8
	str r1, [r3]
	add r2, r11
	movs r3, #0
	b .L_080decd6
	.2byte 0x0000
.L_080dec14:
	.4byte 0x00000000
.L_080dec18:
	.4byte gBattleFxWork
.L_080dec1c:
	.4byte 0x00007828
.L_080dec20:
	.4byte gWorkSlot
.L_080dec24:
	.4byte 0x00000073
.L_080dec28:
	.4byte 0x0000007f
.L_080dec2c:
	.4byte 0x00000080
.L_080dec30:
	.4byte 0x00000081
.L_080dec34:
	.4byte 0x000000b9
.L_080dec38:
	.4byte IwramCopyWords
.L_080dec3c:
	.4byte 0x000000c7
.L_080dec40:
	.4byte 0x00007784
.L_080dec44:
	.4byte 0x0000005d
.L_080dec48:
	.4byte 0x04000050
.L_080dec4c:
	mov r2, r8
	cmp r2, #2
	bne .L_080dec84
	ldr r0, .L_080def84
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080def88
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r2, #0
	movs r3, #0
	ldr r0, .L_080def8c
	mov r1, r11
	bl Resource_LoadAndDecompress
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080def90
	add r3, r11
	mov r4, r8
	str r4, [r3]
	add r2, r11
	movs r3, #50
	b .L_080decd6
.L_080dec84:
	mov r3, r8
	subs r3, #3
	cmp r3, #1
	bls .L_080dec92
	mov r6, r8
	cmp r6, #11
	bne .L_080dec9c
.L_080dec92:
	movs r2, #1
	movs r3, #1
	ldr r0, .L_080def94
	mov r1, r11
	b .L_080decc2
.L_080dec9c:
	mov r7, r8
	cmp r7, #6
	bne .L_080decac
	movs r2, #1
	movs r3, #1
	ldr r0, .L_080def98
	mov r1, r11
	b .L_080decc2
.L_080decac:
	ldr r0, .L_080def9c
	mov r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r1, .L_080defa0
	movs r2, #1
	movs r3, #0
	ldr r0, .L_080defa4
	add r1, r11
.L_080decc2:
	bl Resource_LoadAndDecompress
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080def90
	movs r3, #50
.L_080decd4:
	add r2, r11
.L_080decd6:
	str r3, [r2]
	ldr r5, .L_080defa8
	movs r1, #144
	lsls r1, r1, #3
	add r5, r11
	ldr r0, .L_080defac
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	mov r0, r8
	lsls r0, r0, #2
	str r0, [sp, #84]
	ldr r1, .L_080defb0
	adds r3, r0, #0
	add r3, r8
	ldrb r2, [r1, r3]
	str r2, [sp, #88]
	adds r2, r3, #1
	ldrb r2, [r1, r2]
	mov r10, r2
	adds r2, r3, #2
	ldrb r2, [r1, r2]
	str r2, [sp, #80]
	adds r3, #3
	ldrb r1, [r1, r3]
	str r1, [sp, #76]
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl Battle_GetObjectTableValueFar
	str r0, [sp, #72]
	ldr r5, [r5]
	ldr r4, [sp, #88]
	ldr r3, [r5, #20]
	muls r3, r4
	cmp r3, #63
	ble .L_080ded2c
	movs r3, #1
	str r3, [r5, #20]
.L_080ded2c:
	movs r7, #0
	str r7, [sp, #128]
	ldr r3, [r5, #20]
	cmp r3, #0
	bne .L_080ded38
	b .L_080def48
.L_080ded38:
	ldr r0, .L_080defa8
	movs r1, #0
	add r0, r11
	movs r2, #36
	str r0, [sp, #68]
	str r1, [sp, #24]
	str r2, [sp, #20]
.L_080ded46:
	ldr r4, [sp, #68]
	ldr r7, [sp, #20]
	ldr r3, [r4]
	ldrsh r0, [r3, r7]
	bl GetBattleObjectSlotFar
	ldr r2, [sp, #68]
	ldr r4, [sp, #20]
	ldr r3, [r2]
	ldr r7, [r0]
	ldrsh r0, [r3, r4]
	bl Battle_GetObjectTableValueFar
	ldr r3, [sp, #88]
	movs r2, #0
	str r0, [sp, #64]
	str r2, [sp, #124]
	cmp r3, #0
	bne .L_080ded6e
	b .L_080def28
.L_080ded6e:
	ldr r4, [sp, #72]
	lsrs r3, r4, #31
	ldr r0, [sp, #24]
	adds r3, r4, r3
	asrs r3, r3, #1
	mov r9, r3
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	movs r1, #225
	add r3, r11
	lsls r1, r1, #7
	adds r5, r3, r1
.L_080ded88:
	ldr r3, [r6, #8]
	mov r2, r8
	str r3, [r5]
	cmp r2, #7
	bne .L_080deda8
	bl Random16
	movs r2, #15
	ldr r3, [r6, #12]
	ands r2, r0
	lsls r2, r2, #16
	movs r4, #232
	adds r3, r3, r2
	lsls r4, r4, #14
	adds r3, r3, r4
	b .L_080dee1a
.L_080deda8:
	mov r0, r8
	cmp r0, #10
	bne .L_080dedb4
	ldr r3, [r6, #12]
	add r3, r9
	b .L_080dee1a
.L_080dedb4:
	mov r1, r8
	cmp r1, #6
	bne .L_080dedc0
	ldr r3, [r6, #12]
	add r3, r9
	b .L_080dee1a
.L_080dedc0:
	mov r2, r8
	cmp r2, #9
	bne .L_080dedd4
	bl Random16
	movs r2, #31
	ldr r1, [r6, #12]
	ands r2, r0
	movs r3, #16
	b .L_080dedfc
.L_080dedd4:
	mov r3, r8
	subs r3, #3
	cmp r3, #1
	bhi .L_080dedea
	bl Random16
	movs r2, #31
	ldr r1, [r6, #12]
	ands r2, r0
	movs r3, #16
	b .L_080dedfc
.L_080dedea:
	mov r3, r8
	cmp r3, #11
	bne .L_080dee08
	bl Random16
	movs r2, #63
	ldr r1, [r6, #12]
	ands r2, r0
	movs r3, #32
.L_080dedfc:
	subs r3, r3, r2
	add r1, r9
	lsls r3, r3, #16
	adds r1, r1, r3
	str r1, [r5, #4]
	b .L_080dee1c
.L_080dee08:
	mov r4, r8
	cmp r4, #5
	bne .L_080dee14
	ldr r3, [r6, #12]
	add r3, r9
	b .L_080dee1a
.L_080dee14:
	ldr r3, [r6, #12]
	ldr r0, [sp, #72]
	adds r3, r3, r0
.L_080dee1a:
	str r3, [r5, #4]
.L_080dee1c:
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	ldr r0, [r7, #8]
	ldr r3, [r5]
	mov r1, r10
	subs r0, r0, r3
	bl __divsi3
	mov r1, r8
	str r0, [r5, #12]
	cmp r1, #7
	bne .L_080dee4e
	bl Random16
	movs r3, #63
	ands r3, r0
	ldr r0, [r7, #12]
	lsls r3, r3, #16
	adds r0, r0, r3
	ldr r3, [r5, #4]
	ldr r2, .L_080defb4
	subs r0, r0, r3
	adds r0, r0, r2
	mov r1, r10
	b .L_080def00
.L_080dee4e:
	mov r3, r8
	cmp r3, #8
	bne .L_080dee70
	bl Random16
	movs r3, #7
	ands r3, r0
	ldr r0, [r7, #12]
	lsls r3, r3, #16
	adds r0, r0, r3
	ldr r3, [r5, #4]
	movs r4, #176
	subs r0, r0, r3
	lsls r4, r4, #13
	adds r0, r0, r4
	mov r1, r10
	b .L_080def00
.L_080dee70:
	mov r0, r8
	cmp r0, #9
	bne .L_080dee8a
	bl Random16
	movs r2, #63
	ands r2, r0
	movs r3, #64
	subs r3, r3, r2
	ldr r0, [r7, #12]
	lsls r3, r3, #16
	adds r0, r0, r3
	b .L_080deefa
.L_080dee8a:
	mov r1, r8
	cmp r1, #10
	bne .L_080deeac
	bl Random16
	movs r3, #31
	ands r3, r0
	ldr r0, [r7, #12]
	lsls r3, r3, #16
	adds r0, r0, r3
	ldr r3, [r5, #4]
	movs r2, #128
	subs r0, r0, r3
	lsls r2, r2, #11
	adds r0, r0, r2
	mov r1, r10
	b .L_080def00
.L_080deeac:
	mov r3, r8
	subs r3, #3
	cmp r3, #1
	bls .L_080deebe
	mov r3, r8
	cmp r3, #11
	beq .L_080deebe
	cmp r3, #5
	bne .L_080deec4
.L_080deebe:
	movs r3, #0
	str r3, [r5, #16]
	b .L_080def06
.L_080deec4:
	mov r4, r8
	cmp r4, #6
	bne .L_080deee6
	bl Random16
	adds r2, r0, #0
	ldr r0, [sp, #64]
	lsrs r3, r0, #31
	adds r3, r0, r3
	ldr r0, [r7, #12]
	asrs r3, r3, #1
	adds r0, r0, r3
	movs r3, #15
	ands r3, r2
	lsls r3, r3, #16
	adds r0, r0, r3
	b .L_080deefa
.L_080deee6:
	bl Random16
	movs r3, #15
	ldr r1, [sp, #64]
	adds r2, r0, #0
	ldr r0, [r7, #12]
	ands r3, r2
	adds r0, r0, r1
	lsls r3, r3, #16
	subs r0, r0, r3
.L_080deefa:
	ldr r3, [r5, #4]
	mov r1, r10
	subs r0, r0, r3
.L_080def00:
	bl __divsi3
	str r0, [r5, #16]
.L_080def06:
	ldr r3, [r5, #8]
	ldr r0, [r7, #16]
	mov r1, r10
	subs r0, r0, r3
	bl __divsi3
	movs r3, #0
	str r0, [r5, #20]
	str r3, [r5, #24]
	ldr r2, [sp, #124]
	ldr r3, [sp, #88]
	adds r2, #1
	adds r5, #28
	str r2, [sp, #124]
	cmp r2, r3
	beq .L_080def28
	b .L_080ded88
.L_080def28:
	ldr r4, [sp, #24]
	ldr r0, [sp, #20]
	ldr r1, [sp, #128]
	ldr r7, [sp, #88]
	adds r0, #2
	adds r4, r4, r7
	adds r1, #1
	str r4, [sp, #24]
	str r0, [sp, #20]
	str r1, [sp, #128]
	ldr r2, [sp, #68]
	ldr r3, [r2]
	ldr r3, [r3, #20]
	cmp r1, r3
	beq .L_080def48
	b .L_080ded46
.L_080def48:
	movs r3, #0
	str r3, [sp, #128]
	movs r2, #128
	ldr r3, .L_080defb8
	movs r1, #0
	lsls r2, r2, #3
.L_080def54:
	str r1, [r3]
	ldr r4, [sp, #128]
	adds r4, #1
	adds r3, #28
	str r4, [sp, #128]
	cmp r4, r2
	bne .L_080def54
	mov r6, r8
	cmp r6, #6
	bne .L_080defbc
	ldr r3, .L_080defa8
	add r3, r11
	ldr r3, [r3]
	ldr r7, [sp, #76]
	ldr r3, [r3, #20]
	ldr r0, [sp, #80]
	ldr r1, [sp, #88]
	muls r3, r7
	adds r2, r0, #0
	muls r2, r1
	adds r3, r3, r2
	adds r3, #32
	b .L_080defd4
	.2byte 0x0000
.L_080def84:
	.4byte 0x0000007f
.L_080def88:
	.4byte IwramCopyWords
.L_080def8c:
	.4byte 0x0000005c
.L_080def90:
	.4byte 0x00007784
.L_080def94:
	.4byte 0x0000005b
.L_080def98:
	.4byte 0x00000068
.L_080def9c:
	.4byte 0x000000b8
.L_080defa0:
	.4byte 0x000065c0
.L_080defa4:
	.4byte 0x00000092
.L_080defa8:
	.4byte 0x00007828
.L_080defac:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080defb0:
	.4byte Data_080eebec
.L_080defb4:
	.4byte 0xfff40000
.L_080defb8:
	.4byte gMapCellBuffer + 0x18
.L_080defbc:
	ldr r3, .L_080df2f8
	add r3, r11
	ldr r3, [r3]
	ldr r2, [sp, #76]
	ldr r3, [r3, #20]
	ldr r4, [sp, #80]
	ldr r6, [sp, #88]
	muls r3, r2
	adds r2, r4, #0
	muls r2, r6
	adds r3, r3, r2
	adds r3, #16
.L_080defd4:
	str r3, [sp, #96]
	ldr r0, [sp, #96]
	movs r7, #0
	str r7, [sp, #120]
	cmp r0, #0
	bne .L_080defe4
	bl .L_080df864
.L_080defe4:
	ldr r1, [sp, #108]
	adds r1, #12
	str r1, [sp, #48]
.L_080defea:
	ldr r2, [sp, #100]
	cmp r2, #0
	ble .L_080deff4
	subs r2, #1
	str r2, [sp, #100]
.L_080deff4:
	mov r3, r8
	cmp r3, #6
	bne .L_080df014
	ldr r4, [sp, #120]
	cmp r4, #4
	bne .L_080df006
	movs r0, #136
	bl AudioCommand_PlayFar
.L_080df006:
	ldr r6, [sp, #120]
	cmp r6, #32
	bne .L_080df03a
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080df03a
.L_080df014:
	mov r7, r8
	cmp r7, #7
	bne .L_080df028
	ldr r0, [sp, #120]
	cmp r0, #48
	bne .L_080df03a
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080df03a
.L_080df028:
	mov r1, r8
	cmp r1, #5
	beq .L_080df03a
	ldr r2, [sp, #120]
	cmp r2, #16
	bne .L_080df03a
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080df03a:
	ldr r7, .L_080df2f8
	add r7, r11
	ldr r3, [r7]
	ldr r3, [r3, #28]
	cmp r3, #1
	bne .L_080df0ec
	ldr r3, [sp, #120]
	lsls r5, r3, #11
	adds r0, r5, #0
	bl Trig_Sin
	ldr r3, [sp, #140]
	negs r0, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	lsls r0, r0, #2
	asrs r3, r3, #1
	asrs r0, r0, #16
	adds r0, r0, r3
	subs r0, #10
	mov r10, r0
	adds r0, r5, #0
	bl Trig_Cos
	lsls r0, r0, #1
	ldr r3, [sp, #136]
	asrs r0, r0, #16
	adds r0, r0, r3
	ldr r4, [sp, #120]
	adds r6, r0, #0
	subs r6, #24
	cmp r4, #69
	ble .L_080df084
	lsls r3, r4, #1
	subs r3, r6, r3
	adds r6, r3, #0
	adds r6, #138
.L_080df084:
	ldr r3, [r7]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080df09e
	movs r3, #3
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #7
	bl BattleEffect_LoadWork
	b .L_080df0ac
.L_080df09e:
	movs r3, #3
	movs r0, #47
	movs r1, #7
	movs r2, #7
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
.L_080df0ac:
	ldr r3, .L_080df2fc
	adds r3, #188
	ldr r5, [r3]
	ldr r7, [sp, #120]
	str r5, [sp, #116]
	cmp r7, #3
	bgt .L_080df0d0
	movs r3, #20
	ldr r1, .L_080df300
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	ldr r0, [sp, #132]
	add r1, r11
	mov r2, r10
	adds r3, r6, #0
	bl _call_via_r5
.L_080df0d0:
	movs r3, #20
	ldr r1, .L_080df300
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	ldr r0, [sp, #132]
	add r1, r11
	mov r2, r10
	adds r3, r6, #0
	bl _call_via_r5
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080df0ec:
	bl Render_ResetTransformState
	ldr r0, [sp, #108]
	ldr r1, [sp, #48]
	bl Graphics_PrepareTransferInIwramWork
	movs r0, #0
	str r0, [sp, #60]
	str r0, [sp, #128]
	ldr r2, .L_080df2f8
	mov r1, r11
	ldr r3, [r1, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080df10c
	b .L_080df5c0
.L_080df10c:
	movs r3, #36
	str r3, [sp, #32]
	str r0, [sp, #28]
.L_080df112:
	mov r4, r11
	ldr r3, [r4, r2]
	ldr r6, [sp, #32]
	ldrsh r0, [r3, r6]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r1, [sp, #88]
	str r0, [sp, #56]
	movs r0, #0
	str r0, [sp, #124]
	cmp r1, #0
	bne .L_080df12e
	b .L_080df59e
.L_080df12e:
	ldr r4, [sp, #60]
	lsls r3, r4, #3
	subs r3, r3, r4
	ldr r6, .L_080df304
	ldr r2, [sp, #28]
	lsls r3, r3, #2
	adds r6, r3, r6
	str r2, [sp, #44]
	str r6, [sp, #40]
	str r0, [sp, #36]
.L_080df142:
	ldr r7, [sp, #36]
	ldr r0, [sp, #44]
	ldr r1, [sp, #120]
	adds r3, r7, r0
	cmp r3, r1
	blt .L_080df150
	b .L_080df57c
.L_080df150:
	ldr r3, [sp, #88]
	ldr r4, [sp, #128]
	adds r2, r3, #0
	muls r2, r4
	ldr r6, [sp, #124]
	adds r2, r2, r6
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	movs r7, #225
	add r3, r11
	lsls r7, r7, #7
	adds r6, r3, r7
	add r7, sp, #144
	adds r0, r6, #0
	adds r1, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r7]
	asrs r3, r3, #1
	str r3, [r7]
	ldr r2, [r6]
	ldr r3, [r6, #12]
	adds r2, r2, r3
	str r2, [r6]
	ldr r3, [r6, #16]
	ldr r2, [r6, #4]
	adds r2, r2, r3
	str r2, [r6, #4]
	ldr r3, [r6, #20]
	ldr r2, [r6, #8]
	mov r0, r8
	adds r2, r2, r3
	str r2, [r6, #8]
	cmp r0, #6
	bne .L_080df1fe
	movs r1, #128
	ldr r5, .L_080df308
	movs r2, #0
	lsls r1, r1, #2
	mov r10, r7
	movs r4, #255
.L_080df1a4:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_080df1f2
	mov r0, r10
	ldr r3, [r0]
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, [r0, #4]
	lsls r3, r3, #16
	str r3, [r5, #4]
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl Random16
	ldr r4, [sp, #8]
	ands r0, r4
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ldr r4, [sp, #8]
	ands r0, r4
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	movs r3, #7
	ldr r2, [sp, #12]
	ands r3, r0
	adds r3, #32
	adds r2, #1
	str r3, [r5, #24]
	ldr r1, [sp, #16]
	ldr r4, [sp, #8]
	cmp r2, #2
	beq .L_080df1fe
.L_080df1f2:
	movs r3, #128
	adds r1, #1
	lsls r3, r3, #3
	adds r5, #28
	cmp r1, r3
	bne .L_080df1a4
.L_080df1fe:
	ldr r3, [r6, #24]
	cmp r3, #0
	beq .L_080df206
	b .L_080df3a8
.L_080df206:
	ldr r4, [sp, #56]
	ldr r1, [r4, #8]
	ldr r2, [r6]
	cmp r1, #0
	bge .L_080df218
	lsrs r3, r2, #31
	cmp r3, #0
	bne .L_080df222
	b .L_080df3a8
.L_080df218:
	mvns r3, r2
	lsrs r3, r3, #31
	cmp r3, #0
	bne .L_080df222
	b .L_080df3a8
.L_080df222:
	adds r3, r2, #0
	cmp r3, #0
	bge .L_080df22a
	negs r3, r3
.L_080df22a:
	adds r2, r1, #0
	cmp r2, #0
	bge .L_080df232
	negs r2, r2
.L_080df232:
	cmp r3, r2
	bge .L_080df238
	b .L_080df3a8
.L_080df238:
	movs r0, #0
	movs r3, #1
	mov r1, r8
	str r0, [sp, #52]
	ldr r2, [sp, #40]
	str r3, [r6, #24]
	cmp r1, #5
	bne .L_080df252
	movs r0, #134
	str r2, [sp, #12]
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080df26a
.L_080df252:
	mov r3, r8
	cmp r3, #6
	beq .L_080df26c
	ldr r4, [sp, #100]
	cmp r4, #0
	bne .L_080df26c
	movs r0, #8
	str r0, [sp, #100]
	movs r0, #132
	str r2, [sp, #12]
	bl AudioCommand_PlayFar
.L_080df26a:
	ldr r2, [sp, #12]
.L_080df26c:
	mov r1, r8
	cmp r1, #2
	bne .L_080df2a0
	str r2, [sp, #12]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #32
	lsls r5, r3, #12
	bl Random16
	movs r3, #1
	ands r0, r3
	ldr r2, [sp, #12]
	cmp r0, #0
	beq .L_080df294
	ldr r3, [r6, #16]
	adds r3, r3, r5
	b .L_080df298
.L_080df294:
	ldr r3, [r6, #16]
	subs r3, r3, r5
.L_080df298:
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	negs r3, r3
	str r3, [r6, #12]
.L_080df2a0:
	movs r3, #1
	str r3, [r2, #24]
	ldr r3, [r7]
	str r3, [r2]
	ldr r3, [r7, #4]
	str r3, [r2, #4]
	movs r3, #0
	str r3, [r2, #8]
	mov r2, r8
	cmp r2, #7
	beq .L_080df2be
	ldr r2, .L_080df30c
	movs r3, #2
	add r2, r11
	str r3, [r2]
.L_080df2be:
	ldr r5, .L_080df2f8
	add r5, r11
	ldr r3, [r5]
	ldr r4, [sp, #32]
	ldrsh r0, [r3, r4]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #7
	ldr r3, [sp, #128]
	bl ObjectGroup_UpdateMembers
	mov r2, r8
	cmp r2, #7
	beq .L_080df31c
	cmp r2, #9
	beq .L_080df31c
	cmp r2, #10
	beq .L_080df31c
	cmp r2, #5
	bne .L_080df310
	ldr r3, [r5]
	ldr r4, [sp, #32]
	ldrsh r0, [r3, r4]
	movs r1, #4
	bl BattleMotion_ApplyVariantMotionFar
	b .L_080df31c
	.2byte 0x0000
.L_080df2f8:
	.4byte 0x00007828
.L_080df2fc:
	.4byte gWorkSlot
.L_080df300:
	.4byte 0x000065c0
.L_080df304:
	.4byte gMapCellBuffer
.L_080df308:
	.4byte gMapCellBuffer + 0x3800
.L_080df30c:
	.4byte 0x000077a8
.L_080df310:
	ldr r3, [r5]
	ldr r2, [sp, #32]
	movs r1, #5
	ldrsh r0, [r3, r2]
	bl BattleMotion_ApplyVariantMotionFar
.L_080df31c:
	movs r2, #5
	mov r0, r8
	eors r2, r0
	negs r3, r2
	orrs r3, r2
	lsrs r2, r3, #31
	movs r3, #12
	subs r2, r3, r2
	movs r3, #255
	mov r9, r3
	ldr r3, [sp, #84]
	add r3, r8
	adds r3, #4
	ldr r5, .L_080df68c
	movs r1, #100
	adds r4, r7, #0
	mov r10, r3
.L_080df33e:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_080df39c
	ldr r3, [r4]
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, [r4, #4]
	lsls r3, r3, #16
	str r3, [r5, #4]
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl Random16
	mov r3, r9
	ldr r2, [sp, #12]
	ands r0, r3
	subs r0, #128
	lsls r0, r2
	str r0, [r5, #12]
	bl Random16
	mov r3, r9
	ands r0, r3
	ldr r2, [sp, #12]
	subs r0, #128
	lsls r0, r2
	str r0, [r5, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #16
	str r3, [r5, #24]
	ldr r0, [sp, #52]
	adds r0, #1
	str r0, [sp, #52]
	ldr r3, .L_080df690
	mov r0, r10
	ldrb r3, [r3, r0]
	mov r12, r3
	ldr r3, [sp, #52]
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	ldr r4, [sp, #8]
	cmp r3, r12
	beq .L_080df3a8
.L_080df39c:
	movs r0, #128
	adds r1, #1
	lsls r0, r0, #2
	adds r5, #28
	cmp r1, r0
	bne .L_080df33e
.L_080df3a8:
	mov r1, r8
	cmp r1, #0
	beq .L_080df3b6
	cmp r1, #5
	beq .L_080df3b6
	cmp r1, #8
	bne .L_080df466
.L_080df3b6:
	ldr r3, .L_080df694
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080df3d4
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #7
	bl BattleEffect_LoadWork
	b .L_080df3e4
.L_080df3d4:
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #3
	bl BattleEffect_LoadWork
.L_080df3e4:
	ldr r4, [sp, #92]
	ldr r3, .L_080df698
	ldrb r0, [r3, r4]
	movs r1, #32
	ldr r2, [r7]
	ldr r3, [r7, #4]
	ldr r6, .L_080df69c
	str r1, [sp, #0]
	str r0, [sp, #4]
	movs r1, #128
	lsls r1, r1, #5
	subs r3, r3, r0
	subs r2, #16
	ldr r4, [r6]
	ldr r0, [sp, #132]
	add r1, r11
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_080df694
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080df42c
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #15
	bl BattleEffect_LoadWork
	b .L_080df43c
.L_080df42c:
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #11
	bl BattleEffect_LoadWork
.L_080df43c:
	movs r1, #32
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	ldr r7, [sp, #92]
	ldr r1, .L_080df698
	ldrb r1, [r1, r7]
	ldr r0, .L_080df69c
	str r1, [sp, #4]
	movs r1, #128
	lsls r1, r1, #5
	ldr r4, [r0]
	subs r2, #16
	ldr r0, [sp, #132]
	add r1, r11
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	b .L_080df57c
.L_080df466:
	mov r1, r8
	cmp r1, #1
	bne .L_080df4a6
	ldr r0, [r6]
	cmp r0, #0
	bge .L_080df474
	negs r0, r0
.L_080df474:
	ldr r2, [sp, #56]
	ldr r1, [r2, #8]
	cmp r1, #0
	bge .L_080df47e
	negs r1, r1
.L_080df47e:
	cmp r0, r1
	bgt .L_080df57c
	movs r1, #6
	ldr r0, [sp, #120]
	bl __modsi3
	lsls r1, r0, #1
	ldr r2, [r7]
	ldr r3, [r7, #4]
	adds r1, r1, r0
	movs r0, #32
	lsls r1, r1, #8
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	add r1, r11
	subs r2, #16
	subs r3, #12
	ldr r0, [sp, #132]
	b .L_080df55a
.L_080df4a6:
	mov r6, r8
	cmp r6, #7
	beq .L_080df4b4
	cmp r6, #9
	beq .L_080df4b4
	cmp r6, #10
	bne .L_080df4d4
.L_080df4b4:
	ldr r0, [sp, #124]
	movs r3, #3
	ands r3, r0
	ldr r2, .L_080df6a0
	lsls r3, r3, #1
	ldrh r1, [r2, r3]
	ldr r2, [r7]
	ldr r3, [r7, #4]
	movs r0, #8
	str r0, [sp, #0]
	str r0, [sp, #4]
	add r1, r11
	subs r2, #4
	subs r3, #4
	ldr r0, [sp, #132]
	b .L_080df55a
.L_080df4d4:
	mov r6, r8
	cmp r6, #2
	bne .L_080df4fc
	movs r1, #6
	ldr r0, [sp, #124]
	bl __modsi3
	ldr r2, [r7]
	ldr r3, [r7, #4]
	adds r1, r0, #0
	movs r0, #8
	lsls r1, r1, #7
	str r0, [sp, #0]
	movs r0, #16
	str r0, [sp, #4]
	add r1, r11
	subs r2, #4
	subs r3, #8
	ldr r0, [sp, #132]
	b .L_080df534
.L_080df4fc:
	mov r0, r8
	cmp r0, #3
	bne .L_080df51a
	movs r1, #18
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #13
	str r1, [sp, #4]
	mov r1, r11
	subs r2, #9
	subs r3, #7
	ldr r0, [sp, #132]
	adds r1, #96
	b .L_080df55a
.L_080df51a:
	mov r6, r8
	cmp r6, #4
	bne .L_080df53c
	ldr r2, [r7]
	ldr r3, [r7, #4]
	movs r1, #12
	str r1, [sp, #0]
	movs r1, #8
	str r1, [sp, #4]
	subs r2, #6
	subs r3, #4
	ldr r0, [sp, #132]
	mov r1, r11
.L_080df534:
	ldr r7, [sp, #112]
	bl _call_via_r7
	b .L_080df57c
.L_080df53c:
	mov r0, r8
	cmp r0, #11
	bne .L_080df562
	movs r1, #29
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #23
	str r1, [sp, #4]
	movs r1, #165
	lsls r1, r1, #1
	subs r2, #15
	subs r3, #12
	ldr r0, [sp, #132]
	add r1, r11
.L_080df55a:
	ldr r4, [sp, #112]
	bl _call_via_r4
	b .L_080df57c
.L_080df562:
	ldr r2, [r7]
	ldr r3, [r7, #4]
	movs r1, #40
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	subs r2, #20
	subs r3, #32
	ldr r0, [sp, #132]
	mov r1, r11
	ldr r6, [sp, #112]
	bl _call_via_r6
.L_080df57c:
	ldr r7, [sp, #36]
	ldr r1, [sp, #124]
	ldr r2, [sp, #40]
	ldr r3, [sp, #60]
	ldr r0, [sp, #80]
	ldr r4, [sp, #88]
	adds r7, r7, r0
	adds r1, #1
	adds r2, #28
	adds r3, #1
	str r7, [sp, #36]
	str r1, [sp, #124]
	str r2, [sp, #40]
	str r3, [sp, #60]
	cmp r1, r4
	beq .L_080df59e
	b .L_080df142
.L_080df59e:
	ldr r6, [sp, #32]
	ldr r7, [sp, #28]
	ldr r1, [sp, #128]
	ldr r0, [sp, #76]
	adds r6, #2
	adds r7, r7, r0
	adds r1, #1
	str r6, [sp, #32]
	str r7, [sp, #28]
	str r1, [sp, #128]
	ldr r2, .L_080df694
	mov r4, r11
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r1, r3
	beq .L_080df5c0
	b .L_080df112
.L_080df5c0:
	mov r6, r8
	cmp r6, #0
	beq .L_080df5d0
	cmp r6, #5
	beq .L_080df5d0
	cmp r6, #8
	beq .L_080df5d0
	b .L_080df772
.L_080df5d0:
	movs r7, #0
	ldr r3, .L_080df694
	str r7, [sp, #128]
	add r3, r11
	ldr r3, [r3]
	ldr r0, [sp, #88]
	ldr r3, [r3, #20]
	muls r3, r0
	cmp r3, #0
	bne .L_080df5e6
	b .L_080df772
.L_080df5e6:
	ldr r1, .L_080df6a4
	ldr r2, .L_080df6a8
	ldr r7, .L_080df6ac
	mov r9, r1
	mov r10, r2
.L_080df5f0:
	ldr r1, [r7, #24]
	cmp r1, #1
	beq .L_080df5f8
	b .L_080df758
.L_080df5f8:
	ldr r3, [r7, #8]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r6, r3, #1
	ldr r3, [sp, #128]
	ands r3, r1
	cmp r3, #0
	bne .L_080df6bc
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl BattleEffect_LoadWork
	lsls r5, r6, #1
	mov r4, r9
	mov r0, r10
	ldrh r1, [r4, r5]
	ldr r2, [r7]
	ldrb r4, [r0, r6]
	ldr r3, .L_080df6b0
	subs r2, r2, r4
	ldrb r0, [r3, r6]
	ldr r3, [r7, #4]
	str r4, [sp, #0]
	ldr r4, .L_080df6b4
	subs r3, r3, r0
	ldrb r0, [r4, r6]
	str r0, [sp, #4]
	ldr r0, .L_080df69c
	add r1, r11
	ldr r4, [r0]
	ldr r0, [sp, #132]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #15
	movs r0, #47
	bl BattleEffect_LoadWork
	ldr r3, .L_080df6b8
	ldr r4, .L_080df6b0
	ldrb r0, [r3, r6]
	ldr r3, [r7, #4]
	adds r3, r3, r0
	ldrb r0, [r4, r6]
	mov r4, r10
	subs r3, r3, r0
	ldrb r0, [r4, r6]
	mov r2, r9
	ldrh r1, [r2, r5]
	ldr r4, .L_080df6b4
	ldr r2, [r7]
	str r0, [sp, #0]
	ldrb r0, [r4, r6]
	ldr r6, .L_080df69c
	str r0, [sp, #4]
	add r1, r11
	ldr r4, [r6]
	ldr r0, [sp, #132]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	b .L_080df74a
.L_080df68c:
	.4byte gMapCellBuffer + 0xaf0
.L_080df690:
	.4byte Data_080eebec
.L_080df694:
	.4byte 0x00007828
.L_080df698:
	.4byte Data_080eebe9
.L_080df69c:
	.4byte gTransitionWork + 0xc
.L_080df6a0:
	.4byte Data_080eec52
.L_080df6a4:
	.4byte Data_080eec44
.L_080df6a8:
	.4byte Data_080eec28
.L_080df6ac:
	.4byte gMapCellBuffer
.L_080df6b0:
	.4byte Data_080eec3d
.L_080df6b4:
	.4byte Data_080eec2f
.L_080df6b8:
	.4byte Data_080eec36
.L_080df6bc:
	movs r0, #2
	str r0, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl BattleEffect_LoadWork
	lsls r5, r6, #1
	mov r2, r9
	ldrh r1, [r2, r5]
	ldr r4, .L_080df888
	ldr r3, .L_080df88c
	add r1, r11
	adds r1, r1, r3
	ldr r2, [r7]
	ldrb r3, [r4, r6]
	mov r0, r10
	ldrb r4, [r0, r6]
	subs r2, r2, r3
	ldr r3, [r7, #4]
	subs r3, r3, r4
	mov r12, r3
	ldr r3, .L_080df890
	ldrb r0, [r3, r6]
	str r4, [sp, #4]
	str r0, [sp, #0]
	ldr r0, .L_080df894
	mov r3, r12
	ldr r4, [r0]
	ldr r0, [sp, #132]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #15
	movs r0, #47
	bl BattleEffect_LoadWork
	mov r2, r9
	ldrh r1, [r2, r5]
	ldr r3, .L_080df88c
	add r1, r11
	adds r1, r1, r3
	ldr r3, .L_080df898
	ldr r2, [r7]
	ldrb r3, [r3, r6]
	ldr r4, .L_080df888
	adds r2, r2, r3
	ldrb r3, [r4, r6]
	ldr r4, .L_080df890
	ldrb r0, [r4, r6]
	subs r2, r2, r3
	ldr r3, [r7, #4]
	str r0, [sp, #0]
	mov r4, r10
	ldrb r0, [r4, r6]
	ldr r6, .L_080df894
	str r0, [sp, #4]
	ldr r0, [sp, #132]
	ldr r4, [r6]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080df74a:
	ldr r3, [r7, #8]
	adds r3, #1
	str r3, [r7, #8]
	cmp r3, #12
	bne .L_080df758
	movs r3, #0
	str r3, [r7, #24]
.L_080df758:
	ldr r0, [sp, #128]
	ldr r3, .L_080df89c
	adds r0, #1
	str r0, [sp, #128]
	add r3, r11
	ldr r3, [r3]
	ldr r1, [sp, #88]
	ldr r3, [r3, #20]
	muls r3, r1
	adds r7, #28
	cmp r0, r3
	beq .L_080df772
	b .L_080df5f0
.L_080df772:
	movs r2, #100
	str r2, [sp, #128]
	ldr r5, .L_080df8a0
.L_080df778:
	ldr r0, [r5, #24]
	cmp r0, #0
	ble .L_080df7d4
	asrs r0, r0, #3
	adds r0, #1
	lsls r4, r0, #1
	ldr r2, .L_080df8a4
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #104]
	adds r1, r3, r1
	lsrs r3, r0, #31
	movs r6, #2
	ldrsh r2, [r5, r6]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #132]
	ldr r4, [sp, #112]
	bl _call_via_r4
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #5
	bl EffectStep_AdvanceWithGravity2D
	movs r6, #224
	ldr r3, [r5, #4]
	lsls r6, r6, #15
	cmp r3, r6
	ble .L_080df7ce
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
.L_080df7ce:
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080df7d4:
	ldr r7, [sp, #128]
	movs r0, #128
	adds r7, #1
	lsls r0, r0, #2
	adds r5, #28
	str r7, [sp, #128]
	cmp r7, r0
	bne .L_080df778
	ldr r6, .L_080df8a4
	ldr r5, .L_080df8a8
.L_080df7e8:
	ldr r0, [r5, #24]
	cmp r0, #0
	ble .L_080df82a
	asrs r0, r0, #4
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #104]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #132]
	ldr r4, [sp, #112]
	bl _call_via_r4
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_080df8ac
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080df82a:
	ldr r7, [sp, #128]
	movs r0, #128
	adds r7, #1
	lsls r0, r0, #3
	adds r5, #28
	str r7, [sp, #128]
	cmp r7, r0
	bne .L_080df7e8
	movs r1, #4
	movs r0, #4
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080df8b0
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #120]
	ldr r2, [sp, #96]
	adds r1, #1
	str r1, [sp, #120]
	cmp r1, r2
	beq .L_080df864
	bl .L_080defea
.L_080df864:
	ldr r0, .L_080df8b4
	bl Scheduler_RemoveCallback
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #156
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080df888:
	.4byte Data_080eec3d
.L_080df88c:
	.4byte 0x0000128a
.L_080df890:
	.4byte Data_080eec2f
.L_080df894:
	.4byte gTransitionWork + 0xc
.L_080df898:
	.4byte Data_080eec36
.L_080df89c:
	.4byte 0x00007828
.L_080df8a0:
	.4byte gMapCellBuffer + 0xaf0
.L_080df8a4:
	.4byte ParticleStreams_CellOffsets
.L_080df8a8:
	.4byte gMapCellBuffer + 0x3800
.L_080df8ac:
	.4byte 0xffffc000
.L_080df8b0:
	.4byte 0x00007824
.L_080df8b4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
