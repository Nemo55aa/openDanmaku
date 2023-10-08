// -------------------------------------------------------------------------------------------------
// made by own; by nemo(also known as minato y.)
// first virsion: 2023/09/21
// THIS VER. IS :
//    0.0;
// purpose:
// writing some danmaku Bullets functions;
// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// INFO OF VER 0.0:
//  first made
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// .....Please make sure Copyright to nemo when you using this your project;
// -------------------------------------------------------------------------------------------------
#pragma once

#include "DxLib.h"
#include "utils.h"
#include "dkActor.h"


// ================================各弾共通毎処理================================
// オブジェクトの座標が指定範囲外に出たらTrue
bool isOut(obj chkobj, vector chkSize) {
	if (chkobj.pos.x > chkSize.x)
		return true;
	else if (chkobj.pos.y > chkSize.y)
		return true;
	else if (chkobj.pos.x < 0)
		return true;
	else if (chkobj.pos.y < 0)
		return true;
	else
		return false;
}
bool isOut(obj chkobj, vector chkSize, vector offset) {
	if (chkobj.pos.x > chkSize.x)
		return true;
	else if (chkobj.pos.y > chkSize.y)
		return true;
	else if (chkobj.pos.x < offset.x)
		return true;
	else if (chkobj.pos.y < offset.y)
		return true;
	else
		return false;
}


// nフレーム毎にTrueを返す
// static宣言しているためハンドラを用意している
// 呼び出し下では別々にカウントしたい場合は別なハンドラを用意する必要がある．0～32768
bool trueEachNframe(int timecnt, int n, int handler) {
	static unsigned int prevCnt[255];
	if (timecnt - prevCnt[handler] == n) {
		prevCnt[handler] = timecnt;
		return true;
	}
	else if ((timecnt - prevCnt[handler]) > n) {		// prevCnt前回フレームカウント値リセット
		prevCnt[handler] = timecnt;
		return false;
	}
	else {
		return false;
	}
}


// ================================弾発射動作処理の記述================================
// ---------------- 狙い撃ちの発射（速度ベクトルの計算とセット） ---------------- 
void shootAimingBullet(vector srcPos, vector dstPos, double speed, int hImg, obj blt[]) {
	vector vectTmp;
	int bIdx;
	if (findDisableObj(blt, &bIdx, BULLETS_NUM_MAX) != -1) {
		// 敵座標から発射するよう指定
		blt[bIdx].pos = srcPos;
		// 目標までの距離dを求める
		double d = sqrt((dstPos.x - srcPos.x) * (dstPos.x - srcPos.x) + (dstPos.y - srcPos.y) * (dstPos.y - srcPos.y));
		// 速さが一定値speedになるように速度(vx,vy)を求める：
		// 目標までの距離dが0のときには下方向に発射する。
		if (d) {
			vectTmp.x = (dstPos.x - srcPos.x) / d * speed;
			vectTmp.y = (dstPos.y - srcPos.y) / d * speed;
		}
		else {
			vectTmp.x = 0;
			vectTmp.y = speed;
		}
		// 発射
		PlaySoundMem(blt[bIdx].hsound, DX_PLAYTYPE_BACK);
		blt[bIdx].himg = hImg;
		blt[bIdx].velo = cartesianToPolar(vectTmp);
		blt[bIdx].enable = true;
	}
}
void shootAimingBullet(vector srcPos, vector dstPos, double speed, obj blt[]) {
	vector vectTmp;
	int bIdx;
	if (findDisableObj(blt, &bIdx, BULLETS_NUM_MAX) != -1) {
		// 敵座標から発射するよう指定
		blt[bIdx].pos = srcPos;
		// 目標までの距離dを求める
		double d = sqrt((dstPos.x - srcPos.x) * (dstPos.x - srcPos.x) + (dstPos.y - srcPos.y) * (dstPos.y - srcPos.y));
		// 速さが一定値speedになるように速度(vx,vy)を求める：
		// 目標までの距離dが0のときには下方向に発射する。
		if (d) {
			vectTmp.x = (dstPos.x - srcPos.x) / d * speed;
			vectTmp.y = (dstPos.y - srcPos.y) / d * speed;
		} else {
			vectTmp.x = 0;
			vectTmp.y = speed;
		}
		// 発射
		PlaySoundMem(blt[bIdx].hsound, DX_PLAYTYPE_BACK);
		blt[bIdx].velo = cartesianToPolar(vectTmp);
		blt[bIdx].enable = true;
	}
}


// ---------------- N-way弾を発射 ---------------- 
// srcPos: 敵座標，wayvelo：N-way弾速度（中心）ベクトル, blt: メインルーチンで管理する敵弾配列
void shootNwayBullets(vector srcPos, polarvect wayVelo, double distanceTheta, int n, int hImg, obj blt[]) {
	//vector vectTmp;
	//polarvect* velos;
	polarvect tmpVelo;
	int bIdx;
	// deg2rad convert
	double rad_step = M_PI / 180 * distanceTheta;

	double rad = n % 2 ? n / 2 * rad_step : (n / 2 - 0.5) * rad_step;

	// 一番右の弾から，
	tmpVelo.r = wayVelo.r;
	tmpVelo.t = wayVelo.t - rad;


	// rad_step（指定角度）おきにN個発射する
	for (int i = 0; i < n; i++) {
		if (findDisableObj(blt, &bIdx, BULLETS_NUM_MAX) != -1) {
			PlaySoundMem(blt[bIdx].hsound, DX_PLAYTYPE_BACK);
			// 引数座標から発射するよう指定
			blt[bIdx].pos = srcPos;
			// i = 0で一番右の弾，i = n - 1で一番左の弾
			blt[bIdx].velo = tmpVelo;
			blt[bIdx].enable = true;
			blt[bIdx].himg = hImg;
			tmpVelo.t = tmpVelo.t + rad_step;
		}
	}
}
void shootNwayBullets(vector srcPos, polarvect wayVelo, double distanceTheta, int n, obj blt[]) {
	//vector vectTmp;
	//polarvect* velos;
	polarvect tmpVelo;
	int bIdx;
	// deg2rad convert
	double rad_step = M_PI / 180 * distanceTheta;

	double rad = n % 2 ? n / 2 * rad_step : (n / 2 - 0.5) * rad_step;

	// 一番右の弾から，
	tmpVelo.r = wayVelo.r;
	tmpVelo.t = wayVelo.t - rad;


	// rad_step（指定角度）おきにN個発射する
	for (int i = 0; i < n; i++) {
		if (findDisableObj(blt, &bIdx, BULLETS_NUM_MAX) != -1) {
			PlaySoundMem(blt[bIdx].hsound, DX_PLAYTYPE_BACK);
			// 引数座標から発射するよう指定
			blt[bIdx].pos = srcPos;
			// i = 0で一番右の弾，i = n - 1で一番左の弾
			blt[bIdx].velo = tmpVelo;
			blt[bIdx].enable = true;
			tmpVelo.t = tmpVelo.t + rad_step;
		}
	}
}

// ---------------- 隕石的な弾幕 ---------------- 
void shootCometBullets(vector center, int range, int speed, int timeCnt, int duration, int hImg, obj blt[]) {
	if (trueEachNframe(timeCnt, duration, 32767) == true) {
		vector iposSrc, iposDst;
		iposSrc.y = -80;
		//iposDst.y = HDHEIGHT + 80;
		iposDst.y = 1280 + 80;
		iposSrc.x = (center.x / 2) + (rand() % range) - range / 2;
		iposDst.x = (center.x / 2) + (rand() % range) - range / 2;
		shootAimingBullet(iposSrc, iposDst, rand() % speed + 1, hImg, blt);
	}
}
void shootCometBullets(vector center, int range, int speed, int timeCnt, int duration, int hImg, int hEachTrue, obj blt[]) {
	if (trueEachNframe(timeCnt, duration, hEachTrue) == true) {
		vector iposSrc, iposDst;
		iposSrc.y = -80;
		//iposDst.y = HDHEIGHT + 80;
		iposDst.y = 1280 + 80;
		iposSrc.x = (center.x / 2) + (rand() % range) - range / 2;
		iposDst.x = (center.x / 2) + (rand() % range) - range / 2;
		shootAimingBullet(iposSrc, iposDst, rand() % speed + 1, hImg, blt);
	}
}
void shootCometBullets(vector center, int range, int speed, int timeCnt, int duration, obj blt[]) {
	if (trueEachNframe(timeCnt, duration, 32767) == true) {
		vector iposSrc, iposDst;
		iposSrc.y = -80;
		//iposDst.y = HDHEIGHT + 80;
		iposDst.y = 1280 + 80;
		iposSrc.x = (center.x / 2) + (rand() % range) - range / 2;
		iposDst.x = (center.x / 2) + (rand() % range) - range / 2;
		shootAimingBullet(iposSrc, iposDst, rand() % speed + 1, blt);
	}
}