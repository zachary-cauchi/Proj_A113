# Page flipping: The artist and two-sided canvas solution

Page flipping helps eliminate the issue of having 2 game states -- one from a past draw and another which has just been prepared for drawing -- being drawn at the same time and causing tearing on the scree. Page flipping involves using a back-buffer of the screen to act as the fallback for the hardware draw routines while you are writing to the main screen buffer and then inform the hardware to switch back to the readied one. The software then begins drawing the next frame on the other buffer and the cycle repeats.

### Small problems regarding implementation
- Two pointers must be maintained -- one to the page being displayed and the second to the page being written to.
- Because there are two frames in focus at any given time (1 frame in the past and the frame in the present), when redrawing an object from a past point in a new point, that objects must be erased from 2 frames ago (old one currently being displayed + old one that is being replaced = 2). Not a major issue, but a common pitfall.

## Details

Second page location is at `0600:A000h` and can be enabled by setting bit 4 of the display register (`REG_DISPCNT(4)`)

### Page flip function
Taken from [TONC's bitmap mode docs](http://www.coranac.com/tonc/text/bitmaps.htm)
```c
u16 *vid_flip()
{
    // toggle the write_buffer's page
    vid_page= (u16*)((u32)vid_page ^ VID_FLIP);
    REG_DISPCNT ^= DCNT_PAGE;            // update control register
    return vid_page;
}
```