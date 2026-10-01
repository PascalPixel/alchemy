.syntax unified
	.thumb
	.global Func_080dc1b0
	.thumb_func
Func_080dc1b0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	adds r3, #128
	ldr r4, [r3]
	movs r2, #154
	lsls r2, r2, #5
	adds r7, r0, #0
	adds r0, r4, r2
	movs r2, #224
	movs r3, #128
	lsls r2, r2, #3
	adds r2, #98
	lsls r3, r3, #19
	adds r6, r1, #0
	adds r3, #212
	adds r1, r5, r2
	ldr r2, .L_080dc23c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080dc206
	movs r2, #224
	lsls r2, r2, #4
	adds r0, r4, r2
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #2
	adds r2, #34
	lsls r3, r3, #19
	adds r1, r5, r2
	adds r3, #212
	ldr r2, .L_080dc23c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_080dc206:
	movs r2, #224
	lsls r2, r2, #4
	adds r0, r4, r2
	movs r3, #128
	movs r2, #224
	lsls r2, r2, #2
	lsls r3, r3, #19
	adds r1, r4, r2
	adds r3, #212
	ldr r2, .L_080dc240
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #14
	orrs r0, r6
	movs r1, #1
	bl Func_080d170c
	movs r0, #132
	lsls r0, r0, #1
	bl GameFlag_SetBit
	adds r0, r7, #0
	bl BattleFx_StartBufferInterpolation
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080dc23c:
	.4byte 0x84000150
.L_080dc240:
	.4byte 0x840002a0
