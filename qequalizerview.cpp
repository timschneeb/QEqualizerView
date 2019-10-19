#include "qequalizerview.h"

QEqualizerView::QEqualizerView(QWidget* parent) : QCustomPlot(parent)
{
    xAxis->setScaleType(QCPAxis::stLogarithmic);
    setInteraction(QCP::Interaction::iRangeDrag,true);
    setInteraction(QCP::Interaction::iRangeZoom,true);
    yAxis->setRange(QCPRange(-12, 12));
    xAxis->setRange(QCPRange(25, 16000));
    addGraph();
    addGraph();
    setDragInfo(0, 20); // data of graph(0) is dragable, 20px margin

    graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph(0)->setPen(QPen(QColor(120, 120, 120), 2));

    for(int i = 0; i < NUM_BANDS; i++)
        graph(0)->addData(bands[i],0);

    QSharedPointer<QCPAxisTickerText> fixedTicker(new QCPAxisTickerText);
    xAxis->setTicker(fixedTicker);
    fixedTicker->setTickCount(NUM_BANDS);
    for(int i = 0; i < NUM_BANDS; i++)
        fixedTicker->addTick(bands[i],bands_str.at(i));

    xAxis->setBasePen(QPen(Qt::white, 1));
    yAxis->setBasePen(QPen(Qt::white, 1));
    xAxis->setTickPen(QPen(Qt::white, 1));
    yAxis->setTickPen(QPen(Qt::white, 1));
    xAxis->setSubTickPen(QPen(Qt::white, 1));
    yAxis->setSubTickPen(QPen(Qt::white, 1));
    xAxis->setTickLabelColor(Qt::white);
    yAxis->setTickLabelColor(Qt::white);
    xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    xAxis->grid()->setSubGridVisible(true);
    yAxis->grid()->setSubGridVisible(true);
    xAxis->grid()->setZeroLinePen(Qt::NoPen);
    yAxis->grid()->setZeroLinePen(Qt::NoPen);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    axisRect()->setBackground(axisRectGradient);
}

void QEqualizerView::setDragInfo(int graph, int distance)
{
    dragable_graph_number = graph;
    max_distance_to_add_point = distance;
}

void QEqualizerView::mousePressEvent(QMouseEvent *event)
{
    double x,y;

    double best_dist = 1E+300;
    int best_index = 0;
    if( (dragable_graph_number >= 0) && ( dragable_graph_number < this->graphCount()) )
    {
        QCPGraph* pq_graph = this->graph(dragable_graph_number);
        pq_graph->pixelsToCoords(event->localPos(),x,y);

        //get nearest band and control it
        double nearest = -1;
        double nearest_diff = DBL_MAX;
        for(int i=0;i<NUM_BANDS;i++){
            double diff = std::abs(x-bands[i]);
            if(diff < nearest_diff){
                nearest_diff = diff;
                nearest = bands[i];
            }
        }
        x = nearest;

        if( pq_graph->data()->size() >= 1 )
        {
            for( int n=0; n<(pq_graph->data()->size()); n++ )
            {
                double dist = fabs( (pq_graph->data()->begin()+n)->value - y );
                dist += fabs( (pq_graph->data()->begin()+n)->key - x );
                if( dist < best_dist )
                {
                    best_dist = dist;
                    best_index = n;
                }
            }
            /*if( max_distance_to_add_point > 0 )
            {
                QPointF q_pos_gui = pq_graph->coordsToPixels( (pq_graph->data()->begin()+best_index)->key, (pq_graph->data()->begin()+best_index)->value );
                int dist_px = fabs( event->localPos().x() - q_pos_gui.x()) +  fabs( event->localPos().y() - q_pos_gui.y());
                if( dist_px/2 > max_distance_to_add_point )
                {
                    pq_graph->addData(x,y);
                    pq_graph->data().data()->sort();
                    for( int n=0; n<(pq_graph->data()->size()); n++ )
                    {
                        if(  (pq_graph->data()->begin()+n)->value == y && (pq_graph->data()->begin()+n)->key == x ) best_index = n;
                    }
                }
            }*/
            drag_number = best_index;
        }
    }
    QCustomPlot::mousePressEvent(event);
}

void QEqualizerView::mouseReleaseEvent(QMouseEvent *event)
{
    drag_number = -1;
    if( (dragable_graph_number >= 0) && ( dragable_graph_number < this->graphCount()) )
    {
        this->graph(dragable_graph_number)->data().data()->sort();
    }
    this->replot();
    emit( EditingFinished() );
    QCustomPlot::mouseReleaseEvent(event);
}
void QEqualizerView::mouseMoveEvent(QMouseEvent *event)
{
    double x,y;

    if( (dragable_graph_number >= 0) && ( dragable_graph_number < this->graphCount()) )
    {
        QCPGraph* pq_graph = this->graph(dragable_graph_number);
        pq_graph->pixelsToCoords(event->localPos(),x,y);

        //get nearest band and control it
        double nearest = -1;
        double nearest_diff = DBL_MAX;
        for(int i=0;i<NUM_BANDS;i++){
            double diff = std::abs(x-bands[i]);
            if(diff < nearest_diff){
                nearest_diff = diff;
                nearest = bands[i];
            }
        }
        x = nearest;

        y = round( y*16 ) / 16; //snap to grid
        if(y>12.0)y=12.0;
        else if(y<-12.0)y=-12.0;
        //x = round( x*4 ) / 4; //snap to grid

        if( drag_number >= 0 )
        {
            (pq_graph->data()->begin()+drag_number)->value = y;
            //(pq_graph->data()->begin()+drag_number)->key = x;
            emit( DataChanged() );

            this->replot();
        }
    }

}
