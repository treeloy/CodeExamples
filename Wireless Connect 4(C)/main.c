/* **************************************************************************
 *
 * Author: Eloy Salinas
 * Project: CE121 Connect Four FInal Project
 * Description:
 *
 * Date: 6/12/13
 *
 * **************************************************************************
*/


#include <device.h>//# defines 

int ScanFlag = 0;//ISR for loading LEDs into matrix
//Row then col
int RowRed[8][8];//Array for second player
int RowBlue[8][8];//Array for first player
static unsigned int OnRow = 0; //current row for shift registers

int CapsFlag = 0;//ISR for packet transmision
uint16 curPos = 0;//CapSense
uint16 pastPos = -1;//CapSense
static int selectRow = 1;//Home row selection
int dropToken = 0;//Button edges
int Drop;//Drop disc if == 1
uint8 switch2[2];//Button0
uint8 switch1[2];//Button1
char ch = 12;//Channel
static int column = 1;//Column for LED arrays

uint8 lastTurnRed;//Last row red made
uint8 lastTurnBlue;//Last row blue made
int gameWon = 0;//Game is won and ends if == 1
int playerTurn = player1; //which player starts first

char Data[13];//Packet processing
char DataOld[13];//Packet processing
int blueCol;//Column for sending in packet
int gameStarted = 0;//Has the game begun? Used for xbee transmitting
int Count = 0;//Count bytes in frame


//Function Protoypes
void dropDisc();
void ClearHome();
void SetRedLED();
void SetBlueLED();
void AllOff();
void LoadHomeRow();
//LEDs loaded on the Scan.c ISR file
void LoadLEDs();
void CheckButtons();
void displayLCD();
void CheckWin(uint8 player);
void WinPlayer1();
void WinPlayer2();
void ChangeChannel();
//Packet processing is done in the UART_INT.c
//Packets are sent in the Caps.c ISR file
void sendPacket();



void main(){    
    //Check capsense switches position
    switch2[0] = switch2[1] = CapSense_CSD_CheckIsWidgetActive(CapSense_CSD_BUTTON1__BTN);
    switch1[0] = switch1[1] = CapSense_CSD_CheckIsWidgetActive(CapSense_CSD_BUTTON0__BTN);

    //Start with all LEDs off active low
    AllOff();
    
    //Start everything
    UART_Start();
    CapSense_CSD_Start();
    CyGlobalIntEnable;    
    Timer_Start();
    Timer_Cap_Start();
    Scan_Start();
    Caps_Start();   
    CapSense_CSD_InitializeAllBaselines(); 
    LCD_Start();
    LCD_Position(0,0);   
    ChangeChannel();//Currently F
    
    //Infinite loop
    for(;;) {     
        //Update CapSense sensors
        CapSense_CSD_UpdateEnabledBaselines();
        CapSense_CSD_ScanEnabledWidgets();
        
        //While the capsense is busy load the homerow
        while(CapSense_CSD_IsBusy() != 0){
            LoadHomeRow();      
        }

        //Information about players to the LCD
        displayLCD();
        
        //Check for transitions in the CapSense buttons
        CheckButtons();
        
        
        if(playerTurn == 2){
            //Check to see if it's a new move or a repeat
            if(( Data[8] == player2) && (Data[0] == 0x7E) ){
                dropDisc(); 
            }
        }
        
        //Looks for player one to press button to drop disc
        if(Drop == 1 && gameWon == 0){
            dropDisc();  
        }
     
      
    }
   
     
}


/* ========================================
 * void sendPacket()
 * Used by the Caps ISR to transmit a packet
 * frame containing the last move made by
 * player1. Only transmits a packet after the
 * game has started and it's player2s turn.
 * ========================================
*/
void sendPacket(){
    if(playerTurn == player2 && gameStarted == 1){
        //Add a timer=============================
        //================Send Move A Couple Times====================    
        UART_PutChar(0x7E);//Start
        UART_PutChar(0x00);//Length
        UART_PutChar(0x09);//Length
        UART_PutChar(0x01);//API ID
        UART_PutChar(0x00);//Frame ID
        UART_PutChar(0xFF);//Dest Address
        UART_PutChar(0xFF);//Dest Address
        UART_PutChar(0x01);//Disable Ack
        UART_PutChar(myID);//Player ID
        UART_PutChar(0x00);//Packet Type
        UART_PutChar((uint8) lastTurnBlue);//Row
        UART_PutChar((uint8) blueCol);//Col
        uint8 checksum = 0xFF - (0x01 + 0x00 + 0xFF + 0xFF + 0x01 + myID + 0x00 + (uint8) lastTurnBlue + (uint8) blueCol);
        UART_PutChar((uint8)checksum);
    }



}

/* ========================================
 * void displayLCD()
 * Display turn information on the LCD
 * Displays the col for player 1
 * Displays the player ID row, col for
 * player2
 * ========================================
*/
void displayLCD(){

    //Display the player turn and on player 2 turn the player id and row and col from packet
    if(gameWon == 0){
        curPos = CapSense_CSD_GetCentroidPos(CapSense_CSD_LINEARSLIDER0__LS);
        selectRow =(int) (((curPos*8)/255));
        
        LCD_Position(0,0);
        LCD_PrintString("Turn:");
        if(playerTurn == player1){
            LCD_PrintString("P1");
        }else{
            LCD_PrintString("P2");
        }
        
        if(playerTurn == player1){
            LCD_Position(0,8);
            LCD_PrintString("Col:");
            LCD_PrintNumber(column+1);
            LCD_PrintString(" ");
            //LCD_PrintInt8(ch);
        
            LCD_Position(1,0);
            LCD_PrintString("                          ");
        }else{
            LCD_Position(0,10);
            LCD_PrintString("                          ");
        }
        if(playerTurn == player2 ){
            LCD_Position(1,0);
            
            LCD_PrintString("P2ID: ");
            
            LCD_PrintInt8(Data[8]);//player id
            LCD_PrintString(" ");
            LCD_PrintInt8(Data[10]);//row
            LCD_PrintString(",");
            LCD_PrintInt8(Data[11]);//col
            
        }
    }




}

/* ========================================
 * void changeChannel()
 * Gives the Xbee the commands to enter AT
 * mode and change the channel in the 
 * second putstring call. Default C.
 *
 * ========================================
*/
void ChangeChannel(){
    
    UART_PutString("+++");
    CyDelay(2000);
    UART_PutString("ATCH C\r");
    CyDelay(2000);
    UART_PutString("ATCN\r");

}

/* ========================================
 * void dropDisc()
 * Checks the next LED to simulate a dropping
 * of the disc and checks that the move is 
 * in the bounds of the gameboard. Also
 * animates a dropping effect.
 * ========================================
*/
void dropDisc(){
int i = 6;
if(playerTurn == player1){  
    gameStarted = 1;
    //Make sure playable rows are not all full
    if(RowBlue[6][column] == ON || RowRed[6][column] == ON){
        playerTurn = player1;
    }else{
    ClearHome();
    //Next LED is off and in bounds
    while((RowBlue[i][column] == OFF && RowRed[i][column] == OFF) && i > -1){
        //Check next LED       
        if(i < 6){
            SetBlueLED(i+2, column+1, OFF);           
        }
       
        SetBlueLED(i+1, column+1, ON);
        CyDelay(100);
        lastTurnBlue = i+1;
        i--;       
    }

    //send move once just in case  
    sendPacket();
    //get col to send in packet
    blueCol = column+1;
    CheckWin(player1);
    
    playerTurn = player2;
    }
    
}

    else if(playerTurn == player2){
        //Make sure playable rows are not all full
        if(RowRed[6][(int)Data[11]-1] == ON || RowBlue[6][(int)Data[11]-1] == ON){
            playerTurn = player2;
        }else{
        ClearHome();
        //Next LED is off and in bounds
        while((RowBlue[i][(int)Data[11]-1] == OFF && RowRed[i][(int)Data[11]-1] == OFF) && i > -1){
            //Check next LED       
            if(i < 6){
                SetRedLED(i+2, (int)Data[11], OFF);
                
            }
            
            
            SetRedLED(i+1, (int)Data[11], ON);
            CyDelay(100);
            lastTurnRed= i;
            i--;
            
        }
        DataOld[10] = Data[10];
        DataOld[11] = Data[11];
        //Extract moves from packet
        column = (int)Data[11]-1;
        lastTurnRed = (int) Data[10];
        CheckWin(player2);
        playerTurn = player1;
            
            Drop = 0;
            //Grab old move
            
            int l = 0;
            //Clear packet array
            for(l = 0; l < 14; l++){
                Data[l] = 0;
            }
            
        }
    
    }
    //Reset and populate home row
    Drop = 0;
    ClearHome();
    if(gameWon == 0){   
        selectRow = 1;
        LoadHomeRow();
    }

}

/* ========================================
 * void LoadLEDs()
 * Turns on a row, loads that row's values
 * into a shift register for all colors, 
 * then turns off the row and iterates 
 * through all eight rows.
 * ========================================
*/
void LoadLEDs(){       
    Timer_Stop();
    
    //Restart rows
    if(OnRow > 7 ){
        OnRow = 0;
    }
    
    //Turn on Row
    if(OnRow == 1){
        ControlRow1_Write(1);
    }else if(OnRow == 2){
        ControlRow2_Write(1);
    }else if(OnRow == 3){
        ControlRow3_Write(1);
    }else if(OnRow == 4){
        ControlRow4_Write(1);
    }else if(OnRow == 5){
        ControlRow5_Write(1);
    }else if(OnRow == 6){
        ControlRow6_Write(1);
    }else if(OnRow == 7){
        ControlRow7_Write(1);
    }else if(OnRow == 0){
        ControlRow8_Write(1);
    }else{
        
    }

    
    //Send Low to RCLK
    ControlLatch_Write(0);
   
    int j = 7;  
    for(j = 7; j != -1; j--){
        //Send low to serial CLK
        ControlClk_Write(0);
        //CyDelay(2);
        ControlData_Write(RowRed[OnRow][j]);
        //CyDelay(2);
        ControlData2_Write(RowBlue[OnRow][j]);
        
        //High to serial CLK
        ControlClk_Write(1);
        
    }
    
    //Turn off Rows
    if(OnRow == 1){
        ControlRow1_Write(0);
    }else if(OnRow == 2){
        ControlRow2_Write(0);
    }else if(OnRow == 3){
        ControlRow3_Write(0);
    }else if(OnRow == 4){
        ControlRow4_Write(0);
    }else if(OnRow == 5){
        ControlRow5_Write(0);
    }else if(OnRow == 6){
        ControlRow6_Write(0);
    }else if(OnRow == 7){
        ControlRow7_Write(0);
    }else if(OnRow == 0){
        ControlRow8_Write(0);
    }else{
    }
    //High to the latch to display
    ControlLatch_Write(1);
    
    OnRow++;
    //Reset Timer to 0
    Timer_WriteCounter(0);
    Timer_Start();    

}

/* ========================================
 * void WinPlayer1()
 * Draws a smiley face in the color of 
 * player1 to indicate a win.
 *
 *
 * ========================================
*/
void WinPlayer1(){
    SetBlueLED(4, 1, ON);
    SetBlueLED(3, 2, ON);
    SetBlueLED(2, 3, ON);
    
    SetBlueLED(2, 4, ON);
    SetBlueLED(2, 5, ON);
    SetBlueLED(2, 6, ON);
    
    SetBlueLED(3, 7, ON);
    SetBlueLED(4, 8, ON);
    
    SetBlueLED(7, 2, ON);
    SetBlueLED(6, 2, ON);
    SetBlueLED(7, 3, ON);
    SetBlueLED(6, 3, ON);
    
    SetBlueLED(7, 6, ON);
    SetBlueLED(6, 6, ON);
    SetBlueLED(7, 7, ON);
    SetBlueLED(6, 7, ON);
    
}

/* ========================================
 * void WinPlayer2()
 * Draws a smiley face in the color of 
 * player2 to indicate a win.
 *
 *
 * ========================================
*/
void WinPlayer2(){
    SetRedLED(4, 1, ON);
    SetRedLED(3, 2, ON);
    SetRedLED(2, 3, ON);
    
    SetRedLED(2, 4, ON);
    SetRedLED(2, 5, ON);
    SetRedLED(2, 6, ON);
    
    SetRedLED(3, 7, ON);
    SetRedLED(4, 8, ON);
    
    SetRedLED(7, 2, ON);
    SetRedLED(6, 2, ON);
    SetRedLED(7, 3, ON);
    SetRedLED(6, 3, ON);
    
    SetRedLED(7, 6, ON);
    SetRedLED(6, 6, ON);
    SetRedLED(7, 7, ON);
    SetRedLED(6, 7, ON);
    
}

/* ========================================
 * void CheckWin(uint8 player)
 * Checks the rows horizontally, vertically,
 * and diagonally for each player to find
 * four discs in a row for the player
 * passed into the function
 * ========================================
*/
void CheckWin(uint8 player){
    int horizontal = 0;
    int vertical = 0;
    int diagonal = 0;
    int i = 0;
    //===================Player 1 Blue=====================================
    if(player == player1){
        //Horizontal Win Check
        //To the Right
        //Whole Row
        i = 0;
        for(i = 0; i < 8; i++){
            if(RowBlue[lastTurnBlue-1][i] == ON && RowBlue[lastTurnBlue-1][i+1] == ON){
                horizontal++;    
            }else{
                if(horizontal == 3){
                    AllOff();
                    WinPlayer1();
                    gameWon = 1;
                }
                horizontal = 0;
            }
        }
   
        //Vertical Win check
        //Down
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowBlue[lastTurnBlue-1-i][column]  == ON) && (lastTurnBlue-1-i > -1)){
                vertical++;
            }
        }
        if(vertical == 4){
        AllOff();
        WinPlayer1();
        gameWon = 1;
        }else{
            vertical = 0;
        }
        
        //Diagonal Win Check
        //Check middle of diagonals
        i = 0;
        for(i = 0; i < 2; i++){
            if(RowBlue[lastTurnBlue-1-i][column-i] == ON && RowBlue[lastTurnBlue-1-i-1][column-i-1] == ON && (lastTurnBlue-1-i > -1 && column-i > -1)){
                diagonal++;
            }
        }
        
        i = 0;
        for(i = 0; i < 2; i++){
            if(RowBlue[lastTurnBlue-1+i][column+i] == ON && RowBlue[lastTurnBlue-1+i+1][column+i+1] == ON && (lastTurnBlue-1+i < 8 && column+i < 8)){
                diagonal++;
            }
        }
        if(diagonal >= 3){
        AllOff();
        WinPlayer1();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
        
        // / bot left to top right
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowBlue[lastTurnBlue-1+i][column+i]  == ON) && (lastTurnBlue-1+i < 8 && column+1 < 8)){
                diagonal++;
            }
        }
        if(diagonal== 4){
        AllOff();
        WinPlayer1();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowBlue[lastTurnBlue-1-i][column-i]  == ON) && (column-i > -1 && lastTurnBlue-1-i > -1)){
                diagonal++;
            }
        }
        if(diagonal == 4){
        AllOff();
        WinPlayer1();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
        
        // \ top right to bot left
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowBlue[lastTurnBlue-1-i][column+i]  == ON) && (column+i < 8 && lastTurnBlue-1-i > -1)){
                diagonal++;
            }
        }
        if(diagonal == 4){
        AllOff();
        WinPlayer1();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowBlue[lastTurnBlue-1+i][column-i]  == ON) && (column-i > -1 && lastTurnBlue-1+i < 8)){
                diagonal++;
            }
        }
        if(diagonal == 4){
        AllOff();
        WinPlayer1();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
    }else if(player == player2){
        horizontal = 0;
        vertical = 0;
        diagonal = 0;
        
        
        //====================================Check player 2=========================
        //Horizontal Win Check
        //To the Right
        //Whole Row
        i = 0;
        for(i = 0; i < 8; i++){
            if(RowRed[lastTurnRed-1][i] == ON && RowRed[lastTurnRed-1][i+1] == ON){
                horizontal++;    
            }else{
                if(horizontal == 3){
                    AllOff();
                    WinPlayer2();
                    gameWon = 1;
                }
                horizontal = 0;
            }
        }
        
        //Vertical Win check
        //Down
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowRed[lastTurnRed-1-i][column]  == ON) && (lastTurnRed-1-i > -1)){
                vertical++;
            }
        }
        if(vertical == 4){
            AllOff();
            WinPlayer2();
            gameWon = 1;
        }else{
            vertical = 0;
        }
        
        //Diagonal Win Check
        //Check middle of diagonals
        i = 0;
        for(i = 0; i < 2; i++){
            if(RowRed[lastTurnRed-1-i][column-i] == ON && RowRed[lastTurnRed-1-i-1][column-i-1] == ON && (lastTurnRed-1-i > -1 && column-i > -1)){
                diagonal++;
            }
        }
        
        i = 0;
        for(i = 0; i < 2; i++){
            if(RowRed[lastTurnRed-1+i][column+i] == ON && RowRed[lastTurnRed-1+i+1][column+i+1] == ON && (lastTurnRed-1+i < 8 && column+i < 8)){
                diagonal++;
            }
        }
        if(diagonal >= 3){
        AllOff();
        WinPlayer2();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
        // / bot left to top right
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowRed[lastTurnRed-1+i][column+i]  == ON) && (lastTurnRed-1+i < 8 && column+1 < 8)){
                diagonal++;
            }
        }
        if(diagonal== 4){
        AllOff();
        WinPlayer2();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowRed[lastTurnRed-1-i][column-i]  == ON) && (column-i > -1 && lastTurnRed-1-i > -1)){
                diagonal++;
            }
        }
        if(diagonal == 4){
        AllOff();
        WinPlayer2();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
        
        // \ top right to bot left
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowRed[lastTurnRed-1-i][column+i]  == ON) && (column+i < 8 && lastTurnRed-1-i > -1)){
                diagonal++;
            }
        }
        if(diagonal == 4){
        AllOff();
        WinPlayer2();
        gameWon = 1;
        }else{
            diagonal = 0;
        }
        i = 0;
        for(i = 0; i < 4; i++){
            if( (RowRed[lastTurnRed-1+i][column-i]  == ON) && (column-i > -1 && lastTurnRed-1+i < 8)){
                diagonal++;
            }
        }
        if(diagonal == 4){
        AllOff();
        WinPlayer2();
        gameWon = 1;
        }
    
    }
}



/* ========================================
 * void CheckButtons()
 * Looks for a change in button states to
 * indicate a press on the CapSense buttons.
 *
 *
 * ========================================
*/
void CheckButtons(){

            
            switch1[1] = switch1[0];
            switch1[0] = CapSense_CSD_CheckIsWidgetActive(CapSense_CSD_BUTTON0__BTN);

            switch2[1] = switch2[0];
            switch2[0] = CapSense_CSD_CheckIsWidgetActive(CapSense_CSD_BUTTON1__BTN);
            
            if(switch1[0] != switch1[1]){
            
               dropToken++;
            }
            
            if(switch2[0] != switch2[1]){
                ch++;
                ChangeChannel(ch);
               
            }
            //Load homerow
            if(gameWon != 1){
                LoadHomeRow(); 
            }
        //Look for a drop    
        if(dropToken == 2){
            Drop = 1;
            dropToken = 0;
        }
        else if(dropToken == 4){
            Drop = 0;
            dropToken = 0;
       }

}

/* ========================================
 * void ClearHome()
 * Turns off all the LEDs on the home row.
 *
 *
 *
 * ========================================
*/
void ClearHome(){
    SetRedLED(8,1, OFF);
    SetRedLED(8,2, OFF);
    SetRedLED(8,3, OFF);
    SetRedLED(8,4, OFF);
    SetRedLED(8,5, OFF);
    SetRedLED(8,6, OFF);
    SetRedLED(8,7, OFF);
    SetRedLED(8,8, OFF);
    
    SetBlueLED(8,1, OFF);
    SetBlueLED(8,2, OFF);
    SetBlueLED(8,3, OFF);
    SetBlueLED(8,4, OFF);
    SetBlueLED(8,5, OFF);
    SetBlueLED(8,6, OFF);
    SetBlueLED(8,7, OFF);
    SetBlueLED(8,8, OFF);

}

/* ========================================
 * void LoadHomeRow()
 * Loads the proper LED depending on the
 * state of the selectRow variable and sets
 * the column variable for use in win checks
 *
 * ========================================
*/
void LoadHomeRow(){
        //Lights up proper LED based on value for each player on their turn
        if(selectRow == 1){
            ClearHome();
            if(playerTurn == player1){
            SetBlueLED(8,1, ON);
            } else if(playerTurn == player2){
            SetRedLED(8,1, ON);
            }
            column = 0;
        }
        else if(selectRow == 2){
            ClearHome();
            if(playerTurn == player1){
            SetBlueLED(8,2, ON);
            } else if(playerTurn == player2){
            SetRedLED(8,2, ON);
            }
            column = 1;
        }
        else if(selectRow == 3){
            ClearHome();
            if(playerTurn == player1){
            SetBlueLED(8,3, ON);
            } else if(playerTurn == player2){
            SetRedLED(8,3, ON);
            }
            column = 2;
        }
        else if(selectRow == 4){
            ClearHome();
            if(playerTurn == player1){
            SetBlueLED(8,4, ON);
            } else if(playerTurn == player2){
            SetRedLED(8,4, ON);
            }
            column = 3;
        }
        else if(selectRow == 5){
            ClearHome();
            if(playerTurn == player1){
            SetBlueLED(8,5, ON);
            } else if(playerTurn == player2){
            SetRedLED(8,5, ON);
            }
            column = 4;
        }
        else if(selectRow == 6){
            ClearHome();
            if(playerTurn == player1){
            SetBlueLED(8,6, ON);
            } else if(playerTurn == player2){
            SetRedLED(8,6, ON);
            }
            column = 5;
        }
        else if(selectRow == 7){
            ClearHome();
            if(playerTurn == player1){
            SetBlueLED(8,7, ON);
            } else if(playerTurn == player2){
            SetRedLED(8,7, ON);
            }
            column = 6;
        }
        else if(selectRow == 8){
            ClearHome();
            if(playerTurn == player1){
            SetBlueLED(8,8, ON);
            } else if(playerTurn == player2){
            SetRedLED(8,8, ON);
            }
            column = 7;
        }


}

/* ========================================
 * void SetRedLED()
 * Loads the state, ON or OFF, into the LED
 * array of the second player
 *
 *
 * ========================================
*/
void SetRedLED(int row, int col, int state){
    RowRed[row-1][col-1] = state;

}

/* ========================================
 * void SetBlueLED()
 * Loads the state, ON or OFF, into the LED
 * array of the first player
 *
 *
 * ========================================
*/
void SetBlueLED(int row, int col, int state){
    RowBlue[row-1][col-1] = state;

}

/* ========================================
 * void AllOff()
 * Turns off all the LEDs on the matrix.
 *
 *
 *
 * ========================================
*/
void AllOff(){
    int i = 0;
    int k = 0;
    for(k = 0; k < 8; k++){
        for(i = 0; i < 8; i++){
            RowRed[i][k] = 1;
            RowBlue[i][k] = 1;
        }
    }
}


/* [] END OF FILE */
