#include "Image_Info.h"


Image_Info::Image_Info(void)
{
}
void Image_Info::init(bool check_rgb,bool check_depth,unsigned char *img_rgb,float* img_depth)
{

	//scale_deep_value(img_depth);
	if (check_rgb==true)
	{
		if (check_depth==true)
		{

			int count=0;
			vertex_rgb.resize(M*N);
			vertex_pos.resize(M*N);
			vertex_index.resize(M*N);
			double cx_rgb = 3.2754244941119034e+02;
			double cy_rgb= 2.3965616633400465e+02;
			double fx_rgb = 5.9759790117450188e+02;
			double fy_rgb = 5.9765961112127485e+02;
			x_min=99999.0;
			x_max=-99999.0;
			y_min=99999.0;
			y_max=-99999.0;
			for (int j=0;j<N;j++)
			{
				for(int i=0;i<M;i++)
				{
					/*cout<<"r:"<<(double)img_rgb[j*M+i]/255.0<<" g:"<<(double)img_rgb[M*N+j*M+i]/255.0<<" b:"<<img_rgb[2*M*N+j*M+i]/255.0<<endl;*/
					//cout<<"x:"<<j<<" y:"<<i<<" count:"<<count<<endl;
					//input the rgb information into image_info
					vertex_rgb[count].r=(double)img_rgb[count];
					vertex_rgb[count].g=(double)img_rgb[M*N+count];
					vertex_rgb[count].b=(double)img_rgb[2*M*N+count];

					vertex_pos[count].x=(j - cx_rgb)*img_depth[count]/fx_rgb;;
					vertex_pos[count].y=(cy_rgb - i) * img_depth[count]/ fy_rgb;					
					vertex_pos[count].z=- img_depth[count];

					vertex_index[count].x=j;
					vertex_index[count].y=M-i-1;
					/*cout<<"y:"<<i<<" cy_rgb - i£º"<<(cy_rgb - i)<<" img_depth[count]:"<<img_depth[count]<<" fx_rgb:"<<fx_rgb<<" y_:"<<vertex_pos[count].y<<endl;*/
					//cout<<"x:"<<vertex_pos[count].x<<" y:"<<vertex_pos[count].y<<" z:"<<vertex_pos[count].z<<endl;
					//input the position information into image_info
					/*vertex_pos[count].x=j;
					vertex_pos[count].y=M-i-1;
					vertex_pos[count].z=-img_depth[count];*/
					//cout<<img_depth[count]<<endl;
					if (vertex_pos[count].x<x_min)
					{
						x_min=vertex_pos[count].x;
					}
					if (vertex_pos[count].x>x_max)
					{
						x_max=vertex_pos[count].x;
					}
					if (vertex_pos[count].y<y_min)
					{
						y_min=vertex_pos[count].y;
					}
					if (vertex_pos[count].y>y_max)
					{
						y_max=vertex_pos[count].y;
					}

					count++;
				}
			}

			
		}else
		{
			cout<<"error:no depth information"<<endl;
		}
	}else
	{
		cout<<"error:no rgb information"<<endl;
	}

}


void Image_Info::scale_deep_value(float* img_depth)
{
	float min_value=find_min(img_depth);
	float max_value=find_max(img_depth);
	for (int i=0;i<M*N;i++)
	{
		img_depth[i]=(int)((img_depth[i]-min_value)/(max_value-min_value)*300);
	}
}

float Image_Info::find_min(float* img_depth)
{
	float min_value=99999.0;
	for (int i=0;i<M*N;i++)
	{
		if (img_depth[i]<min_value)
		{
			min_value=img_depth[i];
		}
		
	}
	return min_value;
}

float Image_Info::find_max(float* img_depth)
{
	float max_value=0.0;
	for (int i=0;i<M*N;i++)
	{
		if (img_depth[i]>max_value)
		{
			max_value=img_depth[i];
		}

	}
	return max_value;
}

Image_Info::~Image_Info(void)
{
}

void Image_Info::setM(int data)
{
	M=data;
}
void Image_Info::setN(int data)
{
	N=data;
}

int Image_Info::getM()
{
	return M;
}

int Image_Info::getN()
{
	return N;
}


void Image_Info::inital_normal()
{
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
			double z=vertex_pos[voxel].z;
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
					int adj1=x_adj1*M+M-1-y_adj1;
					int adj2=x_adj2*M+M-1-y_adj2;
					double z1=vertex_pos[adj1].z;
					double z2=vertex_pos[adj2].z;
					if(fabs(z-z1)<0.02&&fabs(z-z2)<0.02)
					{
						normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
						normal_x+=a.x;
						normal_y+=a.y;
						normal_z+=a.z;
						//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
					}

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
					int adj1=x_adj1*M+M-1-y_adj1;
					int adj2=x_adj2*M+M-1-y_adj2;
					double z1=vertex_pos[adj1].z;
					double z2=vertex_pos[adj2].z;
					if(fabs(z-z1)<0.02&&fabs(z-z2)<0.02)
					{
						normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
						normal_x+=a.x;
						normal_y+=a.y;
						normal_z+=a.z;
						//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
					}
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
					int adj1=x_adj1*M+M-1-y_adj1;
					int adj2=x_adj2*M+M-1-y_adj2;
					double z1=vertex_pos[adj1].z;
					double z2=vertex_pos[adj2].z;
					if(fabs(z-z1)<0.02&&fabs(z-z2)<0.02)
					{
						normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
						normal_x+=a.x;
						normal_y+=a.y;
						normal_z+=a.z;
						//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
					}
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
					int adj1=x_adj1*M+M-1-y_adj1;
					int adj2=x_adj2*M+M-1-y_adj2;
					double z1=vertex_pos[adj1].z;
					double z2=vertex_pos[adj2].z;
					if(fabs(z-z1)<0.02&&fabs(z-z2)<0.02)
					{
						normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
						normal_x+=a.x;
						normal_y+=a.y;
						normal_z+=a.z;
						//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
					}
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
					int adj1=x_adj1*M+M-1-y_adj1;
					int adj2=x_adj2*M+M-1-y_adj2;
					double z1=vertex_pos[adj1].z;
					double z2=vertex_pos[adj2].z;
					if(fabs(z-z1)<0.02&&fabs(z-z2)<0.02)
					{
						normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
						normal_x+=a.x;
						normal_y+=a.y;
						normal_z+=a.z;
						//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
					}
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
					int adj1=x_adj1*M+M-1-y_adj1;
					int adj2=x_adj2*M+M-1-y_adj2;
					double z1=vertex_pos[adj1].z;
					double z2=vertex_pos[adj2].z;
					if(fabs(z-z1)<0.02&&fabs(z-z2)<0.02)
					{
						normal_ a=caculate_normal(voxel,x_adj1*M+M-1-y_adj1,x_adj2*M+M-1-y_adj2);
						normal_x+=a.x;
						normal_y+=a.y;
						normal_z+=a.z;
						//cout<<"("<<a.x<<","<<a.y<<","<<a.z<<")"<<endl;
					}
				}
			}
			if (normal_x*normal_x+normal_y*normal_y+normal_z*normal_z==0)
			{
				list_normal[voxel].x=0.0;
				list_normal[voxel].y=0.0;	
				list_normal[voxel].y=0.0;
			}
			else
			{
				list_normal[voxel].x=normal_x/sqrt(normal_x*normal_x+normal_y*normal_y+normal_z*normal_z);
				list_normal[voxel].y=normal_y/sqrt(normal_x*normal_x+normal_y*normal_y+normal_z*normal_z);
				list_normal[voxel].z=normal_z/sqrt(normal_x*normal_x+normal_y*normal_y+normal_z*normal_z);

				//cout<<voxel<<"th normal:("<<list_normal[voxel].x<<","<<list_normal[voxel].y<<","<<list_normal[voxel].z<<")"<<endl;
			}
		}

	}
}


normal_ Image_Info::caculate_normal(int voxel1,int adj1,int adj2)
{
	double pos_x=vertex_pos[voxel1].x;
	double pos_y=vertex_pos[voxel1].y;
	double pos_z=vertex_pos[voxel1].z;

	double adj_x1=vertex_pos[adj1].x;
	double adj_y1=vertex_pos[adj1].y;
	double adj_z1=vertex_pos[adj1].z;
	double adj_x2=vertex_pos[adj2].x;
	double adj_y2=vertex_pos[adj2].y;
	double adj_z2=vertex_pos[adj2].z;

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

void Image_Info::intial_spfh()
{
	list_spfh.resize(M*N);
	for (int x=0;x<N;x++)
	{
		for(int y=0;y<M;y++)
		{
			int voxel=x*M+M-1-y;
			pos p1=vertex_pos[voxel];
			normal_ n1=list_normal[voxel];
			normal_ u=n1;
			SPFH current_spfh=SPFH(voxel);
			for (int x_adj=x-1;x_adj<=x+1;x_adj++)
			{
				for (int y_adj=y-1;y_adj<=y+1;y_adj++)
				{
					if (x_adj>=0&&x_adj<N&&y_adj<M&&y_adj>=0&&(x_adj!=x||y_adj!=y))
					{
						int adj_voxel=x_adj*M+M-1-y_adj;
						current_spfh.adj_vertex.push_back(adj_voxel);
						pos p2=vertex_pos[adj_voxel];
						normal_ n2=list_normal[adj_voxel];
						normal_ p1_p2_={p2.x-p1.x,p2.y-p1.y,p2.z-p1.z};
						double p2_p1_dist=sqrt(p1_p2_.x*p1_p2_.x+p1_p2_.y*p1_p2_.y+p1_p2_.z*p1_p2_.z);
						normal_ p1_p2={p1_p2_.x/p2_p1_dist,p1_p2_.y/p2_p1_dist,p1_p2_.z/p2_p1_dist};
						normal_ v={u.y*p1_p2.z-u.z*p1_p2.y,u.z*p1_p2.x-u.x*p1_p2.z,u.x*p1_p2.y-u.y*p1_p2.x};
						normal_ w={u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x};
						double wn2=w.x*n2.x+w.y*n2.y+w.z*n2.z;
						double un2=u.x*n2.x+u.y*n2.y+u.z*n2.z;
						angles a={v.x*n2.x+v.y*n2.y+v.z*n2.z,u.x*p1_p2.x+u.y*p1_p2.y+u.z*p1_p2.z,atan2(wn2,un2)};
						current_spfh.list_angles.push_back(a);
					}
				}
			}
			current_spfh.spfh_value.resize(8,0.0);
			for (size_t i=0;i<current_spfh.list_angles.size();i++)
			{
				int alpha_value= judge_spfh(current_spfh.list_angles[i].alpha);
				int beta_value= judge_spfh(current_spfh.list_angles[i].beta);
				int theta_value= judge_spfh(current_spfh.list_angles[i].theta);
				//cout<<alpha_value<<" "<<beta_value<<" "<<theta_value<<endl;
				int num=alpha_value*4+beta_value*2+theta_value;
				current_spfh.spfh_value[num]+=1.0/(double)current_spfh.list_angles.size();
				//cout<<1/current_spfh.list_angles.size();
			}
			list_spfh[voxel]=current_spfh;
		}
	}
}

void Image_Info::intial_fpfh()
{
	list_fpfh.resize(M*N);
	for (int i=0;i<N*M;i++)
	{
		list_fpfh[i].resize(8,0.0);
		for (int index=0;index<8;index++)
		{
			list_fpfh[i][index]=list_spfh[i].spfh_value[index];
		}
		for (size_t i2=0;i2<list_spfh[i].adj_vertex.size();i2++)
		{
			SPFH adj_spfh=list_spfh[list_spfh[i].adj_vertex[i2]];
			for (int index=0;index<8;index++)
			{
				if (adj_spfh.spfh_value[index]==0)
				{
					continue;
				}
				list_fpfh[i][index]+=(1.0/pow((double)list_spfh[i].adj_vertex.size(),2))*adj_spfh.spfh_value[index];
			}
		}

	}
}

int Image_Info::judge_spfh(double value_)
{
	if (value_<=0)
	{
		return 0;
	}else
	{
		return 1;
	}
}
