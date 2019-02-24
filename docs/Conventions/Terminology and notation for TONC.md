The author of TONC (Jasper Vijn) has stated that he will use the following notation

# Basic text notation
Type | Notation | Example
--- | --- | ---
bit n in a ``foo`` | foo``{n}`` | ``REG_DISPCNT{4}`` (active page bit)
code | ``<code>`` tag | ``sx``
command/file | ``<tt>`` tag | ``vid.h``
matrix | bold, uppercase | **P**
memory | hex + code | ``0400:002eh``
new term | bold, italic | **_charblock_**
variable | italics | _x_
vector | bold, lowercase | **v**

# Symbols
Symbol | Sescription
--- | ---
α, β, γ | Greek letters
≈ | approximately
½ | one half
¼ | one quarter
¾ | three quarters
≥ | greater or equal
↔ |double-sided arrow
∈ |is in (an interval)
⟨ ⟩ | ‘bra’ & ‘ket’
→ | right arrow
² | superscript 2
× | times

# Variable notations
base type | alt name | unsigned | signed | volatile
--- | --- | --- | --- | ---
char | byte | u8 | s8 | vu8 / vs8
short | halfword | u16 | s16 | vu16 / vs16
int | word | u32 | s32 | vu32 / vs32

# Other notations
## Data representations
### numbers, both hex or decimal
C notation(0x) or assembly affix ('h', 0000:0000h)

### IO Registers
Displayed through the format of a table, dissecting the bit balues from the register.
The table will have the bit/s as the header cells and a unique identifier for the bit/s underneath.
Each unique identifier will be described in a following table.
Bits with an overbar ( ̅ ) are read-only and cannot be modified.
Bits with an underbar (_) are write-only and cannot be read.

Eg. Given a definition for the following memory address as an IO register

```c
#define  REG_DISPSTAT *(u16*)0x04000004
```

The following is a breakdown of register 0x04000004 (in assembly affix, 0400:0004h)

A more accurate dissection of this register can be found on the TONC documentation

F E D C B A 9 8 | 7 | 6 | 5 | 4 | 3 | 2̅ | 1̅ | 0̅
:---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---:
VcT | - | VcI | HbI| VbI | VcS | HbS | VbS

bits | name | define | description
--- | --- | --- | ---
0 | VbS | DSTAT_IN_VBL | VBlank status, read only. Will be set inside VBlank, clear in VDraw.
| other fields | |
8-F | VcT | *DSTAT_VCT#* | VCount trigger value. If the current scanline is at this value, bit 2 is set and an interrupt is fired if requested.

When referencing fields in a register, a special indexing format will be used using hexadecimal indices.
Eg. DSTAT_VCT{0} is the VbS field while DSTAT_VCT{8-F} refers to a byte starting from the left-most bit (equal to the whole VcT field).

**NB:** Defines ending with a # indicate the presence of additional properties pertaining to that define. The following can be expected as additional definitions for that define:
- DSTAT_VCT_MASK -- Defines which bits are used by the define for the memory space it occupies.
- DSTAT_VCT_SHIFT -- Equals how many times a bit must be left-shifted to be moved into the field.
- DSTAT_VCT(_n) -- A 'setter', accepting an input and shifting it using DSTAT_VCT_SHIFT to input it in the correct location.