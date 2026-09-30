.syntax unified
	.thumb
	.global Ui_PrepareTransferFromTableEntry
	.thumb_func
Ui_PrepareTransferFromTableEntry:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #68]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	adds r2, r1, r3
	ldr r3, .L_0803daac
	lsls r0, r0, #2
	ldr r3, [r3, r0]
	movs r0, #192
	lsls r0, r0, #3
	str r3, [r2]
	adds r3, r1, r0
	movs r2, #2
	adds r0, #2
	strh r2, [r3]
	adds r3, r1, r0
	strh r2, [r3]
	adds r0, r1, #0
	movs r1, #0
	bl UiGlyph_DecodeWithHeapRoutines
	pop {pc}
	.2byte 0x0000
.L_0803daac:
	.4byte Data_08058ff4
