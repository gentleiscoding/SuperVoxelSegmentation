#include "mainwindow.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QMessageBox>
#include <QKeyEvent>
#include "renderingwidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	 
	renderingwidget_ = new RenderingWidget(this);
//	setCentralWidget(renderingwidget_);

	setGeometry(300, 150, 800, 600);

	CreateActions();
	CreateMenus();
	CreateStatusBar();



	QHBoxLayout *layout_main = new QHBoxLayout;

	layout_main->addWidget(renderingwidget_);
	layout_main->setStretch(1, 1);
	this->centralWidget()->setLayout(layout_main);

}

MainWindow::~MainWindow()
{

}

void MainWindow::CreateActions()
{
	action_new_ = new QAction(QIcon(":/MainWindow/Resources/images/new.png"), tr("&New"), this);
	action_new_->setShortcut(QKeySequence::New);
	action_new_->setStatusTip(tr("Create a new file"));

	action_read_rgb_ = new QAction(QIcon(":/MainWindow/Resources/images/open.png"), tr("&Read RGB"), this);
	action_read_rgb_->setStatusTip(tr("Open an RGB Image"));
	connect(action_read_rgb_, SIGNAL(triggered()), renderingwidget_, SLOT(Read_Rgb()));

	action_read_depth_ = new QAction(QIcon(":/MainWindow/Resources/images/open.png"), tr("&Read Depth"), this);
	action_read_depth_->setStatusTip(tr("Open an RGB Image"));
	connect(action_read_depth_, SIGNAL(triggered()), renderingwidget_, SLOT(Read_Depth()));

	action_segment_ = new QAction(tr("segment"), this);
	action_segment_->setStatusTip(tr("segment the image"));
	connect(action_segment_, SIGNAL(triggered()), renderingwidget_, SLOT(Do_Segmentation()));

	action_save_ = new QAction(QIcon(":/MainWindow/Resources/images/save.png"), tr("&Save"), this);
	action_save_->setShortcuts(QKeySequence::Save);
	action_save_->setStatusTip(tr("Save the document to disk"));

	action_saveas_ = new QAction(tr("Save &As..."), this);
	action_saveas_->setShortcuts(QKeySequence::SaveAs);
	action_saveas_->setStatusTip(tr("Save the document under a new name"));

	action_about_ = new QAction(tr("about"), this);
}

void MainWindow::CreateMenus()
{
	menu_file_ = menuBar()->addMenu(tr("&File"));
	menu_file_->setStatusTip(tr("File menu"));
	menu_file_->addAction(action_read_rgb_);
	menu_file_->addAction(action_read_depth_);
	menu_file_->addAction(action_save_);
	menu_file_->addAction(action_saveas_);

	menu_file_ = menuBar()->addMenu(tr("&Edit"));
	menu_file_->addAction(action_segment_);

	menu_file_ = menuBar()->addMenu(tr("&About"));
	menu_file_->addAction(action_about_);
}

void MainWindow::CreateStatusBar()
{
	label_imageinfo_ = new QLabel(QString("ImageInfo: p: %1 e: %2 f: %3").arg(0).arg(0).arg(0));
	label_imageinfo_->setAlignment(Qt::AlignCenter);
	label_imageinfo_->setMinimumSize(label_imageinfo_->sizeHint());

	label_operatorinfo_ = new QLabel();
	label_operatorinfo_->setAlignment(Qt::AlignVCenter);
	

	statusBar()->addWidget(label_imageinfo_);
	connect(renderingwidget_, SIGNAL(imageInfo(int, int, int)), this, SLOT(ShowImageInfo(int, int, int)));

	statusBar()->addWidget(label_operatorinfo_);
	connect(renderingwidget_, SIGNAL(operatorInfo(QString)), label_operatorinfo_, SLOT(setText(QString)));
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{

}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{

}

void MainWindow::ShowImageInfo(int npoint, int nedge, int nface)
{
	label_imageinfo_->setText(QString("ImageInfo: p: %1 e: %2 f: %3").arg(npoint).arg(nedge).arg(nface));
}

void MainWindow::OpenFile()
{

}

void MainWindow::ShowAbout()
{
	QMessageBox::information(this, "About QtMeshFrame-1.0.1",

		QString("<h3>This MeshFrame provides some operations about *.obj files sunch as") +
		" IO, render with points , edges, triangles or textures and some interactions with mouse."
		" A fix light source is provided for you."
		"This is a basic and raw frame for handling meshes. The mesh is of half_edge struct.\n"
		"Please contact" "<font color=blue> wkcagd@mail.ustc.edu.cn<\font><font color=black>, Kang Wang if you has any questions.<\font><\h3>"
		,
		QMessageBox::Ok);
}