#ifndef EQWIDGET_H
#define EQWIDGET_H

#include "qcustomplot.h"
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <QStringList>

#define NUM_BANDS 15
const double bands[NUM_BANDS] = { 25.0, 40.0, 63.0, 100.0, 160.0, 250.0, 400.0, 630.0, 1000.0, 1600.0, 2500.0, 4000.0, 6300.0, 10000.0, 16000.0 };
const QStringList bands_str = QStringList() << "25" << "40" << "63" << "100" << "160" << "250" << "400" << "630" << "1k" << "1.6k" << "2.5k" << "4k" << "6.3k" << "10k" << "16k";

class QEqualizerView : public QCustomPlot
{
    Q_OBJECT
public:
    QEqualizerView(QWidget* parent);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void setDragInfo( int graph = -1, int distance = -1);
signals:
    void DataChanged(void);
    void EditingFinished(void);
private:
    int drag_number = -1;
    int dragable_graph_number = -1;
    int max_distance_to_add_point = -1;
};

#endif // EQWIDGET_H
