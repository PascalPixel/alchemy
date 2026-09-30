.syntax unified
	.thumb
	.global Func_08018cac
Func_08018cac:
	.global UiText_DrawGlyph
	.thumb_func
UiText_DrawGlyph:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #144
	str r2, [sp, #12]
	str r3, [sp, #8]
	adds r7, r1, #0
	ldr r1, .L_08018e24
	ldr r2, .L_08018e28
	ldr r6, [r1]
	adds r3, r6, r2
	ldrh r3, [r3]
	ldr r2, .L_08018e2c
	str r3, [sp, #4]
	adds r3, r6, r2
	ldrh r3, [r3]
	mov r11, r3
	ldr r3, [sp, #176]
	mov r8, r0
	cmp r3, #1
	beq .L_08018d50
	ldrh r2, [r0, #22]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_08018d50
	ldr r3, [r1, #88]
	ldr r3, [r3]
	cmp r3, r8
	bne .L_08018d06
	ldr r0, .L_08018e30
	bl Resource_GetTableEntry
	ldr r0, .L_08018e34
	bl Resource_GetTableEntry
	movs r1, #3
	mov r9, r1
	cmp r7, #32
	bne .L_08018d06
	b .L_08018ecc
.L_08018d06:
	ldr r0, .L_08018e34
	bl Resource_GetTableEntry
	movs r2, #4
	mov r10, r0
	mov r9, r2
	cmp r7, #32
	bne .L_08018d18
	b .L_08018ecc
.L_08018d18:
	ldr r5, .L_08018e38
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	ldr r3, .L_08018e3c
	ldr r0, .L_08018e40
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r10
	str r3, [sp, #0]
	adds r1, r7, #0
	ldr r2, [sp, #12]
	ldr r3, [sp, #8]
	mov r0, r8
	bl _call_via_r6
	adds r5, r0, #0
	adds r0, r6, #0
	bl Runtime_BumpFree
	adds r0, r5, #0
	b .L_08018ee8
.L_08018d50:
	movs r1, #5
	mov r9, r1
	cmp r7, #32
	bne .L_08018d5a
	b .L_08018ecc
.L_08018d5a:
	bl Tile_ExpandOpaqueEnd
	adds r5, r0, #0
	movs r0, #0
	cmp r5, #0
	bne .L_08018d68
	b .L_08018ee8
.L_08018d68:
	ldr r2, .L_08018e44
	subs r3, r5, r6
	adds r3, r3, r2
	ldr r2, .L_08018e48
	adds r1, r3, #0
	muls r1, r2
	movs r3, #1
	movs r2, #0
	strb r3, [r5, #5]
	strb r2, [r5, #4]
	ldr r3, [sp, #176]
	mov r10, r1
	cmp r3, #1
	bne .L_08018d8e
	movs r1, #1
	movs r3, #2
	mov r9, r1
	strb r3, [r5, #5]
	b .L_08018dd6
.L_08018d8e:
	ldr r1, .L_08018e4c
	adds r3, r6, r1
	ldrh r3, [r3]
	cmp r3, #3
	beq .L_08018dac
	cmp r3, #3
	bgt .L_08018da2
	cmp r3, #2
	beq .L_08018dc0
	b .L_08018dc6
.L_08018da2:
	cmp r3, #4
	beq .L_08018db2
	cmp r3, #5
	beq .L_08018dbc
	b .L_08018dc6
.L_08018dac:
	movs r3, #5
	strb r3, [r5, #5]
	b .L_08018dc6
.L_08018db2:
	movs r3, #6
	strb r3, [r5, #5]
	movs r3, #8
	strh r3, [r5, #12]
	b .L_08018dc6
.L_08018dbc:
	movs r3, #7
	b .L_08018dc2
.L_08018dc0:
	movs r3, #4
.L_08018dc2:
	strb r3, [r5, #5]
	strh r2, [r5, #12]
.L_08018dc6:
	add r1, sp, #16
	adds r0, r7, #0
	bl UiText_RenderGlyphPair
	cmp r0, #0
	bne .L_08018dd4
	movs r0, #1
.L_08018dd4:
	mov r9, r0
.L_08018dd6:
	ldrb r3, [r5, #5]
	cmp r3, #2
	bne .L_08018e5c
	ldr r2, .L_08018e50
	adds r6, r6, r2
	ldrh r3, [r6]
	adds r7, r5, #0
	adds r7, #16
	cmp r3, #99
	bne .L_08018df0
	bl Resource_FindFreeEntry
	strh r0, [r6]
.L_08018df0:
	mov r3, r8
	ldrh r2, [r3, #12]
	ldr r1, .L_08018e54
	ldrh r3, [r3, #8]
	adds r3, r3, r1
	adds r2, r2, r3
	lsls r2, r2, #3
	ldr r3, .L_08018e20
	adds r2, #4
	ands r2, r3
	ldrh r1, [r7, #6]
	ldr r3, .L_08018e58
	ands r3, r1
	orrs r3, r2
	mov r2, r8
	strh r3, [r7, #6]
	ldrb r3, [r2, #14]
	ldrb r2, [r2, #10]
	adds r2, #254
	adds r3, r3, r2
	lsls r3, r3, #3
	subs r3, #1
	strb r3, [r7, #4]
	b .L_08018eac
.L_08018e20:
	.4byte 0x000001ff
.L_08018e24:
	.4byte Data_03001e8c
.L_08018e28:
	.4byte 0x000012b0
.L_08018e2c:
	.4byte 0x00000ea8
.L_08018e30:
	.4byte 0x00000014
.L_08018e34:
	.4byte 0x00000013
.L_08018e38:
	.4byte 0x00000318
.L_08018e3c:
	.4byte 0x040000d4
.L_08018e40:
	.4byte Func_080155d0
.L_08018e44:
	.4byte 0xfffff968
.L_08018e48:
	.4byte 0xb6db6db7
.L_08018e4c:
	.4byte 0x00000eac
.L_08018e50:
	.4byte 0x000012b6
.L_08018e54:
	.4byte 0x0000fffe
.L_08018e58:
	.4byte 0xfffffe00
.L_08018e5c:
	ldr r3, .L_08018ed4
	adds r4, r6, r3
	ldrh r1, [r4]
	ldr r2, .L_08018ed8
	add r1, r10
	lsls r1, r1, #5
	adds r1, r1, r2
	ldr r3, .L_08018edc
	add r0, sp, #16
	ldr r2, .L_08018ee0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [sp, #8]
	mov r1, r11
	lsrs r3, r1, #1
	mov r1, r8
	adds r3, r2, r3
	ldrh r2, [r1, #14]
	lsls r2, r2, #3
	adds r3, r3, r2
	ldr r2, .L_08018ee4
	adds r3, r3, r2
	strh r3, [r5, #20]
	ldr r3, [sp, #4]
	ldr r1, [sp, #12]
	lsrs r2, r3, #1
	adds r2, r1, r2
	mov r1, r8
	ldrh r3, [r1, #12]
	lsls r3, r3, #3
	adds r2, r2, r3
	ldr r3, .L_08018ed0
	adds r2, #2
	orrs r2, r3
	strh r2, [r5, #22]
	ldrh r3, [r4]
	add r3, r10
	adds r7, r5, #0
	strh r3, [r5, #24]
	adds r7, #16
.L_08018eac:
	movs r3, #254
	strb r3, [r5, #15]
	ldrh r3, [r7, #6]
	lsls r3, r3, #23
	lsrs r3, r3, #23
	strh r3, [r5, #6]
	ldrb r3, [r7, #4]
	movs r2, #0
	strh r3, [r5, #8]
	mov r3, r10
	strb r3, [r5, #14]
	str r2, [r5]
	mov r0, r8
	adds r1, r5, #0
	bl RenderOutput_AppendToList
.L_08018ecc:
	mov r0, r9
	b .L_08018ee8
.L_08018ed0:
	.4byte 0x00004000
.L_08018ed4:
	.4byte 0x000012b8
.L_08018ed8:
	.4byte 0x06010000
.L_08018edc:
	.4byte 0x040000d4
.L_08018ee0:
	.4byte 0x84000020
.L_08018ee4:
	.4byte 0x0000fffe
.L_08018ee8:
	add sp, #144
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
