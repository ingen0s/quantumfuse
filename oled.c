#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OLED_Driver.h"
#include "GUI_Paint.h"
#include "DEV_Config.h"

int main(int argc, char *argv[])
{
    // Initialize hardware
    if (DEV_ModuleInit() != 0) {
        printf("Module initialization failed\n");
        return -1;
    }

    // Initialize OLED (1.5inch SH1107, 128x128)
    OLED_1in5_Init();
    DEV_Delay_ms(500);
    OLED_1in5_Clear();

    // Create image buffer
    UBYTE *BlackImage;
    UWORD Imagesize = ((OLED_1in5_WIDTH % 8 == 0) ? (OLED_1in5_WIDTH / 8) : (OLED_1in5_WIDTH / 8 + 1)) * OLED_1in5_HEIGHT;
    if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to allocate memory for image\n");
        return -1;
    }

    // Initialize image buffer (0: black, 1: white)
    Paint_NewImage(BlackImage, OLED_1in5_WIDTH, OLED_1in5_HEIGHT, 0, WHITE);
    Paint_SetScale(2); // 2-bit grayscale for SH1107

    // Draw text "@microsawft" centered
    Paint_Clear(WHITE); // Clear buffer to white (off pixels)
    Paint_DrawString_EN(20, 50, "@microsawft", &Font16, BLACK, WHITE); // Font16, black text on white
    OLED_1in5_Display(BlackImage); // Update display

    // Delay to keep text visible
    DEV_Delay_ms(5000); // Display for 5 seconds

    // Clean up
    OLED_1in5_Clear();
    free(BlackImage);
    DEV_ModuleExit();

    return 0;
}
