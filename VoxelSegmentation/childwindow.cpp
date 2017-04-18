#include "childwindow.h"

ChildWindow::ChildWindow(Image_Info image_info,double x,double y)
{
	setGeometry(x,y,image_info.getN(),image_info.getM());
	this->resize(image_info.getN(),image_info.getM());
	imagewidget_ = new ImageWidget(this,image_info);
	setCentralWidget(imagewidget_);
}

ChildWindow::~ChildWindow()
{

}
