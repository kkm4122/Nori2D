#pragma once
#include "renderer/Dx2DRenderable.h"

#define UltraFrame(x) ((float)x/2048.f)
#define UltraSize 128.f
#define UF_(x) ((float)((x)*128)/2048.f)
#define UFR_(u,v) { UF_(u), UF_(v), UF_(u+1), UF_(v+1) }
#define UFRI_(x) {					\
					UFR_(x,0),		\
					UFR_(x,1),		\
					UFR_(x,2),		\
					UFR_(x,3),		\
					UFR_(x,4),		\
					UFR_(x,5),		\
					UFR_(x,6),		\
					UFR_(x,7),		\
					UFR_(x,8),		\
					UFR_(x,9),		\
					UFR_(x,10),		\
					UFR_(x,11),		\
					UFR_(x,12),		\
					UFR_(x,13),		\
					UFR_(x,14)		}


struct UltraRect
{
	float U1, V1;
	float U2, V2;
};

UltraRect Ultra_0[] =
{
	UFR_(0,0),
	UFR_(0,1),
	UFR_(0,2),
	UFR_(0,3),
	UFR_(0,4),
	UFR_(0,5),
	UFR_(0,6),
	UFR_(0,7),
	UFR_(0,8),
	UFR_(0,9),
	UFR_(0,10),
	UFR_(0,11),
	UFR_(0,12),
	UFR_(0,13),
	UFR_(0,14),
};

UltraRect Ultra_1[] = UFRI_(1);
UltraRect Ultra_2[] = UFRI_(2);
UltraRect Ultra_3[] = UFRI_(3);
UltraRect Ultra_4[] = UFRI_(4);
UltraRect Ultra_5[] = UFRI_(5);
UltraRect Ultra_6[] = UFRI_(6);
UltraRect Ultra_7[] = UFRI_(7);
UltraRect Ultra_8[] = UFRI_(8);
UltraRect Ultra_9[] = UFRI_(9);
UltraRect Ultra_10[] = UFRI_(10);
UltraRect Ultra_11[] = UFRI_(11);
UltraRect Ultra_12[] = UFRI_(12);
UltraRect Ultra_13[] = UFRI_(13);
UltraRect Ultra_14[] = UFRI_(14);
UltraRect Ultra_15[] = UFRI_(15);

struct AnimRectTime
{
	float totalTime;
	float totalFrame;
};

AnimRectTime Ultra_Time = { 2.f, ARRAYSIZE(Ultra_0) };

UltraRect& GetActorUV(Dx2DRenderable* rd)
{
	if (rd->tex.mName == ULTRA_NAME)
	{
		switch (rd->dir)
		{
		case 0: return Ultra_0[rd->frameNo];
		case 1: return Ultra_1[rd->frameNo];
		case 2: return Ultra_2[rd->frameNo];
		case 3: return Ultra_3[rd->frameNo];
		case 4: return Ultra_4[rd->frameNo];
		case 5: return Ultra_5[rd->frameNo];
		case 6: return Ultra_6[rd->frameNo];
		case 7: return Ultra_7[rd->frameNo];
		case 8: return Ultra_8[rd->frameNo];
		case 9: return Ultra_9[rd->frameNo];
		case 10: return Ultra_10[rd->frameNo];
		case 11: return Ultra_11[rd->frameNo];
		case 12: return Ultra_12[rd->frameNo];
		case 13: return Ultra_13[rd->frameNo];
		case 14: return Ultra_14[rd->frameNo];
		case 15: return Ultra_15[rd->frameNo];
		}
	}
		return Ultra_0[0];
}

AnimRectTime& GetActorTime(Dx2DRenderable* rd)
{
	if (rd->tex.mName == ULTRA_NAME)
	{
		return Ultra_Time;
	}
}