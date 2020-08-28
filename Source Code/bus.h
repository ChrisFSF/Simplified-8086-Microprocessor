#include <stdint.h>
#include "L1_Drivers/gpio.hpp"
#include "utility/log.hpp"
#include "L3_Application/oled_terminal.hpp"
#include <stdio.h>
#include<ctype.h>
#include <string.h> 
#include <stdlib.h> 
class Bus
{
public:
	enum class ControlType {
		kMemory = 0,
		kIO
	};
	Bus() {
		ad[0].GetPin().SetAsOpenDrain();
		ad[1].GetPin().SetAsOpenDrain();
		ad[2].GetPin().SetAsOpenDrain();
		ad[3].GetPin().SetAsOpenDrain();
		ad[4].GetPin().SetAsOpenDrain();
		ad[5].GetPin().SetAsOpenDrain();
		ad[6].GetPin().SetAsOpenDrain();
		ad[7].GetPin().SetAsOpenDrain();
		ALE.SetAsOutput();
		WRITE.SetAsOutput();
		M_IO.SetAsOutput();

		//initialization

	}
void Write(ControlType control, uint8_t address, uint8_t data) {

	   oled.Clear();
		oled.printf("Writing Data...\n");

		ALE.SetAsOutput();
		WRITE.SetAsOutput();
		M_IO.SetAsOutput();

		ad[0].SetAsOutput();
		ad[1].SetAsOutput();
		ad[2].SetAsOutput();
		ad[3].SetAsOutput();
		ad[4].SetAsOutput();
		ad[5].SetAsOutput();
		ad[6].SetAsOutput();
		ad[7].SetAsOutput();

		WRITE.SetHigh();

		ALE.SetHigh();
		//address for loop
		for (int i = 0; i < 8; i++) {
			if (address & (1 << i))//shifting from right to left to check each digit of address
				ad[i].SetHigh();
			else
				ad[i].SetLow();
		}
		ALE.SetLow();

		oled.printf("\nAddress:\n");
		oled.printf("0x%x \n",address);
		//Delay(10);//make delay
		
		if (control == ControlType::kMemory)
			M_IO.SetHigh();
		else
			M_IO.SetLow();

		//data for loop
		for (int i = 0; i < 8; i++) {
			if (data & (1 << i))//shifting from right to left to check each digit of address
				ad[i].SetHigh();
			else
				ad[i].SetLow();
		}

		oled.printf("Data Writing:\n");
		oled.printf("0x%x \n",data);

		M_IO.SetLow();

		oled.printf("\nFinish Writing!\n");
	}

uint8_t Read(ControlType control, uint8_t address){

	oled.Clear();
		oled.printf("Reading Data...\n");

		ALE.SetAsOutput();
		WRITE.SetAsOutput();
		M_IO.SetAsOutput();

		ad[0].SetAsOutput();
		ad[1].SetAsOutput();
		ad[2].SetAsOutput();
		ad[3].SetAsOutput();
		ad[4].SetAsOutput();
		ad[5].SetAsOutput();
		ad[6].SetAsOutput();
		ad[7].SetAsOutput();

		WRITE.SetHigh();

		ALE.SetHigh();
		//address for loop
		for (int i = 0; i < 8; i++) {
			if (address & (1 << i))//shifting from right to left to check each digit of address
				ad[i].SetHigh();
			else
				ad[i].SetLow();
		}
		ALE.SetLow();
		
		oled.printf("\nAddress:\n");
		oled.printf("0x%x \n",address);
		

		WRITE.SetLow();

		ad[0].SetAsInput();
		ad[1].SetAsInput();
		ad[2].SetAsInput();
		ad[3].SetAsInput();
		ad[4].SetAsInput();
		ad[5].SetAsInput();
		ad[6].SetAsInput();
		ad[7].SetAsInput();

		if (control == ControlType::kMemory)
			M_IO.SetHigh();
		else
			M_IO.SetLow();


		uint8_t temp = 0;
		for (uint8_t i = 0; i < 8; i++) {
			temp = temp | (ad[i].Read() << i);
		}
		M_IO.SetLow();
		
		oled.printf("Data Reading:\n");
		oled.printf("0x%x \n",temp);

		oled.printf("\nFinished Reading\n");
		return temp;
	}

void testHigh(){
   		
        ad[0].SetAsOutput();
        ad[1].SetAsOutput();
        ad[2].SetAsOutput();
        ad[3].SetAsOutput();
        

        WRITE.SetHigh();

        uint8_t address = 0x5F;  
        for (int i = 0; i < 4; i++) {       
                ad[i].SetHigh();
        }

    }

 uint8_t ReadKP(){

		M_IO.SetLow();//Chosing IO, IO signal will never change
		WRITE.SetLow();//initialize 74ls74 Pre
		uint8_t data = 0;
		uint8_t address = 0x55;
		uint8_t temp = 0;
		uint8_t row = 8;

	while (temp == 0){
		row = row * 2;
		if(row > 15)
			row = 1;
		
//initialize AD pin
		//oled.printf("row:0x%x\n", row);
///////////////////////////////////////////////
		ad[0].SetAsOutput();
		ad[1].SetAsOutput();
		ad[2].SetAsOutput();
		ad[3].SetAsOutput();
		ad[4].SetAsOutput();
		ad[5].SetAsOutput();
		ad[6].SetAsOutput();
		ad[7].SetAsOutput();

		
		 WRITE.SetHigh();
///////////////////Start Writing Add//////////////////////////
	 	//address for loop1
		 ALE.SetHigh();
		for (int i = 0; i < 8; i++) {
			if (address & (1 << i))//shifting from right to left to check each digit of address
				ad[i].SetHigh();
			else
				ad[i].SetLow();
		}
		//Delay(0.1);
		 ALE.SetLow();	

//writing rows
		 for (int i = 0; i < 8; i++) {
			if (row & (1 << i))//shifting from right to left to check each digit of address
				ad[i].SetHigh();
			else
				ad[i].SetLow();
		}
		//Writing Row 

		//address for loop2
		ALE.SetHigh();
		for (int i = 0; i < 8; i++) {
			if (address & (1 << i))//shifting from right to left to check each digit of address
				ad[i].SetHigh();
			else
				ad[i].SetLow();
		}
		
		ALE.SetLow();	

	
/////////////////////////Start Reading//////////////////////////////////
		WRITE.SetLow();//reading, RD is high
		//reading the keypad from 373
		ad[0].SetAsInput();
		ad[1].SetAsInput();
		ad[2].SetAsInput();
		ad[3].SetAsInput();
		ad[4].SetAsInput();
		ad[5].SetAsInput();
		ad[6].SetAsInput();
		ad[7].SetAsInput();
		
		//M_IO.SetLow();
		
	for (uint8_t i = 0; i < 4; i++) {
			temp = temp | (ad[i].Read() << i); //data , row 
		}

		//oled.printf("temp:0x%x\n", temp);
	}
	//oled.printf("row:0x%x\n", row);
	data = temp | (row <<4); //temp + row == 0001 0001 -> #1 on the kp
	//oled.printf("data:0x%x\n", data);
	//oled.printf("Kp: %c\n",data_decode(data));

	//oled.printf("count:  %d\n", count);
	char ch;
	ch  = data_decode(data);
	//oled.printf("%c \n", ch);
	if(ch == '*'){
		reset();
		//oled.printf("rest");
	}
	else if(ch == 'A' || ch == 'B' || ch == 'C' ||ch == 'D' || ch == '#' ){
		ch = operation(ch);
		if(ch == '=' ){
		print_result();	
		}
	}
	//be carefull, the C will output * as well, which will also triger clear()
	if(ch != '*' && ch != '='){
		if (ch == '$')
			ch = '*' ;
		if(!isdigit(ch) && !isdigit(result_str[count])){
			print_err();
		}
		else{
			count = count + 1;
	 	write_digit(ch);
	 	print_cal();
		}
	}
	// else if(ch == '='){
	// 	print_result();
	// }
	 
    //LCD_DataWrite(data_decode(data));
	return data;
	
}
void print_err(){
 LCD_Initlize();
 cal_clear();
 LCD_Write("Error!");

}

void print_result(){
	// int temp;
	// oled.printf("before: %s\n",result_str);
    char data[count+1];
    int j;
	for (j = 0; j <= count; j++)
		 data[j] = result_str[j]; 

	infixToPostfix(data);
	// oled.printf("after: %s\n",result_str);
	int temp = evaluatePostfix(data);
	// oled.printf("result: %d\n",temp);
	int dig=1;
	if(temp >9)
		dig = 2;

	char ch[dig];

	if (dig == 1) 
	 ch[0] = DecToChar(temp);
	else{
		ch[0] = DecToChar(temp/10); 
		ch[1] = DecToChar(temp%10);

	}
	LCD_Initlize();
	LCD_Clear();
	LCD_DisplayOnOffControl(0x0C);//1DBC 1100

	// snprintf(ch, , "%f", temp);

	LCD_CursorBegin(16-dig, 1);
	LCD_Write(ch);
	// LCD_DataWrite();
	
	LCD_DisplayOnOffControl(0x0E);
	//cursor home
	LCD_NumofRow(1);// set two-row display
	LCD_CmdWrite(0x00,1);//Function set
	LCD_CmdWrite(0x02,1);// 0000 NFXX---> N = 0; 1-line display 10A 11B 12C 13D 14E 15F
	//LCD_CursorBegin(16,1);
	for (int i = 0; i <= count; i++){
		LCD_DataWrite(result_str[i]);
	}

	//store in the Sram
	Cal_store(temp);//store data int sram

}		

uint8_t Cal_store(int data){
	// uint8_t add = checklocation();//fetcht the first available location
	uint8_t add = 0x9c;
	Write(ControlType::kMemory,add, data);
	//oled.printf("Address: %x\n",loc);
    //oled.printf("Result: %x\n",data);
	
    return loc;//return the location that data is stored
}
void ReadData(){
  uint8_t temp;
  uint8_t add = 0x9c;
  temp = Read(ControlType::kMemory, add);
 // oled.Clear();
 // oled.printf("Address: %x\n",add);
 //oled.printf("Data Read: %x\n",temp);
}

uint8_t checklocation(){
	uint8_t data = 0;
	uint8_t temp = -1;
	while (temp != 0 ){
	  loc++;	
	  Write(ControlType::kMemory, loc, data);
	  temp = Read(ControlType::kMemory, loc);
	}
	return loc;
}

void write_digit(char dat){//update new element expect * and #
	result_str[count] = dat;
	// count++;
}

void print_cal(){//print current display
	// LCD_Clear();
	 LCD_Initlize();
	 cal_clear();
	 for (int i = 0; i <= count; i++){
		LCD_DataWrite(result_str[i]);
	}
	// oled.printf("count: %d\n",count);
	// oled.printf("LCD rst: ");
	// for (int j = 0; j <=count; j++){
	// 	oled.printf("%c ", result_str[j]);
	// }
}
//calculator
void cal_clear(){// clear the calculator
	LCD_Clear();
	LCD_DisplayOnOffControl(0x0C);//1DBC 1100
	LCD_CursorBegin(15, 1);
	LCD_DataWrite('0');
	LCD_DisplayOnOffControl(0x0E);
	//cursor home
	LCD_NumofRow(1);// set two-row display
	LCD_CmdWrite(0x00,1);//Function set
	LCD_CmdWrite(0x02,1);// 0000 NFXX---> N = 0; 1-line display 10A 11B 12C 13D 14E 15F
	//LCD_CursorBegin(16,1);
}

void reset(){//deep reset the array and display 
	LCD_Initlize();
	cal_clear();
	count = -1;
	int i = 0;
	while(result_str[i] != '\0'){
		result_str[i] = '\0';
		i++;
	}
}

char data_decode(uint8_t data){
		char val = '-';
	if (data == 0x11)
		val = '1';
	else if (data == 0x12)
		val = '2';
	else if (data == 0x14)
		val = '3';
	else if (data == 0x18)
		val = 'A';
	else if (data == 0x21)
		val = '4';
	else if (data == 0x22)
		val = '5';
	else if (data == 0x24)
		val = '6';
	else if (data == 0x28)
		val = 'B';
	else if (data == 0x41)
		val = '7';
	else if (data == 0x42)
		val = '8';
	else if (data == 0x44)
		val = '9';
	else if (data == 0x48)
		val = 'C';
	else if (data == 0x81)
		val = '*';
	else if (data == 0x82)
		val = '0';
	else if (data == 0x84)
		val = '#';
	else if (data == 0x88)
		val = 'D';
	else val = '-';
	return val;
	}

void LCD_Initlize(){

		ad[0].SetAsOutput();
		ad[1].SetAsOutput();
		ad[2].SetAsOutput();
		ad[3].SetAsOutput();
		ad[4].SetAsOutput();
		ad[5].SetAsOutput();
		ad[6].SetAsOutput();
		ad[7].SetAsOutput();

		WRITE.SetHigh();

		//A7 = not used, A6 = EN, A5 = RS, A4 = R/W, A3-A0 data 
	    									 //->D7, D6, D5, D4
	    //R/W 0 Write, 1, Read
	    //RS 0 CMD, 1 DATA
	    uint8_t address = 0x5F;//0101 1111
	   // uint8_t data = 0x00; //0 0 00 0011

	    M_IO.SetLow();//Chosing IO

		//address for loop
		//step 1  Power on
		ALE.SetHigh();
		for (int i = 0; i < 8; i++) 
		{
			if (address & (1 << i))//shifting from right to left to check each digit of address
				ad[i].SetHigh();
			else
				ad[i].SetLow();
		}
		ALE.SetLow();

		for (int i = 0; i < 8; i++) {
					ad[i].SetLow();
			}
		Delay(1);//need to check the delay of the full decoded

		//set low to all input before starting the initialization
	
		// 1. Display clear
		// 2. Function set: 
		//    DL = 1; 8-bit interface data 
		//    N = 0; 1-line display 
		//    F = 0; 5x8 dot character font 
		// 3. Display on/off control: 
		//    D = 0; Display off 
		//    C = 0; Cursor off 
		//    B = 0; Blinking off 
		// 4. Entry mode set: 
		//    I/D = 1; Increment by 1 
		//    S = 0; No shift 

		//////////////Beginning of Initialization//////////////////
		//step 2  Function set
		LCD_CmdWrite(0x03,5);
		//step 3  Function set
		LCD_CmdWrite(0x03,0.1);
		//step 4  Function set , reset
		LCD_CmdWrite(0x03,0.1);
		//step 5
		LCD_CmdWrite(0x02,0.1);//real Function set

		//step 6 Function set rows	

		LCD_NumofRow(1);// set two-row display

		// LCD_CmdWrite(0x02,0.1);//Function set
		// LCD_CmdWrite(0x00,1);// 0000 NFXX---> N = 0; 1-line display 
		// 							   //---> F = 0; 5x8 dot character font 

		//step 7 Display on/off control
		LCD_DisplayOnOffControl(0x0E);// 0000 0DCB
		// Display (D), the Cursor (C), and the Blinking (B) 
		// D = 0; Display off , C = 0; Cursor off, B = 0; Blinking off 
		// 1111

		//step 8 Display Clear
		//LCD_Clear();
		cal_clear();
		//step 8 Entry Mode Set
		LCD_EntryModeSet(0x06);// 0000 01 I/D S 0110
								//	I/D = 1; Increment by 1 
								//  S = 0; No shift 
		//////////////END of Initialization//////////////////
}


void LCD_Write(char data[], int size){	
		// oled.Clear();
		// oled.printf("LCD Write...\n");

		int ind = 0;
		while (ind < size){
		LCD_DataWrite(data[ind]);
		ind++;
	}
}

void LCD_Write(char data[]){
		// oled.Clear();
		// oled.printf("LCD Write...\n");

		int ind = 0;
		while (ind < LCD_CountChar(data)){
		LCD_DataWrite(data[ind]);
		ind++;
	}
}

//need too be tested
void LCD_WriteTwoLine(char data[]){
		// oled.Clear();
		// oled.printf("LCD Two Lines...\n");

		int ind = 0;
		int num = LCD_CountChar(data);
		if (num <= 16){
			while (ind <= num){
			LCD_DataWrite(data[ind]);
			ind++;
			}
		}
		else if(num >16 && num<32){
			LCD_NumofRow(1);//set two rows
			for (int i = 0; i <=15; i++){
			LCD_DataWrite(data[i]);
			ind++;
			}
			//jump to next line
			LED_SetCursorToSecondRow();
			for(int j = 16; j < num; j++){
			LCD_DataWrite(data[j]);
			}
		}
		else{
			LCD_WriteTwoLine("Error: Exceed lines!");
		}
}

int LCD_CountChar(char arr[]){//count the number of the char array
	int i = 0;
	while(arr[i] != '\0'){
		i++;
	}
	return i;
}

//need to be tested
void LCD_CursorBegin(int col, int row){//set cursor's begin 
		// oled.Clear();
		// oled.printf("LCD col: %i, row: %i\n", col, row);
	//set row
	//LCD_NumofRow(1);//set two rows
	if(row == 1) {
		LED_SetCursorToSecondRow();
	}
	
	for (int i = col; i >0; i--){
		LCD_CursorShiftRight();
	}

}

void LCD_DisplayOnOffControl(uint8_t cmd){
		
		LCD_CmdWrite(0x00,0.1);
		LCD_CmdWrite(cmd,1);// DCB0 

		// Display (D), the Cursor (C), and the Blinking (B) 
		// D = 0; Display off , C = 0; Cursor off, B = 0; Blinking off 
}

void LCD_EntryModeSet(uint8_t cmd){

		LCD_CmdWrite(0x00,1);
		LCD_CmdWrite(cmd,1);// 0000 01 I/D S 0110
							//	I/D = 1; Increment by 1 
							//  S = 0; No shift 
}

void LCD_CursorShiftRight(){
	// oled.Clear();
	// oled.printf("Cursor Right\n");
	LCD_CmdWrite(0x01,1);//Function set
	LCD_CmdWrite(0x04,1);// 0000 NFXX---> N = 0; 1-line display 10A 11B 12C 13D 14E 15F

}

void LCD_CursorShiftLeft(){
	// oled.Clear();
	// oled.printf("Cursor Left\n");
	LCD_CmdWrite(0x01,1);//Function set
	LCD_CmdWrite(0x00,1);// 0000 NFXX---> N = 0; 1-line display 10A 11B 12C 13D 14E 15F

}

void LCD_CursorHome(){
	// oled.Clear();
	// oled.printf("Cursor Home\n");
	LCD_CmdWrite(0x00,1);//Function set
	LCD_CmdWrite(0x02,1);// 0000 NFXX---> N = 0; 1-line display 10A 11B 12C 13D 14E 15F
}

void LED_SetCursorToSecondRow(){//set cursor to second row 
	// oled.Clear();
	// oled.printf("Cursor Down\n");
	LCD_CmdWrite(0x0C,1);//Function set
	LCD_CmdWrite(0x00,1);// 0000 NFXX---> N = 0; 1-line display 10A 11B 12C 13D 14E 15F

}

void LCD_SetCursorBlink(int i){
	// oled.Clear();
	// oled.printf("Cursor Blink\n");
	uint8_t choice;
	uint8_t CursorBlink = 0x0F;
	uint8_t CursorNotBlink = 0x0E;
	choice = i ? CursorBlink : CursorNotBlink;

	LCD_CmdWrite(0x00,0.1);
	LCD_CmdWrite(choice,1);// DCB0 
	// Display (D), the Cursor (C), and the Blinking (B) 
	// D = 0; Display off , C = 0; Cursor off, B = 0; Blinking off 
}

void LCD_Clear(){
	// oled.Clear();
	// oled.printf("LCD clear\n");
	LCD_CmdWrite(0x00,1);//clear
	LCD_CmdWrite(0x01,1);//clear
}

void LCD_Shift(int i){

	uint8_t choice;
	choice = i ? 0x06 : 0x07; 
	//step 8 Entry Mode Set
	LCD_CmdWrite(0x00,1);
	LCD_CmdWrite(choice,1);// 0000 01 I/D S 0110
						//	I/D = 1; Increment by 1 
						//  S = 0; No shift
}

void LCD_NumofRow(int row){

	uint8_t choice;
	choice = row ?0x08 : 0x00; 
	//step 6 Function set begin	
	LCD_CmdWrite(0x02,0.1);//Function set
	LCD_CmdWrite(choice,1);// 0000 NFXX---> N = 0; 1-line display, 1 ---> 2-line display
								   //---> F = 0; 5x8 dot character font 
}
	
void LCD_CmdWrite(uint8_t cmd, double delay){
	for (int i = 0; i < 6; i++) {//7 lines are valid
			if (cmd & (1 << i))
				ad[i].SetHigh();
			else
				ad[i].SetLow();
			}//end of i
			
			ad[6].SetHigh();
			ad[6].SetLow();
			Delay(delay);

}

char DecToChar(int num){
	char ch;
	switch(num){
	case 0:
		ch = '0';
		break;

	case 1:
		ch = '1';
		break;
	case 2:
		ch = '2';
		break;
	case 3:
		ch = '3';
		break;
	case 4:
		ch = '4';
		break;
	case 5:
		ch = '5';
		break;
	case 6:
		ch = '6';
		break;
	case 7:
		ch = '7';
		break;
	case 8:
		ch = '8';
		break;
	case 9:
		ch = '9';
		break;
	default:
			ch = '-';
			break;
	}
	return ch;
}

void LCD_DataWrite(char word){ 
	//A7 = not used, A6 = EN, A5 = RS, A4 = R/W, A3-A0 data 
	    									 //->D7, D6, D5, D4
	    //R/W 0 Write, 1, Read
	    //RS 0 CMD, 1 DATA
	//Because the char type converts to 8 bits data follows the ASCII table, 
	//they represent the higher and lower bit respectively
	//uint8_t temp = word;
	ad[4].SetLow(); // writing
	ad[5].SetHigh();//select data reg

	for (int i = 0; i < 4; i++) {//7 lines are valid
			if (word & (1 << i+4))//higher 4 bits
				ad[i].SetHigh();
			else
				ad[i].SetLow();
			}//end of i

	ad[6].SetHigh();
	ad[6].SetLow();
	// Delay(1);

for (int i = 0; i < 4; i++) {//7 lines are valid
			if (word & (1 << i))//lower 4 bit
				ad[i].SetHigh();
			else
				ad[i].SetLow();
			}//end of i
	ad[6].SetHigh();
	ad[6].SetLow();
	// Delay(1);
	
}

char operation(char op){
	char ch;
	if (op == 'A'){
		ch = '+';
	}
	else if (op == 'B'){
		ch = '-';
	}
	else if (op == 'C'){
		ch = '$';
	}
	else if (op == 'D'){
		ch = '/';
	}
	else if (op == '#'){
		ch = '=';
	}
	return ch;
}

int	char_to_int(char data){
int val = -1;
if (data == '1')
	val = 1;
else if (data == '2')
	val = 2;
else if (data == '3')
	val = 3;
else if (data == '4')
	val = 4;
else if (data == '5')
	val = 5;
else if (data == '6')
	val = 6;
else if (data == '7')
	val = 7;
else if (data == '8')
	val = 8;
else if (data == '9')
	val = 9;
else if (data == '0')
	val = 0;
else val = -1;
return val;
}

//////////////////////////////////Evaluation////////////////////
///////////////////////////////////////////////////////////////
	struct Stack 
{ 
    int top; 
    unsigned capacity; 
    int* array; 
}; 
// Stack Operations 
struct Stack* createStack( unsigned capacity ) 
{ 
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack)); 
  
    if (!stack)  
        return NULL; 
  
    stack->top = -1; 
    stack->capacity = capacity; 
  
    stack->array = (int*) malloc(stack->capacity * sizeof(int)); 
  
    if (!stack->array) 
        return NULL; 
    return stack; 
} 
int isEmpty(struct Stack* stack) 
{ 
    return stack->top == -1 ; 
} 
char peek(struct Stack* stack) 
{ 
    return stack->array[stack->top]; 
} 
char pop(struct Stack* stack) 
{ 
    if (!isEmpty(stack)) 
        return stack->array[stack->top--] ; 
    return '$'; 
} 
void push(struct Stack* stack, char op) 
{ 
    stack->array[++stack->top] = op; 
} 
  
  
// A utility function to check if the given character is operand 
int isOperand(char ch) 
{ 
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); 
} 
  
// A utility function to return precedence of a given operator 
// Higher returned value means higher precedence 
int Prec(char ch) 
{ 
    switch (ch) 
    { 
    case '+': 
    case '-': 
        return 1; 
  
    case '*': 
    case '/': 
        return 2; 
  
    case '^': 
        return 3; 
    } 
    return -1; 
} 

// The main function that converts given infix expression 
// to postfix expression.  
int infixToPostfix(char* exp) 
{ 
    int i, k; 
  
    // Create a stack of capacity equal to expression size  
    struct Stack* stack = createStack(strlen(exp)); 
    if(!stack) // See if stack was created successfully  
        return -1 ; 
  
    for (i = 0, k = -1; exp[i]; ++i) 
    { 
        // If the scanned character is an operand, add it to output. 
        if (isdigit(exp[i])) 
            exp[++k] = exp[i]; 
          
        // If the scanned character is an ‘(‘, push it to the stack. 
        else if (exp[i] == '(') 
            push(stack, exp[i]); 
          
        // If the scanned character is an ‘)’, pop and output from the stack  
        // until an ‘(‘ is encountered. 
        else if (exp[i] == ')') 
        { 
            while (!isEmpty(stack) && peek(stack) != '(') 
                exp[++k] = pop(stack); 
            if (!isEmpty(stack) && peek(stack) != '(') 
                return -1; // invalid expression              
            else
                pop(stack); 
        } 
        else // an operator is encountered 
        { 
            while (!isEmpty(stack) && Prec(exp[i]) <= Prec(peek(stack))) 
                exp[++k] = pop(stack); 
            push(stack, exp[i]); 
        } 
  
    } 
  	// int j = strlen(exp);
  	// oled.printf("length: %d\n",j);
    // pop all the operators from the stack 
    while (!isEmpty(stack)) 
        exp[++k] = pop(stack); 
  		exp[++k] = '\0';
} 
// The main function that returns value of a given postfix expression 
int evaluatePostfix(char* exp) 
{ 
    // Create a stack of capacity equal to expression size 
    struct Stack* stack = createStack(strlen(exp)); 
    int i; 
  
    // See if stack was created successfully 
    if (!stack) return -1; 
  
    // Scan all characters one by one 
    for (i = 0; exp[i]; ++i) 
    { 
        // If the scanned character is an operand (number here), 
        // push it to the stack. 
        if (isdigit(exp[i])) 
            push(stack, exp[i] - '0'); 
  
        // If the scanned character is an operator, pop two 
        // elements from stack apply the operator 
        else
        { 
            int val1 = pop(stack); 
            int val2 = pop(stack); 
            switch (exp[i]) 
            { 
            case '+': push(stack, val2 + val1); break; 
            case '-': push(stack, val2 - val1); break; 
            case '*': push(stack, val2 * val1); break; 
            case '/': push(stack, val2/val1); break; 
            } 
        } 
    } 
    return pop(stack); 
} 


Gpio Int = Gpio(0, 11);//this will be used in lab 4
private:
	Gpio ad[8] = {
		Gpio(2 ,2),
		Gpio(2 ,5),
		Gpio(2 ,7),
		Gpio(2 ,9),
		Gpio(0 ,15),
		Gpio(0 ,18),
		Gpio(0 ,1),
		Gpio(0 ,10)
	};
	Gpio WRITE = Gpio(0, 17);
	Gpio ALE = Gpio(0, 22);
	Gpio M_IO = Gpio(0, 0);
	
	OledTerminal oled;
	int count = -1;
	char result_str[50];
	uint8_t loc = 0x01;
	uint8_t data_arr[50];
	int stored_data_count = 0;
};
