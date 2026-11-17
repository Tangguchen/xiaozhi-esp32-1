#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"
#include <stdlib.h>

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
            ssd1306_clear();
            ssd1306_draw_line(eyeX1-eyeR, eyeY, eyeX1+eyeR, eyeY, 1);
            ssd1306_draw_line(eyeX2-eyeR, eyeY, eyeX2+eyeR, eyeY, 1);
            ssd1306_refresh();
            vTaskDelay(pdMS_TO_TICKS(150));
            blink = false;
        }
        ssd1306_clear();
        ssd1306_draw_circle(eyeX1, eyeY, eyeR, 1);
        ssd1306_draw_circle(eyeX2, eyeY, eyeR, 1);
        ssd1306_fill_circle(eyeX1, eyeY, eyeR, 1);
        ssd1306_fill_circle(eyeX2, eyeY, eyeR, 1);
        ssd1306_fill_circle(eyeX1+px, eyeY+py, pupilR, 0);
        ssd1306_fill_circle(eyeX2+px, eyeY+py, pupilR, 0);
        ssd1306_refresh();
        vTaskDelay(pdMS_TO_TICKS(80));
    }
}
