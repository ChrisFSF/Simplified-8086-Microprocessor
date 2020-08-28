#include <project_config.hpp>
#include <stdint.h>

#include <cstdint>
#include <iterator>


#include "L2_HAL/displays/led/onboard_led.hpp"
#include "L2_HAL/switches/button.hpp"
#include "L0_LowLevel/interrupt.hpp"
//#include "utility/log.hpp"
#include "utility/time.hpp"
//#include "L1_Drivers/gpio.hpp"

#include "bus.h"

	Bus RW;

	void PortISR()
	{	
	  RW.ReadKP();
	  LOG_INFO("Pin0_15_ISR");
	}

int main(void)
{

	OnBoardLed leds;

	 leds.Initialize();
	 OledTerminal OledT;
	 OledT.Initialize();

	LOG_INFO("Button application starting...");
	Button button0(1, 19);
	Button button1(1, 15);
	Button button2(0, 30);
	Button button3(0, 29);

	button0.Initialize();
	button1.Initialize();
	button2.Initialize();
	button3.Initialize();
	

	RW.Int.GetPin().SetMode(Pin::Mode::kPullUp);
  	RW.Int.AttachInterrupt(&PortISR, GpioInterface::Edge::kEdgeRising);
  	RW.Int.EnableInterrupts();
	
// LCD Function:
// 	LCD(char data[], int size) overload function , size can be customized
// 	LCD(char data[])// only string input
// 	LCD_CursorShiftRight()
// 	LCD_CursorShiftLeft()
// 	LCD_CursorHome()
// 	LED_SetCursorRow()
// 	LCD_SetCursorBlink(int i)// 0 blink, 1 stop blinking
// 	LCD_Clear()
// 	LCD_Shift(int i) 0 stop shifting, 1 shifting
// 	LCD_NumofRow(int row) 0, one row, 1, two rows
// 	LCD_CmdWrite(uint8_t cmd, double delay) customized cmd with delay

    uint8_t value, add, data;
	OledT.printf("kkInitlize Start!!!\n");
 	RW.LCD_Initlize();
	OledT.printf("kkInitlize END!!!\n");

 while (true){
		if(button0.Pressed()){
			leds.Toggle(0);
		// uint8_t	add = 0x9c;//0b0111 0101
		// uint8_t	data = 0x58;//0b1001 1000

		// 	RW.Write(Bus::ControlType::kMemory,add ,data );
			// RW.LCD_Write("Hello");
			RW.ReadData();
		//OledT.printf("Reading Data!!!\n");
			
		}
		if (button1.Pressed())
		{
			
			leds.Toggle(1);
		// uint8_t	add = 0x9c; //0b01110100
		// RW.Read(Bus::ControlType::kMemory, add);
			RW.reset();
			OledT.printf("Clear LCD!!!\n");
		}
		if (button2.Pressed())
		{
			leds.Toggle(2);
			// add = 0x9c;//0b0111 0101
			// data = 0x58;//0b1001 1000

			// RW.Write(Bus::ControlType::kMemory,add ,data );
			// OledT.printf("\nAddress:\n");
		 //    OledT.printf("0x%x \n",add);
			// OledT.printf("Data Reading:\n");
		 //   OledT.printf("0x%x \n",data);
			RW.LCD_CursorShiftLeft();
			OledT.printf("Cursor left\n");
			
		}
		if (button3.Pressed())
		{
			leds.Toggle(3);
			// add = 0x9c; //0b01110100
			// value = RW.Read(Bus::ControlType::kMemory, add);
			// OledT.printf("Data: %x\n", value);
			RW.LCD_CursorShiftRight();
			OledT.printf("Cursor right\n");
			
		}
		RW.testHigh();
	 }

	                 
	return 0;
}
