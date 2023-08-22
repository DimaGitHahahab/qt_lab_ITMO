#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "plot.h"

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QMainWindow>
#include <QMenuBar>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QSlider>
#include <QStatusBar>
#include <QToolBar>
#include <QTranslator>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void onFunctionChanged();
    void onSelectionModeChanged();
    void onShowGridChanged(bool checked);
    void onShowLabelChanged(bool checked);
    void onShowLabelBorderChanged(bool checked);

  private:
    Plot *m_plot;
    QRadioButton *m_rbSincR;
    QRadioButton *m_rbSincXZ;
    QRadioButton *m_rbNoSelection;
    QRadioButton *m_rbItemSelection;
    QGroupBox *m_groupBoxChoosePlot;
    QGroupBox *m_groupBoxSelectionMode;
    QCheckBox *m_cbShowGrid;
    QCheckBox *m_cbShowLabelBorder;
    QCheckBox *m_cbShowLabel;
    QSlider *m_columnSlider1;
    QSlider *m_columnSlider2;
    QSlider *m_rowSlider1;
    QSlider *m_rowSlider2;
    QPushButton *m_firstColor;
    QPushButton *m_secondColor;
    QStatusBar *m_sb;
    QHBoxLayout *m_colorHBox;
    QToolBar *m_tb;
    QAction *m_saveButton;
    QMenuBar *m_menu;
    QAction *m_switchToRussian;
    QAction *m_switchToEnglish;
    void saveSettings();
    void loadSettings();
    void switchToRussian();
    void switchToEnglish();
};

#endif	  // MAINWINDOW_H
