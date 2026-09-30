.syntax unified
	.thumb
	.global Func_08118958
	.thumb_func
Func_08118958:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08118ac8
	movs r2, #0
	ldr r7, [r3]
	movs r3, #128
	mov r9, r2
	ands r7, r3
	bl Func_080ad090
.L_08118976:
	movs r5, #181
	lsls r5, r5, #1
	bl Func_08014bac
	bl Func_08014b70
	bl Func_080144c0
	bl Func_08014c6c
	bl Func_08014368
	adds r0, r5, #0
	movs r6, #69
	bl GameFlag_SetBit
	cmp r7, #0
	bne .L_0811899c
	b .L_08118ab8
.L_0811899c:
	movs r2, #1
	movs r3, #0
	negs r2, r2
	adds r0, r5, #0
	mov r8, r3
	mov r11, r2
	bl GameFlag_ClearBit
	ldr r5, .L_08118ac8
.L_081189ae:
	movs r0, #32
	bl GameFlag_ClearBit
	movs r0, #1
	bl WaitFrames
	b .L_081189e4
.L_081189bc:
	ldr r3, [r5, #12]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_081189ca
	bl Func_08118954
.L_081189ca:
	ldr r3, [r5, #12]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_081189da
	mov r3, r9
	cmp r3, #0
	beq .L_081189de
.L_081189da:
	movs r2, #1
	mov r9, r2
.L_081189de:
	movs r0, #1
	bl WaitFrames
.L_081189e4:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_081189f0
	adds r6, #1
.L_081189f0:
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_081189fc
	subs r6, #1
.L_081189fc:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08118a08
	subs r6, #10
.L_08118a08:
	ldr r3, [r5, #12]
	movs r7, #128
	ands r3, r7
	cmp r3, #0
	beq .L_08118a14
	adds r6, #10
.L_08118a14:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08118a24
	movs r3, #1
	add r8, r3
.L_08118a24:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08118a36
	movs r2, #1
	negs r2, r2
	add r8, r2
.L_08118a36:
	ldr r3, [r5, #12]
	movs r2, #1
	ands r3, r2
	mov r10, r2
	cmp r3, #0
	beq .L_081189bc
	cmp r8, r11
	beq .L_08118a52
	bl Func_080ad090
	mov r0, r8
	bl DebugParty_LoadPreset
	mov r11, r8
.L_08118a52:
	ldr r3, [r5]
	ands r3, r7
	cmp r3, #0
	beq .L_08118a62
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_08118a62:
	movs r0, #0
	bl Owner_RecalculateStatsFar
	ldr r3, .L_08118acc
	ldr r2, .L_08118ad0
	ldr r7, .L_08118ad4
	strh r3, [r2]
	cmp r6, #28
	bne .L_08118a7c
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_08118a7c:
	movs r0, #177
	lsls r0, r0, #1
	bl GameFlag_SetBit
	mov r3, r9
	cmp r3, #0
	beq .L_08118a96
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r7, r3
	movs r3, #5
	strb r3, [r2]
.L_08118a96:
	adds r0, r6, #0
	bl Func_081197f0
	ldr r3, .L_08118ad8
	mov r2, r10
	strb r2, [r3]
	bl Func_08014bac
	bl Func_08014b70
	bl Func_080144c0
	bl Func_08014c6c
	bl Func_08014368
	b .L_081189ae
.L_08118ab8:
	movs r0, #177
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #69
	bl Func_081197f0
	b .L_08118976
.L_08118ac8:
	.4byte gInput
.L_08118acc:
	.4byte 0x0000002b
.L_08118ad0:
	.4byte Data_02000436
.L_08118ad4:
	.4byte gPartyState
.L_08118ad8:
	.4byte Data_03001110
