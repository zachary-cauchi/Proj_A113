# Register name: REG_KEYINPUT (REG_P1)
## Location: `0400:0130h`
## Main purpose:
- A read-only buffer whose first 10 keys represent the states of the 10 keys on the GBA console.
- The register has a value of 0x03FF when no keys are pressed, meaning the bit of the corresponding key is set to 0 when that key is pressed. These can also be nickanmed '_normally closed switches_' to help remember.

F̅ E̅ D̅ C̅ B̅ A̅ | 9̅ | 8̅ | 7̅ | 6̅ | 5̅ | 4̅ | 3̅ | 2̅ | 1̅ | 0̅
:---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---:
- | L | R | down | up | left | right | start | select | B | A

To check the state of a key, the key bit in the register must first be inverted before comparing, like so: `#define KEY_DOWN_NOW(key)  (~(REG_KEYINPUT) & key)`