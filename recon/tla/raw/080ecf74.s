.syntax unified
	.thumb
	.global Func_080ecf74
	.thumb_func
Func_080ecf74:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_080ed020
	mov r8, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r6, r1, #0
	ldr r0, .L_080ed024
	cmp r3, #0
	beq .L_080ecf90
	ldr r0, .L_080ed028
.L_080ecf90:
	bl Resource_GetTableEntry
	mov r10, r6
	adds r7, r0, #0
	cmp r6, #23
	ble .L_080ecf9e
	subs r6, #24
.L_080ecf9e:
	lsls r3, r6, #6
	add r3, r8
	mov r2, r8
	mov r9, r3
	movs r3, #31
	ands r3, r2
	lsls r2, r6, #5
	adds r5, r3, r2
	adds r0, r5, #0
	movs r1, #192
	lsls r1, r1, #2
	adds r0, #128
	bl __modsi3
	movs r3, #128
	lsls r3, r3, #1
	mov r2, r10
	adds r5, r0, r3
	lsls r3, r2, #2
	adds r3, r3, r7
	ldr r0, [r3, #4]
	lsls r1, r6, #12
	ldr r6, .L_080ed02c
	adds r0, r7, r0
	adds r1, r1, r6
	bl Resource_DecodeType01
	mov r3, r8
	movs r7, #0
	cmp r3, #63
	bhi .L_080ed016
	movs r2, #192
	lsls r2, r2, #19
	mov r12, r2
	mov r2, r9
	lsls r3, r2, #6
	mov r4, r8
	adds r6, r3, r6
.L_080ecfea:
	movs r3, #128
	movs r2, #132
	lsls r1, r5, #6
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r6, #0
	add r1, r12
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #64
	adds r5, #1
	cmp r4, #31
	bne .L_080ed00a
	subs r5, #32
.L_080ed00a:
	adds r7, #1
	adds r4, #1
	cmp r7, #30
	bhi .L_080ed016
	cmp r4, #63
	bls .L_080ecfea
.L_080ed016:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080ed020:
	.4byte Data_0202a640
.L_080ed024:
	.4byte 0x00000024
.L_080ed028:
	.4byte 0x00000023
.L_080ed02c:
	.4byte gMapCellBuffer
