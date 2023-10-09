#pragma once
#include "DxLib.h"
class fpsCtrl {
private:
    int frame_time;
    int wait_time;
    int last_time, now_time;
    float count;
    float fps;
    int update_time;
    int last_update;
	
    int disp_x, disp_y;
    DWORD disp_color;
    
	void Init(float RefreshRate, int UpdateTime) {
        frame_time = (int)(1000.0f / RefreshRate);
        update_time = UpdateTime;
        wait_time = 0;
        last_time = now_time = 0;
        count = 0.0f;
        fps = 0.0f;
        last_update = 0;
        disp_x = 0;
        disp_y = 0;
        disp_color = GetColor(255, 255, 255);
    }
public:
    fpsCtrl(float RefreshRate, int UpdateTime) {
        Init(RefreshRate, UpdateTime);
    }
    fpsCtrl() {
        Init(60.0f, 800);
    }
	
    void Wait() {
        now_time = GetNowCount();
        wait_time = frame_time - (now_time - last_time);
        if (wait_time > 0) {
            WaitTimer(wait_time);
        }
        last_time = GetNowCount();
    }
	
    float Get() {
        count += 1.0f;
        if (update_time < (last_time - last_update)) { //アップデート時間になっていれば
            fps = count / (float)(last_time - last_update) * 1000.0f; //FPS値の計算
            last_update = last_time;
            count = 0.0f;
        }
        return (fps);
    }
	
	void Disp() {
        DrawFormatString(disp_x, disp_y, disp_color, "fps:%0.1f", fps);
    }
    
	float All() {
        Get();
        Wait();
        Disp();
        return (fps);
    }
    
	void SetDisp(int x, int y, DWORD color) {
        disp_x = x;
        disp_y = y;
        disp_color = color;
    }
};
