
#ifdef __cplusplus
extern "C" {
#endif

//#include "stm32f1xx_hal.h"
#include "DMS.h"
#include "stdbool.h"

uint32_t threshhold = 3000;

uint32_t ADC_Poll2(ADC_HandleTypeDef *hadc)
{

	HAL_ADC_PollForConversion(hadc, 100);
    int result = HAL_ADC_GetValue(hadc);
    return result;
}





bool Check_Threshhold(uint32_t Value){
	if (Value > threshhold){
		return false; //no person detected
	}
	else{
		return true; //person detected
	}
}
