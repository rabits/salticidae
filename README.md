Salticidae
==========

Flexible crossplatform live camera viewer.
Also it is an FPV viewer for your mobile phone & stereo glasses (like Oculus Rift).

Features:
---------
* Cross-platform (tested on Ubuntu & Android)
* v4l2 USB DVR support

TODO:
-----
* Dynamic & Configurable video layout
* Telemetry OSD support
* Oculus Rift barrel distortion support
* Oculus Rift head tracking (telemetry & trainer port) control support
* IP cameras support
* Video caching & navigation
* Video archive support

Requirements:
-------------

* Exec platform:
  * Linux (Ubuntu 14.04)
  * Android phone (Galaxy Note 3 N9005)
    * Phone dock with 1 USB
      * + 1 USB (for stereo view)
      * + HDMI for Oculus Rift
      * + 1 USB for Oculus Rift head tracking
* Video tract:
  * USB DVR (2 x [EasyCAP](http://linuxtv.org/wiki/index.php/Easycap))
  * A/V Receiver [SkyZone 5.8GHz 8CH Diversity A/V Receiver w/Dual Output](http://www.hobbyking.com/hobbyking/store/__27778__SkyZone_5_8GHz_8CH_Diversity_A_V_Receiver_w_Dual_Output.html))
  * A/V Transmitter (2 x [SkyZone TS353 5.8G 400mW FPV Transmitter](http://www.hobbyking.com/hobbyking/store/__27145__SkyZone_TS353_5_8G_400mW_FPV_Transmitter.html)))
  * 12V UBEC ([12V 4.5A UBEC 2-5S Lipoly](http://www.hobbyking.com/hobbyking/store/__29761__12V_4_5A_UBEC_2_5S_Lipoly_7_2_21v_UK_Warehouse_.html))
  * CCD Video Camera or GoPro (2 x [690TVL Ultra WDR Pixim SEAWOLF Sensor Chip OSD Camera 2.1mm Wide View](http://www.securitycamera2000.com/products/690TVL-Ultra-WDR-Pixim-SEAWOLF-Sensor-Chip-OSD-Camera-2.1mm-Wide-View.html))
  * 1000mAh 4s LiPo Battery
  * _Optional_: GoPro 3+ Gimbal ([DYS 3 Axis Smart Gopro Brushless Gimbal](http://www.rcgroups.com/forums/showthread.php?t=2059104))
  * _Optional_: Clever antennas

USB DVR
-------

### EasyCAP Fushicai USBTV007
To use this hardware on your android smartphone, you need to build usbtv driver and replace your kernel.
Clone [Kernel N9005](https://github.com/rabits/kernel_n9005) repo and follow instructions in README.

```
$ v4l2-ctl --all -d /dev/video0
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
```
