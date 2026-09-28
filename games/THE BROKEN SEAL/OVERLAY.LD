/* A code overlay: its whole listing, linked where the resource loader places it. */
OUTPUT_ARCH(arm)
SECTIONS
{
    .text 0x2000000 : { *(.text .text.*) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
