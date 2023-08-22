#include "plot.h"

#include <QtMath>
#include <cmath>

Plot::Plot(QWidget *parent) :
    QWidget(parent), m_graph(new Q3DSurface), m_series(new QSurface3DSeries), m_theme(new Q3DTheme(Q3DTheme::ThemeQt))
{
    m_graph->setActiveTheme(m_theme);
    m_graph->activeTheme()->setLabelTextColor(Qt::black);

    QWidget *container = QWidget::createWindowContainer(m_graph);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(container);
    setLayout(layout);

    m_graph->addSeries(m_series);
}

Plot::~Plot()
{
    delete m_graph;
    delete m_series;
    delete m_theme;
}

void Plot::plot(FunctionType type)
{
    QSurfaceDataProxy *proxy = new QSurfaceDataProxy();
    m_rangeMinX = -8.0f, m_rangeMinZ = -8.0f;
    float minX = -8.0f, maxX = 8.0f;
    float minZ = -8.0f, maxZ = 8.0f;

    int resolutionX = 50, resolutionZ = 50;
    m_stepX = (maxX - minX) / (resolutionX - 1);
    m_stepZ = (maxZ - minZ) / (resolutionZ - 1);
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(resolutionZ);

    for (int i = 0; i < resolutionZ; i++)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(resolutionX);
        float z = qMin(maxX, (i * m_stepZ + minX));
        int index = 0;
        for (int j = 0; j < resolutionX; j++)
        {
            float x = qMin(maxX, (j * m_stepX + minX));
            float R = qSqrt(z * z + x * x) + 0.01f;
            float y;
            switch (type)
            {
            case SincR:
                y = (qSin(R) / R + 0.24f) * 1.61f;
                break;
            case SincXZ:
                y = (qSin(x) / x) * (qSin(z) / z);
                break;
            }

            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }

    proxy->resetArray(dataArray);

    m_series->setSelectedPoint({ -1, -1 });
    m_series->dataProxy()->resetArray(dataArray);
}

void Plot::grid(bool show)
{
    m_theme->setGridEnabled(show);
    m_graph->setActiveTheme(m_theme);
}

void Plot::selection(bool show)
{
    if (show)
    {
        m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem);
    }
    else
    {
        m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone);
    }
}

void Plot::showLabelBorders(bool show)
{
    if (show)
    {
        m_graph->activeTheme()->setLabelBorderEnabled(true);
    }
    else
    {
        m_graph->activeTheme()->setLabelBorderEnabled(false);
    }
}

void Plot::showLabels(bool show)
{
    if (show)
    {
        m_graph->activeTheme()->setLabelTextColor(Qt::black);
    }
    else
    {
        m_graph->activeTheme()->setLabelTextColor(Qt::white);
    }
}

void Plot::adjustXMin(int min)
{
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_maxSliderX->value();
    if (min >= max)
    {
        max = min + 1;
        m_maxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void Plot::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_minSliderX->value();
    if (max <= min)
    {
        min = max - 1;
        m_minSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void Plot::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_maxSliderZ->value();
    if (min >= max)
    {
        max = min + 1;
        m_maxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void Plot::adjustZMax(int max)
{
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_minSliderZ->value();
    if (max <= min)
    {
        min = max - 1;
        m_minSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

void Plot::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void Plot::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void Plot::setFirstColor()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void Plot::setSecondColor()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}
