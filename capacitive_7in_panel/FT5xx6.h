/************************************************************************
This is a library to handling capacitive multitouch panels using FT5xx6.
Originally written to work with ER-TFTM070-5 (LCD module with FT5206 controller) from EastRising.
Later tested with separate 7" touch panel ER-TPC070-6 with FT5316 controller.

Written by Helge Langehaug 2014-2017

BSD license, all text above must be included in any redistribution
*************************************************************************/


/* FT5x06 definitions */
#define FT5xx6_I2C_ADDRESS 0x38
#define FT5xx6_NUMBER_OF_REGISTERS 31     // there are more registers, but this
                                          // is enought to get all 5 touch coordinates.
                                          
#define FT5xx6_NUMBER_OF_TOTAL_REGISTERS 0xFE

#define FT5xx6_DEVICE_MODE 0x00

#define FT5xx6_GEST_ID 0x01
#define FT5xx6_GEST_ID_MOVE_UP     0x10
#define FT5xx6_GEST_ID_MOVE_LEFT   0x14
#define FT5xx6_GEST_ID_MOVE_DOWN   0x18
#define FT5xx6_GEST_ID_MOVE_RIGHT  0x1c
#define FT5xx6_GEST_ID_ZOOM_IN     0x48
#define FT5xx6_GEST_ID_ZOOM_OUT    0x49
#define FT5xx6_GEST_ID_NO_GESTURE  0x00

#define FT5xx6_TD_STATUS 0x02

#define FT5xx6_TOUCH1_XH 0x03
#define FT5xx6_TOUCH1_XL 0x04
#define FT5xx6_TOUCH1_YH 0x05
#define FT5xx6_TOUCH1_YL 0x06

#define FT5xx6_TOUCH2_XH 0x09
#define FT5xx6_TOUCH2_XL 0x0a
#define FT5xx6_TOUCH2_YH 0x0b
#define FT5xx6_TOUCH2_YL 0x0c

#define FT5xx6_TOUCH3_XH 0x0f
#define FT5xx6_TOUCH3_XL 0x10
#define FT5xx6_TOUCH3_YH 0x11
#define FT5xx6_TOUCH3_YL 0x12

#define FT5xx6_TOUCH4_XH 0x15
#define FT5xx6_TOUCH4_XL 0x16
#define FT5xx6_TOUCH4_YH 0x17
#define FT5xx6_TOUCH4_YL 0x18

#define FT5xx6_TOUCH5_XH 0x1b
#define FT5xx6_TOUCH5_XL 0x1c
#define FT5xx6_TOUCH5_YH 0x1d
#define FT5xx6_TOUCH5_YL 0x1e

#define FS5xx6_TOUCH_LIB_VERSION_H 0xa1
#define FS5xx6_TOUCH_LIB_VERSION_L 0xa2

class FT5xx6 {
 public:
  FT5xx6(uint8_t CTP_INT);
  byte getTouchPositions(word *touch_coordinates, byte *reg);
  void init(bool serial_output_enabled);
  void getRegisterInfo(byte *registers);
  void printInfo();
  bool touched();
 private:
  uint8_t _ctpInt;
};

