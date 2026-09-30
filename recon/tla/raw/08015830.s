.syntax unified
	.thumb
	.global Resource_DecompressHalfwords
	.thumb_func
Resource_DecompressHalfwords:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_08015874
	adds r6, r1, #0
	adds r2, r3, #1
	adds r3, #4
	lsrs r3, r3, #2
	lsls r3, r3, #2
	mov r1, sp
	subs r1, r1, r3
	movs r4, #132
	movs r3, #128
	lsls r4, r4, #24
	lsrs r2, r2, #2
	lsls r3, r3, #19
	mov r7, sp
	adds r5, r0, #0
	mov r8, sp
	adds r3, #212
	mov sp, r1
	ldr r0, .L_08015878
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	adds r1, r6, #0
	mov lr, sp
	.2byte 0xf800
	mov sp, r8
	mov sp, r7
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_08015874:
	.4byte 0x00000057
.L_08015878:
	.4byte Resource_DecodeHalfwordLzCode
