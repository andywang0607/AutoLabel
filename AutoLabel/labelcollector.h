#ifndef LABELCOLLECTOR_H
#define LABELCOLLECTOR_H

// Qt related header
#include <QObject>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPolygon>
#include <QPen>

// std related header
#include <vector>

//OpenCV related header
#include <opencv2/core/core.hpp>

struct LabelData{
public:
    LabelData(const cv::Rect &m_rect):
        labelClass(0), rect(m_rect), penIdx(0), isSelect(false){}
    LabelData(const LabelData &e)
    {
        labelClass = e.labelClass;
        penIdx = e.penIdx;
        rect = e.rect;
        isSelect = e.isSelect;
        contoursPoly.assign(e.contoursPoly.begin(),e.contoursPoly.end());
        result.assign(e.result.begin(),e.result.end());
        resultPoly = QPolygon(e.resultPoly);
    }
    ~LabelData()
    {
    }
    int labelClass;
    int penIdx; //0: normal, 1:highlight
    bool isSelect;
    cv::Rect rect;
    std::vector<cv::Point> contoursPoly;
    std::vector<QPoint> result; // record scaled point show in GUI
    QPolygon resultPoly;
};

class LabelCollector : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString imgSrc READ imgSrc WRITE setImgSrc NOTIFY imgSrcChanged)

public:
    explicit LabelCollector(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
public:
    Q_INVOKABLE void RemoveLabel(int idx);
public:
    void SetContours(int labelIdx, std::vector<cv::Point> &contoursPoly);
private:
    bool GetExistLabel(QPointF pt);
    void RemoveAllLabel();
    double DistanceBetween2Point(QPointF p1, QPointF p2);
public:
    QImage image() const;
    QString imgSrc() const;
    QVector<LabelData*> dataVec() const;
    bool setItemAt(int index, LabelData *item);
    qreal getFactorScaled() const;
private:
    QImage m_image;
    QImage m_imageScaled;
    qreal factorScaled;
    int imageWidth;
    int imageHeight;
    QString m_imgSrc;

public slots:
    void setImage(const QImage &image);
    void setImgSrc(QString imgSrc);
public slots:
    void appendData(cv::Rect rect);

signals:

    void imageChanged();
    void imgSrcChanged(QString imgSrc);

    // Model related signal
signals:
  void preItemAppended();
  void postItemAppended();

  void preItemRemoved(int index);
  void postItemRemoved();

  void onModelChanged();

  void processRequest(int rectIdx);
    //mouse behavior
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    bool m_mouseEnabled;
    bool m_mousePressed;
    bool m_mouseMoved;
    bool m_isLabelSelect;

    QPointF m_lastPoint;
    QPointF m_currentPoint;
    QPointF m_firstPoint;

    QPen m_penNormal;
    QPen m_penHighlight;
    QPen m_penPoint;
    QPen m_penPoly;
    QVector<QPen> m_penVec;
    std::vector<int> m_selectLabelIdx;

private:
    QVector<LabelData*> m_dataVec;
};

#endif // LABELCOLLECTOR_H