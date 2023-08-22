#include "mainwindow.h"

QTranslator translator;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_plot(new Plot(this)), m_rbSincR(new QRadioButton("sinc1", this)),
    m_rbSincXZ(new QRadioButton("sinc2", this)), m_rbNoSelection(new QRadioButton(tr("No selection mode"), this)),
    m_rbItemSelection(new QRadioButton(tr("Item"), this)), m_groupBoxChoosePlot(new QGroupBox(tr("Plot"), this)),
    m_groupBoxSelectionMode(new QGroupBox(tr("Selection Mode"), this)), m_cbShowGrid(new QCheckBox(tr("Show grid"), this)),
    m_cbShowLabelBorder(new QCheckBox(tr("Show label border"), this)), m_cbShowLabel(new QCheckBox(tr("Show label"), this)),
    m_columnSlider1(new QSlider(Qt::Horizontal, this)), m_columnSlider2(new QSlider(Qt::Horizontal, this)),
    m_rowSlider1(new QSlider(Qt::Horizontal, this)), m_rowSlider2(new QSlider(Qt::Horizontal, this)),
    m_firstColor(new QPushButton(this)), m_secondColor(new QPushButton(this)), m_colorHBox(new QHBoxLayout(this)),
    m_tb(new QToolBar(this)), m_saveButton(new QAction(tr("Save"), this)), m_menu(new QMenuBar(this)),
    m_switchToRussian(new QAction(tr("Русский"), this)), m_switchToEnglish(new QAction(tr("English"), this))
{
    resize(800, 600);

    m_tb->addAction(m_saveButton);
    m_saveButton->setStatusTip(tr("Save the settings"));
    QMenu *saveMenu = m_menu->addMenu(tr("File"));
    saveMenu->addAction(m_saveButton);
    saveMenu->setStatusTip(tr("Save the settings"));
    QMenu *languageMenu = m_menu->addMenu(tr("Language"));
    languageMenu->addAction(m_switchToRussian);
    languageMenu->addAction(m_switchToEnglish);
    this->setMenuBar(m_menu);

    QVBoxLayout *vLayoutChoosePlot = new QVBoxLayout;
    vLayoutChoosePlot->addWidget(m_rbSincR);
    vLayoutChoosePlot->addWidget(m_rbSincXZ);
    m_groupBoxChoosePlot->setLayout(vLayoutChoosePlot);

    QVBoxLayout *vLayoutSelectionMode = new QVBoxLayout;
    vLayoutSelectionMode->addWidget(m_rbNoSelection);
    vLayoutSelectionMode->addWidget(m_rbItemSelection);
    m_groupBoxSelectionMode->setLayout(vLayoutSelectionMode);

    QVBoxLayout *vLayoutSettings = new QVBoxLayout;
    vLayoutSettings->addWidget(m_cbShowGrid);
    vLayoutSettings->addWidget(m_cbShowLabel);
    vLayoutSettings->addWidget(m_cbShowLabelBorder);
    vLayoutSettings->addWidget(m_groupBoxChoosePlot);
    vLayoutSettings->addWidget(m_groupBoxSelectionMode);
    vLayoutSettings->addWidget(new QLabel(tr("Column range")));
    vLayoutSettings->addWidget(m_columnSlider1);
    vLayoutSettings->addWidget(m_columnSlider2);
    vLayoutSettings->addWidget(new QLabel(tr("Row range")));
    vLayoutSettings->addWidget(m_rowSlider1);
    vLayoutSettings->addWidget(m_rowSlider2);

    QGroupBox *colorGroupBox = new QGroupBox(tr("Custom gradient"));

    QLinearGradient grBtoY(0, 0, 1, 100);
    grBtoY.setColorAt(1.0, Qt::black);
    grBtoY.setColorAt(0.67, Qt::blue);
    grBtoY.setColorAt(0.33, Qt::red);
    grBtoY.setColorAt(0.0, Qt::yellow);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grBtoY));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    m_firstColor->setIcon(QIcon(pm));
    m_firstColor->setIconSize(QSize(24, 100));

    QLinearGradient grGtoR(0, 0, 1, 100);
    grGtoR.setColorAt(1.0, Qt::darkGreen);
    grGtoR.setColorAt(0.5, Qt::yellow);
    grGtoR.setColorAt(0.2, Qt::red);
    grGtoR.setColorAt(0.0, Qt::darkRed);
    pmp.setBrush(QBrush(grGtoR));
    pmp.drawRect(0, 0, 24, 100);
    m_secondColor->setIcon(QIcon(pm));
    m_secondColor->setIconSize(QSize(24, 100));

    m_colorHBox->addWidget(m_firstColor);
    m_colorHBox->addWidget(m_secondColor);
    colorGroupBox->setLayout(m_colorHBox);

    vLayoutSettings->addWidget(colorGroupBox);

    m_columnSlider1->setRange(0, 50);
    m_columnSlider2->setRange(0, 50);
    m_rowSlider1->setRange(0, 50);
    m_rowSlider2->setRange(0, 50);
    m_columnSlider2->setValue(50);
    m_rowSlider2->setValue(50);

    m_columnSlider1->setMinimum(0);
    m_columnSlider1->setTickInterval(1);
    m_columnSlider1->setEnabled(true);

    m_columnSlider2->setMinimum(1);
    m_columnSlider2->setTickInterval(1);
    m_columnSlider2->setEnabled(true);

    m_rowSlider1->setMinimum(0);
    m_rowSlider1->setTickInterval(1);
    m_rowSlider1->setEnabled(true);

    m_rowSlider2->setMinimum(1);
    m_rowSlider2->setTickInterval(1);
    m_rowSlider2->setEnabled(true);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_plot, 4);
    mainLayout->addLayout(vLayoutSettings, 1);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);

    this->addToolBar(m_tb);

    connect(m_rbSincR, &QRadioButton::toggled, this, &MainWindow::onFunctionChanged);
    connect(m_rbSincXZ, &QRadioButton::toggled, this, &MainWindow::onFunctionChanged);
    connect(m_rbNoSelection, &QRadioButton::toggled, this, &MainWindow::onSelectionModeChanged);
    connect(m_rbItemSelection, &QRadioButton::toggled, this, &MainWindow::onSelectionModeChanged);
    connect(m_cbShowGrid, &QCheckBox::toggled, this, &MainWindow::onShowGridChanged);
    connect(m_cbShowLabelBorder, &QCheckBox::toggled, this, &MainWindow::onShowLabelBorderChanged);
    connect(m_cbShowLabel, &QCheckBox::toggled, this, &MainWindow::onShowLabelChanged);

    connect(m_columnSlider1, &QSlider::valueChanged, m_plot, &Plot::adjustXMin);
    connect(m_columnSlider2, &QSlider::valueChanged, m_plot, &Plot::adjustXMax);
    connect(m_rowSlider1, &QSlider::valueChanged, m_plot, &Plot::adjustZMin);
    connect(m_rowSlider2, &QSlider::valueChanged, m_plot, &Plot::adjustZMax);

    connect(m_cbShowLabel, &QCheckBox::toggled, m_cbShowLabelBorder, &QCheckBox::setEnabled);

    connect(m_firstColor, &QPushButton::pressed, m_plot, &Plot::setFirstColor);
    connect(m_secondColor, &QPushButton::pressed, m_plot, &Plot::setSecondColor);

    connect(m_saveButton, &QAction::triggered, this, &MainWindow::saveSettings);

    connect(m_switchToRussian, &QAction::triggered, this, &MainWindow::switchToRussian);
    connect(m_switchToEnglish, &QAction::triggered, this, &MainWindow::switchToEnglish);

    m_plot->setAxisMinSliderX(m_columnSlider1);
    m_plot->setAxisMaxSliderX(m_columnSlider2);
    m_plot->setAxisMinSliderZ(m_rowSlider1);
    m_plot->setAxisMaxSliderZ(m_rowSlider2);

    m_rbSincR->setChecked(true);
    m_rbNoSelection->setChecked(true);
    m_cbShowGrid->setChecked(true);
    m_cbShowLabel->setChecked(true);
    m_cbShowLabelBorder->setChecked(true);

    loadSettings();
}

MainWindow::~MainWindow() {}

void MainWindow::onFunctionChanged()
{
    if (m_rbSincR->isChecked())
    {
        m_plot->plot(Plot::SincR);
    }
    else if (m_rbSincXZ->isChecked())
    {
        m_plot->plot(Plot::SincXZ);
    }
}

void MainWindow::onSelectionModeChanged()
{
    if (m_rbNoSelection->isChecked())
    {
        m_plot->selection(false);
        this->statusBar()->showMessage(tr("no selection"));
    }
    else if (m_rbItemSelection->isChecked())
    {
        m_plot->selection(true);
        this->statusBar()->showMessage(tr("point can be selected"));
    }
}

void MainWindow::onShowGridChanged(bool checked)
{
    m_plot->grid(checked);
}

void MainWindow::onShowLabelChanged(bool checked)
{
    m_plot->showLabels(checked);
    if (!checked)
    {
        m_plot->showLabelBorders(checked);
    }
    else if (m_cbShowLabelBorder->isChecked())
    {
        m_plot->showLabelBorders(true);
    }
    else
    {
        m_plot->showLabelBorders(false);
    }
}
void MainWindow::onShowLabelBorderChanged(bool checked)
{
    m_plot->showLabelBorders(checked);
}

void MainWindow::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    if (m_rbSincR->isChecked())
        settings.setValue("functionType", "SincR");
    else if (m_rbSincXZ->isChecked())
        settings.setValue("functionType", "SincXZ");

    settings.setValue("showGrid", m_cbShowGrid->isChecked());
    settings.setValue("showLabel", m_cbShowLabel->isChecked());
    settings.setValue("showLabelBorder", m_cbShowLabelBorder->isChecked());

    settings.setValue("selectionOff", m_rbNoSelection->isChecked());
    settings.setValue("selectionOn", m_rbItemSelection->isChecked());
}

void MainWindow::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    QString functionType = settings.value("functionType", "SincR").toString();
    if (functionType == "SincR")
        m_rbSincR->setChecked(true);
    else if (functionType == "SincXZ")
        m_rbSincXZ->setChecked(true);

    m_cbShowGrid->setChecked(settings.value("showGrid", true).toBool());
    m_cbShowLabel->setChecked(settings.value("showLabel", true).toBool());
    m_cbShowLabelBorder->setChecked(settings.value("showLabelBorder", true).toBool());

    m_rbNoSelection->setChecked(settings.value("selectionOff", true).toBool());
    m_rbItemSelection->setChecked(settings.value("selectionOn", true).toBool());
}

void MainWindow::switchToRussian()
{
    QApplication::instance()->removeTranslator(&translator);
    if (translator.load("translation_ru"))
    {
        QApplication::instance()->installTranslator(&translator);
    }
}

void MainWindow::switchToEnglish()
{
    QApplication::instance()->removeTranslator(&translator);
}
