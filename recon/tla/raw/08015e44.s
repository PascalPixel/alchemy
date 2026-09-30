.syntax unified
	.thumb
	.global Func_08015e44
	.thumb_func
Func_08015e44:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	adds r6, r1, #0
	ldr r5, [r3]
	bl Func_08015f0c
	cmp r0, #14
	bls .L_08015e5c
	movs r0, #1
	b .L_08015e86
.L_08015e5c:
	bl Func_08015c08
	movs r3, #128
	adds r0, r5, #0
	lsls r3, r3, #19
	adds r3, #212
	adds r0, #76
	adds r1, r6, #0
	ldr r2, .L_08015e88
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #19
	movs r2, #128
	adds r1, #212
	lsls r2, r2, #24
.L_08015e7c:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_08015e7c
	movs r0, #0
.L_08015e86:
	pop {r5, r6, pc}
.L_08015e88:
	.4byte 0x84000bfc
