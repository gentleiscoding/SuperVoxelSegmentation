#pragma once
#include <iostream>
#include <vector>
#include "SPFH.h"

using namespace std;
typedef struct{ double x; double y; double z; } pos;
typedef struct{ double r; double g; double b; } rgb;
typedef struct{ double x; double y; double z; } normal_;
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
	vector<normal_>				list_normal;
	vector<SPFH>	list_spfh;
	vector<vector<double>>	list_fpfh;

private:
	int			M;
	int			N;


public:
	int		getM();
	void	setM(int);
	int		getN();
	void	setN(int);
	void	scale_deep_value(float* );
	float	find_min(float* );
	float	find_max(float* );
	void	inital_normal();
	normal_		caculate_normal(int voxel1,int voxel2,int voxel3);
	void	intial_spfh();
	void	intial_fpfh();
	int	judge_spfh(double);
};

