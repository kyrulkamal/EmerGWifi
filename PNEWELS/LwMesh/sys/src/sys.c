/**
 * Main system routine
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include "sysConfig.h"
#include "phy.h"
#include "nwk.h"
#include "hal.h"
#include "sys.h"
#include "sysTimer.h"

/*- Implementations --------------------------------------------------------*/

/// <summary>
/// Initialize system driver.
/// </summary>
void SYS_Init(void)
{
  HAL_Init();
  SYS_TimerInit();
  PHY_Init();
  NWK_Init();
  PHY_TX_PWR = 0x00; //This value for maximum TX power. This enable better range of TX at the cost of higher power consumption.
}

/// <summary>
/// System service. STRICTLY DO NOT REMOVE OR MODIFY.
/// </summary>
void SYS_TaskHandler(void)
{
  PHY_TaskHandler();
  NWK_TaskHandler();
  SYS_TimerTaskHandler();
}
