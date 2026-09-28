OUTPUT_ARCH(arm)
ENTRY(Entry)
SECTIONS
{
    .startup 0x080000c0 :
    {
        "*/recon/tla/raw/080000c0.o"(.text)
        "*/SRC/SYSTEM/ENTRY.o"(.text)
        "*/SRC/SOUND/RESAMPLER*.o"(.text .padding)
        "*/SRC/SOUND/VSYNC_DMA_REFRESH.o"(.text)
        "*/SRC/SOUND/CHANNEL_UPDATE_ENTRY.o"(.text)
    }
    /* Startup copies the resident module bank to IWRAM at 0x03000100. */
    .iwram 0x03000100 : AT(0x080006b8)
    {
        "*/SRC/SYSTEM/IWRAM_RUNTIME*.o"(.text .rodata .data .padding)
    }
    .text 0x08001658 : AT(0x08001658)
    {
        *(.text .rodata .data .padding)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
