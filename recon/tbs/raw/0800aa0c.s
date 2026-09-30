.syntax unified
	.thumb
	.global Func_0800aa0c
	.thumb_func
Func_0800aa0c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	lsls r1, r1, #16
	sub sp, #56
	asrs r7, r1, #16
	movs r1, #0
	str r0, [sp, #44]
	str r1, [sp, #40]
	ldr r3, .L_0800ad2c
	ldr r2, [r3]
	movs r4, #1
	str r2, [sp, #32]
	str r4, [sp, #24]
	adds r5, r3, #0
	adds r5, #184
	ldr r1, [r5]
	str r1, [sp, #28]
	cmp r1, #0
	bne .L_0800aa62
	ldr r1, .L_0800ad30
	movs r0, #52
	bl Runtime_AllocateHeapBlock
	ldr r2, .L_0800ad34
	adds r1, r0, #0
	ldr r0, .L_0800ad38
	movs r4, #132
	subs r2, r2, r0
	lsls r4, r4, #24
	lsrs r2, r2, #2
	ldr r3, .L_0800ad3c
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [r5]
	movs r2, #0
	str r5, [sp, #28]
	str r2, [sp, #24]
.L_0800aa62:
	ldr r4, [sp, #44]
	movs r3, #0
	adds r4, #39
	ldr r1, [sp, #44]
	mov r8, r3
	ldr r2, [sp, #40]
	ldrb r3, [r4]
	adds r1, #37
	mov r11, r4
	str r1, [sp, #0]
	cmp r2, r3
	blt .L_0800aa7c
	b .L_0800ada0
.L_0800aa7c:
	mov r4, r8
	lsls r3, r4, #2
	ldr r1, [sp, #44]
	adds r3, #40
	ldr r6, [r1, r3]
	cmp r6, #0
	bne .L_0800aa8c
	b .L_0800ad92
.L_0800aa8c:
	ldr r3, [r6, #16]
	cmp r3, #0
	bne .L_0800aa94
	b .L_0800ad92
.L_0800aa94:
	movs r4, #2
	ldrsh r3, [r6, r4]
	ldrh r2, [r6, #2]
	cmp r3, #0
	bgt .L_0800ab66
	ldrb r3, [r6, #20]
	adds r2, r3, #1
	strb r2, [r6, #20]
	ldr r1, [r6, #16]
	lsls r3, r3, #24
	lsrs r3, r3, #24
	ldrb r0, [r1, r3]
	adds r3, r2, #1
	strb r3, [r6, #20]
	lsls r2, r2, #24
	adds r3, r0, #0
	lsrs r2, r2, #24
	subs r3, #239
	ldrb r5, [r1, r2]
	cmp r3, #16
	bhi .L_0800ab5a
	ldr r2, .L_0800ad40
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0800aac8:
	.4byte .L_0800ab46
	.4byte .L_0800ab32
	.4byte .L_0800ab2a
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xab20
	.2byte 0x0800
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xab5a
	.2byte 0x0800
	.2byte 0xaa94
	.2byte 0x0800
	.2byte 0xab1c
	.2byte 0x0800
	.2byte 0xab0c
	.2byte 0x0800
	.2byte 0xab36
	.2byte 0x0800
	.2byte 0x1c30
	.2byte 0x1c29
	.2byte 0xf000
	.2byte 0xff70
	.2byte 0x9b0b
	.2byte 0x3324
	.2byte 0x701d
	.2byte 0xe7bb
	.2byte 0x7535
	.2byte 0xe7b9
	.2byte 0x8873
	.2byte 0x012a
	.2byte 0x189b
	.2byte 0x8073
	.2byte 0xe7b4
.L_0800ab2a:
	ldrb r3, [r6, #20]
	adds r3, #254
	strb r3, [r6, #20]
	b .L_0800ab6c
.L_0800ab32:
	strb r5, [r6, #4]
	b .L_0800aa94
	.2byte 0x23ff
	.2byte 0x75f3
	.2byte 0x8873
	.2byte 0x012a
	.2byte 0x189b
	.2byte 0x20ff
	.2byte 0x8073
	.2byte 0xe013
.L_0800ab46:
	movs r3, #255
	mov r1, r11
	strb r3, [r6, #23]
	movs r3, #0
	str r3, [r6, #16]
	ldrb r3, [r1]
	adds r3, #255
	movs r0, #255
	strb r3, [r1]
	b .L_0800ab6e
.L_0800ab5a:
	ldrh r3, [r6, #2]
	lsls r2, r5, #4
	adds r3, r3, r2
	strb r0, [r6, #23]
	strh r3, [r6, #2]
	b .L_0800ab6e
.L_0800ab66:
	ldrb r3, [r6, #21]
	subs r3, r2, r3
	strh r3, [r6, #2]
.L_0800ab6c:
	ldrb r0, [r6, #23]
.L_0800ab6e:
	ldrb r3, [r6, #4]
	subs r3, #1
	cmp r3, #87
	bls .L_0800ab78
	b .L_0800ad6c
.L_0800ab78:
	ldr r2, .L_0800ad44
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0800ab80:
	.4byte .L_0800ace0
	.4byte .L_0800ace6
	.4byte .L_0800acf2
	.4byte .L_0800acf6
	.4byte .L_0800acfa
	.4byte .L_0800ad04
	.4byte .L_0800ad6c
	.4byte .L_0800ad0e
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ace6
	.4byte .L_0800ad6c
	.4byte .L_0800acec
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad6c
	.4byte .L_0800ad1a
.L_0800ace0:
	ldr r2, .L_0800ad48
	lsls r3, r7, #16
	b .L_0800ad24
.L_0800ace6:
	ldr r2, .L_0800ad4c
	lsls r3, r7, #16
	b .L_0800ad24
.L_0800acec:
	ldr r2, .L_0800ad50
	lsls r3, r7, #16
	b .L_0800ad24
.L_0800acf2:
	ldr r2, .L_0800ad54
	b .L_0800acfc
.L_0800acf6:
	ldr r2, .L_0800ad58
	b .L_0800ad06
.L_0800acfa:
	ldr r2, .L_0800ad5c
.L_0800acfc:
	lsls r3, r7, #16
	lsrs r3, r3, #28
	ldrb r2, [r2, r3]
	b .L_0800ad6e
.L_0800ad04:
	ldr r2, .L_0800ad60
.L_0800ad06:
	lsls r3, r7, #16
	lsrs r3, r3, #26
	ldrb r2, [r2, r3]
	b .L_0800ad6e
.L_0800ad0e:
	movs r4, #128
	lsls r3, r7, #16
	lsls r4, r4, #21
	ldr r2, .L_0800ad64
	adds r3, r3, r4
	b .L_0800ad24
.L_0800ad1a:
	movs r1, #128
	lsls r3, r7, #16
	lsls r1, r1, #21
	ldr r2, .L_0800ad68
	adds r3, r3, r1
.L_0800ad24:
	lsrs r3, r3, #29
	ldrb r2, [r2, r3]
	b .L_0800ad6e
	.2byte 0x0000
.L_0800ad2c:
	.4byte gMenuCtrlWork
.L_0800ad30:
	.4byte 0x000002c4
.L_0800ad34:
	.4byte 0x08009d9c
.L_0800ad38:
	.4byte Render_DecodeFrame
.L_0800ad3c:
	.4byte 0x040000d4
.L_0800ad40:
	.4byte .L_0800aac8
.L_0800ad44:
	.4byte .L_0800ab80
.L_0800ad48:
	.4byte Data_0801307c
.L_0800ad4c:
	.4byte Data_08013094
.L_0800ad50:
	.4byte Data_0801308c
.L_0800ad54:
	.4byte Data_0801309c
.L_0800ad58:
	.4byte Data_080130cc
.L_0800ad5c:
	.4byte Data_080130ac
.L_0800ad60:
	.4byte Data_0801310c
.L_0800ad64:
	.4byte Data_080130bc
.L_0800ad68:
	.4byte Data_080130c4
.L_0800ad6c:
	movs r2, #0
.L_0800ad6e:
	movs r3, #7
	ands r3, r2
	adds r0, r0, r3
	mov r3, r8
	cmp r3, #0
	bne .L_0800ad84
	lsrs r3, r2, #7
	cmp r3, #0
	beq .L_0800ad84
	movs r4, #1
	str r4, [sp, #40]
.L_0800ad84:
	ldrb r3, [r6, #22]
	cmp r3, r0
	beq .L_0800ad92
	strb r0, [r6, #22]
	ldr r1, [sp, #0]
	movs r3, #1
	strb r3, [r1]
.L_0800ad92:
	mov r4, r11
	movs r2, #1
	ldrb r3, [r4]
	add r8, r2
	cmp r8, r3
	bge .L_0800ada0
	b .L_0800aa7c
.L_0800ada0:
	ldr r1, [sp, #0]
	ldrb r3, [r1]
	cmp r3, #0
	bne .L_0800adaa
	b .L_0800b054
.L_0800adaa:
	ldr r2, [sp, #44]
	ldr r3, [sp, #44]
	adds r2, #32
	str r2, [sp, #20]
	adds r3, #33
	ldrb r2, [r2]
	str r3, [sp, #16]
	ldrb r3, [r3]
	adds r4, r3, #0
	muls r4, r2
	adds r0, r4, #0
	str r4, [sp, #36]
	bl Runtime_BumpAllocate
	ldr r3, .L_0800afc8
	ldr r1, [sp, #36]
	mov r10, r0
	bl _call_via_r3
	mov r2, r11
	ldrb r3, [r2]
	movs r1, #1
	negs r1, r1
	subs r3, #1
	mov r9, r1
	mov r8, r3
	cmp r3, #0
	blt .L_0800ae6e
	ldr r4, [sp, #44]
	lsls r3, r3, #2
	adds r3, r3, r4
	adds r3, #40
	movs r1, #0
	mov r12, r3
	mov lr, r1
.L_0800adf0:
	mov r2, r12
	movs r3, #4
	ldr r6, [r2]
	negs r3, r3
	add r12, r3
	cmp r6, #0
	beq .L_0800ae62
	ldr r3, [r6, #8]
	cmp r3, #0
	beq .L_0800ae62
	ldrb r3, [r6, #22]
	cmp r3, #255
	beq .L_0800ae62
	ldrb r0, [r6, #6]
	cmp r0, #3
	bhi .L_0800ae62
	lsls r0, r0, #8
	mov r4, r8
	mov r6, r9
	orrs r0, r4
	cmp r6, #0
	blt .L_0800ae48
	add r5, sp, #48
	lsls r2, r6, #1
	ldrh r3, [r5, r2]
	cmp r3, r0
	bls .L_0800ae50
	mov r1, lr
	strh r3, [r5, r1]
	adds r3, r2, r5
	adds r7, r5, #0
	adds r4, r3, #2
	adds r1, r2, #0
.L_0800ae32:
	subs r6, #1
	subs r1, #2
	cmp r6, #0
	blt .L_0800ae54
	adds r3, r1, #0
	ldrh r2, [r3, r7]
	cmp r2, r0
	bls .L_0800ae56
	subs r4, #2
	strh r2, [r4]
	b .L_0800ae32
.L_0800ae48:
	mov r2, r9
	add r5, sp, #48
	lsls r3, r2, #1
	b .L_0800ae56
.L_0800ae50:
	adds r3, r2, #0
	b .L_0800ae56
.L_0800ae54:
	lsls r3, r6, #1
.L_0800ae56:
	adds r3, #2
	strh r0, [r5, r3]
	movs r4, #1
	movs r3, #2
	add lr, r3
	add r9, r4
.L_0800ae62:
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	cmp r2, #0
	bge .L_0800adf0
.L_0800ae6e:
	movs r3, #1
	movs r4, #0
	add r9, r3
	mov r8, r4
	cmp r8, r9
	bge .L_0800af12
.L_0800ae7a:
	mov r1, r8
	lsls r3, r1, #1
	add r2, sp, #56
	adds r3, r3, r2
	subs r3, #8
	ldrb r3, [r3]
	ldr r4, [sp, #44]
	lsls r3, r3, #2
	adds r3, #40
	ldr r6, [r4, r3]
	ldrb r3, [r6, #7]
	cmp r3, #1
	bne .L_0800aea4
	ldrb r3, [r6, #22]
	ldr r2, [r6, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	mov r1, r10
	bl Resource_DecodeType01
	b .L_0800af0a
.L_0800aea4:
	cmp r3, #3
	bne .L_0800aef8
	ldrb r3, [r6, #5]
	cmp r3, #0
	beq .L_0800aed8
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocate
	ldrb r3, [r6, #22]
	ldr r2, [r6, #8]
	adds r5, r0, #0
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	adds r1, r5, #0
	bl Resource_DecompressLz
	ldrb r2, [r6, #5]
	mov r1, r10
	ldr r3, [sp, #28]
	bl _call_via_r3
	adds r0, r5, #0
	bl Runtime_BumpFree
	b .L_0800af0a
.L_0800aed8:
	ldrb r3, [r6, #22]
	ldr r2, [r6, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	mov r1, r10
	ldr r3, .L_0800afcc
	bl _call_via_r3
	cmp r0, #0
	beq .L_0800af0a
	mov r1, r10
	movs r2, #0
	ldr r4, [sp, #28]
	bl _call_via_r4
	b .L_0800af0a
.L_0800aef8:
	ldrb r3, [r6, #22]
	ldr r2, [r6, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	mov r1, r10
	ldrb r2, [r6, #5]
	ldr r3, [sp, #28]
	bl _call_via_r3
.L_0800af0a:
	movs r4, #1
	add r8, r4
	cmp r8, r9
	blt .L_0800ae7a
.L_0800af12:
	ldr r3, [sp, #44]
	adds r3, #38
	ldrb r2, [r3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0800aff0
	ldr r0, [sp, #36]
	bl Runtime_BumpAllocate
	str r0, [sp, #12]
	ldr r3, [sp, #32]
	ldr r1, [sp, #20]
	ldrb r3, [r3, #6]
	ldr r2, [sp, #16]
	ldrb r7, [r1]
	ldrb r5, [r2]
	ldr r4, [sp, #32]
	str r3, [sp, #8]
	ldrb r4, [r4, #7]
	ldr r3, .L_0800afc8
	str r4, [sp, #4]
	ldr r1, [sp, #36]
	bl _call_via_r3
	mov r1, r10
	ldr r2, [sp, #12]
	adds r3, r1, r7
	adds r1, r3, #1
	adds r3, r2, r7
	adds r4, r3, #1
	subs r5, #1
	movs r3, #1
	mov r8, r3
	mov r9, r5
	cmp r8, r9
	bcs .L_0800afaa
	subs r2, r7, #1
	mov lr, r2
.L_0800af60:
	movs r6, #1
	cmp r6, lr
	bcs .L_0800af9e
	movs r3, #1
	mov r11, r3
	mov r12, lr
	adds r5, r1, r7
	subs r0, r1, r7
	subs r2, r1, #1
.L_0800af72:
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_0800af8e
	ldrb r3, [r2, #2]
	cmp r3, #0
	beq .L_0800af8e
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_0800af8e
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0800af8e
	mov r3, r11
	strb r3, [r4]
.L_0800af8e:
	adds r6, #1
	adds r4, #1
	adds r5, #1
	adds r0, #1
	adds r2, #1
	adds r1, #1
	cmp r6, r12
	bcc .L_0800af72
.L_0800af9e:
	movs r2, #1
	add r8, r2
	adds r4, #2
	adds r1, #2
	cmp r8, r9
	bcc .L_0800af60
.L_0800afaa:
	movs r3, #0
	ldr r2, [sp, #36]
	mov r8, r3
	mov r1, r10
	ldr r4, [sp, #12]
	cmp r8, r2
	bcs .L_0800afea
.L_0800afb8:
	ldrb r3, [r4]
	cmp r3, #0
	beq .L_0800afd0
	add r3, sp, #4
	ldrb r3, [r3]
	strb r3, [r1]
	b .L_0800afdc
	.2byte 0x0000
.L_0800afc8:
	.4byte IwramClearWords
.L_0800afcc:
	.4byte IwramDecompress
.L_0800afd0:
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_0800afdc
	add r2, sp, #8
	ldrb r2, [r2]
	strb r2, [r1]
.L_0800afdc:
	movs r3, #1
	ldr r2, [sp, #36]
	add r8, r3
	adds r4, #1
	adds r1, #1
	cmp r8, r2
	bcc .L_0800afb8
.L_0800afea:
	ldr r0, [sp, #12]
	bl Runtime_BumpFree
.L_0800aff0:
	ldr r3, [sp, #44]
	ldr r1, [sp, #36]
	ldrb r0, [r3, #28]
	movs r2, #0
	bl VramBlock_LoadCached
	ldr r4, .L_0800b048
	adds r5, r0, #0
	lsls r3, r5, #5
	ldr r0, .L_0800b04c
	adds r3, r3, r4
	ldr r2, [sp, #20]
	ldr r4, [sp, #16]
	adds r0, #212
	ldrb r1, [r2]
	ldrb r2, [r4]
	ldr r4, [r0]
	mov r0, r10
	bl _call_via_r4
	ldr r3, .L_0800b044
	ldr r1, [sp, #44]
	ands r5, r3
	ldrh r2, [r1, #8]
	ldr r3, .L_0800b050
	ands r3, r2
	orrs r3, r5
	adds r2, r1, #0
	strh r3, [r2, #8]
	ldr r4, [sp, #0]
	movs r3, #0
	strb r3, [r4]
	ldr r1, [sp, #32]
	ldr r2, [sp, #36]
	ldrh r3, [r1]
	adds r4, r1, #0
	adds r3, r3, r2
	strh r3, [r4]
	mov r0, r10
	bl Runtime_BumpFree
	b .L_0800b054
.L_0800b044:
	.4byte 0x000003ff
.L_0800b048:
	.4byte 0x06010000
.L_0800b04c:
	.4byte Data_03001e50
.L_0800b050:
	.4byte 0xfffffc00
.L_0800b054:
	ldr r1, [sp, #24]
	cmp r1, #0
	bne .L_0800b060
	movs r0, #52
	bl Runtime_ReleaseHeapBlock
.L_0800b060:
	ldr r0, [sp, #40]
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
