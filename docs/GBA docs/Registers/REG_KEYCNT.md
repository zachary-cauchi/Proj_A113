# Register name: REG_KEYCNT (REG_P1CNT)
## Location: `0400:0132h`
## Main purpose:
- Allows for interrupts to be set for keys, much like REG_DISPCNT for video interrupts.

F | E | D C B A | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
:---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---:
Op | I | - | L | R | down | up | left | right | start | select | B | A

bits | name | define | description
:---: | :---: | :---: | ---
0-9 | keys | KEY_x | keys to check for raising a key interrupt.
E | I | KCNT_IRQ | Enables keypad interrupt
F | Op | KCNT_OR, KCNT_AND | Boolean operator used for determining whether to raise a key- interrupt or not. If clear, it uses an OR (raise if any of the keys of bits 0-9 are down); if set, it uses an AND (raise if all of those keys are down). 