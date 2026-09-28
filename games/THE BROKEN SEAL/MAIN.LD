OUTPUT_ARCH(arm)
ENTRY(Entry_080003c0)
SECTIONS
{
    .startup 0x080000c0 :
    {
        "*/SRC/SYSTEM/FAR_CALL/RUNTIME.o"(.text)
        "*/SRC/SYSTEM/ENTRY.o"(.text)
        "*/SRC/SOUND/DRIVER/RESAMPLER*.o"(.text .padding)
        "*/SRC/SOUND/DRIVER/CHANNEL_UPDATE_ENTRY.o"(.text)
    }
    /* Startup copies this complete resident module bank with DMA3. */
    .iwram 0x03000000 : AT(0x08000770)
    {
        "*/SRC/SYSTEM/RUNTIME*.o"(.text .rodata .data .padding)
    }
    .text 0x08001b70 : AT(0x08001b70)
    {
        *(.text .rodata .data .padding)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
