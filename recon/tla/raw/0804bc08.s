.syntax unified
	.thumb
	.global Func_0804bc08
	.thumb_func
Func_0804bc08:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #108
	str r0, [sp, #88]
	movs r0, #128
	lsls r0, r0, #1
	str r0, [sp, #68]
	movs r0, #128
	movs r3, #0
	movs r6, #0
	lsls r0, r0, #3
	str r1, [sp, #84]
	str r2, [sp, #80]
	str r3, [sp, #96]
	str r6, [sp, #60]
	str r6, [sp, #56]
	bl Resource_LoadIntoFreeSlot
	str r0, [sp, #52]
	movs r0, #128
	lsls r0, r0, #2
	bl Resource_LoadIntoFreeSlot
	movs r1, #136
	str r0, [sp, #48]
	lsls r1, r1, #1
	movs r0, #228
	bl Runtime_AllocateBlock
	mov r1, sp
	adds r1, #100
	str r1, [sp, #36]
	str r0, [r1]
	ldr r0, [sp, #52]
	movs r1, #0
	str r6, [sp, #92]
	bl Func_080455dc
	ldr r0, .L_0804bd08
	bl Func_080453d0
	ldr r0, .L_0804bd0c
	bl Func_08045330
	bl Func_080451bc
	ldr r2, [sp, #36]
	movs r4, #130
	ldr r1, [r2]
	lsls r4, r4, #1
	movs r2, #1
	movs r0, #134
	negs r2, r2
	adds r3, r1, r4
	lsls r0, r0, #1
	str r2, [r3]
	adds r3, r1, r0
	str r2, [r3]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r0, #128
	mov r12, r3
	adds r3, r1, #0
	lsls r0, r0, #24
	adds r3, #228
	movs r2, #7
.L_0804bc98:
	subs r2, #1
	stmia r3!, {r0}
	cmp r2, #0
	bge .L_0804bc98
	ldr r2, [sp, #36]
	movs r1, #0
	ldr r3, [r2]
	movs r2, #2
	adds r3, #36
.L_0804bcaa:
	subs r2, #1
	strb r1, [r3]
	adds r3, #1
	cmp r2, #0
	bge .L_0804bcaa
	ldr r4, [sp, #36]
	movs r2, #1
	ldr r1, [r4]
	movs r3, #0
	negs r2, r2
	str r3, [r1, #40]
	str r3, [r1, #44]
	str r3, [r1, #60]
	str r3, [r1, #64]
	str r3, [r1, #80]
	str r3, [r1, #72]
	str r2, [r1, #76]
	str r3, [r1, #68]
	mov r3, r12
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0804bcda
	b .L_0804bdb8
.L_0804bcda:
	movs r3, #1
	ldr r2, .L_0804bd10
	str r3, [r1, #80]
	ldr r3, .L_0804bcfc
	movs r5, #0
	strh r3, [r2, #8]
	ldr r3, .L_0804bd00
	strh r3, [r2, #10]
	strh r3, [r2, #12]
	ldr r3, .L_0804bd04
	strh r3, [r2, #14]
	mov r7, r12
	mov r6, r12
	adds r7, #80
	adds r6, #82
	b .L_0804bd94
	.2byte 0x0000
.L_0804bcfc:
	.4byte 0x00000056
.L_0804bd00:
	.4byte 0x00000053
.L_0804bd04:
	.4byte 0x00000054
.L_0804bd08:
	.4byte 0x06006000
.L_0804bd0c:
	.4byte 0x06006680
.L_0804bd10:
	.4byte Data_02003a74
.L_0804bd14:
	ldr r3, .L_0804bfc0
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0804bd34
	adds r5, #1
	cmp r5, #24
	ble .L_0804bd8e
	movs r1, #1
	negs r1, r1
	str r1, [sp, #80]
	ldr r2, [sp, #36]
	ldr r3, [r2]
	str r0, [r3, #80]
	b .L_0804bdb8
.L_0804bd34:
	ldrh r2, [r1, #8]
	movs r5, #0
	adds r3, r2, #0
	cmp r3, #86
	bne .L_0804bd50
	ldrh r3, [r1, #10]
	cmp r3, #83
	bne .L_0804bd50
	ldrh r3, [r1, #12]
	cmp r3, #83
	bne .L_0804bd50
	ldrh r3, [r1, #14]
	cmp r3, #84
	beq .L_0804bdb8
.L_0804bd50:
	adds r3, r2, #0
	cmp r3, #69
	bne .L_0804bd68
	ldrh r3, [r1, #10]
	cmp r3, #68
	bne .L_0804bd68
	ldrh r3, [r1, #12]
	cmp r3, #86
	bne .L_0804bd68
	ldrh r3, [r1, #14]
	cmp r3, #83
	beq .L_0804bdb8
.L_0804bd68:
	ldrh r3, [r1]
	cmp r3, #69
	bne .L_0804bd80
	ldrh r3, [r1, #2]
	cmp r3, #88
	bne .L_0804bd80
	ldrh r3, [r1, #4]
	cmp r3, #69
	bne .L_0804bd80
	ldrh r3, [r1, #6]
	cmp r3, #67
	beq .L_0804bd8e
.L_0804bd80:
	movs r3, #1
	negs r3, r3
	str r3, [sp, #80]
	ldr r4, [sp, #36]
	movs r2, #0
	ldr r3, [r4]
	b .L_0804bdb6
.L_0804bd8e:
	movs r0, #1
	bl WaitFrames
.L_0804bd94:
	ldrb r2, [r7]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldrb r0, [r6]
	ldr r3, .L_0804bfc4
	lsls r2, r2, #3
	adds r1, r2, r3
	cmp r0, #0
	beq .L_0804bd14
	movs r0, #1
	negs r0, r0
	str r0, [sp, #80]
	ldr r1, [sp, #36]
	movs r2, #0
	ldr r3, [r1]
.L_0804bdb6:
	str r2, [r3, #80]
.L_0804bdb8:
	movs r1, #144
	ldr r0, .L_0804bfc8
	lsls r1, r1, #3
	bl Func_080145a8
.L_0804bdc2:
	ldr r2, [sp, #88]
	ldr r3, [sp, #80]
	str r2, [sp, #96]
	cmp r3, #0
	blt .L_0804bddc
	movs r3, #0
	add r1, sp, #96
	add r2, sp, #92
	ldr r0, [sp, #84]
	str r3, [sp, #92]
	bl Func_0804babc
	str r0, [sp, #80]
.L_0804bddc:
	add r4, sp, #108
	mov r9, r4
	bl Func_0804bba8
	ldr r1, [sp, #36]
	ldr r0, [r1]
	movs r1, #0
	adds r3, r0, #0
	adds r3, #38
	strb r1, [r3]
	adds r2, r0, #0
	movs r3, #1
	negs r3, r3
	adds r2, #224
	str r3, [r2]
	adds r3, r0, #0
	adds r3, #216
	movs r0, #183
	str r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0804be1e
	ldr r3, [sp, #36]
	ldr r2, [r3]
	movs r3, #1
	adds r1, r2, #0
	adds r1, #216
	str r3, [r1]
	adds r2, #220
	movs r3, #60
	str r3, [r2]
.L_0804be1e:
	ldr r4, [sp, #80]
	cmp r4, #0
	blt .L_0804be30
	movs r0, #0
	adds r1, r4, #0
	bl Func_080457d0
	adds r6, r0, #0
	b .L_0804be32
.L_0804be30:
	movs r6, #14
.L_0804be32:
	movs r0, #1
	negs r0, r0
	cmp r6, r0
	bne .L_0804beb4
	ldr r2, [sp, #36]
	movs r4, #130
	ldr r1, [r2]
	lsls r4, r4, #1
	adds r3, r1, r4
	ldr r0, [r3]
	cmp r0, r6
	beq .L_0804bdc2
	movs r3, #192
	lsls r3, r3, #18
	adds r4, #4
	ldr r2, [r3, #36]
	adds r3, r1, r4
	ldr r3, [r3]
	lsls r3, r3, #1
	adds r3, #88
	ldrsh r4, [r2, r3]
	strh r0, [r2, r3]
	lsls r3, r0, #16
	asrs r1, r3, #16
	ldr r3, .L_0804bfcc
	movs r2, #134
	lsls r2, r2, #2
	movs r0, #7
	adds r3, r3, r2
.L_0804be6c:
	ldrb r2, [r3]
	cmp r2, r1
	bne .L_0804be76
	strb r4, [r3]
	b .L_0804be7c
.L_0804be76:
	cmp r2, r4
	bne .L_0804be7c
	strb r1, [r3]
.L_0804be7c:
	subs r0, #1
	adds r3, #1
	cmp r0, #0
	bge .L_0804be6c
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #36]
	adds r5, #228
	adds r3, #65
	ldrb r0, [r3]
	bl Func_0804297c
	bl BattleActor_CommitPlacementFar
	ldr r4, [sp, #36]
	movs r0, #130
	ldr r3, [r4]
	lsls r0, r0, #1
	movs r2, #1
	adds r3, r3, r0
	negs r2, r2
	str r2, [r3]
	bl Func_08118158
	ldr r2, [r5]
	movs r3, #1
	str r3, [r2, #60]
	b .L_0804bdc2
.L_0804beb4:
	cmp r6, #49
	bne .L_0804bef2
	movs r0, #112
	bl Audio_PlayCue
.L_0804bebe:
	movs r0, #0
	movs r1, #2
	movs r2, #1
	movs r3, #8
	bl Func_0804a85c
	movs r5, #1
	adds r6, r0, #0
	negs r5, r5
	cmp r6, r5
	bne .L_0804bed6
	b .L_0804bdc2
.L_0804bed6:
	bl Func_0804b8b8
	adds r6, r0, #0
	cmp r6, r5
	beq .L_0804bebe
	bl Func_08118158
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #60]
	b .L_0804bdc2
.L_0804bef2:
	cmp r6, #7
	bne .L_0804bf46
	movs r0, #18
	bl Runtime_BumpAllocate
	ldr r3, .L_0804bfd0
	adds r7, r0, #0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804bf14
	ldr r3, .L_0804bfd4
	movs r6, #2
	ldr r2, [r3]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	bne .L_0804bf16
.L_0804bf14:
	movs r6, #1
.L_0804bf16:
	adds r0, r6, #0
	adds r1, r7, #0
	bl BattleParty_ListActorIdsFar
	adds r5, r0, #0
	cmp r6, #1
	bne .L_0804bf2e
	lsls r0, r5, #1
	adds r0, r7, r0
	bl Func_08118138
	adds r5, r5, r0
.L_0804bf2e:
	movs r0, #1
	bl WaitFrames
	ldrh r2, [r7]
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_08046b08
	adds r0, r7, #0
	bl Sys_Free
	b .L_0804bdc2
.L_0804bf46:
	cmp r6, #4
	bne .L_0804bf84
	bl Func_0804b7c0
	cmp r0, #0
	beq .L_0804bf54
	b .L_0804bdc2
.L_0804bf54:
	ldr r2, [sp, #88]
	movs r1, #1
	str r2, [sp, #76]
	str r1, [sp, #80]
	ldr r4, [sp, #84]
	adds r1, r2, #0
	ldrh r3, [r4]
	str r0, [sp, #92]
	strh r3, [r1]
	ldr r2, [sp, #76]
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #254
	strh r3, [r2, #4]
	ldr r4, [sp, #76]
	movs r3, #99
	strh r3, [r4, #6]
	ldr r1, [sp, #76]
	adds r3, #157
	strh r0, [r1, #8]
	ldr r2, [sp, #76]
	strh r3, [r2, #10]
	bl .L_0804cc30
.L_0804bf84:
	cmp r6, #14
	beq .L_0804bf8c
	bl .L_0804cc30
.L_0804bf8c:
	movs r0, #154
	bl Audio_PlayCue
	ldr r4, [sp, #80]
	movs r3, #0
	str r3, [sp, #44]
	cmp r3, r4
	blt .L_0804bfa0
	bl .L_0804cc30
.L_0804bfa0:
	mov r0, sp
	adds r0, #104
	str r3, [sp, #24]
	str r3, [sp, #20]
	str r3, [sp, #28]
	str r0, [sp, #32]
.L_0804bfac:
	ldr r1, [sp, #44]
	cmp r1, #0
	bne .L_0804bfd8
	ldr r2, [sp, #36]
	movs r0, #0
	ldr r1, [r2]
	adds r1, #84
	bl Func_08118130
	b .L_0804bff2
.L_0804bfc0:
	.4byte Data_0300124c
.L_0804bfc4:
	.4byte Data_02003874
.L_0804bfc8:
	.4byte Func_0804b6a0
.L_0804bfcc:
	.4byte gPartyState
.L_0804bfd0:
	.4byte Data_03001238
.L_0804bfd4:
	.4byte gInput
.L_0804bfd8:
	ldr r4, [sp, #36]
	ldr r0, [sp, #24]
	ldr r3, [r4]
	movs r1, #3
	adds r3, r0, r3
	adds r2, r3, #0
	adds r2, #80
.L_0804bfe6:
	ldrb r3, [r2]
	subs r1, #1
	strb r3, [r2, #4]
	adds r2, #1
	cmp r1, #0
	bge .L_0804bfe6
.L_0804bff2:
	ldr r3, [sp, #96]
	ldr r1, [sp, #20]
	ldr r2, [sp, #28]
	adds r3, r3, r1
	str r3, [sp, #76]
	ldr r3, [sp, #84]
	ldrh r2, [r2, r3]
	adds r0, r2, #0
	str r2, [sp, #64]
	bl Owner_GetState
	movs r3, #192
	str r0, [sp, #72]
	lsls r3, r3, #18
	adds r3, #228
	ldr r5, [r3]
	ldr r4, [sp, #64]
	adds r3, r5, #0
	adds r3, #224
	str r4, [r3]
	ldr r3, .L_0804c06c
	movs r2, #0
	str r2, [r5, #64]
	adds r5, #24
	str r3, [r5, #4]
	str r2, [r5, #8]
	ldr r0, [sp, #72]
	movs r1, #165
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrh r0, [r3]
	ldr r1, [sp, #48]
	bl Func_08045528
	ldr r3, .L_0804c068
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_0804c070
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	ldrh r2, [r5, #6]
	ldr r3, .L_0804c074
	ands r3, r2
	ldrb r2, [r5, #9]
	strh r3, [r5, #6]
	movs r3, #128
	strb r3, [r5, #4]
	movs r3, #15
	ands r3, r2
	movs r2, #224
	orrs r3, r2
	strb r3, [r5, #9]
	ldr r3, [sp, #36]
	ldr r2, [r3]
	movs r3, #1
	adds r2, #38
	strb r3, [r2]
	b .L_0804c078
.L_0804c068:
	.4byte 0x000003ff
.L_0804c06c:
	.4byte 0x80000400
.L_0804c070:
	.4byte 0xfffffc00
.L_0804c074:
	.4byte 0xfffffe00
.L_0804c078:
	bl Func_08041b68
	movs r5, #192
	lsls r5, r5, #18
	adds r5, #228
	ldr r1, [r5]
	movs r7, #0
	adds r3, r1, #0
	adds r3, #36
	strb r7, [r3]
	ldr r2, [sp, #24]
	movs r3, #128
	adds r2, #228
	lsls r3, r3, #24
	str r3, [r1, r2]
	add r0, sp, #64
	ldr r1, [sp, #32]
	ldrh r0, [r0]
	ldr r4, .L_0804c0d8
	strh r0, [r1]
	ldr r1, [sp, #32]
	movs r3, #255
	strh r3, [r1, #2]
	ldr r0, [sp, #32]
	movs r1, #1
	mov r8, r4
	bl BattlePres_SetActorModesFar
	ldr r0, [sp, #32]
	bl Func_080461c8
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	movs r0, #1
	bl Func_080457d0
	adds r6, r0, #0
	movs r0, #1
	bl WaitFrames
	movs r2, #2
	negs r2, r2
	cmp r6, r2
	bne .L_0804c11a
	b .L_0804c0dc
	.2byte 0x0000
.L_0804c0d8:
	.4byte 0x00000000
.L_0804c0dc:
	movs r0, #12
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r0, #1
	bl BattleParty_ListActorIdsFar
	ldr r4, [sp, #36]
	adds r6, r0, #0
	ldr r3, [r4]
	mov r0, r8
	adds r3, #38
	strb r0, [r3]
	movs r0, #1
	bl WaitFrames
	adds r1, r6, #0
	ldr r2, [sp, #64]
	adds r0, r5, #0
	bl Func_08046b08
	ldr r1, [sp, #36]
	movs r2, #1
	ldr r3, [r1]
	adds r0, r5, #0
	adds r3, #38
	strb r2, [r3]
	bl Sys_Free
	b .L_0804bfac
.L_0804c11a:
	ldr r0, [sp, #32]
	movs r1, #0
	bl BattlePres_SetActorModesFar
	movs r2, #1
	negs r2, r2
	cmp r6, r2
	bne .L_0804c146
	ldr r3, [sp, #44]
	cmp r3, #0
	bne .L_0804c134
	bl .L_0804cc24
.L_0804c134:
	subs r3, #1
	lsls r4, r3, #2
	lsls r0, r3, #4
	lsls r1, r3, #1
	str r3, [sp, #44]
	str r4, [sp, #24]
	str r0, [sp, #20]
	str r1, [sp, #28]
	b .L_0804bfac
.L_0804c146:
	ldr r5, [r5]
	ldr r3, [r5, #76]
	cmp r3, #0
	bne .L_0804c150
	movs r6, #3
.L_0804c150:
	ldr r3, .L_0804c184
	str r7, [r5, #8]
	str r3, [r5, #4]
	ldr r0, [sp, #52]
	adds r1, r6, #0
	bl Func_080455dc
	ldr r3, .L_0804c17c
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_0804c188
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	cmp r6, #15
	bne .L_0804c190
	ldrh r3, [r5, #6]
	ldr r2, .L_0804c18c
	ands r2, r3
	ldr r3, .L_0804c180
	b .L_0804c198
	.2byte 0x0000
.L_0804c17c:
	.4byte 0x000003ff
.L_0804c180:
	.4byte 0x00000080
.L_0804c184:
	.4byte 0x80002400
.L_0804c188:
	.4byte 0xfffffc00
.L_0804c18c:
	.4byte 0xfffffe00
.L_0804c190:
	ldrh r3, [r5, #6]
	ldr r2, .L_0804c1c0
	ands r2, r3
	ldr r3, .L_0804c1bc
.L_0804c198:
	orrs r2, r3
	strh r2, [r5, #6]
	movs r3, #136
	strb r3, [r5, #4]
	ldr r3, [sp, #36]
	ldr r2, [r3]
	movs r3, #1
	adds r2, #36
	strb r3, [r2]
	cmp r6, #16
	bls .L_0804c1b2
	bl .L_0804cb92
.L_0804c1b2:
	ldr r2, .L_0804c1c4
	lsls r3, r6, #2
	ldr r3, [r3, r2]
	b .L_0804c1c8
	.2byte 0x0000
.L_0804c1bc:
	.4byte 0x00000060
.L_0804c1c0:
	.4byte 0xfffffe00
.L_0804c1c4:
	.4byte .L_0804c1cc
.L_0804c1c8:
	mov pc, r3
	.2byte 0x0000
.L_0804c1cc:
	.4byte .L_0804c210
	.4byte .L_0804c280
	.4byte .L_0804c970
	.4byte .L_0804cb8e
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804cb92
	.4byte .L_0804c760
	.4byte .L_0804c4e0
.L_0804c210:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #17
	movs r2, #11
	movs r3, #3
	movs r0, #11
	bl UiWindow_Create
	mov r11, r0
	mov r1, r11
	ldr r0, .L_0804c278
	movs r2, #16
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r4, [sp, #36]
	ldr r3, .L_0804c27c
	ldr r1, [r4]
	movs r0, #112
	ldrh r2, [r1, #6]
	ands r3, r2
	ldr r2, .L_0804c274
	orrs r3, r2
	strh r3, [r1, #6]
	bl Audio_PlayCue
	movs r1, #1
	ldr r0, [sp, #64]
	movs r2, #1
	movs r3, #0
	bl Func_0804a85c
	movs r1, #1
	adds r6, r0, #0
	mov r0, r11
	bl UiWork_Finalize
	movs r0, #1
	negs r0, r0
	cmp r6, r0
	bne .L_0804c264
	b .L_0804c078
.L_0804c264:
	ldr r2, [sp, #76]
	movs r1, #0
	movs r3, #1
	str r1, [sp, #60]
	str r6, [sp, #68]
	strh r3, [r2, #12]
	bl .L_0804cb92
.L_0804c274:
	.4byte 0x00000040
.L_0804c278:
	.4byte 0x0000003a
.L_0804c27c:
	.4byte 0xfffffe00
.L_0804c280:
	movs r0, #112
	bl Audio_PlayCue
	movs r6, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	str r6, [r3, #52]
	str r6, [r3, #48]
	str r6, [r3, #56]
.L_0804c296:
	ldr r3, [sp, #36]
	movs r4, #150
	ldr r1, [r3]
	ldr r3, .L_0804c2d8
	ldrh r2, [r1, #6]
	movs r0, #116
	ands r3, r2
	ldr r2, .L_0804c2d4
	adds r4, r4, r1
	orrs r3, r2
	strh r3, [r1, #6]
	ldr r2, [sp, #72]
	movs r3, #88
	ldrh r3, [r2, r3]
	movs r2, #252
	lsls r2, r2, #6
	adds r2, #255
	adds r5, r2, #0
	adds r0, r0, r1
	ands r5, r3
	mov r8, r4
	mov r10, r0
	movs r4, #0
	movs r1, #0
	cmp r5, #0
	beq .L_0804c310
	ldr r7, [sp, #72]
	mov r6, r8
	adds r7, #88
	mov r9, r2
	b .L_0804c2dc
.L_0804c2d4:
	.4byte 0x00000030
.L_0804c2d8:
	.4byte 0xfffffe00
.L_0804c2dc:
	adds r0, r5, #0
	str r1, [sp, #16]
	str r4, [sp, #4]
	bl BattleAction_Get
	ldrb r2, [r0, #1]
	movs r3, #128
	ands r3, r2
	ldr r1, [sp, #16]
	ldr r4, [sp, #4]
	cmp r3, #0
	beq .L_0804c2fe
	mov r3, r10
	strb r1, [r3, r4]
	strh r5, [r6]
	adds r4, #1
	adds r6, #2
.L_0804c2fe:
	adds r1, #1
	cmp r1, #32
	beq .L_0804c310
	adds r7, #4
	ldrh r3, [r7]
	mov r5, r9
	ands r5, r3
	cmp r5, #0
	bne .L_0804c2dc
.L_0804c310:
	movs r3, #0
	mov r0, r10
	strb r3, [r0, r4]
	ldr r3, .L_0804c334
	lsls r2, r4, #1
	mov r1, r8
	strh r3, [r2, r1]
	ldr r0, [sp, #64]
	adds r2, r4, #0
	bl Func_0804a134
	movs r2, #1
	adds r6, r0, #0
	negs r2, r2
	cmp r6, r2
	bne .L_0804c332
	b .L_0804c078
.L_0804c332:
	b .L_0804c338
.L_0804c334:
	.4byte 0x00000000
.L_0804c338:
	mov r4, r10
	ldrb r3, [r4, r6]
	ldr r0, [sp, #72]
	lsls r3, r3, #2
	adds r3, #88
	ldrh r3, [r0, r3]
	movs r1, #252
	lsls r1, r1, #6
	adds r1, #255
	ands r1, r3
	adds r0, r1, #0
	str r1, [sp, #56]
	bl BattleAction_Get
	adds r6, r0, #0
	ldr r3, [sp, #36]
	ldrb r2, [r6, #8]
	movs r0, #128
	mov r8, r2
	ldr r5, [r3]
	bl Resource_LoadIntoFreeSlot
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #60]
	movs r3, #6
	str r3, [sp, #0]
	mov r10, r0
	movs r1, #17
	movs r2, #18
	movs r3, #3
	movs r0, #8
	bl UiWindow_Create
	ldr r4, [sp, #36]
	mov r11, r0
	ldr r0, [r4]
	ldr r1, .L_0804c3c8
	ldrh r2, [r0, #6]
	adds r3, r1, #0
	ands r3, r2
	ldr r2, .L_0804c3c0
	adds r5, #12
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r3, .L_0804c3cc
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	mov r3, r11
	movs r0, #12
	ldrsh r2, [r3, r0]
	ldr r3, .L_0804c3c4
	lsls r2, r2, #3
	adds r2, #8
	ands r2, r3
	ldrh r3, [r5, #6]
	mov r0, r11
	ands r1, r3
	movs r4, #14
	ldrsh r3, [r0, r4]
	orrs r1, r2
	lsls r3, r3, #3
	adds r3, #4
	strh r1, [r5, #6]
	strb r3, [r5, #4]
	mov r1, r10
	b .L_0804c3d0
.L_0804c3c0:
	.4byte 0x00000028
.L_0804c3c4:
	.4byte 0x000001ff
.L_0804c3c8:
	.4byte 0xfffffe00
.L_0804c3cc:
	.4byte 0x40000400
.L_0804c3d0:
	ldr r0, [sp, #56]
	bl Resource_LoadIndexedEntryToBuffer
	ldr r3, .L_0804c408
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_0804c40c
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	ldr r1, [sp, #36]
	movs r3, #1
	ldr r2, [r1]
	adds r2, #37
	strb r3, [r2]
	movs r3, #5
	strb r3, [r7, #7]
	ldr r0, [sp, #72]
	ldrb r2, [r6, #9]
	movs r4, #58
	ldrsh r3, [r0, r4]
	cmp r2, r3
	ble .L_0804c410
	movs r0, #2
	bl Func_08041f70
	b .L_0804c424
	.2byte 0x0000
.L_0804c408:
	.4byte 0x000003ff
.L_0804c40c:
	.4byte 0xfffffc00
.L_0804c410:
	ldr r1, [sp, #72]
	movs r2, #62
	adds r2, #255
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804c424
	movs r0, #9
	bl Func_08041f70
.L_0804c424:
	ldr r3, [sp, #56]
	ldr r0, .L_0804c4dc
	mov r1, r11
	adds r0, r3, r0
	movs r2, #16
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r5, #0
	ldrb r0, [r6, #9]
	movs r1, #2
	mov r2, r11
	movs r3, #104
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffset
	movs r3, #15
	strb r3, [r7, #7]
	movs r0, #15
	bl Func_08041f70
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #31
	mov r0, r11
	movs r2, #11
	movs r3, #0
	str r5, [sp, #0]
	bl Func_0803c378
	movs r1, #240
	lsls r1, r1, #8
	movs r3, #0
	adds r1, #30
	mov r0, r11
	movs r2, #12
	str r5, [sp, #0]
	bl Func_0803c378
	ldrb r3, [r6, #2]
	cmp r3, #4
	beq .L_0804c48e
	movs r4, #160
	lsls r4, r4, #7
	adds r1, r3, #0
	adds r4, #1
	adds r1, r1, r4
	mov r0, r11
	movs r2, #15
	movs r3, #0
	str r5, [sp, #0]
	bl Func_0803c378
.L_0804c48e:
	ldr r1, [sp, #76]
	mov r0, r8
	strh r0, [r1, #12]
	movs r0, #112
	bl Audio_PlayCue
	adds r0, r6, #0
	bl Battle_ClassifyEntryKind
	ldrb r1, [r6]
	adds r3, r0, #0
	mov r2, r8
	ldr r0, [sp, #64]
	bl Func_0804a85c
	ldr r2, [sp, #36]
	ldr r5, .L_0804c4d8
	ldr r3, [r2]
	adds r6, r0, #0
	adds r3, #37
	strb r5, [r3]
	mov r0, r10
	bl Func_08014274
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #1
	negs r3, r3
	cmp r6, r3
	bne .L_0804c4d0
	b .L_0804c296
.L_0804c4d0:
	movs r4, #1
	str r4, [sp, #60]
.L_0804c4d4:
	str r6, [sp, #68]
	b .L_0804cb92
.L_0804c4d8:
	.4byte 0x00000000
.L_0804c4dc:
	.4byte 0x000005a7
.L_0804c4e0:
	movs r0, #112
	bl Audio_PlayCue
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #52]
	str r3, [r2, #48]
	str r3, [r2, #56]
.L_0804c4f6:
	ldr r0, [sp, #36]
	ldr r3, .L_0804c538
	ldr r1, [r0]
	ldrh r2, [r1, #6]
	ands r3, r2
	ldr r2, .L_0804c534
	orrs r3, r2
	strh r3, [r1, #6]
	ldr r0, [sp, #32]
	bl Func_080461c8
	ldr r1, [sp, #36]
	ldr r3, [sp, #24]
	ldr r2, [r1]
	movs r0, #0
	adds r2, r2, r3
	adds r2, #84
	movs r1, #0
	bl Func_08049154
	adds r6, r0, #0
	ldr r0, [sp, #32]
	bl Func_080461c8
	movs r4, #1
	negs r4, r4
	cmp r6, r4
	bne .L_0804c530
	b .L_0804c078
.L_0804c530:
	b .L_0804c53c
	.2byte 0x0000
.L_0804c534:
	.4byte 0x00000050
.L_0804c538:
	.4byte 0xfffffe00
.L_0804c53c:
	movs r0, #6
	str r0, [sp, #60]
	movs r0, #1
	str r6, [sp, #56]
	bl WaitFrames
	ldr r1, [sp, #36]
	adds r0, r6, #0
	ldr r3, [r1]
	movs r7, #0
	adds r3, #12
	mov r8, r3
	bl SummonDefinition_Get
	mov r9, r0
	ldrh r0, [r0]
	bl BattleAction_Get
	mov r10, r0
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r2, [sp, #60]
	str r0, [sp, #40]
	str r2, [sp, #0]
	movs r1, #17
	movs r2, #17
	movs r3, #3
	movs r0, #10
	bl UiWindow_Create
	mov r11, r0
	ldr r0, [sp, #36]
	ldr r4, [sp, #24]
	ldr r3, [r0]
	mov r1, r9
	adds r1, #4
	adds r4, #84
	ldrb r2, [r1]
	ldrb r3, [r3, r4]
	cmp r2, r3
	bhi .L_0804c5aa
	adds r5, r0, #0
	adds r0, r1, #0
	adds r1, r4, #0
.L_0804c596:
	adds r7, #1
	adds r1, #1
	cmp r7, #3
	bgt .L_0804c5aa
	ldr r3, [r5]
	adds r0, #1
	ldrb r2, [r0]
	ldrb r3, [r3, r1]
	cmp r2, r3
	bls .L_0804c596
.L_0804c5aa:
	ldr r1, [sp, #36]
	movs r3, #4
	eors r3, r7
	ldr r0, [r1]
	negs r2, r3
	orrs r2, r3
	ldr r1, .L_0804c5fc
	lsrs r4, r2, #31
	ldrh r2, [r0, #6]
	adds r3, r1, #0
	ands r3, r2
	ldr r2, .L_0804c5f4
	movs r5, #1
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r3, .L_0804c600
	mov r2, r8
	str r3, [r2, #4]
	movs r3, #0
	subs r4, r5, r4
	str r3, [r2, #8]
	mov r0, r11
	movs r3, #12
	ldrsh r2, [r0, r3]
	ldr r3, .L_0804c5f8
	lsls r2, r2, #3
	mov r0, r8
	adds r2, #8
	ands r2, r3
	ldrh r3, [r0, #6]
	str r4, [sp, #4]
	ands r1, r3
	orrs r1, r2
	mov r2, r8
	strh r1, [r2, #6]
	mov r1, r11
	b .L_0804c604
.L_0804c5f4:
	.4byte 0x00000038
.L_0804c5f8:
	.4byte 0x000001ff
.L_0804c5fc:
	.4byte 0xfffffe00
.L_0804c600:
	.4byte 0x40000400
.L_0804c604:
	movs r0, #14
	ldrsh r3, [r1, r0]
	movs r0, #252
	lsls r3, r3, #3
	adds r3, #4
	strb r3, [r2, #4]
	mov r2, r9
	ldrh r3, [r2]
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	ldr r1, [sp, #40]
	bl Resource_LoadIndexedEntryToBuffer
	ldr r3, .L_0804c658
	ldr r4, [sp, #4]
	ands r0, r3
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r3, .L_0804c65c
	ands r3, r2
	orrs r3, r0
	mov r0, r8
	strh r3, [r0, #8]
	ldr r1, [sp, #36]
	ldr r3, [r1]
	adds r3, #37
	strb r5, [r3]
	cmp r4, #0
	bne .L_0804c648
	movs r0, #2
	bl Func_08041f70
	ldr r4, [sp, #4]
.L_0804c648:
	adds r0, r6, #0
	str r4, [sp, #4]
	bl SummonDefinition_Get
	ldr r3, .L_0804c660
	ldrh r0, [r0]
	b .L_0804c664
	.2byte 0x0000
.L_0804c658:
	.4byte 0x000003ff
.L_0804c65c:
	.4byte 0xfffffc00
.L_0804c660:
	.4byte 0x000005a7
.L_0804c664:
	movs r2, #16
	adds r0, r0, r3
	mov r1, r11
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r2, #0
	lsls r3, r2, #1
	ldr r4, [sp, #4]
	mov r6, r9
	adds r5, r3, #0
	movs r7, #0
	mov r8, r2
	adds r6, #4
	adds r5, #11
.L_0804c682:
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_0804c6b6
	movs r3, #160
	lsls r3, r3, #7
	adds r3, #1
	mov r0, r8
	adds r1, r7, r3
	str r0, [sp, #0]
	adds r2, r5, #0
	mov r0, r11
	movs r3, #0
	str r4, [sp, #4]
	bl Func_0803c378
	ldrb r1, [r6]
	mov r3, r8
	adds r2, r5, #1
	str r3, [sp, #0]
	adds r1, #48
	mov r0, r11
	movs r3, #0
	bl Func_0803c274
	ldr r4, [sp, #4]
	adds r5, #2
.L_0804c6b6:
	adds r7, #1
	adds r6, #1
	cmp r7, #3
	ble .L_0804c682
	cmp r4, #0
	beq .L_0804c6ca
	movs r0, #112
	bl Audio_PlayCue
	b .L_0804c6d0
.L_0804c6ca:
	movs r0, #114
	bl Audio_PlayCue
.L_0804c6d0:
	mov r0, r10
	bl Battle_ClassifyEntryKind
	mov r4, r10
	adds r3, r0, #0
	ldrb r1, [r4]
	ldrb r2, [r4, #8]
	ldr r0, [sp, #64]
	bl Func_0804a85c
	adds r6, r0, #0
	mov r0, r10
	ldr r1, [sp, #76]
	ldrb r3, [r0, #8]
	ldr r2, .L_0804c714
	strh r3, [r1, #12]
	ldr r4, [sp, #36]
	ldr r3, [r4]
	adds r3, #37
	strb r2, [r3]
	ldr r0, [sp, #40]
	bl Func_08014274
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	movs r0, #1
	negs r0, r0
	cmp r6, r0
	bne .L_0804c710
	b .L_0804c4f6
.L_0804c710:
	b .L_0804c718
	.2byte 0x0000
.L_0804c714:
	.4byte 0x00000000
.L_0804c718:
	ldr r1, [sp, #36]
	movs r2, #0
	mov lr, r2
	ldr r2, [sp, #24]
	mov r12, r1
	ldr r1, [r1]
	mov r0, r9
	adds r2, #84
	adds r0, #4
	adds r5, r2, #0
	ldrb r4, [r0]
	ldrb r3, [r1, r5]
	movs r7, #0
	cmp r4, r3
	bls .L_0804c73c
	mov r3, lr
	strb r3, [r1, r5]
	b .L_0804c4d4
.L_0804c73c:
	subs r3, r3, r4
	adds r7, #1
	strb r3, [r1, r5]
	adds r0, #1
	adds r2, #1
	cmp r7, #3
	ble .L_0804c74c
	b .L_0804c4d4
.L_0804c74c:
	mov r3, r12
	ldr r1, [r3]
	adds r5, r2, #0
	ldrb r4, [r0]
	ldrb r3, [r1, r5]
	cmp r4, r3
	bls .L_0804c73c
	mov r4, lr
	strb r4, [r1, r5]
	b .L_0804c4d4
.L_0804c760:
	movs r0, #112
	bl Audio_PlayCue
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #52]
	str r3, [r2, #48]
	str r3, [r2, #56]
.L_0804c776:
	ldr r0, [sp, #36]
	ldr r2, .L_0804c7c0
	ldr r1, [r0]
	mov r10, r2
	ldrh r3, [r1, #6]
	movs r7, #192
	ands r2, r3
	ldr r3, .L_0804c7bc
	lsls r7, r7, #18
	orrs r2, r3
	strh r2, [r1, #6]
	ldr r5, [sp, #24]
	adds r7, #228
	ldr r2, [r7]
	movs r3, #128
	lsls r3, r3, #24
	adds r5, #228
	str r3, [r2, r5]
	movs r1, #1
	ldr r0, [sp, #64]
	bl Func_0804868c
	movs r3, #0
	adds r6, r0, #0
	movs r1, #1
	ldr r0, [sp, #76]
	mov r8, r3
	negs r1, r1
	mov r4, r8
	mov r9, r1
	strh r4, [r0, #12]
	cmp r6, r9
	bne .L_0804c7ba
	b .L_0804c078
.L_0804c7ba:
	b .L_0804c7c4
.L_0804c7bc:
	.4byte 0x0000008c
.L_0804c7c0:
	.4byte 0xfffffe00
.L_0804c7c4:
	movs r2, #5
	str r6, [sp, #56]
	str r2, [sp, #60]
	ldr r3, [r7]
	movs r4, #255
	str r6, [r3, r5]
	ldr r3, [sp, #56]
	ldr r0, [sp, #56]
	asrs r7, r3, #8
	movs r3, #15
	ands r4, r0
	ands r7, r3
	adds r2, r4, #0
	ldr r0, [sp, #64]
	adds r1, r7, #0
	str r4, [sp, #4]
	bl Djinn_IsActiveFar
	adds r5, r0, #0
	ldr r4, [sp, #4]
	cmp r5, #0
	beq .L_0804c8da
	adds r1, r4, #0
	adds r0, r7, #0
	bl Djinn_GetDefinitionHeaderFar
	bl BattleAction_Get
	movs r3, #6
	adds r5, r0, #0
	ldrb r6, [r5, #8]
	movs r1, #17
	str r3, [sp, #0]
	movs r2, #10
	movs r3, #3
	movs r0, #11
	bl UiWindow_Create
	ldr r2, [sp, #36]
	mov r3, r10
	ldr r1, [r2]
	mov r11, r0
	ldrh r2, [r1, #6]
	mov r0, r8
	ands r3, r2
	ldr r2, .L_0804c85c
	orrs r3, r2
	strh r3, [r1, #6]
	movs r3, #160
	lsls r3, r3, #7
	adds r3, #1
	str r0, [sp, #0]
	adds r1, r7, r3
	mov r0, r11
	movs r2, #0
	movs r3, #0
	bl Func_0803c378
	lsls r0, r7, #2
	ldr r4, [sp, #4]
	ldr r3, .L_0804c860
	adds r0, r0, r7
	lsls r0, r0, #2
	adds r0, r0, r4
	movs r2, #16
	adds r0, r0, r3
	mov r1, r11
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r1, [sp, #76]
	movs r0, #1
	strh r6, [r1, #12]
	bl WaitFrames
	b .L_0804c864
.L_0804c85c:
	.4byte 0x00000040
.L_0804c860:
	.4byte 0x000006d3
.L_0804c864:
	movs r0, #112
	bl Audio_PlayCue
	adds r0, r5, #0
	bl Battle_ClassifyEntryKind
	adds r2, r6, #0
	adds r3, r0, #0
	ldrb r1, [r5]
	ldr r0, [sp, #64]
	bl Func_0804a85c
	ldr r2, [sp, #36]
	adds r6, r0, #0
	ldr r3, [r2]
	adds r3, #216
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0804c8c6
	bl Func_0804519c
	movs r1, #15
	adds r0, #14
	movs r2, #8
	bl UiText_ShowMessageAndWaitComplete
	adds r5, r0, #0
	b .L_0804c8a2
.L_0804c89c:
	movs r0, #1
	bl WaitFrames
.L_0804c8a2:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_0804c89c
	movs r1, #1
	adds r0, r5, #0
	bl UiWork_Finalize
	ldr r3, [sp, #36]
	ldr r1, [r3]
	adds r2, r1, #0
	adds r2, #216
	ldr r3, [r2]
	adds r1, #220
	adds r3, #1
	str r3, [r2]
	movs r3, #45
	str r3, [r1]
.L_0804c8c6:
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	movs r4, #1
	negs r4, r4
	cmp r6, r4
	bne .L_0804c8d8
	b .L_0804c776
.L_0804c8d8:
	b .L_0804c4d4
.L_0804c8da:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #17
	movs r2, #10
	movs r3, #3
	movs r0, #11
	str r4, [sp, #4]
	bl UiWindow_Create
	mov r11, r0
	ldr r0, [sp, #36]
	mov r3, r10
	ldr r1, [r0]
	movs r0, #2
	ldrh r2, [r1, #6]
	ands r3, r2
	ldr r2, .L_0804c938
	orrs r3, r2
	strh r3, [r1, #6]
	bl Func_08041f70
	movs r2, #160
	lsls r2, r2, #7
	adds r2, #1
	adds r1, r7, r2
	mov r0, r11
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_0803c378
	lsls r0, r7, #2
	ldr r4, [sp, #4]
	ldr r3, .L_0804c93c
	adds r0, r0, r7
	lsls r0, r0, #2
	adds r0, r0, r4
	mov r1, r11
	movs r2, #16
	adds r0, r0, r3
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl Func_08041f70
	b .L_0804c940
.L_0804c938:
	.4byte 0x00000040
.L_0804c93c:
	.4byte 0x000006d3
.L_0804c940:
	ldr r4, [sp, #76]
	movs r3, #1
	strh r3, [r4, #12]
	movs r0, #1
	bl WaitFrames
	movs r0, #112
	bl Audio_PlayCue
	movs r1, #4
	ldr r0, [sp, #64]
	movs r2, #0
	movs r3, #7
	bl Func_0804a85c
	movs r1, #1
	adds r6, r0, #0
	mov r0, r11
	bl UiWork_Finalize
	cmp r6, r9
	bne .L_0804c96e
	b .L_0804c776
.L_0804c96e:
	b .L_0804c4d4
.L_0804c970:
	movs r0, #112
	bl Audio_PlayCue
	movs r6, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	str r6, [r3, #52]
	str r6, [r3, #48]
	str r6, [r3, #56]
.L_0804c986:
	ldr r0, [sp, #36]
	ldr r3, .L_0804c9c0
	ldr r1, [r0]
	movs r4, #0
	ldrh r2, [r1, #6]
	ands r3, r2
	ldr r2, .L_0804c9bc
	orrs r3, r2
	strh r3, [r1, #6]
	ldr r0, [sp, #72]
	movs r3, #0
	mov r8, r3
	movs r3, #216
	ldrh r5, [r0, r3]
	movs r2, #116
	adds r2, r2, r1
	adds r1, #150
	mov r9, r2
	mov r10, r1
	cmp r5, #0
	beq .L_0804c9f8
	adds r3, r0, #0
	adds r3, #216
	mov r7, r9
	mov r6, r10
	b .L_0804c9c4
	.2byte 0x0000
.L_0804c9bc:
	.4byte 0x00000060
.L_0804c9c0:
	.4byte 0xfffffe00
.L_0804c9c4:
	adds r0, r5, #0
	str r3, [sp, #8]
	str r4, [sp, #4]
	bl Item_Get
	adds r1, r5, #0
	ldr r0, [sp, #64]
	bl Item_ClassifyUseAbility
	ldr r3, [sp, #8]
	ldr r4, [sp, #4]
	cmp r0, #0
	bne .L_0804c9ea
	movs r1, #1
	strh r5, [r6]
	add r8, r1
	strb r4, [r7]
	adds r6, #2
	adds r7, #1
.L_0804c9ea:
	adds r4, #1
	cmp r4, #15
	beq .L_0804c9f8
	adds r3, #2
	ldrh r5, [r3]
	cmp r5, #0
	bne .L_0804c9c4
.L_0804c9f8:
	ldr r2, [sp, #72]
	movs r3, #216
	ldrh r5, [r2, r3]
	movs r4, #0
	cmp r5, #0
	beq .L_0804ca46
	mov r0, r8
	mov r7, r8
	lsls r3, r0, #1
	mov r1, r10
	adds r2, #216
	add r7, r9
	adds r6, r3, r1
.L_0804ca12:
	adds r0, r5, #0
	str r2, [sp, #12]
	str r4, [sp, #4]
	bl Item_Get
	adds r1, r5, #0
	ldr r0, [sp, #64]
	bl Item_ClassifyUseAbility
	ldr r2, [sp, #12]
	ldr r4, [sp, #4]
	cmp r0, #0
	beq .L_0804ca38
	movs r3, #1
	strh r5, [r6]
	add r8, r3
	strb r4, [r7]
	adds r6, #2
	adds r7, #1
.L_0804ca38:
	adds r4, #1
	cmp r4, #15
	beq .L_0804ca46
	adds r2, #2
	ldrh r5, [r2]
	cmp r5, #0
	bne .L_0804ca12
.L_0804ca46:
	ldr r1, .L_0804ca6c
	mov r4, r8
	lsls r3, r4, #1
	mov r0, r10
	strh r1, [r3, r0]
	ldr r0, [sp, #64]
	mov r1, r10
	mov r2, r8
	bl Func_08049a30
	movs r7, #1
	adds r6, r0, #0
	negs r7, r7
	cmp r6, r7
	bne .L_0804ca68
	bl .L_0804c078
.L_0804ca68:
	b .L_0804ca70
	.2byte 0x0000
.L_0804ca6c:
	.4byte 0x00000000
.L_0804ca70:
	mov r2, r9
	ldrb r6, [r2, r6]
	ldr r3, [sp, #72]
	str r6, [sp, #56]
	lsls r6, r6, #1
	adds r6, #216
	ldrh r0, [r3, r6]
	bl Item_Get
	ldrh r0, [r0, #40]
	bl BattleAction_Get
	ldrb r4, [r0, #8]
	mov r8, r0
	ldr r0, [sp, #36]
	mov r10, r4
	ldr r5, [r0]
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	movs r3, #6
	str r3, [sp, #0]
	mov r9, r0
	movs r1, #17
	movs r2, #16
	movs r3, #3
	movs r0, #9
	bl UiWindow_Create
	ldr r1, [sp, #36]
	mov r11, r0
	ldr r0, [r1]
	ldr r1, .L_0804caf8
	ldrh r2, [r0, #6]
	adds r3, r1, #0
	ands r3, r2
	ldr r2, .L_0804caf0
	adds r5, #12
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r3, .L_0804cafc
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	mov r4, r11
	movs r2, #12
	ldrsh r3, [r4, r2]
	ldr r4, .L_0804caf4
	ldrh r2, [r5, #6]
	lsls r3, r3, #3
	adds r3, #8
	ands r3, r4
	ands r1, r2
	orrs r1, r3
	strh r1, [r5, #6]
	mov r1, r11
	movs r0, #14
	ldrsh r3, [r1, r0]
	mov r1, r9
	lsls r3, r3, #3
	adds r3, #4
	strb r3, [r5, #4]
	b .L_0804cb00
	.2byte 0x0000
.L_0804caf0:
	.4byte 0x00000030
.L_0804caf4:
	.4byte 0x000001ff
.L_0804caf8:
	.4byte 0xfffffe00
.L_0804cafc:
	.4byte 0x40000400
.L_0804cb00:
	ldr r2, [sp, #72]
	str r4, [sp, #4]
	ldrh r0, [r2, r6]
	bl Resource_LoadKind26EntryToBuffer
	ldr r3, .L_0804cb44
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_0804cb48
	ldr r4, [sp, #4]
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	ldr r3, [sp, #36]
	ldr r2, [r3]
	movs r3, #1
	adds r2, #37
	strb r3, [r2]
	ldr r1, [sp, #72]
	ldr r3, .L_0804cb4c
	ldrh r0, [r1, r6]
	movs r2, #24
	ands r0, r4
	mov r1, r11
	adds r0, r0, r3
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #76]
	mov r2, r10
	strh r2, [r3, #12]
	movs r0, #112
	b .L_0804cb50
	.2byte 0x0000
.L_0804cb44:
	.4byte 0x000003ff
.L_0804cb48:
	.4byte 0xfffffc00
.L_0804cb4c:
	.4byte 0x0000025f
.L_0804cb50:
	bl Audio_PlayCue
	mov r0, r8
	bl Battle_ClassifyEntryKind
	mov r4, r8
	adds r3, r0, #0
	ldrb r1, [r4]
	ldr r0, [sp, #64]
	mov r2, r10
	bl Func_0804a85c
	adds r6, r0, #0
	ldr r0, [sp, #36]
	movs r1, #0
	ldr r3, [r0]
	mov r0, r9
	adds r3, #37
	strb r1, [r3]
	bl Func_08014274
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	cmp r6, r7
	bne .L_0804cb88
	b .L_0804c986
.L_0804cb88:
	movs r2, #2
	str r2, [sp, #60]
	b .L_0804c4d4
.L_0804cb8e:
	movs r3, #3
	str r3, [sp, #60]
.L_0804cb92:
	movs r0, #110
	bl Audio_PlayCue
	add r4, sp, #64
	ldrh r4, [r4]
	ldr r0, [sp, #76]
	strh r4, [r0]
	ldr r5, [sp, #72]
	adds r5, #64
	ldrh r6, [r5]
	cmp r6, #0
	beq .L_0804cbb6
	bl Random16
	ldrh r3, [r5]
	muls r3, r0
	lsrs r3, r3, #20
	adds r6, r6, r3
.L_0804cbb6:
	ldr r0, [sp, #76]
	strh r6, [r0, #4]
	ldr r1, [sp, #44]
	cmp r1, #0
	beq .L_0804cbde
	ldr r2, [sp, #28]
	ldr r4, [sp, #84]
	adds r3, r2, r4
	subs r2, r3, #2
	ldrh r1, [r3]
	ldrh r3, [r2]
	cmp r1, r3
	bne .L_0804cbde
	lsls r2, r6, #16
	asrs r3, r2, #16
	ldr r0, [sp, #76]
	lsrs r2, r2, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	strh r3, [r0, #4]
.L_0804cbde:
	ldr r2, [sp, #76]
	movs r1, #4
	ldrsh r3, [r2, r1]
	cmp r3, #0
	bge .L_0804cbf0
	movs r3, #250
	lsls r3, r3, #3
	adds r4, r2, #0
	strh r3, [r4, #4]
.L_0804cbf0:
	add r0, sp, #60
	ldrh r0, [r0]
	ldr r1, [sp, #76]
	strh r0, [r1, #6]
	add r1, sp, #56
	ldrh r1, [r1]
	ldr r2, [sp, #76]
	strh r1, [r2, #8]
	add r2, sp, #68
	ldrh r2, [r2]
	ldr r3, [sp, #76]
	strh r2, [r3, #10]
	ldr r3, [sp, #44]
	ldr r2, [sp, #80]
	adds r3, #1
	lsls r4, r3, #2
	lsls r0, r3, #4
	lsls r1, r3, #1
	str r3, [sp, #44]
	str r4, [sp, #24]
	str r0, [sp, #20]
	str r1, [sp, #28]
	cmp r3, r2
	bge .L_0804cc24
	bl .L_0804bfac
.L_0804cc24:
	ldr r3, [sp, #44]
	ldr r4, [sp, #80]
	cmp r3, r4
	bge .L_0804cc30
	bl .L_0804bdc2
.L_0804cc30:
	ldr r1, [sp, #36]
	ldr r0, [r1]
	ldr r3, [r0, #80]
	cmp r3, #0
	beq .L_0804cc4c
	ldr r2, .L_0804cc80
	ldr r3, .L_0804cc70
	strh r3, [r2, #8]
	ldr r3, .L_0804cc74
	strh r3, [r2, #10]
	ldr r3, .L_0804cc78
	strh r3, [r2, #12]
	ldr r3, .L_0804cc7c
	strh r3, [r2, #14]
.L_0804cc4c:
	ldr r0, [r0, #68]
	cmp r0, #0
	beq .L_0804cc58
	movs r1, #1
	bl UiWork_Finalize
.L_0804cc58:
	ldr r0, [sp, #48]
	bl Func_08014274
	ldr r0, [sp, #52]
	bl Func_08014274
	ldr r0, .L_0804cc84
	bl Func_08014644
	ldr r3, [sp, #36]
	ldr r2, [r3]
	b .L_0804cc88
.L_0804cc70:
	.4byte 0x00000045
.L_0804cc74:
	.4byte 0x00000044
.L_0804cc78:
	.4byte 0x00000056
.L_0804cc7c:
	.4byte 0x00000053
.L_0804cc80:
	.4byte Data_02003a74
.L_0804cc84:
	.4byte Func_0804b6a0
.L_0804cc88:
	ldr r3, [r2, #80]
	cmp r3, #0
	beq .L_0804cd78
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #36]
	ldr r3, [r2, #68]
	movs r6, #0
	cmp r3, #0
	bne .L_0804ccca
	adds r7, r5, #0
	adds r7, #82
	ldrb r3, [r7]
	cmp r3, #0
	bne .L_0804ccce
	movs r3, #42
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #30
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	ldr r4, [sp, #36]
	ldr r3, [r4]
	str r0, [r3, #68]
	bl Func_080396bc
	add r0, sp, #108
	mov r9, r0
	bl Func_0804bbd0
	b .L_0804ccd6
.L_0804ccca:
	adds r7, r5, #0
	adds r7, #82
.L_0804ccce:
	ldr r1, [sp, #36]
	movs r3, #0
	ldr r2, [r1]
	str r3, [r2, #68]
.L_0804ccd6:
	adds r5, #80
	ldrb r2, [r5]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_0804cda0
	lsls r2, r2, #3
	adds r1, r2, r3
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_0804ccf6
	movs r2, #1
	negs r2, r2
	str r2, [sp, #80]
	b .L_0804cd68
.L_0804ccf6:
	ldr r3, .L_0804cda4
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0804cd10
	adds r6, #1
	cmp r6, #24
	ble .L_0804cd46
	movs r3, #1
	negs r3, r3
	str r3, [sp, #80]
	b .L_0804cd68
.L_0804cd10:
	ldrh r2, [r1, #8]
	movs r6, #0
	adds r3, r2, #0
	cmp r3, #69
	bne .L_0804cd2c
	ldrh r3, [r1, #10]
	cmp r3, #68
	bne .L_0804cd2c
	ldrh r3, [r1, #12]
	cmp r3, #86
	bne .L_0804cd2c
	ldrh r3, [r1, #14]
	cmp r3, #83
	beq .L_0804cd68
.L_0804cd2c:
	adds r3, r2, #0
	cmp r3, #86
	bne .L_0804cd44
	ldrh r3, [r1, #10]
	cmp r3, #83
	bne .L_0804cd44
	ldrh r3, [r1, #12]
	cmp r3, #83
	bne .L_0804cd44
	ldrh r3, [r1, #14]
	cmp r3, #84
	beq .L_0804cd46
.L_0804cd44:
	movs r6, #1
.L_0804cd46:
	movs r0, #1
	bl WaitFrames
	ldrb r2, [r5]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_0804cda0
	lsls r2, r2, #3
	adds r1, r2, r3
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_0804ccf6
	movs r4, #1
	negs r4, r4
	str r4, [sp, #80]
.L_0804cd68:
	ldr r0, [sp, #36]
	ldr r3, [r0]
	ldr r0, [r3, #68]
	cmp r0, #0
	beq .L_0804cd78
	movs r1, #1
	bl UiWork_Finalize
.L_0804cd78:
	movs r0, #0
	bl Camera_ConfigureSceneFar
	movs r0, #228
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #80]
	cmp r1, #0
	blt .L_0804cd90
	ldr r3, [sp, #92]
	adds r1, r1, r3
	str r1, [sp, #80]
.L_0804cd90:
	ldr r0, [sp, #80]
	add sp, #108
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804cda0:
	.4byte Data_02003874
.L_0804cda4:
	.4byte Data_0300124c
