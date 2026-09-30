.syntax unified
	.thumb
	.global Func_0802be4c
	.thumb_func
Func_0802be4c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #4
	adds r6, r1, #0
	bl Runtime_BumpAllocate
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r7, r0, #0
	mov r8, r3
	cmp r5, #0
	bge .L_0802be72
	ldr r0, .L_0802c030
	adds r5, r5, r0
.L_0802be72:
	asrs r3, r5, #21
	movs r2, #31
	adds r1, r6, #0
	ands r3, r2
	cmp r1, #0
	bge .L_0802be82
	ldr r0, .L_0802c030
	adds r1, r1, r0
.L_0802be82:
	asrs r5, r1, #21
	ands r5, r2
	lsls r5, r5, #5
	ldr r2, .L_0802c034
	adds r5, r3, r5
	lsls r5, r5, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #0
	lsls r0, r0, #2
	lsrs r0, r0, #26
	bl Func_0802d088
	ldr r3, [r5]
	movs r1, #10
	lsls r3, r3, #2
	lsrs r0, r3, #26
	bl __divsi3
	movs r3, #3
	cmp r0, #3
	beq .L_0802bec2
	cmp r0, #3
	bgt .L_0802beba
	movs r3, #2
	cmp r0, #2
	beq .L_0802bec2
	b .L_0802bec0
.L_0802beba:
	movs r3, #1
	cmp r0, #4
	beq .L_0802bec2
.L_0802bec0:
	movs r3, #0
.L_0802bec2:
	lsls r6, r3, #3
	adds r6, r6, r3
	ldr r3, .L_0802c038
	lsls r6, r6, #2
	adds r6, r6, r3
	movs r3, #144
	lsls r3, r3, #1
	movs r5, #160
	add r3, r8
	lsls r5, r5, #19
	str r6, [r3]
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldr r0, [r6]
	mov r8, r3
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_0801591c
	mov r2, r8
	strh r2, [r7]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_0802c03c
	adds r0, r7, #0
	bl Func_080c8970
	ldr r0, [r6, #4]
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0801587c
	adds r0, r5, #0
	bl Func_0802cc9c
	ldr r5, .L_0802c040
	ldr r0, [r6, #8]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c044
	bl Func_0801587c
	ldr r0, [r6, #12]
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_0802c048
	ldr r2, .L_0802c04c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_0802c050
	ldr r0, [r6, #16]
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_0802c054
	ldr r2, .L_0802c04c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_0802c058
	ldr r0, [r6, #20]
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_0802c05c
	ldr r2, .L_0802c04c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_0802c060
	ldr r0, [r6, #24]
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_0802c064
	ldr r2, .L_0802c04c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [r6, #28]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c068
	bl Func_0801587c
	ldr r0, [r6, #32]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c06c
	bl Func_0801587c
	ldr r3, .L_0802c070
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_0802c074
	ldr r2, .L_0802c078
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r4, #128
	ldr r0, .L_0802c07c
	ldr r2, .L_0802c080
	lsls r4, r4, #10
	movs r1, #0
	adds r4, #2
.L_0802bfd8:
	movs r3, #15
.L_0802bfda:
	subs r3, #1
	stmia r0!, {r2}
	adds r2, r2, r4
	cmp r3, #0
	bge .L_0802bfda
	adds r1, #1
	cmp r1, #19
	ble .L_0802bfd8
	movs r4, #128
	ldr r2, .L_0802c084
	lsls r4, r4, #10
	movs r1, #0
	adds r4, #2
.L_0802bff4:
	movs r3, #15
.L_0802bff6:
	subs r3, #1
	stmia r0!, {r2}
	adds r2, r2, r4
	cmp r3, #0
	bge .L_0802bff6
	adds r1, #1
	cmp r1, #23
	ble .L_0802bff4
	movs r4, #128
	ldr r2, .L_0802c084
	lsls r4, r4, #10
	movs r1, #0
	adds r4, #2
.L_0802c010:
	movs r3, #15
.L_0802c012:
	subs r3, #1
	stmia r0!, {r2}
	adds r2, r2, r4
	cmp r3, #0
	bge .L_0802c012
	adds r1, #1
	cmp r1, #3
	ble .L_0802c010
	adds r0, r7, #0
	bl Sys_Free
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0802c030:
	.4byte 0x001fffff
.L_0802c034:
	.4byte gMapBlocks
.L_0802c038:
	.4byte Data_0802ed34
.L_0802c03c:
	.4byte Data_0202d000
.L_0802c040:
	.4byte Data_02038000
.L_0802c044:
	.4byte Data_0202c000
.L_0802c048:
	.4byte 0x06008000
.L_0802c04c:
	.4byte 0x84000800
.L_0802c050:
	.4byte Data_0203a000
.L_0802c054:
	.4byte 0x0600a000
.L_0802c058:
	.4byte Data_0203c000
.L_0802c05c:
	.4byte 0x0600c000
.L_0802c060:
	.4byte Data_0203e000
.L_0802c064:
	.4byte 0x0600e000
.L_0802c068:
	.4byte Data_02028000
.L_0802c06c:
	.4byte Data_0202a000
.L_0802c070:
	.4byte 0xf07ff07f
.L_0802c074:
	.4byte 0x06002800
.L_0802c078:
	.4byte 0x85000180
.L_0802c07c:
	.4byte 0x06003000
.L_0802c080:
	.4byte 0x01810180
.L_0802c084:
	.4byte 0x01010100
