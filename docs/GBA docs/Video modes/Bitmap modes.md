# Bitmap modes

## Summary:
Software-rendered where the contents of VRAM are mapped directly to a matrix of pixels on the GBA screen. Much slower as a result, impractical for most use-cases (but the easiest to teach/grasp). Unless intended for use in dynamic video rendering such as demos or 3-dimensional graphics, not recommended.

### Mode characteristics
mode | width | height | bpp | size | page-flip
:---: | :---: | :---: | :---: | :---: | :---:
3 | 240 | 160 | 16 | 1x 12C00h | No
4 | 240 | 160 | 8  | 2x 9600h | Yes
5 | 160 | 128 | 16 | 2x A000h | Yes 

## Mode 3:
Mode 3 is the most basic bitmap mode, having better a colour range than mode 4 but no page-flip.

## Mode 4:
Mode 4 benefits from the ability to use the palette bank located at `0500:0000h` to define what colours the pixels are. The 'pixels' themselves are actually indices to colour entries in a palette located in the palette bank. Because the pixels are half-sized compared to other modes, page-flipping can be used. Major downside to this mode stems from a hardware limitation, which prevents byte-reads from VRAM -- as such, you can only write 2 pixels minimum versus being able to read 1 pixel at a time.

### Takeaway: Byte-writes to VRAM are not possible -- read a halfword first then write the halfword back with the desired byte masked over
