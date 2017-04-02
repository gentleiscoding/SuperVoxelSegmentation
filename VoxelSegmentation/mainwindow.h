#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"	

class QLabel;
class QPushButton;
class QCheckBox;
class QGroupBox;
class RenderingWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void CreateActions();
	void CreateMenus();
	void CreateStatusBar();


protected:
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

	public slots:
	void ShowImageInfo(int npoint, int nedge, int nface);
	void OpenFile();
	void ShowAbout();


private:
	Ui::MainWindowClass ui;

	// Basic
	QMenu							*menu_file_;
	QMenu							*menu_edit_;
	QMenu							*menu_help_;

	QAction							*action_new_;
	QAction							*action_read_rgb_;
	QAction							*action_read_depth_;
	QAction							*action_segment_;
	QAction							*action_save_;
	QAction							*action_saveas_;
	QAction							*action_about_;

	// Information
	QLabel							*label_imageinfo_;
	QLabel							*label_operatorinfo_;

	RenderingWidget					*renderingwidget_;
};

#endif // MAINWINDOW_H
