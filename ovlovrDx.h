#pragma once
// -------------------------------------------------------------------------------------------------
// made by own; by nemo(also known as minato y.)
// first virsion: 2023/04/22
// THIS VIR. IS :
//    0.1;
// purpose:
// Overloading/Overriding
//
// .....Please make sure Copyright to nemo when you using this your project;
// -------------------------------------------------------------------------------------------------

#include "DxLib.h"
#include "utils.h"

// overloading function uses vector
int DrawGraph(vector pos, int GrHandle, int TransFlag) {
    return DrawGraph((int)pos.x, (int)pos.y, GrHandle, TransFlag);
}
int DrawPixel(vector pos, unsigned int clr) {
    return DrawPixel(pos.x, pos.y, clr);
}

/*
int GetGraphSize(int GrHandle, vector* sizebuf) {
    int tmpx, tmpy;
    int rtnVal;
    rtnVal = GetGraphSize(GrHandle, (int*)&sizebuf->x, (int*)&sizebuf->y);
    //sizebuf->x = tmpx;
    //sizebuf->y = tmpx;
    return rtnVal;
}*/