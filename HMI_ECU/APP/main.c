/*****************************************************************************
 * File: main.c
 * Description: Example main for testing the 4x4 keypad driver
 * Author: Ahmedhh
 * Date: November 27, 2025
 *****************************************************************************/

#include <string.h> // REQUIRED for strcat, strcpy, strcmp
#include <ctype.h>  // REQUIRED for isdigit()
#include "std_types.h"
#include <stdint.h>    // Defines uint32_t, uint8_t
#include <stdbool.h>  // Defines bool
#include <stdio.h>    // <--- REQUIRED for printf
#include "LCD.h"
#include "Led.h"
#include "Button.h"
#include "SYSTICK.h"
#include "keypad.h"
#include "ADC.h"
#include "potentiometer.h"

// --- FUNCTION PROTOTYPE ---
// Function to handle the password entry logic
void GetPassword(char *password);
int CheckPassword(char *password, int *menu);


/*
 * Main function
 */
int main()
{
    SysTick_Init();
    Lcd_Init();
    POT_Init();
    Keypad_Init();
    UART0_Init();
    SysTick_Wait(50);  /* Allow ADC to stabilize */
    
    while (1)
    {
        char isSaved;
        // Use char arrays for C strings, initialized to null terminator {0}
        char password[6] = {0}; // For 5 chars + '\0'
        UART0_SendChar('6'); //////////////////////////////////////////////////////check if pass exists
        SysTick_Wait(500);
        isSaved = UART0_ReceiveChar(); /////////////////////////////////////returns bool 0: not set, 1:Set
        
        if (isSaved == '1'){ 
            Lcd_Clear();
            Lcd_GoToRowColumn(0,0);
            Lcd_DisplayString("Create Password: ");
            // --- 1. GET PASSWORD ---
            GetPassword(password);

            // --- 2. Check for the final action key (e.g., '#') ---
            
            
            // Compare the string pointer to the "#" string literal
                              
                UART0_SendChar('2'); 
                UART0_SendString(password);
                isSaved = true; 
            

        }

        if (isSaved == '0')
        {
            Lcd_Clear();
            Lcd_DisplayString("Password Saved");
            SysTick_Wait(2000);
            Lcd_Clear();
            Lcd_GoToRowColumn(0,0);
            Lcd_DisplayString("A: Open Door");

            int menu = 0;
            
            // --- MENU LOOP (Same logic as before) ---
            while (1) {
                const char *key_str = "";
                switch(menu) { 
                    
                    case 0: // Main menu display and key handling
                    {
                        static int options = 0; 
                        
                        
                        Lcd_GoToRowColumn(1,0); 
                        Lcd_DisplayString("*: Other Options");

                        do {
                            key_str = Keypad_GetKey();
                        } while (key_str == 0);
                        
                        if (strcmp(key_str, "*") == 0) {
                            options = (options + 1) % 3;
                            Lcd_Clear();
                            Lcd_GoToRowColumn(0,0);
                            
                            switch (options) { 
                                case 0:
                                    Lcd_DisplayString("1: Open Door");
                                    break;
                                case 1:
                                    Lcd_DisplayString("2: Change Pass");
                                    break;
                                case 2:
                                    Lcd_DisplayString("3: Set Timeout");
                                    break;
                            } 
                        }
                        else if (strcmp(key_str, "1") == 0)
                        {
                            menu = 1;
                        }
                        else if (strcmp(key_str, "2") == 0)
                        {
                            menu = 2;
                        }
                        else if (strcmp(key_str, "3") == 0)
                        {
                            menu = 3;
                        }
                        
                        SysTick_Wait(200);  
                        break;  
                    }
                    char password[6] = {0}; // For 5 chars + '\0'

                    case 1: // Open door
                    {
                        Lcd_Clear();
                        //printf("Open Door\n"); 
                        Lcd_GoToRowColumn(0,0);
                        Lcd_DisplayString("Enter Password: ");
                        if (CheckPassword(password, &menu)==1){
                          //printf("pass checked");
                               SysTick_Wait(500);
                               UART0_SendChar('3');
                               Lcd_Clear();
                               Lcd_GoToRowColumn(0,0);
                               Lcd_DisplayString("Welcome");
                        }

                            
                        
                        SysTick_Wait(1000); //////////////////////Should stay on welcome until door closes?????
                        menu = 0;
                        Lcd_Clear();
                        Lcd_GoToRowColumn(0,0);
                        Lcd_DisplayString("A: Open Door");

                        break;
                    }
                    
                    case 2: // Change Password
                    {
                        //printf("Change Pass\n");  
                        Lcd_Clear();
                        Lcd_GoToRowColumn(0,0);
                        Lcd_DisplayString("Enter old Pass: ");
                        if(CheckPassword(password, &menu)==1){
                            Lcd_Clear();
                            Lcd_GoToRowColumn(0,0);
                            Lcd_DisplayString("Enter new Pass: ");
                            GetPassword(password);
                            SysTick_Wait(500);
                            UART0_SendChar('2');
                            UART0_SendString(password);
                            Lcd_Clear();
                            Lcd_GoToRowColumn(0,0);
                            Lcd_DisplayString("Password Set!");
                        }
                        

                   
                                 
                        SysTick_Wait(1000);
                        menu = 0;
                        Lcd_Clear();
                        Lcd_GoToRowColumn(0,0);
                        Lcd_DisplayString("B: Change Password"); 
                        break;                            
                       }
                    // ... (Previous code remains the same)

                    case 3: // Set Auto-Lock Timeout
                    {
                       // printf("Auto lock\n");
                        
                        
                                uint32_t time = 0;
                                char time_str[6] = {0}; // Buffer for time (up to 30)
                                const char *action_key = "";
                                
                                // Display static text once before the inner loop
                                Lcd_Clear();
                                Lcd_GoToRowColumn(0,0);
                                Lcd_DisplayString("Set Timeout (s):");

                                // Loop to read potentiometer and update display until '#' is pressed
                                do {
                                    // 1. Read the potentiometer value (mapped between 5 and 30)
                                    uint32_t new_time = POT_ReadMapped(5, 30);
                                    
                                    // Only update and refresh the display if the time value has actually changed
                                    if (new_time != time) 
                                    {
                                        time = new_time;
                                        
                                        // 2. Convert the integer time to a string
                                        snprintf(time_str, sizeof(time_str), "%05lu", (unsigned long)time);                                        
                                        // 3. Update the display area for the time
                                        Lcd_GoToRowColumn(1,0); 
                                        Lcd_DisplayString(time_str); 
                                        Lcd_DisplayString(" s   "); // Overwrite trailing characters with spaces to clear old value (e.g., 10 over 5)
                                        
                                        //printf("Timeout: %lu s\n", time);
                                    }
                                    
                                    // Check for keypad press to exit
                                    action_key = Keypad_GetKey();
                                    
                                    // Small delay to allow potentiometer ADC reading stability and polling
                                    SysTick_Wait(50);

                                } while (action_key == 0);
                                
                                // Compare the string pointer to the "#" string literal
                                if (strcmp(action_key, "#") == 0) {
                                    Lcd_Clear();
                                    Lcd_GoToRowColumn(0,0);
                                    Lcd_DisplayString("Enter Password: ");
                                    if(CheckPassword(password, &menu)==1){
                                    // The final 'time' value now holds the selected timeout
                                    // You would typically save this value to EEPROM or a global variable here.
                                    sprintf(time_str, "%lu", time);
                                    SysTick_Wait(500);

                                    UART0_SendChar('5');
                                    UART0_SendString(time_str);
                                  
                                  
                                    Lcd_Clear();
                                    Lcd_GoToRowColumn(0,0);
                                    
                                    // Display confirmation
                                    Lcd_DisplayString("Timeout Set to ");
                                    Lcd_GoToRowColumn(1,0);
                                    Lcd_DisplayString(time_str);
                                    Lcd_DisplayString(" seconds");
                                    
                                    SysTick_Wait(2000); // Display confirmation for 2 seconds
                                    menu = 0; // Return to main menu
                                    }
                                }
                        
                        // If '#' wasn't pressed (e.g., another key or power cycle), still return to menu
                        menu = 0;
                        Lcd_GoToRowColumn(0,0);
                        Lcd_DisplayString("C: Set Timeout");

                        break;
                    }
// ... (The rest of the code is unchanged)
                } // End switch(menu)       
            } // End while(1) menu loop
        } // End if (isSaved)
        
        SysTick_Wait(1000); 
    }
}

// --- FUNCTION IMPLEMENTATION ---
/**
 * @brief Handles password entry with display, key polling, and digit validation.
 * @param password_buffer: Pointer to the char array where the password will be stored.
 * @param buffer_size: The total size of the password_buffer (e.g., 6 for 5 chars + \0).
 */
int CheckPassword(char *password, int *menu)
{
    // Clear the buffer and display for a fresh start
    
    int correctPass = 0; 

    while(correctPass != 1){
    // The loop runs for one less than the buffer size (e.g., 5 characters for a size 6 buffer)
      GetPassword(password);
      SysTick_Wait(500);

      UART0_SendChar('1');
      UART0_SendString(password);
      correctPass = (int)UART0_ReceiveChar()-'0';      //printf("check pass: %d", correctPass);
      if (correctPass == 0){
        //printf("incorrect pass");
        Lcd_Clear();
        Lcd_GoToRowColumn(0, 0); // Go to the second row, column i
        Lcd_DisplayString("Incorrect Pass!");
        SysTick_Wait(1000);
        Lcd_Clear();
        Lcd_GoToRowColumn(0, 0); // Go to the second row, column i
        Lcd_DisplayString("Try Again, pass:");
        
      }
      
      else if (correctPass == 2){
        Lcd_Clear();
        Lcd_GoToRowColumn(0, 0); // Go to the second row, column i
        Lcd_DisplayString("LOCK OUT");
        SysTick_Wait(5000);
        menu=0;
        return 2;
      }
     
      
    } 
      return correctPass; //////////////return 1
}



void GetPassword(char *password) {
    // Initialize action_key to something other than "#" so the loop starts
    const char *action_key = ""; 

    // The outer loop allows the user to re-enter if they didn't press '#'
    while (strcmp(action_key, "#") != 0) {
        
        // 1. Reset the password buffer for a new attempt
        memset(password, 0, 6); // Size 6 to include null terminator
        
        // 2. Clear the password area on the LCD (second line)
        Lcd_GoToRowColumn(1, 0);
        Lcd_DisplayString("                "); // Clear 16 characters

        // 3. Get exactly 5 digits
        for (int i = 0; i < 5; i++) {
            const char *key_str;

            // Wait for a key press
            do {
                key_str = Keypad_GetKey();
            } while (key_str == 0);

            // Check for digit-only input
            if (!isdigit(key_str[0])) {
                i--; // Stay at current position if non-digit pressed
                continue;
            }

            // Append the digit to the password buffer
            strcat(password, key_str);

            // Display masked character
            Lcd_GoToRowColumn(1, i);
            Lcd_DisplayCharacter('*');

            SysTick_Wait(200); // Debounce
        }
        //printf(password);

        // 4. After 5 digits, wait specifically for '#' to confirm 
        // OR any other key to clear and restart
        do {
            action_key = Keypad_GetKey();
        } while (action_key == 0);
        
        // Debug output: use %s for strings, not %c for pointers
    }
}