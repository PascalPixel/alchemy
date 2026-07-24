// Tool role: library; imported by tools/audio_engine_data.ts, tools/audit_residuals.ts, tools/battle_effect_data.ts (+62 more).
// Compact canonical form: objects one key per line, arrays of primitives
// inline on one line. This replaces JSON.stringify(value, null, 2) as the
// writer default: the old form spent one line per array element, which put
// two million lines of numeric bulk into the tracked tree. Checkers accept
// the compact form, the fully minified form, and (during migration) the
// legacy two-space form.

function isPrimitive(value: unknown): boolean {
  return value === null || typeof value === "number" || typeof value === "string" || typeof value === "boolean";
}

function reflow(value: unknown, indent: string): string {
  if (isPrimitive(value)) return JSON.stringify(value);
  if (Array.isArray(value)) {
    if (value.length === 0) return "[]";
    if (value.every(isPrimitive)) return "[" + value.map((x) => JSON.stringify(x)).join(", ") + "]";
    return "[\n" + value.map((x) => indent + "  " + reflow(x, indent + "  ")).join(",\n") + "\n" + indent + "]";
  }
  const record = value as Record<string, unknown>;
  const keys = Object.keys(record);
  if (keys.length === 0) return "{}";
  return "{\n" + keys.map((k) => `${indent}  ${JSON.stringify(k)}: ${reflow(record[k], indent + "  ")}`).join(",\n") + "\n" + indent + "}";
}

export function canonicalJson(value: unknown): string {
  return reflow(value, "");
}

export function isCanonicalJsonText(text: string, value: unknown): boolean {
  return (
    text === `${canonicalJson(value)}\n` ||
    text === `${JSON.stringify(value)}\n` ||
    text === `${JSON.stringify(value, null, 2)}\n`
  );
}
