.syntax unified
	.thumb
	.global Func_08042e28
	.thumb_func
Func_08042e28:
	push {r5, r6, r7, lr}
	sub sp, #4
	movs r3, #0
	str r3, [sp, #0]
	movs r2, #30
	movs r3, #20
	movs r1, #0
	adds r5, r0, #0
	movs r0, #0
	bl UiWindow_Create
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	adds r1, r7, #0
	ldr r0, .L_08042ed4
	bl Resource_DecodeType01
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_08042ed8
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	cmp r5, #1
	beq .L_08042e7c
	cmp r5, #1
	bcc .L_08042e74
	cmp r5, #2
	beq .L_08042e8a
	b .L_08042e96
.L_08042e74:
	ldr r0, .L_08042edc
	adds r1, r6, #0
	movs r2, #76
	b .L_08042e82
.L_08042e7c:
	ldr r0, .L_08042ee0
	adds r1, r6, #0
	movs r2, #92
.L_08042e82:
	movs r3, #40
	bl UiText_DrawResource
	b .L_08042e96
.L_08042e8a:
	ldr r0, .L_08042ee4
	adds r1, r6, #0
	movs r2, #84
	movs r3, #40
	bl UiText_DrawResource
.L_08042e96:
	ldr r5, .L_08042ee8
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #40
	movs r3, #88
	adds r5, #1
	bl UiText_DrawResource
	movs r2, #40
	movs r3, #104
	adds r0, r5, #0
	adds r1, r6, #0
	bl UiText_DrawResource
	movs r1, #30
	movs r0, #0
	bl Func_081c0040
	movs r0, #149
	lsls r0, r0, #1
	bl Audio_PlayCue
	movs r0, #10
	bl WaitFrames
	adds r0, r7, #0
	bl Sys_Free
	adds r0, r6, #0
	add sp, #4
	pop {r5, r6, r7, pc}
.L_08042ed4:
	.4byte Data_0805f585
.L_08042ed8:
	.4byte 0x06001000
.L_08042edc:
	.4byte 0x0000001d
.L_08042ee0:
	.4byte 0x0000001e
.L_08042ee4:
	.4byte 0x0000001f
.L_08042ee8:
	.4byte 0x00000020
