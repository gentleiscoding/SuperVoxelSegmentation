#pragma once
#include"Image_Info.h"
#include<math.h>
#include <queue>  
typedef struct{ double x; double y; double z; } normal_;

class Segmentation
{
public:
	Segmentation(void);
	Segmentation(Image_Info);
	~Segmentation(void);
	
	void		do_segment();
	int			build_seed(double);
	bool		generate_seed(int);
	void		find_neighbors(int,int,vector<int>&);
	void		insert_ajacenncy(int,int);
	double		caculate_D(int,int);
	double		distance_between(int,int);
	void		delete_voxel(int,int);
	void		inital_normal();
	void		save_as_image();
	normal_		caculate_normal(int voxel1,int voxel2,int voxel3);


public:
	//image
	Image_Info		image_original;
	Image_Info		image_segmented;
	vector<bool>	selected_label;

	//Rseed
	double				Rseed;
	double				Rresearch;

	//list for segmentation
	vector<int>					list_center;
	vector<vector<int>>			list_seed;

	vector<vector<int>>			list_ajacency;
	vector<normal_>				list_normal;
	vector<int>					list_seg;
	vector<double>				list_D;
	

	//importance
	double	color_importance;
	double spatial_importance;
	double normal_importance;
	
};

