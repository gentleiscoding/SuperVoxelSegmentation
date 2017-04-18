#include "imagewidge.h"
#include <QImage>
#include <QPainter>

ImageWidget::ImageWidget(ChildWindow *relatewindow,Image_Info image_info)
{
	image=image_info;
	image_ = new QImage(image.getN(),image.getM(),QImage::Format_RGB32);
	for (int i=0;i<image.getM();i++)
	{
		for (int j=0;j<image.getN();j++)
		{
			QRgb value=qRgb(image.vertex_rgb[j*image.getM()+i].r,image.vertex_rgb[j*image.getM()+i].g,image.vertex_rgb[j*image.getM()+i].b);
			image_->setPixel(j,i,value);
		}
	}
	
}


ImageWidget::~ImageWidget()
{

}

void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	painter.begin(this);
	QRect rect = QRect( 0, 0, image.getN(), image.getM());
	painter.drawImage(rect, *image_); 
	painter.end();
}
