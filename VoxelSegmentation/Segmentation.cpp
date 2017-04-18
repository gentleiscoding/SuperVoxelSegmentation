#include "Segmentation.h"


Segmentation::Segmentation(void)
{
}

Segmentation::Segmentation(Image_Info image_info)
{
	image_original=image_info;
	image_segmented=image_info;
	image_segmented_2d=image_info;
	int N=image_original.getN();
	int M=image_original.getM();
	int count=0;
	for (int j=0;j<N;j++)
	{
		for(int i=0;i<M;i++)
		{
			image_segmented.vertex_rgb[count].r=(double)76.5;
			image_segmented.vertex_rgb[count].g=(double)76.5;
			image_segmented.vertex_rgb[count].b=(double)76.5;
			image_segmented.vertex_pos[count].x=image_original.vertex_pos[count].x;
			image_segmented.vertex_pos[count].y=image_original.vertex_pos[count].y;
			image_segmented.vertex_pos[count].z=image_original.vertex_pos[count].z;
			count++;
		}
	}
	color_importance=1.0;
	spatial_importance=0.4;
	normal_importance=1.0;
	/*for (int i=1;i<M*N;i++)
	{
		cout<<distance_between(i,i-1)<<endl;
	}*/
	image_original.inital_normal();
	/*image_original.intial_spfh();
	image_original.intial_fpfh();*/
}

Segmentation::~Segmentation(void)
{
}

void Segmentation::do_segment()
{
	Rseed=0.2;
	
	int N=image_original.getN();
	int M=image_original.getM();

	list_seg.resize(M*N,-1);
	list_D.resize(M*N,-1);
	selected_label.resize(M*N,true);
	int seed_amount = build_seed(Rseed);
	bool check_end=false;
	while (check_end==false)
	{
		check_end=true;
		for (int i=0;i<seed_amount;i++)
		{
			bool check_end_=generate_seed(i);
			if (check_end==true)
			{
				check_end=check_end_;
			}
		}
	}
	save_as_image();
	mark_contours();
}

int Segmentation::build_seed(double revolution)
{
	int	col_amount=(image_original.x_max-image_original.x_min)/(revolution);

	int N=image_original.getN();
	int M=image_original.getM();
	int col_cluster=N/col_amount;
	int count=0;
	for (int j=0;j<N;j+=col_cluster)
	{
		for(int i=0;i<M;i+=col_cluster)
		{
			int center_=find_min_gradient(j*M+M-i-1);
			list_center_init.push_back(center_);
			center_features c={image_original.vertex_pos[center_].x,image_original.vertex_pos[center_].y,image_original.vertex_pos[center_].z,image_original.vertex_rgb[center_].r,image_original.vertex_rgb[center_].g,image_original.vertex_rgb[center_].b,image_original.list_normal[center_].x,image_original.list_normal[center_].y,image_original.list_normal[center_].z};
			list_center.push_back(c);
			//cout<<"x:"<<j<<" y:"<<i<<endl;
			count++;
		}
	}
	list_seed.resize(count);
	list_ajacency.resize(count);

	for (int i=0;i<count;i++)
	{
		list_seed[i].push_back(list_center_init[i]);
		find_neighbors(list_center_init[i],i,list_ajacency[i]);
		size_t k=list_ajacency[i].size();
	}

	return count;
}

//count is the number of seed
void Segmentation::find_neighbors(int current_voxel,int count_seed,vector<int>& temp_list)
{
	int N=image_original.getN();
	int M=image_original.getM();
	int x_index=image_original.vertex_index[current_voxel].x;
	int y_index=image_original.vertex_index[current_voxel].y;
	double z_=image_original.vertex_pos[current_voxel].z;
	for (int x_adj_index=x_index-1;x_adj_index<=x_index+1;x_adj_index++)
	{
		for (int y_adj_index=y_index-1;y_adj_index<=y_index+1;y_adj_index++)
		{		
				if (x_adj_index>=0&&x_adj_index<N&&y_adj_index>=0&&y_adj_index<M&&(x_adj_index!=x_index||y_adj_index!=y_index))
				{
					int adj_index=x_adj_index*M+M-y_adj_index-1;
						//judge whether it is in the range 
						if (fabs(image_original.vertex_pos[current_voxel].z-z_)<0.01&&distance_between(list_center[count_seed],adj_index)<=(sqrt(3.0)*Rseed))
						{
							if (list_seg[adj_index]!=count_seed)
							{
								 /*vector<int>::iterator ret;
								 ret=find(temp_list.begin(),temp_list.end(),adj_index);
								 if(ret == temp_list.end())*/
								 temp_list.push_back(adj_index);
							}
							
						}
					
				}
		}
	}
}

//void Segmentation::insert_ajacenncy(int count_seed,int adj_index)
//{
//	size_t i=0;
//	double D=caculate_D(list_center[count_seed],adj_index);
//	for (;i<list_ajacency[count_seed].size();i++)
//	{
//		if (D>ajacency_D[count_seed][i])
//		{
//			break;
//		}
//	}
//	if (i==list_ajacency[count_seed].size())
//	{
//		list_ajacency[count_seed].push_back(adj_index);
//		ajacency_D[count_seed].push_back(D);
//	}else
//	{
//		list_ajacency[count_seed].insert(list_ajacency[count_seed].begin()+i,adj_index);
//		ajacency_D[count_seed].insert(ajacency_D[count_seed].begin()+i,D);
//	}
//}

bool Segmentation::generate_seed(int count_seed)
{
	bool flag=true;
	vector<int> list_temp_adj;
	if (list_ajacency[count_seed].size()==0)
	{
		return true;
	}
	for (size_t i=0;i<list_ajacency[count_seed].size();i++)
	{
		int current_voxel=list_ajacency[count_seed][i];
		double temp_D=caculate_D(list_center[count_seed],current_voxel);
		if (list_D[current_voxel]==-1||temp_D<list_D[current_voxel])
		{
			if (temp_D<list_D[current_voxel])
			{
				int placed_count=list_seg[current_voxel];
				delete_voxel(placed_count,current_voxel);
			}
			list_D[current_voxel]=temp_D;
			list_seg[current_voxel]=count_seed;
			list_seed[count_seed].push_back(current_voxel);
			find_neighbors(current_voxel,count_seed,list_temp_adj);
			flag=false;
		}
	}
	list_ajacency[count_seed]=list_temp_adj;
	update_center(count_seed);
	return flag;
}

double Segmentation::distance_between(center_features c,int voxel2)
{
	double x1=c.x;
	double y1=c.y;
	double z1=c.z;
	double x2=image_original.vertex_pos[voxel2].x;
	double y2=image_original.vertex_pos[voxel2].y;
	double z2=image_original.vertex_pos[voxel2].z;
	double a=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);
	return sqrt(a);
}
double Segmentation::distance_between_(center_features c,int voxel2)
{
	double x1=c.x;
	double y1=c.y;
	double x2=image_original.vertex_pos[voxel2].x;
	double y2=image_original.vertex_pos[voxel2].y;
	double a=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	return sqrt(a);
}


double Segmentation::caculate_D(center_features c,int voxel2)
{
	/*lab_ v1={0.0,0.0,0.0};
	lab_ v2={0.0,0.0,0.0};
	RGB2Lab(c.r,c.g,c.b,v1.l,v1.a,v1.b);
	RGB2Lab(image_original.vertex_rgb[voxel2].r,image_original.vertex_rgb[voxel2].g,image_original.vertex_rgb[voxel2].b,v2.l,v2.a,v2.b);
	double Dc=sqrt((v1.l-v2.l)*(v1.l-v2.l)+(v1.a-v2.a)*(v1.a-v2.a)+(v1.b-v2.b)*(v1.b-v2.b));*/
	double Dc=sqrt(pow(c.r-image_original.vertex_rgb[voxel2].r,2)+pow(c.g-image_original.vertex_rgb[voxel2].g,2)+pow(c.b-image_original.vertex_rgb[voxel2].b,2));
	// between_ is without depth 
	double Ds=distance_between_(c,voxel2);
	/*double Dn=caculate_Dn(voxel1,voxel2);*/
	double Dn=1.0-fabs(c.normal_x*image_original.list_normal[voxel2].x+c.normal_y*image_original.list_normal[voxel2].y+c.normal_z*image_original.list_normal[voxel2].z);
	double D_=color_importance*Dc*Dc/(255*255)+spatial_importance*Ds*Ds/(Rseed*Rseed)+normal_importance*Dn*Dn;
	//cout<<"color:"<<Dc*Dc/(255*255)<<" spatial:"<<Ds*Ds/(3*Rseed*Rseed)<<" normal:"<<Dn*Dn<<endl;
	return sqrt(D_);
}

void Segmentation::save_as_image()
{
	size_t size=list_center.size();
	for (size_t i=0;i<size;i++)
	{
		double c1=rand()%255;
		double c2=rand()%255;
		double c3=rand()%255;
		for (size_t i2=0;i2<list_seed[i].size();i2++)
		{

				image_segmented.vertex_rgb[list_seed[i][i2]].r=c1;
				image_segmented.vertex_rgb[list_seed[i][i2]].g=c2;
				image_segmented.vertex_rgb[list_seed[i][i2]].b=c3;

		}
	}
}


void Segmentation::delete_voxel(int count_seed,int current_voxel)
{
	size_t i=0;
	for (;i<list_seed[count_seed].size();i++)
	{
		if (current_voxel==list_seed[count_seed][i])
		{
			break;
		}
	}
	list_seed[count_seed].erase(list_seed[count_seed].begin()+i);
}

void Segmentation::mark_contours()
{
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
	int N=image_original.getN();
	int M=image_original.getM();
	vector<bool> is_taken;
	is_taken.resize(M*N,false);
	for(int x=0;x<N;x++)
	{
		for(int y=0;y<M;y++)
		{
			int voxel=x*M+M-1-y;
			int seg=list_seg[voxel];
			int check_=0;
			for(int k=0;k<8;k++)
			{
				int x_adj=x+dx8[k];
				int y_adj=y+dx8[k];
				int current_voxel=x_adj*M+M-1-y_adj;
				if (!(x_adj<0||x_adj>=N||y_adj<0||y_adj>=M))
				{
					if (list_seg[current_voxel]!=seg&&is_taken[current_voxel]==false)
					{					
						check_++;
					}
				}
					
			}
			if (check_>=2)
			{
				list_contours.push_back(voxel);
				is_taken[voxel]=true;
			}
			
		}
	}
	for(size_t i=0;i<list_contours.size();i++)
	{
		int voxel=list_contours[i];
		image_segmented_2d.vertex_rgb[voxel].r=255;
		image_segmented_2d.vertex_rgb[voxel].g=0;
		image_segmented_2d.vertex_rgb[voxel].b=0;
	}

}
void Segmentation::RGB2Lab(double R, double G, double B, double &L, double &a, double &b)
{
	double X, Y, Z, fX, fY, fZ;
	double BLACK = 20;
	double YELLOW = 70;
	X = 0.412453*R + 0.357580*G + 0.180423*B;
	Y = 0.212671*R + 0.715160*G + 0.072169*B;
	Z = 0.019334*R + 0.119193*G + 0.950227*B;

	X /= (255 * 0.950456);
	Y /=  255;
	Z /= (255 * 1.088754);

	if (Y > 0.008856)
	{
		fY = pow(Y, 1.0/3.0);
		L = 116.0*fY - 16.0;
	}
	else
	{
		fY = 7.787*Y + 16.0/116.0;
		L = 903.3*Y;
	}

	if (X > 0.008856)
		fX = pow(X, 1.0/3.0);
	else
		fX = 7.787*X + 16.0/116.0;

	if (Z > 0.008856)
		fZ = pow(Z, 1.0/3.0);
	else
		fZ = 7.787*Z + 16.0/116.0;

	a = 500.0*(fX - fY);
	b = 200.0*(fY - fZ);

	if (L < BLACK) 
	{
		a *= exp((L - BLACK) / (BLACK / 4));
		b *= exp((L - BLACK) / (BLACK / 4));
		L = BLACK;
	}
	if (b > YELLOW)
		b = YELLOW;


}

int Segmentation::find_min_gradient(int voxel)
{
	int x=image_original.vertex_index[voxel].x;
	int y=image_original.vertex_index[voxel].y;
	int N=image_original.getN();
	int M=image_original.getM();
	double min_gradient=99999;
	int		min_voxel=voxel;
	for (int x_adj=x-1;x_adj<=x+1;x_adj++)
	{
		for (int y_adj=y-1;y_adj<=y+1;y_adj++)
		{
			if (x_adj>=0&&x_adj<N&&y_adj<M&&y_adj>=0&&(x_adj!=x||y_adj!=y))
			{
				int current_voxel=x_adj*M+M-1-y_adj;
				double gradient_=caculate_gradient(current_voxel);
				if (gradient_<min_gradient)
				{
					min_gradient=gradient_;
					min_voxel=current_voxel;
				}
			}
		}
		
	}
	return min_voxel;
}

double Segmentation::caculate_gradient(int voxel)
{

	int x=image_original.vertex_index[voxel].x;
	int y=image_original.vertex_index[voxel].y;
	int N=image_original.getN();
	int M=image_original.getM();
	int count=0;
	double	addition_=0;
	for (int x_adj=x-1;x_adj<=x+1;x_adj++)
	{
		for (int y_adj=y-1;y_adj<=y+1;y_adj++)
		{
			if (x_adj>=0&&x_adj<N&&y_adj<M&&y_adj>=0&&(x_adj!=x||y_adj!=y))
			{
				int current_voxel=x_adj*M+M-1-y_adj;
				count++;
				/*lab_ v1={0.0,0.0,0.0};
				lab_ v2={0.0,0.0,0.0};*/
				/*RGB2Lab(image_original.vertex_rgb[current_voxel].r,image_original.vertex_rgb[current_voxel].g,image_original.vertex_rgb[current_voxel].b,v1.l,v1.a,v1.b);
				RGB2Lab(image_original.vertex_rgb[voxel].r,image_original.vertex_rgb[voxel].g,image_original.vertex_rgb[voxel].b,v2.l,v2.a,v2.b);*/
				addition_+=sqrt(pow(image_original.vertex_rgb[current_voxel].r-image_original.vertex_rgb[voxel].r,2)+pow(image_original.vertex_rgb[current_voxel].g-image_original.vertex_rgb[voxel].g,2)+pow(image_original.vertex_rgb[current_voxel].b-image_original.vertex_rgb[voxel].b,2));
			}
		}
	}
	return addition_/count;
}

double Segmentation::caculate_Dn(int voxel,int voxel2)
{
	vector<double> f1=image_original.list_fpfh[voxel];
	vector<double> f2=image_original.list_fpfh[voxel2];
	double count=0;
	for (int i=0;i<8;i++)
	{
		if (f1[i]!=0&&f2[i]!=0)
		{
			count+=f1[i]*f2[i];
		}
	}
	return count;
}

void Segmentation::update_center(int count)
{
	center_features temp={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	size_t size=list_seed[count].size();
	for (size_t i=0;i<size;i++)
	{
		temp.x+=image_original.vertex_pos[list_seed[count][i]].x/size;
		temp.y+=image_original.vertex_pos[list_seed[count][i]].y/size;
		temp.z+=image_original.vertex_pos[list_seed[count][i]].z/size;
		temp.r+=image_original.vertex_rgb[list_seed[count][i]].r/size;
		temp.g+=image_original.vertex_rgb[list_seed[count][i]].g/size;
		temp.b+=image_original.vertex_rgb[list_seed[count][i]].b/size;
		temp.normal_x=image_original.list_normal[list_seed[count][i]].x/size;
		temp.normal_y=image_original.list_normal[list_seed[count][i]].y/size;
		temp.normal_z=image_original.list_normal[list_seed[count][i]].z/size;
	}
	list_center[count]=temp;
}