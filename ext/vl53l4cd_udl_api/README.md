# Ultra Lite Driver (ULD) for VL53L5CX multi-zone sensor

[![Driver Version](https://img.shields.io/badge/driver_version-2.2.2-blue)](https://www.st.com/en/embedded-software/stsw-img026.html)

## Description
This is a slightly modified version of the STMicroelectronics ultra lite driver (ULD) for VL53L4CD. from:
- [https://www.st.com/en/embedded-software/stsw-img026.html](https://www.st.com/en/embedded-software/stsw-img026.html)

## Changes
- Renamed platform.h to vl53l4cd_platform.h
- Renamed Dev_t to VL53L5CX_Dev_t
- Added `extern "C" {...` to all header files to enabled using them from c++ code
- Changed directory structure

## Documentation
VL53L4CD ULD user manual UM2931 is available [here](https://www.st.com/resource/en/user_manual/um2931-a-guide-to-using-the-vl53l4cd-ultra-lite-driver-uld-stmicroelectronics.pdf)


## License:
[BSD](LICENSE.txt)
