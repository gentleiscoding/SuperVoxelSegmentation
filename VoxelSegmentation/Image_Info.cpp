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
					//cout<<"x:"<<j<<" y:"<<i<<" z:"<<img_depth[j*M+i]<<endl;
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