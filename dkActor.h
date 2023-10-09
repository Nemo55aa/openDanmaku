// -------------------------------------------------------------------------------------------------
// made by own; by nemo(also known as minato y.)
// first virsion: 2023/04/22
// THIS VER. IS :
//    2.0;
// purpose:
// defining some DANMAKU stuff;
// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// INFO OF VER 1.0:
//  updated from 0.1
//  added ....
//     1. shooting bullets function to player class
// INFO OF VER 1.1:
//  updated from 1.0
//  added ....
//     1. support polar vector struction(util.h was also updated)（20230912）
// INFO OF VER 2.0:
//  updated from 2.0
//  added ....
//     1. support polar velocity which obj's member
//     2. fixed player's setvelo func's some problem
//     3. * modified actor class
//              private param     => protected param, 
//              private setsize() => protected setsize();
//        * added enemy method 
//              sethitsize(vector hitsizeIn);
//              isHit(vector bltpos);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// .....Please make sure Copyright to nemo when you using this your project;
// -------------------------------------------------------------------------------------------------
#pragma once

#include "DxLib.h"
#include "utils.h"
#include "ovlovrDx.h"
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#include "stdio.h"
#endif

/*
//LSB
bool front : 1;
bool fright : 1;
bool right : 1;
bool bright : 1;
bool back : 1;
bool bleft : 1;
bool left : 1;
bool fleft : 1;
//MSB
*/

#define DIRF 0x01
#define DIRB 0x10
#define DIRR 0x04
#define DIRL 0x40

#define DIRFR 0x02
#define DIRFL 0x80
#define DIRBR 0x08
#define DIRBL 0x20


/// ====================================================
/// actor Class
/// ====================================================
class actor {
private:
    obj param;
    vector hitsize;
    void mov();
    void drw();
public:
    actor();
    actor(int h);
    void Init();
    void Init(int h);
    void setimg(int h);
    void setpos(vector actposition);
    void setsize(vector size);

    void setvelo(vector actvelocity);
    void setvelo(polarvect actvelocity);
    void setveloR(double r);
    void setveloT(double t);
    
    void getpos(vector* vect);
    void getvelo(vector* velo);
    void getvelo(polarvect* velo);
    void getsize(vector* size);
    void Act();

    void sethitsize(vector hitsizeIn);
    bool isHit(vector bltpos);
    bool isHit(obj blt);
};

actor::actor() {
    actor::Init();
}
actor::actor(int h) {
    actor::Init(h);
}
void actor::Init() {
    vector tempv;
    tempv = { 0, 0 };
    setvelo(tempv);
    setpos(tempv);
}
void actor::Init(int h) {
    vector tempv;
    tempv = { 0, 0 };
    setvelo(tempv);
    setpos(tempv);
    setimg(h);
}

void actor::setsize(vector size) {
    actor::param.size = size;
}

void actor::setimg(int h) {
    int wtemp, htemp;
    vector temp;
    actor::param.himg = h;
    GetGraphSize(h, &wtemp, &htemp);
    temp.x = wtemp;
    temp.y = htemp;
    actor::setsize(temp);
}
void actor::setpos(vector actposition) {
    actor::param.pos = subvect(actposition, devvect(actor::param.size, 2));
}
void actor::setvelo(vector actvelocity) {
    actor::param.velo = cartesianToPolar(actvelocity);
}
void actor::setvelo(polarvect actvelocity) {
    actor::param.velo = actvelocity;
}
void actor::setveloR(double r) {
    actor::param.velo.r = r;
}
void actor::setveloT(double t) {
    actor::param.velo.t = t;
}
void actor::getpos(vector* vect) {
    *vect = addvect(actor::param.pos, devvect(actor::param.size, 2));
}
void actor::getvelo(vector* velo) {
    *velo = polarToCartesian(actor::param.velo);
}
void actor::getvelo(polarvect* velo) {
    *velo = actor::param.velo;
}
void actor::getsize(vector* size) {
    *size = actor::param.size;
}

void actor::sethitsize(vector size) {
    actor::hitsize = size;
}

bool actor::isHit(vector bltpos) {
    vector postemp;
    actor::getpos(&postemp);
#ifdef _DEBUG
    //DrawCircle(postemp.x, postemp.y, actor::hitsize.x / 2, 0xffffff, false);
    //DrawBox(postemp.x, postemp.y, postemp.x + 1, postemp.y + 1, 0xffffff, true);
#endif
    if ((bltpos.x < postemp.x + hitsize.x / 2) && 
        bltpos.x > postemp.x - hitsize.x / 2 &&
        bltpos.y < postemp.y + hitsize.y / 2 && 
        bltpos.y > postemp.y - hitsize.y / 2) {
            return true;
    }
    else {
        return false;
    }
}
bool actor::isHit(obj blt) {
    if (blt.enable == true) {
        return isHit(blt.pos);
    }
    else {
        return false;
    }
}

void actor::mov() {
    param.pos = addvect(actor::param.pos, polarToCartesian(actor::param.velo));
}
void actor::drw() {
    DrawGraph(actor::param.pos, actor::param.himg, true);
}
void actor::Act() {
    actor::mov();
    actor::drw();
}


typedef struct {
    //LSB
    bool front  : 1;
    bool fright : 1;
    bool right  : 1;
    bool bright : 1;
    bool back   : 1;
    bool bleft  : 1;
    bool left   : 1;
    bool fleft  : 1;
    //MSB
}movbit;

typedef union {
    UINT8 bytedir;
    movbit bitdir;
}dirstate;


/// ----------------------------------------------------
/// Player Class
/// ----------------------------------------------------
class player :public actor {
private:
    // 自機移動速度（普通・低速）
    double normalR = 1, slowR = 0.5;  
    bool slowFlag = false;          // Trueで低速移動
    bool isshoting = false;         // Trueで発射
    vector moveAreaSize = { 600, 700 };
    vector moveAreaOffset = { 10 , 10 };
    dirstate pdir;

    bool isOutArea();               // 移動エリア（弾幕範囲）からの逸脱かを判定
    void moveBack();             // isOutAreaなら移動させない
public:
    unsigned char charge = 0;       // ショット釦が押下されている間カウントアップ⇒キャラに応じた閾値でショット発射
    unsigned char chargeth;         // ショットの間隔（単位：フレーム）
    player();
    player(int hgraph);
    player(int hgraph, double fastV, double slowV);
    
    void setchargeTh(unsigned char chgThreshold);   // 何フレーム毎にショットを発射するか設定
    void setVeloLen();              // 速度ベクトル長さ指定
    void setdir();                  // 速度ベクトル角度（方向）指定
    void setMoveArea(vector areaSize, vector offset);


    void checkSlowPad(int joypadstate, int padnum);
    void checkdirPad(int joypadstate, int padnum);
    void checkBombPad(int joypadstate, int padnum, int* bombflag);
    void checkKeypad();

    // 諸々すべて実行する系のメソッド（フレーム毎に呼び出される）
    void pAct(int joypadstate, int padnum);
    void shot(obj bullets[]);
};

player::player() {
    actor::Init();
}
player::player(int hgraph) {
    actor::Init(hgraph);
}
player::player(int hgraph, double fastR, double slowR) {
    actor::Init(hgraph);
    player::slowR = slowR;
    player::normalR = fastR;
}

void player::setMoveArea(vector areaSize, vector offset) {
    player::moveAreaSize = areaSize;
    player::moveAreaOffset = offset;
}

bool player::isOutArea() {
    vector tmpPos;
    player::getpos(&tmpPos);
    if (tmpPos.x > (player::moveAreaSize.x + player::moveAreaOffset.x)) {
        tmpPos.x = player::moveAreaOffset.x + player::moveAreaSize.x - 1;
        player::setpos(tmpPos);
        return true;
    }
    else if (tmpPos.x < player::moveAreaOffset.x) {
        tmpPos.x = player::moveAreaOffset.x + 1;
        player::setpos(tmpPos);
        return true;
    }
    else if (tmpPos.y > (player::moveAreaSize.y + player::moveAreaOffset.y)) {
        tmpPos.y = player::moveAreaOffset.y + player::moveAreaSize.y - 1;
        player::setpos(tmpPos);
        return true;
    }
    else if (tmpPos.y < player::moveAreaOffset.y) {
        tmpPos.y = player::moveAreaOffset.y + 1;
        player::setpos(tmpPos);
        return true;
    }
    else {
        return false;
    }
}
void player::moveBack() {
    if (player::isOutArea() == true) {
        vector tmpPos;
        polarvect tmpvelo;
        player::getpos(&tmpPos);
        player::getvelo(&tmpvelo);
    }   
}

void player::checkSlowPad(int joypadstate, int padnum = 0) {
    if (padnum > 0) {
        if (joypadstate & 0x200 || CheckHitKey(KEY_INPUT_RSHIFT) == true) {
            player::slowFlag = true;
        }
        else
        {
            player::slowFlag = false;
        }
    }
}
void player::checkdirPad(int joypadstate, int padnum = 0) {
    if (padnum > 0) {
        player::pdir.bytedir = 0x00;
        switch (joypadstate & 0x0f) {
        case 0x01:
            player::pdir.bitdir.back = true;
            break;
        case 0x02:
            player::pdir.bitdir.left = true;
            break;
        case 0x04:
            player::pdir.bitdir.right = true;
            break;
        case 0x08:
            player::pdir.bitdir.front = true;
            break;
        case 0x03:
            player::pdir.bitdir.bleft = true;
            break;
        case 0x05:
            player::pdir.bitdir.bright = true;
            break;
        case 0xa:
            player::pdir.bitdir.fleft = true;
            break;
        case 0xc:
            player::pdir.bitdir.fright = true;
            break;
        case 0x0:
            player::pdir.bytedir = 0x00;
            break;
        }
    }
}

void player::checkKeypad() {
    player::pdir.bytedir = 0x00;
    /*switch (joypadstate & 0x0f) {
    case 0x01:
        player::pdir.bitdir.back = true;
        break;
    case 0x02:
        player::pdir.bitdir.left = true;
        break;
    case 0x04:
        player::pdir.bitdir.right = true;
        break;
    case 0x08:
        player::pdir.bitdir.front = true;
        break;
    case 0x03:
        player::pdir.bitdir.bleft = true;
        break;
    case 0x05:
        player::pdir.bitdir.bright = true;
        break;
    case 0xa:
        player::pdir.bitdir.fleft = true;
        break;
    case 0xc:
        player::pdir.bitdir.fright = true;
        break;
    case 0x0:
        player::pdir.bytedir = 0x00;
        break;
    }*/

}

void player::setVeloLen() {
    polarvect temp;
    switch (player::pdir.bytedir) {
    case DIRF:
        setveloR(slowFlag ? slowR : normalR);
        break;
    case DIRB:
        setveloR(slowFlag ? slowR : normalR);
        break;
    case DIRR:
        setveloR(slowFlag ? slowR : normalR);
        break;
    case DIRL:
        setveloR(slowFlag ? slowR : normalR);
        break;
    case DIRFR:
        setveloR(slowFlag ? slowR : normalR);
        break;
    case DIRFL:
        setveloR(slowFlag ? slowR : normalR);
        break;
    case DIRBR:
        setveloR(slowFlag ? slowR : normalR);
        break;
    case DIRBL:
        setveloR(slowFlag ? slowR : normalR);
        break;
    default:
        setveloR(0);
    }
}

void player::setdir() {
    polarvect temp;
    switch (player::pdir.bytedir) {
    case DIRF:
        temp.t = M_PI * 3 / 2;
        player::setveloT(temp.t);
        break;
    case DIRB:
        temp.t = M_PI / 2;
        player::setveloT(temp.t);
        break;
    case DIRR:
        temp.t = 0;
        player::setveloT(temp.t);
        break;
    case DIRL:
        temp.t = M_PI;
        player::setveloT(temp.t);
        break;
    case DIRFR:
        temp.t = M_PI * 7 / 4;
        player::setveloT(temp.t);
        break;
    case DIRFL:
        temp.t = M_PI * 5 / 4;
        player::setveloT(temp.t);
        break;
    case DIRBR:
        temp.t = M_PI / 4;
        player::setveloT(temp.t);
        break;
    case DIRBL:
        temp.t = M_PI * 3 / 4;
        player::setveloT(temp.t);
        break;
    default:
        temp.t = M_PI * 3 / 2;
        player::setveloT(temp.t);
    }
}

void player::pAct(int joypadstate, int padnum) {
    player::checkSlowPad(joypadstate, padnum);
    player::checkdirPad(joypadstate, padnum);
    player::setdir();
    player::setVeloLen();
    player::moveBack();
    actor::Act();
}

void player::shot(obj bullets[]) {
    int idx;
    vector postemp;
    // 使われていないショットデータを探す
    // もし使われていないショットデータがあったらショットを出す
    if (findDisableObj(bullets, &idx, BULLETS_NUM_MAX) != -1)
    {
        int bltWidth, bltHeight;
        player::getpos(&postemp);
        //右側
        GetGraphSize(bullets[idx].himg, &bltWidth, &bltHeight);
        bullets[idx].pos.x = postemp.x - bltWidth / 2 + 10;
        bullets[idx].pos.y = postemp.y;
        bullets[idx].enable = true;

        //左側
        findDisableObj(bullets, &idx, BULLETS_NUM_MAX);
        bullets[idx].pos.x = postemp.x - bltWidth / 2 - 10;
        bullets[idx].pos.y = postemp.y;
        bullets[idx].enable = true;

    }
}

void checkBombPad(int joypadstate, int padnum, int* bombflag) {
    if (padnum > 0) {

        if (joypadstate & 0x40) {
            *bombflag = true;
        }
        else {
            *bombflag = false;
        }
    }
}

void player::setchargeTh(unsigned char chgThreshold) {
    player::chargeth = chgThreshold;
}

/// ----------------------------------------------------
/// Enemy Class
/// ----------------------------------------------------
class enemy :public actor {
private:
    //vector hitsize;
public:
    int life;
    obj pblt;
    enemy(int hgraph);
    
    //bool isHit(vector bltpos);
    void eAct();
};

enemy::enemy(int hgraph) {
    actor::Init(hgraph);
}


void enemy::eAct() {
    actor::Act();
}

