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
    * Phone dock with USB port
      * + USB for stereo view
      * + USB for Oculus Rift head tracking
      * + USB for Telemetry
      * + HDMI for Oculus Rift
* Video tract:
  * USB DVR (2 x [EasyCAP](http://linuxtv.org/wiki/index.php/Easycap))
  * A/V Receiver [SkyZone 5.8GHz 8CH Diversity A/V Receiver w/Dual Output](http://www.hobbyking.com/hobbyking/store/__27778__SkyZone_5_8GHz_8CH_Diversity_A_V_Receiver_w_Dual_Output.html))
  * A/V Transmitter (2 x [SkyZone TS353 5.8G 400mW FPV Transmitter](http://www.hobbyking.com/hobbyking/store/__27145__SkyZone_TS353_5_8G_400mW_FPV_Transmitter.html)))
  * 12V UBEC ([12V 4.5A UBEC 2-5S Lipoly](http://www.hobbyking.com/hobbyking/store/__29761__12V_4_5A_UBEC_2_5S_Lipoly_7_2_21v_UK_Warehouse_.html))
  * CCD Video Camera or GoPro (2 x [690TVL Ultra WDR Pixim SEAWOLF Sensor Chip OSD Camera 2.1mm Wide View](http://www.securitycamera2000.com/products/690TVL-Ultra-WDR-Pixim-SEAWOLF-Sensor-Chip-OSD-Camera-2.1mm-Wide-View.html))
  * 1000mAh 4s LiPo Battery
  * _Optional_: GoPro 3+ Gimbal ([DYS 3 Axis Smart Gopro Brushless Gimbal](http://www.rcgroups.com/forums/showthread.php?t=2059104))
  * _Optional_: Clever antennas

Development
-----------

### Salticidae body

This is cross-platform application, placed in salticidae directory

### Plugins

Salticidae support plugin system to be simple extended in the future.
About plugins you can read in plugins/README.md file.

By default - all plugins in subdirectory plugins will be added to build & deploy.

### Build process

Build master project.pro file by qtcreator or qmake for need platform
