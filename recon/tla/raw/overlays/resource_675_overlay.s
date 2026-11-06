.syntax unified
	.thumb
	.section .text.x02008580,"ax",%progbits
	.balign 4
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	mov r12, r0
	ldr r0, [r3]
	movs r3, #224
	lsls r3, r3, #4
	movs r4, #224
	adds r1, r0, r3
	mov lr, r4
	movs r7, #0
.L_0200859c:
	ldrh r3, [r0]
	movs r5, #31
	lsrs r2, r3, #10
	ldr r6, .L_020085f8
	ands r2, r5
	movs r4, #248
	lsls r2, r2, #10
	lsls r4, r4, #7
	adds r0, #2
	mov r8, r6
	cmp r2, r4
	bls .L_020085b6
	adds r2, r4, #0
.L_020085b6:
	strh r2, [r1]
	lsrs r2, r3, #5
	ands r2, r5
	mov r6, r12
	muls r6, r2
	adds r2, r6, #0
	mov r6, r8
	ands r2, r6
	adds r1, #2
	cmp r2, r4
	bls .L_020085ce
	adds r2, r4, #0
.L_020085ce:
	strh r2, [r1]
	adds r2, r5, #0
	ands r2, r3
	mov r3, r12
	muls r3, r2
	mov r6, r8
	adds r2, r3, #0
	ands r2, r6
	adds r1, #2
	cmp r2, r4
	bls .L_020085e6
	adds r2, r4, #0
.L_020085e6:
	adds r7, #1
	strh r2, [r1]
	adds r1, #2
	cmp r7, lr
	bcc .L_0200859c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020085f8:
	.4byte 0xfffffc00
	.section .bss,"aw",%nobits
	.global gData_02001a14
gData_02001a14:
	.space 0x00000004
	.global gData_02001a18
gData_02001a18:
	.space 0x00000004
	.global gData_02001a1c
gData_02001a1c:
	.space 0x00000004
	.global gData_02001a20
gData_02001a20:
	.space 0x00000004
	.global gData_02001a24
gData_02001a24:
	.space 0x00000004
	.global gData_02001a28
gData_02001a28:
	.space 0x00000004
	.global Data_02001a2c
Data_02001a2c:
	.space 0x00000004
	.global gData_02001a30
gData_02001a30:
	.space 0x00000004
	.global gData_02001a34
gData_02001a34:
