/**
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/***********************************/
/*     VL53L4CD ULD calibration    */
/***********************************/
/*
* This example shows how to calibrate the sensor (offset + xtalk). It
* initializes the VL53L4CD ULD, perform calibrations, and starts a ranging to
* capture 200 frames.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vl53l4cd_api.h"
#include "vl53l4cd_calibration.h"

int example5(void)
{

	/*********************************/
	/*   VL53L4CD ranging variables  */
	/*********************************/

	Dev_t 					dev;
	uint8_t 				status, loop, isReady;
	uint16_t 				sensor_id;
	uint16_t 				xtalk_kcps;
	int16_t 				offset_mm;
	VL53L4CD_ResultsData_t 		results;		/* results data from VL53L4CD */


	/*********************************/
	/*      Customer platform        */
	/*********************************/

	/* Default VL53L4CD I2C address */
	dev = 0x52;

	/* (Optional) Change I2C address */
	// status = VL53L4CD_SetI2CAddress(dev, 0x20);
	// dev = 0x20;


	/*********************************/
	/*   Power on sensor and init    */
	/*********************************/

	/* (Optional) Check if there is a VL53L4CD sensor connected */
	status = VL53L4CD_GetSensorId(dev, &sensor_id);
	if(status || (sensor_id != 0xEBAA))
	{
		printf("VL53L4CD not detected at requested address\n");
		return status;
	}

	/* (Mandatory) Init VL53L4CD sensor */
	status = VL53L4CD_SensorInit(dev);
	if(status)
	{
		printf("VL53L4CD ULD Loading failed\n");
		return status;
	}

	printf("VL53L4CD ULD ready !\n");

	/*********************************/
	/*      Offset calibration       */
	/*********************************/

	/* Hardware required : Place a Grey target at 100mm from the sensor. */

	/* For this example, 20 samples are used for the calibration */
	status = VL53L4CD_CalibrateOffset(dev, 100, &offset_mm, 20);
	if(status)
	{
		printf("VL53L4CD_CalibrateOffset failed with status %u\n", status);
		return status;
	}

	printf("Offset calibration done, offset value = %d mm\n", offset_mm);

	/* Offset value is lost when the sensor is powered down. User can call the
	 * function below to set a new offset value after power up (sensor needs to
	 * be initialized first).
	 */
	// status =  VL53L4CD_SetOffset(dev, offset_mm);


	/*********************************/
	/*      Xtalk calibration       */
	/*********************************/

	/* Hardware required : Place a protective coverglass at the top of the
	 * sensor. Then, find the maximum ranging distance without under-ranging
	 * (please refer to the User Manual for more information) */

	/* For this example, 20 samples are used for the calibration. The
	 * calibration distance is set at 500mm for the example, but it depends of
	 * the coverglass quality.
	 */

	status = VL53L4CD_CalibrateXtalk(dev, 500, &xtalk_kcps, 20);
	if(status)
	{
		printf("VL53L4CD_CalibrateXtalk failed with status %u\n", status);
		return status;
	}

	printf("Xtalk calibration done, Xtalk value = %u kcps\n", xtalk_kcps);

	/* Xtalk value is lost when the sensor is powered down. User can call the
	 * function below to set a new xtalk value after power up (sensor needs to
	 * be initialized first).
	 */
	// status =  VL53L4CD_SetXtalk(dev, xtalk_kcps);


	/*********************************/
	/*         Ranging loop          */
	/*********************************/

	status = VL53L4CD_StartRanging(dev);

	loop = 0;
	while(loop < 200)
	{
		/* Use polling function to know when a new measurement is ready.
		 * Another way can be to wait for HW interrupt raised on PIN 7
		 * (GPIO 1) when a new measurement is ready */
 
		status = VL53L4CD_CheckForDataReady(dev, &isReady);

		if(isReady)
		{
			/* (Mandatory) Clear HW interrupt to restart measurements */
			VL53L4CD_ClearInterrupt(dev);

			/* Read measured distance. RangeStatus = 0 means valid data */
			VL53L4CD_GetResult(dev, &results);
			printf("Status = %6u, Distance = %6u, Signal = %6u\n",
				 results.range_status,
				 results.distance_mm,
				 results.signal_per_spad_kcps);
			loop++;
		}

		/* Wait a few ms to avoid too high polling (function in platform
		 * file, not in API) */
		VL53L4CD_WaitMs(dev, 5);
	}

	status = VL53L4CD_StopRanging(dev);
	printf("End of ULD demo\n");
	return status;
}
