#pragma once
#include <iostream>
#include <vector>
using namespace std;
typedef struct{ double x; double y; double z; } pos;
typedef struct{ double r; double g; double b; } rgb;
typedef struct{ int x; int y;} index;
class Image_Info
{
public:
	Image_Info(void);
	void init(bool,bool,unsigned char*,float*);
	~Image_Info(void);
	void clear();
	vector<pos>	vertex_pos;
	vector<index> vertex_index;
	vector<rgb>	vertex_rgb;
	double			x_min;
	double			x_max;
	double			y_min;
	double			y_max;

private:
	int			M;
	int			N;


public:
	int getM();
	void setM(int);
	int getN();
	void setN(int);
	void scale_deep_value(float* );
	float find_min(float* );
	float find_max(float* );
};

