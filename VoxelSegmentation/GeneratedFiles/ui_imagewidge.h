/********************************************************************************
** Form generated from reading UI file 'imagewidge.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEWIDGE_H
#define UI_IMAGEWIDGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageWidge
{
public:

    void setupUi(QWidget *ImageWidge)
    {
        if (ImageWidge->objectName().isEmpty())
            ImageWidge->setObjectName(QStringLiteral("ImageWidge"));
        ImageWidge->resize(400, 300);

        retranslateUi(ImageWidge);

        QMetaObject::connectSlotsByName(ImageWidge);
    } // setupUi

    void retranslateUi(QWidget *ImageWidge)
    {
        ImageWidge->setWindowTitle(QApplication::translate("ImageWidge", "ImageWidge", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageWidge: public Ui_ImageWidge {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEWIDGE_H
