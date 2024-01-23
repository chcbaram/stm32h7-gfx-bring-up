#include "gt911.h"
#include "i2c.h"




#define GT911_TOUCH_WIDTH    480
#define GT911_TOUCH_HEIGTH   480


static bool readRegs(uint16_t reg_addr, void *p_data, uint32_t length);
static bool writeRegs(uint16_t reg_addr, void *p_data, uint32_t length);
static bool gt911InitRegs(void);

static bool is_init = false;
static uint8_t i2c_addr = 0x5D;
static bool is_detected = false;




bool gt911Init(void)
{
  bool ret = false;



  // I2C Addr for 0x5D
  //
  HAL_GPIO_WritePin(LCD_TP_RESET_GPIO_Port, LCD_TP_RESET_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_TP_INT_GPIO_Port, LCD_TP_INT_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(LCD_TP_RESET_GPIO_Port, LCD_TP_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(50);


  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = LCD_TP_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LCD_TP_INT_GPIO_Port, &GPIO_InitStruct);


  if (HAL_I2C_IsDeviceReady(&hi2c1, i2c_addr << 1, 10, 10) == HAL_OK)
  {
    is_detected = true;
  }

  if (is_detected == true)
  {
    ret = gt911InitRegs();
  }

  is_init = ret;

  return ret;
}

bool gt911InitRegs(void)
{
  uint8_t CTP_CFG_GT911[184] =
  {
    //  47   48   49   4A   4B   4C   4D   4E   4F   50
      0x61,0xE0,0x01,0xE0,0x01,0x05,0x06,0x00,0x01,0xC8,
    //  51   52   53   54   55   56   57   58   59   5A
      0x28,0x0F,0x50,0x3C,0x03,0x05,0x00,0x00,0x00,0x00,
    //  5B   5C   5D   5E   5F   60   61   62   63   64
      0x00,0x00,0x00,0x18,0x1A,0x1E,0x14,0xC5,0x25,0x0A,
    //  65   66   67   68   69   6A   6B   6C   6D   6E
      0xEA,0xEC,0xB5,0x06,0x00,0x00,0x00,0x20,0x21,0x11,
    //  6F   70   71   72   73   74   75   76   77   78
      0x00,0x01,0x00,0x0F,0x00,0x2A,0x64,0x32,0x19,0x50,
    //  79   7A   7B   7C   7D   7E   7F   80   81   82
      0x32,0xDC,0xFA,0x94,0x55,0x02,0x08,0x00,0x00,0x04,
    //  83   84   85   86   87   88   89   8A   8B   8C
      0x80,0xDE,0x00,0x80,0xE4,0x00,0x80,0xEA,0x00,0x7F,
    //  8D   8E   8F   90   91   92   93   94   95   96
      0xF0,0x00,0x7F,0xF6,0x00,0x7F,0x00,0x00,0x00,0x00,
    //  97   98   99   9A   9B   9C   9D   9E   9F   A0
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    //  A1   A2   A3   A4   A5   A6   A7   A8   A9   AA
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    //  AB   AC   AD   AE   AF   B0   B1   B2   B3   B4
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    //  B5   B6   B7   B8   B9   BA   BB   BC   BD   BE
      0x00,0x00,0x1C,0x1A,0x18,0x16,0x14,0x12,0x10,0x0E,
    //  BF   C0   C1   C2   C3   C4   C5   C6   C7   C8
      0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x00,0x00,0x00,
    //  C9   CA   CB   CC   CD   CE   CF   D0   D1   D2
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    //  D3   D4   D5   D6   D7   D8   D9   DA   DB   DC
      0x00,0x00,0x10,0x12,0x14,0x16,0x18,0x1A,0x1C,0x1E,
    //  DD   DE   DF   E0   E1   E2   E3   E4   E5   E6
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    //  E7   E8   E9   EA   EB   EC   ED   EE   EF   F0
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    //  F1   F2   F3   F4   F5   F6   F7   F8   F9   FA
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    //  FB   FC   FD   FE
      0x00,0x00,0x00,0x00,//0x24,0x00,
  };


  CTP_CFG_GT911[(0xB7-0x47) + 0 ] = 28;
  CTP_CFG_GT911[(0xB7-0x47) + 1 ] = 26;
  CTP_CFG_GT911[(0xB7-0x47) + 2 ] = 24;
  CTP_CFG_GT911[(0xB7-0x47) + 3 ] = 22;
  CTP_CFG_GT911[(0xB7-0x47) + 4 ] = 20;
  CTP_CFG_GT911[(0xB7-0x47) + 5 ] = 18;
  CTP_CFG_GT911[(0xB7-0x47) + 6 ] = 16;
  CTP_CFG_GT911[(0xB7-0x47) + 7 ] = 14;
  CTP_CFG_GT911[(0xB7-0x47) + 8 ] = 12;
  CTP_CFG_GT911[(0xB7-0x47) + 9 ] = 10;
  CTP_CFG_GT911[(0xB7-0x47) + 10] = 8;
  CTP_CFG_GT911[(0xB7-0x47) + 11] = 6;
  CTP_CFG_GT911[(0xB7-0x47) + 12] = 4;
  CTP_CFG_GT911[(0xB7-0x47) + 13] = 2;


  CTP_CFG_GT911[(0xD5-0x47) + 0 ] = 33;
  CTP_CFG_GT911[(0xD5-0x47) + 1 ] = 32;
  CTP_CFG_GT911[(0xD5-0x47) + 2 ] = 31;
  CTP_CFG_GT911[(0xD5-0x47) + 3 ] = 30;
  CTP_CFG_GT911[(0xD5-0x47) + 4 ] = 29;
  CTP_CFG_GT911[(0xD5-0x47) + 5 ] = 0;
  CTP_CFG_GT911[(0xD5-0x47) + 6 ] = 2;
  CTP_CFG_GT911[(0xD5-0x47) + 7 ] = 4;
  CTP_CFG_GT911[(0xD5-0x47) + 8 ] = 6;
  CTP_CFG_GT911[(0xD5-0x47) + 9 ] = 8;
  CTP_CFG_GT911[(0xD5-0x47) + 10] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 11] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 12] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 13] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 14] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 15] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 16] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 17] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 18] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 19] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 20] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 21] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 22] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 23] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 24] = 0xFF;
  CTP_CFG_GT911[(0xD5-0x47) + 25] = 0xFF;


  uint8_t check_sum = 0;

  check_sum = 0;
  for (int i=0; i<sizeof(CTP_CFG_GT911); i++)
  {
    check_sum += CTP_CFG_GT911[i];
  }
  check_sum = (~check_sum) + 1;

  uint8_t temp[2];
  temp[0] = check_sum;
  temp[1] = 1;

  writeRegs(0x8047, CTP_CFG_GT911, sizeof(CTP_CFG_GT911));
  writeRegs(0x80FF, temp, 2);
  return true;
}

bool readRegs(uint16_t reg_addr, void *p_data, uint32_t length)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;

  i2c_ret = HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(i2c_addr << 1), reg_addr, I2C_MEMADD_SIZE_16BIT, p_data, length, 10);
  if(i2c_ret == HAL_OK)
    ret = true;
  else
    ret = false;

  return ret;
}

bool writeRegs(uint16_t reg_addr, void *p_data, uint32_t length)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;


  i2c_ret = HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(i2c_addr << 1), reg_addr, I2C_MEMADD_SIZE_16BIT, p_data, length, 10);
  if(i2c_ret == HAL_OK)
    ret = true;
  else
    ret = false;

  return ret;
}

uint16_t gt911GetWidth(void)
{
  return GT911_TOUCH_WIDTH;
}

uint16_t gt911GetHeight(void)
{
  return GT911_TOUCH_HEIGTH;
}

bool gt911GetInfo(gt911_info_t *p_info)
{
  bool ret = false;
  uint8_t buf[14] = {0};
  uint8_t touch_cnt = 0;
  const uint16_t point_addr[] = GT911_POINTS_REG;


  p_info->count = 0;

  if (is_init == false)
  {
    return false;
  }

  if (HAL_GPIO_ReadPin(LCD_TP_INT_GPIO_Port, LCD_TP_INT_Pin) == GPIO_PIN_SET)
  {
    return true;
  }

  ret = readRegs(GT911_POINT_INFO, buf, 1);
  if (ret == true && buf[0] & (1<<7))
  {
    touch_cnt = buf[0] & 0x0F;
    buf[0] = 0;
    ret = writeRegs(GT911_POINT_INFO, buf, 1);
  }
  else
  {
    return ret;
  }

  p_info->count = touch_cnt;

  for (int i=0; i<touch_cnt; i++)
  {
    ret = readRegs(point_addr[i], buf, 8);
    if (ret == true)
    {
      p_info->point[i].id     = buf[0];
      p_info->point[i].event  = 0;
      p_info->point[i].weight = 0;
      p_info->point[i].area   = (buf[6]<<8) | (buf[5]<<0);

      p_info->point[i].x = (buf[2]<<8) | (buf[1]<<0);
      p_info->point[i].y = (buf[4]<<8) | (buf[3]<<0);
    }
    else
    {
      break;
    }
  }

  return ret;
}
