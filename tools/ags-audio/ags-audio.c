/*
 * ags-audio.c
 *
 * Encode editable sounds for the sound driver.
 * WAV parsing validates mono PCM and optional tuning/loop metadata before
 * writing the native sample header. Wave-RAM inputs are already 16-byte
 * patterns and pass through after their size is checked.
 */
#include "../shared/shared.h"
#include <math.h>
#include <strings.h>

static uint32_t
checkedWord(const uint8_t *data, size_t length, size_t at)
{
	if (at > length || length - at < 4)
		fail("truncated WAV field");
	return read32(data + at);
}

typedef struct
{
	Buffer samples;
	uint32_t rate, unity, fraction, loopStart;
	bool loop;
} Wave;

/* WAV chunks may be padded, but only fmt, data, and sampler metadata are valid. */
static Wave
readWave(const Buffer *input)
{
	const uint8_t *data = input->data;
	size_t length = input->len;
	if (length < 12 || memcmp(data, "RIFF", 4) || memcmp(data + 8, "WAVE", 4) ||
		(uint64_t) read32(data + 4) + 8 != length)
		fail("invalid WAV RIFF header");
	Wave wave = {.unity = 60};
	uint32_t end = 0;
	bool sampler = false, haveData = false;
	for (size_t at = 12; at < length;)
	{
		if (length - at < 8)
			fail("truncated WAV chunk");
		size_t size = read32(data + at + 4);
		if (size > length - at - 8)
			fail("truncated WAV payload");
		const uint8_t *chunk = data + at + 8;
		if (!memcmp(data + at, "fmt ", 4))
		{
			if (wave.rate || size != 16 || read16(chunk) != 1 ||
				read16(chunk + 2) != 1 || !read32(chunk + 4) ||
				read32(chunk + 8) != read32(chunk + 4) ||
				read16(chunk + 12) != 1 || read16(chunk + 14) != 8)
				fail("WAV must be mono 8-bit PCM");
			wave.rate = read32(chunk + 4);
		}
		else if (!memcmp(data + at, "data", 4))
		{
			if (!wave.rate || haveData)
				fail("WAV data duplicated or precedes fmt");
			haveData = true;
			for (size_t i = 0; i < size; i++)
				byte(&wave.samples, chunk[i] - 128);
		}
		else if (!memcmp(data + at, "smpl", 4))
		{
			uint32_t loops = checkedWord(chunk, size, 28);
			if (sampler || loops > 1 || checkedWord(chunk, size, 32) ||
				size != 36 + loops * 24 || checkedWord(chunk, size, 12) > 127)
				fail("unsupported WAV sampler metadata");
			sampler = true;
			wave.unity = read32(chunk + 12);
			wave.fraction = read32(chunk + 16);
			wave.loop = loops != 0;
			if (wave.loop)
			{
				if (read32(chunk + 40) || read32(chunk + 52) ||
					read32(chunk + 56))
					fail("WAV needs one infinite forward loop");
				wave.loopStart = read32(chunk + 44);
				end = read32(chunk + 48);
			}
		}
		else
			fail("unsupported WAV chunk");
		at += 8 + size;
		if (size % 2 && at < length)
		{
			if (data[at++])
				fail("invalid WAV padding");
		}
	}
	if (!wave.rate || !haveData || !wave.samples.len ||
		wave.samples.len - 1 > UINT32_MAX)
		fail("WAV has no valid samples");
	if (wave.loop && (wave.loopStart > end || end != wave.samples.len - 1))
		fail("native PCM loop must end at the last sample");
	return wave;
}

/* Convert the WAV tuning into the sound driver's rate at MIDI note 60. */
static uint32_t
samplePitch(const Wave *wave)
{
	double scaled =
		wave->rate *
		pow(2.0, (60.0 - wave->unity - wave->fraction / 4294967296.0) / 12.0) *
		1024.0;
	double pitch = round(scaled);
	if (!isfinite(pitch) || pitch < 1 || pitch > UINT32_MAX)
		fail("WAV pitch exceeds native format");
	if (fabs((scaled - floor(scaled)) - 0.5) < 1e-6)
		fail("WAV pitch is too close to a half; adjust its pitch fraction");
	return (uint32_t) pitch;
}

/*
 * The native header stores the last sample index, not the sample count.
 * Looping samples end at that same index, checked by readWave.
 */
static Buffer
encodeWave(const Wave *wave)
{
	Buffer out = {0};
	word(&out, wave->loop ? 0x40000000 : 0);
	word(&out, samplePitch(wave));
	word(&out, wave->loop ? wave->loopStart : 0);
	word(&out, (uint32_t) wave->samples.len - 1);
	append(&out, wave->samples.data, wave->samples.len);
	alignBuffer(&out, 4);
	return out;
}

int
main(int argc, char **argv)
{
	const char *usage = "usage: ags-audio INPUT.pcm8.wav|INPUT.pcm4 OUTPUT.bin";
	startTool(argc, argv, usage);
	if (argc != 3)
		fail("%s", usage);
	Buffer input = readFile(argv[1]), out = {0};
	if (!strcasecmp(extension(argv[1]), "wav"))
	{
		Wave wave = readWave(&input);
		out = encodeWave(&wave);
		free(wave.samples.data);
	}
	else if (!strcasecmp(extension(argv[1]), "pcm4"))
	{
		if (input.len != 16)
			fail("a wave RAM pattern is sixteen bytes");
		append(&out, input.data, input.len);
	}
	else
		fail("not a sound input: %s", argv[1]);
	writeFile(argv[2], &out);
	free(out.data);
	free(input.data);
	return 0;
}
