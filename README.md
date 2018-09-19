# Diff

Very simple plugin to diff a section in memory with a file.

## Example usage

In this example we have `program_original.exe` and `program_modified.exe` and we want to diff them.

- Create a dump of a file's sections with `python dumpsec.py program_modified.exe`
- Open `program_original.exe` in x64dbg
- Run the command `diff 00401000, "section00.text.bin"`
- The difference will be applied as a patch which you can export (See Patches dialog with Ctrl+P)