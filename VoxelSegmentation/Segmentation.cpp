#include "Segmentation.h"


Segmentation::Segmentation(void)
{
}

Segmentation::Segmentation(Image_Info image_info)
{
	image_original=image_info;
	image_segmented=image_info;
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
	color_importance=0.3;
	spatial_importance=0.3;
	normal_importance=0.1;
	inital_normal();
}

Segmentation::~Segmentation(void)
{
}

void Segmentation::do_segment()
{
	Rseed=0.3;
	Rresearch=0.5;

	int N=image_original.getN();
	int M=image_original.getM();
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
			list_center.push_back(j*M+M-i-1);
			cout<<"x:"<<j<<" y:"<<i<<endl;
			count++;
		}
	}
	list_seed.resize(count);
	ajacency_D.resize(count);
	list_ajacency.resize(count);

	for (int i=0;i<count;i++)
	{
		list_seed[i].push_back(list_center[i]);
		find_neighbors(list_center[i],i);
	}

	return count;
}

//count is the number of seed
void Segmentation::find_neighbors(int current_voxel,int count_seed)
{
	int N=image_original.getN();
	int M=image_original.getM();
	int x_index=image_original.vertex_index[current_voxel].x;
	int y_index=image_original.vertex_index[current_voxel].y;

	
	for (int x_adj_index=x_index-1;x_adj_index<=x_index+1;x_adj_index++)
	{
		for (int y_adj_index=y_index-1;y_adj_index<=y_index+1;y_adj_index++)
		{		
				if (x_adj_index>=0&&x_adj_index<N&&y_adj_index>=0&&y_adj_index<M&&(x_adj_index!=x_index||y_adj_index!=y_index))
				{
					int adj_index=x_adj_index*M+M-y_adj_index-1;
						//judge whether it is in the range 
						if (distance_between(list_center[count_seed],adj_index)<=(Rresearch))
						{
							if (selected_label[adj_index]==true)
							{
								insert_ajacenncy(count_seed,adj_index);
							}
							
						}
					
				}
		}
	}
}

void Segmentation::insert_ajacenncy(int count_seed,int adj_index)
{
	size_t i=0;
	double D=caculate_D(list_center[count_seed],adj_index);
	for (;i<list_ajacency[count_seed].size();i++)
	{
		if (D>ajacency_D[count_seed][i])
		{
			break;
		}
	}
	if (i==list_ajacency[count_seed].size())
	{
		list_ajacency[count_seed].push_back(adj_index);
		ajacency_D[count_seed].push_back(D);
	}else
	{
		list_ajacency[count_seed].insert(list_ajacency[count_seed].begin()+i,adj_index);
		ajacency_D[count_seed].insert(ajacency_D[count_seed].begin()+i,D);
	}
}

bool Segmentation::generate_seed(int count_seed)
{
	/*if (list_ajacency[count_seed].size()==0)
	{
		return true;
	}
	int min_ajacency=list_ajacency[count_seed][list_ajacency[count_seed].size()-1];
	list_ajacency[count_seed].pop_back();
	double D=ajacency_D[count_seed][ajacency_D[count_seed].size()-1];
	ajacency_D[count_seed].pop_back();*/
	bool flag=false;
	int min_ajacency=0;
	while(flag==false)
	{
		if (list_ajacency[count_seed].size()==0)
		{
			return true;
		}
		min_ajacency=list_ajacency[count_seed][list_ajacency[count_seed].size()-1];
		list_ajacency[count_seed].pop_back();
		//D=ajacency_D[count_seed][ajacency_D[count_seed].size()-1];
		ajacency_D[count_seed].pop_back();
		flag=selected_label[min_ajacency];
	}
	if (selected_label[min_ajacency]==true)
	{
		selected_label[min_ajacency]=false;
		list_seed[count_seed].push_back(min_ajacency);
		find_neighbors(min_ajacency,count_seed);
		return false;
	}

}

double Segmentation::distance_between(int voxel1,int voxel2)
{
	double x1=image_original.vertex_pos[voxel1].x;
	double y1=image_original.vertex_pos[voxel1].y;
	double z1=image_original.vertex_pos[voxel1].z;
	double x2=image_original.vertex_pos[voxel2].x;
	double y2=image_original.vertex_pos[voxel2].y;
	double z2=image_original.vertex_pos[voxel2].z;
	double a=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);
	return sqrt(a);
}

double Segmentation::caculate_D(int voxel1,int voxel2)
{
	double Dc=(image_original.vertex_rgb[voxel1].r-image_original.vertex_rgb[voxel2].r)/3+(image_original.vertex_rgb[voxel1].g-image_original.vertex_rgb[voxel2].g)/3+(image_original.vertex_rgb[voxel1].b-image_original.vertex_rgb[voxel2].b)/3;
	double Ds=distance_between(voxel1,voxel2);
	double dominator=(list_normal[voxel1].x*list_normal[voxel1].x+list_normal[voxel1].y*list_normal[voxel1].y+list_normal[voxel1].z*list_normal[voxel1].z)*(list_normal[voxel2].x*list_normal[voxel2].x+list_normal[voxel2].y*list_normal[voxel2].y+list_normal[voxel2].z*list_normal[voxel2].z);
	double cos_value=(list_normal[voxel1].x*list_normal[voxel2].x+list_normal[voxel1].y*list_normal[voxel2].y+list_normal[voxel1].z*list_normal[voxel2].z)/sqrt(dominator);
	double Dn=acos(cos_value)/3.1415927;
	double D_=color_importance*Dc*Dc/(255*255)+spatial_importance*Ds*Ds/(3*Rseed*Rseed)+normal_importance*Dn*Dn;
	//cout<<"color:"<<Dc*Dc/(255*255)<<" spatial:"<<Ds*Ds/(3*Rseed*Rseed)<<" normal:"<<Dn*Dn<<endl;
	return sqrt(D_);
}

void Segmentation::inital_normal()
{
	int N=image_original.getN();
	int M=image_original.getM();
	normal_ default_value={0,0,0};
	list_normal.resize(M*N,default_value);
	
	for (int x=0;x<N;x++)
	{
		for(int y=0;y<M;y++)
		{
			double normal_x=0.0;
			double normal_y=0.0;
			double normal_z=0.0;

			int voxel=x*M+M-1-y;
			//cout<<"begin:"<<voxel<<endl;
			if (voxel==69)
			{
				int iii=0;
			}
			//left up and left
			int x_adj1=x-1;
			int y_adj1=y+1;
			int x_adj2=x-1;
			int y_adj2=y;

			if (0<=x_adj1&&x_adj1<N&&0<=y_adj1&&y_adj1<M)
			{
				if (0<=x_adj2&&x_adj2<N&&0<=y_adj2&&y_adj2<M)
				{
					 normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
					 normal_x+=a.x;
					 normal_y+=a.y;
					 normal_z+=a.z;
					 //cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
					 
				}
			}

			//left and bottom
			x_adj1=x-1;
			y_adj1=y;
			x_adj2=x;
			y_adj2=y-1;

			if (0<=x_adj1&&x_adj1<N&&0<=y_adj1&&y_adj1<M)
			{
				if (0<=x_adj2&&x_adj2<N&&0<=y_adj2&&y_adj2<M)
				{
					normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
					normal_x+=a.x;
					normal_y+=a.y;
					normal_z+=a.z;
					//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
				}
			}

			//bottom and right bottom
			x_adj1=x;
			y_adj1=y-1;
			x_adj2=x+1;
			y_adj2=y-1;

			if (0<=x_adj1&&x_adj1<N&&0<=y_adj1&&y_adj1<M)
			{
				if (0<=x_adj2&&x_adj2<N&&0<=y_adj2&&y_adj2<M)
				{
					normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
					normal_x+=a.x;
					normal_y+=a.y;
					normal_z+=a.z;
					//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
				}	
			}

			//right bottom and right
			x_adj1=x+1;
			y_adj1=y-1;
			x_adj2=x+1;
			y_adj2=y;

			if (0<=x_adj1&&x_adj1<N&&0<=y_adj1&&y_adj1<M)
			{
				if (0<=x_adj2&&x_adj2<N&&0<=y_adj2&&y_adj2<M)
				{
					normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
					normal_x+=a.x;
					normal_y+=a.y;
					normal_z+=a.z;
					//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
				}
			}

			//right and up
			x_adj1=x+1;
			y_adj1=y;
			x_adj2=x;
			y_adj2=y+1;

			if (0<=x_adj1&&x_adj1<N&&0<=y_adj1&&y_adj1<M)
			{
				if (0<=x_adj2&&x_adj2<N&&0<=y_adj2&&y_adj2<M)
				{
					normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
					normal_x+=a.x;
					normal_y+=a.y;
					normal_z+=a.z;
					//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
				}
			}

			//up and left up
			x_adj1=x;
			y_adj1=y+1;
			x_adj2=x-1;
			y_adj2=y+1;

			if (0<=x_adj1&&x_adj1<N&&0<=y_adj1&&y_adj1<M)
			{
				if (0<=x_adj2&&x_adj2<N&&0<=y_adj2&&y_adj2<M)
				{
					normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
					normal_x+=a.x;
					normal_y+=a.y;
					normal_z+=a.z;
					//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
				}
			}

			list_normal[voxel].x=normal_x;
			list_normal[voxel].y=normal_y;
			list_normal[voxel].z=normal_z;
			
			//cout<<voxel<<"th normal:("<<list_normal[voxel].x<<","<<list_normal[voxel].y<<","<<list_normal[voxel].z<<")"<<endl;
		}
			
	}
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
			if (i2==0)
			{
			image_segmented.vertex_rgb[list_seed[i][i2]].r=0.0;
			image_segmented.vertex_rgb[list_seed[i][i2]].g=0.0;
			image_segmented.vertex_rgb[list_seed[i][i2]].b=0.0;
			}
			else
			{
				image_segmented.vertex_rgb[list_seed[i][i2]].r=c1;
				image_segmented.vertex_rgb[list_seed[i][i2]].g=c2;
				image_segmented.vertex_rgb[list_seed[i][i2]].b=c3;
			}
			
		}
	}
}


normal_ Segmentation::caculate_normal(int voxel1,int adj1,int adj2)
{
	double pos_x=image_original.vertex_pos[voxel1].x;
	double pos_y=image_original.vertex_pos[voxel1].y;
	double pos_z=image_original.vertex_pos[voxel1].z;

	double adj_x1=image_original.vertex_pos[adj1].x;
	double adj_y1=image_original.vertex_pos[adj1].y;
	double adj_z1=image_original.vertex_pos[adj1].z;
	double adj_x2=image_original.vertex_pos[adj2].x;
	double adj_y2=image_original.vertex_pos[adj2].y;
	double adj_z2=image_original.vertex_pos[adj2].z;

	double vec_x1=adj_x1-pos_x;
	double vec_y1=adj_y1-pos_y;
	double vec_z1=adj_z1-pos_z;
	double vec_x2=adj_x2-pos_x;
	double vec_y2=adj_y2-pos_y;
	double vec_z2=adj_z2-pos_z;

	double normal_x_=vec_y1*vec_z2-vec_z1*vec_y2;
	double normal_y_=vec_z1*vec_x2-vec_x1*vec_z2;
	double normal_z_=vec_x1*vec_y2-vec_y1*vec_x2;

	double demoninator=sqrt(normal_x_*normal_x_+normal_y_*normal_y_+normal_z_*normal_z_);
	if (demoninator==0)
	{
		normal_ a={0.0,0.0,0.0};
		return a;
	}
	normal_ a={normal_x_/demoninator,normal_y_/demoninator,normal_z_/demoninator};
	return a;
}  