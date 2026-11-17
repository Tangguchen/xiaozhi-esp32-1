#include "emote_display.h"

// Standard C++ headers
#include <algorithm>
#include <cinttypes>
#include <cstring>
#include <memory>
#include <tuple>
#include <unordered_map>

// Standard C headers
#include <sys/time.h>
#include <time.h>

// ESP-IDF headers
#include <esp_lcd_panel_io.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <lvgl.h>

// FreeRTOS headers
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Project headers
#include "assets.h"
#include "assets/lang_config.h"
#include "board.h"
#include "expression_emote.h"
#include "gfx.h"

namespace emote {

static void eye_anim_timer_cb(lv_timer_t *timer)
{
    EmoteDisplay *obj = (EmoteDisplay *)timer->user_data;
    if (!obj || !obj->left_eye_ || !obj->right_eye_)
    {
        return;
    }

    obj->anim_frame_++;
    int h;
    // 眨眼动画：0~30 睁眼，31~35 闭眼，循环
    if (obj->anim_frame_ % 35 < 30)
    {
        h = 12;
    }
    else
    {
        h = 2;
    }
    lv_obj_set_height(obj->left_eye_, h);
    lv_obj_set_height(obj->right_eye_, h);
}

EmoteDisplay::EmoteDisplay()
{
    emote_handle_ = nullptr;
    anim_timer_ = nullptr;
    eye_container_ = nullptr;
    left_eye_ = nullptr;
    right_eye_ = nullptr;
    anim_frame_ = 0;
}

EmoteDisplay::~EmoteDisplay()
{
    if (anim_timer_)
    {
        lv_timer_del(anim_timer_);
        anim_timer_ = nullptr;
    }
}

bool EmoteDisplay::init(lv_obj_t *parent)
{
    emote_handle_ = expression_emote_create(parent);
    if (!emote_handle_)
    {
        ESP_LOGE(TAG, "expression_emote create fail");
        return false;
    }

    // 创建眼睛容器
    eye_container_ = lv_obj_create(parent);
    lv_obj_set_size(eye_container_, 128, 64);
    lv_obj_set_pos(eye_container_, 0, 0);
    lv_obj_set_style_bg_opa(eye_container_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(eye_container_, 0, 0);

    // 左眼
    left_eye_ = lv_obj_create(eye_container_);
    lv_obj_set_size(left_eye_, 14, 12);
    lv_obj_set_pos(left_eye_, 30, 22);
    lv_obj_set_style_bg_color(left_eye_, lv_color_white(), 0);
    lv_obj_set_style_radius(left_eye_, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(left_eye_, 0, 0);

    // 右眼
    right_eye_ = lv_obj_create(eye_container_);
    lv_obj_set_size(right_eye_, 14, 12);
    lv_obj_set_pos(right_eye_, 84, 22);
    lv_obj_set_style_bg_color(right_eye_, lv_color_white(), 0);
    lv_obj_set_style_radius(right_eye_, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(right_eye_, 0, 0);

    // 先隐藏眼睛，待机才显示
    lv_obj_add_flag(eye_container_, LV_OBJ_FLAG_HIDDEN);

    // 启动定时器，50ms刷新一次动画
    anim_timer_ = lv_timer_create(eye_anim_timer_cb, 50, this);
    return true;
}

void EmoteDisplay::SetStatus(const char * const status)
{
    if (!emote_handle_) return;

    // 先把原生表情关掉
    expression_emote_set(emote_handle_, status);

    if (strcmp(status, "idle") == 0)
    {
        // 待机：显示我们的眨眼眼睛
        lv_obj_clear_flag(eye_container_, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        // 说话、聆听、思考：隐藏动画眼睛，使用官方表情
        lv_obj_add_flag(eye_container_, LV_OBJ_FLAG_HIDDEN);
    }
}

void EmoteDisplay::ShowNotification(const char *notification, int duration_ms)
{
    if (!emote_handle_) return;
    expression_emote_notify(emote_handle_, notification, duration_ms);
}

void EmoteDisplay::UpdateStatusBar(bool update_all)
{
    expression_emote_update_status_bar(emote_handle_, update_all);
}

void EmoteDisplay::clear()
{
    if (eye_container_)
    {
        lv_obj_add_flag(eye_container_, LV_OBJ_FLAG_HIDDEN);
    }
    expression_emote_clear(emote_handle_);
}

}
