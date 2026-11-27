#ifndef ALCHEMY_ASM_H
#define ALCHEMY_ASM_H

/* Every piece of assembly the C holds, one macro to each instruction sequence
   and one for a local kept in a register, each a fake match. The helpers that
   use them are ordinary inlines in their own headers. No other file has an asm
   statement with anything in it.

   They are one set, not a scattering: the system library is built to the shape
   of SGL, Sega's engine for the Saturn, on the hardware GBATEK describes, and
   these are the few operations of it small enough to write in place. The system
   library's README, src/system/README.md, says what that rests on. */

/* The BIOS

   The BIOS call the game makes in place, by GBATEK's number: SWI 03h, Stop. The
   BIOS's arithmetic and decompression it never calls: the system library brings
   its own. */

/* Stops the CPU and the screen until an enabled interrupt wakes them (the BIOS
   Stop call). */
#define ASM_BIOS_STOP() \
	/* FAKEMATCH(asm): BIOS Stop has no C instruction. */ \
	__asm__ volatile("swi 3")

/* DMA

   A channel's three registers, source, destination and control (GBATEK's
   DMAxSAD, DMAxDAD and DMAxCNT), written with one instruction. syDMAXCopy takes
   its arguments as SGL's slDMAXCopy does, in the registers a call would pass
   them. */

/* Stores source, destination and control into a DMA channel's three registers
   with one stmia, and steps the channel pointer back. */
#define ASM_DMA_STORE(source, control, channel, destination) \
	/* FAKEMATCH(asm): stmia of three registers with no \
	 * paired ldmia, which no agscc pattern emits. */ \
	__asm__ volatile("stmia %2!, {%0, %3, %1}\n\tsub %2, #12" \
		: "+r"(source), "+r"(control) \
		: "r"(channel), "r"(destination) \
		: "memory", "cc")

/* Fixed point

   SGL's slMulFX. The multiply itself is a function in IWRAM, which the Thumb C
   calls; this is the same multiply written out for the C compiled as ARM. */

/* The 64-bit product of two words, its halves in any two registers. */
#define ASM_SMULL(high, low, left, right) \
	/* FAKEMATCH(asm): smull with RdLo and RdHi in any two \
	 * registers, even over its inputs, where mulsidi3 pairs them (RdHi = RdLo \
	 * + 1) apart from the inputs and smulsi3_highpart discards RdLo; its orr \
	 * takes a fresh output reload register beside two dying inputs and \
	 * nothing is scheduled across it, as only asm statements do. */ \
	__asm__ volatile("smull %1, %0, %3, %2" \
		: "=r"(high), "=r"(low) \
		: "r"(left), "r"(right))

/* The middle 32 bits of a 64-bit product, from its two halves. */
#define ASM_ORR_SHIFTED(result, high, low) \
	/* FAKEMATCH(asm): syMulFXArm's next instruction. */ \
	__asm__ volatile("orr %0, %1, %2, lsr #16" \
		: "=r"(result) \
		: "r"(high), "r"(low))

/* The matrix

   SGL's slUnitMatrix: twelve words, 1.0 on the diagonal. It destroys r0 to r4,
   the registers every function of the system library treats as scratch. */

/* Stores the unit matrix at a pointer: three runs of 1.0 and three zeros. */
#define ASM_MATRIX_UNIT(matrix) \
	/* FAKEMATCH(asm): three stmia no C store or copy \
	 * compiles to. */ \
	__asm__ volatile( \
		"mov r0, %0\n\tmov r1, #128\n\tmov r2, #0\n\tmov r3, #0\n\t" \
		"mov r4, #0\n\tlsl r1, r1, #9\n\t" \
		"stmia r0!, {r1, r2, r3, r4}\n\t" \
		"stmia r0!, {r1, r2, r3, r4}\n\t" \
		"stmia r0!, {r1, r2, r3, r4}" \
		: \
		: "r"(matrix) \
		: "r0", "r1", "r2", "r3", "r4", "memory")

/* ASM_MATRIX_UNIT with 1.0 built before the zeros. */
#define ASM_MATRIX_UNIT_STORE(matrix) \
	/* FAKEMATCH(asm): three stmia no C store or copy \
	 * compiles to. */ \
	__asm__ volatile( \
		"mov r0, %0\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t" \
		"mov r3, #0\n\tmov r4, #0\n\t" \
		"stmia r0!, {r1, r2, r3, r4}\n\t" \
		"stmia r0!, {r1, r2, r3, r4}\n\t" \
		"stmia r0!, {r1, r2, r3, r4}" \
		: \
		: "r"(matrix) \
		: "r0", "r1", "r2", "r3", "r4", "memory")

/* ASM_MATRIX_UNIT as the system library itself writes it for the current
   matrix: the same stores, with r3 left off the list of what it destroys. */
#define ASM_MATRIX_UNIT_CURRENT(matrix) \
	/* FAKEMATCH(asm): syUnitMatrix's three stmia, \
	 * applied to the current matrix. */ \
	__asm__ volatile( \
		"mov r0, %0\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t" \
		"mov r3, #0\n\tmov r4, #0\n\t" \
		"stmia r0!, {r1, r2, r3, r4}\n\t" \
		"stmia r0!, {r1, r2, r3, r4}\n\t" \
		"stmia r0!, {r1, r2, r3, r4}" \
		: \
		: "r"(matrix) \
		: "r0", "r1", "r2", "r4", "memory")

/* Block copies

   A few words copied with one load and one store, where the compiler's own
   block move stops at three. */

/* Copies two words with one block load and store. */
#define ASM_COPY_WORDS_2(source, destination) \
	/* FAKEMATCH(asm): ldmia and stmia of two registers \
	 * from r0 to r1 with scratch from r2 up, in the shape of syDMAXCopy: \
	 * agscc's block moves leave r0 and r1 to the allocator. */ \
	__asm__ volatile("ldmia %0!, {r2, r3}\n\tstmia %1!, {r2, r3}" \
		: "+r"(source), "+r"(destination) \
		: \
		: "r2", "r3", "memory")

/* Copies three words with one block load and store. */
#define ASM_COPY_WORDS_3(source, destination) \
	/* FAKEMATCH(asm): ldmia and stmia of three registers \
	 * from r0 to r1 with scratch from r2 up, in the shape of syDMAXCopy: \
	 * agscc's block moves leave r0 and r1 to the allocator. */ \
	__asm__ volatile("ldmia %0!, {r2, r3, r4}\n\tstmia %1!, {r2, r3, r4}" \
		: "+r"(source), "+r"(destination) \
		: \
		: "r2", "r3", "r4", "memory")

/* Copies four words with one block load and store. */
#define ASM_COPY_WORDS_4(source, destination) \
	/* FAKEMATCH(asm): ldmia and stmia of four registers: \
	 * agscc's block moves stop at three, so a 16-byte struct copy compiles to \
	 * 12 and 4. */ \
	__asm__ volatile( \
		"ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}" \
		: "+r"(source), "+r"(destination) \
		: \
		: "r2", "r3", "r4", "r5", "memory")

/* The stack and the restart

   The soft reset's jump to the cartridge's start. */

/* Jumps to `entry` and does not return: the cartridge restart. */
#define ASM_CART_JUMP(entry) \
	/* FAKEMATCH(asm): the cartridge restart transfers \
	 * control without returning; the target is in r4 because the jump lists \
	 * r0 to r2 as changed while r3 holds REG_IME, which no C call gives. */ \
	__asm__ volatile("bx %0" \
		: \
		: "r"(entry) \
		: "r0", "r1", "r2", "r3", "ip", "lr")

/* Registers and calls C cannot name */

/* Keeps a local in the register named. */
#define ASM_REGISTER(declaration, name) \
	/* FAKEMATCH(asm): a local pinned to a register by an asm register name. \
	 */ \
	register declaration __asm__(name)

/* Calls a stub table slot in Thumb through fdMatrixCallInThumb, with its target
   in ip, and leaves the low half of what it returns in `angle`. */
#define ASM_THUMB_CALL_ARCTAN(angle, xValue, yValue, slot) \
	/* FAKEMATCH(asm): x pinned to r0 by an asm register name. */ \
	register Sint32 x __asm__("r0") = (xValue); \
	/* FAKEMATCH(asm): y pinned to r1 by an asm register name. */ \
	register Sint32 y __asm__("r1") = (yValue); \
	/* FAKEMATCH(asm): result pinned to r0 by an asm register name. */ \
	register Sint32 result __asm__("r0"); \
\
	/* FAKEMATCH(asm): ldr ip from the table and bl \
	 * fdMatrixCallInThumb, a call through ip that returns in Thumb, which no \
	 * C call compiles to; its list is the call's: r2 to r4, ip, lr, flags and \
	 * memory. */ \
	__asm__ volatile("ldr\tip, %3\n\tbl\tfdMatrixCallInThumb" \
		: "=r"(result) \
		: "r"(x), "r"(y), "m"(slot) \
		: "r2", "r3", "r4", "ip", "lr", "cc", "memory"); \
	(angle) = (Uint16)result

/* A statement that emits nothing */

/* The statement that ends a sprite's set-up, a menu sprite's or a battle
   effect's: it emits nothing, the compiler counts four bytes for it when it
   places the next literal pool, and nothing is scheduled across it. */
#define ASM_NOTHING() \
	/* FAKEMATCH(barrier): an empty asm; the literal pools after a menu \
	 * sprite's two stores sit where the ROM has them only with four bytes \
	 * counted that no instruction emits, which nothing else in this compiler \
	 * does, and Daedalus's missile sprite needs the same statement; as no \
	 * asm. */ \
	__asm__ volatile("")

/* The modern build */

/* Declares `symbol` as the bytes at the linked name `name`, whatever type the
   name has elsewhere: how the modern build reaches a fixed address. */
#define ASM_LINKED_NAME(symbol, name) \
	/* FAKEMATCH(asm): another name for a linked symbol, by an asm label. */ \
	extern char symbol[] __asm__(name)

/* Hides what `value` holds from the modern compiler, so that it reads it
   through a register. */
#define ASM_OPAQUE(value) \
	/* FAKEMATCH(barrier): an empty asm that rewrites value; as no asm. */ \
	__asm__("" : "+r"(value))

#endif
