#pragma once
// -------------------------------------------------------------------------------------------------
// made by own; by nemo(also known as minato y.)
// first virsion: 2023/04/22
// THIS VIR. IS :
//    1.1;
// purpose:
// defining vector type, 2d graphic utilities;
// 0.3（20230912）: added pole dimentional vector（20230912）
// 1.0（20230917）: modified obj's member as:
//                      vector velo;    =>      polarvect velo;
// 1.1 (20231001) : added framecounter(timeCnt) to second converting function;
// .....Please make sure Copyright to nemo when you using this your project;
// -------------------------------------------------------------------------------------------------
#pragma once

#include "DxLib.h"
#include "frameCtrl.h"
#include <math.h>

typedef  struct {
    double x, y;
}vector;

//極座標構造体（半径Rおよび角度θ）
typedef struct {
    double r, t;
}polarvect;


typedef struct {
    //int handler;
    vector pos;
    polarvect velo;
    vector size;
    int himg;
    int hsound;
    bool enable;
}obj;

/*
typedef struct {
    obj bltobj;
    vector hitSize; // 当たり判定サイズ
    int hImg;       // 弾画像ハンドラ
    int hSnd;       // ショット時効果音
}bullet;*/

vector addvect(vector a, vector b) {
    vector temp;
    temp.x = a.x + b.x;
    temp.y = a.y + b.y;
    return temp;
}
vector subvect(vector a, vector b) {
    vector temp;
    temp.x = a.x - b.x;
    temp.y = a.y - b.y;
    return temp;
}
vector timvect(vector a, int b) {
    vector temp;
    temp.x = a.x * b;
    temp.y = a.y * b;
    return temp;
}
vector devvect(vector a, int b) {
    vector temp;
    temp.x = a.x / b;
    temp.y = a.y / b;
    return temp;
}

vector polarToCartesian(polarvect a) {
    vector temp;
    temp.x = (double)a.r * cos(a.t);
    temp.y = (double)a.r * sin(a.t);
    return temp;
}
polarvect cartesianToPolar(vector a) {
    polarvect temp;
    temp.r = sqrt(a.x * a.x + a.y * a.y);
    temp.t = atan2(a.y, a.x);
    return temp;
}


//obj配列から使われていない弾（オブジェクト）を探し，そのインデックスを引数に格納
//使われている弾(オブジェクト)が存在しない場合は，－1を返す．                           （使う＝Enable)
int findDisableObj(obj objary[], int* num, int elCnt) {
    int idx;
    for (idx = 0; idx < elCnt; idx++) {
        if (objary[idx].enable == false) {
            *num = idx;
            return 0;
        }
    }
    if (idx == elCnt) {
        printfDx("bullet is FULL!!!!");
        return -1;
    }
}

class scnControl :public fpsCtrl
{
private:
public:
    scnControl();
    scnControl(float setfps, float setupdate);
    // call this each end of your scene loop
    void lend();
};

scnControl::scnControl() {
    SetDrawScreen(DX_SCREEN_BACK);
    fpsCtrl::SetDisp(1280 - 80, 720 - 20, GetColor(255, 255, 255));
}

scnControl::scnControl(float setfps, float setupdate) {
    SetDrawScreen(DX_SCREEN_BACK);
    fpsCtrl(setfps, setupdate);
    fpsCtrl::SetDisp(640 - 80, 480 - 20, GetColor(255, 255, 255));
}

//call this end of your while loop
void scnControl::lend() {
    fpsCtrl::All();
    ScreenFlip();
    ClearDrawScreen();
}

// フレーム毎カウントアップされるtimeCntを秒単位に変換
int ctoi(int timeCnt) {
    int rtnval;
    // 60fpsOnly
    rtnval = timeCnt * 0.016;
    return rtnval;
}