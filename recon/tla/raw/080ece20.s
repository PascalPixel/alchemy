.syntax unified
	.thumb
	.global Func_080ece20
	.thumb_func
Func_080ece20:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_080eceb0
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #44
	adds r3, r1, r2
	ldr r3, [r3]
	ldr r4, .L_080eceb4
	ldmia r3!, {r2}
	sub sp, #8
	adds r5, r2, r4
	ldr r3, [r3]
	ldr r2, .L_080eceb8
	ldr r0, .L_080ecebc
	adds r6, r3, r2
	movs r3, #200
	lsls r3, r3, #3
	adds r1, r1, r3
	movs r4, #0
	ldrsh r3, [r1, r4]
	cmp r3, #0
	beq .L_080ece5e
	ldr r0, .L_080ecec0
	movs r5, #0
	movs r6, #0
.L_080ece5e:
	bl Resource_GetTableEntry
	mov r9, r0
	ldr r0, [r0]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	add r0, r9
	lsls r1, r1, #19
	adds r2, #120
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080eceac
	movs r2, #160
	lsls r2, r2, #19
	strh r3, [r2]
	movs r3, #136
	movs r2, #224
	lsls r3, r3, #17
	lsls r2, r2, #16
	cmp r5, #0
	bge .L_080ece92
	movs r5, #0
.L_080ece92:
	cmp r5, r3
	ble .L_080ece98
	adds r5, r3, #0
.L_080ece98:
	cmp r6, #0
	bge .L_080ece9e
	movs r6, #0
.L_080ece9e:
	cmp r6, r2
	ble .L_080ecea4
	adds r6, r2, #0
.L_080ecea4:
	ldr r3, .L_080ecec4
	asrs r2, r5, #16
	b .L_080ecec8
	.2byte 0x0000
.L_080eceac:
	.4byte 0x00000000
.L_080eceb0:
	.4byte Data_0202a000
.L_080eceb4:
	.4byte 0xff880000
.L_080eceb8:
	.4byte 0xffb00000
.L_080ecebc:
	.4byte 0x00000024
.L_080ecec0:
	.4byte 0x00000023
.L_080ecec4:
	.4byte Data_03001120
.L_080ecec8:
	lsrs r5, r5, #19
	strh r2, [r3, #4]
	mov r8, r5
	asrs r2, r6, #16
	lsrs r6, r6, #19
	strh r2, [r3, #6]
	movs r2, #31
	mov r3, r8
	str r6, [sp, #4]
	movs r1, #0
	ands r3, r2
	mov r10, r1
	mov r11, r3
.L_080ecee2:
	ldr r7, [sp, #4]
	add r7, r10
	adds r2, r7, #0
	cmp r7, #23
	ble .L_080eceee
	subs r7, #24
.L_080eceee:
	lsls r3, r2, #5
	mov r4, r11
	adds r6, r4, r3
	adds r0, r6, #0
	movs r1, #192
	lsls r1, r1, #2
	adds r0, #128
	str r2, [sp, #0]
	bl Math_Mod
	ldr r2, [sp, #0]
	movs r1, #128
	lsls r3, r2, #2
	add r3, r9
	lsls r1, r1, #1
	adds r6, r0, r1
	ldr r2, .L_080ecf70
	ldr r0, [r3, #4]
	lsls r1, r7, #12
	add r0, r9
	adds r1, r1, r2
	bl Resource_DecodeType01
	lsls r5, r7, #6
	ldr r3, .L_080ecf70
	add r5, r8
	movs r7, #192
	lsls r5, r5, #6
	movs r4, #0
	lsls r7, r7, #19
	adds r5, r5, r3
.L_080ecf2c:
	movs r3, #128
	movs r2, #132
	lsls r1, r6, #6
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	adds r1, r1, r7
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r1, r8
	adds r3, r4, r1
	adds r5, #64
	adds r6, #1
	cmp r3, #31
	bne .L_080ecf50
	subs r6, #32
.L_080ecf50:
	adds r4, #1
	cmp r4, #30
	bls .L_080ecf2c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #20
	bls .L_080ecee2
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ecf70:
	.4byte gMapCellBuffer
