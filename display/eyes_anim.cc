#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "oled_display.h"
#include <cstdlib>

static int eyeX1 = 32;
static int eyeX2 = 96;
static int eyeY  = 32;
static int eyeR  = 18;
static int pupilR = 6;
static int px = 0, py = 0;
static bool blink = false;

void eyes_anim_task(void *arg)
{
    while (1)
    {
        if (rand() % 40 == 0)
        {
            px = (rand() % 14) - 7;
            py = (rand() % 8) - 4;
        }
        if (rand() % 60 == 0)
        {
            blink = true;
            oled_clear();
            oled_draw_line(eyeX1-eyeR, eyeY, eyeX1+eyeR, eyeY, 1);
            oled_draw_line(eyeX2-eyeR, eyeY, eyeX2+eyeR, eyeY, 1);
            oled_refresh();
            vTaskDelay(pdMS_TO_TICKS(150));
            blink = false;
        }
        oled_clear();
        oled_draw_circle(eyeX1, eyeY, eyeR, 1);
        oled_draw_circle(eyeX2, eyeY, eyeR, 1);
        oled_fill_circle(eyeX1, eyeY, eyeR, 1);
        oled_fill_circle(eyeX2, eyeY, eyeR, 1);
        oled_fill_circle(eyeX1+px, eyeY+py, pupilR, 0);
        oled_fill_circle(eyeX2+px, eyeY+py, pupilR, 0);
        oled_refresh();
        vTaskDelay(pdMS_TO_TICKS(80));
    }
}
