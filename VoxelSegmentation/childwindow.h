#pragma once
#include <QMainWindow>
#include "Image_Info.h"
#include "imagewidge.h"
QT_BEGIN_NAMESPACE
class QImage;
class ImageWidget;
QT_END_NAMESPACE

class ChildWindow :
	public QMainWindow
{
	Q_OBJECT

public:
	ChildWindow(Image_Info,double,double);
	~ChildWindow(void);
public:
	ImageWidget			*imagewidget_;
	//Image_Info		    *image;
};

