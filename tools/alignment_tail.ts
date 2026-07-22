// Tool role: library; imported by tools/archive_asset.ts, tools/resource_d1_d3.ts, tools/sentou_resources.ts (+1 more).
export type AlignmentTail =
  | { size: number; encoding: "fill"; value: number }
  | { size: number; encoding: "bytes"; values: number[] };

function exactKeys(value: Record<string, unknown>, keys: readonly string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function byte(value: unknown, label: string): number {
  if (!Number.isInteger(value) || (value as number) < 0 || (value as number) > 0xff) {
    throw new Error(`${label} must be a byte`);
  }
  return value as number;
}

export function inspect_alignment_tail(data: Uint8Array, maximum = 3): AlignmentTail {
  if (!Number.isInteger(maximum) || maximum < 1) throw new Error("alignment-tail maximum must be positive");
  if (data.length < 1 || data.length > maximum) throw new Error("alignment tail is outside its bounded extent");
  const values = [...data];
  if (values.every((value) => value === values[0])) {
    return { size: values.length, encoding: "fill", value: values[0] };
  }
  return { size: values.length, encoding: "bytes", values };
}

export function parse_alignment_tail(
  value: unknown,
  expectedSize: number,
  maximum = 3,
  label = "alignment tail",
): AlignmentTail {
  if (!Number.isInteger(expectedSize) || expectedSize < 1 || expectedSize > maximum) {
    throw new Error(`${label} has an invalid expected size`);
  }
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error(`${label} must be an object`);
  }
  const tail = value as Record<string, unknown>;
  if (tail.encoding === "fill") {
    exactKeys(tail, ["size", "encoding", "value"], label);
    if (tail.size !== expectedSize) throw new Error(`${label} size differs from its boundary`);
    return { size: expectedSize, encoding: "fill", value: byte(tail.value, `${label} fill`) };
  }
  if (tail.encoding === "bytes") {
    exactKeys(tail, ["size", "encoding", "values"], label);
    if (tail.size !== expectedSize || !Array.isArray(tail.values) || tail.values.length !== expectedSize) {
      throw new Error(`${label} byte count differs from its boundary`);
    }
    return {
      size: expectedSize,
      encoding: "bytes",
      values: tail.values.map((item, index) => byte(item, `${label} byte ${index}`)),
    };
  }
  throw new Error(`${label} has an unsupported encoding`);
}

export function build_alignment_tail(tail: AlignmentTail): Buffer {
  const parsed = parse_alignment_tail(tail, tail.size, tail.size);
  return parsed.encoding === "fill"
    ? Buffer.alloc(parsed.size, parsed.value)
    : Buffer.from(parsed.values);
}

export function self_test_alignment_tail(): void {
  const fill = inspect_alignment_tail(Buffer.from([0xa5, 0xa5, 0xa5]));
  const bytes = inspect_alignment_tail(Buffer.from([0x12, 0x34]));
  if (!build_alignment_tail(fill).equals(Buffer.from([0xa5, 0xa5, 0xa5])) ||
      !build_alignment_tail(bytes).equals(Buffer.from([0x12, 0x34]))) {
    throw new Error("alignment-tail round trip differs");
  }
  const rejects = (action: () => void): boolean => { try { action(); return false; } catch { return true; } };
  if (!rejects(() => inspect_alignment_tail(Buffer.alloc(0))) ||
      !rejects(() => inspect_alignment_tail(Buffer.alloc(4))) ||
      !rejects(() => parse_alignment_tail({ size: 2, encoding: "fill", value: 0, extra: 1 }, 2)) ||
      !rejects(() => parse_alignment_tail({ size: 2, encoding: "bytes", values: [0] }, 2)) ||
      !rejects(() => parse_alignment_tail({ size: 1, encoding: "fill", value: 256 }, 1))) {
    throw new Error("alignment-tail adversarial test failed");
  }
}
