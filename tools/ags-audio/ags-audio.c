/* Encode mono 8-bit WAV samples and 16-byte wave RAM patterns for the sound
 * engine. */
#include "../common/common.h"
#include <math.h>
#include <strings.h>

static uint32_t checkedWord(const uint8_t *data, size_t length, size_t at) {
  if (at > length || length - at < 4)
    fail("truncated WAV field");
  return read32(data + at);
}

static Buffer convert(const Buffer *input) {
  const uint8_t *p = input->data;
  size_t n = input->len;
  if (n < 12 || memcmp(p, "RIFF", 4) || memcmp(p + 8, "WAVE", 4) ||
      (uint64_t)read32(p + 4) + 8 != n)
    fail("invalid WAV RIFF header");
  uint32_t rate = 0, unity = 60, fraction = 0, start = 0, end = 0;
  bool sampler = false, loop = false, haveData = false;
  Buffer samples = {0};
  for (size_t at = 12; at < n;) {
    if (n - at < 8)
      fail("truncated WAV chunk");
    size_t size = read32(p + at + 4);
    if (size > n - at - 8)
      fail("truncated WAV payload");
    const uint8_t *c = p + at + 8;
    if (!memcmp(p + at, "fmt ", 4)) {
      if (rate || size != 16 || read16(c) != 1 || read16(c + 2) != 1 ||
          !read32(c + 4) || read32(c + 8) != read32(c + 4) ||
          read16(c + 12) != 1 || read16(c + 14) != 8)
        fail("WAV must be mono 8-bit PCM");
      rate = read32(c + 4);
    } else if (!memcmp(p + at, "data", 4)) {
      if (!rate || haveData)
        fail("WAV data duplicated or precedes fmt");
      haveData = true;
      for (size_t i = 0; i < size; i++)
        byte(&samples, c[i] - 128);
    } else if (!memcmp(p + at, "smpl", 4)) {
      uint32_t loops = checkedWord(c, size, 28);
      if (sampler || loops > 1 || checkedWord(c, size, 32) ||
          size != 36 + loops * 24 || checkedWord(c, size, 12) > 127)
        fail("unsupported WAV sampler metadata");
      sampler = true;
      unity = read32(c + 12);
      fraction = read32(c + 16);
      loop = loops != 0;
      if (loop) {
        if (read32(c + 40) || read32(c + 52) || read32(c + 56))
          fail("WAV needs one infinite forward loop");
        start = read32(c + 44);
        end = read32(c + 48);
      }
    } else
      fail("unsupported WAV chunk");
    at += 8 + size;
    if (size % 2 && at < n) {
      if (p[at++])
        fail("invalid WAV padding");
    }
  }
  if (!rate || !haveData || !samples.len || samples.len - 1 > UINT32_MAX)
    fail("WAV has no valid samples");
  if (loop && (start > end || end != samples.len - 1))
    fail("native PCM loop must end at the last sample");
  double scaled =
      rate * pow(2.0, (60.0 - unity - fraction / 4294967296.0) / 12.0) * 1024.0;
  double pitch = round(scaled);
  if (!isfinite(pitch) || pitch < 1 || pitch > UINT32_MAX)
    fail("WAV pitch exceeds native format");
  if (fabs((scaled - floor(scaled)) - 0.5) < 1e-6)
    fail("WAV pitch is too close to a half; adjust its pitch fraction");
  Buffer out = {0};
  word(&out, loop ? 0x40000000 : 0);
  word(&out, (uint32_t)pitch);
  word(&out, loop ? start : 0);
  word(&out, (uint32_t)samples.len - 1);
  append(&out, samples.data, samples.len);
  alignBuffer(&out, 4);
  free(samples.data);
  return out;
}

int main(int argc, char **argv) {
  const char *usage = "usage: ags-audio INPUT.pcm8.wav|INPUT.pcm4 OUTPUT.bin";
  startTool(argc, argv, usage);
  if (argc != 3)
    fail("%s", usage);
  Buffer input = readFile(argv[1]), out = {0};
  if (!strcasecmp(extension(argv[1]), "wav"))
    out = convert(&input);
  else if (!strcasecmp(extension(argv[1]), "pcm4")) {
    if (input.len != 16)
      fail("a wave RAM pattern is sixteen bytes");
    append(&out, input.data, input.len);
  } else
    fail("not a sound input: %s", argv[1]);
  writeFile(argv[2], &out);
  free(out.data);
  free(input.data);
  return 0;
}
