.syntax unified
	.thumb
	.global UiGlyph_DecodeWithHeapRoutines
	.thumb_func
UiGlyph_DecodeWithHeapRoutines:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	mov r10, r1
	ldr r5, .L_0803dc04
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	adds r7, r0, #0
	movs r0, #132
	lsls r0, r0, #24
	mov r8, r0
	movs r3, #128
	lsrs r5, r5, #2
	mov r2, r8
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0803dc08
	adds r1, r7, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #4
	adds r3, r6, r2
	ldr r0, [r3]
	adds r1, r6, #0
	mov lr, r7
	.2byte 0xf800
	adds r0, r7, #0
	bl Sys_Free
	mov r3, r10
	cmp r3, #0
	beq .L_0803dbb8
	ldr r5, .L_0803dc0c
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r3, #128
	adds r7, r0, #0
	lsrs r5, r5, #2
	mov r2, r8
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0803dc10
	b .L_0803dbce
.L_0803dbb8:
	ldr r5, .L_0803dc14
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r3, #128
	adds r7, r0, #0
	lsrs r5, r5, #2
	mov r2, r8
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0803dc18
.L_0803dbce:
	adds r1, r7, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #3
	adds r1, r6, r0
	movs r2, #192
	movs r0, #192
	lsls r2, r2, #3
	lsls r0, r0, #3
	adds r3, r6, r2
	adds r0, #2
	ldrh r2, [r3]
	adds r3, r6, r0
	ldrh r3, [r3]
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	adds r0, r7, #0
	bl Sys_Free
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803dc04:
	.4byte 0x0000027c
.L_0803dc08:
	.4byte Tile_Decompress4bppCode
.L_0803dc0c:
	.4byte 0x000000a0
.L_0803dc10:
	.4byte Tile_ExpandMaskedCode
.L_0803dc14:
	.4byte 0x00000080
.L_0803dc18:
	.4byte Tile_ExpandOpaqueCode
