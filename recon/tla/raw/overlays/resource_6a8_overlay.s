.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020083bc,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	adds r5, r0, #0
	adds r6, r5, #0
	ldr r7, [r3, #108]
	adds r6, #100
	movs r3, #0
	mov r8, r2
	mov r10, r3
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_ja_020083e4
	movs r0, #18
	b .L_ja_020083ee
.L_ja_020083e4:
	cmp r3, #1
	bne .L_ja_020083ec
	movs r0, #16
	b .L_ja_020083ee
.L_ja_020083ec:
	movs r0, #17
.L_ja_020083ee:
	bl Actor_GetById
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #0
	bl Func_020002d8
	cmp r0, #0
	bne .L_ja_02008440
	ldr r3, .L_ja_0200844c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	movs r2, #176
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_ja_02008428
	mov r2, r8
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_ja_02008436
.L_ja_02008428:
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02008436
	movs r3, #1
	mov r10, r3
.L_ja_02008436:
	adds r0, r5, #0
	movs r2, #32
	mov r3, r10
	bl Func_020002d8
.L_ja_02008440:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200844c:
	.4byte gMainRoutineWork
	.endif
	.section .text.x0200bbc8,"ax",%progbits
	.balign 4
	.ifdef TLA_EDITION_JA
	.global Func_02003bc8
	.thumb_func
Func_02003bc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200bc00
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_ja_0200bc04
	ldr r3, .L_ja_0200bbfc
	adds r7, r0, #0
	strh r3, [r2]
.L_ja_0200bbc6:
	bl Func_020038e8
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_ja_0200bc08
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_ja_0200bc0c
.L_ja_0200bbfc:
	.4byte 0x00000000
.L_ja_0200bc00:
	.4byte gMainRoutineWork
.L_ja_0200bc04:
	.4byte gData_020062a4
.L_ja_0200bc08:
	.4byte 0xfff00000
.L_ja_0200bc0c:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004424
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_ja_0200bc78
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_ja_0200bc88
	ldr r3, .L_ja_0200bc7c
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_ja_0200bc80
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_ja_0200bc84
	cmp r3, r2
	bne .L_ja_0200bcb4
	b .L_ja_0200be7c
.L_ja_0200bc78:
	.4byte IwramMulQ16
.L_ja_0200bc7c:
	.4byte gInput
.L_ja_0200bc80:
	.4byte gData_02004780
.L_ja_0200bc84:
	.4byte 0xffff0000
.L_ja_0200bc88:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_ja_0200bcb0
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_ja_0200bcb4
.L_ja_0200bcb0:
	.4byte 0xffffc000
.L_ja_0200bcb4:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	beq .L_ja_0200bd36
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200bd36
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_020043f4
	adds r0, r7, #0
	movs r1, #2
	bl Func_020043c4
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_020043fc
	ldr r3, .L_ja_0200be8c
	str r3, [r7, #108]
	b .L_ja_0200be12
.L_ja_0200bd36:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_ja_0200be5e
.L_ja_0200bd4a:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200be32
	ldr r3, .L_ja_0200be90
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200be32
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_ja_0200bd82:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_ja_0200bdac
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200bdac
	cmp r5, r7
	beq .L_ja_0200bdac
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004454
	cmp r0, #0
	bge .L_ja_0200be32
.L_ja_0200bdac:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_ja_0200bd82
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_020043f4
	b .L_ja_0200bdea
.L_ja_0200bde2:
	movs r0, #1
	bl wait
	adds r5, #1
.L_ja_0200bdea:
	cmp r5, #179
	bgt .L_ja_0200be02
	adds r0, r7, #0
	bl Func_0200444c
	cmp r0, #0
	bne .L_ja_0200be02
	ldr r3, .L_ja_0200be90
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_ja_0200bde2
.L_ja_0200be02:
	ldr r3, .L_ja_0200be90
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200be5e
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_ja_0200be58
.L_ja_0200be12:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	bne .L_ja_0200bd4a
.L_ja_0200be32:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_020043f4
	adds r0, r7, #0
	bl Func_020043fc
	movs r0, #2
	bl wait
	b .L_ja_0200bbc6
.L_ja_0200be58:
	movs r0, #10
	bl wait
.L_ja_0200be5e:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_020043c4
.L_ja_0200be7c:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200be8c:
	.4byte Func_0200385c
.L_ja_0200be90:
	.4byte gData_020062a4
	.else
	.global Func_02003bc8
	.thumb_func
Func_02003bc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200bc28
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_0200bc2c
	ldr r3, .L_0200bc24
	adds r7, r0, #0
	strh r3, [r2]
.L_0200bbee:
	bl Func_020038e8
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200bc30
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_0200bc34
.L_0200bc24:
	.4byte 0x00000000
.L_0200bc28:
	.4byte gMainRoutineWork
.L_0200bc2c:
	.4byte gData_020062a4
.L_0200bc30:
	.4byte 0xfff00000
.L_0200bc34:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004424
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200bca0
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200bcb0
	ldr r3, .L_0200bca4
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200bca8
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200bcac
	cmp r3, r2
	bne .L_0200bcdc
	b .L_0200bea6
.L_0200bca0:
	.4byte IwramMulQ16
.L_0200bca4:
	.4byte gInput
.L_0200bca8:
	.4byte gData_02004780
.L_0200bcac:
	.4byte 0xffff0000
.L_0200bcb0:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200bcd8
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200bcdc
.L_0200bcd8:
	.4byte 0xffffc000
.L_0200bcdc:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	beq .L_0200bd56
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200bd56
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_020043f4
	adds r0, r7, #0
	movs r1, #2
	bl Func_020043c4
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	movs r5, #0
	b .L_0200bd7e
.L_0200bd56:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200bea6
.L_0200bd6a:
	ldr r3, .L_0200bed4
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200bd76
	b .L_0200bea6
.L_0200bd76:
	movs r0, #1
	bl wait
	adds r5, #1
.L_0200bd7e:
	cmp r5, #179
	bgt .L_0200bd8c
	adds r0, r7, #0
	bl Func_0200444c
	cmp r0, #0
	beq .L_0200bd6a
.L_0200bd8c:
	ldr r3, .L_0200bed8
	str r3, [r7, #108]
	b .L_0200be5a
.L_0200bd92:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200be7a
	ldr r3, .L_0200bed4
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200bea6
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200bdca:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200bdf4
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200bdf4
	cmp r5, r7
	beq .L_0200bdf4
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004454
	cmp r0, #0
	bge .L_0200be7a
.L_0200bdf4:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200bdca
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_020043f4
	b .L_0200be32
.L_0200be2a:
	movs r0, #1
	bl wait
	adds r5, #1
.L_0200be32:
	cmp r5, #179
	bgt .L_0200be4a
	adds r0, r7, #0
	bl Func_0200444c
	cmp r0, #0
	bne .L_0200be4a
	ldr r3, .L_0200bed4
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200be2a
.L_0200be4a:
	ldr r3, .L_0200bed4
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200bea6
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200bea0
.L_0200be5a:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	bne .L_0200bd92
.L_0200be7a:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_020043f4
	adds r0, r7, #0
	bl Func_020043fc
	movs r0, #2
	bl wait
	b .L_0200bbee
.L_0200bea0:
	movs r0, #10
	bl wait
.L_0200bea6:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_020043c4
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bed4:
	.4byte gData_020062a4
.L_0200bed8:
	.4byte Func_0200385c
	.endif
	.section .text.x0200c254,"ax",%progbits
	.balign 4
	.global Func_02004254
	.thumb_func
Func_02004254:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #8
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	adds r6, r0, #0
	ldr r0, .L_0200c308
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02004384
	bl Resource_FindFreeEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramCache_Load
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #2
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #160
	movs r3, #192
	lsls r2, r2, #3
	lsls r3, r3, #4
	movs r1, #63
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_0200c2ac:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02004604
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r8, r3
	mov r2, r8
	str r3, [r7, #24]
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_0200c2ac
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200c30c
	bl Task_Add
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c308:
	.4byte 0x000001f0
.L_0200c30c:
	.4byte Func_02004114
	.section .bss,"aw",%nobits
	.global gData_020062a4
gData_020062a4:
