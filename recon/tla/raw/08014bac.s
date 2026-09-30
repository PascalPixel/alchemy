.syntax unified
	.thumb
	.global Func_08014bac
	.thumb_func
Func_08014bac:
	push {lr}
	ldr r0, .L_08014c34
	bl Resource_GetTableEntry
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	lsls r1, r1, #19
	ldr r2, .L_08014c38
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_08014c3c
	ldr r1, .L_08014c40
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #0
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	movs r2, #131
	ldr r3, .L_08014c44
	lsls r2, r2, #7
	strh r2, [r3]
	movs r2, #228
	lsls r2, r2, #6
	adds r2, #96
	adds r3, #2
	strh r2, [r3]
	movs r2, #197
	lsls r2, r2, #6
	adds r3, #2
	strh r2, [r3]
	movs r2, #164
	lsls r2, r2, #6
	adds r2, #32
	adds r3, #2
	strh r2, [r3]
	movs r2, #146
	lsls r2, r2, #7
	adds r2, #160
	adds r3, #2
	strh r2, [r3]
	movs r2, #162
	lsls r2, r2, #7
	adds r2, #192
	adds r3, #2
	strh r2, [r3]
	movs r2, #178
	lsls r2, r2, #7
	adds r2, #224
	adds r3, #2
	strh r2, [r3]
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08014c48
	adds r1, #32
	adds r2, #224
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {pc}
	.2byte 0x0000
.L_08014c34:
	.4byte 0x00000013
.L_08014c38:
	.4byte 0x84000800
.L_08014c3c:
	.4byte Data_08017af0
.L_08014c40:
	.4byte 0x050001e0
.L_08014c44:
	.4byte 0x050001e8
.L_08014c48:
	.4byte Data_08017b10
