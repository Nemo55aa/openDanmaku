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


// ================================�e�e���ʖ�����================================
// �I�u�W�F�N�g�̍��W���w��͈͊O�ɏo����True
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


// n�t���[������True��Ԃ�
// static�錾���Ă��邽�߃n���h����p�ӂ��Ă���
// �Ăяo�����ł͕ʁX�ɃJ�E���g�������ꍇ�͕ʂȃn���h����p�ӂ���K�v������D0�`32768
bool trueEachNframe(int timecnt, int n, int handler) {
	static unsigned int prevCnt[255];
	if (timecnt - prevCnt[handler] == n) {
		prevCnt[handler] = timecnt;
		return true;
	}
	else if ((timecnt - prevCnt[handler]) > n) {		// prevCnt�O��t���[���J�E���g�l���Z�b�g
		prevCnt[handler] = timecnt;
		return false;
	}
	else {
		return false;
	}
}


// ================================�e���˓��쏈���̋L�q================================
// ---------------- �_�������̔��ˁi���x�x�N�g���̌v�Z�ƃZ�b�g�j ---------------- 
void shootAimingBullet(vector srcPos, vector dstPos, double speed, int hImg, obj blt[]) {
	vector vectTmp;
	int bIdx;
	if (findDisableObj(blt, &bIdx, BULLETS_NUM_MAX) != -1) {
		// �G���W���甭�˂���悤�w��
		blt[bIdx].pos = srcPos;
		// �ڕW�܂ł̋���d�����߂�
		double d = sqrt((dstPos.x - srcPos.x) * (dstPos.x - srcPos.x) + (dstPos.y - srcPos.y) * (dstPos.y - srcPos.y));
		// ���������lspeed�ɂȂ�悤�ɑ��x(vx,vy)�����߂�F
		// �ڕW�܂ł̋���d��0�̂Ƃ��ɂ͉������ɔ��˂���B
		if (d) {
			vectTmp.x = (dstPos.x - srcPos.x) / d * speed;
			vectTmp.y = (dstPos.y - srcPos.y) / d * speed;
		}
		else {
			vectTmp.x = 0;
			vectTmp.y = speed;
		}
		// ����
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
		// �G���W���甭�˂���悤�w��
		blt[bIdx].pos = srcPos;
		// �ڕW�܂ł̋���d�����߂�
		double d = sqrt((dstPos.x - srcPos.x) * (dstPos.x - srcPos.x) + (dstPos.y - srcPos.y) * (dstPos.y - srcPos.y));
		// ���������lspeed�ɂȂ�悤�ɑ��x(vx,vy)�����߂�F
		// �ڕW�܂ł̋���d��0�̂Ƃ��ɂ͉������ɔ��˂���B
		if (d) {
			vectTmp.x = (dstPos.x - srcPos.x) / d * speed;
			vectTmp.y = (dstPos.y - srcPos.y) / d * speed;
		} else {
			vectTmp.x = 0;
			vectTmp.y = speed;
		}
		// ����
		PlaySoundMem(blt[bIdx].hsound, DX_PLAYTYPE_BACK);
		blt[bIdx].velo = cartesianToPolar(vectTmp);
		blt[bIdx].enable = true;
	}
}


// ---------------- N-way�e�𔭎� ---------------- 
// srcPos: �G���W�Cwayvelo�FN-way�e���x�i���S�j�x�N�g��, blt: ���C�����[�`���ŊǗ�����G�e�z��
void shootNwayBullets(vector srcPos, polarvect wayVelo, double distanceTheta, int n, int hImg, obj blt[]) {
	//vector vectTmp;
	//polarvect* velos;
	polarvect tmpVelo;
	int bIdx;
	// deg2rad convert
	double rad_step = M_PI / 180 * distanceTheta;

	double rad = n % 2 ? n / 2 * rad_step : (n / 2 - 0.5) * rad_step;

	// ��ԉE�̒e����C
	tmpVelo.r = wayVelo.r;
	tmpVelo.t = wayVelo.t - rad;


	// rad_step�i�w��p�x�j������N���˂���
	for (int i = 0; i < n; i++) {
		if (findDisableObj(blt, &bIdx, BULLETS_NUM_MAX) != -1) {
			PlaySoundMem(blt[bIdx].hsound, DX_PLAYTYPE_BACK);
			// �������W���甭�˂���悤�w��
			blt[bIdx].pos = srcPos;
			// i = 0�ň�ԉE�̒e�Ci = n - 1�ň�ԍ��̒e
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

	// ��ԉE�̒e����C
	tmpVelo.r = wayVelo.r;
	tmpVelo.t = wayVelo.t - rad;


	// rad_step�i�w��p�x�j������N���˂���
	for (int i = 0; i < n; i++) {
		if (findDisableObj(blt, &bIdx, BULLETS_NUM_MAX) != -1) {
			PlaySoundMem(blt[bIdx].hsound, DX_PLAYTYPE_BACK);
			// �������W���甭�˂���悤�w��
			blt[bIdx].pos = srcPos;
			// i = 0�ň�ԉE�̒e�Ci = n - 1�ň�ԍ��̒e
			blt[bIdx].velo = tmpVelo;
			blt[bIdx].enable = true;
			tmpVelo.t = tmpVelo.t + rad_step;
		}
	}
}

// ---------------- 覐ΓI�Ȓe�� ---------------- 
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