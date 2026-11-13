@ Copies of four of sound.c's routines and the head of a fifth, which the
@ cartridge keeps 0x1a70 bytes before them, and an ARM high multiply after
@ them. Nothing refers to any of them.
@ Each copy's bytes are its twin's but for what a link fills in: its pool
@ words are 0 where the twin's hold addresses (the block copy's control word
@ is kept), and its call to Bios_CpuSet, outside the sound library, is a call
@ to itself. Its other calls keep the twin's offsets, so each lands 0x1a70
@ bytes before the twin's callee. The fifth stops short after the player
@ slot's offset, its two loads reaching 8 bytes further than its twin's.
@ Why assembly: copies of routines the ROM also links, their calls keeping
@ those routines' offsets, their pool words never filled in and the last cut
@ short (S1); the multiply switches to ARM inside its Thumb entry.

.syntax unified
	.thumb

@ Unused: the copy of m4aSoundInit.
	.global m4aSoundInitWorkCopy
	.thumb_func
m4aSoundInitWorkCopy:
	push	{r4, r5, r6, lr}
	ldr	r0, .L_081c0520
	movs	r1, #2
	negs	r1, r1
	ands	r0, r1
	ldr	r1, .L_081c0524
	ldr	r2, .L_081c0528
	bl	.	@ Bios_CpuSet in the twin
	ldr	r0, .L_081c052c
	bl	Func_081c08a4 + 0x28	@ SoundInit in the twin
	ldr	r0, .L_081c0530
	bl	SoundDriver_CalculateStereoVolume + 0xe	@ MPlayExtender in the twin
	ldr	r0, .L_081c0534
	bl	Func_081c0a68	@ m4aSoundMode in the twin
	ldr	r0, .L_081c0538
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq	.L_081c051a
	ldr	r5, .L_081c053c
	adds	r6, r0, #0
.L_081c04fe:
	ldr	r4, [r5]
	ldr	r1, [r5, #4]
	ldrb	r2, [r5, #8]
	adds	r0, r4, #0
	bl	Func_081c0c0c	@ MPlayOpen in the twin
	ldrh	r0, [r5, #10]
	strb	r0, [r4, #11]
	ldr	r0, .L_081c0540
	str	r0, [r4, #24]
	adds	r5, #12
	subs	r6, #1
	cmp	r6, #0
	bne	.L_081c04fe
.L_081c051a:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L_081c0520:
	.4byte	0	@ Sound_Mixer
.L_081c0524:
	.4byte	0	@ Ram_SoundMixer
.L_081c0528:
	.4byte	0x04000100	@ the block copy's control word: 256 words
.L_081c052c:
	.4byte	0	@ gSound_Work
.L_081c0530:
	.4byte	0	@ gSound_CgbNotes
.L_081c0534:
	.4byte	0	@ the initial sound mode
.L_081c0538:
	.4byte	0	@ the player count
.L_081c053c:
	.4byte	0	@ gSound_PlayerSlots
.L_081c0540:
	.4byte	0	@ gSound_WorkBytes

@ Unused: the copy of m4aSoundMain.
	.global m4aSoundMainWorkCopy
	.thumb_func
m4aSoundMainWorkCopy:
	push	{lr}
	bl	Resource_FarCall010 - 0x7e4	@ 0x081bf81c: SoundMain in the twin
	pop	{r0}
	bx	r0
	.2byte	0

@ Unused: the copy of m4aSongNumStart.
	.global m4aSongNumStartWorkCopy
	.thumb_func
m4aSongNumStartWorkCopy:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, .L_081c0574
	ldr	r1, .L_081c0578
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r2, [r1]
	ldr	r1, [r0]
	adds	r0, r2, #0
	bl	Sound_Initialize + 0x68	@ 0x081c0c84: MPlayStart in the twin
	pop	{r0}
	bx	r0
.L_081c0574:
	.4byte	0	@ gSound_PlayerSlots
.L_081c0578:
	.4byte	0	@ gSongTable

@ Unused: the copy of m4aSongNumStartOrChange.
	.global m4aSongNumStartOrChangeWorkCopy
	.thumb_func
m4aSongNumStartOrChangeWorkCopy:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, .L_081c05a8
	ldr	r1, .L_081c05ac
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r1, [r1]
	ldr	r3, [r1]
	ldr	r2, [r0]
	cmp	r3, r2
	beq	.L_081c05b0
	adds	r0, r1, #0
	adds	r1, r2, #0
	bl	Sound_Initialize + 0x68	@ 0x081c0c84: MPlayStart in the twin
	b	.L_081c05c4
	.2byte	0
.L_081c05a8:
	.4byte	0	@ gSound_PlayerSlots
.L_081c05ac:
	.4byte	0	@ gSongTable
.L_081c05b0:
	ldr	r2, [r1, #4]
	ldrh	r0, [r1, #4]
	cmp	r0, #0
	beq	.L_081c05bc
	cmp	r2, #0
	bge	.L_081c05c4
.L_081c05bc:
	adds	r0, r1, #0
	adds	r1, r3, #0
	bl	Sound_Initialize + 0x68	@ 0x081c0c84: MPlayStart in the twin
.L_081c05c4:
	pop	{r0}
	bx	r0

@ Unused: the head of a second copy of m4aSongNumStart; its two loads reach
@ words past its end, in the next routine.
	.global m4aSongNumStartWorkCopyHead
	.thumb_func
m4aSongNumStartWorkCopyHead:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, [pc, #36]
	ldr	r1, [pc, #40]
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2

@ Unused: the high word of a 32 by 32 multiply, as umul3232H32
@ (driver/umul_high.s) gives it in another form: a Thumb entry that switches to
@ ARM. The head before it runs into it.
	.global umul3232H32WorkCopy
	.thumb_func
umul3232H32WorkCopy:
	bx	pc
	nop
	.arm
	umull	r3, r0, r1, r0
	bx	lr
