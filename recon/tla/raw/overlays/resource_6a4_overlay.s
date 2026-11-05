.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.balign 4
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	movs r0, #9
	bl Actor_GetById
	mov r10, r0
	movs r0, #10
	bl Actor_GetById
	mov r9, r0
	movs r0, #23
	bl Actor_GetById
	adds r6, r0, #0
	ldr r2, [r6, #12]
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r5, [r6, #80]
	bl Actor_SetPositionAndResetMotion
	mov r2, r8
	cmp r2, #0
	beq .L_0200809a
	cmp r2, #2
	beq .L_0200809a
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002b68
	movs r7, #0
	b .L_02008118
.L_0200809a:
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002b68
	movs r7, #0
.L_020080a4:
	mov r2, r10
	ldr r3, [r2, #12]
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2, #12]
	movs r2, #224
	ldr r3, [r6, #12]
	lsls r2, r2, #3
	adds r2, #174
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_020081b0
	ldr r3, [r6, #8]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #8]
	adds r7, #1
	ldrh r3, [r5, #18]
	adds r3, #32
	strh r3, [r5, #18]
	bl wait
	cmp r7, #63
	ble .L_020080a4
	b .L_0200811c
.L_020080dc:
	mov r2, r9
	ldr r3, [r2, #12]
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	mov r2, r9
	str r3, [r2, #12]
	movs r2, #224
	ldr r3, [r6, #12]
	lsls r2, r2, #3
	adds r2, #174
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r2, #160
	ldr r3, [r6, #8]
	lsls r2, r2, #4
	adds r2, #61
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r2, #255
	ldrh r3, [r5, #18]
	lsls r2, r2, #8
	adds r2, #224
	adds r3, r3, r2
	strh r3, [r5, #18]
	movs r0, #1
	bl wait
	adds r7, #1
.L_02008118:
	cmp r7, #63
	ble .L_020080dc
.L_0200811c:
	mov r3, r8
	subs r3, #2
	cmp r3, #1
	bhi .L_020081a4
	movs r0, #30
	bl FieldEvent_Wait
	mov r0, r10
	bl Func_0200105c
	mov r0, r9
	bl Func_0200105c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl Func_02002b68
	movs r7, #255
.L_02008142:
	movs r0, #9
	bl Actor_GetById
	movs r5, #200
	ldr r3, [r0, #12]
	lsls r5, r5, #5
	adds r5, #153
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #10
	bl Actor_GetById
	ldr r3, [r0, #12]
	subs r7, #1
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #1
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	bl wait
	cmp r7, #0
	bge .L_02008142
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02002b68
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #80
	bl Func_02002b68
	movs r0, #10
	bl FieldEvent_Wait
	bl Sound_WaitForJingle
	mov r3, r10
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
	mov r3, r9
	adds r3, #85
	strb r2, [r3]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
.L_020081a4:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020081b0:
	.4byte 0xfffff5c3
	.section .text.x020097a8,"ax",%progbits
	.global Func_020017a8
	.thumb_func
Func_020017a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r1, #176
	lsls r1, r1, #4
	sub sp, #12
	adds r5, r0, #0
	adds r1, #20
	movs r0, #220
	str r2, [sp, #8]
	bl HeapSlot_AllocFar
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r7, r0, #0
	movs r0, #230
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	adds r0, r5, #0
	mov r11, r3
	bl Actor_GetById
	movs r1, #176
	lsls r1, r1, #4
	adds r1, #12
	adds r3, r7, r1
	str r0, [r3]
	adds r0, r6, #0
	bl Actor_GetById
	movs r2, #177
	lsls r2, r2, #4
	adds r3, r7, r2
	str r0, [r3]
	ldr r0, .L_02009898
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020029a0
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #1
	adds r2, r7, #0
	str r0, [sp, #4]
	bl VramCache_Load
	mov r9, r0
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #6
	adds r3, r7, r0
	mov r1, r9
	movs r2, #224
	movs r0, #128
	strh r1, [r3]
	lsls r2, r2, #3
	movs r3, #0
	lsls r0, r0, #2
	movs r1, #31
	adds r6, r7, r2
	mov r10, r3
	adds r5, r7, r0
	mov r8, r1
.L_02009838:
	mov r2, r9
	adds r0, r5, #0
	str r2, [sp, #0]
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_02002b30
	ldrb r3, [r5, #5]
	ldrb r2, [r5, #9]
	movs r0, #32
	orrs r3, r0
	strb r3, [r5, #5]
	movs r1, #13
	movs r3, #15
	ands r3, r2
	negs r1, r1
	ldr r2, .L_02009894
	ands r3, r1
	adds r1, #12
	add r8, r1
	strb r3, [r5, #9]
	strh r2, [r5, #30]
	mov r3, r10
	subs r0, #34
	mov r2, r8
	str r3, [r6, #24]
	add r10, r0
	adds r5, #40
	adds r6, #28
	cmp r2, #0
	bge .L_02009838
	ldr r0, .L_0200989c
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020029a0
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #2
	adds r1, r5, #0
	adds r2, r7, #0
	mov r10, r0
	b .L_020098a0
.L_02009894:
	.4byte 0x000000f0
.L_02009898:
	.4byte 0x000001eb
.L_0200989c:
	.4byte 0x000001f6
.L_020098a0:
	bl VramCache_Load
	movs r1, #176
	lsls r1, r1, #4
	movs r2, #168
	adds r1, #4
	lsls r2, r2, #4
	adds r5, r7, r2
	adds r3, r7, r1
	strh r0, [r3]
	movs r1, #30
	str r0, [sp, #0]
	movs r2, #7
	adds r0, r5, #0
	ldr r3, .L_020098f4
	bl Func_02002b30
	ldr r3, .L_020098f0
	movs r2, #13
	strh r3, [r5, #30]
	ldrb r3, [r5, #9]
	negs r2, r2
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldrb r3, [r5, #5]
	movs r0, #32
	movs r1, #15
	movs r6, #0
	ands r2, r1
	orrs r3, r0
	strb r3, [r5, #5]
	strb r2, [r5, #9]
	str r6, [r5, #20]
	str r6, [r5, #24]
	ldr r2, [sp, #8]
	cmp r2, #0
	bne .L_0200992e
	b .L_020098f8
	.2byte 0x0000
.L_020098f0:
	.4byte 0x000000f0
.L_020098f4:
	.4byte 0x80004000
.L_020098f8:
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #196
	movs r0, #160
	adds r2, r7, r3
	lsls r0, r0, #4
	movs r3, #152
	lsls r3, r3, #16
	adds r0, #200
	movs r1, #160
	str r3, [r2]
	lsls r1, r1, #4
	adds r3, r7, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r1, #204
	str r0, [r3]
	adds r3, r7, r1
	movs r1, #164
	lsls r1, r1, #16
	str r1, [r3]
	movs r3, #173
	lsls r3, r3, #4
	adds r2, r7, r3
	movs r3, #235
	lsls r3, r3, #17
	b .L_0200995c
.L_0200992e:
	movs r1, #160
	lsls r1, r1, #4
	adds r1, #196
	movs r3, #218
	adds r2, r7, r1
	lsls r3, r3, #18
	str r3, [r2]
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #200
	movs r0, #128
	adds r3, r7, r2
	lsls r0, r0, #14
	adds r1, #8
	str r0, [r3]
	adds r3, r7, r1
	movs r1, #164
	lsls r1, r1, #16
	str r1, [r3]
	movs r3, #173
	lsls r3, r3, #4
	adds r2, r7, r3
	ldr r3, .L_02009a2c
.L_0200995c:
	str r3, [r2]
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #212
	adds r3, r7, r2
	str r0, [r3]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #216
	adds r3, r7, r0
	str r1, [r3]
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #196
	adds r3, r7, r2
	movs r1, #160
	ldr r3, [r3]
	lsls r1, r1, #4
	adds r1, #220
	adds r1, r1, r7
	movs r0, #160
	str r3, [r1]
	lsls r0, r0, #4
	movs r3, #174
	lsls r3, r3, #4
	adds r0, #200
	adds r6, r7, r3
	adds r3, r7, r0
	ldr r3, [r3]
	adds r2, #8
	str r3, [r6]
	mov r8, r1
	adds r3, r7, r2
	movs r1, #160
	ldr r3, [r3]
	lsls r1, r1, #4
	adds r1, #228
	adds r5, r7, r1
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #8
	mov r0, r11
	str r3, [r0, #48]
	str r3, [r0, #52]
	bl Func_02002a20
	mov r2, r8
	ldr r1, [r2]
	ldr r3, [r5]
	ldr r2, [r6]
	mov r0, r11
	bl Func_02002a28
	movs r0, #142
	bl Func_02002b68
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #12
	adds r3, r7, r0
	ldr r0, [r3]
	bl Func_0200105c
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #176
	add r0, sp, #8
	ldrb r0, [r0]
	lsls r1, r1, #4
	adds r1, #11
	adds r3, r7, r1
	strb r0, [r3]
	movs r0, #176
	lsls r0, r0, #4
	subs r1, #11
	adds r0, #2
	movs r2, #0
	adds r5, r7, r1
	adds r3, r7, r0
	strh r2, [r5]
	strh r2, [r3]
	movs r3, #176
	lsls r3, r3, #4
	ldr r1, .L_02009a28
	adds r3, #9
	adds r2, r7, r3
	adds r0, #8
	movs r3, #1
	strb r3, [r2]
	adds r3, r7, r0
	strb r1, [r3]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009a30
	bl Task_Add
	movs r2, #186
	movs r1, #0
	ldrsh r3, [r5, r1]
	b .L_02009a46
	.2byte 0x0000
.L_02009a28:
	.4byte 0x00000000
.L_02009a2c:
	.4byte 0x02260000
.L_02009a30:
	.4byte Func_02001080
.L_02009a34:
	movs r0, #1
	bl wait
	movs r0, #176
	lsls r0, r0, #4
	adds r3, r7, r0
	movs r2, #186
	movs r1, #0
	ldrsh r3, [r3, r1]
.L_02009a46:
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	bne .L_02009a34
	ldr r0, .L_02009a74
	bl Task_RemoveFar
	mov r0, r10
	bl Resource_ResetEntry
	ldr r0, [sp, #4]
	bl Resource_ResetEntry
	movs r0, #220
	bl HeapSlot_Release
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009a74:
	.4byte Func_02001080
	.section .text.x02009a78,"ax",%progbits
	.global Func_02001a78
	.thumb_func
Func_02001a78:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02009c0c
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02009c10
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02009c14
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02009c18
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009c1c
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02009aca
	b .L_02009bfe
.L_02009aca:
	ldr r2, .L_02009c20
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02009ad8
	b .L_02009bee
.L_02009ad8:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02009ae0
	b .L_02009bee
.L_02009ae0:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_02009c24
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02009b56
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_02009bee
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02009bee
	cmp r4, #239
	bgt .L_02009bee
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009c28
	b .L_02009b92
.L_02009b56:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_02009bee
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02009bee
	cmp r4, #175
	bgt .L_02009bee
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009c2c
.L_02009b92:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009c30
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02009bd0
	adds r0, r5, #0
	bl Func_02002b48
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_02009be4
.L_02009bd0:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_02009be4:
	adds r0, r6, #0
	mov r1, r11
	bl Func_020029c0
	adds r6, #12
.L_02009bee:
	ldr r3, .L_02009c1c
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02009bfe
	b .L_02009aca
.L_02009bfe:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009c0c:
	.4byte 0xffff0000
.L_02009c10:
	.4byte gData_02003380
.L_02009c14:
	.4byte gResourceTableEntries
.L_02009c18:
	.4byte Data_020033c4
.L_02009c1c:
	.4byte gData_02003382
.L_02009c20:
	.4byte gData_02003384
.L_02009c24:
	.4byte gData_02003484
.L_02009c28:
	.4byte 0x40002000
.L_02009c2c:
	.4byte 0xc000a000
.L_02009c30:
	.4byte gData_02003486
	.section .bss,"aw",%nobits
	.global gData_0200333c
gData_0200333c:
	.space 0x00000004
	.global gData_02003340
gData_02003340:
	.space 0x00000020
	.global gData_02003360
gData_02003360:
	.space 0x00000004
	.global gData_02003364
gData_02003364:
	.space 0x0000001c
	.global gData_02003380
gData_02003380:
	.space 0x00000002
	.global gData_02003382
gData_02003382:
	.space 0x00000002
	.global gData_02003384
gData_02003384:
	.space 0x00000040
	.global Data_020033c4
Data_020033c4:
	.space 0x000000c0
	.global gData_02003484
gData_02003484:
	.space 0x00000002
	.global gData_02003486
gData_02003486:
	.space 0x00000002
	.global gData_02003488
gData_02003488:
