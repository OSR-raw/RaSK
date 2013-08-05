/********************************************************************************
** Form generated from reading UI file 'qtrask.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTRASK_H
#define UI_QTRASK_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "oglwidget.h"
#include "paintarea.h"

QT_BEGIN_NAMESPACE

class Ui_QtRaSKClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    OGLWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_3;
    QSpacerItem *verticalSpacer;
    PaintArea *pain_w;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QMainWindow *QtRaSKClass)
    {
        if (QtRaSKClass->objectName().isEmpty())
            QtRaSKClass->setObjectName(QStringLiteral("QtRaSKClass"));
        QtRaSKClass->resize(709, 551);
        QtRaSKClass->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        centralWidget = new QWidget(QtRaSKClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setAutoFillBackground(true);
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(4);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        widget = new OGLWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setBaseSize(QSize(506, 404));
        widget->setFocusPolicy(Qt::StrongFocus);

        horizontalLayout_2->addWidget(widget);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        verticalLayout_3->addWidget(pushButton_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        pain_w = new PaintArea(centralWidget);
        pain_w->setObjectName(QStringLiteral("pain_w"));
        pain_w->setBaseSize(QSize(256, 64));

        verticalLayout_3->addWidget(pain_w);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addLayout(verticalLayout_3);

        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        verticalLayout_4->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        verticalLayout_4->addWidget(pushButton_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout_4->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout_4);

        QtRaSKClass->setCentralWidget(centralWidget);

        retranslateUi(QtRaSKClass);
        QObject::connect(pushButton, SIGNAL(clicked()), QtRaSKClass, SLOT(brushColor()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), QtRaSKClass, SLOT(brushWidth()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), QtRaSKClass, SLOT(loadData()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), QtRaSKClass, SLOT(updateTRF()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), QtRaSKClass, SLOT(resetColor()));

        QMetaObject::connectSlotsByName(QtRaSKClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtRaSKClass)
    {
        QtRaSKClass->setWindowTitle(QApplication::translate("QtRaSKClass", "RaSK: RaycastStereoKinect", 0));
        pushButton_3->setText(QApplication::translate("QtRaSKClass", "&LoadData", 0));
        pushButton->setText(QApplication::translate("QtRaSKClass", "&Set Color", 0));
        pushButton_2->setText(QApplication::translate("QtRaSKClass", "Set width", 0));
        pushButton_4->setText(QApplication::translate("QtRaSKClass", "Apply TF", 0));
        pushButton_5->setText(QApplication::translate("QtRaSKClass", "Clear TF", 0));
    } // retranslateUi

};

namespace Ui {
    class QtRaSKClass: public Ui_QtRaSKClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTRASK_H
