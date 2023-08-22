#ifndef PLOT_H
#define PLOT_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>

#include <Q3DTheme>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>

using namespace QtDataVisualization;

class Plot : public QWidget
{
    Q_OBJECT

  public:
    explicit Plot(QWidget *parent = nullptr);
    ~Plot();
    enum FunctionType
    {
        SincR,
        SincXZ
    };

    void plot(FunctionType type);
    void grid(bool show);
    void selection(bool show);
    void showLabels(bool show);
    void showLabelBorders(bool show);

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);

    void setAxisMinSliderX(QSlider *slider) { m_minSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_maxSliderX = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_minSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_maxSliderZ = slider; }

    void setFirstColor();
    void setSecondColor();

  private:
    Q3DSurface *m_graph;
    QSurface3DSeries *m_series;
    Q3DTheme *m_theme;

    float m_stepX;
    float m_stepZ;
    float m_rangeMinX;
    float m_rangeMinZ;

    QSlider *m_minSliderX;
    QSlider *m_maxSliderX;
    QSlider *m_minSliderZ;
    QSlider *m_maxSliderZ;
};

#endif	  // PLOT_H
