rEyes - camera viewer
=====================

* Easycap Fushicai USBTV007 "usbtv" driver backport to linux 3.2:
  https://github.com/mwelchuk/usbtv

* usbtv easycap:
$ v4l2-ctl --all -d /dev/video1 
Driver Info (not using libv4l2):
    Driver name   : usbtv
    Card type     : usbtv
    Bus info      : usb-0000:00:10.0-1
    Driver version: 3.13.6
    Capabilities  : 0x85000001
        Video Capture
        Read/Write
        Streaming
        Device Capabilities
    Device Caps   : 0x05000001
        Video Capture
        Read/Write
        Streaming
Priority: 2
Video input : 0 (Composite: ok)
Video Standard = 0x0000f900
    PAL-M/60
    NTSC-M/M-JP/443/M-KR
Format Video Capture:
    Width/Height  : 720/480
    Pixel Format  : 'YUYV'
    Field         : Interlaced
    Bytes per Line: 1440
    Size Image    : 691200
    Colorspace    : Broadcast NTSC/PAL (SMPTE170M/ITU601)
Streaming Parameters Video Capture:
    Frames per second: 29.970 (30000/1001)
    Read buffers     : 2
