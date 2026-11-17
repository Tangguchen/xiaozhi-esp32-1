#pragma once
#include <lvgl.h>

namespace emote {

class EmoteDisplay {
public:
    EmoteDisplay();
    ~EmoteDisplay();
    bool init(lv_obj_t *parent);
    void SetStatus(const char * const status);
    void ShowNotification(const char *notification, int duration_ms);
    void UpdateStatusBar(bool update_all);
    void clear();
private:
    void* emote_handle_ = nullptr;
    lv_timer_t* anim_timer_ = nullptr;
    lv_obj_t* eye_container_ = nullptr;
    lv_obj_t* left_eye_ = nullptr;
    lv_obj_t* right_eye_ = nullptr;
    int anim_frame_ = 0;
};

}
