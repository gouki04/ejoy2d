#include "screen.h"
#include "opengl.h"
#include "spritepack.h"

struct screen {
	int width;
	int height;
	int scale;
	float invw;
	float invh;
};

static struct screen SCREEN;

void
screen_init(float w, float h, float scale) {
	SCREEN.width = (int)w;
	SCREEN.height = (int)h;
	SCREEN.scale = (int)scale;
	SCREEN.invw = 2.0f / SCREEN_SCALE / w;
	SCREEN.invh = -2.0f / SCREEN_SCALE / h;
	glViewport(0,0,w * scale,h * scale);
}

void
screen_trans(float *x, float *y) {
	*x *= SCREEN.invw;
	*y *= SCREEN.invh;
}

void
screen_scissor(int x, int y, int w, int h) {
	y = SCREEN.height - y - h;
	if (x<0) {
		w += x;
		x = 0;
	} else if (x>SCREEN.width) {
		w=0;
		h=0;
	}
	if (y<0) {
		h += y;
		y = 0;
	} else if (y>SCREEN.height) {
		w=0;
		h=0;
	}
	if (w<=0 || h<=0) {
		w=0;
		h=0;
	}
	x *= SCREEN.scale;
	y *= SCREEN.scale;
	w *= SCREEN.scale;
	h *= SCREEN.scale;

	glScissor(x,y,w,h);
}

bool screen_is_visible(float x,float y)
{
	return x >= 0.0f && x <= 2.0f && y>=-2.0f && y<= 0.0f;
}
bool screen_is_poly_invisible(const float* points,int len,int stride)
{
	int i =0;
	///测试在x的左边
	bool invisible = true;
	for(i =0; i < len && invisible;++i)
	{
		if(points[i*stride] >= 0.0f)
			invisible = false;
	}
	if(invisible)
		return true;
	
	//测试x轴的右边
	invisible = true;
	for(i =0; i < len && invisible;++i)
	{
		if(points[i*stride] <= 2.0f)
			invisible = false;
	}
	if(invisible)
		return true;

	///测试在y的上边
	invisible = true;
	for(i =0; i < len && invisible;++i)
	{
		if(points[i*stride +1] >= -2.0f)
			invisible = false;
	}
	if(invisible)
		return true;
	
	//测试y轴的下边
	invisible = true;
	for(i =0; i < len && invisible;++i)
	{
		if(points[i*stride +1] <= 0.0f)
			invisible = false;
	}
	return invisible;
}


