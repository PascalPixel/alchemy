.syntax unified
	.thumb
	.global Func_08016bdc
	.thumb_func
Func_08016bdc:
	push {r5, r6, lr}
	movs r0, #3
	sub sp, #4
	bl Audio_PlayCue
	bl Func_08016180
	ldr r2, .L_08016c84
	ldr r3, .L_08016c88
	movs r1, #19
.L_08016bf0:
	subs r1, #1
	strh r3, [r2]
	subs r2, #2
	subs r3, #1
	cmp r1, #0
	bge .L_08016bf0
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	ldr r1, .L_08016c8c
	ldr r2, .L_08016c90
	bl Bios_CpuSet
	movs r0, #3
	bl Func_080167d8
.L_08016c10:
	ldr r0, .L_08016c8c
	bl Party_Check
	ldr r6, .L_08016c94
.L_08016c18:
	ldr r3, [r6]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08016c2e
	movs r0, #128
	movs r1, #160
	lsls r0, r0, #20
	lsls r1, r1, #2
	bl Func_0801680c
.L_08016c2e:
	ldr r3, [r6]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08016c42
	movs r1, #160
	ldr r0, .L_08016c98
	lsls r1, r1, #2
	bl Func_0801680c
.L_08016c42:
	ldr r3, [r6]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_08016c5c
	movs r5, #156
	lsls r5, r5, #6
	adds r5, #15
.L_08016c52:
	subs r5, #1
	bl Func_08016bd8
	cmp r5, #0
	bge .L_08016c52
.L_08016c5c:
	ldr r3, .L_08016c9c
	ldr r3, [r3]
	cmp r3, #0
	bne .L_08016c7a
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08016c8c
	ldr r1, .L_08016ca0
	adds r2, #160
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_08016c10
.L_08016c7a:
	movs r0, #1
	bl WaitFrames
	b .L_08016c18
	.2byte 0x0000
.L_08016c84:
	.4byte 0x06002426
.L_08016c88:
	.4byte 0xfffff093
.L_08016c8c:
	.4byte gMapCellBuffer
.L_08016c90:
	.4byte 0x05000100
.L_08016c94:
	.4byte gInput
.L_08016c98:
	.4byte IwramRuntime_Rom + 0x948
.L_08016c9c:
	.4byte Data_020055d0
.L_08016ca0:
	.4byte 0x06001000
