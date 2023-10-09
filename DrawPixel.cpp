// VER. 0.0 
// Wrote : nemo


#define BULLETS_NUM_MAX 1024

#include <math.h>
#include "DxLib.h"
#include "utils.h"
#include "dkActor.h"
#include "bullets.h"

#include "scripts.h"

#define MAXBUF 256
#define MAX_SHOT 256
#define CENTER 640,360
#define HDWIDTH 1280
#define HDHEIGHT 720

#define SCRIPTPATH "C:/script/s1.text"

#define MSG_RETRY 255
#define MSG_EXIT 0
//#define BULLETS_NUM_MAX 1000

// ===== PROTOTYPE DEFINITION ===== 
void defeat(int defeatSound, int* msg);
void pause(int* msg);
void moveBG(int himg, int velo, int* bgyPos);
void bomb(obj bullets[]);
void scoreTime(int timecnt); 
void displayKeyDescription();

// ===== CLASS DEFINITION (for test) ===== 
class kaiwaClass {
private:
	int backImg;
	int bgm;
	int chara1img;
	int chara2img;

public:
	kaiwaClass(int bgImg, int bgmH, int c1img, int c2img);
	void chara1hide();
	void chara2hide();
	void chara1show();
	void cahra2show();
};

// ===== SCENE DEFINITION ===== 
void kaiwa();
void hoge();
void danmakuScene(int *msg);


void hoge() {
	DrawString(0, 0, "�����L�[�F�ړ�\nZ�L�[�F�V���b�g�i�G�ɂ��Ă��邩�͂Ƃ������D�D�D\n�V�t�g�L�[�F�X���E\n�K�v�Ȃ��́F�g������������x�r�q�Ȗ�", 0xffcc22, 0x000000);
	ScreenFlip();
	Sleep(10000);
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �E�C���h�E�T�C�Y��HD�ɐݒ�
	SetGraphMode(HDWIDTH, HDHEIGHT, 32, 60);

	// �^���l�e��STG�ł悭����N���O�̃��b�Z�[�W����
	int fullscrnFlg = IDNO;
	//int fullscrnFlg = IDYES;
	fullscrnFlg = MessageBox(NULL, TEXT("�t���X�N���[�����[�h�ŋN�����܂����H"),
		TEXT("sample"), MB_YESNO | MB_ICONQUESTION);

	if (fullscrnFlg == IDYES) {
		ChangeWindowMode(false);
	}
	else
	{
		ChangeWindowMode(true);
	}
	if (DxLib_Init() == -1) { return -1; }

	int msg = 0;

	// ����������
	displayKeyDescription();
	// -- �{�� --
	SetMouseDispFlag(false);
	do{
		danmakuScene(&msg);
	} while (msg == MSG_RETRY);
	SetMouseDispFlag(true);

	DxLib_End();
	return 0;				// �\�t�g�̏I�� 
}

void danmakuScene(int* msg) {

	int timeCnt = 0;
	int prevTcnt = 0;
	// sprintf�p
#ifdef _DEBUG
	char buf[MAXBUF];
#endif // DEBUG
	// tmp
	int sizeXtmp, sizeYtmp;
	vector postemp, velotemp;
	vector  pPosTmp, ePosTmp;
	int tmpmsg;
	int htempImg;	//�摜���[�f�B���O�p
	int htempSnd;	//�������[�f�B���O�p
	int redB, blueB, cometB;
	redB = LoadGraph("img/b3.png");
	blueB = LoadGraph("img/b2.png");
	cometB = LoadGraph("img/b4.png");
	// ==================== bullets ====================
	double vsize;
	int cntBltNum = 0, himgb2;
	int i = 0;
	int j = 0;
	// bulletsOBJ init
	obj bullets[BULLETS_NUM_MAX];	//�G�e(�ہj
	obj pblt[BULLETS_NUM_MAX];		//���@�e
	//�G�e������
	htempImg = LoadGraph("img/b2.png");
	htempSnd = LoadSoundMem("sounds/noiseFadeout.wav");
	for (i = 0; i < 255; i++) {
		bullets[i].enable = false;
		bullets[i].himg = htempImg;
		bullets[i].hsound = htempSnd;
		bullets[i].pos = { -100, -100 };
		GetGraphSize(htempImg, &sizeXtmp, &sizeYtmp);
		bullets[i].size = { (double)sizeXtmp, (double)sizeYtmp };
	}

	//���@�e������
	htempImg = LoadGraph("img/b1.png", true);
	for (i = 0; i < BULLETS_NUM_MAX; i++) {
		pblt[i].enable = false;
		pblt[i].himg = htempImg;
		pblt[i].velo.r = 12;
		pblt[i].velo.t = 1.5 * M_PI;
	}

	// N�t���[���������O��t���[���J�E���^�l�ۑ��p
	int prevFcounter[255];
	for (i = 0; i < 255; i++) {
		prevFcounter[i] = 0;
	}

	int prevCntEnd = 0;
	int cometDuration = 0;
	int bgyPos = HDHEIGHT;
	

	// ==================== Scene ====================
	scnControl scnsample;
	int backImg = LoadGraph("img/stage1.png", false);

	// ==================== Player ====================
	// �v���C���[�摜
	htempImg = LoadGraph("img/player.png", false);
	//player p1(htempImg);
	player p1(htempImg, 10.0, 5);
	p1.setveloR(0.0);
	p1.setveloT(0.0);
	p1.setchargeTh(6);
	p1.sethitsize({ 10, 10 });
	p1.setpos({ CENTER }); 
	p1.setMoveArea({ 500, HDHEIGHT }, { (HDWIDTH - 500) / 2, 0 });
	bool defFlag = false;
	int playerDefeat = LoadSoundMem("sounds/playerDefeat.wav");

	// ==================== Enemy ====================
	// enemy instance
	htempImg = LoadGraph("img/e1.png", false);
	enemy e1(htempImg);
	postemp = { 25, 25 };
	e1.sethitsize(postemp);
	int enemyDefet = LoadSoundMem("sounds/defeatEnemy.wav");
	e1.life = 5000;
	postemp = { HDWIDTH / 2, 100 };
	e1.setpos(postemp);

	PlaySound("sounds/mooonlightCS.wav", DX_PLAYTYPE_BACK);

	int padstate = 0, oldpadstate;
	while (ProcessMessage() == 0)
	{
		oldpadstate = padstate;
		padstate = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		//printfDx("padstate: %4x\n", padstate);

		// pause�{�^���`�F�b�N�ƃ��[�`���ւ̈ړ�
		if (padstate & 0x800) {
			return pause(msg);
		}

		if (padstate & 0x40) {
			bomb(bullets);
		}
		//printfDx("bgypos: %d\n", bgyPos);
		moveBG(backImg, 8, &bgyPos);

#ifdef _DEBUG
		wsprintf(buf, "enemy life: %d", e1.life);
		DrawString(0, 0, buf, 0xffffff, 0);
		e1.getpos(&postemp);
		wsprintf(buf, "enemy pos: %d, %d", postemp.x, postemp.y);
		DrawString(0, 15, buf, 0xffffff, 0);
		p1.getpos(&postemp);
		wsprintf(buf, "player pos: %d, %d", postemp.x, postemp.y);
		DrawString(0, 30, buf, 0xffffff, 0);
		//printfDx("timeCnt: %6d\n", timeCnt);
		//printfDx("time %d sec\n", ctoi(timeCnt));
#endif // DEBUG

		// TODO: �����ɃR�[�h��ǉ�

		//LoadGraphScreen(0, 0, "img/crosshair_fullHD.png", false);
		p1.pAct(padstate, GetJoypadNum());
		e1.eAct();

		p1.getpos(&pPosTmp);
		e1.getpos(&ePosTmp);
		DrawBox(pPosTmp.x - 5, pPosTmp.y - 5, pPosTmp.x + 5, pPosTmp.y + 5, 0xff0000, true);

		// ���߂�n-way�e
		if (ctoi(timeCnt) <= 8 && ctoi(timeCnt) % 3) {
			if (trueEachNframe(timeCnt, 9, &prevFcounter[0]) == true) {
				// N-way�e�𔭎�
				shootNwayBullets(ePosTmp, { 3.4, M_PI / 2 }, 15, 8, blueB, bullets);
			}
		}

		// 覐ΓI�Ȓe���P�i�ɂ߁j
		if (ctoi(timeCnt) <= 10) {
			shootCometBullets({ CENTER }, 1000, rand() % 6 + 1, timeCnt, 9, redB, &prevFcounter[1],  bullets);
		}
		// 覐ΓI�Ȓe���Q�i���߁j
		if (ctoi(timeCnt) >= 11 && ctoi(timeCnt) <= 21) {
			shootCometBullets({ CENTER }, 1000, rand() % 8 + 3, timeCnt, 10, blueB, &prevFcounter[2], bullets);
			shootCometBullets({ CENTER }, 1000, rand() % 8 + 3, timeCnt, 10, blueB, &prevFcounter[3], bullets);
		}

		if (ctoi(timeCnt) >= 22 && ctoi(timeCnt) <= 27) {
			// 20�t���[������
			if (trueEachNframe(timeCnt, 20, &prevFcounter[3]) == true) {
				// N-way�e�𔭎�
				shootAimingBullet(ePosTmp, pPosTmp, 3.4, bullets);
				shootNwayBullets(ePosTmp, { 3.4, M_PI / 2 }, 10, 9, bullets);
			}
			if (trueEachNframe(timeCnt, 30, &prevFcounter[4]) == true && timeCnt % 3) {
				int randnum = rand() % 3 + 1;
				shootNwayBullets({ ePosTmp.x + 250, ePosTmp.y }, { (double)randnum, M_PI / 2 }, 20, 3, bullets);
				shootNwayBullets({ ePosTmp.x - 250, ePosTmp.y }, { (double)randnum, M_PI / 2 }, 20, 3, bullets);
			}
		}

		if (ctoi(timeCnt) > 28 && ctoi(timeCnt) <= 40) {
			// 20�t���[������
			if (trueEachNframe(timeCnt, 20, &prevFcounter[5]) == true) {
				// �_�������e�𔭎�
				shootAimingBullet({ ePosTmp.x + 100, 100 }, pPosTmp, 5, bullets);
				shootAimingBullet({ ePosTmp.x - 100, 100 }, pPosTmp, 5, bullets);
			}
			if (trueEachNframe(timeCnt, 10, &prevFcounter[6]) == true && ctoi(timeCnt) %2) {
				shootNwayBullets(ePosTmp, { 3.4, M_PI / 2 }, 10, 36, blueB, bullets);
			}
		}

		// �ǂ�ǂ�e�̖��x���オ��
		if (ctoi(timeCnt) > 41 && ctoi(timeCnt) <= 43) {
			shootCometBullets({ CENTER }, 1000, 10, timeCnt, 10, blueB, &prevFcounter[7], bullets);
		}
		if (ctoi(timeCnt) > 43 && ctoi(timeCnt) <= 44) {
			shootCometBullets({ CENTER }, 1000, 10, timeCnt, 5, blueB, &prevFcounter[7], bullets);
		}
		if (ctoi(timeCnt) > 44) {
			shootCometBullets({ CENTER }, 1000, 10, timeCnt, 2, redB, &prevFcounter[7], bullets);
		}

		
		// ================================�e�����蔻��t���[��������================================
		// ------------ ���@�eto�G -----------
		for (i = 0; i < BULLETS_NUM_MAX; i++) {
			if (e1.isHit(pblt[i]) == true) {
				pblt[i].enable = false;
				e1.life -= 3;
			}
		}
		// --- enemy life check ---
		if (e1.life <= 0) {
			//defeat(enemyDefet);
			scoreTime(timeCnt);
			*msg = MSG_EXIT; 
			return;
		}
		// ------------ �G�eto���@ ------------
		for (i = 0; i < BULLETS_NUM_MAX; i++) {
			if (p1.isHit(bullets[i]) == true) {
				//p1.defeatanim();
				
				/*DrawFormatString(CENTER, 0xffffff, "you hitten by bullets[%d]....GAME IS OVER!!!\n", i);
				DrawFormatString(CENTER + 15, 0xffffff, "hit pos X: %f, Y: %f\n", postemp.x, postemp.y);
				DrawFormatString(CENTER + 30, 0xffffff, "bullets[%d]'s pos X: %f, Y: %f\n", i, bullets[i].pos.x, bullets[i].pos.y);
				ScreenFlip();
				//while (CheckHitKeyAll() == 0);
				*/
				defFlag = true;
			}
		}
		if (defFlag == true) {
			defeat(playerDefeat, msg);
			break;
		}

		// ================================���@�e�t���[��������================================
		if (padstate & PAD_INPUT_A) {
			p1.charge++;
			if (p1.charge >= p1.chargeth) {
				p1.charge = 0;
				p1.shot(pblt);
			}
		}

		// ���˂��ꂽ�e�ipblt[n]�j�����̑��x�x�N�g�����i�߂�
		for (i = 0; i < BULLETS_NUM_MAX; i++) {
			if (pblt[i].enable == true) {
				pblt[i].pos = addvect(pblt[i].pos, polarToCartesian(pblt[i].velo));
				if (isOut(pblt[i], { HDWIDTH + 100, HDHEIGHT + 100 }, { -100, -100 }) == true)
					pblt[i].enable = false;		// ��ʊO�ɏo���疳����
			}
		}

		// �L���ȁi���˂��ꂽ�j�e�̕`��
		for (i = 0; i < BULLETS_NUM_MAX; i++)
		{
			// �V���b�g�f�[�^���L���Ȏ��̂ݕ`��
			if (pblt[i].enable == true){
				DrawGraph(pblt[i].pos.x, pblt[i].pos.y, pblt[i].himg, TRUE);
			}
		}

		// ================================�G�e�t���[��������================================
		// ----- �G�e�V���b�g ----- 
		// enemyShotfunc(); 
		
		// ----- �G�e�ړ� ----- 
		//move
		for (i = 0; i < BULLETS_NUM_MAX; i++) {
			if (bullets[i].enable == true) {
				bullets[i].pos = addvect(bullets[i].pos, polarToCartesian(bullets[i].velo));
			}
			// ��ʊO�ɏo���疳����
			if (isOut(bullets[i], { HDWIDTH + 100, HDHEIGHT + 100 }, { -100, -100 }) == true) {
				bullets[i].enable = false;
			}
		}
		// ----- �G�e�`�� ----- 
		//draw
		for (i = 0; i < BULLETS_NUM_MAX; i++) {
			//{ 500, HDHEIGHT }, { (HDWIDTH - 500) / 2, 0 }
			//DrawBox(500, HDHEIGHT, (HDWIDTH - 500) / 2 , 0 , 0xffff00, true);
			if (bullets[i].enable == true)
				DrawGraph(subvect(bullets[i].pos, devvect(bullets[i].size, 2)), bullets[i].himg, true);
		}

		timeCnt++;
		scnsample.lend();
	}
}

void defeat(int defeatSound, int *msg){
	//StopSound();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, HDWIDTH, HDHEIGHT, GetColor(0xff, 00, 00), true);
	
	DrawString(HDWIDTH / 2 - 50, HDHEIGHT / 2, "GAME OVER!!", 0xffffff);
	DrawString(HDWIDTH / 2 - 100, HDHEIGHT / 2 + 15, "press ESC key to RETRY!", 0xffffff);
	ScreenFlip();
	PlaySoundMem(defeatSound, DX_PLAYTYPE_BACK, 1);
	Sleep(1000);
	
	//while (CheckHitKeyAll() != true);
	while (CheckHitKeyAll() == false) {
		if (CheckHitKey(KEY_INPUT_ESCAPE) == true) {
			*msg = MSG_RETRY;
		}
		else {
			*msg = MSG_EXIT;
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
}

void pause(int *msg) {
	DrawString(HDWIDTH / 2 - 50, HDHEIGHT / 2, "PAUSE!!", 0xffffff);
	DrawString(HDWIDTH / 2 - 100, HDHEIGHT / 2 + 15, "press Return key to retry", 0xffffff);
	DrawString(HDWIDTH / 2 - 100, HDHEIGHT / 2 + 30, "press ESC key to EXIT", 0xffffff);
	ScreenFlip();
	Sleep(300);
	while (CheckHitKeyAll() == false) {
		if (CheckHitKey(KEY_INPUT_ESCAPE) == true) {
			*msg = MSG_EXIT;
			break;
		}
		if (CheckHitKey(KEY_INPUT_RETURN) == true) {
			*msg = MSG_RETRY;
			break;
		}
	}
}

void moveBG(int himg, int velo, int * bgyPos) {
	//static int bgyPos;
	int bgyvelo, tmpvelo;
	if (*bgyPos >= 12000) {
		bgyvelo = 0;
	}
	else if (*bgyPos >= 10000) {
		tmpvelo = (12000 - *bgyPos) / 100;
		bgyvelo = tmpvelo > velo ? velo : tmpvelo;
	}
	else {
		bgyvelo = velo;
	}
	DrawGraph((HDWIDTH - 500) / 2 , -12000 + *bgyPos, himg, false);
	*bgyPos += bgyvelo;
}

void bomb(obj bullets[]) {
	for (int i = 0; i < BULLETS_NUM_MAX; i++) {
		bullets[i].enable = false;
	}
}

void scoreTime(int timecnt) {
	DrawString(CENTER, "CLEAR !!\n Time:", 0xffffff, 0xaaff33);
	ScreenFlip();
	Sleep(1000);
}

void displayKeyDescription() {
	DrawString(CENTER, "�����L�[�F�ړ�\nA�{�^���F�V���b�g\nRB�L�[�F�X���E\n", 0xffcc22, 0x000000);
	DrawString(CENTER - 15, "5�b��ɊJ�n���܂�....�I", 0xffcc22, 0x000000);
	ScreenFlip();
	Sleep(5000);

}