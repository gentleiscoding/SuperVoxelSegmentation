#pragma once
#include <QWidget>
#include <Eigen/Core>
#include "Image_Info.h"
class ChildWindow;
QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

class ImageWidget :
	public QWidget
{
	Q_OBJECT
private:
	QImage						*image_;						// image 
	Image_Info					image;

public:
	ImageWidget(ChildWindow *relatewindow,Image_Info image_info);
	~ImageWidget(void);


protected:
	void paintEvent(QPaintEvent *paintevent);
};

