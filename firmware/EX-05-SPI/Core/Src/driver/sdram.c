#include "sdram.h"
#include "fmc.h"





bool sdramInit(void)
{
  FMC_SDRAM_CommandTypeDef Command;
  uint32_t SDRAM_TIMEOUT = 0xFFFF;


  // Step 1: Configure a clock configuration enable command
  //
  Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;
  if(HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT) != HAL_OK)
  {
    Error_Handler( );
  }


  // Step 2: Insert 100 us minimum delay */
  // Inserted delay is equal to 1 ms due to systick time base unit (ms)
  //
  HAL_Delay(1);



  // Step 3: Configure a PALL (precharge all) command
  //
  Command.CommandMode            = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;
  if(HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT) != HAL_OK)
  {
    Error_Handler( );
  }


  // Step 4: Configure a Refresh command
  //
  Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 8;
  Command.ModeRegisterDefinition = 0;
  if(HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT) != HAL_OK)
  {
    Error_Handler( );
  }


  // Step 5: Program the external memory mode register
  //
  uint32_t mode_reg = 0;

  mode_reg |= (0 << 0); // Burst Length = 1
  mode_reg |= (0 << 3); // Burst Type
                        //   0 : Sequential
                        //   1 : Interleaved
  mode_reg |= (2 << 4); // CAS Latency Mode
                        //   2 :
                        //   3 :
  mode_reg |= (0 << 7); // Operation Mode
  mode_reg |= (1 << 9); // Write Burst Mode
                        //   0 : Programmed Burst Length
                        //   1 : Single Location Access

  Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = mode_reg;
  if(HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT) != HAL_OK)
  {
    Error_Handler( );
  }


  // Step 6: Set the refresh rate counter
  //
  // refresh rate = (COUNT + 1) * SDRAM clock freq (91.6666Mhz)
  //
  // COUNT = (SDRAM refresh period/Number of rows) - 20
  //       = (64ms / 8192) - 20
  //       = 7.81us * 91.666 - 20 = 695
  uint32_t refresh_count = 695;

  if(HAL_SDRAM_ProgramRefreshRate(&hsdram1, refresh_count) != HAL_OK)
  {
    Error_Handler( );
  }

  return true;
}
