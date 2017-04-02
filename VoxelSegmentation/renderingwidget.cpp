#include "renderingwidget.h"
#include <QKeyEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QTextCodec>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <algorithm>
#include "mainwindow.h"
#include "ArcBall.h"
#include "globalFunctions.h"
#include <cstdlib>
#include <ctime>

RenderingWidget::RenderingWidget(QWidget *parent, MainWindow* mainwindow)
: QGLWidget(parent), ptr_mainwindow_(mainwindow), eye_distance_(2.0)
{
	ptr_arcball_ = new CArcBall(width(), height());
	eye_goal_.clear();
	eye_goal_.resize(3);
	eye_goal_[0]=(0.0);
	eye_goal_[1]=(0.0);
	eye_goal_[2]=(0.0);
	eye_direction_.clear();
	eye_direction_.resize(3);
	eye_direction_[0]=(0.0);
	eye_direction_[1]=(0.0);
	eye_direction_[2]=(1.0);
	check_rgb=false;
	check_depth=false;
	check_segment=false;
	//initializeGL();
}

RenderingWidget::~RenderingWidget()
{
	SafeDelete(ptr_arcball_);
//	SafeDelete(ptr_mesh_);
	//SafeDelete(mesh);
}

void RenderingWidget::initializeGL()
{
	glClearColor(0.3,0.3,0.3,0.0);

	//glBegin(GL_LINES);
	//glColor3f(0,0,0);
	//glVertex3f(5,5,-5); 
	//glVertex3f(5,15,-5);
	//glEnd(); 

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DOUBLEBUFFER);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1);
	//SetLight();
}

void RenderingWidget::resizeGL(int w, int h)
{
	h = (h == 0) ? 1 : h;

	ptr_arcball_->reSetBound(w, h);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, GLdouble(w) / GLdouble(h), 0.001, 2000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderingWidget::paintGL()
{
	glShadeModel(GL_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vector<float> eyepos;
	eyepos.resize(3);
	eyepos[0]=eye_distance_*eye_direction_[0];
	eyepos[1]=eye_distance_*eye_direction_[1];
	eyepos[2]=eye_distance_*eye_direction_[2];
	gluLookAt(eyepos[0], eyepos[1], eyepos[2],
		eye_goal_[0], eye_goal_[1], eye_goal_[2],
		0.0, 1.0, 0.0); 

	glPushMatrix();
	glMultMatrixf(ptr_arcball_->GetBallMatrix());
	
	if (check_depth==true&&check_rgb==true)
	{
		Render();
	}
	glPopMatrix();
}

void RenderingWidget::mousePressEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		ptr_arcball_->MouseDown(e->pos());
		break;
	case Qt::MidButton:
		current_position_ = e->pos();
		break;
	default:
		break;
	}

	updateGL();
}
void RenderingWidget::mouseMoveEvent(QMouseEvent *e)
{
	switch (e->buttons())
	{
		setCursor(Qt::ClosedHandCursor);
	case Qt::LeftButton:
		ptr_arcball_->MouseMove(e->pos());
		break;
	case Qt::MidButton:
		eye_goal_[0] -= 4.0*GLfloat(e->x() - current_position_.x()) / GLfloat(width());
		eye_goal_[1] += 4.0*GLfloat(e->y() - current_position_.y()) / GLfloat(height());
		current_position_ = e->pos();
		break;
	default:
		break;
	}
	updateGL();
}
void RenderingWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		break;
	default:
		break;
	}
	updateGL();
}
void RenderingWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		ptr_arcball_->MouseUp(e->pos());
		setCursor(Qt::ArrowCursor);
		break;

	case Qt::RightButton:
		break;
	default:
		break;
	}
}

void RenderingWidget::wheelEvent(QWheelEvent *e)
{
	eye_distance_ += e->delta()*0.001;
	eye_distance_ = eye_distance_ < 0 ? 0 : eye_distance_;

	updateGL();
}

void RenderingWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_A:
		break;
	default:
		break;
	}
}

void RenderingWidget::keyReleaseEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_A:
		break;
	default:
		break;
	}
}

//void RenderingWidget::SetLight()
//{
//	static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//	static GLfloat mat_shininess[] = { 50.0 };
//	static GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
//	static GLfloat white_light[] = { 0.8, 0.8, 0.8, 1.0 };
//	static GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
//
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
//
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//}


void RenderingWidget::Read_Rgb()
{
	QString filename = QFileDialog::
		getOpenFileName(this, tr("Open RGB Image"),
		".", tr("Meshes (*.mat)"));
	QByteArray ba = filename.toLatin1(); 
	char* ch=ba.data();
	cout<<ch<<endl;
	MATFile *pmatFile=matOpen(ch,"r"); 
	const char *name;	
	 mxArray *pMxArray=matGetNextVariable(pmatFile,&name);
	 pMxArray=matGetNextVariable(pmatFile,&name);
	 img_rgb = (unsigned char*) mxGetPr(pMxArray);
	 matClose(pmatFile);
	 if (NULL!=img_rgb)
		 check_rgb=true;

	 if (check_depth==true&&check_rgb==true)
	 {
		 image_info.init(check_rgb,check_depth,img_rgb,img_depth);
	 }
}

void RenderingWidget::Read_Depth()
{
	QString filename = QFileDialog::
		getOpenFileName(this, tr("Open RGB Image"),
		".", tr("Meshes (*.mat)"));
	QByteArray ba = filename.toLatin1(); 
	char* ch=ba.data();
	cout<<ch<<endl;
	MATFile *pmatFile=matOpen(ch,"r"); 
	const char *depname;
	mxArray *pMxArray=matGetNextVariable(pmatFile,&depname);
	pMxArray=matGetNextVariable(pmatFile,&depname);
	img_depth = (float*) mxGetPr(pMxArray);
	cout<<"M:"<<mxGetM(pMxArray)<<" N:"<<mxGetN(pMxArray)<<endl;
	image_info.setM(mxGetM(pMxArray));    
	image_info.setN(mxGetN(pMxArray));  
	cout<<"M:"<<image_info.getM()<<" N:"<<image_info.getN()<<endl;
	matClose(pmatFile);
	if (image_info.getM()>=0&&image_info.getN()>=0)
		check_depth=true;
	if (check_depth==true&&check_rgb==true)
	{
		image_info.init(check_rgb,check_depth,img_rgb,img_depth);
	}
}

void RenderingWidget::Do_Segmentation()
{
	seg=Segmentation(image_info);
	seg.do_segment();
	check_segment=true;

}

void RenderingWidget::Render()
{
		int m=image_info.getM();
		int n=image_info.getN();
		if (check_segment==false)
		{
			//render
			glEnable(GL_COLOR_MATERIAL);
			glPointSize(1.2f);
			glBegin(GL_POINTS);

			for (int i=0;i<m*n;i++)
			{
				glColor3f((float)image_info.vertex_rgb[i].r/255.0, (float)image_info.vertex_rgb[i].g/255.0, (float)image_info.vertex_rgb[i].b/255.0);
				//glVertex3f(image_info.vertex_pos[i].x,image_info.vertex_pos[i].y,image_info.vertex_pos[i].z);
				glVertex3f(image_info.vertex_pos[i].x,image_info.vertex_pos[i].y,image_info.vertex_pos[i].z);
				
				/*cout<<"x:"<<image_info.vertex_pos[i].x<<" y:"<<image_info.vertex_pos[i].y<<" z:"<<image_info.vertex_pos[i].z<<endl;*/
				//glVertex3f(image_info.vertex_index[i].x-n/2,image_info.vertex_index[i].y-m/2,0);
			}
			glEnd();
		}else
		{
			glEnable(GL_COLOR_MATERIAL);
			glPointSize(1.2f);
			glBegin(GL_POINTS);
			size_t size=seg.list_center.size();
			//draw every cluster with different color
			for (int i=0;i<m*n;i++)
			{
				glColor3f((float)seg.image_segmented.vertex_rgb[i].r/255.0,(float)seg.image_segmented.vertex_rgb[i].g/255.0,(float)seg.image_segmented.vertex_rgb[i].b/255.0);
				glVertex3f(seg.image_segmented.vertex_pos[i].x,seg.image_segmented.vertex_pos[i].y,seg.image_segmented.vertex_pos[i].z);
				//glVertex3f(image_info.vertex_index[i].x-n/2,image_info.vertex_index[i].y-m/2,0);
			}

			//draw seed center with black 
			/*glPointSize(5.0f);
			for (size_t i=0;i<size;i++)
			{
				int v=seg.list_center[i];
				glColor3f(1.0,1.0,1.0);
				glVertex3f(image_info.vertex_pos[v].x,image_info.vertex_pos[v].y,image_info.vertex_pos[v].z);
			}*/
			glEnd();
		}

}

