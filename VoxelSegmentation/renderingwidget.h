#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H
#include <mat.h>
#include <QGLWidget>
#include <QEvent>
#include "Image_Info.h"
#include "Segmentation.h"
class MainWindow;
class CArcBall;

using namespace std;
class RenderingWidget : public QGLWidget
{
	Q_OBJECT

public:
	RenderingWidget(QWidget *parent, MainWindow* mainwindow=0);
	~RenderingWidget();

	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

	float random_color();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	// mouse events
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);

signals:
	void operatorInfo(QString);

private:
	//void SetLight();

public slots:
	void Read_Rgb();
	void Read_Depth();
	void Render();
	void Do_Segmentation();

public:
	MainWindow					*ptr_mainwindow_;
	CArcBall					*ptr_arcball_;

	// eye
	GLfloat						eye_distance_;
	vector<float>				eye_goal_;
	vector<float>				eye_direction_;
	QPoint						current_position_;

	//img
	unsigned char *img_rgb;
	float	*img_depth; 
	bool	check_rgb;
	bool	check_depth;
	bool	check_segment;
	Image_Info	image_info;

	//segmentation
	Segmentation seg;
	
};

#endif // RENDERINGWIDGET_H
