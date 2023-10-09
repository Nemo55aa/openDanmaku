#pragma once
// -------------------------------------------------------------------------------------------------
// made by own; by nemo(also known as minato y.)
// first virsion: 2023/04/22
// THIS VIR. IS :
//    1.1;
// purpose:
// defining vector type, 2d graphic utilities;
// 0.3�i20230912�j: added pole dimentional vector�i20230912�j
// 1.0�i20230917�j: modified obj's member as:
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

//�ɍ��W�\���́i���aR����ъp�x�Ɓj
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
    vector hitSize; // �����蔻��T�C�Y
    int hImg;       // �e�摜�n���h��
    int hSnd;       // �V���b�g�����ʉ�
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


//obj�z�񂩂�g���Ă��Ȃ��e�i�I�u�W�F�N�g�j��T���C���̃C���f�b�N�X�������Ɋi�[
//�g���Ă���e(�I�u�W�F�N�g)�����݂��Ȃ��ꍇ�́C�|1��Ԃ��D                           �i�g����Enable)
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

// �t���[�����J�E���g�A�b�v�����timeCnt��b�P�ʂɕϊ�
int ctoi(int timeCnt) {
    int rtnval;
    // 60fpsOnly
    rtnval = timeCnt * 0.016;
    return rtnval;
}