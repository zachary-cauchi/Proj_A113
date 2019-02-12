# Register name: REG_DISPSTAT
## Location: `0400:0004h`
## Main purpose:
- Viewing VBlank and HBlank status
- Setting interrupts for Blank events

F E D C B A 9 8 | 7 6 | 5 |	4 | 3 | 2̅ | 1̅ | 0̅
:---: | :---: | --- | --- | --- | --- | --- |---
VcT | - | VcI | HbI | VbI | VcS | HbS | VbS

***

bits | name | define | description
--- | --- | --- | ---
0 | VbS | DSTAT_IN_VBL | VBlank status, read only. Will be set inside VBlank, clear in VDraw.
1 | HbS | DSTAT_IN_HBL | HBlank status, read only. Will be set inside HBlank.
2 | VcS | DSTAT_IN_VCT | VCount trigger status. Set if the current scanline matches the scanline trigger ( REG_VCOUNT == REG_DISPSTAT{8-F} )
3 | VbI | DSTAT_VBL_IRQ | VBlank interrupt request. If set, an interrupt will be fired at VBlank.
4 | HbI | DSTAT_HBL_IRQ | HBlank interrupt request.
5 | VcI | DSTAT_VCT_IRQ | VCount interrupt request. Fires interrupt if current scanline matches trigger value.
8-F | VcT | DSTAT_VCT# | VCount trigger value. If the current scanline is at this value, bit 2 is set and an interrupt is fired if requested.