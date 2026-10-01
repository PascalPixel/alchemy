.syntax unified
	.thumb
	.global Func_080d1f20
	.thumb_func
Func_080d1f20:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	ldr r0, .L_080d2024
	sub sp, #8
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #16
	mov r10, r0
	bl Func_080d2a8c
	movs r0, #130
	lsls r0, r0, #5
	bl Runtime_BumpAllocateAlternatePool
	lsls r5, r5, #2
	ldr r3, [r5, r7]
	ldr r6, .L_080d2028
	mov r8, r0
	cmp r3, #0
	bge .L_080d1f52
	adds r3, #3
.L_080d1f52:
	asrs r3, r3, #2
	lsls r3, r3, #2
	adds r7, r7, r3
	mov r1, r8
	adds r0, r7, #0
	bl Resource_DecodeByteLzInRam
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	mov r0, r8
	ldr r1, .L_080d202c
	ldr r2, .L_080d2030
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r8
	bl Sys_Free
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r4, #240
	movs r3, #128
	add r0, sp, #4
	lsls r4, r4, #8
	lsls r3, r3, #19
	str r4, [r0]
	adds r3, #212
	adds r1, r6, #0
	ldr r2, .L_080d2034
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r6, .L_080d2038
	movs r1, #0
.L_080d1fa0:
	adds r6, #4
	movs r5, #0
.L_080d1fa4:
	mov r2, r10
	movs r0, #128
	lsls r3, r2, #16
	lsls r0, r0, #9
	lsls r2, r2, #16
	adds r3, r3, r0
	lsrs r2, r2, #16
	asrs r3, r3, #16
	orrs r2, r4
	adds r5, #1
	strh r2, [r6]
	mov r10, r3
	adds r6, #2
	cmp r5, #25
	bls .L_080d1fa4
	adds r1, #1
	adds r6, #8
	cmp r1, #4
	bls .L_080d1fa0
	movs r5, #128
	lsls r5, r5, #19
	ldrh r2, [r5]
	movs r3, #248
	lsls r3, r3, #5
	adds r3, #255
	ands r3, r2
	strh r3, [r5]
	ldr r2, .L_080d2010
	ldrh r3, [r5]
	movs r6, #128
	orrs r3, r2
	strh r3, [r5]
	movs r2, #128
	ldr r3, .L_080d2014
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_080d2018
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_080d201c
	lsls r6, r6, #19
	adds r6, #64
	strh r3, [r6]
	ldr r3, .L_080d2020
	subs r2, #6
	strh r3, [r2]
	movs r1, #0
	bl BattleFx_ApplyColorToSourceBuffer
	movs r0, #128
	lsls r0, r0, #9
	b .L_080d203c
	.2byte 0x0000
.L_080d2010:
	.4byte 0x00002000
.L_080d2014:
	.4byte 0x0000003f
.L_080d2018:
	.4byte 0x0000003e
.L_080d201c:
	.4byte 0x00007878
.L_080d2020:
	.4byte 0x00001848
.L_080d2024:
	.4byte 0x00000026
.L_080d2028:
	.4byte 0x06002000
.L_080d202c:
	.4byte 0x06000200
.L_080d2030:
	.4byte 0x84000410
.L_080d2034:
	.4byte 0x85000200
.L_080d2038:
	.4byte 0x060020c0
.L_080d203c:
	adds r0, #8
	movs r1, #0
	bl Func_080d170c
	movs r0, #24
	bl BattleFx_StartBufferInterpolation
	ldrh r3, [r5]
	ldr r2, .L_080d205c
	movs r7, #120
	eors r3, r2
	strh r3, [r5]
	movs r2, #120
	movs r5, #0
	b .L_080d2060
	.2byte 0x0000
.L_080d205c:
	.4byte 0x00000100
.L_080d2060:
	subs r2, #5
	lsls r3, r2, #8
	adds r7, #5
	orrs r3, r7
	strh r3, [r6]
	movs r0, #1
	str r2, [sp, #0]
	bl WaitFrames
	adds r5, #1
	ldr r2, [sp, #0]
	cmp r5, #23
	bls .L_080d2060
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
