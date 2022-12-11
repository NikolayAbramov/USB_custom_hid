/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
#define OUT_REPORT_ID 10
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */
	0x05, 0x01,                    	// USAGE_PAGE (Generic Desktop)
	0x09, 0x08,                    	// USAGE (Undefined)
	0xa1, 0x01,                    	// COLLECTION (Application)
		                                   // 6
	0x85, 0x01,                    	//   REPORT_ID (1)
	0x05, 0x0a,                    	//   USAGE_PAGE (Ordinals)
	0x19, 1,                    	//   USAGE_MINIMUM (Instance 1)
	0x29, 64,                    	//   USAGE_MAXIMUM (Instance 64)
	0x15, 0,                    	//   LOGICAL_MINIMUM (0)
	0x25, 1,	              		//   LOGICAL_MAXIMUM (1)
	0x89, 10,                    	//   STRING_MINIMUM(10)
	0x99, 73,                    	//   STRING_MAXIMUM(73)
	0x75, 1,                    	//   REPORT_SIZE (1)
	0x95, 64,                    	//   REPORT_COUNT (64)
	0x81, 0xa2,                    // INPUT (Data,Var,Abs,NPrf,Vol)

	//0x17, 0x00, 0x00, 0x00, 0x80,  	// LOGICAL_MINIMUM (0)
	//0x27, 0xff, 0xff, 0xff, 0x7f,  	// LOGICAL_MAXIMUM (0xFF FF FF FF)
	//hidcomp doesn't like a proper 32bit ints. I don't know why.
	//I have to truncate to 16 bits
	0x17, 0x00, 0x00, 0x00, 0x00,  	// LOGICAL_MINIMUM (0)
	0x27, 0xff, 0xff, 0x00, 0x00,  	// LOGICAL_MAXIMUM (0xFF FF FF FF)
	0x89, 202,                    	// STRING_MINIMUM (202)
	0x99, 202,                    	// STRING_MAXIMUM (203)
	0x75, 16,                    	// REPORT_SIZE (32)
	0x95, 2,                    	// REPORT_COUNT (2)
	0x81, 0xa2,                   	//   INPUT (Data,Var,Abs,NPrf,Vol)

	0x85, OUT_REPORT_ID,                    //   REPORT_ID
	0x05, 0x09,                    //   USAGE_PAGE (Button)
	0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
	0x29, 16,       				//   USAGE_MAXIMUM (Button 16)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	0x89, 74,                    //   STRING_MINIMUM (74)
	0x99, 89,                    //   STRING_MAXIMUM (89)
	0x75, 0x01,                    //   REPORT_SIZE (1)
	0x95, 16,                    //   REPORT_COUNT (16)
	0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,NPrf)

  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
static uint8_t  *USBD_CUSTOM_HID_GetUsrStrDesc (USBD_HandleTypeDef *pdev ,uint8_t index,  uint16_t *length);

uint8_t OutReportData[OUT_REPORT_SIZE]={0};
/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
	USBD_CUSTOM_HID.GetUsrStrDescriptor = USBD_CUSTOM_HID_GetUsrStrDesc;
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
	 switch (event_idx) {
	  case OUT_REPORT_ID:
	    memcpy( OutReportData, &((USBD_CUSTOM_HID_HandleTypeDef *)hUsbDeviceFS.pClassData)->Report_buf[1], OUT_REPORT_SIZE );
	    break;
	  }
  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */

int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}

/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
static uint8_t  *USBD_CUSTOM_HID_GetUsrStrDesc(USBD_HandleTypeDef *pdev ,uint8_t index,  uint16_t *length) {

  static char buf[USBD_MAX_STR_DESC_SIZ/2];
  static uint8_t unicode_buf[USBD_MAX_STR_DESC_SIZ];

  if(index >= 10 && index <= 73)
    sprintf(buf, "input-%02i", index-10);
  else if(index >= 74 && index <= 89)
    sprintf(buf, "output-%02i", index-74);
  else if(index == 202)
    strcpy(buf, "mpg-0");
  else if(index == 203)
    strcpy(buf, "mpg-1");
  else
    sprintf(buf, "unnamed-%02i", index);
  USBD_GetString ((uint8_t*)buf, unicode_buf, length);
  return unicode_buf;
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

